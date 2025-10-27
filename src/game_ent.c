#include "game_types.h"
#include "game_tools.h"
#include "game_math.h"
#include "game_process.h"

ent_t* InitEnt(ObjectInstance data){
  ent_t* e = malloc(sizeof(ent_t));
  *e = (ent_t){0};  // zero initialize if needed
  e->type = ENT_SHAPE;

  e->shape = data.id;
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

void EntSetPos(ent_t *e, Vector2 pos){
  e->sprite->pos = e->pos = pos;
}

void EntControlStep(ent_t *e){
  if(!e->control || !e->control->bt || !e->control->bt[e->state])
    return;

  behavior_tree_node_t* current = e->control->bt[e->state];

  current->tick(current, e);
}

void SetViableTile(ent_t* e, EntityState old, EntityState s){
  ent_t* neighbors[4];
  
  int num_neighbors = WorldGetEnts(neighbors, FilterEntNeighbor, e->owner);

  for (int i = 0; i < num_neighbors; i++){
    if(!CheckEntPosition(neighbors[i],e->pos))
      continue;

    EntChangeOccupant(e, neighbors[i]);

  }
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
    case STATE_SCORE:
      TraceLog(LOG_INFO,"Scored at [%i,%i]",e->intgrid_pos.x,e->intgrid_pos.y);
    default:
      break;
  }
}

void EntChangeOccupant(ent_t* e, ent_t* owner){
  ent_t* tenant = owner->child;
  ent_t* old = e->owner;

  if(!EntSetOwner(e,owner,true,NULL))
    return;
  
  if(!tenant)
    return;

  if(!EntSetOwner(tenant,old,false,NULL)){
    EntSetOwner(e,old,true,NULL);
    return;
  }

  WorldCheckGrid(e,owner);
  WorldCheckGrid(tenant,old);
}

void EntOnOwnerChange(ent_t *e, ent_t* old, ent_t* owner){
  if(e->uid <= 0){
    RegisterEnt(e);
    SetState(e,STATE_IDLE,NULL);
  }

  e->pos = owner->pos;
  e->intgrid_pos = owner->intgrid_pos;
  owner->shape = e->shape;
}

bool EntSetOwner(ent_t* e, ent_t* owner, bool evict, OwnerChangeCallback cb){
  if (e->owner == owner)
    return false;

  if(!evict && owner->child)
    return false;

  ent_t* old = e->owner;
  e->owner = owner;
  owner->child = e;
  if(old && old->child && old->child == e)
    old->child = NULL;
  
  EntOnOwnerChange(e,old, e->owner);
  if(cb)
    cb(e,old,e->owner);

  return true;
}

bool CheckEntPosition(ent_t* e, Vector2 pos){
  Vector2 topCorner = Vector2Subtract(e->sprite->pos,e->sprite->slice->center);

  Rectangle bounds = RectPos(topCorner, e->sprite->slice->bounds);
  return point_in_rect(pos, bounds);
}

bool CheckEntAvailable(ent_t* e){
  if(!e)
    return false;

  return (e->state < STATE_DIE);
}
