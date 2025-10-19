#include <raylib.h>
#include "game_utils.h"

// Allocates a copy of the filename without extension
char* GetFileStem(const char* filename) {
    const char* dot = strrchr(filename, '.');
    size_t len = dot ? (size_t)(dot - filename) : strlen(filename);

    char* stem = malloc(len + 1);
    if (!stem) return NULL;
    memcpy(stem, filename, len);
    stem[len] = '\0';
    return stem;
}

EntityState EntityStateLookup(const char* name){
  for (int i = 0; i < STATE_END; i++){
    if(strcmp(name, ent_state_alias[i].name) == 0)
      return ent_state_alias[i].state;
  }

  TraceLog(LOG_WARNING,"<====GAME_DATA====>\n====== State Definition for %s not implemented!",name);
  return 0;

}

const char* EntityStateName(EntityState s){
  for (int i = 0; i < STATE_END; i++){
    if(ent_state_alias[i].state == s)
      return ent_state_alias[i].name;
  }

  TraceLog(LOG_WARNING,"<====GAME_DATA====>\n====== State Definition for %d not implemented!",s);
  return 0;

}
