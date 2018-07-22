#include <windows.h>
#include <shellapi.h>

VOID SetShell()
{
	if(GetVersion<0x80000000)
	{
		HKEY hKey;
		if(RegOpenKeyEx(HKEY_LOCAL_MACHINE,"Software\\Microsoft\\Windows NT\\CurrentVersion\\Winlogon",0,KEY_WRITE,&hKey)==ERROR_SUCCESS)
		{
			BYTE* name=new BYTE[14];
			lstrcpy((LPTSTR)name,"Explorer.exe");
			DWORD size=lstrlen((LPCTSTR)name)+1;
			RegSetValueEx(hKey,"Shell",0,REG_SZ,name,size);
			RegCloseKey(hKey);
		}
	}
	else
	{
		WritePrivateProfileString("boot","shell","Explorer.exe","system.ini");
	}
}

int APIENTRY WinMain(HINSTANCE,HINSTANCE,LPSTR,int)
{
	if(GetSystemMetrics(SM_CLEANBOOT)>0)
	{
		SetShell();
		ShellExecute(0,"open","Explorer.exe",NULL,NULL,SW_SHOW);
		exit(0);
	}
	DWORD uhr=0,tray=0,autostart=0,menue=0,desktop=0;
	char dir[MAX_PATH+1];
	char fname[MAX_PATH+1];
	GetPrivateProfileString("boot","shell","",dir,MAX_PATH,"system.ini");
	int i,pos=0;
	for(i=lstrlen(dir)-1;i>=0;i--)
		if(dir[i]=='\\')
		{
			pos=i-1;
			break;
		}
	lstrcpy(fname,"");
	for(i=0;i<=pos;i++)
	{
		fname[i]=dir[i];
		fname[i+1]='\0';
	}
	lstrcpy(dir,fname);
	SetCurrentDirectory(dir);
	lstrcpy(fname,dir);
	lstrcat(fname,"\\sw_conf.ini");
	HKEY hKey;
	if(RegOpenKeyEx(HKEY_CURRENT_USER,"Software\\StabWin\\Start",0,KEY_READ,&hKey)==ERROR_SUCCESS)
	{
		DWORD size;
		size=sizeof(menue);
		RegQueryValueEx(hKey,"Menu",0,0,(BYTE*)&menue,&size);

		size=sizeof(uhr);
		RegQueryValueEx(hKey,"Uhr",0,0,(BYTE*)&uhr,&size);

		size=sizeof(tray);
		RegQueryValueEx(hKey,"Tray",0,0,(BYTE*)&tray,&size);

		size=sizeof(autostart);
		RegQueryValueEx(hKey,"Autostart",0,0,(BYTE*)&autostart,&size);

		size=sizeof(desktop);
		RegQueryValueEx(hKey,"Desktop",0,0,(BYTE*)&desktop,&size);

		RegCloseKey(hKey);
	}
	if(menue==1) ShellExecute(0,"open","menue.exe","",dir,SW_SHOW);
	if(uhr==1) ShellExecute(0,"open","uhr.exe","",dir,SW_SHOW);
	if(tray==1) ShellExecute(0,"open","TrayModule.exe","",dir,SW_SHOW);
	if(desktop==1) ShellExecute(0,"open","desktop.exe","",dir,SW_SHOW);
	if(autostart==1) ShellExecute(0,"open","autostart.exe","",dir,SW_SHOW);
	return 0;
}