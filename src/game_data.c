#include <raylib.h>
#include "game_tools.h"
#include "game_process.h"

#define RATIO(s) ((s).ratio(&(s)))

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

stat_t* InitStatOnMin(StatType attr, float min, float max){
 stat_t* s = malloc(sizeof(stat_t));
 *s =(stat_t){
    .attribute = attr,
      .min = min,
      .max = max,
      .current = min,
      .ratio = StatGetRatio,
      .increment = 1.0f
  }; return s;
}

stat_t InitStatOnMax(StatType attr, float val){}
stat_t InitStatEmpty(void){}
void InitStats(stat_t stats[STAT_BLANK]){}
bool StatIncrementValue(stat_t* attr,bool increase){
  float inc = attr->increment;
  if(!increase)
    inc*=-1;

  float old = attr->current;
  attr->current+=attr->increment;
  attr->current = CLAMPF(attr->current,attr->min, attr->max);
  float cur = attr->current;
  return true;
}

bool StatChangeValue(struct ent_s* owner, stat_t* attr, float val){}
void StatMaxOut(stat_t* s){}
void StatEmpty(stat_t* s){
  s->current = s->min;
}

float StatGetRatio(stat_t *self){}
