#ifndef __GAME_TOOLS__
#define __GAME_TOOLS__
#include <float.h>
#include "game_math.h"
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>

#define CLAMPV2(v,a,b) ((v)<(a)?(a):((v)>(b)?(b):(v)))
#define VEC_UNSET (Vector2){FLT_MAX, FLT_MAX}
#define EPS 1e-6f
#define VECTOR2_CENTER_SCREEN   (Vector2){ GetScreenWidth()/2, GetScreenHeight()/2}
#define VECTOR2_SCREEN   (Vector2){ GetScreenWidth(), GetScreenHeight()}
#define VECTOR2_ZERO   (Vector2){ 0.0f, 0.0f}
#define VECTOR2_ONE    (Vector2){ 1.0f, 1.0f }
#define VECTOR2_UP     (Vector2){ 0.0f, 1.0f }
#define VECTOR2_DOWN   (Vector2){ 0.0f,-1.0f }
#define VECTOR2_LEFT   (Vector2){1.0f, 0.0f }
#define VECTOR2_RIGHT  (Vector2){ -1.0f, 0.0f }
#define Vector2X(x) ((Vector2){ (x), 0.0f })
#define Vector2Y(y) ((Vector2){ 0.0f, (y) })
#define Vector2Inc(v,xi,yi) ((Vector2){ (v.x+xi), (v.y+yi) })

#define Rect(px,py,sx,sy) ((Rectangle){ (px),(py), (sx), (sy) })
#define RECT_ZERO   (Rectangle){ 0.0f, 0.0f,0.0f,0.0f}
#define RectInc(r,xi,yi) ((Rectangle){ (r.x+xi), (r.y+yi),(r.width),(r.height) })

static void shuffle_array(void *base, size_t n, size_t size) {
    char *arr = base;
    for (size_t i = n - 1; i > 0; i--) {
        size_t j = rand() % (i + 1);
        char tmp[size];
        memcpy(tmp, arr + i * size, size);
        memcpy(arr + i * size, arr + j * size, size);
        memcpy(arr + j * size, tmp, size);
    }
}

static uint32_t hash_str(const char *str) {
    uint32_t hash = 5381; // djb2 starting seed
    int c;
    while ((c = *str++))
        hash = ((hash << 5) + hash) + (uint32_t)c; // hash * 33 + c
    return hash;
}

typedef struct {
  int x,y;
} Cell;

static inline float distance(int x1, int y1, int x2, int y2) {
    int dx = x2 - x1;
    int dy = y2 - y1;
    return sqrtf(dx*dx + dy*dy);
}

static inline float frand() {
    return (float)rand() / (float)RAND_MAX;
}

static inline Vector2 Vector2Avg(Vector2 a, Vector2 b){
 return (Vector2){ (a.x + b.x) * 0.5f,
                      (a.y + b.y) * 0.5f };
}

static inline Vector2 CellToVector2(Cell c, float scale){
  Vector2 result = Vector2FromXY(c.x,c.y);

  return Vector2Scale(result,scale);
}
static inline bool v2_compare(Vector2 v1,Vector2 v2){
  return (v1.x==v2.x && v1.y==v2.y);
}
static inline float v2_ang_deg(Vector2 v){ return atan2(v.y,v.x)*180.0 /M_PI;}
static inline float v2_ang(Vector2 v){ return atan2(v.y,v.x);}
static inline float v2_len(Vector2 v){ return sqrtf(v.x*v.x + v.y*v.y); }
static inline Vector2 v2_add(Vector2 a, Vector2 b){ return (Vector2){a.x+b.x,a.y+b.y}; }
static inline Vector2 v2_sub(Vector2 a, Vector2 b){ return (Vector2){a.x-b.x,a.y-b.y}; }
static inline Vector2 v2_scale(Vector2 a, float s){ return (Vector2){a.x*s,a.y*s}; }
static inline Vector2 v2_norm_safe(Vector2 v){
  float L = v2_len(v);
  return (L > 1e-6f) ? v2_scale(v, 1.0f/L) : (Vector2){1,0};
}


static inline int point_in_rect(Vector2 p, Rectangle r){
  return (p.x >= r.x && p.x <= r.x + r.width &&
      p.y >= r.y && p.y <= r.y + r.height);
}

static inline Vector2 clamp_point_to_rect(Vector2 p, Rectangle r){
  return (Vector2){
    CLAMPV2(p.x, r.x, r.x + r.width),
      CLAMPV2(p.y, r.y, r.y + r.height)
  };
}

// Random unit vector (use your RNG if needed)
static inline Vector2 rand_unit(){
  float a = ((float)rand() / (float)RAND_MAX) * 6.28318530718f;
  return (Vector2){cosf(a), sinf(a)};
}

// Distance you can move along dir before leaving rect (start assumed INSIDE).
// Returns a "t_max" so p + dir * t_max is the last point still inside (minus epsilon).
static float max_step_inside_rect(Vector2 p, Vector2 dir, Rectangle r){
  const float eps = 1e-4f;
  float tmax = FLT_MAX;

  if (fabsf(dir.x) > 1e-8f) {
    float tx = (dir.x > 0.0f)
      ? ((r.x + r.width) - p.x) / dir.x
      : ( (r.x) - p.x ) / dir.x;
    if (tx >= 0.0f) tmax = fminf(tmax, tx);
  }
  if (fabsf(dir.y) > 1e-8f) {
    float ty = (dir.y > 0.0f)
      ? ((r.y + r.height) - p.y) / dir.y
      : ( (r.y) - p.y ) / dir.y;
    if (ty >= 0.0f) tmax = fminf(tmax, ty);
  }
  if (tmax == FLT_MAX) tmax = 0.0f; // dir == (0,0)
  tmax = fmaxf(0.0f, tmax - eps);
  return tmax;
}

//TODO this doesnt do what i think..
static inline Vector2 GetNearbyDestination(Vector2 curr, float len, Rectangle area, float bias,float padding){
  // Clamp start to inside
  curr = clamp_point_to_rect(curr, area);

  
  float angle = frand() * 6.28318530718f;

  Vector2 result = v2_add(curr,Vector2FromAngle(angle,len));

  return clamp_point_to_rect(result,RectangleCrop(area,padding,padding));
}

#endif
