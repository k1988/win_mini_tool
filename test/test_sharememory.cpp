#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <strsafe.h>
#include <aclapi.h>
#include <stdio.h>
#include <Sddl.h>

#pragma comment(lib, "Advapi32.lib")

# ref: https://blog.csdn.net/huanglong8/article/details/71105050
const char *sharename = "Global\\mysharedmemory";
const char *sharessdl = "D:P(A;OICI;GA;;;SY)(A;OICI;GA;;;BA)(A;OICI;GR;;;IU)";
char szMsg[] = "Message from first process";
#define SHAMEM_MAX 4 * 1024
HANDLE CreateGlobalFileMapping()
{
    SECURITY_ATTRIBUTES security;
    ZeroMemory(&security, sizeof(security));
    security.nLength = sizeof(security);
    ConvertStringSecurityDescriptorToSecurityDescriptorA(
        sharessdl,
        SDDL_REVISION_1,
        &security.lpSecurityDescriptor,
        NULL);
    HANDLE hMapFile = CreateFileMappingA(INVALID_HANDLE_VALUE, &security,
                                    PAGE_READWRITE, 0, SHAMEM_MAX, sharename);
    LocalFree(security.lpSecurityDescriptor);
    if (hMapFile)
    {
        LPCTSTR pBuf;
        pBuf = (LPTSTR)MapViewOfFile(hMapFile,            // handle to map object
                                     FILE_MAP_ALL_ACCESS, // read/write permission
                                     0,
                                     0,
                                     SHAMEM_MAX);

        if (pBuf == NULL)
        {
            printf("Could not map view of file (%d).\n",
                   GetLastError());
            return hMapFile;
        }
        CopyMemory((PVOID)pBuf, szMsg, strlen(szMsg));
        UnmapViewOfFile(pBuf);
    }
    return hMapFile;
}

HANDLE OpenGlobalFileMapping()
{
    HANDLE hMapFile = OpenFileMappingA(PAGE_READWRITE, FALSE, sharename);
    if (hMapFile)
    {
        LPCTSTR pBuf;
        pBuf = (LPTSTR)MapViewOfFile(hMapFile,            // handle to mapping object
                             PAGE_READWRITE, // read/write permission
                             0,
                             0,
                             SHAMEM_MAX);

        if (pBuf == NULL)
        {
            printf("Could not map view of file (%d).\n",
                   GetLastError());
            return hMapFile;
        }
        printf(pBuf);
        UnmapViewOfFile(pBuf);
    }
    return hMapFile;
}

void usage()
{
    printf("Command: [create|open]");
}

void PrintError()
{
    printf("Error:%d", ::GetLastError());
}

void main(int argc, char *argv[])
{
    if (argc == 1)
    {
        return usage();
    }
    if (strcmp(argv[1], "create") == 0)
    {
        HANDLE mapping = CreateGlobalFileMapping();
        if (!mapping)
        {
            return PrintError();
        }
        printf("input any key to close the filemapping");
        getchar();
        CloseHandle(mapping);
    }
    else if (strcmp(argv[1], "open") == 0)
    {
        HANDLE mapping = OpenGlobalFileMapping();
        if (!mapping)
        {
            return PrintError();
        }
        printf("input any key to close the filemapping");
        getchar();
        CloseHandle(mapping);
    }
    else
    {
        return usage();
    }
}