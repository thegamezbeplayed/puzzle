#ifndef __GAME_PROJECTILE__
#define __GAME_PROJECTILE__
#include "game_types.h"

#define MAX_PROJECTILES 1024

typedef struct {
    ent_t          ents[MAX_PROJECTILES];      // the actual bullet entities
    rigid_body_t   bodies[MAX_PROJECTILES];    // per-slot body storage (no malloc per shot)
    events_t       evslots[MAX_PROJECTILES];   // per-slot events storage (optional)
    uint16_t       free_stack[MAX_PROJECTILES];
    int            top;                    // stack size
    ent_t          base;                   // archetype copy for fast init
    rigid_body_t   body;              // default physics for bullets
    uint32_t       next_uid;
} projectile_pool_t;
ProjectileInstance ProjectileGetData(const char* name);
void InitProjectilePool(ProjectileInstance data);

ent_t* SpawnProjectile(projectile_pool_t* p, Vector2 pos, Vector2 dir);

void DespawnProjectile(projectile_pool_t* p, ent_t* b);

void ProjectileCullOffScreen( Rectangle bounds);

void ProjectileShoot(ent_t* owner, Vector2 pos, Vector2 dir,int damage);
void ProjectileCollision(rigid_body_t* a, rigid_body_t* b);
bool ProjectileCollide(rigid_body_t* a, rigid_body_t* b);
void ProjectilesStep();
void ProjectileSync(ent_t* p);
void ProjectilesRender();
#endif
