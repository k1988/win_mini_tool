'''
转换数字格式，输入整数或hex数据，然后输出其它各种格式的数字
'''

import sys,os

if len(sys.argv) > 1:
    text = sys.argv[1]
    intValue = 0
    if (text[0:2:1] == '0x'):
        intValue = int(text,16)
    elif (text[len(text)-1] == 'h'):
        intValue = int(text[0:-1], 16)
    else:
        intValue = int(text)

    print("int: %d" % (intValue))
    print("hex: %s" % (hex(intValue)))
    print("oct: %s" % (oct(intValue)))
    print("bin: %s" % (bin(intValue)))
else:
    print("Usage: num.py 数字")