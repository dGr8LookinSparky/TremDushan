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
=============================================================================

SAVE CONFIG MENU

=============================================================================
*/

#include "gui_local.h"


#define ART_SAVE0			"menu/art/save_0"
#define ART_SAVE1			"menu/art/save_1"
#define ART_BACKGROUND		"menu/art/cut_frame"

#define ID_NAME			10
#define ID_BACK			11
#define ID_SAVE			12


typedef struct
{
    menuframework_s menu;
    
    menutext_s      banner;
    menubitmap_s    background;
    menufield_s     savename;
    menubitmap_s    back;
    menubitmap_s    save;
} saveConfig_t;

static saveConfig_t saveConfig;


/*
===============
UI_SaveConfigMenu_BackEvent
===============
*/
static void UI_SaveConfigMenu_BackEvent( void* ptr, int event )
{
    if( event != QM_ACTIVATED )
    {
        return;
    }
    
    UI_PopMenu();
}


/*
===============
UI_SaveConfigMenu_SaveEvent
===============
*/
static void UI_SaveConfigMenu_SaveEvent( void* ptr, int event )
{
    char            configname[MAX_QPATH];
    
    if( event != QM_ACTIVATED )
    {
        return;
    }
    
    if( !saveConfig.savename.field.buffer[0] )
    {
        return;
    }
    
    Com_StripExtension( saveConfig.savename.field.buffer, configname, sizeof( configname ) );
    trap_Cmd_ExecuteText( EXEC_APPEND, va( "writeconfig %s.cfg\n", configname ) );
    UI_PopMenu();
}


/*
===============
UI_SaveConfigMenu_SavenameDraw
===============
*/
static void UI_SaveConfigMenu_SavenameDraw( void* self )
{
    menufield_s*    f;
    int             style;
    float*          color;
    
    f = ( menufield_s* ) self;
    
    if( f == Menu_ItemAtCursor( &saveConfig.menu ) )
    {
        style = UI_LEFT | UI_PULSE | UI_SMALLFONT;
        color = text_color_highlight;
    }
    else
    {
        style = UI_LEFT | UI_SMALLFONT;
        color = colorRed;
    }
    
    UI_DrawProportionalString( 320, 192, "Enter filename:", UI_CENTER | UI_SMALLFONT, color_orange );
    UI_FillRect( f->_generic.x, f->_generic.y, f->field.widthInChars * SMALLCHAR_WIDTH, SMALLCHAR_HEIGHT, colorBlack );
    MField_Draw( &f->field, f->_generic.x, f->_generic.y, style, color );
}


/*
=================
UI_SaveConfigMenu_Init
=================
*/
static void UI_SaveConfigMenu_Init( void )
{
    memset( &saveConfig, 0, sizeof( saveConfig ) );
    
    UI_SaveConfigMenu_Cache();
    saveConfig.menu.wrapAround = true;
    saveConfig.menu.fullscreen = true;
    
    saveConfig.banner._generic.type = MTYPE_BTEXT;
    saveConfig.banner._generic.x = 320;
    saveConfig.banner._generic.y = 16;
    saveConfig.banner.string = "SAVE CONFIG";
    saveConfig.banner.color = color_white;
    saveConfig.banner.style = UI_CENTER | UI_DROPSHADOW;
    
    saveConfig.background._generic.type = MTYPE_BITMAP;
    saveConfig.background._generic.name = ART_BACKGROUND;
    saveConfig.background._generic.flags = QMF_INACTIVE;
    saveConfig.background._generic.x = 142;
    saveConfig.background._generic.y = 118;
    saveConfig.background.width = 359;
    saveConfig.background.height = 256;
    
    saveConfig.savename._generic.type = MTYPE_FIELD;
    saveConfig.savename._generic.flags = QMF_NODEFAULTINIT | QMF_UPPERCASE;
    saveConfig.savename._generic.ownerdraw = UI_SaveConfigMenu_SavenameDraw;
    saveConfig.savename.field.widthInChars = 20;
    saveConfig.savename.field.maxchars = 20;
    saveConfig.savename._generic.x = 240;
    saveConfig.savename._generic.y = 155 + 72;
    saveConfig.savename._generic.left = 240;
    saveConfig.savename._generic.top = 155 + 72;
    saveConfig.savename._generic.right = 233 + 20 * SMALLCHAR_WIDTH;
    saveConfig.savename._generic.bottom = 155 + 72 + SMALLCHAR_HEIGHT + 2;
    
    saveConfig.back._generic.type = MTYPE_BITMAP;
    saveConfig.back._generic.name = UI_ART_BUTTON;
    saveConfig.back._generic.flags = QMF_LEFT_JUSTIFY | QMF_PULSEIFFOCUS;
    saveConfig.back._generic.id = ID_BACK;
    saveConfig.back._generic.callback = UI_SaveConfigMenu_BackEvent;
    saveConfig.back._generic.x = 0;
    saveConfig.back._generic.y = 480 - 64;
    saveConfig.back.width = 128;
    saveConfig.back.height = 64;
    saveConfig.back.focuspic = UI_ART_BUTTON_FOCUS;
    saveConfig.back._generic.caption.text = "back";
    saveConfig.back._generic.caption.style = UI_CENTER | UI_DROPSHADOW;
    saveConfig.back._generic.caption.fontsize = 0.6f;
    saveConfig.back._generic.caption.font = &uis.freeSansFont;
    saveConfig.back._generic.caption.color = text_color_normal;
    saveConfig.back._generic.caption.focuscolor = text_color_highlight;
    
    saveConfig.save._generic.type = MTYPE_BITMAP;
    saveConfig.save._generic.name = UI_ART_BUTTON;
    saveConfig.save._generic.flags = QMF_RIGHT_JUSTIFY | QMF_PULSEIFFOCUS;
    saveConfig.save._generic.id = ID_SAVE;
    saveConfig.save._generic.callback = UI_SaveConfigMenu_SaveEvent;
    saveConfig.save._generic.x = 640;
    saveConfig.save._generic.y = 480 - 64;
    saveConfig.save.width = 128;
    saveConfig.save.height = 64;
    saveConfig.save.focuspic = UI_ART_BUTTON_FOCUS;
    saveConfig.save._generic.caption.text = "save";
    saveConfig.save._generic.caption.style = UI_CENTER | UI_DROPSHADOW;
    saveConfig.save._generic.caption.fontsize = 0.6f;
    saveConfig.save._generic.caption.font = &uis.freeSansFont;
    saveConfig.save._generic.caption.color = text_color_normal;
    saveConfig.save._generic.caption.focuscolor = text_color_highlight;
    
    Menu_AddItem( &saveConfig.menu, &saveConfig.banner );
    Menu_AddItem( &saveConfig.menu, &saveConfig.background );
    Menu_AddItem( &saveConfig.menu, &saveConfig.savename );
    Menu_AddItem( &saveConfig.menu, &saveConfig.back );
    Menu_AddItem( &saveConfig.menu, &saveConfig.save );
}


/*
=================
UI_SaveConfigMenu_Cache
=================
*/
void UI_SaveConfigMenu_Cache( void )
{
    trap_R_RegisterShaderNoMip( ART_SAVE0 );
    trap_R_RegisterShaderNoMip( ART_SAVE1 );
    trap_R_RegisterShaderNoMip( ART_BACKGROUND );
}


/*
===============
UI_SaveConfigMenu
===============
*/
void UI_SaveConfigMenu( void )
{
    UI_SaveConfigMenu_Init();
    UI_PushMenu( &saveConfig.menu );
}
