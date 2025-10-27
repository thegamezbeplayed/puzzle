#include "raylib.h"
#include "game_process.h"
#include "game_ui.h"

Camera2D camera = { 0 };
//----------------------------------------------------------------------------------
// Gameplay Screen Functions Definition
//----------------------------------------------------------------------------------
Vector2 CaptureInput(){
  Vector2 input = {0.0f,0.0f};

  if (IsKeyDown(KEY_D)) input.x += 1.0f;
  if (IsKeyDown(KEY_A)) input.x -= 1.0f;
  if (IsKeyDown(KEY_W)) input.y -= 1.0f;
  if (IsKeyDown(KEY_S)) input.y += 1.0f;

  return input;
}

// Gameplay Screen Initialization logic
void InitGameplayScreen(void){
  //camera.target = player.position;
  camera.offset = (Vector2){ GetScreenWidth()/2.0f, GetScreenHeight()/2.0f };
  camera.rotation = 0.0f;
  camera.zoom = 1.0f;

  camera.target = (Vector2){ROOM_WIDTH/2,ROOM_HEIGHT/2};
  InitGameEvents();
  MenuSetState(&ui.menus[MENU_HUD],MENU_ACTIVE);
  InitScreenInteractive();
}

void PreUpdate(void){
  GameProcessStep();
  WorldPreUpdate();
}

void FixedUpdate(void){
  WorldFixedUpdate();
}

void PostUpdate(void){
  WorldPostUpdate();
}

// Gameplay Screen Update logic
void UpdateGameplayScreen(void)
{
  ScreenSyncMouse();
}

// Gameplay Screen Draw logic
void DrawGameplayScreen(void)
{
  //  if(game_process.state == GAME_LOADING)
  //  return;
  BeginDrawing();
  ClearBackground(RAYWHITE);
  BeginMode2D(camera);

  WorldRender();

  EndMode2D();
  DrawFPS(10, 10);
  UISync();
  EndDrawing();
}

// Gameplay Screen Unload logic
void UnloadGameplayScreen(void)
{
  MenuSetState(&ui.menus[MENU_HUD],MENU_INACTIVE);

  GameProcessEnd();
  // TODO: Unload GAMEPLAY screen variables here!
}

// Gameplay Screen should finish?
int FinishGameplayScreen(void)
{
//  return finishScreen;
}
