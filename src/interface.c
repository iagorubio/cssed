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

#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <stdio.h>
#include <gdk/gdkkeysyms.h>
#include <gtk/gtk.h>

#define WITH_PLUGINS
//#define SIDEPANEL_AT_LEFT this is just a test and it's not working
#define WITH_RECENT_FILE_MANAGER

#ifdef WITH_PLUGINS
# include <gmodule.h>
#endif

#include "cssedwindow.h"
#include "document.h"
#include "callbacks.h"
#include "interface.h"
#include "support.h"
#include "xmlparser.h"
#include "cssdialogs-menu.h"
#include "filetypes.h"
#include "utils.h" // tree_iter_compare_markup_string
#include <pixmaps/images.h>

#define GLADE_HOOKUP_OBJECT(component,widget,name) \
  g_object_set_data_full (G_OBJECT (component), name, \
    gtk_widget_ref (widget), (GDestroyNotify) gtk_widget_unref)

#define GLADE_HOOKUP_OBJECT_NO_REF(component,widget,name) \
  g_object_set_data (G_OBJECT (component), name, widget)

enum
{
	COL_CSS_VER = 0,
	COL_ATTR,
	COL_VALUE,
	NUM_COLS
};

void set_css_properties_treeview (CssedWindow * window);

GtkWidget *
create_window (CssedWindow * windowobj)
{
	GtkWidget *window;
	GtkWidget *vbox_main;
	GtkWidget *alignment;
	GtkWidget *separatormenuitem;
	GtkWidget *menubar_main;
	GtkWidget *menuitem_file;
	GtkWidget *menuitem_file_menu;
	GtkWidget *menu_new;
	GtkWidget *menu_open;
#ifdef GTK_ATLEAST_2_4
	GtkWidget *menu_open_path;
#endif
	GtkWidget *menu_revert_to_saved;
	GtkWidget *menu_save;
	GtkWidget *menu_save_as;
	GtkWidget *menu_quit;
	GtkWidget *menuitem_edit;
	GtkWidget *menuitem_edit_menu;
	GtkWidget *menu_cut;
	GtkWidget *menu_copy;
	GtkWidget *menu_paste;
	GtkWidget *menu_delete;
	GtkWidget *menu_scratch;
	GtkWidget *menu_item_scratch;
	GtkWidget *menu_copy_to_scratch;
	GtkWidget *menu_paste_from_scratch;
	GtkWidget *menu_delete_scratch;	
	GtkWidget* menu_close;
	GtkWidget* menu_close_all;
	GtkWidget* menu_save_all;
	GtkWidget* image_menu;
	// undo redo and search replace
	GtkWidget *menu_undo;
	GtkWidget *menu_redo;
	GtkWidget *menu_search;
	GtkWidget *menu_replace;
	GtkWidget *menu_preferences;
	//  view /////////////////
	GtkWidget *menuitem_view;
	GtkWidget *menuitem_view_menu;
	GtkWidget *menu_toolbars_submenu;
	GtkWidget *menu_item_toolbars;
	GtkWidget *menu_toolbar;
	GtkWidget *menu_secodary_toolbar;
	// document list
	GtkWidget *menu_item_document_list;
	GtkWidget *menu_item_document_list_menu;
	GtkWidget *menu_item_document_list_current;
	GtkWidget *menu_item_document_list_open;
	GtkWidget *menu_item_document_list_new;
	// zoom
	GtkWidget *menu_zoom_in;
	GtkWidget *menu_zoom_out;
	GtkWidget *menu_normal_size;
	// document ////////////////
	GtkWidget *menuitem_document;
	GtkWidget *menu_view_line_numbers;
	GtkWidget *menuitem_document_menu;
	GtkWidget *menu_view_line_endings;
	GtkWidget *menu_view_white_spaces;
	GtkWidget *menu_view_lines_wrapped;
	GtkWidget *menu_item_autocompletion;
	GtkWidget *menu_item_folding;
	GtkWidget *menuitem_eol_mode;
	GtkWidget *menuitem_eol_mode_menu;
	GtkWidget *menuitem_eol_mode_cr;
	GtkWidget *menuitem_eol_mode_lf;
	GtkWidget *menuitem_eol_mode_crlf;

	GtkWidget *menuitem_eol_convert;
	GtkWidget *menuitem_eol_convert_menu;
	GtkWidget *menuitem_eol_convert_cr;
	GtkWidget *menuitem_eol_convert_lf;
	GtkWidget *menuitem_eol_convert_crlf;
	GSList    *eol_menus_list = NULL;
	GtkWidget *menuitem_folding;
	GtkWidget *menuitem_folding_menu;
	GtkWidget *menuitem_fonding_foldall;
	GtkWidget *menuitem_folding_unfoldall;
	// highlighting
	GtkWidget *menuitem_filetype;
	GtkWidget *menuitem_filetype_menu;
	// encoding
	GtkWidget *menuitem_encoding;
	GtkWidget *menuitem_encoding_menu;
	GtkWidget *menuitem_encoding_default;
	GtkWidget *menuitem_encoding_utf8;
	GtkWidget *menuitem_encoding_dbcs;
	// charset 
	GtkWidget *menuitem_charset;
	GtkWidget *menuitem_charset_menu;
	GtkWidget *menuitem_charset_ansi;
	GtkWidget *menuitem_charset_easteurope;
	GtkWidget *menuitem_charset_gb2312;	
	GtkWidget *menuitem_charset_hangul;
	GtkWidget *menuitem_charset_shiftjis;	
#ifdef WITH_SPLITTER_WINDOW	
	GtkWidget *menuitem_split;
	GtkWidget *menuitem_split_menu;
	GtkWidget *menuitem_split_hor;
	GtkWidget *menuitem_split_ver;
	GtkWidget *menuitem_split_none;			
#endif
	// change font // 
	GtkWidget *menu_item_font;
	// flag ////
	GtkWidget *menu_item_flag;
	GtkWidget *menu_item_flag_menu;
	GtkWidget *menu_item_flag_red;
	GtkWidget *menu_item_flag_white;
	GtkWidget *menu_item_flag_blue;
	GtkWidget *menu_item_flag_yellow;
	GtkWidget *menu_item_flag_green;
	GtkWidget *menu_item_flag_clear;
	// document goto
	GtkWidget *menu_item_document_goto;
	GtkWidget *menu_item_document_goto_menu;
	GtkWidget *menu_item_document_goto_start;
	GtkWidget *menu_item_document_goto_end;
	GtkWidget *menu_item_document_goto_line;
	// panels ///////////////////
	GtkWidget *menuitem_panel;
	GtkWidget *menuitem_panel_menu;
	GtkWidget *menuitem_show_footer_panel;
	GtkWidget *menuitem_hide_footer_panel;
	GtkWidget *menuitem_show_side_panel;
	GtkWidget *menuitem_hide_side_panel;
#ifdef WITH_PLUGINS
	GtkWidget *menuitem_plugins;
	GtkWidget *menuitem_plugins_menu;
	GtkWidget *menuitem_plugins_load;
#endif
	// help //////////////////////
	GtkWidget *menuitem_ayuda;
	GtkWidget *menuitem_ayuda_menu;
#ifdef WITH_HELP_MENUS
	GtkWidget *menuitem_cssed_website;
	GtkWidget *menuitem_cssed_website_menu;
#ifdef DARWIN
	GtkWidget *menuitem_cssed_website_with_safari;
	GtkWidget *menuitem_cssed_website_with_firefox;
#endif
	GtkWidget *menuitem_cssed_website_with_mozilla;
	GtkWidget *menuitem_cssed_website_with_galeon;
	GtkWidget *menuitem_cssed_website_with_dillo;
	GtkWidget *menuitem_cssed_website_with_gnome;
	GtkWidget *menuitem_cssed_doc_online;
	GtkWidget *menuitem_cssed_doc_online_menu;
#ifdef DARWIN
	GtkWidget *menuitem_cssed_doc_online_with_safari;
	GtkWidget *menuitem_cssed_doc_online_with_firefox;
#endif
	GtkWidget *menuitem_cssed_doc_online_with_mozilla;
	GtkWidget *menuitem_cssed_doc_online_with_galeon;
	GtkWidget *menuitem_cssed_doc_online_with_dillo;
	GtkWidget *menuitem_cssed_doc_online_with_gnome;
#endif /* #ifdef WITH_HELP_MENUS */
	GtkWidget *menu_cssed_help;
	// ///////// CSS ////////////
	GtkWidget *css_menu;
	GtkWidget *menuitem_selector_wizard;
	GtkWidget *menuitem_color_wizard;
	GtkWidget *menuitem_box_wizard;
	GtkWidget *menu_utils_scan_selector;
	GtkWidget *menu_utils_validate;
	GtkWidget *menu_utils_validate_only;

	GtkWidget *menu_about;
	GtkWidget *css_dialogs_submenu;
	GtkWidget *css_dialogs_mainmenu;
	// toolbar //////////////////
	GtkWidget *toolbar_main;
	GtkWidget *button_toolbar_open;
	GtkWidget *button_toolbar_new;
	GtkWidget *button_toolbar_close;
	GtkWidget *button_toolbar_search;
	GtkWidget *button_toolbar_replace;
	GtkWidget *button_toolbar_cut;
	GtkWidget *button_toolbar_copy;
	GtkWidget *button_toolbar_paste;
	GtkWidget *button_next;
	GtkWidget *button_prev;
	GtkWidget *button_undo;
	GtkWidget *button_redo;
	GtkWidget *button_color;
	GtkWidget *button_toolbar_save;
	GtkWidget *button_toolbar_save_as;
	GtkWidget *button_preferences;
	GtkWidget *button_hide_side;
	GtkWidget *button_show_side;
	GtkWidget *button_hide_footer;
	GtkWidget *button_show_footer;
	// second toolbar ///////////
	GtkWidget *toolbar_tools;
	GtkWidget* tt_icon;
	GtkWidget *button_border_all;
	GtkWidget *button_margin_all;
	GtkWidget *button_padding_all;
	GtkWidget *button_box_properties;
	GtkWidget *button_sel_wizard;
	GtkWidget *button_col_wizard;
	GtkWidget *button_font;
	GtkWidget *button_scan_selector;
	GtkWidget *button_doc_info;
	GtkWidget *button_clean_output;
	GtkWidget *button_validate;
	GtkWidget *button_validate_dump;
	// ////// zoom //////////////
	/* those will be taken out
	GtkWidget *button_zoom_in;
	GtkWidget *button_zoom_out;
	GtkWidget *button_zoom_off;
	*/
	// //////////////////////////
	GtkWidget *menuitem_utils;
	GtkWidget *menuitem_utils_menu;
	GtkWidget *menu_utils_doc_info;
	GtkWidget *menu_utils_autocomp;
	GtkWidget *menu_utils_cleanoutput;
	// ////////////////////////////////
	GtkWidget *vpaned_main;
	GtkWidget *hpaned_main;
	GtkWidget *vpaned_css;
	GtkWidget *notebook_docs;
	GtkWidget *scrolledwindow_cssc;
	GtkWidget *treeview_css_current;
	GtkWidget *scrolledwindow_cssp;
	GtkWidget *treeview_css_properties;
	GtkTreePath* css_prop_path;
	GtkWidget *notebook_footer;
	GtkWidget *output_list;
	GtkWidget *scrolledwindow_output_text;
	GtkWidget *scrolledwindow_scratch_text;
	GtkWidget *scratch_text;
	GtkWidget *scratch_label;
	GtkWidget* hpaned_scratch;
	GtkWidget* scrolledwindow_drop_text;
	GtkWidget* drop_text;
#ifdef WITH_TERMINAL
	GtkWidget *term_window;
	GtkWidget *label_terminal;
	GtkWidget* hbox_terminal;
	GtkWidget* frame_terminal;
	GtkWidget* sb_terminal;
	GdkColor terminal_fore_color;
	GdkColor terminal_back_color;
	GdkVisual* visual;
	GdkColormap* colormap;
	const gchar* shell;
#endif
	GtkWidget *label_notebook_footer;
	GtkAccelGroup *accel_group;
	// set up items to the output list
	GtkListStore *output_store;
	GtkTreeViewColumn *output_column;
	GtkCellRenderer *output_render;
	// set up items for the current selector list
	GtkListStore *current_css_store;
	GtkTreeViewColumn *current_css_property_column;
	GtkTreeViewColumn *current_css_value_column;
	GtkCellRenderer *current_css_render_properties;
	GtkCellRenderer *current_css_render_values;
	GtkTreeViewColumn *current_arrow_column;
	GtkCellRenderer *current_css_render_pixbuf;
	// setup thing for the document scanner
	GtkListStore *document_scanner_store;
	GtkTreeViewColumn *document_scanner_column_style;
	GtkTreeViewColumn *document_scanner_column_file;
	GtkTreeViewColumn *document_scanner_column_line;
	GtkTreeViewColumn *document_scanner_column_path;
	GtkCellRenderer *document_scanner_render_text;
	GtkWidget *document_scanner_vbox;
	GtkWidget *document_scanner_button;
	GtkWidget *document_scanner_button_append;
	GtkWidget *document_scanner_button_clean;
	GtkWidget *document_scanner_button_hbox;
	GtkWidget *document_scanner_scrolled_window;
	GtkWidget* document_scanner_treeview;
	// sidebar notebook
	GtkWidget *sidebar_notebook;
	// icon stuff
	GdkPixbuf* icon_pixbuf;
	GtkAccelGroup* css_menu_accels;
	GtkWidget* css_dialogs_item;
	//GtkWidget* image_menu;
	// editor pop menu
	GtkWidget * pop_menu;
	GtkWidget *pop_menu_undo;
	GtkWidget *pop_menu_redo;
	GtkWidget *pop_menu_cut;
	GtkWidget *pop_menu_copy;
	GtkWidget *pop_menu_paste;
	GtkWidget *pop_menu_copy_to_scratch;
	GtkWidget *pop_menu_paste_from_scratch;
	GtkWidget *pop_menu_delete_scratch;
	GtkWidget *pop_menu_delete;
	GtkWidget *pop_menu_search;
	GtkWidget *pop_menu_replace;
	GtkWidget *pop_menu_selectall;
	GtkWidget *pop_menu_editcolor;
	GtkTooltips* scratch_tips;
	CssedConfig* cfg;
#ifdef WITH_RECENT_FILE_MANAGER	
	GtkWidget* recent_file_menu;
	GtkWidget* recent_file_sub_menu;
#endif

	accel_group = gtk_accel_group_new ();
	cfg = cssed_window_get_config( windowobj );

	// pop menu to be used in editors
	pop_menu = gtk_menu_new();

	pop_menu_undo = gtk_image_menu_item_new_from_stock ("gtk-undo", NULL);
	gtk_widget_show (pop_menu_undo);
	gtk_container_add (GTK_CONTAINER (pop_menu), pop_menu_undo);

	pop_menu_redo= gtk_image_menu_item_new_from_stock ("gtk-redo", NULL);
	gtk_widget_show (pop_menu_redo);
	gtk_container_add (GTK_CONTAINER (pop_menu), pop_menu_redo);

	separatormenuitem = gtk_menu_item_new ();
	gtk_widget_show (separatormenuitem);
	gtk_container_add (GTK_CONTAINER (pop_menu), separatormenuitem);
	gtk_widget_set_sensitive (separatormenuitem, FALSE);

	pop_menu_cut= gtk_image_menu_item_new_from_stock ("gtk-cut", NULL);
	gtk_widget_show (pop_menu_cut);
	gtk_container_add (GTK_CONTAINER (pop_menu), pop_menu_cut);

	pop_menu_copy= gtk_image_menu_item_new_from_stock ("gtk-copy", NULL);
	gtk_widget_show (pop_menu_copy);
	gtk_container_add (GTK_CONTAINER (pop_menu), pop_menu_copy);	
	
	pop_menu_paste= gtk_image_menu_item_new_from_stock ("gtk-paste", NULL);
	gtk_widget_show (pop_menu_paste);
	gtk_container_add (GTK_CONTAINER (pop_menu), pop_menu_paste);	
	
	pop_menu_delete= gtk_image_menu_item_new_from_stock ("gtk-delete", NULL);
	gtk_widget_show (pop_menu_delete);
	gtk_container_add (GTK_CONTAINER (pop_menu), pop_menu_delete);

	// scratch pad
	menu_scratch = gtk_menu_new();
	gtk_widget_show (menu_scratch);
	
	menu_item_scratch = gtk_image_menu_item_new_with_mnemonic (_("Scratch pad"));
	gtk_widget_show (menu_item_scratch);
	gtk_menu_item_set_submenu (GTK_MENU_ITEM(menu_item_scratch), menu_scratch);
	gtk_container_add (GTK_CONTAINER (pop_menu), menu_item_scratch);
	
	pop_menu_copy_to_scratch = gtk_image_menu_item_new_with_mnemonic(_("Copy to"));
	gtk_widget_show (pop_menu_copy_to_scratch);
	image_menu = gtk_image_new_from_stock ("gtk-copy", GTK_ICON_SIZE_MENU);
	gtk_widget_show (image_menu);
	gtk_image_menu_item_set_image (GTK_IMAGE_MENU_ITEM (pop_menu_copy_to_scratch), image_menu);
	gtk_container_add (GTK_CONTAINER (menu_scratch), pop_menu_copy_to_scratch);

	pop_menu_paste_from_scratch = gtk_image_menu_item_new_with_mnemonic(_("Paste from"));
	gtk_widget_show (pop_menu_paste_from_scratch);
	image_menu = gtk_image_new_from_stock ("gtk-paste", GTK_ICON_SIZE_MENU);
	gtk_widget_show (image_menu);
	gtk_image_menu_item_set_image (GTK_IMAGE_MENU_ITEM (pop_menu_paste_from_scratch), image_menu);
	gtk_container_add (GTK_CONTAINER (menu_scratch), pop_menu_paste_from_scratch);

	separatormenuitem = gtk_menu_item_new ();
	gtk_widget_show (separatormenuitem);
	gtk_container_add (GTK_CONTAINER (menu_scratch),  separatormenuitem);
	gtk_widget_set_sensitive (separatormenuitem, FALSE);
	
	pop_menu_delete_scratch= gtk_image_menu_item_new_from_stock ("gtk-delete", NULL);
	gtk_widget_show (pop_menu_delete_scratch);
	gtk_container_add (GTK_CONTAINER (menu_scratch), pop_menu_delete_scratch);

	//
	separatormenuitem = gtk_menu_item_new ();
	gtk_widget_show (separatormenuitem);
	gtk_container_add (GTK_CONTAINER (pop_menu),  separatormenuitem);
	gtk_widget_set_sensitive (separatormenuitem, FALSE);

	pop_menu_search =
		gtk_image_menu_item_new_from_stock ("gtk-find", accel_group);
	gtk_widget_show (pop_menu_search);
	gtk_container_add (GTK_CONTAINER (pop_menu), pop_menu_search);

	pop_menu_replace =
		gtk_image_menu_item_new_from_stock ("gtk-find-and-replace", accel_group);
	gtk_widget_show (pop_menu_replace);
	gtk_container_add (GTK_CONTAINER (pop_menu), pop_menu_replace);

	separatormenuitem = gtk_menu_item_new ();
	gtk_widget_show (separatormenuitem);
	gtk_container_add (GTK_CONTAINER (pop_menu),  separatormenuitem);
	gtk_widget_set_sensitive (separatormenuitem, FALSE);

	pop_menu_selectall= gtk_image_menu_item_new_with_mnemonic(_("_Select all"));
	gtk_widget_show (pop_menu_selectall);
	gtk_container_add (GTK_CONTAINER (pop_menu), pop_menu_selectall);

	pop_menu_editcolor = gtk_image_menu_item_new_with_mnemonic(_("_Edit selected color"));
	gtk_widget_show (pop_menu_editcolor);
	gtk_container_add (GTK_CONTAINER (pop_menu), pop_menu_editcolor);

	// main window object
	window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
	cssed_window_set_window_widget( windowobj, window );
	cssed_window_set_pop_menu( windowobj, pop_menu );

	gtk_window_set_title (GTK_WINDOW (window), "cssed");

	icon_pixbuf =  create_pixbuf_from_data(cssed_icon);
	if( icon_pixbuf ) gtk_window_set_icon  (GTK_WINDOW (window), icon_pixbuf);
	
	vbox_main = gtk_vbox_new (FALSE, 0);
	gtk_widget_show (vbox_main);
	gtk_container_add (GTK_CONTAINER (window), vbox_main);

	alignment = gtk_alignment_new (0.5, 0.5, 1, 1);
	gtk_widget_show (alignment);
	gtk_box_pack_start (GTK_BOX (vbox_main), alignment, FALSE, FALSE, 0);

	menubar_main = gtk_menu_bar_new ();
	gtk_widget_show (menubar_main);
	gtk_container_add (GTK_CONTAINER (alignment), menubar_main);

	menuitem_file = gtk_menu_item_new_with_mnemonic (_("_File"));
	gtk_widget_show (menuitem_file);
	gtk_container_add (GTK_CONTAINER (menubar_main), menuitem_file);

	menuitem_file_menu = gtk_menu_new ();
	gtk_menu_item_set_submenu (GTK_MENU_ITEM (menuitem_file),
				   menuitem_file_menu);

	menu_new =
		gtk_image_menu_item_new_from_stock ("gtk-new", accel_group);
	gtk_widget_show (menu_new);
	gtk_container_add (GTK_CONTAINER (menuitem_file_menu), menu_new);

	menu_open =
		gtk_image_menu_item_new_from_stock ("gtk-open", accel_group);
	gtk_widget_show (menu_open);
	gtk_container_add (GTK_CONTAINER (menuitem_file_menu), menu_open);

#ifdef GTK_ATLEAST_2_4 // gtk file chooser don't let you paste a path
	menu_open_path =
		gtk_image_menu_item_new_with_label (_("Open path"));
	gtk_widget_show (menu_open_path);
	gtk_container_add (GTK_CONTAINER (menuitem_file_menu), menu_open_path);
#endif

	menu_close = gtk_image_menu_item_new_from_stock ("gtk-close", accel_group);
	gtk_widget_show (menu_close);
	gtk_container_add (GTK_CONTAINER (menuitem_file_menu),menu_close);

	menu_close_all = gtk_menu_item_new_with_label (_("Close all"));
	gtk_widget_show (menu_close_all);
	gtk_container_add (GTK_CONTAINER (menuitem_file_menu),menu_close_all);

	menu_revert_to_saved  = gtk_image_menu_item_new_from_stock ("gtk-revert-to-saved", accel_group);
	gtk_widget_show (menu_revert_to_saved);
	gtk_container_add (GTK_CONTAINER (menuitem_file_menu),menu_revert_to_saved);

	separatormenuitem = gtk_menu_item_new ();
	gtk_widget_show (separatormenuitem);
	gtk_container_add (GTK_CONTAINER (menuitem_file_menu),  separatormenuitem);
	gtk_widget_set_sensitive (separatormenuitem, FALSE);

	menu_save =
		gtk_image_menu_item_new_from_stock ("gtk-save", accel_group);
	gtk_widget_show (menu_save);
	gtk_container_add (GTK_CONTAINER (menuitem_file_menu), menu_save);

	menu_save_as =
		gtk_image_menu_item_new_from_stock ("gtk-save-as",  accel_group);
	gtk_widget_show (menu_save_as);
	gtk_container_add (GTK_CONTAINER (menuitem_file_menu), menu_save_as);


	image_menu = gtk_image_new_from_stock ("gtk-save", GTK_ICON_SIZE_MENU);
	gtk_widget_show (image_menu);

	menu_save_all = gtk_image_menu_item_new_with_label (_("Save all"));
	gtk_image_menu_item_set_image (GTK_IMAGE_MENU_ITEM (menu_save_all), image_menu);
	gtk_widget_show (menu_save_all);
	gtk_container_add (GTK_CONTAINER (menuitem_file_menu),menu_save_all);

#ifdef WITH_RECENT_FILE_MANAGER
	recent_file_menu = gtk_menu_item_new_with_label (_("Recent files"));
	gtk_widget_show (recent_file_menu);
	gtk_container_add (GTK_CONTAINER (menuitem_file_menu), recent_file_menu);
	
	recent_file_sub_menu =  gtk_recent_chooser_menu_new ();
	gtk_widget_show (menu_save_all);
	gtk_menu_item_set_submenu (GTK_MENU_ITEM(recent_file_menu),recent_file_sub_menu);
#endif

	separatormenuitem = gtk_menu_item_new ();
	gtk_widget_show (separatormenuitem);
	gtk_container_add (GTK_CONTAINER (menuitem_file_menu),  separatormenuitem);
	gtk_widget_set_sensitive (separatormenuitem, FALSE);

	menu_quit =
		gtk_image_menu_item_new_from_stock ("gtk-quit", accel_group);
	gtk_widget_show (menu_quit);
	gtk_container_add (GTK_CONTAINER (menuitem_file_menu), menu_quit);

	menuitem_edit = gtk_menu_item_new_with_mnemonic (_("_Edit"));
	gtk_widget_show (menuitem_edit);
	gtk_container_add (GTK_CONTAINER (menubar_main), menuitem_edit);

	menuitem_edit_menu = gtk_menu_new ();
	gtk_menu_item_set_submenu (GTK_MENU_ITEM (menuitem_edit),
				   menuitem_edit_menu);

	menu_undo =
		gtk_image_menu_item_new_from_stock ("gtk-undo", accel_group);
	gtk_widget_show (menu_undo);
	gtk_container_add (GTK_CONTAINER (menuitem_edit_menu), menu_undo);

	menu_redo =
		gtk_image_menu_item_new_from_stock ("gtk-redo", accel_group);
	gtk_widget_show (menu_redo);
	gtk_container_add (GTK_CONTAINER (menuitem_edit_menu), menu_redo);

	separatormenuitem = gtk_menu_item_new ();
	gtk_widget_show (separatormenuitem);
	gtk_container_add (GTK_CONTAINER (menuitem_edit_menu),
			   separatormenuitem);
	gtk_widget_set_sensitive (separatormenuitem, FALSE);

	menu_cut =
		gtk_image_menu_item_new_from_stock ("gtk-cut", accel_group);
	gtk_widget_show (menu_cut);
	gtk_container_add (GTK_CONTAINER (menuitem_edit_menu), menu_cut);

	menu_copy =
		gtk_image_menu_item_new_from_stock ("gtk-copy", accel_group);
	gtk_widget_show (menu_copy);
	gtk_container_add (GTK_CONTAINER (menuitem_edit_menu), menu_copy);
	
	menu_paste =
		gtk_image_menu_item_new_from_stock ("gtk-paste", accel_group);
	gtk_widget_show (menu_paste);
	gtk_container_add (GTK_CONTAINER (menuitem_edit_menu), menu_paste);

	menu_delete =
		gtk_image_menu_item_new_from_stock ("gtk-delete",
						    accel_group);
	gtk_widget_show (menu_delete);
	gtk_container_add (GTK_CONTAINER (menuitem_edit_menu), menu_delete);

	// scratch pad
	menu_scratch = gtk_menu_new();
	gtk_widget_show (menu_scratch);

	menu_item_scratch = gtk_image_menu_item_new_with_mnemonic (_("Scratch pad"));
	gtk_widget_show (menu_item_scratch);
	gtk_menu_item_set_submenu (GTK_MENU_ITEM(menu_item_scratch), menu_scratch);
	gtk_container_add (GTK_CONTAINER (menuitem_edit_menu), menu_item_scratch);
	
	menu_copy_to_scratch = gtk_image_menu_item_new_with_mnemonic (_("Copy to"));
	gtk_widget_show (menu_copy_to_scratch);
	image_menu = gtk_image_new_from_stock ("gtk-copy", GTK_ICON_SIZE_MENU);
	gtk_widget_show (image_menu);
	gtk_image_menu_item_set_image (GTK_IMAGE_MENU_ITEM (menu_copy_to_scratch), image_menu);
	gtk_container_add (GTK_CONTAINER (menu_scratch), menu_copy_to_scratch);	

	menu_paste_from_scratch = gtk_image_menu_item_new_with_mnemonic (_("Paste from"));
	gtk_widget_show (menu_paste_from_scratch);
	image_menu = gtk_image_new_from_stock ("gtk-paste", GTK_ICON_SIZE_MENU);
	gtk_widget_show (image_menu);
	gtk_image_menu_item_set_image (GTK_IMAGE_MENU_ITEM (menu_paste_from_scratch), image_menu);
	gtk_container_add (GTK_CONTAINER (menu_scratch), menu_paste_from_scratch);

	separatormenuitem = gtk_menu_item_new ();
	gtk_widget_show (separatormenuitem);
	gtk_container_add (GTK_CONTAINER (menu_scratch),  separatormenuitem);
	gtk_widget_set_sensitive (separatormenuitem, FALSE);
	
	menu_delete_scratch= gtk_image_menu_item_new_from_stock ("gtk-delete", NULL);
	gtk_widget_show (menu_delete_scratch);
	gtk_container_add (GTK_CONTAINER (menu_scratch), menu_delete_scratch);

	//
	separatormenuitem = gtk_menu_item_new ();
	gtk_widget_show (separatormenuitem);
	gtk_container_add (GTK_CONTAINER (menuitem_edit_menu),  separatormenuitem);
	gtk_widget_set_sensitive (separatormenuitem, FALSE);

	// search & replace
	menu_search =
		gtk_image_menu_item_new_from_stock ("gtk-find", accel_group);
	gtk_widget_show (menu_search);
	gtk_container_add (GTK_CONTAINER (menuitem_edit_menu), menu_search);

	menu_replace =
		gtk_image_menu_item_new_from_stock ("gtk-find-and-replace", accel_group);
	gtk_widget_show (menu_replace);
	gtk_container_add (GTK_CONTAINER (menuitem_edit_menu), menu_replace);

	separatormenuitem = gtk_menu_item_new ();
	gtk_widget_show (separatormenuitem);
	gtk_container_add (GTK_CONTAINER (menuitem_edit_menu),
			   separatormenuitem);
	
	gtk_widget_set_sensitive (separatormenuitem, FALSE);
	menu_preferences = gtk_image_menu_item_new_from_stock ("gtk-preferences",
						    accel_group);
	gtk_widget_show (menu_preferences);
	gtk_container_add (GTK_CONTAINER (menuitem_edit_menu), menu_preferences);
	// /////////////// view menu /////////////////////////////////////////////
	menuitem_view = gtk_menu_item_new_with_mnemonic (_("_View"));
	gtk_widget_show (menuitem_view);
	gtk_container_add (GTK_CONTAINER (menubar_main), menuitem_view);

	menuitem_view_menu = gtk_menu_new ();
	gtk_menu_item_set_submenu (GTK_MENU_ITEM (menuitem_view),
				   menuitem_view_menu);

	menu_item_toolbars = gtk_menu_item_new_with_mnemonic (_("_Toolbars"));
	gtk_widget_show (menu_item_toolbars);
	gtk_container_add (GTK_CONTAINER (menuitem_view_menu), menu_item_toolbars);

	menu_toolbars_submenu = gtk_menu_new ();
	gtk_menu_item_set_submenu (GTK_MENU_ITEM (menu_item_toolbars),
				   menu_toolbars_submenu);

	menu_toolbar =
		gtk_check_menu_item_new_with_mnemonic (_("View main toolbar"));
	gtk_widget_show (menu_toolbar);
	gtk_container_add (GTK_CONTAINER (menu_toolbars_submenu), menu_toolbar);
	gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(menu_toolbar), TRUE );

	menu_secodary_toolbar =  gtk_check_menu_item_new_with_mnemonic (_("View secondary toolbar"));
	gtk_widget_show (menu_secodary_toolbar);
	gtk_container_add (GTK_CONTAINER (menu_toolbars_submenu), menu_secodary_toolbar);
	gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(menu_secodary_toolbar), TRUE );

//{
	/* panels */
	menuitem_panel = gtk_menu_item_new_with_mnemonic (_("_Panels"));
	gtk_widget_show (menuitem_panel);
	gtk_container_add (GTK_CONTAINER (menuitem_view_menu), menuitem_panel);

	menuitem_panel_menu = gtk_menu_new ();
	gtk_menu_item_set_submenu (GTK_MENU_ITEM (menuitem_panel), menuitem_panel_menu);

	menuitem_show_footer_panel = gtk_image_menu_item_new_with_mnemonic (_("Show _footer panel"));
	image_menu = create_pixmap_from_data(show_footer_menu);
	gtk_widget_show(image_menu);
	gtk_image_menu_item_set_image (GTK_IMAGE_MENU_ITEM(menuitem_show_footer_panel),
									image_menu);
	gtk_widget_show (menuitem_show_footer_panel);
	gtk_container_add (GTK_CONTAINER (menuitem_panel_menu), menuitem_show_footer_panel);

	menuitem_hide_footer_panel = gtk_image_menu_item_new_with_mnemonic (_("Hide footer _panel"));
	image_menu = create_pixmap_from_data(hide_footer_menu);
	gtk_widget_show(image_menu);
	gtk_image_menu_item_set_image (GTK_IMAGE_MENU_ITEM(menuitem_hide_footer_panel),
									image_menu);
	gtk_widget_show (menuitem_hide_footer_panel);
	gtk_container_add (GTK_CONTAINER (menuitem_panel_menu), menuitem_hide_footer_panel);

	separatormenuitem = gtk_menu_item_new ();
	gtk_widget_show (separatormenuitem);
	gtk_container_add (GTK_CONTAINER (menuitem_panel_menu), separatormenuitem);
	gtk_widget_set_sensitive (separatormenuitem, FALSE);

	menuitem_show_side_panel = gtk_image_menu_item_new_with_mnemonic (_("_Show side panel"));
	image_menu = create_pixmap_from_data(show_sidebar_menu);
	gtk_widget_show(image_menu);
	gtk_image_menu_item_set_image (GTK_IMAGE_MENU_ITEM(menuitem_show_side_panel),
									image_menu);
	gtk_widget_show (menuitem_show_side_panel);
	gtk_container_add (GTK_CONTAINER (menuitem_panel_menu), menuitem_show_side_panel);

	menuitem_hide_side_panel = gtk_image_menu_item_new_with_mnemonic (_("_Hide side panel"));
	image_menu = create_pixmap_from_data(hide_sidebar_menu);
	gtk_widget_show(image_menu);
	gtk_image_menu_item_set_image (GTK_IMAGE_MENU_ITEM(menuitem_hide_side_panel),
									image_menu);
	gtk_widget_show (menuitem_hide_side_panel);
	gtk_container_add (GTK_CONTAINER (menuitem_panel_menu), menuitem_hide_side_panel);
//}

	// separator
	separatormenuitem = gtk_menu_item_new ();
	gtk_widget_show (separatormenuitem);
	gtk_container_add (GTK_CONTAINER (menuitem_view_menu), separatormenuitem);
	gtk_widget_set_sensitive (separatormenuitem, FALSE);

	menu_zoom_in = 	gtk_image_menu_item_new_from_stock ("gtk-zoom-in", accel_group);
	gtk_widget_show (menu_zoom_in);
	gtk_container_add (GTK_CONTAINER (menuitem_view_menu), menu_zoom_in);

	menu_zoom_out= 	gtk_image_menu_item_new_from_stock ("gtk-zoom-out", accel_group);
	gtk_widget_show (menu_zoom_out);
	gtk_container_add (GTK_CONTAINER (menuitem_view_menu), menu_zoom_out);


	menu_normal_size= 	gtk_image_menu_item_new_from_stock ("gtk-zoom-100", accel_group);
	gtk_widget_show (menu_normal_size);
	gtk_container_add (GTK_CONTAINER (menuitem_view_menu), menu_normal_size);

	// /////////////// document menu /////////////////////////////////////////
	menuitem_document = gtk_menu_item_new_with_mnemonic (_("_Document"));
	gtk_widget_show (menuitem_document);
	gtk_container_add (GTK_CONTAINER (menubar_main), menuitem_document);

	menuitem_document_menu = gtk_menu_new ();
	gtk_menu_item_set_submenu (GTK_MENU_ITEM (menuitem_document),
				   menuitem_document_menu);

	menu_view_line_numbers =
		gtk_check_menu_item_new_with_mnemonic (_("View line _numbers"));
	gtk_widget_show (menu_view_line_numbers);
	gtk_container_add (GTK_CONTAINER (menuitem_document_menu),
			   menu_view_line_numbers);

	menu_view_line_endings =
		gtk_check_menu_item_new_with_mnemonic (_("View line _endings"));
	gtk_widget_show (menu_view_line_endings);
	gtk_container_add (GTK_CONTAINER (menuitem_document_menu),
			   menu_view_line_endings);

	menu_view_white_spaces =
		gtk_check_menu_item_new_with_mnemonic (_("View white _spaces"));
	gtk_widget_show (menu_view_white_spaces);
	gtk_container_add (GTK_CONTAINER (menuitem_document_menu),
			   menu_view_white_spaces);

	menu_view_lines_wrapped =
		gtk_check_menu_item_new_with_mnemonic (_("View lines _wrapped"));
	gtk_widget_show (menu_view_lines_wrapped);
	gtk_container_add (GTK_CONTAINER (menuitem_document_menu),
			   menu_view_lines_wrapped);

	// separator
	separatormenuitem = gtk_menu_item_new ();
	gtk_widget_show (separatormenuitem);
	gtk_container_add (GTK_CONTAINER (menuitem_document_menu), separatormenuitem);
	gtk_widget_set_sensitive (separatormenuitem, FALSE);

	menu_item_autocompletion =
		gtk_check_menu_item_new_with_mnemonic (_("Enable _autocompletion"));
	gtk_widget_show (menu_item_autocompletion);
	gtk_container_add (GTK_CONTAINER (menuitem_document_menu),
			   menu_item_autocompletion);

	menu_item_folding =
		gtk_check_menu_item_new_with_mnemonic (_("Enable _folding"));
	gtk_widget_show (menu_item_folding);
	gtk_container_add (GTK_CONTAINER (menuitem_document_menu),
			   menu_item_folding);

	// separator
	separatormenuitem = gtk_menu_item_new ();
	gtk_widget_show (separatormenuitem);
	gtk_container_add (GTK_CONTAINER (menuitem_document_menu), separatormenuitem);
	gtk_widget_set_sensitive (separatormenuitem, FALSE);

	// ~~~~~~~~~~~~~~~~~~~~~~~~~ eols ~~~~~~~~~~~~~~~~~~~~
	menuitem_eol_mode = gtk_menu_item_new_with_mnemonic (_("Set EOL _mode"));
	gtk_widget_show (menuitem_eol_mode);
	gtk_container_add (GTK_CONTAINER (menuitem_document_menu),  menuitem_eol_mode);

	menuitem_eol_mode_menu = gtk_menu_new ();
	gtk_menu_item_set_submenu (GTK_MENU_ITEM (menuitem_eol_mode),   menuitem_eol_mode_menu);

	////////////////////// DYNAMIC EOL MENUS /////////////////////////////////////////
	menuitem_eol_mode_cr =
		gtk_radio_menu_item_new_with_mnemonic (eol_menus_list,_("Mac (C_R)"));
	eol_menus_list =  gtk_radio_menu_item_get_group (GTK_RADIO_MENU_ITEM (menuitem_eol_mode_cr));
	gtk_widget_show (menuitem_eol_mode_cr);
	gtk_container_add (GTK_CONTAINER (menuitem_eol_mode_menu), menuitem_eol_mode_cr);

	menuitem_eol_mode_lf =
		gtk_radio_menu_item_new_with_mnemonic (eol_menus_list,_("Unix (L_F)"));
	eol_menus_list =  gtk_radio_menu_item_get_group (GTK_RADIO_MENU_ITEM (menuitem_eol_mode_lf));
	gtk_widget_show (menuitem_eol_mode_lf);
	gtk_container_add (GTK_CONTAINER (menuitem_eol_mode_menu), menuitem_eol_mode_lf);

	menuitem_eol_mode_crlf =
		gtk_radio_menu_item_new_with_mnemonic (eol_menus_list, _("Dos / Windows (_CR/LF)"));
	eol_menus_list =  gtk_radio_menu_item_get_group (GTK_RADIO_MENU_ITEM (menuitem_eol_mode_crlf));
	gtk_widget_show (menuitem_eol_mode_crlf);
	gtk_container_add (GTK_CONTAINER (menuitem_eol_mode_menu), menuitem_eol_mode_crlf);

	// separator
	separatormenuitem = gtk_menu_item_new ();
	gtk_widget_show (separatormenuitem);
	gtk_container_add (GTK_CONTAINER (menuitem_eol_mode_menu), separatormenuitem);
	gtk_widget_set_sensitive (separatormenuitem, FALSE);
	////////////////////////// Convert EOLS ///////////////////////////////////////
	menuitem_eol_convert = gtk_menu_item_new_with_mnemonic (_("Convert E_OLs"));
	gtk_widget_show (menuitem_eol_convert);
	gtk_container_add (GTK_CONTAINER (menuitem_eol_mode_menu),  menuitem_eol_convert);

	menuitem_eol_convert_menu = gtk_menu_new ();
	gtk_menu_item_set_submenu (GTK_MENU_ITEM (menuitem_eol_convert),   menuitem_eol_convert_menu);

	menuitem_eol_convert_cr =
		gtk_radio_menu_item_new_with_mnemonic (eol_menus_list,_("To Mac (C_R)"));
	eol_menus_list =  gtk_radio_menu_item_get_group (GTK_RADIO_MENU_ITEM (menuitem_eol_convert_cr));
	gtk_widget_show (menuitem_eol_convert_cr);
	gtk_container_add (GTK_CONTAINER (menuitem_eol_convert_menu), menuitem_eol_convert_cr);

	menuitem_eol_convert_lf =
		gtk_radio_menu_item_new_with_mnemonic (eol_menus_list,_("To Unix (L_F)"));
	eol_menus_list =  gtk_radio_menu_item_get_group (GTK_RADIO_MENU_ITEM (menuitem_eol_convert_lf));
	gtk_widget_show (menuitem_eol_convert_lf);
	gtk_container_add (GTK_CONTAINER (menuitem_eol_convert_menu), menuitem_eol_convert_lf);

	menuitem_eol_convert_crlf =
		gtk_radio_menu_item_new_with_mnemonic (eol_menus_list, _("To Dos / Windows (_CR/LF)"));
	eol_menus_list =  gtk_radio_menu_item_get_group (GTK_RADIO_MENU_ITEM (menuitem_eol_convert_crlf));
	gtk_widget_show (menuitem_eol_convert_crlf);
	gtk_container_add (GTK_CONTAINER (menuitem_eol_convert_menu), menuitem_eol_convert_crlf);

	//////////   folding /////////////////////////////////
	menuitem_folding = gtk_menu_item_new_with_mnemonic (_("Fol_ding"));
	gtk_widget_show (menuitem_folding);
	gtk_container_add (GTK_CONTAINER (menuitem_document_menu),
			   menuitem_folding);

	menuitem_folding_menu = gtk_menu_new ();
	gtk_menu_item_set_submenu (GTK_MENU_ITEM (menuitem_folding),
				   menuitem_folding_menu);

	menuitem_fonding_foldall =
		gtk_menu_item_new_with_mnemonic (_("_Fold all"));
	gtk_widget_show (menuitem_fonding_foldall);
	gtk_container_add (GTK_CONTAINER (menuitem_folding_menu),
			   menuitem_fonding_foldall);

	menuitem_folding_unfoldall =
		gtk_menu_item_new_with_mnemonic (_("_Unfold all"));
	gtk_widget_show (menuitem_folding_unfoldall);
	gtk_container_add (GTK_CONTAINER (menuitem_folding_menu),
			   menuitem_folding_unfoldall);

	menuitem_filetype = gtk_menu_item_new_with_mnemonic (_("File t_ype"));
	gtk_widget_show (menuitem_filetype);
	gtk_container_add (GTK_CONTAINER (menuitem_document_menu),
			   menuitem_filetype);

	menuitem_filetype_menu = gtk_menu_new ();
	gtk_menu_item_set_submenu (GTK_MENU_ITEM (menuitem_filetype),  menuitem_filetype_menu);
	cssed_window_set_filetypes_menu  (windowobj, menuitem_filetype_menu);
	
	////////////////////////////////////////////////////////////
	create_and_register_css2_file_type (windowobj);
	create_and_register_text_file_type (windowobj);
	create_and_register_c_file_type (windowobj);
	create_and_register_hypertext_file_type (windowobj);
	create_and_register_phpscript_file_type (windowobj);
	create_and_register_python_file_type (windowobj);
	create_and_register_perl_file_type (windowobj);
	create_and_register_sql_file_type (windowobj);
	create_and_register_xml_file_type (windowobj);
	create_and_register_sh_file_type (windowobj);
	create_and_register_make_file_type (windowobj);
	create_and_register_diff_file_type (windowobj);
	create_and_register_js_file_type (windowobj);
	create_and_register_java_file_type (windowobj);
	create_and_register_conf_file_type (windowobj);
	create_and_register_asm_file_type (windowobj);
	////////////////////////////////////////////////////////////

	// encoding
	menuitem_encoding = gtk_menu_item_new_with_mnemonic (_("_Force Encoding"));
	gtk_widget_show (menuitem_encoding);
	gtk_container_add (GTK_CONTAINER (menuitem_document_menu),
			   menuitem_encoding);

	menuitem_encoding_menu = gtk_menu_new ();
	gtk_menu_item_set_submenu (GTK_MENU_ITEM (menuitem_encoding),
				   menuitem_encoding_menu);

	menuitem_encoding_default =
		gtk_menu_item_new_with_mnemonic (_("_Default"));
	gtk_widget_show (menuitem_encoding_default);
	gtk_container_add (GTK_CONTAINER (menuitem_encoding_menu),
			   menuitem_encoding_default);

	menuitem_encoding_utf8 =
		gtk_menu_item_new_with_mnemonic ("_UTF8");
	gtk_widget_show (menuitem_encoding_utf8);
	gtk_container_add (GTK_CONTAINER (menuitem_encoding_menu),
			   menuitem_encoding_utf8);

	menuitem_encoding_dbcs =
		gtk_menu_item_new_with_mnemonic ("D_BCS");
	gtk_widget_show (menuitem_encoding_dbcs);
	gtk_container_add (GTK_CONTAINER (menuitem_encoding_menu),
			   menuitem_encoding_dbcs);
	// charset
	menuitem_charset = gtk_menu_item_new_with_mnemonic (_("Charac_ter set"));
	gtk_widget_show (menuitem_charset);
	gtk_container_add (GTK_CONTAINER (menuitem_document_menu), menuitem_charset);

	menuitem_charset_menu = gtk_menu_new ();
	gtk_menu_item_set_submenu (GTK_MENU_ITEM (menuitem_charset), menuitem_charset_menu);

	menuitem_charset_ansi =	gtk_menu_item_new_with_mnemonic (_("_ANSI"));
	gtk_widget_show (menuitem_charset_ansi);
	gtk_container_add (GTK_CONTAINER (menuitem_charset_menu),  menuitem_charset_ansi);

	menuitem_charset_easteurope =	gtk_menu_item_new_with_mnemonic (_("_East Europe"));
	gtk_widget_show (menuitem_charset_easteurope);
	gtk_container_add (GTK_CONTAINER (menuitem_charset_menu),  menuitem_charset_easteurope);

	menuitem_charset_gb2312 =	gtk_menu_item_new_with_mnemonic (_("_GB 2312"));
	gtk_widget_show (menuitem_charset_gb2312);
	gtk_container_add (GTK_CONTAINER (menuitem_charset_menu),  menuitem_charset_gb2312);

	menuitem_charset_hangul =	gtk_menu_item_new_with_mnemonic (_("_Hangul"));
	gtk_widget_show (menuitem_charset_hangul);
	gtk_container_add (GTK_CONTAINER (menuitem_charset_menu),  menuitem_charset_hangul);

	menuitem_charset_shiftjis =	gtk_menu_item_new_with_mnemonic (_("_Shift JIS"));
	gtk_widget_show (menuitem_charset_shiftjis);
	gtk_container_add (GTK_CONTAINER (menuitem_charset_menu),  menuitem_charset_shiftjis);

#ifdef WITH_SPLITTER_WINDOW
	// Add menu Split view -> Hor, Ver, --, Unsplit

	menuitem_split = gtk_menu_item_new_with_mnemonic (_("Split View"));
	gtk_widget_show (menuitem_split);
	gtk_container_add (GTK_CONTAINER (menuitem_document_menu),
			   menuitem_split);

	menuitem_split_menu = gtk_menu_new ();
	gtk_menu_item_set_submenu (GTK_MENU_ITEM (menuitem_split),
				   menuitem_split_menu);

	menuitem_split_hor =
		gtk_menu_item_new_with_mnemonic (_("_Horizontally"));
	gtk_widget_show (menuitem_split_hor);
	gtk_container_add (GTK_CONTAINER (menuitem_split_menu),
			   menuitem_split_hor);

	menuitem_split_ver =
		gtk_menu_item_new_with_mnemonic (_("_Vertically"));
	gtk_widget_show (menuitem_split_ver);
	gtk_container_add (GTK_CONTAINER (menuitem_split_menu),
			   menuitem_split_ver);
			   
	// separator
	separatormenuitem = gtk_menu_item_new ();
	gtk_widget_show (separatormenuitem);
	gtk_container_add (GTK_CONTAINER (menuitem_split_menu), separatormenuitem);
	gtk_widget_set_sensitive (separatormenuitem, FALSE);
	
	menuitem_split_none =
		gtk_menu_item_new_with_mnemonic (_("Unsplit"));
	gtk_widget_show (menuitem_split_none);
	gtk_container_add (GTK_CONTAINER (menuitem_split_menu),
			   menuitem_split_none);
	
#endif

	// separator
	separatormenuitem = gtk_menu_item_new ();
	gtk_widget_show (separatormenuitem);
	gtk_container_add (GTK_CONTAINER (menuitem_document_menu), separatormenuitem);
	gtk_widget_set_sensitive (separatormenuitem, FALSE);

	// change font
	menu_item_font =
		gtk_menu_item_new_with_mnemonic (_("_Change font"));
	gtk_widget_show (menu_item_font);
	gtk_container_add (GTK_CONTAINER (menuitem_document_menu),
			   menu_item_font);

	// ************** flag current document *******************
	menu_item_flag =
		gtk_menu_item_new_with_mnemonic (_("Tab back_ground colour"));
	gtk_widget_show (menu_item_flag);
	gtk_container_add (GTK_CONTAINER (menuitem_document_menu), menu_item_flag);

	menu_item_flag_menu = gtk_menu_new ();
	gtk_menu_item_set_submenu (GTK_MENU_ITEM (menu_item_flag), menu_item_flag_menu);

	menu_item_flag_clear =
		gtk_menu_item_new_with_mnemonic (_("Revert to original _colour"));
	gtk_widget_show (menu_item_flag_clear);
	gtk_container_add (GTK_CONTAINER (menu_item_flag_menu), menu_item_flag_clear);

	// separator
	separatormenuitem = gtk_menu_item_new ();
	gtk_widget_show (separatormenuitem);
	gtk_container_add (GTK_CONTAINER (menu_item_flag_menu), separatormenuitem);
	gtk_widget_set_sensitive (separatormenuitem, FALSE);

	menu_item_flag_red =
		gtk_menu_item_new_with_label (_("<span background='red'>   </span> Red"));
	gtk_label_set_use_markup(GTK_LABEL(GTK_BIN(menu_item_flag_red)->child), TRUE);
	gtk_widget_show (menu_item_flag_red);
	gtk_container_add (GTK_CONTAINER (menu_item_flag_menu), menu_item_flag_red);

	menu_item_flag_white =
		gtk_menu_item_new_with_label (_("<span foreground='black' background='white'>   </span> White"));
	gtk_label_set_use_markup(GTK_LABEL(GTK_BIN(menu_item_flag_white)->child), TRUE);
	gtk_widget_show (menu_item_flag_white);
	gtk_container_add (GTK_CONTAINER (menu_item_flag_menu), menu_item_flag_white);

	menu_item_flag_blue =
		gtk_menu_item_new_with_label (_("<span foreground='white' background='blue'>   </span> Blue"));
	gtk_label_set_use_markup(GTK_LABEL(GTK_BIN(menu_item_flag_blue)->child), TRUE);
	gtk_widget_show (menu_item_flag_blue);
	gtk_container_add (GTK_CONTAINER (menu_item_flag_menu), menu_item_flag_blue);

	menu_item_flag_yellow =
		gtk_menu_item_new_with_label (_("<span foreground='black' background='yellow'>   </span> Yellow"));
	gtk_label_set_use_markup(GTK_LABEL(GTK_BIN(menu_item_flag_yellow)->child), TRUE);
	gtk_widget_show (menu_item_flag_yellow);
	gtk_container_add (GTK_CONTAINER (menu_item_flag_menu), menu_item_flag_yellow);

	menu_item_flag_green =
		gtk_menu_item_new_with_label (_("<span foreground='black' background='green'>   </span> Green"));
	gtk_label_set_use_markup(GTK_LABEL(GTK_BIN(menu_item_flag_green)->child), TRUE);
	gtk_widget_show (menu_item_flag_green);
	gtk_container_add (GTK_CONTAINER (menu_item_flag_menu), menu_item_flag_green);
	// document goto
	separatormenuitem = gtk_menu_item_new ();
	gtk_widget_show (separatormenuitem);
	gtk_container_add (GTK_CONTAINER (menuitem_document_menu), separatormenuitem);
	gtk_widget_set_sensitive (separatormenuitem, FALSE);

	menu_item_document_goto = gtk_menu_item_new_with_mnemonic (_("G_o To"));
	gtk_widget_show (menu_item_document_goto);
	gtk_container_add (GTK_CONTAINER (menuitem_document_menu), menu_item_document_goto);

	menu_item_document_goto_menu = gtk_menu_new ();
	gtk_menu_item_set_submenu (GTK_MENU_ITEM (menu_item_document_goto), menu_item_document_goto_menu);

	menu_item_document_goto_start =
		gtk_menu_item_new_with_mnemonic (_("_Start of document"));
	gtk_widget_show (menu_item_document_goto_start);
	gtk_container_add (GTK_CONTAINER (menu_item_document_goto_menu), menu_item_document_goto_start);

	menu_item_document_goto_end =
		gtk_menu_item_new_with_mnemonic (_("_End of document"));
	gtk_widget_show (menu_item_document_goto_end);
	gtk_container_add (GTK_CONTAINER (menu_item_document_goto_menu), menu_item_document_goto_end);

	menu_item_document_goto_line =
	gtk_menu_item_new_with_mnemonic (_("_Line Number"));
	gtk_widget_show (menu_item_document_goto_line);
	gtk_container_add (GTK_CONTAINER (menu_item_document_goto_menu), menu_item_document_goto_line);

	// ////////////  CSS dialogs menu created  in //////
	// ////////////  cssdialogs-menu.c /////////////////
	css_dialogs_item = gtk_menu_item_new_with_mnemonic (_("_CSS"));
    gtk_widget_show (css_dialogs_item);

	css_dialogs_submenu = gtk_menu_item_new_with_mnemonic (_("Dialogs"));
 	gtk_widget_show (css_dialogs_submenu);

	css_menu_accels = gtk_accel_group_new();
	css_menu = create_css_dialogs_menu	(windowobj, css_menu_accels );
	gtk_menu_item_set_submenu (GTK_MENU_ITEM (css_dialogs_submenu), css_menu);

	css_dialogs_mainmenu = gtk_menu_new();
	gtk_container_add (GTK_CONTAINER (css_dialogs_mainmenu),
						css_dialogs_submenu);

	// separator
	separatormenuitem = gtk_menu_item_new ();
	gtk_widget_show (separatormenuitem);
	gtk_container_add (GTK_CONTAINER (css_dialogs_mainmenu), separatormenuitem);
	gtk_widget_set_sensitive (separatormenuitem, FALSE);

	menuitem_selector_wizard = gtk_image_menu_item_new_with_mnemonic (_("_Selector wizard"));
	image_menu = create_pixmap_from_data(selector_wizard_menu);
	gtk_widget_show(image_menu);
	gtk_image_menu_item_set_image (GTK_IMAGE_MENU_ITEM(menuitem_selector_wizard),
									image_menu);
	gtk_widget_show (menuitem_selector_wizard);
	gtk_container_add (GTK_CONTAINER (css_dialogs_mainmenu), menuitem_selector_wizard);

	menuitem_color_wizard = gtk_image_menu_item_new_with_mnemonic (_("_Color wizard"));
	image_menu = create_pixmap_from_data(color_wizard_menu);
	gtk_widget_show(image_menu);
	gtk_image_menu_item_set_image (GTK_IMAGE_MENU_ITEM(menuitem_color_wizard),
									image_menu);
	gtk_widget_show (menuitem_color_wizard);
	gtk_container_add (GTK_CONTAINER (css_dialogs_mainmenu), menuitem_color_wizard);

	menuitem_box_wizard = gtk_image_menu_item_new_with_mnemonic (_("_Box properties wizard"));
	image_menu = create_pixmap_from_data(dlg_box_menu);
	gtk_widget_show(image_menu);
	gtk_image_menu_item_set_image (GTK_IMAGE_MENU_ITEM(menuitem_box_wizard),
									image_menu);
	gtk_widget_show (menuitem_box_wizard);
	gtk_container_add (GTK_CONTAINER (css_dialogs_mainmenu), menuitem_box_wizard);

	/* validator - FIXME have been recently moved, the var names should change */
	separatormenuitem = gtk_menu_item_new ();
	gtk_widget_show (separatormenuitem);
	gtk_container_add (GTK_CONTAINER (css_dialogs_mainmenu), separatormenuitem);
	gtk_widget_set_sensitive (separatormenuitem, FALSE);

	menu_utils_validate = gtk_image_menu_item_new_with_mnemonic (_("_Validate and dump"));
	image_menu = create_pixmap_from_data(validate_dump_menu);
	gtk_widget_show(image_menu);
	gtk_image_menu_item_set_image (GTK_IMAGE_MENU_ITEM(menu_utils_validate), image_menu);
	gtk_widget_show (menu_utils_validate);
	gtk_container_add (GTK_CONTAINER (css_dialogs_mainmenu), menu_utils_validate);

	menu_utils_validate_only = gtk_image_menu_item_new_with_mnemonic (_("Validate _only"));
	image_menu = create_pixmap_from_data(validate_menu);
	gtk_widget_show(image_menu);
	gtk_image_menu_item_set_image (GTK_IMAGE_MENU_ITEM(menu_utils_validate_only), image_menu);
	gtk_widget_show (menu_utils_validate_only);
	gtk_container_add (GTK_CONTAINER (css_dialogs_mainmenu), menu_utils_validate_only);

	menu_utils_scan_selector =
		gtk_image_menu_item_new_with_mnemonic (_("_Scan selector"));
	image_menu = create_pixmap_from_data(scan_selector_menu);
	gtk_widget_show(image_menu);
	gtk_image_menu_item_set_image (GTK_IMAGE_MENU_ITEM(menu_utils_scan_selector),
									image_menu);
	gtk_widget_show (menu_utils_scan_selector);
	gtk_container_add (GTK_CONTAINER (css_dialogs_mainmenu),
			   menu_utils_scan_selector);

	gtk_menu_item_set_submenu (GTK_MENU_ITEM (css_dialogs_item), css_dialogs_mainmenu);
	gtk_menu_bar_append(menubar_main,css_dialogs_item);

	// ////////////////////////// Utils //////////////////////////////
	menuitem_utils = gtk_menu_item_new_with_mnemonic (_("_Tools"));
	gtk_widget_show (menuitem_utils);
	gtk_container_add (GTK_CONTAINER (menubar_main), menuitem_utils);

	menuitem_utils_menu = gtk_menu_new ();
	gtk_menu_item_set_submenu (GTK_MENU_ITEM (menuitem_utils),
				   menuitem_utils_menu);

	menu_utils_doc_info = gtk_image_menu_item_new_with_mnemonic (_("_Doc Info"));
	image_menu = create_pixmap_from_data(document_info_menu);
	gtk_widget_show(image_menu);
	gtk_image_menu_item_set_image (GTK_IMAGE_MENU_ITEM(menu_utils_doc_info),
									image_menu);
	gtk_widget_show (menu_utils_doc_info);
	gtk_container_add (GTK_CONTAINER (menuitem_utils_menu), menu_utils_doc_info);

	menu_utils_autocomp  = gtk_menu_item_new_with_mnemonic (_("Show _autocompletion"));
	gtk_widget_show (menu_utils_autocomp);
	gtk_container_add (GTK_CONTAINER (menuitem_utils_menu),
			   menu_utils_autocomp);

	menu_utils_cleanoutput = gtk_image_menu_item_new_with_mnemonic (_("_Clean program output"));
	image_menu = gtk_image_new_from_stock( "gtk-clear", GTK_ICON_SIZE_MENU );
	gtk_widget_show(image_menu);
	gtk_image_menu_item_set_image (GTK_IMAGE_MENU_ITEM(menu_utils_cleanoutput),
									image_menu);
	gtk_widget_show (menu_utils_cleanoutput);
	gtk_container_add (GTK_CONTAINER (menuitem_utils_menu),
						menu_utils_cleanoutput);

	// separator to document list
	separatormenuitem = gtk_menu_item_new ();
	gtk_widget_show (separatormenuitem);
	gtk_container_add (GTK_CONTAINER (menuitem_utils_menu), separatormenuitem);
	gtk_widget_set_sensitive (separatormenuitem, FALSE);
	
	menu_item_document_list  =	gtk_menu_item_new_with_mnemonic (_("Document _list"));
	gtk_widget_show (menu_item_document_list);
	gtk_container_add (GTK_CONTAINER (menuitem_utils_menu), menu_item_document_list);
	
	menu_item_document_list_menu = gtk_menu_new ();
	gtk_menu_item_set_submenu (GTK_MENU_ITEM (menu_item_document_list), menu_item_document_list_menu);

	menu_item_document_list_current  =	gtk_menu_item_new_with_mnemonic (_("_Opened documents"));
	gtk_widget_show (menu_item_document_list_current);
	gtk_container_add (GTK_CONTAINER (menu_item_document_list_menu), menu_item_document_list_current);

	menu_item_document_list_open =	gtk_menu_item_new_with_mnemonic (_("Open from _file"));
	gtk_widget_show (menu_item_document_list_open);
	gtk_container_add (GTK_CONTAINER (menu_item_document_list_menu), menu_item_document_list_open);

	menu_item_document_list_new  =	gtk_menu_item_new_with_mnemonic (_("_Blank document list"));
	gtk_widget_show (menu_item_document_list_new);
	gtk_container_add (GTK_CONTAINER (menu_item_document_list_menu), menu_item_document_list_new);
	// plugins menu ////////////////////////////////////////////////////////////
#ifdef WITH_PLUGINS
	if( g_module_supported() ){
		menuitem_plugins = gtk_menu_item_new_with_mnemonic (_("_Plugins"));
		gtk_widget_show (menuitem_plugins);
		gtk_container_add (GTK_CONTAINER (menubar_main), menuitem_plugins);

		menuitem_plugins_menu = gtk_menu_new ();
		gtk_menu_item_set_submenu (GTK_MENU_ITEM (menuitem_plugins),
				   menuitem_plugins_menu);

		menuitem_plugins_load = gtk_menu_item_new_with_mnemonic (_("_View plugins"));
		gtk_widget_show (menuitem_plugins_load);
		gtk_container_add (GTK_CONTAINER (menuitem_plugins_menu),
						menuitem_plugins_load);

		// separator
		separatormenuitem = gtk_menu_item_new ();
		gtk_widget_show (separatormenuitem);
		gtk_container_add (GTK_CONTAINER (menuitem_plugins_menu), separatormenuitem);
		gtk_widget_set_sensitive (separatormenuitem, FALSE);
	}else{ // keep compiler happy
		menuitem_plugins_menu = menuitem_plugins_load = 0;
	}
#endif
	// help menu ///////////////////////////////////////////////////////////////
	menuitem_ayuda = gtk_menu_item_new_with_mnemonic (_("_Help"));
	gtk_widget_show (menuitem_ayuda);
	gtk_container_add (GTK_CONTAINER (menubar_main), menuitem_ayuda);

	menuitem_ayuda_menu = gtk_menu_new ();
	gtk_menu_item_set_submenu (GTK_MENU_ITEM (menuitem_ayuda),
				   menuitem_ayuda_menu);

/*  For future use, don't delete me please ;-) */
	menu_cssed_help = gtk_menu_item_new_with_mnemonic (_("Cssed _help"));
	gtk_widget_show (menu_cssed_help);
	gtk_container_add (GTK_CONTAINER (menuitem_ayuda_menu), menu_cssed_help);

	menu_about = gtk_menu_item_new_with_mnemonic (_("_About"));
	gtk_widget_show (menu_about);
	gtk_container_add (GTK_CONTAINER (menuitem_ayuda_menu), menu_about);

#ifdef WITH_HELP_MENUS
	// separator
	separatormenuitem = gtk_menu_item_new ();
	gtk_widget_show (separatormenuitem);
	gtk_container_add (GTK_CONTAINER (menuitem_ayuda_menu), separatormenuitem);
	gtk_widget_set_sensitive (separatormenuitem, FALSE);

	menuitem_cssed_website = gtk_menu_item_new_with_mnemonic (_("Cssed _web site"));
	gtk_widget_show (menuitem_cssed_website);
	gtk_container_add (GTK_CONTAINER (menuitem_ayuda_menu), menuitem_cssed_website);

	menuitem_cssed_website_menu = gtk_menu_new();
	gtk_menu_item_set_submenu (GTK_MENU_ITEM (menuitem_cssed_website), menuitem_cssed_website_menu);

#ifdef DARWIN
	menuitem_cssed_website_with_safari = gtk_menu_item_new_with_mnemonic (_("With _Safari"));
	gtk_widget_show (menuitem_cssed_website_with_safari);
	gtk_container_add (GTK_CONTAINER (menuitem_cssed_website_menu), menuitem_cssed_website_with_safari);

	menuitem_cssed_website_with_firefox = gtk_menu_item_new_with_mnemonic (_("With _Firefox"));
	gtk_widget_show (menuitem_cssed_website_with_firefox);
	gtk_container_add (GTK_CONTAINER (menuitem_cssed_website_menu), menuitem_cssed_website_with_firefox);

	// separator
	separatormenuitem = gtk_menu_item_new ();
	gtk_widget_show (separatormenuitem);
	gtk_container_add (GTK_CONTAINER (menuitem_cssed_website_menu), separatormenuitem);
	gtk_widget_set_sensitive (separatormenuitem, FALSE);
#endif

	menuitem_cssed_website_with_mozilla = gtk_menu_item_new_with_mnemonic (_("With _Mozilla"));
	gtk_widget_show (menuitem_cssed_website_with_mozilla);
	gtk_container_add (GTK_CONTAINER (menuitem_cssed_website_menu), menuitem_cssed_website_with_mozilla);

	menuitem_cssed_website_with_galeon = gtk_menu_item_new_with_mnemonic (_("With G_aleon"));
	gtk_widget_show (menuitem_cssed_website_with_galeon);
	gtk_container_add (GTK_CONTAINER (menuitem_cssed_website_menu), menuitem_cssed_website_with_galeon);

	menuitem_cssed_website_with_dillo = gtk_menu_item_new_with_mnemonic (_("With _Dillo"));
	gtk_widget_show (menuitem_cssed_website_with_dillo);
	gtk_container_add (GTK_CONTAINER (menuitem_cssed_website_menu), menuitem_cssed_website_with_dillo);

	menuitem_cssed_website_with_gnome = gtk_menu_item_new_with_mnemonic (_("With _Gnome"));
	gtk_widget_show (menuitem_cssed_website_with_gnome);
	gtk_container_add (GTK_CONTAINER (menuitem_cssed_website_menu), menuitem_cssed_website_with_gnome);

	menuitem_cssed_doc_online = gtk_menu_item_new_with_mnemonic (_("Cssed _documentation on line"));
	gtk_widget_show (menuitem_cssed_doc_online);
	gtk_container_add (GTK_CONTAINER (menuitem_ayuda_menu), menuitem_cssed_doc_online);

	menuitem_cssed_doc_online_menu = gtk_menu_new();
	gtk_menu_item_set_submenu (GTK_MENU_ITEM (menuitem_cssed_doc_online), menuitem_cssed_doc_online_menu);

#ifdef DARWIN
	menuitem_cssed_doc_online_with_safari = gtk_menu_item_new_with_mnemonic (_("With _Safari"));
	gtk_widget_show (menuitem_cssed_doc_online_with_safari);
	gtk_container_add (GTK_CONTAINER (menuitem_cssed_doc_online_menu), menuitem_cssed_doc_online_with_safari);

	menuitem_cssed_doc_online_with_firefox = gtk_menu_item_new_with_mnemonic (_("With _Firefox"));
	gtk_widget_show (menuitem_cssed_doc_online_with_firefox);
	gtk_container_add (GTK_CONTAINER (menuitem_cssed_doc_online_menu), menuitem_cssed_doc_online_with_firefox);

	// separator
	separatormenuitem = gtk_menu_item_new ();
	gtk_widget_show (separatormenuitem);
	gtk_container_add (GTK_CONTAINER (menuitem_cssed_doc_online_menu), separatormenuitem);
	gtk_widget_set_sensitive (separatormenuitem, FALSE);
#endif

	menuitem_cssed_doc_online_with_mozilla = gtk_menu_item_new_with_mnemonic (_("With _Mozilla"));
	gtk_widget_show (menuitem_cssed_doc_online_with_mozilla);
	gtk_container_add (GTK_CONTAINER (menuitem_cssed_doc_online_menu), menuitem_cssed_doc_online_with_mozilla);

	menuitem_cssed_doc_online_with_galeon = gtk_menu_item_new_with_mnemonic (_("With G_aleon"));
	gtk_widget_show (menuitem_cssed_doc_online_with_galeon);
	gtk_container_add (GTK_CONTAINER (menuitem_cssed_doc_online_menu), menuitem_cssed_doc_online_with_galeon);

	menuitem_cssed_doc_online_with_dillo = gtk_menu_item_new_with_mnemonic (_("With _Dillo"));
	gtk_widget_show (menuitem_cssed_doc_online_with_dillo);
	gtk_container_add (GTK_CONTAINER (menuitem_cssed_doc_online_menu), menuitem_cssed_doc_online_with_dillo);

	menuitem_cssed_doc_online_with_gnome = gtk_menu_item_new_with_mnemonic (_("With _Gnome"));
	gtk_widget_show (menuitem_cssed_doc_online_with_gnome);
	gtk_container_add (GTK_CONTAINER (menuitem_cssed_doc_online_menu), menuitem_cssed_doc_online_with_gnome);
#endif /* #ifdef WITH_HELP_MENUS */

	//  main toolbar ///////////////////////////////////////////////////////////
	toolbar_main = gtk_toolbar_new ();
	gtk_widget_show (toolbar_main);
	gtk_box_pack_start (GTK_BOX (vbox_main), toolbar_main, FALSE, FALSE, 0);
	gtk_toolbar_set_style (GTK_TOOLBAR (toolbar_main), GTK_TOOLBAR_ICONS);

	button_toolbar_new =
		gtk_toolbar_insert_stock (GTK_TOOLBAR (toolbar_main),
					  "gtk-new", _("New"), NULL, NULL, NULL,
					  -1);
	gtk_widget_show (button_toolbar_new);

	button_toolbar_open =
		gtk_toolbar_insert_stock (GTK_TOOLBAR (toolbar_main),
					  "gtk-open", _("Open..."), NULL, NULL, NULL,
					  -1);
	gtk_widget_show (button_toolbar_open);

	button_toolbar_save =
		gtk_toolbar_insert_stock (GTK_TOOLBAR (toolbar_main),
					  "gtk-save", _("Save"), NULL, NULL, NULL,
					  -1);
	gtk_widget_show (button_toolbar_save); // initiall state insensitive
	gtk_widget_set_sensitive( GTK_WIDGET( button_toolbar_save ), FALSE );

	button_toolbar_save_as =
		gtk_toolbar_insert_stock (GTK_TOOLBAR (toolbar_main),
					  "gtk-save-as", _("Save As..."), NULL, NULL,
					  NULL, -1);
	gtk_widget_show (button_toolbar_save_as);

	button_toolbar_close =
		gtk_toolbar_insert_stock (GTK_TOOLBAR (toolbar_main),
					  "gtk-close", _("Close"), NULL, NULL, NULL,
					  -1);
	gtk_widget_show (button_toolbar_close);

	gtk_toolbar_append_space (GTK_TOOLBAR (toolbar_main));

	button_toolbar_cut =
		gtk_toolbar_insert_stock (GTK_TOOLBAR (toolbar_main),
					  "gtk-cut", _("Cut"), NULL, NULL, NULL,
					  -1);
	gtk_widget_show (button_toolbar_cut);

	button_toolbar_copy =
		gtk_toolbar_insert_stock (GTK_TOOLBAR (toolbar_main),
					  "gtk-copy", _("Copy"), NULL, NULL, NULL,
					  -1);
	gtk_widget_show (button_toolbar_copy);

	button_toolbar_paste =
		gtk_toolbar_insert_stock (GTK_TOOLBAR (toolbar_main),
					  "gtk-paste", _("Paste"), NULL, NULL, NULL,
					  -1);
	gtk_widget_show (button_toolbar_paste);

	gtk_toolbar_append_space (GTK_TOOLBAR (toolbar_main));

	button_toolbar_search =
		gtk_toolbar_insert_stock (GTK_TOOLBAR (toolbar_main),
					  "gtk-find", _("Find..."), NULL, NULL, NULL,
					  -1);
	gtk_widget_show (button_toolbar_search);

	button_toolbar_replace =
		gtk_toolbar_insert_stock (GTK_TOOLBAR (toolbar_main),
					  "gtk-find-and-replace", _("Replace..."), NULL,
					  NULL, NULL, -1);
	gtk_widget_show (button_toolbar_replace);

	gtk_toolbar_append_space (GTK_TOOLBAR (toolbar_main));

	button_undo =
		gtk_toolbar_insert_stock (GTK_TOOLBAR (toolbar_main),
					  "gtk-undo", _("Undo"), NULL, NULL, NULL,
					  -1);
	gtk_widget_show (button_undo);

	button_redo =
		gtk_toolbar_insert_stock (GTK_TOOLBAR (toolbar_main),
					  "gtk-redo", _("Redo"), NULL, NULL, NULL,
					  -1);
	gtk_widget_show (button_redo);

	gtk_toolbar_append_space (GTK_TOOLBAR (toolbar_main));

	// next prev for bookmarks
	button_prev =
		gtk_toolbar_insert_stock (GTK_TOOLBAR (toolbar_main),
					  "gtk-go-back", _("Previous Bookmark"), NULL, NULL,
					  NULL, -1);
	gtk_widget_show (button_prev);

	button_next =
		gtk_toolbar_insert_stock (GTK_TOOLBAR (toolbar_main),
					  "gtk-go-forward", _("Next Bookmark"), NULL, NULL,
					  NULL, -1);
	gtk_widget_show (button_next);

	gtk_toolbar_append_space (GTK_TOOLBAR (toolbar_main));

	//  zoom in toolbar //////////
	/*
	button_zoom_in =
		gtk_toolbar_insert_stock (GTK_TOOLBAR (toolbar_main),
					  "gtk-zoom-in", _("Bigger Font Size"), NULL, NULL,
					  NULL, -1);
	gtk_widget_show (button_zoom_in);

	button_zoom_out =
		gtk_toolbar_insert_stock (GTK_TOOLBAR (toolbar_main),
					  "gtk-zoom-out", _("Smaller Font Size"), NULL, NULL,
					  NULL, -1);
	gtk_widget_show (button_zoom_out);

	button_zoom_off =
		gtk_toolbar_insert_stock (GTK_TOOLBAR (toolbar_main),
					  "gtk-zoom-100", _("Default Font Size"), NULL, NULL,
					  NULL, -1);
	gtk_widget_show (button_zoom_off);
	*/
	tt_icon = create_pixmap_from_data (hide_sidebar);
	button_hide_side =
		gtk_toolbar_append_item         (GTK_TOOLBAR (toolbar_main),
                                         _("Hide sidebar"),
                                         _("Hide sidebar"),
                                         _("Hide the right sidebar"),
										 tt_icon,
                                         NULL,
                                         NULL);
	gtk_widget_show (button_hide_side);

	tt_icon = create_pixmap_from_data (show_sidebar);
	button_show_side =
		gtk_toolbar_append_item         (GTK_TOOLBAR (toolbar_main),
                                         _("Show sidebar"),
                                         _("Show sidebar"),
                                         _("Show the right sidebar"),
										 tt_icon,
                                         NULL,
                                         NULL);
	gtk_widget_show (button_show_side);

	tt_icon = create_pixmap_from_data (hide_footer);
	button_hide_footer =
		gtk_toolbar_append_item         (GTK_TOOLBAR (toolbar_main),
                                         _("Hide footer"),
                                         _("Hide footer"),
                                         _("Hide the footer panel"),
										 tt_icon,
                                         NULL,
                                         NULL);
	gtk_widget_show (button_hide_footer);

	tt_icon = create_pixmap_from_data (show_footer);
	button_show_footer =
		gtk_toolbar_append_item         (GTK_TOOLBAR (toolbar_main),
                                         _("Show footer"),
                                         _("Show footer"),
                                         _("Show the footer panel"),
										 tt_icon,
                                         NULL,
                                         NULL);
	gtk_widget_show (button_show_footer);

	gtk_toolbar_append_space (GTK_TOOLBAR (toolbar_main));
	// prefs /////////////
	button_preferences = gtk_toolbar_insert_stock (GTK_TOOLBAR (toolbar_main),
					  "gtk-preferences", _("Preferences"), NULL, NULL,
					  NULL, -1);
	gtk_widget_show (button_preferences);
	// secondary toolbar ///////////////////////////////////////////////////////
	toolbar_tools = gtk_toolbar_new ();
	gtk_widget_show (toolbar_tools);
	gtk_box_pack_start (GTK_BOX (vbox_main), toolbar_tools, FALSE, FALSE, 0);
	gtk_toolbar_set_style (GTK_TOOLBAR (toolbar_tools), GTK_TOOLBAR_ICONS);

	tt_icon = create_pixmap_from_data (dlg_margin);
	button_margin_all =
		gtk_toolbar_append_item         (GTK_TOOLBAR (toolbar_tools),
                                         _("Margin all"),
                                         _("Show the margin all dialog"),
                                         _("Show the margin all dialog"),
										 tt_icon,
                                         NULL,
                                         NULL);
	gtk_widget_show (button_margin_all);

	tt_icon = create_pixmap_from_data (dlg_border);
	button_border_all =
		gtk_toolbar_append_item         (GTK_TOOLBAR (toolbar_tools),
                                         _("Border all"),
                                         _("Show the border all dialog"),
                                         _("Show the border all dialog"),
										 tt_icon,
                                         NULL,
                                         NULL);
	gtk_widget_show (button_border_all);

	tt_icon = create_pixmap_from_data (dlg_padding);
	button_padding_all =
		gtk_toolbar_append_item         (GTK_TOOLBAR (toolbar_tools),
                                         _("Padding all"),
                                         _("Show the padding all dialog"),
                                         _("Show the padding all dialog"),
										 tt_icon,
                                         NULL,
                                         NULL);
	gtk_widget_show (button_padding_all);

	tt_icon = create_pixmap_from_data (dlg_box);
	button_box_properties =
		gtk_toolbar_append_item         (GTK_TOOLBAR (toolbar_tools),
                                         _("Box properties wizard"),
                                         _("Show the box properties wizard"),
                                         _("Show the box properties wizard"),
										 tt_icon,
                                         NULL,
                                         NULL);
	gtk_widget_show (button_box_properties);

	gtk_toolbar_append_space (GTK_TOOLBAR (toolbar_tools));

	tt_icon = create_pixmap_from_data (font);
	button_font =
		gtk_toolbar_append_item         (GTK_TOOLBAR (toolbar_tools),
                                         _("Font"),
                                         _("Show the font dialog"),
                                         _("Show the font dialog"),
										 tt_icon,
                                         NULL,
                                         NULL);
	gtk_widget_show (button_font);

	button_color =
		gtk_toolbar_insert_stock (GTK_TOOLBAR (toolbar_tools),
					  "gtk-select-color", _("Choose Color"), NULL,
					  NULL, NULL, -1);
	gtk_widget_show (button_color);

	gtk_toolbar_append_space (GTK_TOOLBAR (toolbar_tools));

	tt_icon = create_pixmap_from_data (selector_wizard);
	button_sel_wizard =
		gtk_toolbar_append_item         (GTK_TOOLBAR (toolbar_tools),
                                         _("Selector wizard"),
                                         _("Show the Selector wizard"),
                                         _("Show the Selector wizard"),
										 tt_icon,
                                         NULL,
                                         NULL);
	gtk_widget_show (button_sel_wizard);

	tt_icon = create_pixmap_from_data (color_wizard);
	button_col_wizard =
		gtk_toolbar_append_item         (GTK_TOOLBAR (toolbar_tools),
                                         _("Color wizard"),
                                         _("Show the Color wizard"),
                                         _("Show the Color wizard"),
										 tt_icon,
                                         NULL,
                                         NULL);
	gtk_widget_show (button_col_wizard);

	gtk_toolbar_append_space (GTK_TOOLBAR (toolbar_tools));

	tt_icon = create_pixmap_from_data (scan_selector);
	button_scan_selector =
		gtk_toolbar_append_item         (GTK_TOOLBAR (toolbar_tools),
                                         _("Scan selector"),
                                         _("Scan the current selector to be shown in the side panel"),
                                         _("Scan the current selector to be shown in the side panel"),
										 tt_icon,
                                         NULL,
                                         NULL);
	gtk_widget_show (button_scan_selector);

	tt_icon = create_pixmap_from_data (document_info);
	button_doc_info =
		gtk_toolbar_append_item         (GTK_TOOLBAR (toolbar_tools),
                                         _("Document info"),
                                         _("Show information about the current document"),
                                         _("Show information about the current document"),
										 tt_icon,
                                         NULL,
                                         NULL);
	gtk_widget_show (button_doc_info);

	button_clean_output =
		gtk_toolbar_insert_stock (GTK_TOOLBAR (toolbar_tools),
					  "gtk-clear", _("Clean program output"), NULL, NULL,
					  NULL, -1);
	gtk_widget_show (button_clean_output);

	gtk_toolbar_append_space (GTK_TOOLBAR (toolbar_tools));

	tt_icon = create_pixmap_from_data (validate);
	button_validate =
		gtk_toolbar_append_item         (GTK_TOOLBAR (toolbar_tools),
                                         _("Validate"),
                                         _("Validate current document"),
                                         _("Validate current document"),
										 tt_icon,
                                         NULL,
                                         NULL);
	gtk_widget_show (button_validate);

	tt_icon = create_pixmap_from_data (validate_dump);
	button_validate_dump =
		gtk_toolbar_append_item         (GTK_TOOLBAR (toolbar_tools),
                                         _("Validate and dump"),
                                         _("Validate and dump current document"),
                                         _("Validate and dump current document"),
										 tt_icon,
                                         NULL,
                                         NULL);
	gtk_widget_show (button_validate_dump);

	//  paned //////////////////////////////////////////////////////////////////
	vpaned_main = gtk_vpaned_new ();
	gtk_widget_show (vpaned_main);
	gtk_box_pack_start (GTK_BOX (vbox_main), vpaned_main, TRUE, TRUE, 0);
	gtk_widget_set_size_request (vpaned_main, 100, 100);
	
	hpaned_main = gtk_hpaned_new ();
	gtk_widget_show (hpaned_main);
	gtk_paned_pack1 (GTK_PANED (vpaned_main), hpaned_main, TRUE, TRUE);
	gtk_widget_set_size_request (hpaned_main, 100, 100);
 
	// ////////////// notebook_docs ////////////////////////////////////////////
	notebook_docs = gtk_notebook_new ();
	gtk_widget_show (notebook_docs);
	gtk_notebook_set_scrollable (GTK_NOTEBOOK (notebook_docs), TRUE);
	gtk_notebook_popup_enable (GTK_NOTEBOOK (notebook_docs));
#ifndef SIDEPANEL_AT_LEFT	
	gtk_paned_pack1 (GTK_PANED (hpaned_main), notebook_docs, TRUE, TRUE);
#else
	gtk_paned_pack2 (GTK_PANED (hpaned_main), notebook_docs, TRUE, TRUE);
#endif
	gtk_widget_set_size_request (notebook_docs, 100, 100);
	
	// //////////////// notebook footer ////////////////////////////////////////
	notebook_footer = gtk_notebook_new ();
	// /////////// set win object settings before creating any doc /////////////
	cssed_window_set_document_notebook( windowobj, notebook_docs );
	cssed_window_set_footer_notebook( windowobj, notebook_footer );
	cssed_window_dynamic_menus_set_full( 	windowobj,
											menu_view_line_numbers,
											menu_view_line_endings,
											menu_view_white_spaces,
											menu_view_lines_wrapped,
											menuitem_eol_mode_cr,
											menuitem_eol_mode_lf,
											menuitem_eol_mode_crlf,
											menu_item_autocompletion,
											menu_item_folding );

	// //////////////////////////////////////////////////////////////////////
	// we need to fill the window->keyword list before creating an editor,
	// if we don't do that the initial editor will have
	// no keywords to style so first of all we create and fill
	// the tree. will be show later.
	treeview_css_properties = gtk_tree_view_new ();
	// this must be set after the xmlparser runs. It could need to
	// write to output
	output_list = gtk_tree_view_new ();
	cssed_window_set_output_treeview (windowobj, GTK_WIDGET(output_list));
	output_render = gtk_cell_renderer_text_new ();
	output_column =	gtk_tree_view_column_new_with_attributes ("", output_render, "markup", 0, NULL);
	gtk_tree_view_insert_column (GTK_TREE_VIEW (output_list), output_column, 0);
	output_store = gtk_list_store_new (1, G_TYPE_STRING);
	gtk_tree_view_set_model (GTK_TREE_VIEW (output_list), GTK_TREE_MODEL (output_store));
	g_object_unref(output_store);
	gtk_tree_view_set_headers_visible (GTK_TREE_VIEW (output_list),	   FALSE);
	// this func will fill the treeview
	// we must fill the window->treeview_css_properties before
	// trying to fill the tree
	cssed_window_set_css_definition_treeview( windowobj, GTK_WIDGET(treeview_css_properties) );
	set_css_properties_treeview (windowobj);
	// the same can be applied to the selector scanner
	treeview_css_current = gtk_tree_view_new ();
	cssed_window_set_selector_scanner_treeview( windowobj, GTK_WIDGET(treeview_css_current));
	// and also for the document scanner list
	document_scanner_treeview = gtk_tree_view_new();
	cssed_window_set_digest_treeview ( windowobj, document_scanner_treeview );
	// those widgets are enabled disabled by the app
	cssed_window_dynamic_widgets_set(	windowobj,
										button_toolbar_save, menu_save,
										button_undo, menu_undo, pop_menu_undo,
										button_redo, menu_redo, pop_menu_redo );

	// now we can create and stylised editor
	create_and_attach_new_doc(windowobj, _("Untitled"));

	// document scanner // ( digest ) ///////////////////////////////////////////////////
	document_scanner_store = gtk_list_store_new (4, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING);
	gtk_tree_view_set_model(GTK_TREE_VIEW(document_scanner_treeview),
										GTK_TREE_MODEL(document_scanner_store));
	g_object_unref(document_scanner_store);
	
	gtk_widget_show (document_scanner_treeview);
	document_scanner_render_text	= gtk_cell_renderer_text_new();

	document_scanner_column_style = gtk_tree_view_column_new_with_attributes (_("Style"),
							  document_scanner_render_text, "markup", 0, NULL);
	document_scanner_column_line = gtk_tree_view_column_new_with_attributes (_("Line"),
							  document_scanner_render_text, "markup", 1, NULL);
	document_scanner_column_file = gtk_tree_view_column_new_with_attributes (_("File"),
							  document_scanner_render_text, "markup", 2, NULL);
	document_scanner_column_path = gtk_tree_view_column_new_with_attributes (_("Path"),
							  document_scanner_render_text, "markup", 3, NULL);


	gtk_tree_view_insert_column (GTK_TREE_VIEW (document_scanner_treeview),
								document_scanner_column_style, 0);
	gtk_tree_view_insert_column (GTK_TREE_VIEW (document_scanner_treeview),
								document_scanner_column_line, 1);
	gtk_tree_view_insert_column (GTK_TREE_VIEW (document_scanner_treeview),
								document_scanner_column_file, 2);
	gtk_tree_view_insert_column (GTK_TREE_VIEW (document_scanner_treeview),
								document_scanner_column_path, 3);			
													
	gtk_tree_view_set_search_column (GTK_TREE_VIEW (document_scanner_treeview), 0);
	gtk_tree_view_set_search_equal_func (GTK_TREE_VIEW (document_scanner_treeview),	
                                         document_scanner_treeview_search_equal_func,
                                         NULL, NULL);
                                             
	gtk_tree_view_column_set_resizable (document_scanner_column_style, TRUE);
	gtk_tree_view_column_set_resizable (document_scanner_column_file, TRUE);
	gtk_tree_view_column_set_resizable (document_scanner_column_line, TRUE);
	gtk_tree_view_column_set_resizable (document_scanner_column_path, TRUE);

	gtk_tree_view_set_headers_visible(GTK_TREE_VIEW (document_scanner_treeview), TRUE);
	gtk_tree_view_set_headers_clickable(GTK_TREE_VIEW (document_scanner_treeview), TRUE);

	gtk_tree_sortable_set_sort_func( GTK_TREE_SORTABLE( document_scanner_store ),
									0, tree_iter_doc_scanner_compare_string,
									GINT_TO_POINTER(0), NULL);
	gtk_tree_sortable_set_sort_func( GTK_TREE_SORTABLE( document_scanner_store ),
									1, tree_iter_doc_scanner_compare_string,
									GINT_TO_POINTER(1), NULL);
	gtk_tree_sortable_set_sort_func( GTK_TREE_SORTABLE( document_scanner_store ),
									2, tree_iter_doc_scanner_compare_string,
									GINT_TO_POINTER(2), NULL);
	gtk_tree_sortable_set_sort_func( GTK_TREE_SORTABLE( document_scanner_store ),
									3, tree_iter_doc_scanner_compare_string,
									GINT_TO_POINTER(3), NULL);

	document_scanner_scrolled_window = gtk_scrolled_window_new(NULL, NULL);
	gtk_widget_show(document_scanner_scrolled_window);
	gtk_container_add (GTK_CONTAINER (document_scanner_scrolled_window), document_scanner_treeview);

	document_scanner_vbox = gtk_vbox_new(TRUE,0);
	gtk_box_set_homogeneous(GTK_BOX(document_scanner_vbox), FALSE);
	gtk_widget_show(document_scanner_vbox);

	document_scanner_button = gtk_button_new_with_label(_("Parse document"));
	gtk_widget_show(document_scanner_button);

	document_scanner_button_append = gtk_button_new_with_label(_("Parse and append"));
	gtk_widget_show(document_scanner_button_append);

	document_scanner_button_clean = gtk_button_new_from_stock(GTK_STOCK_CLEAR);
	gtk_widget_show(document_scanner_button_clean);

	document_scanner_button_hbox = gtk_hbox_new(TRUE, 5);
	gtk_widget_show(document_scanner_button_hbox);	

	gtk_box_pack_start(GTK_BOX(document_scanner_button_hbox), document_scanner_button,
                       TRUE, TRUE, 0);
	gtk_box_pack_start(GTK_BOX(document_scanner_button_hbox), document_scanner_button_append,
                       TRUE, TRUE, 0);
	gtk_box_pack_start(GTK_BOX(document_scanner_button_hbox), document_scanner_button_clean,
                       TRUE, TRUE, 0);

	gtk_box_pack_start(GTK_BOX(document_scanner_vbox), document_scanner_button_hbox,
                       FALSE, FALSE, 0);
	gtk_box_pack_end(GTK_BOX(document_scanner_vbox), document_scanner_scrolled_window,
                       TRUE, TRUE, 0);

	// sidebar notebook ////////////////////////////////////////////////////////
	sidebar_notebook = gtk_notebook_new();
	gtk_widget_show (sidebar_notebook);
	gtk_notebook_set_tab_pos(GTK_NOTEBOOK(sidebar_notebook),GTK_POS_BOTTOM);
	// add to window structure
	cssed_window_set_sidebar_notebook (windowobj, sidebar_notebook);

	vpaned_css = gtk_vpaned_new ();
	gtk_widget_show (vpaned_css);
#ifndef SIDEPANEL_AT_LEFT	
	gtk_paned_pack2 (GTK_PANED (hpaned_main), sidebar_notebook, FALSE, TRUE);
#else
	gtk_paned_pack1 (GTK_PANED (hpaned_main), sidebar_notebook, FALSE, TRUE);
#endif	
	scrolledwindow_cssc = gtk_scrolled_window_new (NULL, NULL);
	gtk_widget_show (scrolledwindow_cssc);
	gtk_paned_pack1 (GTK_PANED (vpaned_css), scrolledwindow_cssc, FALSE, TRUE);

	gtk_notebook_append_page(GTK_NOTEBOOK(sidebar_notebook), GTK_WIDGET(vpaned_css),
							gtk_label_new("CSS"));
	gtk_notebook_append_page(GTK_NOTEBOOK(sidebar_notebook), GTK_WIDGET(document_scanner_vbox),
							gtk_label_new(_("Digest")));

	// Current selector list is added here to a container
	gtk_widget_show (treeview_css_current);
	gtk_container_add (GTK_CONTAINER (scrolledwindow_cssc),
			   				treeview_css_current);

	// current css selector list
	current_css_render_properties = gtk_cell_renderer_text_new ();
	current_css_render_values = gtk_cell_renderer_text_new ();
	current_css_render_pixbuf = gtk_cell_renderer_pixbuf_new();

	g_object_set(G_OBJECT(current_css_render_values), "editable", TRUE, NULL);

	current_css_property_column =
		gtk_tree_view_column_new_with_attributes (_("Property"),
							  current_css_render_properties,
							  "text", 0, NULL);
	gtk_tree_view_column_set_resizable(
						GTK_TREE_VIEW_COLUMN(current_css_property_column),
                        TRUE);

	current_css_value_column = gtk_tree_view_column_new();
	gtk_tree_view_column_set_title  (GTK_TREE_VIEW_COLUMN(current_css_value_column),
                                     _("Value"));
	gtk_tree_view_column_pack_start (GTK_TREE_VIEW_COLUMN(current_css_value_column),
                                             current_css_render_values,
                                             TRUE);
	gtk_tree_view_column_set_attributes  (GTK_TREE_VIEW_COLUMN(current_css_value_column),
                                        current_css_render_values,
                                        "text",
                                        1, NULL);
	gtk_tree_view_column_set_resizable(
						GTK_TREE_VIEW_COLUMN(current_css_value_column),
                        TRUE);

	current_arrow_column =
		gtk_tree_view_column_new_with_attributes(" ",
							current_css_render_pixbuf,
							"pixbuf",2, NULL);
	gtk_tree_view_column_set_resizable(
						GTK_TREE_VIEW_COLUMN(current_arrow_column),
                        FALSE);
	gtk_tree_view_column_set_sizing (
							GTK_TREE_VIEW_COLUMN(current_arrow_column),
							GTK_TREE_VIEW_COLUMN_FIXED);
	 gtk_tree_view_column_set_fixed_width(
							GTK_TREE_VIEW_COLUMN(current_arrow_column),
                            10);

	gtk_tree_view_insert_column (GTK_TREE_VIEW (treeview_css_current),
				     		current_css_property_column, 0);
	gtk_tree_view_insert_column (GTK_TREE_VIEW (treeview_css_current),
				     		current_css_value_column, 1);
	gtk_tree_view_insert_column (GTK_TREE_VIEW (treeview_css_current),
				    		 current_arrow_column, 2);

	current_css_store =
		gtk_list_store_new (3, G_TYPE_STRING, G_TYPE_STRING, GDK_TYPE_PIXBUF);

	gtk_tree_view_set_model (GTK_TREE_VIEW (treeview_css_current),
							 GTK_TREE_MODEL (current_css_store));
	g_object_unref(current_css_store);	
	gtk_tree_view_set_headers_visible (GTK_TREE_VIEW
					  		 (treeview_css_current), TRUE);

	scrolledwindow_cssp = gtk_scrolled_window_new (NULL, NULL);
	gtk_widget_show (scrolledwindow_cssp);
	gtk_paned_pack2 (GTK_PANED (vpaned_css), scrolledwindow_cssp, TRUE, TRUE);
	gtk_widget_set_size_request (scrolledwindow_cssp, 100, 100);

	// HERE THE CSS PROPERTIES TREE VIEW IS ADDED to a container ///////////////
	gtk_widget_show (treeview_css_properties);
	gtk_container_add (GTK_CONTAINER (scrolledwindow_cssp), treeview_css_properties);
	gtk_tree_view_set_rules_hint (GTK_TREE_VIEW (treeview_css_properties),  TRUE);
	// add panes to windowobj
	cssed_window_panes_set(	windowobj, vpaned_main, hpaned_main, vpaned_css );
	/* expand the css properties tree on path "0" */
	css_prop_path = gtk_tree_path_new_first();
	gtk_tree_view_expand_row (GTK_TREE_VIEW (treeview_css_properties),  css_prop_path, FALSE);
	gtk_drag_source_unset( treeview_css_properties ); // avoid DnD

	// Footer notebook was created above
	gtk_widget_show (notebook_footer);
	gtk_paned_pack2 (GTK_PANED (vpaned_main), notebook_footer, TRUE, TRUE);

	scrolledwindow_output_text = gtk_scrolled_window_new (NULL, NULL);
	gtk_widget_show (scrolledwindow_output_text);

	// OUTPUT LIST is added here to a container
	gtk_widget_show (output_list);
	gtk_container_add (GTK_CONTAINER (scrolledwindow_output_text), output_list);
	gtk_container_add (GTK_CONTAINER (notebook_footer), scrolledwindow_output_text);
	
	// ///////////////////////////////////////
	scratch_tips = gtk_tooltips_new ();
	hpaned_scratch = gtk_hpaned_new();
	gtk_widget_show (hpaned_scratch);
	gtk_widget_set_size_request (hpaned_scratch, 100, 100);
	
	scrolledwindow_drop_text = gtk_scrolled_window_new (NULL, NULL);
	gtk_widget_show (scrolledwindow_drop_text);

	drop_text = gtk_text_view_new ();
	gtk_text_view_set_editable (GTK_TEXT_VIEW (drop_text), TRUE);
	gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW (drop_text), GTK_WRAP_WORD);
	gtk_widget_show (drop_text);
	gtk_container_add (GTK_CONTAINER (scrolledwindow_drop_text),  drop_text);
	cssed_window_set_scratch_buffer( windowobj, gtk_text_view_get_buffer(GTK_TEXT_VIEW(drop_text)) );
	

	scrolledwindow_scratch_text = gtk_scrolled_window_new (NULL, NULL);
	gtk_widget_show (scrolledwindow_scratch_text);
	scratch_text = gtk_text_view_new ();	
	gtk_widget_set_size_request (scrolledwindow_scratch_text, 100, 100);
	
	// set the scratch pad in the window object
	cssed_window_set_static_buffer( windowobj, gtk_text_view_get_buffer(GTK_TEXT_VIEW(scratch_text)) );
	gtk_text_view_set_editable (GTK_TEXT_VIEW (scratch_text), TRUE);
	gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW (scratch_text), GTK_WRAP_WORD);
	// load file contents if any
	scratch_pad_load_contents( gtk_text_view_get_buffer(GTK_TEXT_VIEW(scratch_text)) );
	gtk_widget_show (scratch_text);
	gtk_container_add (GTK_CONTAINER (scrolledwindow_scratch_text), scratch_text);
	// add to paned
	gtk_paned_pack1 ( GTK_PANED (hpaned_scratch), scrolledwindow_drop_text, TRUE, TRUE);
	gtk_paned_pack2 ( GTK_PANED (hpaned_scratch), scrolledwindow_scratch_text, TRUE, TRUE);

	scratch_label = gtk_label_new (_("Scratch Pad / Static Pad"));

	gtk_tooltips_set_tip ( scratch_tips, scratch_label,
						  _( "Those buffers are for notes or drag and drop.\n"
						  	 "The first one is the drag & drop buffer to be\n"
							 "used as a second clipboard or a drag & drop target.\n"
							 "The second one will save the text between sessions so it can"
							 "be used for notes"),
						  	 _("Those buffers are for notes or drag and drop.\n"
						  	 "The first one is the drag & drop buffer to be\n"
							 "used as a second clipboard or a drag & drop target.\n"
							 "The second one will save the text between sessions so it can"
							 "be used for notes")
						 );


	gtk_tooltips_enable( scratch_tips );
	gtk_notebook_append_page (GTK_NOTEBOOK (notebook_footer), hpaned_scratch, scratch_label);
	gtk_widget_queue_draw (GTK_WIDGET (notebook_footer));
	// //////////////////////////////////////////////////////////

	label_notebook_footer = gtk_label_new (_("Program output"));
	gtk_widget_show (label_notebook_footer);
	gtk_notebook_set_tab_label (GTK_NOTEBOOK (notebook_footer),
				    gtk_notebook_get_nth_page (GTK_NOTEBOOK (notebook_footer), 0),
				    label_notebook_footer);
	gtk_label_set_justify (GTK_LABEL (label_notebook_footer), GTK_JUSTIFY_LEFT);

	
#ifdef WITH_TERMINAL
	visual = gdk_visual_get_system();
	colormap = gdk_colormap_new(visual, TRUE);

	term_window = vte_terminal_new ();
	vte_terminal_reset (VTE_TERMINAL(term_window), TRUE, TRUE);
	vte_terminal_set_emulation (VTE_TERMINAL(term_window), "xterm");
	vte_terminal_set_mouse_autohide (VTE_TERMINAL(term_window), TRUE);
	vte_terminal_set_size (VTE_TERMINAL(term_window), 50, 7);// with a bigger size the app doesn't fit in 800x600

	gdk_color_parse ("black", &terminal_fore_color);
	gdk_color_parse ("white", &terminal_back_color);
	gdk_colormap_alloc_color(colormap, &terminal_fore_color, FALSE,TRUE);
	gdk_colormap_alloc_color(colormap, &terminal_back_color, FALSE,TRUE);
	vte_terminal_set_colors(VTE_TERMINAL(term_window), &terminal_fore_color, &terminal_back_color, NULL,0);

	vte_terminal_set_cursor_blinks (VTE_TERMINAL(term_window), TRUE);
	vte_terminal_set_scroll_on_output (VTE_TERMINAL(term_window), TRUE);
	vte_terminal_set_scroll_on_keystroke (VTE_TERMINAL(term_window), TRUE);

	shell = g_getenv("SHELL");
	if( shell == NULL){
		vte_terminal_fork_command (VTE_TERMINAL(term_window), "/bin/sh", NULL, NULL, "~/", 0, 0, 0);
	}else{
		if( g_file_test(shell, G_FILE_TEST_EXISTS) && g_file_test(shell,G_FILE_TEST_IS_EXECUTABLE) ){
			vte_terminal_fork_command (VTE_TERMINAL(term_window), shell, NULL, NULL, "~/",  0, 0, 0);
		}else{
			vte_terminal_fork_command (VTE_TERMINAL(term_window), "/bin/sh", NULL, NULL, "~/",  0, 0, 0);
		}
	}

	gtk_widget_show (term_window);

	sb_terminal = gtk_vscrollbar_new (GTK_ADJUSTMENT (VTE_TERMINAL (term_window)->adjustment));
	GTK_WIDGET_UNSET_FLAGS (sb_terminal, GTK_CAN_FOCUS);

	frame_terminal = gtk_frame_new (NULL);
	gtk_widget_show (frame_terminal);
	gtk_frame_set_shadow_type (GTK_FRAME (frame_terminal), GTK_SHADOW_IN);

	hbox_terminal = gtk_hbox_new (FALSE, 0);
	gtk_box_pack_start (GTK_BOX (hbox_terminal), term_window, TRUE, TRUE, 0);
	gtk_box_pack_start (GTK_BOX (hbox_terminal), sb_terminal, FALSE, TRUE, 0);
	gtk_widget_show_all (hbox_terminal);

	gtk_container_add(GTK_CONTAINER(frame_terminal), hbox_terminal);

	label_terminal = gtk_label_new (_("Terminal"));
	gtk_notebook_append_page (GTK_NOTEBOOK (notebook_footer),
				  frame_terminal, label_terminal);

	g_signal_connect ((gpointer) term_window, "child-exited",
			  G_CALLBACK (on_vte_terminal_child_exited), windowobj);
#endif

#ifdef WITH_RECENT_FILE_MANAGER
	g_signal_connect ((gpointer) recent_file_sub_menu, "item-activated",
			  G_CALLBACK (on_recent_file_sub_menu_item_activated), windowobj);
#endif

	g_signal_connect ((gpointer) window, "destroy",
			  G_CALLBACK (on_window_destroy), windowobj);
	g_signal_connect ((gpointer) window, "delete_event",
			  G_CALLBACK (on_window_delete_event), windowobj);

	g_signal_connect ((gpointer) document_scanner_button,"clicked",
				G_CALLBACK (on_document_scanner_button_clicked),  windowobj);
	g_signal_connect ((gpointer) document_scanner_button_append,"clicked",
				G_CALLBACK (on_document_scanner_button_append_clicked),  windowobj);
	g_signal_connect ((gpointer) document_scanner_button_clean,"clicked",
				G_CALLBACK (on_document_scanner_button_clean_clicked),  windowobj);
	g_signal_connect ((gpointer) menu_new, "activate",
			  G_CALLBACK (on_menu_new_activate), windowobj);
	g_signal_connect ((gpointer) menu_open, "activate",
			  G_CALLBACK (on_menu_open_activate), windowobj);
#ifdef GTK_ATLEAST_2_4 // gtk file chooser don't let you paste a path
	g_signal_connect ((gpointer) menu_open_path, "activate",
			  G_CALLBACK (on_menu_open_path_activate), windowobj);
#endif
	g_signal_connect ((gpointer) menu_close, "activate",
			  G_CALLBACK (on_menu_close_activate), windowobj);
	g_signal_connect ((gpointer) menu_close_all, "activate",
			  G_CALLBACK (on_menu_close_all_activate), windowobj);
	g_signal_connect ((gpointer) menu_revert_to_saved, "activate",
			  G_CALLBACK (on_menu_revert_to_saved_activate), windowobj);
	g_signal_connect ((gpointer) menu_save_all, "activate",
			  G_CALLBACK (on_menu_save_all_activate), windowobj);
	g_signal_connect ((gpointer) menu_save, "activate",
			  G_CALLBACK (on_menu_save_activate), windowobj);
	g_signal_connect ((gpointer) menu_save_as, "activate",
			  G_CALLBACK (on_menu_save_as_activate), windowobj);
	g_signal_connect ((gpointer) menu_quit, "activate",
			  G_CALLBACK (on_menu_quit_activate), windowobj);
	g_signal_connect ((gpointer) menu_undo, "activate",
			  G_CALLBACK (on_menu_undo_activate), windowobj);
	g_signal_connect ((gpointer) menu_redo, "activate",
			  G_CALLBACK (on_menu_redo_activate), windowobj);
	g_signal_connect ((gpointer) menu_search, "activate",
			  G_CALLBACK (on_menu_search_activate), windowobj);
	g_signal_connect ((gpointer) menu_replace, "activate",
			  G_CALLBACK (on_menu_replace_activate), windowobj);
	g_signal_connect ((gpointer) menu_cut, "activate",
			  G_CALLBACK (on_menu_cut_activate), windowobj);
	g_signal_connect ((gpointer) menu_copy, "activate",
			  G_CALLBACK (on_menu_copy_activate), windowobj);
	g_signal_connect ((gpointer) menu_copy_to_scratch, "activate",
			  G_CALLBACK (on_menu_copy_to_scratch_activate), windowobj);
	g_signal_connect ((gpointer) menu_paste, "activate",
			  G_CALLBACK (on_menu_paste_activate), windowobj);
	g_signal_connect ((gpointer) menu_paste_from_scratch, "activate",
			  G_CALLBACK (on_menu_paste_from_scratch_activate), windowobj);
	g_signal_connect ((gpointer) menu_delete_scratch, "activate",
			  G_CALLBACK (on_menu_delete_scratch_activate), windowobj);			  
	g_signal_connect ((gpointer) menu_delete, "activate",
			  G_CALLBACK (on_menu_delete_activate), windowobj);
	g_signal_connect ((gpointer) menu_preferences, "activate",
			  G_CALLBACK (on_menu_preferences_activate), windowobj);
	// toolbar hide/show (toggle)
	g_signal_connect ((gpointer) menu_toolbar, "toggled",
			  G_CALLBACK (on_menu_view_toolbar_toggled), toolbar_main);
	g_signal_connect ((gpointer) menu_secodary_toolbar, "toggled",
			  G_CALLBACK (on_menu_view_toolbar_toggled), toolbar_tools);
	// testing here // this must change
	g_signal_connect ((gpointer) menu_zoom_in, "activate",
			  G_CALLBACK (on_button_zoom_in_clicked), windowobj);
	g_signal_connect ((gpointer) menu_zoom_out, "activate",
			  G_CALLBACK (on_button_zoom_out_clicked), windowobj);
	g_signal_connect ((gpointer) menu_normal_size, "activate",
			  G_CALLBACK (on_button_zoom_off_clicked), windowobj);
	g_signal_connect ((gpointer) menu_about, "activate",
			  G_CALLBACK (on_menu_about_activate), NULL);
#ifdef WITH_HELP_MENUS
#ifdef DARWIN
	g_signal_connect ((gpointer) menuitem_cssed_website_with_safari, "activate",
			G_CALLBACK (on_menu_cssed_website_with_safari_activate), NULL);
	g_signal_connect ((gpointer) menuitem_cssed_website_with_firefox, "activate",
			G_CALLBACK (on_menu_cssed_website_with_firefox_activate), NULL);
#endif
	g_signal_connect ((gpointer) menuitem_cssed_website_with_mozilla, "activate",
					  G_CALLBACK (on_menu_cssed_website_with_mozilla_activate), NULL);
	g_signal_connect ((gpointer) menuitem_cssed_website_with_galeon, "activate",
					  G_CALLBACK (on_menu_cssed_website_with_galeon_activate), NULL);
	g_signal_connect ((gpointer) menuitem_cssed_website_with_dillo, "activate",
					  G_CALLBACK (on_menu_cssed_website_with_dillo_activate), NULL);
	g_signal_connect ((gpointer) menuitem_cssed_website_with_gnome, "activate",
					  G_CALLBACK (on_menu_cssed_website_with_gnome_activate), NULL);

#ifdef DARWIN
	g_signal_connect ((gpointer) menuitem_cssed_doc_online_with_safari, "activate",
					  G_CALLBACK (on_menu_cssed_doc_online_with_safari_activate), NULL);
	g_signal_connect ((gpointer) menuitem_cssed_doc_online_with_firefox, "activate",
					  G_CALLBACK (on_menu_cssed_doc_online_with_firefox_activate), NULL);
#endif
	g_signal_connect ((gpointer) menuitem_cssed_doc_online_with_mozilla, "activate",
					  G_CALLBACK (on_menu_cssed_doc_online_with_mozilla_activate), NULL);
	g_signal_connect ((gpointer) menuitem_cssed_doc_online_with_galeon, "activate",
					  G_CALLBACK (on_menu_cssed_doc_online_with_galeon_activate), NULL);
	g_signal_connect ((gpointer) menuitem_cssed_doc_online_with_dillo, "activate",
					  G_CALLBACK (on_menu_cssed_doc_online_with_dillo_activate), NULL);
	g_signal_connect ((gpointer) menuitem_cssed_doc_online_with_gnome, "activate",
					  G_CALLBACK (on_menu_cssed_doc_online_with_gnome_activate), NULL);
#endif /* #ifdef WITH_HELP_MENUS */

	g_signal_connect ((gpointer) menuitem_show_footer_panel, "activate",
			  G_CALLBACK (on_show_footer_panel_activate), vpaned_main);
	g_signal_connect ((gpointer) menuitem_hide_footer_panel, "activate",
			  G_CALLBACK (on_hide_footer_panel_activate), vpaned_main);
	g_signal_connect ((gpointer) menuitem_show_side_panel, "activate",
			  G_CALLBACK (on_show_side_panel_activate), hpaned_main);
	g_signal_connect ((gpointer) menuitem_hide_side_panel, "activate",
			  G_CALLBACK (on_hide_side_panel_activate), hpaned_main);
	/*   For future use */
	g_signal_connect ((gpointer) menu_cssed_help, "activate",
			  G_CALLBACK (on_menu_cssed_help_activate), windowobj);
	
	// toolbar buttons
	g_signal_connect ((gpointer) button_toolbar_open, "clicked",
			  G_CALLBACK (on_button_toolbar_open_clicked), windowobj);
	g_signal_connect ((gpointer) button_toolbar_new, "clicked",
			  G_CALLBACK (on_button_toolbar_new_clicked), windowobj);
	g_signal_connect ((gpointer) button_toolbar_close, "clicked",
			  G_CALLBACK (on_button_toolbar_close_clicked), windowobj);
	g_signal_connect ((gpointer) button_toolbar_search, "clicked",
			  G_CALLBACK (on_button_toolbar_search_clicked), windowobj);
	g_signal_connect ((gpointer) button_toolbar_replace, "clicked",
			  G_CALLBACK (on_button_toolbar_replace_clicked),
			  windowobj);
	g_signal_connect ((gpointer) button_color, "clicked",
			  G_CALLBACK (on_button_color_clicked), windowobj);
	g_signal_connect ((gpointer) button_toolbar_save, "clicked",
			  G_CALLBACK (on_button_toolbar_save_clicked), windowobj);
	g_signal_connect ((gpointer) button_toolbar_save_as, "clicked",
			  G_CALLBACK (on_button_toolbar_save_as_clicked),
			  windowobj);

	g_signal_connect ((gpointer) button_toolbar_copy, "clicked",
			  G_CALLBACK (on_button_toolbar_copy_clicked), windowobj);
	g_signal_connect ((gpointer) button_toolbar_paste, "clicked",
			  G_CALLBACK (on_button_toolbar_paste_clicked), windowobj);
	g_signal_connect ((gpointer) button_toolbar_cut, "clicked",
			  G_CALLBACK (on_button_toolbar_cut_clicked), windowobj);

	g_signal_connect ((gpointer) button_next, "clicked",
			  G_CALLBACK (on_button_next_clicked), windowobj);
	g_signal_connect ((gpointer) button_prev, "clicked",
			  G_CALLBACK (on_button_prev_clicked), windowobj);

	g_signal_connect ((gpointer) button_undo, "clicked",
			  G_CALLBACK (on_button_undo_clicked), windowobj);
	g_signal_connect ((gpointer) button_redo, "clicked",
			  G_CALLBACK (on_button_redo_clicked), windowobj);
	/* zooming callbacks */
	/*
	g_signal_connect ((gpointer) button_zoom_in, "clicked",
			  G_CALLBACK (on_button_zoom_in_clicked), windowobj);
	g_signal_connect ((gpointer) button_zoom_out, "clicked",
			  G_CALLBACK (on_button_zoom_out_clicked), windowobj);
	g_signal_connect ((gpointer) button_zoom_off, "clicked",
			  G_CALLBACK (on_button_zoom_off_clicked), windowobj);
	*/
	g_signal_connect ((gpointer) button_hide_side, "clicked",
			  G_CALLBACK (on_button_hide_side_clicked), hpaned_main);
	g_signal_connect ((gpointer) button_show_side, "clicked",
			  G_CALLBACK (on_button_show_side_clicked), hpaned_main);
	g_signal_connect ((gpointer) button_hide_footer, "clicked",
			  G_CALLBACK (on_button_hide_footer_clicked), vpaned_main);
	g_signal_connect ((gpointer) button_show_footer, "clicked",
			  G_CALLBACK (on_button_show_footer_clicked), vpaned_main);
	/* preferences dialog callbacks */
	g_signal_connect ((gpointer)button_preferences, "clicked",
			  G_CALLBACK (on_button_preferences_clicked), windowobj);
	/* notebook */
	g_signal_connect ((gpointer) notebook_docs, "switch-page",
			  G_CALLBACK (on_notebook_docs_switch_page),
			  windowobj);
	// main view menu, this will change child menus
	//	on a document basis reflecting the opened document state
	g_signal_connect ((gpointer) menuitem_document, "activate",
			  G_CALLBACK (on_menu_document_activate), windowobj);
	// dynamic menus set the callback func used when document specific menu items are toggled
	g_signal_connect ((gpointer) menu_view_line_numbers, "toggled",
			  G_CALLBACK (on_menu_line_numbers_toggled),
			  windowobj);
	g_signal_connect ((gpointer) menu_view_line_endings, "toggled",
			  G_CALLBACK (on_menu_line_endings_toggled),
			  windowobj);
	g_signal_connect ((gpointer) menu_view_lines_wrapped, "toggled",
			  G_CALLBACK (on_menu_line_wrapping_toggled),
			  windowobj);
	g_signal_connect ((gpointer) menu_view_white_spaces, "toggled",
			  G_CALLBACK (on_menu_view_white_spaces_toggled),
			  windowobj);
	g_signal_connect ((gpointer) menu_item_autocompletion, "toggled",
			  G_CALLBACK (on_menu_item_autocompletion_toggled),
			  windowobj);
	g_signal_connect ((gpointer) menu_item_folding, "toggled",
			  G_CALLBACK (on_menu_item_folding_toggled),
			  windowobj);
	g_signal_connect ((gpointer) menu_item_font, "activate",
			  G_CALLBACK (on_menu_item_font_activate),
			  windowobj);
	g_signal_connect ((gpointer) menu_item_flag_clear, "activate",
			  G_CALLBACK (on_menu_item_flag_clear_activate),
			  windowobj);
	g_signal_connect ((gpointer) menu_item_flag_red, "activate",
			  G_CALLBACK (on_menu_item_flag_red_activate),
			  windowobj);
	g_signal_connect ((gpointer) menu_item_flag_white, "activate",
			  G_CALLBACK (on_menu_item_flag_white_activate),
			  windowobj);
	g_signal_connect ((gpointer) menu_item_flag_blue, "activate",
			  G_CALLBACK (on_menu_item_flag_blue_activate),
			  windowobj);
	g_signal_connect ((gpointer) menu_item_flag_yellow, "activate",
			  G_CALLBACK (on_menu_item_flag_yellow_activate),
			  windowobj);
	g_signal_connect ((gpointer) menu_item_flag_green, "activate",
			  G_CALLBACK (on_menu_item_flag_green_activate),
			  windowobj);
	g_signal_connect ((gpointer) menu_item_document_goto_start, "activate",
			  G_CALLBACK (on_menu_item_document_goto_start_activate),
			  windowobj);
	g_signal_connect ((gpointer) menu_item_document_goto_end, "activate",
			  G_CALLBACK (on_menu_item_document_goto_end_activate),
			  windowobj);
	g_signal_connect ((gpointer) menu_item_document_goto_line, "activate",
			  G_CALLBACK (on_menu_item_document_goto_line_activate),
			  windowobj);
	g_signal_connect ((gpointer) menu_item_document_list_current, "activate",
			  G_CALLBACK (on_menu_item_document_list_current_activate),
			  windowobj);
	g_signal_connect ((gpointer) menu_item_document_list_open, "activate",
			  G_CALLBACK (on_menu_item_document_list_open_activate),
			  windowobj);
	g_signal_connect ((gpointer) menu_item_document_list_new, "activate",
			  G_CALLBACK (on_menu_item_document_list_new_activate),
			  windowobj);
/* menu_item_flag
	g_signal_connect ((gpointer) menu_item_autocompletion, "activate",
			  G_CALLBACK (on_menu_item_autocompletion_activate),
			  windowobj);
*/
	g_signal_connect ((gpointer) menuitem_eol_mode, "activate",
			  G_CALLBACK (on_menu_eol_mode_activate),
			  windowobj);
	g_signal_connect ((gpointer) menuitem_eol_mode_cr, "activate",
			  G_CALLBACK (on_menu_eol_mode_cr_activate),
			  windowobj);
	g_signal_connect ((gpointer) menuitem_eol_mode_lf, "activate",
			  G_CALLBACK (on_menu_eol_mode_lf_activate),
			  windowobj);
	g_signal_connect ((gpointer) menuitem_eol_mode_crlf, "activate",
			  G_CALLBACK (on_menu_eol_mode_crlf_activate),
			  windowobj);
	g_signal_connect ((gpointer) menuitem_eol_convert_cr, "activate",
			  G_CALLBACK (on_menu_eol_convert_cr_activate),
			  windowobj);
	g_signal_connect ((gpointer) menuitem_eol_convert_lf, "activate",
			  G_CALLBACK (on_menu_eol_convert_lf_activate),
			  windowobj);
	g_signal_connect ((gpointer) menuitem_eol_convert_crlf, "activate",
			  G_CALLBACK (on_menu_eol_convert_crlf_activate),
			  windowobj);
	// folding
	g_signal_connect ((gpointer) menuitem_fonding_foldall, "activate",
			  G_CALLBACK (on_menuitem_fonding_foldall_activate),
			  windowobj);
	g_signal_connect ((gpointer) menuitem_folding_unfoldall, "activate",
			  G_CALLBACK (on_menuitem_folding_unfoldall_activate),
			  windowobj);
	// encoding
	g_signal_connect ((gpointer) menuitem_encoding_default, "activate",
			  G_CALLBACK (on_menuitem_encoding_default_activate),
			  windowobj);
	g_signal_connect ((gpointer) menuitem_encoding_utf8, "activate",
			  G_CALLBACK (on_menuitem_encoding_utf8_activate),
			  windowobj);
	g_signal_connect ((gpointer) menuitem_encoding_dbcs, "activate",
			  G_CALLBACK (on_menuitem_encoding_dbcs_activate),
			  windowobj);
	// charset
	g_signal_connect ((gpointer) menuitem_charset_ansi, "activate",
			  G_CALLBACK (on_menuitem_charset_ansi_activate),
			  windowobj);
	g_signal_connect ((gpointer) menuitem_charset_easteurope, "activate",
			  G_CALLBACK (on_menuitem_charset_easteurope_activate),
			  windowobj);
	g_signal_connect ((gpointer) menuitem_encoding_dbcs, "activate",
			  G_CALLBACK (on_menuitem_charset_gb2312_activate),
			  windowobj);
	g_signal_connect ((gpointer) menuitem_encoding_utf8, "activate",
			  G_CALLBACK (on_menuitem_charset_hangul_activate),
			  windowobj);
	g_signal_connect ((gpointer) menuitem_encoding_dbcs, "activate",
			  G_CALLBACK (on_menuitem_charset_shiftjis_activate),
			  windowobj);
#ifdef WITH_SPLITTER_WINDOW
/*
	g_signal_connect ((gpointer) menuitem_split, "activate",
			  G_CALLBACK (on_menuitem_split_activate),
			  windowobj);
*/
	g_signal_connect ((gpointer) menuitem_split_hor, "activate",
			  G_CALLBACK (on_menuitem_split_horizontal_activate),
			  windowobj);
	g_signal_connect ((gpointer) menuitem_split_ver, "activate",
			  G_CALLBACK (on_menuitem_split_vertical_activate),
			  windowobj);
	g_signal_connect ((gpointer) menuitem_split_none, "activate",
			  G_CALLBACK (on_menuitem_split_none_activate),
			  windowobj);
#endif

	// selector scan menu
	g_signal_connect ((gpointer) menu_utils_scan_selector, "activate",
			  G_CALLBACK (on_menu_scan_selector_activate),
			  windowobj);
	// doc info
	g_signal_connect ((gpointer) menu_utils_doc_info, "activate",
			  G_CALLBACK (on_menu_doc_info_activate), windowobj);
	// autocompletion,  not in lookup_widget list
	g_signal_connect ((gpointer) menu_utils_autocomp, "activate",
			  G_CALLBACK (on_menu_autocompletion_activate), windowobj);
	// folding,  not in lookup_widget list
	g_signal_connect ((gpointer) menu_utils_autocomp, "activate",
			  G_CALLBACK (on_menu_autocompletion_activate), windowobj);
	// validation and dump,  not in lookup_widget list
	g_signal_connect ((gpointer) menu_utils_validate, "activate",
			  G_CALLBACK (on_menu_utils_validate_activate), windowobj);
	// validation only,  not in lookup_widget list
	g_signal_connect ((gpointer) menu_utils_validate_only, "activate",
			  G_CALLBACK (on_menu_utils_validate_only_activate), windowobj);
	// clean output,  not in lookup_widget list
	g_signal_connect ((gpointer) menu_utils_cleanoutput, "activate",
			  G_CALLBACK (on_menu_utils_cleanoutput_activate), windowobj);

	// css properties treeview callbacks
	g_signal_connect ((gpointer) treeview_css_properties, "row-activated",
			  G_CALLBACK
			  (on_treeview_css_properties_row_activated),
			  windowobj);
	//output window callbacks
	g_signal_connect ((gpointer) output_list, "row-activated",
			  G_CALLBACK
			  (on_output_list_row_activated),
			  windowobj);
	// doc scanner callbacks
	g_signal_connect ((gpointer) document_scanner_treeview, "row-activated",
			  G_CALLBACK
			  (on_doc_scanner_row_activated),
			  windowobj);
	// catch when edited the current CSS selector
	g_signal_connect(G_OBJECT(current_css_render_values), "edited",
						G_CALLBACK(on_current_css_render_values_edited),
						windowobj);
	// selector scanner treeview callbacks
	g_signal_connect ((gpointer) treeview_css_current, "row-activated",
			  G_CALLBACK
			  (on_treeview_css_current_row_activated),
			  windowobj);
	// css selector wizard
	g_signal_connect ((gpointer) menuitem_selector_wizard, "activate",
			  G_CALLBACK (on_menuitem_selector_wizard_activate),
			  windowobj);
	// css color wizard
	g_signal_connect ((gpointer) menuitem_color_wizard, "activate",
			  G_CALLBACK (on_menuitem_color_wizard_activate),
			  windowobj);
	// box properties wizard
	g_signal_connect ((gpointer) menuitem_box_wizard, "activate",
			  G_CALLBACK (on_menuitem_box_wizard_activate),
			  windowobj);
	// secondary toolbar callbacks
	g_signal_connect ((gpointer) button_border_all, "clicked",
			  G_CALLBACK (on_button_border_all_clicked),
			  windowobj);
	g_signal_connect ((gpointer) button_margin_all, "clicked",
			  G_CALLBACK (on_button_margin_all_clicked),
			  windowobj);
	g_signal_connect ((gpointer) button_padding_all, "clicked",
			  G_CALLBACK (on_button_padding_all_clicked),
			  windowobj);
	g_signal_connect ((gpointer) button_box_properties, "clicked",
			  G_CALLBACK (on_button_box_properties_clicked),
			  windowobj);
	g_signal_connect ((gpointer) button_doc_info, "clicked",
			  G_CALLBACK (on_button_doc_info_clicked),
			  windowobj);
	g_signal_connect ((gpointer) button_sel_wizard, "clicked",
			  G_CALLBACK (on_button_sel_wizard_clicked),
			  windowobj);
	g_signal_connect ((gpointer) button_col_wizard, "clicked",
			  G_CALLBACK (on_button_col_wizard_clicked),
			  windowobj);
	g_signal_connect ((gpointer) button_font, "clicked",
			  G_CALLBACK (on_button_font_clicked),
			  windowobj);
	g_signal_connect ((gpointer) button_scan_selector, "clicked",
			  G_CALLBACK (on_button_scan_selector_clicked),
			  windowobj);
	g_signal_connect ((gpointer) button_clean_output, "clicked",
			  G_CALLBACK (on_button_clean_output_clicked),
			  windowobj);
	g_signal_connect ((gpointer) button_validate, "clicked",
			  G_CALLBACK (on_button_validate_clicked),
			  windowobj);
	g_signal_connect ((gpointer) button_validate_dump, "clicked",
			  G_CALLBACK (on_button_validate_dump_clicked),
			  windowobj);
	// editor pop menu callbacks
	g_signal_connect ((gpointer) pop_menu_undo, "activate",
			  G_CALLBACK (on_pop_menu_undo_activate),
			  windowobj);
	g_signal_connect ((gpointer) pop_menu_redo, "activate",
			  G_CALLBACK (on_pop_menu_redo_activate),
			  windowobj);
	g_signal_connect ((gpointer) pop_menu_cut, "activate",
			  G_CALLBACK (on_pop_menu_cut_activate),
			  windowobj);
	g_signal_connect ((gpointer) pop_menu_copy, "activate",
			  G_CALLBACK (on_pop_menu_copy_activate),
			  windowobj);
	g_signal_connect ((gpointer) pop_menu_copy_to_scratch, "activate",
			  G_CALLBACK (on_pop_menu_copy_to_scratch_activate),
			  windowobj);
	g_signal_connect ((gpointer) pop_menu_paste, "activate",
			  G_CALLBACK (on_pop_menu_paste_activate),
			  windowobj);
	g_signal_connect ((gpointer) pop_menu_paste_from_scratch, "activate",
			  G_CALLBACK (on_pop_menu_paste_from_scratch_activate),
			  windowobj);
	g_signal_connect ((gpointer) pop_menu_delete_scratch, "activate",
			  G_CALLBACK (on_pop_menu_delete_scratch_activate),
			  windowobj);		
	g_signal_connect ((gpointer) pop_menu_delete, "activate",
			  G_CALLBACK (on_pop_menu_delete_activate),
			  windowobj);
	g_signal_connect ((gpointer) pop_menu_search, "activate",
			  G_CALLBACK (on_pop_menu_search_activate),
			  windowobj);
	g_signal_connect ((gpointer) pop_menu_replace, "activate",
			  G_CALLBACK (on_pop_menu_replace_activate),
			  windowobj);
	g_signal_connect ((gpointer) pop_menu_selectall, "activate",
			  G_CALLBACK (on_pop_menu_selectall_activate),
			  windowobj);
	g_signal_connect ((gpointer) pop_menu_editcolor, "activate",
			  G_CALLBACK (on_pop_menu_editcolor_activate),
			  windowobj);
	// document scanner columns callbacks
	g_signal_connect ((gpointer) document_scanner_column_style, "clicked",
			  G_CALLBACK (on_doc_scanner_col_styles_clicked),
			  document_scanner_treeview);
	g_signal_connect ((gpointer) document_scanner_column_line, "clicked",
			  G_CALLBACK (on_doc_scanner_col_numbers_clicked),
			  document_scanner_treeview);
	g_signal_connect ((gpointer) document_scanner_column_file, "clicked",
			  G_CALLBACK (on_doc_scanner_col_file_clicked),
			  document_scanner_treeview);
	g_signal_connect ((gpointer) document_scanner_column_path, "clicked",
			  G_CALLBACK (on_doc_scanner_col_path_clicked),
			  document_scanner_treeview);
/* PANEDBUG
	// FIXME - This is being used to track a bug in the paned
	g_signal_connect ((gpointer) vpaned_main, "notify::position",
                                    G_CALLBACK(on_handle_position_changed),
                                    NULL);
*/
   
#ifdef WITH_PLUGINS
	if( g_module_supported() ){
		g_signal_connect ((gpointer) menuitem_plugins_load, "activate",
			  G_CALLBACK (on_plugins_load_activate), windowobj);
	}
#endif
	/* Store pointers to all widgets, for use by lookup_widget(). */
	GLADE_HOOKUP_OBJECT_NO_REF (window, window, "window");
#ifdef WITH_PLUGINS
	GLADE_HOOKUP_OBJECT (window, menuitem_plugins_menu, "plugins_menu");
#endif
	GLADE_HOOKUP_OBJECT (window, menubar_main, "menubar_main");
	GLADE_HOOKUP_OBJECT (window, menuitem_file, "menuitem_file");
	GLADE_HOOKUP_OBJECT (window, menuitem_file_menu,   "menuitem_file_menu");
	GLADE_HOOKUP_OBJECT (window, menu_new, "menu_new");
	GLADE_HOOKUP_OBJECT (window, menu_open, "menu_open");
	GLADE_HOOKUP_OBJECT (window, menu_save, "menu_save");
	GLADE_HOOKUP_OBJECT (window, menu_save_as, "menu_save_as");
	GLADE_HOOKUP_OBJECT (window, menu_quit, "menu_quit");
	GLADE_HOOKUP_OBJECT (window, menuitem_edit, "menuitem_edit");
	GLADE_HOOKUP_OBJECT (window, menuitem_edit_menu,   "menuitem_edit_menu");
	GLADE_HOOKUP_OBJECT (window, menu_cut, "menu_cut");
	GLADE_HOOKUP_OBJECT (window, menu_copy, "menu_copy");
	GLADE_HOOKUP_OBJECT (window, menu_paste, "menu_paste");
	GLADE_HOOKUP_OBJECT (window, menu_delete, "menu_delete");
	GLADE_HOOKUP_OBJECT (window, menuitem_document, "menuitem_document");
	GLADE_HOOKUP_OBJECT (window, menuitem_ayuda, "menuitem_ayuda");
	GLADE_HOOKUP_OBJECT (window, menuitem_show_footer_panel, "menuitem_show_footer_panel");
	GLADE_HOOKUP_OBJECT (window, menuitem_hide_footer_panel, "menuitem_hide_footer_panel");
	GLADE_HOOKUP_OBJECT (window, menuitem_ayuda_menu, "menuitem_ayuda_menu");
	GLADE_HOOKUP_OBJECT (window, menu_about, "menu_about");
#ifdef WITH_HELP_MENUS
	GLADE_HOOKUP_OBJECT (window, menuitem_cssed_website, "menuitem_cssed_website");
	GLADE_HOOKUP_OBJECT (window, menuitem_cssed_website_menu, "menuitem_cssed_website_menu");
#ifdef DARWIN
	GLADE_HOOKUP_OBJECT (window, menuitem_cssed_website_with_safari, "menuitem_cssed_website_with_safari");
	GLADE_HOOKUP_OBJECT (window, menuitem_cssed_website_with_firefox, "menuitem_cssed_website_with_firefox");
#endif
	GLADE_HOOKUP_OBJECT (window, menuitem_cssed_website_with_mozilla, "menuitem_cssed_website_with_mozilla");
	GLADE_HOOKUP_OBJECT (window, menuitem_cssed_website_with_galeon, "menuitem_cssed_website_with_galeon");
	GLADE_HOOKUP_OBJECT (window, menuitem_cssed_website_with_dillo, "menuitem_cssed_website_with_dillo");
	GLADE_HOOKUP_OBJECT (window, menuitem_cssed_website_with_gnome, "menuitem_cssed_website_with_gnome");
	GLADE_HOOKUP_OBJECT (window, menuitem_cssed_doc_online, "menuitem_cssed_doc_online");
	GLADE_HOOKUP_OBJECT (window, menuitem_cssed_doc_online_menu, "menuitem_cssed_doc_online_menu");
#ifdef DARWIN
	GLADE_HOOKUP_OBJECT (window, menuitem_cssed_doc_online_with_safari, "menuitem_cssed_doc_online_with_safari");
	GLADE_HOOKUP_OBJECT (window, menuitem_cssed_doc_online_with_firefox, "menuitem_cssed_doc_online_with_firefox");
#endif
	GLADE_HOOKUP_OBJECT (window, menuitem_cssed_doc_online_with_mozilla, "menuitem_cssed_doc_online_with_mozilla");
	GLADE_HOOKUP_OBJECT (window, menuitem_cssed_doc_online_with_galeon, "menuitem_cssed_doc_online_with_galeon");
	GLADE_HOOKUP_OBJECT (window, menuitem_cssed_doc_online_with_dillo, "menuitem_cssed_doc_online_with_dillo");
	GLADE_HOOKUP_OBJECT (window, menuitem_cssed_doc_online_with_gnome, "menuitem_cssed_doc_online_with_gnome");
#endif /* #ifdef WITH_HELP_MENUS */
	GLADE_HOOKUP_OBJECT (window, toolbar_main, "toolbar_main");
	GLADE_HOOKUP_OBJECT (window, toolbar_tools, "toolbar_tools");
	GLADE_HOOKUP_OBJECT (window, button_toolbar_open, "button_toolbar_open");
	GLADE_HOOKUP_OBJECT (window, button_toolbar_new, "button_toolbar_new");
	GLADE_HOOKUP_OBJECT (window, button_toolbar_close, "button_toolbar_close");
	GLADE_HOOKUP_OBJECT (window, button_toolbar_search, "button_toolbar_search");
	GLADE_HOOKUP_OBJECT (window, button_toolbar_replace, "button_toolbar_replace");
	GLADE_HOOKUP_OBJECT (window, button_toolbar_copy, "button_toolbar_copy");
	GLADE_HOOKUP_OBJECT (window, button_toolbar_paste, "button_toolbar_paste");
	GLADE_HOOKUP_OBJECT (window, button_color, "button_color");
	GLADE_HOOKUP_OBJECT (window, button_toolbar_save, "button_toolbar_save");
	GLADE_HOOKUP_OBJECT (window, button_toolbar_save_as, "button_toolbar_save_as");
	GLADE_HOOKUP_OBJECT (window, notebook_docs, "notebook_docs");
	//GLADE_HOOKUP_OBJECT (window, editor, "editor");
	//GLADE_HOOKUP_OBJECT (window, label_notebook_blank, "label_notebook_blank");
	GLADE_HOOKUP_OBJECT (window, treeview_css_current, "treeview_css_current");
	GLADE_HOOKUP_OBJECT (window, treeview_css_properties, "treeview_css_properties");
	GLADE_HOOKUP_OBJECT (window, notebook_footer, "notebook_footer");
	GLADE_HOOKUP_OBJECT (window, output_list, "output_list");
	GLADE_HOOKUP_OBJECT (window, label_notebook_footer, "label_notebook_footer");
	GLADE_HOOKUP_OBJECT (window, scrolledwindow_scratch_text, "scrolledwindow_scratch_text");
	GLADE_HOOKUP_OBJECT (window, notebook_footer, "notebook_footer");
	GLADE_HOOKUP_OBJECT (window, scratch_text, "scratch_text");
	GLADE_HOOKUP_OBJECT (window, scratch_label, "scratch_label");
//
	GLADE_HOOKUP_OBJECT (window, vpaned_main, "vpaned_main");
	GLADE_HOOKUP_OBJECT (window, hpaned_main, "hpaned_main");
	GLADE_HOOKUP_OBJECT (window, vpaned_css, "vpaned_css");

	GLADE_HOOKUP_OBJECT (window, button_hide_side,"button_hide_side");
	GLADE_HOOKUP_OBJECT (window, button_show_side,"button_show_side");
	GLADE_HOOKUP_OBJECT (window, button_hide_footer,"button_hide_footer");
	GLADE_HOOKUP_OBJECT (window, button_show_footer,"button_show_footer");

	//gtk_widget_grab_focus (editor);
	//gtk_widget_grab_default (editor);
	gtk_window_add_accel_group (GTK_WINDOW (window), accel_group);
	
	return window;
}

GtkWidget *
create_error_dialog (void)
{
	GtkWidget *error_dialog;
	GtkWidget *dialog_vbox;
	GtkWidget *hbox;
	GtkWidget *vbox;
	GtkWidget *image;
	GtkWidget *label_msg;
	GtkWidget *label_detail;
	GtkWidget *dialog_action_area;
	GtkWidget *errordialog_okbutton;
	GtkAccelGroup *accel_group;

	accel_group = gtk_accel_group_new ();

	error_dialog = gtk_dialog_new ();
	gtk_window_set_title (GTK_WINDOW (error_dialog), "");
	gtk_window_set_position (GTK_WINDOW (error_dialog), GTK_WIN_POS_CENTER);
	gtk_window_set_destroy_with_parent (GTK_WINDOW (error_dialog), TRUE);

	dialog_vbox = GTK_DIALOG (error_dialog)->vbox;
	gtk_widget_show (dialog_vbox);

	hbox = gtk_hbox_new (FALSE, 0);
	gtk_widget_show (hbox);
	gtk_box_pack_start (GTK_BOX (dialog_vbox), hbox, TRUE, TRUE, 0);

	image = gtk_image_new_from_stock ("gtk-dialog-warning", GTK_ICON_SIZE_DIALOG);
	gtk_widget_show (image);
	gtk_box_pack_start (GTK_BOX (hbox), image, FALSE, FALSE, 35);

	vbox = gtk_vbox_new (FALSE, 0);
	gtk_widget_show (vbox);
	gtk_box_pack_start (GTK_BOX (hbox), vbox, TRUE, TRUE, 10);

	label_msg = gtk_label_new (NULL);
	gtk_label_set_use_markup(GTK_LABEL(label_msg),TRUE);
	gtk_widget_show (label_msg);
	gtk_box_pack_start (GTK_BOX (vbox), label_msg, TRUE, TRUE, 10);

	label_detail = gtk_label_new (NULL);
	gtk_label_set_line_wrap (GTK_LABEL(label_detail),TRUE);
	gtk_label_set_use_markup(GTK_LABEL(label_detail),TRUE);
	gtk_widget_show (label_detail);	
	gtk_box_pack_start (GTK_BOX (vbox), label_detail, TRUE, TRUE, 10);
	
	dialog_action_area = GTK_DIALOG (error_dialog)->action_area;
	gtk_widget_show (dialog_action_area);
	gtk_button_box_set_layout (GTK_BUTTON_BOX (dialog_action_area),GTK_BUTTONBOX_END);

	errordialog_okbutton = gtk_button_new_from_stock ("gtk-ok");
	gtk_widget_show (errordialog_okbutton);
	gtk_dialog_add_action_widget (GTK_DIALOG (error_dialog),
				      errordialog_okbutton, GTK_RESPONSE_OK);
	GTK_WIDGET_SET_FLAGS (errordialog_okbutton, GTK_CAN_DEFAULT);

	gtk_widget_add_accelerator (errordialog_okbutton, "clicked", accel_group,
				    GDK_O, GDK_CONTROL_MASK,
				    GTK_ACCEL_VISIBLE);
	gtk_widget_add_accelerator (errordialog_okbutton, "clicked", accel_group,
				    GDK_C, GDK_CONTROL_MASK,
				    GTK_ACCEL_VISIBLE);

	g_signal_connect_swapped ((gpointer) error_dialog, "close",
				  G_CALLBACK (gtk_widget_destroy),
				  GTK_OBJECT (error_dialog));
	g_signal_connect_swapped ((gpointer) errordialog_okbutton, "clicked",
				  G_CALLBACK (gtk_widget_destroy),
				  GTK_OBJECT (error_dialog));

	/* Store pointers to all widgets, for use by lookup_widget(). */
	GLADE_HOOKUP_OBJECT_NO_REF (error_dialog, error_dialog,	"error_dialog");
	GLADE_HOOKUP_OBJECT (error_dialog, label_detail, "label_detail");
	GLADE_HOOKUP_OBJECT (error_dialog, image, "image");
	GLADE_HOOKUP_OBJECT (error_dialog, label_msg, "label_msg");
	GLADE_HOOKUP_OBJECT (error_dialog, errordialog_okbutton, "errordialog_okbutton");

	gtk_window_add_accel_group (GTK_WINDOW (error_dialog), accel_group);

	return error_dialog;
}

GtkWidget *
create_about (void)
{
	GtkWidget *dialog_about;
	GdkPixbuf *dialog_about_icon_pixbuf;
	GtkWidget *dialog_vbox;
	GtkWidget *notebook;
	GtkWidget *hbox_author;
	GtkWidget *image_cssed;
	GtkWidget *vbox_author;
	GtkWidget *label_author;
	GtkWidget *label_author_name;
	GtkWidget *label_version;
	GtkWidget *label_app_url;
	GtkWidget *vbox_dev;
	GtkWidget *label_dev_first;
	GtkWidget *label_dev_second;
	//GtkWidget *label_dev_third;
	GtkWidget *label_dev_fourth;
	GtkWidget *label_developers;
	GtkWidget *vbox_translators;
	GtkWidget *label_C_translation;
	GtkWidget *label_translators;
	GtkWidget *dialog_action_area;
	GtkWidget *okbutton;
	gchar* version;
	
	GtkAccelGroup* accel_group;

	version = g_strdup_printf("<span size='larger'>%s - v. %s</span>", PACKAGE, VERSION);

	accel_group = gtk_accel_group_new ();

	dialog_about = gtk_dialog_new ();
	gtk_widget_set_size_request(dialog_about, 400, 200);
	gtk_window_set_title (GTK_WINDOW (dialog_about), _("About cssed"));
	gtk_window_set_position (GTK_WINDOW (dialog_about), GTK_WIN_POS_CENTER);
	gtk_window_set_modal (GTK_WINDOW (dialog_about), TRUE);
	gtk_window_set_destroy_with_parent (GTK_WINDOW (dialog_about), TRUE);
	dialog_about_icon_pixbuf = create_pixbuf_from_data (cssed_icon);
	if (dialog_about_icon_pixbuf)
    {
		gtk_window_set_icon (GTK_WINDOW (dialog_about), dialog_about_icon_pixbuf);
		gdk_pixbuf_unref (dialog_about_icon_pixbuf);
    }

	dialog_vbox = GTK_DIALOG (dialog_about)->vbox;
	gtk_widget_show (dialog_vbox);
	
	notebook = gtk_notebook_new ();
	gtk_widget_show (notebook);
	gtk_box_pack_start (GTK_BOX (dialog_vbox), notebook, TRUE, TRUE, 0);
	
	hbox_author = gtk_hbox_new (FALSE, 0);
	gtk_widget_show (hbox_author);
	gtk_container_add (GTK_CONTAINER (notebook), hbox_author);
	
	image_cssed = create_pixmap_from_data (cssed_about);
	gtk_widget_show (image_cssed);
	gtk_box_pack_start (GTK_BOX (hbox_author), image_cssed, TRUE, TRUE, 0);
	
	vbox_author = gtk_vbox_new (FALSE, 0);
	gtk_widget_show (vbox_author);
	gtk_box_pack_start (GTK_BOX (hbox_author), vbox_author, TRUE, TRUE, 0);
	
	label_author = gtk_label_new (_("<span weight='bold' size='large'><u>Author</u></span>"));
	gtk_widget_show (label_author);
	gtk_box_pack_start (GTK_BOX (vbox_author), label_author, FALSE, FALSE, 5);
	gtk_label_set_use_markup (GTK_LABEL (label_author), TRUE);
	gtk_label_set_justify (GTK_LABEL (label_author), GTK_JUSTIFY_LEFT);
	
	label_author_name = gtk_label_new ("<span size='medium' weight='bold'>Iago Rubio Sanfiz (c) 2003 - 2005</span>");
	gtk_widget_show (label_author_name);
	gtk_box_pack_start (GTK_BOX (vbox_author), label_author_name, TRUE, TRUE, 0);
	gtk_label_set_use_markup (GTK_LABEL (label_author_name), TRUE);
	gtk_label_set_justify (GTK_LABEL (label_author_name), GTK_JUSTIFY_LEFT);
	
	label_version = gtk_label_new (version);
	g_free(version);//not needed anymore
	gtk_widget_show (label_version);
	gtk_box_pack_start (GTK_BOX (vbox_author), label_version, TRUE, TRUE, 0);
	gtk_label_set_use_markup (GTK_LABEL (label_version), TRUE);
	gtk_label_set_justify (GTK_LABEL (label_version), GTK_JUSTIFY_LEFT);
	
	label_app_url = gtk_label_new ("<span weight='bold'><u>http://cssed.sourceforge.net</u></span>");
	gtk_widget_show (label_app_url);
	gtk_box_pack_start (GTK_BOX (vbox_author), label_app_url, FALSE, FALSE, 15);
	gtk_label_set_use_markup (GTK_LABEL (label_app_url), TRUE);
	gtk_label_set_justify (GTK_LABEL (label_app_url), GTK_JUSTIFY_LEFT);
	gtk_label_set_selectable (GTK_LABEL (label_app_url), TRUE);
		
	label_author = gtk_label_new (_("Author"));
	gtk_widget_show (label_author);
	gtk_notebook_set_tab_label (GTK_NOTEBOOK (notebook), gtk_notebook_get_nth_page (GTK_NOTEBOOK (notebook), 0), label_author);
	gtk_label_set_justify (GTK_LABEL (label_author), GTK_JUSTIFY_LEFT);
	//
	vbox_dev = gtk_vbox_new (FALSE, 0);
	gtk_widget_show (vbox_dev);
	gtk_container_add (GTK_CONTAINER (notebook), vbox_dev);
	
	label_dev_first = gtk_label_new ("<span size='medium'>Iago Rubio</span> - <span size='small'>iago.rubio (at) hispalinux.es - http://www.iagorubio.com</span>");
	gtk_widget_show (label_dev_first);
	gtk_box_pack_start (GTK_BOX (vbox_dev), label_dev_first, TRUE, TRUE, 0);
	gtk_label_set_use_markup (GTK_LABEL (label_dev_first), TRUE);
	gtk_label_set_justify (GTK_LABEL (label_dev_first), GTK_JUSTIFY_LEFT);
	
	label_dev_second = gtk_label_new ("<span size='medium'>Michèle Garoche</span> - <span size='small'>michele (dot) garoche (at) easyconnet (dot) fr</span>");
	gtk_widget_show (label_dev_second);
	gtk_box_pack_start (GTK_BOX (vbox_dev), label_dev_second, TRUE, TRUE, 0);
	gtk_label_set_use_markup (GTK_LABEL (label_dev_second), TRUE);
	gtk_label_set_justify (GTK_LABEL (label_dev_second), GTK_JUSTIFY_LEFT);
	
	/* This developer have not been active for this release
	
	label_dev_third = gtk_label_new ("<span size='medium'>Alastair Porter</span> - <span size='small'>trommaster (at) users.sourceforge.net</span>");
	gtk_widget_show (label_dev_third);
	gtk_box_pack_start (GTK_BOX (vbox_dev), label_dev_third, TRUE, TRUE, 0);
	gtk_label_set_use_markup (GTK_LABEL (label_dev_third), TRUE);
	gtk_label_set_justify (GTK_LABEL (label_dev_third), GTK_JUSTIFY_LEFT);
	*/
	
	label_dev_fourth = gtk_label_new ("");
	gtk_widget_show (label_dev_fourth);
	gtk_box_pack_start (GTK_BOX (vbox_dev), label_dev_fourth, TRUE, TRUE, 0);
	gtk_label_set_use_markup (GTK_LABEL (label_dev_fourth), TRUE);
	gtk_label_set_justify (GTK_LABEL (label_dev_fourth), GTK_JUSTIFY_LEFT);
	
	label_developers = gtk_label_new (_("Developers"));
	gtk_widget_show (label_developers);
	gtk_notebook_set_tab_label (GTK_NOTEBOOK (notebook), gtk_notebook_get_nth_page (GTK_NOTEBOOK (notebook), 1), label_developers);
	gtk_label_set_justify (GTK_LABEL (label_developers), GTK_JUSTIFY_LEFT);
	//
	vbox_translators = gtk_vbox_new (FALSE, 0);
	gtk_widget_show (vbox_translators);
	gtk_container_add (GTK_CONTAINER (notebook), vbox_translators);
	
	// translator: put here your name, in your language.po file
	label_C_translation = gtk_label_new (_("This is the default language.\nNo translation is being used."));
	gtk_widget_show (label_C_translation);
	gtk_box_pack_start (GTK_BOX (vbox_translators), label_C_translation, TRUE, TRUE, 0);
	gtk_label_set_use_markup (GTK_LABEL (label_C_translation), TRUE);
	gtk_label_set_justify (GTK_LABEL (label_C_translation), GTK_JUSTIFY_LEFT);
	
	label_translators = gtk_label_new (_("Translator"));
	gtk_widget_show (label_translators);
	gtk_notebook_set_tab_label (GTK_NOTEBOOK (notebook), gtk_notebook_get_nth_page (GTK_NOTEBOOK (notebook), 2), label_translators);
	gtk_label_set_justify (GTK_LABEL (label_translators), GTK_JUSTIFY_LEFT);
	
	dialog_action_area = GTK_DIALOG (dialog_about)->action_area;
	gtk_widget_show (dialog_action_area);
	gtk_button_box_set_layout (GTK_BUTTON_BOX (dialog_action_area), GTK_BUTTONBOX_END);
	
	okbutton = gtk_button_new_from_stock ("gtk-ok");
	gtk_widget_show (okbutton);
	gtk_dialog_add_action_widget (GTK_DIALOG (dialog_about), okbutton, GTK_RESPONSE_OK);
	GTK_WIDGET_SET_FLAGS (okbutton, GTK_CAN_DEFAULT);

  	gtk_widget_add_accelerator (okbutton, "clicked", accel_group,
				    GDK_O, GDK_CONTROL_MASK,
				    GTK_ACCEL_VISIBLE);

	gtk_widget_add_accelerator (okbutton, "clicked", accel_group,
				    GDK_C, GDK_CONTROL_MASK,
				    GTK_ACCEL_VISIBLE);
/*
	g_signal_connect_swapped ((gpointer) dialog_about, "close",
				  G_CALLBACK (gtk_widget_destroy),
				  GTK_OBJECT (dialog_about));
*/
	g_signal_connect_swapped ((gpointer) okbutton, "clicked",
				  G_CALLBACK (gtk_widget_destroy),
				  GTK_OBJECT (dialog_about));

  	gtk_window_add_accel_group (GTK_WINDOW (dialog_about),
				    accel_group);

	return dialog_about;
}

GtkWidget *
create_search_dialog (CssedWindow * window)
{
	GtkWidget *search_dialog;
	GtkWidget *dialog_vbox;
	GtkWidget *vbox;
	GtkWidget *hbox;
	GtkWidget *label;
	GtkWidget *entry_text_to_find;
	GtkWidget *table;
	GtkWidget *checkbutton_regex;
	GtkWidget *radiobutton_perl_regex;
	GtkWidget *radiobutton_regex;
	GtkWidget *checkbutton_match_case;
	GtkWidget *checkbutton_whole_word;
	GtkWidget *checkbutton_word_start;
	GtkWidget *dialog_action_area;
	GtkWidget *dialog_search_help_button;
	GtkWidget *search_dialog_find_button;
	GtkWidget *search_dialog_cancel_button;
	GtkWidget *search_backwards_checkbox;
	CssedDoc* doc;
	GSList *radiobutton_perl_regex_group = NULL;

	search_dialog = gtk_dialog_new ();
	gtk_window_set_title (GTK_WINDOW (search_dialog), _("Search"));
	gtk_window_set_destroy_with_parent (GTK_WINDOW (search_dialog), TRUE);
	gtk_window_set_modal (GTK_WINDOW (search_dialog), FALSE);
	gtk_window_set_transient_for ( GTK_WINDOW(search_dialog),
				      GTK_WINDOW ( cssed_window_get_window_widget(window) ));

	dialog_vbox = GTK_DIALOG (search_dialog)->vbox;
	gtk_widget_show (dialog_vbox);

	vbox = gtk_vbox_new (FALSE, 0);
	gtk_widget_show (vbox);
	gtk_box_pack_start (GTK_BOX (dialog_vbox), vbox, TRUE, TRUE, 0);

	hbox = gtk_hbox_new (FALSE, 0);
	gtk_widget_show (hbox);
	gtk_box_pack_start (GTK_BOX (vbox), hbox, TRUE, TRUE, 0);

	label = gtk_label_new (_("Text to find"));
	gtk_widget_show (label);
	gtk_box_pack_start (GTK_BOX (hbox), label, FALSE, FALSE, 5);
	gtk_label_set_justify (GTK_LABEL (label), GTK_JUSTIFY_LEFT);

	entry_text_to_find = gtk_entry_new ();
	gtk_widget_show (entry_text_to_find);
	gtk_box_pack_start (GTK_BOX (hbox), entry_text_to_find, TRUE, TRUE,
			    3);

	table = gtk_table_new (3, 2, TRUE);
	gtk_widget_show (table);
	gtk_box_pack_start (GTK_BOX (vbox), table, TRUE, TRUE, 0);
	gtk_container_set_border_width (GTK_CONTAINER (table), 6);
	gtk_table_set_row_spacings (GTK_TABLE (table), 5);
	gtk_table_set_col_spacings (GTK_TABLE (table), 5);

	checkbutton_regex =
		gtk_check_button_new_with_mnemonic (_("_Regular expression"));
	gtk_widget_show (checkbutton_regex);
	gtk_table_attach (GTK_TABLE (table), checkbutton_regex, 1, 2, 0, 1,
			  (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
			  (GtkAttachOptions) (0), 0, 0);

	radiobutton_perl_regex =
		gtk_radio_button_new_with_mnemonic (NULL, _("P_erl regexp"));
	gtk_widget_show (radiobutton_perl_regex);
	gtk_table_attach (GTK_TABLE (table), radiobutton_perl_regex, 1, 2, 1,
			  2, (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
			  (GtkAttachOptions) (0), 0, 0);
	gtk_radio_button_set_group (GTK_RADIO_BUTTON (radiobutton_perl_regex),
				    radiobutton_perl_regex_group);
	radiobutton_perl_regex_group =
		gtk_radio_button_get_group (GTK_RADIO_BUTTON
					    (radiobutton_perl_regex));

	radiobutton_regex =
		gtk_radio_button_new_with_mnemonic (NULL, _("P_osix regexp"));
	gtk_widget_show (radiobutton_regex);
	gtk_table_attach (GTK_TABLE (table), radiobutton_regex, 1, 2, 2, 3,
			  (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
			  (GtkAttachOptions) (0), 0, 0);
	gtk_radio_button_set_group (GTK_RADIO_BUTTON (radiobutton_regex),
				    radiobutton_perl_regex_group);
	radiobutton_perl_regex_group =
		gtk_radio_button_get_group (GTK_RADIO_BUTTON
					    (radiobutton_regex));

	checkbutton_match_case =
		gtk_check_button_new_with_mnemonic (_("_Match case"));
	gtk_widget_show (checkbutton_match_case);
	gtk_table_attach (GTK_TABLE (table), checkbutton_match_case, 0, 1, 0,
			  1, (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
			  (GtkAttachOptions) (0), 0, 0);

	checkbutton_whole_word =
		gtk_check_button_new_with_mnemonic (_("_Whole world only"));
	gtk_widget_show (checkbutton_whole_word);
	gtk_table_attach (GTK_TABLE (table), checkbutton_whole_word, 0, 1, 1,
			  2, (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
			  (GtkAttachOptions) (0), 0, 0);

	checkbutton_word_start =
		gtk_check_button_new_with_mnemonic (_("Word _start"));
	gtk_widget_show (checkbutton_word_start);
	gtk_table_attach (GTK_TABLE (table), checkbutton_word_start, 0, 1, 2,
			  3, (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
			  (GtkAttachOptions) (0), 0, 0);

	dialog_action_area = GTK_DIALOG (search_dialog)->action_area;
	gtk_widget_show (dialog_action_area);
	gtk_button_box_set_layout (GTK_BUTTON_BOX (dialog_action_area),
				   GTK_BUTTONBOX_END);

	dialog_search_help_button = gtk_button_new_from_stock ("gtk-help");
	gtk_widget_show (dialog_search_help_button);
	gtk_dialog_add_action_widget (GTK_DIALOG (search_dialog),
				      dialog_search_help_button,
				      GTK_RESPONSE_HELP);
	GTK_WIDGET_SET_FLAGS (dialog_search_help_button, GTK_CAN_DEFAULT);

	search_dialog_find_button = gtk_button_new_from_stock ("gtk-find");
	gtk_widget_show (search_dialog_find_button);
	gtk_dialog_add_action_widget (GTK_DIALOG (search_dialog),
				      search_dialog_find_button,
				      GTK_RESPONSE_CLOSE);
	GTK_WIDGET_SET_FLAGS (search_dialog_find_button, GTK_CAN_DEFAULT);

	search_dialog_cancel_button = gtk_button_new_from_stock ("gtk-close");
	gtk_widget_show (search_dialog_cancel_button);
	gtk_dialog_add_action_widget (GTK_DIALOG (search_dialog),
				      search_dialog_cancel_button,
				      GTK_RESPONSE_CLOSE);
	GTK_WIDGET_SET_FLAGS (search_dialog_cancel_button, GTK_CAN_DEFAULT);

	search_backwards_checkbox = gtk_check_button_new_with_mnemonic (_("Search _backwards"));
	gtk_widget_show (search_backwards_checkbox);
	gtk_box_pack_end(GTK_BOX (dialog_vbox), search_backwards_checkbox, FALSE, FALSE, 10);

	// get the current selected text to the dialog entry
	doc = document_get_current( window );
	if( doc ){
		gint sel_start;
		gint sel_end;

		sel_start = document_get_selection_start(doc);
		sel_end = document_get_selection_end( doc );
		if(  sel_end > sel_start  ){
			gchar* sel_text;
			sel_text = g_malloc0 (sel_end - sel_start + 2);
			document_get_selected_text( doc, sel_text );
			gtk_entry_set_text( GTK_ENTRY(entry_text_to_find), sel_text );
			g_free( sel_text );
		}
	}
	g_signal_connect_swapped ((gpointer) search_dialog, "destroy",
				  G_CALLBACK (gtk_widget_destroy),
				  G_OBJECT (search_dialog));
	g_signal_connect ((gpointer) dialog_search_help_button, "clicked",
			  G_CALLBACK (on_dialog_search_help_button_clicked),
			  NULL);
	g_signal_connect_swapped ((gpointer) search_dialog_cancel_button,
				  "clicked", G_CALLBACK (gtk_widget_destroy),
				  G_OBJECT (search_dialog));
	g_signal_connect ((gpointer) search_dialog_find_button, "clicked",
			  G_CALLBACK (on_search_dialog_find_button_clicked),
			  window);

	GLADE_HOOKUP_OBJECT_NO_REF (search_dialog, search_dialog, "search_dialog");
	GLADE_HOOKUP_OBJECT (search_dialog, entry_text_to_find, "entry_text_to_find");
	GLADE_HOOKUP_OBJECT (search_dialog, checkbutton_regex, "checkbutton_regex");
	GLADE_HOOKUP_OBJECT (search_dialog, radiobutton_perl_regex, "radiobutton_perl_regex");
	GLADE_HOOKUP_OBJECT (search_dialog, radiobutton_regex, "radiobutton_regex");
	GLADE_HOOKUP_OBJECT (search_dialog, checkbutton_match_case, "checkbutton_match_case");
	GLADE_HOOKUP_OBJECT (search_dialog, checkbutton_whole_word, "checkbutton_whole_word");
	GLADE_HOOKUP_OBJECT (search_dialog, checkbutton_word_start, "checkbutton_word_start");
	GLADE_HOOKUP_OBJECT (search_dialog, dialog_search_help_button, "dialog_search_help_button");
	GLADE_HOOKUP_OBJECT (search_dialog, search_backwards_checkbox, "search_backwards_checkbox");
	GLADE_HOOKUP_OBJECT (search_dialog, search_dialog_find_button, "search_dialog_find_button");
	GLADE_HOOKUP_OBJECT (search_dialog, search_dialog_cancel_button, "search_dialog_cancel_button");

	return search_dialog;
}

GtkWidget *
create_replace_dialog (CssedWindow * window)
{
	GtkWidget *replace_dialog;
	GtkWidget *vbox_main;
	GtkWidget *vbox;
	GtkWidget *label_search_for;
	GtkWidget *entry_find;
	GtkWidget *label_replace_with;
	GtkWidget *entry_replace;
	GtkWidget *table_text_entries;
	GtkWidget *table;
	GtkWidget *checkbutton_regex;
	GtkWidget *radiobutton_perl_regex;
	GSList *radiobutton_perl_regex_group = NULL;
	GtkWidget *radiobutton_posix_regex;
	GtkWidget *checkbutton_match_case;
	GtkWidget *checkbutton_whole_word;
	GtkWidget *checkbutton_word_start;
	GtkWidget *hbuttonbox;
	GtkWidget *dialog_replace_help_button;
	GtkWidget *dialog_replace_button;
	GtkWidget *dialog_replace_apply_button;
	GtkWidget *dialog_replace_close_button;
	GtkWidget *search_backwards_checkbox;
	CssedDoc* doc;

	replace_dialog = gtk_dialog_new ();
	gtk_window_set_title (GTK_WINDOW (replace_dialog), _("Replace"));
	gtk_window_set_destroy_with_parent (GTK_WINDOW (replace_dialog), TRUE);
	gtk_window_set_transient_for ( GTK_WINDOW(replace_dialog),
				      GTK_WINDOW ( cssed_window_get_window_widget(window) ));

	vbox_main = GTK_DIALOG (replace_dialog)->vbox;
	gtk_widget_show (vbox_main);

	vbox = gtk_vbox_new (FALSE, 0);
	gtk_widget_show (vbox);
	gtk_box_pack_start (GTK_BOX (vbox_main), vbox, TRUE, TRUE, 0);

	table_text_entries = gtk_table_new (2, 2, FALSE);
	gtk_widget_show (table_text_entries);
	gtk_box_pack_start (GTK_BOX (vbox), table_text_entries, TRUE, TRUE, 0);
	gtk_table_set_col_spacings (GTK_TABLE (table_text_entries), 5);


	label_search_for = gtk_label_new (_("Search for   "));
	gtk_widget_show (label_search_for);
	gtk_table_attach (GTK_TABLE (table_text_entries), label_search_for, 0, 1, 0, 1,
					(GtkAttachOptions) GTK_FILL,
					(GtkAttachOptions) (0), 0, 0);
	gtk_label_set_justify (GTK_LABEL (label_search_for), GTK_JUSTIFY_LEFT);

	entry_find = gtk_entry_new ();
	gtk_widget_show (entry_find);
	gtk_table_attach (GTK_TABLE (table_text_entries), entry_find, 1, 2, 0, 1,
					(GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
					(GtkAttachOptions) (0), 0, 0);	

	label_replace_with = gtk_label_new (_("Replace with"));
	gtk_widget_show (label_replace_with);
	gtk_label_set_justify (GTK_LABEL (label_replace_with), GTK_JUSTIFY_LEFT);
	gtk_table_attach (GTK_TABLE (table_text_entries), label_replace_with, 0, 1, 1, 2,
					(GtkAttachOptions) GTK_FILL,
					(GtkAttachOptions) (0), 0, 0);	
					
	entry_replace = gtk_entry_new ();
	gtk_widget_show (entry_replace);
	gtk_label_set_justify (GTK_LABEL (label_replace_with), GTK_JUSTIFY_LEFT);
	gtk_table_attach (GTK_TABLE (table_text_entries), entry_replace, 1, 2, 1, 2,
					(GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
					(GtkAttachOptions) (0), 0, 0);	

	table = gtk_table_new (3, 2, TRUE);
	gtk_widget_show (table);
	gtk_box_pack_start (GTK_BOX (vbox), table, FALSE, FALSE, 1);
	gtk_table_set_row_spacings (GTK_TABLE (table), 5);
	gtk_table_set_col_spacings (GTK_TABLE (table), 5);

	checkbutton_regex =
		gtk_check_button_new_with_mnemonic (_("Regular expression"));
	gtk_widget_show (checkbutton_regex);
	gtk_table_attach (GTK_TABLE (table), checkbutton_regex, 1, 2, 0, 1,
			  (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
			  (GtkAttachOptions) (0), 0, 0);

	radiobutton_perl_regex =
		gtk_radio_button_new_with_mnemonic (NULL, _("Perl regex"));
	gtk_widget_show (radiobutton_perl_regex);
	gtk_table_attach (GTK_TABLE (table), radiobutton_perl_regex, 1, 2, 1,
			  2, (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
			  (GtkAttachOptions) (0), 0, 0);
	gtk_radio_button_set_group (GTK_RADIO_BUTTON (radiobutton_perl_regex),
				    radiobutton_perl_regex_group);
	radiobutton_perl_regex_group =
		gtk_radio_button_get_group (GTK_RADIO_BUTTON
					    (radiobutton_perl_regex));

	radiobutton_posix_regex =
		gtk_radio_button_new_with_mnemonic (NULL, _("Posix regex"));
	gtk_widget_show (radiobutton_posix_regex);
	gtk_table_attach (GTK_TABLE (table), radiobutton_posix_regex, 1, 2,
			  2, 3, (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
			  (GtkAttachOptions) (0), 0, 0);
	gtk_radio_button_set_group (GTK_RADIO_BUTTON
				    (radiobutton_posix_regex),
				    radiobutton_perl_regex_group);
	radiobutton_perl_regex_group =
		gtk_radio_button_get_group (GTK_RADIO_BUTTON
					    (radiobutton_posix_regex));

	checkbutton_match_case =
		gtk_check_button_new_with_mnemonic (_("Match case"));
	gtk_widget_show (checkbutton_match_case);
	gtk_table_attach (GTK_TABLE (table), checkbutton_match_case, 0, 1, 0,
			  1, (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
			  (GtkAttachOptions) (0), 0, 0);

	checkbutton_whole_word =
		gtk_check_button_new_with_mnemonic (_("Whole world only"));
	gtk_widget_show (checkbutton_whole_word);
	gtk_table_attach (GTK_TABLE (table), checkbutton_whole_word, 0, 1, 1,
			  2, (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
			  (GtkAttachOptions) (0), 0, 0);

	checkbutton_word_start =
		gtk_check_button_new_with_mnemonic (_("Word start"));
	gtk_widget_show (checkbutton_word_start);
	gtk_table_attach (GTK_TABLE (table), checkbutton_word_start, 0, 1, 2,
			  3, (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
			  (GtkAttachOptions) (0), 0, 0);

	hbuttonbox = GTK_DIALOG (replace_dialog)->action_area;
	gtk_widget_show (hbuttonbox);
	gtk_button_box_set_layout (GTK_BUTTON_BOX (hbuttonbox),
				   GTK_BUTTONBOX_END);

	dialog_replace_help_button = gtk_button_new_from_stock ("gtk-help");
	gtk_widget_show (dialog_replace_help_button);
	gtk_dialog_add_action_widget (GTK_DIALOG (replace_dialog),
				      dialog_replace_help_button,
				      GTK_RESPONSE_HELP);
	GTK_WIDGET_SET_FLAGS (dialog_replace_help_button, GTK_CAN_DEFAULT);

	dialog_replace_button = gtk_button_new_from_stock ("gtk-find");
	gtk_widget_show (dialog_replace_button);
	gtk_dialog_add_action_widget (GTK_DIALOG (replace_dialog),
				      dialog_replace_button, 0);
	GTK_WIDGET_SET_FLAGS (dialog_replace_button, GTK_CAN_DEFAULT);

	dialog_replace_apply_button = gtk_button_new_from_stock ("gtk-apply");
	gtk_widget_show (dialog_replace_apply_button);
	gtk_dialog_add_action_widget (GTK_DIALOG (replace_dialog),
				      dialog_replace_apply_button,
				      GTK_RESPONSE_APPLY);
	GTK_WIDGET_SET_FLAGS (dialog_replace_apply_button, GTK_CAN_DEFAULT);

	dialog_replace_close_button = gtk_button_new_from_stock ("gtk-close");
	gtk_widget_show (dialog_replace_close_button);
	gtk_dialog_add_action_widget (GTK_DIALOG (replace_dialog),
				      dialog_replace_close_button,
				      GTK_RESPONSE_CLOSE);
	GTK_WIDGET_SET_FLAGS (dialog_replace_close_button, GTK_CAN_DEFAULT);

	search_backwards_checkbox = gtk_check_button_new_with_mnemonic (_("Search _backwards"));
	gtk_widget_show (search_backwards_checkbox);
	gtk_box_pack_end(GTK_BOX (vbox_main), search_backwards_checkbox, FALSE, FALSE, 10);

	// get the current selected text to the dialog entry
	doc = document_get_current( window );
	if( doc ){
		gint sel_start, sel_end;
		sel_start = document_get_selection_start(doc);
		sel_end = document_get_selection_end( doc );
		if(  sel_end > sel_start  ){
			gchar* sel_text;
			sel_text = g_malloc0 (sel_end - sel_start + 2);
			document_get_selected_text( doc, sel_text );
			gtk_entry_set_text( GTK_ENTRY(entry_find), sel_text );
			g_free( sel_text );
		}
	}

	g_signal_connect_swapped ((gpointer) replace_dialog, "destroy",
				  G_CALLBACK (gtk_widget_destroy), G_OBJECT (replace_dialog));
	g_signal_connect ((gpointer) dialog_replace_help_button, "clicked",
			  G_CALLBACK (on_dialog_replace_help_button_clicked), NULL);

	g_signal_connect ((gpointer) dialog_replace_apply_button, "clicked",
			  G_CALLBACK (on_dialog_replace_apply_button_clicked),  window);
	g_signal_connect ((gpointer) dialog_replace_button, "clicked",
				G_CALLBACK (on_dialog_replace_search_button_clicked), window);

	g_signal_connect_swapped ((gpointer) dialog_replace_close_button,
				  "clicked", G_CALLBACK (gtk_widget_destroy), G_OBJECT (replace_dialog));

	GLADE_HOOKUP_OBJECT_NO_REF (replace_dialog, replace_dialog,  "replace_dialog");
	GLADE_HOOKUP_OBJECT (replace_dialog, entry_find, "entry_find");
	GLADE_HOOKUP_OBJECT (replace_dialog, entry_replace, "entry_replace");
	GLADE_HOOKUP_OBJECT (replace_dialog, checkbutton_regex, "checkbutton_regex");
	GLADE_HOOKUP_OBJECT (replace_dialog, radiobutton_perl_regex, "radiobutton_perl_regex");
	GLADE_HOOKUP_OBJECT (replace_dialog, radiobutton_posix_regex, "radiobutton_posix_regex");
	GLADE_HOOKUP_OBJECT (replace_dialog, checkbutton_match_case, "checkbutton_match_case");
	GLADE_HOOKUP_OBJECT (replace_dialog, checkbutton_whole_word, "checkbutton_whole_word");
	GLADE_HOOKUP_OBJECT (replace_dialog, checkbutton_word_start, "checkbutton_word_start");
	GLADE_HOOKUP_OBJECT (replace_dialog, search_backwards_checkbox, "search_backwards_checkbox");
	GLADE_HOOKUP_OBJECT (replace_dialog, dialog_replace_help_button, "dialog_replace_help_button");
	GLADE_HOOKUP_OBJECT (replace_dialog, dialog_replace_button, "dialog_replace_button");
	GLADE_HOOKUP_OBJECT (replace_dialog, dialog_replace_apply_button, "dialog_replace_apply_button");
	GLADE_HOOKUP_OBJECT (replace_dialog, dialog_replace_close_button, "dialog_replace_close_button");

	return replace_dialog;
}

void
set_css_properties_treeview (CssedWindow* window)
{

	GtkTreeViewColumn *col;
	GtkCellRenderer *renderer;
	GtkTreeStore *model;
	GtkWidget *treeview;
	GtkTreeIter toplevel, child, third;
	Tcssed_tp_data data;

	treeview = cssed_window_get_css_definition_treeview ( window );

	/* --- Column #1 --- */
	col = gtk_tree_view_column_new ();
	gtk_tree_view_column_set_title (col, "Version");
	gtk_tree_view_append_column (GTK_TREE_VIEW (treeview), col);
	renderer = gtk_cell_renderer_text_new ();
	gtk_tree_view_column_pack_start (col, renderer, TRUE);
	gtk_tree_view_column_add_attribute (col, renderer, "text", COL_CSS_VER);

	/* --- Column #2 --- */
	col = gtk_tree_view_column_new ();
	gtk_tree_view_column_set_title (col, "Property");
	gtk_tree_view_append_column (GTK_TREE_VIEW (treeview), col);
	renderer = gtk_cell_renderer_text_new ();
	gtk_tree_view_column_pack_start (col, renderer, TRUE);
	gtk_tree_view_column_add_attribute (col, renderer, "text", COL_ATTR);

	/* --- Column #3 --- */
	col = gtk_tree_view_column_new ();
	gtk_tree_view_column_set_title (col, "Value");
	gtk_tree_view_append_column (GTK_TREE_VIEW (treeview), col);
	renderer = gtk_cell_renderer_text_new ();
	gtk_tree_view_column_pack_start (col, renderer, TRUE);
	gtk_tree_view_column_add_attribute (col, renderer, "text", COL_VALUE);

	model = gtk_tree_store_new (NUM_COLS, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING);
	gtk_tree_view_set_search_column (GTK_TREE_VIEW (treeview), COL_ATTR);
	
	// fill data struct values to pass to func
	data.first_col = &toplevel;
	data.second_col = &child;
	data.thrid_col = &third;
	data.store = model;
	data.window = window;
	// parse the xml and fill the values on the treeview
	parse_css_definition_and_fill_tree_view (&data);

	gtk_tree_view_set_model (GTK_TREE_VIEW (treeview), GTK_TREE_MODEL (model));
	g_object_unref (GTK_TREE_MODEL (model));
	gtk_tree_selection_set_mode (gtk_tree_view_get_selection(GTK_TREE_VIEW (treeview)), GTK_SELECTION_SINGLE);

}
