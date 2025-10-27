#ifndef __GAME_COMMON__
#define __GAME_COMMON__

#include "raylib.h"
#define SHAPE_COUNT 21

typedef enum{
  ENT_NONE,
  ENT_SHAPE,
  ENT_TILE,
  ENT_DONE
}EntityType;

typedef enum{
  BEHAVIOR_NONE,
  BEHAVIOR_CHANGE_STATE,
  BEHAVIOR_SELECT_SHAPE,
  BEHAVIOR_INIT_CHILD,
  BEHAVIOR_ACQUIRE_CHILD,
  BEHAVIOR_COUNT
}BehaviorID;

typedef struct sub_texture_s {
    int tag;
    int originX, originY;
    int positionX, positionY;
    int sourceWidth, sourceHeight;
    int padding;
    bool trimmed;
    int trimRecX, trimRecY, trimRecWidth, trimRecHeight;
    int colliderType;
    int colliderPosX, colliderPosY, colliderSizeX, colliderSizeY;
} sub_texture_t;

typedef enum {
    // Colors (high nibble)
    SHAPE_COLOR_NONE   = 0x00,
    SHAPE_COLOR_GREEN  = 0x10,
    SHAPE_COLOR_GRAY   = 0x20,
    SHAPE_COLOR_BLUE   = 0x30,
    SHAPE_COLOR_RED    = 0x40,
    // ... add more if needed

    SHAPE_COLOR_MASK   = 0xF0,

    // Shapes (low nibble)
    SHAPE_TYPE_NONE     = 0x00,
    SHAPE_TYPE_SQUARE   = 0x01,
    SHAPE_TYPE_DIAMOND  = 0x02,
    SHAPE_TYPE_PENTAGON = 0x03,
    SHAPE_TYPE_STUD     = 0x04,
    SHAPE_TYPE_TRIANGLE = 0x05,
    SHAPE_TYPE_COMPASS  = 0x06,
    SHAPE_TYPE_STAR     = 0x07,
    SHAPE_TYPE_GEM      = 0x08,
    SHAPE_TYPE_CIRCLE   = 0x09,
    SHAPE_TYPE_HEART    = 0x0A,

    SHAPE_TYPE_MASK     = 0x0F,
} ShapeFlags;

typedef enum{
  SHAPE_NONE = SHAPE_COLOR_NONE| SHAPE_TYPE_NONE,
  SHAPE_GREEN_SQUARE = SHAPE_COLOR_GREEN | SHAPE_TYPE_SQUARE,
  SHAPE_GREEN_DIAMOND = SHAPE_COLOR_GREEN | SHAPE_TYPE_DIAMOND,
  SHAPE_GREEN_PENTAGON = SHAPE_COLOR_GREEN | SHAPE_TYPE_PENTAGON,
  SHAPE_GREEN_STUD = SHAPE_COLOR_GREEN | SHAPE_TYPE_STUD,
  SHAPE_GREEN_TRIANGLE = SHAPE_COLOR_GREEN | SHAPE_TYPE_TRIANGLE,
  SHAPE_GREEN_COMPASS = SHAPE_COLOR_GREEN | SHAPE_TYPE_COMPASS,
  SHAPE_GREEN_STAR = SHAPE_COLOR_GREEN | SHAPE_TYPE_STAR,
  SHAPE_GREEN_GEM = SHAPE_COLOR_GREEN | SHAPE_TYPE_GEM,
  SHAPE_GREEN_CIRCLE = SHAPE_COLOR_GREEN | SHAPE_TYPE_CIRCLE,
  SHAPE_GREEN_HEART = SHAPE_COLOR_GREEN | SHAPE_TYPE_HEART,
  SHAPE_GRAY_SQUARE = SHAPE_COLOR_GRAY | SHAPE_TYPE_SQUARE,
  SHAPE_GRAY_DIAMOND = SHAPE_COLOR_GRAY | SHAPE_TYPE_DIAMOND,
  SHAPE_GRAY_PENTAGON = SHAPE_COLOR_GRAY | SHAPE_TYPE_PENTAGON,
  SHAPE_GRAY_STUD = SHAPE_COLOR_GRAY | SHAPE_TYPE_STUD,
  SHAPE_GRAY_TRIANGLE = SHAPE_COLOR_GRAY | SHAPE_TYPE_TRIANGLE,
  SHAPE_GRAY_COMPASS = SHAPE_COLOR_GRAY | SHAPE_TYPE_COMPASS,
  SHAPE_GRAY_STAR = SHAPE_COLOR_GRAY | SHAPE_TYPE_STAR,
  SHAPE_GRAY_GEM = SHAPE_COLOR_GRAY | SHAPE_TYPE_GEM,
  SHAPE_GRAY_CIRCLE = SHAPE_COLOR_GRAY | SHAPE_TYPE_CIRCLE,
  SHAPE_GRAY_HEART = SHAPE_COLOR_GRAY | SHAPE_TYPE_HEART,
  SHAPE_DONE = 0xFF
}ShapeID;

typedef struct{
  const char* text;
  Vector2     pos;
  int         size;
  Color       color;
  int         duration;
}render_text_t;

void AddFloatingText(render_text_t *rt);

struct ent_s;   // forward declaration

//====EVENTS===>
typedef enum{
  EVENT_GAME_PROCESS,
  EVENT_INTERACTION,
  EVENT_PLAY_SFX,
  EVENT_SONG_END,
  EVENT_SONG_FADE_IN,
  EVENT_SONG_FADE_OUT,
  EVENT_WAIT,
  EVENT_FINISH,
  EVENT_NONE,
  MAX_EVENTS
} EventType;

typedef void (*CooldownCallback)(void* params);

typedef struct{
  EventType         type;
  int               duration;
  int               elapsed;
  bool              is_complete;
  bool              is_recycled;
  void*             on_end_params;
  void*             on_step_params;
  CooldownCallback  on_end;
  CooldownCallback  on_step;
}cooldown_t;
cooldown_t* InitCooldown(int dur, EventType type, CooldownCallback on_end_callback, void* params);
void UnloadCooldown(cooldown_t* cd);

typedef struct{
  cooldown_t  cooldowns[MAX_EVENTS];
  bool        cooldown_used[MAX_EVENTS];
}events_t;

events_t* InitEvents();
void UnloadEvents(events_t* ev);
int AddEvent(events_t* pool, cooldown_t* cd);
void StepEvents(events_t* pool);
void StartEvent(events_t* pool, EventType type);
void ResetEvent(events_t* pool, EventType type);
bool CheckEvent(events_t* pool, EventType type);

typedef enum{
  STATE_NONE,//if ent_t is properly initalized to {0} this is already set
  STATE_SPAWN,//Should only be set after NONE
  STATE_EMPTY,
  STATE_IDLE, //should be able to move freely between these ==>
  STATE_SELECTED,
  STATE_PLACED,
  STATE_SCORE,
  STATE_DIE,//<===== In MOST cases. Should not be able to go down from DIE
  STATE_END//sentinel entity state should never be this or greater
}EntityState;
#endif
