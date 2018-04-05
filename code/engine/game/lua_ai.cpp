/*
===========================================================================
Copyright (C) 2006 Robert Beckebans <trebor_7@users.sourceforge.net>

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
// lua_ai.c -- ai library for Lua

#include "g_lua.h"
#include <botai/ai.h>

#if(defined(G_LUA))

// ai.Register(entnum)
static int ai_Register( lua_State* L )
{
    gentity_t*      ent;
    int             entnum;
    
    entnum = luaL_checkint( L, 1 );
    
    ent = &g_entities[entnum];
    if( !ent )
    {
        Com_Printf( "ai.Register failed, invalid entity!\n" );
        
    }
    else
    {
        Com_Printf( "ai.Register: %s ...\n", luaL_checkstring( L, 2 ) );
        
        AI_Register( ent );  // make it an ai!
        
    }
    return 0;
}
// ai.Spawn(entnum)
static int ai_Spawn( lua_State* L )
{
    gentity_t*      ent;
    int             entnum;
    
    entnum = luaL_checkint( L, 1 );
    
    ent = &g_entities[entnum];
    if( !ent )
    {
        Com_Printf( "ai.Spawn failed, invalid entity!\n" );
        
    }
    else
    {
    
        AI_Spawn( ent );  // spawn it
        
    }
    return 0;
}

static const luaL_reg ailib[] =
{
    {"Register", ai_Register},
    {"Spawn", ai_Spawn},
    {NULL, NULL}
};

int luaopen_ai( lua_State* L )
{
    luaL_register( L, "ai", ailib );
    
    return 1;
}
#endif

