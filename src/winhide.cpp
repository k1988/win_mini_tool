/******************************************************************************
 *  版权所有（C）terry.zhao                                                   *
 *  保留所有权利。                                                            *
*******************************************************************************
 *  来源 : https://github.com/k1988/win_mini_tool
 *  版本 : 1.0
 *  功能说明：监控指定名称窗口，并且隐藏之
******************************************************************************/
/*  修改记录: 
      日期       版本    修改人             修改内容
    --------------------------------------------------------------------------
******************************************************************************/

#include <windows.h>
#include <stdio.h>

#pragma comment(lib, "User32.lib")

//#define CHANGE_TITLE 0

//#define CHANGE_ICON 0

#define CHANGE_WINDOW 1

volatile bool quit = 0;
BOOL stop(DWORD signal)
{
	switch(signal)
	{
	case CTRL_C_EVENT:    
	case CTRL_BREAK_EVENT:
	case CTRL_CLOSE_EVENT:
	case CTRL_LOGOFF_EVENT:
	case CTRL_SHUTDOWN_EVENT:
		quit = 1;
		MessageBox(NULL, "", "", MB_OK);
		break;
	}
	return TRUE;
}

void main(int argn, char* argv[])
{
	if (argn < 2)
	{
		printf("usage: winhide 窗口标题 [hide|deamon|restore]\ndesc:隐藏或还原窗口\n\thide:隐藏窗口并且退出\n\tdeamon:监视窗口，一旦发现窗口则隐藏之，并且保持进程不退出，当用户退出控制台时还原窗口\n\trestore:还原被隐藏的窗口");
		return;
	}
	char* windowTitle = argv[1];
	char* mode = "hide";
	if (argn > 2)
	{
		mode = argv[2];
	}
	
	if (stricmp(mode, "restore") == 0)
	{
		HWND dest = FindWindow(NULL, windowTitle);
		if (!dest)
		{
			printf("Window not found,exit...");
			return;
		}

#ifdef CHANGE_WINDOW
		DWORD orgStyle = ::GetWindowLong(dest, GWL_EXSTYLE);
		::SetWindowLong(dest, GWL_EXSTYLE, orgStyle & ~WS_EX_TOOLWINDOW);
		::SetWindowPos(dest, NULL, 400, 400, 0, 0, SWP_NOSIZE | SWP_NOZORDER | SWP_HIDEWINDOW | SWP_FRAMECHANGED);
		//::SendMessage(dest, WM_SYSCOMMAND, SC_MINIMIZE, NULL);
		::ShowWindow(dest, SW_MINIMIZE);
#endif // CHANGE_WINDOW

		return;
	}

	SetConsoleCtrlHandler( (PHANDLER_ROUTINE)stop, TRUE);
	do 
	{
		HWND dest = FindWindow(NULL, windowTitle);
		if (!quit && !dest)
		{
			Sleep(200);
			continue;
		}

		printf("Window found: 0x%x\n", dest);
		Sleep(200);

#ifdef CHANGE_TITLE
		SetWindowText(dest, "记事本");
#endif // CHANGE_TITLE

#ifdef CHANGE_ICON
		// 以下代码未成功
		LRESULT bigIcon = SendMessage(dest, WM_GETICON,ICON_BIG, NULL);
		LRESULT smallIcon = SendMessage(dest, WM_GETICON,ICON_SMALL, NULL);
		SendMessage(dest, WM_SETICON, ICON_BIG,NULL);
		SendMessage(dest, WM_SETICON, ICON_SMALL,NULL);
#endif // CHANGE_ICON

		ShowWindow(dest, SW_HIDE);
		DWORD orgStyle = ::GetWindowLong(dest, GWL_EXSTYLE);
		::SetWindowLong(dest, GWL_EXSTYLE, orgStyle | WS_EX_TOOLWINDOW);
		//::SetWindowPos(dest, NULL, -9999, -9999, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_HIDEWINDOW | SWP_FRAMECHANGED);

		if (stricmp(mode, "deamon") != 0)
		{
			//非守护模式
			printf("not deamon mode, exit...");
			return;
		}

		printf("deamon mode...");
		while (!quit && ::IsWindow(dest))
		{
			Sleep(200);
		}

		if (quit)
		{
#ifdef CHANGE_ICON
			SendMessage(dest, WM_SETICON, ICON_BIG, (LPARAM)bigIcon);
			SendMessage(dest, WM_SETICON, ICON_SMALL, (LPARAM)smallIcon);
#endif // CHANGE_ICON

#ifdef CHANGE_TITLE
			SetWindowText(dest, windowTitle);
#endif // CHANGE_TITLE

#ifdef CHANGE_WINDOW
			::ShowWindow(dest, SW_MINIMIZE);
			::SetWindowLong(dest, GWL_EXSTYLE, orgStyle);
			::SendMessage(dest, WM_SYSCOMMAND, SC_MINIMIZE, NULL);
#endif // CHANGE_WINDOW
			break;
		}
		else
		{
			printf("Window destory: 0x%x\n", dest);
		}
	} while (true);
}
