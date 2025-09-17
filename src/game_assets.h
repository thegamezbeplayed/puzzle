#ifndef __GAME_ASSETS__
#define __GAME_ASSETS__

#include "raylib.h"
#include <stdlib.h>
#include <string.h>
#include "asset_sprites.h"
#include "game_common.h"

#define MAX_SONGS 4

struct ent_s;
void InitResources();

typedef struct{
  Music music;
  float elapsed;
  float duration;
}music_track_t;

typedef struct{
  int           index;
  int           num_songs;
  music_track_t track[MAX_SONGS];
}music_group_t;

typedef enum{
  SFX_ALL,
  SFX_UI,
  SFX_ACTION,
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
  {ACTION_COMBO,SFX_ACTION,5,{"combo_001.ogg","combo_002.ogg","combo_003.ogg","combo_004.ogg","combo_005.ogg"}},
  {ACTION_REFLECT,SFX_ACTION,4,{"error_001.ogg","error_002.ogg","error_003.ogg","error_004.ogg"}},
  {ACTION_BOOM,SFX_ACTION,3,{"boom_001.ogg","boom_002.ogg","boom_003.ogg"}},
};

typedef struct{
  int         num_types;
  sfx_sound_t items[END_SFX];
}sfx_group_t;

typedef struct{
  music_group_t   tracks;
  sfx_group_t     sfx[SFX_DONE];
  events_t        *timers[SFX_DONE];
}audio_manager_t;
void InitAudio();
void AudioStep();
void AudioPlayRandomSfx(SfxGroup group, SfxType type);

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

void DrawSlice(Texture2D tex, sprite_slice_t *slice, Vector2 position,float rot);

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
  //anim_t          *animation;
  //AnimType        active_anim;
  bool            mirror;
  bool            is_visible;
  float           rot;
  Vector2         offset;
  Vector2         pos;
  RenderLayer     layer;
  struct ent_s    *owner;
} sprite_t;

sprite_t* InitSprite(const char* tag, sprite_sheet_data_t* spritesheet);
sprite_t* InitSpriteByIndex(int index, sprite_sheet_data_t* spritesheet);
bool FreeSprite(sprite_t* s);
void DrawSprite(sprite_t* s);
void DrawSpriteAtPos(sprite_t*s , Vector2 pos);
//====SPRITE_T>>
#endif
