#include "game_utils.h"
#include <assert.h>

#define SHAPE_COLOR(shape) ((shape) & SHAPE_COLOR_MASK)
#define SHAPE_TYPE(shape)  ((shape) & SHAPE_TYPE_MASK)

#define IS_COLOR(shape, color) (SHAPE_COLOR(shape) == (color))
#define IS_TYPE(shape, type)   (SHAPE_TYPE(shape) == (type))
#define IS_ANY_COLOR(shape)    ((shape) & SHAPE_COLOR_MASK)
#define IS_ANY_TYPE(shape)     ((shape) & SHAPE_TYPE_MASK)

ShapeID SelectRandomShape(ShapeFlags color, ShapeFlags shape){
  assert((color & ~SHAPE_COLOR_MASK) == 0 && "Invalid color bits set");
  assert((shape & ~SHAPE_TYPE_MASK) == 0 && "Invalid shape bits set");

  if(color == SHAPE_COLOR_NONE)
    color = ((rand() %2)+1) << 4;

  if(shape == SHAPE_TYPE_NONE)
    shape = (rand()%9)+1; 

  return (ShapeID)(color | shape);
}
