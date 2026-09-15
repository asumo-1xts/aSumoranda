#Requires AutoHotkey v2.0

#Include ../vendor/UIA_fixed.ahk

/**
 * @function IS_IME
 * @param WinTitle 対象Windowのタイトル（省略時はアクティブウィンドウ）
 * @return 1:ON / 0:OFF
 */
IS_IME(WinTitle := "A") {
    hwnd := WinExist(WinTitle)
    if !hwnd {
        return 0
    }

    targetHwnd := hwnd ; いったん宣言せざるを得ない

    ; UIAライブラリを利用してWebView2/Chromiumベースのアプリか否か判定
    if UIA.WindowIsChromium(hwnd) {
        ; 入力を処理する内部コントロールのHWNDを取得
        targetHwnd := ControlGetHwnd("Chrome_RenderWidgetHostHWND1", hwnd)
    } else if WinActive(WinTitle) {
        ; 従来のWindowsアプリ向けフォーカス取得処理
        ; https://qiita.com/kenichiro_ayaki/items/d55005df2787da725c6f#31-ime_get-
        ptrSize := !A_PtrSize ? 4 : A_PtrSize
        cbSize := 4 + 4 + (ptrSize * 6) + 16
        stGTI := Buffer(cbSize, 0)
        NumPut("DWORD", cbSize, stGTI.Ptr, 0)
        targetHwnd := DllCall("GetGUIThreadInfo", "Uint", 0, "Uint", stGTI.Ptr)
            ? NumGet(stGTI.Ptr, 8 + ptrSize, "Uint") : hwnd
    }

    ; 取得した適切なHWNDからIMEの状態を取得
    return DllCall("SendMessage"
        , "UInt", DllCall("imm32\ImmGetDefaultIMEWnd", "Uint", targetHwnd)
        , "UInt", 0x0283  ; Message : WM_IME_CONTROL
        , "Int", 0x0005   ; wParam  : IMC_GETOPENSTATUS
        , "Int", 0)       ; lParam  : 0
}