#include "game_ui.h"
#include "game_utils.h"
#include "screens.h"
#include "game_tools.h"
#include "game_process.h"

ui_manager_t ui;

void InitUI(){
  GuiSetStyle(DEFAULT,TEXT_SIZE,20);
  GuiSetStyle(DEFAULT,TEXT_ALIGNMENT,TEXT_ALIGN_CENTER);
  GuiSetStyle(LABEL,TEXT_ALIGNMENT,TEXT_ALIGN_LEFT);
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
  ui.menus[MENU_PAUSE] = InitMenu(MENU_PAUSE,VECTOR2_ZERO,VECTOR2_SCREEN,ALIGN_CENTER|ALIGN_MID,LAYOUT_VERTICAL,true);
  ui.menus[MENU_PAUSE].cb[MENU_OPENED] = TogglePause; 
  ui.menus[MENU_PAUSE].cb[MENU_CLOSE] = TogglePause;

  Vector2 pos = Vector2Subtract(VECTOR2_CENTER_SCREEN,Vector2Scale(DEFAULT_BUTTON_SIZE,0.5f));

  ui_element_t *resumeBtn = InitElement("RESUME_BTN",UI_BUTTON,pos,DEFAULT_BUTTON_SIZE,0,0); 
  strcpy(resumeBtn->text, "RESUME");
  resumeBtn->cb[ELEMENT_ACTIVATED] = UICloseOwner;
  ElementAddChild(ui.menus[MENU_PAUSE].element,resumeBtn);
  ui.menus[MENU_MAIN] = InitMenu(MENU_MAIN,VECTOR2_ZERO,VECTOR2_ZERO,ALIGN_CENTER|ALIGN_MID,LAYOUT_VERTICAL,false);

  ui_element_t *playBtn = InitElement("PLAY_BTN",UI_BUTTON,VECTOR2_ZERO,DEFAULT_BUTTON_SIZE,0,0); 

  strcpy(playBtn->text, "PLAY");
  playBtn->cb[ELEMENT_ACTIVATED] = GameTransitionScreen;
  ElementAddChild(ui.menus[MENU_MAIN].element,playBtn);

  ui.menus[MENU_RECAP] = InitMenu(MENU_RECAP,VECTOR2_ZERO,VECTOR2_ZERO,ALIGN_CENTER,LAYOUT_VERTICAL,false);

  ui_element_t *pointsText =InitElement("POINTS_LBL",UI_STATUSBAR,VECTOR2_ZERO,XS_PANEL_THIN_SIZE,0,0);
  ui_element_t *pointsBox =InitElement("SCORE_LBL",UI_STATUSBAR,VECTOR2_ZERO,XS_PANEL_THIN_SIZE,0,0);
  ui_element_t *newHigh =InitElement("HIGHSCORE_LBL",UI_STATUSBAR,VECTOR2_ZERO,DEFAULT_PANEL_THIN_SIZE,0,0);

  strcpy(newHigh->text,"<<NEW HIGH SCORE>>");
  strcpy(pointsBox->text, "SCORE");
  pointsText->get_val = GetDisplayPoints;
  
  ui_element_t *scoreTable = InitElement("SCORE_TBL",UI_PANEL,VECTOR2_ZERO,DEFAULT_PANEL_THIN_SIZE,ALIGN_CENTER|ALIGN_MID,LAYOUT_VERTICAL);
  
  ui_element_t *nameCol = InitElement("NAME_COL",UI_PANEL,VECTOR2_ZERO,VECTOR2_ZERO, ALIGN_LEFT,LAYOUT_VERTICAL); 
  ui_element_t *scoreCol = InitElement("SCORE_COL",UI_PANEL,VECTOR2_ZERO,VECTOR2_ZERO, ALIGN_LEFT,LAYOUT_VERTICAL); 
  ui_element_t *waveCol = InitElement("WAVE_COL",UI_PANEL,VECTOR2_ZERO,VECTOR2_ZERO, ALIGN_LEFT,LAYOUT_VERTICAL); 
  ui_element_t *tableHeader = InitElement("HEADER_PANE",UI_PANEL,VECTOR2_ZERO,VECTOR2_ZERO,0,LAYOUT_HORIZONTAL);
  ui_element_t *tableData = InitElement("DATA_PANE",UI_BOX,VECTOR2_ZERO,VECTOR2_ZERO,0,LAYOUT_HORIZONTAL);

  ui_element_t *nameHeader = InitElement("NAME_HEADER",UI_LABEL,VECTOR2_ZERO,DEFAULT_BUTTON_SIZE,ALIGN_LEFT,0);
  strcpy(nameHeader->text,"NAME");
  ElementAddChild(tableHeader,nameHeader);

ui_element_t *scoreHeader = InitElement("SCORE_HEADER",UI_LABEL,VECTOR2_ZERO,DEFAULT_BUTTON_SIZE,ALIGN_LEFT,0);
ui_element_t *waveHeader = InitElement("WAVE_HEADER",UI_LABEL,VECTOR2_ZERO,DEFAULT_BUTTON_SIZE,ALIGN_LEFT,0);
  strcpy(scoreHeader->text,"SCORE");
  strcpy(waveHeader->text,"WAVE");
  scoreHeader->spacing[UI_MARGIN_LEFT]=6;
  nameHeader->spacing[UI_MARGIN_LEFT]=6;

  ElementAddChild(tableHeader,scoreHeader);
  ElementAddChild(tableHeader,waveHeader);

  ui_element_t *recapBtn = InitElement("RECAP_BTN",UI_BUTTON,VECTOR2_ZERO,DEFAULT_BUTTON_SIZE,0,0); 

  ElementAddChild(scoreTable,tableHeader);
  ElementAddChild(tableData,nameCol);
  ElementAddChild(tableData,scoreCol);
  ElementAddChild(tableData,waveCol);
  ElementAddChild(scoreTable,tableData);
  
  strcpy(recapBtn->text, "CONTINUE");
  recapBtn->cb[ELEMENT_ACTIVATED] = GameTransitionScreen;
  ElementAddChild(ui.menus[MENU_RECAP].element,newHigh);
  ElementAddChild(ui.menus[MENU_RECAP].element,pointsBox);
  ElementAddChild(ui.menus[MENU_RECAP].element,pointsText);
  ElementAddChild(ui.menus[MENU_RECAP].element,scoreTable);
  ElementAddChild(ui.menus[MENU_RECAP].element,recapBtn);

 ui.menus[MENU_HUD] = InitMenu(MENU_HUD,VECTOR2_ZERO,VECTOR2_ZERO,ALIGN_CENTER,LAYOUT_VERTICAL,false);

 ui_element_t *hudPane = InitElement("HUD_PANE",UI_PANEL,VECTOR2_ZERO,VECTOR2_ZERO,ALIGN_CENTER,LAYOUT_HORIZONTAL);
 
 ui_element_t *hudStatus = InitElement("STATUS_PANE",UI_PANEL,VECTOR2_ZERO,VECTOR2_ZERO,ALIGN_CENTER,LAYOUT_HORIZONTAL);
 ui_element_t *healthText = InitElement("HEALTH_STATUS",UI_STATUSBAR,VECTOR2_ZERO, SMALL_PANEL_THIN_SIZE,0,0);
 strcpy(healthText->text,"HEALTH");
 ui_element_t *healthBar = InitElement("HEALTH_BAR",UI_PROGRESSBAR,VECTOR2_ZERO, SMALL_PANEL_THIN_SIZE,0,0);
 healthBar->get_val = GetDisplayHealth;
 ElementAddChild(hudStatus,healthText);
 ElementAddChild(hudStatus,healthBar);
 ui_element_t *scoreBox = InitElement("SCORE_BAR",UI_STATUSBAR,VECTOR2_ZERO, XS_PANEL_THIN_SIZE,0,0);
 strcpy(scoreBox->text, "SCORE");

 ui_element_t *scoreText = InitElement("POINT_BAR",UI_STATUSBAR,VECTOR2_ZERO,XS_PANEL_THIN_SIZE,0,0);
 scoreText->get_val = GetDisplayPoints;
 ElementAddChild(hudPane,scoreBox);
 ElementAddChild(hudPane,scoreText);

 ui_element_t *waveBox = InitElement("WAVE_BAR",UI_STATUSBAR,VECTOR2_ZERO, XS_PANEL_THIN_SIZE,0,0);
 strcpy(waveBox->text, "WAVE");

 ui_element_t *waveText = InitElement("WAV_NUM_BAR",UI_STATUSBAR,VECTOR2_ZERO,XS_PANEL_THIN_SIZE,0,0);
 waveText->get_val = GetDisplayWave;
 ElementAddChild(hudPane,waveBox);
 ElementAddChild(hudPane,waveText);

 ui_element_t *timeBox = InitElement("TIME_BAR",UI_STATUSBAR,VECTOR2_ZERO, XS_PANEL_THIN_SIZE,0,0);
 strcpy(timeBox->text, "TIME");

 ui_element_t *timeText = InitElement("SECONDS_BAR",UI_STATUSBAR,VECTOR2_ZERO,XS_PANEL_THIN_SIZE,0,0);
 timeText->get_val = GetDisplayTime;
 ElementAddChild(hudPane,timeBox);
 ElementAddChild(hudPane,timeText);

 ElementAddChild(ui.menus[MENU_HUD].element,hudPane);
 ElementAddChild(ui.menus[MENU_HUD].element,hudStatus);
}

ui_menu_t InitMenu(MenuId id,Vector2 pos, Vector2 size, UIAlignment align,UILayout layout, bool modal){
  ui_menu_t m = {0};

  m.is_modal = modal;
  ElementType menutype = UI_PANEL;
  if(m.is_modal){
    m.cb[MENU_CLOSE] = UICloseMenu;
    menutype = UI_MASK;
  }

  m.element = InitElement("UI_MENU_DOM",menutype,pos,size,align,layout);
  for(int i = 0; i < MENU_END; i++)
    m.cb[i] = MenuInert;

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

ui_element_t* InitElement(const char* name, ElementType type, Vector2 pos, Vector2 size, UIAlignment align,UILayout layout){
  ui_element_t* u = malloc(sizeof(ui_element_t));
  *u = (ui_element_t) {0};

  u->hash = hash_str(name);
  u->num_children = 0;
  u->type = type;
  u->state = ELEMENT_IDLE;
  u->get_val = NULL;//CHAR_DO_NOTHING;
  u->bounds = Rect(pos.x,pos.y,size.x,size.y);
  u->width = size.x;
  u->height = size.y;
  //strcpy(u->text,name);
  for(int i = 0; i < ELEMENT_DONE; i++)
    u->cb[i] = BOOL_DO_NOTHING;

  for (int i = 0; i < UI_POSITIONING; i++)
    u->spacing[i] = 0.0f;

  u->align = align;
  u->layout= layout;
  ui.elements[ui.num_elements++] = u;
  return u;
}

void ElementAddChild(ui_element_t *o, ui_element_t* c){
  Vector2 pos_adjustment = Vector2FromXY(c->bounds.x,c->bounds.y);
  c->index = o->num_children;
  c->menu = o->menu;
  c->owner = o;
  o->children[o->num_children++] = c;
}

void ElementResize(ui_element_t *e){
  e->bounds.x = e->bounds.y = 0;
  float centerx = VECTOR2_CENTER_SCREEN.x;
  float centery = VECTOR2_CENTER_SCREEN.y;
  float xinc =0,yinc = 0;

  float omarginx = e->spacing[UI_MARGIN] + e->spacing[UI_MARGIN_LEFT];
  float omarginy = e->spacing[UI_MARGIN] + e->spacing[UI_MARGIN_TOP];

  float owidth = omarginx;
  float oheight = omarginy; 
  float cwidths =0, cheights= 0;

  for(int i = 0; i<e->num_children; i++){
    switch(e->layout){
      case LAYOUT_VERTICAL:
        oheight+=e->children[i]->height;
        if(e->children[i]->width > cwidths)
          cwidths = e->children[i]->width;
        break;
      case LAYOUT_HORIZONTAL:
        owidth += e->children[i]->width;
        if(e->children[i]->height > cheights)
          cheights = e->children[i]->height;
        break;
      default:
        if(e->children[i]->height > cheights)
          cheights = e->children[i]->height;
        if(e->children[i]->width > cwidths)
          cwidths = e->children[i]->width;
        break;
    }
  }
  e->bounds.width = e->width+owidth+cwidths;
  e->bounds.height = e->height+ oheight+cheights;

  UIAlignment align = e->align;
  if(e->owner){
    xinc = e->owner->bounds.x;
    yinc = e->owner->bounds.y;
    centerx = e->owner->bounds.x + e->owner->bounds.width/2;
    centery = e->owner->bounds.y + e->owner->bounds.height/2;
    //align = e->owner->align;
  }

  if(align & ALIGN_CENTER)
    xinc = centerx-e->bounds.width/2;
  if(align & ALIGN_MID)
    yinc = centery- e->bounds.height/2;
  
  xinc += omarginx;
  yinc += omarginy;

  if(e->index > 0){
    Rectangle prior = e->owner->children[e->index-1]->bounds;

    switch(e->owner->layout){
      case LAYOUT_VERTICAL:
        yinc = prior.y + prior.height;
        break;
      case LAYOUT_HORIZONTAL:
        xinc = prior.x + prior.width;
        break;
      default:
        break;
    }
  }

  e->bounds = RectInc(e->bounds,xinc,yinc);
  for(int i = 0; i < e->num_children; i++)
    ElementResize(e->children[i]);
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
    
  UISyncElement(m->element);

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
      GuiPanel(e->bounds,NULL);//e->text);
      break;
    case UI_LABEL:
      GuiLabel(e->bounds,e->text);
      break;
    case UI_BOX:
      GuiGroupBox(e->bounds,NULL);//e->text);
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
  UICloseMenu(e->menu);
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
    case MENU_OPENED:
    case MENU_ACTIVE:
      ElementResize(m->element);
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
