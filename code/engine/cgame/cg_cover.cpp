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
// cg_cover.cpp -- cover layer debugging tools
//

#include "cg_local.h"
#include "Recast.h"


/*
==================
CG_CoverDraw
==================
*/

void CG_CoverDraw( void )
{
    int i, n;
    vec3_t org;
    
    if( !cg_coverDebug.integer )
        return;
        
    /* draw cover plates */
    for( i = 0; i < cg.nv_npcovers; i++ )
    {
        cplate_t* cplate;
        cplate = &cg.nv_pcovers[i];
        
        // draw original edge
        CG_DebugDrawLine( cplate->start, cplate->end, colorRed );
        
        // draw orientation
        VectorMA( cplate->center, COVER_PLATE_ORI_CHECK_DIST, cplate->ori , org );
        CG_DebugDrawLine( cplate->center, org, colorCyan );
        
        // draw slice points
        for( n = 0; n < cplate->nsl; n ++ )
        {
            CG_DebugDrawPoint( cplate->sl[n].pos , 2 );
        }
    }
    
    
    return;
    
    
    
    
    
    
    
    
    
    
    
    
    /* draw cover points */
    for( i = 0; i < cg.nv_ncovers; i++ )
    {
        cover_t* cover;
        cover = &cg.nv_covers[i];
        polyVert_t      verts[3];
        
        // first point
        VectorCopy( cover->point, verts[0].xyz );
        
        // second point
        VectorMA( cover->point, COVER_DIST, cover->a , verts[1].xyz );
        
        // third point
        VectorMA( cover->point, COVER_DIST, cover->b, verts[2].xyz );
        
        // show orientation
        VectorMA( cover->point, COVER_DIST, cover->ori , org );
        
        CG_DebugDrawLine( cover->point, org, colorCyan );
        
        // is it save against bots?
        bool save = true;
        
        for( n = 0; n < MAX_GENTITIES; n++ )
        {
            centity_t* cent = &cg_entities[n];
            float angle;
            vec3_t vec;
            
            if( cent->currentState.eType != ET_PLAYER )
                continue;
                
            VectorSubtract( cent->lerpOrigin,  cover->point, vec );
            VectorNormalize( vec );
            angle = AngleBetweenVectors( vec, cover->ori );
            
            if( angle >= ( cover->angle * 0.5f ) )
            {
                trace_t         trace;
                // it is not covering on its own, but is there something in between?
                
                CG_Trace( &trace, cover->point, NULL, NULL, cent->lerpOrigin, -1, CONTENTS_SOLID );
                
                if( trace.fraction == 1.0 )
                {
                    save = false;
                    break;
                }
            }
        }
        
        if( save )
            CG_DebugDrawVert( verts, colorGreen );
        else
            CG_DebugDrawVert( verts, colorRed );
            
        //CG_DebugDrawNumber(cover->point , (int) cover->angle);
    }
    
}

/*
==================
CG_CoverInit
==================
*/

void CG_CoverInit( void )
{
    int i, j, n, k;
    
    vec3_t          right;
    vec3_t          vec;
    vec3_t          v1, v2;
    const unsigned short* v;
    
    vec3_t			org, dir;
    trace_t         trace;
    
    vec3_t 			center;
    float 			len;
    
    Com_Printf( "==== Cover Debug ==== \n" );
    
    
    Com_Printf( "	calculating cover points..." );
    cg.nv_npcovers = 0;
    
    for( i = 0; i < cg.nv_npolys; i++ )
    {
        const unsigned short* p = &cg.nv_polys[i * cg.nv_nvp * 2];
        for( j = 0; j < cg.nv_nvp; j++ )
        {
            if( p[j] == RC_MESH_NULL_IDX ) break;
            if( p[cg.nv_nvp + j] & 0x8000 )
            {
            
            }
            else
            {
                continue;
            }
            cplate_t* cplate;
            const int nj = ( j + 1 >= cg.nv_nvp || p[j + 1] == RC_MESH_NULL_IDX ) ? 0 : j + 1;
            unsigned short vi[2] = {p[j], p[nj]};
            
            cplate = &cg.nv_pcovers[cg.nv_npcovers];
            memset( cplate, 0, sizeof( cplate_t ) );
            
            // construct the origin cover edge
            vec3_t o, start, end;
            const unsigned short* v;
            
            v = &cg.nv_verts[vi[0] * 3];
            VectorSet( org, v[0]*cg.nv_info.cellSize, ( v[1] + 1 )*cg.nv_info.cellHeight, v[2]*cg.nv_info.cellSize );
            recast2quake( org, cplate->start );
            cplate->start[0] += -cg.nv_info.mins[0];
            cplate->start[1] += -cg.nv_info.mins[2];
            cplate->start[2] += cg.nv_info.mins[1];
            // make sure to drop the point to the ground!
            VectorCopy( cplate->start, org );
            org[2] -= 9999.0f;
            CG_Trace( &trace, cplate->start, NULL, NULL, org, -1, CONTENTS_SOLID );
            VectorCopy( trace.endpos, cplate->start );
            cplate->start[2] += 2;
            
            
            v = &cg.nv_verts[vi[1] * 3];
            VectorSet( org, v[0]*cg.nv_info.cellSize, ( v[1] + 1 )*cg.nv_info.cellHeight, v[2]*cg.nv_info.cellSize );
            recast2quake( org, cplate->end );
            cplate->end[0] += -cg.nv_info.mins[0];
            cplate->end[1] += -cg.nv_info.mins[2];
            cplate->end[2] += cg.nv_info.mins[1];
            // make sure to drop the point to the ground!
            VectorCopy( cplate->end, org );
            org[2] -= 9999.0f;
            CG_Trace( &trace, cplate->end, NULL, NULL, org, -1, CONTENTS_SOLID );
            VectorCopy( trace.endpos, cplate->end );
            cplate->end[2] += 2;
            
            // if edge to small, throw it away. Value comes from playerMins and playerMaxs
            VectorSubtract( cplate->end, cplate->start, dir );
            cplate->len = VectorLength( dir );
            
            if( cplate->len <= abs( playerMins[0] - playerMaxs[0] ) )
                continue;
                
            // calculate center point for various testing
            len = cplate->len * 0.5f;
            VectorNormalize( dir );
            VectorMA( cplate->start, len, dir, cplate->center );
            
            // calculate orientation
            VectorCopy( dir, v1 );
            v1[2] = 0;
            VectorNormalize( v1 );
            VectorSet( v2, 0, 0, 1 );
            VectorNormalize( v2 );
            CrossProduct( v1, v2, cplate->ori );
            VectorNormalize( cplate->ori );
            
            // check if we hit something in one of each directions. continue if none or both are a hit. use cell size as reference
            bool dir1, dir2;
            dir1 = dir2 = false;
            
            VectorMA( cplate->center, COVER_PLATE_ORI_CHECK_DIST, cplate->ori, org );
            CG_Trace( &trace, cplate->center, NULL, NULL, org, -1, CONTENTS_SOLID );
            if( trace.entityNum == ENTITYNUM_WORLD )
                dir1 = true;
                
            VectorNegate( cplate->ori, cplate->ori );
            VectorMA( cplate->center, COVER_PLATE_ORI_CHECK_DIST, cplate->ori, org );
            CG_Trace( &trace, cplate->center, NULL, NULL, org, -1, CONTENTS_SOLID );
            if( trace.entityNum == ENTITYNUM_WORLD )
                dir2 = true;
                
            if( dir1 == dir2 )
                continue;
                
            if( dir1 )
                VectorNegate( cplate->ori, cplate->ori );
                
            // trace again to rule stuck or invalid edges out
            VectorMA( cplate->center, COVER_PLATE_ORI_CHECK_DIST, cplate->ori, org );
            CG_Trace( &trace, cplate->center, NULL, NULL, org, -1, CONTENTS_SOLID );
            
            if( trace.startsolid )
                continue;
                
            if( trace.allsolid )
                continue;
                
            if( ( trace.surfaceFlags & ( SURF_SKY | SURF_SLICK ) ) || ( trace.entityNum != ENTITYNUM_WORLD ) )
                continue;
                
                
            // slice it
            if( cplate->len > COVER_PLATES_MAX_SLICE - 1 )
                continue;
                
            int steps = ( cplate->len ) / COVER_PLATES_SLICE_STEP;
            for( n = 0; n < steps; n ++ )
            {
            
                VectorMA( cplate->start, COVER_PLATES_SLICE_STEP * n, dir, cplate->sl[cplate->nsl].pos );
                cplate->sl[cplate->nsl].height = 0;
                
                for( k = 0; k < ( abs( playerMins[2] - playerMaxs[2] ) / COVER_PLATES_SLICE_HEIGHT_STEP ) ; k++ ) // check height. We need at least crouch height
                {
                    // trace alongside the orientation if we hit something.
                    
                    VectorMA( cplate->sl[cplate->nsl].pos, COVER_PLATE_ORI_CHECK_DIST, cplate->ori, org );
                    CG_Trace( &trace, cplate->sl[cplate->nsl].pos, NULL, NULL, org, -1, CONTENTS_SOLID );
                    
                    if( trace.entityNum != ENTITYNUM_WORLD ) // nothing hit, go back...
                    {
                        cplate->sl[cplate->nsl].height -= COVER_PLATES_SLICE_HEIGHT_STEP;
                        cplate->sl[cplate->nsl].pos[2] -= COVER_PLATES_SLICE_HEIGHT_STEP;
                        break;
                    }
                    cplate->sl[cplate->nsl].height += COVER_PLATES_SLICE_HEIGHT_STEP;
                    cplate->sl[cplate->nsl].pos[2] += COVER_PLATES_SLICE_HEIGHT_STEP;
                    
                }
                
                if( cplate->sl[cplate->nsl].height < CROUCH_HEIGHT ) // we need at least the crouch height
                    continue;
                    
                cplate->nsl++;
            }
            
            // do we have the minimum slices to cover the players body? ( asume a=b)
            if( cplate->nsl < ( ( abs( playerMins[0] - playerMaxs[0] ) ) / COVER_PLATES_SLICE_STEP ) )
                continue;
                
            // ok, add it!
            cg.nv_npcovers++;
        }
    }
    
    
    
    
    
    
    Com_Printf( " %i found  \n", cg.nv_npcovers );
    
    
    
    
    /*
    	VectorSubtract(cplate->end, cplate->start, dir);
    	VectorNormalize(dir);
    
    	VectorMA(cplate->start, cg.nv_info.cellSize, dir, cplate->start);
    	VectorMA(cplate->end, -cg.nv_info.cellSize, dir, cplate->end);
    
    
    	// check if stuck in solid
    	CG_Trace(&trace, cplate->start, NULL, NULL, cplate->end, -1, CONTENTS_SOLID);
    	if(trace.allsolid)
    		continue;
    */
    
    /*
    
    			// ok, build points from edges
    
    
    			// get first point
    			v = &cg.nv_verts[vi[0]*3];
    			VectorSet(org, v[0]*cg.nv_info.cellSize, (v[1]+1)*cg.nv_info.cellHeight, v[2]*cg.nv_info.cellSize );
    			recast2quake(org, cplate->points[0]);
    			cplate->points[0][0] += -cg.nv_info.mins[0];
    			cplate->points[0][1] += -cg.nv_info.mins[2];
    			cplate->points[0][2] += cg.nv_info.mins[1];
    				// make sure to drop the point to the ground!
    				VectorCopy(cplate->points[0], org);
    				org[2] -= 9999.0f;
    				CG_Trace(&trace, cplate->points[0], NULL, NULL, org, -1, CONTENTS_SOLID);
    				VectorCopy(trace.endpos, cplate->points[0]);
    			// create second
    			VectorCopy(cplate->points[0], cplate->points[1]);
    			cplate->points[1][2] += 64.0f;
    
    			// get third
    			v = &cg.nv_verts[vi[1]*3];
    			VectorSet(org, v[0]*cg.nv_info.cellSize, (v[1]+1)*cg.nv_info.cellHeight, v[2]*cg.nv_info.cellSize );
    			recast2quake(org, cplate->points[2]);
    			cplate->points[2][0] += -cg.nv_info.mins[0];
    			cplate->points[2][1] += -cg.nv_info.mins[2];
    			cplate->points[2][2] += cg.nv_info.mins[1];
    				// make sure to drop the point to the ground!
    				VectorCopy(cplate->points[2], org);
    				org[2] -= 9999.0f;
    				CG_Trace(&trace, cplate->points[2], NULL, NULL, org, -1, CONTENTS_SOLID);
    				VectorCopy(trace.endpos, cplate->points[2]);
    
    			// create fourth
    			VectorCopy(cplate->points[2], cplate->points[3]);
    			cplate->points[3][2] += 64.0f;
    
    */
    
    
    /* COVER POINTS get all outer edges of the navmesh. each edge is a potential
     * candidate for dropping cover waypoints. I prefer this solution combined with
     * cover height detection. Cover plates, as used in modern games instead,
     * are rasing some other problems ( hitbox problem )
    */
    
    Com_Printf( "	calculating cover plates..." );
    cg.nv_ncovers = 0;
    
    for( i = 0; i < cg.nv_npolys; i++ )
    {
        const unsigned short* p = &cg.nv_polys[i * cg.nv_nvp * 2];
        for( j = 0; j < cg.nv_nvp; j++ )
        {
            if( p[j] == RC_MESH_NULL_IDX ) break;
            if( p[cg.nv_nvp + j] & 0x8000 )
            {
            
            }
            else
            {
                continue;
            }
            cover_t* cover;
            const int nj = ( j + 1 >= cg.nv_nvp || p[j + 1] == RC_MESH_NULL_IDX ) ? 0 : j + 1;
            unsigned short vi[2] = {p[j], p[nj]};
            
            cover = &cg.nv_covers[cg.nv_ncovers];
            memset( cover, 0, sizeof( cover_t ) );
            
            // construct cover points...
            vec3_t o, start, end;
            const unsigned short* v;
            
            v = &cg.nv_verts[vi[0] * 3];
            VectorSet( o, v[0]*cg.nv_info.cellSize, ( v[1] + 1 )*cg.nv_info.cellHeight, v[2]*cg.nv_info.cellSize );
            recast2quake( o, start );
            start[0] += -cg.nv_info.mins[0];
            start[1] += -cg.nv_info.mins[2];
            start[2] += cg.nv_info.mins[1];
            // make sure to drop the point to the ground!
            VectorCopy( start, org );
            org[2] -= 9999.0f;
            CG_Trace( &trace, start, NULL, NULL, org, -1, CONTENTS_SOLID );
            VectorCopy( trace.endpos, start );
            start[2] += 2;
            
            
            v = &cg.nv_verts[vi[1] * 3];
            VectorSet( o, v[0]*cg.nv_info.cellSize, ( v[1] + 1 )*cg.nv_info.cellHeight, v[2]*cg.nv_info.cellSize );
            recast2quake( o, end );
            end[0] += -cg.nv_info.mins[0];
            end[1] += -cg.nv_info.mins[2];
            end[2] += cg.nv_info.mins[1];
            // make sure to drop the point to the ground!
            VectorCopy( end, org );
            org[2] -= 9999.0f;
            CG_Trace( &trace, end, NULL, NULL, org, -1, CONTENTS_SOLID );
            VectorCopy( trace.endpos, end );
            end[2] += 2;
            
            // check if stuck in solid
            CG_Trace( &trace, start, NULL, NULL, end, -1, CONTENTS_SOLID );
            if( trace.allsolid )
                continue;
                
            // check length of the edge. dont build cover point if to small
            VectorSubtract( end, start, dir );
            len = VectorLength( dir );
            
            if( len <= ( COVER_MIN_EDGE ) )
                continue;
                
            len *= 0.5f;
            
            VectorNormalize( dir );
            VectorMA( start, len, dir, center );
            
            // check for a bad normal. Lets see how 0.8 works
            VectorCopy( center, org );
            org[2] -= 9999.0f;
            CG_Trace( &trace, center, NULL, NULL, org, -1, CONTENTS_SOLID );
            if( trace.plane.normal[2] <= 0.8f )
                continue;
                
            // check if stuck in solid
            if( trace.startsolid )
                continue;
                
            VectorCopy( center, cover->point );
            cover->point[2] += COVER_HEIGHT;
            
            // test if this point can give any cover
            bool 	skip = true;
            
            // compute variables
            VectorSubtract( cover->point, center, vec );
            len = VectorNormalize( vec );
            
            // compute side vector
            VectorSubtract( start, cover->point, v1 );
            VectorNormalize( v1 );
            VectorSubtract( end, cover->point, v2 );
            VectorNormalize( v2 );
            CrossProduct( v1, v2, right );
            VectorNormalize( right );
            
            int num_dirs = 0;
            int start_dir = -1;
            
            for( n = 0; n < COVER_ACCURACY; n++ )
            {
                vec3_t          temp;
                vec3_t			org;
                trace_t         trace;
                
                VectorMA( cover->point, COVER_DIST, right, org );
                
                CG_Trace( &trace, cover->point, NULL, NULL, org, -1, CONTENTS_SOLID );
                
                if( ( trace.fraction == 1.0 ) || ( trace.surfaceFlags & ( SURF_SKY | SURF_SLICK ) ) || ( trace.entityNum != ENTITYNUM_WORLD ) )
                {
                    // direction not suiteable for cover
                    VectorClear( cover->dir[n] );
                }
                else
                {
                    // direction suiteable for cover
                    VectorSubtract( org, cover->point, cover->dir[n] );
                    VectorNormalize( cover->dir[n] );
                    if( start_dir == -1 )
                        start_dir = n;
                    num_dirs++;
                    
                    skip = false;
                }
                
                RotatePointAroundVector( temp, vec, right, ( 360.f / COVER_ACCURACY ) );
                VectorCopy( temp, right );
            }
            // skip if only one coverdir has been found
            if( num_dirs <= 1 )
                continue;
                
            // if to close, skip
            for( k = 0; k < cg.nv_ncovers; k++ )
            {
                VectorSubtract( cg.nv_covers[k].point, cover->point, dir );
                len = VectorLength( dir );
                
                if( len < COVER_MIN_DIST )
                {
                    trace_t         trace;
                    
                    // visible to each other?
                    CG_Trace( &trace, cover->point, NULL, NULL, cg.nv_covers[k].point, -1, CONTENTS_SOLID );
                    if( trace.fraction == 1.0 )
                    {
                        skip = true;
                        break;
                    }
                }
            }
            
            if( skip )
                continue;
                
            // calculate angle and orientation
            VectorCopy( cover->dir[start_dir], cover->a );
            VectorCopy( cover->dir[( start_dir + num_dirs - 1 )], cover->b );
            
            cover->angle = AngleBetweenVectors( cover->a, cover->b );
            
            // skip if angle is to low
            if( cover->angle < 35.0f )
                continue;
                
            VectorAdd( cover->a, cover->b, cover->ori );
            VectorNormalize( cover->ori );
            
            // last check: make sure, the ori realy hits something
            VectorMA( cover->point, COVER_DIST, cover->ori, org );
            CG_Trace( &trace, cover->point, NULL, NULL, org, -1, CONTENTS_SOLID );
            if( trace.entityNum != ENTITYNUM_WORLD )
                continue;
                
            if( cg.nv_ncovers > COVER_MAX ) // god damn, to much coverpoints!
                break;
                
            cg.nv_ncovers++;
        }
    }
    
    Com_Printf( " %i found  \n", cg.nv_ncovers );
}

/*
==================
CG_CoverShutdown
==================
*/

void CG_CoverShutdown( void )
{

}



