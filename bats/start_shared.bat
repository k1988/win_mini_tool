@rem 开启windows访问共享所需要的服务
@rem 参考：https://zhidao.baidu.com/question/2137898808879587428.html

@rem 需要用管理员权限

sc config lmhosts start= auto
sc config SSDPSRV start= auto
sc config LanmanServer start= auto
sc config NlaSvc start= auto
sc config Netman start= auto
sc config Dnscache start= auto
sc config Browser start= auto
sc config upnphost start= auto

sc start lmhosts
sc start SSDPSRV
sc start LanmanServer
sc start NlaSvc
sc start Netman
sc start Dnscache
sc start Browser
sc start upnphost



