#define UNICODE
#include <windows.h>
#include <WinTrust.h>
#include <SoftPub.h>
#include <tchar.h>
#include <stdio.h>

#pragma comment(lib, "WinTrust.lib")

int main()
{
	TCHAR szExe[MAX_PATH];
	//_tprintf(_T("Please Type Exe File Path:/n"));
	//_tscanf_s(_T("%ws"), szExe, MAX_PATH);
	::GetModuleFileName(NULL, (LPTSTR)szExe, MAX_PATH);
	_tprintf(_T("Verifying %ws\n"), szExe);

	GUID gd = WINTRUST_ACTION_GENERIC_VERIFY_V2;

	WINTRUST_FILE_INFO FileData;
	memset(&FileData, 0, sizeof(FileData));
	FileData.cbStruct = sizeof(WINTRUST_FILE_INFO);
	FileData.pcwszFilePath = (LPCTSTR)szExe;
	FileData.hFile = NULL;
	FileData.pgKnownSubject = NULL;

	WINTRUST_DATA WinTrustData;
	memset(&WinTrustData, 0, sizeof(WINTRUST_DATA));
	WinTrustData.cbStruct = sizeof(WINTRUST_DATA);
	WinTrustData.pPolicyCallbackData = NULL;
	WinTrustData.pSIPClientData = NULL;
	WinTrustData.dwUIChoice = WTD_UI_NONE;
	WinTrustData.fdwRevocationChecks = WTD_REVOKE_NONE;
	WinTrustData.dwUnionChoice = WTD_CHOICE_FILE;
	WinTrustData.dwStateAction = 0;
	WinTrustData.hWVTStateData = NULL;
	WinTrustData.pwszURLReference = NULL;
	WinTrustData.dwProvFlags = WTD_SAFER_FLAG;
	WinTrustData.dwUIContext = 0;
	WinTrustData.pFile = &FileData;

	LONG lRst = WinVerifyTrust(NULL, &gd, &WinTrustData);
	if (lRst == ERROR_SUCCESS)
	{
		_tprintf(_T("Verified Success!\n"));
	}
	else
	{
		_tprintf(_T("Verified Fail!Error Number:0x%08X\n"), lRst);
	}
}