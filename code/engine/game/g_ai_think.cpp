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
AI_Think_PickLongRangeGoal

pick a long range goal
================
*/

void AI_Think_PickLongRangeGoal( gentity_t* self )
{

    int             i;
    int             node;
    float           weight, bestWeight = 0.0f;
    int             currentNode, goalNode;
    gentity_t*      goalEnt;
    gclient_t*      cl;
    gentity_t*      ent;
    gentity_t*      player;
    float           cost;
    
    goalNode = INVALID;
    goalEnt = NULL;
    
    // look for a target
    currentNode = AI_Node_FindClosestReachable( self, NODE_DENSITY * 3, NODE_ALL );
    
    self->ai->as.currentNode = currentNode;
    
    // couldnt find any node
    if( currentNode == INVALID )
    {
//	    Com_Printf("AI: can't find starting node, wandering now!\n");
        self->ai->as.moveState = STATE_WANDER;
        self->ai->as.wander_timeout = level.time + 1000;
        self->ai->as.goalNode = -1;
        return;
    }
    
    // find a valid random node
    for( i = 0; i < 3; i++ )
    {
    
        node = AI_Node_FindRandom();
        cost = AI_Node_Cost( currentNode, node );
        
        if( cost == INVALID || cost < 3 )	// ignore invalid and very short hops
            continue;
            
    }
    
    // if do not find a goal, go wandering....
    if( node == INVALID )
    {
//	    Com_Printf("AI: can't find goal, wandering now!\n");
        self->ai->as.goalNode = INVALID;
        self->ai->as.moveState = STATE_WANDER;
        self->ai->as.wander_timeout = level.time + 1000;
        return;
    }
    
    // OK, everything valid, let's start moving to our goal
    self->ai->as.moveState = STATE_MOVE;
//	self->ai->as.tries = 0;			// reset the count of how many times we tried this goal

    AI_Node_SetGoal( self, node, currentNode );
    
}
