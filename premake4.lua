--
-- build configuration script
-- 
solution "OpenWolf"
	--configurations { "Release", "ReleaseWithSymbols", "Debug" }
	configurations { "Release", "Debug" }
	platforms {"x32", "x64", "native"}
	
	--
	-- Release/Debug Configurations
	--
	configuration "Release"
		defines     "NDEBUG"
		flags      
		{
			"OptimizeSpeed",
			"EnableSSE",
			--"StaticRuntime"
		}
		
	--configuration "ReleaseReleaseWithSymbols"
	--	defines     "NDEBUG"
	--	flags
	--	{
	--		"OptimizeSpeed",
	--		"EnableSSE",
	--		"Symbols",
	--		"StaticRuntime"
	--	}
	
	configuration "Debug"
		defines     "_DEBUG"
		flags
		{
			"Symbols",
			--"StaticRuntime",
			--"NoRuntimeChecks"
		}
	
--
-- Options
--

--newoption
--{
--	trigger = "with-freetype",
--	description = "Compile with freetype support"
--}
newoption
{
	trigger = "with-bullet",
	description = "Compile with Bullet physics game code support"
}		
--newoption
--{
--	trigger = "with-openal",
--	value = "TYPE",
--	description = "Specify which OpenAL library",
--	allowed = 
--	{
--		{ "none", "No support for OpenAL" },
--		{ "dlopen", "Dynamically load OpenAL library if available" },
--		{ "link", "Link the OpenAL library as normal" },
--		{ "openal-dlopen", "Dynamically load OpenAL library if available" },
--		{ "openal-link", "Link the OpenAL library as normal" }
--	}
--}

--		
-- Platform specific defaults
--

-- We don't support freetype on VS platform
--if _ACTION and string.sub(_ACTION, 2) == "vs" then
--	_OPTIONS["with-freetype"] = false
--end

-- Default to dlopen version of OpenAL
--if not _OPTIONS["with-openal"] then
--	_OPTIONS["with-openal"] = "dlopen"
--end
--if _OPTIONS["with-openal"] then
--	_OPTIONS["with-openal"] = "openal-" .. _OPTIONS["with-openal"]
--end

include "code/engine"
include "code/engine/game"
include "code/engine/cgame"
include "code/engine/gui"

include "code/tools/OWMap"
include "code/tools/master"




