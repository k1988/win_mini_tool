import win32api, win32con,win32event,pywintypes;

try:
    a = win32event.OpenEvent(win32event.EVENT_ALL_ACCESS, 0, "KSPRODUCT_EVENT_NAME")
    print(a)
    print("begin waited event")
    b = win32event.WaitForSingleObject(a, 9999)
    print(b)
    if b == 0:
        print("waited, input any thing to exit")
    else:
        print("failed, input any thing to exit")
    a = raw_input()
except pywintypes.error as identifier:
    print "exception:"
    print("error:%d,%s" % (identifier[0],identifier[2]))
    pass
