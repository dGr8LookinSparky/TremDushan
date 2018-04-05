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

// Total number of nodes
int             ai_numNodes;

// array for node data
node_t          ai_nodes[MAX_NODES];
short int       ai_path_table[MAX_NODES][MAX_NODES];



/*
================
AI_Node_FollowPath

Move closer to goal by pointing the bot to the next node
that is closer to the goal
================
*/

bool AI_Node_FollowPath( gentity_t* self )
{
    // try again?
    /*	if(self->ai->as.node_timeout++ > 30)
    	{
    		if(self->ai->as.tries++ > 3)
    			return false;
    		else
    			AI_Node_SetGoal(self, self->ai->as.goalNode, INVALID);
    	}
    */
    // are we there yet?
    
    //if(Distance(self->ai->as.origin, nodes[self->ai->as.nextNode].origin) < 64)
    if( BoundsIntersectPoint( self->r.absmin, self->r.absmax, nodes[self->ai->as.nextNode].origin ) )
    {
        // reset timeout
        self->ai->as.node_timeout = 0;
        
        if( self->ai->as.nextNode == self->ai->as.goalNode )
        {
        
            Com_Printf( "AI_Node_FollowPath: Goal reached!\n" );
            self->ai->as.moveState = STATE_WANDER;
            return true;
            
            //AI_Think_PickLongRangeGoal(self);	// pick a new goal
        }
        else
        {
            self->ai->as.currentNode = self->ai->as.nextNode;
            self->ai->as.nextNode = ai_path_table[self->ai->as.currentNode][self->ai->as.goalNode];
            Com_Printf( "AI_Node_FollowPath: Next node selected.. %d \n", self->ai->as.nextNode );
        }
    }
    
    if( self->ai->as.currentNode == INVALID || self->ai->as.nextNode == INVALID )
        return false;
        
    // set bot's movement vector
    VectorSubtract( nodes[self->ai->as.nextNode].origin, self->ai->as.origin, self->ai->as.moveVector );
    
    return true;
}

/*
================
AI_Node_FindClosestReachable

Find the closest node within a certain range

================
*/

int AI_Node_FindClosestReachable( gentity_t* self, float range, int type )
{
    int             node = INVALID;
    int             i;
    float           closest = 99999;
    float           dist;
    
    vec3_t          v;
    trace_t         tr;
    vec3_t          maxs, mins;
    
    VectorCopy( self->r.mins, mins );
    VectorCopy( self->r.maxs, maxs );
    
    mins[2] += STEPSIZE;
    
    for( i = 0; i < ai_numNodes; i++ )
    {
        if( type == NODE_ALL || type == ai_nodes[i].type )	// check node type
        {
            VectorSubtract( ai_nodes[i].origin, self->s.origin, v );	// subtract first
            
            dist = VectorLength( v );
            
            if( dist < closest && dist < range )
            {
                // make sure it is visible
                trap_Trace( &tr, self->s.origin, mins, maxs, ai_nodes[i].origin, self->s.number, MASK_PLAYERSOLID );
                
                if( tr.fraction == 1.0 )
                {
                    node = i;
                    closest = dist;
                }
            }
        }
    }
    
    return node;
}


/*
================
AI_Node_FindRandom

Select a random Node

================
*/

int AI_Node_FindRandom( void )
{
    return ( ( int ) ai_numNodes * random() );
}


/*
================
AI_Node_Cost

Determin cost of moving from one node to another
================
*/

int AI_Node_Cost( int from, int to )
{
    int             curnode;
    int             cost = 1;	// Shortest possible is 1
    
    if( from == INVALID || to == INVALID )
        return INVALID;
        
    // If we can not get there then return invalid
    if( ai_path_table[from][to] == INVALID )
        return INVALID;
        
    // Otherwise check the path and return the cost
    curnode = ai_path_table[from][to];
    
    // Find a path (linear time, very fast)
    while( curnode != to )
    {
        curnode = ai_path_table[curnode][to];
        if( curnode == INVALID )	// something has corrupted the path abort
            return INVALID;
        cost++;
    }
    
    return cost;
}


/*
================
AI_Node_Cost

Determin cost of moving from one node to another
================
*/
void AI_Node_SetGoal( gentity_t* self, int goalNode, int currentNode )
{
    int             node;
    
    self->ai->as.goalNode = goalNode;
    
    if( currentNode != INVALID )
        node = currentNode;
    else
        node = AI_Node_FindClosestReachable( self, NODE_DENSITY * 3, NODE_ALL );
        
    if( node == INVALID )
        return;
        
    Com_Printf( "AI_Node_SetGoal: start node selected %d \n", node );
    
    self->ai->as.currentNode = node;
    self->ai->as.nextNode = self->ai->as.currentNode;	// make sure we get to the nearest node first
    self->ai->as.node_timeout = 0;
    
    Com_Printf( "AI_Node_SetGoal: goal node selected %d \n", goalNode );
    
    /*
    if(ace_showPath.integer)
    {
    	// draw path to LR goal
    	ACEND_DrawPath(self->ai->as.currentNode, self->ai->as.goalNode);
    }
    */
    
}


/*
================
AI_Node_Load

Read from disk file
================
*/

void AI_Node_Load( void )
{
    fileHandle_t    file;
    int             i, j;
    char            filename[MAX_QPATH];
    int             version;
    char            mapname[MAX_QPATH];
    int num_nodes = 0;
    vec3_t nodes[MAX_NODES];
    
    trap_Cvar_VariableStringBuffer( "mapname", mapname, sizeof( mapname ) );
    Com_sprintf( filename, sizeof( filename ), "maps/%s.navmesh", mapname );
    
    trap_FS_FOpenFile( filename, &file, FS_READ );
    if( !file )
    {
        Com_Printf( "AI: No navigation mesh file '%s' found\n", filename );
        return;
    }
    
    // TODO: use another version than acebot
    // determin version
    trap_FS_Read( &version, sizeof( int ), file );	// read version
    
    if( version == 1 )
    {
    
    
//		Com_Printf("AI: Loading node table '%s'...\n", filename);
        Com_Printf( "AI: Loading navigation mesh '%s'...\n", filename );
        
        trap_FS_Read( &num_nodes, sizeof( int ), file );	// read count
        trap_FS_Read( &nodes, sizeof( vec3_t ) * num_nodes, file );
        
        
        /*		trap_FS_Read(&ai_numNodes, sizeof(int), file);	// read count
        		trap_FS_Read(&ai_nodes, sizeof(node_t) * ai_numNodes, file);
        
        		for(i = 0; i < ai_numNodes; i++)
        			for(j = 0; j < ai_numNodes; j++)
        				trap_FS_Read(&ai_path_table[i][j], sizeof(short int), file);	// write count
        */
        
    }
    else
    {
        Com_Printf( "AI: '%s' has wrong version %i\n", filename, version );
    }
    
    trap_FS_FCloseFile( file );
    
    Com_Printf( "done.\n" );
    
    // add nodes
    for( i = 0; i < num_nodes; i++ )
    {
        AI_Add_Node( nodes[i] );
    }
    
    Com_Printf( "%i nodes loaded\n", ai_numNodes );
    
    // TODO: evaluate nodes later...
    //ACEIT_BuildItemNodeTable(true);
}

/*
================
AI_SaveNodes

save node file to disc
================
*/
/*
void AI_SaveNodes( void )
{

    fileHandle_t    file;
	char            filename[MAX_QPATH];
	int             i, j;
	int             version = 1;
	char            mapname[MAX_QPATH];


	trap_Cvar_VariableStringBuffer("mapname", mapname, sizeof(mapname));
	Com_sprintf(filename, sizeof(filename), "maps/%s.nodes", mapname);

	trap_FS_FOpenFile(filename, &file, FS_WRITE);
	if(!file)
	{
		G_Printf("WARNING: Couldn't write node table: %s\n", filename);
		return;
	}
	else
		G_Printf("AI_SaveNodes: Saving node table '%s'...", filename);

	trap_FS_Write(&version, sizeof(int), file);
	trap_FS_Write(&ai_numNodes, sizeof(int), file);
	trap_FS_Write(nodes, sizeof(node_t) * ai_numNodes, file);

	for(i = 0; i < ai_numNodes; i++)
		for(j = 0; j < ai_numNodes; j++)
			trap_FS_Write(&ai_path_table[i][j], sizeof(short int), file);	// write count

	trap_FS_FCloseFile(file);

	G_Printf("done.\n");

	G_Printf("%i nodes saved\n", ai_numNodes);

}
*/

/*
================
AI_ShowNode

update paths
================
*/
/*
void AI_Node_ResolvePaths( void )
{
	int             i, from, to;
	int             num = 0;

	Com_Printf("AI_Node_ResolvePaths: updating paths...\n");

	for(from = 0; from < ai_numNodes; from++)
	{
		for(to = 0; to < ai_numNodes; to++)
		{
			// update unresolved paths
			// Not equal to itself, not equal to -1 and equal to the last link
			if(from != to && ai_path_table[from][to] == to)
			{
				num++;

				// Now for the self-referencing part linear time for each link added
				for(i = 0; i < ai_numNodes; i++)
				{
					if(ai_path_table[i][from] != -1)
					{
						if(i == to)
							ai_path_table[i][to] = -1;	// make sure we terminate
						else
							ai_path_table[i][to] = ai_path_table[i][from];
					}
				}
			}
		}
	}

	Com_Printf("done (%d updated)\n", num);

}
*/
/*
================
AI_ShowNode

show node in cgame for debugging
================
*/

void AI_ShowNode( int node )
{
    gentity_t*      ent;
    
    
    if( ai_nodes[node].type == INVALID )
        return;
        
    ent = G_Spawn();
    
    ent->s.eType = ET_AI_NODE;
    ent->r.svFlags |= SVF_BROADCAST;
    
    ent->classname = "AI_node";
    
    VectorCopy( ai_nodes[node].origin, ent->s.origin );
    VectorCopy( ai_nodes[node].origin, ent->s.pos.trBase );
    
    // otherEntityNum is transmitted with GENTITYNUM_BITS so enough for 1000 nodes
    ent->s.otherEntityNum = node;
    
    trap_LinkEntity( ent );
}

/*
================
AI_Node_Init

init node array (set all to INVALID)
================
*/
void AI_Node_Init( void )
{
    int i, n;
    
    vec3_t dest;
    trace_t		tr;
    gentity_t* ent;
    
    ai_numNodes = 0;
    memset( ai_nodes, 0, sizeof( node_t ) * MAX_NODES );
    memset( ai_path_table, INVALID, sizeof( short int ) * MAX_NODES * MAX_NODES );
    
    // load existing nodes from file ( if any )
    AI_Node_Load();
    
    /*
        // clean node mesh ...
        for(i = 0; i < ai_numNodes; i++)
    	{
    
    	    // for now, only support NODE_MOVE (default nodes)
    	    if(ai_nodes[i].type != NODE_MOVE){
                Com_Printf("AI_Node_Init: node %i bad type (%i)\n", i, ai_nodes[i].type);
                ai_nodes[i].type = INVALID;
                continue;
     	    }
    
    
            // drop nodes to floor
            VectorSet( dest, ai_nodes[i].origin[0], ai_nodes[i].origin[1], ai_nodes[i].origin[2] - 4096 );
            trap_Trace( &tr, ai_nodes[i].origin, NULL, NULL, dest, ENTITYNUM_NONE, MASK_SOLID );
            if ( tr.startsolid ) { // shouldnt be!
                Com_Printf ("AI_Node_Init: node %i startsolid at %s\n", i, vtos(ai_nodes[i].origin));
                ai_nodes[i].type = INVALID;
                continue;
    
            }
    
            ai_nodes[i].origin[2] = tr.endpos[2] + 24;
    
            // check for bad intersection
            for(n = 0, ent = &g_entities[0]; n < level.numEntities; n++, ent++)
            {
                 if(ent->s.eType == ET_AI_NODE)
                    continue;
    
                 if(Distance(ent->s.origin, ai_nodes[i].origin) < 64){
                 //if(BoundsIntersectPoint(ent->r.absmin, ent->r.absmax, ai_nodes[i].origin)){ // shouldnt be!
                    Com_Printf ("AI_Node_Init: node %i bad intersect with %s\n", i, ent->classname);
                    ai_nodes[i].type = INVALID;
                }
            }
    
    	}
    
        // check for to close / double nodes
        for(i = 0; i < ai_numNodes; i++){
            if(ai_nodes[i].type == INVALID)
                continue;
            for(n = 0; n < ai_numNodes; n++){
                if(i == n)
                    continue;
                if(ai_nodes[n].type == INVALID)
                    continue;
                if(Distance(ai_nodes[i].origin, ai_nodes[n].origin) < 32){
                    Com_Printf ("AI_Node_Init: node %i to close to node %i \n", n, i);
                    ai_nodes[n].type = INVALID;
                }
    
            }
        }
    */
    for( i = 0; i < ai_numNodes; i++ )
    {
        // show nodes for debugging
        AI_ShowNode( i );
    }
    
    
    
}



/*
================
AI_Add_Node

================
*/

int AI_Add_Node( vec3_t origin )
{

    int i;
    
    // block if we exceed maximum
    if( ai_numNodes >= MAX_NODES )
    {
        Com_Printf( "AI_Add_Node: MAX_NODES (%i) exceeded!\n", MAX_NODES );
        return INVALID;
    }
    
    // set location
    //origin[2] += 256;
    VectorCopy( origin, ai_nodes[ai_numNodes].origin );
    
    Com_Printf( "AI_Add_Node: adding node at %s\n", vtos( origin ) );
    
    // set type
    ai_nodes[ai_numNodes].type = NODE_MOVE;
    
    SnapVector( ai_nodes[ai_numNodes].origin );
    
    
    ai_numNodes++;
    return ai_numNodes - 1;		// return the node added
    
}



/*
================
AI_Node_Shutdown

================
*/
void AI_Node_Shutdown( void )
{
    /*
        int i,n;
    
        vec3_t dest;
        trace_t		tr;
        gentity_t * ent;
    
        node_t          tmp_nodes[MAX_NODES];
        short int       tmp_path_table[MAX_NODES][MAX_NODES];
        int             tmp_numNodes;
    
        // clean node mesh ...
        for(i = 0; i < ai_numNodes; i++)
    	{
    	    // for now, only support NODE_MOVE (default nodes)
    	    if(ai_nodes[i].type != NODE_MOVE){
                Com_Printf("AI_Node_Init: node %i bad type (%i)\n", i, ai_nodes[i].type);
                ai_nodes[i].type = INVALID;
                continue;
     	    }
    
    
            // drop nodes to floor
            VectorSet( dest, ai_nodes[i].origin[0], ai_nodes[i].origin[1], ai_nodes[i].origin[2] - 4096 );
            trap_Trace( &tr, ai_nodes[i].origin, NULL, NULL, dest, ENTITYNUM_NONE, MASK_SOLID );
            if ( tr.startsolid ) { // shouldnt be!
                Com_Printf ("AI_Node_Init: node %i startsolid at %s\n", i, vtos(ai_nodes[i].origin));
                ai_nodes[i].type = INVALID;
                continue;
    
            }
    
            ai_nodes[i].origin[2] = tr.endpos[2] + 24;
    
            // check for bad intersection
            for(n = 0, ent = &g_entities[0]; n < level.numEntities; n++, ent++)
            {
                 if(ent->s.eType == ET_AI_NODE)
                    continue;
    
                 if(Distance(ent->s.origin, ai_nodes[i].origin) < 64){
                 //if(BoundsIntersectPoint(ent->r.absmin, ent->r.absmax, ai_nodes[i].origin)){ // shouldnt be!
                    Com_Printf ("AI_Node_Init: node %i bad intersect with %s\n", i, ent->classname);
                    ai_nodes[i].type = INVALID;
                }
            }
    
    	}
    
        // check for to close / double nodes
        for(i = 0; i < ai_numNodes; i++){
            if(ai_nodes[i].type == INVALID)
                continue;
            for(n = 0; n < ai_numNodes; n++){
                if(i == n)
                    continue;
                if(ai_nodes[n].type == INVALID)
                    continue;
                if(Distance(ai_nodes[i].origin, ai_nodes[n].origin) < 64){
                    Com_Printf ("AI_Node_Init: node %i to close to node %i \n", n, i);
                    ai_nodes[n].type = INVALID;
                }
    
            }
        }
    
        // remove all INVALID nodes
        memset(tmp_nodes, 0, sizeof(node_t) * MAX_NODES);
    	tmp_numNodes = 0;
    
        for(i = 0; i < ai_numNodes; i++){
             if(ai_nodes[i].type == INVALID)
                    continue;
             memcpy(&tmp_nodes[tmp_numNodes], &ai_nodes[i], sizeof(node_t));
    
             tmp_numNodes++;
        }
    
        // reset tables
    	ai_numNodes = 0;
    	memset(ai_nodes, 0, sizeof(node_t) * MAX_NODES);
    	memset(ai_path_table, INVALID, sizeof(short int) * MAX_NODES * MAX_NODES);
    
        // re-add valid nodes
        for(i = 0; i < tmp_numNodes; i++){
            AI_Add_Node(tmp_nodes[i].origin);
        }
    
    
        // update paths
        AI_Node_ResolvePaths();
    
        AI_SaveNodes();
    */
}




