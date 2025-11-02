#ifndef SCREENS_H
#define SCREENS_H
#include "game_types.h"

#if defined(PLATFORM_WEB)
static int screenWidth = 1538;
static int screenHeight = 956;
#else
static int screenWidth = 1920;
static int screenHeight = 1080;
#endif

typedef struct{
  bool    is_dragging;
  Vector2 offset,pos;
  ent_t   *target;
  ent_t   *hover;
}mouse_controller_t;
void InitScreenInteractive(void);
void ScreenSyncMouse(void);
ent_t* ScreenEntMouseCollision(void);

//----------------------------------------------------------------------------------
// Types and Structures Definition
//----------------------------------------------------------------------------------
typedef enum {
  SCREEN_LOGO,
  SCREEN_TITLE,
  SCREEN_OPTIONS,
  SCREEN_GAMEPLAY,
  SCREEN_ENDING,
  SCREEN_DONE
} GameScreen;

//----------------------------------------------------------------------------------
// Global Variables Declaration (shared by several modules)
//----------------------------------------------------------------------------------
extern GameScreen currentScreen;

#ifdef __cplusplus
extern "C" {            // Prevents name mangling of functions
#endif

//----------------------------------------------------------------------------------
// Logo Screen Functions Declaration
//----------------------------------------------------------------------------------
void InitLogoScreen(void);
void UpdateLogoScreen(void);
void DrawLogoScreen(void);
void UnloadLogoScreen(void);
int FinishLogoScreen(void);

//----------------------------------------------------------------------------------
// Title Screen Functions Declaration
//----------------------------------------------------------------------------------
void InitTitleScreen(void);
void UpdateTitleScreen(void);
void DrawTitleScreen(void);
void UnloadTitleScreen(void);
int FinishTitleScreen(void);

//----------------------------------------------------------------------------------
// Options Screen Functions Declaration
//----------------------------------------------------------------------------------
void InitOptionsScreen(void);
void UpdateOptionsScreen(void);
void DrawOptionsScreen(void);
void UnloadOptionsScreen(void);
int FinishOptionsScreen(void);

//----------------------------------------------------------------------------------
// Gameplay Screen Functions Declaration
//----------------------------------------------------------------------------------
void InitGameplayScreen(void);
void UpdateGameplayScreen(void);
void DrawGameplayScreen(void);
void UnloadGameplayScreen(void);
int FinishGameplayScreen(void);
void PreUpdate(void);
void FixedUpdate(void);
void PostUpdate(void);
//----------------------------------------------------------------------------------
// Ending Screen Functions Declaration
//----------------------------------------------------------------------------------
void InitEndScreen(void);
void UpdateEndScreen(void);
void DrawEndScreen(void);
void UnloadEndScreen(void);
int FinishEndScreen(void);

#ifdef __cplusplus
}
#endif

#endif // SCREENS_H
