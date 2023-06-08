/******************************************************************************
 *  版权所有（C）terry.zhao                                                   *
 *  保留所有权利。                                                            *
*******************************************************************************
 *  来源 : https://github.com/k1988/win_mini_tool
 *  版本 : 1.0
 *  功能说明：监控指定窗口，并一直打印出该窗口信息
******************************************************************************/
/*  修改记录: 
      日期       版本    修改人             修改内容
    --------------------------------------------------------------------------
******************************************************************************/

#include <windows.h>
#include <stdio.h>
#include <map>
#include <string>
#include <iostream>
#include <memory>
#include <functional>
#include <Psapi.h>

#pragma comment(lib, "User32.lib")

#define INFO_LEN 1024

void DumpWindow(HWND hwnd, char info[INFO_LEN])
{
	char title[MAX_PATH] = {0};
	char className[MAX_PATH] = {0};

	GetWindowText(hwnd, title, MAX_PATH);
	GetClassName(hwnd, className, MAX_PATH);

    bool visible = ::IsWindowVisible(hwnd);
	RECT rcApp;
	GetWindowRect(hwnd, &rcApp);//获取当前激活窗口的坐标
	
	RECT rcDesk;
	GetWindowRect(GetDesktopWindow(), &rcDesk);//根据桌面窗口句柄，获取整个屏幕的坐标
	
	sprintf(info, "0x%x [%s] \n\ttitle=%s\n\tclass=%s\n\trect: %d,%d,%d,%d\n\n", hwnd, visible?"Visible":"Hide", title, className, rcApp.left, rcApp.top, rcApp.right, rcApp.bottom);	
}


BOOL WINAPI EnumChildProc(HWND hWnd, LPARAM lParam)
{
    DWORD pid = (DWORD)(lParam);
    DWORD processId = 0;
    if (!::GetWindowThreadProcessId(hWnd, &processId))
    {
        return true;
    }

    if (processId == pid)
    {
        char info[INFO_LEN] = { 0 };
        DumpWindow(hWnd, info);
        printf("Find result: %s\n", info);
    }
    return true;
}

BOOL WINAPI EnumChildProcByPName(HWND hWnd, LPARAM lParam)
{
	auto pName = (char*)lParam;
	DWORD processId = 0;
	if (!::GetWindowThreadProcessId(hWnd, &processId))
	{
		return true;
	}

	static std::map<DWORD /*pid*/, std::string /*pname*/> s_processNameCache;
	if (s_processNameCache.find(processId) == s_processNameCache.end())
	{				
		CHAR tsFileDosPath[MAX_PATH + 1];
		ZeroMemory(tsFileDosPath, sizeof(CHAR)*(MAX_PATH + 1));
		HANDLE hProcess = ::OpenProcess(GENERIC_READ | PROCESS_VM_READ | SYNCHRONIZE, false, processId);
		if (0 == GetProcessImageFileName(hProcess, tsFileDosPath, MAX_PATH + 1))
		{
			s_processNameCache[processId] = {};
			return true;
		}
		std::string path = tsFileDosPath;
		path = path.substr(path.rfind("\\") + 1);				
		s_processNameCache[processId] = path;
	}

	auto processName = s_processNameCache[processId];
	if (processName == pName)
	{
		char info[INFO_LEN] = { 0 };
		DumpWindow(hWnd, info);
		printf("Find result in pid:%d: %s\n", processId, info);
	}
	return true;
}

void PrintWindowsInProcess(DWORD pid)
{
    ::EnumWindows(EnumChildProc, pid);
}

void Usage()
{
    printf("usage: wininfo [-w] \"handle\"");
    printf("\n\twininfo -p \"process pid\"");
	printf("\n\twininfo -pn \"process name\"");
}

void main(int argc,char*argv[])
{
    if (argc < 2){
        Usage();
        return;
    }
    enum class Mode : int
    {
        ByHandle,
        ByPid,
		ByProcessName
    };
    Mode mode = Mode::ByHandle;
    DWORD argValue = 0;
    if (argc == 2)
    {
        argValue = strtol(argv[1], NULL, 0);
    }
    else if (argc == 3)
    {
        if (strcmp("-p", argv[1]) == 0)
        {
            mode = Mode::ByPid;
        }
		else if (strcmp("-pn", argv[1]) == 0)
        {
            mode = Mode::ByProcessName;
        }
    }
    else
    {
        Usage();
        return;
    }

    if (mode == Mode::ByPid)
    {		
        argValue = strtol(argv[2], NULL, 0);
        printf("use arg as pid，print all windows in the process %d\n", argValue);
        PrintWindowsInProcess(argValue);
        return;
    }
	else if (mode == Mode::ByProcessName)
	{
        printf("use arg as pname，print all windows in the process %s\n", argv[2]);
        ::EnumWindows(EnumChildProcByPName, (LPARAM)argv[2]);
        return;
	}
    
	argValue = strtol(argv[2], NULL, 0);
    HWND hResultHwnd = (HWND)argValue;
    if (!::IsWindow(hResultHwnd))
    {
        printf("The hwnd was not found!\n");       
        return;
    }

    while(true){
        ::Sleep(1000);
        char info[INFO_LEN] = {0};
        DumpWindow(hResultHwnd, info);
        printf("Find result: %s\n", info);
        
        HWND hParent = ::GetParent(hResultHwnd); 
        if (hParent){
        DumpWindow(hParent, info);
            printf("parent: %s\n", info);    
        } else {
            printf("parent: %s\n", "NULL");        
        }
        
        hParent = (HWND)::GetWindow(hResultHwnd, GW_OWNER);
        if (hParent){
        DumpWindow(hParent, info);
            printf("owner: %s\n", info);    
        } else {
            printf("owner: %s\n", "NULL");        
        }
    }
}