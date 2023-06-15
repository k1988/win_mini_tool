/******************************************************************************
 *  版权所有（C）terry.zhao                                                   *
 *  保留所有权利。                                                            *
*******************************************************************************
 *  来源 : https://github.com/k1988/win_mini_tool
 *  版本 : 1.0
 *  功能说明：将所有可见窗口移入到当前鼠标所在显示器区域。
 使用场景，机器多显示器的情况下：
 1、在其它单屏电脑上使用远程工具连接上来只能显示一个显示器，这时打开的其它显示器的内容不可见。
 2、多显示器但关闭了副屏，之前副屏上的所有窗口不可见。有时懒得开电源，就想移动窗口到主屏幕中。
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
#include <set>

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
    GetWindowRect(hwnd, &rcApp); // 获取当前激活窗口的坐标

    RECT rcDesk;
    GetWindowRect(GetDesktopWindow(), &rcDesk); // 根据桌面窗口句柄，获取整个屏幕的坐标

    sprintf(info, "0x%x [%s] \n\ttitle=%s\n\tclass=%s\n\trect: %d,%d,%d,%d\n\n", hwnd, visible ? "Visible" : "Hide", title, className, rcApp.left, rcApp.top, rcApp.right, rcApp.bottom);
}

BOOL WINAPI EnumWindowProc(HWND hWnd, LPARAM lParam)
{
    auto monitorInfo = (MONITORINFO *)(lParam);

    if (!::IsWindowVisible(hWnd))
        return TRUE;
    if (::IsIconic(hWnd))
        return TRUE;

    char className[MAX_PATH] = {0};
    GetClassName(hWnd, className, MAX_PATH);
    std::set<std::string> whiteList = {
        "WorkerW",
        "SoWB_Status",
        "Shell_SecondaryTrayWnd"};
    if (whiteList.count(std::string(className)) > 0)
    {
        return TRUE;
    }

    RECT winRect;
    GetWindowRect(hWnd, &winRect);
    if (winRect.right - winRect.left < 100 && winRect.bottom - winRect.top < 100)
    {
        return TRUE;
    }

    auto rect = monitorInfo->rcMonitor;

    // 如果窗口不在当前显示器，则把它左上角移到当前显示器位置
    // 有些窗口有透明边，在其它显示器上显示时还占用一些像素，所以这里判断距当前显示器的边大于20才算不在当前显示器上
    if (winRect.left > rect.right - 20 || winRect.right < rect.left + 20 || winRect.bottom < rect.top - 20 || winRect.top > rect.bottom + 20)
    {
        char buffer[1024];
        DumpWindow(hWnd, buffer);
        printf("found window not in current monitor:\n\t%s", buffer);

        // 如果最大化，则取消最大化状态，否则无法移动
        if (::IsZoomed(hWnd))
        {
            ::ShowWindow(hWnd, SW_SHOWNORMAL);
        }
        ::MoveWindow(hWnd, rect.left, rect.top, winRect.right - winRect.left, winRect.bottom - winRect.top, TRUE);
    }

    return TRUE;
}

void Usage()
{
    printf("usage: windows_in_one.exe");
}

void main(int argc, char *argv[])
{
    DWORD argValue = 0;
    if (argc == 2)
    {
        if (strcmp("-h", argv[1]) == 0 || strcmp("/?", argv[1]) == 0 || strcmp("--help", argv[1]) == 0)
        {
            Usage();
            return;
        }
    }

    POINT mousePos = {0};
    ::GetCursorPos(&mousePos);
    auto monitor = MonitorFromPoint(mousePos, 0);
    MONITORINFO monitorInfo;
    memset(&monitorInfo, 0, sizeof(monitorInfo));
    monitorInfo.cbSize = sizeof(monitorInfo);
    GetMonitorInfo(monitor, &monitorInfo);
    printf("current cursor pos is:%d,%d\n", mousePos.x, mousePos.y);
    printf("current monitor pos is:%d,%d,%d,%d\n", monitorInfo.rcMonitor.left, monitorInfo.rcMonitor.top, monitorInfo.rcMonitor.right, monitorInfo.rcMonitor.bottom);

    ::EnumWindows(EnumWindowProc, (LPARAM)&monitorInfo);
}