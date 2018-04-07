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
// cg_path.cpp -- path debugging and testing

#include "cg_local.h"
#include "Recast.h"

#include "DetourNavMesh.h"
#include "DetourNavMeshBuilder.h"
#include "DetourNavMeshQuery.h"
#include "DetourCommon.h"

vec4_t debugColor_path = { 255, 2, 2, 250};

unsigned char* navData;
int navDataSize;
dtNavMeshCreateParams navParams;
dtNavMeshQuery* navQuery;
dtNavMesh* navMesh;
dtQueryFilter navFilter;

dtPolyRef pathPolys[MAX_PATH_POLYS];
int pathNumPolys;

float pathStraight[MAX_PATH_POLYS * 3];
int pathNumStraight;

// from Mikko: "A good query initial guess for extents is twice the agent height and 5-10 agent radius"
const float pathExtend[3] = {128, 640, 640};

vec3_t startVec, endVec;
dtPolyRef startRef, endRef;



/*
==================
CG_PathDraw
==================
*/

void CG_PathDraw( void )
{

    if( !cg_detourDebug.integer )
        return;
        
        
    for( int i = 0; i < pathNumStraight - 1; i++ )
    {
    
        vec3_t o, start, end;
        
        //VectorSet(o, pathStraight[i*3]*cg.nv_info.cellSize, pathStraight[i*3+1]*cg.nv_info.cellHeight, pathStraight[i*3+2]*cg.nv_info.cellSize );
        //VectorSet(o, pathStraight[i*3], pathStraight[i*3+1], pathStraight[i*3+2] );
        VectorSet( o, pathStraight[i * 3 + 0], pathStraight[i * 3 + 1] + 0.3f, pathStraight[i * 3 + 2] );
        recast2quake( o, start );
        
        //VectorScale(start, 8.0f, start);
        
        start[0] += -cg.nv_info.mins[0];
        start[1] += -cg.nv_info.mins[2];
        start[2] += cg.nv_info.mins[1];
        
        //VectorSet(o, pathStraight[(i+1)*3]*cg.nv_info.cellSize, (pathStraight[(i+1)*3+1]+1)*cg.nv_info.cellHeight, pathStraight[(i+1)*3+2]*cg.nv_info.cellSize );
        //VectorSet(o, pathStraight[(i+1)*3], (pathStraight[(i+1)*3+1]+1), pathStraight[(i+1)*3+2] );
        VectorSet( o, pathStraight[( i + 1 ) * 3 + 0], pathStraight[( i + 1 ) * 3 + 1] + 0.3f, pathStraight[( i + 1 ) * 3 + 2] );
        recast2quake( o, end );
        
        //VectorScale(end, 8.0f, end);
        
        end[0] += -cg.nv_info.mins[0];
        end[1] += -cg.nv_info.mins[2];
        end[2] += cg.nv_info.mins[1];
        
        
        CG_DebugDrawLine( start, end, colorGreen );
        
    }
    
    // draw the testing path:
    
    vec3_t o, v;
    int j;
    
    //startVec
    VectorCopy( startVec, v );
    VectorSet( o, v[0], ( v[1] + 1 ), v[2] );
    
    recast2quake( o, v );
    
    v[0] += -cg.nv_info.mins[0];
    v[1] += -cg.nv_info.mins[2];
    v[2] += cg.nv_info.mins[1];
    
    CG_DebugDrawPoint( v, 24 );
    
    //endVec
    VectorCopy( endVec, v );
    VectorSet( o, v[0], ( v[1] + 1 ), v[2] );
    
    recast2quake( o, v );
    
    v[0] += -cg.nv_info.mins[0];
    v[1] += -cg.nv_info.mins[2];
    v[2] += cg.nv_info.mins[1];
    
    CG_DebugDrawPoint( v, 32 );
    
    // poly refs
    unsigned int salt, it, ip;
    const unsigned short* p;
    unsigned short vi[3];
    
    // start poly
    navMesh->decodePolyId( startRef, salt, it, ip );
    p = &cg.nv_polys[ip * cg.nv_nvp * 2];
    for( j = 2; j < cg.nv_nvp; j++ )
    {
        if( p[j] == RC_MESH_NULL_IDX ) break;
        vi[0] = p[0];
        vi[1] = p[j - 1];
        vi[2] = p[j];
        
        CG_NavDrawPolyVert( vi, colorRed );
    }
    
    // end poly
    navMesh->decodePolyId( endRef, salt, it, ip );
    p = &cg.nv_polys[ip * cg.nv_nvp * 2];
    for( j = 2; j < cg.nv_nvp; j++ )
    {
        if( p[j] == RC_MESH_NULL_IDX ) break;
        vi[0] = p[0];
        vi[1] = p[j - 1];
        vi[2] = p[j];
        
        CG_NavDrawPolyVert( vi, colorYellow );
    }
    
}

/*
==================
CG_PathInit
==================
*/

void CG_PathInit( void )
{
    int i;
    
    Com_Printf( "==== Path Debug ==== \n" );
    
    
    // test detour
    memset( &navParams, 0, sizeof( navParams ) );
    
    navParams.verts = cg.nv_verts;
    navParams.vertCount = cg.nv_nverts;
    navParams.polys = cg.nv_polys;
    navParams.polyAreas = cg.nv_areas;
    navParams.polyFlags = cg.nv_flags;
    navParams.polyCount = cg.nv_npolys;
    navParams.nvp = cg.nv_nvp;
    navParams.detailMeshes = cg.nv_dmeshes;
    navParams.detailVerts = cg.nv_dverts;
    navParams.detailVertsCount = cg.nv_ndverts;
    navParams.detailTris = cg.nv_dtris;
    navParams.detailTriCount = cg.nv_ndtris;
    navParams.cs = cg.nv_info.cellSize;
    navParams.ch = cg.nv_info.cellHeight;
    
    
    navParams.walkableHeight = 64 / cg.nv_info.cellHeight;
    navParams.walkableClimb = STEPSIZE / cg.nv_info.cellHeight;
    navParams.walkableRadius = 15 / cg.nv_info.cellSize;
    
    VectorCopy( cg.nv_info.mins, navParams.bmin );
    VectorCopy( cg.nv_info.maxs, navParams.bmax );
    navParams.buildBvTree = true;
    
    navData = NULL;
    navDataSize = 0;
    
    if( !dtCreateNavMeshData( &navParams, &navData, &navDataSize ) )
    {
        Com_Printf( "Could not build Detour navmesh.\n" );
        return;
    }
    
    navMesh = dtAllocNavMesh();
    
    if( !navMesh )
    {
        Com_Printf( "Could not create Detour navmesh.\n" );
        return;
    }
    
    if( !navMesh->init( navData, navDataSize, DT_TILE_FREE_DATA ) )
    {
        Com_Printf( "Could not init Detour navmesh.\n" );
        return;
    }
    
    navQuery = dtAllocNavMeshQuery();
    
    if( !navQuery->init( navMesh, 2048 ) )
    {
        Com_Printf( "Could not init Detour navmesh query" );
        return;
    }
    
    Com_Printf( "Detour cool ;).\n" );
    
    //navFilter.setIncludeFlags((unsigned short) 0x3);
    //navFilter.setExcludeFlags((unsigned short) 0x0);
    
    
    // quake vects!
    vec3_t v;
    
    VectorSet( v, -704, -512, 640 );
    v[0] -= -cg.nv_info.mins[0];
    v[1] -= -cg.nv_info.mins[2];
    v[2] -= cg.nv_info.mins[1];
    quake2recast( v, startVec );
    
    VectorSet( v, -2816, -2880, 640 );
    v[0] -= -cg.nv_info.mins[0];
    v[1] -= -cg.nv_info.mins[2];
    v[2] -= cg.nv_info.mins[1];
    quake2recast( v, endVec );
    
    
    
    // these are recast vects!
    //VectorSet(startVec, 83, 640, 121);
    //VectorSet(endVec, 2032, 640, 2070);
    
    navQuery->findNearestPoly( startVec, pathExtend, &navFilter, &startRef, 0 );
    navQuery->findNearestPoly( endVec, pathExtend, &navFilter, &endRef, 0 );
    
    if( !startRef )
    {
        Com_Printf( "    couldn't find start ref, exiting detour testing\n" );
        return;
    }
    if( !endRef )
    {
        Com_Printf( "    couldn't find end ref, exiting detour testing\n" );
        return;
    }
    
    // find the path
    navQuery->findPath( startRef, endRef, startVec, endVec, &navFilter, pathPolys, &pathNumPolys, MAX_PATH_POLYS );
    Com_Printf( "    found path: %i\n", pathNumPolys );
    
    // get waypoints from it
    
    navQuery->findStraightPath( startVec, endVec, pathPolys, pathNumPolys, pathStraight, 0, 0, &pathNumStraight, MAX_PATH_POLYS );
    Com_Printf( "    found straight waypoints: %i\n", pathNumStraight );
    
}

/*
==================
CG_PathShutdown
==================
*/

void CG_PathShutdown( void )
{

}



