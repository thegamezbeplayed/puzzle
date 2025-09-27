/*******************************************************************************************
*
*   HudBar v1.0.0 - Tool Description
*
*   MODULE USAGE:
*       #define GUI_HUD_BAR_IMPLEMENTATION
*       #include "gui_hud_bar.h"
*
*       INIT: GuiHudBarState state = InitGuiHudBar();
*       DRAW: GuiHudBar(&state);
*
*   LICENSE: Propietary License
*
*   Copyright (c) 2022 raylib technologies. All Rights Reserved.
*
*   Unauthorized copying of this file, via any medium is strictly prohibited
*   This project is proprietary and confidential unless the owner allows
*   usage in any other form by expresely written permission.
*
**********************************************************************************************/

#include "raylib.h"
#include "game_process.h"
// WARNING: raygui implementation is expected to be defined before including this header
#undef RAYGUI_IMPLEMENTATION
#include "raygui.h"

#include <string.h>     // Required for: strcpy()

#ifndef GUI_HUD_BAR_H
#define GUI_HUD_BAR_H

typedef struct {
    float ProgressBar002Value;

    Rectangle layoutRecs[3];

    // Custom state variables (depend on development software)
    // NOTE: This variables should be added manually if required

} GuiHudBarState;

#ifdef __cplusplus
extern "C" {            // Prevents name mangling of functions
#endif

//----------------------------------------------------------------------------------
// Defines and Macros
//----------------------------------------------------------------------------------
//...

//----------------------------------------------------------------------------------
// Types and Structures Definition
//----------------------------------------------------------------------------------
// ...

//----------------------------------------------------------------------------------
// Module Functions Declaration
//----------------------------------------------------------------------------------
GuiHudBarState InitGuiHudBar(void);
void GuiHudBar(GuiHudBarState *state);

#ifdef __cplusplus
}
#endif

#endif // GUI_HUD_BAR_H

/***********************************************************************************
*
*   GUI_HUD_BAR IMPLEMENTATION
*
************************************************************************************/
#if defined(GUI_HUD_BAR_IMPLEMENTATION)

#include "raygui.h"

//----------------------------------------------------------------------------------
// Global Variables Definition
//----------------------------------------------------------------------------------
//...

//----------------------------------------------------------------------------------
// Internal Module Functions Definition
//----------------------------------------------------------------------------------
//...

//----------------------------------------------------------------------------------
// Module Functions Definition
//----------------------------------------------------------------------------------
GuiHudBarState InitGuiHudBar(void)
{
    GuiHudBarState state = { 0 };

    state.ProgressBar002Value = 0.0f;

    state.layoutRecs[0] = (Rectangle){ 120, 16, 216, 48 };
    state.layoutRecs[1] = (Rectangle){ 528, 16, 168, 48 };
    state.layoutRecs[2] = (Rectangle){ 360, 16, 144, 16 };

    // Custom variables initialization

    return state;
}

void GuiHudBar(GuiHudBarState *state)
{
  state->ProgressBar002Value = RATIO(player->stats[STAT_HEALTH]);
  GuiStatusBar(state->layoutRecs[0], GetPoints());
  GuiStatusBar(state->layoutRecs[1], TextFormat("Wave %d",1+LevelGetCurrentWaveNum()));
  GuiProgressBar(state->layoutRecs[2], NULL, NULL, &state->ProgressBar002Value, 0, 1);
}

#endif // GUI_HUD_BAR_IMPLEMENTATION
