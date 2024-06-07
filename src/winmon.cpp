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
#include <TChar.h>

#pragma comment(lib, "User32.lib")

#define INFO_LEN 1024

bool IsWindowFullScreen(HWND hWnd)
{
	bool bFullscreen = false;//存放当前激活窗口是否是全屏的，true表示是，false表示不是

	RECT rcApp;
	RECT rcDesk;

	if ((hWnd != GetDesktopWindow ()) 
		&& (hWnd != GetShellWindow ()))//如果当前激活窗口不是桌面窗口，也不是控制台窗口
	{
		GetWindowRect(hWnd, &rcApp);//获取当前激活窗口的坐标
		GetWindowRect(GetDesktopWindow(), &rcDesk);//根据桌面窗口句柄，获取整个屏幕的坐标

		if (rcApp.left <= rcDesk.left && //如果当前激活窗口的坐标完全覆盖住桌面窗口，就表示当前激活窗口是全屏的
			rcApp.top <= rcDesk.top &&
			rcApp.right >= rcDesk.right &&
			rcApp.bottom >= rcDesk.bottom)
		{

			TCHAR szTemp[100];

			if (GetClassName (hWnd, szTemp, sizeof (szTemp)) > 0)//如果获取当前激活窗口的类名成功
			{
				bFullscreen = true;
			}
			else bFullscreen = true;//如果获取失败，就认为当前激活窗口是全屏窗口
		}
	}//如果当前激活窗口是桌面窗口，或者是控制台窗口，就直接返回不是全屏

	return bFullscreen;
}

void DumpWindow(HWND hwnd, char info[INFO_LEN])
{
	char title[MAX_PATH] = {0};
	char className[MAX_PATH] = {0};

	GetWindowText(hwnd, title, MAX_PATH);
	GetClassName(hwnd, className, MAX_PATH);
	
	RECT rcApp;
	GetWindowRect(hwnd, &rcApp);//获取当前激活窗口的坐标
	
	RECT rcDesk;
	GetWindowRect(GetDesktopWindow(), &rcDesk);//根据桌面窗口句柄，获取整个屏幕的坐标
	
	sprintf(info, "%x \n\ttitle=%s\n\tclass=%s\n\trect: %d,%d,%d,%d\n\tfullscreen:%d\n", hwnd, title, className, rcApp.left, rcApp.top, rcApp.right, rcApp.bottom, IsWindowFullScreen(hwnd));
	
	sprintf(info, "%s\ndesktop: %d,%d,%d,%d\n", info,rcDesk.left, rcDesk.top, rcDesk.right, rcDesk.bottom);
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
		
		HWND owner = ::GetParent(forge);
		if (owner)
		{
			DumpWindow(owner, info);
			printf("owner: %s\n", info);
		}
		
		POINT cursorPos;
        GetCursorPos(&cursorPos);
		HWND cursorWnd = ::WindowFromPoint(cursorPos);
		if (cursorWnd)
		{
			DumpWindow(cursorWnd, info);
			printf("window under cursor: %s\n", info);
		}

		//DumpWindow(focus, info);
		//printf("focus: %s\n", info);

		//DumpWindow(active, info);
		//printf("active: %s\n", info);

		Sleep(1000);
	} while (true);
}
