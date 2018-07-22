#include <windows.h>
#include "resource.h"

LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);

int APIENTRY WinMain(HINSTANCE hInstance,HINSTANCE,LPSTR,int)
{
	WNDCLASS wc;
	wc.cbClsExtra=0;
	wc.cbWndExtra=0;
	wc.hbrBackground=(HBRUSH) GetStockObject(WHITE_BRUSH);
	wc.hCursor=LoadCursor(0,IDC_ARROW);
	wc.hIcon=LoadIcon(hInstance,MAKEINTRESOURCE(IDI_ICON1));
	wc.hInstance=hInstance;
	wc.lpfnWndProc=WndProc;
	wc.lpszClassName="WinProg";
	wc.lpszMenuName=0;
	wc.style=0;

	RegisterClass(&wc);

	HWND hWindow;
	hWindow=CreateWindow("WinProg","StabWin Info",WS_OVERLAPPED|WS_SYSMENU,100,100,320,215,0,0,hInstance,0);

	CreateWindow("BUTTON","Schliessen",WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON,100,160,90,20,hWindow,0,hInstance,0);

	ShowWindow(hWindow,SW_SHOW);
	UpdateWindow(hWindow);

	MSG msg;
	while(GetMessage(&msg,0,0,0))
	{
		DispatchMessage(&msg);
	}
	return msg.wParam;
}
LRESULT CALLBACK WndProc(HWND hWnd,UINT uiMsg,WPARAM wParam,LPARAM lParam)
{
	switch(uiMsg)
	{
	case WM_PAINT:
		PAINTSTRUCT ps;
		HDC dc;
		dc=BeginPaint(hWnd,&ps);
		
		RECT r;
		SetRect(&r,70,20,170,56);
		FillRect(dc,&r,0);

		SelectObject(dc,CreateFont(36,0,0,0,0,0,0,0,ANSI_CHARSET,0,0,0,0,"Arial"));
		SetTextColor(dc,RGB(255,0,0));
		TextOut(dc,70,20,"STAB",lstrlen("STAB"));
		SelectObject(dc,CreateFont(16,0,0,0,0,0,0,0,ANSI_CHARSET,0,0,0,0,"Arial"));
		TextOut(dc,60,70,"Get stable.",lstrlen("Get stable."));

		MoveToEx(dc,153,20,0);
		LineTo(dc,153,56);

		SelectObject(dc,CreateFont(36,0,0,0,0,0,0,0,ANSI_CHARSET,0,0,0,0,"Arial"));
		SetTextColor(dc,RGB(0,0,0));
		TextOut(dc,156,20,"WIN",lstrlen("WIN"));
		SelectObject(dc,CreateFont(16,0,0,0,0,0,0,0,ANSI_CHARSET,0,0,0,0,"Arial"));
		TextOut(dc,130,70,"Stay in Windows.",lstrlen("Stay in Windows."));
		TextOut(dc,100,110,"Version 1.4",lstrlen("Version 1.4"));
		TextOut(dc,20,130,"(c) 2005 by Martin Unzner, http://stabwin.de.vu",lstrlen("(c) 2005 by Martin Unzner, http://stabwin.de.vu"));

		EndPaint(hWnd,&ps);
		return 0;
	case WM_COMMAND:
		if(HIWORD(wParam)==BN_CLICKED)
			DestroyWindow(hWnd);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	default:
		return DefWindowProc(hWnd,uiMsg,wParam,lParam);
	}
}
