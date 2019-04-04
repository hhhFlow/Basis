
#define TYPE_REQ_LOGIIN 0x01
#define TYPE_RET_LOGIN 0x02

#define TYPE_REQ_FRIENDS 0x04
#define TYPE_RET_FRIENDS 0x08
#define TYPE_MSG 0x0f


#define LOGIN_FAILED      0x10
#define LOGIN_SUCCESSFUL  0x11

typedef struct {
  int type;
  int length;
} Msg_header;

typedef struct {
  char user_id[10];
} Msg_login;  //登陆消息

typedef struct {
  char is_succefful;
} Msg_res;  //登陆返回消息

typedef struct {
  char user_id[10];
} Msg_req_frends;  //请求服务器好友列表

typedef struct {
} Msg_ret_frends;  //服务器返回好友列表

typedef struct {
//   int len;
  char src_usr[10];
  char dst_usr[10];
  char msg_buff[512];
} Msg_msg;  //通用消息

typedef struct {
  Msg_header msg_header;
  union Msg{
    Msg_login msg_login;
    Msg_res msg_res;
    Msg_req_frends msg_req_frends;
    Msg_ret_frends msg_ret_frends;
    Msg_msg msg_msg;
  } msg_body;
} msgPkg;