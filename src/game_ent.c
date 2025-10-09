#include "game_types.h"
#include "game_tools.h"
#include "game_projectiles.h"
#include "game_math.h"
#include "game_process.h"

MAKE_ADAPTER(StepState, ent_t*);
MAKE_ADAPTER(StatMaxOut, stat_t*);

attack_params_t empty_attack_params = {
  .dir = 0.0f
};

ent_t InitEntRef(ObjectInstance ref){
  ent_t e = {0};
  e.pos = Vector2Zero();
  e.name = (char*)malloc(100*sizeof(char));

  strcpy(e.name,ref.name);

  e.type =ref.ent_ref;
  //e.sprite = InitSpriteByIndex(ref.sprite_sheet_index,&spritedata);
  //e.sprite->owner = e;
  e.team = ref.team_enum;

  e.stats[STAT_HEALTH] = InitStatOnMax(STAT_HEALTH,ref.health);
  e.stats[STAT_HEALTH].on_stat_empty = EntKill;

  e.stats[STAT_SPEED] = InitStatOnMax(STAT_SPEED,ref.speed);
  e.stats[STAT_ACCEL] = InitStatOnMax(STAT_ACCEL,ref.accel);
  return e;
}

ent_t* InitEnt(ObjectInstance data){
  ent_t* e = malloc(sizeof(ent_t));
  *e = (ent_t){0};  // zero initialize if needed
  Vector2 pos = Vector2FromXY(data.x,data.y);
  e->name = (char*)malloc(100*sizeof(char));
  e->type = data.ent_ref;
  strcpy(e->name,data.name);
  e->sprite = InitSpriteByIndex(data.sprite_sheet_index,&spritedata);
  e->sprite->owner = e;  
  e->sprite->color = data.color;
  e->sprite->slice->scale = data.size / e->sprite->slice->bounds.width;

  InitShaderChainCache(e->type,e->sprite->slice->bounds.width,e->sprite->slice->bounds.height);

  for(int i = 0; i < SHADER_DONE;i++){
    e->sprite->gls[i] = malloc(sizeof(gl_shader_t));
    *e->sprite->gls[i] = shaders[i];
    if(!data.shaders[i])
      e->sprite->gls[i]->stype = SHADER_NONE;
  }
  Vector4* col = malloc(sizeof(Vector4));
  *col =  ColorNormalize(e->sprite->color);
  e->sprite->gls[SHADER_OUTLINE]->uniforms[UNIFORM_OUTLINECOLOR] = (shader_uniform_t){
    UNIFORM_OUTLINECOLOR,
      STANDARD_VEC4,
      col
  };

  float *osize = malloc(sizeof(float));
  *osize = 3.0f * e->sprite->slice->scale;
  e->sprite->gls[SHADER_OUTLINE]->uniforms[UNIFORM_OUTLINESIZE] = (shader_uniform_t){
    UNIFORM_OUTLINESIZE,
      STANDARD_FLOAT,
      osize
  };

  float radius = data.size * 0.5f;
  pos = Vector2Add(pos,e->sprite->slice->center);
  e->pos = pos;
  e->body = InitRigidBody(e,pos, radius);
  SetState(e,STATE_SPAWN,NULL);
  e->team = data.team_enum;

  e->stats[STAT_HEALTH] = InitStatOnMax(STAT_HEALTH,data.health);
  e->stats[STAT_HEALTH].on_stat_empty = EntKill;

  e->stats[STAT_SPEED] = InitStatOnMax(STAT_SPEED,data.speed);
  e->stats[STAT_ACCEL] = InitStatOnMax(STAT_ACCEL,data.accel);
  e->stats[STAT_POWER] = InitStatOnMax(STAT_POWER,data.damage);
  e->debug_info = malloc(sizeof(debug_info_t));
  e->debug_info->show[DEBUG_HEALTH]=true;
  e->debug_info->offset = Vector2Y(data.size);
  e->events = InitEvents();
  if(e->team == TEAM_ENEMIES){
    for(int i = 0; i < ATTACK_BLANK;i++){
      /*
      if(data.attacks[i].type == ATTACK_BLANK)
        continue;
        */
      e->attacks[i] = InitAttack(e,data.attacks[i]);
      e->num_attacks++;
    }
    e->stats[STAT_POINTS] = InitStatOnMax(STAT_POINTS,data.points);
    e->control = InitController(data);
    e->control->ranges[RANGE_AGGRO] = data.aggro_range;
    e->control->ranges[RANGE_LOITER] = radius*2 + data.aggro_range;
    e->control->ranges[RANGE_NEAR] = radius+data.speed;
    e->control->bt[STATE_IDLE] = InitBehaviorTree("Seek");
    e->control->bt[STATE_WANDER] = InitBehaviorTree("Wander");
    e->control->bt[STATE_AGGRO] = InitBehaviorTree("Chase");
    e->control->bt[STATE_ATTACK] = InitBehaviorTree("AttackOnTheMove");
  }
  else{
    e->stats[STAT_HEALTH].on_stat_change = DisplayStatChange; 
  }

  return e;
}

ent_t* InitEntStatic( TileInstance data, Vector2 pos){
  ent_t* e = malloc(sizeof(ent_t));
  *e = (ent_t){0};  // zero initialize if needed

  e->type = ENT_WALL; 
  e->name = "tile";
  e->pos = pos;
  e->sprite = InitSpriteByIndex(data.tile_index,&spritedata);
  e->sprite->slice->scale=1;
  InitShaderChainCache(e->type,e->sprite->slice->bounds.width,e->sprite->slice->bounds.height);
  e->sprite->color = PINK;
  e->sprite->owner = e;
  float radius = e->sprite->slice->bounds.height * 0.5f;
  e->body = InitRigidBodyStatic(e, e->pos, radius);
  e->events = InitEvents();
  e->team = TEAM_ENVIROMENT;
  for(int i = 0; i < SHADER_DONE;i++){
    e->sprite->gls[i] = malloc(sizeof(gl_shader_t));
    *e->sprite->gls[i] = shaders[i];
    if(!data.shaders[i])
      e->sprite->gls[i]->stype = SHADER_NONE;
  }
  Vector4* col = malloc(sizeof(Vector4));
  *col =  ColorNormalize(e->sprite->color);
  e->sprite->gls[SHADER_OUTLINE]->uniforms[UNIFORM_OUTLINECOLOR] = (shader_uniform_t){
    UNIFORM_OUTLINECOLOR,
      STANDARD_VEC4,
      col
  };
  float *osize = malloc(sizeof(float));
  *osize = 4.0f * e->sprite->slice->scale;
  e->sprite->gls[SHADER_OUTLINE]->uniforms[UNIFORM_OUTLINESIZE] = (shader_uniform_t){
    UNIFORM_OUTLINESIZE,
      STANDARD_FLOAT,
      osize
  };

  SetState(e,STATE_SPAWN,OnStateChange);
  return e;
}

ent_t InitEntProjectile( ProjectileInstance data){
  ent_t e = (ent_t){0};  // zero initialize if needed
  Vector2 pos = Vector2FromXY(0,0);
  e.type = data.ent_ref;

  e.sprite = InitSpriteByIndex(data.sprite_sheet_index,&spritedata);
  if(e.sprite!=NULL){
    e.sprite->slice->scale =data.size / e.sprite->slice->bounds.width;
    e.sprite->owner = NULL;
    pos = Vector2Add(pos,e.sprite->slice->center);
    e.sprite->layer = LAYER_BG;
    InitShaderChainCache(e.type,e.sprite->slice->bounds.width,e.sprite->slice->bounds.height);

  }
  e.name = "bullet";

  e.stats[STAT_HEALTH] = InitStatOnMax(STAT_HEALTH,1);
  e.stats[STAT_HEALTH].on_stat_empty = EntKill;

  e.stats[STAT_SPEED] = InitStatOnMax(STAT_SPEED,data.speed);

  e.stats[STAT_ACCEL] = InitStatOnMax(STAT_ACCEL,data.speed);

  e.pos = pos;
 for(int i = 0; i < SHADER_DONE;i++){
    e.sprite->gls[i] = malloc(sizeof(gl_shader_t));
    *e.sprite->gls[i] = shaders[i];
    if(!data.shaders[i] )
      e.sprite->gls[i]->stype = SHADER_NONE;
  }

 float radius = e.sprite->slice->bounds.height * 0.5f;
  e.events = InitEvents();
  e.team = TEAM_ENEMIES;
  e.state = STATE_SPAWN;
  return e;
}

void EntAddPoints(ent_t *e,EntityState old, EntityState s){
  if(e->team == TEAM_PLAYER)
    return;
  if(e->stats[STAT_POINTS].current <= 0)
    return;

  AddPoints(GetInteractions(e->lastdamage_sourceid)-1, e->stats[STAT_POINTS].current,e->pos);
}

bool EntKill(ent_t* e){
  return SetState(e, STATE_DIE,NULL);
}

bool EntReload(ent_t* e){
  ResetEvent(e->events, EVENT_ATTACK_PREPARE);
}

void EntDestroy(ent_t* e){
  if(!e || !SetState(e, STATE_END,EntAddPoints))
    return;
 
  if(e->sprite!=NULL){
    e->sprite->owner = NULL;
    e->sprite->is_visible = false;
    e->sprite = NULL;
  }

  if(e->body!=NULL){
    e->body->owner = NULL;
    e->body = NULL;
  }

  for(int i = 0; i < e->num_attacks; i++){
    e->attacks[i].owner = NULL;
    if(e->attacks[i].hurtbox)
      e->attacks[i].hurtbox->owner = NULL;
  } 

  e->control = NULL;
}

attack_t InitAttack(ent_t* owner, AttackData data){
  attack_t a = (attack_t){0};

  a.owner = owner;
  a.duid = 10*owner->uid+owner->num_attacks;
  a.damage = data.base_damage + data.damage_mod * owner->stats[STAT_POWER].current;
  //a.duration = data.duration;
  a.attack_type = data.type;
  a.reach = InitStatOnMax(STAT_RANGE,data.range);

  EventType evtype = EVENT_ATTACK_RATE; 
  switch(data.type){
    case ATTACK_RANGED:
      owner->stats[STAT_AMMO] = InitStatOnMax(STAT_AMMO,data.ammo);
      owner->stats[STAT_AMMO].on_stat_empty = EntReload;
      a.reach.min = RANGE_CLOSE;
      cooldown_t* cd =InitCooldown(data.reload,EVENT_ATTACK_PREPARE,StatMaxOut_Adapter,&owner->stats[STAT_AMMO]);
      cd->is_recycled = true;
      AddEvent(owner->events,cd);
      a.attack = AttackShoot;
      break;
    case ATTACK_THORNS:
      evtype = EVENT_THORNS_ATTACK_RATE;
      owner->body->forces[FORCE_STEERING].on_react = CollisionDamage;
      owner->body->forces[FORCE_STEERING].on_resolution = RecoilDamage;
      break;
    case ATTACK_MELEE:
      a.attack = AttackMelee;
      evtype = EVENT_MELEE_ATTACK_RATE;
      owner->body->forces[FORCE_MELEE] = ForceBasic(FORCE_MELEE);
      owner->body->forces[FORCE_MELEE].on_react = CollisionMelee;
      owner->body->forces[FORCE_MELEE].on_resolution = ForceDisable;
      owner->body->forces[FORCE_MELEE].max_velocity = data.speed;
      owner->body->forces[FORCE_MELEE].accel = Vector2FromXY(data.accel,data.accel);
      break;
    default:
      return a;
      break;
  }
  //a.attack = (AttackFunc)data.fn;
  cooldown_t* cd =InitCooldown(data.rate,evtype,NULL,NULL); 
  cd->is_recycled = true;
  a.params = &empty_attack_params;
  int cId = AddEvent(owner->events,cd); 
  a.cooldown = &owner->events->cooldowns[cId];
  return a;
}

bool AttackShoot(attack_t* a, ent_t *e){
  ProjectileShoot(e, e->pos, a->params->dir, a->damage);

  a->cooldown->is_complete = false;
  a->cooldown->elapsed = 0;
  StatChangeValue(e,&e->stats[STAT_AMMO],-1);
  return true;
}

bool AttackMelee(attack_t* a, ent_t *e){
  PhysicsAccelDir(e->body,FORCE_MELEE,a->params->dir);;
  a->cooldown->is_complete = false;
  a->cooldown->elapsed = 0;
}

void EntPrepStep(ent_t* e){
  e->body->forces[FORCE_STEERING].max_velocity = e->stats[STAT_SPEED].current;
}

bool AttackPrepare(attack_t* a){
  Vector2 target = VEC_UNSET;
  if(!a->owner->control)
    return false;

  if(a->owner->control->target)
    target = a->owner->control->target->pos;
  else
    target = a->owner->control->destination;

  if(v2_compare(target, VEC_UNSET))
    return false;

  float dist =Vector2Distance(target,a->owner->pos); 
  if(dist < a->reach.min || dist > a->reach.current)
    return false;

  a->params->dir = VectorDirectionBetween(a->owner->pos, target);
  return true;//a->attack(a->params);
}

bool FreeEnt(ent_t* e){
  if(!e)
    return false;


  if(e->type == ENT_PLAYER){
    player = malloc(sizeof(ent_t));
    *player = *e;
  }

  free(e);
  return true;
}

stat_t InitStatEmpty(){
  return (stat_t){
    .attribute = STAT_BLANK,
      .min = 0,
      .max = 0,
      .current = 0,
      .increment = 0//TODO idk yet
  };
}

stat_t InitStatOnMax(StatType attr, float val){
  return (stat_t){
    .attribute = attr,
      .min = 0.0f,
      .max = val,
      .current = val,
      .ratio = StatGetRatio
  };
}
float StatGetRatio(stat_t *self) {
    return (float)self->current / (float)self->max;
}
void StatMaxOut(stat_t* s){
  s->current = s->max;
}

void InitStats(stat_t stats[STAT_BLANK]){

}

bool StatChangeValue(ent_t* owner, stat_t* attr, float val){
  float old = attr->current;
  attr->current+=val;
  attr->current = CLAMPF(attr->current,attr->min, attr->max);
  float cur = attr->current;
  if(attr->current == old)
    return false;

  if(attr->on_stat_change != NULL)
    attr->on_stat_change(owner,old, cur);

  if(attr->current == attr->min && attr->on_stat_empty!=NULL)
    attr->on_stat_empty(owner);

  return true;
}

controller_t* InitController(){
  controller_t* ctrl = malloc(sizeof(controller_t));
  *ctrl = (controller_t){0};

  ctrl->destination = VEC_UNSET;
  return ctrl;
}

void EntInitOnce(ent_t* e){
  EntSync(e);
  
  cooldown_t* spawner = InitCooldown(3,EVENT_SPAWN,StepState_Adapter,e);
  AddEvent(e->events, spawner);
  if(e->child){
    e->child->control = InitController();
    e->child->control->target = e;
    e->child->control->bt[STATE_IDLE] = InitBehaviorTree("Orbit");
    e->child->control->ranges[RANGE_NEAR] = e->body->collision_bounds.radius + e->child->body->collision_bounds.radius;
    e->child->body->is_kinematic = true;
    e->child->body->forces[FORCE_KINEMATIC].type = FORCE_KINEMATIC;
    e->child->body->forces[FORCE_KINEMATIC].on_react = CollisionReflect;
  }
}

void DamageEnt(ent_t *e, attack_t a){//Copy of attack cuz its subject to Updates
  //AnimPlay(e->sprite,ANIM_HURT);
  StatChangeValue(e,&e->stats[STAT_HEALTH],-a.damage);
}

void EntSync(ent_t* e){
  if(e->control)  
    EntControlStep(e);

  StepEvents(e->events);

  if(!e->sprite)
    return;

  if(e->team == TEAM_ENVIROMENT)
    DO_NOTHING();
  e->sprite->pos = e->pos;// + abs(ent->sprite->offset.y);

  e->sprite->rot = 90+e->facing;
  if(DEBUG && e->debug_info)
    DebugSync(e,e->debug_info);
}

void EntControlStep(ent_t *e){
  if(!e->control || !e->control->bt || !e->control->bt[e->state])
    return;

  behavior_tree_node_t* current = e->control->bt[e->state];

  current->tick(current, e);
}

bool SetState(ent_t *e, EntityState s,StateChangeCallback callback){
  if(CanChangeState(e->state,s)){
    EntityState old = e->state;
    e->state = s;

    if(callback!=NULL)
      callback(e,old,s);
    else
      OnStateChange(e,old,s);
    return true;
  }

  return false;
}

void StepState(ent_t *e){
  SetState(e, e->state+1,NULL);
}

bool CanChangeState(EntityState old, EntityState s){
  if(old == s || old > STATE_END)
    return false;

  switch (COMBO_KEY(old,s)){
    case COMBO_KEY(STATE_NONE,!STATE_SPAWN):
    case COMBO_KEY(!STATE_NONE,STATE_SPAWN):
    case COMBO_KEY(!STATE_DIE,STATE_END):
      return false;
      break;
    case COMBO_KEY(STATE_SPAWN,STATE_ATTACK):
      return false;
      break;
    default:
      return true;
      break;
  }
} 

void OnStateChange(ent_t *e, EntityState old, EntityState s){
  //TraceLog(LOG_INFO,"Entity %s state change from %s to %s",e->name,EntityStateName(old),EntityStateName(s));
  switch(old){
    case STATE_SPAWN:
      if(e->body)
        e->body->simulate=true;
      if(e->sprite)
        e->sprite->is_visible = true;
      break;
    case STATE_WANDER:
    case STATE_ATTACK:
      StatMaxOut(&e->stats[STAT_ACCEL]);
      StatMaxOut(&e->stats[STAT_SPEED]);
    default:
      break;
  }

  switch(s){
    case STATE_WANDER:
    case STATE_AGGRO:
      StatMaxOut(&e->stats[STAT_SPEED]);
      break;
    case STATE_ATTACK:
      e->stats[STAT_ACCEL].current = e->stats[STAT_ACCEL].max*0.925f;
      e->stats[STAT_SPEED].current = e->stats[STAT_SPEED].max*.85f;
      break;
    case STATE_DIE:
      ParticleExplosion(e->pos,e->body->velocity,e->body->collision_bounds.radius,e->sprite->color);
      AudioPlayRandomSfx(SFX_ACTION,ACTION_BOOM);
      break;
    default:
      break;
  }

}

bool CheckEntOutOfBounds(ent_t* e, Rectangle bounds){
  return (CheckCollisionPointRec(e->pos, bounds));
}

void DebugSync(struct ent_s* e,debug_info_t* d){
  d->pos = e->pos;
  for(int i = 0; i < DEBUG_DONE; i++){
    if(!d->show[i])
      continue;

    switch(i){
      case DEBUG_HEALTH:
        strcpy(d->debug_text,TextFormat("%0.1f", e->stats[STAT_HEALTH].ratio(&e->stats[STAT_HEALTH])));
        break;
      default:
        break;
    }

  }  
}

void DebugShowText(debug_info_t* d){
  Vector2 pos = Vector2Add(d->pos,d->offset);
  DrawText(d->debug_text, pos.x,pos.y, 16, RED);
}

EntityType SpriteEntType(sprite_t *spr) {
    return spr->owner ? spr->owner->type : ENT_BLANK;
}

void DisplayStatChange(ent_t* owner, float old, float cur){
  int dif = (int)floorf(cur-old);
  if(dif == 0)
    return;
  
  ent_t *other = WorldGetEntById(owner->lastdamage_sourceid);

  if(other->owner)
    other = other->owner;

  Vector2 pos = Vector2Bisect(owner->pos,other->pos,3*owner->body->collision_bounds.radius);

  pos = Vector2Inc(pos,GetRandomValue(-3.5f,3.5f),GetRandomValue(-3.5f,3.5f));

  Color col = GREEN;
  if (dif < 0)
    col = RED;

  int dur = 3 + abs(3*dif);
  render_text_t *rt = malloc(sizeof(render_text_t));
  *rt = (render_text_t){
    .text = strdup(TextFormat("x%d",dif)),
    .pos = pos,
    .size = 16,
    .color = col,
    .duration = dur
  };
  AddFloatingText(rt);

}

