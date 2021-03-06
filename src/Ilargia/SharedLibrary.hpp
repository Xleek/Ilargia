/*************************************************************************
* Ilargia Engine - http://github.com/Xipiryon/Ilargia
* C++ Modular Data Oriented Game Enginee
*------------------------------------------------------------------------
* Copyright (c) 2014-2015, Louis Schnellbach
*
* This software is provided 'as-is', without any express or implied
* warranty. In no event will the authors be held liable for any damages
* arising from the use of this software.
*
* Permission is granted to anyone to use this software for any purpose,
* including commercial applications, and to alter it and redistribute it
* freely, subject to the following restrictions:
*
* 1. The origin of this software must not be misrepresented; you must not
*    claim that you wrote the original software. If you use this software
*    in a product, an acknowledgment in the product documentation would
*    be appreciated but is not required.
*
* 2. Altered source versions must be plainly marked as such, and must not
*    be misrepresented as being the original software.
*
* 3. This notice may not be removed or altered from any source
*    distribution.
*
*************************************************************************/

#ifndef _DNA_SHAREDLIBRARY_H_INCLUDED
#define _DNA_SHAREDLIBRARY_H_INCLUDED

#include <vector>

#include <Muon/Helper/Singleton.hpp>
#include <Muon/System/Log.hpp>
#include "Ilargia/Core/Define.hpp"

namespace ilg
{
	namespace manager
	{
		class ManagerFactory;
		class IBaseManager;
	}

	struct SharedLibraryInfo
	{
		typedef int(*FuncLoad)(int, char**);
		typedef void(*FuncUnload)();

		void* libInstance;
		m::String name;
		m::String funcLoadName;
		m::String funcUnloadName;
		FuncLoad funcLoadPtr;
		FuncUnload funcUnloadPtr;
	};

	struct CManagerPair
	{
		manager::IBaseManager *manager;
		void* library;
	};

	class SharedLibrary : public m::helper::NonCopyable
	{
	public:
		MUON_SINGLETON_GET(SharedLibrary);

		const std::vector<CManagerPair>& getManagers() const;
		const std::vector<SharedLibraryInfo>& getLibraries() const;

		bool loadLibrary(const m::String& name, const m::String& filename);

		void forwardArg(int, char**);
		void unloadLibraries();

	private:
		int m_argc;
		char** m_argv;

		friend class manager::ManagerFactory;
		friend class Engine;

		void _addModuleRef(manager::IBaseManager*);
		bool _loadLibrary(SharedLibraryInfo&, const char*);
		bool _loadLibrary_LoadFunction(SharedLibraryInfo&, const char*);
		bool _loadLibrary_UnloadFunction(SharedLibraryInfo&, const char*);
		void _closeLibrary(SharedLibraryInfo&);

		SharedLibrary();
		~SharedLibrary();

		m::system::Log m_log;
		std::vector<SharedLibraryInfo> m_libraries;
		std::vector<CManagerPair> m_managers;
	};
}

#endif
