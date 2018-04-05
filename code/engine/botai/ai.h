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
// ai.h - Main header file for AI - wich runs alongside the default ACEBOT


#ifndef _AI_H
#define _AI_H

//
// g_ai.c
//

void            AI_Init( void );
void            AI_Shutdown( void );
int             AI_StartFrame( int time );
void            AI_Register( gentity_t* ent );
void            AI_Spawn( gentity_t* ent );
void            AI_Think( gentity_t* ent );

void            SP_AI( gentity_t* ent ) ;

//
// g_ai_think.c
//

void            AI_Think_PickLongRangeGoal( gentity_t* self );

//
// g_ai_nodes.c
//
void            AI_Node_Init( void );
void            AI_Node_Shutdown( void );
void            AI_Node_Load( void );
int             AI_Node_FindClosestReachable( gentity_t* self, float range, int type );
int             AI_Node_FindRandom( void );
int             AI_Node_Cost( int from, int to );
void            AI_Node_SetGoal( gentity_t* self, int goalNode, int currentNode );
bool        AI_Node_FollowPath( gentity_t* self );
void            AI_ShowNode( int node );
int             AI_Add_Node( vec3_t origin );

//
// g_ai_misc.c
//

bool        AI_InFront( gentity_t* self, gentity_t* other );
bool        AI_Visible( gentity_t* self, gentity_t* other );
bool        AI_FindEnemy( gentity_t* self );
bool        AI_CheckShot( gentity_t* self, vec3_t point );

//
// g_ai_move.c
//

bool        AI_CanMove( gentity_t* self, int direction );
void            AI_Attack_Move( gentity_t* self );
void            AI_MoveToGoal( gentity_t* self );

void            AI_ChangeAngle( gentity_t* ent );

#define MAX_AI 500

/*
#define STATE_WANDER 0
#define STATE_MOVE  1
*/


typedef struct
{
    int             num; // parent entitys num
    // movement
    int             viewheight;
    
    gentity_t*      moveTarget;
    int             moveState;
    vec3_t          moveVector;
    
//	gentity_t      *moveTarget;
//	gentity_t      *goalEntity;	// moveTarget of previous frame


    // timers
    int             wander_timeout;
    int             node_timeout;
    
    
    // nodes
    int             currentNode;
    int             goalNode;
    int             nextNode;	// the node that will take us one step closer to our goal
    int             lastNode;
    
    
    int             tries;
    
    // from playerState_t
    vec3_t          origin;
    vec3_t          angles;
    vec3_t          velocity;
    
    float           speed;
    float           turnSpeed;
    int             gravity;
    
    usercmd_t       cmd;
    int			    commandTime;	// cmd->serverTime of last executed command
    
    int			    pmove_framecount;
    int             pm_type;
    int             pm_flags;	// ducked, jump_held, etc
    int             pm_time;
    
    
    int			    groundEntityNum;
    
} aiState_t;

#ifndef CGAMEDLL
typedef struct
{
    int             health;
    gentity_t*       enemy;
    int             enemyRange;
    
    aiState_t       as;
    
} gai_t ;
#endif

//
// AI Movement
//


typedef struct
{

    vec3_t		forward, right, up;
    float		frametime;
    
    int			msec;
    
    int			can_do_back_move;
    bool	walking;
    bool	groundPlane;
    trace_t		groundTrace;
    
    float		impactSpeed;
    
    vec3_t		previous_origin;
    vec3_t		previous_velocity;
    int			previous_waterlevel;
    
} aipml_t;
typedef struct
{


    // state (in / out)
    aiState_t*	as;
    
    gentity_t*       enemy;
    
    /*
    gnpc_t		*npc;
    npcData_t	*ns;
    
    */
    // command (in)
    usercmd_t	cmd;
    
    
    int			tracemask;			// collide against these types of surfaces
    
    /*
    	int			debugLevel;			// if set, diagnostic output will be printed
    	bool	noFootsteps;		// if the game is setup for no footsteps by the server
    	bool	gauntletHit;		// true if a gauntlet attack would actually hit something
    
    	int			framecount;
    
    	// results (out)
    	int			numtouch;
    	int			touchents[MAXTOUCH];
    */
    vec3_t		mins, maxs;			// bounding box size
    /*
    	int			watertype;
    	int			waterlevel;
    
    	float		xyspeed;
    
    	// for fixed msec Pmove
    
    */
    int			pmove_msec;
    
    
    // callbacks to test the world
    // these will be different functions during game and cgame
    void	( *trace )( trace_t* results, const vec3_t start, const vec3_t mins, const vec3_t maxs, const vec3_t end, int passEntityNum, int contentMask );
    int	( *pointcontents )( const vec3_t point, int passEntityNum );
    
} aipmove_t;


void            AI_PMove( aipmove_t* pmove );


#endif
