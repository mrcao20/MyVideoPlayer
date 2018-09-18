#include "ScreenSaverCtrl.h"

/*
BOOL CALLBACK KillScreenSaverFunc(HWND hWnd,LPARAM lParam)    //注意回调函数不能为普通成员函数，但可为静态成员函数
{
if( IsWindowVisible(hWnd) ) PostMessage( hWnd, WM_CLOSE, 0, 0 );
return true;
}
*/

ScreenSaverCtrl::ScreenSaverCtrl(void)
{

}

bool ScreenSaverCtrl::GetScreenSaverActive()
{
	BOOL isActive = FALSE;   //一定是BOOL而不是bool否则运行时错误
	SystemParametersInfo(SPI_GETSCREENSAVEACTIVE, 0, &isActive, 0);
	return isActive;
}

void ScreenSaverCtrl::SetScreenSaverActive(int Active)
{
	int nullVar = 0;
	SystemParametersInfo(SPI_SETSCREENSAVEACTIVE, Active, &nullVar, SPIF_SENDWININICHANGE);
	if(Active)
		SetThreadExecutionState(ES_CONTINUOUS);			// 播放完毕时
	else
		SetThreadExecutionState(ES_CONTINUOUS | ES_DISPLAY_REQUIRED | ES_SYSTEM_REQUIRED);		// 播放时
}

int ScreenSaverCtrl::GetScreenSaverTimeout()
{
	int value = 0;
	SystemParametersInfo(SPI_GETSCREENSAVETIMEOUT, 0, &value, 0);
	return value;
}

void ScreenSaverCtrl::SetScreenSaverTimeout(int Value)
{
	int nullVar = 0;
	SystemParametersInfo(SPI_SETSCREENSAVETIMEOUT, Value, &nullVar, SPIF_SENDWININICHANGE);
}

bool ScreenSaverCtrl::GetScreenSaverRunning()
{
	BOOL isRunning = FALSE;
	SystemParametersInfo(SPI_GETSCREENSAVERRUNNING, 0, &isRunning, 0);
	return isRunning;
}

void ScreenSaverCtrl::SetScreenSaverRunning(int running) {
	SystemParametersInfo(SPI_SETSCREENSAVERRUNNING, running, NULL, 0);
}

void ScreenSaverCtrl::KillScreenSaver()
{
	HDESK hDesktop = OpenDesktop(TEXT("Screen-saver"), 0, false, DESKTOP_READOBJECTS | DESKTOP_WRITEOBJECTS);
	if (hDesktop != NULL)
	{
		EnumDesktopWindows(hDesktop, KillScreenSaverFunc, 0);
		CloseDesktop(hDesktop);
	}
	else
	{
		PostMessage(GetForegroundWindow(), WM_CLOSE, 0, 0);
	}
}

//注意回调函数不能为非静态成员函数,因为其有隐含参数this 而静态成员函数没有this指针
// 关于回调函数参见文章：http://blog.csdn.net/xylary/archive/2007/04/01/1548596.aspx
BOOL ScreenSaverCtrl::KillScreenSaverFunc(HWND hWnd, LPARAM lParam)
{
	if (IsWindowVisible(hWnd)) PostMessage(hWnd, WM_CLOSE, 0, 0);
	return true;
}

ScreenSaverCtrl::~ScreenSaverCtrl(void)
{

}