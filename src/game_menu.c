#include "game_ui.h"
#include "game_utils.h"
#include "screens.h"
#include "game_tools.h"
#include "game_process.h"

ui_manager_t ui;

void InitUI(){
  GuiSetStyle(DEFAULT,TEXT_SIZE,20);
  GuiSetStyle(DEFAULT,TEXT_ALIGNMENT,TEXT_ALIGN_CENTER);
  GuiSetStyle(DEFAULT,BORDER_COLOR_NORMAL,ColorToInt(WHITE));
  //GuiSetStyle(STATUSBAR,BASE_COLOR_NORMAL,ColorToInt(Fade(WHITE,0.5f)));
  GuiSetStyle(DEFAULT,LINE_COLOR,ColorToInt(BLACK));
  for (int i = 0; i< MENU_DONE; i++)
    ui.menu_key[i] = KEY_NULL;

  ui.menu_key[MENU_PAUSE] = KEY_P;
  ui.menus[MENU_PAUSE] = InitMenu(MENU_PAUSE, true);
  ui.menus[MENU_PAUSE].layout = LAYOUT_VERTICAL;
  ui.menus[MENU_PAUSE].cb[MENU_OPENED] = TogglePause; 
  ui.menus[MENU_PAUSE].cb[MENU_CLOSE] = TogglePause;

  Vector2 pos = Vector2Subtract(VECTOR2_CENTER_SCREEN,Vector2Scale(DEFAULT_BUTTON_SIZE,0.5f));

  ui_element_t *resumeBtn = InitElement(UI_BUTTON,pos,DEFAULT_BUTTON_SIZE); 
  strcpy(resumeBtn->text, "RESUME");
  resumeBtn->cb[ELEMENT_ACTIVATED] = UICloseOwner;
  MenuAddChild(&ui.menus[MENU_PAUSE],resumeBtn);
  ui.menus[MENU_MAIN] = InitMenu(MENU_MAIN,false);
  ui.menus[MENU_MAIN].layout = LAYOUT_VERTICAL;
  ui_element_t *playBtn = InitElement(UI_BUTTON,pos,DEFAULT_BUTTON_SIZE); 

  strcpy(playBtn->text, "PLAY");
  playBtn->cb[ELEMENT_ACTIVATED] = GameTransitionScreen;
  MenuAddChild(&ui.menus[MENU_MAIN],playBtn);

  ui.menus[MENU_RECAP] = InitMenu(MENU_RECAP,false);
  ui.menus[MENU_RECAP].layout = LAYOUT_VERTICAL;
 Vector2 cPos = Vector2Subtract(VECTOR2_CENTER_SCREEN,Vector2Scale(LARGE_BUTTON_SIZE,0.5f));
 ui_element_t *pointsText =InitElement(UI_STATUSBAR,pos,XS_PANEL_THIN_SIZE);
 ui_element_t *pointsBox =InitElement(UI_STATUSBAR,pos,XS_PANEL_THIN_SIZE);
 ui_element_t *newHigh =InitElement(UI_STATUSBAR,pos,DEFAULT_PANEL_THIN_SIZE);

 strcpy(newHigh->text,"<<NEW HIGH SCORE>>");
 strcpy(pointsBox->text, "SCORE");
 pointsText->get_val = GetDisplayPoints;
 ui_element_t *recapBtn = InitElement(UI_BUTTON,pos,LARGE_BUTTON_SIZE); 


 strcpy(recapBtn->text, "CONTINUE");
 recapBtn->cb[ELEMENT_ACTIVATED] = GameTransitionScreen;
 MenuAddChild(&ui.menus[MENU_RECAP],newHigh);
 MenuAddChild(&ui.menus[MENU_RECAP],pointsBox);
 MenuAddChild(&ui.menus[MENU_RECAP],pointsText);
 MenuAddChild(&ui.menus[MENU_RECAP],recapBtn);

 ui.menus[MENU_HUD] = InitMenu(MENU_HUD,false);
 ui.menus[MENU_HUD].layout = LAYOUT_VERTICAL;
 ui.menus[MENU_HUD].align = ALIGN_CENTER;
 Vector2 pPos = Vector2X(172);
 ui_element_t *hudPane = InitElement(UI_PANEL,pPos,XS_PANEL_THIN_SIZE);
 ui_element_t *hudStatus = InitElement(UI_PANEL,pPos, XS_PANEL_THIN_SIZE);

 MenuAddChild(&ui.menus[MENU_HUD],hudPane);
 MenuAddChild(&ui.menus[MENU_HUD],hudStatus);

 ui_element_t *healthText = InitElement(UI_STATUSBAR,VECTOR2_ZERO, SMALL_PANEL_THIN_SIZE);
 strcpy(healthText->text,"HEALTH");
 ui_element_t *healthBar = InitElement(UI_PROGRESSBAR,VECTOR2_ZERO, SMALL_PANEL_THIN_SIZE);
 healthBar->get_val = GetDisplayHealth;
 ElementAddChild(hudStatus,healthText);
 ElementAddChild(hudStatus,healthBar);
 ui_element_t *scoreBox = InitElement(UI_STATUSBAR,VECTOR2_ZERO, XS_PANEL_THIN_SIZE);
 strcpy(scoreBox->text, "SCORE");

 ui_element_t *scoreText = InitElement(UI_STATUSBAR,VECTOR2_ZERO,XS_PANEL_THIN_SIZE);
 scoreText->get_val = GetDisplayPoints;
 ElementAddChild(hudPane,scoreBox);
 ElementAddChild(hudPane,scoreText);

 ui_element_t *waveBox = InitElement(UI_STATUSBAR,VECTOR2_ZERO, XS_PANEL_THIN_SIZE);
 strcpy(waveBox->text, "WAVE");

 ui_element_t *waveText = InitElement(UI_STATUSBAR,VECTOR2_ZERO,XS_PANEL_THIN_SIZE);
 waveText->get_val = GetDisplayWave;
 ElementAddChild(hudPane,waveBox);
 ElementAddChild(hudPane,waveText);

 ui_element_t *timeBox = InitElement(UI_STATUSBAR,VECTOR2_ZERO, XS_PANEL_THIN_SIZE);
 strcpy(timeBox->text, "TIME");

 ui_element_t *timeText = InitElement(UI_STATUSBAR,VECTOR2_ZERO,XS_PANEL_THIN_SIZE);
 timeText->get_val = GetDisplayTime;
 ElementAddChild(hudPane,timeBox);
 ElementAddChild(hudPane,timeText);
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
  c->owner=m;
  if(m->num_children > 0){

    ui_element_t *prior = m->children[m->num_children-1];

    switch(m->layout){
      case LAYOUT_HORIZONTAL:
        break;
      case LAYOUT_VERTICAL:
        c->bounds.y = prior->bounds.y + prior->bounds.height;
        break;
      default:
        break;
    }
  }
  m->children[m->num_children++] = c;
}

void ElementAddChild(ui_element_t *o, ui_element_t* c){
  if(o->num_children == 0){
    c->bounds.x += o->bounds.x;
    c->bounds.y += o->bounds.y;
  }
  else{
    ui_element_t *prior = o->children[o->num_children-1];
    c->bounds.x += prior->bounds.x + prior->bounds.width;
    c->bounds.y += prior->bounds.y;
  }
  
  if(c->bounds.x + c->bounds.width > o->bounds.width)
    o->bounds.width+=c->bounds.width;

  c->owner = o->owner;
  o->children[o->num_children++] = c;
}

void UISync(){
  for(int i = 0; i < MENU_DONE; i++){
    if(IsKeyPressed(ui.menu_key[i]))
      MenuSetState(&ui.menus[i],MENU_OPENED);

    UISyncMenu(&ui.menus[i]);
  }
}

void UISyncMenu(ui_menu_t* m){
  if(m->state < MENU_ACTIVE)
    return;
  for(int i = 0; i < m->num_children;i++)
    UISyncElement(m->children[i]);

  if(IsKeyPressed(KEY_ESCAPE)){
    if(m->cb[MENU_CLOSE](m))
      MenuSetState(m,MENU_CLOSE);
  }
}

void UISyncElement(ui_element_t* e){
  int clicked,toggle,focused = 0;
  if(e->get_val){
    ElementValue ev = e->get_val();
    if(ev.type == VAL_CHAR)
      strcpy(e->text, ev.c);
  }
  switch(e->type){
    case UI_BUTTON:
      clicked = GuiButton(e->bounds,e->text);
      break;
    case UI_PANEL:
      GuiPanel(e->bounds,NULL);
      break;
    case UI_LABEL:
      GuiLabel(e->bounds,e->text);
      break;
    case UI_BOX:
      GuiGroupBox(e->bounds,NULL);
    case UI_LINE:
      GuiLine(e->bounds,NULL);
      break;
    case UI_PROGRESSBAR:
      GuiProgressBar(e->bounds, NULL,NULL, e->get_val().f,0,1);
      break;
    case UI_STATUSBAR:
      GuiStatusBar(e->bounds, e->text);
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

ElementValue GetDisplayPoints(void){
  ElementValue ev = {0};
  ev.type = VAL_CHAR;
  ev.c = GetPoints();
  return ev;
}

ElementValue GetDisplayTime(void){
  ElementValue ev = {0};
  ev.type = VAL_CHAR;
  ev.c = GetGameTime();
  return ev;
}

ElementValue GetDisplayWave(void){
  ElementValue ev = {0};
  ev.type = VAL_CHAR;
  ev.c = LevelGetCurrentWave();
  return ev;
}

ElementValue GetDisplayHealth(void){
  ElementValue ev = {0};
  ev.type = VAL_FLOAT;
  ev.f = malloc(sizeof(float));
  if(player->stats[STAT_HEALTH].ratio ==NULL)
    *ev.f = 0.0f;
  else
    *ev.f = RATIO(player->stats[STAT_HEALTH]);
  return ev;
}
