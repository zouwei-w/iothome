#ifndef _SQL_H
#define _SQL_H

#include "comm.h"

#define DATABASE "./user.db" 

extern int exec_sql(char *database, char *sql);

extern int user_register(char *user_name, char *pass_word, char *phone);

extern int user_login(char *user_name, char *pass_word);

#endif
