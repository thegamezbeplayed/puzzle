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
#define ATLAS_ASSET_SPRITES_SPRITE_COUNT    51

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
static rtpAtlasSprite rtpDescAssetSprites[51] = {
    { "star_large", "", 32, 32, 512, 640, 64, 64, 0, false, 8, 8, 48, 48, 0, 0, 0, 0, 0 },
    { "effect_purple", "", 32, 32, 512, 0, 64, 64, 0, false, 16, 0, 32, 64, 0, 0, 0, 0, 0 },
    { "effect_yellow", "", 32, 32, 576, 0, 64, 64, 0, false, 16, 0, 32, 64, 0, 0, 0, 0, 0 },
    { "enemy_A", "", 32, 32, 640, 0, 64, 64, 0, false, 8, 8, 48, 48, 0, 0, 0, 0, 0 },
    { "enemy_B", "", 32, 32, 704, 0, 64, 64, 0, false, 8, 8, 48, 48, 0, 0, 0, 0, 0 },
    { "enemy_C", "", 32, 32, 768, 0, 64, 64, 0, false, 0, 16, 64, 32, 0, 0, 0, 0, 0 },
    { "enemy_D", "", 32, 32, 832, 0, 64, 64, 0, false, 8, 8, 48, 48, 0, 0, 0, 0, 0 },
    { "enemy_E", "", 32, 32, 896, 0, 64, 64, 0, false, 8, 8, 48, 48, 0, 0, 0, 0, 0 },
    { "icon_crossLarge", "", 32, 32, 960, 0, 64, 64, 0, false, 8, 8, 48, 48, 0, 0, 0, 0, 0 },
    { "icon_crossSmall", "", 32, 32, 0, 512, 64, 64, 0, false, 16, 16, 32, 32, 0, 0, 0, 0, 0 },
    { "icon_exclamationLarge", "", 32, 32, 64, 512, 64, 64, 0, false, 8, 8, 48, 48, 0, 0, 0, 0, 0 },
    { "icon_exclamationSmall", "", 32, 32, 128, 512, 64, 64, 0, false, 16, 16, 32, 32, 0, 0, 0, 0, 0 },
    { "icon_plusLarge", "", 32, 32, 192, 512, 64, 64, 0, false, 8, 8, 48, 48, 0, 0, 0, 0, 0 },
    { "icon_plusSmall", "", 32, 32, 256, 512, 64, 64, 0, false, 16, 16, 32, 32, 0, 0, 0, 0, 0 },
    { "meteor_detailedLarge", "", 32, 32, 320, 512, 64, 64, 0, false, 8, 8, 48, 48, 0, 0, 0, 0, 0 },
    { "meteor_detailedSmall", "", 32, 32, 384, 512, 64, 64, 0, false, 16, 16, 32, 32, 0, 0, 0, 0, 0 },
    { "meteor_large", "", 32, 32, 448, 512, 64, 64, 0, false, 8, 8, 48, 48, 0, 0, 0, 0, 0 },
    { "meteor_small", "", 32, 32, 512, 512, 64, 64, 0, false, 16, 16, 32, 32, 0, 0, 0, 0, 0 },
    { "meteor_squareDetailedLarge", "", 32, 32, 576, 512, 64, 64, 0, false, 8, 8, 48, 48, 0, 0, 0, 0, 0 },
    { "meteor_squareDetailedSmall", "", 32, 32, 640, 512, 64, 64, 0, false, 16, 16, 32, 32, 0, 0, 0, 0, 0 },
    { "meteor_squareLarge", "", 32, 32, 704, 512, 64, 64, 0, false, 8, 8, 48, 48, 0, 0, 0, 0, 0 },
    { "meteor_squareSmall", "", 32, 32, 768, 512, 64, 64, 0, false, 16, 16, 32, 32, 0, 0, 0, 0, 0 },
    { "satellite_A", "", 32, 32, 832, 512, 64, 64, 0, false, 6, 14, 52, 44, 0, 0, 0, 0, 0 },
    { "satellite_B", "", 32, 32, 896, 512, 64, 64, 0, false, 6, 6, 52, 52, 0, 0, 0, 0, 0 },
    { "satellite_C", "", 32, 32, 960, 512, 64, 64, 0, false, 6, 14, 52, 36, 0, 0, 0, 0, 0 },
    { "satellite_D", "", 32, 32, 0, 576, 64, 64, 0, false, 6, 14, 52, 36, 0, 0, 0, 0, 0 },
    { "ship_A", "", 32, 32, 64, 576, 64, 64, 0, false, 16, 20, 32, 24, 0, 0, 0, 0, 0 },
    { "ship_B", "", 32, 32, 128, 576, 64, 64, 0, false, 24, 16, 16, 32, 0, 0, 0, 0, 0 },
    { "ship_C", "", 32, 32, 192, 576, 64, 64, 0, false, 8, 16, 48, 32, 0, 0, 0, 0, 0 },
    { "ship_D", "", 32, 32, 256, 576, 64, 64, 0, false, 8, 16, 48, 32, 0, 0, 0, 0, 0 },
    { "ship_E", "", 32, 32, 320, 576, 64, 64, 0, false, 8, 8, 48, 48, 0, 0, 0, 0, 0 },
    { "ship_F", "", 32, 32, 384, 576, 64, 64, 0, false, 8, 8, 48, 48, 0, 0, 0, 0, 0 },
    { "ship_G", "", 32, 32, 448, 576, 64, 64, 0, false, 8, 8, 48, 48, 0, 0, 0, 0, 0 },
    { "ship_H", "", 32, 32, 512, 576, 64, 64, 0, false, 8, 8, 48, 48, 0, 0, 0, 0, 0 },
    { "ship_I", "", 32, 32, 576, 576, 64, 64, 0, false, 16, 8, 32, 48, 0, 0, 0, 0, 0 },
    { "ship_J", "", 32, 32, 640, 576, 64, 64, 0, false, 8, 8, 48, 48, 0, 0, 0, 0, 0 },
    { "ship_K", "", 32, 32, 704, 576, 64, 64, 0, false, 16, 8, 32, 48, 0, 0, 0, 0, 0 },
    { "ship_L", "", 32, 32, 768, 576, 64, 64, 0, false, 8, 8, 48, 48, 0, 0, 0, 0, 0 },
    { "ship_sidesA", "", 32, 32, 832, 576, 64, 64, 0, false, 11, 8, 42, 53, 0, 0, 0, 0, 0 },
    { "ship_sidesB", "", 32, 32, 896, 576, 64, 64, 0, false, 6, 6, 52, 52, 0, 0, 0, 0, 0 },
    { "ship_sidesC", "", 32, 32, 960, 576, 64, 64, 0, false, 6, 6, 52, 52, 0, 0, 0, 0, 0 },
    { "ship_sidesD", "", 32, 32, 0, 640, 64, 64, 0, false, 8, 16, 48, 32, 0, 0, 0, 0, 0 },
    { "star_medium", "", 32, 32, 64, 640, 64, 64, 0, false, 8, 8, 48, 48, 0, 0, 0, 0, 0 },
    { "star_small", "", 32, 32, 128, 640, 64, 64, 0, false, 24, 24, 16, 16, 0, 0, 0, 0, 0 },
    { "star_tiny", "", 32, 32, 192, 640, 64, 64, 0, false, 24, 24, 16, 16, 0, 0, 0, 0, 0 },
    { "station_A", "", 32, 32, 256, 640, 64, 64, 0, false, 8, 8, 48, 48, 0, 0, 0, 0, 0 },
    { "station_B", "", 32, 32, 320, 640, 64, 64, 0, false, 4, 4, 56, 56, 0, 0, 0, 0, 0 },
    { "station_C", "", 0, 0, 384, 640, 64, 64, 0, false, 2, 2, 60, 60, 0, 0, 0, 0, 0 },
    { "Sprite-0001", "", 32, 32, 448, 640, 64, 64, 0, false, 0, 0, 64, 64, 0, 0, 0, 0, 0 },
    { "Sprite-0002", "", 32, 32, 576, 640, 64, 64, 0, false, 1, 1, 62, 62, 0, 0, 0, 0, 0 },
    { "slash_03", "", 256, 256, 0, 0, 512, 512, 0, false, 194, 37, 125, 438, 0, 0, 0, 0, 0 },
};
