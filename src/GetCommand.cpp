/*
refer: http://wj32.org/wp/2009/01/24/howto-get-the-command-line-of-processes/
*/
#include <windows.h>
#include <stdio.h>

#pragma comment(lib,"Advapi32.lib")

#ifndef NTSTATUS
#define NTSTATUS LONG
#endif
 
typedef NTSTATUS (NTAPI *_NtQueryInformationProcess)(
    HANDLE ProcessHandle,
    DWORD ProcessInformationClass,
    PVOID ProcessInformation,
    DWORD ProcessInformationLength,
    PDWORD ReturnLength
    );
 
typedef struct _UNICODE_STRING
{
    USHORT Length;
    USHORT MaximumLength;
    PWSTR Buffer;
} UNICODE_STRING, *PUNICODE_STRING;
 
typedef struct _PROCESS_BASIC_INFORMATION
{
    LONG ExitStatus;
    PVOID PebBaseAddress;
    ULONG_PTR AffinityMask;
    LONG BasePriority;
    ULONG_PTR UniqueProcessId;
    ULONG_PTR ParentProcessId;
} PROCESS_BASIC_INFORMATION, *PPROCESS_BASIC_INFORMATION;
 
PVOID GetPebAddress(HANDLE ProcessHandle)
{
    _NtQueryInformationProcess NtQueryInformationProcess =
        (_NtQueryInformationProcess)GetProcAddress(
        GetModuleHandleA("ntdll.dll"), "NtQueryInformationProcess");
    PROCESS_BASIC_INFORMATION pbi;
 
    NtQueryInformationProcess(ProcessHandle, 0, &pbi, sizeof(pbi), NULL);
 
    return pbi.PebBaseAddress;
}
 
DWORD AdjustTokenPrivilegesForNT()
{
	DWORD res = 0;
	HANDLE hToken = NULL; 	
	// Get a token for this process.
	if (OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken) == FALSE)
	{
		return 0;
	}

	// Get the LUID for the EmptyWorkingSet privilege.
	TOKEN_PRIVILEGES tkp;
	memset(&tkp, 0, sizeof(tkp));
	if(LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &tkp.Privileges[0].Luid))
	{
		// Get the EmptyWorkingSet privilege for this process.
		bool priority = false;
		for (int i = 0; i < (int)tkp.PrivilegeCount; i++)
		{
			if (tkp.Privileges[i].Attributes & SE_PRIVILEGE_ENABLED)
			{
				//ok find it
				priority = true;
				res = 1;
				break;
			}
		}
		if (!priority)
		{
			if (tkp.PrivilegeCount == 0)
			{
				tkp.PrivilegeCount = 1;
			}			
			tkp.Privileges[0].Attributes |= SE_PRIVILEGE_ENABLED;
			if (AdjustTokenPrivileges(hToken, FALSE, &tkp, sizeof(tkp), (PTOKEN_PRIVILEGES)NULL, 0))
			{
				//ok
				res = 1;
			}
		}
	}

	//free it
	CloseHandle(hToken);

	//ok
	return res;
}

int wmain(int argc, WCHAR *argv[])
{
    int pid;
    HANDLE processHandle;
    PVOID pebAddress;
    PVOID rtlUserProcParamsAddress;
    UNICODE_STRING commandLine;
    WCHAR *commandLineContents;
 
    if (argc < 2)
    {
        printf("Usage: getprocesscommandline [pid]\n");
        return 1;
    }
 
    pid = _wtoi(argv[1]);

    if (AdjustTokenPrivilegesForNT() != 0){
         printf("AdjustTokenPrivilegesForNT failed(%d)!\n", GetLastError());
    }
 
    if ((processHandle = OpenProcess(
        PROCESS_QUERY_INFORMATION | /* required for NtQueryInformationProcess */
        PROCESS_VM_READ, /* required for ReadProcessMemory */
        FALSE, pid)) == 0)
    {
        printf("Could not open process(%d)!\n", GetLastError());
        return GetLastError();
    }
 
    pebAddress = GetPebAddress(processHandle);
 
    /* get the address of ProcessParameters */
    if (!ReadProcessMemory(processHandle, (PCHAR)pebAddress + 0x10,
        &rtlUserProcParamsAddress, sizeof(PVOID), NULL))
    {
        printf("Could not read the address of ProcessParameters!\n");
        return GetLastError();
    }
 
    /* read the CommandLine UNICODE_STRING structure */
    if (!ReadProcessMemory(processHandle, (PCHAR)rtlUserProcParamsAddress + 0x40,
        &commandLine, sizeof(commandLine), NULL))
    {
        printf("Could not read CommandLine!\n");
        return GetLastError();
    }
 
    /* allocate memory to hold the command line */
    commandLineContents = (WCHAR *)malloc(commandLine.Length);
 
    /* read the command line */
    if (!ReadProcessMemory(processHandle, commandLine.Buffer,
        commandLineContents, commandLine.Length, NULL))
    {
        printf("Could not read the command line string!\n");
        return GetLastError();
    }
 
    /* print it */
    /* the length specifier is in characters, but commandLine.Length is in bytes */
    /* a WCHAR is 2 bytes */
    printf("%.*S\n", commandLine.Length / 2, commandLineContents);
    CloseHandle(processHandle);
    free(commandLineContents);
 
    return 0;
}