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

#define PLAT_GTK 1

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <gtk/gtk.h>

#define WITH_PLUGINS

#ifdef WIN32
# include <io.h>
#endif



#include "cssedwindow.h"
#include "document.h"
#include "callbacks.h"
#include "selectorparser.h"
#include "floating-list.h"
#include "cssdialogs-interface.h"
#include "interface.h"
#include "selector-wizard.h"
#include "color-wizard.h"
#include "box-wizard.h"
#include "cssparser.h"
#include "doc-scanner.h"
#include "support.h"
#include "utils.h"
#include "debug.h"
#include "plugins-dialog.h"
#include "document-list.h"
#include "gotoline-dialog.h"

#include <Scintilla.h>
#include <SciLexer.h>
#include <ScintillaWidget.h>
#include <libcroco.h>

//#define WITH_SPLITTER_WINDOW
#define  WITH_RECENT_FILE_MANAGER

typedef struct _DocLineData {
	CssedDoc *doc;
	gint line;
} DocLineData;

#ifdef WITH_RECENT_FILE_MANAGER
void
on_recent_file_sub_menu_item_activated	(GtkRecentChooser *chooser, gpointer user_data)  
{
	CssedWindow *window;
	gchar *uri = NULL, *filename = NULL;
	
	window = CSSED_WINDOW (user_data);
	
    uri = gtk_recent_chooser_get_current_uri  (chooser);
    
    if( uri ){
		filename = g_filename_from_uri (uri, NULL, NULL);
		g_free (uri);
    }    
    
    if( filename ){
		document_open_with_error_check (window , filename);
		g_free (filename);
    }
}
#endif

#ifdef WITH_TERMINAL
void
on_vte_terminal_child_exited            (VteTerminal *vteterminal,
                                            gpointer user_data)
{
	gchar* shell;
	
	shell = getenv("SHELL");
	if( shell == NULL){
		vte_terminal_reset (vteterminal, TRUE, TRUE);
		vte_terminal_fork_command (vteterminal, "/bin/sh", NULL, NULL, "~/", 0, 0, 0);
	}else{
		if( g_file_test(shell, G_FILE_TEST_EXISTS) && g_file_test(shell,G_FILE_TEST_IS_EXECUTABLE) ){
			vte_terminal_reset (vteterminal, TRUE, TRUE);
			vte_terminal_fork_command (vteterminal, shell, NULL, NULL, "~/", 0, 0, 0);
		}else{
			vte_terminal_reset (vteterminal, TRUE, TRUE);
			vte_terminal_fork_command (vteterminal, "/bin/sh", NULL, NULL, "~/", 0, 0, 0);
		}
	}
}
#endif

void
on_notebook_docs_switch_page (	GtkNotebook *notebook,
								GtkNotebookPage *page,
								guint page_num, 
								gpointer user_data )
{
	CssedDoc *tmpdoc;
	CssedWindow *window;
	GtkWidget *window_widget;
	gchar* strout;
	gchar* filename;
	gboolean modified;
	
	window = CSSED_WINDOW (user_data);
	DBGMSG (__FILE__,__LINE__," on_notebook_docs_switch_page - Page switched to %d, notebook %p, page %p", page_num, notebook, page);

	tmpdoc = document_get_from_notebook_page_num (window, page_num);
	if (tmpdoc == NULL)
	{
		cssed_error_message
			(_("Programming error!!"),
			 _("An invalid document has been passed,\nthings are going to fail from this point.\nPlease tell the developers this has occurred."));
		return;
	}
	else
	{
		filename = document_get_filename( tmpdoc );
		modified = document_get_modified( tmpdoc );
#ifdef DEBUG_OUTPUT
		if ( filename != NULL )
			DBGMSG (__FILE__,__LINE__," on_notebook_docs_switch_page  - filename %s", filename);
		else
			DBGMSG (__FILE__,__LINE__," on_notebook_docs_switch_page() - filename NULL");
#endif
		window_widget = cssed_window_get_window_widget( window );
		strout = g_strdup_printf("cssed: %s",filename ==
				      NULL ? _("Untitled doc") : filename );
		gtk_window_set_title (GTK_WINDOW (window_widget),strout);

		if( document_get_modified( tmpdoc ) ){
			cssed_window_enable_save ( window );
		}else{
			cssed_window_disable_save( window );	
		}
		
		if( document_can_undo( tmpdoc ) ){
			cssed_window_enable_undo ( window );
		}else{
			cssed_window_disable_undo( window );
		}
		
		if( document_can_redo( tmpdoc ) ){
			cssed_window_enable_redo ( window );
		}else{
			cssed_window_disable_redo( window );
		}

		g_free(strout);
		g_free(filename);
	}	
}
void
on_menu_doc_info_activate (GtkMenuItem * menuitem, gpointer user_data)
{
	CssedWindow *window;
	CssedDoc *tmpdoc;
	gint len;
	gchar* filename;
	gboolean modified;
	CssedFileType* filetype;
	CssedCodePage codepage;

	window = CSSED_WINDOW (user_data);
	tmpdoc = document_get_current (window);
	filetype = document_get_filetype(tmpdoc);
	cssed_window_output_clear(window);

	filename = document_get_filename(tmpdoc);
	modified = document_get_modified(tmpdoc);
	
	if ( filename == NULL)
		cssed_window_output_write ( window, _("Untitled doc"));
	else
		cssed_window_output_write ( window, filename );

	if ( modified )
		cssed_window_output_write ( window, _("Document modified (not saved)"));
	else
		cssed_window_output_write ( window, _("Document not modified (saved)"));
	
	len = document_get_length(tmpdoc);
	cssed_window_output_write ( window, _("Document length: %d"), len );	
	
	if( document_get_autocompletion_enabled(tmpdoc) ){
		cssed_window_output_write ( window,_("Autocompletion enabled for this document"));
	}else{
		cssed_window_output_write ( window,  _("Autocompletion disabled for this document"));
	}	
	
	if( document_get_folding_margin_visible(tmpdoc) ){
		cssed_window_output_write ( window, _("Folding enabled for this document"));
	}else{
		cssed_window_output_write ( window, _("Folding disabled for this document"));
	}	
	if( filetype->label_language )
		cssed_window_output_write ( window, _("Document type: %s"), filetype->label_language);

	codepage = document_get_codepage(tmpdoc);
	switch( codepage ){
		case CSSED_CODEPAGE_DEFAULT:
			cssed_window_output_write ( window, _("Encoding: Default"));
			break;
		case CSSED_CODEPAGE_UTF8:
			cssed_window_output_write ( window, _("Encoding: UTF-8"));
			break;
		case CSSED_CODEPAGE_DBCS:
			cssed_window_output_write ( window, _("Encoding: DBCS"));
			break;
	}
	
	if( document_get_use_tabs(tmpdoc) )
		cssed_window_output_write ( window, _("Tabs: using tab character \\t instead of spaces"));
	else
		cssed_window_output_write ( window, _("Tabs: using spaces instead of tab character \\t"));

	cssed_window_output_write ( window, _("Spaces per tab: %i"), document_get_tabs_width(tmpdoc));
	
	if( filename ) g_free( filename );
}

/* validation and dump*/
void
on_menu_utils_validate_activate(GtkMenuItem * menuitem, gpointer user_data)
{

	CssedWindow *window;
	CssedDoc *tmpdoc;
	gint doclen;
	gchar* doc_buffer;
	gchar* endbuff;
	
	window = CSSED_WINDOW (user_data);
	tmpdoc = document_get_current (window);
	
	if( document_get_filetype_id (tmpdoc) == CSSED_FILETYPE_CSS ){	
		doclen = document_get_length (tmpdoc);
		doc_buffer = g_malloc0(++doclen);
		document_get_text (tmpdoc, doclen, doc_buffer);
		endbuff = g_strconcat(doc_buffer, "\n",NULL);	
		document_clear_validator_arrow(tmpdoc);
		cssed_cr_parser_parse_buffer (tmpdoc, endbuff,strlen(endbuff),TRUE);
		g_free( endbuff);
		g_free(doc_buffer);
	}else{
		cssed_error_message(_("Unsupported file type"), _("I can only validate CSS files!"));
	}
}
/* only validation */
void 
on_menu_utils_validate_only_activate	(GtkMenuItem 	  * menuitem,
								gpointer user_data)
{
	CssedWindow *window;
	CssedDoc *tmpdoc;
	gint doclen;
	gchar* doc_buffer;
	gchar* endbuff;
	
	window = CSSED_WINDOW (user_data);
	tmpdoc = document_get_current (window);
	
	if( document_get_filetype_id(tmpdoc) == CSSED_FILETYPE_CSS ){
		doclen = document_get_length (tmpdoc);
		doc_buffer = g_malloc (++doclen);
		document_get_text (tmpdoc, doclen, doc_buffer);
		endbuff = g_strconcat(doc_buffer, "\n",NULL);	
		document_clear_validator_arrow(tmpdoc);
		cssed_cr_parser_parse_buffer (tmpdoc, endbuff,strlen(endbuff),FALSE);
		g_free( endbuff);
		g_free(doc_buffer);	
	}else{
		cssed_error_message(_("Unsupported file type"), _("I can only validate CSS files!"));
	}
}


/* end validation */
#ifdef WITH_PLUGINS
void on_plugins_load_activate(GtkMenuItem * menuitem,
					      gpointer user_data)
{
	GtkWidget* dialog;
	dialog = create_plugins_dialog ( CSSED_WINDOW(user_data) );	
	gtk_widget_show( dialog );
}
#endif

void 
on_menu_utils_cleanoutput_activate	(GtkMenuItem* menuitem,  gpointer user_data)
{
	cssed_window_output_clear( CSSED_WINDOW(user_data) );	
}

void
on_menu_new_activate (GtkMenuItem * menuitem, gpointer user_data)
{
	GtkNotebook *notebook;
	CssedWindow *window;
	
	window = CSSED_WINDOW (user_data);
	notebook = GTK_NOTEBOOK ( cssed_window_get_document_notebook ( window ) );

	if (!GTK_IS_NOTEBOOK (notebook))
	{
		cssed_error_message
			(_("Programming error"),_("Please send this error to http://cssed.sf.net\n%s:%d - on_menu_new_activate()- returned pointer %p is not the expected GtkNotebook"), notebook);
		DBGMSG (__FILE__,__LINE__,"  on_menu_new_activate()- %p Is NOT a GtkNotebook", notebook);
		return;
	}
	create_and_attach_new_doc (window, _("Untitled"));
	
	gtk_notebook_set_current_page( notebook, -1 );
}
void
on_menu_open_activate (GtkMenuItem * menuitem, gpointer user_data)
{
	CssedWindow *window;
	GError *error = NULL;
	gchar* filename;

	window = CSSED_WINDOW (user_data);

	filename = cssed_prompt_for_file (window, _("Select file to open"), NULL, CSSED_FILESELECTOR_OPEN);

	if( filename == NULL ) return; // user abort
	document_open(window, filename, &error);

	if( error != NULL )
		cssed_show_message_from_error (error, _("Error opening file"), TRUE);

	g_free(filename);
}
#ifdef GTK_ATLEAST_2_4 // gtk file chooser don't let you paste a path
void
on_menu_open_path_activate (GtkMenuItem * menuitem, gpointer user_data)
{
	GtkWidget *dialog;
	GtkWidget *label;
	GtkWidget *entry;
	CssedWindow *window;
	GError *error = NULL;
	gchar* filename;
	gint response;

	window = CSSED_WINDOW (user_data);
	dialog = gtk_dialog_new_with_buttons    (_("Insert path to open"),
                                             GTK_WINDOW(cssed_window_get_window_widget(window)),
                                             GTK_DIALOG_MODAL  | GTK_DIALOG_DESTROY_WITH_PARENT,
                                             GTK_STOCK_OK, GTK_RESPONSE_OK,
											 GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
                                             NULL);	
	label = gtk_label_new(_("Path"));
	entry = gtk_entry_new();
	gtk_container_add (GTK_CONTAINER (GTK_DIALOG(dialog)->vbox), label);
	gtk_container_add (GTK_CONTAINER (GTK_DIALOG(dialog)->vbox), entry);
	gtk_widget_show_all(dialog);
	response = gtk_dialog_run(GTK_DIALOG(dialog));
	if( response == GTK_RESPONSE_OK ){
		filename = gtk_editable_get_chars(GTK_EDITABLE(entry), 0, -1);
		gtk_widget_destroy(dialog);
		if( filename == NULL ) return;
		document_open(window, filename, &error);
		if( error != NULL )
			cssed_show_message_from_error (error, _("Error opening file"), TRUE);	

		g_free(filename);
	}else{ gtk_widget_destroy(dialog); }
}
#endif
void 
on_menu_close_activate (GtkMenuItem * menuitem, gpointer user_data)
{
	// call the toolbar close button callback
	on_button_toolbar_close_clicked (NULL, user_data);
}

void 
on_menu_save_all_activate (GtkMenuItem * menuitem, gpointer user_data)
{
	CssedWindow *window;
	CssedDoc* doc;
	GtkWidget* confirm;
	gint response;
	GList* doc_list;
	gchar* doc_name;
	
	window = CSSED_WINDOW (user_data);
	doc_list = 	cssed_window_get_document_list_first(window);
	
	while( doc_list ){
		doc = (CssedDoc*) doc_list->data;
		if( document_get_modified(doc) ){
			doc_name = document_get_filename( doc );
			if( doc_name == NULL ){
				document_set_current(doc);
				confirm = create_ok_cancel_dialog (_("This file have no name"), _("I cannot know the location where to store this file.\nDo you want to select the location where this\nfile will be saved?"));
				response = gtk_dialog_run(GTK_DIALOG(confirm));
				if( response == GTK_RESPONSE_OK )
					document_save(doc);					
				
				gtk_widget_destroy( confirm );
			}else{
				document_save(doc);
			}				
		}
		doc_list = g_list_next(doc_list);
	}
}

void  
on_menu_close_all_activate (GtkMenuItem * menuitem, gpointer user_data)
{
	CssedWindow *window;
	CssedDoc* doc;
	GtkWidget* confirm;
	gint response;
	GList* doc_list;
	gchar* doc_name;
	gchar* filename;
	
	window = CSSED_WINDOW (user_data);
	doc_list = 	cssed_window_get_document_list_first(window);
	
	while( doc_list ){
		doc = (CssedDoc*) doc_list->data;
		doc_list = g_list_next(doc_list);
		if( doc != NULL ){
			doc_name = document_get_filename( doc );
			if( document_get_modified(doc) ){
				if( doc_name == NULL ){
					document_set_current(doc);
					confirm = create_yes_no_save_dialog(
							_("File is not saved, do you want to close it?"),
							_("The file has changed and the changes will be lost if you close it. Click in the <b>\"Save\"</b> button to save the file before closing, click the  <b>\"No\"</b> button if you want to skip to close the file, click the <b>\"Yes\"</b> button if you want to close the file unsaved.") );
					response = gtk_dialog_run(GTK_DIALOG(confirm));
					gtk_widget_destroy( confirm );
					if( response == GTK_RESPONSE_OK ){
						// add the filename before save the file to avoid other prompts asking
						// for the file's name.
						filename = cssed_prompt_for_file(window, _("Select file to save"), NULL, CSSED_FILESELECTOR_SAVE);
						if( filename != NULL ){
							if( document_save_as (doc, filename) )
								document_dispose(doc);
							g_free(filename);
						}										
					}else if( response == GTK_RESPONSE_YES ){
						document_dispose( doc );
					}
				}else{
					document_set_current(doc);
					confirm = create_yes_no_save_dialog(
							_("File is not saved, do you want to close it?"),
							_("The file has changed and the changes will be lost if you close it. Click in the <b>\"Save\"</b> button to save the file before closing, click the  <b>\"No\"</b> button if you want to skip to close the file, click the <b>\"Yes\"</b> button if you want to close the file unsaved.\n\nFile name:<b>%s</b>\n"),
							doc_name );
					response = gtk_dialog_run(GTK_DIALOG(confirm));
					gtk_widget_destroy(GTK_WIDGET(confirm));
					switch(response)
					{
						case GTK_RESPONSE_YES:
							document_dispose (doc);
						break;
						case GTK_RESPONSE_OK:
							if( document_save(doc) )
								document_dispose(doc);
						break;
					}
				}				
			}else{
				document_dispose(doc);
			}
		}else{
			DBGMSG (__FILE__,__LINE__,"on_menu_close_all_activate: NULL document on document list");
		}		
	}
}

void // FIXME: avoid to close and open the document, just pick the text and change it.
on_menu_revert_to_saved_activate	 (GtkMenuItem * menuitem, gpointer user_data)
{
	CssedWindow *window;
	CssedDoc *tmpdoc;
	GError *error = NULL;
	GtkWidget* dialog;
	gint response;
	gchar* filename;

	window = CSSED_WINDOW (user_data);
	tmpdoc = document_get_current (window);

	dialog = create_ok_cancel_dialog (_("Please confirm this action"), _("This file will be closed, all changes will be lost,\nand it will be opened the previously saved version.\n\nDo you want to continue?\n"));
	response = gtk_dialog_run(GTK_DIALOG(dialog));
	gtk_widget_destroy(dialog);
	if( response == GTK_RESPONSE_OK ){
		filename = document_get_filename(tmpdoc);
		if( filename == NULL ){
			cssed_error_message (_("File not saved"), _("This file is not saved, unable to revert to previous file."));
			return;
		}else{
			if( g_file_test (filename, G_FILE_TEST_EXISTS | G_FILE_TEST_IS_REGULAR) ){
				document_set_selection_range (tmpdoc, 0, document_get_length(tmpdoc));
				document_clear_sel (tmpdoc);
				document_add_file (tmpdoc, filename, &error);
				document_set_save_point (tmpdoc);
				if( error != NULL )
					cssed_show_message_from_error (error, _("Cannot reopen the file"), TRUE);	
			}else{
				cssed_error_message ( _("File does not exist"), _("The file is not saved on disk\nand may have been deleted by another process.\n%s\n"), filename);
			}
			g_free( filename );
		}
	}
}

void
on_menu_save_activate (GtkMenuItem * menuitem, gpointer user_data)
{
	CssedWindow *window;
	CssedDoc *tmpdoc;

	window = CSSED_WINDOW (user_data);
	tmpdoc = document_get_current (window);
	document_save(tmpdoc);
}


void
on_menu_save_as_activate (GtkMenuItem * menuitem, gpointer user_data)
{
	gchar* filename;
	CssedWindow *window;	// main window object
	CssedDoc *doc;
	gchar *path = NULL;


	window = CSSED_WINDOW (user_data);
	doc = document_get_current(window);

	path = document_get_filename(doc);
	filename = cssed_prompt_for_file (window, _("Enter file name to save .."), path, CSSED_FILESELECTOR_SAVE);
	if( path ) g_free(path);
	if( !filename ) return;
	document_save_as(doc, filename);		
	g_free(filename);
}

// called on window close
void
on_menu_quit_activate (GtkMenuItem * menuitem, gpointer user_data)
{
	CssedWindow *window;	
	window = CSSED_WINDOW (user_data);
	cssed_window_quit( window );	
}

void
on_menu_undo_activate (GtkMenuItem *menuitem, gpointer user_data)
{
	on_button_undo_clicked( NULL, user_data);
}
	
void
on_menu_redo_activate (GtkMenuItem *menuitem, gpointer user_data)
{
	on_button_redo_clicked( NULL, user_data );
}
	
void
on_menu_search_activate (GtkMenuItem *menuitem, gpointer  user_data)
{
	on_button_toolbar_search_clicked( NULL, user_data );
}
	
void
on_menu_replace_activate (GtkMenuItem  *menuitem, gpointer user_data)
{
	on_button_toolbar_replace_clicked( NULL, user_data );
}

void
on_menu_cut_activate (GtkMenuItem * menuitem, gpointer user_data)
{
	on_button_toolbar_cut_clicked (NULL, user_data);
}
void
on_menu_copy_activate (GtkMenuItem * menuitem, gpointer user_data)
{
	on_button_toolbar_copy_clicked (NULL, user_data);
}

void // generic used from all widgets
on_copy_to_scratch_callback (CssedWindow *window)
{
	gint len, startpos, endpos;
	gchar *text;
	CssedDoc *doc;
		
	doc = document_get_current (window);
	
	startpos = document_get_selection_start (doc);
	endpos = document_get_selection_end (doc);
	len = endpos - startpos;
	
	if( len <= 0 ) return;
	
	text = g_malloc0(++len);
	document_get_selected_text (doc, text);
	
	cssed_window_scratch_buffer_write (window, text);
	cssed_window_scratch_buffer_write (window, "\n");
	
	g_free (text); 
}

void
on_menu_copy_to_scratch_activate (GtkMenuItem * menuitem, gpointer user_data)
{
	on_copy_to_scratch_callback  (CSSED_WINDOW(user_data));
}

void
on_menu_paste_activate (GtkMenuItem * menuitem, gpointer user_data)
{
	on_button_toolbar_paste_clicked (NULL, user_data);
}

void
on_paste_from_scratch_callback (CssedWindow *window)
{
	gchar *text;
	CssedDoc *doc;

	doc = document_get_current (window);
	text =  cssed_window_get_scratch_buffer (window);

	if( !text ) return;
	
	document_add_text (doc, text);
	g_free (text);
}

void
on_delete_scratch_callback (CssedWindow *window)
{
	cssed_window_clear_scratch_buffer (window);
}

void
on_menu_paste_from_scratch_activate (GtkMenuItem * menuitem, gpointer user_data)
{
	on_paste_from_scratch_callback (CSSED_WINDOW(user_data));
}

void
on_menu_delete_scratch_activate (GtkMenuItem * menuitem, gpointer user_data)
{
	on_delete_scratch_callback (CSSED_WINDOW(user_data));
}

void
on_menu_delete_activate (GtkMenuItem * menuitem, gpointer user_data)
{
	CssedWindow *window;
	CssedDoc *tmpdoc;

	window = CSSED_WINDOW (user_data);
	tmpdoc = document_get_current (window);
	
	document_clear_sel(tmpdoc);
}

void
on_menu_preferences_activate (GtkMenuItem *menuitem, gpointer user_data)
{
	on_button_preferences_clicked( NULL, user_data );
}

void
on_menu_view_toolbar_toggled(GtkCheckMenuItem      *checkmenuitem,   
	gpointer user_data)
{
	gboolean active;	
	active =  gtk_check_menu_item_get_active( checkmenuitem );
	if( active ){
		gtk_widget_show(GTK_WIDGET(user_data));
	}else{
		gtk_widget_hide(GTK_WIDGET(user_data));
	}
}

void
on_menu_about_activate (GtkMenuItem * menuitem, gpointer user_data)
{
	GtkWidget *about = create_about ();
	if (about != NULL)
		gtk_widget_show (about);
}

#ifdef WITH_HELP_MENUS
#ifdef DARWIN
void
on_menu_cssed_website_with_safari_activate (GtkMenuItem * menuitem, gpointer user_data)
{
	gchar* command;
	gchar* cssedwebsiteurl;
	
	cssedwebsiteurl = "http://cssed.sourceforge.net/";
	
	command = g_strdup_printf ( "open -a 'Safari' %s", cssedwebsiteurl);
	system(command);
	
	g_free( command );
}

void
on_menu_cssed_website_with_firefox_activate (GtkMenuItem * menuitem, gpointer user_data)
{
	gchar* command;
	gchar* cssedwebsiteurl;
	
	cssedwebsiteurl = "http://cssed.sourceforge.net/";
	
	command = g_strdup_printf ( "launch -a 'Firefox' %s", cssedwebsiteurl);
	system(command);
	
	g_free( command );
}
#endif

void
on_menu_cssed_website_with_mozilla_activate (GtkMenuItem * menuitem, gpointer user_data)
{
	gchar* command;
	gchar* cssedwebsiteurl;
	
	cssedwebsiteurl = "http://cssed.sourceforge.net/";
	
	command = g_strdup_printf ( "mozilla %s &", cssedwebsiteurl);
	system(command);
	
	g_free( command );
}

void
on_menu_cssed_website_with_galeon_activate (GtkMenuItem * menuitem, gpointer user_data)
{
	gchar* command;
	gchar* cssedwebsiteurl;
	
	cssedwebsiteurl = "http://cssed.sourceforge.net/";
	
	command = g_strdup_printf ( "galeon -x %s &", cssedwebsiteurl);
	system(command);
	
	g_free( command );
}

void
on_menu_cssed_website_with_dillo_activate (GtkMenuItem * menuitem, gpointer user_data)
{
	gchar* command;
	gchar* cssedwebsiteurl;
	
	cssedwebsiteurl = "http://cssed.sourceforge.net/";
	
	command = g_strdup_printf ( "dillo %s &", cssedwebsiteurl);
	system(command);
	
	g_free( command );
}

void
on_menu_cssed_website_with_gnome_activate (GtkMenuItem * menuitem, gpointer user_data)
{
	gchar* command;
	gchar* cssedwebsiteurl;
	
	cssedwebsiteurl = "http://cssed.sourceforge.net/";
	
	command = g_strdup_printf ( "gnome-moz-remote --newwin %s", cssedwebsiteurl);
	system(command);
	
	g_free( command );
}

#ifdef DARWIN
void
on_menu_cssed_doc_online_with_safari_activate (GtkMenuItem * menuitem, gpointer user_data)
{
	gchar* command;
	gchar* csseddoconlineurl;
	
	csseddoconlineurl = "http://cssed.sourceforge.net/manual/index.html";
	
	command = g_strdup_printf ( "open -a 'Safari' %s", csseddoconlineurl);
	system(command);
	
	g_free( command );
}

void
on_menu_cssed_doc_online_with_firefox_activate (GtkMenuItem * menuitem, gpointer user_data)
{
	gchar* command;
	gchar* csseddoconlineurl;
	
	csseddoconlineurl = "http://cssed.sourceforge.net/manual/index.html";
	
	command = g_strdup_printf ( "launch -a 'Firefox' %s", csseddoconlineurl);
	system(command);
	
	g_free( command );
}
#endif

void
on_menu_cssed_doc_online_with_mozilla_activate (GtkMenuItem * menuitem, gpointer user_data)
{
	gchar* command;
	gchar* csseddoconlineurl;
	
	csseddoconlineurl = "http://cssed.sourceforge.net/manual/index.html";
	
	command = g_strdup_printf ( "mozilla %s &", csseddoconlineurl);
	system(command);
	
	g_free( command );
}

void
on_menu_cssed_doc_online_with_galeon_activate (GtkMenuItem * menuitem, gpointer user_data)
{
	gchar* command;
	gchar* csseddoconlineurl;
	
	csseddoconlineurl = "http://cssed.sourceforge.net/manual/index.html";
	
	command = g_strdup_printf ( "galeon -x %s &", csseddoconlineurl);
	system(command);
	
	g_free( command );
}

void
on_menu_cssed_doc_online_with_dillo_activate (GtkMenuItem * menuitem, gpointer user_data)
{
	gchar* command;
	gchar* csseddoconlineurl;
	
	csseddoconlineurl = "http://cssed.sourceforge.net/manual/index.html";
	
	command = g_strdup_printf ( "dillo %s &", csseddoconlineurl);
	system(command);
	
	g_free( command );
}

void
on_menu_cssed_doc_online_with_gnome_activate (GtkMenuItem * menuitem, gpointer user_data)
{
	gchar* command;
	gchar* csseddoconlineurl;
	
	csseddoconlineurl = "http://cssed.sourceforge.net/manual/index.html";
	
	command = g_strdup_printf ( "gnome-moz-remote --newwin %s", csseddoconlineurl);
	system(command);
	
	g_free( command );
}

#endif /* #ifdef WITH_HELP_MENUS */

/*    For future use*/
void 
on_menu_cssed_help_activate  (GtkMenuItem  *menuitem, gpointer  user_data)
{   // THIS IS OLD TESTING STUFF DON'T USE IT
	CssedWindow *window;

	window = CSSED_WINDOW(user_data);
	cssed_window_show_in_default_browser ( window, "http://cssed.sourceforge.net/manual/" );
}


/* dynamic menu items */       
void
on_menu_line_numbers_toggled (GtkCheckMenuItem * checkmenuitem,
			      gpointer user_data)
{
	CssedWindow *window;
	CssedDoc *tmpdoc;
	gboolean visible;

	window = CSSED_WINDOW (user_data);
	tmpdoc = document_get_current (window);

	if (tmpdoc == NULL)
	{			// FIXME say something to user
		DBGMSG (__FILE__,__LINE__,"on_menu_line_numbers_toggled  - document NULL");
		return;
	}
	else
	{
		visible = gtk_check_menu_item_get_active (checkmenuitem);
		document_set_line_numbers_visible (tmpdoc, visible);
	}
}

void
on_menu_line_endings_toggled (GtkCheckMenuItem * checkmenuitem,
			      gpointer user_data)
{
	CssedWindow *window;
	CssedDoc *tmpdoc;
	gboolean visible;

	window = CSSED_WINDOW (user_data);
	tmpdoc = document_get_current (window);

	if (tmpdoc == NULL)
	{			// FIXME say something to user
		DBGMSG (__FILE__,__LINE__,"on_menu_line_endings_toggled - document NULL");
		return;
	}
	else
	{
		visible = gtk_check_menu_item_get_active (checkmenuitem);
		document_set_line_endings_visible (tmpdoc, visible);
	}
}

void
on_menu_line_wrapping_toggled (GtkCheckMenuItem * checkmenuitem,
			       gpointer user_data)
{
	CssedWindow *window;
	CssedDoc *tmpdoc;
	gboolean wrapped;

	window = CSSED_WINDOW (user_data);
	tmpdoc = document_get_current (window);

	if (tmpdoc == NULL)
	{			// FIXME say something to user
		DBGMSG (__FILE__,__LINE__,"on_menu_line_endings_toggled - document NULL");
		return;
	}
	else
	{
		wrapped = gtk_check_menu_item_get_active (checkmenuitem);
		document_set_lines_wrapped (tmpdoc, wrapped);
	}
}

void
on_menu_view_white_spaces_toggled (GtkCheckMenuItem * checkmenuitem,
				  gpointer user_data)
{
	CssedWindow *window;
	CssedDoc *tmpdoc;
	gboolean visible;

	window = CSSED_WINDOW (user_data);
	tmpdoc = document_get_current (window);

	if (tmpdoc == NULL)
	{	// FIXME say something to user
		DBGMSG (__FILE__,__LINE__,"on_menu_view_white_spaces_toggled - document NULL");
		return;
	}
	else
	{
		visible = gtk_check_menu_item_get_active (checkmenuitem);
		document_set_white_spaces_visible (tmpdoc, visible);
	}
}

void
on_menu_item_autocompletion_toggled	(GtkCheckMenuItem      *checkmenuitem,   
	gpointer user_data)
{
	CssedWindow *window;
	CssedDoc *tmpdoc;
	gboolean active;

	window = CSSED_WINDOW (user_data);
	tmpdoc = document_get_current (window);	
	active = gtk_check_menu_item_get_active (checkmenuitem);
	document_set_autocompletion_enabled( tmpdoc, active );
}

void
on_menu_item_folding_toggled	(GtkCheckMenuItem      *checkmenuitem,   
	gpointer user_data)
{
	CssedWindow *window;
	CssedDoc *tmpdoc;
	gboolean fold;

	window = CSSED_WINDOW (user_data);
	tmpdoc = document_get_current (window);

	if (tmpdoc == NULL)
	{			// FIXME say something to user
		DBGMSG (__FILE__,__LINE__,"on_menu_line_numbers_toggled  - document NULL");
		return;
	}
	else
	{
		fold = gtk_check_menu_item_get_active (checkmenuitem);
		if( !fold )
			document_unfold_all(tmpdoc);	
		document_set_folding_margin_visible (tmpdoc, fold);
	}
}

void
on_menu_item_font_activate( GtkMenuItem *menuitem, gpointer user_data)
{
	CssedWindow *window;
	CssedDoc *tmpdoc;
	CssedConfig* cfg;
	GtkWidget* font_selector;
	gint response;
	gchar* font;
	gchar* family_name;
	gint size;
	PangoFontDescription* pfd; 
	
	window = CSSED_WINDOW (user_data);
	tmpdoc = document_get_current (window);	
	cfg = cssed_window_get_config(window);
	
	font_selector = gtk_font_selection_dialog_new (_("Select font for current document") );
	
	if( cfg->font != NULL){
		gchar* cfont;
		gchar* font_str;
		cfont = g_strdup(cfg->font);
		g_strdelimit(cfont,"!",' ');
		font_str = g_strdup_printf("%s, %d",cfont,cfg->font_size);
		gtk_font_selection_dialog_set_font_name(
							GTK_FONT_SELECTION_DIALOG(font_selector),
							font_str );
		g_free(font_str);
		g_free(cfont);
	}
	
	response = gtk_dialog_run(GTK_DIALOG(font_selector));
	if( response == GTK_RESPONSE_OK ){
		font =  gtk_font_selection_dialog_get_font_name(
							GTK_FONT_SELECTION_DIALOG(font_selector));
		if( font != NULL){
			pfd = pango_font_description_from_string( font );
			family_name = (gchar*) pango_font_description_get_family ( pfd );
			size = pango_font_description_get_size ( pfd );
			font = g_strdup_printf("!%s",family_name);
			document_set_font_by_name(tmpdoc, font);
			document_set_font_size(tmpdoc, size/PANGO_SCALE);
			document_colourise_all(tmpdoc);	
			pango_font_description_free (pfd);
			g_free(font);
		}
	}
	gtk_widget_destroy(font_selector);
}
// ************************* FLAGS ***********************************
void 
on_menu_item_flag_clear_activate( GtkMenuItem *menuitem, gpointer user_data)
{
	CssedWindow *window;
	CssedDoc *tmpdoc;
	GtkWidget* notebook;
	GtkWidget* menu_label;
	GtkWidget* container;
	GList* child = NULL;
	G_CONST_RETURN gchar* label_text;

	window = CSSED_WINDOW (user_data);
	tmpdoc = document_get_current (window);
	notebook = cssed_window_get_document_notebook(window);
	container = document_get_container_widget(tmpdoc);

	if( container == NULL ){
		DBGMSG (__FILE__,__LINE__,"Flag clear: passed NULL container");
		return;
	}	

	menu_label = gtk_notebook_get_tab_label  (GTK_NOTEBOOK(notebook), container);
	if( menu_label == NULL ){
		DBGMSG (__FILE__,__LINE__,"Flag clear: got NULL label");
		return;
	}	

	child = gtk_container_get_children (GTK_CONTAINER(menu_label));
	child = g_list_first(child);
	if( GTK_IS_LABEL(child->data) ){
		label_text = gtk_label_get_text(GTK_LABEL(child->data));
		gtk_label_set_text(GTK_LABEL(child->data), label_text);		
		gtk_label_set_use_markup(GTK_LABEL(child->data), FALSE);	
	}
}

void
menu_item_flag( GtkMenuItem *menuitem, gpointer user_data, gchar* back_color, gchar* fore_color )
{
	CssedWindow *window;
	CssedDoc *tmpdoc;
	GtkWidget* notebook;
	GtkWidget* menu_label;
	GtkWidget* container;
	GList* child = NULL;
	G_CONST_RETURN gchar* label_text;

	window = CSSED_WINDOW (user_data);
	tmpdoc = document_get_current (window);
	notebook = cssed_window_get_document_notebook(window);
	container = document_get_container_widget(tmpdoc);

	if( container == NULL ){
		DBGMSG (__FILE__,__LINE__,"Flag red: passed NULL container");
		return;
	}	

	menu_label = gtk_notebook_get_tab_label  (GTK_NOTEBOOK(notebook), container);
	if( menu_label == NULL ){
		DBGMSG (__FILE__,__LINE__,"Flag red: got NULL label");
		return;
	}	

	child = gtk_container_get_children (GTK_CONTAINER(menu_label));
	child = g_list_first(child);
	if( GTK_IS_LABEL(child->data) ){
		gchar* markup;
		label_text = gtk_label_get_text(GTK_LABEL(child->data));
		markup = g_strdup_printf("<span foreground='%s' background='%s'>%s</span>", fore_color, back_color, label_text);
		gtk_label_set_text(GTK_LABEL(child->data), markup);
		gtk_label_set_use_markup(GTK_LABEL(child->data), TRUE);
		g_free(markup);		
	}
}

void
on_menu_item_flag_red_activate( GtkMenuItem *menuitem, gpointer user_data)
{
	menu_item_flag( menuitem, user_data, "red", "white" );
}

void 
on_menu_item_flag_white_activate ( GtkMenuItem *menuitem, gpointer user_data)
{
	menu_item_flag( menuitem, user_data, "white", "black" );
}

void 
on_menu_item_flag_blue_activate ( GtkMenuItem *menuitem, gpointer user_data)
{
	menu_item_flag( menuitem, user_data, "blue", "white" );
}

void 
on_menu_item_flag_yellow_activate ( GtkMenuItem *menuitem, gpointer user_data)
{
	menu_item_flag( menuitem, user_data, "yellow", "black" );
}

void 
on_menu_item_flag_green_activate ( GtkMenuItem *menuitem, gpointer user_data)
{
	menu_item_flag( menuitem, user_data, "green", "black" );
}
/* **************************** END FLAGS ************************* */

void 
on_menu_item_document_goto_start_activate(GtkMenuItem *menuitem, gpointer user_data)
{
	CssedWindow *window;
	window = CSSED_WINDOW(user_data);
	document_set_current_position(document_get_current(window),0);
}

void 
on_menu_item_document_goto_end_activate(GtkMenuItem *menuitem, gpointer user_data)
{
	CssedWindow *window;
	CssedDoc *doc;

	window = CSSED_WINDOW(user_data);
	doc = document_get_current(window);
	document_set_current_position(doc, document_get_length(doc));
}

void 
on_menu_item_document_goto_line_activate(GtkMenuItem *menuitem, gpointer user_data)
{
	CssedWindow *window;
	GtkWidget* dialog;

	window = CSSED_WINDOW(user_data);	
	dialog = create_goto_line_dialog (window);
	gtk_window_present (GTK_WINDOW(dialog));
}

/* ************************** END document GoTo ***************** */
void 
on_menu_item_document_list_current_activate ( GtkMenuItem *menuitem, gpointer user_data)
{
	GtkWidget* dlg;
	dlg = create_doclist_dialog (CSSED_WINDOW(user_data), CSSED_DOCLIST_CURRENT, NULL);
	gtk_widget_show( dlg );
}


void 
on_menu_item_document_list_open_activate ( GtkMenuItem *menuitem, gpointer user_data)
{
	CssedWindow *window;
	GtkWidget *dlg;
	GtkWidget *opensel;
#ifndef GTK_ATLEAST_2_4
	G_CONST_RETURN  gchar* filename;
#else
	gchar* filename;
	gchar* last_open_dir;
#endif

	window = CSSED_WINDOW(user_data);

#ifndef GTK_ATLEAST_2_4
	opensel = gtk_file_selection_new (_("Select file"));
#else 
	opensel = gtk_file_chooser_dialog_new(_("Select file"), NULL,
												GTK_FILE_CHOOSER_ACTION_OPEN,
												GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
												GTK_STOCK_OPEN, GTK_RESPONSE_OK, NULL);
	last_open_dir = cssed_window_get_last_open_dir( window );
	if( last_open_dir != NULL ){
		gtk_file_chooser_set_current_folder( GTK_FILE_CHOOSER (opensel), last_open_dir );
		g_free(last_open_dir);
	}
#endif

	if( gtk_dialog_run(GTK_DIALOG(opensel)) == GTK_RESPONSE_OK ){
#ifndef GTK_ATLEAST_2_4
		filename = gtk_file_selection_get_filename( GTK_FILE_SELECTION(opensel) );
#else 
		filename = gtk_file_chooser_get_filename( GTK_FILE_CHOOSER(opensel) );
#endif
		gtk_widget_destroy(opensel);
		dlg = create_doclist_dialog (CSSED_WINDOW(user_data), CSSED_DOCLIST_OPEN, (gchar*) filename);
		gtk_widget_show( dlg );
#ifdef GTK_ATLEAST_2_4
		g_free(filename);
#endif
	}else{
		gtk_widget_destroy(opensel);
	}
}


void 
on_menu_item_document_list_new_activate ( GtkMenuItem *menuitem, gpointer user_data)
{
	GtkWidget* dlg;
	dlg = create_doclist_dialog (CSSED_WINDOW(user_data), CSSED_DOCLIST_NEW, NULL);
	gtk_widget_show( dlg );
}

void 
on_menuitem_folding_unfoldall_activate(GtkMenuItem *menuitem, gpointer user_data)
{
	CssedWindow *window;
	CssedDoc *tmpdoc;

	window = CSSED_WINDOW (user_data);
	tmpdoc = document_get_current (window);	
	document_unfold_all(tmpdoc);
}

void
on_menuitem_fonding_foldall_activate(GtkMenuItem  *menuitem, gpointer user_data)
{
	CssedWindow *window;
	CssedDoc *tmpdoc;

	window = CSSED_WINDOW (user_data);
	tmpdoc = document_get_current (window);
	document_fold_all(tmpdoc);	
}

/* ********************************************************* */
void 
on_menuitem_encoding_default_activate	(GtkMenuItem * menuitem,
					      gpointer user_data)
{
	CssedWindow *window;
	CssedDoc *tmpdoc;

	window = CSSED_WINDOW (user_data);
	tmpdoc = document_get_current (window);	
	
	document_set_codepage( tmpdoc, CSSED_CODEPAGE_DEFAULT );
}
void 
on_menuitem_encoding_utf8_activate(GtkMenuItem * menuitem,
					      gpointer user_data)
{
	CssedWindow *window;
	CssedDoc *tmpdoc;

	window = CSSED_WINDOW (user_data);
	tmpdoc = document_get_current (window);	
	
	document_set_codepage( tmpdoc, CSSED_CODEPAGE_UTF8 );
}
void 
on_menuitem_encoding_dbcs_activate	(GtkMenuItem * menuitem,
					      gpointer user_data)
{
	CssedWindow *window;
	CssedDoc *tmpdoc;

	window = CSSED_WINDOW (user_data);
	tmpdoc = document_get_current (window);	
	
	document_set_codepage( tmpdoc, CSSED_CODEPAGE_DBCS );
}
/* ********************************************************* */
void 
on_menuitem_charset_ansi_activate(GtkMenuItem * menuitem, gpointer user_data)
{
	CssedWindow *window;
	CssedDoc *tmpdoc;

	window = CSSED_WINDOW (user_data);
	tmpdoc = document_get_current (window);
	document_set_character_set(tmpdoc, SC_CHARSET_ANSI);
}
void 
on_menuitem_charset_easteurope_activate(GtkMenuItem * menuitem, gpointer user_data)
{
	CssedWindow *window;
	CssedDoc *tmpdoc;

	window = CSSED_WINDOW (user_data);
	tmpdoc = document_get_current (window);
	document_set_character_set(tmpdoc, SC_CHARSET_EASTEUROPE);
}
void 
on_menuitem_charset_gb2312_activate(GtkMenuItem * menuitem, gpointer user_data)
{
	CssedWindow *window;
	CssedDoc *tmpdoc;

	window = CSSED_WINDOW (user_data);
	tmpdoc = document_get_current (window);
	document_set_character_set(tmpdoc, SC_CHARSET_GB2312 );
}
void 
on_menuitem_charset_hangul_activate(GtkMenuItem * menuitem, gpointer user_data)
{
	CssedWindow *window;
	CssedDoc *tmpdoc;

	window = CSSED_WINDOW (user_data);
	tmpdoc = document_get_current (window);
	document_set_character_set(tmpdoc, SC_CHARSET_HANGUL);
}
void 
on_menuitem_charset_shiftjis_activate(GtkMenuItem * menuitem, gpointer user_data)
{
	CssedWindow *window;
	CssedDoc *tmpdoc;

	window = CSSED_WINDOW (user_data);
	tmpdoc = document_get_current (window);
	document_set_character_set(tmpdoc, SC_CHARSET_SHIFTJIS);
}

#ifdef WITH_SPLITTER_WINDOW
void on_menuitem_split_horizontal_activate	(GtkMenuItem * menuitem, gpointer user_data)
{
	CssedWindow *window;
	CssedDoc *tmpdoc;
	
	window = CSSED_WINDOW (user_data);
	tmpdoc = document_get_current (window);
	document_split( tmpdoc, CSSED_DOC_SPLIT_HORIZONTAL );
}
void on_menuitem_split_vertical_activate (GtkMenuItem * menuitem, gpointer user_data)
{
	CssedWindow *window;
	CssedDoc *tmpdoc;
	
	window = CSSED_WINDOW (user_data);
	tmpdoc = document_get_current (window);
	document_split( tmpdoc, CSSED_DOC_SPLIT_VERTICAL );
}
void on_menuitem_split_none_activate (GtkMenuItem * menuitem, gpointer user_data)
{
	CssedWindow *window;
	CssedDoc *tmpdoc;
	
	window = CSSED_WINDOW (user_data);
	tmpdoc = document_get_current (window);
	document_split( tmpdoc, CSSED_DOC_SPLIT_NONE );
}
#endif 

/* ********************************************************* */
void
on_menuitem_selector_wizard_activate(GtkMenuItem *menuitem, gpointer user_data)
{
	CssedWindow *window;
	CssedDoc* doc;
	GtkWidget* selector_wizard;
	GtkWidget* treeview;
	GtkWidget* radio;
	GtkTreeModel* model;
	GtkTreeIter iter;
	gboolean one_declaration_for_each;
	gboolean valid;
	gchar *strtoadd;
	gchar *strout;	
	gint i = 0;
	const gchar* widgets[] = {
		"treeview_selector_selected",
		"treeview_child_selected",
		"treeview_class_selected",
		"treeview_sibling_selected",	
		"treeview_contextual_selected",		
		0	
	}; 

	window = CSSED_WINDOW (user_data);	
	selector_wizard = create_selector_wizard ();
	doc = document_get_current(window);
	
	if( gtk_dialog_run(GTK_DIALOG(selector_wizard)) ==  GTK_RESPONSE_OK)
	{
		radio = lookup_widget(selector_wizard, "radiobutton_for_each");
		one_declaration_for_each =
					gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(radio));
					
		while( widgets[i] ){		
			treeview = lookup_widget(selector_wizard, widgets[i++]);
			model = gtk_tree_view_get_model (GTK_TREE_VIEW(treeview));
			valid = gtk_tree_model_get_iter_first(model, &iter);
			if( one_declaration_for_each ){
				while(valid){
					gtk_tree_model_get (model, &iter, 0, &strtoadd, -1);
					strout = g_strdup_printf("%s {\n\n}\n\n",strtoadd);
					document_add_text(doc,strout);
					g_free(strout);
					valid = gtk_tree_model_iter_next(model,&iter);
				}				
			}else{
				while(valid){
					gtk_tree_model_get (model, &iter, 0, &strtoadd, -1);
					strout = g_strdup_printf("%s",strtoadd);
					document_add_text(doc,strout);
					g_free(strout);
					valid = gtk_tree_model_iter_next(model,&iter);
					if(valid) document_add_text(doc,",\n");					
				}	
				document_add_text(doc,"	{\n\n}\n\n");
			}
		}		
	}
	gtk_widget_destroy(selector_wizard);
}

void
on_menuitem_box_wizard_activate	(GtkMenuItem *menuitem, gpointer user_data)
{
	GtkWidget* dlg;
	dlg = create_box_wizard (CSSED_WINDOW(user_data));
	gtk_widget_show(dlg);
}

void
on_menuitem_color_wizard_activate(GtkMenuItem *menuitem, gpointer user_data)
{
	GtkWidget* color_wizard;
	CssedWindow *window;
	
	window = CSSED_WINDOW (user_data);
	color_wizard = create_color_wizard_dialog (window);	
	gtk_widget_show(color_wizard);
}

void
on_menu_eol_mode_activate(GtkMenuItem * checkmenuitem, gpointer user_data)
{
	CssedWindow *window;
	CssedDoc *tmpdoc;
	CssedEolMode mode;

	window = CSSED_WINDOW (user_data);
	tmpdoc = document_get_current (window);
	mode = document_get_eol_mode(tmpdoc);
	cssed_window_dynamic_menus_set_eols_state( window, mode ); 
}

void
on_menu_eol_mode_cr_activate (GtkMenuItem * checkmenuitem, gpointer user_data)
{
	CssedWindow *window;
	CssedDoc *tmpdoc;
	CssedEolMode eols;

	window = CSSED_WINDOW (user_data);
	tmpdoc = document_get_current (window);

	
	if (tmpdoc == NULL)
	{			// FIXME say something to user
		DBGMSG (__FILE__,__LINE__,"on_menu_eol_mode_cr_activate - document NULL");
		return;
	}
	else
	{
		eols = document_get_eol_mode(tmpdoc);
		if( eols != EOL_MODE_CR ){
			document_set_eol_mode ( tmpdoc, EOL_MODE_CR );
		}
	}
}
// needs fix
void
on_menu_eol_mode_lf_activate (GtkMenuItem * checkmenuitem, gpointer user_data)
{
	CssedWindow *window;
	CssedDoc *tmpdoc;
	CssedEolMode eols;

	window = CSSED_WINDOW (user_data);
	tmpdoc = document_get_current (window);

	if (tmpdoc == NULL)
	{			// FIXME say something to user
		DBGMSG (__FILE__,__LINE__,"on_menu_eol_mode_lf_activate - document NULL");
		return;
	}
	else
	{
		eols = document_get_eol_mode(tmpdoc);
		if( eols != EOL_MODE_LF ){
			document_set_eol_mode ( tmpdoc, EOL_MODE_LF );
		}
	}
}


// needs fix
void
on_menu_eol_mode_crlf_activate (GtkMenuItem * checkmenuitem,
				gpointer user_data)
{
	CssedWindow *window;
	CssedDoc *tmpdoc;
	CssedEolMode eols;

	window = CSSED_WINDOW (user_data);
	tmpdoc = document_get_current (window);

	if (tmpdoc == NULL)
	{			// FIXME say something to user
		DBGMSG (__FILE__,__LINE__,"on_menu_eol_mode_crlf_activate - document NULL");
		return;
	}
	else
	{
		eols = document_get_eol_mode(tmpdoc);
		if( eols != EOL_MODE_CRLF ){
			document_set_eol_mode ( tmpdoc, EOL_MODE_CRLF );
		}
	}
}
void
on_menu_eol_convert_cr_activate    (GtkMenuItem * checkmenuitem,
									gpointer user_data)
{
	CssedWindow *window;
	CssedDoc *tmpdoc;
	CssedEolMode eols;

	window = CSSED_WINDOW (user_data);
	tmpdoc = document_get_current (window);

	if (tmpdoc == NULL)
	{			// FIXME say something to user
		DBGMSG (__FILE__,__LINE__,"menuitem_eol_convert_cr_activate - document NULL");
		return;
	}
	else
	{
		eols = document_get_eol_mode(tmpdoc);
		if( eols != EOL_MODE_CR ){
			document_set_eol_mode ( tmpdoc, EOL_MODE_CR );
			document_convert_eols ( tmpdoc, EOL_MODE_CR );
		}
	}
}

void
on_menu_eol_convert_lf_activate(GtkMenuItem * checkmenuitem,
									gpointer user_data)
{
	CssedWindow *window;
	CssedDoc *tmpdoc;
	CssedEolMode eols;

	window = CSSED_WINDOW (user_data);
	tmpdoc = document_get_current (window);

	if (tmpdoc == NULL)
	{			// FIXME say something to user
		DBGMSG (__FILE__,__LINE__,"menuitem_eol_convert_lf_activate - document NULL");
		return;
	}
	else
	{
		eols = document_get_eol_mode(tmpdoc);
		if( eols != EOL_MODE_LF ){
			document_set_eol_mode ( tmpdoc, EOL_MODE_LF );
			document_convert_eols ( tmpdoc, EOL_MODE_LF );
		}
	}
}

void
on_menu_eol_convert_crlf_activate(GtkMenuItem * checkmenuitem,
									gpointer user_data)
{
	CssedWindow *window;
	CssedDoc *tmpdoc;
	CssedEolMode eols;

	window = CSSED_WINDOW (user_data);
	tmpdoc = document_get_current (window);

	if (tmpdoc == NULL)
	{			// FIXME say something to user
		DBGMSG (__FILE__,__LINE__,"menuitem_eol_convert_crlf_activate - document NULL");
		return;
	}
	else
	{
		eols = document_get_eol_mode(tmpdoc);
		if( eols != EOL_MODE_CRLF ){
			document_set_eol_mode ( tmpdoc, EOL_MODE_CRLF );
			document_convert_eols ( tmpdoc, EOL_MODE_CRLF );
		}
	}
}

/* set the child menus state, FIXME: change the old name to on_menu_document_*/
void
on_menu_document_activate (GtkMenuItem * checkmenuitem, gpointer user_data)
{
	CssedDoc *tmpdoc;
	CssedWindow *window;

	window = CSSED_WINDOW (user_data);
	tmpdoc = document_get_current (window);

	if (tmpdoc == NULL)
	{
		cssed_error_message
			(_("Menu error"),
			 _("Cannot get current document to change menus.\nMenus state will be out of sync with the document\n"));
		return;
	}	

	cssed_window_dynamic_menus_set_state ( 
							window,
							document_get_line_numbers_visible (tmpdoc),
							document_get_line_endings_visible (tmpdoc),
							document_get_white_spaces_visible (tmpdoc),
							document_get_lines_wrapped (tmpdoc),
							document_get_autocompletion_enabled (tmpdoc),
							document_get_folding_margin_visible (tmpdoc) 
							);	

}

/* 
	scan the current selector: extract the buffer and calls 
	parser_css_selector_buffer_to_list()
*/
void
on_menu_scan_selector_activate (GtkMenuItem * menuitem, gpointer user_data)
{
	CssedDoc *tmpdoc;
	CssedWindow *window;
	ScintillaObject *sci;
	gint position;
	gint open_brace;
	gint close_brace;
	gchar *buffer;

	window = CSSED_WINDOW (user_data);
	tmpdoc = document_get_current (window);

	// FIXME this will go to a document function
	sci = SCINTILLA( document_get_scintilla_widget(tmpdoc) );
	position = document_get_current_position (tmpdoc);

	DBGMSG (__FILE__,__LINE__,"on_menu_scan_selector_activate() Position at %d", position);

	// set the anchor
	SSM (sci, SCI_SEARCHANCHOR, 0, 0);
	open_brace = SSM (sci, SCI_SEARCHPREV, 0, (sptr_t) "{");

#ifdef DEBUG_OUTPUT
	if (open_brace == -1)
		DBGMSG (__FILE__,__LINE__,"on_menu_scan_selector_activate() No opening brace before");
	else
		DBGMSG (__FILE__,__LINE__,"on_menu_scan_selector_activate() Opening brace at %d", open_brace);
#endif

	SSM (sci, SCI_SEARCHANCHOR, 0, 0);
	close_brace = SSM (sci, SCI_SEARCHNEXT, 0, (sptr_t) "}");

#ifdef DEBUG_OUTPUT
	if (close_brace == -1)
		DBGMSG (__FILE__,__LINE__,"on_menu_scan_selector_activate() No closing brace after");
	else
		DBGMSG (__FILE__,__LINE__,"on_menu_scan_selector_activate() Closing brace at %d", close_brace);
#endif

	if (close_brace < position)
	{
		cssed_error_message
			(_("The cursor must be into a declaration."),
			_("You should place the cursor between the opening\nand closing braces of any declaration to be scanned\n"));
	}
	else
	{
		if ((open_brace != -1) && (close_brace != -1))
		{		// we're in a selector then
			SSM (sci, SCI_SETSEL, open_brace + 1, close_brace);
			buffer = g_malloc ((close_brace - open_brace) * sizeof (gchar));
			SSM (sci, SCI_GETSELTEXT, 0, (sptr_t) buffer);
			g_strstrip (buffer);
			parser_css_selector_buffer_to_list ( window, buffer );
		}
	}
}

void
on_show_footer_panel_activate(GtkMenuItem *menuitem, gpointer user_data)
{
	GtkWidget* win;
	gint width;
	gint height;

	win = lookup_widget(GTK_WIDGET(menuitem),"window");
 	gtk_window_get_size(GTK_WINDOW(win), &width, &height);
	gtk_paned_set_position(GTK_PANED(user_data) ,height-(height/3));
}

void
on_hide_footer_panel_activate(GtkMenuItem *menuitem, gpointer user_data)
{
	//gint pos;
	GtkWidget* win;
	gint width;
	gint height;

	win = lookup_widget(GTK_WIDGET(menuitem),"window");
 	gtk_window_get_size(GTK_WINDOW(win), &width, &height);
	//pos = gtk_paned_get_position(GTK_PANED(user_data));
	gtk_paned_set_position(GTK_PANED(user_data) ,height);
}

void
on_show_side_panel_activate (GtkMenuItem *menuitem, gpointer user_data)
{
	//gint pos;
	GtkWidget* win;
	gint width;
	gint height;

	win = lookup_widget(GTK_WIDGET(menuitem),"window");
 	gtk_window_get_size(GTK_WINDOW(win), &width, &height);
	gtk_paned_set_position(GTK_PANED(user_data) ,width-(width/3));
}

void
on_hide_side_panel_activate(GtkMenuItem *menuitem, gpointer user_data)
{
	//gint pos;
	GtkWidget* win;
	gint width;
	gint height;

	win = lookup_widget(GTK_WIDGET(menuitem),"window");
 	gtk_window_get_size(GTK_WINDOW(win), &width, &height);
	gtk_paned_set_position(GTK_PANED(user_data) ,width);
}

void
on_button_toolbar_open_clicked (GtkButton * button, gpointer user_data)
{
	on_menu_open_activate (NULL, user_data);
}

void
on_button_toolbar_new_clicked (GtkButton * button, gpointer user_data)
{
	on_menu_new_activate (NULL, user_data);
}

void
on_button_toolbar_close_clicked (GtkButton * button, gpointer user_data)
{
	CssedDoc *doc;
	CssedWindow *window;

	window = CSSED_WINDOW (user_data);
	doc = document_get_current(window);
	document_close(doc);
}

void
on_button_toolbar_search_clicked (GtkButton * button, gpointer user_data)
{
	GtkWidget *searchdlg;

	searchdlg = create_search_dialog (user_data);
	gtk_widget_show (searchdlg);
}
void
on_button_toolbar_replace_clicked (GtkButton * button, gpointer user_data)
{
	GtkWidget *replacedlg;

	replacedlg = create_replace_dialog (user_data);	
	gtk_widget_show (replacedlg);
}
void
on_button_toolbar_copy_clicked (GtkButton * button, gpointer user_data)
{
	CssedWindow *window;
	CssedDoc *tmpdoc;

	window = CSSED_WINDOW (user_data);
	tmpdoc = document_get_current (window);
	document_copy (tmpdoc);
}
void
on_button_toolbar_paste_clicked (GtkButton * button, gpointer user_data)
{
	CssedWindow *window;
	CssedDoc *tmpdoc;

	window = CSSED_WINDOW (user_data);
	tmpdoc = document_get_current (window);
	document_paste (tmpdoc);
}

void
on_button_toolbar_cut_clicked (GtkButton * button, gpointer user_data)
{
	CssedWindow *window;
	CssedDoc *tmpdoc;

	window = CSSED_WINDOW (user_data);
	tmpdoc = document_get_current (window);
	document_cut (tmpdoc);
}

void
on_button_color_clicked (GtkButton * button, gpointer user_data)
{
	GtkWidget *colorselection_dlg;
	gchar* str;
	CssedWindow *window;

	window = CSSED_WINDOW (user_data);

	str = g_strdup("Choose color"); // freed by callbacks
	colorselection_dlg = create_color_selector_dialog (window, str, FALSE, TRUE);
	gtk_widget_show (colorselection_dlg);
}


void
on_button_toolbar_save_clicked (GtkButton * button, gpointer user_data)
{
	on_menu_save_activate (NULL, user_data);
}


void
on_button_toolbar_save_as_clicked (GtkButton * button, gpointer user_data)
{
	on_menu_save_as_activate (NULL, user_data);
}

void
on_button_next_clicked (GtkButton * button, gpointer user_data)
{
	CssedWindow *window;
	CssedDoc *doc;
	gint line;
	gint position;


	window = CSSED_WINDOW (user_data);
	doc = document_get_current (window);
	position = document_get_current_position (doc);
	line = document_get_line_from_position (doc, position);

	if (!document_marker_next (doc, ++line, 1))
	{
		cssed_error_message (_("No next marker"),
							 _("There is no marker between the current\nposition and the end of the document.")	);
	}else{
		document_ensure_line_is_visible(doc,document_get_line_from_position(doc, document_get_current_position(doc)));
	}
}

void
on_button_prev_clicked (GtkButton * button, gpointer user_data)
{
	CssedWindow *window;
	CssedDoc *doc;
	gint line;
	gint position;

	window = CSSED_WINDOW (user_data);
	doc = document_get_current (window);
	position = document_get_current_position (doc);
	line = document_get_line_from_position (doc, position);

	if (!document_marker_prev (doc, --line, 1))

	{
		cssed_error_message (_("No previous marker"),
							 _("There is no marker between the current\nposition and the start of the document."));
	}{
		document_ensure_line_is_visible(doc,document_get_line_from_position(doc, document_get_current_position(doc)));
	}
}

void
on_button_undo_clicked (GtkButton * button, gpointer user_data)
{
	CssedDoc *doc;
	CssedWindow *window;

	window = CSSED_WINDOW (user_data);
	doc = document_get_current( window );
	document_undo (doc);
}

void
on_button_redo_clicked (GtkButton * button, gpointer user_data)
{
	CssedDoc *doc;
	CssedWindow *window;

	window = CSSED_WINDOW (user_data);
	doc = document_get_current( window );
	document_redo (doc);
}

void
on_button_zoom_in_clicked (GtkButton * button, gpointer user_data)
{
	CssedWindow *window;
	CssedDoc *doc;

	window = CSSED_WINDOW (user_data);
	doc = document_get_current (window);
	document_zoom_in (doc);
}
void 
on_button_hide_side_clicked (GtkButton * button, gpointer user_data)
{
	gint pos;
	GtkWidget* win;
	gint width;
	gint height;

	win = lookup_widget(GTK_WIDGET(button),"window");
 	gtk_window_get_size(GTK_WINDOW(win), &width, &height);
	pos = gtk_paned_get_position(GTK_PANED(user_data));
	gtk_paned_set_position(GTK_PANED(user_data) ,width);
}

void
on_button_show_side_clicked (GtkButton * button, gpointer user_data)
{
	gint pos;
	GtkWidget* win;
	gint width;
	gint height;

	win = lookup_widget(GTK_WIDGET(button),"window");
 	gtk_window_get_size(GTK_WINDOW(win), &width, &height);
	pos = gtk_paned_get_position(GTK_PANED(user_data));
	gtk_paned_set_position(GTK_PANED(user_data) ,width-(width/3));
}

void 
on_button_hide_footer_clicked 	(GtkButton * button, gpointer user_data)
{
	gint pos;
	GtkWidget* win;
	gint width;
	gint height;

	win = lookup_widget(GTK_WIDGET(button),"window");
 	gtk_window_get_size(GTK_WINDOW(win), &width, &height);
	pos = gtk_paned_get_position(GTK_PANED(user_data));
	gtk_paned_set_position(GTK_PANED(user_data) ,height);
}

void 
on_button_show_footer_clicked (GtkButton * button, gpointer user_data)
{
	gint pos;
	GtkWidget* win;
	gint width;
	gint height;

	win = lookup_widget(GTK_WIDGET(button),"window");
 	gtk_window_get_size(GTK_WINDOW(win), &width, &height);
	pos = gtk_paned_get_position(GTK_PANED(user_data));
	gtk_paned_set_position(GTK_PANED(user_data) ,height-(height/3));
}

void
on_button_zoom_out_clicked (GtkButton * button, gpointer user_data)
{
	CssedWindow *window;
	CssedDoc *doc;

	window = CSSED_WINDOW (user_data);
	doc = document_get_current (window);
	document_zoom_out (doc);
}

void
on_button_zoom_off_clicked (GtkButton * button, gpointer user_data)
{
	CssedWindow *window;
	CssedDoc *doc;

	window = CSSED_WINDOW (user_data);
	doc = document_get_current (window);
	document_zoom_off (doc);
}

void
on_button_preferences_clicked(GtkButton * button, gpointer user_data)
{
	CssedWindow* window;

	window = CSSED_WINDOW( user_data );
	cssed_window_show_preferences_dialog( window );
}

void
on_window_destroy (GtkObject * object, gpointer user_data)
{
	gtk_main_quit();
}

gboolean    on_window_delete_event      	(GtkWidget *widget,
                                            GdkEvent *event,
                                            gpointer user_data)
{
	CssedWindow *window;	
	window = CSSED_WINDOW (user_data);
	cssed_window_quit( window );
	return TRUE;
}
// secondary toolbar callbacks
void
on_button_border_all_clicked (GtkButton  *button, 	gpointer user_data)
{
	GtkWidget* dialog;
	dialog = create_border_properties_dialog (CSSED_WINDOW(user_data),
				 								g_strdup("border"),
												TRUE,
												TRUE);
	gtk_widget_show(dialog);	
}
void 
on_button_margin_all_clicked (GtkButton  *button, 	gpointer user_data)
{
	GtkWidget* dialog;
	dialog = create_margin_property_dialog (CSSED_WINDOW(user_data),
				 								g_strdup("margin"),
												TRUE,
												TRUE);
	gtk_widget_show(dialog);		
}
void 
on_button_padding_all_clicked (GtkButton  *button, 	gpointer user_data)
{
	GtkWidget* dialog;
	dialog = create_padding_property_dialog (CSSED_WINDOW(user_data),
				 								g_strdup("padding"),
												TRUE,
												TRUE);
	gtk_widget_show(dialog);		
}
void
on_button_box_properties_clicked(GtkButton *button,gpointer user_data)
{
	GtkWidget* dialog;
	dialog = create_box_wizard(CSSED_WINDOW(user_data));
	gtk_widget_show(dialog);
}

void 
on_button_sel_wizard_clicked (GtkButton  *button, 	gpointer user_data)
{
	on_menuitem_selector_wizard_activate(NULL, CSSED_WINDOW(user_data));	
}

void 
on_button_col_wizard_clicked (GtkButton  *button, 	gpointer user_data)
{
	GtkWidget* dialog;
	dialog = create_color_wizard_dialog(CSSED_WINDOW(user_data));
	gtk_widget_show(dialog);	
}

void 
on_button_font_clicked	 (GtkButton  *button, 	gpointer user_data)
{
	GtkWidget* dialog;
	dialog = create_font_property_dialog (CSSED_WINDOW(user_data),
				 								g_strdup("font"),
												TRUE,
												TRUE);
	gtk_widget_show(dialog);	
}

void 
on_button_scan_selector_clicked  (GtkButton  *button, 	gpointer user_data)
{
	on_menu_scan_selector_activate(NULL, user_data);
}

void 
on_button_doc_info_clicked (GtkButton  *button, 	gpointer user_data)
{
	on_menu_doc_info_activate(NULL, user_data);
}

void 
on_button_clean_output_clicked  (GtkButton  *button, 	gpointer user_data)
{
	on_menu_utils_cleanoutput_activate(NULL, user_data);
}

void 
on_button_validate_clicked  (GtkButton  *button, 	gpointer user_data)
{
	on_menu_utils_validate_only_activate(NULL, user_data);
}

void 
on_button_validate_dump_clicked  (GtkButton  *button, 	gpointer user_data)
{
	on_menu_utils_validate_activate(NULL, user_data);
}
// end secondary toolbar callbacks

void
on_search_dialog_find_button_clicked (GtkButton * button, gpointer user_data)
{
	GtkWidget *entry_text_to_find;
	GtkWidget *checkbutton_regex;
	GtkWidget *radiobutton_perl_regex;
	GtkWidget *checkbutton_match_case;
	GtkWidget *checkbutton_whole_word;
	GtkWidget *checkbutton_word_start;
	GtkWidget *search_backwards_checkbox;
	gboolean isregexp;
	gboolean is_perl_regexp;
	gboolean is_match_case;
	gboolean is_whole_word;
	gboolean is_word_start;
	gboolean is_backwards_search;
	gint search_flags = 0; // Keep compiler happy
	CssedWindow *window;
	CssedDoc *doc;

	window = CSSED_WINDOW (user_data);
	doc = document_get_current (window);

	entry_text_to_find =
		lookup_widget (GTK_WIDGET (button), "entry_text_to_find");
	checkbutton_regex =
		lookup_widget (GTK_WIDGET (button), "checkbutton_regex");
	radiobutton_perl_regex =
		lookup_widget (GTK_WIDGET (button), "radiobutton_perl_regex");
	checkbutton_match_case =
		lookup_widget (GTK_WIDGET (button), "checkbutton_match_case");
	checkbutton_whole_word =
		lookup_widget (GTK_WIDGET (button), "checkbutton_whole_word");
	checkbutton_word_start =
		lookup_widget (GTK_WIDGET (button), "checkbutton_word_start");
	search_backwards_checkbox =
		lookup_widget (GTK_WIDGET (button), "search_backwards_checkbox");

	isregexp =
		gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON
					      (checkbutton_regex));
	is_perl_regexp =
		gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON
					      (radiobutton_perl_regex));
	is_match_case =
		gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON
					      (checkbutton_match_case));
	is_whole_word =
		gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON
					      (checkbutton_whole_word));
	is_word_start =
		gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON
					      (checkbutton_word_start));
	is_backwards_search =
		gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON
					      (search_backwards_checkbox));

	if (isregexp)
	{
		if (is_perl_regexp)
		{
			search_flags |= SCFIND_REGEXP;
		}
		else
		{
			search_flags |= SCFIND_POSIX;
		}
	}

	if (is_whole_word)
		search_flags |= SCFIND_WHOLEWORD;
	if (is_match_case)
		search_flags |= SCFIND_MATCHCASE;
	if (is_word_start)
		search_flags |= SCFIND_WORDSTART;

	if ( !is_backwards_search ){
		document_search_next (doc,
					  (gchar*)
					  gtk_entry_get_text (GTK_ENTRY
							  (entry_text_to_find)),
					  search_flags);
	}else{
		document_search_prev (doc,
					  (gchar*)
					  gtk_entry_get_text (GTK_ENTRY
							  (entry_text_to_find)),
					  search_flags);
	}
}

void
on_dialog_replace_search_button_clicked (GtkButton * button,
					 gpointer user_data)
{
	GtkWidget *entry_text_to_find;
	GtkWidget *checkbutton_regex;
	GtkWidget *radiobutton_perl_regex;
	GtkWidget *checkbutton_match_case;
	GtkWidget *checkbutton_whole_word;
	GtkWidget *checkbutton_word_start;
	gboolean isregexp;
	gboolean is_perl_regexp;
	gboolean is_match_case;
	gboolean is_whole_word;
	gboolean is_word_start;
	GtkWidget *search_backwards_checkbox;
	gboolean is_backwards_search;

	gint search_flags = 0; // Keep compiler happy
	CssedWindow *window;
	CssedDoc *doc;

	window = CSSED_WINDOW (user_data);
	doc = document_get_current (window);

	entry_text_to_find =
		lookup_widget (GTK_WIDGET (button), "entry_find");
	checkbutton_regex =
		lookup_widget (GTK_WIDGET (button), "checkbutton_regex");
	radiobutton_perl_regex =
		lookup_widget (GTK_WIDGET (button), "radiobutton_perl_regex");
	checkbutton_match_case =
		lookup_widget (GTK_WIDGET (button), "checkbutton_match_case");
	checkbutton_whole_word =
		lookup_widget (GTK_WIDGET (button), "checkbutton_whole_word");
	checkbutton_word_start =
		lookup_widget (GTK_WIDGET (button), "checkbutton_word_start");
	search_backwards_checkbox =
		lookup_widget (GTK_WIDGET (button), "search_backwards_checkbox");

	isregexp =
		gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON
					      (checkbutton_regex));
	is_perl_regexp =
		gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON
					      (radiobutton_perl_regex));
	is_match_case =
		gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON
					      (checkbutton_match_case));
	is_whole_word =
		gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON
					      (checkbutton_whole_word));
	is_word_start =
		gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON
					      (checkbutton_word_start));
	is_backwards_search =
		gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON
					      (search_backwards_checkbox));

	if (isregexp)
	{
		if (is_perl_regexp)
		{
			search_flags |= SCFIND_REGEXP;
		}
		else
		{
			search_flags |= SCFIND_POSIX;
		}
	}

	if (is_whole_word)
		search_flags |= SCFIND_WHOLEWORD;
	if (is_match_case)
		search_flags |= SCFIND_MATCHCASE;
	if (is_word_start)
		search_flags |= SCFIND_WORDSTART;

	if ( !is_backwards_search ){
		document_search_next (doc,
					  (gchar*)
					  gtk_entry_get_text (GTK_ENTRY
							  (entry_text_to_find)),
					  search_flags);
	}else{
		document_search_prev (doc,
					  (gchar*)
					  gtk_entry_get_text (GTK_ENTRY
							  (entry_text_to_find)),
					  search_flags);
	}
}

void
on_dialog_replace_apply_button_clicked (GtkButton * button,
					gpointer user_data)
{
	GtkWidget *entry_text_to_find;
	GtkWidget *entry_text_to_replace;
	GtkWidget *checkbutton_regex;
	GtkWidget *radiobutton_perl_regex;
	GtkWidget *checkbutton_match_case;
	GtkWidget *checkbutton_whole_word;
	GtkWidget *checkbutton_word_start;
	gboolean isregexp;
	gboolean is_perl_regexp;
	gboolean is_match_case;
	gboolean is_whole_word;
	gboolean is_word_start;
	GtkWidget *search_backwards_checkbox;
	gboolean is_backwards_search;
	gint search_flags = 0; // Keep compiler happy
	CssedWindow *window;
	CssedDoc *doc;
	ScintillaObject *sci;

	gint selection_start;
	gint selection_end;

	window = CSSED_WINDOW (user_data);
	doc = document_get_current (window);
	// FIXME this will go to a document function
	sci = SCINTILLA( document_get_scintilla_widget(doc) );

	selection_start = document_get_selection_start (doc);
	selection_end = document_get_selection_end (doc);

	entry_text_to_find =
		lookup_widget (GTK_WIDGET (button), "entry_find");
	entry_text_to_replace =
		lookup_widget (GTK_WIDGET (button), "entry_replace");
	checkbutton_regex =
		lookup_widget (GTK_WIDGET (button), "checkbutton_regex");
	radiobutton_perl_regex =
		lookup_widget (GTK_WIDGET (button), "radiobutton_perl_regex");
	checkbutton_match_case =
		lookup_widget (GTK_WIDGET (button), "checkbutton_match_case");
	checkbutton_whole_word =
		lookup_widget (GTK_WIDGET (button), "checkbutton_whole_word");
	checkbutton_word_start =
		lookup_widget (GTK_WIDGET (button), "checkbutton_word_start");
	search_backwards_checkbox =
		lookup_widget (GTK_WIDGET (button), "search_backwards_checkbox");

	isregexp =
		gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON
					      (checkbutton_regex));
	is_perl_regexp =
		gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON
					      (radiobutton_perl_regex));
	is_match_case =
		gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON
					      (checkbutton_match_case));
	is_whole_word =
		gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON
					      (checkbutton_whole_word));
	is_word_start =
		gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON
					      (checkbutton_word_start));
	is_backwards_search =
		gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON
					      (search_backwards_checkbox));
	if (selection_start >= selection_end)
	{
		cssed_error_message	(_("Nothing to replace"),
							 _("There must be some text selected\nto be replaced.\nPlease select the text to replace or repeat the search"));
		return;
	}
	else
	{
		if( isregexp ){
			document_replace_sel_re(doc, (gchar*) gtk_entry_get_text(GTK_ENTRY(entry_text_to_replace)));
		}else{
			document_replace_sel (doc,  (gchar*) gtk_entry_get_text(GTK_ENTRY(entry_text_to_replace)));
		}
	}

	// search next
	if (isregexp)
	{
		if (is_perl_regexp)	search_flags |= SCFIND_REGEXP;
		else search_flags |= SCFIND_REGEXP | SCFIND_POSIX;
	}
	
	if (is_whole_word)
		search_flags |= SCFIND_WHOLEWORD;
	if (is_match_case)
		search_flags |= SCFIND_MATCHCASE;
	if (is_word_start)
		search_flags |= SCFIND_WORDSTART;

	if ( !is_backwards_search ){
		document_search_next (doc,
					  (gchar*)
					  gtk_entry_get_text (GTK_ENTRY
							  (entry_text_to_find)),
					  search_flags);
	}else{
		document_search_prev (doc,
					  (gchar*)
					  gtk_entry_get_text (GTK_ENTRY
							  (entry_text_to_find)),
					  search_flags);
	}
}

void// generic scanning function
on_scanner_button_clicked (CssedWindow *window, gboolean clean)
{
	CssedDoc *tmpdoc;
	GtkWidget *output;
	gint doclen;
	gchar* doc_buffer;
	gchar* endbuff;
	gchar* filename;
	
	tmpdoc = document_get_current (window);	
	output = cssed_window_get_digest_treeview (window);

	if( clean ) cssed_window_digest_clear(window);

	filename = document_get_filename(tmpdoc);

	if( filename == NULL ){
		cssed_error_message(_("This file is not saved"), _("The file is not saved, please save it and try again."));
		return;
	}

	if( document_get_filetype_id(tmpdoc) != CSSED_FILETYPE_CSS ){
		cssed_error_message(_("Unsupported file type"), _("I can only parse CSS files!"));
		return;
	}
	
	doclen = document_get_length(tmpdoc);	
	if( doclen == 0) return;
	
	doc_buffer = g_malloc0(++doclen);
	document_get_text (tmpdoc, doclen, doc_buffer);
	endbuff = g_strconcat(doc_buffer, "\n",NULL);
	g_free(doc_buffer);
	document_clear_validator_arrow(tmpdoc);
	cssed_cr_parser_scan_doc_buffer (tmpdoc, endbuff, strlen(endbuff));	
	//cssed_window_set_last_scan_file(window, filename);
	g_free(endbuff);
}

void
on_document_scanner_button_clicked(GtkButton * button,
									gpointer user_data)
{
	CssedWindow *window;	

	window = CSSED_WINDOW (user_data);
	on_scanner_button_clicked(window, TRUE);
}

void
on_document_scanner_button_append_clicked(GtkButton * button,
									gpointer user_data)
{
	CssedWindow *window;	

	window = CSSED_WINDOW (user_data);
	on_scanner_button_clicked(window, FALSE);
}

void
on_document_scanner_button_clean_clicked(GtkButton * button,
										gpointer user_data)
{
	cssed_window_digest_clear(CSSED_WINDOW(user_data));
}

void
on_output_list_row_activated( GtkTreeView * treeview,
							  GtkTreePath * path,
							  GtkTreeViewColumn * col,
							  gpointer user_data)
{
	GtkTreeModel *model;
	GError *error = NULL;
	GtkTreeIter iter;
	gchar *message;
	gchar **splitedlist;
	CssedDoc *tmpdoc;
	CssedWindow *window;	
	gchar *linetext;

	window = CSSED_WINDOW (user_data);
	
	model = gtk_tree_view_get_model (treeview);
	if (gtk_tree_model_get_iter (model, &iter, path))
		gtk_tree_model_get (model, &iter, 0, &message, -1);
	else 
		return;	
	// clean message
	if( pango_parse_markup (message, strlen(message), 0, NULL, &linetext, NULL, NULL) )
	{
		if( !linetext ) { g_free(message); return; }		
		splitedlist = g_strsplit ((const gchar*) linetext, ":", 3);
		if( splitedlist ){
			// check if "file.css:3: message" format is used
			if( g_file_test(splitedlist[0], G_FILE_TEST_IS_REGULAR)	){ 
				if( !cssed_window_is_file_opened(window, splitedlist[0]) ){
					tmpdoc = document_open(window, splitedlist[0], &error);
					if( error != NULL ){
						cssed_show_message_from_error (error, _("Cannot open file"), TRUE);
						g_strfreev(splitedlist);
						g_free (message);
						return;
					}	
				}else{
					tmpdoc = document_get_current(window);
				}
				if( splitedlist[1] )
					if( char_ptr_is_decimal_num(splitedlist[1]) )
						document_set_validator_arrow_at_line_on_idle (tmpdoc, atoi(splitedlist[1]));
			}else if(splitedlist[0]) { // check if "3: message" format is used 
				if( char_ptr_is_decimal_num(splitedlist[0]) ){
					document_set_validator_arrow_at_line_on_idle (document_get_current(window),
																  atoi(splitedlist[0]));
				}
			}
			g_strfreev(splitedlist);
		}
	}
	g_free (message);
}

void
on_doc_scanner_row_activated(GtkTreeView * treeview,  GtkTreePath * path,
							GtkTreeViewColumn * col, gpointer user_data)
{
	GtkTreeModel *model;
	GError *error;
	gchar* file;
	gchar* line;
	gchar *cleaned_line; // this can be markup
	GtkTreeIter iter;
	CssedDoc *doc;
	CssedWindow* window;

	window = CSSED_WINDOW (user_data);
	model = gtk_tree_view_get_model(treeview);
	//scanfile = cssed_window_get_last_scan_file(window);
	gtk_tree_model_get_iter (model, &iter, path);
	gtk_tree_model_get (model, &iter, 1, &line, 3, &file, -1);

	if( file != NULL ){
		if( cssed_window_is_file_opened(window, file) ){
			doc = document_get_current(window);
			if( line && pango_parse_markup (line, strlen(line), 0, NULL, &cleaned_line, NULL, NULL) ){				
				if( char_ptr_is_decimal_num(cleaned_line) )
					document_set_validator_arrow_at_line(doc, atoi(cleaned_line));
				g_free(line);
				g_free(cleaned_line);
			}			
		}else{
			doc = document_open(window, file, &error);
			if ( doc != NULL ){
				if( line && pango_parse_markup (line, strlen(line), 0, NULL, &cleaned_line, NULL, NULL) ){				
					if( char_ptr_is_decimal_num(cleaned_line) ){
						document_set_validator_arrow_at_line_on_idle (doc, atoi(cleaned_line));
						
					}
					g_free(line);
					g_free(cleaned_line);
				}	
			}else{
				cssed_show_message_from_error (error,_("Cannot open file"), TRUE);
			}
		}
		g_free(file);
	}
}

// document scanner column callbacks
void // generic sort function
doc_scanner_col_clicked      (GtkTreeViewColumn *treeviewcolumn, GtkTreeView *view, gint index)
{
	GtkTreeViewColumn *column;
	GtkTreeModel *model;
	GList *cols = NULL;
	GtkSortType sort;

	model = gtk_tree_view_get_model(view);
	cols = gtk_tree_view_get_columns(view);

	sort = gtk_tree_view_column_get_sort_order (treeviewcolumn);

	cols = g_list_first(cols);
	while(cols)
	{
		column = GTK_TREE_VIEW_COLUMN(cols->data);
		gtk_tree_view_column_set_sort_indicator (column, FALSE);
		cols = g_list_next(cols);
	}
	g_list_free(cols);

	gtk_tree_view_column_set_sort_indicator (treeviewcolumn, TRUE);
	gtk_tree_view_column_set_sort_order (treeviewcolumn, !sort);
	gtk_tree_sortable_set_sort_column_id(GTK_TREE_SORTABLE(model), index, !sort);
}

void
on_doc_scanner_col_styles_clicked      (GtkTreeViewColumn *treeviewcolumn,
                                            gpointer user_data)
{
	doc_scanner_col_clicked  (treeviewcolumn, GTK_TREE_VIEW(user_data), 0);
}

void
on_doc_scanner_col_numbers_clicked      (GtkTreeViewColumn *treeviewcolumn,
                                            gpointer user_data)
{
	doc_scanner_col_clicked  (treeviewcolumn, GTK_TREE_VIEW(user_data), 1);
}

void
on_doc_scanner_col_file_clicked      (GtkTreeViewColumn *treeviewcolumn,
                                            gpointer user_data)
{
	doc_scanner_col_clicked  (treeviewcolumn, GTK_TREE_VIEW(user_data), 2);
}

void
on_doc_scanner_col_path_clicked      (GtkTreeViewColumn *treeviewcolumn,
                                            gpointer user_data)
{
	doc_scanner_col_clicked  (treeviewcolumn, GTK_TREE_VIEW(user_data), 3);
}

/* css treview */
void 
on_treeview_css_properties_row_activated (GtkTreeView * treeview,
					  GtkTreePath * path,
					  GtkTreeViewColumn * col,
					  gpointer user_data)
{
	GtkTreeModel *model;
	GtkTreeIter iter;
	gchar *attribute;
	gchar *property;
	CssedDoc *tmpdoc;
	CssedWindow *window;
	gint depth;


	window = CSSED_WINDOW (user_data);
	depth = gtk_tree_path_get_depth (path);
	tmpdoc = document_get_current(window);
	
	if (depth != 3)// catch only the third col signals
	{			
		DBGMSG (__FILE__,__LINE__,"on_treeview_css_properties_row_activated, depth %d ignoring", depth);
		return;
	}
	
	model = gtk_tree_view_get_model (treeview);
	
	if (gtk_tree_model_get_iter (model, &iter, path))
	{
		gtk_tree_model_get (model, &iter, 2, &attribute, -1);	// columns 0,1,2
		DBGMSG (__FILE__,__LINE__,"on_treeview_css_properties_row_activated, attr -> %s", attribute);
	}else{
		return;
	}
	
	gtk_tree_path_up (path);
	
	if (gtk_tree_model_get_iter (model, &iter, path))
	{
		gtk_tree_model_get (model, &iter, 1, &property, -1);	// columns 0,1,2
		DBGMSG (__FILE__,__LINE__,"on_treeview_css_properties_row_activated, property -> %s", property);
	}
	else
	{
		g_free (attribute);
		return;
	}
	// call fire dialog on document.c
	fire_dialog_from_strings_or_write_output(window,tmpdoc,attribute,property, TRUE );
	g_free (attribute);
	g_free (property);
}

void 
on_treeview_css_current_row_activated (GtkTreeView *treeview,  GtkTreePath  *path,
                                         GtkTreeViewColumn *col, gpointer     user_data)
{
	GtkTreeModel *model;
	GtkTreeIter iter;
	gchar *attribute;
	gchar *property;
	CssedDoc *tmpdoc;
	CssedWindow *window;
	gint depth;
	GList* columns;
	GtkTreeViewColumn* first_col = NULL; // Keep compiler happy
	GtkTreeViewColumn* second_col = NULL;
	GtkTreeViewColumn* third_col = NULL;
	GdkRectangle rect;
	GtkWidget* window_widget;
	GtkWidget* list_widget;
	GdkWindow* gdw;
	gint root_x, x, new_x;
	gint root_y, y, new_y;
	
	window = CSSED_WINDOW (user_data);
	depth = gtk_tree_path_get_depth (path);
	tmpdoc = document_get_current(window);

	columns =  gtk_tree_view_get_columns(treeview);
	
	columns = g_list_first(columns);
	if( columns != NULL ){
				first_col = columns->data;
	}
	columns = g_list_next(columns);
	if( columns != NULL ){
				second_col = columns->data;
	}	
	columns = g_list_next(columns);
	if( columns != NULL ){
				third_col = columns->data;
	}
	
	if( first_col == NULL ||
		second_col == NULL ||
		third_col == NULL )
	{
			return;			
	}
	g_list_free(columns);

	DBGMSG (__FILE__,__LINE__," on_treeview_css_current_row_activated() Called from %p , Cols %p %p %p , Match third %s***",
									col, 
									first_col,
									second_col,
									third_col,
									col==third_col?"yes":"no");

	if (col != third_col)// only check third col signals
	{			
		DBGMSG (__FILE__,__LINE__," on_treeview_css_current_row_activated() tree Col %p activated, ignoring", col);
		return;
	}
	
	model = gtk_tree_view_get_model (treeview);
	if (gtk_tree_model_get_iter (model, &iter, path))
	{
		gtk_tree_model_get (model, &iter, 0, &property,1, &attribute, -1);	// columns 0,1,2
		gtk_tree_view_set_cursor (treeview,path,col,TRUE);
		
		// get parent col width and height to create a floating list
		gtk_tree_view_get_cell_area(treeview, path, GTK_TREE_VIEW_COLUMN(second_col), &rect);
		gdw = gtk_widget_get_parent_window(GTK_WIDGET(treeview));
		window_widget = cssed_window_get_window_widget( window );
		gtk_widget_translate_coordinates        (GTK_WIDGET(treeview),
												 window_widget,
												 rect.x,
												 rect.y + (rect.height * 3),
												 &new_x,
												 &new_y); 
		gdk_window_get_root_origin(gdw, &root_x, &root_y);	
		x = root_x + new_x + 10;
		y = root_y + new_y;	
		list_widget = create_floating_list_from_keyword (	
											tmpdoc, 
											property, 
											x,
											y,
											rect.width,
											100,
											model,
											gtk_tree_path_to_string(path)
										);
		// that's important, this window must get the focus, as it's destroyed on
		// a focus-out event.
		gtk_window_present(GTK_WINDOW(list_widget));
		g_free(property);
		g_free(attribute);
	}else{
		DBGMSG (__FILE__,__LINE__," on_treeview_css_current_row_activated() - Nothing in the list?");
		return;
	}	
}

void 
on_current_css_render_values_edited  (GtkCellRendererText *cell,gchar *path_string,
											gchar *new_text,gpointer user_data)
{
	CssedWindow* window;
	GtkWidget* view;
	
	window = CSSED_WINDOW( user_data );	
	view = cssed_window_get_selector_scanner_treeview( window );
	cssed_window_selector_scanner_edited ( 	window,
											path_string,
											new_text 	);
	on_menu_scan_selector_activate (NULL, window);
	gtk_tree_view_set_cursor_on_cell( GTK_TREE_VIEW(view), 
									  gtk_tree_path_new_from_string(path_string),
									  NULL, NULL, FALSE );
}
/* /////////////// EDITOR CALLBACKS TESTING //////////////////// */
/* This will fire some user list in the future, here for listening to the 
 function keys and others, not used yet */
gboolean
on_editor_key_press_event(GtkWidget *widget,    GdkEventKey *event,
                                            gpointer user_data)
{
	DBGMSG (__FILE__,__LINE__," on_editor_key_press_event() - editor key pressed");
	return FALSE;
}

/* this will erase all data in the current css list when the user clicks again 
	on the editor widget, or changes something on it. This way, we are sure that
	the current selector list is in sync with the selected text in the editor */
void
on_editor_grab_focus           (GtkWidget *widget,
                                gpointer user_data)
{
	CssedWindow* window;
	GtkWidget* treeview;
	GtkTreeModel* list_store;
	GtkTreeIter iter;
	gboolean valid;
#ifdef WITH_SPLITTER_WINDOW
	CssedDoc *doc;
#endif	
	window = CSSED_WINDOW(user_data);	
	treeview =	cssed_window_get_selector_scanner_treeview( window );
	
	DBGMSG (__FILE__,__LINE__," on_editor_grab_focus() : editor %p grab focus",widget);

	list_store = gtk_tree_view_get_model (GTK_TREE_VIEW (treeview));
	valid = gtk_tree_model_get_iter_first (list_store, &iter);
	
	if( valid ){ // clean all data	
		DBGMSG (__FILE__,__LINE__," on_editor_grab_focus() - There was data in the css current properties list, cleaning ...");
		gtk_list_store_clear(GTK_LIST_STORE(list_store));
	}
#ifdef WITH_SPLITTER_WINDOW
	doc = document_get_current(window);
	document_set_active_editor(doc, widget);
#endif
}
	
/* /////////////////// EDITOR CALLBACKS END ////////////////// */
void 
on_menu_autocompletion_activate	(GtkMenuItem *menuitem,gpointer user_data)
{
	CssedWindow *window;
	CssedDoc *doc;
	gchar* list;
	
	window = CSSED_WINDOW (user_data);
	doc = document_get_current (window);
	list = cssed_window_get_keyword_list( window );
	document_show_user_list (doc, 0, list);	
}

void
popmenu_autocomp_activate_cb (GtkMenuItem *menuitem, gpointer user_data)
{
	CssedPopmenuData* data;
	CssedWindow* window;
	CssedDoc* doc;
	gchar* attribute;
	gchar* property; // previous_string
	gboolean add_property_string;	
	
	data = (CssedPopmenuData*) user_data;	
	window = CSSED_WINDOW(data->main_window);
	doc = (CssedDoc*) data->document;

	DBGMSG (__FILE__,__LINE__," popmenu_autocomp_activate_cb - %s - %s",(gchar*) data->attribute,(gchar*) data->property);

	attribute = g_strdup( (gchar*) data->attribute);
	property = g_strdup( (gchar*) data->property); 
	add_property_string = data->add_property_string;
	// Memory is disposed in parent's "unmap" event ( see below ). This menu is only for one use.
	fire_dialog_from_strings_or_write_output(window,doc,attribute,property,add_property_string);
	g_free(attribute);
	g_free(property);
}

/* When the pop menu is created some memory is dynamically allocated and must
   be disposed. This callback will track the popmenu unmap event and clear all
   memory allocated. The pointers to the data allocated are in a doubly
   linked list passed as user_data  */
void
popmenu_autocomp_event_after_cb(GtkWidget *widget,
                                GdkEvent *event,
                                gpointer user_data)
{
	GList* list;
	CssedPopmenuData* data;
	
	if ( event->type == GDK_UNMAP) {
		list = g_list_first((GList*) user_data);

		DBGMSG (__FILE__,__LINE__," popmenu_autocomp_event_after_cb() - After menu unmap event let's dispose allocated memory");
	
		if( list != NULL ){
			while( list != NULL ){
				data = (CssedPopmenuData*) list->data;
				DBGMSG (__FILE__,__LINE__," popmenu_autocomp_event_after_cb() - Cleaning element %p - containing %s %s",
							data, data->attribute,data->property);
				g_free(data->attribute);
				g_free(data->property);
				g_free(data);
				list = g_list_next(list);
			}
			g_list_free (list);
		}
#ifdef DEBUG_OUTPUT	
		else
		{
			DBGMSG (__FILE__,__LINE__," popmenu_autocomp_event_after_cb() - List is empty or NULL");		
		}
#endif	
		g_list_free(user_data);	
		
		if( GTK_IS_WIDGET(widget) ){// ensure the menu is destroyed
			gtk_widget_destroy(widget);
			DBGMSG (__FILE__,__LINE__," popmenu_autocomp_event_after_cb() - Menu destroyed");
		}
	}
}
/* common to all file selector dialogs */
/*
void file_selector_close (GtkDialog       *dialog,
                          gpointer         user_data)
{
	g_free(user_data);
}
									
void
file_selector_cancel_button_clicked (GtkButton       *button,  
									gpointer         user_data)
{
	GtkWidget* dialog;
	
	dialog = gtk_widget_get_toplevel(GTK_WIDGET(button));
	if(GTK_WIDGET_TOPLEVEL (dialog)){ //ensure it's the toplevel
		gtk_widget_destroy(dialog);
	}
	g_free(user_data);
}
*/

gboolean
on_editor_button_press_event(GtkWidget *widget, GdkEventButton *event,
                                                gpointer user_data)
{
	GtkMenu *menu;
	CssedWindow* window;
	
	window = CSSED_WINDOW (user_data);
	menu = GTK_MENU (cssed_window_get_pop_menu( window ));
	
	g_return_val_if_fail (widget != NULL, FALSE);
	g_return_val_if_fail (event != NULL, FALSE);
	if (event->type == GDK_BUTTON_PRESS)
	{
		if (event->button == 3)
		{
			gtk_menu_popup ( menu, NULL, NULL, NULL, NULL, 
		  					 event->button, event->time );
		 	return TRUE;
		}
	}
 	return FALSE;
}
// editor pop menu callbacks
void 
on_pop_menu_undo_activate(GtkMenuItem*menuitem, gpointer user_data)
{
	CssedDoc* doc;
	
	doc = document_get_current (CSSED_WINDOW(user_data));
	document_undo (doc);
}
	
void 
on_pop_menu_redo_activate (GtkMenuItem	  *menuitem,   		gpointer user_data)
{
	CssedDoc* doc;
	
	doc = document_get_current (CSSED_WINDOW(user_data));
	document_redo (doc);
}
	
void 
on_pop_menu_cut_activate (GtkMenuItem *menuitem, gpointer user_data)
{
	CssedDoc* doc;
	
	doc = document_get_current (CSSED_WINDOW(user_data));
	document_cut (doc);
}
	
void 
on_pop_menu_copy_activate (GtkMenuItem *menuitem, gpointer user_data)
{
	CssedDoc* doc;
	
	doc = document_get_current (CSSED_WINDOW(user_data));
	document_copy (doc);
}

void
on_pop_menu_copy_to_scratch_activate (GtkMenuItem *menuitem, gpointer user_data)
{
     on_copy_to_scratch_callback (CSSED_WINDOW(user_data));                
}   
                                         
void
on_pop_menu_paste_activate (GtkMenuItem *menuitem, gpointer user_data)
{
	CssedDoc* doc;
	
	doc = document_get_current (CSSED_WINDOW(user_data));
	document_paste (doc);
}

void
on_pop_menu_paste_from_scratch_activate (GtkMenuItem *menuitem, gpointer user_data)
{
	on_paste_from_scratch_callback (CSSED_WINDOW(user_data));
}

void
on_pop_menu_delete_scratch_activate (GtkMenuItem *menuitem, gpointer user_data)
{
	on_delete_scratch_callback (CSSED_WINDOW(user_data));
}	

void 
on_pop_menu_delete_activate (GtkMenuItem	  *menuitem,   		gpointer user_data)
{
	CssedDoc* doc;
	
	doc = document_get_current (CSSED_WINDOW(user_data));
	document_clear_sel (doc);
}
	
void 
on_pop_menu_search_activate(GtkMenuItem *menuitem, gpointer user_data)
{
	on_button_toolbar_search_clicked (NULL, user_data);
}

void 
on_pop_menu_replace_activate(GtkMenuItem *menuitem, gpointer user_data)
{
	on_button_toolbar_replace_clicked (NULL, user_data);
}

void
on_pop_menu_selectall_activate(GtkMenuItem	  *menuitem,   		gpointer user_data)
{
	CssedDoc* doc;
	
	doc = document_get_current (CSSED_WINDOW(user_data));
	document_set_selection_range (doc, 0, document_get_length(doc));
}

void
on_pop_menu_editcolor_activate(GtkMenuItem *menuitem, gpointer user_data)
{
	CssedDoc* doc;
	gint start, end, len, response, i;
	gchar *color, *oldcolor;
	GtkWidget *dialog;
	GtkColorSelection *colorsel;
	GdkColor gdkcolor;
	gchar *colorstr;

	doc = document_get_current(CSSED_WINDOW(user_data));
	end = document_get_selection_end(doc);
	start = document_get_selection_start(doc);

	len = end - start;
	if( len <= 0 || len >= 10 ) return;
	
	color = g_malloc0(sizeof(len)+1);
	document_get_selected_text(doc, color);

	// if have spaces bail out
	for(i=0;i<len;i++){
		if( isspace(color[i]) ){
			g_free(color);
			return;
		}
		// also convert to lower case
		if( g_ascii_isupper(color[i]) ){
			color[i] = g_ascii_tolower(color[i]);
		}
	}
	
	// some colors have not the same meaning 
	// on rgb.txt and CSS's spec
	if( strcmp("maroon",color)==0 )	{
		oldcolor = color;
		color = g_strdup("#800000");
		g_free(oldcolor);
	}else if( strcmp("olive",color)==0 ){
		oldcolor = color;
		color = g_strdup("#808000");
		g_free(oldcolor);
	}else if( strcmp("purple",color)==0 ){
		oldcolor = color;
		color = g_strdup("#800080");
		g_free(oldcolor);
	}else if( strcmp("fuchsia",color)==0 ){
		oldcolor = color;
		color = g_strdup("#ff00ff");
		g_free(oldcolor);
	}else if( strcmp("lime",color)==0 ){
		oldcolor = color;
		color = g_strdup("#00ff00");
		g_free(oldcolor);
	}else if( strcmp("green",color)==0 ){
		oldcolor = color;
		color = g_strdup("#008000");
		g_free(oldcolor);
	}else if( strcmp("aqua",color)==0 ){
		oldcolor = color;
		color = g_strdup("#00ffff");
		g_free(oldcolor);
	}else if( strcmp("silver",color)==0 ){
		oldcolor = color;
		color = g_strdup("#c0c0c0");
		g_free(oldcolor);
	}else if( strcmp("teal",color)==0 ){
		oldcolor = color;
		color = g_strdup("#008080");
		g_free(oldcolor);
	}else if( strcmp("gray",color)==0 ){
		oldcolor = color;
		color = g_strdup("#808080");
		g_free(oldcolor);
	}

	if( gdk_color_parse(color, &gdkcolor) )
	{
		dialog = gtk_color_selection_dialog_new(_("Edit color"));
		colorsel = GTK_COLOR_SELECTION(GTK_COLOR_SELECTION_DIALOG(dialog)->colorsel);
		gtk_color_selection_set_current_color(colorsel, &gdkcolor);
		response = gtk_dialog_run(GTK_DIALOG(dialog));
		if( response == GTK_RESPONSE_OK ){
			gtk_color_selection_get_current_color(colorsel, &gdkcolor);
			colorstr = gdk_color_to_rgb_triplet_string(&gdkcolor);
			document_replace_sel(doc, colorstr);
			g_free(colorstr);
		}
		gtk_widget_destroy(dialog);				
	}
	g_free(color);
	
}

/* ++++++++++++++++++++++ TESTING HERE +++++++++++++++++++++++++++++++ 
// PANEDBUG
void on_handle_position_changed (GObject *pane, GParamSpec *pspec, gpointer data){
	g_print( "handle %p position changed to: %d\n", pane, gtk_paned_get_position(GTK_PANED(pane)) );
}
*/
gboolean
document_scanner_treeview_search_equal_func (GtkTreeModel *model,
                                             gint column,
                                             const gchar *key,
                                             GtkTreeIter *iter,
                                             gpointer search_data)
{
	gchar *store_string;
	gchar *nonmarkup_string;
	gint key_len;
	gboolean retval;

	gtk_tree_model_get (model, iter, column, &store_string, -1);
	if (!store_string)	return FALSE;

	key_len = strlen (key);
	pango_parse_markup (store_string, strlen(store_string), 0, NULL, &nonmarkup_string, NULL, NULL);
	
	retval = (strncmp (key, nonmarkup_string, key_len) != 0);

	g_free (store_string);
	g_free (nonmarkup_string);

	return retval;
}

/* ***************************** HELP BUTTONS ******************************* */
/*    ---------------------------   TO DO ----------------------------------  */
void
on_color_selection_help_button_clicked (GtkButton * button, gpointer user_data)
{

}

void
on_dialog_search_help_button_clicked (GtkButton * button, gpointer user_data)
{

}


void
on_dialog_replace_help_button_clicked (GtkButton * button, gpointer user_data)
{

}
