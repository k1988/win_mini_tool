@echo off
:start http://wpa.qq.com/msgrd?Uin=%1%
:rem start http://wpa.qq.com/msgrd?v=3&uin=13131&site=qq&menu=yes
start tencent://message/?uin=%1%


@echo off
:start http://wpa.qq.com/msgrd?Uin=%1%
:rem start http://wpa.qq.com/msgrd?v=3&uin=13131&site=qq&menu=yes
start tencent://message/?uin=%1%

:打开qq群，可以从http://qm.qq.com/cgi-bin/qm
:tencent://groupwpa/?subcmd=all&param=7B2267726F757055696E223A3237353339363736312C2274696D655374616D70223A313438303437343432397D0A

@rem 谁建的联机群分享个群名片就行。
@rem 然后保存到电脑上，用https://cli.im/deqr分析。
@rem 再使用view-source: 方式查看源码。
@rem 如：view-source:http://qm.qq.com/cgi-bin/qm/qr?k=b8PYDva4M9gcPpCm5waS-moHPAkD4Uto
@rem 找到：tencent://groupwpa/?subcmd=all&param=7B2267726F757055696E223A3832363439313630322C2274696D655374616D70223A313533373431343537367D0A