/*
===========================================================================
Copyright (C) 1999-2005 Id Software, Inc.

This file is part of OpenWolf source code.

OpenWolf source code is free software; you can redistribute it
and/or modify it under the terms of the GNU General Public License as
published by the Free Software Foundation; either version 2 of the License,
or (at your option) any later version.

OpenWolf source code is distributed in the hope that it will be
useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with OpenWolf source code; if not, write to the Free Software
Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
===========================================================================
*/

#include "../client/client.h"

cvar_t*         cl_shownet;

void CL_Shutdown( char* finalmsg )
{
}

void CL_Init( void )
{
    cl_shownet = Cvar_Get( "cl_shownet", "0", CVAR_TEMP );
}

void CL_MouseEvent( int dx, int dy, int time )
{
}

void Key_WriteBindings( fileHandle_t f )
{
}

void CL_Frame( int msec )
{
}

void CL_PacketEvent( netadr_t from, msg_t* msg )
{
}

void CL_CharEvent( int key )
{
}

void CL_Disconnect( bool showMainMenu )
{
}

void CL_MapLoading( void )
{
}

bool CL_GameCommand( void )
{
    return false;
}

void CL_KeyEvent( int key, bool down, unsigned time )
{
}

bool UI_GameCommand( void )
{
    return false;
}

void CL_ForwardCommandToServer( const char* string )
{
}

void CL_ConsolePrint( char* txt )
{
}

void CL_JoystickEvent( int axis, int value, int time )
{
}

void CL_InitKeyCommands( void )
{
}

void CL_CDDialog( void )
{
}

void CL_FlushMemory( void )
{
}

void CL_StartHunkUsers( bool rendererOnly )
{
}

void CL_Snd_Restart( void )
{
}

void CL_ShutdownAll( void )
{
}

bool CL_CDKeyValidate( const char* key, const char* checksum )
{
    return true;
}
