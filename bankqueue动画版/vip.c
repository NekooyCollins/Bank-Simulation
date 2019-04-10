
#include "vip.h"

int vipJudge(int VIPID)         //判断客户输入的vipid是否zhengque
{
    int YES;
    struct VIPInformation client;
    FILE *cfPtr=fopen("vipdata.txt", "r");

    if((cfPtr=fopen("credit.dat","rb"))==NULL)
        printf("客户资料文件无法访问!\n");
    else{
        fseek(cfPtr,(VIPID-1)*sizeof(struct VIPInformation),SEEK_SET);
        fread(&client,sizeof(struct VIPInformation),1,cfPtr);        //根据id的偏移量来搜索数据
        if(client.VIPID==VIPID){         //比较id
                YES=1;          //正确id
        }
        else
               YES=-1;

    }
    if(YES==1)
        return 1;
    else
        return -1;
}



