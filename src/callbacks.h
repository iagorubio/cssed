/*  cssed (c) Iago Rubio 2003, 2005 - A tiny CSS editor.
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Library General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */


void on_menu_new_activate (GtkMenuItem * menuitem, gpointer user_data);
void on_menu_open_activate (GtkMenuItem * menuitem, gpointer user_data);
#ifdef GTK_ATLEAST_2_4 // gtk file chooser don't let you paste a path
void on_menu_open_path_activate (GtkMenuItem * menuitem, gpointer user_data);
#endif
void on_menu_close_activate (GtkMenuItem * menuitem, gpointer user_data);
void on_menu_save_all_activate (GtkMenuItem * menuitem, gpointer user_data);
void on_menu_close_all_activate (GtkMenuItem * menuitem, gpointer user_data);
void on_menu_revert_to_saved_activate (GtkMenuItem * menuitem, gpointer user_data);
void on_menu_save_activate (GtkMenuItem * menuitem, gpointer user_data);
void on_menu_save_as_activate (GtkMenuItem * menuitem, gpointer user_data);
void on_menu_quit_activate (GtkMenuItem * menuitem, gpointer user_data);
void on_menu_undo_activate (GtkMenuItem * menuitem, gpointer user_data);
void on_menu_redo_activate (GtkMenuItem * menuitem, gpointer user_data);
void on_menu_search_activate (GtkMenuItem * menuitem, gpointer user_data);
void on_menu_replace_activate (GtkMenuItem * menuitem, gpointer user_data);
void on_menu_cut_activate (GtkMenuItem * menuitem, gpointer user_data);
void on_menu_copy_activate (GtkMenuItem * menuitem, gpointer user_data);
void on_menu_copy_to_scratch_activate (GtkMenuItem * menuitem, gpointer user_data);
void on_menu_paste_activate (GtkMenuItem * menuitem, gpointer user_data);
void on_menu_paste_from_scratch_activate (GtkMenuItem *menuitem, gpointer user_data);
void on_menu_delete_scratch_activate (GtkMenuItem *menuitem, gpointer user_data);
void on_menu_delete_activate (GtkMenuItem * menuitem, gpointer user_data);
void on_menu_preferences_activate (GtkMenuItem * menuitem, gpointer user_data);
void on_menu_view_toolbar_toggled (GtkCheckMenuItem * checkmenuitem, gpointer user_data);
void on_menu_about_activate (GtkMenuItem * menuitem, gpointer user_data);
#ifdef WITH_HELP_MENUS
#ifdef DARWIN
void on_menu_cssed_website_with_safari_activate (GtkMenuItem * menuitem, gpointer user_data);
void on_menu_cssed_website_with_firefox_activate (GtkMenuItem * menuitem, gpointer user_data);
#endif
void on_menu_cssed_website_with_mozilla_activate (GtkMenuItem * menuitem, gpointer user_data);
void on_menu_cssed_website_with_galeon_activate (GtkMenuItem * menuitem, gpointer user_data);
void on_menu_cssed_website_with_dillo_activate (GtkMenuItem * menuitem, gpointer user_data);
void on_menu_cssed_website_with_gnome_activate (GtkMenuItem * menuitem, gpointer user_data);
#ifdef DARWIN
void on_menu_cssed_doc_online_with_safari_activate (GtkMenuItem * menuitem, gpointer user_data);
void on_menu_cssed_doc_online_with_firefox_activate (GtkMenuItem * menuitem, gpointer user_data);
#endif
void on_menu_cssed_doc_online_with_mozilla_activate (GtkMenuItem * menuitem, gpointer user_data);
void on_menu_cssed_doc_online_with_galeon_activate (GtkMenuItem * menuitem, gpointer user_data);
void on_menu_cssed_doc_online_with_dillo_activate (GtkMenuItem * menuitem, gpointer user_data);
void on_menu_cssed_doc_online_with_gnome_activate (GtkMenuItem * menuitem, gpointer user_data);
#endif /* #ifdef WITH_HELP_MENUS */
void on_menu_cssed_help_activate     (GtkMenuItem *menuitem, gpointer user_data);
void on_errordialog_okbutton_clicked (GtkButton * button, gpointer user_data);
void on_button_toolbar_open_clicked (GtkButton * button, gpointer user_data);
void on_button_toolbar_new_clicked (GtkButton * button, gpointer user_data);
void on_button_toolbar_close_clicked (GtkButton * button, gpointer user_data);
void on_button_toolbar_search_clicked (GtkButton * button, gpointer user_data);
void on_button_toolbar_replace_clicked (GtkButton * button, gpointer user_data);
void on_button_toolbar_copy_clicked (GtkButton * button, gpointer user_data);
void on_button_toolbar_paste_clicked (GtkButton * button, gpointer user_data);
void on_button_color_clicked (GtkButton * button, gpointer user_data);
void on_button_toolbar_save_clicked (GtkButton * button, gpointer user_data);
void on_button_toolbar_save_as_clicked (GtkButton * button, gpointer user_data);
void on_button_toolbar_cut_clicked (GtkButton * button, gpointer user_data);
void on_button_next_clicked (GtkButton * button, gpointer user_data);
void on_button_prev_clicked (GtkButton * button, gpointer user_data);
void on_button_undo_clicked (GtkButton * button, gpointer user_data);
void on_button_redo_clicked (GtkButton * button, gpointer user_data);
void on_button_zoom_in_clicked (GtkButton * button, gpointer user_data);
void on_button_zoom_out_clicked (GtkButton * button, gpointer user_data);
void on_button_zoom_off_clicked (GtkButton * button, gpointer user_data);
void on_button_preferences_clicked (GtkButton * button, gpointer user_data);

void on_button_hide_side_clicked (GtkButton * button, gpointer user_data);
void on_button_show_side_clicked (GtkButton * button, gpointer user_data);
void on_button_hide_footer_clicked (GtkButton * button, gpointer user_data);
void on_button_show_footer_clicked (GtkButton * button, gpointer user_data);

void on_button_border_all_clicked (GtkButton * button, gpointer user_data);
void on_button_margin_all_clicked (GtkButton * button, gpointer user_data);
void on_button_padding_all_clicked (GtkButton * button, gpointer user_data);
void on_button_box_properties_clicked (GtkButton * button,  gpointer user_data);
void on_button_sel_wizard_clicked (GtkButton * button, gpointer user_data);
void on_button_col_wizard_clicked (GtkButton * button, gpointer user_data);
void on_button_font_clicked (GtkButton * button, gpointer user_data);
void on_button_scan_selector_clicked (GtkButton * button, gpointer user_data);
void on_button_doc_info_clicked (GtkButton * button, gpointer user_data);
void on_button_clean_output_clicked (GtkButton * button, gpointer user_data);
void on_button_validate_clicked (GtkButton * button, gpointer user_data);
void on_button_validate_dump_clicked (GtkButton * button, gpointer user_data);
/*
void on_save_fileselection_ok_button_clicked (GtkButton * button, gpointer user_data);
void on_save_as_fileselection_ok_button_clicked (GtkButton * button, gpointer user_data);
*/
void on_window_destroy (GtkObject * object, gpointer user_data);
gboolean on_window_delete_event (GtkWidget * widget, GdkEvent * event,gpointer user_data);
void cssed_error_message (gchar * title, gchar * message, ...);
void on_color_selection_ok_button_clicked (GtkButton * button, gpointer user_data);
void on_color_selection_help_button_clicked (GtkButton * button, gpointer user_data);
void on_dialog_search_help_button_clicked (GtkButton * button, gpointer user_data);
void on_dialog_replace_help_button_clicked (GtkButton * button, gpointer user_data);
void on_search_backwards_checkbox_toggled  (GtkToggleButton *togglebutton,  gpointer user_data);
void on_search_dialog_find_button_clicked (GtkButton * button, gpointer user_data);
void on_dialog_replace_search_button_clicked (GtkButton * button, gpointer user_data);
void on_dialog_replace_apply_button_clicked (GtkButton * button, gpointer user_data);
void on_menu_line_numbers_toggled (GtkCheckMenuItem * checkmenuitem, gpointer user_data);
void on_menu_line_wrapping_toggled (GtkCheckMenuItem * checkmenuitem, gpointer user_data);
void on_menu_line_endings_toggled (GtkCheckMenuItem * checkmenuitem, gpointer user_data);
void on_menu_view_white_spaces_toggled (GtkCheckMenuItem * checkmenuitem, gpointer user_data);
void on_menu_document_activate (GtkMenuItem * checkmenuitem, gpointer user_data);
void on_menu_eol_mode_activate (GtkMenuItem * checkmenuitem, gpointer user_data);
void on_menu_eol_mode_cr_activate (GtkMenuItem * checkmenuitem, gpointer user_data);
void on_menu_eol_mode_lf_activate (GtkMenuItem * checkmenuitem, gpointer user_data);
void on_menu_eol_mode_crlf_activate (GtkMenuItem * checkmenuitem, gpointer user_data);
void on_menu_eol_convert_cr_activate (GtkMenuItem * checkmenuitem, gpointer user_data);
void on_menu_eol_convert_lf_activate (GtkMenuItem * checkmenuitem, gpointer user_data);
void on_menu_eol_convert_crlf_activate (GtkMenuItem * checkmenuitem, gpointer user_data);
void on_menu_item_autocompletion_toggled (GtkCheckMenuItem * checkmenuitem, gpointer user_data);
void on_menu_item_folding_toggled (GtkCheckMenuItem * checkmenuitem, gpointer user_data);
void on_menuitem_folding_unfoldall_activate (GtkMenuItem * menuitem, gpointer user_data);
void on_menu_item_font_activate (GtkMenuItem * menuitem, gpointer user_data);
void on_menu_item_flag_red_activate( GtkMenuItem *menuitem, gpointer user_data);
void on_menu_item_flag_white_activate ( GtkMenuItem *menuitem, gpointer user_data);
void on_menu_item_flag_blue_activate ( GtkMenuItem *menuitem, gpointer user_data);
void on_menu_item_flag_yellow_activate ( GtkMenuItem *menuitem, gpointer user_data);
void on_menu_item_flag_green_activate ( GtkMenuItem *menuitem, gpointer user_data);
void on_menu_item_flag_clear_activate( GtkMenuItem *menuitem, gpointer user_data);
void on_menu_item_document_goto_start_activate(GtkMenuItem *menuitem, gpointer user_data);
void on_menu_item_document_goto_end_activate(GtkMenuItem *menuitem, gpointer user_data);
void on_menu_item_document_goto_line_activate(GtkMenuItem *menuitem, gpointer user_data);
void on_menu_item_document_list_current_activate ( GtkMenuItem *menuitem, gpointer user_data);
void on_menu_item_document_list_new_activate ( GtkMenuItem *menuitem, gpointer user_data);
void on_menu_item_document_list_open_activate ( GtkMenuItem *menuitem, gpointer user_data);
void on_menuitem_fonding_foldall_activate (GtkMenuItem * menuitem, gpointer user_data);
/*
void on_menuitem_highlighting_text_activate (GtkMenuItem * menuitem, gpointer user_data);
void on_menuitem_highlighting_css_activate (GtkMenuItem * menuitem, gpointer user_data);
void on_menuitem_highlighting_html_activate(GtkMenuItem * menuitem, gpointer user_data);
void on_menuitem_highlighting_sh_activate(GtkMenuItem * menuitem, gpointer user_data);	
void on_menuitem_highlighting_c_activate(GtkMenuItem * menuitem, gpointer user_data);
void on_menuitem_highlighting_sql_activate(GtkMenuItem * menuitem, gpointer user_data);
void on_menuitem_highlighting_apache_conf_activate(GtkMenuItem * menuitem, gpointer user_data);
void on_menuitem_highlighting_perl_activate(GtkMenuItem * menuitem, gpointer user_data);		  
void on_menuitem_highlighting_python_activate(GtkMenuItem * menuitem, gpointer user_data);
void on_menuitem_highlighting_xml_activate(GtkMenuItem * menuitem, gpointer user_data);
void on_menuitem_highlighting_diff_activate(GtkMenuItem * menuitem, gpointer user_data);
void on_menuitem_highlighting_make_activate(GtkMenuItem * menuitem, gpointer user_data);
void on_menuitem_highlighting_html_activate(GtkMenuItem * menuitem, gpointer user_data);
*/	
void on_menuitem_encoding_default_activate	(GtkMenuItem * menuitem, gpointer user_data);
void on_menuitem_encoding_utf8_activate(GtkMenuItem * menuitem, gpointer user_data);
void on_menuitem_encoding_dbcs_activate(GtkMenuItem * menuitem, gpointer user_data);	
void on_menuitem_charset_ansi_activate(GtkMenuItem * menuitem, gpointer user_data);
void on_menuitem_charset_easteurope_activate(GtkMenuItem * menuitem, gpointer user_data);
void on_menuitem_charset_gb2312_activate(GtkMenuItem * menuitem, gpointer user_data);
void on_menuitem_charset_hangul_activate(GtkMenuItem * menuitem, gpointer user_data);
void on_menuitem_charset_shiftjis_activate(GtkMenuItem * menuitem, gpointer user_data); 
#ifdef WITH_SPLITTER_WINDOW
void on_menuitem_split_horizontal_activate	(GtkMenuItem * menuitem, gpointer user_data);
void on_menuitem_split_vertical_activate (GtkMenuItem * menuitem, gpointer user_data);
void on_menuitem_split_none_activate (GtkMenuItem * menuitem, gpointer user_data);
#endif 
void on_menuitem_selector_wizard_activate (GtkMenuItem * menuitem, gpointer user_data);
void on_menuitem_color_wizard_activate (GtkMenuItem * menuitem, gpointer user_data);
void on_menuitem_box_wizard_activate (GtkMenuItem * menuitem, gpointer user_data);
//void on_menu_item_autocompletion_activate     (GtkCheckMenuItem *menuitem,            gpointer user_data);
void on_menu_scan_selector_activate (GtkMenuItem * menuitem, gpointer user_data);
void on_menu_doc_info_activate (GtkMenuItem * menuitem, gpointer user_data);
void on_menu_utils_validate_activate (GtkMenuItem * menuitem, gpointer user_data);
void on_menu_utils_validate_only_activate (GtkMenuItem * menuitem, gpointer user_data);
//#ifdef WITH_PLUGINS						  
void on_plugins_load_activate(GtkMenuItem * menuitem, gpointer user_data);
//#endif	  
void on_menu_utils_cleanoutput_activate (GtkMenuItem * menuitem, gpointer user_data);
void on_menu_autocompletion_activate (GtkMenuItem * menuitem, gpointer user_data);
void on_show_footer_panel_activate (GtkMenuItem * menuitem, gpointer user_data);
void on_hide_footer_panel_activate (GtkMenuItem * menuitem, gpointer user_data);
void on_show_side_panel_activate (GtkMenuItem * menuitem, gpointer user_data);
void on_hide_side_panel_activate (GtkMenuItem * menuitem, gpointer user_data);

void
on_output_list_row_activated (	GtkTreeView * treeview, 
								GtkTreePath * path,
			      				GtkTreeViewColumn * col, 
								gpointer user_data );

void
on_doc_scanner_row_activated ( 	GtkTreeView * treeview, 
								GtkTreePath * path,
			      				GtkTreeViewColumn * col,
								gpointer user_data );

void
on_doc_scanner_col_numbers_clicked      (GtkTreeViewColumn *treeviewcolumn,
                                            gpointer user_data);
void
on_doc_scanner_col_styles_clicked      (GtkTreeViewColumn *treeviewcolumn,
                                            gpointer user_data);
void
on_doc_scanner_col_file_clicked      (GtkTreeViewColumn *treeviewcolumn,
                                            gpointer user_data);
void
on_doc_scanner_col_path_clicked      (GtkTreeViewColumn *treeviewcolumn,
                                            gpointer user_data);

void
on_treeview_css_properties_row_activated (GtkTreeView * treeview,
					  			GtkTreePath * path,
					  			GtkTreeViewColumn * col,
					  			gpointer user_data);
void on_treeview_css_current_row_activated (GtkTreeView * treeview,
					    		GtkTreePath * path,
					    		GtkTreeViewColumn * col,
					    		gpointer user_data);
void on_current_css_render_values_edited (GtkCellRendererText * cell,
					  			gchar * path_string,
					  			gchar * new_text,
					  			gpointer user_data);
void on_notebook_docs_switch_page (GtkNotebook * notebook,
					  			GtkNotebookPage * page, guint page_num,
					  			gpointer user_data);

void
popmenu_autocomp_activate_cb (GtkMenuItem * menuitem, gpointer user_data);
void
popmenu_autocomp_event_after_cb (	GtkWidget * widget,
					  				GdkEvent * event, 
									gpointer user_data);
/* file selector funcs */
/*
void file_selector_close (GtkDialog * dialog, gpointer user_data);

void
file_selector_cancel_button_clicked (GtkButton * button, gpointer user_data);
*/

/* testing here editor event for the selector scanner and autocompletion lists */
gboolean
on_editor_key_press_event (	GtkWidget * widget, 
							GdkEventKey * event,
					  		gpointer user_data);

gboolean
on_editor_button_press_event(GtkWidget *widget, GdkEventButton *event,
                                                gpointer user_data);
												
void on_pop_menu_undo_activate(GtkMenuItem *menuitem, gpointer user_data);	
void on_pop_menu_redo_activate(GtkMenuItem *menuitem, gpointer user_data);	
void on_pop_menu_cut_activate(GtkMenuItem *menuitem, gpointer user_data);	
void on_pop_menu_copy_activate(GtkMenuItem *menuitem, gpointer user_data);
void on_pop_menu_copy_to_scratch_activate (GtkMenuItem *menuitem, gpointer user_data);
void on_pop_menu_paste_activate(GtkMenuItem *menuitem, gpointer user_data);	
void on_pop_menu_paste_from_scratch_activate (GtkMenuItem *menuitem, gpointer user_data);
void on_pop_menu_delete_scratch_activate (GtkMenuItem *menuitem, gpointer user_data);
void on_pop_menu_delete_activate(GtkMenuItem *menuitem, gpointer user_data);	
void on_pop_menu_search_activate(GtkMenuItem *menuitem, gpointer user_data);	
void on_pop_menu_replace_activate(GtkMenuItem *menuitem, gpointer user_data);	
void on_pop_menu_selectall_activate(GtkMenuItem *menuitem, gpointer user_data);
void on_pop_menu_editcolor_activate(GtkMenuItem *menuitem, gpointer user_data);
void on_editor_grab_focus (GtkWidget * widget, gpointer user_data);
// new scanner button
void on_document_scanner_button_clicked (GtkButton *button, gpointer user_data);
void on_document_scanner_button_append_clicked(GtkButton *button, gpointer user_data);
void on_document_scanner_button_clean_clicked(GtkButton *button, gpointer user_data);
#ifdef WITH_TERMINAL
/* terminal child exited callback */
void
on_vte_terminal_child_exited (VteTerminal * vteterminal, gpointer user_data);
#endif
/*
void // PANEDBUG
on_handle_position_changed (GObject *pane, GParamSpec *pspec, gpointer data);
*/
gboolean
document_scanner_treeview_search_equal_func (GtkTreeModel *model,
                                             gint column,
                                             const gchar *key,
                                             GtkTreeIter *iter,
                                             gpointer search_data);
void
on_recent_file_sub_menu_item_activated (GtkRecentChooser *chooser, gpointer user_data);
