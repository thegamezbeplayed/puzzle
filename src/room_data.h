#ifndef ROOM_DATA_H
#define ROOM_DATA_H

#define ROOM_WIDTH 1920
#define ROOM_HEIGHT 1080

#define GRID_WIDTH 20
#define GRID_HEIGHT 12

#define CELL_WIDTH 80
#define CELL_HEIGHT 80

#define ROOM_LEVEL_WAVE_COUNT 12
#define ROOM_LEVEL_COUNT 3
typedef enum {
  ENT_PLAYER,
  ENT_SHIELD,
  ENT_MOB,
  ENT_SWARMER,
  ENT_DRONE,
  ENT_SUPER_DRONE,
  ENT_BATTLE_DRONE,
  ENT_HUNTER,
  ENT_BULLET,
  ENT_BLANK
}EntityType;

static EntityType upgrade_next[ENT_BLANK] = {
  [ENT_MOB]   = ENT_DRONE,
  [ENT_DRONE]   = ENT_SUPER_DRONE,
  [ENT_SUPER_DRONE]   = ENT_BATTLE_DRONE,
  [ENT_BATTLE_DRONE]   = ENT_BLANK,
  [ENT_HUNTER]   = ENT_BATTLE_DRONE,
};

typedef struct {
  unsigned char id;
  unsigned char level;
  const char* engine_instance;
  float x;
  float y;
  int   num_spawns;
  int   spawn_refs[ENT_BLANK];
  int   spawn_rate;
} SpawnerInstance;

typedef struct {
  const char* name;
  bool    is_root;
  BehaviorTreeType  bt_type;
  behavior_tree_node_t *(*func)(behavior_params_t *);
  EventType     event;
  EntityState   state;
  ObjectState   obj_state;
  int           duration;
  //CooldownCallback fn;
  //TODO
  int           num_children;
  const char*   children[5];
} BehaviorData;

static const BehaviorData room_behaviors[] = {
  {"start_spawn",false,BT_LEAF,LeafStartEvent,EVENT_SPAWN,STATE_NONE,OBJECT_NONE,33,0,{}},
  {"start_event",false,BT_LEAF,LeafStartEvent,EVENT_WAVE,STATE_NONE,OBJECT_NONE,180,0,{}},
  {"check_start_event",false,BT_LEAF,LeafCheckEvent,EVENT_WAVE,STATE_NONE,OBJECT_NONE,0,0,{}},
  {"check_spawn_event",false,BT_LEAF,LeafCheckEvent,EVENT_SPAWN,STATE_NONE,OBJECT_NONE,0,0,{}},
  {"run_state",false,BT_LEAF,LeafStartState,EVENT_SPAWN,STATE_NONE,OBJECT_RUN,0,0,{}},
  {"start_state",false,BT_LEAF,LeafStartState,EVENT_SPAWN,STATE_NONE,OBJECT_START,0,0,{}},
  {"idle_state",false,BT_LEAF,LeafStartState,EVENT_SPAWN,STATE_NONE,OBJECT_PAUSE,0,0,{}},
  {"finish_state",false,BT_LEAF,LeafStartState,EVENT_SPAWN,STATE_NONE,OBJECT_FINISH,0,0,{}},
  {"end_state",false,BT_LEAF,LeafStartState,EVENT_SPAWN,STATE_NONE,OBJECT_END,0,0,{}},
  {"end_state",false,BT_LEAF,LeafStartState,EVENT_SPAWN,STATE_NONE,OBJECT_END,0,0,{}},
  {"Load", true,BT_SEQUENCE,NULL,EVENT_NONE,STATE_NONE,OBJECT_NONE,0,3,{"start_event","check_start_event","start_state"}},
  {"spawn_ent",false,BT_LEAF,LeafSpawnEnt,EVENT_SPAWN,STATE_NONE,OBJECT_RUN,0,0,{}},
  {"Run", false,BT_SEQUENCE,NULL,EVENT_NONE,STATE_NONE,OBJECT_NONE,0,2,{"spawn_ent","idle_state"}},
  {"Check", true,BT_SELECTOR,NULL,EVENT_NONE,STATE_NONE,OBJECT_NONE,0,2,{"Run","finish_state"}},
  {"Prep", true,BT_SEQUENCE,NULL,EVENT_NONE,STATE_NONE,OBJECT_NONE,0,3,{"start_spawn","check_spawn_event","run_state"}},
  {"Finish", true,BT_SEQUENCE,NULL,EVENT_NONE,STATE_NONE,OBJECT_NONE,0,3,{"start_event","check_start_event","end_state"}},
};
#define ROOM_BEHAVIOR_COUNT 16

static const SpawnerInstance room_spawners[] = {
  {0, 0,"spawn_data", 64,856, 3, {[ENT_DRONE]=4},74},
  {1, 0,"spawn_data", 64,64, 4, {[ENT_DRONE]=4},54},
  {2, 0,"spawn_data", 1534,856, 4, {[ENT_DRONE]=4},54},
  {0, 1,"spawn_data", 64,64, 4, {[ENT_DRONE]=4},54},
  {1, 1,"spawn_data", 1534,64, 4, {[ENT_DRONE]=4},54},
  {2, 1,"spawn_data", 1534,64, 4, {[ENT_DRONE]=4},54},
  {3, 1,"spawn_data", 64,856, 4, {[ENT_DRONE]=4},74},
  {0, 2,"spawn_data", 64,64, 5, {[ENT_DRONE]=5},54},
  {1, 2,"spawn_data", 1534,856, 3, {[ENT_HUNTER]=3},54},
  {2, 2,"spawn_data", 64,64, 4, {[ENT_DRONE]=4},54},
  {3, 2,"spawn_data", 1534,64, 4, {[ENT_DRONE]=4},54},
  {4, 2,"spawn_data", 1534,64, 5, {[ENT_DRONE]=4},54}
};

typedef struct {
  EntityType  ent_ref;
  const char* engine_instance;
  const char* name;
  int	size;
  float x;
  float y;
  int sprite_sheet_index;
  int points;
  int health;
  float speed;
  float accel;
  int damage;
  int	aggro_range;
  int	num_attacks;
  int reload_rate;
  const char* attacks;
  int	team_enum;
} ObjectInstance;

typedef struct {
  EntityType  ent_ref;
  const char* engine_instance;
  const char* name;
  int	size;
  int sprite_sheet_index;
  int duration;
  int fire_rate;
  int speed;
  int	attack_range;
  int damage;
} ProjectileInstance;

static const ObjectInstance room_instances[] = {
  {ENT_PLAYER,"ent_data", "player", 72,416, 288, 0,0, 200,15, 0, 0,450,1,0,"basic_bullet",0},
  {ENT_SHIELD,"ent_data", "shield", 64,1664, 608, 43,0,1, 0, 0, 0,450,0,0,"",0},
  {ENT_MOB,"ent_data", "Drone", 56,1248, 128, 37,2,24, 16, 4, 3,680, 1,30,"basic_bullet",1},
  {ENT_SWARMER,"ent_data", "Drone", 56,1248, 128, 37,1,24, 8, 3.1f, 5,680, 5,30,"basic_bullet",1},
  {ENT_DRONE,"ent_data", "Drone", 54,1248, 128, 37,1,24, 8, 3.1f, 7,680, 6,27,"basic_bullet",1},
  {ENT_SUPER_DRONE,"ent_data", "Drone", 56,1248, 128, 38,2,32, 8.3, 3.3f, 5,680, 6,27,"basic_bullet",1},
  {ENT_BATTLE_DRONE,"ent_data", "Drone", 64,1248, 128, 36,4,48, 8.3, 2.1f, 9,690, 4,17,"basic_bullet",1},
  {ENT_HUNTER,"ent_data", "Hunter", 64,1248, 128, 7,2,36, 7.6, 2.7f, 9,620, 4,34,"basic_bullet",1},
};

#define ROOM_INSTANCE_COUNT 3

static const ProjectileInstance room_projectiles[] = {
  {ENT_BULLET,"projectile_data", "basic_bullet", 16,41, 0,18,25,650,1},
};

#define ROOM_PROJECTILE_COUNT 1

#endif // ROOM_DATA_H
       // Tile Data
typedef struct {
  int tile_index;
  int start_x;
  int start_y;
  int map_x;
  int map_y;
  int rotation;
  bool flip_x;
  bool flip_y;
} TileInstance;

static const TileInstance room_tiles[] = {
};
#define ROOM_TILE_COUNT 0
