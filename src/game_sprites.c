#include "game_assets.h"
#include "game_utils.h"
#include "game_math.h"
#include "game_tools.h"

sprite_sheet_data_t spritedata;
Texture2D sprite_sheet;

void InitResources(){
  LoadrtpAtlasSprite(&spritedata);
  Image spritesImg = LoadImage(TextFormat("resources/%s",ATLAS_ASSET_SPRITES_IMAGE_PATH)); 
  sprite_sheet = LoadTextureFromImage(spritesImg);
}

sprite_t* InitSprite(const char* group, sprite_sheet_data_t* spritesheet){
 sprite_t* spr =malloc(sizeof(sprite_t));
  memset(spr,0,sizeof(sprite_t));

  for(int i = 0; i < spritesheet->num_sprites; i++){
    if(strcmp(group,spritesheet->sprites[i]->group)!=0)
      continue;

    spr->sheet = &sprite_sheet; 
    spr->slice = spritesheet->sprites[i];

  }
  spr->color = PINK;
  return spr;
}

sprite_t* InitSpriteByIndex(int index, sprite_sheet_data_t* spritesheet){
  sprite_t* spr =malloc(sizeof(sprite_t));
  memset(spr,0,sizeof(sprite_t));

  spr->mirror = false;

  spr->sheet = &sprite_sheet;

  spr->slice = spritesheet->sprites[index];
  spr->color = PINK;
  
  return spr;
}

void DrawSlice(sprite_t *spr, Vector2 position,float rot){
  sprite_slice_t* slice = spr->slice; 
  Rectangle src = slice->bounds;

    Rectangle dst = {
        position.x,
        position.y,
        slice->bounds.width * slice->scale,
        slice->bounds.height * slice->scale
    };

    Vector2 origin = (Vector2){
       slice->center.x * slice->scale,//offset.x,
       slice->center.y * slice->scale//offset.y
    };
    
    Texture2D *tex = spr->sheet;

    Texture2D input = *tex;   // start with the base sprite
    Texture2D output = *tex;

    for (int i = 0; i < SHADER_DONE; i++) {
      gl_shader_t *gls = spr->gls[i];
      if (!gls || gls->stype == SHADER_NONE) continue;

      // Choose which buffer to render into
      RenderTexture2D target = (i % 2 == 0) ? spr->chain1 : spr->chain2;

      BeginTextureMode(target);
      ClearBackground(BLANK); // clear buffer
      BeginShaderMode(gls->shader);

      ShaderSetUniforms(gls, input); // set this pass's uniforms

      // Draw input texture into shader pipeline
      DrawTexturePro(input, src,(Rectangle){0, 0, src.width, src.height},VECTOR2_ZERO, 0, WHITE);

      EndShaderMode();
      EndTextureMode();

      // Result of this pass becomes input for next pass
      output = target.texture;
      input = output;
    }
    // After last pass → draw to screen
    DrawTexturePro(output,(Rectangle){0,0,src.width,src.height},dst, origin, rot, WHITE);
    
    return;
}

bool FreeSprite(sprite_t* s){
  if(!s) return false;

  /*if(s->slice)
    free(s->slice);
*/
  for(int i = 0; i<SHADER_DONE;i++)
    free(s->gls[i]);

  free(s);
  return true;
}
void DrawSpriteAtPos(sprite_t*s , Vector2 pos){
  if(s->is_visible)
    DrawSlice(s, pos, s->rot);

}
void DrawSprite(sprite_t* s){
  if(s->is_visible)
    DrawSlice(s, s->pos,s->rot);
}

void LoadrtpAtlasSprite(sprite_sheet_data_t *out){

  for (int i = 0; i < ATLAS_ASSET_SPRITES_SPRITE_COUNT; i++){
    rtpAtlasSprite sprData = rtpDescAssetSprites[i];
 
    TraceLog(LOG_INFO,"Load index %d - %s into sprite sheet data",i,sprData.nameId);

   Vector2 center = Vector2FromXY(sprData.originX,sprData.originY);
    Vector2 offset = Vector2FromXY(sprData.trimRecX,sprData.trimRecY);
    Rectangle bounds = RecFromCoords(sprData.positionX,sprData.positionY,sprData.sourceWidth,sprData.sourceHeight);

    sprite_slice_t *spr = malloc(sizeof(sprite_slice_t));
    memset(spr,0,sizeof(sprite_slice_t));

    spr->name = (char*)malloc(MAX_NAME_LEN*sizeof(char));
    spr->group = (char*)malloc(MAX_NAME_LEN*sizeof(char));

    strcpy(spr->name,sprData.nameId);
    strcpy(spr->group,sprData.tag);
    spr->center = center;
    spr->offset = offset;
    spr->bounds = bounds;
    out->sprites[out->num_sprites++] = spr;

  }

  TraceLog(LOG_INFO,"Done with %s",ATLAS_ASSET_SPRITES_IMAGE_PATH);

}

