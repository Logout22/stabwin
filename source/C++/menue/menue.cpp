#include <windows.h>
#include <vector>
#include <string>

using namespace std;

vector <string>* apps;
HINSTANCE hInst;
LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);

/*
string command("");
string param("");

VOID separateCmdLine(char* cmdline)
{
	int addtocommand=0;
	for(int i=0;i<lstrlen(cmdline);i++)
	{
		if(cmdline[i]=='\"')
		{
			addtocommand++;
			if(addtocommand==2)
			{
				command=command+"\"";
				i=i+2;
			}
		}
		if(addtocommand==0 && cmdline[i]==' ')
		{
			addtocommand=2;
		}
		if(addtocommand<2)
		{
			command=command+cmdline[i];
		}
		else
		{
			param=param+cmdline[i];
		}
	}
}*/
VOID executeCmdLine(char* cmdline)
{
	STARTUPINFO startupinfo;
	PROCESS_INFORMATION processinfo;
	memset(&startupinfo,0,sizeof(startupinfo));
	startupinfo.cb=sizeof(startupinfo);
    CreateProcess(NULL,(LPTSTR)cmdline,NULL,NULL,FALSE,NORMAL_PRIORITY_CLASS,
                  NULL,NULL,&startupinfo,&processinfo);
}

int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE,
                     LPSTR,
                     int)
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
	hWindow=CreateWindow("WinProg","Menü",WS_POPUP,0,0,0,0,0,0,hInstance,0);

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
	int i;
	switch(uiMsg)
	{
	case WM_CREATE:
//		DeleteFile("log.txt");
		apps=new vector<string>;
		char name[12];
		BYTE* buf;
		buf=new BYTE[MAX_PATH+1];
		char zahl[6];
		DWORD count;
		count=0;
		DWORD size;
		size=sizeof(count);

		HKEY hKey;
		if(RegOpenKeyEx(HKEY_CURRENT_USER,"Software\\StabWin\\All",0,KEY_READ,&hKey)==ERROR_SUCCESS)
		{
			RegQueryValueEx(hKey,"ButtonCount",0,0,(BYTE*)&count,&size);
			RegCloseKey(hKey);
		}
//		count=GetPrivateProfileInt("All","ButtonCount",-5,fname);
		if(count==0)
		{
			MessageBox(hWnd,"Fehler: Die StabWin-Konfiguration existiert nicht. Starten Sie bitte StabWin Config.","Fehler",0);
			DestroyWindow(hWnd);
		}
		DWORD length;
		length=120;
		int buttonpoint;
		buttonpoint=10;
		int winlength;
		if(RegOpenKeyEx(HKEY_CURRENT_USER,"Software\\StabWin",0,KEY_READ,&hKey)==ERROR_SUCCESS)
		{
			for(i=0;i<=count;i++)
			{
				lstrcpy(name,"Button");
				itoa(i,zahl,10);
				lstrcat(name,zahl);
				HKEY subkey;
	
				if(RegOpenKeyEx(hKey,name,0,KEY_READ,&subkey)==ERROR_SUCCESS)
				{
					size=sizeof(length);
					RegQueryValueEx(subkey,"Length",0,0,(BYTE*)&length,&size);
/*					ofstream xxx("log.txt",ios::ate);
					xxx << i;
					xxx << length << "\n";*/
					size=MAX_PATH+1;
					RegQueryValueEx(subkey,"Text",0,0,buf,&size);
//					xxx << buf << "\n";
					CreateWindow("BUTTON",buf,WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON,buttonpoint,5,length,20,hWnd,(HMENU) i,hInst,0);
					buttonpoint=buttonpoint+10+length;
					winlength=buttonpoint;
//					xxx << buttonpoint;
					size=MAX_PATH+1;
					RegQueryValueEx(subkey,"App",0,0,buf,&size);
//					xxx << buf;
					apps->push_back((char*)buf);
					RegCloseKey(subkey);
				}
//				length=GetPrivateProfileInt(name,"Length",120,fname);
//				GetPrivateProfileString(name,"Text","",buf,MAX_PATH,fname);
//				GetPrivateProfileString(name,"App","s_file.exe C:\\",buf,MAX_PATH,fname);
			}
			RegCloseKey(hKey);
		}
		SetWindowPos(hWnd,0,0,GetSystemMetrics(SM_CYSCREEN)-75,winlength,32,0);
		delete [] buf;
		return 0;
	case WM_COMMAND:
		i=0;
		vector<string>::iterator it;
		for(it=apps->begin();it!=apps->end();it++)
		{
/*			ofstream xxx("log.txt",ios::ate);
			xxx << i << "\n" << (*it).c_str();*/
			if(LOWORD(wParam)==i)
			{
				char* cmdline;
				cmdline=new char[(*it).length()+1];
				lstrcpy(cmdline,(*it).c_str());
/*				xxx << cmdline;
				char* cmd;
				char* temp;
				cmd=strtok(cmdline," ");
				temp=strtok(0," ");
				string line("");
				while(temp)
				{
					line=line+" "+temp;
					temp=strtok(0," ");
				}
				separateCmdLine(cmdline);

				ShellExecute(hWnd,"open",command.c_str(),param.c_str(),"",SW_SHOW);
				command="";
				param="";*/
				executeCmdLine(cmdline);
				delete [] cmdline;
			}
			++i;
		}
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	default:
		return DefWindowProc(hWnd,uiMsg,wParam,lParam);
	}
}


