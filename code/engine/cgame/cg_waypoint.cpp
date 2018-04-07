/*
===========================================================================
Copyright (C) 1999-2005 Id Software, Inc.
Copyright (C) 2006 Robert Beckebans <trebor_7@users.sourceforge.net>
Copyright (C) 2011 Adrian Fuhrmann <aliasng@gmail.com>

This file is part of Legacy source code.

Legacy source code is free software; you can redistribute it
and/or modify it under the terms of the GNU General Public License as
published by the Free Software Foundation; either version 2 of the License,
or (at your option) any later version.

Legacy source code is distributed in the hope that it will be
useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Legacy source code; if not, write to the Free Software
Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
===========================================================================
*/
// cg_waypoint.cpp -- waypoint layer debugging tools
//

#include "cg_local.h"
#include "Recast.h"

/*
 * drop waypoints at interesting points...
 *
 *
*/

/*
==================
CG_WaypointDraw
==================
*/

void CG_WaypointDraw( void )
{
    int i, n;
    
    if( !cg_waypointDebug.integer )
        return;
        
        
}

/*
==================
CG_WaypointInit
==================
*/

void CG_WaypointInit( void )
{
    int i, j, n, k;
    vec4_t color;
    
    Com_Printf( "==== Waypoint Debug ==== \n" );
    
    
    Com_Printf( "	searching waypoints..." );
    Com_Printf( " %i found  \n", cg.nv_nwaypoints );
    
}

/*
==================
CG_WaypointShutdown
==================
*/

void CG_WaypointShutdown( void )
{

}



