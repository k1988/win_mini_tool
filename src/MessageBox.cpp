#include <windows.h>

#pragma comment(lib, "User32.lib")

void main()
{
    MessageBox(NULL, "这是一个信息提示框","信息提示框", MB_OK | MB_ICONINFORMATION);
    MessageBox(NULL, "这是一个警告提示框!","警告提示框", MB_OK | MB_ICONWARNING);    
	MessageBox(NULL, "这是一个询问提示框？","询问提示框", MB_YESNO | MB_ICONQUESTION);
	MessageBox(NULL, "这是一个错误提示框！","错误提示框", MB_OK | MB_ICONERROR);
}