unit Unit2;

interface

uses
  Windows, Messages, SysUtils, Variants, Classes, Graphics, Controls, Forms,
  Dialogs, StdCtrls, FileCtrl;

type
  TForm2 = class(TForm)
    Label1: TLabel;
    Edit1: TEdit;
    Label2: TLabel;
    Edit2: TEdit;
    Label3: TLabel;
    Edit3: TEdit;
    Label4: TLabel;
    Button1: TButton;
    Button2: TButton;
    Button3: TButton;
    Button4: TButton;
    procedure Button2Click(Sender: TObject);
    procedure Button1Click(Sender: TObject);
    procedure Button3Click(Sender: TObject);
    procedure Button4Click(Sender: TObject);
  private
    { Private declarations }
  public
    abortcode:boolean;
    { Public declarations }
  end;

var
  Form2: TForm2;

implementation

uses Unit1;

{$R *.dfm}

procedure TForm2.Button2Click(Sender: TObject);
begin
abortcode:=true;
close;
end;

procedure TForm2.Button1Click(Sender: TObject);
begin
abortcode:=false;
close;
end;

procedure TForm2.Button3Click(Sender: TObject);
begin
with Form1.OpenDialog1 do begin
if execute then Edit3.Text:=filename;
end;
end;

procedure TForm2.Button4Click(Sender: TObject);
var dir:string;
begin
SelectDirectory('Bitte wählen Sie ein Verzeichnis:','',dir);
if dir<>'' then Edit3.Text:=dir;
end;

end.
