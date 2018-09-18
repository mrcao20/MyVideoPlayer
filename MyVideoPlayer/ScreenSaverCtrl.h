#pragma once
#include "Windows.h"
/*   //静态链接
#pragma comment(lib,"User32.lib")   //在link时，链接到User32.lib文件

extern "C"
{
WINUSERAPI BOOL WINAPI SystemParametersInfoW(__in UINT uiAction,__in UINT uiParam,__inout_opt PVOID pvParam,__in UINT fWinIni);
WINUSERAPI BOOL WINAPI SystemParametersInfoA(__in UINT uiAction,__in UINT uiParam,__inout_opt PVOID pvParam,__in UINT fWinIni);
WINUSERAPI BOOL WINAPI PostMessageA(__in_opt HWND hWnd,__in UINT Msg, __in WPARAM wParam,__in LPARAM lParam);
WINUSERAPI HDESK WINAPI OpenDesktopA(__in LPCSTR lpszDesktop,__in DWORD dwFlags,__in BOOL fInherit,__in ACCESS_MASK dwDesiredAccess);
WINUSERAPI BOOL WINAPI CloseDesktop(__in HDESK hDesktop);
WINUSERAPI BOOL WINAPI EnumDesktopWindows( __in_opt HDESK hDesktop, __in WNDENUMPROC lpfn, __in LPARAM lParam);
WINUSERAPI BOOL WINAPI IsWindowVisible(__in HWND hWnd);
WINUSERAPI HWND WINAPI GetForegroundWindow(VOID);
WINUSERAPI BOOL WINAPI EnumDesktopWindows(__in_opt HDESK hDesktop,__in WNDENUMPROC lpfn,__in LPARAM lParam);

}
*/
//BOOL CALLBACK KillScreenSaverFunc(HWND hWnd,LPARAM lParam);
class ScreenSaverCtrl
{
public:
	ScreenSaverCtrl(void);
	bool GetScreenSaverActive();
	void SetScreenSaverActive(int Active);
	int GetScreenSaverTimeout();
	void SetScreenSaverTimeout(int Value);
	bool GetScreenSaverRunning();
	void SetScreenSaverRunning(int running);
	void KillScreenSaver();
private:
	static BOOL CALLBACK KillScreenSaverFunc(HWND hWnd, LPARAM lParam);

public:
	~ScreenSaverCtrl(void);
};