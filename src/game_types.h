#ifndef __GAME_TYPES__
#define __GAME_TYPES__

#include "game_common.h"
#include "game_assets.h"
#include "game_utils.h"
#include "room_data.h"
#include "raylib.h"
#include "game_math.h"

#define GRAVITY 0.65f
#define MAX_VELOCITY  64.0f
#define TERMINAL_VELOCITY 7.0f
#define GRID_SIZE 128
#define GRID_STEP 32

#define MAX_ENTS 128  
#define MAX_SPAWNS 8  
#define MAX_ATTACKS 8

#define RATIO(s) ((s).ratio(&(s)))
typedef struct ent_s ent_t;

typedef struct bounds_s {
  Vector2     pos,offset;
  float       radius;
  float       width,height;
  //BoundsCheckCall col_check;
} bounds_t;

typedef struct{
  ent_t*                  target;
  Vector2                 destination;
  bool                    has_arrived;
  behavior_tree_node_t*   bt[STATE_END];
}controller_t;
controller_t* InitController();

//===ENT_T===>
typedef struct ent_s{
  int                   uid;
  EntityType            type;
  Vector2               pos;
  EntityState           state;
  controller_t          *control;
  events_t              *events;
  sprite_t              *sprite;
  struct ent_s          *child;
  struct ent_s          *owner;
} ent_t;

ent_t* InitEnt(ObjectInstance data);
ent_t* InitEntStatic(TileInstance data,Vector2 pos);

void EntSync(ent_t* e);
bool EntKill(ent_t* e);
void EntDestroy(ent_t* e);
bool FreeEnt(ent_t* e);
bool EntReload(ent_t* e);
void EntAddPoints(ent_t* e,EntityState old, EntityState s);
void EntPrepStep(ent_t *e);
void EntControlStep(ent_t *e);
typedef void (*StateChangeCallback)(ent_t *e, EntityState old, EntityState s);
bool CheckEntAvailable(ent_t* e);
bool CheckEntOutOfBounds(ent_t* e, Rectangle bounds);
bool SetState(ent_t *e, EntityState s,StateChangeCallback callback);
void StepState(ent_t *e);
void OnStateChange(ent_t *e, EntityState old, EntityState s);
bool CanChangeState(EntityState old, EntityState s);

typedef struct {
  ShapeID   reference_ents[MAX_SPAWNS];
  unsigned int current_ref;
  int          num_references;
}entity_pool_t;

entity_pool_t* InitEntityPool();
//===ENT_T==>
void RegisterPoolRef(unsigned int level_index, unsigned int index, ShapeID ref);

#endif
