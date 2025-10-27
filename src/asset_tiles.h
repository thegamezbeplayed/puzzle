//////////////////////////////////////////////////////////////////////////////////
//                                                                              //
// rTexpacker v5.0 Atlas Descriptor Code exporter v5.0                          //
//                                                                              //
// more info and bugs-report:  github.com/raylibtech/rtools                     //
// feedback and support:       ray[at]raylibtech.com                            //
//                                                                              //
// Copyright (c) 2019-2025 raylib technologies (@raylibtech)                    //
//                                                                              //
//////////////////////////////////////////////////////////////////////////////////

#define ATLAS_ASSET_TILES_IMAGE_PATH      "asset_tiles.png"
#define ATLAS_ASSET_TILES_SPRITE_COUNT    18

// Atlas sprite properties
typedef struct rtpAtlasSprite {
    const char *nameId;
    const char *tag;
    int originX, originY;
    int positionX, positionY;
    int sourceWidth, sourceHeight;
    int padding;
    bool trimmed;
    int trimRecX, trimRecY, trimRecWidth, trimRecHeight;
    int colliderType;
    int colliderPosX, colliderPosY, colliderSizeX, colliderSizeY;
} rtpAtlasSprite;

// Atlas sprites array
static rtpAtlasSprite rtpDescAssetTiles[18] = {
    { "BackTile_01", "", 128, 128, 256, 552, 256, 256, 0, false, 0, 0, 256, 256, 0, 0, 0, 0, 0 },
    { "BackTile_02", "", 138, 138, 0, 0, 276, 276, 0, false, 0, 0, 276, 276, 0, 0, 0, 0, 0 },
    { "BackTile_03", "", 128, 128, 1064, 276, 256, 256, 0, false, 0, 0, 256, 256, 0, 0, 0, 0, 0 },
    { "BackTile_04", "", 138, 138, 276, 0, 276, 276, 0, false, 0, 0, 276, 276, 0, 0, 0, 0, 0 },
    { "BackTile_05", "", 128, 128, 552, 276, 256, 256, 0, false, 0, 0, 256, 256, 0, 0, 0, 0, 0 },
    { "BackTile_06", "", 138, 138, 552, 0, 276, 276, 0, false, 0, 0, 276, 276, 0, 0, 0, 0, 0 },
    { "BackTile_07", "", 128, 128, 1576, 276, 256, 256, 0, false, 0, 0, 256, 256, 0, 0, 0, 0, 0 },
    { "BackTile_08", "", 138, 138, 828, 0, 276, 276, 0, false, 0, 0, 276, 276, 0, 0, 0, 0, 0 },
    { "BackTile_09", "", 128, 128, 768, 552, 256, 256, 0, false, 0, 0, 256, 256, 0, 0, 0, 0, 0 },
    { "BackTile_10", "", 138, 138, 1104, 0, 276, 276, 0, false, 0, 0, 276, 276, 0, 0, 0, 0, 0 },
    { "BackTile_11", "", 128, 128, 808, 276, 256, 256, 0, false, 0, 0, 256, 256, 0, 0, 0, 0, 0 },
    { "BackTile_12", "", 138, 138, 1380, 0, 276, 276, 0, false, 0, 0, 276, 276, 0, 0, 0, 0, 0 },
    { "BackTile_13", "", 128, 128, 1320, 276, 256, 256, 0, false, 0, 0, 256, 256, 0, 0, 0, 0, 0 },
    { "BackTile_14", "", 138, 138, 1656, 0, 276, 276, 0, false, 0, 0, 276, 276, 0, 0, 0, 0, 0 },
    { "BackTile_15", "", 128, 128, 0, 552, 256, 256, 0, false, 0, 0, 256, 256, 0, 0, 0, 0, 0 },
    { "BackTile_16", "", 138, 138, 0, 276, 276, 276, 0, false, 0, 0, 276, 276, 0, 0, 0, 0, 0 },
    { "BackTile_17", "", 128, 128, 512, 552, 256, 256, 0, false, 0, 0, 256, 256, 0, 0, 0, 0, 0 },
    { "BackTile_18", "", 138, 138, 276, 276, 276, 276, 0, false, 0, 0, 276, 276, 0, 0, 0, 0, 0 },
};
