

project "base_cgame"
	targetname  "cgame"
	targetdir 	"../../../main"
	language    "C++"
	kind        "SharedLib"
	flags       { "ExtraWarnings" }
	files
	{
		"../qcommon/q_shared.h",
		"cg_public.h",
		"cg_local.h",
		"../GPURenderer/r_types.h",
		"../qcommon/keycodes.h",
		"../qcommon/surfaceflags.h",
		
		--"**.c", "**.cpp", "**.h",
		
		"cg_animation.cpp",
		"cg_consolecmds.cpp",
		"cg_cover.cpp",
		"cg_debug.cpp",
		"cg_draw.cpp",
		"cg_drawtools.cpp",
		"cg_effects.cpp",
		"cg_ents.cpp",
		"cg_event.cpp",
		"cg_info.cpp",
		"cg_localents.cpp",
		"cg_lua.cpp",
		"cg_main.cpp",
		"cg_marks.cpp",
		"cg_nav.cpp",
		"cg_osd.cpp",
		"cg_particles.cpp",
		"cg_path.cpp",
		"cg_players.cpp",
		"cg_playerstate.cpp",
		"cg_predict.cpp",
		"cg_scoreboard.cpp",
		"cg_servercmds.cpp",
		"cg_snapshot.cpp",
		"cg_syscalls.cpp",
		"cg_view.cpp",
		"cg_weapons.cpp",
		"cg_waypoint.cpp",
		
		"../qcommon/q_math.cpp",
		"../qcommon/q_shared.cpp",
		
		"lua_cgame.cpp",
		"lua_particle.cpp",
		
		"../bgame/bg_**.cpp", "../bgame/bg_**.h",
		
		"../game/lua_qmath.cpp",
		"../game/lua_vector.cpp",
		
		"../../../code/libs/lua/src/lapi.c",
		"../../../code/libs/lua/src/lcode.c",
		"../../../code/libs/lua/src/ldebug.c",
		"../../../code/libs/lua/src/ldo.c",
		"../../../code/libs/lua/src/ldump.c",
		"../../../code/libs/lua/src/lfunc.c",
		"../../../code/libs/lua/src/lgc.c",
		"../../../code/libs/lua/src/llex.c",
		"../../../code/libs/lua/src/lmem.c",
		"../../../code/libs/lua/src/lobject.c",
		"../../../code/libs/lua/src/lopcodes.c",
		"../../../code/libs/lua/src/lparser.c",
		"../../../code/libs/lua/src/lstate.c",
		"../../../code/libs/lua/src/lstring.c",
		"../../../code/libs/lua/src/ltable.c",
		"../../../code/libs/lua/src/ltm.c",
		"../../../code/libs/lua/src/lundump.c",
		"../../../code/libs/lua/src/lvm.c",
		"../../../code/libs/lua/src/lzio.c",
		"../../../code/libs/lua/src/lauxlib.c",
		"../../../code/libs/lua/src/lbaselib.c",
		"../../../code/libs/lua/src/ldblib.c",
		"../../../code/libs/lua/src/liolib.c",
		"../../../code/libs/lua/src/lmathlib.c",
		"../../../code/libs/lua/src/ltablib.c",
		"../../../code/libs/lua/src/lstrlib.c",
		"../../../code/libs/lua/src/loadlib.c",
		"../../../code/libs/lua/src/linit.c",
		"../../../code/libs/lua/src/loslib.c",


		"../../../code/libs/recast/**.cpp",
		"../../../code/libs/recast/**.h",

		"../../../code/libs/detour/**.cpp",
		"../../../code/libs/detour/**.h",
	}
	--excludes
	--{
	--	"cg_unlagged.cpp",
	--	"cg_newdraw.cpp",
	--	"g_bullet.cpp",
	--}
	includedirs
	{
		"../",
		"../../../code/libs/lua/src",
		"../../../code/libs/recast",
		"../../../code/libs/detour",
	}
	defines
	{ 
		"LUA"
	}

	--configuration "with-bullet"
	--	files
	--	{
	--		"cg_bullet.cpp",
	--	
	--		"../../../code/libs/bullet/*.h",
	--		"../../../code/libs/bullet/LinearMath/**.cpp", "../../../code/libs/bullet/LinearMath/**.h",
	--		"../../../code/libs/bullet/BulletCollision/**.cpp", "../../../code/libs/bullet/BulletCollision/**.h",
	--		"../../../code/libs/bullet/BulletDynamics/**.cpp", "../../../code/libs/bullet/BulletDynamics/**.h",
	--		"../../../code/libs/bullet/BulletSoftBody/**.cpp", "../../../code/libs/bullet/BulletSoftBody/**.h",
	--	}
	--	includedirs
	--	{
	--		"../../../code/libs/bullet"
	--	}
	--	defines
	--	{ 
	--		"USE_BULLET"
	--	}
	
	--
	-- Platform Configurations
	--
	configuration "x32"
		targetname  "cgamex86"
	
	configuration "x64"
		targetname  "cgamex86_64"
				
	-- 
	-- Project Configurations
	-- 
	configuration "vs*"
		linkoptions
		{
			"/DEF:cgame.def",
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
			"CGAMEDLL"
		}	
	
	configuration { "vs*", "x64" }
		defines
		{
			"_WIN32",
			"_WIN64",
			"__WIN64__",
			"__AMD64__",
			"_CRT_SECURE_NO_WARNINGS",
			"CGAMEDLL"
		}
	
	configuration { "linux", "x32" }
		targetname  "cgamei386"
		targetprefix ""
	
	configuration { "linux", "x64" }
		targetname  "cgamex86_64"
		targetprefix ""
	
	
