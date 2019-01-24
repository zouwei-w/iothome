#include "sql.h"

int exec_sql(char *database, char *sql)
{
    int ret;
    int nrow;
    int ncolumn;
    sqlite3 *pdb;
    char *errmsp;
    char **result;

    ret = sqlite3_open(database, &pdb);
    if(ret < 0)
    {    
        fprintf(stderr,"Failed to sqlite3_open: %s\n",sqlite3_errmsg(pdb));
        return -1;
    }

    ret = sqlite3_get_table(pdb, sql, &result, &nrow, &ncolumn, &errmsp);
    if(ret != SQLITE_OK)
    {
        fprintf(stderr,"Failed to sqlite3_get_table:%s\n",errmsp);
        sqlite3_free(errmsp);
        return -1;
    }
    
    return nrow;
}


int user_register(char *user_name, char *pass_word, char *phone)
{
    int ret;
    char sql[1024];
    sprintf(sql,"select * from user_table where username='%s';",user_name);
    ret = exec_sql(DATABASE, sql);
    if(ret == 1)
        return 1; //已经注册

    sprintf(sql,"select * from user_table where phone='%s';",phone);
    ret = exec_sql(DATABASE, sql);
    if(ret == 1)
        return 2; //已经使用该电话号码
    
    sprintf(sql,"insert into user_table values('%s','%s','%s');",user_name, pass_word, phone);
    ret = exec_sql(DATABASE, sql);
    if(ret < 0)
        return -1; //注册失败

    return 0;  //注册成功
}

int user_login(char *user_name, char *pass_word)
{
    int ret;
    char sql[1024];
    sprintf(sql,"select * from user_table where username='%s' and password='%s';",user_name,pass_word);
    ret = exec_sql(DATABASE, sql);
    if(ret == 1)
        return 0;  //登录成功
    else
        return -1; //登录失败
}

