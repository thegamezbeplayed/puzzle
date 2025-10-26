#include "game_types.h"
#include "game_tools.h"
#include "game_math.h"
#include "game_process.h"

MAKE_ADAPTER(StepState, ent_t*);

ent_t* InitEnt(ObjectInstance data){
  ent_t* e = malloc(sizeof(ent_t));
  *e = (ent_t){0};  // zero initialize if needed
  e->type = ENT_SHAPE;

  e->sprite = InitSpriteByID(data.id,&shapedata);
  e->sprite->owner = e;
 
  SetState(e,STATE_SPAWN,NULL);
  return e;
}

ent_t* InitEntStatic(TileInstance data,Vector2 pos){
  ent_t* e = malloc(sizeof(ent_t));
  *e = (ent_t){0};  // zero initialize if needed
  e->type = ENT_TILE;

  e->pos = pos;
  e->sprite = InitSpriteByIndex(data.id,&tiledata);
  e->sprite->owner = e;

  e->control = InitController();
  for (int i = STATE_SPAWN; i < STATE_END; i++){
    if(data.behaviors[i] == BEHAVIOR_NONE)
      continue;
    e->control->bt[i] = InitBehaviorTree(data.behaviors[i]);
  }

  SetState(e,STATE_SPAWN,NULL);
  return e;
}

void EntAddPoints(ent_t *e,EntityState old, EntityState s){
  //if(e->stats[STAT_POINTS].current <= 0)
   // return;

  //AddPoints(GetInteractions(e->lastdamage_sourceid)-1, e->stats[STAT_POINTS].current,e->pos);
}

bool EntKill(ent_t* e){
  return SetState(e, STATE_DIE,NULL);
}

void EntDestroy(ent_t* e){
  if(!e || !SetState(e, STATE_END,EntAddPoints))
    return;
 
  /*if(e->sprite!=NULL){
    e->sprite->owner = NULL;
    e->sprite->is_visible = false;
  }*/

  e->control = NULL;
}
void EntPrepStep(ent_t* e){
}

bool FreeEnt(ent_t* e){
  if(!e)
    return false;

  free(e);
  return true;
}

controller_t* InitController(){
  controller_t* ctrl = malloc(sizeof(controller_t));
  *ctrl = (controller_t){0};

  ctrl->destination = VEC_UNSET;
  return ctrl;
}

void EntSync(ent_t* e){
  if(e->control)  
    EntControlStep(e);

  if(e->events)
    StepEvents(e->events);

  if(!e->sprite)
    return;

  e->sprite->pos = e->pos;// + abs(ent->sprite->offset.y);
}

void EntControlStep(ent_t *e){
  if(!e->control || !e->control->bt || !e->control->bt[e->state])
    return;

  behavior_tree_node_t* current = e->control->bt[e->state];

  current->tick(current, e);
}

bool SetState(ent_t *e, EntityState s,StateChangeCallback callback){
  if(CanChangeState(e->state,s)){
    EntityState old = e->state;
    e->state = s;

    if(callback!=NULL)
      callback(e,old,s);
    else
      OnStateChange(e,old,s);
    return true;
  }

  return false;
}

void StepState(ent_t *e){
  SetState(e, e->state+1,NULL);
}

bool CanChangeState(EntityState old, EntityState s){
  if(old == s || old > STATE_END)
    return false;

  switch (COMBO_KEY(old,s)){
    case COMBO_KEY(STATE_NONE,!STATE_SPAWN):
    case COMBO_KEY(!STATE_NONE,STATE_SPAWN):
    case COMBO_KEY(!STATE_DIE,STATE_END):
      return false;
      break;
    default:
      return true;
      break;
  }
} 

void OnStateChange(ent_t *e, EntityState old, EntityState s){
  switch(old){
    case STATE_SPAWN:
      if(e->owner)
        e->pos = e->owner->pos;
      if(e->sprite)
        e->sprite->is_visible = true;
      break;
    default:
      break;
  }

  switch(s){
    case STATE_DIE:
      AudioPlayRandomSfx(SFX_ACTION,ACTION_BOOM);
      break;
    default:
      break;
  }

}
bool CheckEntAvailable(ent_t* e){
  if(!e)
    return false;

  return (e->state < STATE_DIE);
}
