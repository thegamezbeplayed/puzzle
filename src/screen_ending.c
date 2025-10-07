#include "screens.h"
#include "game_ui.h"
#include "game_utils.h"
#include "game_process.h"

//#define RAYGUI_IMPLEMENTATION

// Title Screen Initialization logic
void InitEndScreen(void)
{

  if(DataUploadScore("thegamezbeplayed",GetPointsInt())){
    //new high score
  }

  MenuSetState(&ui.menus[MENU_RECAP],MENU_ACTIVE);
}

// Title Screen Update logic
void UpdateEndScreen(void)
{
    // TODO: Update TITLE screen variables here!
}

// Title Screen Draw logic
void DrawEndScreen(void)
{
  BeginDrawing();
  ClearBackground(RAYWHITE);

  DrawFPS(10, 10);
  UISync();
  EndDrawing();
}

// Title Screen Unload logic
void UnloadEndScreen(void)
{
    // TODO: Unload TITLE screen variables here!
  MenuSetState(&ui.menus[MENU_RECAP],MENU_INACTIVE);
}
