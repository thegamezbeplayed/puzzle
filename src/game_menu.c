//#include "game_ui.h"
#include "game_utils.h"
#include "screens.h"
#include "game_tools.h"
#include "game_process.h"
#include "game_ui.h"
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"
void GuiDrawRectangle(Rectangle rec, int borderWidth, Color borderColor, Color baseColor) { }
ui_manager_t ui;
void InitUI(void){
#if defined(PLATFORM_WEB)
  GuiSetStyle(DEFAULT,TEXT_SIZE,27);
#else
  GuiSetStyle(DEFAULT,TEXT_SIZE,20);
#endif
  GuiSetStyle(DEFAULT,TEXT_ALIGNMENT,TEXT_ALIGN_CENTER);
  GuiSetStyle(LABEL,TEXT_ALIGNMENT,TEXT_ALIGN_LEFT);
  GuiSetStyle(DEFAULT,BORDER_COLOR_NORMAL,ColorToInt(WHITE));
  //GuiSetStyle(STATUSBAR,BASE_COLOR_NORMAL,ColorToInt(Fade(WHITE,0.5f)));

  for (int i = 0; i< MENU_DONE; i++)
    ui.menu_key[i] = KEY_NULL;

  ui.menu_key[MENU_PAUSE] = KEY_P;
  ui.menus[MENU_PAUSE] = InitMenu(MENU_PAUSE,VECTOR2_ZERO,VECTOR2_SCREEN,ALIGN_CENTER|ALIGN_MID,LAYOUT_VERTICAL,true);
  ui.menus[MENU_PAUSE].cb[MENU_OPENED] = TogglePause; 
  ui.menus[MENU_PAUSE].cb[MENU_CLOSE] = TogglePause;

  ui_element_t *resumeBtn = InitElement("RESUME_BTN",UI_BUTTON,VECTOR2_ZERO,DEFAULT_BUTTON_SIZE,0,0); 
  strcpy(resumeBtn->text, "RESUME");
  resumeBtn->cb[ELEMENT_ACTIVATED] = UICloseOwner;
  ElementAddChild(ui.menus[MENU_PAUSE].element,resumeBtn);
  ui.menus[MENU_MAIN] = InitMenu(MENU_MAIN,VECTOR2_ZERO,DEFAULT_MENU_SIZE,ALIGN_CENTER|ALIGN_MID,LAYOUT_VERTICAL,false);

  ui.menus[MENU_HUD] = InitMenu(MENU_HUD,VECTOR2_ZERO,VECTOR2_ZERO,ALIGN_CENTER,LAYOUT_HORIZONTAL,false);

  ui.menus[MENU_MAIN].element->texture = InitScalingElement(ELEMENT_PANEL_GRAY);
  ui_element_t *turnBox = InitElement("TURN_BAR",UI_STATUSBAR,VECTOR2_ZERO, XS_PANEL_SIZE,0,0);
  ui_element_t *playBtn = InitElement("PLAY_BTN",UI_BUTTON,VECTOR2_ZERO,DEFAULT_BUTTON_SIZE,ALIGN_CENTER|ALIGN_MID,0); 
  playBtn->texture = InitScalingElement(ELEMENT_BUTTON_GRAY);
  strcpy(playBtn->text, "PLAY");
  playBtn->cb[ELEMENT_ACTIVATED] = UITransitionScreen;
  ElementAddChild(ui.menus[MENU_MAIN].element,playBtn);

  strcpy(turnBox->text, "TURN");

  ui_element_t *comboBox = InitElement("COMBO_BAR",UI_STATUSBAR,VECTOR2_ZERO, XS_PANEL_SIZE,0,0);
  strcpy(comboBox->text, "COMBO");

 ui_element_t *scoreBox = InitElement("SCORE_BAR",UI_STATUSBAR,VECTOR2_ZERO, XS_PANEL_SIZE,0,0);
  strcpy(scoreBox->text, "SCORE");
   ui_element_t *turnText = InitElement("TURN_BAR",UI_STATUSBAR,VECTOR2_ZERO,XS_PANEL_SIZE,0,0);
  turnText->get_val = GetDisplayTurn;

 ui_element_t *comboText = InitElement("STREAK_BAR",UI_STATUSBAR,VECTOR2_ZERO,XS_PANEL_SIZE,0,0);
  comboText->get_val = GetDisplayCombo;

  ui_element_t *scoreText = InitElement("POINT_BAR",UI_STATUSBAR,VECTOR2_ZERO,XS_PANEL_SIZE,0,0);
  scoreText->get_val = GetDisplayPoints;
  
   ui_element_t *turnPanel = InitElement("TURN_PANEL",UI_PANEL,VECTOR2_ZERO,VECTOR2_ZERO,0,LAYOUT_HORIZONTAL);
  turnPanel->texture = InitScalingElement(ELEMENT_PANEL_GRAY_WIDE);
  ElementAddChild(turnPanel,turnBox);
  ElementAddChild(turnPanel,turnText);

  ui_element_t *comboPanel = InitElement("COMBO_PANEL",UI_PANEL,VECTOR2_ZERO,VECTOR2_ZERO,0,LAYOUT_HORIZONTAL);
  comboPanel->texture = InitScalingElement(ELEMENT_PANEL_GRAY_WIDE);
  ElementAddChild(comboPanel,comboBox);
  ElementAddChild(comboPanel,comboText);

  ui_element_t *scorePanel = InitElement("SCORE_PANEL",UI_PANEL,VECTOR2_ZERO,VECTOR2_ZERO,0,LAYOUT_HORIZONTAL);
  scorePanel->texture = InitScalingElement(ELEMENT_PANEL_GRAY_WIDE);
  ElementAddChild(scorePanel,scoreBox);
  ElementAddChild(scorePanel,scoreText);


  ElementAddChild(ui.menus[MENU_HUD].element,turnPanel);
  ElementAddChild(ui.menus[MENU_HUD].element,comboPanel);
  ElementAddChild(ui.menus[MENU_HUD].element,scorePanel);

  ui.menus[MENU_RECAP] = InitMenu(MENU_RECAP,VECTOR2_ZERO,DEFAULT_MENU_SIZE,ALIGN_CENTER|ALIGN_MID,LAYOUT_VERTICAL,false);

  ui.menus[MENU_RECAP].element->spacing[UI_MARGIN_TOP]=6;
  ui.menus[MENU_RECAP].element->texture = InitScalingElement(ELEMENT_PANEL_GRAY);

  ui_element_t *endScore = InitElement("SCORE_PANEL",UI_PANEL,VECTOR2_ZERO,VECTOR2_ZERO,ALIGN_CENTER|ALIGN_MID,LAYOUT_HORIZONTAL);
  endScore->texture = InitScalingElement(ELEMENT_PANEL_GRAY_WIDE);
  ui_element_t *pointsBox =InitElement("SCORE_LBL",UI_STATUSBAR,VECTOR2_ZERO,XS_PANEL_SIZE,0,0);
  ui_element_t *pointsText =InitElement("POINTS_LBL",UI_STATUSBAR,VECTOR2_ZERO,XS_PANEL_SIZE,0,0);
  strcpy(pointsBox->text, "SCORE");
  pointsText->get_val = GetDisplayPoints;

  ui_element_t *recapBtn = InitElement("RECAP_BTN",UI_BUTTON,VECTOR2_ZERO,DEFAULT_BUTTON_SIZE,ALIGN_CENTER|ALIGN_MID,0); 
  recapBtn->texture =  InitScalingElement(ELEMENT_PANEL_GRAY_WIDE);
  strcpy(recapBtn->text, "CONTINUE");
  recapBtn->cb[ELEMENT_ACTIVATED] = UITransitionScreen;

  ElementAddChild(endScore,pointsBox);
  ElementAddChild(endScore,pointsText);

  ElementAddChild(ui.menus[MENU_RECAP].element,endScore);
  ElementAddChild(ui.menus[MENU_RECAP].element,recapBtn);

  ui.menus[MENU_EXIT] = InitMenu(MENU_EXIT,VECTOR2_ZERO,SMALL_PANEL_SIZE,ALIGN_CENTER|ALIGN_MID,LAYOUT_VERTICAL,true);
  ui.menus[MENU_EXIT].element->texture = InitScalingElement(ELEMENT_ERROR_WIDE);
  ui_element_t *endBtn = InitElement("END_BTN",UI_BUTTON,VECTOR2_ZERO,DEFAULT_BUTTON_SIZE,ALIGN_CENTER|ALIGN_MID,0);
  strcpy(endBtn->text,"GAME OVER");
  endBtn->cb[ELEMENT_ACTIVATED] = UITransitionScreen;
  ElementAddChild(ui.menus[MENU_EXIT].element,endBtn);


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
    u->cb[i] = UI_BOOL_DO_NOTHING;

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

float ElementGetHeightSum(ui_element_t *e){
  float height = e->bounds.height + e->spacing[UI_MARGIN] + e->spacing[UI_MARGIN_TOP];

  float cheight = 0;
  for(int i = 0; i < e->num_children; i++)
    if(e->layout == LAYOUT_VERTICAL)
      cheight += ElementGetHeightSum(e->children[i]);
    else
      cheight = (cheight < ElementGetHeightSum(e->children[i]))?ElementGetHeightSum(e->children[i]):cheight;

  return height+cheight;
}

float ElementGetWidthSum(ui_element_t *e){
  float width = e->bounds.width + e->spacing[UI_MARGIN] + e->spacing[UI_MARGIN_LEFT];

  float cwidth = 0;
  for(int i = 0; i < e->num_children; i++)
    if(e->layout == LAYOUT_HORIZONTAL)
      cwidth += ElementGetWidthSum(e->children[i]);
    else
      cwidth = (cwidth < ElementGetWidthSum(e->children[i]))?ElementGetWidthSum(e->children[i]):cwidth;

  return width+cwidth;
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
        oheight+=ElementGetHeightSum(e->children[i]);
        if(ElementGetWidthSum(e->children[i]) > cwidths)
          cwidths = ElementGetWidthSum(e->children[i]);
        break;
      case LAYOUT_HORIZONTAL:
        owidth += ElementGetWidthSum(e->children[i]);
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

  e->bounds.width = e->width + owidth+cwidths;
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
    omarginx = e->owner->spacing[UI_MARGIN] + e->owner->spacing[UI_MARGIN_LEFT];
    omarginy = e->owner->spacing[UI_MARGIN] + e->owner->spacing[UI_MARGIN_TOP];


    switch(e->owner->layout){
      case LAYOUT_VERTICAL:
        yinc = omarginy+prior.y + prior.height;
        break;
      case LAYOUT_HORIZONTAL:
        xinc = omarginx + prior.x + prior.width;
        break;
      default:
        break;
    }
  }

  e->bounds = RectInc(e->bounds,xinc,yinc);
  for(int i = 0; i < e->num_children; i++)
    ElementResize(e->children[i]);
}

void UISync(void){
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
   if(e->texture)
    DrawNineSlice(e->texture,e->bounds);

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
  return UICloseMenu(e->menu);
}

bool UICloseMenu(ui_menu_t* m){
  return MenuSetState(m,MENU_CLOSE);
}

bool UITransitionScreen(ui_element_t* e){
  GameTransitionScreen();
  return true; 
}

void MenuOnStateChanged(ui_menu_t*m, MenuState old, MenuState s){
  m->cb[m->state](m);

  switch(old){
    case MENU_INACTIVE:
      ElementResize(m->element);
      break;
    default:
      break;
  }

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

ElementValue GetDisplayTurn(void){
  ElementValue ev = {0};
  ev.type = VAL_CHAR;
  ev.c = GetTurn();
  return ev;

}

ElementValue GetDisplayCombo(void){
  ElementValue ev = {0};
  ev.type = VAL_CHAR;
  ev.c = GetComboStreak();
  return ev;

}

ElementValue GetDisplayTime(void){
  ElementValue ev = {0};
  ev.type = VAL_CHAR;
  ev.c = GetGameTime();
  return ev;
}
