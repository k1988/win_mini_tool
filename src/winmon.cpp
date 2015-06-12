/******************************************************************************
 *  版权所有（C）terry.zhao                                                   *
 *  保留所有权利。                                                            *
*******************************************************************************
 *  来源 : https://github.com/k1988/win_mini_tool
 *  版本 : 1.0
 *  功能说明：监控系统当前顶层窗口，并打印出该窗口信息
******************************************************************************/
/*  修改记录: 
      日期       版本    修改人             修改内容
    --------------------------------------------------------------------------
******************************************************************************/

#include <windows.h>
#include <stdio.h>

#pragma comment(lib, "User32.lib")

#define INFO_LEN 1024

void DumpWindow(HWND hwnd, char info[INFO_LEN])
{
	char title[MAX_PATH] = {0};
	char className[MAX_PATH] = {0};

	GetWindowText(hwnd, title, MAX_PATH);
	GetClassName(hwnd, className, MAX_PATH);
	
	sprintf(info, "%x \n\ttitle=%s\n\tclass=%s\n", hwnd, title, className);
}


void main()
{
	do 
	{
		HWND forge = ::GetForegroundWindow();

		//当前前置窗口的线程id
		DWORD dwForeID = GetWindowThreadProcessId(forge, NULL);

		//目标窗口的线程id
		DWORD dwDstID = GetWindowThreadProcessId(forge, NULL);

		::AttachThreadInput(dwDstID, dwForeID, TRUE);
		HWND focus = ::GetFocus();
		HWND active = ::GetActiveWindow();
		::AttachThreadInput(dwDstID, dwForeID, FALSE);

		char info[INFO_LEN] = {0};
		DumpWindow(forge, info);
		printf("forge: %s\n", info);

		//DumpWindow(focus, info);
		//printf("focus: %s\n", info);

		//DumpWindow(active, info);
		//printf("active: %s\n", info);

		Sleep(1000);
	} while (true);
}
