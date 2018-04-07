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

void            GraphicsOptions_MenuInit( void );

/*
=======================================================================

DRIVER INFORMATION MENU

otty: do we really need this in ui ? wouldnt it be better to have something like

"dump driverinfo" in console ?

driverinfo is disabled by default in ui now, because it exeeds ui space
=======================================================================
*/


#define DRIVERINFO_FRAMEL	"menu/art/frame2_l"
#define DRIVERINFO_FRAMER	"menu/art/frame1_r"
#define DRIVERINFO_BACK0	"menu/art/back_0"
#define DRIVERINFO_BACK1	"menu/art/back_1"

static char*    driverinfo_artlist[] =
{
    DRIVERINFO_FRAMEL,
    DRIVERINFO_FRAMER,
    DRIVERINFO_BACK0,
    DRIVERINFO_BACK1,
    NULL,
};

#define ID_DRIVERINFOBACK	100

typedef struct
{
    menuframework_s menu;
    menutext_s      banner;
    menubitmap_s    back;
    menubitmap_s    framel;
    menubitmap_s    framer;
    char            stringbuff[1024];
    char*           strings[64];
    int             numstrings;
} driverinfo_t;

static driverinfo_t s_driverinfo;

/*
=================
DriverInfo_Event
=================
*/
static void DriverInfo_Event( void* ptr, int event )
{
    if( event != QM_ACTIVATED )
        return;
        
    switch( ( ( menucommon_s* ) ptr )->id )
    {
        case ID_DRIVERINFOBACK:
            UI_PopMenu();
            break;
    }
}

/*
=================
DriverInfo_MenuDraw
=================
*/
static void DriverInfo_MenuDraw( void )
{
    int             i;
    int             y;
    
    Menu_Draw( &s_driverinfo.menu );
    
    UI_DrawString( 320, 80, "VENDOR", UI_CENTER | UI_SMALLFONT, color_red );
    UI_DrawString( 320, 152, "PIXELFORMAT", UI_CENTER | UI_SMALLFONT, color_red );
    UI_DrawString( 320, 192, "EXTENSIONS", UI_CENTER | UI_SMALLFONT, color_red );
    
    UI_DrawString( 320, 80 + 16, uis.glconfig.vendor_string, UI_CENTER | UI_SMALLFONT, text_color_normal );
    UI_DrawString( 320, 96 + 16, uis.glconfig.version_string, UI_CENTER | UI_SMALLFONT, text_color_normal );
    UI_DrawString( 320, 112 + 16, uis.glconfig.renderer_string, UI_CENTER | UI_SMALLFONT, text_color_normal );
    UI_DrawString( 320, 152 + 16,
                   va( "color(%d-bits) Z(%d-bits) stencil(%d-bits)", uis.glconfig.colorBits, uis.glconfig.depthBits,
                       uis.glconfig.stencilBits ), UI_CENTER | UI_SMALLFONT, text_color_normal );
                       
    // double column
    y = 192 + 16;
    for( i = 0; i < s_driverinfo.numstrings / 2; i++ )
    {
        UI_DrawString( 320 - 4, y, s_driverinfo.strings[i * 2], UI_RIGHT | UI_SMALLFONT, text_color_normal );
        UI_DrawString( 320 + 4, y, s_driverinfo.strings[i * 2 + 1], UI_LEFT | UI_SMALLFONT, text_color_normal );
        y += SMALLCHAR_HEIGHT;
    }
    
    if( s_driverinfo.numstrings & 1 )
        UI_DrawString( 320, y, s_driverinfo.strings[s_driverinfo.numstrings - 1], UI_CENTER | UI_SMALLFONT, text_color_normal );
}

/*
=================
DriverInfo_Cache
=================
*/
void DriverInfo_Cache( void )
{
    int             i;
    
    // touch all our pics
    for( i = 0;; i++ )
    {
        if( !driverinfo_artlist[i] )
            break;
        trap_R_RegisterShaderNoMip( driverinfo_artlist[i] );
    }
}

/*
=================
UI_DriverInfo_Menu
=================
*/
static void UI_DriverInfo_Menu( void )
{
    char*           eptr;
    int             i;
    int             len;
    
    // zero set all our globals
    memset( &s_driverinfo, 0, sizeof( driverinfo_t ) );
    
    DriverInfo_Cache();
    
    s_driverinfo.menu.fullscreen = true;
    s_driverinfo.menu.draw = DriverInfo_MenuDraw;
    
    s_driverinfo.banner._generic.type = MTYPE_BTEXT;
    s_driverinfo.banner._generic.x = 320;
    s_driverinfo.banner._generic.y = 16;
    s_driverinfo.banner.string = "DRIVER INFO";
    s_driverinfo.banner.color = color_white;
    s_driverinfo.banner.style = UI_CENTER | UI_DROPSHADOW;
    
    s_driverinfo.framel._generic.type = MTYPE_BITMAP;
    s_driverinfo.framel._generic.name = DRIVERINFO_FRAMEL;
    s_driverinfo.framel._generic.flags = QMF_INACTIVE;
    s_driverinfo.framel._generic.x = 0;
    s_driverinfo.framel._generic.y = 78;
    s_driverinfo.framel.width = 256;
    s_driverinfo.framel.height = 329;
    
    s_driverinfo.framer._generic.type = MTYPE_BITMAP;
    s_driverinfo.framer._generic.name = DRIVERINFO_FRAMER;
    s_driverinfo.framer._generic.flags = QMF_INACTIVE;
    s_driverinfo.framer._generic.x = 376;
    s_driverinfo.framer._generic.y = 76;
    s_driverinfo.framer.width = 256;
    s_driverinfo.framer.height = 334;
    
    s_driverinfo.back._generic.type = MTYPE_BITMAP;
    s_driverinfo.back._generic.name = UI_ART_BUTTON;
    s_driverinfo.back._generic.flags = QMF_LEFT_JUSTIFY | QMF_PULSEIFFOCUS;
    s_driverinfo.back._generic.callback = DriverInfo_Event;
    s_driverinfo.back._generic.id = ID_DRIVERINFOBACK;
    s_driverinfo.back._generic.x = 0;
    s_driverinfo.back._generic.y = 480 - 64;
    s_driverinfo.back.width = 128;
    s_driverinfo.back.height = 64;
    s_driverinfo.back.focuspic = UI_ART_BUTTON_FOCUS;
    s_driverinfo.back._generic.caption.text = "back";
    s_driverinfo.back._generic.caption.style = UI_CENTER | UI_DROPSHADOW;
    s_driverinfo.back._generic.caption.fontsize = 0.6f;
    s_driverinfo.back._generic.caption.font = &uis.freeSansFont;
    s_driverinfo.back._generic.caption.color = text_color_normal;
    s_driverinfo.back._generic.caption.focuscolor = text_color_highlight;
    
    // TTimo: overflow with particularly long GL extensions (such as the gf3)
    // https://zerowing.idsoftware.com/bugzilla/show_bug.cgi?id=399
    // NOTE: could have pushed the size of stringbuff, but the list is already out of the screen
    // (no matter what your resolution)
    Q_strncpyz( s_driverinfo.stringbuff, uis.glconfig.extensions_string, 1024 );
    
    // build null terminated extension strings
    eptr = s_driverinfo.stringbuff;
    while( s_driverinfo.numstrings < 40 && *eptr )
    {
        while( *eptr && *eptr == ' ' )
            *eptr++ = '\0';
            
        // track start of valid string
        if( *eptr && *eptr != ' ' )
            s_driverinfo.strings[s_driverinfo.numstrings++] = eptr;
            
        while( *eptr && *eptr != ' ' )
            eptr++;
    }
    
    // safety length strings for display
    for( i = 0; i < s_driverinfo.numstrings; i++ )
    {
        len = strlen( s_driverinfo.strings[i] );
        if( len > 32 )
        {
            s_driverinfo.strings[i][len - 1] = '>';
            s_driverinfo.strings[i][len] = '\0';
        }
    }
    
    Menu_AddItem( &s_driverinfo.menu, &s_driverinfo.banner );
    Menu_AddItem( &s_driverinfo.menu, &s_driverinfo.framel );
    Menu_AddItem( &s_driverinfo.menu, &s_driverinfo.framer );
    Menu_AddItem( &s_driverinfo.menu, &s_driverinfo.back );
    
    UI_PushMenu( &s_driverinfo.menu );
}

/*
=======================================================================

GRAPHICS OPTIONS MENU

=======================================================================
*/

#define GRAPHICSOPTIONS_FRAMEL	"menu/art/frame2_l"
#define GRAPHICSOPTIONS_FRAMER	"menu/art/frame1_r"


#define ID_BACK2		101
#define ID_FULLSCREEN		102
#define ID_LIST			103
#define ID_MODE			104
#define ID_DRIVERINFO		105
#define ID_GRAPHICS		106
#define ID_DISPLAY		107
#define ID_SOUND		108
#define ID_NETWORK		109
#define ID_RATIO		110
#define ID_BRIGHTNESS		111

typedef struct
{
    menuframework_s menu;
    
    menutext_s      banner;
    menubitmap_s    framel;
    menubitmap_s    framer;
    
    menubitmap_s    graphics;
//  menutext_s      display;
    menubitmap_s    sound;
    menubitmap_s    network;
    
    menulist_s      list;
    menulist_s      ratio;
    menulist_s      mode;
    menuslider_s    tq;
    menulist_s      fs;
    menulist_s      vsync;
    menuslider_s    brightness;
//	menulist_s      texturebits;
//  menulist_s      colordepth;
    menulist_s      geometry;
    menulist_s      filter;
    menulist_s      compression;
    menuslider_s    anisotropicFilter;
    menulist_s      deferredShading;
    menulist_s      normalMapping;
    menulist_s      parallax;
    menulist_s      shadowType;
    menulist_s      shadowFilter;
    menuslider_s    shadowBlur;
    menulist_s      shadowQuality;
    menulist_s      dynamicLightsCastShadows;
    menulist_s      hdr;
    menulist_s      bloom;
    menulist_s      vertexLighting;
    menutext_s      driverinfo;
    
    menubitmap_s    apply;
    menubitmap_s    back;
} graphicsoptions_t;

typedef struct
{
    int             mode;
    bool        fullscreen;
    bool		vsync;
    int             tq;
//  int             colordepth;
//	int             texturebits;
    int             geometry;
    int             filter;
    int             compression;
    int             anisotropicFilter;
    int             deferredShading;
    int				normalMapping;
    int             parallax;
    int             shadowType;
    int             shadowFilter;
    int             shadowBlur;
    int             shadowQuality;
    int             dynamicLightsCastShadows;
    int             hdr;
    int             bloom;
} InitialVideoOptions_s;

static InitialVideoOptions_s s_ivo;
static graphicsoptions_t s_graphicsoptions;

// *INDENT-OFF*
static InitialVideoOptions_s s_ivo_templates[] =
{
    { 4, true, false, 2, 2, 1, 1, 1, 0, 1, 0, 1, 1, 1, 1, 0, 0, 0},
    { 3, true, false, 2, 1, 1, 1, 1, 0, 1, 0, 1, 1, 1, 1, 0, 0, 0},
    { 2, true, false, 1, 0, 1, 1, 1, 0, 1, 0, 1, 1, 1, 1, 0, 0, 0},
    { 2, true, false, 1, 0, 1, 1, 1, 0, 1, 0, 1, 1, 1, 1, 0, 0, 0},
    { 3, true, false, 1, 1, 1, 1, 1, 0, 1, 0, 1, 1, 1, 1, 0, 0, 0}
};
// *INDENT-ON*

#define NUM_IVO_TEMPLATES ( sizeof( s_ivo_templates ) / sizeof( s_ivo_templates[0] ) )

static const char* builtinResolutions[] =
{
    "320x240",
    "400x300",
    "512x384",
    "640x480",
    "800x600",
    "960x720",
    "1024x768",
    "1152x864",
    "1280x720",
    "1280x768",
    "1280x800",
    "1280x1024",
    "1360x768",
    "1440x900",
    "1680x1050",
    "1600x1200",
    "1920x1080",
    "1920x1200",
    "2048x1536",
    "2560x1600",
    NULL
};

static const char* knownRatios[][2] =
{
    {"1.25:1", "5:4"},
    {"1.33:1", "4:3"},
    {"1.50:1", "3:2"},
    {"1.56:1", "14:9"},
    {"1.60:1", "16:10"},
    {"1.67:1", "5:3"},
    {"1.78:1", "16:9"},
    {NULL, NULL}
};

#define MAX_RESOLUTIONS	32

static const char* ratios[MAX_RESOLUTIONS];
static char     ratioBuf[MAX_RESOLUTIONS][8];
static int      ratioToRes[MAX_RESOLUTIONS];
static int      resToRatio[MAX_RESOLUTIONS];

static char     resbuf[MAX_STRING_CHARS];
static const char* detectedResolutions[MAX_RESOLUTIONS];

static const char** resolutions = builtinResolutions;
static bool resolutionsDetected = false;

/*
=================
GraphicsOptions_FindBuiltinResolution
=================
*/
static int GraphicsOptions_FindBuiltinResolution( int mode )
{
    int             i;
    
    if( !resolutionsDetected )
        return mode;
        
    if( mode < 0 )
        return -1;
        
    for( i = 0; builtinResolutions[i]; i++ )
    {
        if( !Q_stricmp( builtinResolutions[i], detectedResolutions[mode] ) )
            return i;
    }
    
    return -1;
}

/*
=================
GraphicsOptions_FindDetectedResolution
=================
*/
static int GraphicsOptions_FindDetectedResolution( int mode )
{
    int             i;
    
    if( !resolutionsDetected )
        return mode;
        
    if( mode < 0 )
        return -1;
        
    for( i = 0; detectedResolutions[i]; i++ )
    {
        if( !Q_stricmp( builtinResolutions[mode], detectedResolutions[i] ) )
            return i;
    }
    
    return -1;
}

/*
=================
GraphicsOptions_GetAspectRatios
=================
*/
static void GraphicsOptions_GetAspectRatios( void )
{
    int             i, r;
    
    // build ratio list from resolutions
    for( r = 0; resolutions[r]; r++ )
    {
        int             w, h;
        char*           x;
        char            str[sizeof( ratioBuf[0] )];
        
        // calculate resolution's aspect ratio
        x = ( char* )strchr( resolutions[r], 'x' ) + 1;
        Q_strncpyz( str, resolutions[r], x - resolutions[r] );
        w = atoi( str );
        h = atoi( x );
        Com_sprintf( str, sizeof( str ), "%.2f:1", ( float )w / ( float )h );
        
        // add ratio to list if it is new
        // establish res/ratio relationship
        for( i = 0; ratioBuf[i][0]; i++ )
        {
            if( !Q_stricmp( str, ratioBuf[i] ) )
                break;
        }
        if( !ratioBuf[i][0] )
        {
            Q_strncpyz( ratioBuf[i], str, sizeof( ratioBuf[i] ) );
            ratioToRes[i] = r;
        }
        resToRatio[r] = i;
    }
    
    // prepare itemlist pointer array
    // rename common ratios ("1.33:1" -> "4:3")
    for( r = 0; ratioBuf[r][0]; r++ )
    {
        for( i = 0; knownRatios[i][0]; i++ )
        {
            if( !Q_stricmp( ratioBuf[r], knownRatios[i][0] ) )
            {
                Q_strncpyz( ratioBuf[r], knownRatios[i][1], sizeof( ratioBuf[r] ) );
                break;
            }
        }
        ratios[r] = ratioBuf[r];
    }
    ratios[r] = NULL;
}

/*
=================
GraphicsOptions_GetInitialVideo
=================
*/
static void GraphicsOptions_GetInitialVideo( void )
{
//  s_ivo.colordepth = s_graphicsoptions.colordepth.curvalue;
    s_ivo.mode = s_graphicsoptions.mode.curvalue;
    s_ivo.fullscreen = s_graphicsoptions.fs.curvalue;
    s_ivo.vsync = s_graphicsoptions.vsync.curvalue;
    s_ivo.tq = s_graphicsoptions.tq.curvalue;
    s_ivo.geometry = s_graphicsoptions.geometry.curvalue;
    s_ivo.filter = s_graphicsoptions.filter.curvalue;
//	s_ivo.texturebits = s_graphicsoptions.texturebits.curvalue;
    s_ivo.compression = s_graphicsoptions.compression.curvalue;
    s_ivo.deferredShading = s_graphicsoptions.deferredShading.curvalue;
    s_ivo.normalMapping = s_graphicsoptions.normalMapping.curvalue;
    s_ivo.parallax = s_graphicsoptions.parallax.curvalue;
    s_ivo.anisotropicFilter = s_graphicsoptions.anisotropicFilter.curvalue;
    s_ivo.shadowType = s_graphicsoptions.shadowType.curvalue;
    s_ivo.shadowFilter = s_graphicsoptions.shadowFilter.curvalue;
    s_ivo.shadowBlur = s_graphicsoptions.shadowBlur.curvalue;
    s_ivo.shadowQuality = s_graphicsoptions.shadowQuality.curvalue;
    s_ivo.dynamicLightsCastShadows = s_graphicsoptions.dynamicLightsCastShadows.curvalue;
    s_ivo.hdr = s_graphicsoptions.hdr.curvalue;
    s_ivo.bloom = s_graphicsoptions.bloom.curvalue;
}

/*
=================
GraphicsOptions_GetResolutions
=================
*/
static void GraphicsOptions_GetResolutions( void )
{
    Q_strncpyz( resbuf, UI_Cvar_VariableString( "r_availableModes" ), sizeof( resbuf ) );
    if( *resbuf )
    {
        char*           s = resbuf;
        unsigned int    i = 0;
        
        while( s && i < sizeof( detectedResolutions ) / sizeof( detectedResolutions[0] ) - 1 )
        {
            detectedResolutions[i++] = s;
            s = strchr( s, ' ' );
            if( s )
                *s++ = '\0';
        }
        detectedResolutions[i] = NULL;
        
        if( i > 0 )
        {
            resolutions = detectedResolutions;
            resolutionsDetected = true;
        }
    }
}

/*
=================
GraphicsOptions_CheckConfig
=================
*/
static void GraphicsOptions_CheckConfig( void )
{
    int             i;
    
    for( i = 0; i < NUM_IVO_TEMPLATES; i++ )
    {
//      if(s_ivo_templates[i].colordepth != s_graphicsoptions.colordepth.curvalue)
//          continue;
        //if(s_ivo_templates[i].mode != s_graphicsoptions.mode.curvalue)
        //  continue;
        if( GraphicsOptions_FindDetectedResolution( s_ivo_templates[i].mode ) != s_graphicsoptions.mode.curvalue )
            continue;
        if( s_ivo_templates[i].fullscreen != s_graphicsoptions.fs.curvalue )
            continue;
        if( s_ivo_templates[i].vsync != s_graphicsoptions.vsync.curvalue )
            continue;
        if( s_ivo_templates[i].tq != s_graphicsoptions.tq.curvalue )
            continue;
        if( s_ivo_templates[i].geometry != s_graphicsoptions.geometry.curvalue )
            continue;
        if( s_ivo_templates[i].filter != s_graphicsoptions.filter.curvalue )
            continue;
//      if ( s_ivo_templates[i].texturebits != s_graphicsoptions.texturebits.curvalue )
//          continue;
        if( s_ivo_templates[i].compression != s_graphicsoptions.compression.curvalue )
            continue;
        if( s_ivo_templates[i].anisotropicFilter != s_graphicsoptions.anisotropicFilter.curvalue )
            continue;
        if( s_ivo_templates[i].deferredShading != s_graphicsoptions.deferredShading.curvalue )
            continue;
        if( s_ivo_templates[i].normalMapping != s_graphicsoptions.normalMapping.curvalue )
            continue;
        if( s_ivo_templates[i].parallax != s_graphicsoptions.parallax.curvalue )
            continue;
        if( s_ivo_templates[i].shadowType != s_graphicsoptions.shadowType.curvalue )
            continue;
        if( s_ivo_templates[i].shadowFilter != s_graphicsoptions.shadowFilter.curvalue )
            continue;
        if( s_ivo_templates[i].shadowBlur != s_graphicsoptions.shadowBlur.curvalue )
            continue;
        if( s_ivo_templates[i].shadowQuality != s_graphicsoptions.shadowQuality.curvalue )
            continue;
        if( s_ivo_templates[i].dynamicLightsCastShadows != s_graphicsoptions.dynamicLightsCastShadows.curvalue )
            continue;
        if( s_ivo_templates[i].hdr != s_graphicsoptions.hdr.curvalue )
            continue;
        if( s_ivo_templates[i].bloom != s_graphicsoptions.bloom.curvalue )
            continue;
        s_graphicsoptions.list.curvalue = i;
        return;
    }
    
    // return 'Custom' ivo template
    s_graphicsoptions.list.curvalue = NUM_IVO_TEMPLATES - 1;
}

/*
=================
GraphicsOptions_UpdateMenuItems
=================
*/
static void GraphicsOptions_UpdateMenuItems( void )
{
    s_graphicsoptions.fs._generic.flags &= ~QMF_GRAYED;
    
    /*
       if(s_graphicsoptions.fs.curvalue == 0)
       {
       s_graphicsoptions.colordepth.curvalue = 0;
       s_graphicsoptions.colordepth._generic.flags |= QMF_GRAYED;
       }
       else
       {
       s_graphicsoptions.colordepth._generic.flags &= ~QMF_GRAYED;
       }
     */
    
    s_graphicsoptions.apply._generic.flags |= QMF_GRAYED | QMF_INACTIVE;
    
    if( s_ivo.mode != s_graphicsoptions.mode.curvalue )
    {
        s_graphicsoptions.apply._generic.flags &= ~( QMF_GRAYED | QMF_INACTIVE );
    }
    
    if( s_ivo.fullscreen != s_graphicsoptions.fs.curvalue )
    {
        s_graphicsoptions.apply._generic.flags &= ~( QMF_GRAYED | QMF_INACTIVE );
    }
    
    if( s_ivo.vsync != s_graphicsoptions.vsync.curvalue )
    {
        s_graphicsoptions.apply._generic.flags &= ~( QMF_GRAYED | QMF_INACTIVE );
    }
    
    if( s_ivo.tq != s_graphicsoptions.tq.curvalue )
    {
        s_graphicsoptions.apply._generic.flags &= ~( QMF_GRAYED | QMF_INACTIVE );
    }
    
    /*
       if(s_ivo.colordepth != s_graphicsoptions.colordepth.curvalue)
       {
       s_graphicsoptions.apply._generic.flags &= ~(QMF_GRAYED | QMF_INACTIVE);
       }
     */
    
    /*
    if(s_ivo.texturebits != s_graphicsoptions.texturebits.curvalue)
    {
    	s_graphicsoptions.apply._generic.flags &= ~(QMF_GRAYED | QMF_INACTIVE);
    }
    */
    
    if( s_ivo.geometry != s_graphicsoptions.geometry.curvalue )
    {
        s_graphicsoptions.apply._generic.flags &= ~( QMF_GRAYED | QMF_INACTIVE );
    }
    
    if( s_ivo.filter != s_graphicsoptions.filter.curvalue )
    {
        s_graphicsoptions.apply._generic.flags &= ~( QMF_GRAYED | QMF_INACTIVE );
    }
    
    if( s_ivo.compression != s_graphicsoptions.compression.curvalue )
    {
        s_graphicsoptions.apply._generic.flags &= ~( QMF_GRAYED | QMF_INACTIVE );
    }
    
    if( s_ivo.anisotropicFilter != s_graphicsoptions.anisotropicFilter.curvalue )
    {
        s_graphicsoptions.apply._generic.flags &= ~( QMF_GRAYED | QMF_INACTIVE );
    }
    
    if( s_ivo.deferredShading != s_graphicsoptions.deferredShading.curvalue )
    {
        s_graphicsoptions.apply._generic.flags &= ~( QMF_GRAYED | QMF_INACTIVE );
    }
    
    if( s_ivo.parallax != s_graphicsoptions.parallax.curvalue )
    {
        s_graphicsoptions.apply._generic.flags &= ~( QMF_GRAYED | QMF_INACTIVE );
    }
    
    if( s_ivo.normalMapping != s_graphicsoptions.normalMapping.curvalue )
    {
        s_graphicsoptions.apply._generic.flags &= ~( QMF_GRAYED | QMF_INACTIVE );
    }
    
    if( !s_graphicsoptions.normalMapping.curvalue )
    {
        s_graphicsoptions.parallax.curvalue = 0;
        s_graphicsoptions.parallax._generic.flags |= QMF_GRAYED;
    }
    else
    {
        s_graphicsoptions.parallax._generic.flags &= ~QMF_GRAYED;
    }
    
    if( s_ivo.shadowType != s_graphicsoptions.shadowType.curvalue )
    {
        s_graphicsoptions.apply._generic.flags &= ~( QMF_GRAYED | QMF_INACTIVE );
    }
    
    if( s_graphicsoptions.shadowType.curvalue <= SHADOWING_BLOB )
    {
        s_graphicsoptions.shadowFilter.curvalue = 0;
        s_graphicsoptions.shadowFilter._generic.flags |= QMF_GRAYED;
    }
    else
    {
        s_graphicsoptions.shadowFilter._generic.flags &= ~QMF_GRAYED;
    }
    
    if( s_ivo.shadowFilter != s_graphicsoptions.shadowFilter.curvalue )
    {
        s_graphicsoptions.apply._generic.flags &= ~( QMF_GRAYED | QMF_INACTIVE );
    }
    
    if( s_graphicsoptions.shadowType.curvalue <= SHADOWING_BLOB || s_graphicsoptions.shadowFilter.curvalue == 0 )
    {
        s_graphicsoptions.shadowBlur.curvalue = 1;
        s_graphicsoptions.shadowBlur._generic.flags |= QMF_GRAYED;
    }
    else
    {
        s_graphicsoptions.shadowBlur._generic.flags &= ~QMF_GRAYED;
    }
    
    if( s_ivo.shadowBlur != s_graphicsoptions.shadowBlur.curvalue )
    {
        s_graphicsoptions.apply._generic.flags &= ~( QMF_GRAYED | QMF_INACTIVE );
    }
    
    if( s_graphicsoptions.shadowType.curvalue <= SHADOWING_BLOB )
    {
        s_graphicsoptions.shadowQuality.curvalue = 0;
        s_graphicsoptions.shadowQuality._generic.flags |= QMF_GRAYED;
    }
    else
    {
        s_graphicsoptions.shadowQuality._generic.flags &= ~QMF_GRAYED;
    }
    
    if( s_ivo.shadowQuality != s_graphicsoptions.shadowQuality.curvalue )
    {
        s_graphicsoptions.apply._generic.flags &= ~( QMF_GRAYED | QMF_INACTIVE );
    }
    
    if( s_graphicsoptions.shadowType.curvalue <= SHADOWING_BLOB )
    {
        s_graphicsoptions.dynamicLightsCastShadows.curvalue = 0;
        s_graphicsoptions.dynamicLightsCastShadows._generic.flags |= QMF_GRAYED;
    }
    else
    {
        s_graphicsoptions.dynamicLightsCastShadows._generic.flags &= ~QMF_GRAYED;
    }
    
    if( s_ivo.dynamicLightsCastShadows != s_graphicsoptions.dynamicLightsCastShadows.curvalue )
    {
        s_graphicsoptions.apply._generic.flags &= ~( QMF_GRAYED | QMF_INACTIVE );
    }
    
    if( s_ivo.hdr != s_graphicsoptions.hdr.curvalue )
    {
        s_graphicsoptions.apply._generic.flags &= ~( QMF_GRAYED | QMF_INACTIVE );
    }
    
    if( s_ivo.bloom != s_graphicsoptions.bloom.curvalue )
    {
        s_graphicsoptions.apply._generic.flags &= ~( QMF_GRAYED | QMF_INACTIVE );
    }
    
    GraphicsOptions_CheckConfig();
}

/*
=================
GraphicsOptions_ApplyChanges
=================
*/
static void GraphicsOptions_ApplyChanges( void* unused, int notification )
{
    if( notification != QM_ACTIVATED )
        return;
        
    trap_Cvar_SetValue( "r_picmip", 3 - s_graphicsoptions.tq.curvalue );
    
    if( resolutionsDetected )
    {
        // search for builtin mode that matches the detected mode
        int             mode;
        
        if( s_graphicsoptions.mode.curvalue == -1
                || s_graphicsoptions.mode.curvalue >= sizeof( detectedResolutions ) / sizeof( detectedResolutions[0] ) )
            s_graphicsoptions.mode.curvalue = 0;
            
        mode = GraphicsOptions_FindBuiltinResolution( s_graphicsoptions.mode.curvalue );
        if( mode == -1 )
        {
            char            w[16], h[16];
            
            Q_strncpyz( w, detectedResolutions[s_graphicsoptions.mode.curvalue], sizeof( w ) );
            *strchr( w, 'x' ) = 0;
            Q_strncpyz( h, strchr( detectedResolutions[s_graphicsoptions.mode.curvalue], 'x' ) + 1, sizeof( h ) );
            trap_Cvar_Set( "r_customwidth", w );
            trap_Cvar_Set( "r_customheight", h );
        }
        
        trap_Cvar_SetValue( "r_mode", mode );
    }
    else
        trap_Cvar_SetValue( "r_mode", s_graphicsoptions.mode.curvalue );
        
    trap_Cvar_SetValue( "r_fullscreen", s_graphicsoptions.fs.curvalue );
    
    trap_Cvar_SetValue( "r_swapInterval", s_graphicsoptions.vsync.curvalue );
    
    trap_Cvar_SetValue( "r_colorbits", 0 );
    trap_Cvar_SetValue( "r_depthbits", 0 );
    trap_Cvar_SetValue( "r_stencilbits", 8 );
    
    trap_Cvar_SetValue( "r_deferredShading", s_graphicsoptions.deferredShading.curvalue );
    trap_Cvar_SetValue( "r_normalMapping", s_graphicsoptions.normalMapping.curvalue );
    trap_Cvar_SetValue( "r_parallaxMapping", s_graphicsoptions.parallax.curvalue );
    
    if( s_graphicsoptions.geometry.curvalue == 2 )
    {
        trap_Cvar_SetValue( "r_lodBias", 0 );
        trap_Cvar_SetValue( "r_subdivisions", 4 );
    }
    else if( s_graphicsoptions.geometry.curvalue == 1 )
    {
        trap_Cvar_SetValue( "r_lodBias", 1 );
        trap_Cvar_SetValue( "r_subdivisions", 12 );
    }
    else
    {
        trap_Cvar_SetValue( "r_lodBias", 1 );
        trap_Cvar_SetValue( "r_subdivisions", 20 );
    }
    
    if( s_graphicsoptions.filter.curvalue )
    {
        trap_Cvar_Set( "r_textureMode", "GL_LINEAR_MIPMAP_LINEAR" );
    }
    else
    {
        trap_Cvar_Set( "r_textureMode", "GL_LINEAR_MIPMAP_NEAREST" );
    }
    
    trap_Cvar_SetValue( "r_ext_compressed_textures", s_graphicsoptions.compression.curvalue );
    trap_Cvar_SetValue( "r_ext_texture_filter_anisotropic", s_graphicsoptions.anisotropicFilter.curvalue );
    
    trap_Cvar_SetValue( "cg_shadows", s_graphicsoptions.shadowType.curvalue );
    trap_Cvar_SetValue( "r_softShadows", s_graphicsoptions.shadowFilter.curvalue );
    trap_Cvar_SetValue( "r_shadowBlur", s_graphicsoptions.shadowBlur.curvalue );
    
    trap_Cvar_SetValue( "r_shadowMapQuality", s_graphicsoptions.shadowQuality.curvalue );
    switch( s_graphicsoptions.shadowQuality.curvalue )
    {
        case 1:				// low
            trap_Cvar_SetValue( "r_shadowMapSizeUltra", 256 );
            trap_Cvar_SetValue( "r_shadowMapSizeVeryHigh", 128 );
            trap_Cvar_SetValue( "r_shadowMapSizeHigh", 64 );
            trap_Cvar_SetValue( "r_shadowMapSizeMedium", 32 );
            trap_Cvar_SetValue( "r_shadowMapSizeLow", 16 );
            break;
            
        case 2:				// medium
            trap_Cvar_SetValue( "r_shadowMapSizeUltra", 512 );
            trap_Cvar_SetValue( "r_shadowMapSizeVeryHigh", 256 );
            trap_Cvar_SetValue( "r_shadowMapSizeHigh", 128 );
            trap_Cvar_SetValue( "r_shadowMapSizeMedium", 64 );
            trap_Cvar_SetValue( "r_shadowMapSizeLow", 32 );
            break;
            
        case 3:				// high
            trap_Cvar_SetValue( "r_shadowMapSizeUltra", 1024 );
            trap_Cvar_SetValue( "r_shadowMapSizeVeryHigh", 512 );
            trap_Cvar_SetValue( "r_shadowMapSizeHigh", 256 );
            trap_Cvar_SetValue( "r_shadowMapSizeMedium", 128 );
            trap_Cvar_SetValue( "r_shadowMapSizeLow", 64 );
            break;
            
        case 4:				// very high
            trap_Cvar_SetValue( "r_shadowMapSizeUltra", 2048 );
            trap_Cvar_SetValue( "r_shadowMapSizeVeryHigh", 1024 );
            trap_Cvar_SetValue( "r_shadowMapSizeHigh", 512 );
            trap_Cvar_SetValue( "r_shadowMapSizeMedium", 256 );
            trap_Cvar_SetValue( "r_shadowMapSizeLow", 128 );
            break;
            
            /*
               case 5: // ultra
               trap_Cvar_SetValue("r_shadowMapSizeUltra", 2048);
               trap_Cvar_SetValue("r_shadowMapSizeVeryHigh", 2048);
               trap_Cvar_SetValue("r_shadowMapSizeHigh", 1024);
               trap_Cvar_SetValue("r_shadowMapSizeMedium", 512);
               trap_Cvar_SetValue("r_shadowMapSizeLow", 128);
               break;
             */
            
        case 0:				// custom
        default:
            break;
    }
    
    trap_Cvar_SetValue( "r_dynamicLightCastShadows", s_graphicsoptions.dynamicLightsCastShadows.curvalue );
    trap_Cvar_SetValue( "r_hdrRendering", s_graphicsoptions.hdr.curvalue );
    trap_Cvar_SetValue( "r_bloom", s_graphicsoptions.bloom.curvalue );
    
    trap_Cmd_ExecuteText( EXEC_APPEND, "vid_restart\n" );
}

/*
=================
GraphicsOptions_Event
=================
*/
static void GraphicsOptions_Event( void* ptr, int event )
{
    InitialVideoOptions_s* ivo;
    
    if( event != QM_ACTIVATED )
    {
        return;
    }
    
    switch( ( ( menucommon_s* ) ptr )->id )
    {
        case ID_RATIO:
            s_graphicsoptions.mode.curvalue = ratioToRes[s_graphicsoptions.ratio.curvalue];
            // fall through to apply mode constraints
            
        case ID_MODE:
            s_graphicsoptions.ratio.curvalue = resToRatio[s_graphicsoptions.mode.curvalue];
            break;
            
        case ID_LIST:
            ivo = &s_ivo_templates[s_graphicsoptions.list.curvalue];
            
            s_graphicsoptions.mode.curvalue = GraphicsOptions_FindDetectedResolution( ivo->mode );
            s_graphicsoptions.ratio.curvalue = resToRatio[s_graphicsoptions.mode.curvalue];
            s_graphicsoptions.tq.curvalue = ivo->tq;
            //s_graphicsoptions.colordepth.curvalue = ivo->colordepth;
            //s_graphicsoptions.texturebits.curvalue = ivo->texturebits;
            s_graphicsoptions.geometry.curvalue = ivo->geometry;
            s_graphicsoptions.filter.curvalue = ivo->filter;
            s_graphicsoptions.fs.curvalue = ivo->fullscreen;
            s_graphicsoptions.vsync.curvalue = ivo->vsync;
            s_graphicsoptions.compression.curvalue = ivo->compression;
            s_graphicsoptions.anisotropicFilter.curvalue = ivo->anisotropicFilter;
            s_graphicsoptions.deferredShading.curvalue = ivo->deferredShading;
            s_graphicsoptions.normalMapping.curvalue = ivo->normalMapping;
            s_graphicsoptions.parallax.curvalue = ivo->parallax;
            s_graphicsoptions.shadowType.curvalue = ivo->shadowType;
            s_graphicsoptions.shadowFilter.curvalue = ivo->shadowFilter;
            s_graphicsoptions.shadowBlur.curvalue = ivo->shadowBlur;
            s_graphicsoptions.shadowQuality.curvalue = ivo->shadowQuality;
            s_graphicsoptions.dynamicLightsCastShadows.curvalue = ivo->dynamicLightsCastShadows;
            s_graphicsoptions.hdr.curvalue = ivo->hdr;
            s_graphicsoptions.bloom.curvalue = ivo->bloom;
            break;
            
        case ID_DRIVERINFO:
            UI_DriverInfo_Menu();
            break;
            
        case ID_BACK2:
            UI_PopMenu();
            break;
            
        case ID_GRAPHICS:
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
            UI_PopMenu();
            UI_NetworkOptionsMenu();
            break;
        case ID_BRIGHTNESS:
            trap_Cvar_SetValue( "r_gamma", s_graphicsoptions.brightness.curvalue / 10.0f );
            break;
    }
}


/*
================
GraphicsOptions_TQEvent
================
*/
static void GraphicsOptions_TQEvent( void* ptr, int event )
{
    if( event != QM_ACTIVATED )
    {
        return;
    }
    s_graphicsoptions.tq.curvalue = ( int )( s_graphicsoptions.tq.curvalue + 0.5 );
}

/*
================
GraphicsOptions_AnisotropicFilterEvent
================
*/
static void GraphicsOptions_AnisotropicFilterEvent( void* ptr, int event )
{
    if( event != QM_ACTIVATED )
    {
        return;
    }
    s_graphicsoptions.anisotropicFilter.curvalue = ( int )( s_graphicsoptions.anisotropicFilter.curvalue + 0.5 );
}

/*
================
GraphicsOptions_ShadowBlurEvent
================
*/
static void GraphicsOptions_ShadowBlurEvent( void* ptr, int event )
{
    if( event != QM_ACTIVATED )
    {
        return;
    }
    s_graphicsoptions.shadowBlur.curvalue = ( int )( s_graphicsoptions.shadowBlur.curvalue + 0.5 );
}


/*
================
GraphicsOptions_MenuDraw
================
*/
void GraphicsOptions_MenuDraw( void )
{
//APSFIX - rework this
    GraphicsOptions_UpdateMenuItems();
    
    Menu_Draw( &s_graphicsoptions.menu );
}

/*
=================
GraphicsOptions_SetMenuItems
=================
*/
static void GraphicsOptions_SetMenuItems( void )
{
    s_graphicsoptions.mode.curvalue = GraphicsOptions_FindDetectedResolution( trap_Cvar_VariableValue( "r_mode" ) );
    
    if( s_graphicsoptions.mode.curvalue < 0 )
    {
        if( resolutionsDetected )
        {
            int             i;
            char            buf[MAX_STRING_CHARS];
            
            trap_Cvar_VariableStringBuffer( "r_customwidth", buf, sizeof( buf ) - 2 );
            buf[strlen( buf ) + 1] = 0;
            buf[strlen( buf )] = 'x';
            trap_Cvar_VariableStringBuffer( "r_customheight", buf + strlen( buf ), sizeof( buf ) - strlen( buf ) );
            
            for( i = 0; detectedResolutions[i]; ++i )
            {
                if( !Q_stricmp( buf, detectedResolutions[i] ) )
                {
                    s_graphicsoptions.mode.curvalue = i;
                    break;
                }
            }
            if( s_graphicsoptions.mode.curvalue < 0 )
                s_graphicsoptions.mode.curvalue = 0;
        }
        else
        {
            s_graphicsoptions.mode.curvalue = 3;
        }
    }
    s_graphicsoptions.ratio.curvalue = resToRatio[s_graphicsoptions.mode.curvalue];
    s_graphicsoptions.fs.curvalue = trap_Cvar_VariableValue( "r_fullscreen" );
    s_graphicsoptions.vsync.curvalue = trap_Cvar_VariableValue( "r_swapInterval" );
    s_graphicsoptions.tq.curvalue = 3 - trap_Cvar_VariableValue( "r_picmip" );
    if( s_graphicsoptions.tq.curvalue < 0 )
    {
        s_graphicsoptions.tq.curvalue = 0;
    }
    else if( s_graphicsoptions.tq.curvalue > 3 )
    {
        s_graphicsoptions.tq.curvalue = 3;
    }
    
    s_graphicsoptions.deferredShading.curvalue = trap_Cvar_VariableValue( "r_deferredShading" ) != 0;
    s_graphicsoptions.normalMapping.curvalue = trap_Cvar_VariableValue( "r_normalMapping" ) != 0;
    s_graphicsoptions.parallax.curvalue = trap_Cvar_VariableValue( "r_parallaxMapping" ) != 0;
    
    if( !Q_stricmp( UI_Cvar_VariableString( "r_textureMode" ), "GL_LINEAR_MIPMAP_NEAREST" ) )
    {
        s_graphicsoptions.filter.curvalue = 0;
    }
    else
    {
        s_graphicsoptions.filter.curvalue = 1;
    }
    
    if( trap_Cvar_VariableValue( "r_lodBias" ) > 0 )
    {
        if( trap_Cvar_VariableValue( "r_subdivisions" ) >= 20 )
        {
            s_graphicsoptions.geometry.curvalue = 0;
        }
        else
        {
            s_graphicsoptions.geometry.curvalue = 1;
        }
    }
    else
    {
        s_graphicsoptions.geometry.curvalue = 2;
    }
    
    /*
       switch ((int)trap_Cvar_VariableValue("r_colorbits"))
       {
       default:
       case 0:
       s_graphicsoptions.colordepth.curvalue = 0;
       break;
       case 16:
       s_graphicsoptions.colordepth.curvalue = 1;
       break;
       case 32:
       s_graphicsoptions.colordepth.curvalue = 2;
       break;
       }
       if(s_graphicsoptions.fs.curvalue == 0)
       {
       s_graphicsoptions.colordepth.curvalue = 0;
       }
     */
    
    s_graphicsoptions.compression.curvalue = trap_Cvar_VariableValue( "r_ext_texture_compression" );
    
    s_graphicsoptions.anisotropicFilter.curvalue = trap_Cvar_VariableValue( "r_ext_texture_filter_anisotropic" );
    if( s_graphicsoptions.anisotropicFilter.curvalue < 0 )
    {
        s_graphicsoptions.anisotropicFilter.curvalue = 0;
    }
    else if( s_graphicsoptions.anisotropicFilter.curvalue > uis.glconfig2.maxTextureAnisotropy )
    {
        s_graphicsoptions.anisotropicFilter.curvalue = uis.glconfig2.maxTextureAnisotropy;
    }
    
    s_graphicsoptions.shadowType.curvalue = trap_Cvar_VariableValue( "cg_shadows" );
    if( s_graphicsoptions.shadowType.curvalue < 0 )
    {
        s_graphicsoptions.shadowType.curvalue = 0;
    }
    else if( s_graphicsoptions.shadowType.curvalue > SHADOWING_BLOB )
    {
        if( !uis.glconfig2.framebufferObjectAvailable || !uis.glconfig2.textureFloatAvailable )
        {
            s_graphicsoptions.shadowType.curvalue = SHADOWING_BLOB;
        }
        else if( uis.glconfig.driverType != GLDRV_OPENGL3 )
        {
            if( uis.glconfig.hardwareType != GLHW_NV_DX10 && uis.glconfig.hardwareType != GLHW_ATI_DX10 )
            {
                s_graphicsoptions.shadowType.curvalue = SHADOWING_ESM16;
            }
        }
    }
    
    s_graphicsoptions.shadowFilter.curvalue = trap_Cvar_VariableValue( "r_softShadows" );
    s_graphicsoptions.shadowBlur.curvalue = trap_Cvar_VariableValue( "r_shadowBlur" );
    s_graphicsoptions.shadowQuality.curvalue = trap_Cvar_VariableValue( "r_shadowMapQuality" );
    s_graphicsoptions.dynamicLightsCastShadows.curvalue = trap_Cvar_VariableValue( "r_dynamicLightCastShadows" );
    
    s_graphicsoptions.hdr.curvalue = trap_Cvar_VariableValue( "r_hdrRendering" );
    if( s_graphicsoptions.hdr.curvalue > 0 )
    {
        if( !uis.glconfig2.framebufferObjectAvailable || !uis.glconfig2.textureFloatAvailable || !uis.glconfig2.framebufferBlitAvailable )
        {
            s_graphicsoptions.hdr.curvalue = 0;
        }
    }
    
    s_graphicsoptions.bloom.curvalue = trap_Cvar_VariableValue( "r_bloom" );
}

/*
================
GraphicsOptions_MenuInit
================
*/
void GraphicsOptions_MenuInit( void )
{
    /*static const char *s_driver_names[] = {
    	"Default",
    	"Voodoo",
    	NULL
    };
    
    static const char *tq_names[] = {
    	"Default",
    	"16 bit",
    	"32 bit",
    	NULL
    };
    
    static const char *s_graphics_options_names[] = {
    	"High Quality",
    	"Normal",
    	"Fast",
    	"Fastest",
    	"Custom",
    	NULL
    };
    
    static const char *lighting_names[] = {
    	"Real-Time",
    	"Deluxe Light Mapping",
    	NULL
    };
    
    static const char *colordepth_names[] = {
    	"Default",
    	"16 bit",
    	"32 bit",
    	NULL
    };*/
    
    static const char* filter_names[] =
    {
        "Bilinear",
        "Trilinear",
        NULL
    };
    
    static const char* quality_names[] =
    {
        "Low",
        "Medium",
        "High",
        NULL
    };
    
    static const char* shadowType_names[] =
    {
        "Off",
        "Blob",
        "ESM 16 bit",
        "ESM 32 bit",
        "VSM 16 bit",
        "VSM 32 bit",
        "EVSM 32 bit",
        NULL
    };
    
    static const char* shadowFilter_names[] =
    {
        "Off",
        "PCF 2x2",
        "PCF 3x3",
        "PCF 4x4",
        "PCF 5x5",
        "PCF 6x6",
        NULL
    };
    
    static const char* shadowQuality_names[] =
    {
        "Custom",
        "Low",
        "Medium",
        "High",
        "Very High",
//      "Ultra",
        NULL
    };
    
    static const char* bloom_names[] =
    {
        "Off",
        "On",
        NULL
    };
    
    static const char* enabled_names[] =
    {
        "Off",
        "On",
        NULL
    };
    
    int             y;
    
    // zero set all our globals
    memset( &s_graphicsoptions, 0, sizeof( graphicsoptions_t ) );
    
    GraphicsOptions_GetResolutions();
    GraphicsOptions_GetAspectRatios();
    
    GraphicsOptions_Cache();
    
    s_graphicsoptions.menu.wrapAround = true;
    s_graphicsoptions.menu.fullscreen = true;
    s_graphicsoptions.menu.draw = GraphicsOptions_MenuDraw;
    
    s_graphicsoptions.banner._generic.type = MTYPE_BTEXT;
    s_graphicsoptions.banner._generic.x = 320;
    s_graphicsoptions.banner._generic.y = 16;
    s_graphicsoptions.banner.string = "SYSTEM SETUP";
    s_graphicsoptions.banner.color = color_white;
    s_graphicsoptions.banner.style = UI_CENTER | UI_DROPSHADOW;
    
    /*	s_graphicsoptions.framel._generic.type = MTYPE_BITMAP;
    	s_graphicsoptions.framel._generic.name = GRAPHICSOPTIONS_FRAMEL;
    	s_graphicsoptions.framel._generic.flags = QMF_INACTIVE;
    	s_graphicsoptions.framel._generic.x = 0;
    	s_graphicsoptions.framel._generic.y = 78;
    	s_graphicsoptions.framel.width = 256;
    	s_graphicsoptions.framel.height = 329;
    
    	s_graphicsoptions.framer._generic.type = MTYPE_BITMAP;
    	s_graphicsoptions.framer._generic.name = GRAPHICSOPTIONS_FRAMER;
    	s_graphicsoptions.framer._generic.flags = QMF_INACTIVE;
    	s_graphicsoptions.framer._generic.x = 376;
    	s_graphicsoptions.framer._generic.y = 76;
    	s_graphicsoptions.framer.width = 256;
    	s_graphicsoptions.framer.height = 334;
    */
    
    s_graphicsoptions.back._generic.type = MTYPE_BITMAP;
    s_graphicsoptions.back._generic.name = UI_ART_BUTTON;
    s_graphicsoptions.back._generic.flags = QMF_LEFT_JUSTIFY | QMF_PULSEIFFOCUS;
    s_graphicsoptions.back._generic.callback = GraphicsOptions_Event;
    s_graphicsoptions.back._generic.id = ID_BACK2;
    s_graphicsoptions.back._generic.x = 0;
    s_graphicsoptions.back._generic.y = 480 - 64;
    s_graphicsoptions.back.width = 128;
    s_graphicsoptions.back.height = 64;
    s_graphicsoptions.back.focuspic = UI_ART_BUTTON_FOCUS;
    s_graphicsoptions.back._generic.caption.text = "back";
    s_graphicsoptions.back._generic.caption.style = UI_CENTER | UI_DROPSHADOW;
    s_graphicsoptions.back._generic.caption.fontsize = 0.6f;
    s_graphicsoptions.back._generic.caption.font = &uis.freeSansFont;
    s_graphicsoptions.back._generic.caption.color = text_color_normal;
    s_graphicsoptions.back._generic.caption.focuscolor = text_color_highlight;
    
    s_graphicsoptions.graphics._generic.type = MTYPE_BITMAP;
    s_graphicsoptions.graphics._generic.name = UI_ART_BUTTON;
    s_graphicsoptions.graphics._generic.flags = QMF_LEFT_JUSTIFY | QMF_PULSEIFFOCUS;
    s_graphicsoptions.graphics._generic.callback = GraphicsOptions_Event;
    s_graphicsoptions.graphics._generic.id = ID_GRAPHICS;
    s_graphicsoptions.graphics._generic.x = 128;
    s_graphicsoptions.graphics._generic.y = 480 - 64;
    s_graphicsoptions.graphics.width = 128;
    s_graphicsoptions.graphics.height = 64;
    s_graphicsoptions.graphics.focuspic = UI_ART_BUTTON_FOCUS;
    s_graphicsoptions.graphics._generic.caption.text = "graphics";
    s_graphicsoptions.graphics._generic.caption.style = UI_CENTER | UI_DROPSHADOW;
    s_graphicsoptions.graphics._generic.caption.fontsize = 0.6f;
    s_graphicsoptions.graphics._generic.caption.font = &uis.freeSansFont;
    s_graphicsoptions.graphics._generic.caption.color = text_color_normal;
    s_graphicsoptions.graphics._generic.caption.focuscolor = text_color_highlight;
    
    
    s_graphicsoptions.sound._generic.type = MTYPE_BITMAP;
    s_graphicsoptions.sound._generic.name = UI_ART_BUTTON;
    s_graphicsoptions.sound._generic.flags = QMF_LEFT_JUSTIFY | QMF_PULSEIFFOCUS;
    s_graphicsoptions.sound._generic.callback = GraphicsOptions_Event;
    s_graphicsoptions.sound._generic.id = ID_SOUND;
    s_graphicsoptions.sound._generic.x = 256;
    s_graphicsoptions.sound._generic.y = 480 - 64;
    s_graphicsoptions.sound.width = 128;
    s_graphicsoptions.sound.height = 64;
    s_graphicsoptions.sound.focuspic = UI_ART_BUTTON_FOCUS;
    s_graphicsoptions.sound._generic.caption.text = "sound";
    s_graphicsoptions.sound._generic.caption.style = UI_CENTER | UI_DROPSHADOW;
    s_graphicsoptions.sound._generic.caption.fontsize = 0.6f;
    s_graphicsoptions.sound._generic.caption.font = &uis.freeSansFont;
    s_graphicsoptions.sound._generic.caption.color = text_color_normal;
    s_graphicsoptions.sound._generic.caption.focuscolor = text_color_highlight;
    
    s_graphicsoptions.network._generic.type = MTYPE_BITMAP;
    s_graphicsoptions.network._generic.name = UI_ART_BUTTON;
    s_graphicsoptions.network._generic.flags = QMF_LEFT_JUSTIFY | QMF_PULSEIFFOCUS;
    s_graphicsoptions.network._generic.callback = GraphicsOptions_Event;
    s_graphicsoptions.network._generic.id = ID_NETWORK;
    s_graphicsoptions.network._generic.x = 384;
    s_graphicsoptions.network._generic.y = 480 - 64;
    s_graphicsoptions.network.width = 128;
    s_graphicsoptions.network.height = 64;
    s_graphicsoptions.network.focuspic = UI_ART_BUTTON_FOCUS;
    s_graphicsoptions.network._generic.caption.text = "network";
    s_graphicsoptions.network._generic.caption.style = UI_CENTER | UI_DROPSHADOW;
    s_graphicsoptions.network._generic.caption.fontsize = 0.6f;
    s_graphicsoptions.network._generic.caption.font = &uis.freeSansFont;
    s_graphicsoptions.network._generic.caption.color = text_color_normal;
    s_graphicsoptions.network._generic.caption.focuscolor = text_color_highlight;
    
    s_graphicsoptions.apply._generic.type = MTYPE_BITMAP;
    s_graphicsoptions.apply._generic.name = UI_ART_BUTTON;
    s_graphicsoptions.apply._generic.flags = QMF_RIGHT_JUSTIFY | QMF_PULSEIFFOCUS | QMF_GRAYED | QMF_INACTIVE;
    s_graphicsoptions.apply._generic.callback = GraphicsOptions_ApplyChanges;
    s_graphicsoptions.apply._generic.x = 640;
    s_graphicsoptions.apply._generic.y = 480 - 64;
    s_graphicsoptions.apply.width = 128;
    s_graphicsoptions.apply.height = 64;
    s_graphicsoptions.apply.focuspic = UI_ART_BUTTON_FOCUS;
    s_graphicsoptions.apply._generic.caption.text = "apply";
    s_graphicsoptions.apply._generic.caption.style = UI_CENTER | UI_DROPSHADOW;
    s_graphicsoptions.apply._generic.caption.fontsize = 0.6f;
    s_graphicsoptions.apply._generic.caption.font = &uis.freeSansFont;
    s_graphicsoptions.apply._generic.caption.color = text_color_normal;
    s_graphicsoptions.apply._generic.caption.focuscolor = text_color_highlight;
    
    
    
    
    
    y = 180 - 7 * ( BIGCHAR_HEIGHT + 2 );
    /*
    otty: do we need this ?
    	s_graphicsoptions.list._generic.type = MTYPE_SPINCONTROL;
    	s_graphicsoptions.list._generic.name = "Graphics Settings:";
    	s_graphicsoptions.list._generic.flags = QMF_PULSEIFFOCUS | QMF_SMALLFONT;
    	s_graphicsoptions.list._generic.x = 320;
    	s_graphicsoptions.list._generic.y = y;
    	s_graphicsoptions.list._generic.callback = GraphicsOptions_Event;
    	s_graphicsoptions.list._generic.id = ID_LIST;
    	s_graphicsoptions.list.itemnames = s_graphics_options_names;
    	y += 2 * (BIGCHAR_HEIGHT + 2);
    */
    s_graphicsoptions.ratio._generic.type = MTYPE_SPINCONTROL;
    s_graphicsoptions.ratio._generic.name = "Aspect Ratio:";
    s_graphicsoptions.ratio._generic.flags = QMF_PULSEIFFOCUS | QMF_SMALLFONT;
    s_graphicsoptions.ratio._generic.x = 320;
    s_graphicsoptions.ratio._generic.y = y;
    s_graphicsoptions.ratio.itemnames = ratios;
    s_graphicsoptions.ratio._generic.callback = GraphicsOptions_Event;
    s_graphicsoptions.ratio._generic.id = ID_RATIO;
    y += BIGCHAR_HEIGHT + 2;
    
    // references/modifies "r_mode"
    s_graphicsoptions.mode._generic.type = MTYPE_SPINCONTROL;
    s_graphicsoptions.mode._generic.name = "Resolution:";
    s_graphicsoptions.mode._generic.flags = QMF_PULSEIFFOCUS | QMF_SMALLFONT;
    s_graphicsoptions.mode._generic.x = 320;
    s_graphicsoptions.mode._generic.y = y;
    s_graphicsoptions.mode.itemnames = resolutions;
    s_graphicsoptions.mode._generic.callback = GraphicsOptions_Event;
    s_graphicsoptions.mode._generic.id = ID_MODE;
    y += BIGCHAR_HEIGHT + 2;
    
    // references "r_colorbits"
    /*
       s_graphicsoptions.colordepth._generic.type = MTYPE_SPINCONTROL;
       s_graphicsoptions.colordepth._generic.name = "Color Depth:";
       s_graphicsoptions.colordepth._generic.flags = QMF_PULSEIFFOCUS | QMF_SMALLFONT;
       s_graphicsoptions.colordepth._generic.x = 320;
       s_graphicsoptions.colordepth._generic.y = y;
       s_graphicsoptions.colordepth.itemnames = colordepth_names;
       y += BIGCHAR_HEIGHT + 2;
     */
    
    // references/modifies "r_fullscreen"
    s_graphicsoptions.fs._generic.type = MTYPE_SPINCONTROL;
    s_graphicsoptions.fs._generic.name = "Fullscreen:";
    s_graphicsoptions.fs._generic.flags = QMF_PULSEIFFOCUS | QMF_SMALLFONT;
    s_graphicsoptions.fs._generic.x = 320;
    s_graphicsoptions.fs._generic.y = y;
    s_graphicsoptions.fs.itemnames = enabled_names;
    y += BIGCHAR_HEIGHT + 2;
    
    // references/modifies "r_swapInterval"
    s_graphicsoptions.vsync._generic.type = MTYPE_SPINCONTROL;
    s_graphicsoptions.vsync._generic.name = "VSync:";
    s_graphicsoptions.vsync._generic.flags = QMF_PULSEIFFOCUS | QMF_SMALLFONT;
    s_graphicsoptions.vsync._generic.x = 320;
    s_graphicsoptions.vsync._generic.y = y;
    s_graphicsoptions.vsync.itemnames = enabled_names;
    y += BIGCHAR_HEIGHT + 2;
    
    s_graphicsoptions.brightness._generic.type = MTYPE_SLIDER;
    s_graphicsoptions.brightness._generic.name = "Brightness:";
    s_graphicsoptions.brightness._generic.flags = QMF_PULSEIFFOCUS | QMF_SMALLFONT;
    s_graphicsoptions.brightness._generic.callback = GraphicsOptions_Event;
    s_graphicsoptions.brightness._generic.id = ID_BRIGHTNESS;
    s_graphicsoptions.brightness._generic.x = 320;
    s_graphicsoptions.brightness._generic.y = y;
    s_graphicsoptions.brightness.minvalue = 5;
    s_graphicsoptions.brightness.maxvalue = 20;
    if( !uis.glconfig.deviceSupportsGamma )
    {
        s_graphicsoptions.brightness._generic.flags |= QMF_GRAYED;
    }
    y += BIGCHAR_HEIGHT + 2;
    
    // references/modifies "r_lodBias" & "subdivisions"
    s_graphicsoptions.geometry._generic.type = MTYPE_SPINCONTROL;
    s_graphicsoptions.geometry._generic.name = "Geometric Detail:";
    s_graphicsoptions.geometry._generic.flags = QMF_PULSEIFFOCUS | QMF_SMALLFONT;
    s_graphicsoptions.geometry._generic.x = 320;
    s_graphicsoptions.geometry._generic.y = y;
    s_graphicsoptions.geometry.itemnames = quality_names;
    y += BIGCHAR_HEIGHT + 2;
    
    // references/modifies "r_picmip"
    s_graphicsoptions.tq._generic.type = MTYPE_SLIDER;
    s_graphicsoptions.tq._generic.name = "Texture Detail:";
    s_graphicsoptions.tq._generic.flags = QMF_PULSEIFFOCUS | QMF_SMALLFONT;
    s_graphicsoptions.tq._generic.x = 320;
    s_graphicsoptions.tq._generic.y = y;
    s_graphicsoptions.tq.minvalue = 0;
    s_graphicsoptions.tq.maxvalue = 3;
    s_graphicsoptions.tq._generic.callback = GraphicsOptions_TQEvent;
    y += BIGCHAR_HEIGHT + 2;
    
    // references/modifies "r_textureMode"
    s_graphicsoptions.filter._generic.type = MTYPE_SPINCONTROL;
    s_graphicsoptions.filter._generic.name = "Texture Filter:";
    s_graphicsoptions.filter._generic.flags = QMF_PULSEIFFOCUS | QMF_SMALLFONT;
    s_graphicsoptions.filter._generic.x = 320;
    s_graphicsoptions.filter._generic.y = y;
    s_graphicsoptions.filter.itemnames = filter_names;
    y += BIGCHAR_HEIGHT + 2;
    
#if 1
    // references/modifies "r_ext_texture_compression"
    if( strstr( uis.glconfig.extensions_string, "GL_ARB_texture_compression" ) ||
            strstr( uis.glconfig.extensions_string, "GL_EXT_texture_compression_s3tc" ) )
    {
        s_graphicsoptions.compression._generic.type = MTYPE_SPINCONTROL;
        s_graphicsoptions.compression._generic.name = "Texture Compression:";
        s_graphicsoptions.compression._generic.flags = QMF_PULSEIFFOCUS | QMF_SMALLFONT;
        s_graphicsoptions.compression._generic.x = 320;
        s_graphicsoptions.compression._generic.y = y;
        s_graphicsoptions.compression.itemnames = enabled_names;
        y += BIGCHAR_HEIGHT + 2;
    }
#endif
    
    // references/modifies "r_ext_texture_filter_anisotropic"
    if( strstr( uis.glconfig.extensions_string, "GL_EXT_texture_filter_anisotropic" ) )
    {
        s_graphicsoptions.anisotropicFilter._generic.type = MTYPE_SLIDER;
        s_graphicsoptions.anisotropicFilter._generic.name = "Anisotropic Filter:";
        s_graphicsoptions.anisotropicFilter._generic.flags = QMF_PULSEIFFOCUS | QMF_SMALLFONT;
        s_graphicsoptions.anisotropicFilter._generic.x = 320;
        s_graphicsoptions.anisotropicFilter._generic.y = y;
        s_graphicsoptions.anisotropicFilter.minvalue = 0;
        s_graphicsoptions.anisotropicFilter.maxvalue = uis.glconfig2.maxTextureAnisotropy;
        s_graphicsoptions.anisotropicFilter._generic.callback = GraphicsOptions_AnisotropicFilterEvent;
        y += BIGCHAR_HEIGHT + 2;
    }
    
    // references/modifies "r_deferredShading"
    s_graphicsoptions.deferredShading._generic.type = MTYPE_SPINCONTROL;
    s_graphicsoptions.deferredShading._generic.name = "Deferred Shading:";
    s_graphicsoptions.deferredShading._generic.flags = QMF_PULSEIFFOCUS | QMF_SMALLFONT;
    s_graphicsoptions.deferredShading._generic.x = 320;
    s_graphicsoptions.deferredShading._generic.y = y;
    s_graphicsoptions.deferredShading.itemnames = enabled_names;
    y += BIGCHAR_HEIGHT + 2;
    
    // references/modifies "r_normalMapping"
    s_graphicsoptions.normalMapping._generic.type = MTYPE_SPINCONTROL;
    s_graphicsoptions.normalMapping._generic.name = "Normal Mapping:";
    s_graphicsoptions.normalMapping._generic.flags = QMF_PULSEIFFOCUS | QMF_SMALLFONT;
    s_graphicsoptions.normalMapping._generic.x = 320;
    s_graphicsoptions.normalMapping._generic.y = y;
    s_graphicsoptions.normalMapping.itemnames = enabled_names;
    y += BIGCHAR_HEIGHT + 2;
    
    // references/modifies "r_parallaxMapping"
    s_graphicsoptions.parallax._generic.type = MTYPE_SPINCONTROL;
    s_graphicsoptions.parallax._generic.name = "Relief Mapping:";
    s_graphicsoptions.parallax._generic.flags = QMF_PULSEIFFOCUS | QMF_SMALLFONT;
    s_graphicsoptions.parallax._generic.x = 320;
    s_graphicsoptions.parallax._generic.y = y;
    s_graphicsoptions.parallax.itemnames = enabled_names;
    y += BIGCHAR_HEIGHT + 2;
    
    // references/modifies "r_shadows"
    s_graphicsoptions.shadowType._generic.type = MTYPE_SPINCONTROL;
    s_graphicsoptions.shadowType._generic.name = "Shadow Type:";
    s_graphicsoptions.shadowType._generic.flags = QMF_PULSEIFFOCUS | QMF_SMALLFONT;
    s_graphicsoptions.shadowType._generic.x = 320;
    s_graphicsoptions.shadowType._generic.y = y;
    s_graphicsoptions.shadowType.itemnames = shadowType_names;
    y += BIGCHAR_HEIGHT + 2;
    
    // references/modifies "r_softShadows"
    s_graphicsoptions.shadowFilter._generic.type = MTYPE_SPINCONTROL;
    s_graphicsoptions.shadowFilter._generic.name = "Shadow Filter:";
    s_graphicsoptions.shadowFilter._generic.flags = QMF_PULSEIFFOCUS | QMF_SMALLFONT;
    s_graphicsoptions.shadowFilter._generic.x = 320;
    s_graphicsoptions.shadowFilter._generic.y = y;
    s_graphicsoptions.shadowFilter.itemnames = shadowFilter_names;
    y += BIGCHAR_HEIGHT + 2;
    
    // references/modifies "r_shadowBlur"
    s_graphicsoptions.shadowBlur._generic.type = MTYPE_SLIDER;
    s_graphicsoptions.shadowBlur._generic.name = "Shadow Blur:";
    s_graphicsoptions.shadowBlur._generic.flags = QMF_PULSEIFFOCUS | QMF_SMALLFONT;
    s_graphicsoptions.shadowBlur._generic.x = 320;
    s_graphicsoptions.shadowBlur._generic.y = y;
    s_graphicsoptions.shadowBlur.minvalue = 1;
    s_graphicsoptions.shadowBlur.maxvalue = 10;
    s_graphicsoptions.shadowBlur._generic.callback = GraphicsOptions_ShadowBlurEvent;
    y += BIGCHAR_HEIGHT + 2;
    
    // references/modifies "r_shadowMapQuality"
    s_graphicsoptions.shadowQuality._generic.type = MTYPE_SPINCONTROL;
    s_graphicsoptions.shadowQuality._generic.name = "Shadow Map Quality:";
    s_graphicsoptions.shadowQuality._generic.flags = QMF_PULSEIFFOCUS | QMF_SMALLFONT;
    s_graphicsoptions.shadowQuality._generic.x = 320;
    s_graphicsoptions.shadowQuality._generic.y = y;
    s_graphicsoptions.shadowQuality.itemnames = shadowQuality_names;
    y += BIGCHAR_HEIGHT + 2;
    
    // references/modifies "r_dynamicLightCastShadows"
    s_graphicsoptions.dynamicLightsCastShadows._generic.type = MTYPE_SPINCONTROL;
    s_graphicsoptions.dynamicLightsCastShadows._generic.name = "Dynamic Light Shadows:";
    s_graphicsoptions.dynamicLightsCastShadows._generic.flags = QMF_PULSEIFFOCUS | QMF_SMALLFONT;
    s_graphicsoptions.dynamicLightsCastShadows._generic.x = 320;
    s_graphicsoptions.dynamicLightsCastShadows._generic.y = y;
    s_graphicsoptions.dynamicLightsCastShadows.itemnames = enabled_names;
    y += BIGCHAR_HEIGHT + 2;
    
    // references/modifies "r_hdrRendering"
    s_graphicsoptions.hdr._generic.type = MTYPE_SPINCONTROL;
    s_graphicsoptions.hdr._generic.name = "HDR Rendering:";
    s_graphicsoptions.hdr._generic.flags = QMF_PULSEIFFOCUS | QMF_SMALLFONT;
    s_graphicsoptions.hdr._generic.x = 320;
    s_graphicsoptions.hdr._generic.y = y;
    s_graphicsoptions.hdr.itemnames = enabled_names;
    y += BIGCHAR_HEIGHT + 2;
    
    // references/modifies "r_bloom"
    s_graphicsoptions.bloom._generic.type = MTYPE_SPINCONTROL;
    s_graphicsoptions.bloom._generic.name = "Bloom:";
    s_graphicsoptions.bloom._generic.flags = QMF_PULSEIFFOCUS | QMF_SMALLFONT;
    s_graphicsoptions.bloom._generic.x = 320;
    s_graphicsoptions.bloom._generic.y = y;
    s_graphicsoptions.bloom.itemnames = bloom_names;
    y += 2 * BIGCHAR_HEIGHT;
    
    /*	s_graphicsoptions.driverinfo._generic.type = MTYPE_PTEXT;
    	s_graphicsoptions.driverinfo._generic.flags = QMF_CENTER_JUSTIFY | QMF_PULSEIFFOCUS;
    	s_graphicsoptions.driverinfo._generic.callback = GraphicsOptions_Event;
    	s_graphicsoptions.driverinfo._generic.id = ID_DRIVERINFO;
    	s_graphicsoptions.driverinfo._generic.x = 320;
    	s_graphicsoptions.driverinfo._generic.y = y;
    	s_graphicsoptions.driverinfo.string = "Driver Info";
    	s_graphicsoptions.driverinfo.style = UI_CENTER | UI_SMALLFONT;
    	s_graphicsoptions.driverinfo.color = color_red;
    	y += BIGCHAR_HEIGHT + 2;
    */
    
    
    Menu_AddItem( &s_graphicsoptions.menu, ( void* )&s_graphicsoptions.banner );
//  Menu_AddItem(&s_graphicsoptions.menu, (void *)&s_graphicsoptions.framel);
//  Menu_AddItem(&s_graphicsoptions.menu, (void *)&s_graphicsoptions.framer);

    Menu_AddItem( &s_graphicsoptions.menu, ( void* )&s_graphicsoptions.graphics );
//  Menu_AddItem(&s_graphicsoptions.menu, (void *)&s_graphicsoptions.display);
    Menu_AddItem( &s_graphicsoptions.menu, ( void* )&s_graphicsoptions.sound );
    Menu_AddItem( &s_graphicsoptions.menu, ( void* )&s_graphicsoptions.network );
    
    //Menu_AddItem(&s_graphicsoptions.menu, (void *)&s_graphicsoptions.list);
    Menu_AddItem( &s_graphicsoptions.menu, ( void* )&s_graphicsoptions.ratio );
    Menu_AddItem( &s_graphicsoptions.menu, ( void* )&s_graphicsoptions.mode );
//  Menu_AddItem(&s_graphicsoptions.menu, (void *)&s_graphicsoptions.colordepth);
    Menu_AddItem( &s_graphicsoptions.menu, ( void* )&s_graphicsoptions.fs );
    Menu_AddItem( &s_graphicsoptions.menu, ( void* )&s_graphicsoptions.vsync );
    Menu_AddItem( &s_graphicsoptions.menu, ( void* )&s_graphicsoptions.brightness );
    Menu_AddItem( &s_graphicsoptions.menu, ( void* )&s_graphicsoptions.geometry );
    Menu_AddItem( &s_graphicsoptions.menu, ( void* )&s_graphicsoptions.tq );
//	Menu_AddItem(&s_graphicsoptions.menu, (void *)&s_graphicsoptions.texturebits);
    Menu_AddItem( &s_graphicsoptions.menu, ( void* )&s_graphicsoptions.filter );
    
#if 1
    if( strstr( uis.glconfig.extensions_string, "GL_ARB_texture_compression" ) ||
            strstr( uis.glconfig.extensions_string, "GL_EXT_texture_compression_s3tc" ) )
        Menu_AddItem( &s_graphicsoptions.menu, ( void* )&s_graphicsoptions.compression );
#endif
        
    if( strstr( uis.glconfig.extensions_string, "GL_EXT_texture_filter_anisotropic" ) )
        Menu_AddItem( &s_graphicsoptions.menu, ( void* )&s_graphicsoptions.anisotropicFilter );
        
    Menu_AddItem( &s_graphicsoptions.menu, ( void* )&s_graphicsoptions.deferredShading );
    Menu_AddItem( &s_graphicsoptions.menu, ( void* )&s_graphicsoptions.normalMapping );
    Menu_AddItem( &s_graphicsoptions.menu, ( void* )&s_graphicsoptions.parallax );
    
    Menu_AddItem( &s_graphicsoptions.menu, ( void* )&s_graphicsoptions.shadowType );
    Menu_AddItem( &s_graphicsoptions.menu, ( void* )&s_graphicsoptions.shadowFilter );
    Menu_AddItem( &s_graphicsoptions.menu, ( void* )&s_graphicsoptions.shadowBlur );
    Menu_AddItem( &s_graphicsoptions.menu, ( void* )&s_graphicsoptions.shadowQuality );
    Menu_AddItem( &s_graphicsoptions.menu, ( void* )&s_graphicsoptions.dynamicLightsCastShadows );
    Menu_AddItem( &s_graphicsoptions.menu, ( void* )&s_graphicsoptions.hdr );
    Menu_AddItem( &s_graphicsoptions.menu, ( void* )&s_graphicsoptions.bloom );
    
    //Menu_AddItem(&s_graphicsoptions.menu, (void *)&s_graphicsoptions.driverinfo);
    
    Menu_AddItem( &s_graphicsoptions.menu, ( void* )&s_graphicsoptions.back );
    Menu_AddItem( &s_graphicsoptions.menu, ( void* )&s_graphicsoptions.apply );
    
    s_graphicsoptions.brightness.curvalue = trap_Cvar_VariableValue( "r_gamma" ) * 10;
    
    GraphicsOptions_SetMenuItems();
    GraphicsOptions_GetInitialVideo();
}

/*
=================
GraphicsOptions_Cache
=================
*/
void GraphicsOptions_Cache( void )
{
    trap_R_RegisterShaderNoMip( GRAPHICSOPTIONS_FRAMEL );
    trap_R_RegisterShaderNoMip( GRAPHICSOPTIONS_FRAMER );
    
}


/*
=================
UI_GraphicsOptionsMenu
=================
*/
void UI_GraphicsOptionsMenu( void )
{
    GraphicsOptions_MenuInit();
    UI_PushMenu( &s_graphicsoptions.menu );
    Menu_SetCursorToItem( &s_graphicsoptions.menu, &s_graphicsoptions.graphics );
}
