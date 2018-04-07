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
/*
=======================================================================

INGAME MENU

=======================================================================
*/


#include "gui_local.h"


//#define INGAME_FRAME                  "menu/art/addbotframe"
//#define INGAME_FRAME                  "menu/art/cut_frame"
#define INGAME_MENU_VERTICAL_SPACING	42

#define ID_TEAM					10
#define ID_ADDBOTS				11
#define ID_REMOVEBOTS			12
#define ID_SETUP				13
#define ID_SERVERINFO			14
#define ID_LEAVEARENA			15
#define ID_RESTART				16
#define ID_QUIT					17
#define ID_RESUME				18
#define ID_TEAMORDERS			19


typedef struct
{
    menuframework_s menu;
    
    //menubitmap_s    frame;
    menutext_s      banner;
    menutext_s      team;
    menutext_s      setup;
    menutext_s      server;
    menutext_s      addbots;
    menutext_s      removebots;
    menutext_s      teamorders;
    
    menubitmap_s    quit;
    menubitmap_s    restart;
    menubitmap_s    leave;
    menubitmap_s    resume;
} ingamemenu_t;

static ingamemenu_t s_ingame;


/*
=================
InGame_LeaveAction
=================
*/
static void InGame_LeaveAction( bool result )
{
    if( !result )
    {
        return;
    }
    
    
    trap_Cmd_ExecuteText( EXEC_APPEND, "disconnect\n" );
}

/*
=================
InGame_RestartAction
=================
*/
static void InGame_RestartAction( bool result )
{
    if( !result )
    {
        return;
    }
    
    UI_PopMenu();
    trap_Cmd_ExecuteText( EXEC_APPEND, "map_restart 0\n" );
}


/*
=================
InGame_QuitAction
=================
*/
static void InGame_QuitAction( bool result )
{
    if( !result )
    {
        return;
    }
    UI_PopMenu();
    UI_CreditMenu();
}


/*
=================
InGame_Event
=================
*/
void InGame_Event( void* ptr, int notification )
{
    if( notification != QM_ACTIVATED )
    {
        return;
    }
    
    switch( ( ( menucommon_s* ) ptr )->id )
    {
        case ID_TEAM:
            UI_TeamMainMenu();
            break;
            
        case ID_SETUP:
            UI_SetupMenu();
            break;
            
        case ID_LEAVEARENA:
            UI_ConfirmMenu( "DISCONNECT FROM SERVER?", ( voidfunc_f ) NULL, InGame_LeaveAction );
            break;
            
        case ID_RESTART:
            UI_ConfirmMenu( "RESTART ARENA?", ( voidfunc_f ) NULL, InGame_RestartAction );
            break;
            
        case ID_QUIT:
            UI_ConfirmMenu( "EXIT GAME?", ( voidfunc_f ) NULL, InGame_QuitAction );
            break;
            
        case ID_SERVERINFO:
            UI_ServerInfoMenu();
            break;
            
        case ID_ADDBOTS:
            UI_AddBotsMenu();
            break;
            
        case ID_REMOVEBOTS:
            UI_RemoveBotsMenu();
            break;
            
        case ID_TEAMORDERS:
            UI_TeamOrdersMenu();
            break;
            
        case ID_RESUME:
            UI_PopMenu();
            break;
    }
}


/*
=================
InGame_MenuInit
=================
*/
void InGame_MenuInit( void )
{
    int             y;
    uiClientState_t cs;
    char            info[MAX_INFO_STRING];
    int             team;
    
    memset( &s_ingame, 0, sizeof( ingamemenu_t ) );
    
    InGame_Cache();
    
    s_ingame.menu.wrapAround = true;
    s_ingame.menu.fullscreen = true;
    
    
    s_ingame.banner._generic.type = MTYPE_BTEXT;
    s_ingame.banner._generic.x = 320;
    s_ingame.banner._generic.y = 16;
    s_ingame.banner.string = "INGAME MENU";
    s_ingame.banner.color = color_white;
    s_ingame.banner.style = UI_CENTER | UI_DROPSHADOW;
    
    
    y = 134;
    s_ingame.team._generic.type = MTYPE_PTEXT;
    s_ingame.team._generic.flags = QMF_CENTER_JUSTIFY | QMF_PULSEIFFOCUS;
    s_ingame.team._generic.x = 320;
    s_ingame.team._generic.y = y;
    s_ingame.team._generic.id = ID_TEAM;
    s_ingame.team._generic.callback = InGame_Event;
    s_ingame.team.string = "START";
    s_ingame.team.color = color_white;
    s_ingame.team.style = UI_CENTER | UI_DROPSHADOW;
    
    y += INGAME_MENU_VERTICAL_SPACING;
    s_ingame.addbots._generic.type = MTYPE_PTEXT;
    s_ingame.addbots._generic.flags = QMF_CENTER_JUSTIFY | QMF_PULSEIFFOCUS;
    s_ingame.addbots._generic.x = 320;
    s_ingame.addbots._generic.y = y;
    s_ingame.addbots._generic.id = ID_ADDBOTS;
    s_ingame.addbots._generic.callback = InGame_Event;
    s_ingame.addbots.string = "ADD BOTS";
    s_ingame.addbots.color = color_white;
    s_ingame.addbots.style = UI_CENTER | UI_DROPSHADOW;
    if( !trap_Cvar_VariableValue( "sv_running" ) || ( trap_Cvar_VariableValue( "g_gametype" ) == GT_SINGLE_PLAYER ) )
    {
        s_ingame.addbots._generic.flags |= QMF_GRAYED;
    }
    
    y += INGAME_MENU_VERTICAL_SPACING;
    s_ingame.removebots._generic.type = MTYPE_PTEXT;
    s_ingame.removebots._generic.flags = QMF_CENTER_JUSTIFY | QMF_PULSEIFFOCUS;
    s_ingame.removebots._generic.x = 320;
    s_ingame.removebots._generic.y = y;
    s_ingame.removebots._generic.id = ID_REMOVEBOTS;
    s_ingame.removebots._generic.callback = InGame_Event;
    s_ingame.removebots.string = "REMOVE BOTS";
    s_ingame.removebots.color = color_white;
    s_ingame.removebots.style = UI_CENTER | UI_DROPSHADOW;
    if( !trap_Cvar_VariableValue( "sv_running" ) || ( trap_Cvar_VariableValue( "g_gametype" ) == GT_SINGLE_PLAYER ) )
    {
        s_ingame.removebots._generic.flags |= QMF_GRAYED;
    }
    
    if( ( trap_Cvar_VariableValue( "g_gametype" ) >= GT_TEAM ) )
    {
        y += INGAME_MENU_VERTICAL_SPACING;
        s_ingame.teamorders._generic.type = MTYPE_PTEXT;
        s_ingame.teamorders._generic.flags = QMF_CENTER_JUSTIFY | QMF_PULSEIFFOCUS;
        s_ingame.teamorders._generic.x = 320;
        s_ingame.teamorders._generic.y = y;
        s_ingame.teamorders._generic.id = ID_TEAMORDERS;
        s_ingame.teamorders._generic.callback = InGame_Event;
        s_ingame.teamorders.string = "TEAM ORDERS";
        s_ingame.teamorders.color = color_white;
        s_ingame.teamorders.style = UI_CENTER | UI_DROPSHADOW;
        
        trap_GetClientState( &cs );
        trap_GetConfigString( CS_PLAYERS + cs.clientNum, info, MAX_INFO_STRING );
        team = atoi( Info_ValueForKey( info, "t" ) );
        if( team == TEAM_SPECTATOR )
        {
            s_ingame.teamorders._generic.flags |= QMF_GRAYED;
        }
    }
    
    y += INGAME_MENU_VERTICAL_SPACING;
    s_ingame.setup._generic.type = MTYPE_PTEXT;
    s_ingame.setup._generic.flags = QMF_CENTER_JUSTIFY | QMF_PULSEIFFOCUS;
    s_ingame.setup._generic.x = 320;
    s_ingame.setup._generic.y = y;
    s_ingame.setup._generic.id = ID_SETUP;
    s_ingame.setup._generic.callback = InGame_Event;
    s_ingame.setup.string = "SETUP";
    s_ingame.setup.color = color_white;
    s_ingame.setup.style = UI_CENTER | UI_DROPSHADOW;
    
    y += INGAME_MENU_VERTICAL_SPACING;
    s_ingame.server._generic.type = MTYPE_PTEXT;
    s_ingame.server._generic.flags = QMF_CENTER_JUSTIFY | QMF_PULSEIFFOCUS;
    s_ingame.server._generic.x = 320;
    s_ingame.server._generic.y = y;
    s_ingame.server._generic.id = ID_SERVERINFO;
    s_ingame.server._generic.callback = InGame_Event;
    s_ingame.server.string = "SERVER INFO";
    s_ingame.server.color = color_white;
    s_ingame.server.style = UI_CENTER | UI_DROPSHADOW;
    
    
    s_ingame.restart._generic.type = MTYPE_BITMAP;
    s_ingame.restart._generic.name = UI_ART_BUTTON;
    s_ingame.restart._generic.flags = QMF_RIGHT_JUSTIFY | QMF_PULSEIFFOCUS;
    s_ingame.restart._generic.callback = InGame_Event;
    s_ingame.restart._generic.id = ID_RESTART;
    s_ingame.restart._generic.x = 320 - 32;
    s_ingame.restart._generic.y = 480 - 64;
    s_ingame.restart.width = 128;
    s_ingame.restart.height = 64;
    s_ingame.restart.focuspic = UI_ART_BUTTON_FOCUS;
    s_ingame.restart._generic.caption.text = "RESTART";
    s_ingame.restart._generic.caption.style = UI_CENTER | UI_DROPSHADOW;
    s_ingame.restart._generic.caption.fontsize = 0.6f;
    s_ingame.restart._generic.caption.font = &uis.freeSansFont;
    s_ingame.restart._generic.caption.color = text_color_normal;
    s_ingame.restart._generic.caption.focuscolor = text_color_highlight;
    if( !trap_Cvar_VariableValue( "sv_running" ) )
    {
        s_ingame.restart._generic.flags |= QMF_GRAYED;
    }
    
    
    s_ingame.quit._generic.type = MTYPE_BITMAP;
    s_ingame.quit._generic.name = UI_ART_BUTTON;
    s_ingame.quit._generic.flags = QMF_LEFT_JUSTIFY | QMF_PULSEIFFOCUS;
    s_ingame.quit._generic.callback = InGame_Event;
    s_ingame.quit._generic.id = ID_QUIT;
    s_ingame.quit._generic.x = 320 + 32;
    s_ingame.quit._generic.y = 480 - 64;
    s_ingame.quit.width = 128;
    s_ingame.quit.height = 64;
    s_ingame.quit.focuspic = UI_ART_BUTTON_FOCUS;
    s_ingame.quit._generic.caption.text = "Quit";
    s_ingame.quit._generic.caption.style = UI_CENTER | UI_DROPSHADOW;
    s_ingame.quit._generic.caption.fontsize = 0.6f;
    s_ingame.quit._generic.caption.font = &uis.freeSansFont;
    s_ingame.quit._generic.caption.color = text_color_normal;
    s_ingame.quit._generic.caption.focuscolor = text_color_highlight;
    
    
    s_ingame.leave._generic.type = MTYPE_BITMAP;
    s_ingame.leave._generic.name = UI_ART_BUTTON;
    s_ingame.leave._generic.flags = QMF_RIGHT_JUSTIFY | QMF_PULSEIFFOCUS;
    s_ingame.leave._generic.callback = InGame_Event;
    s_ingame.leave._generic.id = ID_LEAVEARENA;
    s_ingame.leave._generic.x = 640;
    s_ingame.leave._generic.y = 480 - 64;
    s_ingame.leave.width = 128;
    s_ingame.leave.height = 64;
    s_ingame.leave.focuspic = UI_ART_BUTTON_FOCUS;
    s_ingame.leave._generic.caption.text = "leave";
    s_ingame.leave._generic.caption.style = UI_CENTER | UI_DROPSHADOW;
    s_ingame.leave._generic.caption.fontsize = 0.6f;
    s_ingame.leave._generic.caption.font = &uis.freeSansFont;
    s_ingame.leave._generic.caption.color = text_color_normal;
    s_ingame.leave._generic.caption.focuscolor = text_color_highlight;
    
    
    s_ingame.resume._generic.type = MTYPE_BITMAP;
    s_ingame.resume._generic.name = UI_ART_BUTTON;
    s_ingame.resume._generic.flags = QMF_LEFT_JUSTIFY | QMF_PULSEIFFOCUS;
    s_ingame.resume._generic.callback = InGame_Event;
    s_ingame.resume._generic.id = ID_RESUME;
    s_ingame.resume._generic.x = 0;
    s_ingame.resume._generic.y = 480 - 64;
    s_ingame.resume.width = 128;
    s_ingame.resume.height = 64;
    s_ingame.resume.focuspic = UI_ART_BUTTON_FOCUS;
    s_ingame.resume._generic.caption.text = "back";
    s_ingame.resume._generic.caption.style = UI_CENTER | UI_DROPSHADOW;
    s_ingame.resume._generic.caption.fontsize = 0.6f;
    s_ingame.resume._generic.caption.font = &uis.freeSansFont;
    s_ingame.resume._generic.caption.color = text_color_normal;
    s_ingame.resume._generic.caption.focuscolor = text_color_highlight;
    
    
    Menu_AddItem( &s_ingame.menu, &s_ingame.banner );
    Menu_AddItem( &s_ingame.menu, &s_ingame.team );
    Menu_AddItem( &s_ingame.menu, &s_ingame.addbots );
    Menu_AddItem( &s_ingame.menu, &s_ingame.removebots );
    
    if( ( trap_Cvar_VariableValue( "g_gametype" ) >= GT_TEAM ) )
        Menu_AddItem( &s_ingame.menu, &s_ingame.teamorders );
        
    Menu_AddItem( &s_ingame.menu, &s_ingame.setup );
    Menu_AddItem( &s_ingame.menu, &s_ingame.server );
    Menu_AddItem( &s_ingame.menu, &s_ingame.restart );
    Menu_AddItem( &s_ingame.menu, &s_ingame.resume );
    Menu_AddItem( &s_ingame.menu, &s_ingame.leave );
    Menu_AddItem( &s_ingame.menu, &s_ingame.quit );
}


/*
=================
InGame_Cache
=================
*/
void InGame_Cache( void )
{
    //trap_R_RegisterShaderNoMip(INGAME_FRAME);
}


/*
=================
UI_InGameMenu
=================
*/
void UI_InGameMenu( void )
{
    // force as top level menu
    uis.menusp = 0;
    
    // set menu cursor to a nice location
    uis.cursorx = 319;
    uis.cursory = 80;
    
    InGame_MenuInit();
    UI_PushMenu( &s_ingame.menu );
}
