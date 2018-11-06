import win32api, win32con,win32event;

b = win32event.CreateEvent(None, 1, 1, "KSPRODUCT_EVENT_NAME")
print(b)
print("input any thing to reset")
a = raw_input()
win32event.ResetEvent(b)
print("reseted, input any thing to exit")
a = raw_input()