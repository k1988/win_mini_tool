#!python3
'''
枚举指定目录下所有的目录，如果是git或svn目录，则记录其远程仓库地址和当前版本
'''

import os
import sys
import subprocess


def usage():
    print("Usage: PrintProjects -L n folder_path")


def print_projects(level: int, folder: str):
    dirs = []
    for item in os.listdir(folder):
        full_path = os.path.join(folder, item)

        if os.path.isdir(full_path):
            dirs.append(os.path.join(folder, item))
            if item == ".git":
                 # git projects
                cmd = "git --git-dir=\"%s\" remote -v" % (full_path)
                try:
                    result = subprocess.check_output(cmd, shell=True)
                    print("git: %s\n%s" % (folder, result.decode()))
                except Exception as e:
                    print(e)
                    pass
            elif item == ".svn":
                # svn projects
                cmd = "svn info \"%s\"" % (folder)
                try:
                    result = subprocess.check_output(cmd, shell=True)
                    print("svn: %s\n%s" % (folder, result.decode("gbk")))
                except Exception as e:
                    print(e)
                    pass
            else:
                # recursive
                if level >= 1:
                    print_projects(level - 1, full_path)
    pass


if len(sys.argv) < 4:
    usage()
    exit()
else:
    level = 1
    if (sys.argv[1] == "-L"):
        level = int(sys.argv[2], 10)
    else:
        usage()
        exit()
    path = sys.argv[3]
    if not os.path.isdir(path):
        print("指定目录不存在")
    else:
        print_projects(level, path)
        exit()
