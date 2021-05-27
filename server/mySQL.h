#include "data.h"

void open_db(sqlite3 **db);
void creat_user_db(sqlite3 *db,char **errmsg);
void creat_online_db(sqlite3 *db,char **errmsg);

void is_sqlite_ok(int rc);
int read_user(sqlite3 *db,char **errmsg,char *user);
void reg_db(sqlite3 *db,char **errmsg,char *name,char *passwd);
int read_online_ok(sqlite3 *db,char **errmsg,char *user);
int read_pass(sqlite3 *db,char **errmsg,char *user,char *passwd);
void write_online_all(sqlite3 *db,char **errmsg,vpChat temp);
void insert_server_db(sqlite3 *db,char *time,char **errmsg);
void delete_online_db(sqlite3 *db,char **errmsg,int sockfd);
void log_db(sqlite3 *db,char **errmsg,char *name,int sockfd);
void is_sqlite(int rc);