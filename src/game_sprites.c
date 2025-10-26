#include "game_assets.h"
#include "game_utils.h"
#include "game_math.h"
#include "game_tools.h"

sprite_sheet_data_t shapedata;
sprite_sheet_data_t tiledata;

texture_chain_t TexChain;

void InitShaderChainCache(ShapeID type,int maxWidth, int maxHeight) {
  if(TexChain.has_chain[type])
    return;

  TexChain.has_chain[type] = true;
  TexChain.chain1[type]= LoadRenderTexture(maxWidth, maxHeight);
  TexChain.chain2[type]= LoadRenderTexture(maxWidth, maxHeight);
}

void InitResources(){
  SpriteLoadSubTextures(&shapedata,SHAPES);
  SpriteLoadSubTextures(&tiledata,TILES);
  Image spritesImg = LoadImage(TextFormat("resources/%s",TILES_IMAGE_PATH)); 
  Image shapesImg = LoadImage(TextFormat("resources/%s",SHAPES_IMAGE_PATH)); 
  tiledata.sprite_sheet = malloc(sizeof(Texture2D));
  shapedata.sprite_sheet = malloc(sizeof(Texture2D));
  *tiledata.sprite_sheet = LoadTextureFromImage(spritesImg);
  *shapedata.sprite_sheet = LoadTextureFromImage(shapesImg);
}

sprite_t* InitSpriteByID(int id, sprite_sheet_data_t* data){
  sprite_t* spr =malloc(sizeof(sprite_t));
  memset(spr,0,sizeof(sprite_t));

  for (int i = 0; i < data->num_sprites; i++){
    if(data->sprites[i]->id != id)
      continue;

    spr->slice = data->sprites[i];
    spr->sheet = data->sprite_sheet;

    spr->slice->scale = SPRITE_SCALE;
  }
  return spr;

}

sprite_t* InitSpriteByIndex(int index, sprite_sheet_data_t* data){
  sprite_t* spr =malloc(sizeof(sprite_t));
  memset(spr,0,sizeof(sprite_t));

  spr->sheet = data->sprite_sheet;
  spr->slice = data->sprites[index];

  spr->slice->scale = SPRITE_SCALE;
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

  DrawTexturePro(*spr->sheet,src,dst, origin, rot, WHITE);

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

void SpriteLoadSubTextures(sprite_sheet_data_t *out, int sheet_id){

  sub_texture_t sprData;

  for (int i = 0; i < MAX_SPRITES; i++){
    switch(sheet_id){
      case TILES:
        if(i >= BACKTILE_DONE)
          return;
        sprData = BACK_TILES[i];
        break;
      case SHAPES:
        if(i >= SHAPE_COUNT)
          return;

        sprData = SHAPE_SPRITES[i];
        break;
    }

    Vector2 center = Vector2FromXY(sprData.originX,sprData.originY);
    Vector2 offset = Vector2FromXY(sprData.trimRecX,sprData.trimRecY);
    Rectangle bounds = RecFromCoords(sprData.positionX,sprData.positionY,sprData.sourceWidth,sprData.sourceHeight);

    sprite_slice_t *spr = malloc(sizeof(sprite_slice_t));
    memset(spr,0,sizeof(sprite_slice_t));

    spr->id = sprData.tag;
    spr->center = center;
    spr->offset = offset;
    spr->bounds = bounds;
    out->sprites[out->num_sprites++] = spr;

  }
}

