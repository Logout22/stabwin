unit Unit1;

interface

uses Controls, StdCtrls, Classes, ComCtrls, TabNotBk,
     Forms, SysUtils, Windows, ShlObj, INIFiles, FileCtrl,
     Registry, Dialogs,ActiveX, ComObj,ShellAPI;

type
  TForm1 = class(TForm)
    TabbedNotebook1: TTabbedNotebook;
    Button1: TButton;
    Button2: TButton;
    Label2: TLabel;
    CheckBox1: TCheckBox;
    CheckBox2: TCheckBox;
    CheckBox3: TCheckBox;
    Edit1: TEdit;
    Label3: TLabel;
    Button3: TButton;
    Label4: TLabel;
    Edit2: TEdit;
    CheckBox6: TCheckBox;
    Label5: TLabel;
    CheckBox7: TCheckBox;
    CheckBox8: TCheckBox;
    CheckBox9: TCheckBox;
    Label6: TLabel;
    Button4: TButton;
    ListBox1: TListBox;
    Button5: TButton;
    Button6: TButton;
    CheckBox5: TCheckBox;
    CheckBox10: TCheckBox;
    CheckBox11: TCheckBox;
    Memo1: TMemo;
    Label8: TLabel;
    CheckBox16: TCheckBox;
    CheckBox17: TCheckBox;
    CheckBox18: TCheckBox;
    CheckBox19: TCheckBox;
    CheckBox20: TCheckBox;
    Label7: TLabel;
    CheckBox4: TCheckBox;
    CheckBox12: TCheckBox;
    CheckBox14: TCheckBox;
    Label1: TLabel;
    RadioButton1: TRadioButton;
    RadioButton2: TRadioButton;
    Button7: TButton;
    Button9: TButton;
    Label9: TLabel;
    Edit3: TEdit;
    Button10: TButton;
    OpenDialog1: TOpenDialog;
    Button11: TButton;
    Label10: TLabel;
    Button12: TButton;
    Button13: TButton;
    Button8: TButton;
    CheckBox13: TCheckBox;
    Label11: TLabel;
    Label12: TLabel;
    Edit4: TEdit;
    Label13: TLabel;
    Edit5: TEdit;
    Label14: TLabel;
    Label15: TLabel;
    procedure Button2Click(Sender: TObject);
    procedure Button5Click(Sender: TObject);
    procedure ListButtons;
    procedure Button6Click(Sender: TObject);
    procedure Button4Click(Sender: TObject);
    procedure Button1Click(Sender: TObject);
    procedure SetStandardSettings;
    procedure Button3Click(Sender: TObject);
    procedure FormCreate(Sender: TObject);
    procedure CreateLink(filename,param,desc:string;iconname:string='';iconnumber:integer=0);
    procedure setshell(prgname:string;msgbox:boolean=true);
    procedure FormClose(Sender: TObject; var Action: TCloseAction);
    procedure Button7Click(Sender: TObject);
    procedure Button8Click(Sender: TObject);
    procedure Button9Click(Sender: TObject);
    procedure SetStandardButtons;
    procedure Button10Click(Sender: TObject);
    procedure Button11Click(Sender: TObject);
    procedure Button12Click(Sender: TObject);
    procedure Button13Click(Sender: TObject);
    procedure TabbedNotebook1Change(Sender: TObject; NewTab: Integer;
      var AllowChange: Boolean);
  private
    { Private declarations }
  public
    { Public declarations }
  end;

  button=record
   text:string;
   app:string;
   length:integer;
  end;

var
  Form1: TForm1;buttons:array of button;reg:TRegistry;
  install:boolean;

implementation

uses Unit2;

{$R *.dfm}

function ChangeSecurityForShutdown: Boolean;
var
hToken    : THandle ;
tkp, Newt : TTokenPrivileges ;
retlength : DWORD ;
begin
Result := False ;
if OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES + TOKEN_QUERY, hToken) <> False then begin
if LookupPrivilegeValue( nil, 'SeShutdownPrivilege',tkp.Privileges[0].Luid) = True then begin
tkp.PrivilegeCount := 1; // One to set
tkp.Privileges[0].Attributes := SE_PRIVILEGE_ENABLED;
{ Get shutdown privilege for this process }
Result := AdjustTokenPrivileges(hToken, False, tkp,SizeOf(TTokenPrivileges),Newt,retlength);
end;
end;
end;

function GetLinkName(desc:string):string;
var buf:array[0..MAX_PATH+1] of char;
begin
Result:='';
SHGetSpecialFolderPath(0,@buf,CSIDL_STARTMENU,false);
Result:=buf;
Result:=Result+'\StabWin\'+desc+'.lnk';
end;

function CheckForLink(desc:string):boolean;
var fileh:hfile;fname:string;
begin
fname := GetLinkName(desc);
fileh:=CreateFile(PChar(fname),GENERIC_READ,0,nil,OPEN_EXISTING,0,0);
if fileh=INVALID_HANDLE_VALUE then begin
result:=false
end
else begin
CloseHandle(fileh);
result:=true;
end;
end;

function ReadString(Group,ValueName,Default:string):String;
begin
Result:=Default;
if reg.OpenKey('Software\StabWin\'+Group,true) then begin
try
Result:=reg.ReadString(ValueName);
except
Result:=Default;
end;
reg.CloseKey;
end;
end;

function ReadInteger(Group,ValueName:string;Default:Integer):Integer;
begin
Result:=Default;
if reg.OpenKey('Software\StabWin\'+Group,true) then begin
try
Result:=reg.ReadInteger(ValueName);
except
Result:=Default;
end;
reg.CloseKey;
end;
end;

function ReadBool(Group,ValueName:string;Default:Boolean):Boolean;
begin
Result:=Default;
if reg.OpenKey('Software\StabWin\'+Group,true) then begin
try
Result:=reg.ReadBool(ValueName);
except
Result:=Default;
end;
reg.CloseKey;
end;
end;

procedure WriteString(Group,ValueName:string;Value:String);
begin
if reg.OpenKey('Software\StabWin\'+Group,true) then begin
reg.WriteString(ValueName,Value);
reg.CloseKey;
end;
end;

procedure WriteInteger(Group,ValueName:string;Value:Integer);
begin
if reg.OpenKey('Software\StabWin\'+Group,true) then begin
reg.WriteInteger(ValueName,Value);
reg.CloseKey;
end;
end;

procedure WriteBool(Group,ValueName:string;Value:Boolean);
begin
if reg.OpenKey('Software\StabWin\'+Group,true) then begin
reg.WriteBool(ValueName,Value);
reg.CloseKey;
end;
end;

function GetAdminSid: PSID;
const
  // bekannte SIDs ... (WinNT.h)
  SECURITY_NT_AUTHORITY: TSIDIdentifierAuthority = (Value: (0, 0, 0, 0, 0, 5));
  // bekannte RIDs ... (WinNT.h)
  SECURITY_BUILTIN_DOMAIN_RID: DWORD = $00000020;
  DOMAIN_ALIAS_RID_ADMINS: DWORD = $00000220;
begin
  Result := nil;
  AllocateAndInitializeSid(SECURITY_NT_AUTHORITY, 2,
    SECURITY_BUILTIN_DOMAIN_RID, DOMAIN_ALIAS_RID_ADMINS,
    0, 0, 0, 0, 0, 0, Result);
end;

function IsAdmin: LongBool;
var
  TokenHandle: THandle;
  ReturnLength: DWORD;
  TokenInformation: PTokenGroups;
  AdminSid: PSID;
  Loop: Integer;
begin
  Result := False;
  TokenHandle := 0;
  if OpenProcessToken(GetCurrentProcess, TOKEN_QUERY, TokenHandle) then
  try  
    ReturnLength := 0;
    GetTokenInformation(TokenHandle, TokenGroups, nil, 0, ReturnLength);
    TokenInformation := GetMemory(ReturnLength);
    if Assigned(TokenInformation) then
    try
      if GetTokenInformation(TokenHandle, TokenGroups, TokenInformation,
        ReturnLength, ReturnLength) then
      begin
        AdminSid := GetAdminSid;
        for Loop := 0 to TokenInformation^.GroupCount - 1 do  
        begin  
          if EqualSid(TokenInformation^.Groups[Loop].Sid, AdminSid) then
          begin
            Result := True;
            Break;  
          end;  
        end;
        FreeSid(AdminSid);  
      end;
    finally
      FreeMemory(TokenInformation);
    end;
  finally
    CloseHandle(TokenHandle);  
  end;
end;

function shortpath(longname:string):string;
var buf:array[0..max_path] of char;
begin
GetShortPathName(pchar(longname),buf,MAX_PATH);
result:=buf;
end;

procedure TForm1.CreateLink(filename,param,desc:string;iconname:string='';iconnumber:integer=0);
var  IObj: IUnknown;
     Link: IShellLink;
     IPFile: IPersistFile;
     TargetW: WideString;
     shfi:SHFileInfo;
begin
  IObj := CreateComObject (CLSID_ShellLink);
  Link := IObj as IShellLink;
  IPFile := IObj as IPersistFile;
  link.SetWorkingDirectory(pchar(edit1.Text));
  link.SetArguments(PChar(param));
  link.SetShowCmd(SW_SHOW);
  link.SetPath(pchar('"'+filename+'"'));
  if iconname='' then begin
    SHGetFileInfo(pchar(filename),0,shfi,sizeof(shfi),SHGFI_ICONLOCATION);
    iconname:=shfi.szDisplayName;
  end;
  link.SetIconLocation(PChar(iconname),iconnumber);
  TargetW := GetLinkName(desc);
  IPFile.Save (PWChar (TargetW), False);
end;

procedure tform1.setshell(prgname:string;msgbox:boolean=true);
var inifile:TInifile;
shellreg:TRegistry;
begin
if GetVersion<$80000000 then begin
if not IsAdmin then begin
ShowMessage('Sie haben keine Administratorrechte. Lassen Sie die Shelländerung von Ihrem Systemadministrator durchführen.');
exit;
end;
shellreg:=TRegistry.Create;
shellreg.RootKey:=HKEY_LOCAL_MACHINE;
if shellreg.OpenKey('Software\Microsoft\Windows NT\CurrentVersion\Winlogon',false) then
shellreg.WriteString('Shell',prgname);
shellreg.CloseKey;
shellreg.Free;
end
else begin
inifile:=tinifile.Create('system.ini');;
inifile.WriteString('boot','shell',prgname);
inifile.Free;
end;
if msgbox then messagebox(form1.handle,'Bitte starten Sie nun ihr System neu.','Neustart',0);
end;

function GetFirstCDDrive:string;
var buf:array[0..78] of char;i:integer;
begin
GetLogicalDriveStrings(78,buf);
for i:=0 to 77 do
  if buf[i]=#0 then begin
    if buf[i-2]=':' then begin
      result:=buf[i-3]+buf[i-2];
      if GetDriveType(pchar(result))=DRIVE_CDROM then begin
        break;
      end;
    end;
  end;
end;

procedure TForm1.SetStandardButtons;
var buf:button;
begin
setlength(buttons,2);
buf.text:='Startmenü';
buf.app:=edit1.Text+'\startmenu.exe';
buf.length:=200;
buttons[0]:=buf;
buf.text:='CD / DVD';
buf.app:=edit1.Text+'\s_file.exe '+GetFirstCDDrive+' s';
buf.length:=200;
buttons[1]:=buf;
ListButtons;
end;

procedure TForm1.SetStandardSettings;
begin
edit1.Text:=GetCurrentDir;
edit3.Text:=edit1.Text+'\s_file.exe';
SetStandardButtons;
end;

procedure TForm1.ListButtons;
var i:integer;
begin
listbox1.Clear;
for i:=0 to length(buttons)-1 do
  listbox1.AddItem(buttons[i].text,nil);
end;

procedure TForm1.Button2Click(Sender: TObject);
begin
close;
end;

procedure TForm1.Button5Click(Sender: TObject);
var buf:button;i:integer;
begin
Form2.showmodal;
if not form2.abortcode then begin
buf.text:=form2.Edit1.Text;
buf.app:=form2.Edit2.Text;
buf.length:=strtoint(form2.Edit3.Text);
i:=length(buttons)+1;
setlength(buttons,i);
buttons[i-1]:=buf;
ListButtons;
end;
end;

procedure TForm1.Button6Click(Sender: TObject);
var buf:array of button;i,j:integer;
begin
setlength(buf,length(buttons)-1);
j:=0;
for i:=0 to length(buttons)-1 do begin
  if not listbox1.Selected[i] then begin
    buf[j]:=buttons[i];
    inc(j);
  end
  else begin
    with TRegistry.Create do begin
      DeleteKey('Software\StabWin\Button'+inttostr(i));
      free;
    end;
  end;
end;
setlength(buttons,length(buf));
for i:=0 to length(buf)-1 do
  buttons[i]:=buf[i];
ListButtons;
end;

procedure TForm1.Button4Click(Sender: TObject);
var i,pos:integer;buf:button;found:boolean;
begin
found:=false;
pos:=0;
for i:=0 to listbox1.Count-1 do
if listbox1.Selected[i] then begin
buf:=buttons[i];
pos:=i;
found:=true;
end;
if not found then begin
MessageBox(handle,'Bitte wählen Sie einen Eintrag aus!','Fehler',0);
exit;
end;
Form2.Edit1.Text:=buf.text;
Form2.Edit2.Text:=buf.app;
Form2.Edit3.Text:=inttostr(buf.length);
form2.Showmodal;
if not form2.abortcode then begin
buttons[pos].text:=Form2.Edit1.Text;
buttons[pos].app:=Form2.Edit2.Text;
buttons[pos].length:=strtoint(Form2.Edit3.Text);
end;
end;

procedure TForm1.Button1Click(Sender: TObject);
var i:integer;buf:array[0..MAX_PATH+1] of char;trayx,trayy:integer;
label shellset;
begin
WriteBool('Desktop','One Click',RadioButton1.Checked);
WriteBool('Start','Menu',checkbox1.Checked);
WriteBool('Start','Desktop',Checkbox2.Checked);
WriteBool('Start','Uhr',Checkbox3.Checked);
WriteBool('Start','Tray',Checkbox11.Checked);
WriteBool('Start','Autostart',Checkbox5.Checked);
WriteString('All','StabWinDir',Edit1.Text);
WriteString('All','FManPath',Edit3.Text);
WriteString('S_File','Type',Edit2.Text);
WriteBool('S_File','Archive',checkbox6.checked);
WriteBool('S_File','System',checkbox7.checked);
WriteBool('S_File','Hidden',checkbox8.checked);
WriteBool('S_File','Read-only',checkbox9.checked);
WriteInteger('All','ButtonCount',length(buttons));
if length(buttons)>0 then
for i:=0 to length(buttons)-1 do begin
WriteString('Button'+inttostr(i),'Text',buttons[i].text);
WriteString('Button'+inttostr(i),'App',buttons[i].app);
WriteInteger('Button'+inttostr(i),'Length',buttons[i].length);
end;
if checkbox10.Checked then begin
if install then begin
if MessageBox(handle,'Bevor Sie die Oberfläche auf StabWin setzen, müssen Sie sicherstellen,'+
#10'dass dieser Assistent für alle Benutzer auf dem Computer ausgeführt wurde.'+
#10'Ist das der Fall? Wenn nicht, deaktivieren Sie "StabWin als Oberfläche einsetzen".','Frage',
MB_YESNO or MB_ICONQUESTION)=IDYES then begin
shellset:
setshell(shortpath(Edit1.Text+'\stabwin.exe'));
end {MSGBox}
else begin
exit;
end; {elseExit}
end {ifInstall}
else begin
goto shellset;
end; {elseInstall}
end  {ifCheckbox10}
else begin
setshell('Explorer.exe');
end;  {elseCheckbox10}
SHGetSpecialFolderPath(0,@buf,CSIDL_STARTMENU,false);
lstrcat(buf,'\StabWin');
mkdir(buf);
if checkbox4.Checked then CreateLink(edit1.Text+'\shutdown.exe','','Abmelden+Beenden')
else SysUtils.DeleteFile(GetLinkName('Abmelden+Beenden'));
if checkbox13.Checked then CreateLink(edit1.Text+'\changeuser.exe','','Benutzer wechseln bzw. Computer sperren')
else SysUtils.DeleteFile(GetLinkName('Benutzer wechseln bzw. Computer sperren'));
if checkbox12.Checked then CreateLink(edit1.Text+'\sw_info.exe','','Informationen über StabWin')
else SysUtils.DeleteFile(GetLinkName('Informationen über StabWin'));
if checkbox14.Checked then CreateLink(edit1.Text+'\config.exe','','StabWin Konfiguration')
else SysUtils.DeleteFile(GetLinkName('StabWin Konfiguration'));

WriteBool('Desktop','Arbeitsplatz',checkbox16.Checked);
WriteBool('Desktop','EigeneDateien',checkbox17.Checked);
WriteBool('Desktop','IE',checkbox18.Checked);
WriteBool('Desktop','Netzwerkumgebung',checkbox19.Checked);
WriteBool('Desktop','Papierkorb',checkbox20.Checked);

trayx:=0;trayy:=0;
try
  trayx:=StrToInt(Edit4.Text);
  trayy:=StrToInt(Edit5.Text);
except
  ShowMessage('Geben Sie als Tray-Koordinaten bitte ganze Zahlen an.');
end;

try
  with TIniFile.Create(edit1.Text+'\TrayModule.ini') do begin
    WriteInteger('layout','x',trayx);
    WriteInteger('layout','y',trayy);
    free;
  end;
except
  ShowMessage('Die INI-Datei TrayModule.ini konnte nicht geschrieben werden.'#10+
  'Eventuell haben Sie keine Schreibrechte für das StabWin-Verzeichnis.');
end;

close;
end;

procedure TForm1.Button3Click(Sender: TObject);
var dir:string;
begin
SelectDirectory('Bitte wählen Sie das StabWin-Verzeichnis:','',dir);
if dir<>'' then edit1.Text:=dir;
end;

procedure TForm1.FormCreate(Sender: TObject);
var i,count:integer;oneclick:boolean;menu:thandle;
begin
if paramstr(1)='i' then begin
ShowMessage('Sie können nun die Konfiguration von StabWin anpassen.'+#10+
            'Lesen Sie sich vorher bitte das Handbuch durch, um unnötige'+#10+
            'Schwierigkeiten zu vermeiden.'+#10+
            'Es ist UNBEDINGT notwendig, dass Sie am Schluss'+#10+
            'auf OK klicken!');
Button2.Enabled:=false;
menu:=GetSystemMenu(Form1.handle,false);
DeleteMenu(menu,6,MF_BYPOSITION);
Checkbox4.Checked:=true;
Checkbox12.Checked:=true;
Checkbox14.Checked:=true;
install:=true;
end;
Label10.Caption:=
'Herzlich willkommen im StabWin-Konfigurationsprogramm. Dieser Assistent'+#10+
'wird Sie durch die Installation von StabWin leiten. Durch einen Druck auf'+#10+
'den Button Weiter gelangen Sie zum nächsten Abschnitt der Installation,'+#10+
'Zurück führt Sie einen Abschnitt zurück. Sie können die einzelnen Schritte'+#10+
'auch aufrufen, indem Sie auf die Registerreiter klicken.'+#10+
#10+
'Deaktivieren Sie vor der Installation UNBEDINGT nebenbei laufende'+#10+
'Programme, vor allem Antivirenprogramme etc.';
reg:=TRegistry.Create;
count:=ReadInteger('All','ButtonCount',-5);    //Sinloser Standardwert
if count=-5 then begin //Wenn der Standardwert zurückgeliefert wird, sind keine Einstellungen gesetzt
SetStandardSettings;
exit;
end;
oneclick:=ReadBool('Desktop','One Click',RadioButton1.Checked);
if oneclick then RadioButton1.Checked:=true
else RadioButton2.Checked:=true;
checkbox1.Checked:=ReadBool('Start','Menu',checkbox1.Checked);
Checkbox2.Checked:=ReadBool('Start','Desktop',Checkbox2.Checked);
Checkbox3.Checked:=ReadBool('Start','Uhr',Checkbox3.Checked);
Checkbox5.Checked:=ReadBool('Start','Autostart',Checkbox5.Checked);
Checkbox11.Checked:=ReadBool('Start','Tray',Checkbox11.Checked);
Edit1.Text:=ReadString('All','StabWinDir',Edit1.Text);
Edit2.Text:=ReadString('S_File','Type',Edit2.Text);
Edit3.Text:=ReadString('All','FManPath',Edit3.Text);
checkbox6.checked:=ReadBool('S_File','Archive',checkbox6.checked);
checkbox7.checked:=ReadBool('S_File','System',checkbox7.checked);
checkbox8.checked:=ReadBool('S_File','Hidden',checkbox8.checked);
checkbox9.checked:=ReadBool('S_File','Read-only',checkbox9.checked);
if CheckForLink('Abmelden+Beenden') then checkbox4.Checked:=true
else checkbox4.Checked:=false;
if CheckForLink('Benutzer wechseln bzw. Computer sperren') then checkbox13.Checked:=true
else checkbox13.Checked:=false;
if CheckForLink('Informationen über StabWin') then checkbox12.Checked:=true
else checkbox12.Checked:=false;
if CheckForLink('StabWin Konfiguration') then checkbox14.Checked:=true
else checkbox14.Checked:=false;
checkbox16.checked:=ReadBool('Desktop','Arbeitsplatz',checkbox16.checked);
checkbox17.checked:=ReadBool('Desktop','EigeneDateien',checkbox17.checked);
checkbox18.checked:=ReadBool('Desktop','IE',checkbox18.checked);
checkbox19.checked:=ReadBool('Desktop','Netzwerkumgebung',checkbox19.checked);
checkbox20.checked:=ReadBool('Desktop','Papierkorb',checkbox20.checked);
with TIniFile.Create(Edit1.Text+'\TrayModule.ini') do begin
Edit4.Text:=IntToStr(ReadInteger('layout','x',0));
Edit5.Text:=IntToStr(ReadInteger('layout','y',0));
free;
end;
SetLength(buttons,count);
for i:=0 to count-1 do begin
buttons[i].text:=ReadString('Button'+inttostr(i),'Text','');
buttons[i].app:=ReadString('Button'+inttostr(i),'App','');
buttons[i].length:=ReadInteger('Button'+inttostr(i),'Length',0);
end;
ListButtons;
end;

procedure TForm1.FormClose(Sender: TObject; var Action: TCloseAction);
begin
reg.Free;
end;

procedure TForm1.Button7Click(Sender: TObject);
var buf:array [0..MAX_PATH+1] of char;
label shutdown;
begin
if MessageBox(handle,'WARNUNG: Sie sind gerade dabei, StabWin von Ihrem PC zu entfernen.'+
#10'Alle Dateien und Einstellungen werden gelöscht. Wollen Sie das?','Warnung',MB_YESNO or MB_ICONEXCLAMATION)=IDYES
then begin
if GetVersion<$80000000 then begin
if not IsAdmin then begin
ShowMessage('Sie haben keine Administrator-Rechte.');
Exit;
end;
end;
with TRegistry.Create do begin
DeleteKey('Software\StabWin');
Free;
end;
SysUtils.DeleteFile(GetLinkName('Abmelden+Beenden'));
SysUtils.DeleteFile(GetLinkName('Benutzer wechseln bzw. Computer sperren'));
SysUtils.DeleteFile(GetLinkName('Informationen über StabWin'));
SysUtils.DeleteFile(GetLinkName('StabWin Konfiguration'));
SHGetSpecialFolderPath(0,@buf,CSIDL_STARTMENU,false);
RmDir(buf+'\StabWin');
SHGetSpecialFolderPath(handle,@buf,CSIDL_STARTUP,false);
Memo1.Lines.Add('del '+Edit1.Text+'\autostart.exe');
Memo1.Lines.Add('del '+Edit1.Text+'\autostart_log.txt');
Memo1.Lines.Add('del '+Edit1.Text+'\changeuser.exe');
Memo1.Lines.Add('del '+Edit1.Text+'\config.exe');
Memo1.Lines.Add('del '+Edit1.Text+'\desktop.exe');
Memo1.Lines.Add('del '+Edit1.Text+'\menue.exe');
Memo1.Lines.Add('del '+Edit1.Text+'\s_file.exe');
Memo1.Lines.Add('del '+Edit1.Text+'\shutdown.exe');
Memo1.Lines.Add('del '+Edit1.Text+'\stabwin.exe');
Memo1.Lines.Add('del '+Edit1.Text+'\startmenu.exe');
Memo1.Lines.Add('del '+Edit1.Text+'\sw_help.cnt');
Memo1.Lines.Add('del '+Edit1.Text+'\sw_help.hlp');
Memo1.Lines.Add('del '+Edit1.Text+'\sw_info.exe');
Memo1.Lines.Add('del '+Edit1.Text+'\traymodule-all-settings.ini');
Memo1.Lines.Add('del '+Edit1.Text+'\traymodule-readme.txt');
Memo1.Lines.Add('del '+Edit1.Text+'\TrayModule.exe');
Memo1.Lines.Add('del '+Edit1.Text+'\TrayModule.ini');
Memo1.Lines.Add('del '+Edit1.Text+'\uhr.exe');
Memo1.Lines.Add('rmdir '+Edit1.Text);
Memo1.Lines.Add('del %0');
Memo1.Lines.SaveToFile(buf+'\delstabwin.bat');
SetShell('Explorer.exe',false);
ShowMessage('Schließen Sie alle Programme und klicken Sie auf OK.');
if GetVersion<$80000000 then begin
if ChangeSecurityForShutdown then begin
shutdown:
ExitWindowsEx(EWX_REBOOT,0);
end; {ChangeSec}
end {GetVer}
else begin
goto shutdown;
end; {elseGetVer}
end;
end;

procedure TForm1.Button8Click(Sender: TObject);
begin
ShellExecute(handle,'open','sw_help.hlp',nil,nil,SW_SHOW);
end;

procedure TForm1.Button9Click(Sender: TObject);
begin
SetStandardButtons;
end;

procedure TForm1.Button10Click(Sender: TObject);
begin
if OpenDialog1.Execute then Edit3.Text:=OpenDialog1.FileName;
end;

procedure TForm1.Button11Click(Sender: TObject);
begin
edit3.Text:=edit1.Text+'\s_file.exe';
end;

procedure TForm1.Button12Click(Sender: TObject);
begin
Button13.Enabled:=true;
if TabbedNotebook1.PageIndex<=(TabbedNotebook1.Pages.Count-1) then begin
TabbedNotebook1.PageIndex:=TabbedNotebook1.PageIndex+1;
end;
if TabbedNotebook1.PageIndex=(TabbedNotebook1.Pages.Count-1) then begin
Button12.Enabled:=false;
end;
end;

procedure TForm1.Button13Click(Sender: TObject);
begin
Button12.Enabled:=true;
if TabbedNotebook1.PageIndex>=0 then begin
TabbedNotebook1.PageIndex:=TabbedNotebook1.PageIndex-1;
end;
if TabbedNotebook1.PageIndex=0 then begin
Button13.Enabled:=false;
end;
end;

procedure TForm1.TabbedNotebook1Change(Sender: TObject; NewTab: Integer;
  var AllowChange: Boolean);
var special:boolean;
begin
special:=false;
if NewTab=(TabbedNotebook1.Pages.Count-1) then begin
Button13.Enabled:=true;
Button12.Enabled:=false;
special:=true;
end;
if NewTab=0 then begin
Button13.Enabled:=false;
Button12.Enabled:=true;
special:=true;
end;
if not special then begin
Button13.Enabled:=true;
Button12.Enabled:=true;
end;
AllowChange:=true;
end;

end.
