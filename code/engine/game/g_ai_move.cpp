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

#include "g_local.h"


/*
================
AI_CanMove

very basic movement checking. Works fine.
================
*/

bool AI_CanMove( gentity_t* self, int direction )
{
    vec3_t          forward, right;
    vec3_t          offset, start, end;
    vec3_t          angles;
    trace_t         tr;
    
    // airborne
    if( self->s.groundEntityNum == ENTITYNUM_NONE )
        return true;
        
    // now check to see if move will move us off an edge
    VectorCopy( self->ai->as.angles, angles );
    
    if( direction == MOVE_LEFT )
        angles[YAW] += 90;
    else if( direction == MOVE_RIGHT )
        angles[YAW] -= 90;
    else if( direction == MOVE_BACK )
        angles[YAW] -= 180;
        
    // set up the vectors
    AngleVectors( angles, forward, right, NULL );
    
    VectorSet( offset, 36, 0, 24 );
    G_ProjectSource( self->s.origin, offset, forward, right, start );
    
    VectorSet( offset, 36, 0, -400 );
    G_ProjectSource( self->s.origin, offset, forward, right, end );
    
    trap_Trace( &tr, start, NULL, NULL, end, self->s.number, MASK_PLAYERSOLID );
    
//  if((tr.fraction > 0.3 && tr.fraction != 1) || (tr.contents & (CONTENTS_LAVA | CONTENTS_SLIME)))
    if( ( tr.fraction == 1.0 ) || ( tr.contents & ( CONTENTS_LAVA | CONTENTS_SLIME | CONTENTS_DONOTENTER ) ) )
    {
        return false;
    }
    
    return true;				// yup, can move
    
}

/*
================
AI_MoveToGoal

================
*/
void AI_MoveToGoal( gentity_t* self )
{

    // set bot's movement direction
    VectorSubtract( self->ai->as.moveTarget->s.origin, self->ai->as.origin, self->ai->as.moveVector );
    AI_ChangeAngle( self );
    
    if( AI_CanMove( self, MOVE_FORWARD ) )
        self->ai->as.cmd.forwardmove = 127;
    return;
    
}
void AI_ChangeAngle( gentity_t* ent )
{
    vec3_t          ideal_angles;
    float           ideal_yaw;
    float           ideal_pitch;
    float           current_yaw;
    float           current_pitch;
    float           move;
    float           speed;
    
    // Normalize the move angle first
    VectorNormalize( ent->ai->as.moveVector );
    
    current_yaw = AngleNormalize360( ent->ai->as.angles[YAW] );
    current_pitch = AngleNormalize360( ent->ai->as.angles[PITCH] );
    
    VectorToAngles( ent->ai->as.moveVector, ideal_angles );
    
    ideal_yaw = AngleNormalize360( ideal_angles[YAW] );
    ideal_pitch = AngleNormalize360( ideal_angles[PITCH] );
    
    // yaw
    if( current_yaw != ideal_yaw )
    {
        move = ideal_yaw - current_yaw;
        speed = ent->ai->as.turnSpeed;
        if( ideal_yaw > current_yaw )
        {
            if( move >= 180 )
                move = move - 360;
        }
        else
        {
            if( move <= -180 )
                move = move + 360;
        }
        if( move > 0 )
        {
            if( move > speed )
                move = speed;
        }
        else
        {
            if( move < -speed )
                move = -speed;
        }
        ent->ai->as.angles[YAW] = AngleNormalize360( current_yaw + move );
    }
    
    // pitch
    if( current_pitch != ideal_pitch )
    {
        move = ideal_pitch - current_pitch;
        speed = ent->ai->as.turnSpeed;
        if( ideal_pitch > current_pitch )
        {
            if( move >= 180 )
                move = move - 360;
        }
        else
        {
            if( move <= -180 )
                move = move + 360;
        }
        if( move > 0 )
        {
            if( move > speed )
                move = speed;
        }
        else
        {
            if( move < -speed )
                move = -speed;
        }
        ent->ai->as.angles[PITCH] = AngleNormalize360( current_pitch + move );
    }
}


/*
================
AI_Move

Main movement code. (following node path)
================
*/

void AI_Move( gentity_t* self )
{
    int             currentNodeType = -1;
    int             nextNodeType = -1;
    
    // get current and next node back from nav code.
    if( !AI_Node_FollowPath( self ) )
    {
        self->ai->as.moveState = STATE_WANDER;
        self->ai->as.wander_timeout = level.time + 1000;
        
        // center view
        //self->ai->as.viewAngles[PITCH] = 0;   //-self->ai->as.delta_angles[PITCH];
        return;
    }
    
    currentNodeType = nodes[self->ai->as.currentNode].type;
    nextNodeType = nodes[self->ai->as.nextNode].type;
    
    // move to a selected goal, if any
    if( self->ai->as.moveTarget )
    {
        AI_MoveToGoal( self );
    }
    
    
    // falling off ledge?
    if( self->s.groundEntityNum == ENTITYNUM_NONE )
    {
        AI_ChangeAngle( self );
        
        //self->ai->as.velocity[0] = self->ai->as.moveVector[0] * 360;
        //self->ai->as.velocity[1] = self->ai->as.moveVector[1] * 360;
        return;
    }
    
    // check to see if stuck, and if so try to free us
    // also handles crouching
    if( VectorLength( self->ai->as.velocity ) < 37 )
    {
        // keep a random factor just in case....
//		if(random() > 0.1 && AI_SpecialMove(self))
//			return;

        self->ai->as.angles[YAW] += random() * 180 - 90;
        
        if( AI_CanMove( self, MOVE_FORWARD ) )
            self->ai->as.cmd.forwardmove = 127;
        else if( AI_CanMove( self, MOVE_BACK ) )
            self->ai->as.cmd.forwardmove = -127;
        return;
    }
    
    // otherwise move as fast as we can
    if( AI_CanMove( self, MOVE_FORWARD ) )
        self->ai->as.cmd.forwardmove = 127;
        
    AI_ChangeAngle( self );
}


/*
================
AI_Attack_Move

very basic movement based on attack states
================
*/

void AI_Attack_Move( gentity_t* self )
{
    float           c;
    vec3_t          target, forward, right, up;
    float           distance;
    vec3_t          oldAimVec;
    float           aimTremble[2] = { 0.11f, 0.11f };
    //float           slowness = 0.35;	//lower is slower
    
    // randomly choose a movement direction
    c = random();
    
    if( c < 0.2 && AI_CanMove( self, MOVE_LEFT ) )
        self->ai->as.cmd.rightmove -= 127;
    else if( c < 0.4 && AI_CanMove( self, MOVE_RIGHT ) )
        self->ai->as.cmd.rightmove += 127;
        
    if( c < 0.6 && AI_CanMove( self, MOVE_FORWARD ) )
        self->ai->as.cmd.forwardmove += 127;
        
    // dont run away
    //else if(c < 0.8 && AI_CanMove(self, MOVE_FORWARD))
    //	self->ai->as.cmd.forwardmove -= 127;
    
    // dont jump
    /*
    if(c < 0.95 && self->ai->as.weapon != WP_GAUNTLET)
    	self->ai->as.cmd.upmove -= 90;
    else
    	self->ai->as.cmd.upmove += 90;
    */
    
    // aim
    if( self->ai->enemy->client )
        VectorCopy( self->ai->enemy->client->ps.origin, target );
    else
        VectorCopy( self->ai->enemy->s.origin, target );
        
    // modify attack angles based on accuracy (mess this up to make the bot's aim not so deadly)
    
    // save the current angles
    VectorCopy( self->ai->as.moveVector, oldAimVec );
    VectorNormalize( oldAimVec );
    
    VectorSubtract( target, self->s.origin, forward );
    distance = VectorNormalize( forward );
    
    PerpendicularVector( up, forward );
    CrossProduct( up, forward, right );
    
    VectorMA( forward, crandom() * aimTremble[0], up, forward );
    VectorMA( forward, crandom() * aimTremble[1], right, forward );
    VectorNormalize( forward );
    
    //VectorLerp(oldAimVec, forward, slowness, forward);
    //VectorMA(oldAimVec, slowness, forward, forward);
    //VectorNormalize(forward);
    
    VectorScale( forward, distance, self->ai->as.moveVector );
    //ACEMV_ChangeBotAngle(self);
    VectorToAngles( self->ai->as.moveVector, self->ai->as.angles );
    
    
    // don't attack too much, and only if in range
    if( random() < 0.8 && AI_CheckShot( self, target ) )
    {
        Com_Printf( "AI_Attack_Move: Attack!\n" );
        self->ai->as.cmd.buttons = BUTTON_ATTACK;
        
    }
    
}

