#include "game_assets.h"
#include "game_utils.h"
#include "game_math.h"
#include "game_tools.h"
#include "asset_ui.h"
#include "asset_sprites.h"
#include "asset_shapes.h"

sprite_sheet_data_t shapedata;
sprite_sheet_data_t tiledata;
scaling_sprite_data_t uidata;
texture_chain_t TexChain;

void InitShaderChainCache(ShapeID type,int maxWidth, int maxHeight) {
  if(TexChain.has_chain[type])
    return;

  TexChain.chain1[type]= LoadRenderTexture(maxWidth, maxHeight);
  TexChain.chain2[type]= LoadRenderTexture(maxWidth, maxHeight);
}

void InitResources(){
  SpriteLoadSubTextures(&shapedata,SHAPES);
  SpriteLoadSubTextures(&tiledata,TILES);
  SpriteLoadSlicedTextures();
  Image spritesImg = LoadImage(TextFormat("resources/%s",TILES_IMAGE_PATH)); 
  Image shapesImg = LoadImage(TextFormat("resources/%s",SHAPES_IMAGE_PATH)); 
  Image uiImg = LoadImage(TextFormat("resources/%s",UI_IMAGE_PATH)); 
  tiledata.sprite_sheet = malloc(sizeof(Texture2D));
  shapedata.sprite_sheet = malloc(sizeof(Texture2D));
  uidata.sprite_sheet = malloc(sizeof(Texture2D));
  *tiledata.sprite_sheet = LoadTextureFromImage(spritesImg);
  *shapedata.sprite_sheet = LoadTextureFromImage(shapesImg);
  *uidata.sprite_sheet = LoadTextureFromImage(uiImg);
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

scaling_slice_t* InitScalingElement(ElementID id){

  scaling_slice_t* el = malloc(sizeof(scaling_slice_t));

  if(uidata.sprites[id])
    el = uidata.sprites[id];

  return el;
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

void DrawNineSlice(scaling_slice_t *spr, Rectangle dst){

    float border = spr->slices[SLICE_TOP_LEFT]->bounds.width; // assuming uniform border
    float left   = dst.x;
    float top    = dst.y;
    float right  = dst.x + dst.width;
    float bottom = dst.y + dst.height;

    float midWidth  = dst.width  - 2 * border;
    float midHeight = dst.height - 2 * border;

  spr = uidata.sprites[1];
  for(int i = 0; i<SLICE_ALL;i++){
    sprite_slice_t* slice = spr->slices[i];
    Rectangle src = slice->bounds;

    Rectangle d = {0};

    switch (i) {
      case SLICE_TOP_LEFT:
        d = (Rectangle){ left, top, border, border };
        break;
      case SLICE_TOP_MID:
        d = (Rectangle){ left + border, top, midWidth, border };
        break;
      case SLICE_TOP_RIGHT:
        d = (Rectangle){ right - border, top, border, border };
        break;
      case SLICE_EDGE_LEFT:
        d = (Rectangle){ left, top + border, border, midHeight };
        break;
      case SLICE_CENTER:
        d = (Rectangle){ left + border, top + border, midWidth, midHeight };
        break;
      case SLICE_EDGE_RIGHT:
        d = (Rectangle){ right - border, top + border, border, midHeight };
        break;
      case SLICE_BOT_LEFT:
        d = (Rectangle){ left, bottom - border, border, border };
        break;
      case SLICE_BOT_MID:
        d = (Rectangle){ left + border, bottom - border, midWidth, border };
        break;
      case SLICE_BOT_RIGHT:
        d = (Rectangle){ right - border, bottom - border, border, border };
        break;
      default:
        continue;
    }
    Vector2 origin = VECTOR2_ZERO;
    DrawTexturePro(*uidata.sprite_sheet,src,d, origin, 0, WHITE);
  }
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

void SpriteLoadSlicedTextures(){
  for (int i = 0; i < ELEMENT_COUNT; i++){
    sub_texture_t sprData = UI_SPRITES[i];

    uidata.sprites[i] = malloc(sizeof(scaling_slice_t));
    uidata.sprites[i]->scale = 7.0f;
    int border_w = sprData.colliderPosX;
    int width = sprData.sourceWidth;
    int height = sprData.sourceHeight;
    int start_x = sprData.positionX;
    int start_y = sprData.positionY;
    
    Rectangle bounds[SLICE_ALL] = {
      /*TL*/{start_x,start_y, border_w, border_w},
      /*TM*/{start_x+border_w,start_y, width-2*border_w, border_w},
      /*TR*/{start_x+width-border_w,start_y, border_w, border_w},
      /*EL*/{start_x,border_w+start_y, border_w, height-2*border_w},
      /*MC*/{start_x+border_w,border_w+start_y, width-2*border_w, height-2*border_w},
      /*ER*/{start_x+width-border_w,border_w, border_w, height-2*border_w},
      {start_x,start_y+height-border_w, border_w, border_w},
      {start_x+border_w,start_y+height-border_w, width-2*border_w, border_w},
      {start_x+width-border_w,start_y+height-border_w, border_w, border_w},
    };
    
    for(int s = 0; s < SLICE_ALL; s++){
      sprite_slice_t *spr = malloc(sizeof(sprite_slice_t));
      memset(spr,0,sizeof(sprite_slice_t));

      spr->bounds = bounds[s];
      spr->center = VECTOR2_ZERO;
      spr->offset = VECTOR2_ZERO;
      spr->id = sprData.tag;
      spr->scale = 1.0f;
      uidata.sprites[i]->slices[s]=spr;
      uidata.sprites[i]->scaling[s]=scaling_rules[s].rules;
    }
  }
}

