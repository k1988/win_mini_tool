#coding=utf-8
import sys,os
import re
import shutil

# 删除文件夹下所有测试版本文件夹
# v4.0.6.1234 倒数第二个版本号是偶数即为测试版本

test_version_folders = []
def clear_test_version(folder):
    for root, dirs, files in os.walk(folder):
        for dir in dirs:
            result = re.match("v[0-9]?\.[0-9]?\.[02468]?\.[0-9]?", dir)
            if result:
                test_version_folders.append(os.path.join(root, dir))
        #print(root,dirs,files)

    print(test_version_folders)
    print("sure to delete all these folder?(Y/N)")
    input = raw_input()
    if input == "Y" or input == "y":
        for folder in test_version_folders:
            shutil.rmtree(folder)
            pass
    pass

if len(sys.argv) < 2:
    print("usage: clear_test_version.py <clear_path>")
else:
    print("clear at %s" % sys.argv[1])
    clear_test_version(sys.argv[1])

