// // #include <stdio.h>
// // #include <stdlib.h>
// // #include <string.h>

// // #include "include/msg_protocol.h"

// // int main() {
// //   msgPkg mp;
// //   // printf("%d\n", sizeof(mp));
// //   mp.msg_header.type = TYPE_MSG;

// //   strncpy(mp.msg_body.msg_msg.src_usr, "12345678", 10);
// //   strncpy(mp.msg_body.msg_msg.dst_usr, "112233", 10);
// //   char *buff = "Hello, how are you!";

// //   strncpy(mp.msg_body.msg_msg.msg_buff, buff, strlen(buff));
// //   mp.msg_header.length =
// sizeof(Msg_header)+20+strlen(mp.msg_body.msg_msg.msg_buff);

// //   printf("%d\n", mp.msg_header.length);

// //   return 0;
// // }
// #define THREAD 32
// #define QUEUE  256

// #include <stdio.h>
// #include <pthread.h>
// #include <unistd.h>
// #include <assert.h>

// #include "threadpool.h"

// int tasks = 0, done = 0;
// pthread_mutex_t lock;

// void dummy_task(void *arg) {
//     usleep(10000);
//     pthread_mutex_lock(&lock);
//     done++;
//     pthread_mutex_unlock(&lock);
// }

// int main(int argc, char **argv)
// {
//     threadpool_t *pool;

//     pthread_mutex_init(&lock, NULL);

//     assert((pool = threadpool_create(THREAD, QUEUE, 0)) != NULL);
//     fprintf(stderr, "Pool started with %d threads and "
//             "queue size of %d\n", THREAD, QUEUE);

//     while(threadpool_add(pool, &dummy_task, NULL, 0) == 0) {
//         pthread_mutex_lock(&lock);
//         tasks++;
//         pthread_mutex_unlock(&lock);
//     }

//     fprintf(stderr, "Added %d tasks\n", tasks);

//     while((tasks / 2) > done) {
//         usleep(10000);
//     }
//     assert(threadpool_destroy(pool, 0) == 0);
//     fprintf(stderr, "Did %d tasks\n", done);

//     return 0;
// }
// #include <cstring>
// #include <iostream>
// #include <queue>
// #include <map>

// typedef struct{
//     int a;
//     char buff[16];
// }ss;

// class query_task_t {
//  public:
//   char query_str[64];
//   query_task_t(char *str, int n) { strncat(this->query_str, str, n); }
// };

// int main() {
//     std::map<int, ss> mm;
//     ss s1;
//     s1.a = 1;
//     strcat(s1.buff, "hello");
//     mm[1] = s1;

//   return 0;
// }

// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>
// #include <map>
// typedef struct{
//     int a;
//     int b;
// }box;
// int main() {
//     // box b1;
//     // b1.a =1;
//     // b1.b = 2;
//     // box b2 = b1;
//     int a = 8;
//     a/=3;
//     printf("%d\n", a);
//   return 0;
// }


#include <stdio.h>  // printf
#include <stdlib.h> // exit
#include <unistd.h> // pipe
#include <string.h> // strlen
#include <pthread.h> // pthread_create
 
using namespace std;
 
void *func(void * fd)
{
        printf("in pthread %d\n",pthread_self());

}
 
int main()
{
      
        pthread_t tid = 0;
        pthread_create(&tid, NULL, func, NULL);
        printf("id %d\n", tid);
        pthread_join(tid, NULL);
 
 
        return 0;
}