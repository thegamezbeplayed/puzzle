#include "game_projectiles.h"
#include "game_tools.h"

particle_t particles[MAX_PARTICLES];
gl_shader_t shaders[SHADER_DONE];

void InitShaders(){
  for (int i = 0; i < SHADER_DONE; i++)
    shaders[i] = (gl_shader_t){0};
  
  FilePathList shaderfiles = LoadDirectoryFiles("resources/shaders/glsl100");
  for (int i = 0; i < shaderfiles.count; i++){
    const char* filename = GetFileName(shaderfiles.paths[i]);
    char* shadername = GetFileStem(filename);
    const char* extStr = GetFileExtension(filename);

    ShaderType type = ShaderTypeLookup(shadername);
    gl_shader_t *s = &shaders[type];
    if(shaders[type].stype == SHADER_NONE){
      s->stype = type;
      s->version = 100;
    }
    
    if(strcmp(extStr,".vs") == 0)
      s->vs_path = strdup(shaderfiles.paths[i]);
    else if (strcmp(extStr,".fs") == 0)
      s->fs_path = strdup(shaderfiles.paths[i]);
  
    free(shadername);
  }

  UnloadDirectoryFiles(shaderfiles);
}

void LoadShaders(){
  for(int i = 0; i < SHADER_DONE;i++){
    if(shaders[i].stype == SHADER_NONE)
      continue;

    shaders[i].shader = LoadShader(shaders[i].vs_path, shaders[i].fs_path);
  }
}

void ShaderSetUniforms(gl_shader_t *s, Texture2D texture){
    float outlineSize = 2.0f;
    float outlineColor[4] = { 1.0f, 0.0f, 0.0f, 1.0f };     // Normalized RED color
    float textureSize[2] = { (float)texture.width, (float)texture.height };

    // Get shader locations
    int outlineSizeLoc = GetShaderLocation(s->shader, "outlineSize");
    int outlineColorLoc = GetShaderLocation(s->shader, "outlineColor");
    int textureSizeLoc = GetShaderLocation(s->shader, "textureSize");

    SetShaderValue(s->shader, outlineSizeLoc, &outlineSize, SHADER_UNIFORM_FLOAT);
    SetShaderValue(s->shader, outlineColorLoc, outlineColor, SHADER_UNIFORM_VEC4);
    SetShaderValue(s->shader, textureSizeLoc, textureSize, SHADER_UNIFORM_VEC2);


}

ShaderType ShaderTypeLookup(const char* name){
  for (int i = 0; i < SHADER_DONE; i++){
    if(strcmp(name, shader_alias[i].name) == 0)
      return shader_alias[i].type;
  }

  TraceLog(LOG_WARNING,"<====GAME_DATA====>\n====== State Definition for %s not implemented!",name);
  return 0;

}

void InitParticle(){
  particle_t p = {0};

  p.force = ForceEmpty(FORCE_NONE);

  p.pos = VECTOR2_ZERO;
  AddParticle(&p);
}

void AddParticle(particle_t *p){
  for (int i = 0; i < MAX_PARTICLES; i++){
    if(particles[i].force.type != FORCE_BLANK)
      continue;

    particles[i] = *p;
  }
}

void SetParticle(Vector2 pos,force_t *f, Color color, ShapeType shape, Vector2 size, float duration){
  for (int i = 0; i < MAX_PARTICLES; i++){
    if(particles[i].force.type != FORCE_NONE)
      continue;

    particles[i].force = *f;
    particles[i].color = color;
    particles[i].shape = shape;
    particles[i].height = size.y;
    particles[i].width = size.x;
    particles[i].life = duration;
    particles[i].pos = pos;

    return;
  }
}

void StepParticles(){
  for (int i = 0; i < MAX_PARTICLES; i++){
    if (particles[i].life <= 0){
      particles[i].force.type = FORCE_NONE;
      continue;
    }

    particles[i].life--;
    PhysicsStepForce(&particles[i].force,true);

    particles[i].pos = Vector2Add(particles[i].pos,particles[i].force.vel);
  }
}

void ParticlesRender(){
  for(int i = 0; i < MAX_PARTICLES; i++){
    if(particles[i].life <= 0)
      continue;

    switch(particles[i].shape){
      case SHAPE_PIXEL:
        DrawPixelV(particles[i].pos, particles[i].color);
        break;
      case SHAPE_CIRCLE:
        break;
      case SHAPE_RECTANGLE:
        break;
    }
  }
}
void ParticleExplosion(Vector2 pos, Vector2 momentum,float size, Color color){

  int amnt =  ceilf(size);
  for (int i = 0; i< amnt; i++){

    float distrX = (float)GetRandomValue(-size, size) / size; // ~0.0 … 1.0
    float distrY = (float)GetRandomValue(-size, size) / size; // ~0.0 … 1.0

    Vector2 pPos = Vector2Inc(pos,distrX,distrY);
    Vector2 dir = Vector2Subtract(pPos, pos);

    // normalize to unit vector (avoid div0 by checking length)
    if (Vector2Length(dir) > 0.001f) {
      dir = Vector2Avg(Vector2Normalize(momentum),Vector2Normalize(dir));
      dir = Vector2Avg(dir,Vector2Normalize(momentum));
    }

    float speed = Vector2Length(momentum) + (float)GetRandomValue(i, size) / size;
    // scale to your desired acceleration strength
    Vector2 accel = Vector2Scale(dir, speed); // 200 = explosion force
                                              
    force_t f = ForceBasic(FORCE_KINEMATIC);
    f.accel = accel;
    f.friction = Vector2Inc(VECTOR2_ZERO, 0.885+distrX/10,0.885+distrY/10);
    f.threshold=0.1f;
    SetParticle(pPos,&f,color,SHAPE_PIXEL,VECTOR2_ONE, 20+amnt);

  }
}


