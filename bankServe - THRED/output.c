#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<windows.h>
#include<string.h>

#include "head.h"
#include "control.h"
#include "input.h"
#include "output.h"
#include "queueServe.h"
#include "vip.h"
/**************输出信息函数********************/
int printOutput()//定时输出信息
{
    int i;//控制循环
    int flag;
    int counter=0;
    int current_time,left_time;//系统时间
    clock_t time;

    while(1){ //休眠8秒打印一次
        Sleep(3000);
        time=clock();
        current_time=(int)(time-Start)/CLOCKS_PER_SEC;

        left_time=closebank-current_time;
        printf("\n\n*******************************************************************************\n");
        fprintf(diaryPtr,"\n\n*******************************************************************************\n");
        printf("当前时间为：%ds，距离下班还有%ds\n",current_time,left_time);
        fprintf(diaryPtr,"当前时间为：%ds，距离下班还有%ds\n",current_time,left_time);
        printf("当前队列中的客户数；普通客户数:%d人,VIP客户数:%d人。\n",norwaiting-nordoing,vipwaiting-vipdoing);
        fprintf(diaryPtr,"当前队列中的客户数；普通客户数:%d人,VIP客户数:%d人。\n",norwaiting-nordoing,vipwaiting-vipdoing);

        if(nordoing!=norwaiting){
            printf("普通客户排队号码:%d-%d；\n",nordoing,norwaiting);
            fprintf(diaryPtr,"普通客户排队号码:%d-%d；\n",nordoing,norwaiting);
        }
        else{
            printf("无客户排队\n");
            fprintf(diaryPtr,"无客户排队\n");
        }
        if(vipdoing!=vipwaiting){
            printf("VIP客户排队号码:V%d-V%d；\n",vipdoing,vipwaiting);
            fprintf(diaryPtr,"VIP客户排队号码:V%d-V%d；\n",vipdoing,vipwaiting);
        }
        else{
            printf("无VIP客户排队\n");
            fprintf(diaryPtr,"无VIP客户排队\n");
        }

        printf("各窗口状态：\n");
        fprintf(diaryPtr,"各窗口状态：\n");
   /*******输出VIP窗口的状态*********/
        for(i=0;i<=1;i++){
            switch (VIP_windows[i].Vstate)
            {
            case FREE:
                printf("%d号VIP窗口：空闲\n",i+1);
                fprintf(diaryPtr,"%d号VIP窗口--空闲\n",i+1);
                break;
            case SERVE:
                if(single==0){
                    printf("%d号VIP窗口：正在办理%d号VIP顾客\n",i+1,VIP_windows[i].VIP_number);
                    fprintf(diaryPtr,"%d号VIP窗口--服务中--%d号VIP顾客\n",i+1,VIP_windows[i].VIP_number);
                }
                else{
                    printf("%d号VIP窗口：正在办理%d号普通顾客\n",i+1,VIP_windows[i].VIP_number);
                    fprintf(diaryPtr,"%d号VIP窗口--服务中--%d号普通顾客\n",i+1,VIP_windows[i].VIP_number);
                }
                break;
            case REST:
                printf("%d号VIP窗口：休息\n",i+1);
                fprintf(diaryPtr,"%d号VIP窗口--休息\n",i+1);
                break;
            case CLOSE:
                printf("%d号VIP窗口：关闭\n",i+1);
                fprintf(diaryPtr,"%d号VIP窗口--关闭\n",i+1);
                break;
            }
        }
    /**************输出普通客户的状态********/
        for(i=0;i<=5;i++){
            switch (windows[i].Gstate)
            {
            case FREE:
                printf("%d号普通窗口：空闲\n",i+1);
                fprintf(diaryPtr,"%d号普通窗口--空闲\n",i+1);
                break;
            case SERVE:
                printf("%d号普通窗口：正在办理%d号顾客\n",i+1,windows[i].G_number);
                fprintf(diaryPtr,"%d号普通窗口--服务中--正在办理%d号顾客\n",i+1,windows[i].G_number);
                break;
            case REST:
                printf("%d号普通窗口：休息\n",i+1);
                fprintf(diaryPtr,"%d号普通窗口--休息\n",i+1);
                break;
            case CLOSE:
                printf("%d号普通窗口：关闭\n",i+1);
                fprintf(diaryPtr,"%d号普通窗口--关闭\n",i+1);
                break;
            }
        }

        switch (isquit)
        {
                case working:
                    printf("银行正在营业\n");
                    fprintf(diaryPtr,"银行正在营业%c\n",' ');
                    break;
                case waiting_finish_working:
                    if(enter_Q==1){
                        printf("已接收下班申请，已关闭取号服务，正在服务等待的客户，银行正在等待下班\n");
                        fprintf(diaryPtr,"已接收下班申请，已关闭取号服务，正在服务等待的客户，银行正在等待下班\n");
                    }
                    else{
                        printf("下班时间到，已关闭取号服务，正在服务等待的客户，银行正在等待下班\n");
                        fprintf(diaryPtr,"下班时间到，已关闭取号服务，正在服务等待的客户，银行正在等待下班\n");
                    }
                    break;
                case finish_working:
                    printf("银行已下班\n");
                    fprintf(diaryPtr,"银行已下班\n");
                break;
        }

        printf("\n\n*******************************************************************************\n");
        fprintf(diaryPtr,"\n\n*******************************************************************************\n");
    }
}

void writeDiary()
{
    int i;
    double  average_time; //服务的平均时长
    int serve_time;       //服务总时长
    for(i=0;i<=5;i++){
        if(win_num[i]!=0){
            average_time=(double)windows[i].nor_serve_time/win_num[i];
            fprintf(diaryPtr,"%d号普通窗口受理客户的总数为%d，总业务办理时长为%ds，平均单笔业务办理时长为%fs\n",i+1,win_num[i],windows[i].nor_serve_time,average_time);
        }
        else
            fprintf(diaryPtr,"%d号普通窗口受理客户的总数为%d，总业务办理时长为%ds，平均单笔业务办理时长为%fs\n",i+1,win_num[i],windows[i].nor_serve_time,0.0);
    }
    for(i=0;i<=1;i++){
        if(win_VIP_num[i]!=0){
            average_time=(double)VIP_windows[i].VIP_serve_time/win_VIP_num[i];
            fprintf(diaryPtr,"%d号VIP窗口受理客户的总数为%d，总业务办理时长为%ds，平均单笔业务办理时长为%fs\n",i+1,win_VIP_num[i],VIP_windows[i].VIP_serve_time,average_time);
        }
        else
            fprintf(diaryPtr,"%d号VIP窗口受理客户的总数为%d，总业务办理时长为%ds，平均单笔业务办理时长为%fs\n",i+1,win_VIP_num[i],VIP_windows[i].VIP_serve_time,0.0);
    }
    fprintf(diaryPtr,"营业厅总共受理了%d位普通客户，%d位VIP客户\n",nortotal,viptotal);
    serve_time=(int)(End-Start)/CLOCKS_PER_SEC;
    fprintf(diaryPtr,"银行服务的总时长为%ds\n",serve_time);
    fclose(diaryPtr);
}
