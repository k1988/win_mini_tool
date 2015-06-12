#include <Windows.h>
#include <cstdio>
#include <cstdlib>

#pragma comment(lib, "Advapi32.lib")

BOOL IsUserAdmin(VOID)
/*++ 
Routine Description: This routine returns TRUE if the caller's process 
is a member of the Administrators local group. Caller is NOT expected 
to be impersonating anyone and is expected to be able to open its own 
process and process token. 
Arguments: None. 
Return Value: 
   TRUE - Caller has Administrators local group. 
   FALSE - Caller does not have Administrators local group. --
*/ 
{
BOOL b;
SID_IDENTIFIER_AUTHORITY NtAuthority = SECURITY_NT_AUTHORITY;
PSID AdministratorsGroup; 
b = AllocateAndInitializeSid(
    &NtAuthority,
    2,
    SECURITY_BUILTIN_DOMAIN_RID,
    DOMAIN_ALIAS_RID_ADMINS,
    0, 0, 0, 0, 0, 0,
    &AdministratorsGroup); 
if(b) 
{
    if (!CheckTokenMembership( NULL, AdministratorsGroup, &b)) 
    {
         b = FALSE;
    } 
    FreeSid(AdministratorsGroup); 
}

return(b);
}

BOOL IsAdministrator() {//判断是否管理员模式   
   
    BOOL bIsElevated = FALSE;  
    HANDLE hToken = NULL;  
    UINT16 uWinVer = LOWORD(GetVersion());  
    uWinVer = MAKEWORD(HIBYTE(uWinVer),LOBYTE(uWinVer));  
   
    if (uWinVer < 0x0600)//不是VISTA、Windows7   
        return(FALSE);  
   
    if (OpenProcessToken(GetCurrentProcess(),TOKEN_QUERY,&hToken)) 
	{    
        struct {  
            DWORD TokenIsElevated;  
        } /*TOKEN_ELEVATION*/te;  
        DWORD dwReturnLength = 0;  
   
        if (GetTokenInformation(hToken,/*TokenElevation*/(_TOKEN_INFORMATION_CLASS)20,&te,sizeof(te),&dwReturnLength)) {  
            if (dwReturnLength == sizeof(te))  
                bIsElevated = te.TokenIsElevated;  
        }  
        CloseHandle( hToken );  
    }  
    return bIsElevated;  
}  

void main()
{
if (IsUserAdmin())
{
    printf("管理员权限");
	getchar();
}

if (IsAdministrator())
{
    printf("管理员权限2");
	getchar();
}
getchar();
}
