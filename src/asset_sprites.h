#ifndef TEXTURE_ATLAS_H
#define TEXTURE_ATLAS_H
#include <stdbool.h>

// ============================================================================
// Common Struct
// ============================================================================
typedef struct {
    int identifier; // enum reference
    const char *file;

    // Texture origin and position
    int originX, originY;
    int positionX, positionY;

    // Source dimensions
    int sourceWidth, sourceHeight;

    // Padding / trimming
    int padding;
    bool trimmed;
    int trimRecX, trimRecY, trimRecWidth, trimRecHeight;

    // Collider information
    int colliderType;
    int colliderPosX, colliderPosY;
    int colliderSizeX, colliderSizeY;
} SubTexture;

static const char *ATLAS_IMAGE_PATH = "sprites.png";

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
    BACKTILE_18
} BackTileID;

static const SubTexture BACK_TILES[] = {
    {BACKTILE_01, "BackTile_01.png", 0,0, 814,278, 256,256, 0,false,0,0,0,0, 0,0,0,0,0},
    {BACKTILE_02, "BackTile_02.png", 0,0, 0,278, 276,276, 0,false,0,0,0,0, 0,0,0,0,0},
    {BACKTILE_03, "BackTile_03.png", 0,0, 556,794, 256,256, 0,false,0,0,0,0, 0,0,0,0,0},
    {BACKTILE_04, "BackTile_04.png", 0,0, 0,556, 276,276, 0,false,0,0,0,0, 0,0,0,0,0},
    {BACKTILE_05, "BackTile_05.png", 0,0, 814,1052, 256,256, 0,false,0,0,0,0, 0,0,0,0,0},
    {BACKTILE_06, "BackTile_06.png", 0,0, 278,556, 276,276, 0,false,0,0,0,0, 0,0,0,0,0},
    {BACKTILE_07, "BackTile_07.png", 0,0, 814,536, 256,256, 0,false,0,0,0,0, 0,0,0,0,0},
    {BACKTILE_08, "BackTile_08.png", 0,0, 278,834, 276,276, 0,false,0,0,0,0, 0,0,0,0,0},
    {BACKTILE_09, "BackTile_09.png", 0,0, 556,1052, 256,256, 0,false,0,0,0,0, 0,0,0,0,0},
    {BACKTILE_10, "BackTile_10.png", 0,0, 556,0, 276,276, 0,false,0,0,0,0, 0,0,0,0,0},
    {BACKTILE_11, "BackTile_11.png", 0,0, 814,794, 256,256, 0,false,0,0,0,0, 0,0,0,0,0},
    {BACKTILE_12, "BackTile_12.png", 0,0, 278,0, 276,276, 0,false,0,0,0,0, 0,0,0,0,0},
    {BACKTILE_13, "BackTile_13.png", 0,0, 834,0, 256,256, 0,false,0,0,0,0, 0,0,0,0,0},
    {BACKTILE_14, "BackTile_14.png", 0,0, 0,0, 276,276, 0,false,0,0,0,0, 0,0,0,0,0},
    {BACKTILE_15, "BackTile_15.png", 0,0, 556,278, 256,256, 0,false,0,0,0,0, 0,0,0,0,0},
    {BACKTILE_16, "BackTile_16.png", 0,0, 278,278, 276,276, 0,false,0,0,0,0, 0,0,0,0,0},
    {BACKTILE_17, "BackTile_17.png", 0,0, 556,536, 256,256, 0,false,0,0,0,0, 0,0,0,0,0},
    {BACKTILE_18, "BackTile_18.png", 0,0, 0,834, 276,276, 0,false,0,0,0,0, 0,0,0,0,0}
};

// ============================================================================
// Tile Pink
// ============================================================================
typedef enum {
    TILEPINK_01,
    TILEPINK_02,
    TILEPINK_03,
    TILEPINK_04,
    TILEPINK_05,
    TILEPINK_06,
    TILEPINK_07,
    TILEPINK_08,
    TILEPINK_09,
    TILEPINK_10,
    TILEPINK_11,
    TILEPINK_12,
    TILEPINK_13,
    TILEPINK_14,
    TILEPINK_15,
    TILEPINK_16,
    TILEPINK_17,
    TILEPINK_18,
    TILEPINK_19,
    TILEPINK_20,
    TILEPINK_21,
    TILEPINK_22,
    TILEPINK_23,
    TILEPINK_24,
    TILEPINK_25,
    TILEPINK_26,
    TILEPINK_27,
    TILEPINK_28,
    TILEPINK_29,
    TILEPINK_30,
    TILEPINK_31,
    TILEPINK_32,
    TILEPINK_33,
    TILEPINK_34,
    TILEPINK_35,
    TILEPINK_36,
    TILEPINK_37,
    TILEPINK_38,
    TILEPINK_39,
    TILEPINK_40,
    TILEPINK_41,
    TILEPINK_42,
    TILEPINK_43,
    TILEPINK_44,
    TILEPINK_45,
    TILEPINK_46,
    TILEPINK_47,
    TILEPINK_48,
    TILEPINK_49,
    TILEPINK_50,
    TILEPINK_51,
    TILEPINK_52,
    TILEPINK_53,
    TILEPINK_54,
    TILEPINK_55,
    TILEPINK_56,
    TILEPINK_57,
    TILEPINK_58,
    TILEPINK_59,
    TILEPINK_60,
    TILEPINK_61,
    TILEPINK_62,
    TILEPINK_63,
    TILEPINK_64,
    TILEPINK_65,
    TILEPINK_66,
    TILEPINK_67,
    TILEPINK_68,
    TILEPINK_69,
    TILEPINK_70,
    TILEPINK_71,
    TILEPINK_72
} TilePinkID;

static const SubTexture TILE_PINKS[] = {
    {TILEPINK_01,"tilePink_01.png",0,0,905,1134,128,128,0,false,0,0,0,0,0,0,0,0,0},
    {TILEPINK_02,"tilePink_02.png",0,0,0,530,188,88,0,false,0,0,0,0,0,0,0,0,0},
    {TILEPINK_03,"tilePink_03.png",0,0,0,1180,88,88,0,false,0,0,0,0,0,0,0,0,0},
    {TILEPINK_04,"tilePink_04.png",0,0,1035,1134,128,128,0,false,0,0,0,0,0,0,0,0,0},
    {TILEPINK_05,"tilePink_05.png",0,0,1036,390,128,128,0,false,0,0,0,0,0,0,0,0,0},
    {TILEPINK_06,"tilePink_06.png",0,0,1035,1004,128,128,0,false,0,0,0,0,0,0,0,0,0},
    {TILEPINK_07,"tilePink_07.png",0,0,1035,874,128,128,0,false,0,0,0,0,0,0,0,0,0},
    {TILEPINK_08,"tilePink_08.png",0,0,1036,260,128,128,0,false,0,0,0,0,0,0,0,0,0},
    {TILEPINK_09,"tilePink_09.png",0,0,812,240,129,120,0,false,0,0,0,0,0,0,0,0,0},
    {TILEPINK_10,"tilePink_10.png",0,0,303,1166,129,104,0,false,0,0,0,0,0,0,0,0,0},
    {TILEPINK_11,"tilePink_11.png",0,0,905,614,129,128,0,false,0,0,0,0,0,0,0,0,0},
    {TILEPINK_12,"tilePink_12.png",0,0,812,0,130,118,0,false,0,0,0,0,0,0,0,0,0},
    {TILEPINK_13,"tilePink_13.png",0,0,944,130,128,128,0,false,0,0,0,0,0,0,0,0,0},
    {TILEPINK_14,"tilePink_14.png",0,0,0,440,188,88,0,false,0,0,0,0,0,0,0,0,0},
    {TILEPINK_15,"tilePink_15.png",0,0,943,260,88,88,0,false,0,0,0,0,0,0,0,0,0},
    {TILEPINK_16,"tilePink_16.png",0,0,1035,744,128,128,0,false,0,0,0,0,0,0,0,0,0},
    {TILEPINK_17,"tilePink_17.png",0,0,944,0,128,128,0,false,0,0,0,0,0,0,0,0,0},
    {TILEPINK_18,"tilePink_18.png",0,0,905,1004,128,128,0,false,0,0,0,0,0,0,0,0,0},
    {TILEPINK_19,"tilePink_19.png",0,0,905,744,128,128,0,false,0,0,0,0,0,0,0,0,0},
    {TILEPINK_20,"tilePink_20.png",0,0,905,874,128,128,0,false,0,0,0,0,0,0,0,0,0},
    {TILEPINK_21,"tilePink_21.png",0,0,905,492,129,120,0,false,0,0,0,0,0,0,0,0,0},
    {TILEPINK_22,"tilePink_22.png",0,0,434,1166,129,104,0,false,0,0,0,0,0,0,0,0,0},
    {TILEPINK_23,"tilePink_23.png",0,0,905,362,129,128,0,false,0,0,0,0,0,0,0,0,0},
    {TILEPINK_24,"tilePink_24.png",0,0,812,120,130,118,0,false,0,0,0,0,0,0,0,0,0},
    {TILEPINK_25,"tilePink_25.png",0,0,755,1050,148,148,0,false,0,0,0,0,0,0,0,0,0},
    {TILEPINK_26,"tilePink_26.png",0,0,0,220,208,108,0,false,0,0,0,0,0,0,0,0,0},
    {TILEPINK_27,"tilePink_27.png",0,0,1036,520,108,108,0,false,0,0,0,0,0,0,0,0,0},
    {TILEPINK_28,"tilePink_28.png",0,0,755,900,148,148,0,false,0,0,0,0,0,0,0,0,0},
    {TILEPINK_29,"tilePink_29.png",0,0,755,750,148,148,0,false,0,0,0,0,0,0,0,0,0},
    {TILEPINK_30,"tilePink_30.png",0,0,454,1010,149,148,0,false,0,0,0,0,0,0,0,0,0},
    {TILEPINK_31,"tilePink_31.png",0,0,755,450,148,148,0,false,0,0,0,0,0,0,0,0,0},
    {TILEPINK_32,"tilePink_32.png",0,0,303,890,149,148,0,false,0,0,0,0,0,0,0,0,0},
    {TILEPINK_33,"tilePink_33.png",0,0,341,418,149,140,0,false,0,0,0,0,0,0,0,0,0},
    {TILEPINK_34,"tilePink_34.png",0,0,210,292,149,124,0,false,0,0,0,0,0,0,0,0,0},
    {TILEPINK_35,"tilePink_35.png",0,0,454,710,149,148,0,false,0,0,0,0,0,0,0,0,0},
    {TILEPINK_36,"tilePink_36.png",0,0,0,620,150,138,0,false,0,0,0,0,0,0,0,0,0},
    {TILEPINK_37,"tilePink_37.png",0,0,755,600,148,148,0,false,0,0,0,0,0,0,0,0,0},
    {TILEPINK_38,"tilePink_38.png",0,0,0,330,208,108,0,false,0,0,0,0,0,0,0,0,0},
    {TILEPINK_39,"tilePink_39.png",0,0,492,450,108,108,0,false,0,0,0,0,0,0,0,0,0},
    {TILEPINK_40,"tilePink_40.png",0,0,512,300,148,148,0,false,0,0,0,0,0,0,0,0,0},
    {TILEPINK_41,"tilePink_41.png",0,0,662,0,148,148,0,false,0,0,0,0,0,0,0,0,0},
    {TILEPINK_42,"tilePink_42.png",0,0,361,0,149,148,0,false,0,0,0,0,0,0,0,0,0},
    {TILEPINK_43,"tilePink_43.png",0,0,605,750,148,148,0,false,0,0,0,0,0,0,0,0,0},
    {TILEPINK_44,"tilePink_44.png",0,0,454,860,149,148,0,false,0,0,0,0,0,0,0,0,0},
    {TILEPINK_45,"tilePink_45.png",0,0,361,150,149,140,0,false,0,0,0,0,0,0,0,0,0},
    {TILEPINK_46,"tilePink_46.png",0,0,361,292,149,124,0,false,0,0,0,0,0,0,0,0,0},
    {TILEPINK_47,"tilePink_47.png",0,0,454,560,149,148,0,false,0,0,0,0,0,0,0,0,0},
    {TILEPINK_48,"tilePink_48.png",0,0,0,760,150,138,0,false,0,0,0,0,0,0,0,0,0},
    {TILEPINK_49,"tilePink_49.png",0,0,512,150,148,148,0,false,0,0,0,0,0,0,0,0,0},
    {TILEPINK_50,"tilePink_50.png",0,0,0,110,208,108,0,false,0,0,0,0,0,0,0,0,0},
    {TILEPINK_51,"tilePink_51.png",0,0,1036,630,108,108,0,false,0,0,0,0,0,0,0,0,0},
    {TILEPINK_52,"tilePink_52.png",0,0,662,300,148,148,0,false,0,0,0,0,0,0,0,0,0},
    {TILEPINK_53,"tilePink_53.png",0,0,662,150,148,148,0,false,0,0,0,0,0,0,0,0,0},
    {TILEPINK_54,"tilePink_54.png",0,0,303,740,149,148,0,false,0,0,0,0,0,0,0,0,0},
    {TILEPINK_55,"tilePink_55.png",0,0,605,1050,148,148,0,false,0,0,0,0,0,0,0,0,0},
    {TILEPINK_56,"tilePink_56.png",0,0,210,142,149,148,0,false,0,0,0,0,0,0,0,0,0},
    {TILEPINK_57,"tilePink_57.png",0,0,210,0,149,140,0,false,0,0,0,0,0,0,0,0,0},
    {TILEPINK_58,"tilePink_58.png",0,0,303,1040,149,124,0,false,0,0,0,0,0,0,0,0,0},
    {TILEPINK_59,"tilePink_59.png",0,0,152,1038,149,148,0,false,0,0,0,0,0,0,0,0,0},
    {TILEPINK_60,"tilePink_60.png",0,0,0,1040,150,138,0,false,0,0,0,0,0,0,0,0,0},
    {TILEPINK_61,"tilePink_61.png",0,0,512,0,148,148,0,false,0,0,0,0,0,0,0,0,0},
    {TILEPINK_62,"tilePink_62.png",0,0,0,0,208,108,0,false,0,0,0,0,0,0,0,0,0},
    {TILEPINK_63,"tilePink_63.png",0,0,1074,0,108,108,0,false,0,0,0,0,0,0,0,0,0},
    {TILEPINK_64,"tilePink_64.png",0,0,605,450,148,148,0,false,0,0,0,0,0,0,0,0,0},
    {TILEPINK_65,"tilePink_65.png",0,0,605,600,148,148,0,false,0,0,0,0,0,0,0,0,0},
    {TILEPINK_66,"tilePink_66.png",0,0,190,440,149,148,0,false,0,0,0,0,0,0,0,0,0},
    {TILEPINK_67,"tilePink_67.png",0,0,605,900,148,148,0,false,0,0,0,0,0,0,0,0,0},
    {TILEPINK_68,"tilePink_68.png",0,0,152,620,149,148,0,false,0,0,0,0,0,0,0,0,0},
    {TILEPINK_69,"tilePink_69.png",0,0,152,770,149,140,0,false,0,0,0,0,0,0,0,0,0},
    {TILEPINK_70,"tilePink_70.png",0,0,152,912,149,124,0,false,0,0,0,0,0,0,0,0,0},
    {TILEPINK_71,"tilePink_71.png",0,0,303,590,149,148,0,false,0,0,0,0,0,0,0,0,0},
    {TILEPINK_72,"tilePink_72.png",0,0,0,900,150,138,0,false,0,0,0,0,0,0,0,0,0}
};

#define ATLAS_SPRITE_COUNT 99
#endif // TEXTURE_ATLAS_H

