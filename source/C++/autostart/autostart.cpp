#include <windows.h>
#include <io.h>
#include <fstream>
#include <shlobj.h>
#include <shellapi.h>
#include <string>

using namespace std;


VOID log(string text)
{
	text+="\n";
	ofstream file("autostart_log.txt",ios::out|ios::ate);
	file << text.c_str();
}

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
VOID executeCmdLine(char* cmdline,char* directory)
{
	BOOL usedir=TRUE;
	if(lstrcmp(directory,"")==0) usedir=FALSE;
	STARTUPINFO startupinfo;
	PROCESS_INFORMATION processinfo;
	memset(&startupinfo,0,sizeof(startupinfo));
	startupinfo.cb=sizeof(startupinfo);
    if(!CreateProcess(NULL,(LPTSTR)cmdline,NULL,NULL,FALSE,NORMAL_PRIORITY_CLASS,
	                  NULL,usedir ? directory : NULL,&startupinfo,&processinfo))
	{
		char errstring[100];
		lstrcpy(errstring,"Fehler beim Ausführen von ");
		lstrcat(errstring,(LPCTSTR)cmdline);
		log(errstring);
		if(lstrcmp(cmdline,"")!=0)
		{
			MessageBox(0,errstring,"Fehler",0);
		}
	}
}

void StartAutoStart()
{
	long sh;
	_finddata_t fd;
	int res;

	char* directory=new char[MAX_PATH+1];
	char* searchpath=new char[MAX_PATH+5];
	SHGetSpecialFolderPath(0,directory,CSIDL_STARTUP,false);
	if(lstrcmp(directory,"")!=0)
	{
		log("Autostart: Durchsuche Verz.:");
		log(directory);
		lstrcpy(searchpath,directory);
		lstrcat(searchpath,"\\*.*");
		sh=_findfirst(searchpath,&fd);
		log("FindFirst gestartet");
		if(sh!=-1) res=0;
		while(res==0)
		{
			if(lstrcmp(fd.name,".")!=0 && lstrcmp(fd.name,"..")!=0 && lstrcmp(fd.name,"desktop.ini"))
			{
				log("FindNext: Datei:");
				log(fd.name);
				string cmdline("url.dll,FileProtocolHandler ");
				cmdline+=directory;
				cmdline+="\\";
				cmdline+=fd.name;
				ShellExecute(NULL,"open","rundll32.exe",cmdline.c_str(),directory,SW_SHOW);
			}
			res=_findnext(sh,&fd);
		}
		_findclose(sh);
	}

	SHGetSpecialFolderPath(0,directory,CSIDL_COMMON_STARTUP,false);
	if(lstrcmp(directory,"")!=0)
	{
		log("All Users Autostart: Durchsuche Verz.:");
		log(directory);
		lstrcpy(searchpath,directory);
		lstrcat(searchpath,"\\*.*");
		sh=_findfirst(searchpath,&fd);
		log("FindFirst gestartet");
		if(sh!=-1) res=0;
		while(res==0)
		{
			if(lstrcmp(fd.name,".")!=0 && lstrcmp(fd.name,"..")!=0 && lstrcmp(fd.name,"desktop.ini"))
			{
				log("FindNext: Datei:");
				log(fd.name);
				string cmdline("url.dll,FileProtocolHandler ");
				cmdline+=directory;
				cmdline+="\\";
				cmdline+=fd.name;
				ShellExecute(NULL,"open","rundll32.exe",cmdline.c_str(),directory,SW_SHOW);
			}
			res=_findnext(sh,&fd);
		}
		_findclose(sh);
	}
	delete [] directory;
	delete [] searchpath;
}

void Start_HKLM_Registry()
{
	HKEY myKey;
	int i=0;
	log("HKLM Registry");
	if(RegOpenKeyEx(HKEY_LOCAL_MACHINE,"Software\\Microsoft\\Windows\\CurrentVersion\\Run",0,KEY_READ,&myKey)==ERROR_SUCCESS)
	{
		log("RegKey erfolgreich geöffnet.");
		char* buffer=new char[MAX_PATH+1];
		char* errstring=new char[100];
		lstrcpy(buffer,"");
		int iLastError;
		do
		{
			char cValueName[256] = "";
			DWORD dwValueName = 256;
			DWORD ValueType = 0;
			BYTE cValueData[256] = "\0";
			DWORD dwValueData = 256;
			iLastError = RegEnumValue(myKey,i,cValueName,&dwValueName,0,&ValueType,cValueData,&dwValueData);
			++i;

			if(ValueType==REG_SZ && iLastError==ERROR_SUCCESS)
			{
				lstrcpy(buffer,(LPCTSTR) cValueData);
				log("Wert gefunden:");
				log(buffer);
				executeCmdLine(buffer,"");
/*				separateCmdLine(buffer);
				if((int)ShellExecute(0,"open",command.c_str(),param.c_str(),"",SW_SHOW)<=32)
				{
					lstrcpy(errstring,"Fehler beim Ausführen von ");
					lstrcat(errstring,buffer);
					MessageBox(0,errstring,"",0);
				}
				command="";
				param="";*/
			}
		}
		while(iLastError!=ERROR_NO_MORE_ITEMS);
		RegCloseKey(myKey);
		delete [] buffer;
		delete [] errstring;
	}
}

void Start_HKCU_Registry()
{
	HKEY myKey;
	char* buffer=new char[MAX_PATH+1];
	char* errstring=new char[100];
	int	iLastError;
	int i=0;
	log("HKCU Registry 1");
	if(RegOpenKeyEx(HKEY_CURRENT_USER,"Software\\Microsoft\\Windows\\CurrentVersion\\Run",0,KEY_READ,&myKey)==ERROR_SUCCESS)
	{
		log("RegKey erfolgreich geöffnet.");
		lstrcpy(buffer,"");
		do
		{
			char cValueName[256] = "";
			DWORD dwValueName = 256;
			DWORD ValueType = 0;
			BYTE cValueData[256] = "\0";
			DWORD dwValueData = 256;
			iLastError = RegEnumValue(myKey,i,cValueName,&dwValueName,0,&ValueType,cValueData,&dwValueData);
			++i;

			if(ValueType==REG_SZ && iLastError==ERROR_SUCCESS)
			{
				lstrcpy(buffer,(LPCTSTR) cValueData);
				log("Wert gefunden:");
				log(buffer);
				executeCmdLine(buffer,"");
/*				separateCmdLine(buffer);
				if((int)ShellExecute(0,"open",command.c_str(),param.c_str(),"",SW_SHOW)<=32)
				{
					lstrcpy(errstring,"Fehler beim Ausführen von ");
					lstrcat(errstring,buffer);
					MessageBox(0,errstring,"",0);
				}
				command="";
				param="";*/
			}
		}
		while(iLastError!=ERROR_NO_MORE_ITEMS);

		RegCloseKey(myKey);
	}

	log("HKCU Registry 2");
	if(RegOpenKeyEx(HKEY_CURRENT_USER,"Software\\Microsoft\\Windows\\CurrentVersion\\RunOnce",0,KEY_READ,&myKey)==ERROR_SUCCESS)
	{
		i=0;
		log("RegKey erfolgreich geöffnet.");
		lstrcpy(buffer,"");
		do
		{
			char cValueName[256] = "";
			DWORD dwValueName = 256;
			DWORD ValueType = 0;
			BYTE cValueData[256] = "\0";
			DWORD dwValueData = 256;
			iLastError = RegEnumValue(myKey,i,cValueName,&dwValueName,0,&ValueType,cValueData,&dwValueData);
			++i;

			if(ValueType==REG_SZ && iLastError==ERROR_SUCCESS)
			{
				lstrcpy(buffer,(LPCTSTR) cValueData);
				log("Wert gefunden:");
				log(buffer);
				executeCmdLine(buffer,"");
/*				separateCmdLine(buffer);
				if((int)ShellExecute(0,"open",command.c_str(),param.c_str(),"",SW_SHOW)<=32)
				{
					lstrcpy(errstring,"Fehler beim Ausführen von ");
					lstrcat(errstring,buffer);
					MessageBox(0,errstring,"",0);
				}
				command="";
				param="";*/
			}
			RegDeleteValue(myKey,cValueName);
		}
		while(iLastError!=ERROR_NO_MORE_ITEMS);

		RegCloseKey(myKey);
		delete [] buffer;
		delete [] errstring;
	}
}

void Start_HKUSERS_Registry()
{
	HKEY myKey;
	char* buffer=new char[MAX_PATH+1];
	char* errstring=new char[100];
	log("HKUSERS Registry 1");
	int iLastError;
	int i=0;
	if(RegOpenKeyEx(HKEY_USERS,"Software\\Microsoft\\Windows\\CurrentVersion\\Run",0,KEY_READ,&myKey)==ERROR_SUCCESS)
	{
		log("RegKey erfolgreich geöffnet.");
		lstrcpy(buffer,"");
		do
		{
			char cValueName[256] = "";
			DWORD dwValueName = 256;
			DWORD ValueType = 0;
			BYTE cValueData[256] = "\0";
			DWORD dwValueData = 256;
			iLastError = RegEnumValue(myKey,i,cValueName,&dwValueName,0,&ValueType,cValueData,&dwValueData);
			++i;

			if(ValueType==REG_SZ && iLastError==ERROR_SUCCESS)
			{
				lstrcpy(buffer,(LPCTSTR) cValueData);
				log("Wert gefunden:");
				log(buffer);
				executeCmdLine(buffer,"");
/*				separateCmdLine(buffer);
				if((int)ShellExecute(0,"open",command.c_str(),param.c_str(),"",SW_SHOW)<=32)
				{
					lstrcpy(errstring,"Fehler beim Ausführen von ");
					lstrcat(errstring,buffer);
					MessageBox(0,errstring,"",0);
				}
				command="";
				param="";*/
			}
		}
		while(iLastError!=ERROR_NO_MORE_ITEMS);

		RegCloseKey(myKey);
	}

	log("HKUSERS Registry 2");
	if(RegOpenKeyEx(HKEY_USERS,"Software\\Microsoft\\Windows\\CurrentVersion\\RunOnce",0,KEY_READ,&myKey)==ERROR_SUCCESS)
	{
		i=0;
		log("RegKey erfolgreich geöffnet.");
		lstrcpy(buffer,"");
		do
		{
			char cValueName[256] = "";
			DWORD dwValueName = 256;
			DWORD ValueType = 0;
			BYTE cValueData[256] = "\0";
			DWORD dwValueData = 256;
			iLastError = RegEnumValue(myKey,i,cValueName,&dwValueName,0,&ValueType,cValueData,&dwValueData);
			++i;

			if(ValueType==REG_SZ && iLastError==ERROR_SUCCESS)
			{
				lstrcpy(buffer,(LPCTSTR) cValueData);
				log("Wert gefunden:");
				log(buffer);
				executeCmdLine(buffer,"");
/*				separateCmdLine(buffer);
				if((int)ShellExecute(0,"open",command.c_str(),param.c_str(),"",SW_SHOW)<=32)
				{
					lstrcpy(errstring,"Fehler beim Ausführen von ");
					lstrcat(errstring,command.c_str());
					lstrcat(errstring," mit den Parametern:");
					lstrcat(errstring,param.c_str());
					MessageBox(0,errstring,"",0);
				}
				command="";
				param="";*/
			}	
		}
		while(iLastError!=ERROR_NO_MORE_ITEMS);

		RegCloseKey(myKey);
		delete [] buffer;
		delete [] errstring;
	}
}

int APIENTRY WinMain(HINSTANCE,HINSTANCE,LPSTR,int)
{
	DeleteFile("autostart_log.txt");
	log("StabWin Autostart Log");
	StartAutoStart();
	Start_HKLM_Registry();
	Start_HKCU_Registry();
	Start_HKUSERS_Registry();
	return 0;
}


