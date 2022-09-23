/******************************************************************************
 *  版权所有（C）terry.zhao                                                   *
 *  保留所有权利。                                                            *
*******************************************************************************
 *  来源 : https://github.com/k1988/win_mini_tool
 *  版本 : 1.0
 *  功能说明：在不同的系统桌面之前切换，但如果该桌面未创建则切换失败
******************************************************************************/
/*  修改记录: 
      日期       版本    修改人             修改内容
    --------------------------------------------------------------------------
******************************************************************************/

#include <windows.h>
#include <stdio.h>

#pragma comment(lib, "User32.lib")

#include <Windows.h>
#include <stdio.h>

//Create a new desktop or open an existing one
HDESK CreateHiddenDesktop(CHAR* desktop_name)
{
    CHAR explorer_path[MAX_PATH];
    HDESK hidden_desktop = NULL, original_desktop;
    STARTUPINFOA startup_info = { 0 };
    PROCESS_INFORMATION process_info = { 0 };

    ExpandEnvironmentStringsA("%windir%\\explorer.exe", explorer_path, MAX_PATH - 1);

    hidden_desktop = OpenDesktopA(desktop_name, NULL, FALSE, GENERIC_ALL);
    if (!hidden_desktop)
    {
        hidden_desktop = CreateDesktopA(desktop_name, NULL, NULL, 0, GENERIC_ALL, NULL);
        if (hidden_desktop)
        {
            original_desktop = GetThreadDesktop(GetCurrentThreadId());

            if (SetThreadDesktop(hidden_desktop))
            {
                startup_info.cb = sizeof(startup_info);
                startup_info.lpDesktop = desktop_name;

                //We need to create an explorer.exe in the context of the new desktop for start menu, etc
                CreateProcessA(explorer_path, NULL, NULL, NULL, FALSE, 0, NULL, NULL, &startup_info, &process_info);

                SetThreadDesktop(original_desktop);
            }
        }
    }
    return hidden_desktop;
}


int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        printf("Usage:SwitchDesktop <Desktop Name>\n");
    }


    HDESK original_desktop, hidden_desktop;

    hidden_desktop = CreateHiddenDesktop(argv[1]);
    original_desktop = GetThreadDesktop(GetCurrentThreadId());

    printf("Entering hidden desktop\n");

    //Need to switch thread into context of new desktop to register hotkeys
    SetThreadDesktop(hidden_desktop);

    if (SwitchDesktop(hidden_desktop))
    {
        printf("切换桌面成功");
    }
    else
    {
        printf("切换桌面失败：%d\n", GetLastError());
    }

    // Ctrl+Alt+S 切换回原桌面
    if (RegisterHotKey(NULL, 1, MOD_CONTROL | MOD_ALT , 0x53))
    {
        MSG msg = { 0 };
        while (GetMessage(&msg, NULL, 0, 0) != 0)
        {
            if (msg.message == WM_HOTKEY)
            {
                printf("Exiting hidden desktop\n");
                SwitchDesktop(original_desktop);
                break;
            }
        }
    }
    else
    {
        printf("RegisterHotKey Failed, switch to original desktop\n");
        SwitchDesktop(original_desktop);
    }

    CloseDesktop(hidden_desktop);
    CloseHandle(hidden_desktop);
    getchar();
}