#include "data.h"

void is_sqlite_ok(int rc) {
    if (rc == SQLITE_OK) {
        printf("sqlite %d succse \n", __LINE__);
    } else {
        printf("sqlite %d error\n", __LINE__);
    }
}

/**
 * @description: test database
 * @param {int} rc sqlite state
 * @return {*}
 */
void is_sqlite(int rc) {
    if (rc == SQLITE_OK) {
        printf("sqlite %d succse \n", __LINE__);
    } else {
        printf("An error occurred in the database, please use SQLITE3 to view the database!\n");
        printf("sqlite %d error\n", __LINE__);
        exit(1);
    }
}

/**
 * @description: open database
 * @param {sqlite3} * pointer of database
 * @return {*}
 */
void open_db(sqlite3 **db) {
    int rc;
    rc = sqlite3_open("server.db", db);
    is_sqlite(rc);
}

/**
 * @description: create user data table
 * @param {sqlite3} *db database
 * @param {char} *
 * @return {*}
 */
void creat_user_db(sqlite3 *db, char **errmsg) {
    int rc;
    rc = sqlite3_exec(db,
                      "create table if not exists user(id integer primary key "
                      "autoincrement,name text,passwd text)",
                      NULL, NULL, errmsg);
    is_sqlite_ok(rc);
}

/**
 * @description: create online data table
 * @param {sqlite3} *db database
 * @param {char} *
 * @return {*}
 */
void creat_online_db(sqlite3 *db, char **errmsg) {
    int rc;
    sqlite3_exec(db, "drop table online", NULL, NULL, errmsg);
    rc = sqlite3_exec(db,
                      "create table online(id integer primary key "
                      "autoincrement,name text,socket integer,flag integer)",
                      NULL, NULL, errmsg);
    is_sqlite_ok(rc);
}

/**
 * @description: query whether this user existed
 * @param {sqlite3} *db database
 * @param {char} *
 * @param {char} *user username
 * @return {*}
 */
int read_user(sqlite3 *db, char **errmsg, char *user) {
    int rc;
    int i;
    sqlite3_stmt *stmt = NULL;
    rc = sqlite3_prepare(db, "select * from user", -1, &stmt, 0);
    is_sqlite_ok(rc);
    rc = sqlite3_step(stmt);
    int userflag = 1;
    while (rc == SQLITE_ROW) {
        userflag = strcmp(user, sqlite3_column_text(stmt, 1));
        if (userflag == 0) {
            return USERIN;
        }
        rc = sqlite3_step(stmt);
    }
    return USEROUT;
}

/**
 * @description: insert a new registered message
 * @param {sqlite3} *db database
 * @param {char} *
 * @param {char} *name username
 * @param {char} *passwd user password
 * @return {*}
 */
void reg_db(sqlite3 *db, char **errmsg, char *name, char *passwd) {
    char sql[1024];
    int rc;
    sprintf(sql, "insert into user(name,passwd)values('%s','%s')", name,
            passwd);
    rc = sqlite3_exec(db, sql, NULL, NULL, errmsg);
    is_sqlite(rc);
}

/**
 * @description: Verify online users
 * @param {sqlite3} *db database
 * @param {char} *
 * @param {char} *user username
 * @return {*}
 */
int read_online_ok(sqlite3 *db, char **errmsg, char *user) {
    int rc;
    int i;
    sqlite3_stmt *stmt = NULL;
    rc = sqlite3_prepare(db, "select * from online", -1, &stmt, 0);
    is_sqlite_ok(rc);
    rc = sqlite3_step(stmt);
    int userflag = 1;
    while (rc == SQLITE_ROW) {
        userflag = strcmp(user, sqlite3_column_text(stmt, 1));
        if (userflag == 0) {
            return ONLINEIN;
        }
        rc = sqlite3_step(stmt);
    }
    return ONLINEOUT;
}

/**
 * @description: verify (user name and password) whether you can log in
 * @param {*}
 * @return {*}
 */
int read_pass(sqlite3 *db, char **errmsg, char *user, char *passwd) {
    int rc;
    int i;
    sqlite3_stmt *stmt = NULL;
    rc = sqlite3_prepare(db, "select * from user", -1, &stmt, 0);
    is_sqlite_ok(rc);
    rc = sqlite3_step(stmt);
    int userflag = 1;
    int passwdflag = 1;
    while (rc == SQLITE_ROW) {
        userflag = strcmp(user, sqlite3_column_text(stmt, 1));
        passwdflag = strcmp(passwd, sqlite3_column_text(stmt, 2));
        if (userflag == 0 && passwdflag == 0) {
            return PASSWDOK;
        }
        rc = sqlite3_step(stmt);
    }
    return PASSWDNO;
}
/**
 * @description: send information to online users
 * @param {*}
 * @return {*}
 */
void write_online_all(sqlite3 *db, char **errmsg, vpChat temp) {
    time_t timep;
    time(&timep);
    char *timedata = ctime(&timep);
    timedata[strlen(timedata) - 1] = '\0';
    strcpy(temp->time, timedata);

    int rc;
    char fd[100];
    sqlite3_stmt *stmt = NULL;
    rc = sqlite3_prepare(db, "select * from online", -1, &stmt, 0);
    is_sqlite_ok(rc);
    rc = sqlite3_step(stmt);
    int userflag = 1;
    while (rc == SQLITE_ROW) {
        strcpy(fd, sqlite3_column_text(stmt, 2));
        write(atoi(fd), temp, sizeof(stChat));
        rc = sqlite3_step(stmt);
    }
}

/**
 * @description: delete login information
 * @param {sqlite3} *db database
 * @param {char} *
 * @param {int} sockfd
 * @return {*}
 */
void delete_online_db(sqlite3 *db, char **errmsg, int sockfd) {
    char sql[1024];
    int rc;
    sprintf(sql, "delete from online where socket = %d", sockfd);
    rc = sqlite3_exec(db, sql, NULL, NULL, errmsg);
    is_sqlite(rc);
}

/**
 * @description: login infor insert
 * @param {sqlite3} *db database
 * @param {char} *
 * @param {char} *name username
 * @param {int} sockfd
 * @return {*}
 */
void log_db(sqlite3 *db, char **errmsg, char *name, int sockfd) {
    char sql[1024];
    int rc;
    sprintf(sql, "insert into online(name,socket,flag)values('%s',%d,1)", name,
            sockfd);
    rc = sqlite3_exec(db, sql, NULL, NULL, errmsg);
    is_sqlite(rc);
}