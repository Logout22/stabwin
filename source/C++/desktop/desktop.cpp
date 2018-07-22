#include <windows.h>
#include <list>
#include <string>
#include <io.h>
#include <shlobj.h>
#include <shellapi.h>
#include <direct.h>

using namespace std;

struct deskEntry
{
	string fileName;
	int desk;
	POINT pos;
};

bool operator<(deskEntry& de1,deskEntry& de2)
{
	return (de1.fileName < de2.fileName);  //Alphabetische Sortierung
}
struct specialEntry
{
	string fileName;
	string param;
	string desc;
	string pathToIcon;
	int iconNumber;
	POINT pos;
};

list <deskEntry> entries;
list <specialEntry> specialEntries;
string deskpath[2];
char fmanpath[MAX_PATH+1];
int x=10,y=-52;//damit man bei der ersten Positionsänderung auf 10 kommt
int lasty=y;
HINSTANCE hInst;
typedef list<deskEntry>::iterator dit;
typedef list<specialEntry>::iterator sit;
BOOL oneclick;

LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);
VOID RebuildView(HWND hWnd);

void ClickProcedure(HWND hWnd,LPARAM lParam)
{
	int mousex,mousey;
	sit j;
	dit i;
	mousex=LOWORD(lParam);
	mousey=HIWORD(lParam);
	for(i=entries.begin();i!=entries.end();i++)
	{
		if(mousex >= (*i).pos.x && mousey >= (*i).pos.y && mousex <= (*i).pos.x+32 && mousey <= (*i).pos.y+32)
		{
			string fullpath(deskpath[i->desk].c_str());
			fullpath=fullpath+"\\"+(*i).fileName;
			if((GetFileAttributes(fullpath.c_str()) & FILE_ATTRIBUTE_DIRECTORY)==FILE_ATTRIBUTE_DIRECTORY)
			{
				fullpath="\""+fullpath+"\"";
				ShellExecute(hWnd,"open",fmanpath,fullpath.c_str(),"",SW_SHOWNORMAL);
			}
			else
			{
				string cmdline("url.dll,FileProtocolHandler ");
				cmdline+=(*i).fileName;
				ShellExecute(hWnd,"open","rundll32.exe",cmdline.c_str(),deskpath[i->desk].c_str(),SW_SHOWNORMAL);
			}
		}
	}
	for(j=specialEntries.begin();j!=specialEntries.end();j++)
	{
		string fullparam("\"");
		fullparam=fullparam+(*j).param+"\"";
		if(mousex >= (*j).pos.x && mousey >= (*j).pos.y && mousex <= (*j).pos.x+32 && mousey <= (*j).pos.y+32)
		{
			if((*j).desc!="Internet Explorer") ShellExecute(hWnd,"open",(*j).fileName.c_str(),fullparam.c_str(),"",SW_SHOWNORMAL);
			else ShellExecute(hWnd,"open",(*j).fileName.c_str(),NULL,"",SW_SHOWNORMAL);
		}
	}
}


int APIENTRY WinMain(HINSTANCE hInstance,HINSTANCE,LPSTR,int)
{
/*	char* buf=new char[MAX_PATH+1];
	GetCurrentDirectory(MAX_PATH,buf);
	lstrcat(buf,"\\sw_conf.ini");
	int res=GetPrivateProfileInt("All","One Click",0,buf);*/
	HKEY hKey;
	if(RegOpenKeyEx(HKEY_CURRENT_USER,"Software\\StabWin\\All",0,KEY_READ,&hKey)==ERROR_SUCCESS)
	{
		BYTE* buf=new BYTE[MAX_PATH+1];
		DWORD size=MAX_PATH;
		RegQueryValueEx(hKey,"FManPath",0,0,buf,&size);
		RegCloseKey(hKey);
		lstrcpy(fmanpath,(LPCTSTR)buf);
		delete [] buf;
	}
	DWORD res=0;
	if(RegOpenKeyEx(HKEY_CURRENT_USER,"Software\\StabWin\\Desktop",0,KEY_READ,&hKey)==ERROR_SUCCESS)
	{
		DWORD size=sizeof(res);
		RegQueryValueEx(hKey,"One Click",0,0,(BYTE*)&res,&size);
		RegCloseKey(hKey);
	}
	if(res)	oneclick=TRUE;
	else oneclick=FALSE;
//	delete [] buf;

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
	if(!oneclick) wc.style=CS_DBLCLKS;
	else wc.style=0;

	RegisterClass(&wc);

	HWND hWindow;
	hWindow=CreateWindow("WinProg","Desktop",WS_POPUP,0,0,GetSystemMetrics(SM_CXSCREEN),GetSystemMetrics(SM_CYSCREEN)-150,0,0,hInstance,0);

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
	dit i;
	sit j;
	string fullpath;
	switch(uiMsg)
	{
	case WM_CREATE:
		RebuildView(hWnd);
		return 0;
	case WM_KEYDOWN:
		if(wParam==116)
		{
			RebuildView(hWnd);
		}
		return 0;
	case WM_WINDOWPOSCHANGING:
		LPWINDOWPOS pWinpos;
		pWinpos=(LPWINDOWPOS)lParam;
		pWinpos->hwndInsertAfter=HWND_BOTTOM;
		return 0;
	case WM_PAINT:
		HICON hIcon;
		SHFILEINFO shfi;
		char path[MAX_PATH+1];
		HDC dc;
		PAINTSTRUCT ps;
		RECT r;
		dc=BeginPaint(hWnd,&ps);
		for(j=specialEntries.begin();j!=specialEntries.end();j++)
		{
			hIcon=ExtractIcon(hInst,j->pathToIcon.c_str(),j->iconNumber);
			if((hIcon!=(HICON) NULL) && (hIcon!=(HICON) 1))
			{
				DrawIcon(dc,j->pos.x,j->pos.y,hIcon);
				DestroyIcon(hIcon);
			}

			SetRect(&r,(*j).pos.x-5,(*j).pos.y+37,(*j).pos.x+190,(*j).pos.y+92);
			DrawText(dc,(*j).desc.c_str(),-1,&r,DT_LEFT);
		}
		for(i=entries.begin();i!=entries.end();i++)
		{
			lstrcpy(path,deskpath[i->desk].c_str());
			lstrcat(path,"\\");
			lstrcat(path,(*i).fileName.c_str());
			SHGetFileInfo(path,0,&shfi,sizeof(shfi),SHGFI_ICON);
			DrawIcon(dc,(*i).pos.x,(*i).pos.y,shfi.hIcon);

			SetRect(&r,(*i).pos.x-5,(*i).pos.y+37,(*i).pos.x+190,(*i).pos.y+92);
			char show[21];
			char* work;
			work=new char[(*i).fileName.length()+1];
			lstrcpy(work,(*i).fileName.c_str());
			if(lstrlen(work)>18)
			{
				for(int i=0;i<18;i++)
					show[i]=work[i];
				show[18]='.';
				show[19]='.';
				show[20]='.';
				show[21]='\0';
			}
			else
			{
				lstrcpy(show,work);
			}
			DrawText(dc,show,-1,&r,DT_LEFT);
			delete [] work;
		}
		EndPaint(hWnd,&ps);
		return 0;
	case WM_LBUTTONDBLCLK:
		if(!oneclick) ClickProcedure(hWnd,lParam);
		return 0;
	case WM_LBUTTONDOWN:
		if(oneclick) ClickProcedure(hWnd,lParam);
		return 0;
	case WM_RBUTTONDOWN:
		if(deskpath[0]!="")
		{
			fullpath="\"";
			fullpath=fullpath+deskpath[0];
			fullpath=fullpath+"\"";
			ShellExecute(hWnd,"open",fmanpath,fullpath.c_str(),"",SW_SHOWNORMAL);
		}
		if(deskpath[1]!="")
		{
			fullpath="\"";
			fullpath=fullpath+deskpath[1];
			fullpath=fullpath+"\"";
			ShellExecute(hWnd,"open",fmanpath,fullpath.c_str(),"",SW_SHOWNORMAL);
		}
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	default:
		return DefWindowProc(hWnd,uiMsg,wParam,lParam);
	}
}
VOID GetPathFromReg(specialEntry* se,string regpath)
{
	HKEY hKey;
	LONG ret=RegOpenKeyEx(HKEY_CLASSES_ROOT,regpath.c_str(),0,KEY_READ,&hKey);
	if(ret==ERROR_SUCCESS)
	{
		BYTE* data;
		DWORD size;
		RegQueryValueEx(hKey,NULL,0,0,NULL,&size);
		data=new BYTE[size+1];
		RegQueryValueEx(hKey,NULL,0,0,data,&size);
		char* buf=new char[size+1];
		lstrcpy(buf,(LPCTSTR)data);
		se->pathToIcon=strtok(buf,",");
		se->iconNumber=atoi(strtok(NULL,","));
		RegCloseKey(hKey);
		delete [] data;
		delete [] buf;
	}
}

VOID GetFilesFromFolder(int nFolder,HWND hWnd,BOOL* changePos,int f_desk)
{
	deskEntry e;
	dit i;

	int res;
	res=0;
	long sh;
	_finddata_t fd;
	char searchpath[MAX_PATH+4];
	char temp[MAX_PATH+1];
	SHGetSpecialFolderPath(hWnd,temp,nFolder,true);
	deskpath[f_desk]=temp;
	lstrcpy(searchpath,temp);
	lstrcat(searchpath,"\\*.*");
	sh=_findfirst(searchpath,&fd);

	if(sh!=-1) res=0;
	while(res==0)
	{
		if(lstrcmp(fd.name,".")!=0 && lstrcmp(fd.name,"..")!=0 && (GetFileAttributes(fd.name) & FILE_ATTRIBUTE_HIDDEN)==FILE_ATTRIBUTE_HIDDEN)
		{
			e.fileName=fd.name;
			e.desk=f_desk;
			entries.push_back(e);
			(*changePos)=TRUE;
		}
		res=_findnext(sh,&fd);
	}
	_findclose(sh);
}

VOID RebuildView(HWND hWnd)
{
	BOOL changePos=FALSE;
	x=10;
	y=-52;//damit man bei der ersten Positionsänderung auf 10 kommt
	lasty=y;
	specialEntries.clear();
	char buf[MAX_PATH+1];
	entries.clear();
	
	//Sondersymbole
	HKEY hKey,subkey;
	if(RegOpenKeyEx(HKEY_CURRENT_USER,"Software\\StabWin",0,KEY_READ,&hKey)==ERROR_SUCCESS)
	{
		DWORD res=0;
		DWORD size=sizeof(res);
		char* swdir;
		if(RegOpenKeyEx(hKey,"All",0,KEY_READ,&subkey)==ERROR_SUCCESS)
		{
			BYTE buf[MAX_PATH+1];
			lstrcpy((LPTSTR)buf,"");
			size=sizeof(buf);
			RegQueryValueEx(subkey,"StabWinDir",0,0,buf,&size);
			swdir=new char[lstrlen((LPCTSTR)buf)+1];
			lstrcpy(swdir,(LPCTSTR)buf);
			RegCloseKey(subkey);
		}
		if(RegOpenKeyEx(hKey,"Desktop",0,KEY_READ,&subkey)==ERROR_SUCCESS)
		{
			RegQueryValueEx(subkey,"Arbeitsplatz",0,0,(BYTE*)&res,&size);
			if(res)
			{
				if(lstrcmp(swdir,"")!=0)
				{
				//Positionsänderung zuerst,sonst wird ein Platz zuviel reserviert 
					y=y+62;
					if(y>(GetSystemMetrics(SM_CYSCREEN)-150))
					{
						y=10;
						x=x+200;
					}
					lasty=y;
					changePos=TRUE;
					specialEntry se;
					se.desc="Arbeitsplatz";
					se.fileName=fmanpath;
					se.param="";
					GetPathFromReg(&se,"CLSID\\{20D04FE0-3AEA-1069-A2D8-08002B30309D}\\DefaultIcon");
					se.pos.x=x;
					se.pos.y=y;
					specialEntries.push_back(se);
				}
			}
			RegQueryValueEx(subkey,"EigeneDateien",0,0,(BYTE*)&res,&size);
			if(res)
			{
				if(lstrcmp(swdir,"")!=0)
				{
					y=y+62;
					if(y>(GetSystemMetrics(SM_CYSCREEN)-150))
					{
						y=10;
						x=x+200;
					}
					lasty=y;
					changePos=TRUE;
					specialEntry se;
					se.desc="Eigene Dateien";
					se.fileName=fmanpath;
					SHGetSpecialFolderPath(hWnd,buf,CSIDL_PERSONAL,FALSE);
					se.param=buf;
					GetPathFromReg(&se,"CLSID\\{450D8FBA-AD25-11D0-98A8-0800361B1103}\\DefaultIcon");
					se.pos.x=x;
					se.pos.y=y;
					specialEntries.push_back(se);
				}
			}

			RegQueryValueEx(subkey,"IE",0,0,(BYTE*)&res,&size);
			if(res)
			{
				if(lstrcmp(swdir,"")!=0)
				{
					y=y+62;
					if(y>(GetSystemMetrics(SM_CYSCREEN)-150))
					{
						y=10;
						x=x+200;
					}
					lasty=y;
					changePos=TRUE;
					specialEntry se;
					se.desc="Internet Explorer";
					lstrcpy(buf,"");
					SHGetSpecialFolderPath(hWnd,buf,0x0026,FALSE);
					if(lstrcmp(buf,"")==0)
					{
						HKEY progKey;
						if(RegOpenKeyEx(HKEY_LOCAL_MACHINE,"Software\\Microsoft\\Windows\\CurrentVersion",0,KEY_READ,&progKey)==ERROR_SUCCESS)
						{
							BYTE* bbuf=new BYTE[MAX_PATH+1];
							DWORD size=MAX_PATH;
							RegQueryValueEx(progKey,"ProgramFilesDir",0,0,bbuf,&size);
							lstrcpy(buf,(LPCTSTR)bbuf);
							delete [] bbuf;
							RegCloseKey(progKey);
						}
					}
					se.fileName=buf;
					se.fileName=se.fileName+"\\Internet Explorer\\IExplore.exe";
					se.param="";
					se.pathToIcon=se.fileName;
					se.iconNumber=0;
					se.pos.x=x;
					se.pos.y=y;
					specialEntries.push_back(se);
				}
			}
			RegQueryValueEx(subkey,"Netzwerkumgebung",0,0,(BYTE*)&res,&size);
			if(res)
			{
				if(lstrcmp(swdir,"")!=0)
				{
					y=y+62;
					if(y>(GetSystemMetrics(SM_CYSCREEN)-150))
					{
						y=10;
						x=x+200;
					}
					lasty=y;
					changePos=TRUE;
					specialEntry se;
					se.desc="Netzwerkumgebung";
					se.fileName=fmanpath;
					se.param="Netzwerkumgebung";
					GetPathFromReg(&se,"CLSID\\{208D2C60-3AEA-1069-A2D7-08002B30309D}\\DefaultIcon");
					se.pos.x=x;
					se.pos.y=y;
					specialEntries.push_back(se);
				}
			}
			RegQueryValueEx(subkey,"Papierkorb",0,0,(BYTE*)&res,&size);
			if(res)
			{
				if(lstrcmp(swdir,"")!=0)
				{
					y=y+62;
					if(y>(GetSystemMetrics(SM_CYSCREEN)-150))
					{
						y=10;
						x=x+200;
					}
					lasty=y;
					changePos=TRUE;
					specialEntry se;
					se.desc="Papierkorb";
					se.fileName=fmanpath;
					se.param="Papierkorb";
					GetPathFromReg(&se,"CLSID\\{645FF040-5081-101B-9F08-00AA002F954E}\\DefaultIcon");
					se.pos.x=x;
					se.pos.y=y;
					specialEntries.push_back(se);
				}
			}
			RegCloseKey(subkey);
		}
		delete [] swdir;
		RegCloseKey(hKey);
	}

	//reguläre Dateien
	GetFilesFromFolder(CSIDL_COMMON_DESKTOPDIRECTORY,hWnd,&changePos,0);
	GetFilesFromFolder(CSIDL_DESKTOPDIRECTORY,hWnd,&changePos,1);

	//Sortierung
	deskEntry e;
	dit i;
	entries.sort();
	for(i=entries.begin();i!=entries.end();i++)
	{
		y=y+62;
		if(y>(GetSystemMetrics(SM_CYSCREEN)-150))
		{
			y=10;
			x=x+200;
		}
		(*i).pos.x=x;
		(*i).pos.y=y;
		if(y>lasty)
		{
			lasty=y;
		}
	}
	
	if(changePos)
	{
		SetWindowPos(hWnd,0,50,0,x+200,lasty+100,0);
	}
	InvalidateRect(hWnd,NULL,TRUE);
}