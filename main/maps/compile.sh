#!/bin/bash
################################################################################
#                                                                              #
#  XMAP2 map compilation script.                                               #
#  By Evan 'Odin' Goers                                                        #
#                                                                              #
#  Nav Mesh Generation Options added for Legacy       		       	       #   
#  by Adrian 'otty' Fuhrmann 2011                                              #
#                                                                              #
# To use this script, add your compilation options for each stage(bsp, vis,    #
# light) to your map's worldspawn entity. You can do this by adding a key      #
# called xmap_bspopts, xmap_visopts, or xmap_lightopts. An example is as       #
# follows:                                                                     #
#                                                                              #
#  "xmap_lightopts" "-fast -filter -bounce 2"                                  #
#                                                                              #
################################################################################


# fs_basepath is generally '../..' since xmap2_compile is in the base/maps/ dir.
FS_BASEPATH='../..'
# fs_game should be 'base'
FS_GAME='base'
# game should be 'xreal'
GAME='legacy'

# DO NOT EDIT THIS LINE OR THE FOLLOWING LINES. DOING SO MAY RESULT IN
# UNEXPECTED SCRIPT BEHAVIOR.
MAP="`basename $2 .map`.map"
THREADS=`grep processor /proc/cpuinfo | awk '{a++} END {print a}'`

# map_ convention used to prevent possible binary name collision

map_bsp() {
	if ! $COMPILE -v -meta -patchmeta -leaktest -navMesh $BSPOPTS $MAP; then exit 1; fi
}

map_debugportals() {
	if ! $COMPILE -v -debugportals -meta -leaktest $MAP; then exit 1; fi
}

map_debugsurfaces() {
	if ! $COMPILE -v -debugsurfaces -meta -leaktest $MAP; then exit 1; fi
}

map_fastvis() {
	if ! $COMPILE -vis -threads $THREADS -fast $VISOPTS $MAP; then exit 1; fi
}

map_nav() {
	if ! $COMPILE -nav $MAP; then exit 1; fi
}

map_vis() {
	if ! $COMPILE -vis -threads $THREADS $VISOPTS $MAP; then exit 1; fi
}

map_light() {
	if ! $COMPILE -light -threads $THREADS -v $LIGHTOPTS $MAP; then exit 1; fi
}

map_scale() {
	if ! $COMPILE -v -scale "$@" $MAP; then exit 1; fi
}

map_package() {
	cd ..
	DATE=`date +%Y%m%d`
	MAP_STRIPPED=`basename $MAP .map`
	PK3NAME=map-$MAP_STRIPPED-$DATE.pk3
	zip $PK3NAME maps/$MAP_STRIPPED.bsp
	zip $PK3NAME maps/$MAP_STRIPPED.map
	zip $PK3NAME maps/$MAP_STRIPPED/*.hdr
	zip $PK3NAME maps/$MAP_STRIPPED/*.png
	zip $PK3NAME levelshot/$MAP_STRIPPED.png
	zip $PK3NAME scripts/$MAP_STRIPPED.arena
}

map_readbspopts() {
	if [ -r $MAP ]; then
		#grab the bspopts from the map
		BSPOPTS=`grep -A 256 "^// entity 0$" $MAP | \
		grep xmap_bspopts | \
		head -n1 | \
		sed -r 's/^"xmap_bspopts" "(.*)"$/\1/'`
		if [[ $BSPOPTS != "" ]]; then
			echo "BSPOPTS: $BSPOPTS"
		fi
	else
		echo "$MAP: no such map."
		exit 1
	fi
}

map_readvisopts() {
	if [ -r $MAP ]; then
		#grab the visopts from the map
		VISOPTS=`grep -A 256 "^// entity 0$" $MAP | \
		grep xmap_visopts | \
		head -n1 | \
		sed -r 's/^"xmap_visopts" "(.*)"$/\1/'`
		if [[ $VISOPTS != "" ]]; then
			echo "VISOPTS: $VISOPTS"
		fi
	else
		echo "$MAP: no such map."
		exit 1
	fi
}

map_readlightopts() {
	if [ -r $MAP ]; then
		#grab the lightopts from the map
		LIGHTOPTS=`grep -A 256 "^// entity 0$" $MAP | \
		grep xmap_lightopts | \
		head -n1 | \
		sed -r 's/^"xmap_lightopts" "(.*)"$/\1/'`
		if [[ $LIGHTOPTS != "" ]]; then
			echo "LIGHTOPTS: $LIGHTOPTS"
		else
			echo "Forcing '-fast -filter -lightmapsize 1024' as per map standards."
			echo "Add 'xmap_lightopts in worldspawn to override."
			LIGHTOPTS="-fast -filter -lightmapsize 1024"
		fi
	else
		echo "$MAP: no such map."
		exit 1
	fi
}

map_defaultcommand() {
	echo "Usage: $0 [OPTION] [FILE]"
	echo "Try \`$0 -h' for more information."
}

map_help() {
	echo "Usage: $0 [OPTION] [FILE]"
	echo "Compile a map into a BSP file and perform other functions."
	echo "Example $0 -all arena1.map"
	echo "Compile options(only one may be used):"
	echo "  -bsp            Compile the map into a basic BSP."
	echo "  -vis            Calculate VIS for an existing BSP."
	echo "  -nav            Calculate navigation mesh for an existing BSP."
	echo "  -fastvis        Same as -vis, at the cost of accuracy."
	echo "  -light          Calculate light for an existing BSP."
	echo "  -pk3            Attempt to automatically package a map project."
	echo "  -all            Short for -bsp -vis -light."
	echo "  -h              This help message."
	echo ""
	echo "This compilation script was written by Evan 'Odin' Goers, with"
	echo "help and additions from DavidSev, and Robert 'Tr3B' Beckebans."
	echo "Recast Navigation Mesh generation added by Adrian 'otty' Fuhrmann."
}

check_compiler() {
	case `uname -m` in
		x86_64)
			COMPILER='../../bin64/legacymap2'
			;;
		i386|i486|i586|i686)
			COMPILER='../../bin32/legacymap2'
			;;
		*)
			echo "You are not running a supported platform for compiling XreaL maps. If you would like to compile XreaL maps on your platform, please visit http://www.xreal-project.net for contact information."
			exit 0
			;;
	esac

	if [ ! -f $COMPILER ]; then
		echo "`basename $COMPILER` not found!"
		echo "Did you build XreaL with scons xmap=1?"
		exit 1
	fi
	COMPILE="$COMPILER -fs_basepath $FS_BASEPATH -fs_game $FS_GAME -game $GAME"
}

case $1 in
	-bsp)
		check_compiler;
		map_readbspopts;
		map_bsp $BSPOPTS;
		;;
		
	-debugportals)
		check_compiler;
		map_debugportals;
		;;
	
	-debugsurfaces)
		check_compiler;
		map_debugsurfaces;
		;;

	-fastvis)
		check_compiler;
		map_readvisopts;
		map_fastvis $VISOPTS;
		;;
		
	-vis)
		check_compiler;
		map_readvisopts;
		map_vis $VISOPTS;
		;;		
	-nav)
		check_compiler;
		map_nav;
		;;

	-light)
		check_compiler;
		map_readlightopts;
		map_light $LIGHTOPTS;
		;;
		
	-pk3)
		map_package $PACKAGEOTPS;
		;;

	-all)
		check_compiler;
		map_readbspopts;
		map_bsp $BSPOPTS;
		map_readvisopts;
		map_vis $VISOPTS;
		map_readlightopts;
		map_light $LIGHTOPTS;
		map_nav;
		;;
	-h)
		map_help;
		;;
	*)
		map_defaultcommand;
		;;
esac

