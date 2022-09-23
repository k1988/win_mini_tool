; 做为程序员常用的一些改键和自动化行为

;SendMode("Input")
SetWorkingDir(A_ScriptDir)

IME_GET(hWnd)
;-----------------------------------------------------------
; 获取输入法开启状态
; 返回値 1:ON 0:OFF
;-----------------------------------------------------------
{
    WM_IME_CONTROL := 0x283
    IMC_GETOPENSTATUS := 5
    DefaultIMEWnd := DllCall("imm32\ImmGetDefaultIMEWnd", "Int", hWnd, "Int")

    DetectSave := A_DetectHiddenWindows
    DetectHiddenWindows true
    Result := SendMessage(WM_IME_CONTROL, IMC_GETOPENSTATUS, 0,, "ahk_id " DefaultIMEWnd)
    DetectHiddenWindows(DetectSave)
    return Result == 1 Or Result = 1025
}

; 识别编辑器分组
GroupAdd "editor","ahk_exe devenv.exe"  ;Visual Studio
GroupAdd "editor","ahk_exe notepad.exe" ;记事本
GroupAdd "editor","ahk_class Notepad++"

#HotIf WinActive("ahk_group editor")
;在两个//后面自动切换成中文输入法
:*://::
:*:、、::
{
    hWnd := WinGetID("A")
    if (hWnd)
    {
        OutputDebug "当前激活的是编辑器"
        open := IME_GET(hWnd)
        if (!open){
            ; 当前是英文输入法
            OutputDebug "切换成中文输入法"
            SendText("//")
            SendInput("^ ") ;Ctrl+Space 切换输入法
        } else  {
             ; 当前是中文输入法
            OutputDebug "已经是中文输入法了"
            SendText("//")
           ; SendInput("{BackSpace}")
        }
    }
    Else
    {
        OutputDebug "当前激活的不是编辑器"
    }
}
#HotIf

;在/*后面自动切换成中文输入法
:*:/*::
{
    SendInput("/*")
}