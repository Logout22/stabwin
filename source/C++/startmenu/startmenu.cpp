#include <windows.h>
#include <shellapi.h>
#include <shlobj.h>
#include <list>
#include <string>

using namespace std;

struct SMEntry
{
	string e;  //Entry file name
	string ue; //Upper entry
	string p;  //Path of Startmenu
	HMENU m;   //Submenu
	UINT id;   //Menu Entry ID
};

char SMPath[MAX_PATH+1];
list <SMEntry> smenu;
HMENU MainMenu;
HMENU lastmenu;
HWND comWnd;
UINT id=1000;

VOID FindDirs(int nFolder);
HMENU FindFiles(char* path,BOOL firstcall);
VOID ShowPopupMenu();
VOID FindEntry(UINT e_id);
VOID BuildMenu(HWND hWnd);
VOID InsertSpecialEntries();
VOID RunFileMan();
BOOL IsNotIn(string en,string up);
LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);

int APIENTRY WinMain(HINSTANCE hInstance,HINSTANCE,LPSTR,int)
{
	WNDCLASS wc;
	wc.cbClsExtra=0;
	wc.cbWndExtra=0;
	wc.hbrBackground=(HBRUSH) (COLOR_WINDOW+1);
	wc.hCursor=LoadCursor(0,IDC_ARROW);
	wc.hIcon=LoadIcon(0,IDI_WINLOGO);
	wc.hInstance=hInstance;
	wc.lpfnWndProc=WndProc;
	wc.lpszClassName="WinProg";
	wc.lpszMenuName=0;
	wc.style=0;

	RegisterClass(&wc);

	CreateWindow("WinProg","Startmenu",WS_POPUP,0,0,0,0,0,0,hInstance,0);

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
		BuildMenu(hWnd);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	default:
		return DefWindowProc(hWnd,uiMsg,wParam,lParam);
	}
}

VOID FindDirs(int nFolder)
{
	char path[MAX_PATH+1];
	lstrcpy(path,"");
	SHGetSpecialFolderPath(NULL,path,nFolder,FALSE);
	if(lstrcmp(path,"")!=0)
	{
		lstrcpy(SMPath,path);
		lstrcpy(path,"\\");
		lastmenu=MainMenu;
		MainMenu=FindFiles(path,TRUE);
	}
}

HMENU FindFiles(char* path,BOOL firstcall)
{
	HMENU menu;
	if(!firstcall)
	{
		menu=CreatePopupMenu();
	}
	else
	{
		menu=lastmenu;
	}
	HANDLE sh;
	BOOL res=TRUE;
	WIN32_FIND_DATA fd;

	char findvar[MAX_PATH+4];
	lstrcpy(findvar,SMPath);
	lstrcat(findvar,path);
	lstrcat(findvar,"*.*");
	sh=FindFirstFile(findvar,&fd);
	if(sh==INVALID_HANDLE_VALUE) res=FALSE;
	while(res)
	{
		if(lstrcmp(fd.cFileName,".")!=0 && lstrcmp(fd.cFileName,"..")!=0 && (fd.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN)!=FILE_ATTRIBUTE_HIDDEN)
		{
			SMEntry e;
			e.e=fd.cFileName;
			e.p=SMPath;
			e.ue=path;
			e.id=++id;
			
			if((fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)==FILE_ATTRIBUTE_DIRECTORY)
			{
				char newpath[MAX_PATH+1];
				lstrcpy(newpath,path);
				lstrcat(newpath,fd.cFileName);
				lstrcat(newpath,"\\");
				if(IsNotIn(e.e,e.ue))
				{
					HMENU newMenu=FindFiles(newpath,FALSE);
					AppendMenu(menu,MF_POPUP,(UINT)newMenu,fd.cFileName);
					e.m=newMenu;
					smenu.push_back(e);
				}
				else
				{
					FindFiles(newpath,TRUE);
				}
			}
			else
			{
				e.m=NULL;
				smenu.push_back(e);
				AppendMenu(menu,MF_STRING,id,fd.cFileName);
			}

		}
		res=FindNextFile(sh,&fd);
	}
	FindClose(sh);
	return menu;
}
VOID ShowPopupMenu()
{
	POINT p;
	GetCursorPos(&p);

	UINT ret_id=TrackPopupMenu(MainMenu,TPM_LEFTALIGN|TPM_BOTTOMALIGN|TPM_RETURNCMD|TPM_LEFTBUTTON,p.x,p.y,0,comWnd,0);
	if(ret_id!=0)
	{
		FindEntry(ret_id);
	}
}
VOID FindEntry(UINT e_id)
{
	list<SMEntry>::iterator i;
	for(i=smenu.begin();i!=smenu.end();i++)
	{
		if(i->id==e_id)
		{
			BOOL special=FALSE;
			if(i->e.compare("\\")==0)
			{
				special=TRUE;
				DestroyWindow(comWnd);
			}
			if(i->e.compare("\\\\")==0)
			{
				special=TRUE;
				RunFileMan();
			}
			if(!special)
			{
				string execPath("url.dll,FileProtocolHandler \"");
				execPath+=i->p;
				execPath+=i->ue;
				execPath+=i->e;
				execPath+="\"";
				ShellExecute(comWnd,"open","rundll32.exe",execPath.c_str(),i->p.c_str(),SW_SHOW);
			}
		}
	}
}
BOOL IsNotIn(string en,string up)
{
	BOOL res=TRUE;
	for(list<SMEntry>::iterator it=smenu.begin();it!=smenu.end();it++)
	{
		if(it->e.compare(en)==0 && it->ue.compare(up)==0)
		{
			res=FALSE;
			if(it->m!=NULL) lastmenu=it->m;
		}
	}
	return res;
}
VOID BuildMenu(HWND hWnd)
{
	comWnd=hWnd;
	MainMenu=CreatePopupMenu();
	FindDirs(CSIDL_STARTMENU);
	FindDirs(CSIDL_COMMON_STARTMENU);
	InsertSpecialEntries();
	ShowPopupMenu();
	DestroyWindow(hWnd);
}
VOID InsertSpecialEntries()
{
	SMEntry e;
	e.e="\\\\";
	e.id=++id;
	e.m=NULL;
	e.p="";
	AppendMenu(MainMenu,MF_STRING,id,"-- Startmenü im Dateimanager öffnen --");
	smenu.push_back(e);

	e.e="\\";
	e.id=++id;
	e.m=NULL;
	e.p="";
	AppendMenu(MainMenu,MF_STRING,id,"-- Startmenü schließen --");
	smenu.push_back(e);
}
VOID RunFileMan()
{
	HKEY hKey;
	string fmanpath;
	if(RegOpenKeyEx(HKEY_CURRENT_USER,"Software\\StabWin\\All",0,KEY_READ,&hKey)==ERROR_SUCCESS)
	{
		BYTE* data;
		DWORD size;
		RegQueryValueEx(hKey,"FManPath",0,0,NULL,&size);
		data=new BYTE[size+1];
		RegQueryValueEx(hKey,"FManPath",0,0,data,&size);
		fmanpath=(char*)data;
		RegCloseKey(hKey);
		delete [] data;
	}
	char smenupath[MAX_PATH+1];
	lstrcpy(smenupath,"");
	SHGetSpecialFolderPath(NULL,smenupath,CSIDL_COMMON_STARTMENU,FALSE);
	if(lstrcmp(smenupath,"")!=0)
	{
		string executePath("\"");
		executePath=executePath+smenupath;
		executePath=executePath+"\"";
		ShellExecute(NULL,"open",fmanpath.c_str(),executePath.c_str(),NULL,SW_SHOW);
	}
	SHGetSpecialFolderPath(NULL,smenupath,CSIDL_STARTMENU,FALSE);
	if(lstrcmp(smenupath,"")!=0)
	{
		string executePath("\"");
		executePath=executePath+smenupath;
		executePath=executePath+"\"";
		ShellExecute(NULL,"open",fmanpath.c_str(),executePath.c_str(),NULL,SW_SHOW);
	}
}