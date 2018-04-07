project "base_gui"
	targetname  "gui"
	targetdir 	"../../../main"
	language    "C++"
	kind        "SharedLib"
	flags       { "ExtraWarnings" }
	files
	{
		"../qcommon/q_shared.h",
		"../qcommon/q_platform.h",
		"gui_public.h",
		"../GPURenderer/r_types.h",
		"../qcommon/keycodes.h",
		"../qcommon/surfaceflags.h",
		"../qcommon/q_math.cpp",
		"../qcommon/q_shared.cpp",
		
		"**.c", "**.cpp", "**.h",
	}
	excludes
	{
		"gui_login.cpp",
		"gui_rankings.cpp",
		"gui_rankstatus.cpp",
		"gui_signup.cpp",
		"gui_specifyleague.cpp",
		"gui_spreset.cpp",
	}
	includedirs
	{
		"../"
	}
	defines
	{ 
		--"CGAMEDLL",
	}
	
	--
	-- Platform Configurations
	--
	configuration "x32"
		targetname  "guix86"
	
	configuration "x64"
		targetname  "guix86_64"
				
	-- 
	-- Project Configurations
	-- 
	configuration "vs*"
		linkoptions
		{
			"/DEF:gui.def",
		}
		defines
		{
			"WIN32",
			"_CRT_SECURE_NO_WARNINGS",
		}
		
	--
	-- Dushan
	--
	configuration { "vs*", "x32" }
		defines
		{
			"_WIN32",
			"_CRT_SECURE_NO_WARNINGS",
		}	
	
	configuration { "vs*", "x64" }
		defines
		{
			"_WIN32",
			"_WIN64",
			"__WIN64__",
			"__AMD64__",
			"_CRT_SECURE_NO_WARNINGS",
		}
			
	configuration { "linux", "x32" }
		targetname  "guii386"
		targetprefix ""
	
	configuration { "linux", "x64" }
		targetname  "guix86_64"
		targetprefix ""
	