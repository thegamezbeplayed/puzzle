#include "game_process.h"
#include "game_tools.h"

level_order_t levels;

level_t* LevelCurrent(){
  return levels.levels[levels.current];
}

void SetLevelState(level_t *l, LevelState state){
  if(!CanChangeLevelState(l->state, state))
    return;

  LevelState old = l->state;
  l->state = state;

  OnLevelStateChange(l, state);
}

level_t* GetLevel(unsigned int index){
  if(index < 4)
    return levels.levels[index];

  return NULL;
}

void OnLevelStateChange(level_t *l, LevelState state){
  switch(state){
    case LEVEL_FINISH:
      break;
    case LEVEL_LOADING:
      InitLevel(l);
      break;
    case LEVEL_START:
      SetLevelState(CURRENT_LEVEL, LEVEL_RUNNING);
      break;
    case LEVEL_RUNNING:
      break;
    case LEVEL_COMPLETE:
      AddPoints(1,l->points,VECTOR2_CENTER_SCREEN);
      break;
    default:
      break;
  }
}

bool CanChangeLevelState(LevelState old, LevelState s){
  if(old == s)
    return false;

  return true;
}

void LevelBegin(level_t *l){
  levels.current = l->luid;
  SetLevelState(CURRENT_LEVEL,LEVEL_START);
}
void LevelStep(){
  switch(CURRENT_LEVEL->state){
    case LEVEL_RUNNING:
      break;
    default:
      break;
  }
}

entity_pool_t* InitEntityPool(void) {
    entity_pool_t* pool = malloc(sizeof(entity_pool_t));
    pool->num_references = 0;
    pool->current_ref = 0;
    for(int i = 0; i < MAX_SPAWNS; i++)
      pool->reference_ents[i] = ENT_MOB;
    return pool;
}

void FreeLevels(){
  for(int i = 0; i<levels.num_levels;i++)
    FreeLevel(levels.levels[i]);

  levels = (level_order_t){0};

}

void FreeLevel(level_t* l){
  if(!l)
    return;

  free(l);
}

void InitLevel(level_t *l){
  TraceLog(LOG_INFO,"levels initialized");
}

void ResetLevel(level_t* l){
}

void GenerateLevels(int num_levels, bool inc_diff){
  LevelBegin(levels.levels[0]);
}

void RegisterPoolRef(unsigned int level_index,unsigned int index, EntityType ref){
}
