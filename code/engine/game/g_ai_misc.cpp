/*
===========================================================================
Copyright (C) 2011 Adrian Fuhrmann <aliasng@gmail.com>

This file is part of Tremulous source code.

Tremulous source code is free software; you can redistribute it
and/or modify it under the terms of the GNU General Public License as
published by the Free Software Foundation; either version 2 of the License,
or (at your option) any later version.

Tremulous source code is distributed in the hope that it will be
useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Tremulous source code; if not, write to the Free Software
Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
===========================================================================
*/


#include "g_local.h"

/*
================
AI_CheckShot

checks if we can shoot ( range, visibility ).
================
*/

bool AI_CheckShot( gentity_t* self, vec3_t point )
{
    trace_t         tr;
    vec3_t          start, forward, right, offset;
    
    if( self->ai->enemyRange > 150 ) // to far
        return false;
        
    AngleVectors( self->ai->as.angles, forward, right, NULL );
    
    VectorSet( offset, 8, 8, self->ai->as.viewheight - 8 );
    //G_ProjectSource(self->ai->as.origin, offset, forward, right, start);
    G_ProjectSource( self->s.origin, offset, forward, right, start );
    
    // blocked, don't shoot
    trap_Trace( &tr, start, NULL, NULL, point, self->s.number, MASK_SOLID );
    if( tr.fraction < 0.3 )		//just enough to prevent self damage (by now)
        return false;
        
    return true;
}

/*
=============
AI_InFront

returns 1 if the entity is in front (in sight) of self
=============
*/
bool AI_InFront( gentity_t* self, gentity_t* other )
{
    vec3_t          vec;
    float           angle;
    vec3_t          forward;
    
    AngleVectors( self->ai->as.angles, forward, NULL, NULL );
    //AngleVectors(self->s.angles, forward, NULL, NULL);
    VectorSubtract( other->s.origin, self->s.origin, vec );
    
    VectorNormalize( vec );
    angle = AngleBetweenVectors( vec, forward );
    //Com_Printf("Angle to enemy: %f\n", angle);
    if( angle <= 85 )
        return true;
        
    return false;
}


/*
=============
AI_Visible

returns 1 if the entity is visible to self, even if not infront ()
=============
*/
bool AI_Visible( gentity_t* self, gentity_t* other )
{
    vec3_t          spot1;
    vec3_t          spot2;
    trace_t         trace;
    
    //if(!self->client || !other->client)
    //  return false;
    
    //VectorCopy(self->ai->as.origin, spot1);
    VectorCopy( self->s.origin, spot1 );
    //spot1[2] += self->client->ps.viewheight;
    if( other->client )
    {
        VectorCopy( other->client->ps.origin, spot2 );
        //spot2[2] += other->client->ps.viewheight;
        
    }/*else if(other->ai) {
        VectorCopy(other->ai->as.origin, spot2);
    }else{
        return false;
    }*/

    trap_Trace( &trace, spot1, NULL, NULL, spot2, self->s.number, MASK_PLAYERSOLID );
    
    if( trace.entityNum == other->s.number )
        return true;
        
    return false;
}

/*
================
AI_FindEnemy

Pick a visible enemy

TODO: client only for now

================
*/


bool AI_FindEnemy( gentity_t* self )
{
    int             i;
    gclient_t*      cl;
    gentity_t*      enemy = NULL;
    float           enemyRange;
    float           bestRange = 99999;
    
    
    //Com_Printf("AI_FindEnemy: picking enemy...\n");
    
    for( i = 0; i < level.numConnectedClients; i++ )
    {
        cl = level.clients + i;
        enemy = level.gentities + cl->ps.clientNum;
        
        if( cl->pers.connected != CON_CONNECTED )
            continue;
            
        if( enemy->health <= 0 )
            continue;
            
        if( enemy->client->noclip || enemy->client->ps.pm_type == PM_NOCLIP )
            continue;
            
            
        //enemyRange = Distance(self->ai->as.origin, enemy->client->ps.origin);
        enemyRange = Distance( self->s.origin, enemy->client->ps.origin );
        
        if( enemyRange > bestRange )
            continue;
            
        if( !trap_InPVS( self->s.origin, enemy->client->ps.origin ) )
            continue;
            
        if( !AI_Visible( self, enemy ) )
            continue;
            
        if( !AI_InFront( self, enemy ) )
            continue;
            
        bestRange = enemyRange;
        
        self->ai->enemy = enemy;
        self->ai->enemyRange = ( int ) enemyRange;
        
    }
    
    
    if( self->ai->enemy )
    {
        //Com_Printf("AI_FindEnemy: enemy found: %s, range %f\n", self->ai->enemy->client->pers.netname, enemyRange);
        return true;
    }
    //Com_Printf("AI_FindEnemy: no valid enemy\n");
    return false;
    
}


