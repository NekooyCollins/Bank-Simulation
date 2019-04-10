#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<string.h>
#include<windows.h>
#include"graphics.h"

int picnum;

extern "C"
{
    #include "head.h"
    #include "control.h"
    #include "input.h"
    #include "output.h"
    #include "queueServe.h"
    #include "vip.h"
};

void mainloop(void);
void drawGetInput(PIMAGE);
void queueServe(void);
void drawPrintOutput(void);
void setPenstyle(int);
void drawBackGround(PIMAGE);
void drawJudgeGRest(int,int,int);
void drawJudgeVRest(int,int,int);
PIMAGE cusArrive(void);
void callNum(void);
void deleteWaitPic(void);
PIMAGE drawQueueServe(PIMAGE,int);

int main()
{
    Initialization1();

    if((diaryPtr=fopen("bankDiary.dat","w"))==NULL)
        printf("文件无法打开\n");
    else{
        initgraph(1060,680);
        // 图形初始化窗口尺寸
        // 程序主循环
        mainloop();

    }
    writeDiary();       //记录日志
    fclose(diaryPtr);
    // 关闭绘图设备
	getch();
	closegraph();
	return 0;
}

void mainloop()
{
    DWORD ThreadID1 = 1;
    HANDLE hRead1 = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)queueServe, NULL, 0, &ThreadID1);

    CloseHandle(hRead1);

    PIMAGE img;
    img=newimage();
    getimage(img,"bank.png");

    while(isquit!=finish_working){
        int b;
        b=judgeQuit();
        if(b==1)
            isquit=finish_working;
        drawGetInput(img);
    }
    drawBackGround(img);
    drawPrintOutput();
}


void drawGetInput(PIMAGE img)
{
    char message[100]={' '};
	mouse_msg msg = {0};
	int flag=0;
	int FLAG=1;
	int counter=0;
    int i;
	int ID=0;

	PIMAGE img1;
	PIMAGE IMGgetnum,IMGwait,IMGdoing;
    IMGgetnum=newimage();
    getimage(IMGgetnum,"m1.png");

	IMGwait=newimage();
	getimage(IMGwait,"mw.png");

	IMGdoing=newimage();
	getimage(IMGdoing,"md1.png");

	img1=newimage();

	int a;
	setPenstyle(12);


	for ( ; isquit!=finish_working; delay_fps(60))
	{
	    int c;
	    c=judgeQuit();
	    if(c==1)
            isquit=finish_working;
		//获取鼠标消息，这个函数会等待，等待到有消息为止
		if(isquit==working){
            while (mousemsg())
            {
                msg = getmouse();
            }
            if(msg.is_up())
                flag=0;
		}
        //普通客户到达
        if(msg.x>=140 && msg.x<=170 && msg.y>=350 && msg.y<=370 && msg.is_down() && flag==0){
            getInput('G');

            FLAG=0;
            sprintf(message,"尊敬的客户，您的号码为%d，请到休息区等待",nortotal);
            flag=1;
        }
        //VIP客户到达
        if(msg.x>=140 && msg.x<=170 && msg.y>=380 && msg.y<=405 && msg.is_down() && flag==0){
            int aflag=0;
            int time;
            for(time=2;time>=0;time--){
                if(aflag==0){
                    inputbox_getline("请输入", "请输入您的VIPID", message, 100);
                    outtextxy(130,415, message);        //显
                    ID = atoi(message);                 //字符串强制转换为数字
                    if(vipJudge(ID)==1){            //判断
                        getInput('V');
                        FLAG=0;
                        sprintf(message,"尊敬的VIP客户，您的号码为V%d，请到休息区等待",viptotal);
                        aflag=1;
                        break;
                    }
                    else{
                        outtextxy(100, 450, "ID 输入错误!!!!");
                        aflag=0;
                    }
                }
            }
            if(time==-1){
                getInput('G');
                sprintf(message,"尊敬的客户，您已经三次ID输入错误，为您分配普通客户号码。您的号码为%d，请到休息区等待",nortotal);
                outtextxy(20,310,message);
            }
            flag=1;
        }

        //退出
        if(msg.x >=795 && msg.x<=945 && msg.y>=480 && msg.y<=580 && msg.is_down() && flag==0){
            getimage(img1,"exit_01.png");
            putimage_withalpha(NULL,img1,797,480);
            enter_Q=1;
            a=judgeQuit();
            if(a==1)
                isquit=finish_working;
            flag=1;
        }
        //1号普通窗口申请休息
        if(msg.x>=190 && msg.x<=230 && msg.y>=170 && msg.y<=180 && msg.is_down() && flag==0){
            drawJudgeGRest(0,190,170);
            flag=1;
        }
        //2号普通窗口申请休息
        if(msg.x>=285 && msg.x<=320 && msg.y>=170 && msg.y<=180 && msg.is_down() && flag==0){
            drawJudgeGRest(1,285,170);
            flag=1;
        }
        //3号普通窗口申请休息
        if(msg.x>=370 && msg.x<=410 && msg.y>=170 && msg.y<=180 && msg.is_down() && flag==0){
            drawJudgeGRest(2,370,170);
            flag=1;
        }
        //4号普通窗口申请休息
        if(msg.x>=455 && msg.x<=495 && msg.y>=170 && msg.y<=180 && msg.is_down() && flag==0){
            drawJudgeGRest(3,455,170);
            flag=1;
        }
        //5号普通窗口申请休息
        if(msg.x>=545 && msg.x<=580 && msg.y>=170 && msg.y<=180 && msg.is_down() && flag==0){
            drawJudgeGRest(4,545,170);
            flag=1;
        }
        //6号普通窗口申请休息
        if(msg.x>=635 && msg.x<=670 && msg.y>=170 && msg.y<=180 && msg.is_down() && flag==0){
            drawJudgeGRest(5,635,670);
            flag=1;
        }

        //1号VIP窗口申请休息
        if(msg.x>=720 &&msg.x<=760 && msg.y>=170 && msg.y<=180 && msg.is_down() && flag==0){
            drawJudgeVRest(0,720,170);
            flag=1;
        }
        //2号VIP窗口申请休息
        if(msg.x>=805 &&msg.x<=840 && msg.y>=170 && msg.y<=180 && msg.is_down() && flag==0){
            drawJudgeVRest(1,805,170);
            flag=1;
        }

        drawBackGround(img);
        outtextxy(300,400,message);

	    int x,y;

	    x=40+counter*1;
	    if(x<=120&&FLAG==0){
            putimage_withalpha(NULL,IMGgetnum,x,500);
            counter++;
	    }
        else{
            counter=0;
            FLAG=1;
        }

        for(i=0;i<=5;i++){
            if(windows[i].Gstate==SERVE){
                putimage_withalpha(NULL,IMGdoing,100*i-90,150);
            }
        }

        for(i=0;i<=1;i++){
            if(VIP_windows[i].Vstate==SERVE){
                putimage_withalpha(NULL,IMGdoing,100*(i+6)-100,150);
            }
        }

	    callNum();
        printOutput();
        drawPrintOutput();
	}
}

void callNum()
{
    char message[40];

    setPenstyle(18);
    switch(Gtype){
        case 'G':
            sprintf(message,"尊敬的%d号客户，请您到%d号窗口办理业务",Gnum,Gwin);
            outtextxy(320,50,message);
    }

    switch(Vtype){
    case 'V':
        sprintf(message,"尊敬的V%d号客户，请您到V%d号窗口办理业务",Vnum,Vwin);
        outtextxy(320,70,message);
        break;
    case 'G':
        sprintf(message,"尊敬的%d号客户，请您到V%d号窗口办理业务",Vnum,Vwin);
        outtextxy(320,70,message);
        break;
    }
}

void drawPrintOutput()//定时输出信息
{
    int i;
    char s[15];
    int current_time,left_time;//系统时间
    clock_t time;
    int counter=1;

    while(counter==1){
        time=clock();
        current_time=(int)(time-Start)/CLOCKS_PER_SEC;

        setPenstyle(18);

        left_time=closebank-current_time;

        outtextxy(800,20,"当前时间为：");
        sprintf(s,"%ds",current_time);
        outtextxy(900,20,s);
        outtextxy(800,40,"距离下班还有");
        sprintf(s,"%ds",left_time);
        outtextxy(910,40,s);

        setPenstyle(22);
        sprintf(s,"%d",vipwaiting-vipdoing);
        outtextxy(978,333,s);

        sprintf(s,"%d",norwaiting-nordoing);
        outtextxy(978,413,s);

        if(nordoing!=norwaiting){
            sprintf(s,"%d---%d",nordoing,norwaiting);
            outtextxy(800,450,s);
        }
        else{
            outtextxy(800,450,"无客户排队\n");
        }
        if(vipdoing!=vipwaiting){
            sprintf(s,"V%d-V%d\n",vipdoing,vipwaiting);
            outtextxy(800,370,s);
        }
        else{
            outtextxy(800,370,"无VIP客户排队\n");
        }

        setPenstyle(14);
   /*******输出VIP窗口的状态*********/
        for(i=0;i<=1;i++){
            switch (VIP_windows[i].Vstate)
            {
            case FREE:
                outtextxy(180+i*90+540,128,"FREE");
                sprintf(s,"%d",i-i);
                outtextxy(180+i*90+540+10,146,s);

                break;
            case SERVE:
                if(single==0){
                    outtextxy(180+i*90+540,128,"SERVE");
                    sprintf(s,"V%d",VIP_windows[i].VIP_number);
                    outtextxy(180+i*90+540,146,s);
                }
                else{
                    outtextxy(180+i*90+540,128,"SERVE");
                    sprintf(s,"%d",VIP_windows[i].VIP_number);
                    outtextxy(180+i*90+540,146,s);
                }
                break;
            case REST:
                outtextxy(180+i*90+540,128,"REST");
                break;
            case CLOSE:
                outtextxy(180+i*90+540,128,"CLOSE");
                break;
            }
        }
    /**************输出普通客户的状态********/
        for(i=0;i<=5;i++){
            switch (windows[i].Gstate)
            {
            case FREE:
                outtextxy(195+i*90,128,"FREE");
                sprintf(s,"%d",0);
                outtextxy(195+i*90+10,146,s);
                break;
            case SERVE:
                outtextxy(195+i*90,128,"SERVE");
                sprintf(s,"%d",windows[i].G_number);
                outtextxy(195+i*90,146,s);
                break;
            case REST:
                outtextxy(195+i*90,128,"REST");
                break;
            case CLOSE:
                outtextxy(185+i*90,128,"CLOSE");
                break;
            }
        }

        if(enter_Q==1)
            judgeQuit();

        switch (isquit)
        {
            case working:
                outtextxy(830,590,"银行正在营业");
                break;
            case waiting_finish_working:
                if(enter_Q==1){
                    outtextxy(830,590,"已接收下班申请\n银行正在等待下班");
                }
                else{
                    outtextxy(830,590,"下班时间到\n银行正在等待下班");
                }
                break;
            case finish_working:
                outtextxy(830,590,"银行已下班");
                break;
        }
        counter++;
    }
}

void drawJudgeGRest(int k,int x,int y)
{
    PIMAGE img;
    img=newimage();
    if(judgeGRest(k)!=0){
        getimage(img,"NO.png");
        putimage_withalpha(NULL,img,x,y);
    }
    else{
        getimage(img,"resting.png");
        putimage_withalpha(NULL,img,x,y);
    }
}

void drawJudgeVRest(int a,int x,int y)
{
    PIMAGE img;
    img=newimage();
    if(judgeVRest(a) == 0){
        getimage(img,"resting.png");
        putimage_withalpha(NULL,img,x,y);
    }
    else{
        getimage(img,"NO.png");
        putimage_withalpha(NULL,img,x,y);
    }
}

void drawBackGround(PIMAGE img)    //画背景
{

    putimage(0,0,img);
}

void setPenstyle(int a)
{
    setcolor(RED);
    setfont(a,0,"宋体");
    setbkmode(TRANSPARENT);  //设置字体颜色，大小，字体，背景为透明
}
