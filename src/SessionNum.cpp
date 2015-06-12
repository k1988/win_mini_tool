/******************************************************************************
 *  版权所有（C）terry.zhao                                                   *
 *  保留所有权利。                                                            *
*******************************************************************************
 *  来源 : https://github.com/k1988/win_mini_tool
 *  版本 : 1.0
 *  功能说明：显示当前系统的session个数
******************************************************************************/
/*  修改记录: 
      日期       版本    修改人             修改内容
    --------------------------------------------------------------------------
******************************************************************************/

#include <windows.h>   
#include <commctrl.h>   
#include <process.h>   
#include <WtsApi32.h> 
#include <cstdio>
#include <TCHar.h>

#pragma comment(lib, "WtsApi32.lib")
   
int GetSessionNum(DWORD &SessionNum)   
{   
    DWORD i;   
    HANDLE hServer;    
   
    DWORD dwSiCount = 0;    //统计当前所有登陆系统的会话个数   
    DWORD dwSiCountbak = 0;    
    DWORD dwSize  =  0;    
    TCHAR *szLogName  =  NULL;  //记录当前登陆系统的用户名   
    PWTS_SESSION_INFO session_info = NULL;   
   
    hServer = WTSOpenServer(NULL);    
    if (hServer)    
    {    
        WTSEnumerateSessions(hServer, 0, 1, &session_info, &dwSiCount);    
   
        dwSiCountbak = dwSiCount;   
        for( i = 0; i < dwSiCountbak; i++ )    
        {     
            if ( WTSQuerySessionInformation( hServer,   
                session_info[i].SessionId,    
                WTSUserName,    
                &szLogName,    
                &dwSize) )    
            {   
                //如果用户名为空的，不统计在会话个数内   
                if ( strcmp( szLogName,"" ) == 0 )   
                {   
                    dwSiCount--;   
                }   
                printf("ID:%-8luUserName:%s\n", session_info[i].SessionId, szLogName);   
                WTSFreeMemory(szLogName);      
            }   
        }    
    }    
   
    SessionNum = dwSiCount;   
   
    WTSFreeMemory(session_info);    
    WTSCloseServer(hServer);    
    return 0;   
}   
   
int _tmain(int argc, _TCHAR* argv[])   
{   
    DWORD SessionNum;   
    GetSessionNum(SessionNum);   
    printf("SessionCount=%lu\n",SessionNum);   
    getchar();   
    return 0;   
}  