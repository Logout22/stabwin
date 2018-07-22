#include <windows.h>

HWND rb1,rb2,rb3,rb4,rb5,rb6,but1,but2;
int checked=1;
HINSTANCE hInst;

void GetPrivileges()
{
	HANDLE Token; 
	if (OpenProcessToken(GetCurrentProcess(), TOKEN_ALL_ACCESS, &Token) == 0) 
	{ 
		return;
	} 
	TOKEN_PRIVILEGES Privilege; 
	Privilege.PrivilegeCount            = 1; 
	Privilege.Privileges->Attributes    = SE_PRIVILEGE_ENABLED; 
	if (LookupPrivilegeValue(NULL, SE_SHUTDOWN_NAME, &Privilege.Privileges->Luid) == 0) 
	{ 
		return;
	} 
	if (AdjustTokenPrivileges(Token, FALSE, &Privilege, 0, NULL, NULL) == 0) 
	{ 
		return; 
	} 
	CloseHandle(Token); 
}

LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);

int APIENTRY WinMain(HINSTANCE hInstance,HINSTANCE,LPSTR,int)
{
	hInst=hInstance;
	WNDCLASS wc;
	wc.cbClsExtra=0;
	wc.cbWndExtra=0;
	wc.hbrBackground=(HBRUSH) GetStockObject(WHITE_BRUSH);
	wc.hCursor=LoadCursor(0,IDC_ARROW);
	wc.hIcon=LoadIcon(0,IDI_WINLOGO);
	wc.hInstance=hInstance;
	wc.lpfnWndProc=WndProc;
	wc.lpszClassName="WinProg";
	wc.lpszMenuName=0;
	wc.style=0;

	RegisterClass(&wc);

	HWND hWindow;
	hWindow=CreateWindow("WinProg","Abmelden+Herunterfahren",WS_OVERLAPPED|WS_SYSMENU|WS_MINIMIZEBOX,100,100,200,210,0,0,hInstance,0);

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
	case WM_CREATE:
		rb1=CreateWindow("BUTTON","Abmelden",WS_VISIBLE|WS_CHILD|BS_AUTORADIOBUTTON,10,10,84,20,hWnd,(HMENU) 1,hInst,0);
		rb2=CreateWindow("BUTTON","Standby-Modus",WS_VISIBLE|WS_CHILD|BS_AUTORADIOBUTTON,10,40,125,20,hWnd,(HMENU) 2,hInst,0);
		rb3=CreateWindow("BUTTON","Ruhezustand",WS_VISIBLE|WS_CHILD|BS_AUTORADIOBUTTON,10,70,110,20,hWnd,(HMENU) 3,hInst,0);
		rb4=CreateWindow("BUTTON","Herunterfahren",WS_VISIBLE|WS_CHILD|BS_AUTORADIOBUTTON,10,100,120,20,hWnd,(HMENU) 4,hInst,0);
		rb5=CreateWindow("BUTTON","Neustarten",WS_VISIBLE|WS_CHILD|BS_AUTORADIOBUTTON,10,130,95,20,hWnd,(HMENU) 5,hInst,0);
		but1=CreateWindow("BUTTON","OK",WS_VISIBLE|WS_CHILD|BS_PUSHBUTTON,10,160,70,20,hWnd,(HMENU) 6,hInst,0);
		but2=CreateWindow("BUTTON","Abbrechen",WS_VISIBLE|WS_CHILD|BS_PUSHBUTTON,100,160,80,20,hWnd,(HMENU) 7,hInst,0);
		SendMessage(rb1,BM_SETCHECK,BST_CHECKED,0);
		SetFocus(but1);
		return 0;
	case WM_COMMAND:
		if(LOWORD(wParam) < 6 && LOWORD(wParam) > 0)
		{
			checked=LOWORD(wParam);
			SetFocus(but1);
		}
		if(LOWORD(wParam)==6)
		{
			DWORD ver=GetVersion();
			if(ver < 0x80000000) GetPrivileges();
			switch(checked)
			{
			case 1:
				ExitWindowsEx(EWX_LOGOFF,0);
				DestroyWindow(hWnd);
				break;
			case 2:
				SetSystemPowerState(TRUE,FALSE);
				DestroyWindow(hWnd);
				break;
			case 3:
				SetSystemPowerState(FALSE,FALSE);
				DestroyWindow(hWnd);
				break;
			case 4:
				ExitWindowsEx(EWX_SHUTDOWN|EWX_POWEROFF,0);
				DestroyWindow(hWnd);
				break;
			case 5:
				ExitWindowsEx(EWX_REBOOT,0);
				DestroyWindow(hWnd);
				break;
			default:
				MessageBox(hWnd,"Markieren Sie bitte ihre Wahl.","Nichts ausgewählt",MB_ICONINFORMATION);
			}
		}
		if(LOWORD(wParam)==7)
		{
			DestroyWindow(hWnd);
		}
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	default:
		return DefWindowProc(hWnd,uiMsg,wParam,lParam);
	}
}
