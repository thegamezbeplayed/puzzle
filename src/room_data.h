#ifndef ROOM_DATA_H
#define ROOM_DATA_H

#define ROOM_WIDTH 1920
#define ROOM_HEIGHT 1080

#define GRID_WIDTH 30
#define GRID_HEIGHT 17

#define CELL_WIDTH 64
#define CELL_HEIGHT 64

#define ROOM_LEVEL_WAVE_COUNT 12
#define ROOM_LEVEL_COUNT 3
/*
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
*/
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
  int   spawn_refs[ENT_BLANK];
  float chance;
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
  int           num_children;
  const char*   children[5];
} BehaviorData;

static const BehaviorData room_behaviors[] = {
  {"idle_state",false,BT_LEAF,LeafChangeState,EVENT_NONE,STATE_IDLE,OBJECT_NONE,0,0,{}},
  {"change_state",false,BT_LEAF,LeafChangeState,EVENT_NONE,STATE_AGGRO,OBJECT_NONE,0,0,{}},
  {"attack_state",false,BT_LEAF,LeafChangeState,EVENT_NONE,STATE_ATTACK,OBJECT_NONE,0,0,{}},
  {"change_state_wander",false,BT_LEAF,LeafChangeState,EVENT_NONE,STATE_WANDER,OBJECT_NONE,0,0,{}},
  {"acquire_mouse",false,BT_LEAF,LeafAcquireMousePosition,EVENT_NONE,STATE_NONE,OBJECT_NONE,0,0,{}},
  {"bisect_dest",false,BT_LEAF,LeafBisectDestination,EVENT_NONE,STATE_NONE,OBJECT_NONE,0,0,{}},
  {"acquire_target",false,BT_LEAF,LeafAcquireTarget,EVENT_NONE,STATE_NONE,OBJECT_NONE,0,0,{}},
  {"can_attack",false,BT_LEAF,LeafCanAttackTarget,EVENT_NONE,STATE_NONE,OBJECT_NONE,0,0,{}},
  {"attack",false,BT_LEAF,LeafAttackTarget,EVENT_NONE,STATE_NONE,OBJECT_NONE,0,0,{}},
  {"acquire_destination",false,BT_LEAF,LeafAcquireDestination,EVENT_NONE,STATE_NONE,OBJECT_NONE,0,0,{}},
  {"acquire_trajectory",false,BT_LEAF,LeafAcquireTrajectory,EVENT_NONE,STATE_NONE,OBJECT_NONE,0,0,{}},
  {"move_to_dest",false,BT_LEAF,LeafMoveToDestination,EVENT_NONE,STATE_NONE,OBJECT_NONE,0,0,{}},
  {"move_trajectory",false,BT_LEAF,LeafMoveAtTrajectory,EVENT_NONE,STATE_NONE,OBJECT_NONE,0,0,{}},
  {"move_to_target",false,BT_LEAF,LeafMoveToTarget,EVENT_NONE,STATE_NONE,OBJECT_NONE,0,0,{}},
  {"make_kinematic",false,BT_LEAF,LeafMakeKinematic,EVENT_NONE,STATE_NONE,OBJECT_NONE,0,0,{}},
  {"wander_state",false,BT_LEAF,LeafChangeState,EVENT_NONE,STATE_WANDER,OBJECT_NONE,0,0,{}},
  {"Orbit",true,BT_SEQUENCE, NULL,EVENT_NONE,STATE_NONE,OBJECT_NONE,0,2,{"acquire_mouse","bisect_dest"}},
  {"Acquire",false, BT_SEQUENCE, NULL,EVENT_NONE,STATE_NONE,OBJECT_NONE,0,2,{"acquire_target","change_state"}},
  {"Seek",true, BT_SELECTOR, NULL,EVENT_NONE,STATE_NONE,OBJECT_NONE,0,2,{"Acquire","Wander"}},
  {"EndSeek",false,BT_SEQUENCE, NULL,0,0,0,0,1,{"wander_state"}},
  {"CheckAggro",false,BT_SEQUENCE, NULL,0,0,0,0,2,{"acquire_target","change_state_idle"}},
  {"Move",false,BT_SEQUENCE, NULL,0,0,0,0,2,{"acquire_destination","move_to_dest"}},
  {"MoveTar",false,BT_SEQUENCE, NULL,0,0,0,0,2,{"acquire_target","move_to_target"}},
  {"NoTar",false,BT_SEQUENCE, NULL,0,0,0,0,1,{"idle_state"}},
  {"TryAttack",false,BT_SEQUENCE, NULL,0,0,0,0,2,{"acquire_target","attack_state"}},
  {"Wander",true,BT_SELECTOR, NULL,0,0,0,0,2,{"CheckAggro","Move"}},
  {"Chase",true,BT_SELECTOR, NULL,0,0,0,0,3,{"TryAttack","MoveTar","NoTar"}},
  {"Attack",false,BT_SEQUENCE, NULL,0,0,0,0,2,{"can_attack","attack"}},
  {"Traject",true,BT_SEQUENCE, NULL,0,0,0,0,3,{"acquire_trajectory","move_trajectory","change_state"}},
  {"Kinematic",true,BT_SEQUENCE, NULL,0,0,0,0,2,{"make_kinematic","change_state_wander"}},
  {"AttackOnTheMove",true,BT_CONCURRENT, NULL,0,0,0,0,2,{"Attack","MoveTar"}},
};
#define ROOM_BEHAVIOR_COUNT 31

static const SpawnerInstance room_spawners[] = {
  {0, 0,"spawn_data", 164,856,{[ENT_DRONE]=3,[ENT_STRIKER]=1},1.0f},
  {1, 0,"spawn_data", 164,164, {[ENT_DRONE]=4},1.0f},
  {2, 0,"spawn_data", 1534,856, {[ENT_DRONE]=4},1.0f},
  {0, 1,"spawn_data", 164,164, {[ENT_DRONE]=4},1.0f},
  {1, 1,"spawn_data", 1534,164,{[ENT_DRONE]=4},1.0f},
  {2, 1,"spawn_data", 950,164, {[ENT_DRONE]=4},1.0f},
  {3, 1,"spawn_data", 164,856, {[ENT_DRONE]=4},1.0f},
  {0, 2,"spawn_data", 164,164, {[ENT_DRONE]=5},1.0f},
  {1, 2,"spawn_data", 1534,856,{[ENT_HUNTER]=3},1.0f},
  {2, 2,"spawn_data", 164,164,{[ENT_DRONE]=4},1.0f},
  {3, 2,"spawn_data", 1534,164,{[ENT_DRONE]=4},1.0f},
  {4, 2,"spawn_data", 950,164, {[ENT_STRIKER]=3,[ENT_DRONE]=1},1.0f}
};

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
  bool shaders[SHADER_DONE];
} ProjectileInstance;

typedef struct{
  AttackType type;
  int        ammo;
  int        range;
  int        rate;
  int        reload;
  int        base_damage;
  float      damage_mod;
  float      speed;
  float      accel;
  EntityType bul;
}AttackData;

static const AttackData attack_data[]={
  {ATTACK_RANGED,5,RANGE_FAR,INTERVAL_FAST,INTERVAL_SLOWER,1,1,0,0,ENT_BULLET},
  {ATTACK_THORNS,1,RANGE_COL,INTERVAL_IMEDIATE,INTERVAL_NONE,1,0,0,0,ENT_BLANK},
  {ATTACK_MELEE,1,RANGE_CLOSE,INTERVAL_SLOWER,INTERVAL_SLOWER,1,1,11.4f,5.4f,ENT_BLANK},
  {ATTACK_RANGED,6,RANGE_AVERAGE,INTERVAL_FAST,INTERVAL_SLOWEST,1,1,0,0,ENT_BULLET},
  {ATTACK_BLANK,0,0,0,0,0,0,0,0,ENT_BLANK}
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
  AttackData attacks[ATTACK_BLANK];
  Color color;
  int	team_enum;
  bool shaders[SHADER_DONE];
} ObjectInstance;

static const ObjectInstance room_instances[] = {
  {ENT_PLAYER,"ent_data", "player", 72,416, 288, 0,0, 200,15, 0, 0,450,{},WHITE,0,{[SHADER_INVERT] = true,[SHADER_OUTLINE]=true}},
  {ENT_SHIELD,"ent_data", "shield", 64,1664, 608, 50,0,1, 0, 0, 0,450,{},WHITE,0,{[SHADER_BLOOM] = true}},
  {ENT_MOB,"ent_data", "Drone", 56, -1, -1, 37,2,24, 16, 4, 3,680,{},PINK,1},
  {ENT_SWARMER,"ent_data", "Drone", 42,-1, -1, 37,1,9, 8, 3.1f, 5,680,{[ATTACK_RANGED]=attack_data[3],[ATTACK_THORNS]=attack_data[1],[ATTACK_MELEE]=attack_data[4]},PINK,1,{[SHADER_INVERT] = true,[SHADER_OUTLINE]=true}},
  {ENT_DRONE,"ent_data", "Drone", 48,1, -1, 5,2,11, 8, 3.1f, 6,680,{[ATTACK_RANGED]=attack_data[3],[ATTACK_THORNS]=attack_data[1],[ATTACK_MELEE]=attack_data[4]},PINK,1,{[SHADER_INVERT] = true,[SHADER_OUTLINE]=true}},
  {ENT_SUPER_DRONE,"ent_data", "Super Drone", 60,-1, -1, 4,3,23, 8, 3.1f, 8,680,{[ATTACK_RANGED]=attack_data[0],[ATTACK_THORNS]=attack_data[1]},PINK,1,{[SHADER_INVERT] = true,[SHADER_OUTLINE]=true}},
  {ENT_BATTLE_DRONE,"ent_data", "Battle Drone", 60,-1, -1,6, 4,36, 8, 3.1f, 5,680,{[ATTACK_RANGED]=attack_data[0],[ATTACK_THORNS]=attack_data[1]},PINK,1,{[SHADER_INVERT] = true,[SHADER_OUTLINE]=true}},
  {ENT_HUNTER,"ent_data", "Hunter", 56,-1, -1, 39,3,40, 7.1, 2.4f, 9,600,{[ATTACK_RANGED]=attack_data[0],[ATTACK_THORNS]=attack_data[4],[ATTACK_MELEE]=attack_data[2]},RED,1,{[SHADER_INVERT] = true,[SHADER_OUTLINE]=true}},
  {ENT_STRIKER,"ent_data", "Striker", 48,-1, -1, 42,2,20, 8.2, 3.2f, 5,600,{[ATTACK_RANGED]=attack_data[4],[ATTACK_THORNS]=attack_data[1],[ATTACK_MELEE]=attack_data[2]},YELLOW,1,{[SHADER_INVERT] = true,[SHADER_OUTLINE]=true}},
  {ENT_OBSTACLE,"ent_data", "obstacle", 56, -1, -1, 37,2,24, 16, 4, 3,680,{},PINK,1},
  {ENT_ASTEROID,"ent_data", "Asteroid", 56, -1, -1, 19,2,24, 16, 4, 3,680,{[ATTACK_MELEE]=attack_data[4],[ATTACK_RANGED]=attack_data[4],[ATTACK_THORNS]=attack_data[1]},GREEN,2,{[SHADER_INVERT] = true,[SHADER_OUTLINE]=true}},
};

#define ROOM_INSTANCE_COUNT 3

static const ProjectileInstance room_projectiles[] = {
  {ENT_BULLET,"projectile_data", "basic_bullet", 16,2, 0,18,25,650,1,{[SHADER_BLOOM]=true,[SHADER_BLUR]=true}},
};

#define ROOM_PROJECTILE_COUNT 1

#endif // ROOM_DATA_H
       // Tile Data
typedef struct {
  int     tile_index;
  Vector2 start,map,inc;
  bool    shaders[SHADER_DONE];
  int     rot;
  int     amount;
} TileInstance;

static const TileInstance room_tiles[] = {
  {51,(Vector2){64,130},(Vector2){0,0},(Vector2){128,0},{[SHADER_INVERT]=true,[SHADER_OUTLINE]=true},90,15},
  {51,(Vector2){64,1064},(Vector2){0,0},(Vector2){128,0},{[SHADER_INVERT]=true,[SHADER_OUTLINE]=true},90,15},
  {51,(Vector2){16,214},(Vector2){0,0},(Vector2){0,128},{[SHADER_INVERT]=true,[SHADER_OUTLINE]=true},180,7},
  {51,(Vector2){1904,214},(Vector2){0,0},(Vector2){0,128},{[SHADER_INVERT]=true,[SHADER_OUTLINE]=true},180,7},
};
#define ROOM_TILE_COUNT 4
