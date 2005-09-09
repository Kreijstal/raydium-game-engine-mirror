/*
    Raydium - CQFD Corp.
    http://raydium.cqfd-corp.org
    License: GPL - GNU General Public License, see "gpl.txt" file.
*/

// Raydium configuration file
// For other options, see common.h

// Enable PHP support
#ifndef NO_PHP_SUPPORT
#define PHP_SUPPORT
#define PHP_INI_PATH "./"
#endif


#ifndef NO_ODE_SUPPORT
// Enable ODE physics support
#define ODE_SUPPORT
#define ODE_QUICKSTEP
#define ODE_PREDICTION
//#define ODE_NETWORK_GHOSTS
#endif

// RegApi Support (exports Raydium API to PHP)
#define REG_API

// enable profiling
#define DEBUG_PROFILE

// (link to OpenAL is always required !)
//#define NO_SOUND_DEBUG

// will highlight triangle with tag != 0 at render time :
//#define RENDER_DEBUG_TAG

// enable key traces
//#define DEBUG_KEYS

// disable clipping
//#define RENDER_DEBUG_NO_CLIP

// disable display list optims
//#define DEBUG_RENDER_DISABLE_DISPLAYLISTS

// debug network "tcp style" ACKs
//#define DEBUG_NETWORK

// debug ODE Network interface (verbose !)
//#define DEBUG_ODENET

// movie linear framerate (1/x sec) (starts recording, too)
//#define DEBUG_MOVIE 25

// allow vertex lighting for lightmaps textures
//#define RENDER_ALLOW_LIGHTING_FOR_LIGHTMAPS

// raydium_capture_frame_jpeg() JPEG quality percentage
#define DEBUG_JPEG_QUALITY	75
