#ifndef __GAME_COMMON__
#define __GAME_COMMON__

#include "raylib.h"

#define MAX_EVENTS 16

typedef struct{
  const char* text;
  Vector2     pos;
  int         size;
  Color       color;
  int         duration;
}render_text_t;

typedef enum {
  RANGE_NONE =0,
  RANGE_COL = 64,
  RANGE_CLOSE = 96,
  RANGE_COMMON = 480,
  RANGE_FAR = 640
}RangeLengths;

typedef enum{
  INTERVAL_NONE =0,
  INTERVAL_IMEDIATE = 6,
  INTERVAL_FAST = 18,
  INTERVAL_NORMAL = 24,
  INTERVAL_SLOW = 30
}IntervalRates;
typedef enum {
  ATTACK_MELEE,
  ATTACK_RANGED,
  ATTACK_THORNS,
  ATTACK_BLANK
} AttackType;
//===STATS===>
typedef enum{
  STAT_HEALTH,
  STAT_DAMAGE,
  STAT_POWER,
  STAT_SPEED,
  STAT_ACCEL,
  STAT_POINTS,
  STAT_DURATION,
  STAT_AMMO,
  STAT_RANGE,
  STAT_BLANK//sentinel
}StatType;

struct ent_s;   // forward declaration

struct stat_s;
typedef struct stat_s stat_t;
typedef bool (*StatCallback)(struct ent_s* owner);
typedef float (*StatGetter)(stat_t* self);
typedef struct stat_s{
  StatType  attribute;
  float     min;
  float     max;
  float     current;
  float     increment;
  StatGetter ratio;
  StatCallback on_stat_change;
  StatCallback on_stat_empty;
} stat_t;

stat_t InitStatOnMax(StatType attr, float val);
stat_t InitStatEmpty();
void InitStats(stat_t stats[STAT_BLANK]);
bool StatChangeValue(struct ent_s* owner, stat_t* attr, float val);
void StatMaxOut(stat_t* s);
float StatGetRatio(stat_t *self);
//<====STATS

//====EVENTS===>
typedef enum{
  EVENT_GAME_PROCESS,
  EVENT_INTERACTION,
  EVENT_SPAWN,
  EVENT_WAVE,
  EVENT_ATTACK_INPUT,
  EVENT_ATTACK_RATE,
  EVENT_ATTACK_PREPARE,
  EVENT_PLAY_SFX,
  EVENT_FINISH,
  EVENT_NONE
} EventType;

typedef struct {
  EventType   ev;
  stat_t      dur;
}EventDefaultDuration;

static EventDefaultDuration event_durations[EVENT_NONE] = {
  {EVENT_NONE,0},
  {EVENT_NONE,0},
  {EVENT_SPAWN, (stat_t){.attribute = STAT_DURATION, .max = 35, .current = 35, .min = 24}},
  {EVENT_WAVE, (stat_t){.attribute = STAT_DURATION, .max = 180, .current = 196, .min = 66}},
  {EVENT_NONE,0},
  {EVENT_NONE,0},
  {EVENT_NONE,0},
  {EVENT_NONE,0},
};

typedef void (*CooldownCallback)(void* params);

typedef struct{
  EventType         type;
  int               duration;
  int               elapsed;
  bool              is_complete;
  bool              is_recycled;
  void*             on_end_params;
  CooldownCallback  on_end;
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
//<======EVENTS>
typedef enum{
  TEAM_PLAYER,
  TEAM_ENEMIES,
  TEAM_ENVIROMENT,
  TEAM_NONE
}TeamName;

typedef enum {
  SHAPE_CIRCLE,
  SHAPE_RECTANGLE,
  SHAPE_TRIANGLE,
  SHAPE_PIXEL,
  SHAPE_NONE
} ShapeType;

typedef enum{
  STATE_NONE,//if ent_t is properly initalized to {0} this is already set
  STATE_SPAWN,//Should only be set after NONE
  STATE_IDLE, //should be able to move freely between these ==>
  STATE_WANDER,
  STATE_AGGRO,
  STATE_ATTACK,
  STATE_DIE,//<===== In MOST cases. Should not be able to go down from DIE
  STATE_END//sentinel entity state should never be this or greater
}EntityState;

typedef enum{
  OBJECT_NONE,//if ent_t is properly initalized to {0} this is already set
  OBJECT_LOAD,
  OBJECT_START,//Should only be set after NONE
  OBJECT_PAUSE,
  OBJECT_RUN,
  OBJECT_FINISH,
  OBJECT_END//sentinel entity state should never be this or greater
}ObjectState;

typedef struct {
  const char* name;
  EntityState   state;
}EntityStateAlias;

static EntityStateAlias ent_state_alias[STATE_END] = {
  {"STATE_IDLE",    STATE_IDLE},
  {"STATE_WANDER",  STATE_WANDER},
  {"STATE_SPAWN",   STATE_SPAWN},
  {"STATE_ATTACK",  STATE_ATTACK},
  {"STATE_AGGRO",   STATE_AGGRO},
  {"STATE_DIE",     STATE_DIE},
  {"STATE_END",     STATE_END} 
};

EntityState EntityStateLookup(const char* name);
const char* EntityStateName(EntityState s);
#endif
