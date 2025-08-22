#include "game_projectiles.h"

static projectile_pool_t projectile_pool;

void InitProjectilePool(projectile_pool_t* p, ObjectInstance data){
  memset(p,0,sizeof(*p));

  for (int i = 0; i < MAX_PROJECTILES; i++)
    p->free_stack[i] = (uint16_t)(MAX_PROJECTILES - 1 - i);
  
  p->top = MAX_PROJECTILES;

  p->base = InitEntProjectile(data);
}

ent_t* SpawnProjectile(projectile_pool_t* p, Vector2 pos, Vector2 dir){
  if (p->top == 0)
    return NULL; // pool full, decide whether to drop or overwrite oldest
  uint16_t i = p->free_stack[--p->top];
  // Shallow copy the archetype for fast setup
  ent_t* e = &p->ents[i];
  *e = p->base;
  e->uid   = ++p->next_uid;
  e->pos   = pos;
  e->facing = Vector2Normalize(dir);

  // Give this bullet its own body storage
  rigid_body_t* b = &p->bodies[i];
  b = InitRigidBodyKinematic(e, pos, .5);
  
  e->events = &p->evslots[i];
  InitEvents(e->events);

  return e;
}

void DespawnProjectile(projectile_pool_t* p, ent_t* b){

}
void ProjectileCullOffScreen(projectile_pool_t* p, Rectangle bounds){

}

void ProjectileShoot(ent_t* owner, Vector2 dir){

}

void ProjectileSync(ent_t* p){

}

