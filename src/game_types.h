#ifndef __GAME_TYPES__
#define __GAME_TYPES__

#include "game_common.h"
#include "game_assets.h"
#include "game_utils.h"
#include "room_data.h"
#include "raylib.h"
#include "game_math.h"
#include "game_tools.h"

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
  ShapeID               shape;
  EntityType            type;
  Vector2               pos;
  Cell                  intgrid_pos;
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
void EntAddPoints(ent_t* e,EntityState old, EntityState s);
void EntPrepStep(ent_t *e);
void EntSetPos(ent_t *e, Vector2 pos);
void EntControlStep(ent_t *e);
typedef void (*StateChangeCallback)(ent_t *e, EntityState old, EntityState s);
void SetViableTile(ent_t*, EntityState old, EntityState s);
bool CheckEntAvailable(ent_t* e);
bool CheckEntPosition(ent_t* e, Vector2 pos);
bool SetState(ent_t *e, EntityState s,StateChangeCallback callback);
void StepState(ent_t *e);
void OnStateChange(ent_t *e, EntityState old, EntityState s);
bool CanChangeState(EntityState old, EntityState s);

typedef void (*OwnerChangeCallback)(ent_t *e, ent_t* old, ent_t* owner);
void EntOnOwnerChange(ent_t *e, ent_t* old, ent_t* owner);
void EntChangeOccupant(ent_t* e, ent_t* owner);
bool EntSetOwner(ent_t*, ent_t* owner, bool evict,OwnerChangeCallback cb);
#endif
