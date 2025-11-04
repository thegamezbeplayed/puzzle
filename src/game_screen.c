#include "raylib.h"
#include "game_tools.h"
#include "game_process.h"

mouse_controller_t mousectrl;

void InitScreenInteractive(void){
  mousectrl = (mouse_controller_t) {
    .is_dragging = false,
      .pos = GetMousePosition(),
      .offset = VECTOR2_ZERO,
      .target = NULL
  };

}

void ClearMouse(void){
  mousectrl.target = NULL;
  mousectrl.offset = VECTOR2_ZERO;
  mousectrl.is_dragging = false;
}

void ScreenSyncMouse(void){

  mousectrl.pos = GetMousePosition();
  if(!mousectrl.target && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){

    mousectrl.target = ScreenEntMouseCollision();
    if(mousectrl.target){
      mousectrl.offset = Vector2Subtract(mousectrl.pos, mousectrl.target->pos);
      mousectrl.is_dragging = true;
    }
    else
      mousectrl.is_dragging =false;
  }
  else if(IsMouseButtonReleased(MOUSE_LEFT_BUTTON)){
    if(mousectrl.target){

      SetState(mousectrl.target,STATE_PLACED,SetViableTile);
      EntSetPos(mousectrl.target, mousectrl.target->owner->pos);
    }
    ClearMouse(); 
  }
  else if (!mousectrl.is_dragging){
    ent_t* hover = ScreenEntMouseHover();
    if(mousectrl.hover && mousectrl.hover != hover){
      SetState(mousectrl.hover,STATE_IDLE,NULL);
    }

    mousectrl.hover = hover;
    if(mousectrl.hover)
      SetState(mousectrl.hover,STATE_HOVER,NULL);
  }

  if(mousectrl.is_dragging){
    SetState(mousectrl.target,STATE_SELECTED,NULL);
    EntSetPos(mousectrl.target, Vector2Add(mousectrl.pos,mousectrl.offset));
  }
}

ent_t* ScreenEntMouseHover(void){
  ent_t* shape_pool[GRID_WIDTH * GRID_HEIGHT];

  int num_shapes = WorldGetEnts(shape_pool,FilterEntShape, NULL);
  if(num_shapes <= 0)
    return NULL;

  for(int i = 0; i < num_shapes; i++){
    if(CheckEntPosition(shape_pool[i],mousectrl.pos))
      return shape_pool[i];
  }

  return NULL;
}
ent_t* ScreenEntMouseCollision(void){
  ent_t* shape_pool[GRID_WIDTH * GRID_HEIGHT];

  int num_shapes = WorldGetEnts(shape_pool,FilterEntShape, NULL);
  if(num_shapes <= 0)
    return NULL;

  for(int i = 0; i < num_shapes; i++){
    if(shape_pool[i]->control->moves<1)
      continue;
    if(CheckEntPosition(shape_pool[i],mousectrl.pos))
      return shape_pool[i];
  }

  return NULL;
}
