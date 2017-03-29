
// stdafx.cpp : 只包括标准包含文件的源文件
// UAV_3D_FCS.pch 将作为预编译头
// stdafx.obj 将包含预编译类型信息

#include "stdafx.h"


MessageBus g_Bus;				//消息总线全局变量
std::atomic<int> g_TSPIPackNum;	//数据包全局原子变量



std::atomic<int> g_PID = -1;  //飞机编号，默认为无效的编号-1
DWORD gStartTime = 0;
DWORD gCurTime = 0;
DOUBLE gTimeOffset = 0.0;
DOUBLE gPixel = 0;
INT g_CurID = 0;
BOOL gResetID = FALSE;//判断是否有点击滑动条


