#ifndef ROOM_DATA_H
#define ROOM_DATA_H

#define ROOM_WIDTH 1600
#define ROOM_HEIGHT 900

#define GRID_WIDTH 20
#define GRID_HEIGHT 12

#define CELL_WIDTH 80
#define CELL_HEIGHT 80

typedef struct {
  const char* engine_instance;
  const char* name;
  float x;
  float y;
  int sprite_sheet_index;
  int speed;
  int accel;
  int	aggro_range;
  bool	is_projectile;
  int	num_attacks;
  char* attacks;
  int	team_enum;
} ObjectInstance;

typedef struct {
  const char* engine_instance;
  const char* name;
  int sprite_sheet_index;
  int duration;
  int fire_rate;
  int speed;
  int	attack_range;
  //bool	(*fn)(void* params);
} ProjectileInstance;

static const ObjectInstance room_instances[] = {
  {"ent_data", "Drone", 1248, 128, 37, 4, 4, 600, 0,1,"basic_bullet",1},
  {"ent_data", "player", 416, 288, 0, 4, 0, 450, 0,1,"basic_bullet",0},
};

#define ROOM_INSTANCE_COUNT 2

static const ProjectileInstance room_projectiles[] = {
  {"projectile_data", "basic_bullet",41,0,9,32,620 }
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
