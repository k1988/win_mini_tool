#coding:utf8
import sys,os,urllib2,re

def get_http_size(url):
    opener = urllib2.build_opener()    
    try:
        request = urllib2.Request(url)
        request.add_header('range',"bytes=0-1")
        request.get_method = lambda: 'HEAD'
        response = opener.open(request)
        response.read()
    except Exception, e: # 远程文件不存在
        print(e)
    else:
        bytes = dict(response.headers).get('content-range', '')
        fileSize = 0
        if bytes != '':
            sg = re.search(r'bytes 0-1/(\d+)',bytes)
            if sg:
              fileSize = sg.group(1)
        if fileSize == 0:            
            fileSize = dict(response.headers).get('content-length', 0)
        print fileSize

if len(sys.argv) < 2:
    print("usage: getsize.py url")
else:
    get_http_size(sys.argv[1])