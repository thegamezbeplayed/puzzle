#include <stdio.h>
#include <unistd.h>
#include "game_process.h"
#include "game_tools.h"
#include "game_helpers.h"
#include "game_utils.h"
#include "game_ui.h"

game_process_t game_process;
TreeCacheEntry tree_cache[16] = {0};
int tree_cache_count = 0;

bool TogglePause(ui_menu_t* m){
  if(game_process.state[SCREEN_GAMEPLAY] == GAME_READY)
    GameSetState(GAME_PAUSE);
  else if (game_process.state[SCREEN_GAMEPLAY] == GAME_PAUSE)
    GameSetState(GAME_READY);

  return true;
}

void GameSetState(GameState state){
  if(game_process.screen != SCREEN_GAMEPLAY)
    return;

  game_process.state[SCREEN_GAMEPLAY] = state;
}

void GameReady(void *context){
  WorldInitOnce();
  game_process.state[SCREEN_GAMEPLAY] = GAME_READY;
}

static world_t world;

void AddFloatingText(render_text_t *rt){
  for (int i = 0; i < MAX_EVENTS; i++){
    if(world.floatytext_used[i])
      continue;

    world.floatytext_used[i] = true;
    world.texts[i] = rt;
    return;
  }
}

int GridGetRow(int row, ent_t** out){
  int count = 0;
  for (int x = 0; x < GRID_WIDTH; x++) {
    ent_t *e = world.grid.combos[x][row]->tile;
    if (e)
      out[count++] = e;
  }

  return count;
}

int GridGetCol(int col,ent_t** out){
  int count = 0;
  for (int y = 0; y < GRID_HEIGHT; y++) {
    ent_t *e = world.grid.combos[col][y]->tile;
    if (e)
      out[count++] = e;
  }

  return count;
}

int GridCompare(ent_t* start, int num_others, ent_t** others, Cell* results){
  int matches = 0;
  ShapeFlags og_type = SHAPE_TYPE(start->child->shape);
  ShapeFlags og_col = SHAPE_COLOR(start->child->shape);
  ShapeFlags bonus_type = SHAPE_TYPE_NONE;
  ShapeFlags bonus_col = SHAPE_COLOR_NONE;
  
  if(start->shape){
    bonus_type = SHAPE_TYPE(start->shape);
    bonus_col = SHAPE_COLOR(start->shape);
  }
  
  for(int i = 0; i<num_others;i++){
    if(!IS_TYPE(others[i]->child->shape,og_type)){
      results[i].x = 1.0f;
      results[i].y = 1.0f;
      continue;
    }

    results[matches].x = 1;

    if(IS_COLOR(others[i]->child->shape,og_col))
      results[i].y=1;

    if(IS_TYPE(others[i]->child->shape,bonus_type))
      results[i].x+=1;

    if(IS_COLOR(others[i]->child->shape,bonus_col))
      results[i].y+=1;

    matches++;
  }

  return matches;
}

float WorldGetGridCombo(Cell intgrid){
  grid_combo_t *grid = world.grid.combos[intgrid.x][intgrid.y];

  return grid->color_mul->current + grid->type_mul->current;
}

Cell WorldGetMaxShapes(){
  return (Cell){(int)world.max_shape->current,(int)world.max_color->current};
}

int WorldGetShapeSums(int* out){
  for (int x = 0; x < GRID_WIDTH; x++){
    for (int y = 0; y < GRID_HEIGHT; y++){
      ent_t* t = world.grid.combos[x][y]->tile;
      if(!t->child)
        continue;
      ShapeFlags s = SHAPE_TYPE(t->child->shape);
      out[s]++;
    }
  }

  return (int)world.max_shape->current;
}

bool WorldTestGrid(void){
  ShapeID grid[GRID_WIDTH][GRID_HEIGHT];
  for(int x = 0; x < GRID_WIDTH; x++){
    for(int y = 0; y < GRID_HEIGHT; y++)
      grid[x][y] =world.grid.combos[x][y]->tile->child->shape;
  }

  return CanBeSolvedInMoves(grid,4);
}

bool WorldCheckGrid(ent_t *e,ent_t* owner){
  for(int x = 0; x < GRID_WIDTH; x++){
    for(int y = 0; y < GRID_HEIGHT; y++){
      SetState(world.grid.combos[x][y]->tile,STATE_CALCULATING, NULL);
    }
  }
  ent_t* row_comp[GRID_WIDTH];
  Cell row_results[GRID_WIDTH];
  ent_t* col_comp[GRID_HEIGHT];
  Cell col_results[GRID_HEIGHT];

  int row_start = GridGetRow(owner->intgrid_pos.y,row_comp);
  int col_start = GridGetCol(owner->intgrid_pos.x,col_comp);
  int rowComb = GridCompare(owner,row_start,row_comp,row_results);
  int colComb = GridCompare(owner,col_start,col_comp, col_results);

  for(int i = 0; i < rowComb; i++){
    if(rowComb == GRID_WIDTH){
      world.grid.turn_connections++;
      for(int j = 0; j <row_results[i].x;j++)
        StatIncrementValue(world.grid.combos[row_comp[i]->intgrid_pos.x][row_comp[i]->intgrid_pos.y]->type_mul,true);
      for(int k = 0; k < row_results[i].y;k++)
        StatIncrementValue(world.grid.combos[row_comp[i]->intgrid_pos.x][row_comp[i]->intgrid_pos.y]->color_mul,true);
    }
    else{
      StatEmpty(world.grid.combos[row_comp[i]->intgrid_pos.x][row_comp[i]->intgrid_pos.y]->type_mul);
      StatEmpty(world.grid.combos[row_comp[i]->intgrid_pos.x][row_comp[i]->intgrid_pos.y]->color_mul);

    }
  }

  for(int i = 0; i < colComb; i++)
    if(colComb == GRID_HEIGHT){
      world.grid.turn_connections++;

      for(int j = 0; j <col_results[i].x;j++)
        StatIncrementValue(world.grid.combos[col_comp[i]->intgrid_pos.x][col_comp[i]->intgrid_pos.y]->type_mul,true);
      for(int k = 0; k < col_results[i].y;k++)
        StatIncrementValue(world.grid.combos[col_comp[i]->intgrid_pos.x][col_comp[i]->intgrid_pos.y]->color_mul,true);
 
    } 
    else{
      //world.grid.combos[col_comp[i]->intgrid_pos.x][col_comp[i]->intgrid_pos.y]->type_mul=1;
      //world.grid.combos[col_comp[i]->intgrid_pos.x][col_comp[i]->intgrid_pos.y]->color_mul=1;

    }

  return (colComb == GRID_HEIGHT || rowComb == GRID_WIDTH);
}

bool CheckWorldTilesReady(void){
  for(int x = 0; x < GRID_WIDTH; x++){
    for(int y = 0; y < GRID_HEIGHT; y++)
      if(world.grid.combos[x][y]->tile->state!=STATE_IDLE)
        return false;
  }
  return true;
}

bool TurnSetState(TurnState state){
  if(!TurnCanChangeState(state))
    return false;

  world.grid.state = state;

  TraceLog(LOG_INFO,"Turn State now - %s",turn_name[state].name);
  TurnOnChangeState(state);
  return true;
}

bool TurnCanChangeState(TurnState state){
  if(state == world.grid.state)
    return false;

  switch(COMBO_KEY(world.grid.state, state)){
    default:
      return true;
      break;
  }

  return true;
}

void TurnOnChangeState(TurnState state){
  switch(state){
    case TURN_STANDBY:
      if(CheckWorldTilesReady())
        TurnSetState(TURN_START);
      break;
    case TURN_SCORE:
      WorldCalcGrid();
      break;
    case TURN_START:
      world.grid.turn++;
      if(world.grid.turn%19==0)
        StatIncrementValue(world.max_color,true);
      if(world.grid.turn%33==0)
        StatIncrementValue(world.max_shape,true);
      world.grid.turn_connections = 0;
      if(!WorldTestGrid())
        MenuSetState(&ui.menus[MENU_EXIT],MENU_ACTIVE);
        break;
    case TURN_END:
      if(world.grid.turn_connections > 0)
        world.combo_streak+=(int)(world.grid.turn_connections/3);
      else{
        StatEmpty(world.combo_mul);
        world.combo_streak = 0;
      }
      for(int x = 0; x < GRID_WIDTH; x++)
        for(int y = 0; y < GRID_HEIGHT; y++){
          StatEmpty(world.grid.combos[x][y]->type_mul);
          StatEmpty(world.grid.combos[x][y]->color_mul);
        
          if(world.grid.turn%21==0)
            world.grid.combos[x][y]->color_mul->increment+=0.25f;
          if(world.grid.turn%33==0)
            world.grid.combos[x][y]->type_mul->increment +=0.25f;

        }
      
      TurnSetState(TURN_STANDBY);
      break;
    default:
      break;
  }
}

void WorldCalcGrid(void){
  if(world.grid.turn_connections == 0){

    TurnSetState(TURN_END);
    return;
  }
  for(int i = 0; i < world.grid.turn_connections; i++)
    StatIncrementValue(world.combo_mul,true);

  for(int x = 0; x < GRID_WIDTH; x++){
    for(int y = 0; y < GRID_HEIGHT; y++){
      if(world.grid.combos[x][y]->type_mul->current == 1)
        continue;

      SetState(world.grid.combos[x][y]->tile->child,STATE_SCORE,EntAddPoints);
    }
  }

  TurnSetState(TURN_END);
}

bool CheckWorldGridAdjacent(ent_t* e, ent_t* other){

  return is_adjacent(e->intgrid_pos,other->intgrid_pos);
}

int WorldGetEnts(ent_t** results,EntFilterFn fn, void* params){
  int num_results = 0;
  for(int i = 0; i < world.num_ent; i++){
    if(!fn(world.ents[i],params))
      continue;

    results[num_results] = world.ents[i];
    num_results++;
  }

  return num_results;
}

bool RegisterBehaviorTree(BehaviorData data){
  TreeCacheEntry entry = {0};
  entry.id = data.id;
  entry.root = BuildTreeNode(data.id,NULL);
  tree_cache[tree_cache_count++] = entry;

  return entry.root!=NULL;
}

ent_t* WorldGetEnt(const char* name){
  return NULL;
}

ent_t* WorldGetEntById(unsigned int uid){
  for(int i = 0; i < world.num_ent; i++){
    if(world.ents[i]->uid == uid)
      return world.ents[i];
  }

  return NULL;
}

int RemoveSprite(int index){
  int last_pos = world.num_spr -1;
  
  if(!FreeSprite(world.sprs[index]))
    return 0;

  world.num_spr--;
  if(index!=last_pos){
    world.sprs[index] = world.sprs[last_pos];
    return 1;
  }
 
  return 0;
  
}

int RemoveEnt(int index){
  int last_pos = world.num_ent -1;

  if(!FreeEnt(world.ents[index]))
    return 0;

  world.num_ent--;
  if(index!=last_pos){
    world.ents[index] = world.ents[last_pos];
    return 1;
  }

  return 0;

}

int AddEnt(ent_t *e) {
  if (world.num_ent < MAX_ENTS) {
    int index = world.num_ent;
    world.ents[index] = e;
    world.num_ent++;

    return CURRENT_ENT_IDENTIFIER++;
  }
  return -1;
} 

int AddSprite(sprite_t *s){
  if(world.num_spr < MAX_ENTS){
    int index = world.num_spr;
    world.sprs[world.num_spr++] = s;

    return index;
  }

  return -1;
}

ObjectInstance GetObjectInstanceByShapeID(ShapeID id){
  for (int i = SHAPE_NONE; i<SHAPE_COUNT;i++){
    if(room_instances[i].id != id)
      continue;

    return room_instances[i];
  }
  
  return room_instances[0];
}

bool RegisterEnt( ent_t *e){
  e->uid = AddEnt(e);

  if(e->sprite)
    RegisterSprite(e->sprite);

  return e->uid > -1;
}

bool RegisterSprite(sprite_t *s){
  s->suid = AddSprite(s);

  return s->suid > -1;
}

void WorldInitOnce(){
  
  InteractionStep();

  TurnSetState(TURN_START);

}

void WorldPreUpdate(){
  InteractionStep();
  
  for(int i = 0; i < world.num_spr; i++){
    SpriteSync(world.sprs[i]);
  }
}

void WorldFixedUpdate(){
  for(int i = 0; i < world.num_ent; i++){
    switch(world.ents[i]->state){
      case STATE_END:
        i-=RemoveEnt(i);
        break;
      case STATE_DIE:
        EntDestroy(world.ents[i]);
        break;
      default:
        EntSync(world.ents[i]);
        break;
    }
  }
}

void WorldPostUpdate(){
  for(int i = 0; i < MAX_EVENTS; i++){
    if(!world.floatytext_used[i])
      continue;

    if(world.texts[i]->duration <=0){
      world.floatytext_used[i] =false;
      continue;
    }

    world.texts[i]->duration--;
  
  }
  if(world.grid.state != TURN_STANDBY)
    return;

  for(int x = 0; x < GRID_WIDTH; x++){
    for(int y = 0; y < GRID_HEIGHT; y++){
      if(world.grid.combos[x][y]->tile->state == STATE_CALCULATING){
        SetState(world.grid.combos[x][y]->tile,STATE_IDLE, NULL);
        return;
      }
    if(world.grid.combos[x][y]->tile->state != STATE_IDLE)
      return;
    }
  }

  TurnSetState(TURN_START);

}

void InitWorld(world_data_t data){
  world = (world_t){0};
  world.combo_streak = 0;
  world.combo_mul=InitStatOnMin(STAT_COMBO_MUL,1.0f,16.0F);
  world.combo_mul->increment = 0.25f;
  world.max_shape = InitStatOnMin(STAT_MAX_TYPE,SHAPE_TYPE_STUD,SHAPE_TYPE_MAX);
  world.max_color = InitStatOnMin(STAT_MAX_COLOR,SHAPE_COLOR_GRAY,SHAPE_COLOR_MAX);

  world.max_color->increment = 16;
  TraceLog(LOG_INFO,"Screen Width %02f",+VECTOR2_CENTER_SCREEN.x);
  float playX = (CELL_WIDTH/2)+VECTOR2_CENTER_SCREEN.x-(GRID_WIDTH*CELL_WIDTH)/2;
  
  TraceLog(LOG_INFO,"Start Pos x %02f",+playX);
  float playY = (XS_PANEL_THIN_SIZE.y/2)+(CELL_HEIGHT/2)+VECTOR2_CENTER_SCREEN.y-(GRID_HEIGHT*CELL_HEIGHT)/2;
  
  TraceLog(LOG_INFO,"Start Pos Y %02f",+playY);
  TraceLog(LOG_INFO,"HUD Height %02f",ui.menus[MENU_HUD].element->bounds.height);
  world.play_area =Rect(playX,playY,GRID_WIDTH*CELL_WIDTH,GRID_HEIGHT*CELL_HEIGHT);
  for(int x = 0; x < GRID_WIDTH; x++)
    for(int y = 0; y < GRID_HEIGHT; y++){
      Vector2 pos = {x*CELL_WIDTH + world.play_area.x,y*CELL_HEIGHT+world.play_area.y};
      ent_t* tile = InitEntStatic(BASE_TILE,pos);
      tile->intgrid_pos = (Cell){x,y};
      RegisterEnt(tile);
      world.grid.combos[x][y] = malloc(sizeof(grid_combo_t));
      world.grid.combos[x][y]->tile = tile;
      world.grid.combos[x][y]->type_mul = InitStatOnMin(STAT_TYPE_MUL,1.0f,10.0f);
      world.grid.combos[x][y]->color_mul = InitStatOnMin(STAT_COLOR_MUL,1.0f,10.0f);
      world.grid.combos[x][y]->type_mul->increment=0.25f;
      world.grid.combos[x][y]->color_mul->increment=0.5f;
    }

  for (int i = 0; i < data.num_ents; i++)
    RegisterEnt(InitEnt(data.ents[i]));
}

void FreeWorld(){
  for (int i = 0; i < world.num_spr; i++){
    RemoveSprite(i);
  }
  world.num_spr = 0;

  for (int i = 0; i < world.num_ent; i++){
    RemoveEnt(i);
  }
  world.num_ent = 0;
}

void WorldRender(){
  for(int i = 0; i < world.num_spr;i++)
    if(world.sprs[i]->owner !=NULL)
      DrawSprite(world.sprs[i]);
    else
      i-=RemoveSprite(i);

  for(int i = 0; i < MAX_EVENTS; i++){
    if(!world.floatytext_used[i])
      continue;
    render_text_t rt = *world.texts[i];
    DrawText(rt.text, rt.pos.x,rt.pos.y,rt.size,rt.color);
  }

}

void InitGameProcess(){
  for(int i = 0; i < BEHAVIOR_COUNT; i++){
    if(room_behaviors[i].is_root)
      RegisterBehaviorTree(room_behaviors[i]);
  }

  for(int s = 0; s<SCREEN_DONE; s++){
    game_process.album_id[s] = -1;
    for(int u = 0; u<UPDATE_DONE;u++){
      game_process.update_steps[s][u] = DO_NOTHING;
    
    }
    game_process.children[s].process= PROCESS_NONE;
    for(int p = 0; p < PROCESS_DONE; p++)
      game_process.children[s].state[p]=GAME_NONE;
  }

  game_process.next[SCREEN_TITLE] = SCREEN_GAMEPLAY;
  game_process.init[SCREEN_TITLE] = InitTitleScreen;
  game_process.finish[SCREEN_TITLE] = UnloadTitleScreen;
  game_process.update_steps[SCREEN_TITLE][UPDATE_DRAW] = DrawTitleScreen;
  game_process.update_steps[SCREEN_TITLE][UPDATE_FRAME] = UpdateTitleScreen;

  game_process.next[SCREEN_GAMEPLAY] = SCREEN_ENDING;
  game_process.init[SCREEN_GAMEPLAY] = InitGameplayScreen;
  game_process.finish[SCREEN_GAMEPLAY] = UnloadGameplayScreen;
  game_process.update_steps[SCREEN_GAMEPLAY][UPDATE_FIXED] = FixedUpdate;
  game_process.update_steps[SCREEN_GAMEPLAY][UPDATE_PRE] = PreUpdate;
  game_process.update_steps[SCREEN_GAMEPLAY][UPDATE_DRAW] = DrawGameplayScreen;
  game_process.update_steps[SCREEN_GAMEPLAY][UPDATE_FRAME] = UpdateGameplayScreen;
  game_process.update_steps[SCREEN_GAMEPLAY][UPDATE_POST] = PostUpdate;
   
  game_process.next[SCREEN_ENDING] = SCREEN_TITLE;
  game_process.init[SCREEN_ENDING] = InitEndScreen;
  game_process.finish[SCREEN_ENDING] = UnloadEndScreen;
  game_process.update_steps[SCREEN_ENDING][UPDATE_DRAW] = DrawEndScreen;
  game_process.update_steps[SCREEN_ENDING][UPDATE_FRAME] = UpdateEndScreen;

  game_process.screen = SCREEN_TITLE;
  game_process.state[SCREEN_GAMEPLAY] = GAME_LOADING;
  game_process.events = InitEvents();
  game_process.init[SCREEN_TITLE]();

}

void InitGameEvents(){
  world_data_t wdata = {0};

  cooldown_t* loadEvent = InitCooldown(6,EVENT_GAME_PROCESS,GameReady,NULL);
  AddEvent(game_process.events,loadEvent);
  InitWorld(wdata);
  game_process.children[SCREEN_GAMEPLAY].process = PROCESS_LEVEL;
  game_process.game_frames = 0; 
  MenuSetState(&ui.menus[MENU_PAUSE],MENU_READY);
}

bool GameTransitionScreen(){
  GameScreen current = game_process.screen;
  GameScreen prepare = game_process.next[current];
  if(game_process.state[current] >= GAME_FINISHED)
    return false;
  game_process.init[prepare]();
  game_process.state[current] = GAME_FINISHED;
  game_process.finish[current]();
  game_process.screen = prepare;
  game_process.state[prepare] = GAME_LOADING;
  return true;
}

void GameProcessStep(){
  if(game_process.screen != SCREEN_GAMEPLAY)
    return;

  game_process.game_frames++;
  if(game_process.events)
    StepEvents(game_process.events);
}

void GameProcessSync(bool wait){
  if(game_process.state[game_process.screen] > GAME_READY){

    game_process.update_steps[SCREEN_GAMEPLAY][UPDATE_DRAW]();
  
    return;
  }
  
  for(int i = 0; i <UPDATE_DONE;i++){
    if(i > UPDATE_DRAW && wait)
      return;
    game_process.update_steps[game_process.screen][i]();
  }

  //AudioStep();
  for(int i = 0; i < PROCESS_DONE;i++){
    if(game_process.children[game_process.screen].process==PROCESS_NONE)
      continue;
    child_process_t* kids = &game_process.children[game_process.screen];
    for(int j = 0; j < UPDATE_DONE; j++)
      if(kids->update_steps[i][j]!=NULL)
        kids->update_steps[i][j]();
  }
}

void GameProcessEnd(){
  UnloadEvents(game_process.events);
  FreeWorld();
  FreeInteractions();
}

void AddPoints(float mul,float points, Vector2 pos){
  //TraceLog(LOG_INFO,"===Add %0.2f Points===",points*mul);
  mul+=world.combo_mul->current;
  world.points+=mul*points;

  render_text_t *rt = malloc(sizeof(render_text_t));
  *rt = (render_text_t){
    .text = strdup(TextFormat("+%d",(int)(points*mul))),
    .pos = pos,
    .size = 24,
    .color =YELLOW,
    .duration = (int)(45+mul*9)
  };
  AddFloatingText(rt);
}

ShapeFlags WorldGetPossibleShape(){
  return world.max_shape->current;
}

const char* GetGameTime(){
  return TextFormat("%09i",(int)(game_process.game_frames/fixedFPS));
}

const char* GetPoints(){
  return TextFormat("%08i",GetPointsInt());
}

const char* GetTurn(){
  return TextFormat("%02i",world.grid.turn);
}

const char* GetComboStreak(){
  return TextFormat("%02i",GetComboInt());
}

int GetComboInt(){
  return world.combo_streak;

}
int GetPointsInt(){
  return (int)world.points;
}
