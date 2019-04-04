#include <string.h>
#include <stdio.h>
#include <mysql/mysql.h>
 
int main(int argc, const char *argv[])
{
    MYSQL   mysql;
    MYSQL_RES       *res = NULL;
    MYSQL_ROW       row;
    char            *query_str = NULL;
    int             rc, i, fields;
    int             rows;

    if (NULL == mysql_init(&mysql)) {    //分配和初始化MYSQL对象
        printf("mysql_init(): %s\n", mysql_error(&mysql));
        return -1;
    }
 
    //尝试与运行在主机上的MySQL数据库引擎建立连接
    if (NULL == mysql_real_connect(&mysql,
                "localhost",
                "root",
                "13693268858",
                "myChat",
                0,
                NULL,
                0)) {
        printf("mysql_real_connect(): %s\n", mysql_error(&mysql));
        return -1;
    }
 
    printf("Connected MySQL successful! \n");
    query_str = "select * from user";
    rc = mysql_real_query(&mysql, query_str, strlen(query_str));
    if (0 != rc) {
        printf("mysql_real_query(): %s\n", mysql_error(&mysql));
        return -1;
    }
    res = mysql_store_result(&mysql);
    if (NULL == res) {
         printf("mysql_restore_result(): %s\n", mysql_error(&mysql));
         return -1;
    }
    rows = mysql_num_rows(res);
    printf("The total rows is: %d\n", rows);
    fields = mysql_num_fields(res);
    printf("The total fields is: %d\n", fields);
    while ((row = mysql_fetch_row(res))) {
        for (i = 0; i < fields; i++) {
            printf("%s %d\t", row[i],strlen(row[i]));
        }
        printf("\n");
    }
    mysql_close(&mysql); 
    return 0;
}
