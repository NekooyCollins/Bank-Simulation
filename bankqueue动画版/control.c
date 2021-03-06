#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<string.h>
#include<windows.h>

#include "head.h"
#include "control.h"
#include "input.h"
#include "output.h"
#include "queueServe.h"
#include "vip.h"

/*****************读取配置文件参数*************************/
void readConfigurationFile()
{
    FILE *cfPtr;
    struct canshu client;
    int i=1;

    if((cfPtr=fopen("configurationFile.dat","r"))==NULL)
        printf("No\n");
    else{
        fscanf(cfPtr,"%s%d",client.A,&client.num);
        while(!feof(cfPtr)){
            switch (i)
            {
            case 1:
                Minsec=client.num;
                break;
            case 2:
                Maxsec=client.num;
                break;
            case 3:
                MinRestsec=client.num;
                break;
            case 4:
                MaxRestsec=client.num;
                break;
            case 5:
                VIPSERVELen=client.num;
                break;
            case 6:
                closebank=client.num;
                break;
            }
            i++;
            fscanf(cfPtr,"%s%d",client.A,&client.num);
        }
    }
    fclose(cfPtr);
}
/***************初始化函数*****************************/
void Initialization1()   //初始化函数
{
    int i;//控制循环

    readConfigurationFile();   //读取配置文件参数

    for(i=0;i<=2;i++)
        windows[i].Gstate=FREE;    //1-3号普通窗口设为空闲
    for(i=3;i<=5;i++)
        windows[i].Gstate=CLOSE;    //4-6号普通窗口设为关闭（等待开启服务）

    VIP_windows[0].Vstate=FREE;    //1号vip窗口设为空闲
    VIP_windows[1].Vstate=CLOSE;   //2号vip窗口设为关闭等待开启服务

    for(i=0;i<=5;i++)
        win_num[i]=0;    //各个窗口一天服务的人数归零
    win_VIP_num[0]=0;
    win_VIP_num[1]=0;  //1-2vip窗口一天服务的人数归零

    vipwaiting=0;
    vipdoing=0;
    viptotal=0;

    nordoing=0;
    norwaiting=0;
    nortotal=0;      //全局变量归零

    enter_Q=0;  //作为下班类型的标记

    GHEADPTR=NULL;
    GTAILPTR=NULL;
    VHEADPTR=NULL;
    VTAILPTR=NULL;

    for(i=0;i<=5;i++){
        win_num[i]=0;  //各窗口受理的客户总数置0
        windows[i].nor_serve_time=0;  //各窗口的服务总时长置零
    }
    win_VIP_num[0]=0;
    win_VIP_num[1]=0;

    VIP_windows[0].VIP_serve_time=0;
    VIP_windows[1].VIP_serve_time=0;

    Start=clock();    //时钟开启，银行开始服务
    isquit=working;   //将银行的状态设为服务状态

}

/*****************判断窗口申请休息函数*******************/
int judgeGRest(int Gwin_number)
{
    int a,i;     //计数
    int yip;
    srand(time (NULL));
    a=0;

    if(windows[Gwin_number].Gstate==SERVE||windows[Gwin_number].Gstate==FREE){    // 当窗口处于空闲或者服务状态时才会申请休息
        for(i=0;i<=5;i++){
            if(windows[i].Gstate==SERVE)
                a++;
        }                      //计算正在服务的窗口数
        if(a>=4&&norwaiting<=3*(a-1)){  //若正在服务的窗口大于等于4且等待的人数小于3(a-1)个人，则允许休息
            windows[Gwin_number].Gstate=REST;//改状态
            GrestTime[Gwin_number]=MinRestsec+(rand()%MaxRestsec); //休息时长
            GbeginRest[Gwin_number]=clock();      //记录该窗口开始休息的时间
            yip=0;
        }
        else
            yip=1;          //printf("暂时不允许休息，请等待\n");
    }
    else
        yip=2;              //printf("该窗口处于休息或者关闭状态\n");
    return yip;
}

/*****************判断窗口申请休息函数*******************/
int judgeVRest(int Vwin_number)
{
    int yip;
    int a,i;     //计数

    srand(time (NULL));
    a=0;

    if(VIP_windows[Vwin_number].Vstate==SERVE||VIP_windows[Vwin_number].Vstate==FREE||VIP_windows[Vwin_number].Vstate==CHANGE){  //当窗口处于空闲或者服务状态时才会申请休息
        for(i=0;i<=1;i++){
            if(VIP_windows[i].Vstate==SERVE)
                a++;
        }

        if(a>=2&&vipwaiting<=3){
            VIP_windows[Vwin_number].Vstate=REST;
            VrestTime[Vwin_number]=MinRestsec+(rand()%MaxRestsec);
            VbeginRest[Vwin_number]=clock();
            yip=0;      //允许休息
        }
        else
            yip=1;     //printf("暂时不允许休息，请等待\n");
    }
    else
        yip=2;   //printf("该窗口处于休息或者关闭状态\n");
    return yip;
}


int judgeQuit()       //下班策略，无论是接受到下班请求还是下班时间到，都将下班状态变为等待下班，然后在等待下班的基础上判断客户情况再决定下班
{
    int workingtime;
    int out;

    End=clock();

    workingtime=(int)(End-Start)/CLOCKS_PER_SEC;

    if(enter_Q==1){ //得到申请下班指令时
        out=judgeQuit1();
    }
    else{         //当没申请到下班指令时
        if(workingtime>=closebank){
            out=judgeQuit1();
        }
    }
    return out;  // 返回1表示已经下班，返回0表示不可以下班
}

int judgeQuit1()
{
    int i;
    int live1=0,live2=0;
    if(isGempty(GHEADPTR)&&isVempty(VHEADPTR)){//当没有顾客排队且窗口都服务完毕时下班
        for(i=0;i<=5;i++){
            if(windows[i].Gstate==FREE||windows[i].Gstate==CLOSE||windows[i].Gstate==REST)
                live1++;               //无客户在窗口接受服务时，统计窗口数
        }

        for(i=0;i<=1;i++){
            if(VIP_windows[i].Vstate==FREE||VIP_windows[i].Vstate==CLOSE||VIP_windows[i].Vstate==REST)
                live2++;                  //无客户在窗口接受服务时，统计窗口数
        }

        if(live1==6&&live2==2)
            isquit=finish_working;
        else
            isquit=waiting_finish_working;
    }
    else                              //否则，等待下班
        isquit=waiting_finish_working;

    if(isquit==finish_working)
        return 1;             //返回1表示已经下班
    else
        return 0;         //返回0表示不可以下班
}


int judgeWindowsState()
{
    int i;
    int live1=0,live2=0;

    for(i=0;i<=5;i++){
        if(windows[i].Gstate==FREE)
            live1++;               // 统计FREE窗口
    }

    for(i=0;i<=1;i++){
        if(VIP_windows[i].Vstate==FREE)
            live2++;              //统计FREE窗口
    }

    if(live1>0||live2>0)
        return 1;
    else
        return 0;
}

