#ifndef ROOM_DATA_H
#define ROOM_DATA_H

#define ROOM_WIDTH 1920
#define ROOM_HEIGHT 1080

#define GRID_WIDTH 3
#define GRID_HEIGHT 3

#define CELL_WIDTH 276
#define CELL_HEIGHT 276

#define ROOM_LEVEL_COUNT 3

typedef struct {
  ShapeID     id;
  int         points;
} ObjectInstance;

typedef struct {
  BackTileID  id;
  BehaviorID  behaviors[STATE_END];
} TileInstance;

static const ObjectInstance room_instances[SHAPE_COUNT] = {
  {SHAPE_NONE},
  {SHAPE_GREEN_SQUARE,1},
  {SHAPE_GREEN_DIAMOND,1},
  {SHAPE_GREEN_PENTAGON,1},
  {SHAPE_GREEN_STUD,1},
  {SHAPE_GREEN_TRIANGLE,1},
  {SHAPE_GREEN_COMPASS,1},
  {SHAPE_GREEN_STAR,1},
  {SHAPE_GREEN_GEM,1},
  {SHAPE_GREEN_CIRCLE,1},
  {SHAPE_GREEN_HEART,1},
  {SHAPE_GRAY_SQUARE,1},
  {SHAPE_GRAY_DIAMOND,1},
  {SHAPE_GRAY_PENTAGON,1},
  {SHAPE_GRAY_STUD,1},
  {SHAPE_GRAY_TRIANGLE,1},
  {SHAPE_GRAY_COMPASS,1},
  {SHAPE_GRAY_STAR,1},
  {SHAPE_GRAY_GEM,1},
  {SHAPE_GRAY_CIRCLE,1},
  {SHAPE_GRAY_HEART,1},
};

static const TileInstance BASE_TILE = {BACKTILE_01,{[STATE_EMPTY]=BEHAVIOR_ACQUIRE_CHILD,[STATE_SPAWN]=BEHAVIOR_ACQUIRE_CHILD}};

typedef struct {
  BehaviorID           id;
  bool                 is_root;
  BehaviorTreeType     bt_type;
  behavior_tree_node_t *(*func)(behavior_params_t *);
  bool          param_overide;
  EntityState   state;
  int           num_children;
  BehaviorID   children[5];
} BehaviorData;

static const BehaviorData room_behaviors[BEHAVIOR_COUNT] = {
  {BEHAVIOR_NONE},
  {BEHAVIOR_CHANGE_STATE,false,BT_LEAF,LeafChangeState,false, STATE_IDLE,0,{}},
  {BEHAVIOR_SELECT_SHAPE,false,BT_LEAF,LeafSelectShape,false, STATE_NONE,0,{}},
  {BEHAVIOR_INIT_CHILD,false,BT_LEAF,LeafInitChild,false, STATE_NONE,0,{}},
  {BEHAVIOR_ACQUIRE_CHILD,true,BT_SEQUENCE,NULL,true,STATE_IDLE,2, {BEHAVIOR_SELECT_SHAPE,BEHAVIOR_CHANGE_STATE}},
};
#endif
