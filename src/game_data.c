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

bool DataUploadScore(player_score_t *up){
  player_score_t results = DataGetUserRow(up->name);

  if(results.valid && results.score > up->score)
    return false;

  if(!results.valid)
    DataInsertScore(up);
  else{
    up->id = results.id;
    DataUpdateScore(up);
  }

  return true;
}
player_score_t* DataGetSortedRows(int *out_count){
MYSQL_STMT *stmt;
    MYSQL_RES *res;
    MYSQL_BIND bind[3];
    MYSQL_ROW row;

    *out_count = 0;

    const char* query = "SELECT player,score,wave FROM player_scores ORDER BY score DESC";

    stmt = mysql_stmt_init(conn);
    if (!stmt) {
        TraceLog(LOG_ERROR, "mysql_stmt_init() failed");
        return NULL;
    }

    if (mysql_stmt_prepare(stmt, query, strlen(query))) {
        TraceLog(LOG_ERROR, "mysql_stmt_prepare() failed: %s", mysql_stmt_error(stmt));
        mysql_stmt_close(stmt);
        return NULL;
    }

    if (mysql_stmt_execute(stmt)) {
        TraceLog(LOG_ERROR, "mysql_stmt_execute() failed: %s", mysql_stmt_error(stmt));
        mysql_stmt_close(stmt);
        return NULL;
    }

    // Get metadata for column info
    res = mysql_stmt_result_metadata(stmt);
    if (!res) {
        TraceLog(LOG_ERROR, "mysql_stmt_result_metadata() failed: %s", mysql_stmt_error(stmt));
        mysql_stmt_close(stmt);
        return NULL;
    }

    // Temporary variables for each column
    int wave;
    char name[64];
    int score;

    memset(bind, 0, sizeof(bind));

    bind[0].buffer_type = MYSQL_TYPE_STRING;
    bind[0].buffer = (char *)name;
    bind[0].buffer_length = sizeof(name);

    bind[1].buffer_type = MYSQL_TYPE_LONG;
    bind[1].buffer = (char *)&score;

    bind[2].buffer_type = MYSQL_TYPE_LONG;
    bind[2].buffer = (char *)&wave;


    if (mysql_stmt_bind_result(stmt, bind)) {
        TraceLog(LOG_ERROR, "mysql_stmt_bind_result() failed: %s", mysql_stmt_error(stmt));
        mysql_free_result(res);
        mysql_stmt_close(stmt);
        return NULL;
    }

    // Store results in client
    if (mysql_stmt_store_result(stmt)) {
        TraceLog(LOG_ERROR, "mysql_stmt_store_result() failed: %s", mysql_stmt_error(stmt));
        mysql_free_result(res);
        mysql_stmt_close(stmt);
        return NULL;
    }

    int num_rows = mysql_stmt_num_rows(stmt);
    *out_count = num_rows;

    player_score_t *scores = malloc(sizeof(player_score_t) * num_rows);
    if (!scores) {
        TraceLog(LOG_ERROR, "malloc failed");
        mysql_free_result(res);
        mysql_stmt_close(stmt);
        return NULL;
    }

    int i = 0;
    while (mysql_stmt_fetch(stmt) == 0) {
        scores[i].wave = wave;
        strncpy(scores[i].name, name, sizeof(scores[i].name) - 1);
        scores[i].name[sizeof(scores[i].name) - 1] = '\0';
        scores[i].score = score;
        i++;
    }

    mysql_free_result(res);
    mysql_stmt_close(stmt);
    return scores;
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

bool DataUpdateScore(player_score_t* up){
  MYSQL_STMT *stmt;
  MYSQL_BIND bind[3];

  const char* query = "UPDATE player_scores SET score = ?, wave = ? WHERE id = ?";

  stmt = mysql_stmt_init(conn);

  if (mysql_stmt_prepare(stmt, query, strlen(query))) {
    TraceLog(LOG_ERROR, "mysql_stmt_prepare() failed: %s\n", mysql_error(conn));
  }

  memset(bind, 0, sizeof(bind));

  // Bind score
  bind[0].buffer_type   = MYSQL_TYPE_LONG;
  bind[0].buffer        = (char *)&up->score;
  bind[0].is_null       = 0;
  bind[0].length        = 0;

  bind[1].buffer_type   = MYSQL_TYPE_LONG;
  bind[1].buffer        = (char *)&up->wave;
  bind[1].is_null       = 0;
  bind[1].length        = 0;


  // Bind id
  bind[2].buffer_type   = MYSQL_TYPE_LONG;
  bind[2].buffer        = (char *)&up->id;
  bind[2].is_null       = 0;
  bind[2].length        = 0;
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
      up->score, up->id, affected);

  mysql_stmt_close(stmt);
  return affected > 0;
}

bool DataInsertScore(player_score_t* in){

  MYSQL_STMT *stmt;
  MYSQL_BIND bind[3];
  const char *query = "INSERT INTO player_scores (player,score,wave) VALUES(?, ?, ?)";

  stmt = mysql_stmt_init(conn);
  if (mysql_stmt_prepare(stmt, query, strlen(query))) {
    TraceLog(LOG_ERROR, "mysql_stmt_prepare() failed: %s\n", mysql_error(conn));
  }

  memset(bind, 0, sizeof(bind));

  bind[0].buffer_type = MYSQL_TYPE_STRING;
  bind[0].buffer = in->name;
  bind[0].buffer_length = strlen(in->name);

  bind[1].buffer_type = MYSQL_TYPE_LONG;
  bind[1].buffer = (char *) &in->score;
  bind[1].is_null = 0;
  bind[1].length = 0;

  bind[2].buffer_type = MYSQL_TYPE_LONG;
  bind[2].buffer = (char *) &in->wave;
  bind[2].is_null = 0;
  bind[2].length = 0;
  // Bind params to statement
  if (mysql_stmt_bind_param(stmt, bind)) {
    TraceLog(LOG_ERROR, "mysql_stmt_bind_param() failed: %s\n", mysql_stmt_error(stmt));
  }

  // Execute
  if (mysql_stmt_execute(stmt)) {
    TraceLog(LOG_ERROR, "mysql_stmt_execute() failed: %s\n", mysql_stmt_error(stmt));
  }

  TraceLog(LOG_INFO,"Inserted player %s with score %d\n",in->name, in->score);

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
