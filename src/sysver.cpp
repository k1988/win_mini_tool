//
#include <windows.h>
#include <cstdio>
#include <TChar.h>

typedef VOID(WINAPI *PFNGetNativeSystemInfo)(LPSYSTEM_INFO);
typedef BOOL(WINAPI *PFNGetProductInfo)(DWORD, DWORD, DWORD, DWORD, PDWORD);
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

	printf("win %d.%d.%d", osvi.dwMajorVersion, osvi.dwMinorVersion, osvi.dwBuildNumber);

	char osName[MAX_PATH] = {0};
	DWORD dwType = PRODUCT_UNDEFINED;
	PFNGetProductInfo pfnGetProductInfo = (PFNGetProductInfo)GetProcAddress(GetModuleHandle(TEXT("kernel32.dll")), "GetProductInfo");
	if (pfnGetProductInfo)
	{
		pfnGetProductInfo(osvi.dwMajorVersion, osvi.dwMinorVersion, osvi.dwMinorVersion, osvi.dwBuildNumber, &dwType);
	}

	switch (dwType)
	{
	case PRODUCT_ULTIMATE:
	{
		_tcscat(osName, TEXT("Ultimate Edition"));
	}
	break;
	case PRODUCT_HOME_PREMIUM:
	{
		_tcscat(osName, TEXT("Home Premium Edition"));
	}
	break;
	case PRODUCT_HOME_BASIC:
	{
		_tcscat(osName, TEXT("Home Basic Edition"));
	}
	break;
	case PRODUCT_ENTERPRISE:
	{
		_tcscat(osName, TEXT("Enterprise Edition"));
	}
	break;
	case PRODUCT_BUSINESS:
	{
		_tcscat(osName, TEXT("Business Edition"));
	}
	break;
	case PRODUCT_STARTER_N:
	case PRODUCT_STARTER:
	{
		_tcscat(osName, TEXT("Starter Edition"));
	}
	break;
	case PRODUCT_CLUSTER_SERVER:
	{
		_tcscat(osName, TEXT("Cluster Server Edition"));
	}
	break;
	case PRODUCT_DATACENTER_SERVER:
	{
		_tcscat(osName, TEXT("Datacenter Edition"));
	}
	break;
	case PRODUCT_DATACENTER_SERVER_CORE:
	{
		_tcscat(osName, TEXT("Datacenter Edition (core installation)"));
	}
	break;
	case PRODUCT_ENTERPRISE_SERVER:
	{
		_tcscat(osName, TEXT("Enterprise Edition"));
	}
	break;
	case PRODUCT_ENTERPRISE_SERVER_CORE:
	{
		_tcscat(osName, TEXT("Enterprise Edition (core installation)"));
	}
	break;
	case PRODUCT_ENTERPRISE_SERVER_IA64:
	{
		_tcscat(osName, TEXT("Enterprise Edition for Itanium-based Systems"));
	}
	break;
	case PRODUCT_SMALLBUSINESS_SERVER:
	{
		_tcscat(osName, TEXT("Small Business Server"));
	}
	break;
	case PRODUCT_SMALLBUSINESS_SERVER_PREMIUM:
	{
		_tcscat(osName, TEXT("Small Business Server Premium Edition"));
	}
	break;
	case PRODUCT_STANDARD_SERVER:
	{
		_tcscat(osName, TEXT("Standard Edition"));
	}
	break;
	case PRODUCT_STANDARD_SERVER_CORE:
	{
		_tcscat(osName, TEXT("Standard Edition (core installation)"));
	}
	break;
	case PRODUCT_WEB_SERVER:
	{
		_tcscat(osName, TEXT("Web Server Edition"));
	}
	break;
	case PRODUCT_PROFESSIONAL:
	case PRODUCT_PROFESSIONAL_N:
	{
		_tcscat(osName, TEXT("Professional Edition"));
	}
	break;
	default:
	{
		// win8 无系统类型
		_tcscat(osName, TEXT("Unknown"));
		TCHAR type[20] = {0};
		_itoa(dwType, type, 10);
		_tcscat(osName, type);
	}
	break;
	}

	if (si.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_AMD64)
	{
		_tcscat(osName, TEXT(", 64-bit"));
	}
	else if (si.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_INTEL)
	{
		_tcscat(osName, TEXT(", 32-bit"));
	}

	printf(osName);

	getchar();
}