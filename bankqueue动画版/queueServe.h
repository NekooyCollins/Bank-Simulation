#ifndef _QUEUESERVE_H_
#define _QUEUESERVE_H_
void queueServe(void);
void decreaseOrIncrease(void);


char Gtype;
int Gnum;
int Gwin;


char Vtype;
int Vnum;
int Vwin;

char Gpng[6];
int GPicwin[6];
char Vpng[2];
int VPicwin[2];

int single;

struct nor_queue      //普通客户的队列结构体，自引用结构（c程序设计教程P384）
{
    int number;       //服务号码
    struct nor_queue *nextPtr;
};
typedef struct nor_queue nor_Queue;
typedef nor_Queue *nor_QueuePtr;       //自定义数据类型
nor_QueuePtr GHEADPTR,GTAILPTR;

struct vip_queue       //vip客户队列
{
    int vip_number;     //vip服务号
    struct vip_queue *nextPtr;
};
typedef struct vip_queue vip_Queue;
typedef vip_Queue *vip_QueuePtr;
vip_QueuePtr VHEADPTR,VTAILPTR;


/****************普通客户队列、vip客户队列函数原型**********************/
int isGempty(nor_QueuePtr);           //判断队列是否为空
int deGqueue(nor_QueuePtr *,nor_QueuePtr *);   //删除队列中的第一个客户
void enGqueue(nor_QueuePtr *,nor_QueuePtr *);   //加入客户到队列

int isVempty(vip_QueuePtr);
int deVqueue(vip_QueuePtr *,vip_QueuePtr *);
void enVqueue(vip_QueuePtr *,vip_QueuePtr *);

#endif // _QUEUESERVE_H_



