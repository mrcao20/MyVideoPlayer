#include "ScreenSaverCtrl.h"

/*
BOOL CALLBACK KillScreenSaverFunc(HWND hWnd,LPARAM lParam)    //ע��ص���������Ϊ��ͨ��Ա����������Ϊ��̬��Ա����
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
	BOOL isActive = FALSE;   //һ����BOOL������bool��������ʱ����
	SystemParametersInfo(SPI_GETSCREENSAVEACTIVE, 0, &isActive, 0);
	return isActive;
}

void ScreenSaverCtrl::SetScreenSaverActive(int Active)
{
	int nullVar = 0;
	SystemParametersInfo(SPI_SETSCREENSAVEACTIVE, Active, &nullVar, SPIF_SENDWININICHANGE);
	if(Active)
		SetThreadExecutionState(ES_CONTINUOUS);			// �������ʱ
	else
		SetThreadExecutionState(ES_CONTINUOUS | ES_DISPLAY_REQUIRED | ES_SYSTEM_REQUIRED);		// ����ʱ
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

//ע��ص���������Ϊ�Ǿ�̬��Ա����,��Ϊ������������this ����̬��Ա����û��thisָ��
// ���ڻص������μ����£�http://blog.csdn.net/xylary/archive/2007/04/01/1548596.aspx
BOOL ScreenSaverCtrl::KillScreenSaverFunc(HWND hWnd, LPARAM lParam)
{
	if (IsWindowVisible(hWnd)) PostMessage(hWnd, WM_CLOSE, 0, 0);
	return true;
}

ScreenSaverCtrl::~ScreenSaverCtrl(void)
{

}