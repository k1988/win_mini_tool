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


void main(int argc,char*argv[])
{
    if (argc < 2){
        printf("usage: wininfo \"handle\"");
    }
        
    HWND hResultHwnd = (HWND)strtol(argv[1], NULL, 0);
    if (!hResultHwnd){
        printf("Not Found");
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