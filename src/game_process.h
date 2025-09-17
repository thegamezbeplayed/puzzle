#ifndef __GAME_PROCESS__
#define __GAME_PROCESS__

#include "game_types.h"
#include "game_common.h"
#include "screens.h"
#include "game_projectiles.h"

#define MAX_INTERACTIONS 256
#define MAX_SPAWNERS 16
#define DEBUG false
#define CURRENT_LEVEL levels.levels[levels.current]
extern Font font;
extern ent_t* player;

typedef void (*UpdateFn)(void);
typedef bool EntFilterFn(ent_t* e, ent_t* other); 
static bool FilterEntNotOnTeam(ent_t* e,ent_t* other){
  if(e->team == other->team || e->team == TEAM_ENVIROMENT)
    return false;
  else
    return true;
}

static bool FilterEntTargetable(ent_t* e, ent_t* other){
    if(e->team == other->team || e->team == TEAM_ENVIROMENT || e->state >= STATE_DIE || !e->sprite->is_visible || e->body->is_kinematic)
    return false;
  else
    return true;
}

static bool FilterEntNotOnTeamAlive(ent_t* e,ent_t* other){
  if(e->team == other->team || e->team == TEAM_ENVIROMENT || e->state >= STATE_DIE)
    return false;
  else
    return true;
}

//INTERACTIONS_T===>
typedef struct {
  int             source_uid; //uid of who interacted (body, ent)
  int             target_uid; //uid of interactee (body, ent)
  cooldown_t*     timer;
} interaction_t;

int InitInteractions();
interaction_t* EntInteraction(unsigned int source, unsigned int target, int duration);
bool AddInteraction(interaction_t* inter);
bool CanInteract(int source, int target);
int GetInteractions(int source);
void FreeInteraction(interaction_t* item);
void FreeInteractionByIndex(int i);
void InteractionStep();
//==INTERACTIONS_T==>
//EVENTS==>

typedef enum{
  PROCESS_NONE,
  PROCESS_LEVEL,
  PROCESS_DONE
}ProcessType;

typedef enum{
  UPDATE_FRAME,//update running at full fps
  UPDATE_DRAW,
  UPDATE_PRE,
  UPDATE_FIXED,
  UPDATE_POST,
  UPDATE_DONE
}UpdateType;

typedef enum{
  GAME_NONE,
  GAME_LOADING,
  GAME_READY,
  GAME_FINISHED,
  GAME_OVER
}GameState;

typedef enum{
  LEVEL_NONE,
  LEVEL_LOADING,
  LEVEL_START,
  LEVEL_RUNNING,
  LEVEL_FINISH,
  LEVEL_COMPLETE,
  LEVEL_DONE
}LevelState;

typedef struct{
  ProcessType process;
  GameState   state[PROCESS_DONE];
  events_t    *events;
  UpdateFn    init[PROCESS_DONE];
  UpdateFn    update_steps[PROCESS_DONE][UPDATE_DONE];
  UpdateFn    finish[PROCESS_DONE];
}child_process_t;

typedef struct{
  GameScreen     screen;
  child_process_t children[SCREEN_DONE];
  GameScreen     next[SCREEN_DONE];
  GameState      state[SCREEN_DONE];//TODO each screen needs a state
  events_t       *events;
  UpdateFn       init[SCREEN_DONE];
  UpdateFn       update_steps[SCREEN_DONE][UPDATE_DONE];
  UpdateFn       finish[SCREEN_DONE];
}game_process_t;

void InitGameEvents();
void InitGameProcess();
void GameProcessStep();
void GameProcessSync(bool wait);
void GameTransitionScreen();
void GameProcessEnd();
//====level process==>
typedef struct{
  unsigned int      luid;
  LevelState        state;
  float             points;
  int               current_spawner;
  int               num_spawners;
  game_object_t*    mob_spawners[MAX_SPAWNERS];
  entity_pool_t     spawns[MAX_SPAWNERS];
}level_t;

typedef struct{
  unsigned int current;
  int          num_levels;
  level_t      *levels[10];
}level_order_t;

void InitLevel();
void LevelAddSpawn(unsigned int index, EntityType ref, int count);
game_object_t* LevelGetSpawner(unsigned int index);
void AddPoints(float mul,float points,Vector2 pos);
const char* GetPoints();
level_t* LevelCurrent();
void InitLevelEvents();
void LevelStep();
void LevelBegin(level_t *l);
void LevelEnd(level_t *l);
void SetLevelState(level_t *l, LevelState state);
void OnLevelStateChange(level_t *l, LevelState state);
bool CanChangeLevelState(LevelState old, LevelState s);

//===WORLD_T===>

typedef struct{
  ObjectInstance  ents[ROOM_INSTANCE_COUNT];
  TileInstance    tiles[ROOM_TILE_COUNT];
  unsigned int    num_ents;
}world_data_t;

typedef struct world_s{
  Rectangle     room_bounds;
  bool          intgrid[GRID_WIDTH][GRID_HEIGHT];
  ent_t*        ents[MAX_ENTS];
  unsigned int  num_ent;
  rigid_body_t* cols[MAX_ENTS];
  unsigned int  num_col;
  sprite_t*     sprs[MAX_ENTS];
  unsigned int  num_spr;
  render_text_t *texts[MAX_EVENTS];
  bool          floatytext_used[MAX_EVENTS];
  int           points;
} world_t;

ent_t* WorldGetEnt(const char* name);
int WorldGetEnts(ent_t** results,EntFilterFn fn, void* params);
Vector2 GetWorldCoordsFromIntGrid(Vector2 pos, float len);
bool RegisterEnt( ent_t *e);
bool RegisterRigidBody(rigid_body_t *b);
bool RegisterSprite(sprite_t *s);
void WorldInitOnce();
void WorldPreUpdate();
void WorldFixedUpdate();
void WorldPostUpdate();
void InitWorld(world_data_t data);
void WorldRender();
Rectangle WorldRoomBounds();

#endif

