#include "raygui.h"

#define UI_SCALE 1.0F
#define DEFAULT_BUTTON_SIZE (Vector2){120*UI_SCALE, 32*UI_SCALE}

typedef enum{
  MENU_INACTIVE,
  MENU_READY,
  MENU_CLOSED,
  MENU_ACTIVE,
  MENU_OPENED,
  MENU_FOCUSED,
  MENU_CLOSE,
  MENU_END
}MenuState;

typedef enum{
  ELEMENT_NONE,
  ELEMENT_IDLE,
  ELEMENT_FOCUSED,
  ELEMENT_ACTIVATED,
  ELEMENT_TOGGLED,
  ELEMENT_DONE
}ElementState;

typedef enum{
  UI_MASK,
  UI_BUTTON,
  UI_BLANK
}ElementType;

typedef enum{
  MENU_NONE,
  MENU_MAIN,
  MENU_OPTIONS,
  MENU_PAUSE,
  MENU_DEBUG,
  MENU_DONE
}MenuId;

struct ui_element_s;
typedef bool (*ElementCallback)(struct ui_element_s* self);

typedef struct ui_element_s{
  ElementType         type;
  ElementState        state;
  ElementCallback     cb[ELEMENT_DONE];
  int                 num_children;
  struct ui_element_s *children[4];
}ui_element_t;

ui_element_t* InitElement(ElementType type, Vector2 pos, Vector2 size);
void DrawElement(ui_element_t* element);
void UISyncElement(ui_element_t* e);

struct ui_menu_s;
typedef bool (*MenuCallback)(struct ui_menu_s* self);

typedef struct ui_menu_s{
  int           num_children;
  MenuCallback  cb[MENU_END];
  ui_element_t  *children[8];
  MenuState     state;
  bool          is_modal;
}ui_menu_t;

ui_menu_t InitMenu(MenuId id, bool mask);
void UISyncMenu(ui_menu_t* m);
bool UICloseMenu(ui_menu_t* m);
void DrawMenu(ui_menu_t* m);
bool MenuCanChangeState(MenuState old, MenuState s);
bool MenuSetState(ui_menu_t* m, MenuState s);
void MenuOnStateChanged(ui_menu_t* m, MenuState old, MenuState s);
static bool MenuInert(ui_menu_t* self){
  return false;
}
void MenuAddChild(ui_menu_t *m, ui_element_t* c);

typedef struct{
  //MenuId      open_menu;
  KeyboardKey menu_key[MENU_DONE];
  ui_menu_t   menus[MENU_DONE];
}ui_manager_t;

extern ui_manager_t ui;

void InitUI();

void UISync();
void UIRender();

bool TogglePause(ui_menu_t* m);
