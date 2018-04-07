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

SOUND OPTIONS MENU

=======================================================================
*/

#include "gui_local.h"

/*
#define ART_FRAMEL			"menu/art/frame2_l"
#define ART_FRAMER			"menu/art/frame1_r"
*/
#define ID_GRAPHICS			10
#define ID_DISPLAY			11
#define ID_SOUND			12
#define ID_NETWORK			13
#define ID_EFFECTSVOLUME	14
#define ID_MUSICVOLUME		15
#define ID_QUALITY			16
#define ID_OPENAL			17
#define ID_BACK				18


static const char* quality_items[] =
{
    "Low", "High", 0
};

typedef struct
{
    menuframework_s menu;
    
    menutext_s      banner;
    //menubitmap_s    framel;
    //menubitmap_s    framer;
    
    menubitmap_s    graphics;
    //menutext_s      display;
    menubitmap_s    sound;
    menubitmap_s    network;
    
    menuslider_s    sfxvolume;
    menuslider_s    musicvolume;
    menulist_s      quality;
    menuradiobutton_s openal;
    
    menubitmap_s    back;
} soundOptionsInfo_t;

static soundOptionsInfo_t soundOptionsInfo;


/*
=================
UI_SoundOptionsMenu_Event
=================
*/
static void UI_SoundOptionsMenu_Event( void* ptr, int event )
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
            break;
            
        case ID_NETWORK:
            UI_PopMenu();
            UI_NetworkOptionsMenu();
            break;
            
        case ID_EFFECTSVOLUME:
            trap_Cvar_SetValue( "s_volume", soundOptionsInfo.sfxvolume.curvalue / 10 );
            break;
            
        case ID_MUSICVOLUME:
            trap_Cvar_SetValue( "s_musicvolume", soundOptionsInfo.musicvolume.curvalue / 10 );
            break;
            
        case ID_QUALITY:
            if( soundOptionsInfo.quality.curvalue )
            {
                trap_Cvar_SetValue( "s_khz", 22 );
                trap_Cvar_SetValue( "s_compression", 0 );
            }
            else
            {
                trap_Cvar_SetValue( "s_khz", 11 );
                trap_Cvar_SetValue( "s_compression", 1 );
            }
            UI_ForceMenuOff();
            trap_Cmd_ExecuteText( EXEC_APPEND, "snd_restart\n" );
            break;
            
        case ID_OPENAL:
            if( soundOptionsInfo.openal.curvalue )
                trap_Cvar_SetValue( "s_useOpenAL", 1 );
            else
                trap_Cvar_SetValue( "s_useOpenAL", 0 );
            UI_ForceMenuOff();
            trap_Cmd_ExecuteText( EXEC_APPEND, "snd_restart\n" );
            break;
            
        case ID_BACK:
            UI_PopMenu();
            break;
    }
}


/*
===============
UI_SoundOptionsMenu_Init
===============
*/
static void UI_SoundOptionsMenu_Init( void )
{
    int             y;
    
    memset( &soundOptionsInfo, 0, sizeof( soundOptionsInfo ) );
    
    UI_SoundOptionsMenu_Cache();
    soundOptionsInfo.menu.wrapAround = true;
    soundOptionsInfo.menu.fullscreen = true;
    
    soundOptionsInfo.banner._generic.type = MTYPE_BTEXT;
    soundOptionsInfo.banner._generic.flags = QMF_CENTER_JUSTIFY;
    soundOptionsInfo.banner._generic.x = 320;
    soundOptionsInfo.banner._generic.y = 16;
    soundOptionsInfo.banner.string = "SYSTEM SETUP";
    soundOptionsInfo.banner.color = color_white;
    soundOptionsInfo.banner.style = UI_CENTER | UI_DROPSHADOW;
    
    soundOptionsInfo.graphics._generic.type = MTYPE_BITMAP;
    soundOptionsInfo.graphics._generic.name = UI_ART_BUTTON;
    soundOptionsInfo.graphics._generic.flags = QMF_LEFT_JUSTIFY | QMF_PULSEIFFOCUS;
    soundOptionsInfo.graphics._generic.callback = UI_SoundOptionsMenu_Event;
    soundOptionsInfo.graphics._generic.id = ID_GRAPHICS;
    soundOptionsInfo.graphics._generic.x = 128;
    soundOptionsInfo.graphics._generic.y = 480 - 64;
    soundOptionsInfo.graphics.width = 128;
    soundOptionsInfo.graphics.height = 64;
    soundOptionsInfo.graphics.focuspic = UI_ART_BUTTON_FOCUS;
    soundOptionsInfo.graphics._generic.caption.text = "graphics";
    soundOptionsInfo.graphics._generic.caption.style = UI_CENTER | UI_DROPSHADOW;
    soundOptionsInfo.graphics._generic.caption.fontsize = 0.6f;
    soundOptionsInfo.graphics._generic.caption.font = &uis.freeSansFont;
    soundOptionsInfo.graphics._generic.caption.color = text_color_normal;
    soundOptionsInfo.graphics._generic.caption.focuscolor = text_color_highlight;
    
    
    soundOptionsInfo.sound._generic.type = MTYPE_BITMAP;
    soundOptionsInfo.sound._generic.name = UI_ART_BUTTON;
    soundOptionsInfo.sound._generic.flags = QMF_LEFT_JUSTIFY | QMF_PULSEIFFOCUS;
    soundOptionsInfo.sound._generic.callback = UI_SoundOptionsMenu_Event;
    soundOptionsInfo.sound._generic.id = ID_SOUND;
    soundOptionsInfo.sound._generic.x = 256;
    soundOptionsInfo.sound._generic.y = 480 - 64;
    soundOptionsInfo.sound.width = 128;
    soundOptionsInfo.sound.height = 64;
    soundOptionsInfo.sound.focuspic = UI_ART_BUTTON_FOCUS;
    soundOptionsInfo.sound._generic.caption.text = "sound";
    soundOptionsInfo.sound._generic.caption.style = UI_CENTER | UI_DROPSHADOW;
    soundOptionsInfo.sound._generic.caption.fontsize = 0.6f;
    soundOptionsInfo.sound._generic.caption.font = &uis.freeSansFont;
    soundOptionsInfo.sound._generic.caption.color = text_color_normal;
    soundOptionsInfo.sound._generic.caption.focuscolor = text_color_highlight;
    
    soundOptionsInfo.network._generic.type = MTYPE_BITMAP;
    soundOptionsInfo.network._generic.name = UI_ART_BUTTON;
    soundOptionsInfo.network._generic.flags = QMF_LEFT_JUSTIFY | QMF_PULSEIFFOCUS;
    soundOptionsInfo.network._generic.callback = UI_SoundOptionsMenu_Event;
    soundOptionsInfo.network._generic.id = ID_NETWORK;
    soundOptionsInfo.network._generic.x = 384;
    soundOptionsInfo.network._generic.y = 480 - 64;
    soundOptionsInfo.network.width = 128;
    soundOptionsInfo.network.height = 64;
    soundOptionsInfo.network.focuspic = UI_ART_BUTTON_FOCUS;
    soundOptionsInfo.network._generic.caption.text = "network";
    soundOptionsInfo.network._generic.caption.style = UI_CENTER | UI_DROPSHADOW;
    soundOptionsInfo.network._generic.caption.fontsize = 0.6f;
    soundOptionsInfo.network._generic.caption.font = &uis.freeSansFont;
    soundOptionsInfo.network._generic.caption.color = text_color_normal;
    soundOptionsInfo.network._generic.caption.focuscolor = text_color_highlight;
    
    
    y = 240 - 1.5 * ( BIGCHAR_HEIGHT + 2 );
    soundOptionsInfo.sfxvolume._generic.type = MTYPE_SLIDER;
    soundOptionsInfo.sfxvolume._generic.name = "Effects Volume:";
    soundOptionsInfo.sfxvolume._generic.flags = QMF_PULSEIFFOCUS | QMF_SMALLFONT;
    soundOptionsInfo.sfxvolume._generic.callback = UI_SoundOptionsMenu_Event;
    soundOptionsInfo.sfxvolume._generic.id = ID_EFFECTSVOLUME;
    soundOptionsInfo.sfxvolume._generic.x = 320;
    soundOptionsInfo.sfxvolume._generic.y = y;
    soundOptionsInfo.sfxvolume.minvalue = 0;
    soundOptionsInfo.sfxvolume.maxvalue = 10;
    
    y += BIGCHAR_HEIGHT + 2;
    soundOptionsInfo.musicvolume._generic.type = MTYPE_SLIDER;
    soundOptionsInfo.musicvolume._generic.name = "Music Volume:";
    soundOptionsInfo.musicvolume._generic.flags = QMF_PULSEIFFOCUS | QMF_SMALLFONT;
    soundOptionsInfo.musicvolume._generic.callback = UI_SoundOptionsMenu_Event;
    soundOptionsInfo.musicvolume._generic.id = ID_MUSICVOLUME;
    soundOptionsInfo.musicvolume._generic.x = 320;
    soundOptionsInfo.musicvolume._generic.y = y;
    soundOptionsInfo.musicvolume.minvalue = 0;
    soundOptionsInfo.musicvolume.maxvalue = 10;
    
    y += BIGCHAR_HEIGHT + 2;
    soundOptionsInfo.quality._generic.type = MTYPE_SPINCONTROL;
    soundOptionsInfo.quality._generic.name = "Sound Quality:";
    soundOptionsInfo.quality._generic.flags = QMF_PULSEIFFOCUS | QMF_SMALLFONT;
    soundOptionsInfo.quality._generic.callback = UI_SoundOptionsMenu_Event;
    soundOptionsInfo.quality._generic.id = ID_QUALITY;
    soundOptionsInfo.quality._generic.x = 320;
    soundOptionsInfo.quality._generic.y = y;
    soundOptionsInfo.quality.itemnames = quality_items;
    
    y += BIGCHAR_HEIGHT + 2;
    soundOptionsInfo.openal._generic.type = MTYPE_RADIOBUTTON;
    soundOptionsInfo.openal._generic.name = "OpenAL:";
    soundOptionsInfo.openal._generic.flags = QMF_PULSEIFFOCUS | QMF_SMALLFONT;
    soundOptionsInfo.openal._generic.callback = UI_SoundOptionsMenu_Event;
    soundOptionsInfo.openal._generic.id = ID_OPENAL;
    soundOptionsInfo.openal._generic.x = 320;
    soundOptionsInfo.openal._generic.y = y;
    
    soundOptionsInfo.back._generic.type = MTYPE_BITMAP;
    soundOptionsInfo.back._generic.name = UI_ART_BUTTON;
    soundOptionsInfo.back._generic.flags = QMF_LEFT_JUSTIFY | QMF_PULSEIFFOCUS;
    soundOptionsInfo.back._generic.callback = UI_SoundOptionsMenu_Event;
    soundOptionsInfo.back._generic.id = ID_BACK;
    soundOptionsInfo.back._generic.x = 0;
    soundOptionsInfo.back._generic.y = 480 - 64;
    soundOptionsInfo.back.width = 128;
    soundOptionsInfo.back.height = 64;
    soundOptionsInfo.back.focuspic = UI_ART_BUTTON_FOCUS;
    soundOptionsInfo.back._generic.caption.text = "back";
    soundOptionsInfo.back._generic.caption.style = UI_CENTER | UI_DROPSHADOW;
    soundOptionsInfo.back._generic.caption.fontsize = 0.6f;
    soundOptionsInfo.back._generic.caption.font = &uis.freeSansFont;
    soundOptionsInfo.back._generic.caption.color = text_color_normal;
    soundOptionsInfo.back._generic.caption.focuscolor = text_color_highlight;
    
    Menu_AddItem( &soundOptionsInfo.menu, ( void* )&soundOptionsInfo.banner );
    //Menu_AddItem(&soundOptionsInfo.menu, (void *)&soundOptionsInfo.framel);
    //Menu_AddItem(&soundOptionsInfo.menu, (void *)&soundOptionsInfo.framer);
    Menu_AddItem( &soundOptionsInfo.menu, ( void* )&soundOptionsInfo.graphics );
    //Menu_AddItem(&soundOptionsInfo.menu, (void *)&soundOptionsInfo.display);
    Menu_AddItem( &soundOptionsInfo.menu, ( void* )&soundOptionsInfo.sound );
    Menu_AddItem( &soundOptionsInfo.menu, ( void* )&soundOptionsInfo.network );
    Menu_AddItem( &soundOptionsInfo.menu, ( void* )&soundOptionsInfo.sfxvolume );
    Menu_AddItem( &soundOptionsInfo.menu, ( void* )&soundOptionsInfo.musicvolume );
    Menu_AddItem( &soundOptionsInfo.menu, ( void* )&soundOptionsInfo.quality );
    Menu_AddItem( &soundOptionsInfo.menu, ( void* )&soundOptionsInfo.openal );
    Menu_AddItem( &soundOptionsInfo.menu, ( void* )&soundOptionsInfo.back );
    
    soundOptionsInfo.sfxvolume.curvalue = trap_Cvar_VariableValue( "s_volume" ) * 10;
    soundOptionsInfo.musicvolume.curvalue = trap_Cvar_VariableValue( "s_musicvolume" ) * 10;
    soundOptionsInfo.quality.curvalue = !trap_Cvar_VariableValue( "s_compression" );
    soundOptionsInfo.openal.curvalue = trap_Cvar_VariableValue( "s_useOpenAL" );
}


/*
===============
UI_SoundOptionsMenu_Cache
===============
*/
void UI_SoundOptionsMenu_Cache( void )
{
    //trap_R_RegisterShaderNoMip(ART_FRAMEL);
    //trap_R_RegisterShaderNoMip(ART_FRAMER);
    
}

/*
===============
UI_SoundOptionsMenu
===============
*/
void UI_SoundOptionsMenu( void )
{
    UI_SoundOptionsMenu_Init();
    UI_PushMenu( &soundOptionsInfo.menu );
    Menu_SetCursorToItem( &soundOptionsInfo.menu, &soundOptionsInfo.sound );
}
