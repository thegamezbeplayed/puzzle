#ifndef ROOM_DATA_H
#define ROOM_DATA_H

#define ROOM_WIDTH 1920
#define ROOM_HEIGHT 1080

#define GRID_WIDTH 30
#define GRID_HEIGHT 17

#define CELL_WIDTH 64
#define CELL_HEIGHT 64

#define ROOM_LEVEL_COUNT 3

typedef struct {
  EntityType  ent_ref;
  const char* name;
  int   size;
  float x;
  float y;
  int sprite_sheet_index;
  int points;
  Color color;
  int   team_enum;
  bool shaders[SHADER_DONE];
} ObjectInstance;
static const ObjectInstance room_instances[] = {};

#define ROOM_INSTANCE_COUNT 3

typedef struct {
  int     tile_index;
  Vector2 start,map,inc;
  bool    shaders[SHADER_DONE];
  int     rot;
  int     amount;
} TileInstance;
static const TileInstance room_tiles[] = {};
#define ROOM_TILE_COUNT 4

typedef struct {
  const char* name;
  bool    is_root;
  BehaviorTreeType  bt_type;
  behavior_tree_node_t *(*func)(behavior_params_t *);
  EventType     event;
  EntityState   state;
  int           duration;
  //CooldownCallback fn;
  int           num_children;
  const char*   children[5];
} BehaviorData;

static const BehaviorData room_behaviors[] = {};
#define ROOM_BEHAVIOR_COUNT 0
#endif
