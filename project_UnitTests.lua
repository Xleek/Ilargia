
-- Unit Tests
-------------------------------------------

project "UnitTests"
	local ProjectRoot = os.getcwd()

	language "C++"
	targetname "UnitTests"
	targetdir (SolutionRoot.."/bin")
	kind "ConsoleApp"

	if os.is("windows") then
		-- Because on Windows, you can't start a program if .dll are not in the same folder...
		postbuildcommands { string.gsub("copy "..SolutionRoot.."/bin/lib/*.dll "..SolutionRoot.."/bin/", "/", "\\") }
	end

	files	{
		ProjectRoot.."/unittests/main.cpp",
	}

	filter "Debug*"
		links	{ "Ilargia-d", "Muon-d" }

	filter "Release*"
		links { "Ilargia", "Muon" }