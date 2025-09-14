#ifndef ROOM_DATA_H
#define ROOM_DATA_H

#define ROOM_WIDTH 1600
#define ROOM_HEIGHT 900

#define GRID_WIDTH 20
#define GRID_HEIGHT 12

#define CELL_WIDTH 80
#define CELL_HEIGHT 80

#define ROOM_LEVEL_WAVE_COUNT 6

typedef enum {
  ENT_PLAYER,
  ENT_SHIELD,
  ENT_DRONE,
  ENT_BULLET,
  ENT_BLANK
}EntityType;

typedef struct {
  unsigned char id;
  const char* engine_instance;
  float x;
  float y;
  int   num_spawns;
  int   spawn_refs[ENT_BLANK];
  int   spawn_rate;
} SpawnerInstance;

static const SpawnerInstance room_spawners[] = {
  {0,"spawn_data", 64,64, 4, {[ENT_DRONE]=4},54},
  {1,"spawn_data", 64,64, 4, {[ENT_DRONE]=4},54},
  {2,"spawn_data", 64,64, 4, {[ENT_DRONE]=4},54},
  {3,"spawn_data", 64,64, 4, {[ENT_DRONE]=4},54},
  {4,"spawn_data", 1534,64, 4, {[ENT_DRONE]=4},54},
  {5,"spawn_data", 1534,64, 4, {[ENT_DRONE]=4},54}
};


typedef struct {
  EntityType  ent_ref;
  const char* engine_instance;
  const char* name;
  int	size;
  float x;
  float y;
  int sprite_sheet_index;
  int health;
  int speed;
  int accel;
  int	aggro_range;
  bool	is_projectile;
  int	num_attacks;
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
} ProjectileInstance;

static const ObjectInstance room_instances[] = {
  {ENT_PLAYER,"ent_data", "player", 72,416, 288, 0, 200,16, 0, 450, 0,1,"basic_bullet",0},
  {ENT_SHIELD,"ent_data", "shield", 64,1664, 608, 43,1, 0, 0, 450, 0,0,"",0},
  {ENT_DRONE,"ent_data", "Drone", 56,1248, 128, 37,24, 16, 4, 680, 0,1,"basic_bullet",1},
};

#define ROOM_INSTANCE_COUNT 3

static const ProjectileInstance room_projectiles[] = {
  {ENT_BULLET,"projectile_data", "basic_bullet", 16,41, 0,12,26,650},
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
