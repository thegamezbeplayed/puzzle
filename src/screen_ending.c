#include "screens.h"
#include "game_ui.h"
#include "game_utils.h"
#include "game_tools.h"
#include "game_process.h"

//#define RAYGUI_IMPLEMENTATION

// Title Screen Initialization logic
void InitEndScreen(void)
{

  player_score_t pscore = {-1,"BenDover",GetPointsInt(),LevelGetCurrentWaveNum()};
  if(!DataUploadScore(&pscore)){
    ElementSetState(GetElement("HIGHSCORE_LBL"),ELEMENT_HIDDEN);
  }

  int count;
  player_score_t *scores = DataGetSortedRows(&count);
  ui_element_t* nameCol = GetElement("NAME_COL");
  ui_element_t* scoreCol = GetElement("SCORE_COL");
  ui_element_t* waveCol = GetElement("WAVE_COL");
  for(int i = 0; i < count; i++){
    ui_element_t *name = InitElement("PLAYER_NAME",UI_LABEL,VECTOR2_ZERO,DEFAULT_PANEL_THIN_SIZE,0,0);
    ui_element_t *score = InitElement("PLAYER_SCORE",UI_LABEL,VECTOR2_ZERO,DEFAULT_PANEL_THIN_SIZE,0,0);
    ui_element_t *wave = InitElement("PLAYER_WAVE",UI_LABEL,VECTOR2_ZERO,DEFAULT_PANEL_THIN_SIZE,0,0);
    score->spacing[UI_MARGIN_LEFT]=6;
    wave->spacing[UI_MARGIN_LEFT]=6;
    name->spacing[UI_MARGIN_LEFT]=6;
    strcpy(name->text,scores[i].name);
    strcpy(score->text,TextFormat("%09i",scores[i].score));
    strcpy(wave->text,TextFormat("%02i",scores[i].wave));
    ElementAddChild(nameCol,name);
    ElementAddChild(scoreCol,score);
    ElementAddChild(waveCol,wave);
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
