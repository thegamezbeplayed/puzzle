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

#define ATLAS_ASSET_SPRITES_IMAGE_PATH      "asset_sprites.png"
#define ATLAS_ASSET_SPRITES_SPRITE_COUNT    42

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
static rtpAtlasSprite rtpDescAssetSprites[42] = {
    { "star_large", "", 64, 64, 0, 0, 128, 128, 0, false, 16, 16, 96, 96, 0, 0, 0, 0, 0 },
    { "star_medium", "", 64, 64, 128, 0, 128, 128, 0, false, 16, 16, 96, 96, 0, 0, 0, 0, 0 },
    { "station_A", "", 64, 64, 256, 0, 128, 128, 0, false, 16, 16, 96, 96, 0, 0, 0, 0, 0 },
    { "station_B", "", 64, 64, 384, 0, 128, 128, 0, false, 8, 8, 112, 112, 0, 0, 0, 0, 0 },
    { "station_C", "", 64, 64, 512, 0, 128, 128, 0, false, 4, 4, 120, 120, 0, 0, 0, 0, 0 },
    { "ship_sidesD", "", 64, 64, 640, 0, 128, 128, 0, false, 16, 32, 96, 64, 0, 0, 0, 0, 0 },
    { "ship_sidesC", "", 64, 64, 768, 0, 128, 128, 0, false, 12, 12, 104, 104, 0, 0, 0, 0, 0 },
    { "ship_sidesB", "", 64, 64, 896, 0, 128, 128, 0, false, 12, 12, 104, 104, 0, 0, 0, 0, 0 },
    { "ship_sidesA", "", 64, 64, 0, 128, 128, 128, 0, false, 22, 16, 84, 106, 0, 0, 0, 0, 0 },
    { "ship_L", "", 64, 64, 128, 128, 128, 128, 0, false, 16, 16, 96, 96, 0, 0, 0, 0, 0 },
    { "ship_K", "", 64, 64, 256, 128, 128, 128, 0, false, 32, 16, 64, 96, 0, 0, 0, 0, 0 },
    { "ship_J", "", 64, 64, 384, 128, 128, 128, 0, false, 16, 16, 96, 96, 0, 0, 0, 0, 0 },
    { "ship_I", "", 64, 64, 512, 128, 128, 128, 0, false, 32, 16, 64, 96, 0, 0, 0, 0, 0 },
    { "ship_H", "", 64, 64, 640, 128, 128, 128, 0, false, 16, 16, 96, 96, 0, 0, 0, 0, 0 },
    { "ship_G", "", 64, 64, 768, 128, 128, 128, 0, false, 16, 16, 96, 96, 0, 0, 0, 0, 0 },
    { "ship_F", "", 64, 64, 896, 128, 128, 128, 0, false, 16, 16, 96, 96, 0, 0, 0, 0, 0 },
    { "ship_E", "", 64, 64, 0, 256, 128, 128, 0, false, 16, 16, 96, 96, 0, 0, 0, 0, 0 },
    { "ship_D", "", 64, 64, 128, 256, 128, 128, 0, false, 16, 32, 96, 64, 0, 0, 0, 0, 0 },
    { "ship_C", "", 64, 64, 256, 256, 128, 128, 0, false, 16, 32, 96, 64, 0, 0, 0, 0, 0 },
    { "ship_B", "", 64, 64, 384, 256, 128, 128, 0, false, 48, 32, 32, 64, 0, 0, 0, 0, 0 },
    { "ship_A", "", 64, 64, 512, 256, 128, 128, 0, false, 32, 40, 64, 48, 0, 0, 0, 0, 0 },
    { "satellite_D", "", 64, 64, 640, 256, 128, 128, 0, false, 12, 28, 104, 72, 0, 0, 0, 0, 0 },
    { "satellite_C", "", 64, 64, 768, 256, 128, 128, 0, false, 12, 28, 104, 72, 0, 0, 0, 0, 0 },
    { "satellite_B", "", 64, 64, 896, 256, 128, 128, 0, false, 12, 12, 104, 104, 0, 0, 0, 0, 0 },
    { "satellite_A", "", 64, 64, 0, 384, 128, 128, 0, false, 12, 28, 104, 88, 0, 0, 0, 0, 0 },
    { "meteor_squareSmall", "", 64, 64, 128, 384, 128, 128, 0, false, 32, 32, 64, 64, 0, 0, 0, 0, 0 },
    { "meteor_squareLarge", "", 64, 64, 256, 384, 128, 128, 0, false, 16, 16, 96, 96, 0, 0, 0, 0, 0 },
    { "meteor_squareDetailedSmall", "", 64, 64, 384, 384, 128, 128, 0, false, 32, 32, 64, 64, 0, 0, 0, 0, 0 },
    { "meteor_squareDetailedLarge", "", 64, 64, 512, 384, 128, 128, 0, false, 16, 16, 96, 96, 0, 0, 0, 0, 0 },
    { "meteor_small", "", 64, 64, 640, 384, 128, 128, 0, false, 32, 32, 64, 64, 0, 0, 0, 0, 0 },
    { "meteor_large", "", 64, 64, 768, 384, 128, 128, 0, false, 16, 16, 96, 96, 0, 0, 0, 0, 0 },
    { "meteor_detailedSmall", "", 64, 64, 896, 384, 128, 128, 0, false, 32, 32, 64, 64, 0, 0, 0, 0, 0 },
    { "meteor_detailedLarge", "", 64, 64, 0, 512, 128, 128, 0, false, 16, 16, 96, 96, 0, 0, 0, 0, 0 },
    { "icon_crossSmall", "", 64, 64, 128, 512, 128, 128, 0, false, 32, 32, 64, 64, 0, 0, 0, 0, 0 },
    { "icon_crossLarge", "", 64, 64, 256, 512, 128, 128, 0, false, 16, 16, 96, 96, 0, 0, 0, 0, 0 },
    { "enemy_E", "", 64, 64, 384, 512, 128, 128, 0, false, 16, 16, 96, 96, 0, 0, 0, 0, 0 },
    { "enemy_D", "", 64, 64, 512, 512, 128, 128, 0, false, 16, 16, 96, 96, 0, 0, 0, 0, 0 },
    { "enemy_C", "", 64, 64, 640, 512, 128, 128, 0, false, 0, 32, 128, 64, 0, 0, 0, 0, 0 },
    { "enemy_B", "", 64, 64, 768, 512, 128, 128, 0, false, 16, 16, 96, 96, 0, 0, 0, 0, 0 },
    { "enemy_A", "", 64, 64, 896, 512, 128, 128, 0, false, 16, 16, 96, 96, 0, 0, 0, 0, 0 },
    { "effect_purple", "", 0, 0, 0, 640, 128, 128, 0, false, 32, 0, 64, 127, 0, 0, 0, 0, 0 },
    { "effect_yellow", "", 0, 0, 128, 640, 128, 128, 0, false, 32, 0, 64, 127, 0, 0, 0, 0, 0 },
};
