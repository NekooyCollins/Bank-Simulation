#ifndef _CONTROL_H_
#define _CONTROL_H_


/**********初始化函数原型***********/
void Initialization1(void);
/*****************判断窗口休息函数原型*********************/
int judgeGRest(int);
int judgeVRest(int);
/*****************判断下班函数原型*********************/
int judgeQuit(void);
int judgeQuit1(void);
/*****************读取配置文件参数*************************/
void readConfigurationFile(void);

int judgeWindowsState(void);

struct canshu{
    char A[20];     //配置文件结构体
    int num;
};

#endif // CONTROL_H

