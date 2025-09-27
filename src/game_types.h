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

#define MAX_ENTS 64  
#define MAX_SPAWNS 8  
#define MAX_ATTACKS 8

#define RATIO(s) ((s).ratio(&(s)))
typedef struct ent_s ent_t;
typedef struct rigid_body_s rigid_body_t;

//PHYSICS====>
typedef struct rigid_body_s rigid_body_t;

typedef enum {
  FORCE_BLANK,
  //FORCE_GRAVITY,
  FORCE_STEERING,
  FORCE_IMPULSE,
  FORCE_AVOID,
  FORCE_KINEMATIC,
  //FORCE_JUMP,
  FORCE_NONE
}ForceType;

typedef struct bounds_s {
  ShapeType   shape;
  Vector2     pos,offset;
  float       radius;
  float       width,height;
  //BoundsCheckCall col_check;
} bounds_t;

typedef void (*ForceReactionCallback)(rigid_body_t *a, rigid_body_t *b, ForceType type);

typedef struct {
  Vector2    vel;
  ForceType  type;
  Vector2    accel;
  float      max_velocity;
  Vector2    friction;
  float      threshold;
  bool       is_active;
  //int        steps;
  ForceReactionCallback on_react;
  ForceReactionCallback on_resolution;
}force_t;
//===RIGID_BODY===>
typedef bool (*CollisionCallback)(rigid_body_t* a, rigid_body_t* b, ent_t *e);
void ReactionBumpForce(rigid_body_t* a, rigid_body_t* b, ForceType t);
void ReactionAvoidForce(rigid_body_t* a, rigid_body_t* b, ForceType t);
void CollisionBoundsAvoid(rigid_body_t* a, rigid_body_t* b, ForceType t);
bool CheckStep(rigid_body_t* b, Vector2 vel, float dist, Vector2* out);
bool CheckCanSeeTarget(rigid_body_t* a, rigid_body_t *b, float range);

typedef struct rigid_body_s {
  int           buid; //body uid
  Vector2       velocity;
  Vector2       position;
  force_t       forces[FORCE_NONE];
  bounds_t      collision_bounds;
  int           num_collisions_detected;
  bounds_t      collisions[4];//TODO make define
  ForceType     counter_force[FORCE_NONE];
  float         restitution;
  bool          is_static;
  bool          is_kinematic;
  bool          is_ground;
  bool          is_grounded;
  bool          simulate;
  struct ent_s  *owner;
  int           col_rate;
  CollisionCallback on_collision;
} rigid_body_t;

rigid_body_t* InitRigidBody(ent_t* owner,Vector2 pos, float radius);
rigid_body_t* InitRigidBodyStatic(ent_t* owner,Vector2 pos, float radius);
rigid_body_t InitRigidBodyKinematic(ent_t* owner, Vector2 pos,float radius);
void RigidBodySetPosition(rigid_body_t* b, Vector2 pos);
bool FreeRigidBody(rigid_body_t* b);
//<====RIGID_BODY
static inline float PhysicsSimpleDistCheck(rigid_body_t* a, rigid_body_t* b){
  return Vector2Distance(a->position, b->position);
} 

void PhysicsInitOnce(rigid_body_t* b);
void PhysicsStep(rigid_body_t *b);
void CancelForce(force_t *f);
bool PhysicsStepForce(force_t *force,bool accelerate);
void PhysicsApplyForce(rigid_body_t* body, force_t force);
void PhysicsAccelDir(rigid_body_t *b, ForceType type, Vector2 dir);
void PhysicsSetAccel(rigid_body_t *b, ForceType type,Vector2 accel);
void PhysicsCollision(int i,rigid_body_t* bodies[MAX_ENTS],int num_bodies, CollisionCallback callback);
bool CheckCollision(rigid_body_t *a, rigid_body_t *b, int len);
bool RigidBodyCollide(rigid_body_t* a, rigid_body_t* b, ent_t *e);
void CollisionReflect(rigid_body_t* a, rigid_body_t* b, ForceType t);
void CollisionDamage(rigid_body_t* a, rigid_body_t* b, ForceType t);
void RecoilDamage(rigid_body_t* a, rigid_body_t* b, ForceType t);
void CollisionMelee(rigid_body_t* a, rigid_body_t* b, ForceType t);
void ForceDisable(rigid_body_t* a, rigid_body_t* b, ForceType t);

force_t ForceFromVector2(ForceType type, Vector2 vec);
force_t ForceEmpty(ForceType type);
force_t ForceBasic(ForceType type);
//<====PHYSICS
typedef struct{
  Vector2 dir;
}attack_params_t;
typedef struct attack_s attack_t;
typedef bool (*AttackFunc)(struct attack_s* a, struct ent_s* e);

typedef struct attack_s{
  int               duid;
  int               duration;
  stat_t            reach;
  int               damage;
  cooldown_t*       cooldown;
  AttackType        attack_type;
  attack_params_t*  params;
  AttackFunc        attack;
  rigid_body_t	  *hurtbox;
  struct ent_s* owner;
}attack_t;

attack_t InitAttack(ent_t* owner,AttackData data);
bool AttackPrepare(attack_t* a);
bool AttackShoot(attack_t *a, ent_t* e);
bool AttackMelee(attack_t *a, ent_t* e);
bool AttackDefault(attack_t *a, ent_t* e);

typedef enum{
  RANGE_AGGRO,
  RANGE_LOITER,
  RANGE_NEAR,
  RANGE_EMPTY
}RangeType;

typedef struct{
  ent_t*                  target;
  Vector2                 destination;
  int                     ranges[RANGE_EMPTY];
  bool                    has_arrived;
  behavior_tree_node_t*   bt[STATE_END];
}controller_t;
controller_t* InitController();

//===ENT_T===>
typedef struct ent_s{
  int                   uid;
  char*                 name;
  EntityType            type;
  TeamName              team;
  Vector2               pos;
  rigid_body_t          *body;
  EntityState           state;
  controller_t          *control;
  events_t              *events;
  struct ent_s*         child;
  int                   lastdamage_sourceid;
  int                   num_attacks;
  int                   active_attack_id;
  attack_t              attacks[ATTACK_BLANK];
  stat_t                stats[STAT_BLANK];
  float                 facing;
  sprite_t              *sprite;
} ent_t;

ent_t InitEntRef(ObjectInstance ref);
ent_t* InitEntStatic(TileInstance data);
ent_t* InitEnt(ObjectInstance data);
ent_t InitEntProjectile( ProjectileInstance data);

void EntInitOnce(ent_t* e);
void EntSync(ent_t* e);
bool EntKill(ent_t* e);
void EntDestroy(ent_t* e);
bool FreeEnt(ent_t* e);
bool EntReload(ent_t* e);
void EntAddPoints(ent_t* e,EntityState old, EntityState s);
void DamageEnt(ent_t *e, attack_t a);
void EntPrepStep(ent_t *e);
void EntControlStep(ent_t *e);
typedef void (*StateChangeCallback)(ent_t *e, EntityState old, EntityState s);
bool CheckEntOutOfBounds(ent_t* e, Rectangle bounds);
bool SetState(ent_t *e, EntityState s,StateChangeCallback callback);
void StepState(ent_t *e);
void OnStateChange(ent_t *e, EntityState old, EntityState s);
bool CanChangeState(EntityState old, EntityState s);

typedef struct {
  EntityType   reference_ents[MAX_SPAWNS];
  int          num_references;
}entity_pool_t;

entity_pool_t* InitEntityPool();
//===ENT_T==>
//====GAME_OBJECT_T===>
typedef struct game_object_s{
  unsigned int   id;
  unsigned int   level_id;
  Vector2         pos;
  ObjectState     state;
  events_t        *events;
  controller_t    *control;
}game_object_t;
bool SpawnEnt(game_object_t* spawner);
game_object_t* InitObjectStatic(SpawnerInstance inst);
void SetObjectState(game_object_t *obj, ObjectState state);
void StepObjectState(game_object_t *obj);
void StepObject(game_object_t *obj);
bool CanChangeObjectState(ObjectState old, ObjectState s);
void RegisterPoolRef(unsigned int level_index, unsigned int index, EntityType ref);
//===GAME_OBJECT_T===>
#endif
