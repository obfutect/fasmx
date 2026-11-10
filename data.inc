
; fasmx - flat assembler IDE for Linux/X11
; variables

align 4

compile_pipe dd -1
run_arguments dd _shell_name,_shell_option,_run_outer_script,_run_name,_run_inner_script,run_path,0

; uninitialized data

align 4

atom_values:
atom_wm_protocols dd ?
atom_wm_delete_window dd ?
atom_net_wm_name dd ?
atom_utf8_string dd ?
atom_net_wm_window_type dd ?
atom_net_wm_window_type_dialog dd ?
atom_net_wm_state dd ?
atom_net_wm_state_maximized_vert dd ?
atom_net_wm_state_maximized_horz dd ?
atom_net_active_window dd ?
atom_clipboard dd ?
atom_targets dd ?
atom_text dd ?
atom_incr dd ?
atom_fasmx_selection dd ?
atom_xdnd_aware dd ?
atom_xdnd_enter dd ?
atom_xdnd_position dd ?
atom_xdnd_status dd ?
atom_xdnd_leave dd ?
atom_xdnd_drop dd ?
atom_xdnd_finished dd ?
atom_xdnd_selection dd ?
atom_xdnd_action_copy dd ?
atom_xdnd_type_list dd ?
atom_uri_list dd ?
atom_fasmx_drop dd ?
atom_fasmx_instance dd ?
atom_fasmx_command dd ?

; system

argument_count dd ?
arguments dd ?
environment dd ?
timespec rd 2
number_buffer rb 12
load_file_buffer dd ?
load_file_size dd ?
socketcall_arguments rd 3
full_path_output dd ?
stat_buffer rb 100
format_signed db ?

align 4

; X11

x11_socket dd ?
x11_sequence dd ?
x11_out_length dd ?
x11_in_start dd ?
x11_in_end dd ?
x11_queue_head dd ?
x11_queue_count dd ?
x11_display_number dd ?
x11_display_digits dd ?
x11_display_digits_length dd ?
x11_screen_number dd ?
x11_socket_address_length dd ?
x11_setup_memory dd ?
x11_id_base dd ?
x11_id_mask dd ?
x11_id_next dd ?
x11_root dd ?
x11_root_visual dd ?
x11_colormap dd ?
x11_white_pixel dd ?
x11_black_pixel dd ?
x11_red_shift dd ?
x11_red_bits dd ?
x11_green_shift dd ?
x11_green_bits dd ?
x11_blue_shift dd ?
x11_blue_bits dd ?
x11_auth_file dd ?
x11_auth_name_length dd ?
x11_auth_data_length dd ?
x11_host_name_length dd ?
x11_opened_font dd ?
x11_keysyms_per_keycode dd ?
x11_keysyms_count dd ?
x11_alt_mask dd ?
x11_numlock_mask dd ?
x11_level3_mask dd ?
x11_mode_switch_mask dd ?
x11_max_request_size dd ?
x11_screen_width dd ?
x11_screen_height dd ?
x11_min_keycode db ?
x11_max_keycode db ?
x11_root_depth db ?
x11_truecolor db ?
arc_opcode db ?

align 4

created_window dd ?
window_value_mask dd ?
window_values rd 16
property_offset dd ?
authority_family dd ?
authority_address dd ?
authority_address_length dd ?
authority_number dd ?
authority_number_length dd ?
authority_name dd ?
authority_name_length dd ?
authority_data dd ?
authority_data_length dd ?
property_window dd ?
property_type dd ?
property_format dd ?
property_count dd ?
property_size dd ?
lookup_state dd ?
lookup_group_column dd ?
lookup_level_column dd ?

; main window

editor_font_id dd ?
cursor_font dd ?
text_cursor dd ?
main_window dd ?
editor_gc dd ?
caret_gc dd ?
main_width dd ?
main_height dd ?
menubar_window dd ?
menubar_height dd ?
status_height dd ?
tabs_height dd ?
tabs_control dd ?
status_control dd ?
tabs dd ?
tabs_count dd ?
current_tab dd ?
assigned_file dd ?
tabs_first dd ?
painted_tab dd ?
tab_x dd ?
tab_width dd ?
tab_top dd ?
tab_bottom dd ?
main_focused dd ?
quit_flag dd ?
close_flags dd ?
command_id dd ?
selected_tab dd ?
new_tab_name dd ?
open_readonly dd ?
open_path dd ?
open_file_memory dd ?
open_file_size dd ?
open_cells_memory dd ?
open_path_copy dd ?
open_title dd ?
save_only_modified dd ?
save_tab dd ?
save_editor dd ?
save_memory dd ?
save_as_tab dd ?
save_as_current dd ?
save_as_destination dd ?
save_as_old_path dd ?
save_as_duplicate dd ?
save_as_title dd ?
save_copy_memory dd ?
deleted_tab dd ?
arguments_base dd ?
argument_flags dd ?
status_editor dd ?
status_mode dd ?
pending_memory_error dd ?
menu_editor dd ?
menu_selection_flag dd ?
menu_editor_mode dd ?
fepos rd 4
editor_font_record rd FONT_RECORD_SIZE/4
window_left dd ?
window_top dd ?
window_right dd ?
window_bottom dd ?
window_position_known dd ?
window_maximized dd ?
instance_flags dd ?
size_hints rd 18
gc_values rd 8
status_position rb 32
font_name rb 256

; user interface

ui_font dd ?
ui_font_width dd ?
ui_font_height dd ?
ui_font_ascent dd ?
ui_gc dd ?
ui_gc_foreground dd ?
ui_gc_background dd ?
ui_pixels:
px_face dd ?
px_light dd ?
px_shadow dd ?
px_dark dd ?
px_text dd ?
px_gray dd ?
px_window dd ?
px_selection dd ?
px_selection_text dd ?
px_trough dd ?
px_red dd ?
px_blue dd ?
px_yellow dd ?
px_white dd ?
frame_x dd ?
frame_y dd ?
frame_right dd ?
frame_bottom dd ?
text_window dd ?
text_x dd ?
text_y dd ?
text_underline dd ?
text_foreground dd ?
ui_text_background dd ?
ui_text_mnemonics dd ?
building_dialog dd ?
current_dialog dd ?
new_control_type dd ?
new_control_id dd ?
new_control_x dd ?
new_control_y dd ?
new_control_width dd ?
new_control_height dd ?
new_control_style dd ?
new_control_text dd ?
new_control_parent dd ?
new_control_notify dd ?
new_control_notify_data dd ?
new_control_procedure dd ?
blink_active dd ?
blink_time dd ?
repeat_control dd ?
repeat_time dd ?
poll_descriptors rd 4
last_event_time dd ?
cm_x dd ?
cm_y dd ?
cm_root_x dd ?
cm_root_y dd ?
cm_button dd ?
cm_state dd ?
cm_keysym dd ?
cm_base_keysym dd ?
cm_character dd ?
last_click_time dd ?
last_click_control dd ?
last_click_x dd ?
last_click_y dd ?
keyboard_focus_change dd ?
focus_backward dd ?
dropped_combo dd ?
menu_active dd ?
new_dialog_template dd ?
new_dialog_procedure dd ?
new_dialog_parameter dd ?
dialog_x dd ?
dialog_y dd ?
static_color dd ?
check_x dd ?
check_y dd ?
check_line dd ?
ebx_control_width dd ?
ebx_control_height dd ?
scroll_arrow_size dd ?
scroll_trough_size dd ?
scroll_thumb_size dd ?
scroll_thumb_start dd ?
scroll_thumb_free dd ?
scroll_range dd ?
scroll_arrow_index dd ?
scroll_track_position dd ?
arrow_x dd ?
arrow_y dd ?
arrow_width dd ?
arrow_height dd ?
arrow_window dd ?
arrow_direction dd ?
arrow_center_x dd ?
arrow_center_y dd ?
triangle_step dd ?
edit_background dd ?
edit_visible dd ?
edit_text_y dd ?
edit_selection_start dd ?
edit_selection_end dd ?
edit_visible_end dd ?
edit_run_start dd ?
edit_run_length dd ?
edit_caret_on dd ?
edit_inserted_cell dd ?
edit_paste_memory dd ?
list_border dd ?
list_row_width dd ?
list_row_y dd ?
list_row_color dd ?
list_wheel_code dd ?
combo_search_character dd ?
fedit_text_memory dd ?
menu_items dd ?
menu_count dd ?
menu_selected dd ?
menu_window dd ?
menu_x dd ?
menu_y dd ?
menu_width dd ?
menu_height dd ?
menu_accelerator_width dd ?
menu_bar_index dd ?
menu_ignore_release dd ?
menu_open_by_keyboard dd ?
menu_entry_y dd ?
menu_entry_height dd ?
menu_entry_color dd ?
menubar_positions rd 16
menubar_widths rd 16
menubar_count dd ?
menubar_item_width dd ?
menubar_item_color dd ?
message_flags dd ?
message_text dd ?
message_items dd ?
message_text_height dd ?
message_text_width dd ?
message_item_x dd ?
message_item_y dd ?
message_item_width dd ?
message_item_height dd ?
message_item_style dd ?
message_width dd ?
message_buttons_y dd ?
message_buttons dd ?
cm_event db ?
cm_flags db ?
ui_font_16bit db ?

align 4

; current editor (copied from and to the memory of editor)

label editor_data

  include 'ide/variable.inc'

  editor_style dd ?
  editor_window dd ?
  editor_font dd ?
  editor_parent dd ?
  editor_owner dd ?
  editor_x dd ?
  editor_y dd ?
  editor_width dd ?
  editor_height dd ?
  editor_vscroll dd ?
  editor_hscroll dd ?
  editor_visible dd ?
  client_width dd ?
  client_height dd ?
  font_width dd ?
  font_height dd ?
  font_ascent dd ?
  screen_base dd ?
  screen_offset dd ?
  screen_width dd ?
  screen_height dd ?
  screen_size dd ?
  line_buffer dd ?
  line_buffer_size dd ?
  background_color dd ?
  text_color dd ?
  selection_background dd ?
  selection_text dd ?
  syntax_proc dd ?
  syntax_colors dd ?
  caret_x dd ?
  caret_y dd ?
  caret_shown_x dd ?
  caret_shown_y dd ?
  caret_shown_width dd ?
  caret_shown_height dd ?
  current_operation db ?
  last_operation db ?
  was_selection db ?
  mouse_select db ?
  focus db ?
  macro_operation db ?
  caret_shown db ?

  align 4

editor_data_size = $ - editor_data

; editor control variables

fedit_handle dd ?
fedit_stack dd ?
fe_message dd ?
fe_wparam dd ?
fe_lparam dd ?
fe_state dd ?
fe_x dd ?
fe_y dd ?
fe_width dd ?
fe_height dd ?
fedit_create_flags dd ?
return_value dd ?
line_number dd ?
inserted_character dd ?
following_character dd ?
gettext_length dd ?
gettext_next_line dd ?
paint_x dd ?
paint_y dd ?
paint_first_column dd ?
paint_first_row dd ?
paint_last_column dd ?
paint_last_row dd ?
paint_cells_a dd ?
paint_attributes_a dd ?
paint_cells_b dd ?
paint_attributes_b dd ?
pasted_cells dd ?
editor_gc_font dd ?
pointer_editor dd ?
notification db ?
key_flags db ?

align 4

; dialogs

search_settings dd ?
search_text_size dd ?
command_flags dd ?
replace_editor dd ?
replaces_count dd ?
replace_mode dd ?
tmp_colors rd 8
tmp_font_record rd FONT_RECORD_SIZE/4
file_dialog_mode dd ?
dialog_readonly dd ?
dialog_filter dd ?
selected_names_count dd ?
file_list dd ?
directory_entries dd ?
directory_entries_count dd ?
directory_names dd ?
directory_names_used dd ?
directory_handle dd ?
preview_font dd ?
preview_font_ascent dd ?
preview_font_16bit dd ?
font_list dd ?
font_names dd ?
font_names_used dd ?
xlfd_fields rd 16
dialog_color dd ?
color_updating dd ?
history_list rd CONTROL_SIZE/4

; compiler

compiling dd ?
resume_esp dd ?
resume_eip dd ?
compile_results dd ?
compile_source dd ?
compile_pid dd ?
compile_status dd ?
compile_show_summary dd ?
pipe_descriptors rd 2
compile_start_time dd ?
environment_limit dd ?
show_line_path dd ?
show_line_number dd ?
show_line_buffer dd ?
pipe_input rb 16
progress_byte db ?
preprocessing_done db ?

align 4

; clipboard, drag and drop, single instance

waited_event_window dd ?
wait_deadline dd ?
wait_poll rd 2
clipboard_data dd ?
clipboard_length dd ?
clipboard_owned dd ?
clipboard_target dd ?
clipboard_latin1 dd ?
received_property dd ?
received_data dd ?
received_length dd ?
received_size dd ?
received_type dd ?
received_bytes_after dd ?
selection_targets rd 4
incr_window dd ?
incr_property dd ?
incr_offset dd ?
incr_chunk dd ?
incr_type dd ?
dnd_source dd ?
dnd_accept dd ?
dnd_pending dd ?
instance_owner dd ?
instance_time dd ?
waited_event_type db ?

align 4

; settings

ini_data dd ?
ini_length dd ?
ini_section dd ?
ini_key dd ?
ini_value dd ?
ini_key_line dd ?
ini_key_line_end dd ?
ini_insert_position dd ?
ini_line_start dd ?
color_to_write dd ?

; core variables of assembler

include 'source/variable.inc'

; buffers

align 4

waited_event rd 8
selection_notify rd 8
dnd_event rd 8
x11_event rd 8
number_text rb 32
upper_case_table rb 100h
syntax_symbols rb 100h
char2b_buffer rb 256*2
name_buffer rb 100h
x11_socket_address rb 112
x11_auth_name rb 20
x11_auth_data rb 104h
utsname rb 400
tmp_font_name rb 256
chosen_font_name rb 256
font_name_buffer rb 256
font_description rb 512
x11_event_queue rb X11_QUEUE_LENGTH*32
window_table rb WINDOW_TABLE_LENGTH*16
tab_positions rd MAX_VISIBLE_TABS
tab_widths rd MAX_VISIBLE_TABS
message_template rb 2000h
string_buffer rb 2000h
search_string rb 1000h
replace_string rb 1000h
search_cells rd 1000h
replace_cells rd 1000h
show_line_cells rd SHOW_LINE_CELLS
full_path_buffer rb PATH_SIZE*2
path_buffer rb 4000h
argument_path rb PATH_SIZE
include_path rb PATH_SIZE
dialog_path rb PATH_SIZE
dialog_directory rb PATH_SIZE
dialog_initial_name rb PATH_SIZE
dialog_files rb PATH_SIZE*8
dialog_names rb PATH_SIZE*4
entry_path rb PATH_SIZE
dirent_buffer rb 4000h
core_path rb PATH_SIZE
compile_directory rb PATH_SIZE
symbols_path rb PATH_SIZE
run_path rb PATH_SIZE
run_directory rb PATH_SIZE
show_path rb PATH_SIZE
dropped_path rb PATH_SIZE
ini_path rb PATH_SIZE
instance_buffer rb INSTANCE_BUFFER_SIZE
instance_arguments rd INSTANCE_MAX_ARGUMENTS+1
x11_keymap rd (X11_PACKET_SIZE-32)/4
x11_out_buffer rb X11_OUT_BUFFER_SIZE
x11_in_buffer rb X11_IN_BUFFER_SIZE
x11_packet rb X11_PACKET_SIZE
ini_temporary rb INI_BUFFER_SIZE
