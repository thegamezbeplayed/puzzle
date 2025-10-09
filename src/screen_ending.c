#include "screens.h"
#include "game_ui.h"
#include "game_utils.h"
#include "game_tools.h"
#include "game_process.h"

//#define RAYGUI_IMPLEMENTATION

// Title Screen Initialization logic
void InitEndScreen(void)
{

  if(!DataUploadScore("mindGoblin",GetPointsInt())){
    ElementSetState(GetElement("HIGHSCORE_LBL"),ELEMENT_HIDDEN);
  }

  int count;
  player_score_t *scores = DataGetSortedRows(&count);
  ui_element_t* table = GetElement("NAME_COL");
  for(int i = 0; i < count; i++){
    ui_element_t *name = InitElement("SCORE_NAME",UI_LABEL,VECTOR2_ZERO,DEFAULT_PANEL_THIN_SIZE);
    strcpy(name->text,scores[i].name);
    ElementAddChild(table,name);
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
