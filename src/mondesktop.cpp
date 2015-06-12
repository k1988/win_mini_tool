#include <windows.h>
#include <stdio.h>

#pragma comment(lib, "User32.lib")

#define INFO_LEN 1024

void DumpWindow(HWND hwnd, char info[INFO_LEN])
{
	char title[MAX_PATH] = {0};
	char className[MAX_PATH] = {0};

	GetWindowText(hwnd, title, MAX_PATH);
	GetClassName(hwnd, className, MAX_PATH);
	
	sprintf(info, "%x \n\ttitle=%s\n\tclass=%s\n\tparent=%x\n", hwnd, title, className, GetParent(hwnd));
}

HWND g_vShellDefViewHandle = NULL;
void main()
{
	do 
	{
		HWND vHandle = FindWindow("Progman", "Program Manager");
		HWND vShellDefViewHandle = FindWindowEx(vHandle, 0, "SHELLDLL_DefView", 0);
		HWND vFolderViewHandle = FindWindowEx(vShellDefViewHandle, 0, "SysListView32", "FolderView");

		printf("Progman: %x\n", vHandle);
		if (vShellDefViewHandle)
		{
			printf("\tSHELLDLL_DefView: %x\n", vShellDefViewHandle);
		}
		else
		{
			vShellDefViewHandle = FindWindow("SHELLDLL_DefView", 0);
			printf("SHELLDLL_DefView: %x\n", vShellDefViewHandle);
		}

		if (g_vShellDefViewHandle == NULL)
		{
			g_vShellDefViewHandle = vShellDefViewHandle;
		}

		printf("\n");
		char info[INFO_LEN]; 
		DumpWindow(GetParent(g_vShellDefViewHandle), info);
		printf("parent of SHELLDLL_DefView:%s\n", info);

		Sleep(1000);
	} while (true);
}
