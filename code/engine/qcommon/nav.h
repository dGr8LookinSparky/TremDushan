
// should be the same as in rest of engine + map compiler
// should be the same as in rest of engine
#ifndef STEPSIZE
#define	STEPSIZE		18.f
#endif

#define MAX_PATH_POLYS 512

#define recast2quake(a,b)((b)[0]=-(a)[0], (b)[1]=-(a)[2], (b)[2]=(a)[1])
#define quake2recast(a,b)((b)[0]=-(a)[0], (b)[1]=(a)[2], (b)[2]=-(a)[1])

// make this quake 3!!
enum SamplePolyAreas
{
    SAMPLE_POLYAREA_GROUND,
    SAMPLE_POLYAREA_WATER,
    SAMPLE_POLYAREA_ROAD,
    SAMPLE_POLYAREA_DOOR,
    SAMPLE_POLYAREA_GRASS,
    SAMPLE_POLYAREA_JUMP,
};
enum SamplePolyFlags
{
    SAMPLE_POLYFLAGS_WALK		= 0x01,		// Ability to walk (ground, grass, road)
    SAMPLE_POLYFLAGS_SWIM		= 0x02,		// Ability to swim (water).
    SAMPLE_POLYFLAGS_DOOR		= 0x04,		// Ability to move through doors.
    SAMPLE_POLYFLAGS_JUMP		= 0x08,		// Ability to jump.
    SAMPLE_POLYFLAGS_DISABLED	= 0x10,		// Disabled polygon
    SAMPLE_POLYFLAGS_ALL		= 0xffff	// All abilities.
};


typedef struct
{

    int version;
    int vertsOffset;
    int numVerts;
    int polysOffset;
    int areasOffset;
    int flagsOffset;
    int numPolys;
    int numVertsPerPoly;
    vec3_t mins;
    vec3_t maxs;
    
    int dMeshesOffset;
    int dNumMeshes;
    int dVertsOffset;
    int dNumVerts;
    int dTrisOffset;
    int dNumTris;
    
    float cellSize;
    float cellHeight;
    
    int filesize;
    
} navMeshDataInfo_t;

/*
 *
 * WAYPOINTS
 *
 *
 */

#define WAYPOINT_MAX 8192

typedef struct
{
    vec3_t point;
    vec3_t type;
    
} waypoint_t;

/*
 *
 * COVER PLATES
 *
 */

#define COVER_PLATES_MAX 1024
#define COVER_PLATES_MAX_SLICE 256
#define COVER_PLATES_SLICE_STEP 16
#define COVER_PLATES_SLICE_HEIGHT_STEP 4

// 3 * cell size is good
#define COVER_PLATE_ORI_CHECK_DIST 24.0f

typedef struct
{
    vec3_t pos;
    float height;
    
} cpslice_t;

typedef struct
{
    // info about the origin edge
    vec3_t start;
    vec3_t end;
    vec3_t center;
    vec3_t ori;
    float len;
    
    // slicing
    cpslice_t sl[COVER_PLATES_MAX_SLICE];
    int nsl;
    
    
} cplate_t;
/*
 *
 * COVER POINTS
 *
 */

#define COVER_MAX 1024
#define COVER_ACCURACY 32
#define COVER_DIST 64.0f
#define COVER_HEIGHT 24.0f
#define COVER_MIN_DIST 64.0f
#define COVER_MIN_EDGE 48.0f

typedef struct
{
    vec3_t point;
    vec3_t dir[COVER_ACCURACY];
    
    vec3_t a, b;
    float angle;
    
    vec3_t ori;
    
} cover_t;



