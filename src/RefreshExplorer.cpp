#include <windows.h>

#pragma comment(lib, "User32.lib")
#pragma comment(lib, "Advapi32.lib")

void FreshExplorer()
{
	HKEY hKey; 
	struct HKEY__*RootKey; 
	char *SubKey; 
	DWORD dwType=REG_SZ;	
	DWORD dwLength=256; 
	char *ValueName; 
	LPBYTE SetContent_S; 
	RootKey=HKEY_CURRENT_USER; 
	SubKey= "Control Panel\\Desktop\\WindowMetrics"; 
	char content[256]; 
	ValueName= "Shell Icon Size"; 
	RegOpenKeyEx(RootKey,SubKey,0,KEY_READ,&hKey); 
	RegQueryValueEx(hKey,ValueName,NULL,&dwType,(unsigned char  *)content,&dwLength); 
	RegCloseKey(hKey); 
	RootKey=HKEY_CURRENT_USER; 
	SubKey= "Control Panel\\Desktop\\WindowMetrics "; 
	ValueName= "Shell Icon Size"; 
	SetContent_S=LPBYTE("24"); 
	RegOpenKeyEx(RootKey,SubKey,0,KEY_WRITE,&hKey); 
	RegSetValueEx(hKey,ValueName,NULL,REG_SZ,SetContent_S, strlen((char*)SetContent_S)); 
	RegCloseKey(hKey); 
	SendMessageTimeout(HWND_BROADCAST,WM_SETTINGCHANGE, SPI_SETNONCLIENTMETRICS,0,SMTO_ABORTIFHUNG,5000,NULL); 
	SetContent_S=LPBYTE(content); 
	RegOpenKeyEx(RootKey,SubKey,0,KEY_WRITE,&hKey); 
	RegSetValueEx(hKey,ValueName,NULL,REG_SZ,SetContent_S,strlen((char*)SetContent_S)); 
	RegCloseKey(hKey); 
	SendMessageTimeout(HWND_BROADCAST,WM_SETTINGCHANGE, SPI_SETNONCLIENTMETRICS,0,SMTO_ABORTIFHUNG,5000,NULL);
}
void main()
{
	FreshExplorer();
}