#ifndef __GAME_PROCESS__
#define __GAME_PROCESS__

#include "game_types.h"
#include "game_common.h"
#include "screens.h"

#define MAX_INTERACTIONS 256
#define DEBUG false
#define CURRENT_LEVEL levels.levels[levels.current]

extern Font font;
static int fixedFPS = 60;

typedef void (*UpdateFn)(void);
typedef bool EntFilterFn(ent_t* e, ent_t* other); 
bool CheckWorldGridAdjacent(ent_t* e, ent_t* other);

static bool FilterEntShape(ent_t* e,ent_t* other){
  if(e->type == ENT_SHAPE)
    return true;
  else
    return false;
}

static bool FilterEntNeighbor(ent_t* e,ent_t* other){
  if(e->type!= other->type)
    return false;

  if(CheckWorldGridAdjacent(e, other))
    return true;

  return false;
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
void FreeInteractions();
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
  GAME_PAUSE,
  GAME_FINISHED,
  GAME_OVER
}GameState;

void GameSetState(GameState state);

typedef enum{
  LEVEL_NONE,
  LEVEL_LOADING,
  LEVEL_RELOADING,
  LEVEL_START,
  LEVEL_RUNNING,
  LEVEL_FINISH,
  LEVEL_COMPLETE,
  LEVEL_DONE
}LevelState;

typedef enum{
  DEFINE_LVL_NONE,
  DEFINE_LVL_TUT,
  DEFINE_LVL_DEMO,
  DEFINE_LVL_DONE
}LevelDefine;

typedef struct{
  LevelDefine   define;
  const char*   subdir;
}level_info_t;

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
  int            game_frames;
  child_process_t children[SCREEN_DONE];
  GameScreen     next[SCREEN_DONE];
  GameState      state[SCREEN_DONE];//TODO each screen needs a state
  events_t       *events;
  int            album_id[SCREEN_DONE];
  UpdateFn       init[SCREEN_DONE];
  UpdateFn       update_steps[SCREEN_DONE][UPDATE_DONE];
  UpdateFn       finish[SCREEN_DONE];
}game_process_t;

void InitGameEvents();
void InitGameProcess();
void GameProcessStep();
void GameProcessSync(bool wait);
bool GameTransitionScreen();
void GameProcessEnd();

void AddPoints(float mul,float points,Vector2 pos);
int GetPointsInt();
const char* GetPoints();
const char* GetGameTime();
//===WORLD_T===>

typedef struct{
  ObjectInstance  ents[SHAPE_DONE];
  unsigned int    num_ents;
}world_data_t;

typedef struct{
  ent_t*      tile;
  ShapeFlags  bonus_color,bonus_type;
  float       color_mul,type_mul;
}grid_combo_t;

typedef struct{
  grid_combo_t* combos[GRID_WIDTH][GRID_HEIGHT];
}grid_manager_t;

int GridCompare(ent_t* start, int num_others,ent_t** others, ent_t** results);
int GridGetRow(int row, ent_t** out);
int GridGetCol(int col, ent_t** out);

typedef struct world_s{
  grid_manager_t grid;
  Rectangle     play_area;
  ent_t*        ents[MAX_ENTS];
  unsigned int  num_ent;
  sprite_t*     sprs[MAX_ENTS];
  unsigned int  num_spr;
  render_text_t *texts[MAX_EVENTS];
  bool          floatytext_used[MAX_EVENTS];
  int           points;
} world_t;

ent_t* WorldGetEnt(const char* name);
ent_t* WorldGetEntById(unsigned int uid);
int WorldGetEnts(ent_t** results,EntFilterFn fn, void* params);
void WorldCheckGrid(ent_t *e, ent_t* owner);
bool RegisterEnt( ent_t *e);
bool RegisterSprite(sprite_t *s);
void WorldInitOnce();
void WorldPreUpdate();
void WorldFixedUpdate();
void WorldPostUpdate();
void InitWorld(world_data_t data);
void WorldRender();
Rectangle WorldRoomBounds();

ObjectInstance GetObjectInstanceByShapeID(ShapeID id);
#endif

