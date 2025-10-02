#include "game_ui.h"
#include "game_utils.h"
#include "screens.h"
#include "game_tools.h"
#include "game_process.h"

ui_manager_t ui;

void InitUI(){
  //GuiSetStyle(DEFAULT,BASE_COLOR_NORMAL,ColorToInt(WHITE));
  for (int i = 0; i< MENU_DONE; i++)
    ui.menu_key[i] = KEY_NULL;

  ui.menu_key[MENU_PAUSE] = KEY_P;
  ui.menus[MENU_PAUSE] = InitMenu(MENU_PAUSE, true);
  ui.menus[MENU_PAUSE].cb[MENU_OPENED] = TogglePause; 
  ui.menus[MENU_PAUSE].cb[MENU_CLOSE] = TogglePause;

  Vector2 pos = Vector2Subtract(VECTOR2_CENTER_SCREEN,Vector2Scale(DEFAULT_BUTTON_SIZE,0.5f));

  ui_element_t *resumeBtn = InitElement(UI_BUTTON,pos,DEFAULT_BUTTON_SIZE); 
  strcpy(resumeBtn->text, "RESUME");
  resumeBtn->cb[ELEMENT_ACTIVATED] = UICloseOwner;
  MenuAddChild(&ui.menus[MENU_PAUSE],resumeBtn);
  ui.menus[MENU_MAIN] = InitMenu(MENU_MAIN,false);
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

 ui.menus[MENU_HUD] = InitMenu(MENU_HUD,false);
 Vector2 pPos = VECTOR2_ZERO;
 ui_element_t *hudPane = InitElement(UI_PANEL,pPos, DEFAULT_PANEL_SIZE);

 MenuAddChild(&ui.menus[MENU_HUD],hudPane);

 ui_element_t *scoreBox = InitElement(UI_PANEL,VECTOR2_ONE, SMALL_PANEL_SIZE);
 strcpy(scoreBox->text, "SCORE");

 ui_element_t *scoreText = InitElement(UI_LABEL,VECTOR2_ONE,SMALL_PANEL_SIZE);
 scoreText->get_val = GetPoints;
 ElementAddChild(hudPane,scoreBox);
 ElementAddChild(hudPane,scoreText);
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
  u->state = ELEMENT_IDLE;
  u->get_val = NULL;//CHAR_DO_NOTHING;
  u->bounds = Rect(pos.x,pos.y,size.x,size.y);
  for(int i = 0; i < ELEMENT_DONE; i++)
    u->cb[i] = BOOL_DO_NOTHING;
  return u;
}

void MenuAddChild(ui_menu_t *m, ui_element_t* c){
  m->children[m->num_children++] = c;
  c->owner=m;
}

void ElementAddChild(ui_element_t *o, ui_element_t* c){
  for(int i = 0; i < o->num_children; i++){
    c->bounds.x += o->children[i]->bounds.width;
  }
  if(o->num_children ==0){
    c->bounds.x += o->bounds.x;
    c->bounds.y += o->bounds.y;
  }
  c->owner = o->owner;
  o->children[o->num_children++] = c;
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
    m->cb[MENU_CLOSE](m);
}

void UISyncElement(ui_element_t* e){
  int clicked,toggle,focused = 0;
  if(e->get_val)
    strcpy(e->text, e->get_val());

  switch(e->type){
    case UI_BUTTON:
      clicked = GuiButton(e->bounds,e->text);
      break;
    case UI_PANEL:
      GuiPanel(e->bounds,e->text);
      break;
    case UI_LABEL:
      GuiLabel(e->bounds,e->text);
      break;
    default:
      break;
  }

  if(clicked>0)
    e->cb[ELEMENT_ACTIVATED](e);

  for(int i = 0; i<e->num_children; i++)
    UISyncElement(e->children[i]);
}

bool UICloseOwner(ui_element_t* e){
  UICloseMenu(e->owner);
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
