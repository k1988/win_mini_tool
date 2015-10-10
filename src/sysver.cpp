//
#include <windows.h>
#include <cstdio>

typedef VOID (WINAPI* PFNGetNativeSystemInfo)(LPSYSTEM_INFO);
typedef BOOL (WINAPI* PFNGetProductInfo)(DWORD, DWORD, DWORD, DWORD, PDWORD);
void main()
{
	OSVERSIONINFOEX osvi;
    SYSTEM_INFO si;

    ZeroMemory(&si, sizeof(SYSTEM_INFO));
    ZeroMemory(&osvi, sizeof(OSVERSIONINFOEX));

    osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);

    BOOL isOsVersionInfoEx = GetVersionEx((OSVERSIONINFO *)&osvi);

    // Call GetNativeSystemInfo if supported or GetSystemInfo otherwise.
    PFNGetNativeSystemInfo pfnGetNativeSystemInfo = (PFNGetNativeSystemInfo)GetProcAddress(GetModuleHandle(TEXT("kernel32.dll")), "GetNativeSystemInfo");
    if (pfnGetNativeSystemInfo)
	{
		pfnGetNativeSystemInfo(&si);
	}
    else
	{
		GetSystemInfo(&si);
	}
	
	printf("win %d.%d", osvi.dwMajorVersion, osvi.dwMinorVersion);
	getchar();
}