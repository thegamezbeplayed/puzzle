#include "game_types.h"
#include "game_process.h"

game_object_t* InitObjectStatic(SpawnerInstance inst){
  game_object_t* obj = malloc(sizeof(game_object_t));
  *obj = (game_object_t){0};

  obj->pos = (Vector2) {inst.x,inst.y};

  for (int i =0; i < ENT_BLANK; i++){
    if(inst.spawn_refs[i] == 0)
      continue;
    for(int j = 0; j < inst.spawn_refs[i]; j++)
      RegisterPoolRef(inst.id, i);
  
  }

  return obj;
}
