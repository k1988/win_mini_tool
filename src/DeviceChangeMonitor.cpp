#undef _MBCS
#define _UNICODE
#define UNICODE
// #pragma comment(linker, "/ENTRY:wWinMainCRTStartup")
#pragma comment(lib, "User32.lib")

#include <Windows.h>
#include <iostream>
#include <cstdio>
#include <Dbt.h>
#include <tchar.h>

int main()
{
    // ����WM_DEVICECHANGE��Ϣ������
    LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

    // ע�ᴰ����
    WNDCLASS wc = {};
    wc.lpfnWndProc = WndProc;
    wc.hInstance = nullptr;
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.lpszClassName = L"MyWindowClass";
    RegisterClass(&wc);

    // ��������
    HWND hWnd = CreateWindow(
        L"MyWindowClass",
        L"��ʹ��DebugView�鿴��־",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 400, 320,
        NULL, NULL, nullptr, NULL);
    ::ShowWindow(hWnd, SW_SHOWDEFAULT);
    // ��Ϣѭ��
    MSG msg;
   while(GetMessage(&msg, NULL, 0, 0) > 0)
    {
        if (msg.wParam == WM_QUIT)
        {
            break;
        }
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    printf("end of program");
    return 0;
}

inline void DebugPrint(const TCHAR *format, ...)
{
    TCHAR temp[4098] = {0};

    va_list list;
    long ret;

    va_start(list, format);
    ret = _vsntprintf_s(temp, 1024, _TRUNCATE, format, list);
    OutputDebugString(temp);
    va_end(list);
}

inline wchar_t* DeviceType(DWORD type)
{
    switch (type)
    {
    case DBT_DEVTYP_OEM:
        return L"DBT_DEVTYP_OEM";
    case DBT_DEVTYP_DEVNODE:
        return L"DBT_DEVTYP_DEVNODE";
    case DBT_DEVTYP_VOLUME:
        return L"DBT_DEVTYP_VOLUME";
    case DBT_DEVTYP_PORT:
        return L"DBT_DEVTYP_PORT";
    case DBT_DEVTYP_NET:
        return L"DBT_DEVTYP_NET";
    case DBT_DEVTYP_DEVICEINTERFACE:
        return L"DBT_DEVTYP_DEVICEINTERFACE";
    case DBT_DEVTYP_HANDLE:
        return L"DBT_DEVTYP_HANDLE";
    default:
        return L"Unkown";
    }
}



LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
    case WM_DEVICECHANGE:
    {
        // ��ӡWM_DEVICECHANGE��Ϣ

        switch (wParam)
        {
            DebugPrint(L"WM_DEVICECHANGE: event: %x , param: %x\n", wParam, lParam);
        case DBT_DEVNODES_CHANGED:
        {
            DebugPrint(L"WM_DEVICECHANGE: event: %x , param: %x\n", wParam, lParam);
        }
        break;
        case DBT_DEVICEARRIVAL:
        case DBT_DEVICEQUERYREMOVE:
        case DBT_DEVICEQUERYREMOVEFAILED:
        case DBT_DEVICEREMOVEPENDING:
        case DBT_DEVICEREMOVECOMPLETE:
        case DBT_DEVICETYPESPECIFIC:
        case DBT_CUSTOMEVENT:
        {
            wchar_t* deviceArray[] = { L"DBT_DEVICEARRIVAL", L"DBT_DEVICEQUERYREMOVE", L"DBT_DEVICEQUERYREMOVEFAILED", L"DBT_DEVICEREMOVEPENDING", L"DBT_DEVICEREMOVECOMPLETE", L"DBT_DEVICETYPESPECIFIC", L"DBT_CUSTOMEVENT" };

            auto hdr = (DEV_BROADCAST_HDR *)(lParam);
            auto deviceType = hdr->dbch_devicetype;
            DebugPrint(L"device message: %s device_type: %s ", deviceArray[wParam - 0x8000], DeviceType(deviceType));
        }
        break;
        }
    }
    break;
    case WM_CLOSE:
    {
        DestroyWindow(hWnd);
    }
    break;
    case WM_DESTROY:
    {
        PostQuitMessage(0);
    }
    break;
    default:
        return DefWindowProc(hWnd, msg, wParam, lParam);
    }
}