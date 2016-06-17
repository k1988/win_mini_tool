#ifndef UNICODE
#define UNICODE
#endif

#include <windows.h>
#include <stdio.h>
#include <tchar.h>

#ifndef NTSTATUS
#define NTSTATUS LONG
#endif

#define NT_SUCCESS(x) ((x) >= 0)
#define STATUS_INFO_LENGTH_MISMATCH 0xc0000004

#define SystemHandleInformation 16
#define ObjectBasicInformation 0
#define ObjectNameInformation 1
#define ObjectTypeInformation 2

typedef NTSTATUS(NTAPI *_NtQuerySystemInformation)(
	ULONG SystemInformationClass,
	PVOID SystemInformation,
	ULONG SystemInformationLength,
	PULONG ReturnLength
	);
typedef NTSTATUS(NTAPI *_NtDuplicateObject)(
	HANDLE SourceProcessHandle,
	HANDLE SourceHandle,
	HANDLE TargetProcessHandle,
	PHANDLE TargetHandle,
	ACCESS_MASK DesiredAccess,
	ULONG Attributes,
	ULONG Options
	);
typedef NTSTATUS(NTAPI *_NtQueryObject)(
	HANDLE ObjectHandle,
	ULONG ObjectInformationClass,
	PVOID ObjectInformation,
	ULONG ObjectInformationLength,
	PULONG ReturnLength
	);

typedef struct _UNICODE_STRING
{
	USHORT Length;
	USHORT MaximumLength;
	PWSTR Buffer;
} UNICODE_STRING, *PUNICODE_STRING;

typedef struct _SYSTEM_HANDLE
{
	ULONG ProcessId;
	BYTE ObjectTypeNumber;
	BYTE Flags;
	USHORT Handle;
	PVOID Object;
	ACCESS_MASK GrantedAccess;
} SYSTEM_HANDLE, *PSYSTEM_HANDLE;

typedef struct _SYSTEM_HANDLE_INFORMATION
{
	ULONG HandleCount;
	SYSTEM_HANDLE Handles[1];
} SYSTEM_HANDLE_INFORMATION, *PSYSTEM_HANDLE_INFORMATION;

typedef enum _POOL_TYPE
{
	NonPagedPool,
	PagedPool,
	NonPagedPoolMustSucceed,
	DontUseThisType,
	NonPagedPoolCacheAligned,
	PagedPoolCacheAligned,
	NonPagedPoolCacheAlignedMustS
} POOL_TYPE, *PPOOL_TYPE;

typedef struct _OBJECT_TYPE_INFORMATION
{
	UNICODE_STRING Name;
	ULONG TotalNumberOfObjects;
	ULONG TotalNumberOfHandles;
	ULONG TotalPagedPoolUsage;
	ULONG TotalNonPagedPoolUsage;
	ULONG TotalNamePoolUsage;
	ULONG TotalHandleTableUsage;
	ULONG HighWaterNumberOfObjects;
	ULONG HighWaterNumberOfHandles;
	ULONG HighWaterPagedPoolUsage;
	ULONG HighWaterNonPagedPoolUsage;
	ULONG HighWaterNamePoolUsage;
	ULONG HighWaterHandleTableUsage;
	ULONG InvalidAttributes;
	GENERIC_MAPPING GenericMapping;
	ULONG ValidAccess;
	BOOLEAN SecurityRequired;
	BOOLEAN MaintainHandleCount;
	USHORT MaintainTypeList;
	POOL_TYPE PoolType;
	ULONG PagedPoolUsage;
	ULONG NonPagedPoolUsage;
} OBJECT_TYPE_INFORMATION, *POBJECT_TYPE_INFORMATION;

PVOID GetLibraryProcAddress(PSTR LibraryName, PSTR ProcName)
{
	return GetProcAddress(GetModuleHandleA(LibraryName), ProcName);
}

typedef struct
{
	_NtQueryObject FuncPtr;
	NTSTATUS Result;
	HANDLE ObjectHandle;
	ULONG ObjectInformationClass;
	PVOID ObjectInformation;
	ULONG ObjectInformationLength;
	PULONG ReturnLength;
} QueryObjectNameInfoThreadParam, *PQueryObjectNameInfoThreadParam;

DWORD WINAPI QueryObjectNameInfoThread(LPVOID lpParam)
{
	PQueryObjectNameInfoThreadParam pInParam = (PQueryObjectNameInfoThreadParam)lpParam;
	ULONG Length = 0;
	NTSTATUS status;
	
	pInParam->Result = pInParam->FuncPtr(
		pInParam->ObjectHandle,
		pInParam->ObjectInformationClass,
		pInParam->ObjectInformation,
		pInParam->ObjectInformationLength,
		pInParam->ReturnLength
	);
	return 0;
}

NTSTATUS MyNtQueryObject(_NtQueryObject FuncPtr,
	HANDLE ObjectHandle,
	ULONG ObjectInformationClass,
	PVOID ObjectInformation,
	ULONG ObjectInformationLength,
	PULONG ReturnLength)
{
	// HANDLE hFile, PINTHREAD_PARAM pInParam
	HANDLE hThread;
	PQueryObjectNameInfoThreadParam pInParam = PQueryObjectNameInfoThreadParam(malloc(sizeof(QueryObjectNameInfoThreadParam)));
	if (pInParam)
	{
		pInParam->FuncPtr = FuncPtr;
		pInParam->ObjectHandle = ObjectHandle;
		pInParam->ObjectInformation = ObjectInformation;
		pInParam->ObjectInformationLength = ObjectInformationLength;
		pInParam->ReturnLength = ReturnLength;
		pInParam->ObjectInformationClass = ObjectInformationClass;
		pInParam->Result = -1;

		//使用线程是为了防止函数执行过程中卡死
		hThread = CreateThread(NULL, 0, QueryObjectNameInfoThread, pInParam, 0, NULL);
		if (WaitForSingleObject(hThread, 1000) == WAIT_TIMEOUT)
		{
			TerminateThread(hThread, 0);
		}
		CloseHandle(hThread);
		return pInParam->Result;
	}
	free(pInParam);
}

int wmain(int argc, WCHAR *argv[])
{
	_NtQuerySystemInformation NtQuerySystemInformation =
		(_NtQuerySystemInformation)GetLibraryProcAddress("ntdll.dll", "NtQuerySystemInformation");
	_NtDuplicateObject NtDuplicateObject =
		(_NtDuplicateObject)GetLibraryProcAddress("ntdll.dll", "NtDuplicateObject");
	_NtQueryObject NtQueryObject =
		(_NtQueryObject)GetLibraryProcAddress("ntdll.dll", "NtQueryObject");
	NTSTATUS status;
	PSYSTEM_HANDLE_INFORMATION handleInfo;
	ULONG handleInfoSize = 0x10000;
	ULONG pid = 0;
	HANDLE processHandle;
	ULONG i;

	if (argc < 2)
	{
		printf("Usage: handles filepath [pid]\n");
		return 1;
	}

	if (argc > 2)
	{
		pid = _wtoi(argv[2]);
	}

	// convert C:\Windows\System32 to \Device\HarddiskVolume1\Windows\System32
	const WCHAR* filePath = argv[1];
	if (wcslen(filePath) < 2 || filePath[1] != L':')
	{
		printf("Can't process input path which is tool short or not contain local driver!\n");
		return 1;
	}
	PWSTR pDosDriveName = new TCHAR[MAX_PATH];
	TCHAR szDrive[3] = TEXT(" :");
	szDrive[0] = filePath[0];
	DWORD uiLen = QueryDosDeviceW(szDrive, pDosDriveName, MAX_PATH);
	if (0 == uiLen)
	{
		if (ERROR_INSUFFICIENT_BUFFER != GetLastError())
		{
			printf("QueryDosDeviceW failed: %d\n", GetLastError());
			return 1;
		}

		delete[]pDosDriveName;
		pDosDriveName = new TCHAR[uiLen + 1];
		uiLen = QueryDosDevice(szDrive, pDosDriveName, uiLen + 1);
		if (0 == uiLen)
		{
			printf("QueryDosDeviceW failed: %d\n", GetLastError());
			return 1;
		}
	}
	wcscat(pDosDriveName, &filePath[2]);

	handleInfo = (PSYSTEM_HANDLE_INFORMATION)malloc(handleInfoSize);
	/* NtQuerySystemInformation won't give us the correct buffer size,
	so we guess by doubling the buffer size. */
	while ((status = NtQuerySystemInformation(
		SystemHandleInformation,
		handleInfo,
		handleInfoSize,
		NULL
	)) == STATUS_INFO_LENGTH_MISMATCH)
		handleInfo = (PSYSTEM_HANDLE_INFORMATION)realloc(handleInfo, handleInfoSize *= 2);

	/* NtQuerySystemInformation stopped giving us STATUS_INFO_LENGTH_MISMATCH. */
	if (!NT_SUCCESS(status))
	{
		printf("NtQuerySystemInformation failed!\n");
		return 1;
	}

	for (i = 0; i < handleInfo->HandleCount; i++)
	{
		SYSTEM_HANDLE handle = handleInfo->Handles[i];
		HANDLE dupHandle = NULL;
		POBJECT_TYPE_INFORMATION objectTypeInfo;
		PVOID objectNameInfo;
		UNICODE_STRING objectName;
		ULONG returnLength;

		// Jump of no file
		/*if (handle.ObjectTypeNumber != 31)
		{
			continue;
		}*/

		/* Check if this handle belongs to the PID the user specified. */
		if (pid != 0 && handle.ProcessId != pid)
			continue;

		if (!(processHandle = OpenProcess(PROCESS_DUP_HANDLE, FALSE, handle.ProcessId)))
		{
			//printf("Could not open PID %d! (Don't try to open a system process.)\n", handle.ProcessId);
			continue;
		}

		/* Duplicate the handle so we can query it. */
		DWORD re = NtDuplicateObject(
			processHandle,
			(HANDLE)handle.Handle,
			GetCurrentProcess(),
			&dupHandle,
			0,
			0,
			0
		);
		if (ERROR_SUCCESS != re)
		{
			printf("[%#x] Error!\n", handle.Handle);
			continue;
		}

		/* Query the object type. */
		objectTypeInfo = (POBJECT_TYPE_INFORMATION)malloc(0x1000);
		if (!NT_SUCCESS(NtQueryObject(
			dupHandle,
			ObjectTypeInformation,
			objectTypeInfo,
			0x1000,
			NULL
		)))
		{
			printf("[%#x] Error!\n", handle.Handle);
			CloseHandle(dupHandle);
			continue;
		}

		/* Query the object name (unless it has an access of
		0x0012019f, on which NtQueryObject could hang. */
		if (handle.GrantedAccess == 0x0012019f)
		{
			/* We have the type, so display that. */
			printf(
				"[%#x] %.*S: (did not get name)\n",
				handle.Handle,
				objectTypeInfo->Name.Length / 2,
				objectTypeInfo->Name.Buffer
			);
			free(objectTypeInfo);
			CloseHandle(dupHandle);
			continue;
		}

		objectNameInfo = malloc(0x1000);
		if (!NT_SUCCESS(MyNtQueryObject(
			NtQueryObject,
			dupHandle,
			ObjectNameInformation,
			objectNameInfo,
			0x1000,
			&returnLength
		)))
		{
			/* Reallocate the buffer and try again. */
			objectNameInfo = realloc(objectNameInfo, returnLength);
			if (!NT_SUCCESS(MyNtQueryObject(
				NtQueryObject,
				dupHandle,
				ObjectNameInformation,
				objectNameInfo,
				returnLength,
				NULL
			)))
			{
				/* We have the type name, so just display that. */
				printf(
					"[%#x] %.*S: (could not get name)\n",
					handle.Handle,
					objectTypeInfo->Name.Length / 2,
					objectTypeInfo->Name.Buffer
				);
				free(objectTypeInfo);
				free(objectNameInfo);
				CloseHandle(dupHandle);
				continue;
			}
		}

		/* Cast our buffer into an UNICODE_STRING. */
		objectName = *(PUNICODE_STRING)objectNameInfo;

		/* Print the information! */
		if (objectName.Length)
		{
			/* The object has a name. */
			printf(
				"[%#x] %.*S: %.*S\n",
				handle.Handle,
				objectTypeInfo->Name.Length / 2,
				objectTypeInfo->Name.Buffer,
				objectName.Length / 2,
				objectName.Buffer
			);

			if (wcscmp(objectName.Buffer, pDosDriveName) == 0)
			{
				printf("opend by process: %d", handle.ProcessId);
				break;
			}
		}
		else
		{
			/* Print something else. */
			printf(
				"[%#x] %.*S: (unnamed)\n",
				handle.Handle,
				objectTypeInfo->Name.Length / 2,
				objectTypeInfo->Name.Buffer
			);
		}

		free(objectTypeInfo);
		free(objectNameInfo);
		CloseHandle(dupHandle);
	}

	free(handleInfo);
	CloseHandle(processHandle);

	return 0;
}
