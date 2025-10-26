#ifndef TEXTURE_ATLAS_H
#define TEXTURE_ATLAS_H
#include <stdbool.h>


static const char *TILES_IMAGE_PATH = "asset_tiles.png";

// ============================================================================
// Back Tiles
// ============================================================================
typedef enum {
    BACKTILE_01,
    BACKTILE_02,
    BACKTILE_03,
    BACKTILE_04,
    BACKTILE_05,
    BACKTILE_06,
    BACKTILE_07,
    BACKTILE_08,
    BACKTILE_09,
    BACKTILE_10,
    BACKTILE_11,
    BACKTILE_12,
    BACKTILE_13,
    BACKTILE_14,
    BACKTILE_15,
    BACKTILE_16,
    BACKTILE_17,
    BACKTILE_18,
    BACKTILE_DONE
} BackTileID;

static const sub_texture_t BACK_TILES[] = {
  {BACKTILE_01, 128,128, 256,552, 256,256, 0,false,0,0,256,256},
  {BACKTILE_02, 138,138, 0,0, 276,276, 0,false,0,0,276, 276},
  {BACKTILE_03, 128,128, 556,794, 256,256, 0,false,0,0,0, 0,0,0,0,0},
  {BACKTILE_04, 138,138, 0,556, 276,276, 0,false,0,0,0, 0,0,0,0,0},
  {BACKTILE_05, 128,128, 814,1052, 256,256, 0,false,0,0,0, 0,0,0,0,0},
  {BACKTILE_06, 138,138, 278,556, 276,276, 0,false,0,0,0, 0,0,0,0,0},
  {BACKTILE_07, 128,128, 814,536, 256,256, 0,false,0,0,0, 0,0,0,0,0},
  {BACKTILE_08, 0,0, 278,834, 276,276, 0,false,0,0,0, 0,0,0,0,0},
  {BACKTILE_09, 0,0, 556,1052, 256,256, 0,false,0,0,0, 0,0,0,0,0},
  {BACKTILE_10, 0,0, 556,0, 276,276, 0,false,0,0,0, 0,0,0,0,0},
  {BACKTILE_11, 0,0, 814,794, 256,256, 0,false,0,0,0, 0,0,0,0,0},
  {BACKTILE_12, 0,0, 278,0, 276,276, 0,false,0,0,0, 0,0,0,0,0},
  {BACKTILE_13, 0,0, 834,0, 256,256, 0,false,0,0,0, 0,0,0,0,0},
  {BACKTILE_14, 0,0, 0,0, 276,276, 0,false,0,0,0, 0,0,0,0,0},
  {BACKTILE_15, 0,0, 556,278, 256,256, 0,false,0,0,0, 0,0,0,0,0},
  {BACKTILE_16, 0,0, 278,278, 276,276, 0,false,0,0,0, 0,0,0,0,0},
  {BACKTILE_17, 0,0, 556,536, 256,256, 0,false,0,0,0, 0,0,0,0,0},
  {BACKTILE_18, 0,0, 0,834, 276,276, 0,false,0,0,0, 0,0,0,0,0}
};

#endif // TEXTURE_ATLAS_H

