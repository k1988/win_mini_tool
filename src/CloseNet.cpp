// 使用命令行编译: 
/*
cd /d %~dp0%
call "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat"
cl /EHsc /FeCloseNetRelease.exe CloseNet.cpp /link /manifest:embed /MANIFESTUAC:"level='requireAdministrator' uiAccess='false'"
*/
// 功能:
//   命令行: CloseNet enable 启用所有网卡
//   命令行: CloseNet disable 禁用所有网卡
//   命令行: CloseNet 先禁用所有网卡再等5秒后启用所有网卡

#define UNICODE

#include <windows.h>
#include <iphlpapi.h>
#include <iostream>
#include <vector>
#include <SetupAPI.h>
#include <cfgmgr32.h>
#include <devguid.h>
#include <tchar.h>

#pragma comment(lib, "iphlpapi.lib")
#pragma comment(lib, "Cfgmgr32.lib")
#pragma comment(lib, "SetupAPI.lib")
#pragma comment(lib, "User32.lib")
#pragma comment(lib, "Shell32.lib")

#pragma comment(linker, "/SUBSYSTEM:WINDOWS")
//#pragma comment(linker, "/SUBSYSTEM:CONSOLE")


/// @brief windows 窄字符串转换为unicode字符串
/// @param local_str 输入的待转换的字符串
/// @param codepage utf8:65001 OEMCP:1 GB2313:936
/// @return 返回转换的字符串
inline std::wstring local_to_unicode(const std::string& local_str, int codepage = 0)
{
    std::wstring result;
    int minSize = ::MultiByteToWideChar(/*codepage*/ codepage, /*flags*/ 0, local_str.c_str(), local_str.length(), NULL, 0);
    if (minSize > 0)
    {
        result.resize(minSize);
        int ret = ::MultiByteToWideChar(/*codepage*/ codepage, /*flags*/ 0, local_str.c_str(), local_str.length(),
            (wchar_t*)result.data(), minSize);
        if ((ret > 0))
        {
            return result;
        }
    }
    return {};
}

void SetAllNetworkAdaptersStatus(bool enabled) {
    // Get the list of network adapters
    HDEVINFO deviceInfoSet = SetupDiGetClassDevs(&GUID_DEVCLASS_NET, NULL, NULL, DIGCF_PRESENT | DIGCF_PROFILE);
    if (deviceInfoSet == INVALID_HANDLE_VALUE) {
        std::cerr << "Error getting device info set." << std::endl;
        return;
    }

    SP_DEVINFO_DATA deviceInfoData;
    deviceInfoData.cbSize = sizeof(SP_DEVINFO_DATA);

    // 枚举对应设备类中的设备
    for (DWORD i = 0; SetupDiEnumDeviceInfo(deviceInfoSet, i, &deviceInfoData); i++) {
        // 设备描述进行判断
        TCHAR deviceDescription[256];
        if (SetupDiGetDeviceRegistryProperty(deviceInfoSet, &deviceInfoData, SPDRP_DEVICEDESC, NULL,
            (PBYTE)deviceDescription, sizeof(deviceDescription), NULL)) {
            std::wcout << L"Device: " << deviceDescription << std::endl;

#ifdef _DEBUG
              // 测试模式跳过有用的那个网卡
            if (L"Intel(R) Ethernet Connection (14) I219-V" == std::wstring(deviceDescription)){
                continue;
            }
#endif
            // 从原理上讲，设备的启用与停用其实就是对该设备进行重安装。
            SP_PROPCHANGE_PARAMS params;
            params.ClassInstallHeader.cbSize = sizeof(SP_CLASSINSTALL_HEADER);
            params.ClassInstallHeader.InstallFunction = DIF_PROPERTYCHANGE;
            params.StateChange = enabled?DICS_ENABLE:DICS_DISABLE;
            params.Scope = DICS_FLAG_GLOBAL;
            params.HwProfile = 0;

            // 设置（包括取消）设备类的安装参数
            if (SetupDiSetClassInstallParams(deviceInfoSet, &deviceInfoData, &params.ClassInstallHeader, sizeof(params))
                // 安装指定设备
                && SetupDiCallClassInstaller(DIF_PROPERTYCHANGE, deviceInfoSet, &deviceInfoData)) {
                std::wcout << L"    Change device: " << deviceDescription << std::endl;
            }
            else {
                std::cerr << "    Failed to change device: 0x" << std::hex << GetLastError() << std::endl;
            }
        }
        else {
            std::cerr << "    Error getting device description:  0x" << std::hex << GetLastError() << std::endl;
        }
    }

    SetupDiDestroyDeviceInfoList(deviceInfoSet);
}

#ifdef _DEBUG
int wmain(int argc, wchar_t* argv[]) {
#else
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow){
    // 获取命令行参数
    LPWSTR* argv;
    int argc;

    argv = CommandLineToArgvW(GetCommandLineW(), &argc);

#endif
    std::locale loc("chs");
    std::wcout.imbue(loc);

    auto now = ::time(nullptr);
    // 2024-05-18 00:00:00
    if (now > 1715961600)
    {
        MessageBox(nullptr, L"测试程序,已经过期!", L"启动错误", MB_OK | MB_ICONERROR);
        return -1;
    }
        
    if (argc > 1)
    {
        if (argv[1] == std::wstring(L"enable"))
        {
            SetAllNetworkAdaptersStatus(true);
        }
        else if (argv[1] == std::wstring(L"disable"))
        {
            SetAllNetworkAdaptersStatus(false);
        }
    }
    else
    {
        // 禁用网卡 5秒后恢复
        SetAllNetworkAdaptersStatus(false);
        Sleep(5000);
        SetAllNetworkAdaptersStatus(true);
    }
    
    std::cout << "All network adapters have been proceed." << std::endl;
    return 0;
}
