object Form1: TForm1
  Left = 190
  Top = 111
  Width = 419
  Height = 389
  BorderIcons = [biSystemMenu, biMinimize]
  Caption = 'StabWin Konfiguration'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  Visible = True
  OnClose = FormClose
  OnCreate = FormCreate
  PixelsPerInch = 96
  TextHeight = 13
  object TabbedNotebook1: TTabbedNotebook
    Left = 8
    Top = 8
    Width = 393
    Height = 273
    TabFont.Charset = DEFAULT_CHARSET
    TabFont.Color = clBtnText
    TabFont.Height = -11
    TabFont.Name = 'MS Sans Serif'
    TabFont.Style = []
    TabOrder = 0
    OnChange = TabbedNotebook1Change
    object TTabPage
      Left = 4
      Top = 24
      Caption = 'Willkommen'
      object Label10: TLabel
        Left = 0
        Top = 56
        Width = 3
        Height = 13
      end
    end
    object TTabPage
      Left = 4
      Top = 24
      Caption = 'Allgemein'
      object Label2: TLabel
        Left = 8
        Top = 96
        Width = 96
        Height = 13
        Caption = 'Automatisch starten:'
      end
      object Label3: TLabel
        Left = 8
        Top = 16
        Width = 101
        Height = 13
        Caption = 'StabWin-Verzeichnis:'
      end
      object CheckBox1: TCheckBox
        Left = 8
        Top = 112
        Width = 49
        Height = 17
        Caption = 'Men'#252
        Checked = True
        State = cbChecked
        TabOrder = 0
      end
      object CheckBox2: TCheckBox
        Left = 8
        Top = 128
        Width = 65
        Height = 17
        Caption = 'Desktop'
        Checked = True
        State = cbChecked
        TabOrder = 1
      end
      object CheckBox3: TCheckBox
        Left = 8
        Top = 144
        Width = 41
        Height = 17
        Caption = 'Uhr'
        Checked = True
        State = cbChecked
        TabOrder = 2
      end
      object Edit1: TEdit
        Left = 8
        Top = 32
        Width = 161
        Height = 21
        TabOrder = 3
      end
      object Button3: TButton
        Left = 176
        Top = 32
        Width = 27
        Height = 25
        Caption = '...'
        TabOrder = 4
        OnClick = Button3Click
      end
      object CheckBox5: TCheckBox
        Left = 8
        Top = 160
        Width = 65
        Height = 17
        Caption = 'Autostart'
        Checked = True
        State = cbChecked
        TabOrder = 5
      end
      object CheckBox10: TCheckBox
        Left = 8
        Top = 64
        Width = 185
        Height = 17
        Caption = 'StabWin als Oberfl'#228'che einsetzen'
        Checked = True
        State = cbChecked
        TabOrder = 6
      end
      object CheckBox11: TCheckBox
        Left = 8
        Top = 176
        Width = 81
        Height = 17
        Caption = 'System-Tray'
        Checked = True
        State = cbChecked
        TabOrder = 7
      end
      object Button7: TButton
        Left = 160
        Top = 128
        Width = 129
        Height = 25
        Caption = 'StabWin deinstallieren'
        TabOrder = 8
        OnClick = Button7Click
      end
    end
    object TTabPage
      Left = 4
      Top = 24
      Caption = 'StabWin Datei-Manager'
      object Label4: TLabel
        Left = 8
        Top = 80
        Width = 180
        Height = 13
        Caption = 'Dateien von folgendem Typ anzeigen:'
      end
      object Label5: TLabel
        Left = 8
        Top = 136
        Width = 66
        Height = 13
        Caption = 'Dateiattribute:'
      end
      object Label9: TLabel
        Left = 8
        Top = 8
        Width = 116
        Height = 13
        Caption = 'Pfad zum Dateimanager:'
      end
      object Edit2: TEdit
        Left = 8
        Top = 104
        Width = 121
        Height = 21
        TabOrder = 0
        Text = '*.*'
      end
      object CheckBox6: TCheckBox
        Left = 8
        Top = 160
        Width = 57
        Height = 17
        Caption = 'Archiv'
        Checked = True
        State = cbChecked
        TabOrder = 1
      end
      object CheckBox7: TCheckBox
        Left = 8
        Top = 176
        Width = 57
        Height = 17
        Caption = 'System'
        Checked = True
        State = cbChecked
        TabOrder = 2
      end
      object CheckBox8: TCheckBox
        Left = 8
        Top = 192
        Width = 65
        Height = 17
        Caption = 'Versteckt'
        Checked = True
        State = cbChecked
        TabOrder = 3
      end
      object CheckBox9: TCheckBox
        Left = 8
        Top = 208
        Width = 105
        Height = 17
        Caption = 'Schreibgesch'#252'tzt'
        Checked = True
        State = cbChecked
        TabOrder = 4
      end
      object Edit3: TEdit
        Left = 8
        Top = 24
        Width = 241
        Height = 21
        TabOrder = 5
      end
      object Button10: TButton
        Left = 264
        Top = 24
        Width = 25
        Height = 25
        Caption = '...'
        TabOrder = 6
        OnClick = Button10Click
      end
      object Button11: TButton
        Left = 16
        Top = 48
        Width = 75
        Height = 25
        Caption = 'Zur'#252'cksetzen'
        TabOrder = 7
        OnClick = Button11Click
      end
    end
    object TTabPage
      Left = 4
      Top = 24
      Caption = 'Men'#252
      object Label6: TLabel
        Left = 8
        Top = 0
        Width = 39
        Height = 13
        Caption = 'Buttons:'
      end
      object Label7: TLabel
        Left = 8
        Top = 136
        Width = 224
        Height = 13
        Caption = 'Folgende Programme im Startmen'#252' verkn'#252'pfen:'
      end
      object Button4: TButton
        Left = 152
        Top = 72
        Width = 81
        Height = 25
        Caption = 'Eigenschaften'
        TabOrder = 0
        OnClick = Button4Click
      end
      object ListBox1: TListBox
        Left = 8
        Top = 16
        Width = 305
        Height = 49
        ItemHeight = 13
        TabOrder = 1
      end
      object Button5: TButton
        Left = 8
        Top = 72
        Width = 65
        Height = 25
        Caption = 'Hinzuf'#252'gen'
        TabOrder = 2
        OnClick = Button5Click
      end
      object Button6: TButton
        Left = 80
        Top = 72
        Width = 65
        Height = 25
        Caption = 'Entfernen'
        TabOrder = 3
        OnClick = Button6Click
      end
      object CheckBox4: TCheckBox
        Left = 16
        Top = 160
        Width = 225
        Height = 17
        Caption = 'Abmelden+Beenden (dringend empfohlen)'
        Checked = True
        State = cbChecked
        TabOrder = 4
      end
      object CheckBox12: TCheckBox
        Left = 16
        Top = 192
        Width = 153
        Height = 17
        Caption = 'Informationen '#252'ber StabWin'
        Checked = True
        State = cbChecked
        TabOrder = 5
      end
      object CheckBox14: TCheckBox
        Left = 16
        Top = 208
        Width = 129
        Height = 17
        Caption = 'StabWin Konfiguration'
        Checked = True
        State = cbChecked
        TabOrder = 6
      end
      object Button9: TButton
        Left = 8
        Top = 104
        Width = 185
        Height = 25
        Caption = 'Standard-Buttons wiederherstellen'
        TabOrder = 7
        OnClick = Button9Click
      end
      object CheckBox13: TCheckBox
        Left = 16
        Top = 176
        Width = 209
        Height = 17
        Caption = 'Benutzer wechseln / Computer sperren'
        Checked = True
        State = cbChecked
        TabOrder = 8
      end
    end
    object TTabPage
      Left = 4
      Top = 24
      Caption = 'Desktop'
      object Label8: TLabel
        Left = 8
        Top = 64
        Width = 223
        Height = 13
        Caption = 'Folgende Symbole auf dem Desktop einrichten:'
      end
      object Label1: TLabel
        Left = 8
        Top = 8
        Width = 43
        Height = 13
        Caption = 'Auswahl:'
      end
      object CheckBox16: TCheckBox
        Left = 24
        Top = 88
        Width = 81
        Height = 17
        Caption = 'Arbeitsplatz'
        Checked = True
        State = cbChecked
        TabOrder = 0
      end
      object CheckBox17: TCheckBox
        Left = 24
        Top = 104
        Width = 97
        Height = 17
        Caption = 'Eigene Dateien'
        Checked = True
        State = cbChecked
        TabOrder = 1
      end
      object CheckBox18: TCheckBox
        Left = 24
        Top = 120
        Width = 97
        Height = 17
        Caption = 'Internet Explorer'
        Checked = True
        State = cbChecked
        TabOrder = 2
      end
      object CheckBox19: TCheckBox
        Left = 24
        Top = 136
        Width = 121
        Height = 17
        Caption = 'Netzwerkumgebung'
        Checked = True
        State = cbChecked
        TabOrder = 3
      end
      object CheckBox20: TCheckBox
        Left = 24
        Top = 152
        Width = 73
        Height = 17
        Caption = 'Papierkorb'
        Checked = True
        State = cbChecked
        TabOrder = 4
      end
      object RadioButton1: TRadioButton
        Left = 8
        Top = 24
        Width = 65
        Height = 17
        Caption = 'Ein Klick'
        TabOrder = 5
      end
      object RadioButton2: TRadioButton
        Left = 8
        Top = 40
        Width = 81
        Height = 17
        Caption = 'Doppelklick'
        Checked = True
        TabOrder = 6
        TabStop = True
      end
    end
    object TTabPage
      Left = 4
      Top = 24
      Caption = 'Tray'
      object Label11: TLabel
        Left = 104
        Top = 8
        Width = 126
        Height = 13
        Caption = 'Position des System-Trays:'
      end
      object Label12: TLabel
        Left = 112
        Top = 40
        Width = 8
        Height = 13
        Caption = 'x:'
      end
      object Label13: TLabel
        Left = 112
        Top = 64
        Width = 8
        Height = 13
        Caption = 'y:'
      end
      object Label14: TLabel
        Left = 16
        Top = 104
        Width = 350
        Height = 13
        Caption = 
          'Hinweis: Der System-Tray wurde von Anish Mistry geschrieben. Meh' +
          'r dazu'
      end
      object Label15: TLabel
        Left = 16
        Top = 120
        Width = 219
        Height = 13
        Caption = 'in der beliegenden Datei traymodule-reade.txt .'
      end
      object Edit4: TEdit
        Left = 128
        Top = 32
        Width = 121
        Height = 21
        TabOrder = 0
        Text = '0'
      end
      object Edit5: TEdit
        Left = 128
        Top = 64
        Width = 121
        Height = 21
        TabOrder = 1
        Text = '0'
      end
    end
  end
  object Button1: TButton
    Left = 208
    Top = 288
    Width = 75
    Height = 25
    Caption = 'Fertigstellen'
    TabOrder = 1
    OnClick = Button1Click
  end
  object Button2: TButton
    Left = 296
    Top = 288
    Width = 75
    Height = 25
    Caption = 'Abbrechen'
    TabOrder = 2
    OnClick = Button2Click
  end
  object Memo1: TMemo
    Left = 8
    Top = 208
    Width = 0
    Height = 0
    TabOrder = 3
    Visible = False
  end
  object Button12: TButton
    Left = 32
    Top = 288
    Width = 75
    Height = 25
    Caption = 'Weiter'
    Default = True
    TabOrder = 4
    OnClick = Button12Click
  end
  object Button13: TButton
    Left = 120
    Top = 288
    Width = 75
    Height = 25
    Caption = 'Zur'#252'ck'
    Enabled = False
    TabOrder = 5
    OnClick = Button13Click
  end
  object Button8: TButton
    Left = 160
    Top = 320
    Width = 75
    Height = 25
    Caption = 'Hilfe'
    TabOrder = 6
    OnClick = Button8Click
  end
  object OpenDialog1: TOpenDialog
    Filter = 'Ausf'#252'hrbare Dateien (*.exe)|*.exe|Alle Dateien (*.*)|*.*'
    Left = 384
    Top = 320
  end
end
