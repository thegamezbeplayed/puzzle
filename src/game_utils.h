#ifndef __GAME_UTIL__
#define __GAME_UTIL__

#include <stdlib.h>
#include <string.h>
#include "game_common.h"

#define MAX_BEHAVIOR_TREE 8
#define MAX_NAME_LEN 64
#define COMBO_KEY(a, b) ((a << 8) | b)
#define CALL_FUNC(type, ptr, ...) ((type)(ptr))(__VA_ARGS__)
#define MAKE_ADAPTER(name, T) \
    static void name##_Adapter(void *p) { name((T)p); }

typedef struct ent_s ent_t;

//====FILE & STRINGS====>
char* GetFileStem(const char* filename);
//<==========
//
static inline void DO_NOTHING(void){}
static inline bool BOOL_DO_NOTHING(){return false;}
static inline const char* CHAR_DO_NOTHING(){return "\0";}
//<===BEHAVIOR TREES

//forward declare
typedef struct behavior_tree_node_s behavior_tree_node_t;

typedef struct {
    const char           *name;
    behavior_tree_node_t *tree;
} bt_register_entry_t;

behavior_tree_node_t *BehaviorGetTree( BehaviorID id);

typedef enum{
  BEHAVIOR_SUCCESS,
  BEHAVIOR_FAILURE,
  BEHAVIOR_RUNNING
}BehaviorStatus;

typedef enum{
  BT_LEAF,
  BT_SEQUENCE,
  BT_SELECTOR,
  BT_CONCURRENT
}BehaviorTreeType;

typedef struct {
  BehaviorID           id;
  behavior_tree_node_t *root;
} TreeCacheEntry;

extern TreeCacheEntry tree_cache[16];
extern int tree_cache_count;

static behavior_tree_node_t* BehaviorFindLeafFactory(const char *name);

typedef BehaviorStatus (*BehaviorTreeTickFunc)(behavior_tree_node_t* self, void* context);

typedef struct behavior_params_s{
  struct ent_s*         owner;
  EntityState           state;
  ObjectState           obj_state;
  EventType             event;
  CooldownCallback      eventFn;
  int                   duration;
}behavior_params_t;

behavior_tree_node_t *BuildTreeNode(BehaviorID id, behavior_params_t* parent_params);

typedef struct behavior_tree_node_s{
  BehaviorTreeType      bt_type;
  BehaviorTreeTickFunc  tick;
  void*                 data;
}behavior_tree_node_t;

typedef struct{
  behavior_tree_node_t  **children;
  int                   num_children;
  int                   current;
}behavior_tree_sequence_t;

typedef struct{
  behavior_tree_node_t  **children;
  int                   num_children;
  int                   current;
}behavior_tree_selector_t;

typedef BehaviorStatus (*BehaviorTreeLeafFunc)(behavior_params_t* params);

typedef struct{
  BehaviorTreeLeafFunc  action;
  behavior_params_t*    params;
}behavior_tree_leaf_t;

typedef struct {
    const char *name;  // "CanSeeTarget", "MoveToTarget", ...
    behavior_tree_node_t* (*factory)(behavior_params_t *params); // params is leaf-specific (can be NULL)
} BTLeafRegistryEntry;

behavior_tree_node_t* InitBehaviorTree( BehaviorID id);
void FreeBehaviorTree(behavior_tree_node_t* node);
BehaviorStatus BehaviorTickSequence(behavior_tree_node_t *self, void *context);
BehaviorStatus BehaviorTickSelector(behavior_tree_node_t *self, void *context);
BehaviorStatus BehaviorTickConcurrent(behavior_tree_node_t *self, void *context);
behavior_tree_node_t* BehaviorCreateLeaf(BehaviorTreeLeafFunc fn, behavior_params_t* params);
behavior_tree_node_t* BehaviorCreateSequence(behavior_tree_node_t **children, int count);
behavior_tree_node_t* BehaviorCreateSelector(behavior_tree_node_t **children, int count);
behavior_tree_node_t* BehaviorCreateConcurrent(behavior_tree_node_t **children, int count);

BehaviorStatus BehaviorChangeState(behavior_params_t *params);
BehaviorStatus BehaviorInitChild(behavior_params_t *params);
BehaviorStatus BehaviorMoveToTarget(behavior_params_t *params);
BehaviorStatus BehaviorMoveToDestination(behavior_params_t *params);
BehaviorStatus BehaviorSelectShape(behavior_params_t *params);

static inline behavior_tree_node_t* LeafInitChild(behavior_params_t *params)  { return BehaviorCreateLeaf(BehaviorInitChild,params); }
static inline behavior_tree_node_t* LeafChangeState(behavior_params_t *params)  { return BehaviorCreateLeaf(BehaviorChangeState,params); }
static inline behavior_tree_node_t* LeafSelectShape(behavior_params_t *params)  { return BehaviorCreateLeaf(BehaviorSelectShape,params); }
static inline behavior_tree_node_t* LeafMoveToTarget(behavior_params_t *params)  { return BehaviorCreateLeaf(BehaviorMoveToTarget,params); }
static inline behavior_tree_node_t* LeafMoveToDestination(behavior_params_t *params)  { return BehaviorCreateLeaf(BehaviorMoveToDestination,params); }
#endif
