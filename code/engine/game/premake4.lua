project "base_game"
	targetname  "game"
	targetdir 	"../../../main"
	language    "C++"
	kind        "SharedLib"
	flags       { "ExtraWarnings" }
	files
	{
		"../qcommon/q_shared.h",
		"../qcommon/q_platform.h",
		"../qcommon/q_shared.cpp",
		"../qcommon/q_math.cpp",
		"g_public.h",
		"g_local.h",
		"../qcommon/surfaceflags.h",
		
		--"**.c", "**.cpp", "**.h",
		"*.h",
		"../botai/ai.h",
		"../botai/acebot.h",
		"../botai/acebot_ai.cpp",
		"../botai/acebot_cmds.cpp",
		"../botai/acebot_items.cpp",
		"../botai/acebot_movement.cpp",
		"../botai/acebot_nodes.cpp",
		"../botai/acebot_spawn.cpp",
		"../bgame/bg_local.h",
		"../bgame/bg_public.h",
		"../bgame/bg_misc.cpp",
		"../bgame/bg_pmove.cpp",
		"../bgame/bg_slidemove.cpp",
		"g_active.cpp",
		"g_arenas.cpp",
		"g_ai.cpp",
		"g_ai_misc.cpp",
		"g_ai_move.cpp",
		"g_ai_pmove.cpp",
		"g_ai_nodes.cpp",
		"g_ai_think.cpp",
		"g_bot.cpp",
		"g_client.cpp",
		"g_cmds.cpp",
		"g_combat.cpp",
		"g_explosive.cpp",
		"g_items.cpp",
		"g_lua.cpp",
		"g_main.cpp",
		"g_mem.cpp",
		"g_misc.cpp",
		"g_missile.cpp",
		"g_mover.cpp",
		"g_session.cpp",
		"g_spawn.cpp",
		"g_svcmds.cpp",
		"g_syscalls.cpp",
		"g_target.cpp",
		"g_team.cpp",
		"g_trigger.cpp",
		"g_utils.cpp",
		"g_weapon.cpp",
		"lua_*.cpp",
		
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
	}
	--excludes
	--{
	--	"g_unlagged.c",
	--	"g_rankings.c",
	--	"g_bullet.cpp",
	--}
	includedirs
	{
		"../",
		"../../../code/shared",
		"../../../code/libs/lua/src",
	}
	defines
	{ 
		"QAGAME",
		"LUA"
	}
	
	configuration "with-bullet"
		files
		{
			"g_bullet.cpp",
		
			"../../../code/libs/bullet/*.h",
			"../../../code/libs/bullet/LinearMath/**.cpp", "../../../code/libs/bullet/LinearMath/**.h",
			"../../../code/libs/bullet/BulletCollision/**.cpp", "../../../code/libs/bullet/BulletCollision/**.h",
			"../../../code/libs/bullet/BulletDynamics/**.cpp", "../../../code/libs/bullet/BulletDynamics/**.h",
			"../../../code/libs/bullet/BulletSoftBody/**.cpp", "../../../code/libs/bullet/BulletSoftBody/**.h",
		}
		includedirs
		{
			"../../../code/libs/bullet"
		}
		defines
		{ 
			"USE_BULLET"
		}
	
	--
	-- Platform Configurations
	--
	configuration "x32"
		targetname  "qagamex86"
	
	configuration "x64"
		targetname  "qagamex86_64"
				
	-- 
	-- Project Configurations
	-- 
	configuration "vs*"
		linkoptions
		{
			"/DEF:game.def",
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
		targetname  "qagamei386"
		targetprefix ""
	
	configuration { "linux", "x64" }
		targetname  "qagamex86_64"
		targetprefix ""

	
