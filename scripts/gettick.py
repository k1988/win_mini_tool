#coding=utf-8
import win32api
import datetime
while True:
    print("time:%s,tick:%s" % ( datetime.datetime.now().strftime('%Y-%m-%d %H:%M:%S'),win32api.GetTickCount()))
    win32api.Sleep(2000)