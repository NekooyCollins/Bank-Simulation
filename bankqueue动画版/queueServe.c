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

void queueServe()
{
	int i, j;			//i为普通窗口下标，j为VIP窗口下标， number1为当前普通客户队列的排队人数，number2为当前VIP队列排队人数

	while(1){
	    int time1;
	    decreaseOrIncrease();
	    srand(time(NULL));					//产生随机时间种子
	    for(i = 0; i <= 5; i++)				//遍历所有普通窗口
	    {
                switch(windows[i].Gstate)
                {
                        case FREE:					//窗口空闲状态
                                if(!isGempty(GHEADPTR)) 					 //如果有客户等待
                                {
                                    win_num[i]++;      //统计各个窗口受理客户的总数
                                    windows[i].G_number = deGqueue(&GHEADPTR, &GTAILPTR);//记录当前的客户号码，删除队列的第一个数据
                                    nordoing++;
                                    GserveBegin[i] = clock();				 //服务开始计时
                                    Sleep(1000);
                                    windows[i].Gstate = SERVE;				 //窗口状态改变为服务状态
                                    GserveTime[i] = Minsec + (rand()%Maxsec);//产生随机服务时间
                                    windows[i].nor_serve_time=windows[i].nor_serve_time+GserveTime[i];   //统计各个窗口服务的总时长
                                }
                                break;
			    case SERVE:					//窗口服务状态
				    GserveFinish[i] = clock();					//当前时间作为服务结束时间
				    time1=(int)(GserveFinish[i] - GserveBegin[i])/CLOCKS_PER_SEC;
				    if(time1>= GserveTime[i]){          //如果结束时间和开始时间之差大于随机服务时间，将其状态改为空闲
                                        Sleep(1000);
                                        windows[i].Gstate = FREE;
                                        break;
				    }

			    case REST:					//窗口休息状态
                                GfinishRest[i] = clock();					//当前时间作为休息结束时间
                                time1=(int)(GfinishRest[i] - GbeginRest[i])/CLOCKS_PER_SEC;
                                if(time1>=GrestTime[i]){                //如果结束时间和开始时间之差大于随机休息时间，改为空闲状态
                                    Sleep(1000);
                                    windows[i].Gstate = FREE;
				    break;
                                }
			    case CLOSE:					//窗口关闭状态
			            ;//没有操作
				    break;
		    }
	    }

	    for(j = 0; j <= 1; j++){
		    if(isVempty(VHEADPTR)&&!isGempty(GHEADPTR)&&VIP_windows[j].Vstate == FREE)				//判断VIP队列是否为空，为空则变为普通窗口进行顺带服务
		    {
			    VIP_windows[j].Vstate = CHANGE;
			    single = 1;							//服务普通客户
		    }

		    switch(VIP_windows[j].Vstate){
			    case FREE:				//窗口空闲状态
				    if(!isVempty(VHEADPTR))					//队列中有客户
				    {
				        win_VIP_num[j]++;
                                        VserveBegin[j] = clock();				//服务开始计时
                                        VIP_windows[j].VIP_number = deVqueue(&VHEADPTR, &VTAILPTR);   //记录客户服务号码
                                        vipdoing++;
                                        Sleep(1000);
                                        VIP_windows[j].Vstate = SERVE;			//窗口状态改变为服务状态
                                        VserveTime[j] = Minsec + (rand()%Maxsec);//产生随机服务时间
                                        VIP_windows[j].VIP_serve_time=VIP_windows[j].VIP_serve_time+VserveTime[j]; //统计各个窗口的服务总时长
				    }
				    break;
			    case SERVE:				//窗口服务状态
				    if(single != 1)		//vip服务状态
				    {
					    VserveFinish[j] = clock();				//当前时间作为服务结束时间
					    time1=(int)(VserveFinish[j] - VserveBegin[j])/CLOCKS_PER_SEC;
					    if(time1>= VserveTime[j]){          //如果结束时间和开始时间之差大于随机服务时间，窗口状态改为空闲
                                                        Sleep(1000);
                                                        VIP_windows[j].Vstate = FREE;
                                                        break;
					    }
				    }
                                    else   //顺带服务普通客户
				    {
                                        VserveFinish[j] = clock();				//当前时间作为服务结束时间
                                        time1=(int)(VserveFinish[j] - VserveBegin[j])/CLOCKS_PER_SEC;
                                        if( time1>= VserveTime[j]){//如果结束时间和开始时间之差大于随机服务时间，改为FREE
                                                Sleep(1000);
                                                VIP_windows[j].Vstate = FREE;
                                                single=0;    //vip服务状态
                                        }
                                        break;
				    }
			    case REST:				//窗口休息状态
                                VfinishRest[j] = clock();					//当前时间作为休息结束时间
                                time1=(int)(VfinishRest[j] - VbeginRest[j])/CLOCKS_PER_SEC;
                                if(time1 >= VrestTime[j]){      //如果结束时间和开始时间之差大于随机休息时间，状态改为空闲
                                        Sleep(1000);
                                        VIP_windows[j].Vstate = FREE;
                                        break;
                                }
                            case CLOSE:				//窗口关闭状态
				    break;
			    case CHANGE:
			        win_VIP_num[j]++;
			        VIP_windows[j].VIP_number = deGqueue(&GHEADPTR, &GTAILPTR);//得到普通客户号码
                                VserveBegin[j] = clock();					//此时VIP窗口进行顺带服务，服务开始计时
                                nordoing++;
                                Sleep(1000);
                                VIP_windows[j].Vstate = SERVE;				//窗口进入服务状态
                                VserveTime[j] = Minsec + (rand()%Maxsec);	//得到随机服务时间
                                VIP_windows[j].VIP_serve_time=VIP_windows[j].VIP_serve_time+VserveTime[j];
                                break;
                        }
                }
	}
}


void decreaseOrIncrease()
{
    int s, s1, counter=0,i;			//s为开启的普通窗口数，s1为开启的VIP窗口数
	int count1=0,count2=0,count3=0,count4=0;  //vip窗口各个状态的窗口数
	int number3;   //记录队列中排队的人数
    for(s = 0; s <= 5; s++)				//判断普通窗口开启数量
	    {
		    if(windows[s].Gstate == CLOSE||windows[s].Gstate == REST)
			counter++;
	    }
	    counter=6-counter;

	    for(s1 = 0; s1 <= 1; s1++)			//判断VIP窗口开启数量
	    {
		    if(VIP_windows[s1].Vstate == SERVE|| VIP_windows[s1].Vstate==CHANGE)
			    count1++;                   //处于服务的VIP窗口数量
            else if(VIP_windows[s1].Vstate==CLOSE)
                count2++;                   //处于关闭状态的VIP窗口数量
            else if(VIP_windows[s1].Vstate==FREE)
                count3++;                   //处于空闲状态的VIP窗口数量
            else
                count4++;                   //处于休息状态的VIP窗口数量
	    }

	    if(count3==2||(count1==1&&count3==1)){   //有两个窗口空闲或者一个窗口空闲，一个窗口服务时，考虑减少窗口
            if((vipwaiting-vipdoing)<=3){        //处于等待区的VIP客户人数小于三人
                for(i=0;i<=1;i++){
                    if(VIP_windows[i].Vstate==FREE)
                        VIP_windows[i].Vstate=CLOSE;
                }
            }
	    }
	    if(count2==1){     //当有一个窗口处于关闭时，考虑是否开启
            if((vipwaiting-vipdoing)>3){
                for(i=0;i<=1;i++){
                    if(VIP_windows[i].Vstate==CLOSE)
                        VIP_windows[i].Vstate=FREE;
                }
            }
        }

        number3=norwaiting-nordoing;            //普通客户等待的人数

	    int u=0,kill=0;       //flag
	    if(counter>=3){
            while(u==0){      //开放服务的窗口数大于3时，进入，判断是否需要减少窗口
                if(number3<2*counter&&counter>3){       //等待人数小于窗口数的2倍时,且提供服务的窗口数不小于3
                    for(i=0;i<=5;i++){
                        if(windows[i].Gstate==SERVE){    //统计此时正在办理业务的窗口数
                            kill++;
                        }
                        if(windows[i].Gstate==FREE){   //将空闲的窗口关闭
                            windows[i].Gstate=CLOSE;
                            counter--;        //提供服务的窗口数减一
                            break;            //立即跳出循环
                        }
                        if(kill==counter)  //若此时服务窗口全都在办理业务，则不减少窗口的数量
                            u=1;
	                }
                }
                else
                    u=1;
            }
            u=0;//BIOSHIFU
            while(u==0){              //判断是否增加窗口
                if(number3>=3*counter&&counter>=3&&counter!=6){            //等待人数大于窗口数的3倍时，且提供服务的窗口数不小于3，且窗口数最大为6
                    for(i=0;i<=5;i++){
                        if(windows[i].Gstate==CLOSE){      //将关闭的窗口开启服务
                            windows[i].Gstate=FREE;
                            counter++;        //提供服务的窗口数加一
                            break;
                        }
                    }
                }
                else
                    u=1;
            }
	    }

}


/****************普通客户队列、vip客户队列函数*******************/
/*--------------以下是普通客户的入队出队以及判断队列是否为空的函数----------*/
void enGqueue(nor_QueuePtr *GHEADPTR,nor_QueuePtr *GTAILPTR)   //指向队头和队尾的指针的地址
{
    nor_QueuePtr newPtr;      //新节点
    newPtr=(nor_QueuePtr)malloc(sizeof(nor_QueuePtr));      //分配内存单元

    if(newPtr!=NULL){           //内存分配成功
        newPtr->number=nortotal;     //为顾客分配普通客户号码
        newPtr->nextPtr=NULL;
        if(isGempty(*GHEADPTR))       //若队列为空
            *GHEADPTR=newPtr;
        else
            (*GTAILPTR)->nextPtr=newPtr;
        *GTAILPTR=newPtr;
    }
    else
        printf("NO memory available.\n");
}

int deGqueue(nor_QueuePtr *GHEADPTR,nor_QueuePtr *GTAILPTR)
{
    int value;
    nor_QueuePtr tempPtr;

    tempPtr=*GHEADPTR;     //tempPtr用来释放头节点内存
    value=(*GHEADPTR)->number;
    *GHEADPTR=(*GHEADPTR)->nextPtr;    //*headPtr指向队列中新的第一个结点

    if(*GHEADPTR==NULL)
        *GTAILPTR=NULL;    //此时队列为空队列

    free(tempPtr);
    return value;
}

int isGempty(nor_QueuePtr GHEADPTR)
{
    return GHEADPTR==NULL;
}

/*------------------以下是vip客户入队出队以及判断队列是否为空的函数----------------------*/

void enVqueue(vip_QueuePtr *VHEADPTR,vip_QueuePtr *VTAILPTR)
{
    vip_QueuePtr newPtr;
    newPtr=(vip_QueuePtr)malloc(sizeof(vip_QueuePtr));


    if(newPtr!=NULL){
        newPtr->vip_number=viptotal;
        //newPtr->time=5+(rand()%15);
        newPtr->nextPtr=NULL;
        if(isVempty(*VHEADPTR))
            *VHEADPTR=newPtr;
        else
            (*VTAILPTR)->nextPtr=newPtr;
        *VTAILPTR=newPtr;
    }
    else
        printf("NO memory available.\n");
}

int deVqueue(vip_QueuePtr *VHEADPTR,vip_QueuePtr *VTAILPTR)
{
    int value;
    vip_QueuePtr tempPtr;

    tempPtr=*VHEADPTR;
    value=(*VHEADPTR)->vip_number;  //保存数据
    *VHEADPTR=(*VHEADPTR)->nextPtr;

    if(*VHEADPTR==NULL)
        *VTAILPTR=NULL;

    free(tempPtr);
    return value;
}

int isVempty(vip_QueuePtr VHEADPTR)
{
    return VHEADPTR==NULL;
}



