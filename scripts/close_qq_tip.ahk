#Persistent
SetTimer, KillQQPop, 1000
return

KillQQPop:
IfWinExist, 腾迅新闻
{
	;sleep 1000
	WinGetPos, Xpos, Ypos, Width, Height
    WinGetTitle, Title
    ;TrayTip,Found,%Title%
    if (StrLen(Title)!= 0 AND Title!= "QQ" AND !(Title~="@") AND !(Title~="(") AND !(Title="TXMenuWindow"))
    {
        WinClose       
        file := FileOpen("log.txt", "a")
        file.WriteLine(A_YYYY " " A_MM " " A_DD " " A_Hour ":" A_Min ":" A_Sec " —— " Title  "`n`r")
        file.Close()
        ;sleep 1000
        ;TrayTip
    }
}
IfWinExist,腾讯大苏网新闻
{
	;sleep 1000
	WinGetPos, Xpos, Ypos, Width, Height
    WinGetTitle, Title
    ;TrayTip,Found,%Title%
    if (StrLen(Title)!= 0 AND Title!= "QQ" AND !(Title~="@") AND !(Title~="(") AND !(Title="TXMenuWindow"))
    {
        WinClose       
        file := FileOpen("log.txt", "a")
        file.WriteLine(A_YYYY " " A_MM " " A_DD " " A_Hour ":" A_Min ":" A_Sec " —— " Title  "`n`r")
        file.Close()
        ;sleep 1000
        ;TrayTip
    }
}
return