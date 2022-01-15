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

#include <gtk/gtk.h>
#include <string.h>

#ifndef WIN32
#ifdef DARWIN
# include <sys/types.h>
#endif
# include <unistd.h>
# include <sys/mman.h>
# include <libgen.h>
#endif

//#define WITH_SPLITTER_WINDOW

#include "cssedwindow.h"
#include "document.h"
#include "file-type-manager.h"
#include "plugin.h"
#include "configdlg-interface.h"
#include "cssdialogs-hash.h"
#include "cssdialogs-class.h"
#include "support.h"
#include "utils.h"
#include <pixmaps/images.h>

#define CSSED_DYNWIDGETS(obj) ((obj)->dynamic_widgets)
#define CSSED_PANES(obj) ((obj)->panes)
#define CSSED_FILETYPE_MANAGER(obj) ((obj)->manager)

typedef struct _CssedWindowPanes {
	GtkWidget* vpaned_main;
	GtkWidget* hpaned_up;
	GtkWidget* vpaned_sidebar;		
} CssedWindowPanes;

typedef struct _CssedDynamicWidgets { // those widgets are managed by the app
	GtkWidget* save_button;			
	GtkWidget* save_main_menu;	
	GtkWidget* undo_button;
	GtkWidget* undo_main_menu;
	GtkWidget* undo_pop_menu;
	GtkWidget* redo_button;
	GtkWidget* redo_main_menu;
	GtkWidget* redo_pop_menu;
} CssedDynamicWidgets;

typedef struct _CssedDynamicMenus { // those menus change their state depending on current document
	GtkWidget *menu_item_line_numbers;
	GtkWidget *menu_item_endings;
	GtkWidget *menu_item_white_spaces;
	GtkWidget *menu_item_lines_wrapped;
	GtkWidget *menu_item_eol_mode_cr;
	GtkWidget *menu_item_eol_mode_lf;
	GtkWidget *menu_item_eol_mode_crlf;
	GtkWidget *menu_item_autocompletion;
	GtkWidget *menu_item_folding;
} CssedDynamicMenus;

struct _CssedWindow {
	GtkWidget* window_widget;				// the window widget
	GtkWidget* notebook_docs;				// the documents notebook
	GtkWidget* notebook_utils;  			// the output and scratch pad notebook
	GtkWidget* notebook_sidebar;			// the side bar notebook
	GtkWidget* tree_view_css_properties;	// the tree with CSS def from XML parsing
	GtkWidget* tree_view_css_current;		// The current selector ( selector scanner ) properties
	GtkWidget* tree_view_output;			// The program output will be stored here
	GtkWidget* tree_view_digest;			// this is for the document parser
	GtkWidget* pop_menu;					// the editor pop menu
	GtkWidget* filetypes_menu;				// the file types menu
	gchar* keywords_list;					// a space separated string with all def keywords from XML parsing	
	gchar* last_open_dir;					// last open directory
	gchar* last_save_dir;					// last saved directory	
	GList* plugins_list;					// all enabled plugins should be here	
	GList* document_list;					// the document list	
	GtkTextBuffer* scratch_buffer;			// the scratch pad
	GtkTextBuffer* static_buffer;			// the static pad	
	GHashTable* cssdialogs_hash;			// a hash containing css dialog classes
	CssedConfig* config;					// configuration data
	CssedDynamicMenus* dynamic_menus;		// the dynamic view menu, must change on a per-document basis
	CssedDynamicWidgets* dynamic_widgets;	// those are changed by the app (not the user)
	CssedWindowPanes* panes;				// the panes that divide the app's UI
	CssedFileTypeManager* manager;			// TESTING here file type manager
};

// prototypes for this module
CssedDynamicMenus* cssed_window_dynamic_menus_new (void);
CssedDynamicWidgets* cssed_window_dynamic_widgets_new (void);
CssedWindowPanes* cssed_window_panes_new (void);

// to dispose lists memory using g_list_foreach
void   
free_data_foreach (gpointer data,  gpointer user_data)
{
	g_free(data);
}

// to unload plugins using g_list_foreach
void
unload_plugins_foreach (gpointer data,  gpointer user_data)
{
	cssed_unload_plugin((CssedPlugin*) data);
}

// window functions
CssedWindow*
cssed_window_new()
{
	CssedWindow* window;
	
	window = g_malloc(sizeof (CssedWindow));
	window->window_widget = NULL;
	window->notebook_docs = NULL;
	window->notebook_utils = NULL;
	window->notebook_sidebar = NULL;
	window->tree_view_css_properties = NULL;
	window->tree_view_css_current = NULL;
	window->tree_view_output = NULL;
	window->tree_view_digest = NULL;
	window->dynamic_menus = cssed_window_dynamic_menus_new();	
	window->document_list = NULL;
	window->keywords_list = g_strdup("");
	window->config = cssed_window_config_new();
	window->cssdialogs_hash = create_and_fill_cssdialogs_hash_table();
	window->plugins_list = NULL;
	window->last_open_dir = NULL;
	window->last_save_dir = NULL;
	//window->last_scan_file = NULL;
	window->pop_menu = NULL;
	window->scratch_buffer = NULL;
	window->dynamic_widgets = cssed_window_dynamic_widgets_new();
	window->panes = cssed_window_panes_new();
	window->manager = cssed_file_type_manager_new();	

	return window;
}

void
cssed_window_destroy (CssedWindow* window)
{
	gtk_widget_hide(window->window_widget);	
	g_free(window->dynamic_menus);	
	g_list_foreach(window->document_list, free_data_foreach , NULL);
	g_list_free(window->document_list);
	g_list_foreach(window->plugins_list, unload_plugins_foreach, NULL);
	g_list_free(window->plugins_list);
	g_hash_table_destroy (window->cssdialogs_hash);	
	if( window->last_open_dir )	g_free(window->last_open_dir);
	if( window->last_save_dir )	g_free(window->last_save_dir);
	if( window->config->browser_command ) g_free(window->config->browser_command);
	g_free(window->config);
	g_free(window->dynamic_widgets);
	g_free(window->panes);
	cssed_window_free_keyword_list(window);	
	cssed_file_type_manager_free(window->manager);
	g_free(window);
	gtk_main_quit ();	
}

void
cssed_window_quit( CssedWindow* window )
{
	CssedDoc* tmpdoc;
	GtkWidget* window_widget;
	GList* doclist;
	GList* unsaved_docs = NULL;
	gint ntosave;
	GtkWidget* dlg;
	gint response;
	gint untitled_count = 0;
	gchar* filename, *name;

	doclist = g_list_first((GList*)  window->document_list);
	window_widget = cssed_window_get_window_widget(window); 
	while( doclist != NULL ){
		tmpdoc = (CssedDoc*) doclist->data;
		if( document_get_modified(tmpdoc) ){
			unsaved_docs = g_list_append(unsaved_docs, tmpdoc);
		}
		doclist = g_list_next(doclist);
	}
	ntosave = g_list_length(unsaved_docs);
	if( ntosave > 0 ){
		dlg = create_unsaved_files_dialog();
		response = gtk_dialog_run(GTK_DIALOG(dlg));
		gtk_widget_destroy(dlg);
		switch( response ){
			case GTK_RESPONSE_OK: 
				while( unsaved_docs != NULL ){
					tmpdoc = (CssedDoc*) unsaved_docs->data;
					filename = document_get_filename(tmpdoc);
					if( filename != NULL ){
						document_save((CssedDoc*) tmpdoc);
						g_free(filename);
					}else{
						name = g_strdup_printf(_("untitled_%d"),untitled_count);
						document_set_filename(tmpdoc, name);
						document_save((CssedDoc*) tmpdoc);
						untitled_count++;
						g_free(name);
					}
					unsaved_docs = g_list_next(unsaved_docs);
				}
				break;
			case GTK_RESPONSE_ACCEPT: // choose files
				while( unsaved_docs != NULL ){
					tmpdoc = (CssedDoc*) unsaved_docs->data;
					filename = document_get_filename(tmpdoc);
					if( filename == NULL ){
						filename = g_strdup_printf(_("untitled_%d"),untitled_count);
						document_set_filename(tmpdoc, filename);
						untitled_count++;
					}
		
					dlg = create_yes_no_cancel_dialog(_("Do you want to save this file?"), _("File name: %s"), filename);
					g_free(filename);
					response = gtk_dialog_run(GTK_DIALOG(dlg));
					gtk_widget_destroy(dlg);

					if( response == GTK_RESPONSE_YES ){
						document_save(tmpdoc);
						unsaved_docs = g_list_next(unsaved_docs);
					}else if ( response == GTK_RESPONSE_NO ){
						unsaved_docs = g_list_next(unsaved_docs);
					}else{ // this is cancel or close so aborting
						return;
					}
 				}
				break;
			case GTK_RESPONSE_CANCEL: // canceled by user don't quit
				return;
		}		
	}	
	save_config_to_file(window);
	scratch_pad_save_contents(window->static_buffer);
	cssed_window_destroy(window);
}
// last open dir / last open file
void
cssed_window_set_last_open_dir( CssedWindow* window, gchar* dirname )
{
	g_return_if_fail(dirname != NULL);
	
	if( window->last_open_dir != NULL ){
		g_free(window->last_open_dir);
	}
	window->last_open_dir = g_strdup(dirname);
}

gint 
cssed_window_get_width( CssedWindow* window )
{
	gint width, height;
 	gtk_window_get_size(GTK_WINDOW(window->window_widget), &width, &height);
	return width;	
}

gint 
cssed_window_get_height( CssedWindow* window )
{
	gint width, height;
 	gtk_window_get_size(GTK_WINDOW(window->window_widget), &width, &height);
	return height;		
}

gchar*
cssed_window_get_last_open_dir( CssedWindow* window )
{
	if( window->last_open_dir != NULL )
		return g_strdup(window->last_open_dir);	
	else
		return NULL;
}

void
cssed_window_set_last_save_dir( CssedWindow* window, gchar* dirname )
{
	g_return_if_fail(dirname != NULL);	
	if( window->last_save_dir != NULL )
		g_free(window->last_save_dir);
	window->last_save_dir = g_strdup(dirname);
}

gchar*
cssed_window_get_last_save_dir( CssedWindow* window )
{
	if( window->last_save_dir == NULL ) return NULL;
	return g_strdup(window->last_save_dir);	

}
/* This should be moved to the CSS module FIXME
void
cssed_window_set_last_scan_file( CssedWindow* window, gchar* filename )
{
	if( window->last_scan_file != NULL ){
		g_free(window->last_scan_file);
	}
	if( filename == NULL ){
		window->last_scan_file = filename;
	}else{
		window->last_scan_file = g_strdup(filename);
	}
}

gchar*
cssed_window_get_last_scan_file( CssedWindow* window )
{
	if( window->last_scan_file == NULL ) return NULL;
	return g_strdup(window->last_scan_file);	

}
*/

CssedConfig*
cssed_window_get_config( CssedWindow* window )
{
	return window->config;
}

CssedConfig*
cssed_window_config_new ()
{
	return g_malloc0(sizeof(CssedConfig));
}

// get/set window object fields
GtkWidget* 
cssed_window_get_window_widget( CssedWindow* window )
{
	return window->window_widget;
}

void
cssed_window_set_window_widget( CssedWindow* window, GtkWidget* widget )
{
	window->window_widget = widget;
}
//
GtkWidget* 
cssed_window_get_document_notebook ( CssedWindow* window )
{
	return window->notebook_docs;
}

void 
cssed_window_set_document_notebook ( CssedWindow* window, GtkWidget* widget )
{
	window->notebook_docs = widget;
}
//
GtkWidget* 
cssed_window_get_footer_notebook ( CssedWindow* window )
{
	return window->notebook_utils;
}

void 
cssed_window_set_footer_notebook ( CssedWindow* window, GtkWidget* widget )
{
	window->notebook_utils = widget;
}
//
GtkWidget* 
cssed_window_get_sidebar_notebook ( CssedWindow* window )
{
	return window->notebook_sidebar;
}

void 
cssed_window_set_sidebar_notebook ( CssedWindow* window, GtkWidget* widget )
{
	window->notebook_sidebar = widget;
}
//
GtkWidget* 
cssed_window_get_css_definition_treeview ( CssedWindow* window )
{
	return window->tree_view_css_properties;
}

void 
cssed_window_set_css_definition_treeview ( CssedWindow* window, GtkWidget* widget )
{
	window->tree_view_css_properties = widget;
}
//
GtkWidget* 
cssed_window_get_selector_scanner_treeview ( CssedWindow* window )
{
	return window->tree_view_css_current;
}

void 
cssed_window_set_selector_scanner_treeview ( CssedWindow* window, GtkWidget* widget )
{
	window->tree_view_css_current = widget;
}
//
GtkWidget* 
cssed_window_get_output_treeview ( CssedWindow* window )
{
	return window->tree_view_output;
}

void 
cssed_window_set_output_treeview ( CssedWindow* window, GtkWidget* widget )
{
	window->tree_view_output = widget;
}
//
GtkWidget* 
cssed_window_get_digest_treeview ( CssedWindow* window )
{
	return window->tree_view_digest;
}

void 
cssed_window_set_digest_treeview ( CssedWindow* window, GtkWidget* widget )
{
	window->tree_view_digest = widget;
}
//
GtkWidget* 
cssed_window_get_pop_menu ( CssedWindow* window )
{
	return window->pop_menu;
}

void cssed_window_set_pop_menu ( CssedWindow* window, GtkWidget* widget )
{
	window->pop_menu = widget;
}

//

gchar*
cssed_window_get_scratch_buffer  (CssedWindow* window)
{
	GtkTextIter start, end;
	gchar *text;
	gint len;
	
	gtk_text_buffer_get_start_iter (window->scratch_buffer, &start);
	gtk_text_buffer_get_end_iter (window->scratch_buffer, &end);
	
	text =  gtk_text_buffer_get_text (window->scratch_buffer, &start, &end, TRUE);
	len = strlen(text);
	if(  len > 0 ){
		if( text[len-1] == '\n' ) text[len-1] = '\0'; //take out last line end
		return text;
	}
	
	g_free (text);
	return NULL;
}

gchar*
cssed_window_get_static_buffer  (CssedWindow* window)
{
	GtkTextIter start, end;
	gchar *text;

	gtk_text_buffer_get_start_iter (window->static_buffer, &start);
	gtk_text_buffer_get_end_iter (window->static_buffer, &end);
	
	text =  gtk_text_buffer_get_text (window->static_buffer, &start, &end, TRUE);

	if( strlen(text) > 0 )
		return text;
	
	g_free (text);
	return NULL;
}

void
cssed_window_clear_scratch_buffer  (CssedWindow* window)
{
	GtkTextIter start, end;
	
	gtk_text_buffer_get_start_iter (window->scratch_buffer, &start);
	gtk_text_buffer_get_end_iter (window->scratch_buffer, &end);
	
	gtk_text_buffer_delete (window->scratch_buffer, &start, &end);
}

void
cssed_window_clear_static_buffer  (CssedWindow* window)
{
	GtkTextIter start, end;
	
	gtk_text_buffer_get_start_iter (window->static_buffer, &start);
	gtk_text_buffer_get_end_iter (window->static_buffer, &end);
	
	gtk_text_buffer_delete (window->static_buffer, &start, &end);
}

void
cssed_window_scratch_buffer_write  (CssedWindow* window, gchar* text)
{
	gtk_text_buffer_insert_at_cursor (window->scratch_buffer, (const gchar*)text, strlen(text));
}

void
cssed_window_static_buffer_write  (CssedWindow* window, gchar* text)
{
	gtk_text_buffer_insert_at_cursor (window->static_buffer, (const gchar*)text, strlen(text));
}

void
cssed_window_set_static_buffer  (CssedWindow* window, GtkTextBuffer* widget)
{
	window->static_buffer = widget;
}

void 
cssed_window_set_scratch_buffer ( CssedWindow* window, GtkTextBuffer* widget )
{
	window->scratch_buffer = widget;
}

//
GtkWidget* 
cssed_window_get_filetypes_menu  (CssedWindow* window )
{
	return window->filetypes_menu;
}
void
cssed_window_set_filetypes_menu  (CssedWindow* window, GtkWidget* menu )
{	
	window->filetypes_menu = menu;
}
//
void
cssed_window_set_filetype_manager( CssedWindow* window, CssedFileTypeManager* manager)
{
	window->manager = manager;
}

CssedFileTypeManager*
cssed_window_get_filetype_manager( CssedWindow* window)
{
	return window->manager;
}

// dynamic menus
CssedDynamicMenus* 
cssed_window_dynamic_menus_new( void )
{
	CssedDynamicMenus* menus;
	menus = g_malloc (sizeof (CssedDynamicMenus));
	return menus;
}

void 
cssed_window_dynamic_menus_set_full( CssedWindow* window,
										  GtkWidget *menu_item_line_numbers,
										  GtkWidget *menu_item_endings,
										  GtkWidget *menu_item_white_spaces,
										  GtkWidget *menu_item_lines_wrapped,
										  GtkWidget *menu_item_eol_mode_cr,
										  GtkWidget *menu_item_eol_mode_lf,
										  GtkWidget *menu_item_eol_mode_crlf,
										  GtkWidget *menu_item_autocompletion,
										  GtkWidget *menu_item_folding )
{
	CssedDynamicMenus* menus;
	
	menus = window->dynamic_menus;	
	menus->menu_item_line_numbers = menu_item_line_numbers; 
	menus->menu_item_endings = menu_item_endings;
	menus->menu_item_white_spaces = menu_item_white_spaces;
	menus->menu_item_lines_wrapped = menu_item_lines_wrapped;
	menus->menu_item_eol_mode_cr = menu_item_eol_mode_cr;
	menus->menu_item_eol_mode_lf = menu_item_eol_mode_lf;
	menus->menu_item_eol_mode_crlf = menu_item_eol_mode_crlf;
	menus->menu_item_autocompletion = menu_item_autocompletion;
	menus->menu_item_folding = menu_item_folding;	
}	

void 
cssed_window_dynamic_menus_set_state(CssedWindow* window,
										  gboolean line_numbers_enabled,
										  gboolean line_endings_enabled,
										  gboolean white_spaces_enabled,
										  gboolean menu_item_lines_wrapped,
										  gboolean autocompletion_enabled,
										  gboolean folding_enabled )
{
	CssedDynamicMenus* menus;
	
	menus = window->dynamic_menus;	
	
	gtk_check_menu_item_set_active  (GTK_CHECK_MENU_ITEM(menus->menu_item_line_numbers),
                                     line_numbers_enabled);
	gtk_check_menu_item_set_active  (GTK_CHECK_MENU_ITEM(menus->menu_item_endings),
                                     line_endings_enabled);	
	gtk_check_menu_item_set_active  (GTK_CHECK_MENU_ITEM(menus->menu_item_white_spaces),
                                     white_spaces_enabled);
	gtk_check_menu_item_set_active  (GTK_CHECK_MENU_ITEM(menus->menu_item_lines_wrapped),
                                     menu_item_lines_wrapped);	
	gtk_check_menu_item_set_active  (GTK_CHECK_MENU_ITEM(menus->menu_item_autocompletion),
                                     autocompletion_enabled);	
	gtk_check_menu_item_set_active  (GTK_CHECK_MENU_ITEM(menus->menu_item_folding),
                                     folding_enabled);		
	
}										  
										  
void 
cssed_window_dynamic_menus_set_eols_state( CssedWindow* window, CssedEolMode eols )
{
	CssedDynamicMenus* menus;
	
	menus = window->dynamic_menus;	

	switch( eols ){
		case EOL_MODE_CR:
			gtk_check_menu_item_set_active  (
				GTK_CHECK_MENU_ITEM(menus->menu_item_eol_mode_cr),
                TRUE);				
			break;
		case EOL_MODE_LF:
			gtk_check_menu_item_set_active  (
				GTK_CHECK_MENU_ITEM(menus->menu_item_eol_mode_lf),
                TRUE);	
			break;
		case EOL_MODE_CRLF:
			gtk_check_menu_item_set_active  (
				GTK_CHECK_MENU_ITEM(menus->menu_item_eol_mode_crlf),
                TRUE);	
			break;
		default:
			break;
	}
}

// dynamic widgets
CssedDynamicWidgets* 
cssed_window_dynamic_widgets_new()
{
	return g_malloc(sizeof(CssedDynamicWidgets));	
}

void 
cssed_window_dynamic_widgets_set(	CssedWindow* window,
									GtkWidget* save_button,			
									GtkWidget* save_main_menu,	
									GtkWidget* undo_button,
									GtkWidget* undo_main_menu,
									GtkWidget* undo_pop_menu,
									GtkWidget* redo_button,
									GtkWidget* redo_main_menu,
									GtkWidget* redo_pop_menu )
{
	CSSED_DYNWIDGETS(window)->save_button = save_button;
	CSSED_DYNWIDGETS(window)->save_main_menu =  save_main_menu;	
	CSSED_DYNWIDGETS(window)->undo_button = undo_button;
	CSSED_DYNWIDGETS(window)->undo_main_menu = undo_main_menu;
	CSSED_DYNWIDGETS(window)->undo_pop_menu = undo_pop_menu;
	CSSED_DYNWIDGETS(window)->redo_button = redo_button;
	CSSED_DYNWIDGETS(window)->redo_main_menu = redo_main_menu;
	CSSED_DYNWIDGETS(window)->redo_pop_menu = redo_pop_menu;	
}

void 
cssed_window_enable_save ( CssedWindow* window )
{
	gtk_widget_set_sensitive(CSSED_DYNWIDGETS(window)->save_button, TRUE);
	gtk_widget_set_sensitive(CSSED_DYNWIDGETS(window)->save_main_menu, TRUE);	
}

void 
cssed_window_disable_save(CssedWindow* window )
{
	gtk_widget_set_sensitive(CSSED_DYNWIDGETS(window)->save_button, FALSE);
	gtk_widget_set_sensitive(CSSED_DYNWIDGETS(window)->save_main_menu, FALSE);		
}

void 
cssed_window_enable_undo (CssedWindow* window )
{
	gtk_widget_set_sensitive(CSSED_DYNWIDGETS(window)->undo_button, TRUE);
	gtk_widget_set_sensitive(CSSED_DYNWIDGETS(window)->undo_main_menu, TRUE);
	gtk_widget_set_sensitive(CSSED_DYNWIDGETS(window)->undo_pop_menu, TRUE);	
}

void 
cssed_window_disable_undo(CssedWindow* window )
{
	gtk_widget_set_sensitive(CSSED_DYNWIDGETS(window)->undo_button, FALSE);
	gtk_widget_set_sensitive(CSSED_DYNWIDGETS(window)->undo_main_menu, FALSE);
	gtk_widget_set_sensitive(CSSED_DYNWIDGETS(window)->undo_pop_menu, FALSE);		
}

void 
cssed_window_enable_redo (CssedWindow* window )
{
	gtk_widget_set_sensitive(CSSED_DYNWIDGETS(window)->redo_button, TRUE);
	gtk_widget_set_sensitive(CSSED_DYNWIDGETS(window)->redo_main_menu, TRUE);
	gtk_widget_set_sensitive(CSSED_DYNWIDGETS(window)->redo_pop_menu, TRUE);	
}

void 
cssed_window_disable_redo(CssedWindow* window )
{
	gtk_widget_set_sensitive(CSSED_DYNWIDGETS(window)->redo_button, FALSE);
	gtk_widget_set_sensitive(CSSED_DYNWIDGETS(window)->redo_main_menu, FALSE);
	gtk_widget_set_sensitive(CSSED_DYNWIDGETS(window)->redo_pop_menu, FALSE);	
}


// window panes
CssedWindowPanes* 
cssed_window_panes_new()
{
	return g_malloc0(sizeof(CssedWindowPanes));	
}

void 
cssed_window_panes_set(	CssedWindow* window, 
						GtkWidget* vpaned_main,
						GtkWidget* hpaned_up,
						GtkWidget* vpaned_sidebar )
{
	CSSED_PANES(window)->vpaned_main = vpaned_main;
	CSSED_PANES(window)->hpaned_up = hpaned_up;
	CSSED_PANES(window)->vpaned_sidebar = vpaned_sidebar;	
}
void
cssed_window_ensure_output_visible( CssedWindow* window )
{
	GtkWidget* notebook;
	GtkWidget* vpaned;
	gint pos;
	gint height;
	gint width;
	
	notebook = cssed_window_get_footer_notebook(window);
	vpaned = CSSED_PANES(window)->vpaned_main;
	
	gtk_window_get_size(GTK_WINDOW(window->window_widget), &width, &height);
	pos = gtk_paned_get_position(GTK_PANED(vpaned));	
	gtk_notebook_set_current_page   (GTK_NOTEBOOK(notebook), 0);
	
	if( pos > height-(height/3) )
		gtk_paned_set_position(GTK_PANED(vpaned) ,height-(height/3));
}

void
cssed_window_ensure_selector_scanner_visible( CssedWindow* window )
{
	GtkWidget* notebook;
	GtkWidget* hpaned;
	GtkWidget* vpaned;
	GtkWidget* vpaned_css;
	gint pos;
	gint height;
	gint width;	
	
	notebook = cssed_window_get_sidebar_notebook(window);
	vpaned = CSSED_PANES(window)->vpaned_main;
	vpaned_css = CSSED_PANES(window)->vpaned_sidebar;
	hpaned = CSSED_PANES(window)->hpaned_up;
	
	gtk_window_get_size(GTK_WINDOW(window->window_widget), &width, &height);
	pos = gtk_paned_get_position(GTK_PANED(hpaned));
	
	gtk_notebook_set_current_page   (GTK_NOTEBOOK(notebook), 0);
	if( pos < width/3 ){
		gtk_paned_set_position(GTK_PANED(hpaned), width/3);
	}	
	pos = 	gtk_paned_get_position(GTK_PANED(vpaned_css));
	
	if( pos < height/3 ){
		gtk_paned_set_position(GTK_PANED(vpaned_css), height/3);
	}	
}

// window functions
void
cssed_window_output_write( CssedWindow* window, gchar* format, ... )
{
	GtkTreeView* view;
	GtkListStore* store;
	GtkTreeIter iter;
	GtkTreePath* path;
	gchar* message;
 	va_list args;

    va_start(args, format);
    message = g_strdup_vprintf(format, args);
    va_end(args);
	
	view = GTK_TREE_VIEW(window->tree_view_output);
	store = GTK_LIST_STORE(gtk_tree_view_get_model(view));
	gtk_list_store_append (store, &iter);
	gtk_list_store_set (store, &iter, 0, message, -1);	
	g_free(message);

	path = gtk_tree_model_get_path (GTK_TREE_MODEL(store), &iter);
	gtk_tree_view_scroll_to_cell (GTK_TREE_VIEW(view), path, NULL, FALSE, 0, 0);
	cssed_window_ensure_output_visible (window);
	gtk_tree_path_free(path);
}

void
cssed_window_output_clear( CssedWindow* window )
{
	GtkTreeView* view;
	GtkListStore* store;
	
	view = GTK_TREE_VIEW(window->tree_view_output);
	store = GTK_LIST_STORE(gtk_tree_view_get_model(view));	
	gtk_list_store_clear(store);
}

// flagged to be skiped FIXME 
void 
cssed_window_digest_write( CssedWindow* window, gchar* first_col,  gchar* second_col )
{
	GtkTreeView* view;
	GtkListStore* store;
	GtkTreeIter iter;
	GtkTreePath* path;
	
	view = GTK_TREE_VIEW(window->tree_view_digest);
	store = GTK_LIST_STORE(gtk_tree_view_get_model(view));
	gtk_list_store_append (store, &iter);
	gtk_list_store_set (store, &iter, 0, first_col, 1, second_col, -1);	
	
	path = gtk_tree_model_get_path (GTK_TREE_MODEL(store),&iter);
	gtk_tree_view_scroll_to_cell (GTK_TREE_VIEW(view), path, NULL, FALSE, 0, 0);
	gtk_tree_path_free(path);
}

void 
cssed_window_digest_clear( CssedWindow* window )
{
	GtkTreeView* view;
	GtkListStore* store;
	
	view = GTK_TREE_VIEW(window->tree_view_digest);
	store = GTK_LIST_STORE(gtk_tree_view_get_model(view));	
	gtk_list_store_clear(store);	
}

// This may be moved to a future CSS support implementation
// FIXME
void 
cssed_window_selector_scanner_write(CssedWindow* window, gchar* first_col, 
									gchar* second_col)
{
	GtkTreeView* view;
	GtkListStore* store;
	GtkTreeIter iter;
	GtkTreePath* path;
	GdkPixbuf* arrow_pixbuf;	
	
	view = GTK_TREE_VIEW(window->tree_view_css_current);
	store = GTK_LIST_STORE(gtk_tree_view_get_model(view));
	arrow_pixbuf = create_pixbuf_from_data (arrow);	
	
	gtk_list_store_append (store, &iter);
	gtk_list_store_set (store, &iter, 0, first_col, 1, second_col, 2, arrow_pixbuf, -1);	
	g_object_unref(arrow_pixbuf);
	path = gtk_tree_model_get_path (GTK_TREE_MODEL(store),&iter);
	gtk_tree_view_scroll_to_cell (GTK_TREE_VIEW(view), path, NULL, FALSE, 0, 0);
	gtk_tree_path_free(path);	
}

// This may be moved to a future CSS support implementation
// FIXME
void 
cssed_window_selector_scanner_clear(CssedWindow* window ) 
{
	GtkTreeView* view;
	GtkListStore* store;
	
	view = GTK_TREE_VIEW(window->tree_view_css_current);
	store = GTK_LIST_STORE(gtk_tree_view_get_model(view));	
	gtk_list_store_clear(store);	
}

// This may be moved to a future CSS support implementation
// FIXME
void 
cssed_window_selector_scanner_edited (	CssedWindow* window,
										gchar *path_string,
										gchar *new_text 	)
{
	CssedDoc* doc;
	GtkTreeView* view;
	GtkTreePath *path;
	GtkTreeIter iter;
	GtkListStore *store;
	gint start_pos;
	gint end_pos;
	gboolean valid;
	gchar* property;
	gchar* value;
	gchar* selector_contents;
	gchar* declaration;	

	path = gtk_tree_path_new_from_string (path_string);

	view = GTK_TREE_VIEW(window->tree_view_css_current);
	store = GTK_LIST_STORE(gtk_tree_view_get_model(view));	
	doc = document_get_current(window);
	
	start_pos = document_get_selection_start(doc);
	end_pos = document_get_selection_end(doc);
	
	gtk_tree_model_get_iter (GTK_TREE_MODEL(store), &iter, path);
	gtk_tree_path_free (path);
	gtk_list_store_set (store, &iter, 1, new_text, -1);

	if( start_pos == end_pos ){ // selection out
		return;
	}
	selector_contents = g_strdup(""); // empty but not NULL string must be allocated in memory
	// as will be disposed in allocated_buffer_add_string()

	valid = gtk_tree_model_get_iter_first (GTK_TREE_MODEL(store), &iter);
	selector_contents = allocated_buffer_add_string(selector_contents, "\n\t");
	while( valid){
		gtk_tree_model_get (GTK_TREE_MODEL(store), &iter,
							0, &property, 
							1, &value, 	-1);
				
		if( (property != NULL) || (value != NULL)){ // avoid NULL values
			declaration = g_strdup_printf("%s: %s;",property,value);
			selector_contents = allocated_buffer_add_string(selector_contents, declaration);
			g_free(declaration);
			g_free(property);
			g_free(value);
		}
		valid = gtk_tree_model_iter_next (GTK_TREE_MODEL(store), &iter);	
		if( valid){
			selector_contents = allocated_buffer_add_string(selector_contents, "\n\t");
		}else{
			selector_contents = allocated_buffer_add_string(selector_contents, "\n");
		}			
	}	
	document_replace_sel(doc,selector_contents);
	g_free(selector_contents);
}

//
void 
cssed_window_show_preferences_dialog( CssedWindow* window)
{
	GtkWidget* dialog;
	gint response;
	GList* doclist;
	CssedConfig* tmpcfg;
	CssedConfig* cfg;
	CssedDoc* tmpdoc;
	CssedFileType* filetype;
	// here will be copied the old configuration
	// to be restored if the user click on cancel
	// as changes are made directly to the window->config structure
	tmpcfg = cssed_window_config_new ();
	cfg = cssed_window_get_config (window );
	
	dialog = create_preferences_dialog (window, tmpcfg );

	response = gtk_dialog_run(GTK_DIALOG(dialog));
	switch( response){
		case GTK_RESPONSE_OK:
			if( GTK_IS_WIDGET(dialog) )
				gtk_widget_destroy(dialog );
			// colourise all			
			doclist = g_list_first((GList*) window->document_list );	
			if( doclist != NULL){
				while( doclist != NULL){
					tmpdoc = (CssedDoc*) doclist->data;
					// the higlighting may have changed
					if( (filetype = document_get_filetype(tmpdoc)) != NULL ){
#ifdef WITH_SPLITTER_WINDOW
						if( document_splitted(tmpdoc) ){
							filetype->apply_style(tmpdoc, document_get_primary_scintilla_widget(tmpdoc));
							filetype->apply_style(tmpdoc, document_get_secondary_scintilla_widget(tmpdoc));
						}else{
							filetype->apply_style(tmpdoc, NULL);
						}
#else
						filetype->apply_style(tmpdoc, NULL);	
#endif
						document_colourise_all(tmpdoc);								
					}						
					document_set_font(tmpdoc);
					document_set_current_font_size(tmpdoc);
					doclist = g_list_next(doclist);
				}
			}
			break;
		default:
			// restore old configuration
			if( GTK_IS_WIDGET(dialog) )
				gtk_widget_destroy(dialog);
			cssed_config_copy_style_and_defaults (tmpcfg, cssed_window_get_config(window));
			break;	
	}
	g_free(tmpcfg);
}
// document list
void 
cssed_window_add_document_to_list(CssedWindow* window, CssedDoc* doc )
{
	window->document_list = g_list_append(window->document_list, doc);
}

void 
cssed_window_delete_document_from_list(CssedWindow* window, CssedDoc* doc )
{
	window->document_list = g_list_remove(window->document_list, doc);
}

gint 
cssed_window_get_document_list_length(CssedWindow* window )
{
	return g_list_length(window->document_list);
}

GList* 
cssed_window_get_document_list_first(CssedWindow* window )
{
	return g_list_first(window->document_list);
}

// returns -1 if not found
gint 
cssed_window_get_document_list_index(CssedWindow* window, CssedDoc* doc )
{
	return g_list_index(window->document_list, doc);
}

// TRUE if document is moved, false otherwise
gboolean 
cssed_window_set_document_list_index(CssedWindow* window, CssedDoc* doc, gint index )
{
	gint old_index, size, cur_page=0;
	GList* old_list, *new_list=NULL;
	CssedDoc* cur_doc;

	old_index = g_list_index(window->document_list, doc);

	if( old_index == -1 || index == -1 || old_index == index )
			return FALSE;

	size = gtk_notebook_get_n_pages(GTK_NOTEBOOK(window->notebook_docs));
	while(cur_page < size){
		cur_doc = document_get_from_notebook_page_num(window, cur_page);
		if(cur_doc != doc){
			new_list = g_list_append(new_list, (gpointer) cur_doc);
		}
		cur_page++;
	}
	new_list = g_list_insert(new_list, doc, index);

	gtk_notebook_reorder_child  (GTK_NOTEBOOK(window->notebook_docs),
										document_get_container_widget(doc),
										index);

	old_list = window->document_list;
	window->document_list = new_list;
	g_list_free(old_list);
	return TRUE;
}

// plugins list
void 
cssed_window_add_plugin_to_list( CssedWindow* window, gpointer plugin )
{
	window->plugins_list = g_list_append(window->plugins_list, plugin);	
}

void 
cssed_window_delete_plugin_from_list( CssedWindow* window, gpointer plugin )
{
	window->plugins_list = g_list_remove(window->plugins_list, plugin);
}

gint
cssed_window_get_plugin_list_length( CssedWindow* window )
{
	return g_list_length(window->plugins_list);
}

GList*
cssed_window_get_plugin_list_first( CssedWindow* window )
{
	return g_list_first(window->plugins_list);	
}

// keywords list FIXME: this should go to a DocumentTypeManager struct 
gchar* 
cssed_window_get_keyword_list( CssedWindow* window )
{
	g_return_val_if_fail(window->keywords_list != NULL, NULL);
	return window->keywords_list;
}
	
void 
cssed_window_set_keyword_list(CssedWindow* window, gchar* keywords )
{
	if( window->keywords_list != NULL )
		g_free(window->keywords_list);
	window->keywords_list = keywords;	
}

void
cssed_window_free_keyword_list( CssedWindow* window )
{
	if(window->keywords_list != NULL)
		g_free(window->keywords_list);
}
// css dialogs hash table
void 
cssed_window_css_dialog_insert(CssedWindow* window, gchar* key, gpointer klass )
{
	g_hash_table_insert(window->cssdialogs_hash, (gpointer) key, klass);
}
	

void 
cssed_window_css_dialog_remove_by_keyword( CssedWindow* window, gchar* key )
{
	g_hash_table_remove(window->cssdialogs_hash, (gpointer) key);
}

gpointer
cssed_window_css_dialog_lookup_by_keyword( CssedWindow* window, gchar* key )
{
	return g_hash_table_lookup (window->cssdialogs_hash, (gpointer) key);
}

gboolean
cssed_window_css_dialog_keyword_in_use( CssedWindow* window, gchar* key )
{
	gpointer klass;
	
	klass = g_hash_table_lookup (window->cssdialogs_hash, (gpointer) key);
	if(klass == NULL)
		return FALSE;
	else
		return TRUE;	
}

// check for an opened file and select it if exists (use only full path)
gboolean
cssed_window_is_file_opened( CssedWindow* window, gchar* file )
{
	GList* docs;
	CssedDoc* doc;
	gchar* filename;
	gint page = 0;
	
	g_return_val_if_fail(file != NULL, FALSE);
	
	docs = g_list_first(window->document_list);	
	while( docs != NULL ){
		doc = (CssedDoc*) docs->data;
		filename = document_get_filename(doc);
		
		if( filename != NULL ){
			if( strcmp(filename, file) == 0 ){
				gtk_notebook_set_current_page(
						GTK_NOTEBOOK(window->notebook_docs), page);
				return TRUE;
			}
			g_free(filename);
		}
		page++;
		docs = g_list_next(docs);
	}
	return FALSE;
}
// 
gint 
cssed_window_get_num_docs( CssedWindow* window )
{
	return g_list_length(window->document_list);	
}

void
cssed_window_set_ndoc_active( CssedWindow* window, gint doc_number )
{
	gtk_notebook_set_current_page(GTK_NOTEBOOK(window->notebook_docs), doc_number);
}


void
cssed_window_add_filetype (CssedWindow* window, CssedFileType* filetype, gboolean free_previous)
{
	GSList* list;

	g_return_if_fail( window != NULL );
	g_return_if_fail( filetype != NULL );
	g_return_if_fail( window->manager != NULL );

	if( free_previous && (CSSED_FILETYPE_MANAGER(window)->filetype[filetype->id] != NULL) ){
		cssed_file_type_free( CSSED_FILETYPE_MANAGER(window)->filetype[filetype->id] );
	}

	CSSED_FILETYPE_MANAGER(window)->filetype[filetype->id] = filetype;
	
	// add the patterns
	for ( list = g_slist_nth (filetype->patterns, 0);list != NULL; list = g_slist_next(list) ) {
		cssed_file_type_manager_add_pattern( window->manager, list->data, filetype->id );
	}

	if( filetype->menu_item != NULL ){
		gtk_widget_show (filetype->menu_item);
		gtk_container_add (GTK_CONTAINER (window->filetypes_menu), filetype->menu_item);
	}	
}

// read cssdwindow.h for an explanation about this function
void
cssed_window_apply_stored_size_and_position( CssedWindow* window )
{
	gtk_window_move(GTK_WINDOW(window->window_widget), window->config->window_x, window->config->window_y );
	gtk_window_resize (GTK_WINDOW(window->window_widget),window->config->window_width, window->config->window_height);
	if( window->config->initiall_state_maximized ) gtk_window_maximize (GTK_WINDOW(window->window_widget));// PANEDBUG
}

void 
cssed_window_apply_stored_paned_positions( CssedWindow* window )
{
	gtk_paned_set_position(GTK_PANED (window->panes->hpaned_up), window->config->sidebar_pos);
	gtk_paned_set_position(GTK_PANED (window->panes->vpaned_main), window->config->footer_pos);		
	gtk_paned_set_position(GTK_PANED (window->panes->vpaned_sidebar), window->config->css_treeview_pos);
}

// to show help in a system HTML browser
void
cssed_window_show_in_default_browser ( CssedWindow* window, gchar* uri )
{
	gchar *command;
	GError *error = NULL;
	gchar* browser_command;

	browser_command = window->config->browser_command;

	if( browser_command == NULL ){
		cssed_error_message( _("Browser command not set"), _("You must set the default browser command on the\n\"Other\" tab in the preferences dialog before to use this command."));
		return;
	}

	if( strstr(browser_command, "%u") == NULL ){
		command = g_strdup_printf("%s %s", browser_command, uri );
	}else{
		command = cssed_string_replace_all(browser_command, "%u", uri);
	}
	
	if( command == NULL ){
		cssed_error_message( _("Internall error while creating browser command"), _("It may indicate a programming error.\nPlease comunicate it in the cssed-devel list or send a mail to iagorubio@users.sourceforge.net."));
		return;
	}

	if( !g_spawn_command_line_async (command, &error) ){
		cssed_error_message( _("Unable to show the browser, command failed."), error->message);
	}
	g_free(command);
}


