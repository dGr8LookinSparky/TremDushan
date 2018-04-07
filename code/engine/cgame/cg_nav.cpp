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
// cg_nav.cpp -- navigation mesh debugging

#include "cg_local.h"
#include "Recast.h"

vec4_t debugColor_walk = { 120, 100, 10, 150};
vec4_t debugColor_swim = { 10, 100, 120, 150};
vec4_t debugColor_door = { 10, 120, 100, 150};

vec4_t debugColor_other = { 200, 30, 30, 150};

vec4_t debugColor_edge1 = { 100, 100, 120, 100};
vec4_t debugColor_edge2 = { 100, 120, 100, 100};
vec4_t debugColor_edge3 = { 120, 100, 100, 100};

void CG_NavDrawPolyPoint( const unsigned short* vi, bool detailed )
{
    vec3_t o, start;
    const unsigned short* v;
    
    v = vi;
    if( !detailed )
        VectorSet( o, v[0]*cg.nv_info.cellSize, ( v[1] + 1 )*cg.nv_info.cellHeight, v[2]*cg.nv_info.cellSize );
    else
        VectorSet( o, v[0], ( v[1] + 1 ), v[2] );
        
    recast2quake( o, start );
    start[0] += -cg.nv_info.mins[0];
    start[1] += -cg.nv_info.mins[2];
    start[2] += cg.nv_info.mins[1];
    
    CG_DebugDrawPoint( start, detailed ? 3 : 12 );
    
}

void CG_NavDrawPolyEdge( const unsigned short* vi, vec4_t color )
{
    vec3_t o, start, end;
    const unsigned short* v;
    
    v = &cg.nv_verts[vi[0] * 3];
    VectorSet( o, v[0]*cg.nv_info.cellSize, ( v[1] + 1 )*cg.nv_info.cellHeight, v[2]*cg.nv_info.cellSize );
    recast2quake( o, start );
    start[0] += -cg.nv_info.mins[0];
    start[1] += -cg.nv_info.mins[2];
    start[2] += cg.nv_info.mins[1];
    
    v = &cg.nv_verts[vi[1] * 3];
    VectorSet( o, v[0]*cg.nv_info.cellSize, ( v[1] + 1 )*cg.nv_info.cellHeight, v[2]*cg.nv_info.cellSize );
    recast2quake( o, end );
    end[0] += -cg.nv_info.mins[0];
    end[1] += -cg.nv_info.mins[2];
    end[2] += cg.nv_info.mins[1];
    
    CG_DebugDrawLine( start, end, color );
}

void CG_NavDrawPolyVert( unsigned short* vi, vec4_t color )
{
    vec3_t          o;
    polyVert_t      verts[3];
    int             i;
    const unsigned short* v;
    
    v = &cg.nv_verts[vi[0] * 3];
    VectorSet( o, v[0]*cg.nv_info.cellSize, ( v[1] + 1 )*cg.nv_info.cellHeight, v[2]*cg.nv_info.cellSize );
    recast2quake( o, verts[0].xyz );
    verts[0].xyz[0] += -cg.nv_info.mins[0];
    verts[0].xyz[1] += -cg.nv_info.mins[2];
    verts[0].xyz[2] += cg.nv_info.mins[1];
    
    v = &cg.nv_verts[vi[1] * 3];
    VectorSet( o, v[0]*cg.nv_info.cellSize, ( v[1] + 1 )*cg.nv_info.cellHeight, v[2]*cg.nv_info.cellSize );
    recast2quake( o, verts[1].xyz );
    verts[1].xyz[0] += -cg.nv_info.mins[0];
    verts[1].xyz[1] += -cg.nv_info.mins[2];
    verts[1].xyz[2] += cg.nv_info.mins[1];
    
    v = &cg.nv_verts[vi[2] * 3];
    VectorSet( o, v[0]*cg.nv_info.cellSize, ( v[1] + 1 )*cg.nv_info.cellHeight, v[2]*cg.nv_info.cellSize );
    recast2quake( o, verts[2].xyz );
    verts[2].xyz[0] += -cg.nv_info.mins[0];
    verts[2].xyz[1] += -cg.nv_info.mins[2];
    verts[2].xyz[2] += cg.nv_info.mins[1];
    
    CG_DebugDrawVert( verts, color );
}



/*
==================
CG_NavDraw
==================
*/

void CG_NavDraw( void )
{
    int i, j;
    vec4_t color;
    
    if( !cg_recastDebug.integer )
        return;
        
    // draw polys
    for( i = 0; i < cg.nv_npolys; i++ )
    {
        const unsigned short* p = &cg.nv_polys[i * cg.nv_nvp * 2];
        
        if( cg.nv_areas[i] == SAMPLE_POLYFLAGS_WALK )
            Vector4Copy( debugColor_walk, color );
        else if( cg.nv_areas[i] == SAMPLE_POLYFLAGS_SWIM )
            Vector4Copy( debugColor_swim, color );
        else if( cg.nv_areas[i] == SAMPLE_POLYAREA_DOOR )
            Vector4Copy( debugColor_door, color );
        else
            Vector4Copy( debugColor_other, color );
            
        unsigned short vi[3];
        for( j = 2; j < cg.nv_nvp; j++ )
        {
            if( p[j] == RC_MESH_NULL_IDX ) break;
            vi[0] = p[0];
            vi[1] = p[j - 1];
            vi[2] = p[j];
            
            CG_NavDrawPolyVert( vi, color );
        }
    }
    
    // draw edges
    for( i = 0; i < cg.nv_npolys; i++ )
    {
        const unsigned short* p = &cg.nv_polys[i * cg.nv_nvp * 2];
        for( j = 0; j < cg.nv_nvp; j++ )
        {
            if( p[j] == RC_MESH_NULL_IDX ) break;
            if( p[cg.nv_nvp + j] & 0x8000 )
            {
                Vector4Copy( debugColor_edge1, color );
            }
            else
            {
                Vector4Copy( debugColor_edge2, color );
                if( ( p[cg.nv_nvp + j] & 0xf ) != 0xf )
                    Vector4Copy( debugColor_edge3, color );
            }
            const int nj = ( j + 1 >= cg.nv_nvp || p[j + 1] == RC_MESH_NULL_IDX ) ? 0 : j + 1;
            unsigned short vi[2] = {p[j], p[nj]};
            
            CG_NavDrawPolyEdge( vi, color );
        }
    }
    /*
    	// draw points
    	for (  i = 0, j = 0; i < cg.nv_nverts; i++, j += 3 )
    	{
    		unsigned short vi[3] = {cg.nv_verts[j + 0], cg.nv_verts[j + 1], cg.nv_verts[j + 2] };
    		CG_NavDrawPolyPoint(vi, false);
    	}
    */
    
}



/*
================
CG_NavLoadMeshFile

read from disk file
================
*/

static void CG_NavLoadMeshFile( void )
{
    fileHandle_t    file;
    char            filename[MAX_QPATH];
    //int             version;
    char            mapname[MAX_QPATH];
    int             i, j;
    
    trap_Cvar_VariableStringBuffer( "mapname", mapname, sizeof( mapname ) );
    Com_sprintf( filename, sizeof( filename ), "maps/%s.navMesh", mapname );
    Com_Printf( "    loading navigation mesh file '%s'...\n", filename );
    
    trap_FS_FOpenFile( filename, &file, FS_READ );
    if( !file )
    {
        Com_Printf( "    no navigation mesh file '%s' found!\n", filename );
        return;
    }
    
    // determin version
    trap_FS_Read( &cg.nv_info.version, sizeof( int ), file );	// read version
    
    if( cg.nv_info.version == 1 )
    {
        // load cg.nv_info
        trap_FS_Read( &cg.nv_info.vertsOffset, sizeof( int ), file );
        trap_FS_Read( &cg.nv_info.numVerts, sizeof( int ), file );
        trap_FS_Read( &cg.nv_info.polysOffset, sizeof( int ), file );
        trap_FS_Read( &cg.nv_info.areasOffset, sizeof( int ), file );
        trap_FS_Read( &cg.nv_info.flagsOffset, sizeof( int ), file );
        trap_FS_Read( &cg.nv_info.numPolys, sizeof( int ), file );
        trap_FS_Read( &cg.nv_info.numVertsPerPoly, sizeof( int ), file );
        trap_FS_Read( &cg.nv_info.mins, sizeof( float ) * 3, file );
        trap_FS_Read( &cg.nv_info.maxs, sizeof( float ) * 3, file );
        trap_FS_Read( &cg.nv_info.dMeshesOffset, sizeof( int ), file );
        trap_FS_Read( &cg.nv_info.dNumMeshes, sizeof( int ), file );
        trap_FS_Read( &cg.nv_info.dVertsOffset, sizeof( int ), file );
        trap_FS_Read( &cg.nv_info.dNumVerts, sizeof( int ), file );
        trap_FS_Read( &cg.nv_info.dTrisOffset, sizeof( int ), file );
        trap_FS_Read( &cg.nv_info.dNumTris, sizeof( int ), file );
        trap_FS_Read( &cg.nv_info.cellSize, sizeof( int ), file );
        trap_FS_Read( &cg.nv_info.cellHeight, sizeof( int ), file );
        trap_FS_Read( &cg.nv_info.filesize, sizeof( int ), file );
        
        // load verts
        int vertsSize = cg.nv_info.numVerts * sizeof( unsigned short ) * 3;
        cg.nv_verts = ( unsigned short* )malloc( vertsSize );
        memset( cg.nv_verts, 0, vertsSize );
        cg.nv_nverts = cg.nv_info.numVerts;
        trap_FS_Read( cg.nv_verts, vertsSize,  file );
        
        // load polys
        int polysSize = cg.nv_info.numPolys * sizeof( unsigned short ) * cg.nv_info.numVertsPerPoly * 2;
        cg.nv_polys = ( unsigned short* )malloc( polysSize );
        memset( cg.nv_polys, 0, polysSize );
        cg.nv_npolys = cg.nv_info.numPolys;
        cg.nv_nvp = cg.nv_info.numVertsPerPoly;
        trap_FS_Read( cg.nv_polys, polysSize,  file );
        
        // load areas
        int areasSize = cg.nv_info.numPolys * sizeof( unsigned char );
        cg.nv_areas = ( unsigned char* )malloc( areasSize );
        memset( cg.nv_areas, 0, areasSize );
        trap_FS_Read( cg.nv_areas, areasSize,  file );
        
        // load flags
        int flagsSize = cg.nv_info.numPolys * sizeof( unsigned short );
        cg.nv_flags = ( unsigned short* )malloc( flagsSize );
        memset( cg.nv_flags, 0, flagsSize );
        trap_FS_Read( cg.nv_flags, flagsSize,  file );
        
        // load detailed meshes
        int dMeshesSize = cg.nv_info.dNumMeshes * sizeof( unsigned int ) * 4;
        cg.nv_dmeshes = ( unsigned int* )malloc( dMeshesSize );
        memset( cg.nv_dmeshes, 0, dMeshesSize );
        cg.nv_ndmeshes = cg.nv_info.dNumMeshes;
        trap_FS_Read( cg.nv_dmeshes, dMeshesSize,  file );
        
        // load detailed verts
        int dVertsSize = cg.nv_info.dNumVerts * sizeof( float ) * 3;
        cg.nv_dverts = ( float* )malloc( dVertsSize );
        memset( cg.nv_dverts, 0, dVertsSize );
        cg.nv_ndverts = cg.nv_info.dNumVerts;
        trap_FS_Read( cg.nv_dverts, dVertsSize,  file );
        
        // load detailed tris
        int dTrisSize = cg.nv_info.dNumTris * sizeof( unsigned char ) * 4;
        cg.nv_dtris = ( unsigned char* )malloc( dTrisSize );
        memset( cg.nv_dtris, 0, dTrisSize );
        cg.nv_ndtris = cg.nv_info.dNumTris;
        trap_FS_Read( cg.nv_dtris, dTrisSize,  file );
        
    }
    else
    {
        Com_Printf( "    '%s' has wrong version %i!\n", filename, cg.nv_info.version );
    }
    
    trap_FS_FCloseFile( file );
    
    Com_Printf( "    done.\n" );
    
}

/*
==================
CG_NavInit
==================
*/

void CG_NavInit( void )
{
    Com_Printf( "==== Navigation Debug ==== \n" );
    
    memset( &cg.nv_info, 0, sizeof( navMeshDataInfo_t ) );
    
    CG_NavLoadMeshFile();
}


/*
==================
CG_NavShutdown
==================
*/

void CG_NavShutdown( void )
{

}

