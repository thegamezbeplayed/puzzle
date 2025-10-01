#include "screens.h"
#include "game_ui.h"
#define RAYGUI_IMPLEMENTATION

// Title Screen Initialization logic
void InitTitleScreen(void)
{
  MenuSetState(&ui.menus[MENU_MAIN],MENU_ACTIVE);
}

// Title Screen Update logic
void UpdateTitleScreen(void)
{
    // TODO: Update TITLE screen variables here!
}

// Title Screen Draw logic
void DrawTitleScreen(void)
{
  BeginDrawing();
  ClearBackground(RAYWHITE);

  DrawFPS(10, 10);

  EndDrawing();
}

// Title Screen Unload logic
void UnloadTitleScreen(void)
{
    // TODO: Unload TITLE screen variables here!
  MenuSetState(&ui.menus[MENU_MAIN],MENU_INACTIVE);
}
