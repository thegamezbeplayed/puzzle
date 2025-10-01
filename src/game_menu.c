#include "game_ui.h"
#include "game_utils.h"
#include "screens.h"
#include "game_tools.h"
#include "game_process.h"

ui_manager_t ui;

void InitUI(){
  for (int i = 0; i< MENU_DONE; i++)
    ui.menu_key[i] = KEY_NULL;

  ui.menu_key[MENU_PAUSE] = KEY_P;
  ui.menus[MENU_PAUSE] = InitMenu(MENU_PAUSE, true);
  ui.menus[MENU_PAUSE].cb[MENU_OPENED] = TogglePause; 
  ui.menus[MENU_PAUSE].cb[MENU_CLOSE] = TogglePause;

  ui.menus[MENU_MAIN] = InitMenu(MENU_MAIN,false);
  Vector2 pos = Vector2Subtract(VECTOR2_CENTER_SCREEN,Vector2Scale(DEFAULT_BUTTON_SIZE,0.5f));
 ui_element_t *playBtn = InitElement(UI_BUTTON,pos,DEFAULT_BUTTON_SIZE); 

 strcpy(playBtn->text, "PLAY");
 playBtn->cb[ELEMENT_ACTIVATED] = GameTransitionScreen;
 MenuAddChild(&ui.menus[MENU_MAIN],playBtn);

 ui.menus[MENU_RECAP] = InitMenu(MENU_RECAP,false);
 Vector2 cPos = Vector2Subtract(VECTOR2_CENTER_SCREEN,Vector2Scale(LARGE_BUTTON_SIZE,0.5f));
 ui_element_t *recapBtn = InitElement(UI_BUTTON,pos,LARGE_BUTTON_SIZE); 

 strcpy(recapBtn->text, "CONTINUE");
 recapBtn->cb[ELEMENT_ACTIVATED] = GameTransitionScreen;
 MenuAddChild(&ui.menus[MENU_RECAP],recapBtn);
}

ui_menu_t InitMenu(MenuId id, bool modal){
  ui_menu_t m = {0};

  m.is_modal = modal;

  for(int i = 0; i < MENU_END; i++)
    m.cb[i] = MenuInert;

  if(m.is_modal){
    m.cb[MENU_CLOSE] = UICloseMenu;
    m.children[m.num_children++] = InitElement(UI_MASK,VECTOR2_ZERO,VECTOR2_SCREEN);
  }
  return m;
}

ui_element_t* InitElement(ElementType type, Vector2 pos, Vector2 size){
  ui_element_t* u = malloc(sizeof(ui_element_t));
//  *u = (ui_element_t) {0};

  u->num_children = 0;
  u->type = type;
  u->bounds = Rect(pos.x,pos.y,size.x,size.y);
  for(int i = 0; i < ELEMENT_DONE; i++)
    u->cb[i] = BOOL_DO_NOTHING;
  return u;
}

void DrawMenu(ui_menu_t* m){
/*  for(int i = 0; i < m->num_children;i++){
    DrawElement(m->children[i]);
  }*/
}

void MenuAddChild(ui_menu_t *m, ui_element_t* c){
  m->children[m->num_children++] = c;
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
  for(int i = 0; i < m->num_children;i++)
    UISyncElement(m->children[i]);

  if(IsKeyPressed(KEY_ESCAPE))
    if(UICloseMenu(m)) return;
}

void UISyncElement(ui_element_t* e){
  int clicked,toggle,focused = 0;
  switch(e->type){
    case UI_BUTTON:
      clicked = GuiButton(e->bounds,e->text);
      break;
    default:
      break;
  }

  if(clicked>0)
    e->cb[ELEMENT_ACTIVATED](e);

}

void UIRender(){
  for (int i = 0; i < MENU_DONE; i++){
    if(ui.menus[i].state < MENU_ACTIVE)
      continue;

    DrawMenu(&ui.menus[i]);
  }
}

bool UICloseMenu(ui_menu_t* m){
  return MenuSetState(m,MENU_CLOSE);
}

void MenuOnStateChanged(ui_menu_t*m, MenuState old, MenuState s){
  m->cb[m->state](m);

  switch(s){
    case MENU_CLOSE:
      MenuSetState(m,MENU_CLOSED);
      break;
    default:
      break;
  }
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

  MenuState old = m->state;
  m->state = s;

  MenuOnStateChanged(m,old,s);

  return true;
}
