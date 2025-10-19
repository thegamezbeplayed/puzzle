#ifndef __GAME_ASSETS__
#define __GAME_ASSETS__

//#include "raylib.h"
#include <stdlib.h>
#include <string.h>
#include "asset_sprites.h"
#include "game_common.h"

#define MAX_SONGS 4
struct ent_s;
void InitResources();

//====SHADERS===>
typedef struct{
  bool              has_chain[ENT_BLANK];
  RenderTexture2D   chain1[ENT_BLANK];
  RenderTexture2D   chain2[ENT_BLANK];
}texture_chain_t;

typedef enum{
  SHADER_NONE,
  SHADER_BASE,
  SHADER_BLOOM,
  SHADER_BLUR,
  SHADER_INVERT,
  SHADER_OUTLINE,
  SHADER_DONE
}ShaderType;

typedef struct{
  const char* name;
  ShaderType  type;
}ShaderTypeAlias;

typedef enum {
  UNIFORM_TEXSIZE,
  UNIFORM_OUTLINESIZE,
  UNIFORM_OUTLINECOLOR,
  UNIFORM_COUNT,
  UNIFORM_NONE
} ShaderUniform;

typedef enum{
  STANDARD_INT,
  STANDARD_FLOAT,
  STANDARD_UINT,
  STANDARD_BOOL,
  STANDARD_VEC2,
  STANDARD_VEC3,
  STANDARD_VEC4,
}UniformTypes;

typedef struct{
  ShaderUniform uniform;
  UniformTypes  type;
  void*         val;
}shader_uniform_t;

static ShaderTypeAlias shader_alias[SHADER_DONE] = {
  {"base", SHADER_BASE},
  {"bloom", SHADER_BLOOM},
  {"blur", SHADER_BLUR},
  {"invert", SHADER_INVERT},
  {"outline", SHADER_OUTLINE},
};

ShaderType ShaderTypeLookup(const char* name);

typedef struct{
  ShaderType        stype;
  int               version;
  Shader            shader;
  const char*       vs_path;
  const char*       fs_path;
  shader_uniform_t  uniforms[UNIFORM_NONE];
}gl_shader_t;
extern gl_shader_t shaders[SHADER_DONE];

void InitShaderChainCache(EntityType,int maxWidth, int maxHeight);
void InitShaders();
void LoadShaders();
void ShaderSetUniforms(gl_shader_t *s, Texture2D texture);
//<=====SHADERS====

typedef struct{
  int     duration;
  int     elapsed;
  float   vol;
  bool    increase;
}audio_fade_t;

typedef struct{
  Music        music;
  events_t     *events;
  audio_fade_t *fade;
}music_track_t;

typedef struct{
  unsigned int  index;
  int           num_songs;
  unsigned int  current_index;
  float         volume;
  music_track_t track[MAX_SONGS];
}music_group_t;

typedef enum{
  SFX_ALL,
  SFX_UI,
  SFX_ACTION,
  SFX_IMPORTANT,
  SFX_DONE
}SfxGroup;

typedef enum {
  ACTION_SHOOT,
  ACTION_SHOT,
  ACTION_FRAG,
  ACTION_COMBO,
  ACTION_REFLECT,
  ACTION_BOOM,
  END_SFX
}SfxType;

typedef struct{
  SfxType     type;
  SfxGroup    group;
  int         num_sounds;
  const char* paths[5];
}sfx_info_t;

typedef struct{
  SfxType   type;
  int       num_sounds;
  Sound     *sounds;
}sfx_sound_t;

static sfx_info_t sfx_catalog[]={
  {ACTION_SHOOT,SFX_ACTION,4,{"close_001.ogg","close_002.ogg","close_003.ogg","close_004.ogg"}},
  {ACTION_SHOT,SFX_ACTION,4,{"select_001.ogg","select_002.ogg","select_003.ogg","select_004.ogg"}},
  {ACTION_FRAG,SFX_ACTION,4,{"scratch_001.ogg","scratch_002.ogg","scratch_003.ogg","scratch_004.ogg"}},
  {ACTION_COMBO,SFX_IMPORTANT,5,{"combo_001.ogg","combo_002.ogg","combo_003.ogg","combo_004.ogg","combo_005.ogg"}},
  {ACTION_REFLECT,SFX_ACTION,4,{"error_001.ogg","error_002.ogg","error_003.ogg","error_004.ogg"}},
  {ACTION_BOOM,SFX_ACTION,3,{"boom_001.ogg","boom_002.ogg","boom_003.ogg"}},
};

typedef struct{
  int         num_types;
  float       volume;
  sfx_sound_t items[END_SFX];
}sfx_group_t;

typedef struct{
  music_group_t   tracks[6];
  float           volume;
  int             current_track;
  int             concurrent_track;
  int             num_tracks;
  sfx_group_t     sfx[SFX_DONE];
  events_t        *timers[SFX_DONE];
}audio_manager_t;

void InitAudio();
int AudioBuildMusicTracks(const char* subdir);
void AudioStep();
void AudioPlaySfx(SfxGroup group, SfxType type, int index);
void AudioPlayRandomSfx(SfxGroup group, SfxType type);
void AudioPlayMusic(int index);
bool AudioPlayNext(music_group_t* t);
void AudioMusicFade(music_track_t* t);

typedef enum{
  LAYER_ROOT,
  LAYER_BG,
  LAYER_MAIN,
  LAYER_FLOOR,
  LAYER_FX_MAIN,
  LAYER_TOP
}RenderLayer;

typedef struct {
  char      *name;
  char      *group;
  int       sequence_index;
  Vector2   center;
  Rectangle bounds;
  Vector2   offset;
  float     scale;
} sprite_slice_t;

typedef struct{
  int             num_sprites;
  sprite_slice_t  *sprites[1024];
}sprite_sheet_data_t;
extern sprite_sheet_data_t spritedata;
void LoadrtpAtlasSprite(sprite_sheet_data_t *out);
//SPRITE_T===>
typedef struct {
  int             suid;
  Texture2D       *sheet;
  sprite_slice_t* slice;
  Color           color;
  gl_shader_t*    gls[SHADER_DONE];
  bool            is_visible;
  float           rot;
  Vector2         offset;
  Vector2         pos;
  RenderLayer     layer;
  struct ent_s    *owner;
} sprite_t;

void DrawSlice(sprite_t *spr, Vector2 position,float rot);
sprite_t* InitSprite(const char* tag, sprite_sheet_data_t* spritesheet);
sprite_t* InitSpriteByIndex(int index, sprite_sheet_data_t* spritesheet);
bool FreeSprite(sprite_t* s);
void DrawSprite(sprite_t* s);
void DrawSpriteAtPos(sprite_t*s , Vector2 pos);
EntityType SpriteEntType(sprite_t *spr);
//====SPRITE_T>>
#endif
