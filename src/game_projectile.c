#include "game_projectiles.h"

static projectile_pool_t projectile_pool;
static int attack_count = 0;
static attack_register_entry_t Attack_Registry[MAX_ATTACKS];

void RegisterAttack(const char *name, AttackInstance *instance){
  if(attack_count >= MAX_ATTACKS)
    return;

  Attack_Registry[attack_count].name = strdup(name);
  Attack_Registry[attack_count].data = instance;
  attack_count++;
}

AttackInstance* AttackGetData(const char *name){
  for (int i = 0; i < attack_count; i++){
    if(strcmp(Attack_Registry[i].name,name)!=0)
      continue;

    if(!Attack_Registry[i].data)
      return NULL;

    return Attack_Registry[i].data;
  }

  return NULL;
}
 
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

void DespawnProjectile(projectile_pool_t* p, ent_t* e){
  if (!p) return;
  uint16_t i = (uint16_t)(e - p->ents); // index from pointer arithmetic
                                        // Quick scrub of volatile fields to help catch use-after-free in debug builds
  e->body = NULL;
  e->events = NULL;
  p->free_stack[p->top++] = i;
}

void ProjectileCullOffScreen(projectile_pool_t* p, Rectangle bounds){

}

void ProjectileShoot(ent_t* owner, Vector2 pos, Vector2 dir){
  ent_t* p = SpawnProjectile(&projectile_pool,pos, dir);
}

void ProjectileSync(ent_t* p){

}

