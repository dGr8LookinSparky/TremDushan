/*
===========================================================================
Copyright (C) 1999-2005 Id Software, Inc.
Copyright (C) 2006 Robert Beckebans <trebor_7@users.sourceforge.net>

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
//
//
// ui_team.c
//

#include "gui_local.h"


#define TEAMMAIN_FRAME	"menu/art/cut_frame"

#define ID_JOINRED		100
#define ID_JOINBLUE		101
#define ID_JOINGAME		102
#define ID_SPECTATE		103
#define ID_BACK			104

#define TEAM_VERTICAL_SPACING	32


typedef struct
{
    menuframework_s menu;
    menubitmap_s    back;
    
    menutext_s      banner;
    
    menutext_s      joinred;
    menutext_s      joinblue;
    menutext_s      joingame;
    menutext_s      spectate;
} teammain_t;

static teammain_t s_teammain;

/*
===============
TeamMain_MenuEvent
===============
*/
static void TeamMain_MenuEvent( void* ptr, int event )
{
    if( event != QM_ACTIVATED )
    {
        return;
    }
    
    switch( ( ( menucommon_s* ) ptr )->id )
    {
        case ID_JOINRED:
            trap_Cmd_ExecuteText( EXEC_APPEND, "cmd team red\n" );
            UI_ForceMenuOff();
            break;
            
        case ID_JOINBLUE:
            trap_Cmd_ExecuteText( EXEC_APPEND, "cmd team blue\n" );
            UI_ForceMenuOff();
            break;
            
        case ID_JOINGAME:
            trap_Cmd_ExecuteText( EXEC_APPEND, "cmd team free\n" );
            UI_ForceMenuOff();
            break;
            
        case ID_SPECTATE:
            trap_Cmd_ExecuteText( EXEC_APPEND, "cmd team spectator\n" );
            UI_ForceMenuOff();
            break;
        case ID_BACK:
            UI_PopMenu();
            break;
    }
}


/*
===============
TeamMain_MenuInit
===============
*/
void TeamMain_MenuInit( void )
{
    int             y;
    int             gametype;
    char            info[MAX_INFO_STRING];
    
    memset( &s_teammain, 0, sizeof( s_teammain ) );
    
    TeamMain_Cache();
    
    s_teammain.menu.wrapAround = true;
    s_teammain.menu.fullscreen = true;
    
    s_teammain.banner._generic.type = MTYPE_BTEXT;
    s_teammain.banner._generic.x = 320;
    s_teammain.banner._generic.y = 16;
    s_teammain.banner.string = "CHOOSE TEAM";
    s_teammain.banner.color = color_white;
    s_teammain.banner.style = UI_CENTER | UI_DROPSHADOW;
    
    
    
    y = 194;
    s_teammain.joingame._generic.type = MTYPE_PTEXT;
    s_teammain.joingame._generic.flags = QMF_CENTER_JUSTIFY | QMF_PULSEIFFOCUS;
    s_teammain.joingame._generic.id = ID_JOINGAME;
    s_teammain.joingame._generic.callback = TeamMain_MenuEvent;
    s_teammain.joingame._generic.x = 320;
    s_teammain.joingame._generic.y = y;
    s_teammain.joingame.string = "JOIN GAME";
    s_teammain.joingame.style = UI_CENTER | UI_DROPSHADOW;
    s_teammain.joingame.color = color_white;
    y += TEAM_VERTICAL_SPACING;
    
    s_teammain.joinred._generic.type = MTYPE_PTEXT;
    s_teammain.joinred._generic.flags = QMF_CENTER_JUSTIFY | QMF_PULSEIFFOCUS;
    s_teammain.joinred._generic.id = ID_JOINRED;
    s_teammain.joinred._generic.callback = TeamMain_MenuEvent;
    s_teammain.joinred._generic.x = 320;
    s_teammain.joinred._generic.y = y;
    s_teammain.joinred.string = "JOIN RED";
    s_teammain.joinred.style = UI_CENTER | UI_DROPSHADOW;
    s_teammain.joinred.color = color_white;
    y += TEAM_VERTICAL_SPACING;
    
    s_teammain.joinblue._generic.type = MTYPE_PTEXT;
    s_teammain.joinblue._generic.flags = QMF_CENTER_JUSTIFY | QMF_PULSEIFFOCUS;
    s_teammain.joinblue._generic.id = ID_JOINBLUE;
    s_teammain.joinblue._generic.callback = TeamMain_MenuEvent;
    s_teammain.joinblue._generic.x = 320;
    s_teammain.joinblue._generic.y = y;
    s_teammain.joinblue.string = "JOIN BLUE";
    s_teammain.joinblue.style = UI_CENTER | UI_DROPSHADOW;
    s_teammain.joinblue.color = color_white;
    y += TEAM_VERTICAL_SPACING;
    
    
    s_teammain.spectate._generic.type = MTYPE_PTEXT;
    s_teammain.spectate._generic.flags = QMF_CENTER_JUSTIFY | QMF_PULSEIFFOCUS;
    s_teammain.spectate._generic.id = ID_SPECTATE;
    s_teammain.spectate._generic.callback = TeamMain_MenuEvent;
    s_teammain.spectate._generic.x = 320;
    s_teammain.spectate._generic.y = y;
    s_teammain.spectate.string = "SPECTATE";
    s_teammain.spectate.style = UI_CENTER | UI_DROPSHADOW;
    s_teammain.spectate.color = color_white;
    y += TEAM_VERTICAL_SPACING;
    
    
    s_teammain.back._generic.type = MTYPE_BITMAP;
    s_teammain.back._generic.name = UI_ART_BUTTON;
    s_teammain.back._generic.flags = QMF_LEFT_JUSTIFY | QMF_PULSEIFFOCUS;
    s_teammain.back._generic.id = ID_BACK;
    s_teammain.back._generic.callback = TeamMain_MenuEvent;
    s_teammain.back._generic.x = 0;
    s_teammain.back._generic.y = 480 - 64;
    s_teammain.back.width = 128;
    s_teammain.back.height = 64;
    s_teammain.back.focuspic = UI_ART_BUTTON_FOCUS;
    s_teammain.back._generic.caption.text = "back";
    s_teammain.back._generic.caption.style = UI_CENTER | UI_DROPSHADOW;
    s_teammain.back._generic.caption.fontsize = 0.6f;
    s_teammain.back._generic.caption.font = &uis.freeSansFont;
    s_teammain.back._generic.caption.color = text_color_normal;
    s_teammain.back._generic.caption.focuscolor = text_color_highlight;
    
    
    trap_GetConfigString( CS_SERVERINFO, info, MAX_INFO_STRING );
    gametype = atoi( Info_ValueForKey( info, "g_gametype" ) );
    
    // set initial states
    switch( gametype )
    {
        case GT_SINGLE_PLAYER:
        case GT_FFA:
        case GT_TOURNAMENT:
            s_teammain.joinred._generic.flags |= QMF_GRAYED;
            s_teammain.joinblue._generic.flags |= QMF_GRAYED;
            break;
            
        default:
        case GT_TEAM:
        case GT_CTF:
            s_teammain.joingame._generic.flags |= QMF_GRAYED;
            break;
    }
    
    Menu_AddItem( &s_teammain.menu, ( void* )&s_teammain.banner );
    Menu_AddItem( &s_teammain.menu, ( void* )&s_teammain.joinred );
    Menu_AddItem( &s_teammain.menu, ( void* )&s_teammain.joinblue );
    Menu_AddItem( &s_teammain.menu, ( void* )&s_teammain.joingame );
    Menu_AddItem( &s_teammain.menu, ( void* )&s_teammain.spectate );
    Menu_AddItem( &s_teammain.menu, ( void* )&s_teammain.back );
}


/*
===============
TeamMain_Cache
===============
*/
void TeamMain_Cache( void )
{
    trap_R_RegisterShaderNoMip( TEAMMAIN_FRAME );
}


/*
===============
UI_TeamMainMenu
===============
*/
void UI_TeamMainMenu( void )
{
    TeamMain_MenuInit();
    UI_PushMenu( &s_teammain.menu );
}
