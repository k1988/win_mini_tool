#define _WIN32_WINNT 0x0500
#define WINVER 0x0500

#include <windows.h>
#include <stdio.h>
#include <conio.h>
#include <winioctl.h>
#include <TChar.h>

#define IOCTL_STORAGE_QUERY_PROPERTY   CTL_CODE(IOCTL_STORAGE_BASE, 0x0500, METHOD_BUFFERED, FILE_ANY_ACCESS)

typedef enum _STORAGE_PROPERTY_ID {
	StorageDeviceProperty = 0,
	StorageAdapterProperty,
	StorageDeviceIdProperty
} STORAGE_PROPERTY_ID, *PSTORAGE_PROPERTY_ID;

typedef enum _STORAGE_QUERY_TYPE {
	PropertyStandardQuery = 0, 
	PropertyExistsQuery, 
	PropertyMaskQuery, 
	PropertyQueryMaxDefined 
} STORAGE_QUERY_TYPE, *PSTORAGE_QUERY_TYPE;


typedef struct _STORAGE_PROPERTY_QUERY {
	STORAGE_PROPERTY_ID PropertyId;
	STORAGE_QUERY_TYPE QueryType;
	UCHAR AdditionalParameters[1];
} STORAGE_PROPERTY_QUERY, *PSTORAGE_PROPERTY_QUERY;

typedef struct _STORAGE_DEVICE_DESCRIPTOR {
	ULONG Version;
	ULONG Size;
	UCHAR DeviceType;
	UCHAR DeviceTypeModifier;
	BOOLEAN RemovableMedia;
	BOOLEAN CommandQueueing;
	ULONG VendorIdOffset;
	ULONG ProductIdOffset;
	ULONG ProductRevisionOffset;
	ULONG SerialNumberOffset;
	STORAGE_BUS_TYPE BusType;
	ULONG RawPropertiesLength;
	UCHAR RawDeviceProperties[1];
} STORAGE_DEVICE_DESCRIPTOR, *PSTORAGE_DEVICE_DESCRIPTOR;

static CHAR BufType[0x7f][128] = {"未知类型","SCSI","Atapi","Ata","1394","Ssa","Fibre","Usb","RAID","iSCSI","SATA","SAS","",};

//typedef BOOL (WINAPI* osGetVolumePathNamesForVolumeName)(LPCTSTR, LPTSTR, DWORD, PDWORD);
//
//osGetVolumePathNamesForVolumeName GetFunction_GetVolumePathNamesForVolumeName(void)
//{
//	TCHAR	tcPath[MAX_PATH] = {0};
//	osGetVolumePathNamesForVolumeName	pFunc = NULL;
//	HMODULE	hKer32 = GetModuleHandle(_T("Kernel32.dll"));
//	if( hKer32 == NULL )
//	{
//		if( ::GetSystemDirectory(tcPath, sizeof(tcPath) / sizeof(tcPath[0])) )
//		{
//			_tcscat(tcPath, _T("\\Kernel32.dll"));
//			hKer32 = LoadLibrary(tcPath);
//		}
//	}
//
//	if( hKer32 )
//	{
//#ifdef _UNICODE
//		pFunc = (osGetVolumePathNamesForVolumeName)GetProcAddress(hKer32, "GetVolumePathNamesForVolumeNameW");
//#else
//		pFunc = (osGetVolumePathNamesForVolumeName)GetProcAddress(hKer32, "GetVolumePathNamesForVolumeNameA");
//#endif
//	}
//
//	return pFunc;
//}

void DisplayVolumeBusType(TCHAR* szPath)
{
	STORAGE_PROPERTY_QUERY StoragePropertyQuery;
	STORAGE_DEVICE_DESCRIPTOR StDescript = { 0 };
	StoragePropertyQuery.PropertyId = StorageDeviceProperty;
	StoragePropertyQuery.QueryType = PropertyStandardQuery;
	DWORD NumberOfBytesReturned;
	HANDLE Handle;
    
    TCHAR Name[MAX_PATH]=_T("");
	Name[0]=_T('\\');
	Name[1]=_T('\\');
	Name[2]=_T('.');
	Name[3]=_T('\\');
	Name[4]=_T('\0');
	
	_tcscpy(&Name[4],szPath);
	Name[6]=_T('\0');
	Handle = ::CreateFile(Name,           
		GENERIC_READ,                          
		FILE_SHARE_READ | FILE_SHARE_WRITE,    
		NULL,                                  
		OPEN_EXISTING,                         
		0,                                     
		NULL);

	if (Handle == INVALID_HANDLE_VALUE)
	{
		printf("CreateFileW File,Error Code=%d\n", GetLastError());
		return;
	}
    
	BOOL Status = DeviceIoControl( Handle,
		IOCTL_STORAGE_QUERY_PROPERTY,
		&StoragePropertyQuery,
		sizeof(STORAGE_PROPERTY_QUERY),
		&StDescript,
		sizeof(STORAGE_DEVICE_DESCRIPTOR),
		&NumberOfBytesReturned,
		NULL );
	if (Status)
	{
		printf("\n磁盘类型:%s类型<%s>\n", BufType[StDescript.BusType],StDescript.RemovableMedia ? "移动磁盘" : "固定磁盘");
		//printf("BusType %X\n",(BYTE)StDescript.BusType);
	}
    
  
    
	CloseHandle(Handle);

}

void DisplayVolumePaths(
						TCHAR* VolumeName
						)
{
	DWORD  CharCount = MAX_PATH + 1;
	TCHAR* Names     = NULL;
	TCHAR* NameIdx   = NULL;
	BOOL   Success   = FALSE;

	//osGetVolumePathNamesForVolumeName funcGetVolume = GetFunction_GetVolumePathNamesForVolumeName();
	//if( funcGetVolume == NULL ){
	//	printf("GetFunction_GetVolumePathNamesForVolumeName Fail, Error Code=%d\n", GetLastError());
	//	return;
	//}

	for (;;) 
	{
		Names = (TCHAR*) new BYTE [CharCount * sizeof(TCHAR)];

		if ( !Names ) 
		{
			return;
		}
		
		Success = GetVolumePathNamesForVolumeName(
			VolumeName, Names, CharCount, &CharCount
			);

		if ( Success ) 
		{
			break;
		}

		if ( GetLastError() != ERROR_MORE_DATA ) 
		{
			break;
		}

		delete [] Names;
		Names = NULL;
	}

	if ( Success )
	{
		for ( NameIdx = Names; 
			NameIdx[0] != L'\0'; 
			NameIdx += _tcslen(NameIdx) + 1 ) 
		{
			_tprintf(_T("  %s\n"), NameIdx);
			DisplayVolumeBusType(NameIdx);
		}
		_tprintf(_T("\n"));
	}

	if ( Names != NULL ) 
	{
		delete [] Names;
		Names = NULL;
	}

	return;
}


int _tmain(int argc, _TCHAR* argv[])
{
	DWORD  CharCount            = 0;
	TCHAR  DeviceName[MAX_PATH] = _T("");
	DWORD  Error                = ERROR_SUCCESS;
	HANDLE FindHandle           = INVALID_HANDLE_VALUE;
	BOOL   Found                = FALSE;
	size_t Index                = 0;
	BOOL   Success              = FALSE;
	TCHAR  VolumeName[MAX_PATH] = _T("");

	FindHandle = FindFirstVolume(VolumeName,MAX_PATH);

	if (FindHandle == INVALID_HANDLE_VALUE)
	{
		Error = GetLastError();
		_tprintf(_T("FindFirstVolumeW failed with error code %d\n"), Error);
		_getch();
		return -1;
	}

	for (;;)
	{
		Index = _tcslen(VolumeName) - 1;

		if (VolumeName[0]     != _T('\\') ||
			VolumeName[1]     != _T('\\') ||
			VolumeName[2]     != _T('?')  ||
			VolumeName[3]     != _T('\\') ||
			VolumeName[Index] != _T('\\')) 
		{
			Error = ERROR_BAD_PATHNAME;
			_tprintf(_T("FindFirstVolume/FindNextVolume returned a bad path: %s\n"), VolumeName);
			break;
		}
		VolumeName[Index] = _T('\0');

		CharCount = QueryDosDevice(&VolumeName[4], DeviceName,MAX_PATH-4); 

		VolumeName[Index] = _T('\\');

		if ( CharCount == 0 ) 
		{
			Error = GetLastError();
			_tprintf(_T("QueryDosDevice failed with error code %d\n"), Error);
			break;
		}

		_tprintf(_T("\nFound a device:\n %s"), DeviceName);
		_tprintf(_T("\nVolume name: %s"), VolumeName);
		_tprintf(_T("\nPaths:"));
		DisplayVolumePaths(VolumeName);
        _tprintf(_T("\n"));

		Success = FindNextVolume(FindHandle, VolumeName, MAX_PATH);

		if ( !Success ) 
		{
			Error = GetLastError();

			if (Error != ERROR_NO_MORE_FILES) 
			{
				_tprintf(_T("FindNextVolume failed with error code %d\n"), Error);
				break;
			}

			Error = ERROR_SUCCESS;
			break;
		}
	}

	FindVolumeClose(FindHandle);
	FindHandle = INVALID_HANDLE_VALUE;
	_getch();
	return 0;
}
