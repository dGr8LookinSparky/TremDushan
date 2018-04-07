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


#define ART_FRAMEL		"menu/art/frame2_l"
#define ART_FRAMER		"menu/art/frame1_r"

#define VERTICAL_SPACING	30

#define ID_BACK			10
#define ID_CIN_IDLOGO	11
#define ID_CIN_INTRO	12
#define ID_CIN_TIER1	13
#define ID_CIN_TIER2	14
#define ID_CIN_TIER3	15
#define ID_CIN_TIER4	16
#define ID_CIN_TIER5	17
#define ID_CIN_TIER6	18
#define ID_CIN_TIER7	19
#define ID_CIN_END		20


typedef struct
{
    menuframework_s menu;
    menutext_s      banner;
    menubitmap_s    framel;
    menubitmap_s    framer;
    menutext_s      cin_idlogo;
    menutext_s      cin_intro;
    menutext_s      cin_tier1;
    menutext_s      cin_tier2;
    menutext_s      cin_tier3;
    menutext_s      cin_tier4;
    menutext_s      cin_tier5;
    menutext_s      cin_tier6;
    menutext_s      cin_tier7;
    menutext_s      cin_end;
    menubitmap_s    back;
} cinematicsMenuInfo_t;

static cinematicsMenuInfo_t cinematicsMenuInfo;

static char*    cinematics[] =
{
    "logo",
    "intro",
    "tier1",
    "tier2",
    "tier3",
    "tier4",
    "tier5",
    "tier6",
    "tier7",
    "end"
};

/*
===============
UI_CinematicsMenu_BackEvent
===============
*/
static void UI_CinematicsMenu_BackEvent( void* ptr, int event )
{
    if( event != QM_ACTIVATED )
    {
        return;
    }
    UI_PopMenu();
}


/*
===============
UI_CinematicsMenu_Event
===============
*/
static void UI_CinematicsMenu_Event( void* ptr, int event )
{
    int             n;
    
    if( event != QM_ACTIVATED )
        return;
        
    n = ( ( menucommon_s* ) ptr )->id - ID_CIN_IDLOGO;
    trap_Cvar_Set( "nextmap", va( "ui_cinematics %i", n ) );
    if( ( ( menucommon_s* ) ptr )->id == ID_CIN_END )
    {
        trap_Cmd_ExecuteText( EXEC_APPEND, "disconnect; cinematic demoEnd.RoQ 1\n" );
    }
    else
    {
        trap_Cmd_ExecuteText( EXEC_APPEND, va( "disconnect; cinematic %s.RoQ\n", cinematics[n] ) );
    }
}


/*
===============
UI_CinematicsMenu_Init
===============
*/
static void UI_CinematicsMenu_Init( void )
{
    int             y;
    
    UI_CinematicsMenu_Cache();
    
    memset( &cinematicsMenuInfo, 0, sizeof( cinematicsMenuInfo ) );
    cinematicsMenuInfo.menu.fullscreen = true;
    
    cinematicsMenuInfo.banner._generic.type = MTYPE_BTEXT;
    cinematicsMenuInfo.banner._generic.x = 320;
    cinematicsMenuInfo.banner._generic.y = 16;
    cinematicsMenuInfo.banner.string = "CINEMATICS";
    cinematicsMenuInfo.banner.color = color_white;
    cinematicsMenuInfo.banner.style = UI_CENTER | UI_DROPSHADOW;
    
    /*	cinematicsMenuInfo.framel._generic.type = MTYPE_BITMAP;
    	cinematicsMenuInfo.framel._generic.name = ART_FRAMEL;
    	cinematicsMenuInfo.framel._generic.flags = QMF_INACTIVE;
    	cinematicsMenuInfo.framel._generic.x = 0;
    	cinematicsMenuInfo.framel._generic.y = 78;
    	cinematicsMenuInfo.framel.width = 256;
    	cinematicsMenuInfo.framel.height = 329;
    
    	cinematicsMenuInfo.framer._generic.type = MTYPE_BITMAP;
    	cinematicsMenuInfo.framer._generic.name = ART_FRAMER;
    	cinematicsMenuInfo.framer._generic.flags = QMF_INACTIVE;
    	cinematicsMenuInfo.framer._generic.x = 376;
    	cinematicsMenuInfo.framer._generic.y = 76;
    	cinematicsMenuInfo.framer.width = 256;
    	cinematicsMenuInfo.framer.height = 334;
    */
    y = 100;
    cinematicsMenuInfo.cin_idlogo._generic.type = MTYPE_PTEXT;
    cinematicsMenuInfo.cin_idlogo._generic.flags = QMF_CENTER_JUSTIFY | QMF_PULSEIFFOCUS;
    cinematicsMenuInfo.cin_idlogo._generic.x = 320;
    cinematicsMenuInfo.cin_idlogo._generic.y = y;
    cinematicsMenuInfo.cin_idlogo._generic.id = ID_CIN_IDLOGO;
    cinematicsMenuInfo.cin_idlogo._generic.callback = UI_CinematicsMenu_Event;
    cinematicsMenuInfo.cin_idlogo.string = "ID LOGO";
    cinematicsMenuInfo.cin_idlogo.color = color_red;
    cinematicsMenuInfo.cin_idlogo.style = UI_CENTER;
    
    y += VERTICAL_SPACING;
    cinematicsMenuInfo.cin_intro._generic.type = MTYPE_PTEXT;
    cinematicsMenuInfo.cin_intro._generic.flags = QMF_CENTER_JUSTIFY | QMF_PULSEIFFOCUS;
    cinematicsMenuInfo.cin_intro._generic.x = 320;
    cinematicsMenuInfo.cin_intro._generic.y = y;
    cinematicsMenuInfo.cin_intro._generic.id = ID_CIN_INTRO;
    cinematicsMenuInfo.cin_intro._generic.callback = UI_CinematicsMenu_Event;
    cinematicsMenuInfo.cin_intro.string = "INTRO";
    cinematicsMenuInfo.cin_intro.color = color_red;
    cinematicsMenuInfo.cin_intro.style = UI_CENTER;
    
    y += VERTICAL_SPACING;
    cinematicsMenuInfo.cin_tier1._generic.type = MTYPE_PTEXT;
    cinematicsMenuInfo.cin_tier1._generic.flags = QMF_CENTER_JUSTIFY | QMF_PULSEIFFOCUS;
    cinematicsMenuInfo.cin_tier1._generic.x = 320;
    cinematicsMenuInfo.cin_tier1._generic.y = y;
    cinematicsMenuInfo.cin_tier1._generic.id = ID_CIN_TIER1;
    cinematicsMenuInfo.cin_tier1._generic.callback = UI_CinematicsMenu_Event;
    cinematicsMenuInfo.cin_tier1.string = "Tier 1";
    cinematicsMenuInfo.cin_tier1.color = color_red;
    cinematicsMenuInfo.cin_tier1.style = UI_CENTER;
    if( !UI_CanShowTierVideo( 1 ) )
    {
        cinematicsMenuInfo.cin_tier1._generic.flags |= QMF_GRAYED;
    }
    
    y += VERTICAL_SPACING;
    cinematicsMenuInfo.cin_tier2._generic.type = MTYPE_PTEXT;
    cinematicsMenuInfo.cin_tier2._generic.flags = QMF_CENTER_JUSTIFY | QMF_PULSEIFFOCUS;
    cinematicsMenuInfo.cin_tier2._generic.x = 320;
    cinematicsMenuInfo.cin_tier2._generic.y = y;
    cinematicsMenuInfo.cin_tier2._generic.id = ID_CIN_TIER2;
    cinematicsMenuInfo.cin_tier2._generic.callback = UI_CinematicsMenu_Event;
    cinematicsMenuInfo.cin_tier2.string = "Tier 2";
    cinematicsMenuInfo.cin_tier2.color = color_red;
    cinematicsMenuInfo.cin_tier2.style = UI_CENTER;
    if( !UI_CanShowTierVideo( 2 ) )
    {
        cinematicsMenuInfo.cin_tier2._generic.flags |= QMF_GRAYED;
    }
    
    y += VERTICAL_SPACING;
    cinematicsMenuInfo.cin_tier3._generic.type = MTYPE_PTEXT;
    cinematicsMenuInfo.cin_tier3._generic.flags = QMF_CENTER_JUSTIFY | QMF_PULSEIFFOCUS;
    cinematicsMenuInfo.cin_tier3._generic.x = 320;
    cinematicsMenuInfo.cin_tier3._generic.y = y;
    cinematicsMenuInfo.cin_tier3._generic.id = ID_CIN_TIER3;
    cinematicsMenuInfo.cin_tier3._generic.callback = UI_CinematicsMenu_Event;
    cinematicsMenuInfo.cin_tier3.string = "Tier 3";
    cinematicsMenuInfo.cin_tier3.color = color_red;
    cinematicsMenuInfo.cin_tier3.style = UI_CENTER;
    if( !UI_CanShowTierVideo( 3 ) )
    {
        cinematicsMenuInfo.cin_tier3._generic.flags |= QMF_GRAYED;
    }
    
    y += VERTICAL_SPACING;
    cinematicsMenuInfo.cin_tier4._generic.type = MTYPE_PTEXT;
    cinematicsMenuInfo.cin_tier4._generic.flags = QMF_CENTER_JUSTIFY | QMF_PULSEIFFOCUS;
    cinematicsMenuInfo.cin_tier4._generic.x = 320;
    cinematicsMenuInfo.cin_tier4._generic.y = y;
    cinematicsMenuInfo.cin_tier4._generic.id = ID_CIN_TIER4;
    cinematicsMenuInfo.cin_tier4._generic.callback = UI_CinematicsMenu_Event;
    cinematicsMenuInfo.cin_tier4.string = "Tier 4";
    cinematicsMenuInfo.cin_tier4.color = color_red;
    cinematicsMenuInfo.cin_tier4.style = UI_CENTER;
    if( !UI_CanShowTierVideo( 4 ) )
    {
        cinematicsMenuInfo.cin_tier4._generic.flags |= QMF_GRAYED;
    }
    
    y += VERTICAL_SPACING;
    cinematicsMenuInfo.cin_tier5._generic.type = MTYPE_PTEXT;
    cinematicsMenuInfo.cin_tier5._generic.flags = QMF_CENTER_JUSTIFY | QMF_PULSEIFFOCUS;
    cinematicsMenuInfo.cin_tier5._generic.x = 320;
    cinematicsMenuInfo.cin_tier5._generic.y = y;
    cinematicsMenuInfo.cin_tier5._generic.id = ID_CIN_TIER5;
    cinematicsMenuInfo.cin_tier5._generic.callback = UI_CinematicsMenu_Event;
    cinematicsMenuInfo.cin_tier5.string = "Tier 5";
    cinematicsMenuInfo.cin_tier5.color = color_red;
    cinematicsMenuInfo.cin_tier5.style = UI_CENTER;
    if( !UI_CanShowTierVideo( 5 ) )
    {
        cinematicsMenuInfo.cin_tier5._generic.flags |= QMF_GRAYED;
    }
    
    y += VERTICAL_SPACING;
    cinematicsMenuInfo.cin_tier6._generic.type = MTYPE_PTEXT;
    cinematicsMenuInfo.cin_tier6._generic.flags = QMF_CENTER_JUSTIFY | QMF_PULSEIFFOCUS;
    cinematicsMenuInfo.cin_tier6._generic.x = 320;
    cinematicsMenuInfo.cin_tier6._generic.y = y;
    cinematicsMenuInfo.cin_tier6._generic.id = ID_CIN_TIER6;
    cinematicsMenuInfo.cin_tier6._generic.callback = UI_CinematicsMenu_Event;
    cinematicsMenuInfo.cin_tier6.string = "Tier 6";
    cinematicsMenuInfo.cin_tier6.color = color_red;
    cinematicsMenuInfo.cin_tier6.style = UI_CENTER;
    if( !UI_CanShowTierVideo( 6 ) )
    {
        cinematicsMenuInfo.cin_tier6._generic.flags |= QMF_GRAYED;
    }
    
    y += VERTICAL_SPACING;
    cinematicsMenuInfo.cin_tier7._generic.type = MTYPE_PTEXT;
    cinematicsMenuInfo.cin_tier7._generic.flags = QMF_CENTER_JUSTIFY | QMF_PULSEIFFOCUS;
    cinematicsMenuInfo.cin_tier7._generic.x = 320;
    cinematicsMenuInfo.cin_tier7._generic.y = y;
    cinematicsMenuInfo.cin_tier7._generic.id = ID_CIN_TIER7;
    cinematicsMenuInfo.cin_tier7._generic.callback = UI_CinematicsMenu_Event;
    cinematicsMenuInfo.cin_tier7.string = "Tier 7";
    cinematicsMenuInfo.cin_tier7.color = color_red;
    cinematicsMenuInfo.cin_tier7.style = UI_CENTER;
    if( !UI_CanShowTierVideo( 7 ) )
    {
        cinematicsMenuInfo.cin_tier7._generic.flags |= QMF_GRAYED;
    }
    
    y += VERTICAL_SPACING;
    cinematicsMenuInfo.cin_end._generic.type = MTYPE_PTEXT;
    cinematicsMenuInfo.cin_end._generic.flags = QMF_CENTER_JUSTIFY | QMF_PULSEIFFOCUS;
    cinematicsMenuInfo.cin_end._generic.x = 320;
    cinematicsMenuInfo.cin_end._generic.y = y;
    cinematicsMenuInfo.cin_end._generic.id = ID_CIN_END;
    cinematicsMenuInfo.cin_end._generic.callback = UI_CinematicsMenu_Event;
    cinematicsMenuInfo.cin_end.string = "END";
    cinematicsMenuInfo.cin_end.color = color_red;
    cinematicsMenuInfo.cin_end.style = UI_CENTER;
    if( !UI_CanShowTierVideo( 8 ) )
    {
        cinematicsMenuInfo.cin_end._generic.flags |= QMF_GRAYED;
    }
    
    cinematicsMenuInfo.back._generic.type = MTYPE_BITMAP;
    cinematicsMenuInfo.back._generic.name = UI_ART_BUTTON;
    cinematicsMenuInfo.back._generic.flags = QMF_LEFT_JUSTIFY | QMF_PULSEIFFOCUS;
    cinematicsMenuInfo.back._generic.id = ID_BACK;
    cinematicsMenuInfo.back._generic.callback = UI_CinematicsMenu_BackEvent;
    cinematicsMenuInfo.back._generic.x = 0;
    cinematicsMenuInfo.back._generic.y = 480 - 64;
    cinematicsMenuInfo.back.width = 128;
    cinematicsMenuInfo.back.height = 64;
    cinematicsMenuInfo.back.focuspic = UI_ART_BUTTON_FOCUS;
    cinematicsMenuInfo.back._generic.caption.text = "back";
    cinematicsMenuInfo.back._generic.caption.style = UI_CENTER | UI_DROPSHADOW;
    cinematicsMenuInfo.back._generic.caption.fontsize = 0.6f;
    cinematicsMenuInfo.back._generic.caption.font = &uis.freeSansFont;
    cinematicsMenuInfo.back._generic.caption.color = text_color_normal;
    cinematicsMenuInfo.back._generic.caption.focuscolor = text_color_highlight;
    
    Menu_AddItem( &cinematicsMenuInfo.menu, &cinematicsMenuInfo.banner );
//  Menu_AddItem(&cinematicsMenuInfo.menu, &cinematicsMenuInfo.framel);
//  Menu_AddItem(&cinematicsMenuInfo.menu, &cinematicsMenuInfo.framer);
    Menu_AddItem( &cinematicsMenuInfo.menu, &cinematicsMenuInfo.cin_idlogo );
    Menu_AddItem( &cinematicsMenuInfo.menu, &cinematicsMenuInfo.cin_intro );
    Menu_AddItem( &cinematicsMenuInfo.menu, &cinematicsMenuInfo.cin_tier1 );
    Menu_AddItem( &cinematicsMenuInfo.menu, &cinematicsMenuInfo.cin_tier2 );
    Menu_AddItem( &cinematicsMenuInfo.menu, &cinematicsMenuInfo.cin_tier3 );
    Menu_AddItem( &cinematicsMenuInfo.menu, &cinematicsMenuInfo.cin_tier4 );
    Menu_AddItem( &cinematicsMenuInfo.menu, &cinematicsMenuInfo.cin_tier5 );
    Menu_AddItem( &cinematicsMenuInfo.menu, &cinematicsMenuInfo.cin_tier6 );
    Menu_AddItem( &cinematicsMenuInfo.menu, &cinematicsMenuInfo.cin_tier7 );
    Menu_AddItem( &cinematicsMenuInfo.menu, &cinematicsMenuInfo.cin_end );
    Menu_AddItem( &cinematicsMenuInfo.menu, &cinematicsMenuInfo.back );
}


/*
=================
UI_CinematicsMenu_Cache
=================
*/
void UI_CinematicsMenu_Cache( void )
{
    trap_R_RegisterShaderNoMip( ART_FRAMEL );
    trap_R_RegisterShaderNoMip( ART_FRAMER );
}


/*
===============
UI_CinematicsMenu
===============
*/
void UI_CinematicsMenu( void )
{
    UI_CinematicsMenu_Init();
    UI_PushMenu( &cinematicsMenuInfo.menu );
}


/*
===============
UI_CinematicsMenu_f
===============
*/
void UI_CinematicsMenu_f( void )
{
    int             n;
    
    n = atoi( UI_Argv( 1 ) );
    UI_CinematicsMenu();
    Menu_SetCursorToItem( &cinematicsMenuInfo.menu, cinematicsMenuInfo.menu.items[n + 3] );
}
