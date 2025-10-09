#include "game_projectiles.h"
#include "game_process.h"
#include "game_tools.h"

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

ProjectileInstance ProjectileGetData(const char* name){
  return room_projectiles[0];
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
 
void InitProjectilePool(ProjectileInstance data){
  projectile_pool = (projectile_pool_t){0};
  for (int i = 0; i < MAX_PROJECTILES; i++)
    projectile_pool.free_stack[i] = (uint16_t)(MAX_PROJECTILES - 1 - i);
  
  projectile_pool.top = MAX_PROJECTILES;

  projectile_pool.base = InitEntProjectile(data);
  Rectangle bounds = projectile_pool.base.sprite->slice->bounds;
  Vector2 offset = projectile_pool.base.sprite->slice->offset;
  Rectangle cBounds = RectangleScale(bounds, projectile_pool.base.sprite->slice->scale);
  Vector2 cOffset = Vector2Scale(offset, projectile_pool.base.sprite->slice->scale);

  projectile_pool.body = InitRigidBodyKinematic(NULL,Vector2Zero(),cBounds,cOffset);
  projectile_pool.body.forces[FORCE_STEERING].max_velocity = data.speed;
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
  e->facing = v2_ang_deg(Vector2Normalize(dir));

  e->sprite->owner = e;
  // Give this bullet its own body storage
  rigid_body_t* b = &p->bodies[i];
  *b = p->body;
  b->position = pos;
  b->owner    = e;
  b->buid = MAX_ENTS + e->uid; 
  e->body     = b;
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
  e->sprite = NULL;
  p->free_stack[p->top++] = i;
}

void ProjectileCullOffScreen(Rectangle bounds){
  // Simple linear sweep; you can also track a compact "alive list" if needed
  for (int i = 0; i < MAX_PROJECTILES; ++i) {
    ent_t* e = &projectile_pool.ents[i];
    // A dead slot has no body because we null it in Despawn
    if (!e->body) continue;
    if (!CheckCollisionPointRec(e->pos, bounds)) {
      DespawnProjectile(&projectile_pool, e);
    }
  }
}

void ProjectileShoot(ent_t* owner, Vector2 pos, Vector2 dir,int damage){
  ent_t* p = SpawnProjectile(&projectile_pool,pos, dir);

  p->owner = owner;
  PhysicsInitOnce(p->body);
  EntInitOnce(p);
  PhysicsAccelDir(p->body,FORCE_STEERING,dir);
  p->attacks[ATTACK_THORNS].damage = damage;
}

bool ProjectileCollide(rigid_body_t* a, rigid_body_t* b){
  if(b->forces[FORCE_KINEMATIC].type  == FORCE_KINEMATIC){
    b->forces[FORCE_KINEMATIC].on_react(b,a,FORCE_KINEMATIC);
    //return true;
  }
  
  for (int i = 0; i < FORCE_NONE; i++){
    if (a->forces[i].type == FORCE_NONE || !a->forces[i].on_react)
      continue;

    a->forces[i].on_react(a,b,i);
  }

  return true;
}

void ProjectileCollision(rigid_body_t* a, rigid_body_t* b){
  if(!b->simulate)
      return;

   if(!CanInteract(a->buid, b->buid))
      return;

   if(!CheckCollision(a, b,0))
      return;

   if(!ProjectileCollide(a,b))
     return;

   AddInteraction(EntInteraction(a->buid,b->buid,a->col_rate));
   if(b->owner->team == TEAM_ENEMIES)
    AddPoints(GetInteractions(a->buid)-1,1,b->position);
   else if (b->owner->type == ENT_PLAYER)
     SetState(a->owner,STATE_DIE,NULL);
   
   AudioPlayRandomSfx(SFX_ACTION,ACTION_SHOOT);

}

void ProjectilesStep(){
  for (int i = 0; i < MAX_PROJECTILES; ++i) {
    ent_t* e = &projectile_pool.ents[i];
    if (!e->body) continue;
    ProjectileSync(e);
  }
}

void ProjectileSync(ent_t* p){
  PhysicsStep(p->body);

  struct ent_s* others[MAX_ENTS];
  int num_others =  WorldGetEnts(others,FilterEntNotOnTeamAlive, p);

  for(int i = 0; i < num_others; i++)
    ProjectileCollision(p->body,others[i]->body);
  
  EntSync(p);
  if(p->state == STATE_DIE){
    if(!SetState(p,STATE_END,NULL))
      return;

    DespawnProjectile(&projectile_pool, p);
  }
}

void ProjectilesRender(){
  for (int i = 0; i < MAX_PROJECTILES; ++i) {
    ent_t* e = &projectile_pool.ents[i];
    if (!e->sprite) continue;

    DrawSpriteAtPos(e->sprite,e->pos);
  }
}
