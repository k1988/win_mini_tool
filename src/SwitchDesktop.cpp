/******************************************************************************
 *  版权所有（C）terry.zhao                                                   *
 *  保留所有权利。                                                            *
*******************************************************************************
 *  来源 : https://github.com/k1988/win_mini_tool
 *  版本 : 1.0
 *  功能说明：在不同的系统桌面之前切换，但如果该桌面未创建则切换失败
******************************************************************************/
/*  修改记录: 
      日期       版本    修改人             修改内容
    --------------------------------------------------------------------------
******************************************************************************/

#include <windows.h>
#include <stdio.h>

#pragma comment(lib, "User32.lib")

int main(int argc, char* argv[])
{
  if (argc < 2)
  {
     printf("Usage:SwitchDesktop <Desktop Name>\n");
  }
  
  HDESK desk = OpenDesktop(argv[1], 0, FALSE, DESKTOP_SWITCHDESKTOP);
  if (desk == NULL)
  {
    printf("切换桌面失败：%d\n",  GetLastError());
    getchar();
    return 1;
  }
  
  if (SwitchDesktop(desk))
  {
    printf("切换桌面成功");    
  } 
  else
  {
    printf("切换桌面失败：%d\n",  GetLastError());  
  }
    getchar();
}