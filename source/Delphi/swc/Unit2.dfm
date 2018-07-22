object Form2: TForm2
  Left = 236
  Top = 158
  Width = 305
  Height = 202
  Caption = 'Button-Eigenschaften'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  PixelsPerInch = 96
  TextHeight = 13
  object Label1: TLabel
    Left = 0
    Top = 0
    Width = 31
    Height = 13
    Caption = 'Name:'
  end
  object Label2: TLabel
    Left = 0
    Top = 40
    Width = 59
    Height = 13
    Caption = 'Befehlszeile:'
  end
  object Label3: TLabel
    Left = 0
    Top = 80
    Width = 33
    Height = 13
    Caption = 'L'#228'nge:'
  end
  object Label4: TLabel
    Left = 216
    Top = 96
    Width = 22
    Height = 13
    Caption = 'Pixel'
  end
  object Edit1: TEdit
    Left = 0
    Top = 16
    Width = 241
    Height = 21
    TabOrder = 0
  end
  object Edit2: TEdit
    Left = 0
    Top = 56
    Width = 241
    Height = 21
    TabOrder = 1
  end
  object Edit3: TEdit
    Left = 0
    Top = 96
    Width = 209
    Height = 21
    TabOrder = 2
    Text = '200'
  end
  object Button1: TButton
    Left = 24
    Top = 136
    Width = 75
    Height = 25
    Caption = 'OK'
    Default = True
    TabOrder = 3
    OnClick = Button1Click
  end
  object Button2: TButton
    Left = 136
    Top = 136
    Width = 75
    Height = 25
    Caption = 'Abbrechen'
    TabOrder = 4
    OnClick = Button2Click
  end
  object Button3: TButton
    Left = 248
    Top = 56
    Width = 17
    Height = 17
    Caption = 'D'
    TabOrder = 5
    OnClick = Button3Click
  end
  object Button4: TButton
    Left = 272
    Top = 56
    Width = 17
    Height = 17
    Caption = 'V'
    TabOrder = 6
    OnClick = Button4Click
  end
end
