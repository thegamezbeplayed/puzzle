#ifndef __GAME_HELPERS__
#define __GAME_HELPERS__

#include <assert.h>

#define SHAPE_COLOR(shape) ((shape) & SHAPE_COLOR_MASK)
#define SHAPE_TYPE(shape)  ((shape) & SHAPE_TYPE_MASK)

#define IS_COLOR(shape, color) (SHAPE_COLOR(shape) == (color))
#define IS_TYPE(shape, type)   (SHAPE_TYPE(shape) == (type))
#define IS_ANY_COLOR(shape)    ((shape) & SHAPE_COLOR_MASK)
#define IS_ANY_TYPE(shape)     ((shape) & SHAPE_TYPE_MASK)

static bool HasMatch(ShapeID grid[GRID_WIDTH][GRID_HEIGHT]) {
  // check rows
  for (int y = 0; y < GRID_HEIGHT; y++) {
    for (int x = 0; x < GRID_WIDTH - 2; x++) {

      if (IS_TYPE(grid[y][x],SHAPE_TYPE(grid[y][x+1])) &&
          IS_TYPE(grid[y][x],SHAPE_TYPE(grid[y][x+2])))
        return true;
    }
  }

  // check columns
  for (int x = 0; x < GRID_WIDTH; x++) {
    for (int y = 0; y < GRID_HEIGHT - 2; y++) {
      if (IS_TYPE(grid[y][x],SHAPE_TYPE(grid[y+1][x])) &&
          IS_TYPE(grid[y][x],SHAPE_TYPE(grid[y+2][x])))
        return true;
    }
  }

  return false;
}

static bool CanBeSolvedInMoves(ShapeID grid[GRID_WIDTH][GRID_HEIGHT], int depth) {
    if (HasMatch(grid)) return true;
    if (depth <= 0) return false;

    for (int y = 0; y < GRID_HEIGHT; y++) {
        for (int x = 0; x < GRID_WIDTH; x++) {
            int dx[2] = {1, 0};
            int dy[2] = {0, 1};

            for (int d = 0; d < 2; d++) {
                int nx = x + dx[d], ny = y + dy[d];
                if (nx >= 3 || ny >= 3) continue;

                ShapeID newGrid[GRID_HEIGHT][GRID_WIDTH];
                memcpy(newGrid, grid, sizeof(newGrid));

                // Swap
                ShapeID tmp = newGrid[y][x];
                newGrid[y][x] = newGrid[ny][nx];
                newGrid[ny][nx] = tmp;

                if (CanBeSolvedInMoves(newGrid, depth - 1)){
                  TraceLog(LOG_INFO,"Solve in %d moves",depth);
                  return true;
                }
            }
        }
    }
    return false;
}

static bool CanBeSolved(ShapeID grid[GRID_WIDTH][GRID_HEIGHT]) {
  for (int y = 0; y < GRID_HEIGHT; y++) {
    for (int x = 0; x < GRID_WIDTH; x++) {
      // Try horizontal swap
      if (x + 1 < GRID_WIDTH) {
        int temp = grid[y][x];
        grid[y][x] = grid[y][x+1];
        grid[y][x+1] = temp;
        bool result = HasMatch(grid);
        // Swap back
        temp = grid[y][x];
        grid[y][x] = grid[y][x+1];
        grid[y][x+1] = temp;
        if (result) return true;
      }

      // Try vertical swap
      if (y + 1 < GRID_HEIGHT) {
        int temp = grid[y][x];
        grid[y][x] = grid[y+1][x];
        grid[y+1][x] = temp;
        bool result = HasMatch(grid);
        // Swap back
        temp = grid[y][x];
        grid[y][x] = grid[y+1][x];
        grid[y+1][x] = temp;
        if (result) return true;
      }
    }
  }
  return false;
}

static int SelectWeightedIndex(const int *weights, int count)
{
  if (!weights || count <= 0) return -1;

  // Compute total weight
  int total = 0;
  for (int i = 0; i < count; i++)
    if (weights[i] > 0) total += weights[i];

  if (total <= 0) return -1; // nothing to choose from

  // Pick a random point between [0, total)
  int r = rand() % total;

  // Find which index corresponds to that random range
  int cumulative = 0;
  for (int i = 0; i < count; i++) {
    if (weights[i] <= 0) continue;
    cumulative += weights[i];
    if (r < cumulative)
      return i;
  }

  return count - 1; // fallback (should not happen)
}

static ShapeID SelectRandomShape(ShapeFlags color, ShapeFlags shape){
  assert((color & ~SHAPE_COLOR_MASK) == 0 && "Invalid color bits set");
  assert((shape & ~SHAPE_TYPE_MASK) == 0 && "Invalid shape bits set");

  if(color == SHAPE_COLOR_NONE)
    color = ((rand() %2)+1) << 4;

  if(shape == SHAPE_TYPE_NONE)
    shape = (rand()%9)+1; 

  return (ShapeID)(color | shape);
}

static ShapeID SelectShapeFromRange(ShapeFlags color, ShapeFlags start, ShapeFlags end){
  if(color == SHAPE_COLOR_NONE)
    color = ((rand() %2)+1) << 4;

  ShapeFlags shape = (rand()%(int)end)+start;

  return (ShapeID)(color | shape);

}

static ShapeID SelectHelpfulShape(){
  int shapes[SHAPE_TYPE_MASK]={0};

  int num_shapes = WorldGetShapeSums(shapes);
  for(int i = 0; i < num_shapes ; i++){
    if(shapes[i] > 2)
      shapes[i] = 0;
    else{
      shapes[i] = 3-shapes[i];
      shapes[i]*=shapes[i];
    }
  }

  ShapeFlags color = ((rand() %2)+1) << 4;
  ShapeFlags shape = SelectWeightedIndex(shapes,num_shapes);

  if(shape<1)
    shape = (rand()%(int)num_shapes)+1;

  return (ShapeID)(color|shape);
}

#endif
