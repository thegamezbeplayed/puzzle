#include "game_process.h"
#include "game_tools.h"

difficulty_modifier_t level_mods[MOD_DONE] = {
  {.type = MOD_NONE},
  {.type = MOD_LEVEL_DIFF, .denom = 1,.amount = 0.33f, .modFn = ModifyLevelDifficulty},
  {.type = MOD_LEVEL_POINTS, .denom = 150,.amount = 1, .modFn = ModifyLevelPoints},
  {.type = MOD_MOB_UPGRADE, .denom = 150,.amount = 1, .modFn = ModifyMobUpgrade},
  {.type = MOD_WAVE_INTERVAL, .denom = 100, .amount = -6, .modFn = ModifyWaveInterval},
};

level_order_t levels;
level_t level_data[4] = {
  {.luid = 0, .points = 20, .difficulty = 0.33f, .modifiers={[MOD_LEVEL_DIFF]=1,[MOD_WAVE_INTERVAL]=1,[MOD_MOB_UPGRADE]=2, [MOD_LEVEL_POINTS]=3}},
  {.luid = 1, .points = 24, .difficulty = 0.66f, .modifiers={[MOD_LEVEL_DIFF]=1,[MOD_WAVE_INTERVAL]=1,[MOD_MOB_UPGRADE]=2,[MOD_LEVEL_POINTS]=3}},
  {.luid = 2, .points = 28, .difficulty = 0.99f, .modifiers={[MOD_LEVEL_DIFF]=1,[MOD_WAVE_INTERVAL]=1,[MOD_MOB_UPGRADE]=2, [MOD_LEVEL_POINTS]=4}},
  {.luid = 3, .points = 30, .difficulty = 1.32f, .modifiers={[MOD_LEVEL_DIFF]=1,[MOD_WAVE_INTERVAL]=1,[MOD_MOB_UPGRADE]=3, [MOD_LEVEL_POINTS]=5}},
  };

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

void OnLevelStateChange(level_t *l, LevelState state){
  switch(state){
    case LEVEL_FINISH:
      LevelEnd(l);
      break;
    case LEVEL_LOADING:
      InitLevel(l);
      break;
    case LEVEL_START:
      SetLevelState(CURRENT_LEVEL, LEVEL_RUNNING);
      break;
    case LEVEL_RUNNING:
      SetObjectState(l->mob_spawners[0],OBJECT_START);
    case LEVEL_COMPLETE:
      AddPoints(1,l->points,VECTOR2_CENTER_SCREEN);
    default:
      break;
  }
}

bool CanChangeLevelState(LevelState old, LevelState s){
  return true;
}

int LevelGetCurrentWaveNum(){
  return levels.round_num * levels.num_levels + levels.current;
}

void LevelBegin(level_t *l){
  levels.current = l->luid;
  SetLevelState(CURRENT_LEVEL,LEVEL_START);
}

void LevelEnd(level_t *l){
  SetLevelState(l,LEVEL_COMPLETE);
  if(l->luid+1 >= ROOM_LEVEL_COUNT){
    levels.round_num++;
    GenerateLevels(ROOM_LEVEL_COUNT, true);
    return;
  }

  LevelBegin(levels.levels[l->luid+1]);
}

void InitLevelEvents(){
  levels.num_levels = ROOM_LEVEL_COUNT;
  for (int i = 0; i<levels.num_levels; i++){
    levels.levels[i] = &level_data[i];
    SetLevelState(levels.levels[i],LEVEL_LOADING);
  }

  LevelBegin(CURRENT_LEVEL);
}

void LevelLoadWave(unsigned int index){
  if(index < CURRENT_LEVEL->num_spawners)
    SetObjectState(CURRENT_LEVEL->mob_spawners[index],OBJECT_LOAD);
  else{
    //begin level end 
  }
}

void LevelStep(){
  bool end = true;
  switch(CURRENT_LEVEL->state){
    case LEVEL_RUNNING:
      for(int i = 0; i< CURRENT_LEVEL->num_spawners; i++){
        StepObject(CURRENT_LEVEL->mob_spawners[i]);
        if(CURRENT_LEVEL->spawns->num_references > 0)
          end = false;
      }
      break;
    default:
      end = false;
      break;
  }
  if(end){
  ent_t* any[MAX_ENTS];
  if(WorldGetEnts(any,FilterEntNotOnTeam,player)==0)
    SetLevelState(CURRENT_LEVEL,LEVEL_FINISH);
  }
}

entity_pool_t* InitEntityPool(void) {
    entity_pool_t* pool = malloc(sizeof(entity_pool_t));
    pool->num_references = 0;
    return pool;
}

void InitLevel(level_t *l){
  l->current_spawner = 0;
  l->num_spawners = 0;
  for (int i = 0; i < ROOM_LEVEL_WAVE_COUNT; i++) {
    if(room_spawners[i].level != l->luid)
      continue;
    if(l->state == LEVEL_NONE)
      memcpy(l->event_durations, event_durations, sizeof(l->event_durations));
    
    int idx = l->num_spawners;
    l->spawns[idx] = *InitEntityPool(); // or store pointers too
    l->mob_spawners[idx] = InitObjectStatic(room_spawners[i]);
    l->num_spawners++;
  }

}

void GenerateLevels(int num_levels, bool inc_diff){
  //StatChangeValue(NULL,&event_durations[EVENT_WAVE].dur,-6);
  for (int i = 0; i < num_levels; i++){
    //levels.levels[i]->difficulty++;
    InitLevel(levels.levels[i]);
    for (int j = 0; j < MOD_DONE; j++){
      if(levels.levels[i]->modifiers[j]==0)
        continue;

      for (int k = 0; k < levels.levels[i]->modifiers[j]; k++){
        int r = GetRandomValue(0,level_mods[j].denom);
        if(r > levels.levels[i]->difficulty)
          continue;
        level_mods[j].level_id = levels.levels[i]->luid;
        level_mods[j].modFn(&level_mods[j]);
        TraceLog(LOG_INFO,"Modify level %d with mod %d",i,j);
      }
    }

    TraceLog(LOG_INFO,"Reset Level %d diffuclty %0.1f",i,levels.levels[i]->difficulty);
  }

  LevelBegin(levels.levels[0]);
}

void RegisterPoolRef(unsigned int level_index,unsigned int index, EntityType ref){
  int num_refs = levels.levels[level_index]->spawns[index].num_references;
  levels.levels[level_index]->spawns[index].reference_ents[num_refs] = ref;
  levels.levels[level_index]->spawns[index].num_references++;
}

bool SpawnEnt(game_object_t* spawner){

  int ref_ent = ENT_MOB;
  for (int i = ENT_MOB; i < ENT_BLANK; i++){
    if(CURRENT_LEVEL->spawns[spawner->id].num_references == 0)
      continue;
    CURRENT_LEVEL->spawns[spawner->id].num_references--;
    ref_ent = CURRENT_LEVEL->spawns[spawner->id].reference_ents[i];
    break;
  }

  if (ref_ent == ENT_MOB)
    return false;

  ent_t* spawn = InitEnt(room_instances[ref_ent]);
  spawn->body->position = spawner->pos;
  RegisterEnt(spawn);
  SetState(spawn,STATE_SPAWN,NULL);
  return true;
}

bool ModifyWaveInterval(difficulty_modifier_t* self){
  level_t *l = levels.levels[self->level_id];
  return StatChangeValue(NULL,&l->event_durations[EVENT_WAVE].dur,-6);
}

bool ModifyMobUpgrade(difficulty_modifier_t* self){

}

bool ModifyLevelDifficulty(difficulty_modifier_t* self){
  levels.levels[self->level_id]->difficulty+=self->amount;

  return true;
}

bool ModifyLevelPoints(difficulty_modifier_t* self){
  levels.levels[self->level_id]->points+=self->amount;

  return true;
}
