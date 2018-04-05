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
//

/*
    The Tremulous AI code is an approach for evolutionary AI. That means, the AI is
    making decisions, beeing rewared on success. The AI is learning via trial-and-error.

    To calibrate the AI we need a very basic stupid AI. This basic AI will:
        - move randomly across a map
        - attack any other AI which is visible and in shooting range
        - maintain shooting range

    The basic AI is hardcoded into the gamecode and very similar to the ACEBOT. The advanced AI
    must be able to compete the basic AI, even with a weaker weapon or lower health. The advanced AI
    must be able to evaluate its decicions, choosen paths and usings of elements in the world.

    The AI is aware of the world's layout via a simple node mesh, representated via single nodes
    in the world.



*/

#include "g_local.h"

static int num_ai;
gai_t ai_list[MAX_AI];

/*
================
AI_StartFrame


this should:

- rebuild the node table / node nav mesh
- re-evaluate weight of nodes
- re-evaluate entitys / objective weights
- run on a specific frame rate ( 10 fps ? ) with constant time

================
*/

int AI_StartFrame( int time )
{
    //G_Printf("AI_StartFrame()\n");
    
    //ACEIT_BuildItemNodeTable(true);
    
    return 0;
}

/*
================
AI_Register

create AI

this should:

- create the aiState

================
*/

void AI_Register( gentity_t* self )
{

    Com_Printf( "AI_Register: registering...\n" );
    // flash the ai state
    if( self->ai )
    {
        Com_Printf( "AI_Register Error: AI state present!\n" );
        return;
    }
    
    if( num_ai > MAX_AI ) //TODO
    {
        Com_Printf( "AI_Register Error: to many AIs', > %i!\n", MAX_AI );
        return;
    }
    
    Com_Printf( "AI_Register: done!\n" );
    
    memset( &ai_list[num_ai], 0, sizeof( gai_t ) );
    self->ai = &ai_list[num_ai];
    num_ai++;
    
    Com_Printf( "AI_Register: done(%i)!\n", num_ai );
    
}

/*
================
AI_Spawn

spawn ai in game
================
*/

void AI_Spawn( gentity_t* self )
{
    trace_t		tr;
    vec3_t		dest;
    
    return;
    
    if( !self->ai )
    {
        Com_Printf( "AI_Spawn: can't spawn non-ai entity!\n" );
        G_FreeEntity( self );
        return;
    }
    
    
    Com_Printf( "AI_Spawn: %s ORIGIN:  %s\n", self->classname, vtos( self->s.origin ) );
    Com_Printf( "AI_Spawn: %s ANGLES:  %s\n", self->classname, vtos( self->s.angles ) );
    
    Com_Printf( "AI_Spawn: setting defaults...\n" );
    
    // set the defaults
    self->ai->health = 100;
    self->ai->as.moveState = STATE_WANDER;
    
    self->ai->as.viewheight = 32;
    self->ai->as.speed = 600;
    self->ai->as.turnSpeed = 15;
    self->ai->as.gravity = 800;
    
    self->ai->as.num = self->s.number;
    
    // TODO:
    self->s.modelindex = G_ModelIndex( "models/ank.md3" );
    VectorSet( self->r.mins, -16, -24, -24 );
    VectorSet( self->r.maxs, 16, 24, 40 );
    
    Com_Printf( "AI_Spawn: linking into world...\n" );
    
    // drop to floor
    VectorSet( dest, self->s.origin[0], self->s.origin[1], self->s.origin[2] - 4096 );
    trap_Trace( &tr, self->s.origin, self->r.mins, self->r.maxs, dest, self->s.number, MASK_SOLID );
    if( tr.startsolid )
    {
        Com_Printf( "AI_Spawn: %s startsolid at %s\n", self->classname, vtos( self->s.origin ) );
        G_FreeEntity( self );
        return;
    }
    
    // allow to ride movers
    self->s.groundEntityNum = tr.entityNum;
    self->ai->as.groundEntityNum = tr.entityNum;
    
    G_SetOrigin( self, tr.endpos );
    
    
    
    self->s.eType = ET_AI;
    //self->takedamage = true;
    self->r.contents = CONTENTS_BODY;
    self->clipmask = MASK_PLAYERSOLID;
    
    
    
    // set up movement
    /*
            VectorCopy( self->s.angles, self->s.apos.trBase );
            VectorCopy( self->s.origin, self->s.pos.trBase );
            VectorClear(self->s.pos.trDelta );
    
    
            VectorCopy( self->s.angles, self->ai->as.angles );
            VectorCopy( self->s.origin, self->ai->as.origin );
            VectorClear( self->ai->as.velocity );
            */
    
    VectorCopy( self->s.pos.trBase, self->ai->as.origin );
    VectorCopy( self->s.pos.trDelta, self->ai->as.velocity );
    
    trap_LinkEntity( self );
    
    Com_Printf( "AI_Spawn: done!\n" );
    
    
}


/*
================
AI_Think

main AI thinking
================
*/

void AI_Think( gentity_t* self )
{

    int i;
    aipmove_t	pm;
    gai_t*	ai;
    
    
    G_RunThink( self );
    
    // im not an ai
    if( !self->ai )
        return;
        
    ai = self->ai;
    
    ai->enemy = NULL;
    ai->as.moveTarget = NULL;
    
    // set up movement
    
    //VectorCopy( self->s.angles, ai->as.angles );
    //VectorCopy( self->s.origin, ai->as.origin );
    //VectorClear( ai->as.velocity );
    
    memset( &ai->as.cmd, 0, sizeof( usercmd_t ) );
    ai->as.cmd.serverTime = level.time;
    
    
    // am i dead?
    if( ai->health <= 0 )
        return;
        
    // select a random node as long range goal.
    if( ai->as.moveState == STATE_WANDER && ai->as.wander_timeout < level.time )
    {
//	    Com_Printf("AI: i am wandering, selecting new long range goal!\n");
        AI_Think_PickLongRangeGoal( self );
    }
    
    
    /*
        The basic AI should not pick any short range goals,
        except near by nodes selected randomly
    */
    // find any short range goal
    
    /*
    ACEAI_PickShortRangeGoal(self);
    */
    
    
    // find us an enemy
    /*	if(AI_FindEnemy(self))
    	{
    
    
    	    // We got an enemy. All movement is now done by the attack code.
    
    
    	    // ACEAI_ChooseWeapon(self);
            AI_Attack_Move(self);
    
    	}
    	else
    	{
    
    */
    //   We got no enemy. All movement is done by path code.
    
    
    
    // execute the move, or wander
    
    if( self->ai->as.moveState == STATE_WANDER )
    {
        //AI_Wander(self);
    }
    else if( self->ai->as.moveState == STATE_MOVE )
    {
        AI_Move( self );
        
    }
    
    
//	}


    for( i = 0; i < 3; i++ )
    {
        ai->as.cmd.angles[i] = ANGLE2SHORT( ai->as.angles[i] );
    }
    
    // do the moving
    
    memset( &pm, 0, sizeof( pm ) );
    
    
    pm.as = &ai->as;
    pm.cmd = ai->as.cmd;
    
    VectorCopy( self->r.mins, pm.mins );
    VectorCopy( self->r.maxs, pm.maxs );
    
    pm.trace = trap_Trace;
    pm.pointcontents = trap_PointContents;
    
    AI_PMove( &pm );
    
    self->s.eType = ET_AI;
    
    self->s.pos.trType = TR_INTERPOLATE;
    VectorCopy( ai->as.origin, self->s.pos.trBase );
    
    SnapVector( self->s.pos.trBase );
    
    // set the trDelta for flag direction
    VectorCopy( ai->as.velocity, self->s.pos.trDelta );
    
    self->s.apos.trType = TR_INTERPOLATE;
    
    VectorCopy( ai->as.angles, self->s.apos.trBase );
    
    SnapVector( self->s.apos.trBase );
    
    
    //self->s.eFlags = ai->as.eFlags;
    
    self->s.groundEntityNum = ai->as.groundEntityNum;
    
    VectorCopy( self->s.pos.trBase, self->s.origin );
    
//	ent->waterlevel = pm.waterlevel;
//	ent->watertype = pm.watertype;

//	NPCEvents( ent, oldEventSequence );		// EVENTS!!??

    trap_LinkEntity( self );
    
//	if ( !ent->client->noclip ) {			// TOUCH TRIGGERS??
//		G_TouchTriggers( ent );
//	}


    VectorCopy( ai->as.origin, self->r.currentOrigin );
    
    
    trap_LinkEntity( self );
    //Com_Printf("I am at %s\n", vtos(self->s.origin ));
    
}


/*
================
AI_Init

init ai states
================
*/

void AI_Init( void )
{

    return;
    
    Com_Printf( "AI INIT!\n" );
    num_ai = 0;
    // build node mesh
    AI_Node_Init();
    
}


/*
================
AI_Shutdown

================
*/

void AI_Shutdown( void )
{

    Com_Printf( "AI SHUTDOWN!\n" );
    num_ai = 0;
    // node mesh, save nodes to file
    AI_Node_Shutdown();
    
}

/*
================
SP_AI

game ai spawn
================
*/

void SP_AI( gentity_t* ent )
{
    float fa;
    
    
    AI_Register( ent );
    
    
    ent->nextthink = level.time + FRAMETIME * 2;
    ent->think = AI_Spawn;
    
    
    VectorCopy( ent->s.angles, ent->s.apos.trBase );
}






