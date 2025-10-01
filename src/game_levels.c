#include "game_process.h"
#include "game_tools.h"

float total_upgrades = 3.66;

difficulty_modifier_t level_mods[MOD_DONE] = {
  {.type = MOD_NONE},
  {.type = MOD_LEVEL_DIFF, .denom = 1,.amount = 0.99f, .modFn = ModifyLevelDifficulty},
  {.type = MOD_LEVEL_POINTS, .denom = 250,.amount = 1, .modFn = ModifyLevelPoints},
  {.type = MOD_MOB_UPGRADE, .denom = 5,.amount = 1, .modFn = ModifyMobUpgrade},
  {.type = MOD_MOB_COUNT, .denom = 80,.amount = 1, .modFn = ModifyMobCount},
  {.type = MOD_WAVE_INTERVAL, .denom = 50, .amount = -6, .modFn = ModifyWaveInterval},
};

level_info_t level_info[]={
  {DEFINE_LVL_NONE,"Title"},
  {DEFINE_LVL_TUT,"Tut"},
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

level_t* GetLevel(unsigned int index){
  if(index < 4)
    return levels.levels[index];

  return NULL;
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
  levels.level_info = level_info[1];
  levels.album_id = AudioBuildMusicTracks(levels.level_info.subdir);
  for (int i = 0; i<levels.num_levels; i++){
    levels.levels[i] = &level_data[i];
    SetLevelState(levels.levels[i],LEVEL_LOADING);
  }

  AudioPlayMusic(levels.album_id);
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
  switch(CURRENT_LEVEL->state){
    case LEVEL_RUNNING:
      for(int i = 0; i< CURRENT_LEVEL->num_spawners; i++)
        StepObject(CURRENT_LEVEL->mob_spawners[i]);
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
  for(int i = 0; i<levels.num_levels;i++){
    level_t *l = levels.levels[i];
    for(int j = 0; j < l->num_spawners; j++){
      FreeObject(l->mob_spawners[j]);
    }  
  }
  levels = (level_order_t){0};
}

void InitLevel(level_t *l){
  l->current_spawner = 0;
  l->num_spawners = 0;
  l->spawner_done = 0u;
  for (int i = 0; i < ROOM_LEVEL_WAVE_COUNT; i++) {
    if(room_spawners[i].level != l->luid)
      continue;
    
    memcpy(l->event_durations, event_durations, sizeof(l->event_durations));
    int idx = l->num_spawners;
    l->spawns[idx] = *InitEntityPool(); // or store pointers too
    l->mob_spawners[idx] = InitObjectStatic(room_spawners[i]);
    l->mob_spawners[idx]->level_id =room_spawners[i].level;
    
    l->num_spawners++;
  }

}

void ResetLevel(level_t* l){
  l->current_spawner = 
  l->spawner_done = 0u;
  for (int i = 0; i < l->num_spawners; i++) {
    l->spawns[i].current_ref = 0;
    SetObjectState(l->mob_spawners[i],OBJECT_LOAD);
  }
}

void GenerateLevels(int num_levels, bool inc_diff){
  int num_upgrades = (int) total_upgrades;
  for (int i = num_levels-1; i > -1; i--){
    ResetLevel(levels.levels[i]);
    if(num_upgrades < 0)
      continue;

    for (int j = 0; j < MOD_DONE; j++){
      if(levels.levels[i]->modifiers[j]==0)
        continue;

      for (int k = 0; k < levels.levels[i]->modifiers[j]; k++){
        int r = GetRandomValue(0,level_mods[j].denom);
        if(r > levels.levels[i]->difficulty){
          total_upgrades += levels.levels[i]->difficulty;
          continue;
        }
        level_mods[j].level_id = levels.levels[i]->luid;
        if(level_mods[j].modFn(&level_mods[j])){
          num_upgrades--;
        }
      }
    }

  }

  LevelBegin(levels.levels[0]);
}

void RegisterPoolRef(unsigned int level_index,unsigned int index, EntityType ref){
  int num_refs = levels.levels[level_index]->spawns[index].num_references;
  levels.levels[level_index]->spawns[index].reference_ents[num_refs] = ref;
  levels.levels[level_index]->spawns[index].num_references++;
}

bool SpawnEnt(game_object_t* spawner){
  if(spawner->state != OBJECT_RUN)
    return false;

  int cur = CURRENT_LEVEL->spawns[spawner->id].current_ref;
  int ref_ent = CURRENT_LEVEL->spawns[spawner->id].reference_ents[cur];

  if (ref_ent == ENT_MOB)
    return false;

  CURRENT_LEVEL->spawns[spawner->id].current_ref++;
  ent_t* spawn = InitEnt(room_instances[ref_ent]);
  spawn->body->position = spawner->pos;
  RegisterEnt(spawn);
  SetState(spawn,STATE_SPAWN,NULL);
  return true;
}

bool ModifyWaveInterval(difficulty_modifier_t* self){
  level_t *l = levels.levels[self->level_id];
  TraceLog(LOG_INFO,"[MOD]====level %d spawn rate now %d===[MOD]",self->level_id, l->event_durations[EVENT_WAVE].dur.current + self->amount);
  return StatChangeValue(NULL,&l->event_durations[EVENT_WAVE].dur,self->amount);
}

bool ModifyMobUpgrade(difficulty_modifier_t* self){
  level_t *l = levels.levels[self->level_id];
  for (int i = l->num_spawners-1; i >-1; i--){
    entity_pool_t *p = &l->spawns[i];

    for (int j = p->num_references-1; j > -1; j--){
      if(upgrade_next[p->reference_ents[j]] == ENT_BLANK)
        continue;

      l->spawns[i].reference_ents[j] = upgrade_next[p->reference_ents[j]];
      TraceLog(LOG_INFO,"[MOD]====level %d mob upgraded===[MOD]",self->level_id);
      return true;
    }
  }

  return false;
}

bool ModifyLevelDifficulty(difficulty_modifier_t* self){
  levels.levels[self->level_id]->difficulty+=self->amount;
  TraceLog(LOG_INFO,"[MOD]====level %d difficulty now %0.1f===[MOD]",self->level_id, levels.levels[self->level_id]->difficulty);
  return true;
}

bool ModifyLevelPoints(difficulty_modifier_t* self){
  levels.levels[self->level_id]->points+=self->amount;
  TraceLog(LOG_INFO,"[MOD]====level %d points now %0.0f ===[MOD]",self->level_id, levels.levels[self->level_id]->points);
  return true;
}

bool ModifyMobCount(difficulty_modifier_t* self){
 level_t *l = levels.levels[self->level_id];
  for (int i = l->num_spawners-1; i >-1; i--){
    entity_pool_t *p = &l->spawns[i];

    if(p->num_references >= MAX_SPAWNS)
      continue;

    p->reference_ents[p->num_references++] = p->reference_ents[0];
    self->denom += p->num_references;
    TraceLog(LOG_INFO,"[MOD]====level %d mob count now %d===[MOD]",self->level_id, p->num_references );
    return true;
  }

  return false;
}

void LevelSyncSpawners(unsigned int level_index, unsigned int spawner_index){
  levels.levels[level_index]->spawner_done |= (1u << spawner_index);

    if (levels.levels[level_index]->spawner_done == (1u << levels.levels[level_index]->num_spawners) - 1) {
        SetLevelState(levels.levels[level_index], LEVEL_FINISH);
    }
}
