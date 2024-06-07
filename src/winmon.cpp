/******************************************************************************
 *  ��Ȩ���У�C��terry.zhao                                                   *
 *  ��������Ȩ����                                                            *
*******************************************************************************
 *  ��Դ : https://github.com/k1988/win_mini_tool
 *  �汾 : 1.0
 *  ����˵�������ϵͳ��ǰ���㴰�ڣ�����ӡ���ô�����Ϣ
******************************************************************************/
/*  �޸ļ�¼: 
      ����       �汾    �޸���             �޸�����
    --------------------------------------------------------------------------
******************************************************************************/

#include <windows.h>
#include <stdio.h>
#include <TChar.h>

#pragma comment(lib, "User32.lib")

#define INFO_LEN 1024

bool IsWindowFullScreen(HWND hWnd)
{
	bool bFullscreen = false;//��ŵ�ǰ������Ƿ���ȫ���ģ�true��ʾ�ǣ�false��ʾ����

	RECT rcApp;
	RECT rcDesk;

	if ((hWnd != GetDesktopWindow ()) 
		&& (hWnd != GetShellWindow ()))//�����ǰ����ڲ������洰�ڣ�Ҳ���ǿ���̨����
	{
		GetWindowRect(hWnd, &rcApp);//��ȡ��ǰ����ڵ�����
		GetWindowRect(GetDesktopWindow(), &rcDesk);//�������洰�ھ������ȡ������Ļ������

		if (rcApp.left <= rcDesk.left && //�����ǰ����ڵ�������ȫ����ס���洰�ڣ��ͱ�ʾ��ǰ�������ȫ����
			rcApp.top <= rcDesk.top &&
			rcApp.right >= rcDesk.right &&
			rcApp.bottom >= rcDesk.bottom)
		{

			TCHAR szTemp[100];

			if (GetClassName (hWnd, szTemp, sizeof (szTemp)) > 0)//�����ȡ��ǰ����ڵ������ɹ�
			{
				bFullscreen = true;
			}
			else bFullscreen = true;//�����ȡʧ�ܣ�����Ϊ��ǰ�������ȫ������
		}
	}//�����ǰ����������洰�ڣ������ǿ���̨���ڣ���ֱ�ӷ��ز���ȫ��

	return bFullscreen;
}

void DumpWindow(HWND hwnd, char info[INFO_LEN])
{
	char title[MAX_PATH] = {0};
	char className[MAX_PATH] = {0};

	GetWindowText(hwnd, title, MAX_PATH);
	GetClassName(hwnd, className, MAX_PATH);
	
	RECT rcApp;
	GetWindowRect(hwnd, &rcApp);//��ȡ��ǰ����ڵ�����
	
	RECT rcDesk;
	GetWindowRect(GetDesktopWindow(), &rcDesk);//�������洰�ھ������ȡ������Ļ������
	
	sprintf(info, "%x \n\ttitle=%s\n\tclass=%s\n\trect: %d,%d,%d,%d\n\tfullscreen:%d\n", hwnd, title, className, rcApp.left, rcApp.top, rcApp.right, rcApp.bottom, IsWindowFullScreen(hwnd));
	
	sprintf(info, "%s\ndesktop: %d,%d,%d,%d\n", info,rcDesk.left, rcDesk.top, rcDesk.right, rcDesk.bottom);
}


void main()
{
	do 
	{
		HWND forge = ::GetForegroundWindow();

		//��ǰǰ�ô��ڵ��߳�id
		DWORD dwForeID = GetWindowThreadProcessId(forge, NULL);

		//Ŀ�괰�ڵ��߳�id
		DWORD dwDstID = GetWindowThreadProcessId(forge, NULL);

		::AttachThreadInput(dwDstID, dwForeID, TRUE);
		HWND focus = ::GetFocus();
		HWND active = ::GetActiveWindow();
		::AttachThreadInput(dwDstID, dwForeID, FALSE);

		char info[INFO_LEN] = {0};
		DumpWindow(forge, info);
		printf("forge: %s\n", info);
		
		HWND owner = ::GetParent(forge);
		if (owner)
		{
			DumpWindow(owner, info);
			printf("owner: %s\n", info);
		}
		
		POINT cursorPos;
        GetCursorPos(&cursorPos);
		HWND cursorWnd = ::WindowFromPoint(cursorPos);
		if (cursorWnd)
		{
			DumpWindow(cursorWnd, info);
			printf("window under cursor: %s\n", info);
		}

		//DumpWindow(focus, info);
		//printf("focus: %s\n", info);

		//DumpWindow(active, info);
		//printf("active: %s\n", info);

		Sleep(1000);
	} while (true);
}
