
#ifndef _INPUT_H_
#define _INPUT_H_


struct bankstate{
    char type[20];      //事件类型(G,V,R,Q)
    int intervalTime;  //事件发生的时间间隔
    //int num;       //type为R，申请休息的窗口类型（普通，num=1，vip，num=2，其余事件发生时为0）
    int ID;      //V：vip客户的id 或者  R 申请窗口休息的号码
};

FILE *bankPtr;
clock_t now;
char type;
int intervalTime,num,id;
/***********获取指令函数原型***************************/
void getInput(char);

#endif // _INPUT_H_

