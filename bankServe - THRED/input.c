
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

/***********************输入函数，获取操作指令***************************/
void getInput()
{
    int counter=0,i;
    char chose[100];

    while (isquit==working)   //当银行只处于正常工作状态时才会读取数据
    {
        printf("G-普通客户到达\n"
        "V-VIP客户到达\n"
        "R-窗口申请休息\n"
        "Q-申请下班\n"
        "请输入指令:\n");

        scanf("%s",chose);


        for(i=0;chose[i]!='\0';i++){
            switch (chose[i]){
                case 'G':              //普通客户处理
                    norwaiting++;     //普通客户等待数加一
                    nortotal++;      //普通客户总数加一
                    printf("尊敬的客户，您的号码为%d，请到休息区等待\n",nortotal);
                    fprintf(diaryPtr,"尊敬的客户，您的号码为%d，请到休息区等待\n",nortotal);
                    enGqueue(&GHEADPTR,&GTAILPTR);        //将该客户加入普通客户队列
                    break;
                case 'V':             //vip处理
                    if(vipJudge()==1){   //vipid正确
                        vipwaiting++;
                        viptotal++;
                        printf("尊敬的客户，您的号码为V%d，请到休息区等待\n",viptotal);
                        fprintf(diaryPtr,"尊敬的客户，您的号码为V%d，请到休息区等待\n",viptotal);
                        enVqueue(&VHEADPTR,&VTAILPTR);
                    }
                    else{       //vipid不正确加入普通队列
                        norwaiting++;
                        nortotal++;
                        printf("尊敬的客户，您的号码为%d，请到休息区等待\n",nortotal);
                        fprintf(diaryPtr,"尊敬的客户，您的号码为%d，请到休息区等待\n",nortotal);
                        enGqueue(&GHEADPTR,&GTAILPTR);
                    }
                    break;
                case 'R':           //判断窗口休息
                    judgeRest();
                    break;
                case 'Q':         //判断下班
                    enter_Q=1;
                    judgeQuit();
                    break;
                default:
                    printf("输入的请求有误！\n");
                    break;
            }
        }
    }
}


