#include <raylib.h>
#include "game_utils.h"
#include "game_process.h"
#include "game_tools.h"

static bt_register_entry_t BT_Registry[MAX_BEHAVIOR_TREE];
static int registry_count = 0;

behavior_tree_node_t *BehaviorGetTree(const char *name) {
   for (int i = 0; i < tree_cache_count; i++){
    if (strcmp(tree_cache[i].name, name) == 0)
      return tree_cache[i].root;
  }

   return NULL;
}

behavior_tree_node_t *BuildTreeNode(const char *name) {
  for (int i = 0; i < ROOM_BEHAVIOR_COUNT; i++){
    if (strcmp(room_behaviors[i].name, name) != 0)
      continue;

    BehaviorData data = room_behaviors[i];
    if(data.bt_type == BT_LEAF){
      behavior_params_t *params = malloc(sizeof *params);
      *params =(behavior_params_t){
        .owner = NULL,
        .event = data.event,
        .state = data.state,
        .obj_state = data.obj_state,
        .duration = data.duration,
      };
      return room_behaviors[i].func(params);
    }
    else{
      behavior_tree_node_t **kids = malloc(sizeof(*kids) * data.num_children);
      for (int j = 0; j < data.num_children; ++j)
        kids[j] = BuildTreeNode(data.children[j]);

      switch(data.bt_type){
        case BT_SEQUENCE:
          return BehaviorCreateSequence(kids, data.num_children);
          break;
        case BT_SELECTOR:
          return BehaviorCreateSelector(kids, data.num_children);
          break;
        case BT_CONCURRENT:
          return BehaviorCreateConcurrent(kids, data.num_children);
          break;
        default:
          TraceLog(LOG_WARNING,"Behavior Node Type %d NOT FOUND!",data.bt_type);
          break;
      }

    }
  }
  return NULL;
}

behavior_tree_node_t* InitBehaviorTree( const char* name){
  behavior_tree_node_t* node = BehaviorGetTree(name);

  if(node != NULL)
    return node;

  TraceLog(LOG_WARNING,"<=====Behavior Tree %s not found=====>",name);
  return NULL;
}

BehaviorStatus BehaviorChangeState(behavior_params_t *params){
  struct ent_s* e = params->owner;
  if(!e || !e->control)
    return BEHAVIOR_FAILURE;

  if(!params->state)
    return BEHAVIOR_FAILURE;

  SetState(e, params->state,NULL);
  //TraceLog(LOG_INFO,"Change e %s state to %d",e->name, params->state);
  return BEHAVIOR_SUCCESS;

}

BehaviorStatus BehaviorAcquireMousePosition(behavior_params_t *params){
  struct ent_s* e = params->owner;
  if(!e || !e->control)
    return BEHAVIOR_FAILURE;

  e->control->destination = GetMousePosition();

  return BEHAVIOR_SUCCESS;
}

BehaviorStatus BehaviorBisectDestination(behavior_params_t *params){
  struct ent_s* e = params->owner;
  if(!e || !e->control)
    return BEHAVIOR_FAILURE;

  if(v2_compare(e->control->destination,VEC_UNSET))
    return BEHAVIOR_FAILURE;

  if(!e->control->target)
    return BEHAVIOR_FAILURE;

  Vector2 newpos = Vector2Bisect(e->control->target->pos, e->control->destination, e->control->ranges[RANGE_NEAR]);

  e->facing = v2_ang_deg(Vector2Normalize(Vector2Subtract(e->control->target->pos, e->control->destination))) + 90;
  RigidBodySetPosition(e->body,newpos);
}

BehaviorStatus BehaviorAcquireDestination(behavior_params_t *params){
  struct ent_s* e = params->owner;
  if(!e || !e->control)
    return BEHAVIOR_FAILURE;

  if(!e->control->has_arrived && !v2_compare(e->control->destination,VEC_UNSET))
    return BEHAVIOR_SUCCESS;

  e->control->destination = GetWorldCoordsFromIntGrid(e->pos, e->control->ranges[RANGE_LOITER]);

 TraceLog(LOG_INFO,"Ent %d New Destination <%0.2f,%0.2f>",e->uid, e->control->destination.x,  e->control->destination.y);  
  return BEHAVIOR_SUCCESS;
}

BehaviorStatus BehaviorMoveToDestination(behavior_params_t *params){
  struct ent_s* e = params->owner;
  if(!e || !e->control)
    return BEHAVIOR_FAILURE;

  if(e->control->has_arrived){
    e->control->has_arrived = false;
    e->control->destination = VEC_UNSET;
    return BEHAVIOR_SUCCESS;
  }
  if(Vector2Distance(e->pos,e->control->destination) <  e->control->ranges[RANGE_NEAR]){
    e->control->has_arrived = true;
    return BEHAVIOR_SUCCESS;
  }

  PhysicsAccelDir(e->body, FORCE_STEERING,Vector2Normalize(Vector2Subtract(e->control->destination,e->pos)));
  return BEHAVIOR_RUNNING;

}

BehaviorStatus BehaviorAcquireTarget(behavior_params_t *params){
  struct ent_s* e = params->owner;
  if(!e || !e->control)
    return BEHAVIOR_FAILURE;

  if(e->control->target && CheckEntAvailable(e->control->target))
    return BEHAVIOR_SUCCESS;

  e->control->target = NULL;
  if(!CheckEntAvailable(player))
    return BEHAVIOR_FAILURE;

  e->control->target = player;
  return BEHAVIOR_SUCCESS;
  /*
  struct ent_s* others[MAX_ENTS];
  int num_others =  WorldGetEnts(others,FilterEntTargetable, e);
  for (int i = 0; i < num_others; i++){
    if(CheckCanSeeTarget(e->body,others[i]->body, e->control->ranges[RANGE_AGGRO])){
      e->control->target = others[i];
      return BEHAVIOR_SUCCESS;
    }
  }
*/
  return BEHAVIOR_FAILURE;
}

BehaviorStatus BehaviorCanSeeTarget(behavior_params_t *params){
  struct ent_s* e = params->owner;
  if(!e)
    return BEHAVIOR_FAILURE;
  return BEHAVIOR_FAILURE;
}

BehaviorStatus BehaviorMoveToTarget(behavior_params_t *params){
  struct ent_s* e = params->owner;
  if(!e || !e->control)
    return BEHAVIOR_FAILURE;
 
  if(!e->control->target || e->control->target->state == STATE_DIE)
    return BEHAVIOR_FAILURE;

  if(PhysicsSimpleDistCheck(e->body,e->control->target->body) < e->control->ranges[RANGE_NEAR])
    return BEHAVIOR_SUCCESS;

  PhysicsAccelDir(e->body, FORCE_STEERING,Vector2Normalize(Vector2Subtract(e->control->target->pos,e->pos)));
  return BEHAVIOR_RUNNING;
}

BehaviorStatus BehaviorCanAttackTarget(behavior_params_t *params){
  struct ent_s* e = params->owner;
  if(!e || !e->control)
    return BEHAVIOR_FAILURE;

  if(!e->control->target && !e->control->target->body)
    return BEHAVIOR_FAILURE;



  for(int i = 0; i < ATTACK_THORNS; i++){
    if(e->attacks[i].attack_type == ATTACK_BLANK)
      continue;

    if(e->attacks[i].cooldown->is_complete){
      if(!e->attacks[i].cooldown->is_complete)
        continue;

      if(PhysicsSimpleDistCheck(e->body,e->control->target->body)>e->attacks[i].reach.current)
        continue;

      return BEHAVIOR_SUCCESS;
    }
  }

  return BEHAVIOR_FAILURE;

}

BehaviorStatus BehaviorAttackTarget(behavior_params_t *params){
  struct ent_s* e = params->owner;
  if(!e)
    return BEHAVIOR_FAILURE;

  for(int i = 0; i <ATTACK_THORNS; i++){
    if(e->attacks[i].attack_type == ATTACK_BLANK)
      continue;

    if(!e->attacks[i].cooldown->is_complete)
      continue;
    
    if(!AttackPrepare(&e->attacks[i]))
      continue;

    e->attacks[i].attack(&e->attacks[i],e);

    return BEHAVIOR_SUCCESS;
  }

  return BEHAVIOR_FAILURE;
}

BehaviorStatus BehaviorCheckEvent(behavior_params_t *params){
  struct game_object_s* o = params->obj;
  if(!o || !o->control)
    return BEHAVIOR_FAILURE;

  //int index = GetEventIndex(o->events, params->event);

  if(index < 0)
    return BEHAVIOR_FAILURE;
/*
  cooldown_t ev = o->events[index];

  if(ev.is_complete)
    return BEHAVIOR_SUCCESS;
*/
  return BEHAVIOR_RUNNING;
}

BehaviorStatus BehaviorAddEvent(behavior_params_t *params){
  struct game_object_s* o = params->obj;
  if(!o || !o->control)
    return BEHAVIOR_FAILURE;

  if(CheckEvent(o->events, params->event))
    return BEHAVIOR_SUCCESS;

  int dur = params->duration;
  CooldownCallback cb = NULL;

  if(LevelCurrent()->event_durations[params->event].ev!=EVENT_NONE)
    dur = LevelCurrent()->event_durations[params->event].dur.current;

  AddEvent(o->events, InitCooldown(dur,params->event,cb,o));
  return BEHAVIOR_SUCCESS;
}

BehaviorStatus BehaviorStartEvent(behavior_params_t *params){
  struct game_object_s* o = params->obj;
  if(!o || !o->control)
    return BEHAVIOR_FAILURE;

  if(CheckEvent(o->events, params->event))
    return BEHAVIOR_SUCCESS;

  int dur = params->duration;
  CooldownCallback cb = NULL;

  if(LevelCurrent()->event_durations[params->event].ev!=EVENT_NONE)
    dur = LevelCurrent()->event_durations[params->event].dur.current;

  AddEvent(o->events, InitCooldown(dur,params->event,cb,o));

  return BEHAVIOR_SUCCESS;
}

BehaviorStatus BehaviorStartState(behavior_params_t *params){
  struct game_object_s* o = params->obj;
  if(!o || !o->control)
    return BEHAVIOR_FAILURE;

  SetObjectState(o,params->obj_state);
  return BEHAVIOR_SUCCESS;
}

BehaviorStatus BehaviorSpawnEnt(behavior_params_t *params){
  struct game_object_s* o = params->obj;
  if(!o || !o->control)
    return BEHAVIOR_FAILURE;
  
  if(CheckEvent(o->events, params->event))
    return BEHAVIOR_RUNNING;

  //if(SpawnEnt(o))
    return BEHAVIOR_SUCCESS;

  return BEHAVIOR_FAILURE;
}


BehaviorStatus BehaviorTickLeaf(behavior_tree_node_t *self, void *context) {
    behavior_tree_leaf_t *leaf = (behavior_tree_leaf_t *)self->data;
    if (!leaf || !leaf->action) return BEHAVIOR_FAILURE;
    leaf->params->owner = context;
    leaf->params->obj = context;
    return leaf->action(leaf->params);
}

behavior_tree_node_t* BehaviorCreateLeaf(BehaviorTreeLeafFunc fn, behavior_params_t* params){
  behavior_tree_leaf_t *data = malloc(sizeof(behavior_tree_leaf_t));

  data->action = fn;
  data->params = params;
  
  behavior_tree_node_t* node = malloc(sizeof(behavior_tree_node_t));
  node->bt_type = BT_LEAF;
  node->tick = BehaviorTickLeaf;
  node->data = data;

  return node;
}

behavior_tree_node_t* BehaviorCreateSequence(behavior_tree_node_t **children, int count) {
    behavior_tree_sequence_t *data = calloc(1,sizeof(behavior_tree_sequence_t));
    data->children = children;
    data->num_children = count;
    data->current = 0;

    behavior_tree_node_t *node = malloc(sizeof(behavior_tree_node_t));
    node->bt_type = BT_SEQUENCE;
    node->tick = BehaviorTickSequence;
    node->data = data;
    return node;
}

behavior_tree_node_t* BehaviorCreateSelector(behavior_tree_node_t **children, int count) {
    behavior_tree_selector_t *data = malloc(sizeof(behavior_tree_selector_t));
    data->children = children;
    data->num_children = count;
    data->current = 0;

    behavior_tree_node_t *node = malloc(sizeof(behavior_tree_node_t));
    node->bt_type = BT_SELECTOR;
    node->tick = BehaviorTickSelector;
    node->data = data;
    return node;
}

behavior_tree_node_t* BehaviorCreateConcurrent(behavior_tree_node_t **children, int count) {
    behavior_tree_selector_t *data = malloc(sizeof(behavior_tree_selector_t));
    data->children = children;
    data->num_children = count;
    data->current = 0;

    behavior_tree_node_t *node = malloc(sizeof(behavior_tree_node_t));
    node->bt_type = BT_CONCURRENT;
    node->tick = BehaviorTickConcurrent;
    node->data = data;
    return node;
}

BehaviorStatus BehaviorTickSequence(behavior_tree_node_t *self, void *context) {
    behavior_tree_sequence_t *seq = (behavior_tree_sequence_t *)self->data;
    while (seq->current < seq->num_children) {
        BehaviorStatus status = seq->children[seq->current]->tick(seq->children[seq->current], context);
        if (status == BEHAVIOR_RUNNING) return BEHAVIOR_RUNNING;
        if (status == BEHAVIOR_FAILURE) {
            seq->current = 0;
            return BEHAVIOR_FAILURE;
        }
        seq->current++;
    }

    seq->current = 0;
    return BEHAVIOR_SUCCESS;
}

BehaviorStatus BehaviorTickSelector(behavior_tree_node_t *self, void *context) {
    behavior_tree_selector_t *sel = (behavior_tree_selector_t *)self->data;

    while (sel->current < sel->num_children) {
        BehaviorStatus status = sel->children[sel->current]->tick(sel->children[sel->current], context);
        if (status == BEHAVIOR_RUNNING) return BEHAVIOR_RUNNING;
        if (status == BEHAVIOR_SUCCESS) {
            sel->current = 0;
            return BEHAVIOR_SUCCESS;
        }
        sel->current++;
    }

    sel->current = 0;
    return BEHAVIOR_FAILURE;
}

BehaviorStatus BehaviorTickConcurrent(behavior_tree_node_t *self, void *context) {
  behavior_tree_selector_t *sel = (behavior_tree_selector_t *)self->data;

  bool anyRunning = false;
  bool anySuccess = false;
  bool anyFailure = false;

  for (int i = 0; i < sel->num_children; i++) {
    BehaviorStatus status = sel->children[i]->tick(sel->children[i], context);
    if (status == BEHAVIOR_RUNNING) anyRunning = true;
    else if (status == BEHAVIOR_SUCCESS) anySuccess = true;
    else if (status == BEHAVIOR_FAILURE) anyFailure = true;
  }

  // Rule set: "success if all succeed"
  if (!anyRunning && !anyFailure) return BEHAVIOR_SUCCESS;
  if (anyRunning) return BEHAVIOR_RUNNING;
  return BEHAVIOR_FAILURE;

}

