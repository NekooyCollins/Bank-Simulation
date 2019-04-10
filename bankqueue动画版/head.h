
#ifndef _HEAD_H_
#define _HEAD_H_


#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<string.h>


void mainloop(void);
void drawBackGround(void);

int closebank;   //银行模拟营业时间500s
int VIPSERVELen;   //设置时长内动态关闭vip窗口

int Minsec;       //单笔业务服务时长
int Maxsec;

int MinRestsec;       //单窗口休息时长
int MaxRestsec;

int sofa[3][7];    //表示沙发的状态 ：FREE，没人坐，SERVE：有人坐

struct nor_windows
{
    int Gstate;			 //6个普通窗口，记录工作窗口的状态（0:空闲，1:营业，2:休息，-1：关闭
    int G_number;		 //当前服务的客户号码
    int nor_serve_time;  //各个窗口服务客户的总时长
};
struct nor_windows windows[6];

struct VIP_windows
{
    int Vstate;			 //2个VIP窗口，记录工作窗口的状态（0:空闲，1:营业，2:休息，-1:关闭,3:转普）
    int VIP_number;		 //当前服务的客户号码
	int VIP_serve_time;  //各个客户服务客户的总时长
};
struct VIP_windows VIP_windows[2];

#define FREE 0
#define SERVE 1
#define REST 2
#define CLOSE -1
#define CHANGE 3

int win_num[6];         //统计各窗口受理的客户总数
int win_VIP_num[2];

int vipwaiting;         //VIP等待客户的最大号码
int vipdoing;          //vip窗口正在办理的客户的最大号码
int viptotal;         //一天中vip客户的总人数

int norwaiting;         //普通客户等待客户的最大号码
int nordoing;           //普通窗口正在办理客户的最大号码
int nortotal;         //一天中普通客户的总人数

clock_t Start;          //银行开始营业的时间
clock_t End;            //银行结束服务的时间

clock_t GserveBegin[6];     //记录窗口开始服务的时间
clock_t GserveFinish[6];    //记录窗口结束服务的时间
int GserveTime[6];        //记录窗口服务一个客户的服务时长
clock_t VserveBegin[2];
clock_t VserveFinish[2];
int VserveTime[2];

clock_t GbeginRest[6];  //记录窗口开始休息的时间
clock_t GfinishRest[6]; //记录窗口结束休息的时间
int GrestTime[6];        //记录窗口休息的时间长短
clock_t VbeginRest[2];
clock_t VfinishRest[2];
int VrestTime[2];

int enter_Q;
int isquit;                 //上班中为1，输入下班指令：若客户未服务完，处于等待下班状态为2，正式下班为0
#define working 1
#define waiting_finish_working 2
#define finish_working 0


FILE *diaryPtr;                //银行日志记录文件指针
#endif // HEAD_H


