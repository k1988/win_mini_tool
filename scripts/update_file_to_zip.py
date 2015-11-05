#coding:utf8
import os,sys,io,shutil
import subprocess

# 将指定的文件，替换zipfile中找到的第一个同名文件
def update_file_to_zip(file, zipfile):
    temp = subprocess.Popen(["7z.exe","l",zipfile],stdout=subprocess.PIPE,shell=True)
    (output,errput)=temp.communicate()   
    exitcode = temp.wait()
    #print(exitcode, "output:", output,errput)
    lines = output.split("\r\n")
    step = 0
    for line in lines:
        pos = line.find("-------------------")
        if pos == 0:
            if step == 0:
                step = 1
                continue
            else:
                break
        if step == 0:
            continue
        
        parts = line.split()
        if (len(parts)) == 0:
            continue
        filepath = parts[len(parts) - 1]
        infilename = os.path.basename(file)
        findfilename = os.path.basename(filepath)
        
        print(infilename, findfilename)
        if infilename == findfilename:
            dir = os.path.dirname(filepath)
            
            # 在磁盘上创建和zip中该文件所在目录一样的层次
            print("mkdir:" + dir)
            if not os.path.exists(dir):
                os.makedirs(dir)
            shutil.copy(file, dir)
            
            # 便历子目录，zip中没有文件的不添加，zip中比磁盘中新的文件也覆盖
            temp = subprocess.Popen(["7z.exe","u", "-r", "-ur0x2",zipfile, file],stdout=subprocess.PIPE,shell=True)
            (output,errput)=temp.communicate()
            exitcode = temp.wait()
            print(output)
            break    
    pass;

update_file_to_zip("123.txt", "test.zip")
