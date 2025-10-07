#include <raylib.h>
#include "game_utils.h"

static MYSQL *conn;
int InitDB(){
  conn = mysql_init(NULL);

  TraceLog(LOG_INFO,("<====!MYSQL!====>"));
  if (conn == NULL) {
    TraceLog(LOG_WARNING,"====mysql_init() failed====");
    return EXIT_FAILURE;
  }

  if (mysql_real_connect(conn, "localhost", "root", "", "bulletshine", 0, NULL, 0) == NULL) {
    TraceLog(LOG_WARNING,"====mysql_real_connect() failed===");
    mysql_close(conn);
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
bool DataUploadScore(const char* user, int score){
  player_score_t results = DataGetUserRow(user);

  if(results.valid && results.score > score)
    return false;

  if(!results.valid)
    DataInsertScore(user,score);
  else
    DataUpdateScore(results.id,score);

  return true;
}

player_score_t DataGetUserRow(const char* user){
  MYSQL_STMT *stmt;
  MYSQL_BIND bind[2];
  MYSQL_BIND param[1];
  
  player_score_t row;
  row.id = -1;
  row.score = 0;
  row.valid = false;
  const char* query = "SELECT id,score FROM player_scores WHERE player LIKE (?)";

  stmt = mysql_stmt_init(conn);

  if (mysql_stmt_prepare(stmt, query, strlen(query))) {
    TraceLog(LOG_ERROR, "mysql_stmt_prepare() failed: %s\n", mysql_error(conn));
  }

  memset(bind, 0, sizeof(bind));
  // Bind output: int id
  unsigned long length;
  my_bool is_null;

  bind[0].buffer_type = MYSQL_TYPE_LONG;
  bind[0].buffer = (char *)&row.id;
  bind[0].buffer_length = sizeof(row.id);
  bind[0].length = &length;
  bind[0].is_null = &is_null;

  bind[1].buffer_type   = MYSQL_TYPE_LONG;
  bind[1].buffer        = (char *)&row.score;
  bind[1].is_null       = 0;
  bind[1].length        = 0;

  memset(param, 0, sizeof(param));
  param[0].buffer_type = MYSQL_TYPE_STRING;
  param[0].buffer = strdup(user);
  param[0].buffer_length = strlen(user);
  mysql_stmt_bind_param(stmt, param);

  mysql_stmt_execute(stmt);

  mysql_stmt_bind_result(stmt, bind);

  int status = mysql_stmt_fetch(stmt);
  if(status == MYSQL_NO_DATA)
   row.valid = false; 
  else
    row.valid = true;

  mysql_stmt_free_result(stmt);
  mysql_stmt_close(stmt);
  
  return row;
}

bool DataUpdateScore(int row_id, int score){
  MYSQL_STMT *stmt;
  MYSQL_BIND bind[2];

  const char* query = "UPDATE player_scores SET score = ? WHERE id = ?";

  stmt = mysql_stmt_init(conn);

  if (mysql_stmt_prepare(stmt, query, strlen(query))) {
    TraceLog(LOG_ERROR, "mysql_stmt_prepare() failed: %s\n", mysql_error(conn));
  }

  if (mysql_stmt_prepare(stmt, query, strlen(query))) {
    TraceLog(LOG_ERROR, "mysql_stmt_prepare() failed: %s\n", mysql_error(conn));
  }

  memset(bind, 0, sizeof(bind));

  // Bind score
  bind[0].buffer_type   = MYSQL_TYPE_LONG;
  bind[0].buffer        = (char *)&score;
  bind[0].is_null       = 0;
  bind[0].length        = 0;

  // Bind id
  bind[1].buffer_type   = MYSQL_TYPE_LONG;
  bind[1].buffer        = (char *)&row_id;
  bind[1].is_null       = 0;
  bind[1].length        = 0;
  // Bind params to statement
  if (mysql_stmt_bind_param(stmt, bind)) {
    TraceLog(LOG_ERROR, "mysql_stmt_bind_param() failed: %s\n", mysql_stmt_error(stmt));
  }

  // Execute
  if (mysql_stmt_execute(stmt)) {
    TraceLog(LOG_ERROR, "mysql_stmt_execute() failed: %s\n", mysql_stmt_error(stmt));
  }

  int affected = mysql_stmt_affected_rows(stmt);
  TraceLog(LOG_INFO, "Updated score=%d for row id=%d (affected %d rows)",
      score, row_id, affected);

  mysql_stmt_close(stmt);
  return affected > 0;
}

bool DataInsertScore(const char* user, int score){

  MYSQL_STMT *stmt;
  MYSQL_BIND bind[2];
  const char *query = "INSERT INTO player_scores (player, score) VALUES(?, ?)";

  stmt = mysql_stmt_init(conn);
  if (mysql_stmt_prepare(stmt, query, strlen(query))) {
    TraceLog(LOG_ERROR, "mysql_stmt_prepare() failed: %s\n", mysql_error(conn));
  }

  memset(bind, 0, sizeof(bind));

  bind[0].buffer_type = MYSQL_TYPE_STRING;
  bind[0].buffer = strdup(user);
  bind[0].buffer_length = strlen(user);

  bind[1].buffer_type = MYSQL_TYPE_LONG;
  bind[1].buffer = (char *) &score;
  bind[1].is_null = 0;
  bind[1].length = 0;
  // Bind params to statement
  if (mysql_stmt_bind_param(stmt, bind)) {
    TraceLog(LOG_ERROR, "mysql_stmt_bind_param() failed: %s\n", mysql_stmt_error(stmt));
  }

  // Execute
  if (mysql_stmt_execute(stmt)) {
    TraceLog(LOG_ERROR, "mysql_stmt_execute() failed: %s\n", mysql_stmt_error(stmt));
  }

  TraceLog(LOG_INFO,"Inserted player %s with score %d\n", user, score);

  mysql_stmt_close(stmt);
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
