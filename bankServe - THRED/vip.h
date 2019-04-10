#ifndef VIP_H
#define VIP_H
#include<stdio.h>
//#include<stdlib.h>

struct VIPInformation
{
    int VIPID;   // VIP的ID号
    char Firstname[10];    // 姓名
    char Lastname[10];
    double balance;   // 余额
};

/***********VIP客户资料维护函数原型************/
void vipMaintenance(void);      //vip客户资料的维护总程序
int enterChoice(void);         //vip客户资料维护菜单选项
void printFile(FILE *);         //查看客户资料函数
void updataRecord(FILE *);       //更新客户资料信息
void newRecord(FILE *);          //加入新的vip客户信息
void deleteRecord(FILE *);       //删除客户资料信息

int vipJudge(void);          //判断客户输入的vipid是否zhengque

#endif // _VIP_H_

