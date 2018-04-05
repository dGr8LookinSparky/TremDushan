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

aipmove_t*		aipm;
aipml_t		aipml;

// movement parameters
float	aipm_stopspeed = 100.0f;
float	aipm_duckScale = 0.25f;
float	aipm_swimScale = 0.50f;
float	aipm_wadeScale = 0.70f;

float	aipm_accelerate = 15.0f;
float	aipm_airaccelerate = 1.0f;
float	aipm_wateraccelerate = 4.0f;
float	aipm_flyaccelerate = 8.0f;

float	aipm_friction = 8.0f;
float	aipm_waterfriction = 1.0f;
float	aipm_flightfriction = 3.0f;
float	aipm_spectatorfriction = 5.0f;

int		ai_pmove = 0;



#define	MAX_CLIP_PLANES	5
#define	OVERCLIP		1.001f
#define	MIN_WALK_NORMAL	0.7f		// can't walk on very steep slopes

#define	STEPSIZE		18


/*
==================
AI_PMoveFriction

Handles both ground friction and water friction
==================
*/
static void AI_PMoveFriction( void )
{
    vec3_t	vec;
    float*	vel;
    float	speed, newspeed, control;
    float	drop;
    
    vel = aipm->as->velocity;
    
    VectorCopy( vel, vec );
    if( aipml.walking )
    {
        vec[2] = 0;	// ignore slope movement
    }
    
    speed = VectorLength( vec );
    if( speed < 1 )
    {
        vel[0] = 0;
        vel[1] = 0;		// allow sinking underwater
        // FIXME: still have z friction underwater?
        return;
    }
    
    drop = 0;
    
    // apply ground friction
    //if ( aipm->waterlevel <= 1 ) {
    if( aipml.walking && !( aipml.groundTrace.surfaceFlags & SURF_SLICK ) )
    {
        // if getting knocked back, no friction
        if( !( aipm->as->pm_flags & PMF_TIME_KNOCKBACK ) )
        {
            control = speed < aipm_stopspeed ? aipm_stopspeed : speed;
            drop += control * aipm_friction * aipml.frametime;
        }
    }
    //}
    /*
    	if (aipm->ns->inFlight)	// JPL - bat flying has "ground" friction
    	{
    		control = speed < aipm_stopspeed ? aipm_stopspeed : speed;
    		drop += control*aipm_friction*aipml.frametime;
    	}
    */
    /*
    	// apply water friction even if just wading
    	if ( aipm->waterlevel ) {
    		drop += speed*aipm_waterfriction*aipm->waterlevel*aipml.frametime;
    	}
    
    	// apply flying friction
    	if ( aipm->as->powerups[PW_FLIGHT]) {
    		drop += speed*aipm_flightfriction*aipml.frametime;
    	}
    
    	if ( aipm->as->pm_type == PM_SPECTATOR) {
    		drop += speed*aipm_spectatorfriction*aipml.frametime;
    	}
    */
    // scale the velocity
    newspeed = speed - drop;
    if( newspeed < 0 )
    {
        newspeed = 0;
    }
    newspeed /= speed;
    
    vel[0] = vel[0] * newspeed;
    vel[1] = vel[1] * newspeed;
    vel[2] = vel[2] * newspeed;
}



/*
============
AI_CmdScale

Returns the scale factor to apply to cmd movements
This allows the clients to use axial -127 to 127 values for all directions
without getting a sqrt(2) distortion in speed.
============
*/
static float AI_CmdScale( usercmd_t* cmd )
{
    int		max;
    float	total;
    float	scale;
    
    max = abs( cmd->forwardmove );
    if( abs( cmd->rightmove ) > max )
    {
        max = abs( cmd->rightmove );
    }
    if( abs( cmd->upmove ) > max )
    {
        max = abs( cmd->upmove );
    }
    if( !max )
    {
        return 0;
    }
    
    total = ( float )sqrt( ( float )( cmd->forwardmove * cmd->forwardmove + cmd->rightmove * cmd->rightmove + cmd->upmove * cmd->upmove ) );
    scale = ( float )aipm->as->speed * max / ( 127.0 * total );
    
    //if (aipm->ns->toFire==2) scale*=2.7;		// metlar jump attack!
    
    return scale;
}

/*
==================
AI_PMoveClipVelocity

Slide off of the impacting surface
==================
*/
void AI_PMoveClipVelocity( vec3_t in, vec3_t normal, vec3_t out, float overbounce )
{
    float	backoff;
    float	change;
    int		i;
    
    backoff = DotProduct( in, normal );
    
    if( backoff < 0 )
    {
        backoff *= overbounce;
    }
    else
    {
        backoff /= overbounce;
    }
    
    for( i = 0 ; i < 3 ; i++ )
    {
        change = normal[i] * backoff;
        out[i] = in[i] - change;
    }
}

/*
==============
AI_PMove_Accelerate

Handles user intended acceleration
==============
*/
static void AI_PMove_Accelerate( vec3_t wishdir, float wishspeed, float accel )
{
    int			i;
    float		addspeed, accelspeed, currentspeed;
    
    currentspeed = DotProduct( aipm->as->velocity, wishdir );
    addspeed = wishspeed - currentspeed;
    if( addspeed <= 0 )
    {
        return;
    }
    accelspeed = accel * aipml.frametime * wishspeed;
    if( accelspeed > addspeed )
    {
        accelspeed = addspeed;
    }
    
    for( i = 0 ; i < 3 ; i++ )
    {
        aipm->as->velocity[i] += accelspeed * wishdir[i];
    }
}



/*
==================
AI_PMove_SlideMove

Returns true if the velocity was clipped in some way
==================
*/


bool	AI_PMove_SlideMove( bool gravity )
{
    int			bumpcount, numbumps;
    vec3_t		dir;
    float		d;
    int			numplanes;
    vec3_t		planes[MAX_CLIP_PLANES];
    vec3_t		primal_velocity;
    vec3_t		clipVelocity;
    int			i, j, k;
    trace_t	trace;
    vec3_t		end;
    float		time_left;
    float		into;
    vec3_t		endVelocity;
    vec3_t		endClipVelocity;
    
    numbumps = 4;
    
    VectorCopy( aipm->as->velocity, primal_velocity );
    
    if( gravity )
    {
        VectorCopy( aipm->as->velocity, endVelocity );
        endVelocity[2] -= aipm->as->gravity * aipml.frametime;
        aipm->as->velocity[2] = ( aipm->as->velocity[2] + endVelocity[2] ) * 0.5;
        primal_velocity[2] = endVelocity[2];
        if( aipml.groundPlane )
        {
            // slide along the ground plane
            AI_PMoveClipVelocity( aipm->as->velocity, aipml.groundTrace.plane.normal,
                                  aipm->as->velocity, OVERCLIP );
        }
    }
    
    time_left = aipml.frametime;
    
    // never turn against the ground plane
    if( aipml.groundPlane )
    {
        numplanes = 1;
        VectorCopy( aipml.groundTrace.plane.normal, planes[0] );
    }
    else
    {
        numplanes = 0;
    }
    
    // never turn against original velocity
    VectorNormalize2( aipm->as->velocity, planes[numplanes] );
    numplanes++;
    
    for( bumpcount = 0 ; bumpcount < numbumps ; bumpcount++ )
    {
    
        // calculate position we are trying to move to
        VectorMA( aipm->as->origin, time_left, aipm->as->velocity, end );
        
        // see if we can make it there
        aipm->trace( &trace, aipm->as->origin, aipm->mins, aipm->maxs, end, aipm->as->num, aipm->tracemask );
        
        if( trace.allsolid )
        {
            // entity is completely trapped in another solid
            aipm->as->velocity[2] = 0;	// don't build up falling damage, but allow sideways acceleration
            return true;
        }
        
        if( trace.fraction > 0 )
        {
            // actually covered some distance
            VectorCopy( trace.endpos, aipm->as->origin );
        }
        
        if( trace.fraction == 1 )
        {
            break;		// moved the entire distance
        }
        
        // save entity for contact
        //NPM_AddTouchEnt( trace.entityNum );
        Com_Printf( "AI_PMove_SlideMove: Touched ent %i\n", trace.entityNum );
        
        time_left -= time_left * trace.fraction;
        
        if( numplanes >= MAX_CLIP_PLANES )
        {
            // this shouldn't really happen
            VectorClear( aipm->as->velocity );
            return true;
        }
        
        //
        // if this is the same plane we hit before, nudge velocity
        // out along it, which fixes some epsilon issues with
        // non-axial planes
        //
        for( i = 0 ; i < numplanes ; i++ )
        {
            if( DotProduct( trace.plane.normal, planes[i] ) > 0.99 )
            {
                VectorAdd( trace.plane.normal, aipm->as->velocity, aipm->as->velocity );
                break;
            }
        }
        if( i < numplanes )
        {
            continue;
        }
        VectorCopy( trace.plane.normal, planes[numplanes] );
        numplanes++;
        
        //
        // modify velocity so it parallels all of the clip planes
        //
        
        // find a plane that it enters
        for( i = 0 ; i < numplanes ; i++ )
        {
            into = DotProduct( aipm->as->velocity, planes[i] );
            if( into >= 0.1 )
            {
                continue;		// move doesn't interact with the plane
            }
            
            // see how hard we are hitting things
            if( -into > aipml.impactSpeed )
            {
                aipml.impactSpeed = -into;
            }
            
            // slide along the plane
            AI_PMoveClipVelocity( aipm->as->velocity, planes[i], clipVelocity, OVERCLIP );
            
            // slide along the plane
            AI_PMoveClipVelocity( endVelocity, planes[i], endClipVelocity, OVERCLIP );
            
            // see if there is a second plane that the new move enters
            for( j = 0 ; j < numplanes ; j++ )
            {
                if( j == i )
                {
                    continue;
                }
                if( DotProduct( clipVelocity, planes[j] ) >= 0.1 )
                {
                    continue;		// move doesn't interact with the plane
                }
                
                // try clipping the move to the plane
                AI_PMoveClipVelocity( clipVelocity, planes[j], clipVelocity, OVERCLIP );
                AI_PMoveClipVelocity( endClipVelocity, planes[j], endClipVelocity, OVERCLIP );
                
                // see if it goes back into the first clip plane
                if( DotProduct( clipVelocity, planes[i] ) >= 0 )
                {
                    continue;
                }
                
                // slide the original velocity along the crease
                CrossProduct( planes[i], planes[j], dir );
                VectorNormalize( dir );
                d = DotProduct( dir, aipm->as->velocity );
                VectorScale( dir, d, clipVelocity );
                
                CrossProduct( planes[i], planes[j], dir );
                VectorNormalize( dir );
                d = DotProduct( dir, endVelocity );
                VectorScale( dir, d, endClipVelocity );
                
                // see if there is a third plane the the new move enters
                for( k = 0 ; k < numplanes ; k++ )
                {
                    if( k == i || k == j )
                    {
                        continue;
                    }
                    if( DotProduct( clipVelocity, planes[k] ) >= 0.1 )
                    {
                        continue;		// move doesn't interact with the plane
                    }
                    
                    // stop dead at a tripple plane interaction
                    VectorClear( aipm->as->velocity );
                    return true;
                }
            }
            
            // if we have fixed all interactions, try another move
            VectorCopy( clipVelocity, aipm->as->velocity );
            VectorCopy( endClipVelocity, endVelocity );
            break;
        }
    }
    
    if( gravity )
    {
        VectorCopy( endVelocity, aipm->as->velocity );
    }
    
    // don't change velocity if in a timer (FIXME: is this correct?)
    if( aipm->as->pm_time )
    {
        VectorCopy( primal_velocity, aipm->as->velocity );
    }
    
    return ( bumpcount != 0 );
}


/*
==================
AI_PMove_StepSlideMove

==================
*/
void AI_PMove_StepSlideMove( bool gravity )
{
    vec3_t		start_o, start_v;
    vec3_t		down_o, down_v;
    trace_t		trace;
//	float		down_dist, up_dist;
//	vec3_t		delta, delta2;
    vec3_t		up, down;
    
    VectorCopy( aipm->as->origin, start_o );
    VectorCopy( aipm->as->velocity, start_v );
    
    if( AI_PMove_SlideMove( gravity ) == 0 )
    {
        return;		// we got exactly where we wanted to go first try
    }
    
    VectorCopy( start_o, down );
    down[2] -= STEPSIZE;
    aipm->trace( &trace, start_o, aipm->mins, aipm->maxs, down, aipm->as->num, aipm->tracemask );
    VectorSet( up, 0, 0, 1 );
    // never step up when you still have up velocity
    if( aipm->as->velocity[2] > 0 && ( trace.fraction == 1.0 ||
                                       DotProduct( trace.plane.normal, up ) < 0.7 ) )
    {
        return;
    }
    
    VectorCopy( aipm->as->origin, down_o );
    VectorCopy( aipm->as->velocity, down_v );
    
    VectorCopy( start_o, up );
    up[2] += STEPSIZE;
    
    // test the player position if they were a stepheight higher
    aipm->trace( &trace, up, aipm->mins, aipm->maxs, up, aipm->as->num, aipm->tracemask );
    if( trace.allsolid )
    {
        //if ( aipm->debugLevel ) {
        Com_Printf( "%i:bend can't step\n", ai_pmove );
        //}
        return;		// can't step up
    }
    
    // try slidemove from this position
    VectorCopy( up, aipm->as->origin );
    VectorCopy( start_v, aipm->as->velocity );
    
    AI_PMove_SlideMove( gravity );
    
    // push down the final amount
    VectorCopy( aipm->as->origin, down );
    down[2] -= STEPSIZE;
    aipm->trace( &trace, aipm->as->origin, aipm->mins, aipm->maxs, down, aipm->as->num, aipm->tracemask );
    if( !trace.allsolid )
    {
        VectorCopy( trace.endpos, aipm->as->origin );
    }
    if( trace.fraction < 1.0 )
    {
        AI_PMoveClipVelocity( aipm->as->velocity, trace.plane.normal, aipm->as->velocity, OVERCLIP );
    }
    
    {
        // use the step move
        float	delta;
        
        delta = aipm->as->origin[2] - start_o[2];
        /*if ( delta > 2 ) {
        	if ( delta < 7 ) {
        		NPM_AddEvent( EV_STEP_4 );
        	} else if ( delta < 11 ) {
        		NPM_AddEvent( EV_STEP_8 );
        	} else if ( delta < 15 ) {
        		NPM_AddEvent( EV_STEP_12 );
        	} else {
        		NPM_AddEvent( EV_STEP_16 );
        	}
        }*/
        //if ( aipm->debugLevel ) {
        Com_Printf( "%i:stepped %f\n", ai_pmove, delta );
        //}
    }
    
}


/*
===================
AI_PMoveAir

===================
*/
static void AI_PMoveAir( void )
{
    int			i;
    vec3_t		wishvel;
    float		fmove, smove;
    vec3_t		wishdir;
    float		wishspeed;
    float		scale;
    usercmd_t	cmd;
    
    AI_PMoveFriction();
    
    fmove = aipm->cmd.forwardmove;
    smove = aipm->cmd.rightmove;
    
    cmd = aipm->cmd;
    scale = AI_CmdScale( &cmd );
    
    // set the movementDir so clients can rotate the legs for strafing
    //NPM_SetMovementDir();
    
    // project moves down to flat plane
    aipml.forward[2] = 0;
    aipml.right[2] = 0;
    VectorNormalize( aipml.forward );
    VectorNormalize( aipml.right );
    
    for( i = 0 ; i < 2 ; i++ )
    {
        wishvel[i] = aipml.forward[i] * fmove + aipml.right[i] * smove;
    }
    wishvel[2] = 0;
    
    VectorCopy( wishvel, wishdir );
    wishspeed = VectorNormalize( wishdir );
    wishspeed *= scale;
    
    // not on ground, so little effect on velocity
    if( cmd.upmove > 0 )
        AI_PMove_Accelerate( wishdir, wishspeed, aipm_accelerate );	// just jumped, do normal acceleration
    else
        AI_PMove_Accelerate( wishdir, wishspeed, aipm_airaccelerate );
        
    // we may have a ground plane that is very steep, even
    // though we don't have a groundentity
    // slide along the steep plane
    if( aipml.groundPlane )
    {
        AI_PMoveClipVelocity( aipm->as->velocity, aipml.groundTrace.plane.normal,
                              aipm->as->velocity, OVERCLIP );
    }
    /*
    	if (aipm->ns->toFire==2 && aipm->as->velocity[2]<0)
    	{
    		aipm->ps->velocity[2]=0;
    //		aipm->ps->velocity[1]*=1.1;
    //		aipm->ps->velocity[0]*=1.1;
    	}
    */
    AI_PMove_StepSlideMove( true );
}



/*
===================
AI_PMoveWalk

===================
*/
static void AI_PMoveWalk( void )
{
    int			i;
    vec3_t		wishvel;
    float		fmove, smove;
    vec3_t		wishdir;
    float		wishspeed;
    float		scale;
    usercmd_t	cmd;
    float		accelerate;
    float		vel;
    
    /*
    	if ( aipm->waterlevel > 2 && DotProduct( aipml.forward, aipml.groundTrace.plane.normal ) > 0 ) {
    		// begin swimming
    		NPM_WaterMove();
    		return;
    	}
    */
    /*
    	if ( NPM_CheckJump () ) {
    		// jumped away
    		if ( aipm->waterlevel > 1 ) {
    			NPM_WaterMove();
    		} else {
    			NPM_AirMove();
    		}
    		return;
    	}
    */
    AI_PMoveFriction();
    
    fmove = aipm->cmd.forwardmove;
    smove = aipm->cmd.rightmove;
    
    cmd = aipm->cmd;
    scale = AI_CmdScale( &cmd );
    
    // set the movementDir so clients can rotate the legs for strafing
    //NPM_SetMovementDir();
    //aipm->as->movementDir = 0;
    
    // project moves down to flat plane
    aipml.forward[2] = 0;
    aipml.right[2] = 0;
    
    // project the forward and right directions onto the ground plane
    AI_PMoveClipVelocity( aipml.forward, aipml.groundTrace.plane.normal, aipml.forward, OVERCLIP );
    AI_PMoveClipVelocity( aipml.right, aipml.groundTrace.plane.normal, aipml.right, OVERCLIP );
    //
    VectorNormalize( aipml.forward );
    VectorNormalize( aipml.right );
    
    for( i = 0 ; i < 3 ; i++ )
    {
        wishvel[i] = aipml.forward[i] * fmove + aipml.right[i] * smove;
    }
    // when going up or down slopes the wish velocity should Not be zero
//	wishvel[2] = 0;

    VectorCopy( wishvel, wishdir );
    wishspeed = VectorNormalize( wishdir );
    wishspeed *= scale;
    
    // clamp the speed lower if ducking
    if( aipm->as->pm_flags & PMF_DUCKED )
    {
        if( wishspeed > aipm->as->speed * aipm_duckScale )
        {
            wishspeed = aipm->as->speed * aipm_duckScale;
        }
    }
    
    // clamp the speed lower if wading or walking on the bottom
    /*	if ( aipm->waterlevel ) {
    		float	waterScale;
    
    		waterScale = aipm->waterlevel / 3.0;
    		waterScale = 1.0 - ( 1.0 - aipm_swimScale ) * waterScale;
    		if ( wishspeed > aipm->as->speed * waterScale ) {
    			wishspeed = aipm->as->speed * waterScale;
    		}
    	}
    */
    // when a player gets hit, they temporarily lose
    // full control, which allows them to be moved a bit
    if( ( aipml.groundTrace.surfaceFlags & SURF_SLICK ) || aipm->as->pm_flags & PMF_TIME_KNOCKBACK )
    {
        accelerate = aipm_airaccelerate;
    }
    else
    {
        accelerate = aipm_accelerate;
    }
    
    AI_PMove_Accelerate( wishdir, wishspeed, accelerate );
    
    //Com_Printf("velocity = %1.1f %1.1f %1.1f\n", pm->ps->velocity[0], pm->ps->velocity[1], pm->ps->velocity[2]);
    //Com_Printf("velocity1 = %1.1f\n", VectorLength(pm->ps->velocity));
    
    if( ( aipml.groundTrace.surfaceFlags & SURF_SLICK ) || aipm->as->pm_flags & PMF_TIME_KNOCKBACK )
    {
        aipm->as->velocity[2] -= aipm->as->gravity * aipml.frametime;
    }
    else
    {
        // don't reset the z velocity for slopes
//		pm->ps->velocity[2] = 0;
    }
    
    vel = VectorLength( aipm->as->velocity );
    
    // slide along the ground plane
    AI_PMoveClipVelocity( aipm->as->velocity, aipml.groundTrace.plane.normal,
                          aipm->as->velocity, OVERCLIP );
                          
    // don't decrease velocity when going up or down a slope
    VectorNormalize( aipm->as->velocity );
    VectorScale( aipm->as->velocity, vel, aipm->as->velocity );
    
    
    // don't do anything if standing still
    if( !aipm->as->velocity[0] && !aipm->as->velocity[1] )
    {
        return;
    }
    
    AI_PMove_StepSlideMove( false );
    
    //Com_Printf("velocity2 = %1.1f\n", VectorLength(pm->ps->velocity));
    
}


/*
=============
AI_PMove_CorrectAllSolid

=============
*/
static int AI_PMove_CorrectAllSolid( trace_t* trace )
{
    int			i, j, k;
    vec3_t		point;
    
    Com_Printf( "%i:allsolid\n", ai_pmove );
    
    
    // jitter around
    for( i = -1; i <= 1; i++ )
    {
        for( j = -1; j <= 1; j++ )
        {
            for( k = -1; k <= 1; k++ )
            {
                VectorCopy( aipm->as->origin, point );
                point[0] += ( float ) i;
                point[1] += ( float ) j;
                point[2] += ( float ) k;
                aipm->trace( trace, point, aipm->mins, aipm->maxs, point, aipm->as->num, aipm->tracemask );
                if( !trace->allsolid )
                {
                    point[0] = aipm->as->origin[0];
                    point[1] = aipm->as->origin[1];
                    point[2] = aipm->as->origin[2] - 0.25;
                    
                    aipm->trace( trace, aipm->as->origin, aipm->mins, aipm->maxs, point, aipm->as->num, aipm->tracemask );
                    aipml.groundTrace = *trace;
                    return true;
                }
            }
        }
    }
    
    aipm->as->groundEntityNum = ENTITYNUM_NONE;
    aipml.groundPlane = false;
    aipml.walking = false;
    
    return false;
}


/*
=============
AI_PMove_GroundTrace

=============
*/
static void AI_PMove_GroundTrace( void )
{
    vec3_t		point;
    trace_t		trace;
    
    point[0] = aipm->as->origin[0];
    point[1] = aipm->as->origin[1];
    point[2] = aipm->as->origin[2] - 0.25;
    //point[2] = aipm->as->origin[2] - 64;
    
    //Com_Printf("Tracing from %s to %s for %i\n", vtos(aipm->as->origin), vtos(point), aipm->as->num);
    //aipm->trace (&trace, aipm->as->origin, aipm->mins, aipm->maxs, point, aipm->as->num, aipm->tracemask);
    aipm->trace( &trace, aipm->as->origin, aipm->mins, aipm->maxs, point, aipm->as->num, MASK_SOLID );
    
    //trap_Trace(&trace, aipm->as->origin, NULL, NULL, point, aipm->as->num, MASK_SHOT);
    
    
    aipml.groundTrace = trace;
    
    // do something corrective if the trace starts in a solid...
    if( trace.allsolid )
    {
        Com_Printf( "Start all solid!\n" );
        if( !AI_PMove_CorrectAllSolid( &trace ) )
            return;
    }
    
    // if the trace didn't hit anything, we are in free fall
    if( trace.fraction == 1.0 )
    {
        //NPM_GroundTraceMissed();
        //Com_Printf("Free fall!\n");
        aipml.groundPlane = false;
        aipml.walking = false;
        return;
    }
    
    // do not allow to stand over another NPC or a player
    /*
    if (aipml.can_do_back_move &&			// only once
    	g_entities[trace.entityNum].s.eType==ET_NPC || g_entities[trace.entityNum].s.eType==ET_PLAYER
    	&& aipm->ns->toFire!=2)
    {
    	VectorScale(aipm->as->velocity,-1,aipm->as->velocity);
    	aipm->cmd.forwardmove=0;
    	aipm->cmd.rightmove=0;
    	aipm->cmd.upmove=15;
    	aipml.can_do_back_move=0;
    	NPM_AirMove();
    }
    */
    
    // check if getting thrown off the ground
    if( aipm->as->velocity[2] > 0 && DotProduct( aipm->as->velocity, trace.plane.normal ) > 10 )
    {
    
        Com_Printf( "%i:kickoff\n", ai_pmove );
        
        /*
        		// go into jump animation
        		if (aipm->ns->toFire!=2)		// metlar jump attack?
        		{
        			if ( aipm->cmd.forwardmove >= 0 ) {
        				NPM_ForceLegsAnim( ANPC_JUMP );
        				aipm->as->pm_flags &= ~PMF_BACKWARDS_JUMP;
        			} else {
        				NPM_ForceLegsAnim( ANPC_JUMP );
        				aipm->as->pm_flags |= PMF_BACKWARDS_JUMP;
        			}
        		}
        */
        aipm->as->groundEntityNum = ENTITYNUM_NONE;
        aipml.groundPlane = false;
        aipml.walking = false;
        return;
    }
    
    // slopes that are too steep will not be considered onground
    if( trace.plane.normal[2] < MIN_WALK_NORMAL )
    {
        Com_Printf( "%i:steep\n", ai_pmove );
        
        // FIXME: if they can't slide down the slope, let them
        // walk (sharp crevices)
        aipm->as->groundEntityNum = ENTITYNUM_NONE;
        aipml.groundPlane = true;
        aipml.walking = false;
        return;
    }
    
    aipml.groundPlane = true;
    aipml.walking = true;
    
    // hitting solid ground will end a waterjump
    if( aipm->as->pm_flags & PMF_TIME_WATERJUMP )
    {
        aipm->as->pm_flags &= ~( PMF_TIME_WATERJUMP | PMF_TIME_LAND );
        aipm->as->pm_time = 0;
    }
    
    if( aipm->as->groundEntityNum == ENTITYNUM_NONE )
    {
        // just hit the ground
        Com_Printf( "%i:Land\n", ai_pmove );
        
        
        /*		if (aipm->npc->npcType==NPC_METLAR)
        		{
        			G_Printf("%d metlar land %d\n",level.time,aipm->ns->toFire);
        
        		}
        */
        /*
        		if ( ( aipm->as->legsAnim & ~ANIM_TOGGLEBIT ) == ANPC_JUMP	// only do crash animation if it was jumping
        			|| (aipm->npc->npcType==NPC_METLAR)  )
        		{
        			NPM_CrashLand();
        //			G_Printf("crash\n");
        		}
        */
        // don't do landing time if we were just going down a slope
        if( aipml.previous_velocity[2] < -200 )
        {
            // don't allow another jump for a little while
            aipm->as->pm_flags |= PMF_TIME_LAND;
            aipm->as->pm_time = 250;
        }
    }
    
    aipm->as->groundEntityNum = trace.entityNum;
    //Com_Printf("walking!\n");
    
    // don't reset the z velocity for slopes
//	pm->ps->velocity[2] = 0;
    //Com_Printf("Touching ent: %i\n", trace.entityNum);
    //NPM_AddTouchEnt( trace.entityNum );
}




/*
================
AI_PMoveSingle

local movement
================
*/
void AI_PMoveSingle( aipmove_t* pmove )
{
    aipm = pmove;
    
    // this counter lets us debug movement problems with a journal
    // by setting a conditional breakpoint fot the previous frame
    ai_pmove++;
    
    // clear results
    aipml.can_do_back_move = 1;
    //aipm->numtouch = 0;
    //aipm->watertype = 0;
    //aipm->waterlevel = 0;
    aipm->as->viewheight = 32;
    
    // clear all pmove local vars
    memset( &aipml, 0, sizeof( aipml ) );
    
    // determine the time
    aipml.msec = pmove->cmd.serverTime - aipm->as->commandTime;
    if( aipml.msec < 1 )
    {
        aipml.msec = 1;
    }
    else if( aipml.msec > 200 )
    {
        aipml.msec = 200;
    }
    aipm->as->commandTime = pmove->cmd.serverTime;
    
    // save old org in case we get stuck
    VectorCopy( aipm->as->origin, aipml.previous_origin );
    
    // save old velocity for crashlanding
    VectorCopy( aipm->as->velocity, aipml.previous_velocity );
    
    aipml.frametime = aipml.msec * 0.001;
    
    // update the viewangles
    AngleVectors( aipm->as->angles, aipml.forward, aipml.right, aipml.up );
    
    if( aipm->cmd.upmove < 10 )
    {
        // not holding jump
        aipm->as->pm_flags &= ~PMF_JUMP_HELD;
    }
    
    // decide if backpedaling animations should be used
    if( aipm->cmd.forwardmove < 0 )
    {
        aipm->as->pm_flags |= PMF_BACKWARDS_RUN;
    }
    else if( aipm->cmd.forwardmove > 0 || ( aipm->cmd.forwardmove == 0 && aipm->cmd.rightmove ) )
    {
        aipm->as->pm_flags &= ~PMF_BACKWARDS_RUN;
    }
    
    if( aipm->as->pm_type >= PM_DEAD )
    {
        aipm->cmd.forwardmove = 0;
        aipm->cmd.rightmove = 0;
        aipm->cmd.upmove = 0;
    }
    
    if( aipm->as->pm_type == PM_FREEZE )
    {
        return;		// no movement at all
    }
    /*
    	// set watertype, and waterlevel
    	NPM_SetWaterLevel();
    	aipml.previous_waterlevel = pmove->waterlevel;
    
    	// set mins, maxs, and viewheight
    //	NPM_CheckDuck ();
    */
    // set groundentity
    AI_PMove_GroundTrace();
    /*
    	if ( aipm->as->pm_type == PM_DEAD ) {
    //		G_Prinuf("dead");
    		NPM_DeadMove ();
    	}
    */
    /*
    	NPM_DropTimers();
    */
//	if ( aipm->npc->npcType==NPC_BAT && aipm->ns->inFlight) {
    // flight powerup doesn't allow jump and has different friction
//		G_Printf("fly ");
//		NPM_FlyMove();
    /*	} else if (aipm->as->pm_flags & PMF_GRAPPLE_PULL) {
    		NPM_GrappleMove();
    		// We can wiggle a bit
    		NPM_AirMove();
    */
//	} else if (aipm->as->pm_flags & PMF_TIME_WATERJUMP) {
//		G_Printf("water_jump ");
//		NPM_WaterJumpMove();
//	} else if ( aipm->waterlevel > 1 ) {
    // swimming
//		G_Printf("water ");
//		NPM_WaterMove();
//	} else
    if( aipml.walking )
    {
        // walking on ground
        //G_Printf("walk\n");
        AI_PMoveWalk();
        
    }
    else
    {
        // airborne
        //G_Printf("air\n");
        AI_PMoveAir();
    }
    
//	NPM_Animate();

    // set groundentity, watertype, and waterlevel
    AI_PMove_GroundTrace();
//	NPM_SetWaterLevel();

    // weapons
//	NPM_Weapon();

    // torso animation
//	PM_TorsoAnimation();

    // footstep events / legs animations
//	NPM_Footsteps();

    // entering / leaving water splashes
//	NPM_WaterEvents();

    // snap some parts of playerstate to save network bandwidth
    SnapVector( aipm->as->velocity );
}



/*
================
AI_PMove

movement code
================
*/
void    AI_PMove( aipmove_t* pmove )
{


    int			finalTime;
    
    finalTime = pmove->cmd.serverTime;
    if( finalTime < pmove->as->commandTime )
    {
        return;					// should not happen
    }
    
    if( finalTime > pmove->as->commandTime + 1000 )
    {
        pmove->as->commandTime = finalTime - 1000;
    }
    
    pmove->as->pmove_framecount = ( pmove->as->pmove_framecount + 1 ) & ( ( 1 << PS_PMOVEFRAMECOUNTBITS ) - 1 );
    
    // chop the move up if it is too long, to prevent framerate
    // dependent behavior
    while( pmove->as->commandTime != finalTime )
    {
        int             msec;
        
        
        msec = finalTime - pmove->as->commandTime;
        
        if( msec > 66 )
        {
            msec = 66;
        }
        pmove->cmd.serverTime = pmove->as->commandTime + msec;
        AI_PMoveSingle( pmove );
        
        if( pmove->as->pm_flags & PMF_JUMP_HELD )
        {
            pmove->cmd.upmove = 20;
        }
    }
    
    //PM_CheckStuck();
    
    
}

