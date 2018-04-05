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

// snddma_null.c
// all other sound mixing is portable

#include "../client/client.h"

bool SNDDMA_Init( void )
{
    return false;
}

int SNDDMA_GetDMAPos( void )
{
    return 0;
}

void SNDDMA_Shutdown( void )
{
}

void SNDDMA_BeginPainting( void )
{
}

void SNDDMA_Submit( void )
{
}

sfxHandle_t S_RegisterSound( const char* sample )
{
    return 0;
}

void S_StartLocalSound( sfxHandle_t sfxHandle, int channelNum )
{
}

void S_ClearSoundBuffer( void )
{
}
