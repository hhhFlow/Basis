
#include <mysql/mysql.h>
#include <pthread.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <map>
#include <queue>

class QueryQueue;

typedef struct {
  QueryQueue *q;
} args;

class query_task_t {
 public:
  char query_str[64];
  query_task_t(){};
  query_task_t(char *str, int n) {
    memset(query_str, '\0', 64);
    strncpy(this->query_str, str, n);
  }
  bool operator==(const query_task_t &that) {
    char *left1 = this->query_str;
    char *left2 = this->query_str;
    
    while (1) {
      *left1 ++;
      *left2 ++;
      if (*left1 != *left2) return false;
    }
  }
};

class QueryQueue {
 private:
  MYSQL mysql;
  pthread_mutex_t lock;

  std::queue<query_task_t> query_queue;
  std::map<query_task_t, void *> query_result;

 public:
  QueryQueue();
  static void *query_thread(void *arg);

  void run();
  void query_task_add(query_task_t task_t);
};

QueryQueue::QueryQueue() {
  mysql_init(&mysql);
  pthread_mutex_init(&lock, NULL);
  if (NULL == mysql_real_connect(&mysql, "localhost", "root", "13693268858",
                                 "myChat", 0, NULL, 0)) {
    printf("mysql_real_connect(): %s\n", mysql_error(&mysql));
    exit(0);
  }
}

void QueryQueue::query_task_add(query_task_t task_t) {
  pthread_mutex_lock(&(this->lock));
  query_queue.push(task_t);
  pthread_mutex_unlock(&(this->lock));
}

void *QueryQueue::query_thread(void *arg) {
  args *a = (args *)(arg);
  QueryQueue *qq = a->q;
  MYSQL mysql = qq->mysql;

  int rc, i, fields;
  MYSQL_RES *res = NULL;
  char *query_str = NULL;

  int rows;
  MYSQL_ROW row;
  query_task_t task;
  while (1) {
    if (qq->query_queue.size() == 0) continue;

    pthread_mutex_lock(&(qq->lock));
    task = qq->query_queue.front();
    query_str = task.query_str;
    rc = mysql_real_query(&mysql, query_str, strlen(query_str));
    qq->query_queue.pop();
    res = mysql_store_result(&mysql);
    pthread_mutex_unlock(&(qq->lock));

    qq->query_result[task] = res;

    if (NULL == res) {
      printf("mysql_restore_result(): %s\n", mysql_error(&mysql));
      continue;
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

void QueryQueue::run() {
  pthread_t thread_id;

  args *arg = new args();
  arg->q = this;

  pthread_create(&(thread_id), NULL, query_thread, (void *)arg);
}

int main() {
  char *buff = "select * from user";
  query_task_t task_t(buff, strlen(buff));
  printf("%s\n", task_t.query_str);
  QueryQueue qq;
  qq.run();
  while (1) {
    char ch = getchar();
    if (ch == 'a') qq.query_task_add(task_t);
  }

  return 0;
}