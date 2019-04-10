
#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<string.h>
#include<windows.h>
//#include<graphics.h>

#include "head.h"
#include "control.h"
#include "input.h"
#include "output.h"
#include "queueServe.h"
#include "vip.h"

/***********************输入函数，获取操作指令***************************/
void getInput(char type)
{
    switch (type){
        case 'G':              //普通客户处理
            norwaiting++;     //普通客户等待数加一
            nortotal++;      //普通客户总数加一
            fprintf(diaryPtr,"尊敬的客户，您的号码为%d，请到休息区等待\n",nortotal);
            enGqueue(&GHEADPTR,&GTAILPTR);        //将该客户加入普通客户队列
            break;
        case 'V':             //vip处理
            vipwaiting++;
            viptotal++;
            fprintf(diaryPtr,"尊敬的客户，您的号码为V%d，请到休息区等待\n",viptotal);
            enVqueue(&VHEADPTR,&VTAILPTR);
            break;
    }
}




