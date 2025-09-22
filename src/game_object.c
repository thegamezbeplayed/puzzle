#include "game_types.h"
#include "game_process.h"

MAKE_ADAPTER(SpawnEnt, game_object_t*);

game_object_t* InitObjectStatic(SpawnerInstance inst){
  game_object_t* obj = malloc(sizeof(game_object_t));
  *obj = (game_object_t){0};
  obj->id = inst.id;
  obj->pos = (Vector2) {inst.x,inst.y};

  for (int i =0; i < ENT_BLANK; i++){
    if(inst.spawn_refs[i] == 0)
      continue;
    for(int j = 0; j < inst.spawn_refs[i]; j++)
      RegisterPoolRef(inst.level,inst.id, i);
  
  }

  //TODO make events trigger fn upon complete rather than having BT trigger
  obj->events = InitEvents();
  cooldown_t *cd =InitCooldown(EVENT_SPAWN,inst.spawn_rate,SpawnEnt_Adapter,obj);
  cd->is_recycled= true;
  AddEvent(obj->events,cd);
  obj->control = InitController();
  obj->control->bt[OBJECT_LOAD] = InitBehaviorTree("Load");
  obj->control->bt[OBJECT_START] = InitBehaviorTree("Prep");
  obj->control->bt[OBJECT_RUN] = InitBehaviorTree("Check");
  obj->control->bt[OBJECT_PAUSE] =InitBehaviorTree("Prep");
  obj->control->bt[OBJECT_FINISH] =InitBehaviorTree("Finish");

  return obj;

}

void OnObjectStateChange(game_object_t *obj, ObjectState s){
  switch(s){
    case OBJECT_START:
      LevelLoadWave(obj->id+1);
      break;
    case OBJECT_END:
      LevelSyncSpawners(obj->level_id,obj->id);
      break;
    default:
      break;
  }
}

bool CanChangeObjectState(ObjectState old, ObjectState s){
  if(old == s)
    return false;

  return true;
}

void SetObjectState(game_object_t *obj, ObjectState state){
  ObjectState old = obj->state;
  if(!CanChangeObjectState(old,state))
      return;
  
  obj->state = state;

  if(old!=state)
    OnObjectStateChange(obj,state);
}

void StepObject(game_object_t *obj){
  StepEvents(obj->events);
  if(!obj->control || !obj->control->bt || !obj->control->bt[obj->state])
    return;

  behavior_tree_node_t* current = obj->control->bt[obj->state];

  current->tick(current, obj);

}
