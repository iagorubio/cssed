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

#include <string.h>
#include <stdio.h>

#include <gdk/gdkkeysyms.h>
#include <gtk/gtk.h>

#include "cssedwindow.h"
#include "document.h"
#include "interface.h"
#include "cssdialogs-class.h"
#include "plugin.h"
#include "support.h"
#include "utils.h"
#include "debug.h"

#define PRIV(obj) ((obj)->priv)

struct _CssedPluginPriv
{
	GModule* module_instance;	
	CssedWindow* window;
	gchar* filename;
	gboolean loaded;
};

/* debugging function */
void
dump_list (gpointer data,  gpointer user_data)
{
	CssedPlugin* p;
	
	p = (CssedPlugin*) data;
	
	g_print( "dump_list:\n" );
	if( p == NULL ){
		g_print( "Null plugin\n" );
		return;
	}else{
		g_print( "Plugin %p\n", p);
	}
	g_print("Name %s\n",  p->name );
	if( PRIV(p)->filename == NULL ){
		g_print( "Plugin filename NULL\n");
		return;
	}else{
		g_print( "Plugin filename %s\n", PRIV(p)->filename);
	}
}
/* end debugging function */

CssedPlugin*
cssed_init_plugin(CssedWindow* window, gchar* filename)
{
	GModule* module = NULL;
	CssedPlugin *plugin = NULL;
	CssedPlugin *(*init_plugin)() = NULL;
	gboolean test;

	
   	module = g_module_open( filename, 0 );	
	
	if( module == NULL ){
		cssed_error_message(_("Cannot load plugin"), _("Cannot open module \"%s\"\n\n%s\n"), filename, g_module_error());
		return NULL;		
	}

	test = g_module_symbol( module, "init_plugin", (gpointer) &init_plugin);

	if (!test)
	{
		cssed_error_message(_("Cannot load plugin"), _("Error while loading \"%s\"\n\n%s\n"), filename, g_module_error());
		g_module_close(module);
		return NULL;
	}
	
	plugin = (*init_plugin)();
	DBGMSG (__FILE__,__LINE__,"cssed_init_plugin address: %p", plugin);

	if (!plugin)
	{
		cssed_error_message(_("Cannot load plugin"), _("Error while initializing: \"%s\"\n"), filename);
		g_module_close(module);
		return NULL;
	}

	PRIV(plugin) = g_malloc0 (sizeof (CssedPluginPriv)) ;
	PRIV(plugin)->module_instance = (gpointer) module;
	PRIV(plugin)->window = window;
	PRIV(plugin)->filename = g_strdup (filename);
	PRIV(plugin)->loaded = FALSE;
	return plugin;	
}

void
cssed_load_plugin( CssedPlugin* plugin )
{
	CssedWindow* window;
	
	if(	plugin->load_plugin(plugin) == TRUE ){
		PRIV(plugin)->loaded = TRUE;
		window = PRIV(plugin)->window;
		cssed_window_add_plugin_to_list (window, (gpointer) plugin);
	}else{
		cssed_unload_plugin (plugin);
		cssed_error_message (_("Cannot load plugin"), _("The plugin returned an internal error"));
	}
}

void
cssed_unload_plugin( CssedPlugin* plugin )
{
	GModule* module;
	gboolean closed;
	CssedPluginPriv* priv;
	CssedWindow* window;
	
	g_return_if_fail( plugin != NULL );
	
	plugin->clean_plugin( plugin );	
	module = PRIV(plugin)->module_instance;
	window = PRIV(plugin)->window;
	priv = PRIV(plugin);	

	if( module != NULL ){
		g_free( priv->filename );
		g_free( priv );
		cssed_window_delete_plugin_from_list ( window, (gpointer)  plugin );
		closed =  g_module_close ( module );
		if(!closed){
			cssed_error_message(_("Unable to unload"),
								_("Cannot unload the plugin"));
		}
		return;
	}else{
		return;
	}
}

gboolean
cssed_is_plugin_file_loaded ( CssedWindow* window, gchar* path )
{
	GList* plugin_list;
	gchar* filename;
	
	plugin_list = cssed_window_get_plugin_list_first(  window );		

	while( plugin_list ){
		CssedPlugin* current_plugin;
		current_plugin = (CssedPlugin*) plugin_list->data;
		filename = cssed_plugin_get_filename( current_plugin );
		if( filename != NULL ){
			if( strcmp( path, filename ) == 0 ){
				return TRUE;
			}
		}
		plugin_list = g_list_next( plugin_list );
	}	
	return FALSE;
}

CssedPlugin* 
cssed_get_plugin_if_file_loaded ( CssedWindow* window, gchar* path )
{
	GList* plugin_list;
	gchar* filename;
	
	plugin_list  = cssed_window_get_plugin_list_first(  window );	

	while( plugin_list ){
		CssedPlugin* current_plugin;
		current_plugin = (CssedPlugin*) plugin_list->data;
		filename = cssed_plugin_get_filename(current_plugin);
		if( filename != NULL ){
			if( strcmp( path, filename ) == 0 ){
				return current_plugin;
			}
		}
		plugin_list = g_list_next( plugin_list );
	}	
	return NULL;
}

gchar*
cssed_plugin_get_filename( CssedPlugin* plugin )
{
	g_return_val_if_fail( plugin != NULL, NULL );
	g_return_val_if_fail( PRIV(plugin)->filename != NULL, NULL );
	return  PRIV(plugin)->filename;
}
// callback
G_MODULE_EXPORT void
cssed_plugin_unload_callback( GtkWidget* widget, gpointer plugin )
{
	cssed_unload_plugin( (CssedPlugin*) plugin );
}

// UI functions
G_MODULE_EXPORT gboolean
cssed_add_menu_item_to_plugins_menu( CssedPlugin* plugin, GtkWidget* menu_item )
{
	GtkWidget* plugins_menu;
	GtkWidget* window_widget;
	CssedWindow* window;
	
	window = PRIV(plugin)->window;
	window_widget = cssed_window_get_window_widget( window );
	
	if( !GTK_IS_WIDGET( window_widget ) ){
		return FALSE;
	}
	plugins_menu = lookup_widget( window_widget, "plugins_menu");
	if( plugins_menu == NULL ){
		return FALSE;
	}

	gtk_container_add(GTK_CONTAINER(plugins_menu), menu_item);		
	return TRUE;
}

G_MODULE_EXPORT gboolean
cssed_plugin_add_widget_to_toolbar( CssedPlugin* plugin, 
							 GtkWidget* widget,
							 const char *tooltip_text,
							 const char *tooltip_private_text
							)
{
	GtkWidget* toolbar;
	GtkWidget* window_widget;
	CssedWindow* window;
	
	window = PRIV(plugin)->window;
	window_widget = cssed_window_get_window_widget( window );
	
	if( !GTK_IS_WIDGET( window_widget ) ){
		return FALSE;
	}
	toolbar = lookup_widget( window_widget, "toolbar_tools");
	if( toolbar == NULL ){
		return FALSE;
	}
	gtk_toolbar_append_widget (	GTK_TOOLBAR(toolbar), widget, 
								tooltip_text, tooltip_private_text );
	
	return TRUE;	
}
							
G_MODULE_EXPORT gboolean
cssed_plugin_add_page_with_widget_to_footer( CssedPlugin* plugin,
											GtkWidget* widget,
											gchar* label_text )
{
	GtkWidget* footer_notebook;
	GtkWidget* window_widget;
	GtkWidget* label;
	CssedWindow* window;
	
	if( plugin == NULL )
		return FALSE;
	
	window = PRIV(plugin)->window;
	window_widget = cssed_window_get_window_widget( window );	
	footer_notebook = cssed_window_get_footer_notebook (  window );
	
	label = gtk_label_new( label_text );
	
	gtk_notebook_append_page   (GTK_NOTEBOOK(footer_notebook),
                                widget,
                                label);
	return TRUE;
}

G_MODULE_EXPORT gboolean
cssed_plugin_remove_page_with_widget_in_footer( CssedPlugin* plugin,
											GtkWidget* widget )
{
	GtkWidget* footer_notebook;
	GtkWidget* window_widget;
	gint page_index;
	CssedWindow* window;
	
	if( plugin == NULL )
		return FALSE;
	
	window = PRIV(plugin)->window;
	window_widget = cssed_window_get_window_widget( window );	
	footer_notebook = cssed_window_get_footer_notebook ( window );	
	
	page_index = gtk_notebook_page_num  (GTK_NOTEBOOK(footer_notebook),
                                         widget);
	if( page_index != -1 )
	{
		gtk_notebook_remove_page (GTK_NOTEBOOK(footer_notebook), page_index);
		return TRUE;
	}
	else
	{
		return FALSE;
	}		
}

G_MODULE_EXPORT gboolean
cssed_plugin_add_page_with_widget_to_sidebar( CssedPlugin* plugin,
											GtkWidget* widget,
											gchar* label_text )
{
	GtkWidget* sidebar_notebook;
	GtkWidget* label;
	CssedWindow* window;
	
	if( plugin == NULL )
		return FALSE;
	
	window = PRIV(plugin)->window;
	sidebar_notebook = cssed_window_get_sidebar_notebook ( window );
	
	label = gtk_label_new( label_text );
	
	gtk_notebook_append_page   (GTK_NOTEBOOK(sidebar_notebook),
                                widget,
                                label);
	return TRUE;
}

G_MODULE_EXPORT gboolean
cssed_plugin_remove_page_with_widget_in_sidebar( CssedPlugin* plugin,
											GtkWidget* widget )
{
	GtkWidget* sidebar_notebook;
	gint page_index;
	CssedWindow* window;
	
	if( plugin == NULL )
		return FALSE;
	
	window = PRIV(plugin)->window;
	sidebar_notebook = cssed_window_get_sidebar_notebook ( window );	
	
	page_index = gtk_notebook_page_num  (GTK_NOTEBOOK(sidebar_notebook),
                                         widget);
	if( page_index != -1 )
	{
		gtk_notebook_remove_page        (GTK_NOTEBOOK(sidebar_notebook),
                                           page_index);
		return TRUE;
	}
	else
	{
		return FALSE;
	}		
}

G_MODULE_EXPORT gboolean
cssed_plugin_select_page_with_widget_in_sidebar( CssedPlugin* plugin,
											GtkWidget* widget )
{
	GtkWidget* sidebar_notebook;
	gint page_index;
	CssedWindow* window;
	
	if( plugin == NULL )
		return FALSE;
	
	window = PRIV(plugin)->window;
	sidebar_notebook = cssed_window_get_sidebar_notebook ( window );	

	page_index = gtk_notebook_page_num  (GTK_NOTEBOOK(sidebar_notebook), widget);
	if( page_index != -1 )
	{
		gtk_notebook_set_current_page     (GTK_NOTEBOOK(sidebar_notebook), page_index);
		return TRUE;
	}
	else
	{
		return FALSE;
	}

}

G_MODULE_EXPORT gboolean
cssed_plugin_select_page_with_widget_in_footer( CssedPlugin* plugin, GtkWidget* widget )
{
	GtkWidget* footer_notebook;
	gint page_index;
	CssedWindow* window;
	
	if( plugin == NULL )
		return FALSE;
	
	window = PRIV(plugin)->window;
	footer_notebook = cssed_window_get_footer_notebook ( window );	

	page_index = gtk_notebook_page_num (GTK_NOTEBOOK(footer_notebook), widget);
	if( page_index != -1 )
	{
		gtk_notebook_set_current_page (GTK_NOTEBOOK(footer_notebook), page_index);
		return TRUE;
	}
	else
		return FALSE;
}

/* ***************** utility dialogs ************************* */
// show an error dialog
G_MODULE_EXPORT void
cssed_plugin_error_message( gchar* title, gchar* message, ... )
{	
	va_list args;
	gchar* error_message;
	
	va_start(args, message);
    error_message = g_strdup_vprintf(message, args);
    va_end(args);
    
	cssed_error_message (title, error_message);
	g_free (error_message);
}

// this returns a boolean with the user's response OK = TRUE, !OK = FALSE
G_MODULE_EXPORT gboolean
cssed_plugin_confirm_dialog ( gchar* title, gchar* message, ... )
{
	va_list args;
	GtkWidget* dialog;
	gint response;
	gchar* format_message;
	
	va_start(args, message);
    format_message = g_strdup_vprintf(message, args);
    va_end(args);
    
	dialog = create_ok_cancel_dialog (title, format_message);
	response = gtk_dialog_run (GTK_DIALOG(dialog));
	gtk_widget_destroy (dialog);	
	g_free (format_message);
	
	if( response == GTK_RESPONSE_OK ) return TRUE;
	else return FALSE;
}
/* ***************** document functions ********************** */
// open a named file
G_MODULE_EXPORT void
cssed_plugin_open_file( CssedPlugin* plugin, gchar* filename )
{
	CssedWindow* window;

	g_return_if_fail( plugin != NULL );
	
	window = PRIV(plugin)->window;
	document_open_with_error_check( window, filename );
}
// shows the open file selector dialog
G_MODULE_EXPORT void
cssed_plugin_open_file_with_fileselector( CssedPlugin* plugin )
{
	CssedWindow* window;
	gchar* filename;

	g_return_if_fail( plugin != NULL );
	window = PRIV(plugin)->window;		

	filename = cssed_prompt_for_file(window, _("Select file to open"), NULL, CSSED_FILESELECTOR_OPEN);
	if( filename != NULL ){
		document_open_with_error_check(window, filename);
		g_free(filename);
	}
}

// will add text to the current document at the current document's position
G_MODULE_EXPORT void 
cssed_plugin_add_text_to_document( CssedPlugin* plugin, gchar* text )
{
	CssedWindow* window;
	CssedDoc* doc;

	g_return_if_fail( plugin != NULL );
	
	window = PRIV(plugin)->window;	
	doc = document_get_current( window );
	
	document_add_text( doc, text );	
}

G_MODULE_EXPORT gchar*
cssed_plugin_get_text( CssedPlugin* plugin )
{
	CssedWindow* window;
	CssedDoc* doc;	
	gchar* buffer;
	gint len;
	
	g_return_val_if_fail( plugin != NULL, NULL );
	
	window = PRIV(plugin)->window;	
	doc = document_get_current( window );		
	len = document_get_length( doc ); // what if len == 0 ? FIXME
	buffer = g_malloc0(++len);
	document_get_text(doc, len, buffer);

	return buffer;	
}
	

G_MODULE_EXPORT void //deprecated
cssed_plugin_select_text_range( CssedPlugin* plugin, gint start, gint end )
{
	cssed_plugin_set_selection_range ( plugin, start, end );
}
	
G_MODULE_EXPORT gchar*
cssed_plugin_get_selected_text(  CssedPlugin* plugin  )
{
	CssedWindow* window;
	CssedDoc* doc;	
	gchar* buffer;
	gint start;
	gint end;
	gint len;
	
	g_return_val_if_fail( plugin != NULL, NULL );
	
	window = PRIV(plugin)->window;	
	doc = document_get_current( window );
	start = document_get_selection_start( doc );
	end = document_get_selection_end( doc );
	len = end - start;
	
	if( len <= 0 ) return NULL;
	
	buffer = g_malloc0(++len);
	document_get_selected_text( doc, buffer );

	return buffer;
}
	
G_MODULE_EXPORT void
cssed_plugin_clear_selected_text( CssedPlugin* plugin )
{
	CssedWindow* window;
	CssedDoc* doc;	
	
	g_return_if_fail( plugin != NULL );
	
	window = PRIV(plugin)->window;	
	doc = document_get_current( window );
	document_clear_sel( doc );
}


// undo redo functions, useful to revert to previous state
G_MODULE_EXPORT void
cssed_plugin_undo_document_action ( CssedPlugin* plugin )
{
	CssedWindow* window;
	CssedDoc* doc;

	g_return_if_fail( plugin != NULL );
	
	window = PRIV(plugin)->window;	
	doc = document_get_current( window );	
	document_undo( doc );	
}
	
G_MODULE_EXPORT void
cssed_plugin_redo_document_action ( CssedPlugin* plugin )
{
	CssedWindow* window;
	CssedDoc* doc;

	g_return_if_fail( plugin != NULL );
	
	window = PRIV(plugin)->window;	
	doc = document_get_current( window );		
	document_redo( doc );
}

G_MODULE_EXPORT gboolean
cssed_plugin_document_can_redo ( CssedPlugin* plugin )
{
	CssedWindow* window;
	CssedDoc* doc;

	g_return_val_if_fail( plugin != NULL, FALSE );

	window = PRIV(plugin)->window;	
	doc = document_get_current( window );		
	return document_can_redo( doc );
}

G_MODULE_EXPORT gboolean
cssed_plugin_document_can_undo ( CssedPlugin* plugin )
{
	CssedWindow* window;
	CssedDoc* doc;

	g_return_val_if_fail( plugin != NULL, FALSE );

	window = PRIV(plugin)->window;	
	doc = document_get_current( window );		
	return document_can_undo( doc );
}

// search functions
G_MODULE_EXPORT void
cssed_plugin_search_next( CssedPlugin* plugin, gchar* text_to_search )
{
	CssedWindow* window;
	CssedDoc* doc;

	g_return_if_fail( plugin != NULL );
	window = PRIV(plugin)->window;	
	doc = document_get_current( window );		
	document_search_next( doc, text_to_search, 4 ); // WHOLEWORD only
}

G_MODULE_EXPORT void
cssed_plugin_search_prev( CssedPlugin* plugin, gchar* text_to_search )
{
	CssedWindow* window;
	CssedDoc* doc;

	g_return_if_fail( plugin != NULL );
	window = PRIV(plugin)->window;	
	doc = document_get_current( window );		
	document_search_prev( doc, text_to_search, 4 ); // WHOLEWORD only	
}
// for marker 0 bookmarks ////////////////////////////////////////////////////////////////////////////////////////////
G_MODULE_EXPORT void
cssed_plugin_bookmark_next ( CssedPlugin* plugin )
{
	CssedWindow* window;
	CssedDoc* doc;
	gint position, line;

	g_return_if_fail( plugin != NULL );
	window = PRIV(plugin)->window;	
	doc = document_get_current (window);
	position = document_get_current_position (doc);
	line = document_get_line_from_position (doc, position);
	document_marker_next (doc, line, 1);
}
G_MODULE_EXPORT void
cssed_plugin_bookmark_prev ( CssedPlugin* plugin )
{
	CssedWindow* window;
	CssedDoc* doc;
	gint position, line;

	g_return_if_fail( plugin != NULL );
	window = PRIV(plugin)->window;	
	doc = document_get_current (window);
	position = document_get_current_position (doc);
	line = document_get_line_from_position (doc, position);
	document_marker_prev (doc, line, 1);	

}

G_MODULE_EXPORT void
cssed_plugin_set_bookmark_at_line ( CssedPlugin* plugin, gboolean set, gint line )
{
	CssedWindow* window;
	CssedDoc* doc;

	g_return_if_fail( plugin != NULL );
	window = PRIV(plugin)->window;	
	doc = document_get_current (window);		
	document_set_marker_at_line (doc, line, set, 0);
}


G_MODULE_EXPORT gboolean
cssed_plugin_is_bookmark_set_at_line ( CssedPlugin* plugin, gint line )
{
	CssedWindow* window;
	CssedDoc* doc;

	g_return_val_if_fail (plugin != NULL, FALSE);
	window = PRIV(plugin)->window;	
	doc = document_get_current (window);		
	return document_is_marker_set_at_line (doc, line, 0);
}
// for marker 2 (roud)
G_MODULE_EXPORT gboolean
cssed_plugin_marker_next ( CssedPlugin* plugin )
{
	CssedWindow* window;
	CssedDoc* doc;
	gint position, line;

	g_return_val_if_fail( plugin != NULL, FALSE );
	window = PRIV(plugin)->window;	
	doc = document_get_current (window);
	position = document_get_current_position (doc);
	line = document_get_line_from_position (doc, position);
	return document_marker_next (doc, line, 1 << 2);
}

G_MODULE_EXPORT gboolean
cssed_plugin_marker_prev ( CssedPlugin* plugin )
{
	CssedWindow* window;
	CssedDoc* doc;
	gint position, line;

	g_return_val_if_fail( plugin != NULL, FALSE );
	window = PRIV(plugin)->window;	
	doc = document_get_current (window);
	position = document_get_current_position (doc);
	line = document_get_line_from_position (doc, position);
	return document_marker_prev (doc, line, 1 << 2); 
}

G_MODULE_EXPORT void
cssed_plugin_set_marker_at_line  ( CssedPlugin* plugin, gboolean set, gint line )
{
	CssedWindow* window;
	CssedDoc* doc;

	g_return_if_fail( plugin != NULL );
	window = PRIV(plugin)->window;	
	doc = document_get_current (window);		
	document_set_marker_at_line (doc, line, set, 2);
}

G_MODULE_EXPORT gboolean
cssed_plugin_is_marker_set_at_line ( CssedPlugin* plugin, gint line )
{
	CssedWindow* window;
	CssedDoc* doc;

	g_return_val_if_fail (plugin != NULL, FALSE);
	window = PRIV(plugin)->window;	
	doc = document_get_current (window);
 
	return document_is_marker_set_at_line (doc, line, 2);
}
// output
G_MODULE_EXPORT void
cssed_plugin_output_write ( CssedPlugin* plugin, gchar *markup, ... )
{
	CssedWindow* window;
	va_list args;
	gchar* format_message;
	    
	g_return_if_fail( plugin != NULL );
	
	va_start(args, markup);
    format_message = g_strdup_vprintf (markup, args);
    va_end(args);	
	
	window = PRIV(plugin)->window;
	cssed_window_output_write (window, format_message);
	g_free (format_message);
}

G_MODULE_EXPORT void
cssed_plugin_output_clear ( CssedPlugin* plugin )
{
	CssedWindow* window;

	g_return_if_fail( plugin != NULL );
	window = PRIV(plugin)->window;
	cssed_window_output_clear (window);
}
//
G_MODULE_EXPORT gint
cssed_plugin_get_line_from_position ( CssedPlugin* plugin, gint pos )
{
	CssedWindow* window;
	CssedDoc* doc;

	g_return_val_if_fail (plugin != NULL, FALSE);
	window = PRIV(plugin)->window;	
	doc = document_get_current (window);

	return document_get_line_from_position (doc, pos);
}

G_MODULE_EXPORT gint
cssed_plugin_get_position_from_line ( CssedPlugin* plugin, gint line )
{
	CssedWindow* window;
	CssedDoc* doc;

	g_return_val_if_fail (plugin != NULL, FALSE);
	window = PRIV(plugin)->window;	
	doc = document_get_current (window);

	return document_get_position_from_line (doc, line);
}

G_MODULE_EXPORT gint // deprecated
cssed_plugin_get_position_at_line ( CssedPlugin* plugin, gint line )
{
	return cssed_plugin_get_position_from_line ( plugin, line );
}

G_MODULE_EXPORT gint
cssed_plugin_get_current_position ( CssedPlugin* plugin )
{
	CssedWindow* window;
	CssedDoc* doc;

	g_return_val_if_fail (plugin != NULL, FALSE);
	window = PRIV(plugin)->window;	
	doc = document_get_current (window);

	return document_get_current_position (doc);
}

G_MODULE_EXPORT void
cssed_plugin_set_current_position ( CssedPlugin* plugin, gint pos )
{
	CssedWindow* window;
	CssedDoc* doc;

	g_return_if_fail (plugin != NULL);
	window = PRIV(plugin)->window;	
	doc = document_get_current (window);

	document_set_current_position (doc, pos);
}

G_MODULE_EXPORT void
cssed_plugin_set_current_line ( CssedPlugin* plugin , gint line )
{
	CssedWindow* window;
	CssedDoc* doc;

	g_return_if_fail (plugin != NULL);
	window = PRIV(plugin)->window;	
	doc = document_get_current (window);

	document_set_current_line (doc, line);
}

G_MODULE_EXPORT gint
cssed_plugin_get_current_line ( CssedPlugin* plugin )
{
	CssedWindow* window;
	CssedDoc* doc;
	gint pos;

	g_return_val_if_fail (plugin != NULL, FALSE);
	window = PRIV(plugin)->window;	
	doc = document_get_current (window);
	pos = document_get_current_position (doc);

	return document_get_line_from_position (doc, pos);
}

G_MODULE_EXPORT gint
cssed_plugin_get_line_end_from_position ( CssedPlugin* plugin, gint pos )
{
	CssedWindow* window;
	CssedDoc* doc;

	g_return_val_if_fail (plugin != NULL, FALSE);
	window = PRIV(plugin)->window;	
	doc = document_get_current (window);

	return document_get_line_end_from_position (doc, pos);
}
// cut copy paste
G_MODULE_EXPORT void
cssed_plugin_cut ( CssedPlugin* plugin )
{
	CssedWindow* window;
	CssedDoc* doc;

	g_return_if_fail (plugin != NULL);
	window = PRIV(plugin)->window;	
	doc = document_get_current (window);
	document_cut (doc);
}
G_MODULE_EXPORT void
cssed_plugin_copy ( CssedPlugin* plugin )
{
	CssedWindow* window;
	CssedDoc* doc;

	g_return_if_fail (plugin != NULL);
	window = PRIV(plugin)->window;	
	doc = document_get_current (window);
	document_copy (doc);
}
G_MODULE_EXPORT void
cssed_plugin_paste ( CssedPlugin* plugin )
{
	CssedWindow* window;
	CssedDoc* doc;

	g_return_if_fail (plugin != NULL);
	window = PRIV(plugin)->window;	
	doc = document_get_current (window);
	document_paste (doc);
}
// selection
G_MODULE_EXPORT gint
cssed_plugin_get_selection_end ( CssedPlugin* plugin )
{
	CssedWindow* window;
	CssedDoc* doc;
	g_return_val_if_fail (plugin != NULL, FALSE);
	window = PRIV(plugin)->window;	
	doc = document_get_current (window);

	return document_get_selection_end (doc);
}

G_MODULE_EXPORT gint
cssed_plugin_get_selection_start ( CssedPlugin* plugin )
{
	CssedWindow* window;
	CssedDoc* doc;
	g_return_val_if_fail (plugin != NULL, FALSE);
	window = PRIV(plugin)->window;	
	doc = document_get_current (window);

	return document_get_selection_start (doc);
}

G_MODULE_EXPORT void
cssed_plugin_set_selection_start ( CssedPlugin* plugin, gint start )
{
	CssedWindow* window;
	CssedDoc* doc;

	g_return_if_fail (plugin != NULL);
	window = PRIV(plugin)->window;	
	doc = document_get_current (window);
	document_set_selection_start (doc, start);
}

G_MODULE_EXPORT void
cssed_plugin_set_selection_end ( CssedPlugin* plugin, gint end )
{
	CssedWindow* window;
	CssedDoc* doc;

	g_return_if_fail (plugin != NULL);
	window = PRIV(plugin)->window;	
	doc = document_get_current (window);
	document_set_selection_end (doc, end);
}

G_MODULE_EXPORT void
cssed_plugin_set_selection_range ( CssedPlugin* plugin, gint start, gint end )
{
	CssedWindow* window;
	CssedDoc* doc;	
	
	g_return_if_fail( plugin != NULL );	
	window = PRIV(plugin)->window;	
	doc = document_get_current( window );
	document_set_selection_range( doc, start, end );
}

G_MODULE_EXPORT void
cssed_plugin_replace_sel  ( CssedPlugin* plugin, gchar *text )
{
	CssedWindow* window;
	CssedDoc* doc;	
	
	g_return_if_fail( plugin != NULL );
	g_return_if_fail( text != NULL );
	
	window = PRIV(plugin)->window;	
	doc = document_get_current( window );
	document_replace_sel (doc, text );
}

G_MODULE_EXPORT void
cssed_plugin_clear_sel  ( CssedPlugin* plugin )
{
	CssedWindow* window;
	CssedDoc* doc;	
	
	g_return_if_fail( plugin != NULL );
	
	window = PRIV(plugin)->window;	
	doc = document_get_current (window);
	document_clear_sel (doc);
}
//
G_MODULE_EXPORT gint
cssed_plugin_get_length	 ( CssedPlugin* plugin )
{
	CssedWindow* window;
	CssedDoc* doc;	
	
	g_return_val_if_fail( plugin != NULL, 0 );
	
	window = PRIV(plugin)->window;	
	doc = document_get_current (window);
	return document_get_length (doc);
}
//
G_MODULE_EXPORT void
cssed_plugin_show_user_list ( CssedPlugin* plugin, gchar* list )
{
	CssedWindow* window;
	CssedDoc* doc;	
	
	g_return_if_fail( plugin != NULL );
	g_return_if_fail( list != NULL );
	
	window = PRIV(plugin)->window;	
	doc = document_get_current (window);
	document_show_user_list (doc, 0, list); // it's not tracked on notify
}
G_MODULE_EXPORT void
cssed_plugin_show_autocompletion_list (CssedPlugin* plugin, gint len, gchar* list)
{
	CssedWindow* window;
	CssedDoc* doc;	
	
	g_return_if_fail( plugin != NULL );
	g_return_if_fail( list != NULL );
	
	window = PRIV(plugin)->window;	
	doc = document_get_current (window);
	document_show_autocompletion_list (doc, len, list); // it's not tracked on notify
}
G_MODULE_EXPORT void 
cssed_plugin_autocompletion_cancel (CssedPlugin* plugin)
{
	CssedWindow* window;
	CssedDoc* doc;	
	
	g_return_if_fail( plugin != NULL );
	
	window = PRIV(plugin)->window;	
	doc = document_get_current (window);
	document_autocompletion_cancel (doc);
}
G_MODULE_EXPORT gboolean
cssed_plugin_is_autocompletion_active (CssedPlugin* plugin)
{
	CssedWindow* window;
	CssedDoc* doc;	
	
	g_return_val_if_fail( plugin != NULL, FALSE);
	
	window = PRIV(plugin)->window;	
	doc = document_get_current (window);
	return document_is_autocompletion_active (doc);
}
G_MODULE_EXPORT void 
cssed_plugin_autocompletion_select (CssedPlugin* plugin, gchar* select)
{
	CssedWindow* window;
	CssedDoc* doc;	
	
	g_return_if_fail( plugin != NULL );
	g_return_if_fail( select != NULL );
	
	window = PRIV(plugin)->window;	
	doc = document_get_current (window);
	document_autocompletion_select  (doc, select); 
}
// calltip
G_MODULE_EXPORT void
cssed_plugin_show_calltip ( CssedPlugin* plugin, gint start, gchar* definition )
{
	CssedWindow* window;
	CssedDoc* doc;	
	
	g_return_if_fail( plugin != NULL );	

	window = PRIV(plugin)->window;	
	doc = document_get_current (window);
	document_calltipshow  (doc, start, definition); 
}

G_MODULE_EXPORT void
cssed_plugin_cancel_calltip ( CssedPlugin* plugin )
{
	CssedWindow* window;
	CssedDoc* doc;	
	
	g_return_if_fail( plugin != NULL );	

	window = PRIV(plugin)->window;	
	doc = document_get_current (window);
	document_calltipcancel (doc);
}

G_MODULE_EXPORT gboolean
cssed_plugin_calltipactive ( CssedPlugin* plugin )
{
	CssedWindow* window;
	CssedDoc* doc;	
	
	g_return_val_if_fail( plugin != NULL, FALSE );	

	window = PRIV(plugin)->window;	
	doc = document_get_current (window);
	return document_calltipactive (doc);
}

G_MODULE_EXPORT gint
cssed_plugin_calltiposstart ( CssedPlugin* plugin )
{
	CssedWindow* window;
	CssedDoc* doc;	
	
	g_return_val_if_fail( plugin != NULL, -1);	

	window = PRIV(plugin)->window;	
	doc = document_get_current (window);
	return document_calltiposstart (doc);
}
//
G_MODULE_EXPORT void
cssed_plugin_document_grab_focus ( CssedPlugin* plugin )
{
	CssedWindow* window;
	CssedDoc* doc;	
	
	g_return_if_fail( plugin != NULL);	

	window = PRIV(plugin)->window;	
	doc = document_get_current (window);
	document_grab_focus (doc);
}

// line related
G_MODULE_EXPORT gchar*
cssed_plugin_get_text_line (CssedPlugin* plugin, gint line)
{
	CssedWindow* window;
	CssedDoc* doc;	
	gint count, size;
	gchar* text;

	g_return_val_if_fail( plugin != NULL, NULL );
	
	window = PRIV(plugin)->window;	
	doc = document_get_current (window);

	count = document_get_line_count (doc);
	if( line > count ) return NULL;

	size = document_get_line_length (doc, line);
	if( size == 0 ) return g_strdup("");

	text = g_malloc0(sizeof(gchar)*(size+1));
	document_get_text_line(doc, line, text);

	return text;
}
G_MODULE_EXPORT gint
cssed_plugin_get_line_length (CssedPlugin* plugin, gint line)
{
	CssedWindow* window;
	CssedDoc* doc;	
	
	g_return_val_if_fail( plugin != NULL, 0);
	
	window = PRIV(plugin)->window;	
	doc = document_get_current (window);
	return document_get_line_length  (doc, line); 
}
G_MODULE_EXPORT gint
cssed_plugin_get_line_count (CssedPlugin* plugin)
{
	CssedWindow* window;
	CssedDoc* doc;	
	
	g_return_val_if_fail( plugin != NULL, 0);
	
	window = PRIV(plugin)->window;	
	doc = document_get_current (window);
	return document_get_line_count (doc);
}
// folding
G_MODULE_EXPORT void
cssed_plugin_toggle_fold_at_line (CssedPlugin* plugin, gint line)
{
	CssedWindow* window;
	CssedDoc* doc;	
	
	g_return_if_fail( plugin != NULL);
	
	window = PRIV(plugin)->window;	
	doc = document_get_current (window);
	document_toggle_fold_at_line (doc, line);
}

G_MODULE_EXPORT gboolean
cssed_plugin_get_line_is_visible (CssedPlugin* plugin, gint line)
{
	CssedWindow* window;
	CssedDoc* doc;	
	
	g_return_val_if_fail( plugin != NULL, 0);
	
	window = PRIV(plugin)->window;	
	doc = document_get_current (window);
	return document_get_line_is_visible (doc, line);
}

G_MODULE_EXPORT void
cssed_plugin_ensure_line_is_visible (CssedPlugin* plugin, gint line)
{
	CssedWindow* window;
	CssedDoc* doc;	
	
	g_return_if_fail( plugin != NULL);
	
	window = PRIV(plugin)->window;	
	doc = document_get_current (window);
	document_ensure_line_is_visible (doc, line);
}

G_MODULE_EXPORT gint // just returns the depth not as in the document API
cssed_plugin_get_fold_level (CssedPlugin* plugin, gint line)
{
	CssedWindow* window;
	CssedDoc* doc;	
	gint level;

	g_return_val_if_fail( plugin != NULL, 0);
	
	window = PRIV(plugin)->window;	
	doc = document_get_current (window);
	level = document_get_fold_level (doc, line);
	return level;
}
G_MODULE_EXPORT gint // just returns the depth not as in the document API
cssed_plugin_get_fold_depth (CssedPlugin* plugin, gint line)
{
	CssedWindow* window;
	CssedDoc* doc;	
	gint depth;

	g_return_val_if_fail( plugin != NULL, 0);
	
	window = PRIV(plugin)->window;	
	doc = document_get_current (window);
	depth = document_get_fold_depth (doc, line);
	return depth;
}
G_MODULE_EXPORT void
cssed_plugin_set_folding_margin_visible (CssedPlugin* plugin, gboolean set)
{
	CssedWindow* window;
	CssedDoc* doc;	
	
	g_return_if_fail( plugin != NULL);
	
	window = PRIV(plugin)->window;	
	doc = document_get_current (window);
	document_set_folding_margin_visible (doc, set);
}

G_MODULE_EXPORT gboolean
cssed_plugin_get_folding_margin_visible (CssedPlugin* plugin)
{
	CssedWindow* window;
	CssedDoc* doc;	
	
	g_return_val_if_fail( plugin != NULL, 0);
	
	window = PRIV(plugin)->window;	
	doc = document_get_current (window);
	return document_get_folding_margin_visible (doc);
}

G_MODULE_EXPORT gboolean 
cssed_plugin_get_folding_enabled (CssedPlugin* plugin)
{
	CssedWindow* window;
	CssedDoc* doc;	
	
	g_return_val_if_fail( plugin != NULL, 0);
	
	window = PRIV(plugin)->window;	
	doc = document_get_current (window);
	return document_get_folding_enabled (doc);
}

G_MODULE_EXPORT void
cssed_plugin_fold_all (CssedPlugin* plugin)
{
	CssedWindow* window;
	CssedDoc* doc;	
	
	g_return_if_fail( plugin != NULL);
	
	window = PRIV(plugin)->window;	
	doc = document_get_current (window);
	document_fold_all (doc);
}

G_MODULE_EXPORT void
cssed_plugin_unfold_all (CssedPlugin* plugin)
{
	CssedWindow* window;
	CssedDoc* doc;	
	
	g_return_if_fail( plugin != NULL);
	
	window = PRIV(plugin)->window;	
	doc = document_get_current (window);
	document_unfold_all (doc);
}


G_MODULE_EXPORT void
cssed_plugin_set_use_tabs (CssedPlugin* plugin, gboolean set )
{
	CssedWindow* window;
	CssedDoc* doc;	
	
	g_return_if_fail( plugin != NULL);
	
	window = PRIV(plugin)->window;	
	doc = document_get_current (window);
	document_set_use_tabs (doc, set);
}

G_MODULE_EXPORT gboolean
cssed_plugin_get_use_tabs (CssedPlugin* plugin)
{
	CssedWindow* window;
	CssedDoc* doc;	
	
	g_return_if_fail( plugin != NULL);
	
	window = PRIV(plugin)->window;	
	doc = document_get_current (window);
	return document_get_use_tabs(doc);
}

G_MODULE_EXPORT void
cssed_plugin_set_tabs_width (CssedPlugin* plugin, gint width )
{
	CssedWindow* window;
	CssedDoc* doc;	

	g_return_if_fail( plugin != NULL);

	window = PRIV(plugin)->window;	
	doc = document_get_current (window);
	document_set_tabs_width (doc, width);
}

G_MODULE_EXPORT gint
cssed_plugin_get_tabs_width (CssedPlugin* plugin)
{
	CssedWindow* window;
	CssedDoc* doc;	

	g_return_if_fail( plugin != NULL);

	window = PRIV(plugin)->window;	
	doc = document_get_current (window);
	return document_get_tabs_width(doc);
}

//
G_MODULE_EXPORT void
cssed_plugin_set_arrow_marker_at_line (CssedPlugin* plugin, gint line)
{
	CssedWindow* window;
	CssedDoc* doc;	
	
	g_return_if_fail( plugin != NULL);
	
	window = PRIV(plugin)->window;	
	doc = document_get_current (window);
	document_set_validator_arrow_at_line_on_idle (doc, line);
}

G_MODULE_EXPORT void
cssed_plugin_clear_arrow_marker (CssedPlugin* plugin)
{
	CssedWindow* window;
	CssedDoc* doc;	
	
	g_return_if_fail( plugin != NULL);
	
	window = PRIV(plugin)->window;	
	doc = document_get_current (window);
	document_clear_validator_arrow (doc);
}

// zooming
G_MODULE_EXPORT void
cssed_plugin_zoom_in (CssedPlugin* plugin)
{
	CssedWindow* window;
	CssedDoc* doc;	
	
	g_return_if_fail( plugin != NULL);
	
	window = PRIV(plugin)->window;	
	doc = document_get_current (window);
	document_zoom_in (doc);
}

G_MODULE_EXPORT void
cssed_plugin_zoom_out (CssedPlugin* plugin)
{
	CssedWindow* window;
	CssedDoc* doc;	
	
	g_return_if_fail( plugin != NULL);
	
	window = PRIV(plugin)->window;	
	doc = document_get_current (window);
	document_zoom_out (doc);
}
G_MODULE_EXPORT void
cssed_plugin_zoom_off (CssedPlugin* plugin)
{
	CssedWindow* window;
	CssedDoc* doc;	
	
	g_return_if_fail( plugin != NULL);
	
	window = PRIV(plugin)->window;	
	doc = document_get_current (window);
	document_zoom_off (doc);
}
//
G_MODULE_EXPORT gboolean
cssed_plugin_get_modified (CssedPlugin* plugin)
{
	CssedWindow* window;
	CssedDoc* doc;	
	
	g_return_val_if_fail( plugin != NULL, FALSE);
	
	window = PRIV(plugin)->window;	
	doc = document_get_current (window);
	return document_get_modified (doc);
}
//
G_MODULE_EXPORT gboolean
cssed_plugin_is_file_opened (CssedPlugin* plugin, gchar* filename)
{
	CssedWindow* window;

	g_return_val_if_fail( plugin != NULL, FALSE);	
	g_return_val_if_fail( filename != NULL, FALSE);	

	window = PRIV(plugin)->window;	
	return cssed_window_is_file_opened(window, filename);
}
G_MODULE_EXPORT gboolean
cssed_plugin_save_document( CssedPlugin* plugin )
{
	CssedWindow* window;
	CssedDoc* doc;

	g_return_val_if_fail( plugin != NULL, FALSE );
	window = PRIV(plugin)->window;	
	doc = document_get_current( window );	
	return document_save (doc);	
}
G_MODULE_EXPORT gboolean
cssed_plugin_save_document_as( CssedPlugin* plugin, gchar* filename )
{
	CssedWindow* window;
	CssedDoc* doc;

	g_return_val_if_fail( plugin != NULL, FALSE );
	g_return_val_if_fail( filename != NULL, FALSE );

	window = PRIV(plugin)->window;	
	doc = document_get_current( window );	
	return document_save_as (doc, filename);	
}

G_MODULE_EXPORT gint
cssed_plugin_get_document_index( CssedPlugin* plugin )
{
	CssedWindow* window;
	CssedDoc* doc;

	g_return_val_if_fail( plugin != NULL, -1 );

	window = PRIV(plugin)->window;	
	doc = document_get_current( window );	
	return document_get_index (doc);	
}
G_MODULE_EXPORT void
cssed_plugin_set_document_by_index (CssedPlugin* plugin, gint index)
{
	CssedWindow* window;
	GtkWidget* notebook;

	window = PRIV(plugin)->window;	
	notebook = cssed_window_get_document_notebook (window);
	gtk_notebook_set_current_page (GTK_NOTEBOOK(notebook), index);
}

G_MODULE_EXPORT gint
cssed_plugin_get_num_docs( CssedPlugin* plugin )
{
	CssedWindow* window;

	g_return_val_if_fail( plugin != NULL, -1 );

	window = PRIV(plugin)->window;	
	return cssed_window_get_num_docs (window);
}
G_MODULE_EXPORT gboolean
cssed_plugin_new_document (CssedPlugin* plugin)
{
	CssedWindow* window;
	CssedDoc* doc;

	g_return_val_if_fail( plugin != NULL, FALSE );

	window = PRIV(plugin)->window;
	doc = create_and_attach_new_doc (window, _("Untitled"));
	if( doc == NULL ) return FALSE;
	return TRUE;
}
G_MODULE_EXPORT gchar*
cssed_plugin_document_get_filename (CssedPlugin* plugin)
{
	CssedWindow* window;
	CssedDoc* doc;

	g_return_val_if_fail( plugin != NULL, NULL );

	window = PRIV(plugin)->window;	
	doc = document_get_current( window );	
	return document_get_filename (doc);	
}
G_MODULE_EXPORT gint
cssed_plugin_get_style_at(CssedPlugin* plugin, gint pos)
{
	CssedWindow* window;
	CssedDoc* doc;

	g_return_val_if_fail( plugin != NULL, 0 );
	g_return_val_if_fail( pos >= 0, 0 );

	window = PRIV(plugin)->window;	
	doc = document_get_current( window );	
	return document_get_style_at (doc, pos);	
}
// free result if not NULL
G_MODULE_EXPORT gchar*
cssed_plugin_prompt_for_file_to_open(CssedPlugin* plugin, gchar* dialog_title, gchar* basedir)
{
	CssedWindow *window;
	gchar *file;
	g_return_val_if_fail( plugin != NULL, 0 );
	g_return_val_if_fail( dialog_title != NULL, 0 );
	
	window = PRIV(plugin)->window;	
	file = cssed_prompt_for_file (window, dialog_title, basedir, CSSED_FILESELECTOR_OPEN);
	return file;
}
// free result if not NULL
G_MODULE_EXPORT gchar*
cssed_plugin_prompt_for_file_to_save(CssedPlugin* plugin, gchar* dialog_title, gchar* basedir)
{
	CssedWindow *window;
	gchar *file;
	g_return_val_if_fail( plugin != NULL, 0 );
	g_return_val_if_fail( dialog_title != NULL, 0 );
	
	window = PRIV(plugin)->window;	
	file = cssed_prompt_for_file (window, dialog_title, basedir, CSSED_FILESELECTOR_SAVE);
	return file;
}
// free result if not NULL
G_MODULE_EXPORT gchar*
cssed_plugin_prompt_for_directory_to_open(CssedPlugin* plugin, gchar* dialog_title, gchar* basedir)
{
	CssedWindow *window;
	gchar *file;
	g_return_val_if_fail( plugin != NULL, 0 );
	g_return_val_if_fail( dialog_title != NULL, 0 );
	
	window = PRIV(plugin)->window;	
	file = cssed_prompt_for_file (window, dialog_title, basedir, CSSED_FILESELECTOR_SELECT_FOLDER);
	return file;
}

// free result if not NULL
G_MODULE_EXPORT gchar*
cssed_plugin_prompt_for_directory_to_create(CssedPlugin* plugin, gchar* dialog_title, gchar* basedir)
{
	CssedWindow *window;
	gchar *file;
	g_return_val_if_fail( plugin != NULL, 0 );
	g_return_val_if_fail( dialog_title != NULL, 0 );
	
	window = PRIV(plugin)->window;	
	file = cssed_prompt_for_file (window, dialog_title, basedir, CSSED_FILESELECTOR_CREATE_FOLDER);
	return file;
}
///////////////////////////////////////////////////////////////////////////////////
// css dialogs
G_MODULE_EXPORT void 
cssed_plugin_css_dialog_insert( CssedPlugin* plugin, gchar* key, gpointer klass )
{
	CssedWindow* window;
	g_return_if_fail( plugin != NULL );	
	window = PRIV(plugin)->window;	
	cssed_window_css_dialog_insert( window, key, klass );	
}

G_MODULE_EXPORT void 
cssed_plugin_css_dialog_remove_by_keyword( CssedPlugin* plugin, gchar* key )
{
	CssedWindow* window;
	g_return_if_fail( plugin != NULL );	
	window = PRIV(plugin)->window;	
	cssed_window_css_dialog_remove_by_keyword( window, key );	
}

G_MODULE_EXPORT gpointer 
cssed_plugin_css_dialog_lookup_by_keyword( CssedPlugin* plugin, gchar* key )
{
	CssedWindow* window;
	if( plugin == NULL ) return NULL;	
	window = PRIV(plugin)->window;	
	return cssed_window_css_dialog_lookup_by_keyword( window, key );	
}

G_MODULE_EXPORT gboolean 
cssed_plugin_css_dialog_keyword_in_use( CssedPlugin* plugin, gchar* key )
{
	CssedWindow* window;
	if( plugin == NULL ) return FALSE;	
	window = PRIV(plugin)->window;	
	return cssed_window_css_dialog_keyword_in_use( window, key );	
}

G_MODULE_EXPORT gpointer
cssed_plugin_get_window( CssedPlugin* plugin )
{
	return (gpointer) PRIV(plugin)->window;
}

/* ******************************************************************* */
G_MODULE_EXPORT gint
cssed_plugin_get_filetype_id (CssedPlugin* plugin)
{
	CssedWindow* window;
	CssedDoc* doc;

	g_return_val_if_fail( plugin != NULL, 0 );

	window = PRIV(plugin)->window;	
	doc = document_get_current (window);	
	return document_get_filetype_id (doc);	
}

G_MODULE_EXPORT void
cssed_plugin_set_filetype_by_id (CssedPlugin* plugin, gint id)
{
	CssedWindow* window;
	CssedDoc* doc;

	g_return_if_fail (plugin != NULL);
	g_return_if_fail (CSSED_FILETYPE_UNKNOW <= id && CSSED_NUMFILETYPES >= id);
	  
	window = PRIV(plugin)->window;	
	doc = document_get_current (window);	
	document_set_filetype_by_id (doc, id);
}

G_MODULE_EXPORT gint
cssed_plugin_get_eol_mode (CssedPlugin* plugin)
{
	CssedWindow* window;
	CssedDoc* doc;

	g_return_val_if_fail( plugin != NULL, -1 );

	window = PRIV(plugin)->window;	
	doc = document_get_current (window);	
	return document_get_eol_mode (doc);	
}

G_MODULE_EXPORT void
cssed_plugin_set_eol_mode (CssedPlugin* plugin, gint eol_mode)
{
	CssedWindow* window;
	CssedDoc* doc;

	g_return_if_fail (plugin != NULL);
	g_return_if_fail (EOL_MODE_CRLF <= eol_mode && eol_mode <= EOL_MODE_LF);
	  
	window = PRIV(plugin)->window;	
	doc = document_get_current (window);	
	document_set_eol_mode (doc, eol_mode);
}
G_MODULE_EXPORT void 
cssed_plugin_convert_eols (CssedPlugin* plugin, gint eol_mode)
{
	CssedWindow* window;
	CssedDoc* doc;

	g_return_if_fail (plugin != NULL);
	g_return_if_fail (EOL_MODE_CRLF <= eol_mode && eol_mode <= EOL_MODE_LF);
	  
	window = PRIV(plugin)->window;	
	doc = document_get_current (window);	
	document_convert_eols (doc, eol_mode);
}

G_MODULE_EXPORT gint
cssed_plugin_get_codepage (CssedPlugin* plugin)
{
	CssedWindow* window;
	CssedDoc* doc;

	g_return_val_if_fail( plugin != NULL, -1 );

	window = PRIV(plugin)->window;	
	doc = document_get_current (window);	
	return document_get_codepage (doc);	
}

G_MODULE_EXPORT void
cssed_plugin_set_codepage (CssedPlugin* plugin, gint codepage)
{
	CssedWindow* window;
	CssedDoc* doc;

	g_return_if_fail (plugin != NULL);
	g_return_if_fail (CSSED_CODEPAGE_DEFAULT >= codepage && CSSED_CODEPAGE_DBCS <= codepage);
	  
	window = PRIV(plugin)->window;	
	doc = document_get_current (window);	
	document_set_codepage (doc, codepage);
}

/* ******************************************************************* */
G_MODULE_EXPORT gboolean
cssed_plugin_get_line_endings_visible (CssedPlugin* plugin)
{
	CssedWindow* window;
	CssedDoc* doc;

	g_return_val_if_fail( plugin != NULL, -1 );

	window = PRIV(plugin)->window;	
	doc = document_get_current (window);	
	return document_get_line_endings_visible (doc);	
}

G_MODULE_EXPORT void
cssed_plugin_set_line_endings_visible (CssedPlugin* plugin, gboolean visible)
{
	CssedWindow* window;
	CssedDoc* doc;

	g_return_if_fail (plugin != NULL);
	  
	window = PRIV(plugin)->window;	
	doc = document_get_current (window);	
	document_set_line_endings_visible (doc, visible);
}

G_MODULE_EXPORT gboolean
cssed_plugin_get_lines_wrapped (CssedPlugin* plugin)
{
	CssedWindow* window;
	CssedDoc* doc;

	g_return_val_if_fail( plugin != NULL, -1 );

	window = PRIV(plugin)->window;	
	doc = document_get_current (window);	
	return document_get_lines_wrapped (doc);	
}

G_MODULE_EXPORT void
cssed_plugin_set_lines_wrapped (CssedPlugin* plugin, gboolean wrapped)
{
	CssedWindow* window;
	CssedDoc* doc;

	g_return_if_fail (plugin != NULL);
	  
	window = PRIV(plugin)->window;	
	doc = document_get_current (window);	
	document_set_lines_wrapped (doc, wrapped);
}



G_MODULE_EXPORT gboolean
cssed_plugin_get_white_spaces_visible (CssedPlugin* plugin)
{
	CssedWindow* window;
	CssedDoc* doc;

	g_return_val_if_fail( plugin != NULL, -1 );

	window = PRIV(plugin)->window;	
	doc = document_get_current (window);	
	return document_get_white_spaces_visible (doc);	
}

G_MODULE_EXPORT void
cssed_plugin_set_white_spaces_visible (CssedPlugin* plugin, gboolean visible)
{
	CssedWindow* window;
	CssedDoc* doc;

	g_return_if_fail (plugin != NULL);
	  
	window = PRIV(plugin)->window;	
	doc = document_get_current (window);	
	document_set_white_spaces_visible (doc, visible);
}


G_MODULE_EXPORT GtkWidget* 
cssed_plugin_get_pop_menu (CssedPlugin* plugin)
{
	CssedWindow* window;
	
	g_return_val_if_fail (plugin != NULL, NULL);
	window = PRIV(plugin)->window;
	return cssed_window_get_pop_menu(window);
}

G_MODULE_EXPORT gpointer // to be wrapped by the plugin's interface
cssed_plugin_send_scintilla_message (CssedPlugin* plugin, guint iMessage, gulong wParam, glong lParam)
{
	CssedWindow* window;
	CssedDoc* doc;

	g_return_val_if_fail (plugin != NULL, NULL);	  
	window = PRIV(plugin)->window;	
	doc = document_get_current (window);
	return document_send_scintilla_message (doc, iMessage, wParam, lParam);
}
