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
// cg_debug.c - general cgame debugging tools


#include "cg_local.h"


#define LINE_WIDTH 0.8f

void CG_DebugDrawNumber( vec3_t pos , int num )
{
    refEntity_t     ent;
    vec3_t          delta, origin, dir, vec, up = { 0, 0, 1 };
    float           len;
    int             i, digits[10], numdigits, negative;
    int             numberSize = 8;
    
    VectorCopy( pos, origin );
    origin[2] += 5;
    
    // draw node number as sprite
    // code based on CG_AddScorePlum
    
    ent.reType = RT_SPRITE;
    ent.radius = 5;
    
    ent.shaderRGBA[0] = 0xff;
    ent.shaderRGBA[1] = 0xff;
    ent.shaderRGBA[2] = 0xff;
    ent.shaderRGBA[3] = 0xff;
    
    VectorCopy( origin, ent.origin );
    VectorCopy( origin, ent.oldorigin );
    
    VectorSubtract( cg.refdef.vieworg, origin, dir );
    CrossProduct( dir, up, vec );
    VectorNormalize( vec );
    
    // if the view would be "inside" the sprite, kill the sprite
    // so it doesn't add too much overdraw
    VectorSubtract( origin, cg.refdef.vieworg, delta );
    len = VectorLength( delta );
    if( len < 20 )
    {
        return;
    }
    
    negative = false;
    if( num < 0 )
    {
        negative = true;
        num = -num;
    }
    
    for( numdigits = 0; !( numdigits && !num ); numdigits++ )
    {
        digits[numdigits] = num % 10;
        num = num / 10;
    }
    
    if( negative )
    {
        digits[numdigits] = 10;
        numdigits++;
    }
    
    for( i = 0; i < numdigits; i++ )
    {
        VectorMA( origin, ( float )( ( ( float )numdigits / 2 ) - i ) * numberSize, vec, ent.origin );
        ent.customShader = cgs.media.numberShaders[digits[numdigits - 1 - i]];
        trap_R_AddRefEntityToScene( &ent );
    }
    
}

void CG_DebugDrawPoint( vec3_t pos, int radius )
{
    refEntity_t     ent;
    vec3_t origin;
    
    VectorCopy( pos, origin );
    origin[2] += 5;
    
    memset( &ent, 0, sizeof( ent ) );
    
    // set frame
    VectorCopy( origin, ent.origin );
    VectorCopy( origin, ent.oldorigin );
    
    ent.reType = RT_SPRITE;
    ent.rotation = 0;
    ent.radius = radius;
    ent.customShader = cgs.media.plasmaBallShader;
    
    trap_R_AddRefEntityToScene( &ent );
    
}

void CG_DebugDrawLine( vec3_t start, vec3_t end, vec4_t color )
{
    vec3_t up;
    polyVert_t      verts[4];
    
    VectorSet( up, 0, 0, 1 );
    
    VectorMA( start, LINE_WIDTH, up, verts[0].xyz );
    verts[0].st[0] = 0;
    verts[0].st[1] = 1;
    
    verts[0].modulate[0] = color[0] * 255;
    verts[0].modulate[1] = color[1] * 255;
    verts[0].modulate[2] = color[2] * 255;
    verts[0].modulate[3] = color[3] * 255;
    
    VectorMA( start, -LINE_WIDTH, up, verts[1].xyz );
    verts[1].st[0] = 1;
    verts[1].st[1] = 0;
    verts[1].modulate[0] = color[0] * 255;
    verts[1].modulate[1] = color[1] * 255;
    verts[1].modulate[2] = color[2] * 255;
    verts[1].modulate[3] = color[3] * 255;
    
    VectorMA( end, -LINE_WIDTH, up, verts[2].xyz );
    verts[2].st[0] = 1;
    verts[2].st[1] = 1;
    verts[2].modulate[0] = color[0] * 255;;
    verts[2].modulate[1] = color[1] * 255;;
    verts[2].modulate[2] = color[2] * 255;;
    verts[2].modulate[3] = color[3] * 255;;
    
    VectorMA( end, LINE_WIDTH, up, verts[3].xyz );
    verts[3].st[0] = 0;
    verts[3].st[1] = 0;
    verts[3].modulate[0] = color[0] * 255;;
    verts[3].modulate[1] = color[1] * 255;;
    verts[3].modulate[2] = color[2] * 255;;
    verts[3].modulate[3] = color[3] * 255;;
    
    trap_R_AddPolyToScene( cgs.media.whiteShader, 4, verts );
    
}


void CG_DebugDrawVert( polyVert_t verts[3], vec4_t color )
{
    int             i;
    
    // set the polygon's texture coordinates
    verts[0].st[0] = 1;
    verts[0].st[1] = 0;
    verts[1].st[0] = 0;
    verts[1].st[1] = 0;
    verts[2].st[0] = 0;
    verts[2].st[1] = 1;
    
    // set the polygon's vertex colors
    for( i = 0; i < 3; i++ )
    {
        verts[i].modulate[0] = color[0] * 255;
        verts[i].modulate[1] = color[1] * 255;
        verts[i].modulate[2] = color[2] * 255;
        verts[i].modulate[3] = color[3] * 120;
    }
    
    trap_R_AddPolyToScene( cgs.media.whiteShader, 3, verts );
}

void CG_DebugDrawVert4( polyVert_t verts[4], vec4_t color )
{
    int             i;
    
    // set the polygon's texture coordinates
    verts[0].st[0] = 0;
    verts[0].st[1] = 0;
    verts[1].st[0] = 0;
    verts[1].st[1] = 1;
    verts[2].st[0] = 1;
    verts[2].st[1] = 1;
    verts[3].st[0] = 1;
    verts[3].st[1] = 0;
    
    // set the polygon's vertex colors
    for( i = 0; i < 4; i++ )
    {
        verts[i].modulate[0] = color[0] * 255;
        verts[i].modulate[1] = color[1] * 255;
        verts[i].modulate[2] = color[2] * 255;
        verts[i].modulate[3] = color[3] * 120;
    }
    
    trap_R_AddPolyToScene( cgs.media.whiteShader, 4, verts );
}


