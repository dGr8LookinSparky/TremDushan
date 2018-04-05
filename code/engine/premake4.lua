project "OpenWolf"
	targetname  "OpenWolf"
	language    "C++"
	kind        "WindowedApp"
	flags       { "ExtraWarnings", "NoRTTI" }
	files
	{		
		"audio/**.cpp", "audio/**.h",
		"client/**.cpp", "client/**.h",
		"server/**.cpp", "server/**.h",
		
		"qcommon/**.h", 
		"qcommon/cmd.cpp",
		"qcommon/common.cpp",
		"qcommon/cvar.cpp",
		"qcommon/files.cpp",
		"qcommon/huffman.cpp",
		"qcommon/md4.cpp",
		"qcommon/md5.cpp",
		"qcommon/msg.cpp",
		"qcommon/vm.cpp",
		"qcommon/net_*.cpp",
		"qcommon/unzip.cpp",
		"qcommon/unzip.h",
		"qcommon/parse.cpp",
		"qcommon/q_shared.cpp",
		"qcommon/q_math.cpp",
		"qcommon/ioapi.cpp",
		"qcommon/ioapi.h",
		
		"cm/cm_load.cpp",
		"cm/cm_local.h",
		"cm/cm_patch.cpp",
		"cm/cm_polylib.cpp",
		"cm/cm_polylib.h",
		"cm/cm_public.h",
		"cm/cm_test.cpp",
		"cm/cm_trace.cpp",
		"cm/cm_trisoup.cpp",
		
		"GPURenderer/**.cpp", "GPURenderer/**.h",
		
		"../libs/glew/src/glew.c",
		"../libs/glew/src/glew.h",
		
		"../libs/jpeg/**.c", "../libs/jpeg/**.h",
		"../libs/png/**.c", "../libs/png/**.h",
		"../libs/zlib/adler32.c", 
		"../libs/zlib/compress.c",
		"../libs/zlib/crc32.c",
		"../libs/zlib/deflate.c",
		"../libs/zlib/gzio.c",
		"../libs/zlib/infback.c",
		"../libs/zlib/inffast.c",
		"../libs/zlib/inflate.c",
		"../libs/zlib/inftrees.c",
		"../libs/zlib/trees.c",
		"../libs/zlib/uncompr.c",
		"../libs/zlib/zutil.c",
		
		"../libs/zlib/crc32.h",
		"../libs/zlib/deflate.h",
		"../libs/zlib/inffast.h",
		"../libs/zlib/inffixed.h",
		"../libs/zlib/inflate.h",
		"../libs/zlib/inftrees.h",
		"../libs/zlib/trees.h",
		"../libs/zlib/zconf.h",
		"../libs/zlib/zlib.h",
		"../libs/zlib/zutil.h",
		
		"../libs/openexr/include**.cpp", "../libs/openexr/include**.h",
		
		--"../libs/ft2/**.c", "../libs/ft2/**.h",
		
		"../libs/freetype/src/autofit/autofit.c",
		"../libs/freetype/src/bdf/bdf.c",
		"../libs/freetype/src/cff/cff.c",
		"../libs/freetype/src/base/ftbase.c",
		"../libs/freetype/src/base/ftbitmap.c",
		"../libs/freetype/src/cache/ftcache.c",
		"../libs/freetype/src/base/ftdebug.c",
		"../libs/freetype/src/base/ftgasp.c",
		"../libs/freetype/src/base/ftglyph.c",
		"../libs/freetype/src/gzip/ftgzip.c",
		"../libs/freetype/src/base/ftinit.c",
		"../libs/freetype/src/lzw/ftlzw.c",
		"../libs/freetype/src/base/ftstroke.c",
		"../libs/freetype/src/base/ftsystem.c",
		"../libs/freetype/src/smooth/smooth.c",
		"../libs/freetype/src/base/ftbbox.c",
		"../libs/freetype/src/base/ftmm.c",
		"../libs/freetype/src/base/ftpfr.c",
		"../libs/freetype/src/base/ftsynth.c",
		"../libs/freetype/src/base/fttype1.c",
		"../libs/freetype/src/base/ftwinfnt.c",
		"../libs/freetype/src/pcf/pcf.c",
		"../libs/freetype/src/pfr/pfr.c",
		"../libs/freetype/src/psaux/psaux.c",
		"../libs/freetype/src/pshinter/pshinter.c",
		"../libs/freetype/src/psnames/psmodule.c",
		"../libs/freetype/src/raster/raster.c",
		"../libs/freetype/src/sfnt/sfnt.c",
		"../libs/freetype/src/truetype/truetype.c",
		"../libs/freetype/src/type1/type1.c",
		"../libs/freetype/src/cid/type1cid.c",
		"../libs/freetype/src/type42/type42.c",
		"../libs/freetype/src/winfonts/winfnt.c",
		
		"../libs/ogg/src/bitwise.c",
		"../libs/ogg/src/framing.c",
		
		"../libs/vorbis/lib/mdct.c",
		"../libs/vorbis/lib/smallft.c",
		"../libs/vorbis/lib/block.c",
		"../libs/vorbis/lib/envelope.c",
		"../libs/vorbis/lib/window.c",
		"../libs/vorbis/lib/lsp.c",
		"../libs/vorbis/lib/lpc.c",
		"../libs/vorbis/lib/analysis.c",
		"../libs/vorbis/lib/synthesis.c",
		"../libs/vorbis/lib/psy.c",
		"../libs/vorbis/lib/info.c",
		"../libs/vorbis/lib/floor1.c",
		"../libs/vorbis/lib/floor0.c",
		"../libs/vorbis/lib/res0.c",
		"../libs/vorbis/lib/mapping0.c",
		"../libs/vorbis/lib/registry.c",
		"../libs/vorbis/lib/codebook.c",
		"../libs/vorbis/lib/sharedbook.c",
		"../libs/vorbis/lib/lookup.c",
		"../libs/vorbis/lib/bitrate.c",
		"../libs/vorbis/lib/vorbisfile.c",
		
		-- "../libs/speex/bits.c",
		-- "../libs/speex/buffer.c",
		-- "../libs/speex/cb_search.c",
		-- "../libs/speex/exc_10_16_table.c",
		-- "../libs/speex/exc_10_32_table.c",
		-- "../libs/speex/exc_20_32_table.c",
		-- "../libs/speex/exc_5_256_table.c",
		-- "../libs/speex/exc_5_64_table.c",
		-- "../libs/speex/exc_8_128_table.c",
		-- "../libs/speex/fftwrap.c",
		-- "../libs/speex/filterbank.c",
		-- "../libs/speex/filters.c",
		-- "../libs/speex/gain_table.c",
		-- "../libs/speex/gain_table_lbr.c",
		-- "../libs/speex/hexc_10_32_table.c",
		-- "../libs/speex/hexc_table.c",
		-- "../libs/speex/high_lsp_tables.c",
		-- "../libs/speex/jitter.c",
		-- "../libs/speex/kiss_fft.c",
		-- "../libs/speex/kiss_fftr.c",
		-- "../libs/speex/lsp_tables_nb.c",
		-- "../libs/speex/ltp.c",
		-- "../libs/speex/mdf.c",
		-- "../libs/speex/modes.c",
		-- "../libs/speex/modes_wb.c",
		-- "../libs/speex/nb_celp.c",
		-- "../libs/speex/preprocess.c",
		-- "../libs/speex/quant_lsp.c",
		-- "../libs/speex/resample.c",
		-- "../libs/speex/sb_celp.c",
		-- "../libs/speex/speex_smallft.c",
		-- "../libs/speex/speex.c",
		-- "../libs/speex/speex_callbacks.c",
		-- "../libs/speex/speex_header.c",
		-- "../libs/speex/speex_lpc.c",
		-- "../libs/speex/speex_lsp.c",
		-- "../libs/speex/speex_window.c",
		-- "../libs/speex/vbr.c",
		-- "../libs/speex/stereo.c",
		-- "../libs/speex/vq.c",
		
		"../libs/theora/lib/dec/apiwrapper.c",
		"../libs/theora/lib/dec/bitpack.c",
		"../libs/theora/lib/dec/decapiwrapper.c",
		"../libs/theora/lib/dec/decinfo.c",
		"../libs/theora/lib/dec/decode.c",
		"../libs/theora/lib/dec/dequant.c",
		"../libs/theora/lib/dec/fragment.c",
		"../libs/theora/lib/dec/huffdec.c",
		"../libs/theora/lib/dec/idct.c",
		"../libs/theora/lib/dec/thinfo.c",
		"../libs/theora/lib/dec/internal.c",
		"../libs/theora/lib/dec/quant.c",
		"../libs/theora/lib/dec/state.c",
	}
	includedirs
	{
		"../libs/png",
		"../libs/jpeg",
		"../libs/zlib",
		"../libs/openexr/include",
		"../libs/glew/include",
		"../libs/freetype/include",
		"../libs/ogg/include",
		"../libs/vorbis/include",
		"../libs/theora/include",
		"../libs/speex/include",
	}
	defines
	{ 
		"STANDALONE",
		"REF_HARD_LINKED",
		"GLEW_STATIC",
		"BUILD_FREETYPE",
		"FT2_BUILD_LIBRARY",
		"USE_CODEC_VORBIS",
		--"USE_VOIP",
		"USE_CIN_THEORA",
		"USE_ALLOCA",
		"FLOATING_POINT",
		--"USE_CURL", 
		--"USE_MUMBLE",
		--"USE_INTERNAL_GLFW",
		--"USE_INTERNAL_GLEW",
		"AL_NO_PROTOTYPES",
		"ALC_NO_PROTOTYPES",
	}
	excludes
	{
		"GPURenderer/tr_animation_mdm.cpp",
		"GPURenderer/tr_model_mdm.cpp",
	}
	
	--
	-- Platform Configurations
	-- 	
	configuration "x32"
		targetdir 	"../../bin32"
		files
		{ 
			--"code/qcommon/vm_x86.cpp",
			"../libs/theora/lib/dec/x86/mmxidct.c",
			"../libs/theora/lib/dec/x86/mmxfrag.c",
			"../libs/theora/lib/dec/x86/mmxstate.c",
			"../libs/theora/lib/dec/x86/x86state.c"
		}
	
	configuration "x64"
		targetdir 	"../../bin64"
		files
		{ 
			--"qcommon/vm_x86_64.c",
			--"qcommon/vm_x86_64_assembler.c",
			"../libs/theora/lib/dec/x86/mmxidct.c",
			"../libs/theora/lib/dec/x86/mmxfrag.c",
			"../libs/theora/lib/dec/x86/mmxstate.c",
			"../libs/theora/lib/dec/x86/x86state.c"
		}
		
	--
	-- Options Configurations
	--	
		
	configuration "with-bullet"
		defines
		{
			"USE_BULLET"
		}
		includedirs
		{
			"../libs/bullet"
		}
		files
		{
			"cm/cm_bullet.cpp",
		
			"../libs/bullet/*.h",
			"../libs/bullet/LinearMath/**.cpp", "../libs/bullet/LinearMath/**.h",
			"../libs/bullet/BulletCollision/**.cpp", "../libs/bullet/BulletCollision/**.h",
			"../libs/bullet/BulletDynamics/**.cpp", "../libs/bullet/BulletDynamics/**.h",
			"../libs/bullet/BulletSoftBody/**.cpp", "../libs/bullet/BulletSoftBody/**.h",
		}
		
	--configuration "with-freetype"
	--	links        { "freetype" }
	--	buildoptions { "`pkg-config --cflags freetype2`" }
	--	defines      { "BUILD_FREETYPE" }

	--configuration "openal-dlopen"
	--	defines      
	--	{
	--		"USE_OPENAL",
	--		"USE_OPENAL_DLOPEN",
	--		"USE_OPENAL_LOCAL_HEADERS"
	--	}
		
	--configuration "openal-link"
	--	links        { "openal " }
	--	defines      { "USE_OPENAL" }

	
	--configuration { "vs*", "Release" }
	-- newaction {
		-- trigger = "prebuild",
		-- description = "Compile libcurl.lib",
		-- execute = function ()
			-- os.execute("cd ../libs/curl-7.12.2;cd lib;nmake /f Makefile.vc6 CFG=release")
		-- end
	-- }
	
	-- 
	-- Project Configurations
	-- 
	configuration "vs*"
		flags       { "WinMain" }
		files
		{
			"sys/sys_main.cpp",
			"sys/sys_win32.cpp",
			"sys/con_log.cpp",
			"sys/con_win32.cpp",
			"sys/sdl_gamma.cpp",
			"sys/sdl_glimp.cpp",
			"sys/sdl_input.cpp",
			"sys/sdl_snd.cpp",
			"sys/sys_local.h",
			"sys/sdl_icon.h",
			"sys/win_resource.h",
			"sys/sys_loadlib.h",
			
			"sys/Legacy.ico",
			"sys/win_resource.rc",
			
			"../libs/glew/src/wglew.h",
			
			--"code/glfw/lib/win32/*.c", "code/glfw/lib/win32/*.h",
		}
		excludes
		{
			--"win32/win_gamma.c",
			--"win32/win_glimp.c",
			--"win32/win_eh.cpp",
		}
		defines
		{
			"USE_OPENAL",
			"USE_OPENAL_DLOPEN",
			"USE_OPENAL_LOCAL_HEADERS"
		}
		includedirs
		{
			"./",
			"../libs/sdl2/include",
			"../libs/openal/include",
			"../libs/zlibwapi/include",
		}
		libdirs
		{
			--"../libs/curl-7.12.2/lib"
		}
		
		links
		{
			"SDL2",
			"SDL2main",
			--"dinput8",
			"winmm",
			"wsock32",
			--"Iphlpapi",
			"opengl32",
			"user32",
			"advapi32",
			"ws2_32",
			"Psapi"
		}
		buildoptions
		{
			--"/MT"
		}
		linkoptions 
		{
			"/LARGEADDRESSAWARE",
			--"/NODEFAULTLIB:libcmt.lib",
			--"/NODEFAULTLIB:libcmtd.lib"
			--"/NODEFAULTLIB:libc"
		}
		defines
		{
			"WIN32",
			"_WIN32",
			"_CRT_SECURE_NO_WARNINGS",
		}
		
		
	configuration { "vs*", "x32" }
		files
		{
			"gldraw.c",
		}
		libdirs
		{
			"../libs/sdl2/lib/x86",
			"../libs/openal/libs/win32",
			"../libs/zlibwapi/lib/x32",
			--"../libs/curl-7.12.2/lib"
		}
		links
		{ 
			--"libcurl",
			"OpenAL32",
			"zlibwapi",
		}
		
	configuration { "vs*", "x64" }
		libdirs
		{
			"../libs/sdl2/lib/x64",
			"../libs/openal/libs/win64",
			--"../libs/curl-7.12.2/lib"
			"../libs/zlibwapi/lib/lib64",
		}
		links
		{ 
			--"libcurl",
			"OpenAL32",
			"zlibwapi",
		}
		defines
		{
			"_WIN32",
			"_WIN64",
			"__WIN64__",
			"__AMD64__",
			"_CRT_SECURE_NO_WARNINGS",
		}		
		
	configuration { "linux", "gmake" }
		buildoptions
		{
			--"`pkg-config --cflags x11`",
			--"`pkg-config --cflags xext`",
			--"`pkg-config --cflags xxf86dga`",
			--"`pkg-config --cflags xxf86vm`",
			"`pkg-config --cflags sdl2`",
			"`pkg-config --cflags libcurl`",
		}
		linkoptions
		{
			--"`pkg-config --libs x11`",
			--"`pkg-config --libs xext`",
			--"`pkg-config --libs xxf86dga`",
			--"`pkg-config --libs xxf86vm`",
			"`pkg-config --libs sdl2`",
			"`pkg-config --libs libcurl`",
		}
		links
		{ 
			--"libcurl",
			"openal",
		}
	
	configuration "linux"
		targetname  "openwolf"
		files
		{
			"sys/sys_main.cpp",
			"sys/sys_unix.cpp",
			"sys/con_log.cpp",
			"sys/con_passive.cpp",
			"sys/sdl_gamma.cpp",
			"sys/sdl_glimp.cpp",
			"sys/sdl_input.cpp",
			"sys/sdl_audio.cpp",
			"../libs/glew/src/glew.c",
		}
		--buildoptions
		--{
		--	"-pthread"
		--}
		links
		{
			"GL",
		}
		defines
		{
            "PNG_NO_ASSEMBLER_CODE",
		}
		

project "OpenWolf-dedicated"
	targetname  "Openwolf-dedicated"
	language    "C++"
	kind        "ConsoleApp"
	targetdir 	"../.."
	flags       { "ExtraWarnings" }
	files
	{
		"server/**.cpp", "server/**.h",
		
		"null/null_client.cpp",
		"null/null_input.cpp",
		"null/null_snddma.cpp",
		
		"qcommon/**.h", 
		"qcommon/cmd.cpp",
		"qcommon/common.cpp",
		"qcommon/cvar.cpp",
		"qcommon/files.cpp",
		"qcommon/huffman.cpp",
		"qcommon/md4.cpp",
		"qcommon/md5.cpp",
		"qcommon/msg.cpp",
		"qcommon/vm.cpp",
		"qcommon/net_*.cpp",
		"qcommon/unzip.cpp",
		"qcommon/unzip.h",
		"qcommon/q_shared.cpp",
		"qcommon/q_math.cpp",
		"qcommon/ioapi.cpp",
		"qcommon/ioapi.h",
		
        "cm/cm_load.cpp",
		"cm/cm_local.h",
		"cm/cm_patch.cpp",
		"cm/cm_polylib.cpp",
		"cm/cm_polylib.h",
		"cm/cm_public.h",
		"cm/cm_test.cpp",
		"cm/cm_trace.cpp",
		"cm/cm_trisoup.cpp",
		
		"../libs/zlib/adler32.c", 
		"../libs/zlib/compress.c",
		"../libs/zlib/crc32.c",
		"../libs/zlib/deflate.c",
		"../libs/zlib/gzio.c",
		"../libs/zlib/infback.c",
		"../libs/zlib/inffast.c",
		"../libs/zlib/inflate.c",
		"../libs/zlib/inftrees.c",
		"../libs/zlib/trees.c",
		"../libs/zlib/uncompr.c",
		"../libs/zlib/zutil.c",
		
		"../libs/zlib/crc32.h",
		"../libs/zlib/deflate.h",
		"../libs/zlib/inffast.h",
		"../libs/zlib/inffixed.h",
		"../libs/zlib/inflate.h",
		"../libs/zlib/inftrees.h",
		"../libs/zlib/trees.h",
		"../libs/zlib/zconf.h",
		"../libs/zlib/zlib.h",
		"../libs/zlib/zutil.h",
	}
	includedirs
	{
		"./",
		"../",
		"../libs/zlib",
		"../libs/sdl2/include",
	}
	defines
	{ 
		"DEDICATED",
		"STANDALONE",
		--"USE_MUMBLE",
		--"USE_VOIP",
	}
	links
	{
		"SDL2",
		"SDL2main",
		--"dinput8",
	}
	
	--
	-- Platform Configurations
	-- 	
	configuration "x32"
		targetdir 	"../../bin32"
		files
		{ 
			--"code/qcommon/vm_x86.cpp",
		}
	
	configuration "x64"
		targetdir 	"../../bin64"
		
	--
	-- Options Configurations
	--
		
	configuration "with-bullet"
		defines
		{
			"USE_BULLET"
		}
		includedirs
		{
			"../libs/bullet"
		}
		files
		{
			"cm/cm_bullet.cpp",
		
			"../libs/bullet/*.h",
			"../libs/bullet/LinearMath/**.cpp", "../libs/bullet/LinearMath/**.h",
			"../libs/bullet/BulletCollision/**.cpp", "../libs/bullet/BulletCollision/**.h",
			"../libs/bullet/BulletDynamics/**.cpp", "../libs/bullet/BulletDynamics/**.h",
			"../libs/bullet/BulletSoftBody/**.cpp", "../libs/bullet/BulletSoftBody/**.h",
		}
	
	-- 
	-- Project Configurations
	-- 
	configuration "vs*"
		flags       { "WinMain" }
		files
		{
			"sys/sys_main.cpp",
			"sys/sys_win32.cpp",
			"sys/con_log.cpp",
			"sys/con_win32.cpp",
			
			"sys/OpenWolf.ico",
			"sys/win_resource.rc",
		}
		libdirs
		{
			--"../libs/curl-7.12.2/lib"
		}
		links
		{ 
			"winmm",
			"wsock32",
			"user32",
			"advapi32",
			"ws2_32",
			"Psapi"
		}
		--linkoptions
		--{
		--	"/NODEFAULTLIB:libc"
		--}
		-- buildoptions
		-- {
			-- "/EHa",
		-- }
		defines
		{
			"WIN32",
			"_CRT_SECURE_NO_WARNINGS",
			--"USE_INTERNAL_SPEEX",
			--"USE_INTERNAL_ZLIB",
			--"FLOATING_POINT",
			--"USE_ALLOCA"
		}
		
	configuration { "vs*", "x32" }
		files
		{
			"gldraw.c",
		}
		libdirs
		{
			"../libs/sdl2/lib/x86",
			"../libs/openal/libs/win32",
			--"../libs/curl-7.12.2/lib"
			"../libs/zlibwapi/lib/x32",
		}
		includedirs
		{
			"./",
			"../libs/sdl2/include",
			"../libs/openal/include",
			"../libs/zlibwapi/include",
		}
		links
		{
			"SDL2",
			"SDL2main",
			--"dinput8",
			"winmm",
			"wsock32",
			--"Iphlpapi",
			"user32",
			"advapi32",
			"ws2_32",
			"Psapi",
			"zlibwapi",
		}
		
	configuration { "vs*", "x64" }
		libdirs
		{
			"../libs/sdl2/lib/x64",
			"../libs/openal/libs/win64",
			--"../libs/curl-7.12.2/lib",
			"../libs/zlibwapi/lib/lib64",
		}
		includedirs
		{
			"./",
			"../libs/sdl2/include",
			"../libs/openal/include",
			"../libs/zlibwapi/include",
		}
		links
		{
			"SDL2",
			"SDL2main",
			--"dinput8",
			"winmm",
			"wsock32",
			--"Iphlpapi",
			"user32",
			"advapi32",
			"ws2_32",
			"Psapi",
			"zlibwapi",
		}

		defines
		{
			"_WIN32",
			"_WIN64",
			"__WIN64__",
			"__AMD64__",
			"_CRT_SECURE_NO_WARNINGS",
		}


	configuration { "linux", "gmake" }
		buildoptions
		{
			"`pkg-config --cflags sdl2`",
		}
		linkoptions
		{
			"`pkg-config --libs sdl2`",
		}
	
	configuration "linux"
		targetname  "Openwolf-dedicated"
		files
		{
			"sys/sys_main.cpp",
			"sys/sys_unix.cpp",
			"sys/con_log.cpp",
			"sys/con_tty.cpp",
		}
		--buildoptions
		--{
		--	"-pthread"
		--}
		links
		{
			"dl",
			"m",
		}
		
		
