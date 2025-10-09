#include "raygui.h"
#include <stdint.h>

#define MAX_ELEMENTS 64
#define UI_SCALE 1.0F
#define DEFAULT_BUTTON_SIZE (Vector2){120*UI_SCALE, 32*UI_SCALE}
#define LARGE_BUTTON_SIZE (Vector2){164*UI_SCALE, 32*UI_SCALE}

#define DEFAULT_PANEL_SIZE (Vector2){GetScreenWidth(), 64*UI_SCALE}
#define DEFAULT_PANEL_THIN_SIZE (Vector2){224*UI_SCALE, 32*UI_SCALE}
#define LARGE_PANEL_THIN_SIZE (Vector2){GetScreenWidth(), 32*UI_SCALE}
#define SMALL_PANEL_SIZE (Vector2){192*UI_SCALE, 64*UI_SCALE}
#define SMALL_PANEL_THIN_SIZE (Vector2){184*UI_SCALE, 32*UI_SCALE}
#define XS_PANEL_SIZE (Vector2){128*UI_SCALE, 64*UI_SCALE}
#define XS_PANEL_THIN_SIZE (Vector2){128*UI_SCALE, 32*UI_SCALE}

#define DEFAULT_LINE_SIZE (Vector2){2 *UI_SCALE, 64*UI_SCALE}
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
  ELEMENT_HIDDEN,
  ELEMENT_IDLE,
  ELEMENT_FOCUSED,
  ELEMENT_ACTIVATED,
  ELEMENT_TOGGLED,
  ELEMENT_DONE
}ElementState;

typedef enum{
  UI_MASK,
  UI_BUTTON,
  UI_LABEL,
  UI_STATUSBAR,
  UI_PROGRESSBAR,
  UI_PANEL,
  UI_BOX,
  UI_LINE,
  UI_BLANK
}ElementType;

typedef enum{
  LAYOUT_FREE,
  LAYOUT_VERTICAL,
  LAYOUT_HORIZONTAL,
}UILayout;

typedef enum{
  ALIGN_NONE,
  ALIGN_CENTER,
  ALIGN_LEFT,
  ALIGN_RIGHT,
}UIAlignment;

typedef enum{
  MENU_NONE,
  MENU_MAIN,
  MENU_OPTIONS,
  MENU_PAUSE,
  MENU_RECAP,
  MENU_HUD,
  MENU_DEBUG,
  MENU_DONE
}MenuId;

struct ui_element_s;
typedef bool (*ElementCallback)(struct ui_element_s* self);
typedef enum { VAL_INT, VAL_FLOAT, VAL_CHAR } ValueType;
typedef struct {
    ValueType type;
    union {
        int   *i;
        float *f;
        const char*  c;
    };
} ElementValue;

typedef ElementValue (*ElementValueSync)(void);

typedef struct ui_element_s{
  uint32_t            hash;
  struct ui_menu_s    *owner;
  ElementType         type;
  ElementState        state;
  ElementCallback     cb[ELEMENT_DONE];
  Rectangle           bounds;
  char                text[65];
  ElementValueSync    get_val;
  int                 num_children;
  struct ui_element_s *children[8];
}ui_element_t;

ui_element_t* InitElement(const char* name,ElementType type, Vector2 pos, Vector2 size);
ui_element_t* GetElement(const char* name);
bool ElementSetState(ui_element_t* e, ElementState s);
void ElementAddChild(ui_element_t *o, ui_element_t* c);
void UISyncElement(ui_element_t* e);
bool UICloseOwner(ui_element_t* e);
struct ui_menu_s;
typedef bool (*MenuCallback)(struct ui_menu_s* self);

ElementValue GetDisplayPoints(void);
ElementValue GetDisplayTime(void);
ElementValue GetDisplayWave(void);
ElementValue GetDisplayHealth(void);

typedef struct ui_menu_s{
  UILayout      layout;
  UIAlignment   align;
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
  KeyboardKey  menu_key[MENU_DONE];
  ui_menu_t    menus[MENU_DONE];
  int          num_elements;
  ui_element_t *elements[MAX_ELEMENTS];
}ui_manager_t;

extern ui_manager_t ui;

void InitUI();

void UISync();
void UIRender();
bool TogglePause(ui_menu_t* m);
