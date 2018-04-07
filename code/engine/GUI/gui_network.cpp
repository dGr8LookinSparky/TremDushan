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

NETWORK OPTIONS MENU

=======================================================================
*/

#include "gui_local.h"

//#define ART_FRAMEL            "menu/art/frame2_l"
//#define ART_FRAMER            "menu/art/frame1_r"

#define ID_GRAPHICS			10
#define ID_DISPLAY			11
#define ID_SOUND			12
#define ID_NETWORK			13
#define ID_RATE				14
#define ID_PACKETDUP		15
#define ID_MAXPACKETS		16
#define ID_BACK				17

static const char* rate_items[] =
{
    "Modem",
    "ISDN",
    "Cable/xDSL/LAN",
    0
};

typedef struct
{
    menuframework_s menu;
    
    menutext_s      banner;
    //menubitmap_s    framel;
    //menubitmap_s    framer;
    
    menubitmap_s    graphics;
    //menubitmap_s      display;
    menubitmap_s    sound;
    menubitmap_s    network;
    
    menulist_s      rate;
    menuslider_s    packetdup;
    menuslider_s    maxpackets;
    
    menubitmap_s    back;
} networkOptionsInfo_t;

static networkOptionsInfo_t networkOptionsInfo;

/*
=================
UI_NetworkOptionsMenu_Event
=================
*/
static void UI_NetworkOptionsMenu_Event( void* ptr, int event )
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
            UI_PopMenu();
            UI_DisplayOptionsMenu();
            break;
            
        case ID_SOUND:
            UI_PopMenu();
            UI_SoundOptionsMenu();
            break;
            
        case ID_NETWORK:
            break;
            
        case ID_RATE:
            if( networkOptionsInfo.rate.curvalue == 0 )
                trap_Cvar_SetValue( "rate", 5500 );
            else if( networkOptionsInfo.rate.curvalue == 1 )
                trap_Cvar_SetValue( "rate", 14000 );
            else if( networkOptionsInfo.rate.curvalue == 2 )
                trap_Cvar_SetValue( "rate", 25000 );
            break;
            
        case ID_PACKETDUP:
            trap_Cvar_SetValue( "cl_packetdup", ( int )networkOptionsInfo.packetdup.curvalue );
            break;
            
        case ID_MAXPACKETS:
            trap_Cvar_SetValue( "cl_maxpackets", ( int )networkOptionsInfo.maxpackets.curvalue );
            break;
            
        case ID_BACK:
            UI_PopMenu();
            break;
    }
}

/*
===============
UI_NetworkOptionsMenu_Init
===============
*/
static void UI_NetworkOptionsMenu_Init( void )
{
    int             y;
    int             rate;
    
    memset( &networkOptionsInfo, 0, sizeof( networkOptionsInfo ) );
    
    UI_NetworkOptionsMenu_Cache();
    networkOptionsInfo.menu.wrapAround = true;
    networkOptionsInfo.menu.fullscreen = true;
    
    networkOptionsInfo.banner._generic.type = MTYPE_BTEXT;
    networkOptionsInfo.banner._generic.flags = QMF_CENTER_JUSTIFY;
    networkOptionsInfo.banner._generic.x = 320;
    networkOptionsInfo.banner._generic.y = 16;
    networkOptionsInfo.banner.string = "SYSTEM SETUP";
    networkOptionsInfo.banner.color = color_white;
    networkOptionsInfo.banner.style = UI_CENTER | UI_DROPSHADOW;
    
    /*	networkOptionsInfo.framel._generic.type = MTYPE_BITMAP;
    	networkOptionsInfo.framel._generic.name = ART_FRAMEL;
    	networkOptionsInfo.framel._generic.flags = QMF_INACTIVE;
    	networkOptionsInfo.framel._generic.x = 0;
    	networkOptionsInfo.framel._generic.y = 78;
    	networkOptionsInfo.framel.width = 256;
    	networkOptionsInfo.framel.height = 329;
    
    	networkOptionsInfo.framer._generic.type = MTYPE_BITMAP;
    	networkOptionsInfo.framer._generic.name = ART_FRAMER;
    	networkOptionsInfo.framer._generic.flags = QMF_INACTIVE;
    	networkOptionsInfo.framer._generic.x = 376;
    	networkOptionsInfo.framer._generic.y = 76;
    	networkOptionsInfo.framer.width = 256;
    	networkOptionsInfo.framer.height = 334;
    */
    
    
    /*	networkOptionsInfo.graphics._generic.type = MTYPE_PTEXT;
    	networkOptionsInfo.graphics._generic.flags = QMF_RIGHT_JUSTIFY | QMF_PULSEIFFOCUS;
    	networkOptionsInfo.graphics._generic.id = ID_GRAPHICS;
    	networkOptionsInfo.graphics._generic.callback = UI_NetworkOptionsMenu_Event;
    	networkOptionsInfo.graphics._generic.x = 216;
    	networkOptionsInfo.graphics._generic.y = 240 - 2 * PROP_HEIGHT;
    	networkOptionsInfo.graphics.string = "GRAPHICS";
    	networkOptionsInfo.graphics.style = UI_RIGHT;
    	networkOptionsInfo.graphics.color = color_red;
    
    	networkOptionsInfo.display._generic.type = MTYPE_PTEXT;
    	networkOptionsInfo.display._generic.flags = QMF_RIGHT_JUSTIFY | QMF_PULSEIFFOCUS;
    	networkOptionsInfo.display._generic.id = ID_DISPLAY;
    	networkOptionsInfo.display._generic.callback = UI_NetworkOptionsMenu_Event;
    	networkOptionsInfo.display._generic.x = 216;
    	networkOptionsInfo.display._generic.y = 240 - PROP_HEIGHT;
    	networkOptionsInfo.display.string = "DISPLAY";
    	networkOptionsInfo.display.style = UI_RIGHT;
    	networkOptionsInfo.display.color = color_red;
    
    	networkOptionsInfo.sound._generic.type = MTYPE_PTEXT;
    	networkOptionsInfo.sound._generic.flags = QMF_RIGHT_JUSTIFY | QMF_PULSEIFFOCUS;
    	networkOptionsInfo.sound._generic.id = ID_SOUND;
    	networkOptionsInfo.sound._generic.callback = UI_NetworkOptionsMenu_Event;
    	networkOptionsInfo.sound._generic.x = 216;
    	networkOptionsInfo.sound._generic.y = 240;
    	networkOptionsInfo.sound.string = "SOUND";
    	networkOptionsInfo.sound.style = UI_RIGHT;
    	networkOptionsInfo.sound.color = color_red;
    
    	networkOptionsInfo.network._generic.type = MTYPE_PTEXT;
    	networkOptionsInfo.network._generic.flags = QMF_RIGHT_JUSTIFY;
    	networkOptionsInfo.network._generic.id = ID_NETWORK;
    	networkOptionsInfo.network._generic.callback = UI_NetworkOptionsMenu_Event;
    	networkOptionsInfo.network._generic.x = 216;
    	networkOptionsInfo.network._generic.y = 240 + PROP_HEIGHT;
    	networkOptionsInfo.network.string = "NETWORK";
    	networkOptionsInfo.network.style = UI_RIGHT;
    	networkOptionsInfo.network.color = color_red;
    
    */
    
    networkOptionsInfo.graphics._generic.type = MTYPE_BITMAP;
    networkOptionsInfo.graphics._generic.name = UI_ART_BUTTON;
    networkOptionsInfo.graphics._generic.flags = QMF_LEFT_JUSTIFY | QMF_PULSEIFFOCUS;
    networkOptionsInfo.graphics._generic.callback = UI_NetworkOptionsMenu_Event;
    networkOptionsInfo.graphics._generic.id = ID_GRAPHICS;
    networkOptionsInfo.graphics._generic.x = 128;
    networkOptionsInfo.graphics._generic.y = 480 - 64;
    networkOptionsInfo.graphics.width = 128;
    networkOptionsInfo.graphics.height = 64;
    networkOptionsInfo.graphics.focuspic = UI_ART_BUTTON_FOCUS;
    networkOptionsInfo.graphics._generic.caption.text = "graphics";
    networkOptionsInfo.graphics._generic.caption.style = UI_CENTER | UI_DROPSHADOW;
    networkOptionsInfo.graphics._generic.caption.fontsize = 0.6f;
    networkOptionsInfo.graphics._generic.caption.font = &uis.freeSansFont;
    networkOptionsInfo.graphics._generic.caption.color = text_color_normal;
    networkOptionsInfo.graphics._generic.caption.focuscolor = text_color_highlight;
    
    
    networkOptionsInfo.sound._generic.type = MTYPE_BITMAP;
    networkOptionsInfo.sound._generic.name = UI_ART_BUTTON;
    networkOptionsInfo.sound._generic.flags = QMF_LEFT_JUSTIFY | QMF_PULSEIFFOCUS;
    networkOptionsInfo.sound._generic.callback = UI_NetworkOptionsMenu_Event;
    networkOptionsInfo.sound._generic.id = ID_SOUND;
    networkOptionsInfo.sound._generic.x = 256;
    networkOptionsInfo.sound._generic.y = 480 - 64;
    networkOptionsInfo.sound.width = 128;
    networkOptionsInfo.sound.height = 64;
    networkOptionsInfo.sound.focuspic = UI_ART_BUTTON_FOCUS;
    networkOptionsInfo.sound._generic.caption.text = "sound";
    networkOptionsInfo.sound._generic.caption.style = UI_CENTER | UI_DROPSHADOW;
    networkOptionsInfo.sound._generic.caption.fontsize = 0.6f;
    networkOptionsInfo.sound._generic.caption.font = &uis.freeSansFont;
    networkOptionsInfo.sound._generic.caption.color = text_color_normal;
    networkOptionsInfo.sound._generic.caption.focuscolor = text_color_highlight;
    
    networkOptionsInfo.network._generic.type = MTYPE_BITMAP;
    networkOptionsInfo.network._generic.name = UI_ART_BUTTON;
    networkOptionsInfo.network._generic.flags = QMF_LEFT_JUSTIFY | QMF_PULSEIFFOCUS;
    networkOptionsInfo.network._generic.callback = UI_NetworkOptionsMenu_Event;
    networkOptionsInfo.network._generic.id = ID_NETWORK;
    networkOptionsInfo.network._generic.x = 384;
    networkOptionsInfo.network._generic.y = 480 - 64;
    networkOptionsInfo.network.width = 128;
    networkOptionsInfo.network.height = 64;
    networkOptionsInfo.network.focuspic = UI_ART_BUTTON_FOCUS;
    networkOptionsInfo.network._generic.caption.text = "network";
    networkOptionsInfo.network._generic.caption.style = UI_CENTER | UI_DROPSHADOW;
    networkOptionsInfo.network._generic.caption.fontsize = 0.6f;
    networkOptionsInfo.network._generic.caption.font = &uis.freeSansFont;
    networkOptionsInfo.network._generic.caption.color = text_color_normal;
    networkOptionsInfo.network._generic.caption.focuscolor = text_color_highlight;
    
    
    y = 240 - 1.5 * ( BIGCHAR_HEIGHT + 2 );
    networkOptionsInfo.rate._generic.type = MTYPE_SPINCONTROL;
    networkOptionsInfo.rate._generic.name = "Data Rate:";
    networkOptionsInfo.rate._generic.flags = QMF_PULSEIFFOCUS | QMF_SMALLFONT;
    networkOptionsInfo.rate._generic.callback = UI_NetworkOptionsMenu_Event;
    networkOptionsInfo.rate._generic.id = ID_RATE;
    networkOptionsInfo.rate._generic.x = 320;
    networkOptionsInfo.rate._generic.y = y;
    networkOptionsInfo.rate.itemnames = rate_items;
    
    y += BIGCHAR_HEIGHT + 2;
    networkOptionsInfo.packetdup._generic.type = MTYPE_SLIDER;
    networkOptionsInfo.packetdup._generic.name = "Duplicate Packets:";
    networkOptionsInfo.packetdup._generic.flags = QMF_PULSEIFFOCUS | QMF_SMALLFONT;
    networkOptionsInfo.packetdup._generic.callback = UI_NetworkOptionsMenu_Event;
    networkOptionsInfo.packetdup._generic.id = ID_PACKETDUP;
    networkOptionsInfo.packetdup._generic.x = 320;
    networkOptionsInfo.packetdup._generic.y = y;
    networkOptionsInfo.packetdup.minvalue = 0;
    networkOptionsInfo.packetdup.maxvalue = 5;
    
    y += BIGCHAR_HEIGHT + 2;
    networkOptionsInfo.maxpackets._generic.type = MTYPE_SLIDER;
    networkOptionsInfo.maxpackets._generic.name = "Max. Packets:";
    networkOptionsInfo.maxpackets._generic.flags = QMF_PULSEIFFOCUS | QMF_SMALLFONT;
    networkOptionsInfo.maxpackets._generic.callback = UI_NetworkOptionsMenu_Event;
    networkOptionsInfo.maxpackets._generic.id = ID_MAXPACKETS;
    networkOptionsInfo.maxpackets._generic.x = 320;
    networkOptionsInfo.maxpackets._generic.y = y;
    networkOptionsInfo.maxpackets.minvalue = 1;
    networkOptionsInfo.maxpackets.maxvalue = 125;
    
    networkOptionsInfo.back._generic.type = MTYPE_BITMAP;
    networkOptionsInfo.back._generic.name = UI_ART_BUTTON;
    networkOptionsInfo.back._generic.flags = QMF_LEFT_JUSTIFY | QMF_PULSEIFFOCUS;
    networkOptionsInfo.back._generic.callback = UI_NetworkOptionsMenu_Event;
    networkOptionsInfo.back._generic.id = ID_BACK;
    networkOptionsInfo.back._generic.x = 0;
    networkOptionsInfo.back._generic.y = 480 - 64;
    networkOptionsInfo.back.width = 128;
    networkOptionsInfo.back.height = 64;
    networkOptionsInfo.back.focuspic = UI_ART_BUTTON_FOCUS;
    networkOptionsInfo.back._generic.caption.text = "back";
    networkOptionsInfo.back._generic.caption.style = UI_CENTER | UI_DROPSHADOW;
    networkOptionsInfo.back._generic.caption.fontsize = 0.6f;
    networkOptionsInfo.back._generic.caption.font = &uis.freeSansFont;
    networkOptionsInfo.back._generic.caption.color = text_color_normal;
    networkOptionsInfo.back._generic.caption.focuscolor = text_color_highlight;
    
    Menu_AddItem( &networkOptionsInfo.menu, ( void* )&networkOptionsInfo.banner );
    //Menu_AddItem(&networkOptionsInfo.menu, (void *)&networkOptionsInfo.framel);
    //Menu_AddItem(&networkOptionsInfo.menu, (void *)&networkOptionsInfo.framer);
    Menu_AddItem( &networkOptionsInfo.menu, ( void* )&networkOptionsInfo.graphics );
    //Menu_AddItem(&networkOptionsInfo.menu, (void *)&networkOptionsInfo.display);
    Menu_AddItem( &networkOptionsInfo.menu, ( void* )&networkOptionsInfo.sound );
    Menu_AddItem( &networkOptionsInfo.menu, ( void* )&networkOptionsInfo.network );
    Menu_AddItem( &networkOptionsInfo.menu, ( void* )&networkOptionsInfo.rate );
    Menu_AddItem( &networkOptionsInfo.menu, ( void* )&networkOptionsInfo.packetdup );
    Menu_AddItem( &networkOptionsInfo.menu, ( void* )&networkOptionsInfo.maxpackets );
    Menu_AddItem( &networkOptionsInfo.menu, ( void* )&networkOptionsInfo.back );
    
    rate = trap_Cvar_VariableValue( "rate" );
    if( rate <= 5500 )
        networkOptionsInfo.rate.curvalue = 0;
    else if( rate <= 14000 )
        networkOptionsInfo.rate.curvalue = 1;
    else
        networkOptionsInfo.rate.curvalue = 2;
        
    networkOptionsInfo.packetdup.curvalue = trap_Cvar_VariableValue( "cl_packetdup" );
    networkOptionsInfo.maxpackets.curvalue = trap_Cvar_VariableValue( "cl_maxpackets" );
}

/*
===============
UI_NetworkOptionsMenu_Cache
===============
*/
void UI_NetworkOptionsMenu_Cache( void )
{
    //trap_R_RegisterShaderNoMip(ART_FRAMEL);
    //trap_R_RegisterShaderNoMip(ART_FRAMER);
    
}

/*
===============
UI_NetworkOptionsMenu
===============
*/
void UI_NetworkOptionsMenu( void )
{
    UI_NetworkOptionsMenu_Init();
    UI_PushMenu( &networkOptionsInfo.menu );
    Menu_SetCursorToItem( &networkOptionsInfo.menu, &networkOptionsInfo.network );
}
