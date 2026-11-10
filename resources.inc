
; fasmx - flat assembler IDE for Linux/X11
; menus, accelerators, dialog templates and texts

  IDM_NEW	      = 1101
  IDM_OPEN	      = 1102
  IDM_SAVE	      = 1103
  IDM_SAVEAS	      = 1104
  IDM_NEXT	      = 1105
  IDM_PREVIOUS	      = 1106
  IDM_OPENFOLDER      = 1107
  IDM_CLOSE	      = 1108
  IDM_EXIT	      = 1109
  IDM_SELECTFILE      = 1110
  IDM_UNDO	      = 1201
  IDM_REDO	      = 1202
  IDM_CUT	      = 1203
  IDM_COPY	      = 1204
  IDM_PASTE	      = 1205
  IDM_DELETE	      = 1206
  IDM_SELECTALL       = 1207
  IDM_VERTICAL	      = 1208
  IDM_DISCARD_UNDO    = 1209
  IDM_READONLY	      = 1210
  IDM_POSITION	      = 1301
  IDM_FIND	      = 1302
  IDM_FINDNEXT	      = 1303
  IDM_REPLACE	      = 1304
  IDM_RUN	      = 1401
  IDM_COMPILE	      = 1402
  IDM_DEBUG	      = 1403
  IDM_SYMBOLS	      = 1404
  IDM_ASSIGN	      = 1409
  IDM_APPEARANCE      = 1501
  IDM_COMPILERSETUP   = 1502
  IDM_SECURESEL       = 1505
  IDM_AUTOBRACKETS    = 1506
  IDM_AUTOINDENT      = 1507
  IDM_SMARTTABS       = 1508
  IDM_OPTIMALFILL     = 1509
  IDM_REVIVEDEADKEYS  = 1510
  IDM_TIMESCROLL      = 1511
  IDM_ONEINSTANCEONLY = 1512
  IDM_CALCULATOR      = 1904
  IDM_ABOUT	      = 1909

  ID_CHANGE	   = 2001
  ID_SELECT	   = 2101
  ID_CASESENSITIVE = 2102
  ID_WHOLEWORDS    = 2103
  ID_BACKWARD	   = 2104
  ID_INWHOLETEXT   = 2105
  ID_PROMPT	   = 2106
  ID_CONSOLECARET  = 2107
  ID_ROW	   = 2201
  ID_COLUMN	   = 2202
  ID_DISPLAY	   = 2203
  ID_INSTRUCTION   = 2204
  ID_EXPRESSION    = 2205
  ID_DECRESULT	   = 2206
  ID_HEXRESULT	   = 2207
  ID_BINRESULT	   = 2208
  ID_OCTRESULT	   = 2209
  ID_TEXT	   = 2301
  ID_NEWTEXT	   = 2302
  ID_SETTING	   = 2303
  ID_MEMORY	   = 2304
  ID_PRIORITY	   = 2305
  ID_PASSES	   = 2306
  ID_LINES	   = 2307
  ID_INCLUDEPATH   = 2308
  ID_ICON	   = 2401
  ID_MESSAGE	   = 2402
  ID_DECSELECT	   = 2501
  ID_HEXSELECT	   = 2502
  ID_BINSELECT	   = 2503
  ID_OCTSELECT	   = 2504
  ID_PROGRESS	   = 2801
  ID_PREVIEW	   = 2901
  ID_FD_DIRECTORY  = 3001
  ID_FD_FILES	   = 3002
  ID_FD_NAME	   = 3003
  ID_FD_FILTER	   = 3004
  ID_FD_READONLY   = 3005
  ID_FONTS	   = 3101
  ID_FONT_PREVIEW  = 3102
  ID_COLOR_GRID    = 3201
  ID_COLOR_RED	   = 3202
  ID_COLOR_GREEN   = 3203
  ID_COLOR_BLUE    = 3204
  ID_COLOR_SAMPLE  = 3205

  XK_F1  = 0FFBEh
  XK_F2  = 0FFBFh
  XK_F3  = 0FFC0h
  XK_F4  = 0FFC1h
  XK_F5  = 0FFC2h
  XK_F7  = 0FFC4h
  XK_F8  = 0FFC5h
  XK_F9  = 0FFC6h
  XK_F10 = 0FFC7h

; menus

menubar_items dd _menu_file,file_menu
	      dd _menu_edit,edit_menu
	      dd _menu_search,search_menu
	      dd _menu_run,run_menu
	      dd _menu_options,options_menu
	      dd _menu_help,help_menu
	      dd 0,0

menus_list dd file_menu,edit_menu,search_menu,run_menu,options_menu,help_menu,tab_menu,0

file_menu:
	menuitem '&New','Ctrl+N',IDM_NEW
	menuitem '&Open...','Ctrl+O',IDM_OPEN
	menuitem '&Save','Ctrl+S',IDM_SAVE
	menuitem 'Save &as...',,IDM_SAVEAS
	menuseparator
	menuitem 'E&xit','Alt+X',IDM_EXIT
	endmenu

edit_menu:
	menuitem '&Undo','Ctrl+Z',IDM_UNDO
	menuitem '&Redo','Ctrl+Shift+Z',IDM_REDO
	menuseparator
	menuitem 'Cu&t','Ctrl+X',IDM_CUT
	menuitem '&Copy','Ctrl+C',IDM_COPY
	menuitem '&Paste','Ctrl+V',IDM_PASTE
	menuitem '&Delete',,IDM_DELETE
	menuseparator
	menuitem 'Select &all','Ctrl+A',IDM_SELECTALL
	menuseparator
	menuitem '&Vertical selection','Alt+Ins',IDM_VERTICAL
	endmenu

search_menu:
	menuitem '&Position...','Ctrl+G',IDM_POSITION
	menuseparator
	menuitem '&Find...','Ctrl+F',IDM_FIND
	menuitem 'Find &next','F3',IDM_FINDNEXT
	menuitem '&Replace...','Ctrl+H',IDM_REPLACE
	endmenu

run_menu:
	menuitem '&Run','F9',IDM_RUN
	menuitem '&Compile','Ctrl+F9',IDM_COMPILE
	menuseparator
	menuitem 'Build &symbols','Ctrl+F8',IDM_SYMBOLS
	endmenu

options_menu:
	menuitem '&Appearance...',,IDM_APPEARANCE
	menuitem '&Compiler setup...',,IDM_COMPILERSETUP
	menuseparator
	menuitem '&Secure selection',,IDM_SECURESEL
	menuitem 'Automatic &brackets',,IDM_AUTOBRACKETS
	menuitem 'Automatic &indents',,IDM_AUTOINDENT
	menuitem 'Smart &tabulation',,IDM_SMARTTABS
	menuitem '&Optimal fill on saving',,IDM_OPTIMALFILL
	menuitem '&Revive dead keys',,IDM_REVIVEDEADKEYS
	menuitem 'Ti&me scrolling',,IDM_TIMESCROLL
	menuitem 'O&ne instance only',,IDM_ONEINSTANCEONLY
	endmenu

help_menu:
	menuitem 'Ca&lculator...','Ctrl+F6',IDM_CALCULATOR
	menuseparator
	menuitem '&About...',,IDM_ABOUT
	endmenu

tab_menu:
	menuitem '&Assign to compiler','Shift+F9',IDM_ASSIGN
	menuitem 'Open &folder',,IDM_OPENFOLDER
	menuitem '&Read-only mode','Shift+F6',IDM_READONLY
	menuseparator
	menuitem '&Close','Esc',IDM_CLOSE
	endmenu

	menu_texts

; accelerators: key (lower case letter or keysym), modifiers, command

macro accelerator key,flags,command
{ dd key
  db flags,0,0,0
  dd command }

accelerators:
	accelerator 'n',KS_CONTROL,IDM_NEW
	accelerator 'o',KS_CONTROL,IDM_OPEN
	accelerator 's',KS_CONTROL,IDM_SAVE
	accelerator 'z',KS_CONTROL,IDM_UNDO
	accelerator 'z',KS_CONTROL+KS_SHIFT,IDM_REDO
	accelerator 'x',KS_CONTROL,IDM_CUT
	accelerator 'c',KS_CONTROL,IDM_COPY
	accelerator 'v',KS_CONTROL,IDM_PASTE
	accelerator 'a',KS_CONTROL,IDM_SELECTALL
	accelerator 'g',KS_CONTROL,IDM_POSITION
	accelerator 'f',KS_CONTROL,IDM_FIND
	accelerator 'h',KS_CONTROL,IDM_REPLACE
	accelerator XK_Tab,KS_CONTROL,IDM_NEXT
	accelerator XK_ISO_Left_Tab,KS_CONTROL+KS_SHIFT,IDM_PREVIOUS
	accelerator XK_Tab,KS_CONTROL+KS_SHIFT,IDM_PREVIOUS
	accelerator XK_F2,0,IDM_SAVE
	accelerator XK_F2,KS_SHIFT,IDM_SAVEAS
	accelerator XK_F4,0,IDM_OPEN
	accelerator XK_F3,0,IDM_FINDNEXT
	accelerator XK_F5,0,IDM_POSITION
	accelerator XK_F6,KS_SHIFT,IDM_READONLY
	accelerator XK_F7,0,IDM_FIND
	accelerator XK_F7,KS_SHIFT,IDM_FINDNEXT
	accelerator XK_F7,KS_CONTROL,IDM_REPLACE
	accelerator XK_F9,0,IDM_RUN
	accelerator XK_F9,KS_CONTROL,IDM_COMPILE
	accelerator XK_F8,KS_CONTROL,IDM_SYMBOLS
	accelerator XK_F9,KS_SHIFT,IDM_ASSIGN
	accelerator XK_F6,KS_CONTROL,IDM_CALCULATOR
	accelerator XK_Escape,0,IDM_CLOSE
	accelerator XK_Delete,KS_ALT,IDM_DISCARD_UNDO
	accelerator 'x',KS_ALT,IDM_EXIT
	accelerator '1',KS_ALT,IDM_SELECTFILE+1
	accelerator '2',KS_ALT,IDM_SELECTFILE+2
	accelerator '3',KS_ALT,IDM_SELECTFILE+3
	accelerator '4',KS_ALT,IDM_SELECTFILE+4
	accelerator '5',KS_ALT,IDM_SELECTFILE+5
	accelerator '6',KS_ALT,IDM_SELECTFILE+6
	accelerator '7',KS_ALT,IDM_SELECTFILE+7
	accelerator '8',KS_ALT,IDM_SELECTFILE+8
	accelerator '9',KS_ALT,IDM_SELECTFILE+9
	dd 0

; dialog templates (coordinates in dialog units)

  dialog position_dialog,'Position',40,40,126,54,0
    dialogitem 'STATIC','&Row:',-1,4,8,28,8,SS_RIGHT
    dialogitem 'EDIT','',ID_ROW,36,6,34,12,WS_BORDER+WS_TABSTOP+ES_NUMBER
    dialogitem 'STATIC','&Column:',-1,4,26,28,8,SS_RIGHT
    dialogitem 'EDIT','',ID_COLUMN,36,24,34,12,WS_BORDER+WS_TABSTOP+ES_NUMBER
    dialogitem 'BUTTON','&Select',ID_SELECT,36,42,48,8,WS_TABSTOP+BS_AUTOCHECKBOX
    dialogitem 'BUTTON','OK',IDOK,78,6,42,14,WS_TABSTOP+BS_DEFPUSHBUTTON
    dialogitem 'BUTTON','C&ancel',IDCANCEL,78,22,42,14,WS_TABSTOP
  enddialog

  dialog find_dialog,'Find',60,60,254,54,0
    dialogitem 'STATIC','&Text to find:',-1,4,8,40,8,SS_RIGHT
    dialogitem 'COMBOBOX','',ID_TEXT,48,6,150,13,WS_TABSTOP+CBS_DROPDOWN
    dialogitem 'BUTTON','&Case sensitive',ID_CASESENSITIVE,48,24,70,8,WS_TABSTOP+BS_AUTOCHECKBOX
    dialogitem 'BUTTON','&Whole words',ID_WHOLEWORDS,48,38,70,8,WS_TABSTOP+BS_AUTOCHECKBOX
    dialogitem 'BUTTON','&Backward',ID_BACKWARD,124,24,70,8,WS_TABSTOP+BS_AUTOCHECKBOX
    dialogitem 'BUTTON','&In whole text',ID_INWHOLETEXT,124,38,70,8,WS_TABSTOP+BS_AUTOCHECKBOX
    dialogitem 'BUTTON','&Find first',IDOK,206,6,42,14,WS_TABSTOP+BS_DEFPUSHBUTTON
    dialogitem 'BUTTON','C&ancel',IDCANCEL,206,22,42,14,WS_TABSTOP
  enddialog

  dialog replace_dialog,'Replace',60,60,254,86,0
    dialogitem 'STATIC','&Text to find:',-1,4,8,40,8,SS_RIGHT
    dialogitem 'COMBOBOX','',ID_TEXT,48,6,150,13,WS_TABSTOP+CBS_DROPDOWN
    dialogitem 'STATIC','&New text:',-1,4,26,40,8,SS_RIGHT
    dialogitem 'COMBOBOX','',ID_NEWTEXT,48,24,150,13,WS_TABSTOP+CBS_DROPDOWN
    dialogitem 'BUTTON','&Case sensitive',ID_CASESENSITIVE,48,42,70,8,WS_TABSTOP+BS_AUTOCHECKBOX
    dialogitem 'BUTTON','&Whole words',ID_WHOLEWORDS,48,56,70,8,WS_TABSTOP+BS_AUTOCHECKBOX
    dialogitem 'BUTTON','&Backward',ID_BACKWARD,124,42,70,8,WS_TABSTOP+BS_AUTOCHECKBOX
    dialogitem 'BUTTON','&In whole text',ID_INWHOLETEXT,124,56,70,8,WS_TABSTOP+BS_AUTOCHECKBOX
    dialogitem 'BUTTON','&Prompt on replace',ID_PROMPT,48,70,90,8,WS_TABSTOP+BS_AUTOCHECKBOX
    dialogitem 'BUTTON','&Replace',IDOK,206,6,42,14,WS_TABSTOP+BS_DEFPUSHBUTTON
    dialogitem 'BUTTON','C&ancel',IDCANCEL,206,22,42,14,WS_TABSTOP
  enddialog

  dialog compile_dialog,'Compile',64,64,192,42,0
    dialogitem 'PROGRESS','',ID_PROGRESS,8,6,176,12,0
    dialogitem 'BUTTON','C&ancel',IDCANCEL,75,24,42,14,WS_TABSTOP
  enddialog

  dialog summary_dialog,'Compile',48,24,192,130,0
    dialogitem 'BUTTON','OK',IDCANCEL,142,108,42,14,WS_TABSTOP+BS_DEFPUSHBUTTON
    dialogitem 'STATIC','',ID_ICON,8,4,14,20,SS_ICON+ICON_INFORMATION shl 24
    dialogitem 'STATIC','',ID_MESSAGE,36,10,148,8,0
    dialogitem 'STATIC','&Display:',-1,8,28,176,8,0
    dialogitem 'EDIT','',ID_DISPLAY,8,40,176,64,WS_BORDER+WS_TABSTOP+ES_MULTILINE+ES_READONLY
  enddialog

  dialog error_summary_dialog,'Compile',48,24,192,144,0
    dialogitem 'STATIC','&Source:',-1,8,96,176,8,0
    dialogitem 'LISTBOX','',ID_LINES,8,108,126,32,WS_BORDER+WS_TABSTOP
    dialogitem 'BUTTON','OK',IDCANCEL,142,108,42,14,WS_TABSTOP+BS_DEFPUSHBUTTON
    dialogitem 'STATIC','',ID_ICON,8,4,14,20,SS_ICON+ICON_ERROR shl 24
    dialogitem 'STATIC','',ID_MESSAGE,36,10,148,8,0
    dialogitem 'STATIC','&Display:',-1,8,28,176,8,0
    dialogitem 'EDIT','',ID_DISPLAY,8,40,176,24,WS_BORDER+WS_TABSTOP+ES_MULTILINE+ES_READONLY
    dialogitem 'STATIC','&Instruction:',-1,8,68,176,8,0
    dialogitem 'EDIT','',ID_INSTRUCTION,8,80,176,12,WS_BORDER+WS_TABSTOP+ES_READONLY
  enddialog

  dialog appearance_dialog,'Appearance',50,20,186,166,0
    dialogitem 'COMBOBOX','',ID_SETTING,8,6,120,13,WS_TABSTOP+CBS_DROPDOWNLIST
    dialogitem 'BUTTON','C&hange...',ID_CHANGE,134,6,42,13,WS_TABSTOP
    dialogitem 'FEDIT','',ID_PREVIEW,8,24,168,120,WS_DISABLED
    dialogitem 'BUTTON','&Console caret',ID_CONSOLECARET,8,151,80,8,WS_TABSTOP+BS_AUTOCHECKBOX
    dialogitem 'BUTTON','OK',IDOK,88,148,42,14,WS_TABSTOP+BS_DEFPUSHBUTTON
    dialogitem 'BUTTON','C&ancel',IDCANCEL,134,148,42,14,WS_TABSTOP
  enddialog

  dialog compiler_setup_dialog,'Compiler setup',54,28,200,82,0
    dialogitem 'STATIC','&Include path(s):',-1,8,4,184,8,0
    dialogitem 'COMBOBOX','',ID_INCLUDEPATH,8,14,184,13,WS_TABSTOP+CBS_DROPDOWN
    dialogitem 'STATIC','&Memory (KB):',-1,8,30,42,8,0
    dialogitem 'COMBOBOX','',ID_MEMORY,8,40,54,13,WS_TABSTOP+ES_NUMBER+CBS_DROPDOWN
    dialogitem 'STATIC','&Passes limit:',-1,72,30,60,8,0
    dialogitem 'EDIT','',ID_PASSES,72,40,54,12,WS_BORDER+WS_TABSTOP+ES_NUMBER
    dialogitem 'STATIC','&Thread priority:',-1,136,30,60,8,0
    dialogitem 'COMBOBOX','',ID_PRIORITY,136,40,56,13,WS_TABSTOP+CBS_DROPDOWNLIST
    dialogitem 'BUTTON','OK',IDOK,104,64,42,14,WS_TABSTOP+BS_DEFPUSHBUTTON
    dialogitem 'BUTTON','C&ancel',IDCANCEL,150,64,42,14,WS_TABSTOP
  enddialog

  dialog calculator_dialog,'Calculator',48,32,380,80,0
    dialogitem 'STATIC','&Expression:',-1,4,8,44,8,SS_RIGHT
    dialogitem 'EDIT','',ID_EXPRESSION,52,6,274,12,WS_BORDER+WS_TABSTOP
    dialogitem 'BUTTON','&Decimal:',ID_DECSELECT,4,20,56,12,WS_TABSTOP+BS_AUTORADIOBUTTON+BS_LEFTTEXT
    dialogitem 'EDIT','',ID_DECRESULT,62,22,264,10,ES_READONLY
    dialogitem 'BUTTON','&Hexadecimal:',ID_HEXSELECT,4,34,56,12,WS_TABSTOP+BS_AUTORADIOBUTTON+BS_LEFTTEXT
    dialogitem 'EDIT','',ID_HEXRESULT,62,36,264,10,ES_READONLY
    dialogitem 'BUTTON','&Binary:',ID_BINSELECT,4,48,56,12,WS_TABSTOP+BS_AUTORADIOBUTTON+BS_LEFTTEXT
    dialogitem 'EDIT','',ID_BINRESULT,62,50,264,10,ES_READONLY
    dialogitem 'BUTTON','&Octal:',ID_OCTSELECT,4,62,56,12,WS_TABSTOP+BS_AUTORADIOBUTTON+BS_LEFTTEXT
    dialogitem 'EDIT','',ID_OCTRESULT,62,64,264,10,ES_READONLY
    dialogitem 'BUTTON','E&valuate',IDOK,332,6,42,14,WS_TABSTOP+BS_DEFPUSHBUTTON
    dialogitem 'BUTTON','&Close',IDCANCEL,332,22,42,14,WS_TABSTOP
  enddialog

  dialog about_dialog,'About',40,40,172,60,0
    dialogitem 'STATIC',<'fasmx - assembler IDE for X11',0Ah,'Copyright ',0C2h,0A9h,' 1999-2024 Tomasz Grysztar.'>,-1,27,10,144,24,SS_CENTER
    dialogitem 'STATIC','',-1,6,6,14,20,SS_ICON+ICON_INFORMATION shl 24
    dialogitem 'STATIC','',-1,4,30,164,6,SS_ETCHEDHORZ
    dialogitem 'STATIC',<'flat assembler version ',VERSION_STRING>,-1,4,38,100,9,0
    dialogitem 'STATIC',<'flat editor version ',FEDIT_VERSION_STRING>,-1,4,48,100,9,0
    dialogitem 'BUTTON','OK',IDOK,124,40,42,14,WS_TABSTOP+BS_DEFPUSHBUTTON
  enddialog

  dialog open_dialog,'Open',0,0,300,196,0
    dialogitem 'STATIC','Look &in:',-1,6,8,40,8,0
    dialogitem 'EDIT','',ID_FD_DIRECTORY,48,6,246,12,WS_BORDER+WS_TABSTOP
    dialogitem 'LISTBOX','',ID_FD_FILES,6,22,288,120,WS_BORDER+WS_TABSTOP+LBS_EXTENDEDSEL
    dialogitem 'STATIC','File &name:',-1,6,148,40,8,0
    dialogitem 'EDIT','',ID_FD_NAME,48,146,190,12,WS_BORDER+WS_TABSTOP
    dialogitem 'STATIC','Files of &type:',-1,6,164,40,8,0
    dialogitem 'COMBOBOX','',ID_FD_FILTER,48,162,190,13,WS_TABSTOP+CBS_DROPDOWNLIST
    dialogitem 'BUTTON','Open as &read-only',ID_FD_READONLY,48,180,120,10,WS_TABSTOP+BS_AUTOCHECKBOX
    dialogitem 'BUTTON','&Open',IDOK,244,146,50,14,WS_TABSTOP+BS_DEFPUSHBUTTON
    dialogitem 'BUTTON','Cancel',IDCANCEL,244,162,50,14,WS_TABSTOP
  enddialog

  dialog save_dialog,'Save as',0,0,300,182,0
    dialogitem 'STATIC','Save &in:',-1,6,8,40,8,0
    dialogitem 'EDIT','',ID_FD_DIRECTORY,48,6,246,12,WS_BORDER+WS_TABSTOP
    dialogitem 'LISTBOX','',ID_FD_FILES,6,22,288,120,WS_BORDER+WS_TABSTOP
    dialogitem 'STATIC','File &name:',-1,6,148,40,8,0
    dialogitem 'EDIT','',ID_FD_NAME,48,146,190,12,WS_BORDER+WS_TABSTOP
    dialogitem 'STATIC','Files of &type:',-1,6,164,40,8,0
    dialogitem 'COMBOBOX','',ID_FD_FILTER,48,162,190,13,WS_TABSTOP+CBS_DROPDOWNLIST
    dialogitem 'BUTTON','&Save',IDOK,244,146,50,14,WS_TABSTOP+BS_DEFPUSHBUTTON
    dialogitem 'BUTTON','Cancel',IDCANCEL,244,162,50,14,WS_TABSTOP
  enddialog

  dialog font_dialog,'Font',0,0,260,170,0
    dialogitem 'STATIC','&Font:',-1,6,6,100,8,0
    dialogitem 'LISTBOX','',ID_FONTS,6,16,248,100,WS_BORDER+WS_TABSTOP
    dialogitem 'STATIC','Sample:',-1,6,120,100,8,0
    dialogitem 'FONTPREVIEW','',ID_FONT_PREVIEW,6,130,248,20,0
    dialogitem 'BUTTON','OK',IDOK,150,154,50,14,WS_TABSTOP+BS_DEFPUSHBUTTON
    dialogitem 'BUTTON','Cancel',IDCANCEL,204,154,50,14,WS_TABSTOP
  enddialog

  dialog color_dialog,'Color',0,0,200,150,0
    dialogitem 'COLORGRID','',ID_COLOR_GRID,6,6,188,72,WS_TABSTOP
    dialogitem 'STATIC','&Red:',-1,6,86,30,8,0
    dialogitem 'EDIT','',ID_COLOR_RED,40,84,30,12,WS_BORDER+WS_TABSTOP+ES_NUMBER
    dialogitem 'STATIC','&Green:',-1,6,102,30,8,0
    dialogitem 'EDIT','',ID_COLOR_GREEN,40,100,30,12,WS_BORDER+WS_TABSTOP+ES_NUMBER
    dialogitem 'STATIC','&Blue:',-1,6,118,30,8,0
    dialogitem 'EDIT','',ID_COLOR_BLUE,40,116,30,12,WS_BORDER+WS_TABSTOP+ES_NUMBER
    dialogitem 'COLORSAMPLE','',ID_COLOR_SAMPLE,90,84,104,44,0
    dialogitem 'BUTTON','OK',IDOK,90,132,50,14,WS_TABSTOP+BS_DEFPUSHBUTTON
    dialogitem 'BUTTON','Cancel',IDCANCEL,144,132,50,14,WS_TABSTOP
  enddialog

; texts

_caption db 'fasmx - flat assembler ',VERSION_STRING,0
_caption_length = $ - _caption - 1
_wm_class db 'fasmx',0,'Fasmx',0
_wm_class_length = $ - _wm_class

_menu_file db '&File',0
_menu_edit db '&Edit',0
_menu_search db '&Search',0
_menu_run db '&Run',0
_menu_options db '&Options',0
_menu_help db '&Help',0

_memory_error db 'Not enough memory to complete this operation.',0
_loading_error db 'Could not load file ',0
_run_object_error db 'Cannot execute this output format.',0
_save_destination_question db 'The destination has unsaved edits.',0Ah,'Yes: save those edits to a new file before replacing it.',0Ah,'No: discard those edits and replace it.',0Ah,'Cancel: keep both tabs and stop.',0
_copy_needs_new_file db 'Choose a new filename to preserve the destination edits.',0
_copy_path_in_use db 'Choose a different path that is not open in another tab.',0
_saving_error db 'Could not save file ',0
_saving_question db 'File was modified. Save it now?',0
_text_prefix db 'Text ',0
_not_found db ' not found.',0
_not_found_after db ' not found after current position.',0
_not_found_before db ' not found before current position.',0
_replace_prompt db 'Replace this occurence?',0
_replaces_made db ' replaces made.',0
_untitled db 'Untitled',0
_passes_suffix db ' passes, ',0
_seconds_suffix db ' seconds, ',0
_bytes_suffix db ' bytes.',0
_error_message_prefix db 'Error: ',0
_modified_status db 'Modified',0
_readonly_status db 'Read-only',0
_find db 'Find',0
_replace db 'Replace',0
_file_missing_message db 'File not found:',0Ah,0
_overwrite_question db ' already exists.',0Ah,'Do you want to replace it?',0
_compile_failed db 'Cannot start the compiler.',0
_null db 0
_empty_string db 0

_appearance_settings db 'Font',0
		     db 'Text color',0
		     db 'Background color',0
		     db 'Selection text color',0
		     db 'Selection background color',0
		     db 'Symbols color',0
		     db 'Numbers color',0
		     db 'Strings color',0
		     db 'Comments color',0
		     db 0
_memory_settings db '1024',0
		 db '2048',0
		 db '4096',0
		 db '8192',0
		 db '16384',0
		 db '32768',0
		 db '65536',0
		 db '131072',0
		 db '262144',0
		 db '524288',0
		 db 0
_priority_settings db 'Idle',0
		   db 'Low',0
		   db 'Normal',0
		   db 'High',0
		   db 'Realtime',0
		   db 0
_file_filters db 'Assembly files (*.asm;*.inc;*.ash)',0
	      db 'All files',0
	      db 0

preview_text db 0Ah
	     db ' org 100h',0Ah
	     db 0Ah
	     db ' mov ah,09h ',' ; write',0Ah
	     db ' mov dx,text',0Ah
	     db ' int 21h',0Ah
	     db ' int 20h',0Ah
	     db 0Ah
	     db ' text db "Hello!",24h',0Ah
	     db 0
align 4
preview_selection dd 1,5,1,6

_icon_information db 'i',0
_icon_question db '?',0
_icon_error db 'X',0
_icon_warning db '!',0

_button_ok db 'OK',0
_button_cancel db 'Cancel',0
_button_yes db '&Yes',0
_button_no db '&No',0

align 4
message_button_sets dd ok_buttons,ok_cancel_buttons,0,yes_no_cancel_buttons,yes_no_buttons
ok_buttons dd IDOK,_button_ok
ok_cancel_buttons dd IDOK,_button_ok,IDCANCEL,_button_cancel
yes_no_cancel_buttons dd IDYES,_button_yes,IDNO,_button_no,IDCANCEL,_button_cancel
yes_no_buttons dd IDYES,_button_yes,IDNO,_button_no

_ui_font db '-misc-fixed-medium-r-normal--13-*-*-*-c-70-iso10646-1',0
_default_font db '-misc-fixed-medium-r-normal--15-*-*-*-c-90-iso10646-1',0
_fallback_font db 'fixed',0
_cursor_font db 'cursor',0

_font_patterns db '-*-*-*-*-*-*-*-*-*-*-c-*-iso10646-1',0
	       db '-*-*-*-*-*-*-*-*-*-*-m-*-iso10646-1',0
	       db 0
_font_sample db 'AaBbCcXxYyZz 0123456789 ',0C3h,0A4h,0C3h,0B6h,0C3h,0BCh,' ',0D0h,0B0h,0D0h,0B1h,0D0h,0B2h,0

_xdg_open db '/usr/bin/xdg-open',0
_shell db '/bin/sh',0
_shell_name db 'sh',0
_shell_option db '-c',0
_run_outer_script db 'T=x-terminal-emulator; command -v $T >/dev/null 2>&1 || T=xterm; exec $T -e /bin/sh -c "$1" fasmx-run "$2"',0
_run_inner_script db '"$1"; s=$?; echo; printf "Process exited with code %d. Press Enter to close." $s; read x',0
_run_name db 'fasmx',0

_display_variable db 'DISPLAY',0
_xauthority_variable db 'XAUTHORITY',0
_home_variable db 'HOME',0
_config_home_variable db 'XDG_CONFIG_HOME',0
_include_variable db 'INCLUDE',0
_xauthority_file db '/.Xauthority',0
_config_directory db '/.config',0
_ini_file_name db '/fasmx.ini',0
_include_directory_name db '/include',0
_self_executable db '/proc/self/exe',0
_x11_socket_path db '/tmp/.X11-unix/X',0
_mit_magic_cookie db 'MIT-MAGIC-COOKIE-1'
_uri_file_prefix db 'file://',0

_error_prefix db 'fasmx: ',0
_error_no_display db 'DISPLAY variable is not set',0
_error_bad_display db 'invalid value of DISPLAY variable',0
_error_remote_display db 'only local X displays are supported',0
_error_connect db 'cannot connect to X server',0
_error_setup db 'X server refused the connection',0
_error_memory db 'not enough memory',0
_error_font db 'cannot open font',0
_error_connection_lost db 'fasmx: connection to X server lost',0Ah,0
_error_x11_code db 'fasmx: X error ',0
_error_x11_request db ' in request ',0
_new_line db 0Ah,0

atom_names db 'WM_PROTOCOLS',0
	   db 'WM_DELETE_WINDOW',0
	   db '_NET_WM_NAME',0
	   db 'UTF8_STRING',0
	   db '_NET_WM_WINDOW_TYPE',0
	   db '_NET_WM_WINDOW_TYPE_DIALOG',0
	   db '_NET_WM_STATE',0
	   db '_NET_WM_STATE_MAXIMIZED_VERT',0
	   db '_NET_WM_STATE_MAXIMIZED_HORZ',0
	   db '_NET_ACTIVE_WINDOW',0
	   db 'CLIPBOARD',0
	   db 'TARGETS',0
	   db 'TEXT',0
	   db 'INCR',0
	   db 'FASMX_SELECTION',0
	   db 'XdndAware',0
	   db 'XdndEnter',0
	   db 'XdndPosition',0
	   db 'XdndStatus',0
	   db 'XdndLeave',0
	   db 'XdndDrop',0
	   db 'XdndFinished',0
	   db 'XdndSelection',0
	   db 'XdndActionCopy',0
	   db 'XdndTypeList',0
	   db 'text/uri-list',0
	   db 'FASMX_DROP',0
	   db '_FASMX_INSTANCE',0
	   db '_FASMX_COMMAND',0
	   db 0

align 4

asm_syntax_colors_default dd 3030F0h,009000h,0B00000h,808080h

basic_colors dd 0FF8080h,0FFFF80h,080FF80h,000FF80h,080FFFFh,00080FFh,0FF80C0h,0FF80FFh
	     dd 0FF0000h,0FFFF00h,080FF00h,000FF40h,000FFFFh,00080C0h,08080C0h,0FF00FFh
	     dd 0804040h,0FF8040h,000FF00h,0008080h,0004080h,08080FFh,0800040h,0FF0080h
	     dd 0800000h,0FF8000h,0008000h,0008040h,00000FFh,00000A0h,0800080h,08000FFh
	     dd 0400000h,0804000h,0004000h,0004040h,0000080h,0000040h,0400040h,0400080h
	     dd 0000000h,0808000h,0808040h,0808080h,0408080h,0C0C0C0h,0FFFFFFh,0F0F0F0h

ui_colors dd 0D4D0C8h	; face
	  dd 0FFFFFFh	; light
	  dd 0808080h	; shadow
	  dd 0404040h	; dark
	  dd 0000000h	; text
	  dd 0808080h	; gray text
	  dd 0FFFFFFh	; window
	  dd 00078D7h	; selection
	  dd 0FFFFFFh	; selection text
	  dd 0E8E6E2h	; trough
	  dd 0D02020h	; red
	  dd 02060D0h	; blue
	  dd 0E0B000h	; yellow
	  dd 0FFFFFFh	; white
UI_COLORS_COUNT = ($ - ui_colors) / 4

control_procedures dd static_proc,button_proc,checkbox_proc,radio_proc,edit_proc
		   dd listbox_proc,combobox_proc,progress_proc,etched_proc,icon_proc
		   dd scrollbar_proc,fedit_control_proc,static_proc,font_preview_proc
		   dd color_grid_proc,color_sample_proc

wm_hints dd 1+2,1,1,0,0,0,0,0,0
xdnd_version dd 5
fedit_style dd FES_AUTOINDENT+FES_SMARTTABS+FES_CONSOLECARET
wheel_scroll_lines dd 3
gc_foreground dd -1
gc_background dd -1
editor_colors dd 0000000h,0FFFFFFh,0FFFFFFh,00078D7h
asm_syntax_colors dd 3030F0h,009000h,0B00000h,808080h
compiler_memory dd 65536
compiler_priority dd 0
viewer_arguments dd _xdg_open,0,0

_section_environment db 'Environment',0
_key_environment_include db 'Include',0
_section_compiler db 'Compiler',0
_key_compiler_memory db 'Memory',0
_key_compiler_priority db 'Priority',0
_key_compiler_passes db 'Passes',0
_section_options db 'Options',0
_key_options_securesel db 'SecureSelection',0
_key_options_autobrackets db 'AutoBrackets',0
_key_options_autoindent db 'AutoIndent',0
_key_options_smarttabs db 'SmartTabs',0
_key_options_optimalfill db 'OptimalFill',0
_key_options_revivedeadkeys db 'ReviveDeadKeys',0
_key_options_consolecaret db 'ConsoleCaret',0
_key_options_timescroll db 'TimeScroll',0
_key_options_oneinstanceonly db 'OneInstanceOnly',0
_section_colors db 'Colors',0
_key_color_text db 'Text',0
_key_color_background db 'Background',0
_key_color_seltext db 'SelectionText',0
_key_color_selbackground db 'SelectionBackground',0
_key_color_symbols db 'Symbols',0
_key_color_numbers db 'Numbers',0
_key_color_strings db 'Strings',0
_key_color_comments db 'Comments',0
_section_font db 'Font',0
_key_font_name db 'Name',0
_section_window db 'Window',0
_key_window_top db 'Top',0
_key_window_left db 'Left',0
_key_window_right db 'Right',0
_key_window_bottom db 'Bottom',0
_key_window_maximized db 'Maximized',0

align 4

option_settings dd _key_options_securesel,FES_SECURESEL
		dd _key_options_autobrackets,FES_AUTOBRACKETS
		dd _key_options_autoindent,FES_AUTOINDENT
		dd _key_options_smarttabs,FES_SMARTTABS
		dd _key_options_optimalfill,FES_OPTIMALFILL
		dd _key_options_revivedeadkeys,FES_REVIVEDEADKEYS
		dd _key_options_consolecaret,FES_CONSOLECARET
		dd _key_options_timescroll,FES_TIMESCROLL
		dd 0

color_settings dd _key_color_text,editor_colors
	       dd _key_color_background,editor_colors+4
	       dd _key_color_seltext,editor_colors+8
	       dd _key_color_selbackground,editor_colors+12
	       dd _key_color_symbols,asm_syntax_colors
	       dd _key_color_numbers,asm_syntax_colors+4
	       dd _key_color_strings,asm_syntax_colors+8
	       dd _key_color_comments,asm_syntax_colors+12
	       dd 0

priority_values dd -15,-2,0,2,15
nice_values dd 19,10,0,-10,-20

align 2

latin2_keysyms dw 0104h,02D8h,0141h,00A4h,013Dh,015Ah,00A7h,00A8h,0160h,015Eh,0164h,0179h,00ADh,017Dh,017Bh
	       dw 00B0h,0105h,02DBh,0142h,00B4h,013Eh,015Bh,02C7h,00B8h,0161h,015Fh,0165h,017Ah,02DDh,017Eh,017Ch
	       dw 0154h,00C1h,00C2h,0102h,00C4h,0139h,0106h,00C7h,010Ch,00C9h,0118h,00CBh,011Ah,00CDh,00CEh,010Eh
	       dw 0110h,0143h,0147h,00D3h,00D4h,0150h,00D6h,00D7h,0158h,016Eh,00DAh,0170h,00DCh,00DDh,0162h,00DFh
	       dw 0155h,00E1h,00E2h,0103h,00E4h,013Ah,0107h,00E7h,010Dh,00E9h,0119h,00EBh,011Bh,00EDh,00EEh,010Fh
	       dw 0111h,0144h,0148h,00F3h,00F4h,0151h,00F6h,00F7h,0159h,016Fh,00FAh,0171h,00FCh,00FDh,0163h,02D9h

cyrillic_keysyms dw 0452h,0453h,0451h,0454h,0455h,0456h,0457h,0458h,0459h,045Ah,045Bh,045Ch,0491h,045Eh,045Fh
		 dw 2116h,0402h,0403h,0401h,0404h,0405h,0406h,0407h,0408h,0409h,040Ah,040Bh,040Ch,0490h,040Eh,040Fh
		 dw 044Eh,0430h,0431h,0446h,0434h,0435h,0444h,0433h,0445h,0438h,0439h,043Ah,043Bh,043Ch,043Dh,043Eh
		 dw 043Fh,044Fh,0440h,0441h,0442h,0443h,0436h,0432h,044Ch,044Bh,0437h,0448h,044Dh,0449h,0447h,044Ah
		 dw 042Eh,0410h,0411h,0426h,0414h,0415h,0424h,0413h,0425h,0418h,0419h,041Ah,041Bh,041Ch,041Dh,041Eh
		 dw 041Fh,042Fh,0420h,0421h,0422h,0423h,0416h,0412h,042Ch,042Bh,0417h,0428h,042Dh,0429h,0427h,042Ah

dead_keysyms dw 0060h,00B4h,005Eh,007Eh,00AFh,02D8h,02D9h,00A8h,02DAh,02DDh,02C7h,00B8h,02DBh

align 4
