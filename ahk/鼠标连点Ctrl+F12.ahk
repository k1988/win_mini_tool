; 定义全局变量
isClicking := false  ; 控制是否在连续点击

^F12::  ; 按下 Ctrl+F12 切换状态
    isClicking := !isClicking  ; 切换点击状态
    if (isClicking) {
        SetTimer, AutoClick, 100  ; 开启定时器，每100毫秒触发一次
    } else {
        SetTimer, AutoClick, Off  ; 关闭定时器
    }
return

AutoClick:
    Click  ; 执行鼠标点击
return
