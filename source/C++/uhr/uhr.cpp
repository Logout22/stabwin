#include <windows.h>

LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);
SYSTEMTIME mytime;

int power(int base,int exponent)
/*eigene Funktion, denn beim Vergleichen mit der pow-Funktion gibt es Probleme
  und die exe-Datei wird größer*/
{
	int neuint=1;
	for(int i=1;i<exponent;i++)
		neuint=neuint*base;
	return neuint;
}

char* inttoansi(int zahl,int digits)
{
	char* nr=new char[5];
	char* result=new char[5+digits];
	itoa(zahl,nr,10);
	lstrcpy(result,"");
	for(int i=1;i<=digits;i++)
		if(power(10,i)>zahl) lstrcat(result,"0");
	if(zahl>0) lstrcat(result,nr);
	delete [] nr;
	return result;
}

void adddayofweek(char* str,int day)
{
	switch(day)
	{
	case 0:
		lstrcat(str,"Sonntag");
		break;
	case 1:
		lstrcat(str,"Montag");
		break;
	case 2:
		lstrcat(str,"Dienstag");
		break;
	case 3:
		lstrcat(str,"Mittwoch");
		break;
	case 4:
		lstrcat(str,"Donnerstag");
		break;
	case 5:
		lstrcat(str,"Freitag");
		break;
	case 6:
		lstrcat(str,"Samstag");
		break;
	}
}

void Paint(HDC dc)
{
	char* nr;
	char* comp;
	comp=new char[30];
	lstrcpy(comp,"");

	adddayofweek(comp,mytime.wDayOfWeek);

	nr=inttoansi(mytime.wDay,2);
	lstrcat(comp,", ");
	lstrcat(comp,nr);
	delete [] nr;

	nr=inttoansi(mytime.wMonth,2);
	lstrcat(comp,".");
	lstrcat(comp,nr);
	delete [] nr;

	nr=inttoansi(mytime.wYear,4);
	lstrcat(comp,".");
	lstrcat(comp,nr);
	delete [] nr;

	nr=inttoansi(mytime.wHour,2);
	lstrcat(comp," ");
	lstrcat(comp,nr);
	delete [] nr;
		
	nr=inttoansi(mytime.wMinute,2);
	lstrcat(comp,":");
	lstrcat(comp,nr);
	delete [] nr;
	
	nr=inttoansi(mytime.wSecond,2);
	lstrcat(comp,":");
	lstrcat(comp,nr);
	delete [] nr;

	HGDIOBJ penAlt=SelectObject(dc,GetStockObject(WHITE_PEN));
	Rectangle(dc,0,0,220,30);
	SelectObject(dc,penAlt);

	TextOut(dc,1,5,comp,lstrlen(comp));
}

int APIENTRY WinMain(HINSTANCE hInstance,HINSTANCE,LPSTR,int)
{
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
	wc.style=CS_DBLCLKS;

	RegisterClass(&wc);

	HWND hWindow;
	hWindow=CreateWindow("WinProg","Uhr",WS_POPUP,GetSystemMetrics(SM_CXSCREEN)-220,GetSystemMetrics(SM_CYSCREEN)-75,220,30,0,0,hInstance,0);

	ShowWindow(hWindow,SW_SHOW);
	UpdateWindow(hWindow);

	SetTimer(hWindow,1001,1000,0);

	MSG msg;
	while(GetMessage(&msg,0,0,0))
	{
		DispatchMessage(&msg);
	}
	KillTimer(hWindow,1001);
	return msg.wParam;
}
LRESULT CALLBACK WndProc(HWND hWnd,UINT uiMsg,WPARAM wParam,LPARAM lParam)
{
	HDC dc;
	switch(uiMsg)
	{
	case WM_CREATE:
		GetLocalTime(&mytime);
		return 0;
	case WM_LBUTTONDBLCLK:
		ShellExecute(hWnd, "cplopen", "timedate.cpl", 0, 0, SW_NORMAL);
		return 0;
	case WM_TIMER:
		GetLocalTime(&mytime);
		dc=GetDC(hWnd);
		Paint(dc);
		ReleaseDC(hWnd,dc);
		return 0;
	case WM_PAINT:
		PAINTSTRUCT ps;
		dc=BeginPaint(hWnd,&ps);
		Paint(dc);
		EndPaint(hWnd,&ps);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	default:
		return DefWindowProc(hWnd,uiMsg,wParam,lParam);
	}
}
