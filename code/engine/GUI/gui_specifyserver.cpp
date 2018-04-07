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
#include "gui_local.h"

/*********************************************************************************
	SPECIFY SERVER
*********************************************************************************/

#define SPECIFYSERVER_FRAMEL	"menu/art/frame2_l"
#define SPECIFYSERVER_FRAMER	"menu/art/frame1_r"
#define SPECIFYSERVER_BACK0		"menu/art/back_0"
#define SPECIFYSERVER_BACK1		"menu/art/back_1"
#define SPECIFYSERVER_FIGHT0	"menu/art/fight_0"
#define SPECIFYSERVER_FIGHT1	"menu/art/fight_1"

#define ID_SPECIFYSERVERBACK	102
#define ID_SPECIFYSERVERGO		103

static char*    specifyserver_artlist[] =
{
    SPECIFYSERVER_FRAMEL,
    SPECIFYSERVER_FRAMER,
    SPECIFYSERVER_BACK0,
    SPECIFYSERVER_BACK1,
    SPECIFYSERVER_FIGHT0,
    SPECIFYSERVER_FIGHT1,
    NULL
};

typedef struct
{
    menuframework_s menu;
    menutext_s      banner;
    menubitmap_s    framel;
    menubitmap_s    framer;
    menufield_s     domain;
    menufield_s     port;
    menubitmap_s    go;
    menubitmap_s    back;
} specifyserver_t;

static specifyserver_t s_specifyserver;

/*
=================
SpecifyServer_Event
=================
*/
static void SpecifyServer_Event( void* ptr, int event )
{
    char            buff[256];
    
    switch( ( ( menucommon_s* ) ptr )->id )
    {
        case ID_SPECIFYSERVERGO:
            if( event != QM_ACTIVATED )
                break;
                
            if( s_specifyserver.domain.field.buffer[0] )
            {
                strcpy( buff, s_specifyserver.domain.field.buffer );
                if( s_specifyserver.port.field.buffer[0] )
                    Com_sprintf( buff + strlen( buff ), 128, ":%s", s_specifyserver.port.field.buffer );
                    
                trap_Cmd_ExecuteText( EXEC_APPEND, va( "connect %s\n", buff ) );
            }
            break;
            
        case ID_SPECIFYSERVERBACK:
            if( event != QM_ACTIVATED )
                break;
                
            UI_PopMenu();
            break;
    }
}

/*
=================
SpecifyServer_MenuInit
=================
*/
void SpecifyServer_MenuInit( void )
{
    // zero set all our globals
    memset( &s_specifyserver, 0, sizeof( specifyserver_t ) );
    
    SpecifyServer_Cache();
    
    s_specifyserver.menu.wrapAround = true;
    s_specifyserver.menu.fullscreen = true;
    
    s_specifyserver.banner._generic.type = MTYPE_BTEXT;
    s_specifyserver.banner._generic.x = 320;
    s_specifyserver.banner._generic.y = 16;
    s_specifyserver.banner.string = "SPECIFY SERVER";
    s_specifyserver.banner.color = color_white;
    s_specifyserver.banner.style = UI_CENTER | UI_DROPSHADOW;
    
    /*	s_specifyserver.framel._generic.type = MTYPE_BITMAP;
    	s_specifyserver.framel._generic.name = SPECIFYSERVER_FRAMEL;
    	s_specifyserver.framel._generic.flags = QMF_INACTIVE;
    	s_specifyserver.framel._generic.x = 0;
    	s_specifyserver.framel._generic.y = 78;
    	s_specifyserver.framel.width = 256;
    	s_specifyserver.framel.height = 329;
    
    	s_specifyserver.framer._generic.type = MTYPE_BITMAP;
    	s_specifyserver.framer._generic.name = SPECIFYSERVER_FRAMER;
    	s_specifyserver.framer._generic.flags = QMF_INACTIVE;
    	s_specifyserver.framer._generic.x = 376;
    	s_specifyserver.framer._generic.y = 76;
    	s_specifyserver.framer.width = 256;
    	s_specifyserver.framer.height = 334;
    */
    s_specifyserver.domain._generic.type = MTYPE_FIELD;
    s_specifyserver.domain._generic.name = "Address:";
    s_specifyserver.domain._generic.flags = QMF_PULSEIFFOCUS | QMF_SMALLFONT;
    s_specifyserver.domain._generic.x = 206;
    s_specifyserver.domain._generic.y = 220;
    s_specifyserver.domain.field.widthInChars = 38;
    s_specifyserver.domain.field.maxchars = 80;
    
    s_specifyserver.port._generic.type = MTYPE_FIELD;
    s_specifyserver.port._generic.name = "Port:";
    s_specifyserver.port._generic.flags = QMF_PULSEIFFOCUS | QMF_SMALLFONT | QMF_NUMBERSONLY;
    s_specifyserver.port._generic.x = 206;
    s_specifyserver.port._generic.y = 250;
    s_specifyserver.port.field.widthInChars = 6;
    s_specifyserver.port.field.maxchars = 5;
    
    s_specifyserver.go._generic.type = MTYPE_BITMAP;
    s_specifyserver.go._generic.name = UI_ART_BUTTON;
    s_specifyserver.go._generic.flags = QMF_RIGHT_JUSTIFY | QMF_PULSEIFFOCUS;
    s_specifyserver.go._generic.callback = SpecifyServer_Event;
    s_specifyserver.go._generic.id = ID_SPECIFYSERVERGO;
    s_specifyserver.go._generic.x = 640;
    s_specifyserver.go._generic.y = 480 - 64;
    s_specifyserver.go.width = 128;
    s_specifyserver.go.height = 64;
    s_specifyserver.go.focuspic = UI_ART_BUTTON_FOCUS;
    s_specifyserver.go._generic.caption.text = "specify";
    s_specifyserver.go._generic.caption.style = UI_CENTER | UI_DROPSHADOW;
    s_specifyserver.go._generic.caption.fontsize = 0.6f;
    s_specifyserver.go._generic.caption.font = &uis.freeSansFont;
    s_specifyserver.go._generic.caption.color = text_color_normal;
    s_specifyserver.go._generic.caption.focuscolor = text_color_highlight;
    
    s_specifyserver.back._generic.type = MTYPE_BITMAP;
    s_specifyserver.back._generic.name = UI_ART_BUTTON;
    s_specifyserver.back._generic.flags = QMF_LEFT_JUSTIFY | QMF_PULSEIFFOCUS;
    s_specifyserver.back._generic.callback = SpecifyServer_Event;
    s_specifyserver.back._generic.id = ID_SPECIFYSERVERBACK;
    s_specifyserver.back._generic.x = 0;
    s_specifyserver.back._generic.y = 480 - 64;
    s_specifyserver.back.width = 128;
    s_specifyserver.back.height = 64;
    s_specifyserver.back.focuspic = UI_ART_BUTTON_FOCUS;
    s_specifyserver.back._generic.caption.text = "back";
    s_specifyserver.back._generic.caption.style = UI_CENTER | UI_DROPSHADOW;
    s_specifyserver.back._generic.caption.fontsize = 0.6f;
    s_specifyserver.back._generic.caption.font = &uis.freeSansFont;
    s_specifyserver.back._generic.caption.color = text_color_normal;
    s_specifyserver.back._generic.caption.focuscolor = text_color_highlight;
    
    Menu_AddItem( &s_specifyserver.menu, &s_specifyserver.banner );
    //Menu_AddItem(&s_specifyserver.menu, &s_specifyserver.framel);
    //Menu_AddItem(&s_specifyserver.menu, &s_specifyserver.framer);
    Menu_AddItem( &s_specifyserver.menu, &s_specifyserver.domain );
    Menu_AddItem( &s_specifyserver.menu, &s_specifyserver.port );
    Menu_AddItem( &s_specifyserver.menu, &s_specifyserver.go );
    Menu_AddItem( &s_specifyserver.menu, &s_specifyserver.back );
    
    Com_sprintf( s_specifyserver.port.field.buffer, 6, "%i", 27960 );
}

/*
=================
SpecifyServer_Cache
=================
*/
void SpecifyServer_Cache( void )
{
    int             i;
    
    // touch all our pics
    for( i = 0;; i++ )
    {
        if( !specifyserver_artlist[i] )
            break;
        trap_R_RegisterShaderNoMip( specifyserver_artlist[i] );
    }
}

/*
=================
UI_SpecifyServerMenu
=================
*/
void UI_SpecifyServerMenu( void )
{
    SpecifyServer_MenuInit();
    UI_PushMenu( &s_specifyserver.menu );
}
