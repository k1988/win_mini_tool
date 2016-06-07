# -*- coding: utf-8 -*-
import os
import re
import sys
import zipfile
import datetime
from subprocess import Popen, PIPE
from os.path import dirname, realpath, join

# tools path
AAPT_PATH = join(dirname(realpath(__file__)), 'aapt')

# package name
CMD_PACKAGE_INFO = AAPT_PATH + ' dump badging "%s"'
PACKAGE_INFO_REGEX = re.compile(r"package: name='(.*)' versionCode='(.*)' versionName='(.*)'", re.I)

def extract_apk_infos(file_path):
  apk_infos = {}
  # get package name and version_name & version_code
  try:
    info = Popen(CMD_PACKAGE_INFO % file_path, stdout=PIPE, shell=True).communicate()[0].replace('\n', '')
    m = PACKAGE_INFO_REGEX.search(info)
    if m:
      apk_infos['package_name'] = m.group(1)
      apk_infos['version_code'] = int(m.group(2) or 0)
      apk_infos['version_name'] = m.group(3)
  except Exception, e:
    'get package info failed, cmd: %s, error: %s' % (CMD_PACKAGE_INFO % file_path, e)
 
  return apk_infos

fout = open(join(dirname(realpath(__file__)), 'result.csv'), 'w+')
def process(path):
    if not path.endswith('.apk'):
        print "跳过非apk文件" + path
        return
    apk_infos = extract_apk_infos(path)
    if apk_infos['package_name']:
        fout.write('%s,%s\n' % (path, apk_infos['package_name']) )
    else:
        fout.write('%s,%s\n' % (path, '解析失败') )
        
def process_list(path):
    print("prcess file from list")
    listf = open(path,"r")
    if listf:
        lines = listf.readlines()
        for line in lines:
            line=line.strip('\r\n')
            if os.path.isfile(line):
                process(line)
            else:
                continue
                
                
if len(sys.argv) < 2:
    print("usage: parse_apk_packagename.py <path>")
else:
    if os.path.isdir(sys.argv[1]):
        for root,dirs,files in os.walk(sys.argv[1]):
             for f in files:
                process(os.path.join(root, f))
    else:
        if sys.argv[1].endswith("apk_list.txt"):
            # 列表式处理
            process_list(sys.argv[1])
        else:
            process(sys.argv[1])
        