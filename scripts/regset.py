#coding=gbk
# 使用指定路径添加路径表项
import winreg
winreg.SetValueEx(winreg.CreateKey(winreg.HKEY_LOCAL_MACHINE,r"Software\xxx"),'First', None, winreg.REG_DWORD, 1)