
#include "vip.h"

/***************VIP客户资料维护函数************************/
int enterChoice()    //vip客户资料维护菜单选项
{
    int menuChoice;

    printf("\n请输入您的选项\n1-查看VIP客户资料\n2-更新客户资料\n3-添加客户资料\n4-删除客户资料\n5-结束客户资料维护\n?");
    scanf("%d",&menuChoice);
    return menuChoice;
}

void vipMaintenance()     //vip客户资料维护函数
{
    FILE *cfPtr;           //指向客户资料文件的指针
    int choice;

    if((cfPtr=fopen("credit.dat","r+"))==NULL)
        printf("文件无法打开！\n");
    else{
        while((choice=enterChoice())!=5){       //输入选项
            switch(choice){
            case 1:
                printFile(cfPtr);
                break;
            case 2:
                updataRecord(cfPtr);
                break;
            case 3:
                newRecord(cfPtr);
                break;
            case 4:
                deleteRecord(cfPtr);
                break;
            }
        }
    }
    fclose(cfPtr);        //维护完毕
}

void printFile(FILE *readPtr)        //查看客户资料函数
{
    struct VIPInformation client;
    rewind(readPtr);
    fread(&client,sizeof(struct VIPInformation),1,readPtr);   //读取一条信息存入结构体client
    while(!feof(readPtr)){
        if(client.VIPID!=0)
            printf("%-12d%-16s%-11s%10lf\n",client.VIPID,client.Firstname,client.Lastname,client.balance);      //打印信息
        fread(&client,sizeof(struct VIPInformation),1,readPtr);
    }
}


void updataRecord(FILE *fPtr)     //更新客户资料信息
{
    int VIPID,n,counter;
    struct VIPInformation client;

    counter=1; //控制循环
    printf("请输入要更新VIP客户的人数:");
    scanf("%d",&n);

    while(counter<=n){
        printf("\n请输入客户的ID(1-100):");
        scanf("%d",&VIPID);
        fseek(fPtr,(VIPID-1)*sizeof(struct VIPInformation),SEEK_SET);       //寻找该id信息
        fread(&client,sizeof(struct VIPInformation),1,fPtr);                //将寻找到的信息存入client

        if(client.VIPID==0)
            printf("此VIPID为%d的客户不存在\n",VIPID);
        else{
            printf("%-12d%-16s%-11s%-10f\n\n",client.VIPID,client.Firstname,client.Lastname,client.balance);     //打印原始数据
            printf("\n请输入新的客户信息(ID，姓名，余额)：");
            scanf("%d%s%s%lf",&client.VIPID,client.Firstname,client.Lastname,&client.balance);
            fseek(fPtr,(VIPID-1)*sizeof(struct VIPInformation),SEEK_SET);        //根据id偏移量来存入新数据
            fwrite(&client,sizeof(struct VIPInformation),1,fPtr);
        }
        counter++;
    }
}


void deleteRecord(FILE *fPtr)      //删除客户资料信息
{
    struct VIPInformation client,blankClient={0,"","",0.0};      //定义可以空白的结构体
    int n,counter,vip[100];

    printf("请输入要删除客户的数目:");
    scanf("%d",&n);
    printf("请输入所有要删除客户的ID:");
    for(counter=0;counter<=n-1;counter++)
        scanf("%d",&vip[counter]);

    for(counter=1;counter<=n;counter++){
        fseek(fPtr,(vip[counter-1]-1)*sizeof(struct VIPInformation),SEEK_SET);
        fread(&client,sizeof(struct VIPInformation),1,fPtr);
        if(client.VIPID==0)
            printf("ID为%d的用户不存在.\n",vip[counter-1]);
        else{
            fseek(fPtr,(vip[counter-1]-1)*sizeof(struct VIPInformation),SEEK_SET);
            fwrite(&blankClient,sizeof(struct VIPInformation),1,fPtr);              //用空白结构体覆盖原始数据达到删除目的
        }
    }
}

void newRecord(FILE *fPtr)        //加入新的vip客户信息
{
    struct VIPInformation client;
    int ID,n,counter;

    printf("请输入新VIP客户的人数:");
    scanf("%d",&n);
    for(counter=1;counter<=n;){
        printf("请输入新的VIP客户的ID(1-100):");
        scanf("%d",&ID);
        fseek(fPtr,(ID-1)*sizeof(struct VIPInformation),SEEK_SET);
        fread(&client,sizeof(struct VIPInformation),1,fPtr);          //搜索是否存改id的信息

        if(client.VIPID!=0)
            printf("ID为%d的用户已存在.\n",client.VIPID);
        else{
            printf("请输入姓名,金额：\n");
            scanf("%s%s%lf",client.Firstname,client.Lastname,&client.balance);     //读取新的信息
            client.VIPID=ID;
            fseek(fPtr,(client.VIPID-1)*sizeof(struct VIPInformation),SEEK_SET);
            fwrite(&client,sizeof(struct VIPInformation),1,fPtr);         //将新信息写入文件
            counter++;
        }
    }

}



int vipJudge()         //判断客户输入的vipid是否zhengque
{
    int time,VIPID;
    int YES;
    struct VIPInformation client;
    FILE *cfPtr=fopen("vipdata.txt", "r");

    if((cfPtr=fopen("credit.dat","rb"))==NULL)
        printf("客户资料文件无法访问!\n");
    else{
        printf("请输入您的VIPID(只允许输入三次):\n");
        scanf("%d",&VIPID);
        fseek(cfPtr,(VIPID-1)*sizeof(struct VIPInformation),SEEK_SET);
        fread(&client,sizeof(struct VIPInformation),1,cfPtr);        //根据id的偏移量来搜索数据

        for(time=2;time>0;time--){
            if(client.VIPID==VIPID){         //比较id
                YES=1;          //正确id
                break;
            }
            else{
                printf("您输入的ID号码有误，请重新输入:\n");
                scanf("%d",&VIPID);
                fseek(cfPtr,(VIPID-1)*sizeof(struct VIPInformation),SEEK_SET);
                fread(&client,sizeof(struct VIPInformation),1,cfPtr);
            }
        }
        if(time==0){
            printf("很抱歉，您已输入三次错误ID，系统将为您分配普通客户服务号码\n");
            YES=0;
        }

    }
    if(YES==1)
        return 1;
    else
        return 0;
}


