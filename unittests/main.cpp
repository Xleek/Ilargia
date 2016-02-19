
#include <fstream>
#include <Ilargia/Engine.hpp>

#include "tinyxml2.h"

int main(int argc, char** argv)
{
	if(!muon::meta::MetaDatabase::isInstantiated())
	{
		muon::meta::MetaDatabase::createInstance();
	}
	muon::system::Log mainLog("Main", muon::LOG_INFO);

	mainLog() << "Number of arguments: " << argc << muon::endl;
	for (muon::i32 i = 0; i < argc; ++i)
	{
		mainLog() << "\t: " << argv[i] << muon::endl;
	}

	// Required variables
	muon::system::Time clockTest;
	muon::String title;
	muon::u32 errorCount = 0;
	muon::u32 totalTests = 0;
	tinyxml2::XMLDocument xmlDoc;
	tinyxml2::XMLElement* xmlRoot = xmlDoc.NewElement("testsuite");

#define ILARGIA_TITLE(msg) do { mainLog() << msg << muon::endl; title = msg; } while(false);
#define ILARGIA_NODE_BEGIN(cond)	++totalTests; tinyxml2::XMLElement* xmlNode = xmlDoc.NewElement("testcase"); \
									xmlNode->SetAttribute("name", #cond); \
									xmlNode->SetAttribute("classname", title.cStr()); \
									xmlNode->SetAttribute("time", clockTest.now()*1000);
#define ILARGIA_NODE_ERR(err)		++errorCount; tinyxml2::XMLElement* xmlErr = xmlDoc.NewElement("failure"); xmlErr->SetText(err); xmlNode->InsertEndChild(xmlErr);
#define ILARGIA_NODE_END			xmlRoot->InsertEndChild(xmlNode); clockTest.start();
#if defined(MUON_PLATFORM_WINDOWS)
#	define ILARGIA_CHECK(cond, err, ...)  do { ILARGIA_NODE_BEGIN(cond) if(!(cond)) { ILARGIA_NODE_ERR(err); MUON_ERROR("\t-> " err, __VA_ARGS__);} ILARGIA_NODE_END } while(false);
#else
#	define ILARGIA_CHECK(cond, err, args...) do { ILARGIA_NODE_BEGIN(cond) if(!(cond)) { ILARGIA_NODE_ERR(err); MUON_ERROR("\t-> " err, ##args);} ILARGIA_NODE_END } while(false);
#endif

	// ***************
	// BEGIN UNIT TEST
	clockTest.start();

	{
		//ILARGIA_TITLE("Checking 'Expression' script");
		//ILARGIA_CHECK(context->load(file.cStr()), "Couldn't load file \"%s\"!", file.cStr());

	}

	// END UNIT TEST
	// ***************
	mainLog(errorCount == 0 ? muon::LOG_INFO : muon::LOG_ERROR) << "Error Count: " << errorCount << muon::endl;

	xmlRoot->SetAttribute("tests", totalTests);
	xmlDoc.InsertFirstChild(xmlRoot);
	xmlDoc.SaveFile("unittests.xml");

	muon::system::Log::close();

	return -((muon::i32)errorCount);
}
