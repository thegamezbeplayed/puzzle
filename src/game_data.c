#include <raylib.h>
#include "game_tools.h"
#include "game_process.h"
void UploadScore(void){
TraceLog(LOG_INFO,"Call EMSCRIPTEN");
#ifdef __EMSCRIPTEN__
  
TraceLog(LOG_INFO,"Call JS");
EM_ASM({
      sendScore($0);
      }, GetPointsInt());
#endif
}
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
