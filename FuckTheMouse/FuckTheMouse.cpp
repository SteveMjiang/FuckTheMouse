// FuckTheMouse.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "FuckTheMouse.h"
#include "ClockR.h"

#define WM_NOTIFYICON (WM_USER + 1)

HINSTANCE hinst;
LRESULT CALLBACK MainWndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK MouseProc(int nCode, WPARAM wParam, LPARAM lPram);
void toTray(HWND hwnd, int nCmdShow);
ClockR cr;
HHOOK g_hMouse;
NOTIFYICONDATA nc;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	WNDCLASSEX wcx;
	HWND hwnd;
	MSG msg;
	BOOL fGotMessage;
	hinst = hInstance;
	cr.click([] {MessageBox(NULL, L"clicked", L"cr", MB_OK); });


	wcx.cbSize = sizeof(wcx);
	wcx.style = CS_HREDRAW | CS_VREDRAW;
	wcx.lpfnWndProc = MainWndProc;
	wcx.cbClsExtra = 0;
	wcx.cbWndExtra = 0;
	wcx.hInstance = hInstance;
	wcx.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wcx.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcx.hbrBackground = (HBRUSH)(GetStockObject(WHITE_BRUSH));
	wcx.lpszMenuName = NULL;
	wcx.lpszClassName = L"MainWClass";
	wcx.hIconSm = (HICON)LoadImage(hInstance, MAKEINTRESOURCE(5), IMAGE_ICON, GetSystemMetrics(SM_CXSMICON), GetSystemMetrics(SM_CYSMICON), LR_DEFAULTCOLOR);

	if (!RegisterClassEx(&wcx))
		return 1;

	//g_hMouse = SetWindowsHookEx(WH_MOUSE, MouseProc, NULL, GetCurrentThreadId());
	g_hMouse = SetWindowsHookEx(WH_MOUSE_LL, MouseProc, GetModuleHandle(L"GHookDll"), 0);

	hwnd = CreateWindow(L"MainWClass", L"FTM", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 220, 100, (HWND)NULL, (HMENU)NULL, hInstance, (LPVOID)NULL);
	if (!hwnd)
		return 1;
	//ShowWindow(hwnd, nCmdShow);
	ShowWindow(SW_HIDE, nCmdShow);
	toTray(hwnd, nCmdShow);
	UpdateWindow(hwnd);
	//cr.click([] {MessageBox(NULL, L"clicked", L"cr", MB_OK); });
	//MessageBox(NULL, L"clicked", L"cr", MB_OK);
	while ((fGotMessage = GetMessage(&msg, (HWND)NULL, 0, 0)) != 0 && fGotMessage != -1)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return msg.wParam;
}

LRESULT CALLBACK MainWndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_NOTIFYICON:
		if (lParam != WM_LBUTTONDBLCLK)
			break;
			//MessageBox(NULL, L"fuck", L"r", MB_OK);
		//if ((wParam == IDI_SMALL) && (lParam == WM_RBUTTONUP))
		//	MessageBox(NULL, L"fuck", L"r", MB_OK);
		//break;
	case WM_DESTROY:
		Shell_NotifyIcon(NIM_DELETE, &nc);
		UnhookWindowsHookEx(g_hMouse);
		ExitThread(0);
		return 0;
	default:
		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}
}


LRESULT CALLBACK MouseProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	if (nCode < 0)
		return CallNextHookEx(g_hMouse, nCode, wParam, lParam);
	if (wParam == WM_LBUTTONUP)
	{
		cr.update();
	}
	if (wParam == WM_LBUTTONDOWN && cr.click([] {}))
		return 1;

	//if (cr.click([] {/*MessageBox(NULL,L"fuck",L"shit",MB_OK);*/}))
	//{
	//	return 1;
	//}
	return CallNextHookEx(g_hMouse, nCode, wParam, lParam);
}

void toTray(HWND hwnd, int nCmdShow)
{
	//NOTIFYICONDATA nc;
	nc.cbSize = (DWORD)sizeof(NOTIFYICONDATA);
	nc.hWnd = hwnd;
	nc.uID = IDR_MAINFRAME;
	nc.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
	nc.uCallbackMessage = WM_NOTIFYICON;
	//nc.hIcon = LoadIcon(hinst, MAKEINTRESOURCE(5));
	nc.hIcon = LoadIcon(hinst, MAKEINTRESOURCE(IDI_SMALL));
	wcscpy_s(nc.szTip, L"Fuck The Mouse");
	Shell_NotifyIcon(NIM_ADD, &nc);
	ShowWindow(SW_HIDE, nCmdShow);
}





//#define MYWM_NOTIFYICON(WM_APP + 100) 
//
//void ShowTrayIcon()
//{
//	NOTIFYICONDATA  ncd;
//
//	char * szToolTip=L"TrayIcon Demo";
//	UINT uID＝1;//自己定义的标识值，随便什么值  
//      
//	ncd.cbSize = sizeof(NOTIFYICONDATA);
//	ncd.hWnd   =hwnd; //窗口的句柄
//	ncd.uID    = ID;      //
//	ncd.hIcon  = icon;    //图标的句柄
//	ncd.uFlags = NIF_MESSAGE | NIF_ICON | NIF_TIP;
//	ncd.uCallbackMessage = MYWM_NOTIFYICON;  //你可以在窗口消息中处理这个消息
//	_tcscpy(ncd.szTip, szToolTip);
//
//	Shell_NotifyIcon(NIM_ADD, &ncd);
//}






