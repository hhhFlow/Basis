#include <mysql/mysql.h>
#include <pthread.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <map>
#include <queue>

class query_task_t {
 public:
  char query_str[64];
  query_task_t(char *str, int n) { strncat(this->query_str, str, n); }
};

class QueryQueue {
 public:
  static std::queue<query_task_t> query_queue;
  static std::map<query_task_t, void *> query_result;
  static pthread_mutex_t lock;
  MYSQL *mysql;

  QueryQueue() {
    mysql = (MYSQL *)malloc(sizeof(MYSQL));
    mysql_init(mysql);
    pthread_mutex_init(&lock, NULL);
  }

  static void* query_thread(void *arg) {
    MYSQL *mysql = (MYSQL *)arg;
    int rc, i, fields;
    MYSQL_RES *res = NULL;
    char *query_str = NULL;

    int rows;
    MYSQL_ROW row;
    while (1) {
      if (query_queue.size() == 0) continue;

      pthread_mutex_lock(&(lock));
      query_str = query_queue.front().query_str;
      rc = mysql_real_query(mysql, query_str, strlen(query_str));
      this->query_queue.pop();
      res = mysql_store_result(mysql);
      pthread_mutex_unlock(&(lock));

      res = mysql_store_result(mysql);
      if (NULL == res) {
        printf("mysql_restore_result(): %s\n", mysql_error(mysql));
        return;
      }
      rows = mysql_num_rows(res);
      printf("The total rows is: %d\n", rows);
      fields = mysql_num_fields(res);
      printf("The total fields is: %d\n", fields);
      while ((row = mysql_fetch_row(res))) {
        for (i = 0; i < fields; i++) {
          printf("%s %d\t", row[i], strlen(row[i]));
        }
        printf("\n");
      }
    }
  }


  void run() {
    pthread_t thread_id;
    mysql_real_connect(this->mysql, "localhost", "root", "13693268858",
                       "myChat", 0, NULL, 0);

    pthread_create(&thread_id, NULL, query_thread, (void *)(this->mysql));
  }

  void query_task_add(query_task_t task_t) {
    pthread_mutex_lock(&(this->lock));
    query_queue.push(task_t);
    pthread_mutex_unlock(&(this->lock));
  }
};

int main() {
  char *buff = "select * from user";
 query_task_t task_t(buff, strlen(buff));

  QueryQueue qq;
  qq.run();
  qq.query_task_add(task_t);

  return 0;
}