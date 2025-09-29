#include "game_ui.h"
#include "game_utils.h"
#include "screens.h"

ui_manager_t ui;

void InitUI(){
  for (int i = 0; i< MENU_DONE; i++)
    ui.menu_key[i] = KEY_NULL;

  ui.menu_key[MENU_PAUSE] = KEY_P;
  ui.menus[MENU_PAUSE] = InitMenu(MENU_PAUSE, true);
  ui.menus[MENU_PAUSE].cb[MENU_OPENED] = PauseGame;
}

ui_menu_t InitMenu(MenuId id, bool modal){
  ui_menu_t m = {0};

  m.is_modal = modal;

  for(int i = 0; i < MENU_END; i++)
    m.cb[i] = MenuInert;

  if(m.is_modal){
    m.cb[MENU_CLOSE] = UICloseMenu;
    m.children[m.num_children++] = InitElement(UI_MASK,screenWidth,screenHeight);
  }
  return m;
}

ui_element_t InitElement(ElementType type, int wid, int hei){
  ui_element_t u = {0};

  u.type = type;

  return u;
}

void DrawElement(ui_element_t* element){
}

void DrawMenu(ui_menu_t* m){
    DrawRectangle(0, 0, screenWidth, screenHeight, Fade(BLACK, 0.5f));
}


void UISync(){
  for(int i = 0; i < MENU_DONE; i++){
    if(IsKeyPressed(ui.menu_key[i])){
      MenuSetState(&ui.menus[i],MENU_OPENED);
      continue;
    }

    UISyncMenu(&ui.menus[i]);
  }
}

void UISyncMenu(ui_menu_t* m){
  if(m->state < MENU_ACTIVE)
    return;

  if(IsKeyPressed(KEY_ESCAPE))
    if(m->cb[MENU_CLOSE](m)) return;
}

void UIRender(){
  for (int i = 0; i < MENU_DONE; i++){
    if(ui.menus[i].state < MENU_ACTIVE)
      continue;

    DrawMenu(&ui.menus[i]);
  }
}

bool UICloseMenu(ui_menu_t* m){
  return MenuSetState(m,MENU_CLOSED);
}

bool MenuCanChangeState(MenuState old, MenuState s){
  if(old == s)
    return false;

  switch(COMBO_KEY(old,s)){
    case COMBO_KEY(MENU_INACTIVE,MENU_OPENED):
      return false;
      break;
    default:
      break;
  }
  return true;
}

bool MenuSetState(ui_menu_t* m, MenuState s){
  if(!MenuCanChangeState(m->state, s))
    return false;

  m->state = s;

  m->cb[m->state](m);
  return true;
}
