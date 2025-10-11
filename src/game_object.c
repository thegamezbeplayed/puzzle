#include "game_types.h"
#include "game_process.h"
#include "game_tools.h"

MAKE_ADAPTER(SpawnEnt, game_object_t*);
MAKE_ADAPTER(StepObjectState, game_object_t*);

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

  if(frand()>inst.chance)
    SetObjectState(obj,OBJECT_INACTIVE);
  else
    TraceLog(LOG_INFO,"loading wave %d",inst.id);

  obj->events = InitEvents();
/*
  obj->control = InitController();
  obj->control->bt[OBJECT_LOAD] = InitBehaviorTree("Load");
  obj->control->bt[OBJECT_START] = InitBehaviorTree("Prep");
  //obj->control->bt[OBJECT_RUN] = InitBehaviorTree("Check");
  obj->control->bt[OBJECT_PAUSE] =InitBehaviorTree("Prep");
  obj->control->bt[OBJECT_FINISH] =InitBehaviorTree("Finish");
*/
  return obj;

}

void FreeObject(game_object_t* o){
  UnloadEvents(o->events);
  free(o);
}

void OnObjectStateChange(game_object_t *obj, ObjectState s){
  level_t* l = GetLevel(obj->level_id);
  switch(s){
    case OBJECT_START:
      EventDefaultDuration evd = l->event_durations[EVENT_SPAWN];
      cooldown_t* cd = InitCooldown(evd.dur.current,evd.ev,StepObjectState_Adapter,obj);
      cd->is_recycled = true;
      int evIndex = AddEvent(obj->events,cd);
      TraceLog(LOG_INFO,"event id:%d",evIndex);
      LevelLoadWave(obj->id+1,obj->level_id);
      break;
    case OBJECT_END:
      UnloadEvents(obj->events);
      break;
    case OBJECT_RUN:
      if(SpawnEnt(obj)){
        SetObjectState(obj,OBJECT_PAUSE);
        ResetEvent(obj->events,EVENT_SPAWN);
      }
      else
        SetObjectState(obj,OBJECT_END);
      break;
    case OBJECT_LOAD:
      EventDefaultDuration evwd = l->event_durations[EVENT_WAVE];
      cooldown_t* cdw = InitCooldown(evwd.dur.current,evwd.ev,StepObjectState_Adapter,obj);
      AddEvent(obj->events,cdw);
      break;
    case OBJECT_FINISH:
      LevelLoadWave(obj->id+1,obj->level_id);
      SetObjectState(obj,OBJECT_END);
      break;
    default:
      break;
  }
//  TraceLog(LOG_INFO,"obj %d state changed to %d",obj->id, s);
}

bool CanChangeObjectState(ObjectState old, ObjectState s){
  if(old == s)
    return false;

  switch(COMBO_KEY(old,s)){
    case COMBO_KEY(OBJECT_INACTIVE,OBJECT_LOAD):
    case COMBO_KEY(OBJECT_INACTIVE,OBJECT_START):
      return false;
      break;
    default:
      break;
  }
   
  return true;
}

void SetObjectState(game_object_t *obj, ObjectState state){
  ObjectState old = obj->state;
  if(!CanChangeObjectState(old,state)){
    SetObjectState(obj,OBJECT_FINISH);  
    return;
  }
  
  obj->state = state;

  if(old!=state)
    OnObjectStateChange(obj,state);
}

void StepObjectState(game_object_t *obj){
  switch(obj->state){
    case OBJECT_LOAD:
      SetObjectState(obj,OBJECT_START);
      break;
    case OBJECT_START:
    case OBJECT_PAUSE:
      SetObjectState(obj,OBJECT_RUN);
      break;
    case OBJECT_RUN:
      SetObjectState(obj,OBJECT_PAUSE);
      break;
    default:
      break;
  }
}

void StepObject(game_object_t *obj){
  StepEvents(obj->events);
  if(!obj->control || !obj->control->bt || !obj->control->bt[obj->state])
    return;

  behavior_tree_node_t* current = obj->control->bt[obj->state];

  current->tick(current, obj);

}
