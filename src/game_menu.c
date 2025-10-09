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

  Vector2 default_menu_pos = (Vector2){
    .x = (GetScreenWidth()-DEFAULT_MENU_SIZE.x)/2,
    .y = (GetScreenHeight()-DEFAULT_MENU_SIZE.y)/2
  };

  ui.menu_key[MENU_PAUSE] = KEY_P;
  ui.menus[MENU_PAUSE] = InitMenu(MENU_PAUSE,default_menu_pos,DEFAULT_MENU_SIZE, true);
  ui.menus[MENU_PAUSE].layout = LAYOUT_VERTICAL;
  ui.menus[MENU_PAUSE].cb[MENU_OPENED] = TogglePause; 
  ui.menus[MENU_PAUSE].cb[MENU_CLOSE] = TogglePause;

  Vector2 pos = Vector2Subtract(VECTOR2_CENTER_SCREEN,Vector2Scale(DEFAULT_BUTTON_SIZE,0.5f));

  ui_element_t *resumeBtn = InitElement("RESUME_BTN",UI_BUTTON,pos,DEFAULT_BUTTON_SIZE); 
  strcpy(resumeBtn->text, "RESUME");
  resumeBtn->cb[ELEMENT_ACTIVATED] = UICloseOwner;
  MenuAddChild(&ui.menus[MENU_PAUSE],resumeBtn);
  ui.menus[MENU_MAIN] = InitMenu(MENU_MAIN,default_menu_pos,DEFAULT_MENU_SIZE,false);
  ui.menus[MENU_MAIN].layout = LAYOUT_VERTICAL;
  ui_element_t *playBtn = InitElement("PLAY_BTN",UI_BUTTON,pos,DEFAULT_BUTTON_SIZE); 

  strcpy(playBtn->text, "PLAY");
  playBtn->cb[ELEMENT_ACTIVATED] = GameTransitionScreen;
  MenuAddChild(&ui.menus[MENU_MAIN],playBtn);

  ui.menus[MENU_RECAP] = InitMenu(MENU_RECAP,default_menu_pos,DEFAULT_MENU_SIZE,false);

  ui.menus[MENU_RECAP].layout = LAYOUT_VERTICAL;
  ui.menus[MENU_RECAP].align = ALIGN_CENTER;

 Vector2 rPos = Vector2Y(128); 
  ui_element_t *pointsText =InitElement("POINTS_LBL",UI_STATUSBAR,rPos,XS_PANEL_THIN_SIZE);
  ui_element_t *pointsBox =InitElement("SCORE_LBL",UI_STATUSBAR,rPos,XS_PANEL_THIN_SIZE);
  ui_element_t *newHigh =InitElement("HIGHSCORE_LBL",UI_STATUSBAR,rPos,DEFAULT_PANEL_THIN_SIZE);

  strcpy(newHigh->text,"<<NEW HIGH SCORE>>");
  strcpy(pointsBox->text, "SCORE");
  pointsText->get_val = GetDisplayPoints;
  
  ui_element_t *scoreTable = InitElement("SCORE_TBL",UI_PANEL,rPos,DEFAULT_PANEL_THIN_SIZE);
  scoreTable->layout = LAYOUT_HORIZONTAL;
  
  ui_element_t *nameCol = InitElement("NAME_COL",UI_PANEL,rPos,DEFAULT_PANEL_THIN_SIZE);
 
 nameCol->layout = LAYOUT_VERTICAL; 
  ui_element_t *tableHeader = InitElement("HEADER_PANE",UI_PANEL,rPos,DEFAULT_PANEL_THIN_SIZE);

  ui_element_t *nameHeader = InitElement("NAME_HEADER",UI_LABEL,rPos,DEFAULT_BUTTON_SIZE);

  strcpy(nameHeader->text,"NAME");
  ElementAddChild(tableHeader,nameHeader);

  ui_element_t *recapBtn = InitElement("RECAP_BTN",UI_BUTTON,rPos,DEFAULT_BUTTON_SIZE); 


  ElementAddChild(scoreTable,tableHeader);
  ElementAddChild(scoreTable,nameCol);
  
  strcpy(recapBtn->text, "CONTINUE");
  recapBtn->cb[ELEMENT_ACTIVATED] = GameTransitionScreen;
  MenuAddChild(&ui.menus[MENU_RECAP],newHigh);
  MenuAddChild(&ui.menus[MENU_RECAP],pointsBox);
  MenuAddChild(&ui.menus[MENU_RECAP],pointsText);
  MenuAddChild(&ui.menus[MENU_RECAP],scoreTable);
  //MenuAddChild(&ui.menus[MENU_RECAP],recapBtn);

 ui.menus[MENU_HUD] = InitMenu(MENU_HUD,default_menu_pos,DEFAULT_MENU_THIN_SIZE,false);

 ui.menus[MENU_HUD].layout = LAYOUT_VERTICAL;
 ui.menus[MENU_HUD].align = ALIGN_CENTER;

 Vector2 pPos = Vector2X(172);
 ui_element_t *hudPane = InitElement("HUD_PANE",UI_PANEL,pPos,XS_PANEL_THIN_SIZE);
 ui_element_t *hudStatus = InitElement("STATUS_PANE",UI_PANEL,pPos, XS_PANEL_THIN_SIZE);

 ui_element_t *healthText = InitElement("HEALTH_STATUS",UI_STATUSBAR,VECTOR2_ZERO, SMALL_PANEL_THIN_SIZE);
 strcpy(healthText->text,"HEALTH");
 ui_element_t *healthBar = InitElement("HEALTH_BAR",UI_PROGRESSBAR,VECTOR2_ZERO, SMALL_PANEL_THIN_SIZE);
 healthBar->get_val = GetDisplayHealth;
 ElementAddChild(hudStatus,healthText);
 ElementAddChild(hudStatus,healthBar);
 ui_element_t *scoreBox = InitElement("SCORE_BAR",UI_STATUSBAR,VECTOR2_ZERO, XS_PANEL_THIN_SIZE);
 strcpy(scoreBox->text, "SCORE");

 ui_element_t *scoreText = InitElement("POINT_BAR",UI_STATUSBAR,VECTOR2_ZERO,XS_PANEL_THIN_SIZE);
 scoreText->get_val = GetDisplayPoints;
 ElementAddChild(hudPane,scoreBox);
 ElementAddChild(hudPane,scoreText);

 ui_element_t *waveBox = InitElement("WAVE_BAR",UI_STATUSBAR,VECTOR2_ZERO, XS_PANEL_THIN_SIZE);
 strcpy(waveBox->text, "WAVE");

 ui_element_t *waveText = InitElement("WAV_NUM_BAR",UI_STATUSBAR,VECTOR2_ZERO,XS_PANEL_THIN_SIZE);
 waveText->get_val = GetDisplayWave;
 ElementAddChild(hudPane,waveBox);
 ElementAddChild(hudPane,waveText);

 ui_element_t *timeBox = InitElement("TIME_BAR",UI_STATUSBAR,VECTOR2_ZERO, XS_PANEL_THIN_SIZE);
 strcpy(timeBox->text, "TIME");

 ui_element_t *timeText = InitElement("SECONDS_BAR",UI_STATUSBAR,VECTOR2_ZERO,XS_PANEL_THIN_SIZE);
 timeText->get_val = GetDisplayTime;
 ElementAddChild(hudPane,timeBox);
 ElementAddChild(hudPane,timeText);

 MenuAddChild(&ui.menus[MENU_HUD],hudPane);
 MenuAddChild(&ui.menus[MENU_HUD],hudStatus);
}

ui_menu_t InitMenu(MenuId id,Vector2 pos, Vector2 size, bool modal){
  ui_menu_t m = {0};

  m.is_modal = modal;
  m.bounds = Rect(pos.x,pos.y,size.x,size.y);

  for(int i = 0; i < MENU_END; i++)
    m.cb[i] = MenuInert;

  if(m.is_modal){
    m.cb[MENU_CLOSE] = UICloseMenu;
    m.children[m.num_children++] = InitElement("UI_MODAL_MASK",UI_MASK,VECTOR2_ZERO,VECTOR2_SCREEN);
  }
  return m;
}

ui_element_t* GetElement(const char* name){
  uint32_t hash = hash_str(name);

  for (int i = 0; i < MAX_ELEMENTS; i++){
    if(ui.elements[i]->hash != hash)
      continue;

    return ui.elements[i];
  }

  return NULL;
}

ui_element_t* InitElement(const char* name, ElementType type, Vector2 pos, Vector2 size){
  ui_element_t* u = malloc(sizeof(ui_element_t));
//  *u = (ui_element_t) {0};

  u->hash = hash_str(name);
  u->num_children = 0;
  u->type = type;
  u->state = ELEMENT_IDLE;
  u->get_val = NULL;//CHAR_DO_NOTHING;
  u->bounds = Rect(pos.x,pos.y,size.x,size.y);
  for(int i = 0; i < ELEMENT_DONE; i++)
    u->cb[i] = BOOL_DO_NOTHING;
  
  ui.elements[ui.num_elements++] = u;
  return u;
}

void MenuAddChild(ui_menu_t *m, ui_element_t* c){
  c->owner=m;
  Vector2 pos_adjustment = Vector2FromXY(c->bounds.x,c->bounds.y);
  if(m->num_children > 0){

    ui_element_t *prior = m->children[m->num_children-1];

    switch(m->layout){
      case LAYOUT_HORIZONTAL:
        break;
      case LAYOUT_VERTICAL:
        pos_adjustment.y = prior->bounds.y + prior->bounds.height;
        break;
      default:
        break;
    }
  }

  switch(m->align){
    case ALIGN_CENTER:
      float centerx = m->bounds.x + m->bounds.width/2;
      pos_adjustment.x = centerx - c->bounds.width/2;
      break;
    default:
      break;
  }

  ElementSetPosition(c,pos_adjustment);

  if(c->bounds.y + c->bounds.height > m->bounds.height)
    m->bounds.height = c->bounds.y + c->bounds.height;

  m->children[m->num_children++] = c;
}

void ElementAddChild(ui_element_t *o, ui_element_t* c){
  Vector2 pos_adjustment = Vector2FromXY(c->bounds.x,c->bounds.y);

  pos_adjustment.x+=o->bounds.x;
  pos_adjustment.y+=o->bounds.y;
  if(o->num_children > 0){
    ui_element_t *prior = o->children[o->num_children-1];
    switch(o->layout){
      case LAYOUT_HORIZONTAL:
        break;
      case LAYOUT_VERTICAL:
        pos_adjustment.y = prior->bounds.y + prior->bounds.height;
        break;
      default:
        break;
    }
  }
  else{
  /*
    ui_element_t *prior = o->children[o->num_children-1];
    c->bounds.x += prior->bounds.x + prior->bounds.width;
    c->bounds.y += prior->bounds.y;
  */
    }

  ElementSetPosition(c,pos_adjustment);

  if(c->bounds.x + c->bounds.width > o->bounds.width)
    o->bounds.width+=c->bounds.width;

  if(c->bounds.y + c->bounds.height > o->bounds.height)
    o->bounds.height+=c->bounds.height;


  c->owner = o->owner;
  o->children[o->num_children++] = c;
}

void ElementSetPosition(ui_element_t *e, Vector2 pos){
  Vector2 oldPos = Vector2FromXY(e->bounds.x,e->bounds.y);
  Vector2 dif = Vector2Subtract(pos,oldPos);

  e->bounds.x = pos.x;
  e->bounds.y = pos.y;

  for(int i = 0; i < e->num_children; i++)
    ElementAdjustPosition(e->children[i],dif);
}

void ElementAdjustPosition(ui_element_t *e, Vector2 inc){
  e->bounds.x+=inc.x;
  e->bounds.y+=inc.y;

  for(int i = 0; i < e->num_children; i++)
    ElementAdjustPosition(e->children[i],inc);
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
  if(e->state < ELEMENT_IDLE)
    return;

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

bool ElementSetState(ui_element_t* e, ElementState s){
  e->state = s;

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
