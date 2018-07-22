#include <windows.h>

int APIENTRY WinMain(HINSTANCE,HINSTANCE,LPSTR,int)
{
	if(MessageBox(0,"Wollen Sie den Computer sperren bzw. den Benutzer wechseln?","Frage",MB_YESNO)==IDYES) {
		LockWorkStation();
	}
	return 0;
}