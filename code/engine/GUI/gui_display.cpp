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

DISPLAY OPTIONS MENU

otty: display options are merged with the video options now.

=======================================================================
*/

#include "gui_local.h"


#define ART_FRAMEL			"menu/art/frame2_l"
#define ART_FRAMER			"menu/art/frame1_r"

#define ID_GRAPHICS			10
#define ID_DISPLAY			11
#define ID_SOUND			12
#define ID_NETWORK			13
#define ID_BRIGHTNESS		14
#define ID_SCREENSIZE		15
#define ID_BACK				16


typedef struct
{
    menuframework_s menu;
    
    menutext_s      banner;
    menubitmap_s    framel;
    menubitmap_s    framer;
    
    menutext_s      graphics;
    menutext_s      display;
    menutext_s      sound;
    menutext_s      network;
    
    menuslider_s    brightness;
    menuslider_s    screensize;
    
    menubitmap_s    back;
} displayOptionsInfo_t;

static displayOptionsInfo_t displayOptionsInfo;


/*
=================
UI_DisplayOptionsMenu_Event
=================
*/
static void UI_DisplayOptionsMenu_Event( void* ptr, int event )
{
    if( event != QM_ACTIVATED )
    {
        return;
    }
    
    switch( ( ( menucommon_s* ) ptr )->id )
    {
        case ID_GRAPHICS:
            UI_PopMenu();
            UI_GraphicsOptionsMenu();
            break;
            
        case ID_DISPLAY:
            break;
            
        case ID_SOUND:
            UI_PopMenu();
            UI_SoundOptionsMenu();
            break;
            
        case ID_NETWORK:
            UI_PopMenu();
            UI_NetworkOptionsMenu();
            break;
            
        case ID_BRIGHTNESS:
            trap_Cvar_SetValue( "r_gamma", displayOptionsInfo.brightness.curvalue / 10.0f );
            break;
            
        case ID_SCREENSIZE:
            trap_Cvar_SetValue( "cg_viewsize", displayOptionsInfo.screensize.curvalue * 10 );
            break;
            
        case ID_BACK:
            UI_PopMenu();
            break;
    }
}


/*
===============
UI_DisplayOptionsMenu_Init
===============
*/
static void UI_DisplayOptionsMenu_Init( void )
{
    int             y;
    
    memset( &displayOptionsInfo, 0, sizeof( displayOptionsInfo ) );
    
    UI_DisplayOptionsMenu_Cache();
    displayOptionsInfo.menu.wrapAround = true;
    displayOptionsInfo.menu.fullscreen = true;
    
    displayOptionsInfo.banner._generic.type = MTYPE_BTEXT;
    displayOptionsInfo.banner._generic.flags = QMF_CENTER_JUSTIFY;
    displayOptionsInfo.banner._generic.x = 320;
    displayOptionsInfo.banner._generic.y = 16;
    displayOptionsInfo.banner.string = "SYSTEM SETUP";
    displayOptionsInfo.banner.color = color_white;
    displayOptionsInfo.banner.style = UI_CENTER | UI_DROPSHADOW;
    
    displayOptionsInfo.framel._generic.type = MTYPE_BITMAP;
    displayOptionsInfo.framel._generic.name = ART_FRAMEL;
    displayOptionsInfo.framel._generic.flags = QMF_INACTIVE;
    displayOptionsInfo.framel._generic.x = 0;
    displayOptionsInfo.framel._generic.y = 78;
    displayOptionsInfo.framel.width = 256;
    displayOptionsInfo.framel.height = 329;
    
    displayOptionsInfo.framer._generic.type = MTYPE_BITMAP;
    displayOptionsInfo.framer._generic.name = ART_FRAMER;
    displayOptionsInfo.framer._generic.flags = QMF_INACTIVE;
    displayOptionsInfo.framer._generic.x = 376;
    displayOptionsInfo.framer._generic.y = 76;
    displayOptionsInfo.framer.width = 256;
    displayOptionsInfo.framer.height = 334;
    
    displayOptionsInfo.graphics._generic.type = MTYPE_PTEXT;
    displayOptionsInfo.graphics._generic.flags = QMF_RIGHT_JUSTIFY | QMF_PULSEIFFOCUS;
    displayOptionsInfo.graphics._generic.id = ID_GRAPHICS;
    displayOptionsInfo.graphics._generic.callback = UI_DisplayOptionsMenu_Event;
    displayOptionsInfo.graphics._generic.x = 216;
    displayOptionsInfo.graphics._generic.y = 240 - 2 * PROP_HEIGHT;
    displayOptionsInfo.graphics.string = "GRAPHICS";
    displayOptionsInfo.graphics.style = UI_RIGHT;
    displayOptionsInfo.graphics.color = color_red;
    
    displayOptionsInfo.display._generic.type = MTYPE_PTEXT;
    displayOptionsInfo.display._generic.flags = QMF_RIGHT_JUSTIFY;
    displayOptionsInfo.display._generic.id = ID_DISPLAY;
    displayOptionsInfo.display._generic.callback = UI_DisplayOptionsMenu_Event;
    displayOptionsInfo.display._generic.x = 216;
    displayOptionsInfo.display._generic.y = 240 - PROP_HEIGHT;
    displayOptionsInfo.display.string = "DISPLAY";
    displayOptionsInfo.display.style = UI_RIGHT;
    displayOptionsInfo.display.color = color_red;
    
    displayOptionsInfo.sound._generic.type = MTYPE_PTEXT;
    displayOptionsInfo.sound._generic.flags = QMF_RIGHT_JUSTIFY | QMF_PULSEIFFOCUS;
    displayOptionsInfo.sound._generic.id = ID_SOUND;
    displayOptionsInfo.sound._generic.callback = UI_DisplayOptionsMenu_Event;
    displayOptionsInfo.sound._generic.x = 216;
    displayOptionsInfo.sound._generic.y = 240;
    displayOptionsInfo.sound.string = "SOUND";
    displayOptionsInfo.sound.style = UI_RIGHT;
    displayOptionsInfo.sound.color = color_red;
    
    displayOptionsInfo.network._generic.type = MTYPE_PTEXT;
    displayOptionsInfo.network._generic.flags = QMF_RIGHT_JUSTIFY | QMF_PULSEIFFOCUS;
    displayOptionsInfo.network._generic.id = ID_NETWORK;
    displayOptionsInfo.network._generic.callback = UI_DisplayOptionsMenu_Event;
    displayOptionsInfo.network._generic.x = 216;
    displayOptionsInfo.network._generic.y = 240 + PROP_HEIGHT;
    displayOptionsInfo.network.string = "NETWORK";
    displayOptionsInfo.network.style = UI_RIGHT;
    displayOptionsInfo.network.color = color_red;
    
    y = 240 - 1 * ( BIGCHAR_HEIGHT + 2 );
    displayOptionsInfo.brightness._generic.type = MTYPE_SLIDER;
    displayOptionsInfo.brightness._generic.name = "Brightness:";
    displayOptionsInfo.brightness._generic.flags = QMF_PULSEIFFOCUS | QMF_SMALLFONT;
    displayOptionsInfo.brightness._generic.callback = UI_DisplayOptionsMenu_Event;
    displayOptionsInfo.brightness._generic.id = ID_BRIGHTNESS;
    displayOptionsInfo.brightness._generic.x = 400;
    displayOptionsInfo.brightness._generic.y = y;
    displayOptionsInfo.brightness.minvalue = 5;
    displayOptionsInfo.brightness.maxvalue = 20;
    if( !uis.glconfig.deviceSupportsGamma )
    {
        displayOptionsInfo.brightness._generic.flags |= QMF_GRAYED;
    }
    
    y += BIGCHAR_HEIGHT + 2;
    displayOptionsInfo.screensize._generic.type = MTYPE_SLIDER;
    displayOptionsInfo.screensize._generic.name = "Screen Size:";
    displayOptionsInfo.screensize._generic.flags = QMF_PULSEIFFOCUS | QMF_SMALLFONT;
    displayOptionsInfo.screensize._generic.callback = UI_DisplayOptionsMenu_Event;
    displayOptionsInfo.screensize._generic.id = ID_SCREENSIZE;
    displayOptionsInfo.screensize._generic.x = 400;
    displayOptionsInfo.screensize._generic.y = y;
    displayOptionsInfo.screensize.minvalue = 3;
    displayOptionsInfo.screensize.maxvalue = 10;
    
    displayOptionsInfo.back._generic.type = MTYPE_BITMAP;
    displayOptionsInfo.back._generic.name = UI_ART_BUTTON;
    displayOptionsInfo.back._generic.flags = QMF_LEFT_JUSTIFY | QMF_PULSEIFFOCUS;
    displayOptionsInfo.back._generic.callback = UI_DisplayOptionsMenu_Event;
    displayOptionsInfo.back._generic.id = ID_BACK;
    displayOptionsInfo.back._generic.x = 0;
    displayOptionsInfo.back._generic.y = 480 - 64;
    displayOptionsInfo.back.width = 128;
    displayOptionsInfo.back.height = 64;
    displayOptionsInfo.back.focuspic = UI_ART_BUTTON_FOCUS;
    displayOptionsInfo.back._generic.caption.text = "back";
    displayOptionsInfo.back._generic.caption.style = UI_CENTER | UI_DROPSHADOW;
    displayOptionsInfo.back._generic.caption.fontsize = 0.6f;
    displayOptionsInfo.back._generic.caption.font = &uis.freeSansFont;
    displayOptionsInfo.back._generic.caption.color = text_color_normal;
    displayOptionsInfo.back._generic.caption.focuscolor = text_color_highlight;
    
    
    Menu_AddItem( &displayOptionsInfo.menu, ( void* )&displayOptionsInfo.banner );
    Menu_AddItem( &displayOptionsInfo.menu, ( void* )&displayOptionsInfo.framel );
    Menu_AddItem( &displayOptionsInfo.menu, ( void* )&displayOptionsInfo.framer );
    Menu_AddItem( &displayOptionsInfo.menu, ( void* )&displayOptionsInfo.graphics );
    Menu_AddItem( &displayOptionsInfo.menu, ( void* )&displayOptionsInfo.display );
    Menu_AddItem( &displayOptionsInfo.menu, ( void* )&displayOptionsInfo.sound );
    Menu_AddItem( &displayOptionsInfo.menu, ( void* )&displayOptionsInfo.network );
    Menu_AddItem( &displayOptionsInfo.menu, ( void* )&displayOptionsInfo.brightness );
    Menu_AddItem( &displayOptionsInfo.menu, ( void* )&displayOptionsInfo.screensize );
    Menu_AddItem( &displayOptionsInfo.menu, ( void* )&displayOptionsInfo.back );
    
    displayOptionsInfo.brightness.curvalue = trap_Cvar_VariableValue( "r_gamma" ) * 10;
    displayOptionsInfo.screensize.curvalue = trap_Cvar_VariableValue( "cg_viewsize" ) / 10;
}


/*
===============
UI_DisplayOptionsMenu_Cache
===============
*/
void UI_DisplayOptionsMenu_Cache( void )
{
    trap_R_RegisterShaderNoMip( ART_FRAMEL );
    trap_R_RegisterShaderNoMip( ART_FRAMER );
    
}


/*
===============
UI_DisplayOptionsMenu
===============
*/
void UI_DisplayOptionsMenu( void )
{
    UI_DisplayOptionsMenu_Init();
    UI_PushMenu( &displayOptionsInfo.menu );
    Menu_SetCursorToItem( &displayOptionsInfo.menu, &displayOptionsInfo.display );
}
