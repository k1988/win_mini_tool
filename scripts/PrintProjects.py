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
    root_depth = len(folder.split(os.path.sep))
    for root, dirs, files in os.walk(folder):
        folder_depth = len(root.split(os.path.sep))
        if folder_depth - root_depth <= level:
            #print("%s %d dirs, %d files" % (root, len(dirs), len(files)))

            if ".git" in dirs:
                # git projects
                cmd = "git --git-dir=\"%s\" remote -v" % (os.path.join(root, ".git"))
                try:
                    result = subprocess.check_output(cmd, shell=True)
                    print("git: %s\n%s" % (root, result.decode()))
                except:
                    pass
            if ".svn" in dirs:
                # git projects
                cmd = "svn info \"%s\"" % (os.path.join(root))
                try:
                    result = subprocess.check_output(cmd, shell=True)
                    print("git: %s\n%s" % (root, result.decode()))
                except:
                    pass
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
