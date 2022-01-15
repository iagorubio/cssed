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

#include <gmodule.h>

/** @file plugin.h
 *  @brief Header file for cssed plugin interface. 
 *  Those functions are the plugable interface to extend cssed.
 */

/** @struct _CssedPluginPriv.  
 *  @brief This object is opaque and should be ignored.
 */	
typedef struct _CssedPluginPriv CssedPluginPriv;

/** @typedef CssedPlugin  
 *  @brief A data type for ::_CssedPlugin structures.
 */		
typedef struct _CssedPlugin CssedPlugin;

/** @struct _CssedPlugin.  
 *  @brief This object represents a plugin.
 * 	One ::CssedPlugin object's pointer must be returned with the function   
 *  ::CssedPlugin *init_plugin() exported by the module .
 *  <br><br>
 *	You can create this object static or reserve memory for it and then return
 *	it in the <b>init_plugin()</b> function .
 *	<br><br>
 *	@ref hello_world "[view example]"
 */
struct _CssedPlugin
{
	/** The plugin's name. One name to show to users in the plugin's list. */
	gchar *name; 		
	/** Plugin short description. One short description to show to users in the plugin's list.*/	
	gchar *description;						
	/** Plugin load function. It will be called by cssed to initialize the plugin, put all your UI construction and initialization code here. */
	gboolean (*load_plugin) (CssedPlugin*);	
	/** Plugin clean function. It will be called by cssed before unloading the plugin, put all your UI destruction and free all memory here. */
	void (*clean_plugin) (CssedPlugin*); 	
	/** Use this field to store plugin's needed data. You can use this field to pass any data needed by your implementation. */
	gpointer user_data;	
	/** Private data. This field is opaque and should be ignored */	
	CssedPluginPriv* priv;
};
 

/* internall use */
CssedPlugin* cssed_init_plugin (CssedWindow*, gchar*);
void cssed_load_plugin (CssedPlugin* plugin);
void cssed_unload_plugin (CssedPlugin* plugin);
gchar* cssed_plugin_get_filename (CssedPlugin* plugin);// don't free return value
gboolean cssed_is_plugin_file_loaded  (CssedWindow* window, gchar* path );
CssedPlugin* cssed_get_plugin_if_file_loaded  (CssedWindow* window, gchar* path);

/* *************************** plugins API ***************************** */
/* *************************** UI functions **************************** */
G_MODULE_EXPORT void 
cssed_plugin_unload_callback( GtkWidget* widget, gpointer plugin);

G_MODULE_EXPORT gboolean
cssed_add_menu_item_to_plugins_menu (CssedPlugin* plugin, GtkWidget* menu_item);

G_MODULE_EXPORT gboolean
cssed_plugin_add_widget_to_toolbar (CssedPlugin* plugin, 
									 GtkWidget* widget,
									 const char *tooltip_text,
									 const char *tooltip_private_text);
G_MODULE_EXPORT gboolean
cssed_plugin_add_page_with_widget_to_footer (CssedPlugin* plugin,
											GtkWidget* widget,
											gchar* label_text);
G_MODULE_EXPORT gboolean
cssed_plugin_remove_page_with_widget_in_footer (CssedPlugin* plugin,
											GtkWidget* widget);
G_MODULE_EXPORT gboolean
cssed_plugin_add_page_with_widget_to_sidebar (CssedPlugin* plugin,
											GtkWidget* widget,
											gchar* label_text);
G_MODULE_EXPORT gboolean
cssed_plugin_remove_page_with_widget_in_sidebar (CssedPlugin* plugin,
											GtkWidget* widget);
G_MODULE_EXPORT gboolean
cssed_plugin_select_page_with_widget_in_sidebar (CssedPlugin* plugin,
											GtkWidget* widget);
G_MODULE_EXPORT gboolean
cssed_plugin_select_page_with_widget_in_footer (CssedPlugin* plugin,
											GtkWidget* widget);

/* *************************** Dialog functions ************************ */
G_MODULE_EXPORT void
cssed_plugin_error_message ( gchar* title, gchar* message, ...);

G_MODULE_EXPORT gboolean
cssed_plugin_confirm_dialog ( gchar* title, gchar* message, ... );

/* ************************ document funtions ************************ */
// Not added:
//	* close_doc

G_MODULE_EXPORT void
cssed_plugin_open_file (CssedPlugin* plugin, gchar* filename);
G_MODULE_EXPORT void
cssed_plugin_open_file_with_fileselector (CssedPlugin* plugin);
G_MODULE_EXPORT void 
cssed_plugin_add_text_to_document (CssedPlugin* plugin, gchar* text);
G_MODULE_EXPORT gchar*
cssed_plugin_get_text (CssedPlugin* plugin);
G_MODULE_EXPORT void
cssed_plugin_select_text_range (CssedPlugin* plugin, gint start, gint end);
G_MODULE_EXPORT gchar*
cssed_plugin_get_selected_text (CssedPlugin* plugin);
G_MODULE_EXPORT void
cssed_plugin_clear_selected_text (CssedPlugin* plugin);
G_MODULE_EXPORT void
cssed_plugin_undo_document_action (CssedPlugin* plugin);
G_MODULE_EXPORT void
cssed_plugin_redo_document_action (CssedPlugin* plugin);
G_MODULE_EXPORT gboolean
cssed_plugin_document_can_redo (CssedPlugin* plugin);
G_MODULE_EXPORT gboolean
cssed_plugin_document_can_undo (CssedPlugin* plugin);
G_MODULE_EXPORT void
cssed_plugin_search_prev (CssedPlugin* plugin, gchar* text_to_search);
G_MODULE_EXPORT void
cssed_plugin_search_next (CssedPlugin* plugin, gchar* text_to_search);
// for marker 0 bookmarks
G_MODULE_EXPORT void
cssed_plugin_bookmark_next (CssedPlugin* plugin);
G_MODULE_EXPORT void
cssed_plugin_bookmark_prev (CssedPlugin* plugin);
G_MODULE_EXPORT void
cssed_plugin_set_bookmark_at_line (CssedPlugin* plugin, gboolean set, gint line);
G_MODULE_EXPORT gboolean
cssed_plugin_is_bookmark_set_at_line (CssedPlugin* plugin, gint line);
// for marker 1 arrow (validator arrow)
G_MODULE_EXPORT gboolean
cssed_plugin_marker_next (CssedPlugin* plugin);
G_MODULE_EXPORT gboolean
cssed_plugin_marker_prev (CssedPlugin* plugin);
G_MODULE_EXPORT void
cssed_plugin_set_marker_at_line (CssedPlugin* plugin, gboolean set, gint line);
G_MODULE_EXPORT gboolean
cssed_plugin_is_marker_set_at_line (CssedPlugin* plugin, gint line);
// output
G_MODULE_EXPORT void
cssed_plugin_output_write (CssedPlugin* plugin, gchar *markup, ... );
G_MODULE_EXPORT void
cssed_plugin_output_clear (CssedPlugin* plugin);
// line and position (caret)
G_MODULE_EXPORT gint
cssed_plugin_get_line_from_position (CssedPlugin* plugin, gint pos);
G_MODULE_EXPORT gint
cssed_plugin_get_position_from_line (CssedPlugin* plugin, gint line);
G_MODULE_EXPORT gint
cssed_plugin_get_position_at_line (CssedPlugin* plugin, gint line);

G_MODULE_EXPORT gint
cssed_plugin_get_current_position (CssedPlugin* plugin);
G_MODULE_EXPORT void
cssed_plugin_set_current_position (CssedPlugin* plugin, gint pos);
G_MODULE_EXPORT void
cssed_plugin_set_current_line (CssedPlugin* plugin , gint line);
G_MODULE_EXPORT gint
cssed_plugin_get_current_line (CssedPlugin* plugin);
G_MODULE_EXPORT gint
cssed_plugin_get_line_end_from_position (CssedPlugin* plugin, gint pos);
// cut copy paste
G_MODULE_EXPORT void
cssed_plugin_cut (CssedPlugin* plugin);
G_MODULE_EXPORT void
cssed_plugin_copy (CssedPlugin* plugin);
G_MODULE_EXPORT void
cssed_plugin_paste (CssedPlugin* plugin);
// selection
G_MODULE_EXPORT gint
cssed_plugin_get_selection_end (CssedPlugin* plugin);
G_MODULE_EXPORT gint
cssed_plugin_get_selection_start (CssedPlugin* plugin);
G_MODULE_EXPORT void
cssed_plugin_set_selection_start (CssedPlugin* plugin, gint start);
G_MODULE_EXPORT void
cssed_plugin_set_selection_end (CssedPlugin* plugin, gint end);
G_MODULE_EXPORT void
cssed_plugin_set_selection_range (CssedPlugin* plugin, gint start, gint end);
G_MODULE_EXPORT void
cssed_plugin_replace_sel (CssedPlugin* plugin, gchar *text);
G_MODULE_EXPORT void
cssed_plugin_clear_sel (CssedPlugin* plugin);
//
G_MODULE_EXPORT gint
cssed_plugin_get_length	 (CssedPlugin* plugin);
//
G_MODULE_EXPORT void
cssed_plugin_show_user_list (CssedPlugin* plugin, gchar* list);
// call tips
G_MODULE_EXPORT void
cssed_plugin_show_calltip (CssedPlugin* plugin, gint start, gchar* definition);
G_MODULE_EXPORT void
cssed_plugin_cancel_calltip (CssedPlugin* plugin);
G_MODULE_EXPORT gboolean
cssed_plugin_calltipactive (CssedPlugin* plugin);
G_MODULE_EXPORT gint
cssed_plugin_calltiposstart (CssedPlugin* plugin);
//
G_MODULE_EXPORT void
cssed_plugin_document_grab_focus (CssedPlugin* plugin);
//
G_MODULE_EXPORT void
cssed_plugin_show_autocompletion_list (CssedPlugin* plugin, gint len, gchar* list);
G_MODULE_EXPORT void 
cssed_plugin_autocompletion_cancel (CssedPlugin* plugin);
G_MODULE_EXPORT gboolean
cssed_plugin_is_autocompletion_active (CssedPlugin* plugin);
G_MODULE_EXPORT void 
cssed_plugin_autocompletion_select (CssedPlugin* plugin, gchar* select);
// line related
G_MODULE_EXPORT gchar*
cssed_plugin_get_text_line (CssedPlugin* plugin, gint line);
G_MODULE_EXPORT gint
cssed_plugin_get_line_length (CssedPlugin* plugin, gint line);
G_MODULE_EXPORT gint
cssed_plugin_get_line_count (CssedPlugin* plugin);
// folding
G_MODULE_EXPORT void
cssed_plugin_toggle_fold_at_line (CssedPlugin* plugin, gint line);
G_MODULE_EXPORT gboolean
cssed_plugin_get_line_is_visible (CssedPlugin* plugin, gint line);
G_MODULE_EXPORT void
cssed_plugin_ensure_line_is_visible (CssedPlugin* plugin, gint line);
G_MODULE_EXPORT gint
cssed_plugin_get_fold_level (CssedPlugin* plugin, gint line);
G_MODULE_EXPORT gint
cssed_plugin_get_fold_depth (CssedPlugin* plugin, gint line);
G_MODULE_EXPORT void
cssed_plugin_set_folding_margin_visible (CssedPlugin* plugin, gboolean set);
G_MODULE_EXPORT gboolean
cssed_plugin_get_folding_margin_visible (CssedPlugin* plugin);
G_MODULE_EXPORT gboolean 
cssed_plugin_get_folding_enabled (CssedPlugin* plugin);
G_MODULE_EXPORT void
cssed_plugin_fold_all (CssedPlugin* plugin);
G_MODULE_EXPORT void
cssed_plugin_unfold_all (CssedPlugin* plugin);
//tabs
G_MODULE_EXPORT void
cssed_plugin_set_use_tabs (CssedPlugin* plugin, gboolean set);
G_MODULE_EXPORT gboolean
cssed_plugin_get_use_tabs (CssedPlugin* plugin);
G_MODULE_EXPORT void
cssed_plugin_set_tabs_width (CssedPlugin* plugin, gint width);
G_MODULE_EXPORT gint
cssed_plugin_get_tabs_width (CssedPlugin* plugin);
// validator arrow marker
G_MODULE_EXPORT void
cssed_plugin_set_arrow_marker_at_line (CssedPlugin* plugin, gint line);
G_MODULE_EXPORT void
cssed_plugin_clear_arrow_marker (CssedPlugin* plugin);
// zooming
G_MODULE_EXPORT void
cssed_plugin_zoom_in (CssedPlugin* plugin);
G_MODULE_EXPORT void
cssed_plugin_zoom_out (CssedPlugin* plugin);
G_MODULE_EXPORT void
cssed_plugin_zoom_off (CssedPlugin* plugin);
//
G_MODULE_EXPORT gboolean
cssed_plugin_get_modified (CssedPlugin* plugin);
G_MODULE_EXPORT gboolean
cssed_plugin_is_file_opened (CssedPlugin* plugin, gchar* filename);
/* ** */
G_MODULE_EXPORT gboolean
cssed_plugin_save_document (CssedPlugin* plugin);
G_MODULE_EXPORT gboolean
cssed_plugin_save_document_as (CssedPlugin* plugin, gchar* filename);
G_MODULE_EXPORT gint
cssed_plugin_get_document_index (CssedPlugin* plugin);
G_MODULE_EXPORT void
cssed_plugin_set_document_by_index (CssedPlugin* plugin, gint index);
G_MODULE_EXPORT gint
cssed_plugin_get_num_docs (CssedPlugin* plugin);
G_MODULE_EXPORT gboolean
cssed_plugin_new_document (CssedPlugin* plugin);
G_MODULE_EXPORT gchar*
cssed_plugin_document_get_filename (CssedPlugin* plugin);
G_MODULE_EXPORT gint
cssed_plugin_get_style_at(CssedPlugin* plugin, gint pos);
//
G_MODULE_EXPORT gchar*
cssed_plugin_prompt_for_file_to_open(CssedPlugin* plugin, gchar* dialog_title, gchar* basedir);
G_MODULE_EXPORT gchar*
cssed_plugin_prompt_for_file_to_save(CssedPlugin* plugin, gchar* dialog_title, gchar* basedir);
G_MODULE_EXPORT gchar*
cssed_plugin_prompt_for_directory_to_open(CssedPlugin* plugin, gchar* dialog_title, gchar* basedir);
G_MODULE_EXPORT gchar*
cssed_plugin_prompt_for_directory_to_create(CssedPlugin* plugin, gchar* dialog_title, gchar* basedir);
/* *************************** css dialogs ***************************** */
G_MODULE_EXPORT void 
cssed_plugin_css_dialog_insert (CssedPlugin* plugin, gchar* key, gpointer klass);
G_MODULE_EXPORT void 
cssed_plugin_css_dialog_remove_by_keyword (CssedPlugin* plugin, gchar* key);
G_MODULE_EXPORT gpointer 
cssed_plugin_css_dialog_lookup_by_keyword (CssedPlugin* plugin, gchar* key);
G_MODULE_EXPORT gboolean 
cssed_plugin_css_dialog_keyword_in_use (CssedPlugin* plugin, gchar* key);

/* *************************** main window object ******************** */
G_MODULE_EXPORT gpointer
cssed_plugin_get_window (CssedPlugin* plugin);

/* ******************************************************************* */
G_MODULE_EXPORT gint
cssed_plugin_get_filetype_id (CssedPlugin* plugin);
G_MODULE_EXPORT void
cssed_plugin_set_filetype_by_id (CssedPlugin* plugin, gint id);

G_MODULE_EXPORT gint
cssed_plugin_get_eol_mode (CssedPlugin* plugin);
G_MODULE_EXPORT void
cssed_plugin_set_eol_mode (CssedPlugin* plugin, gint eol_mode);
G_MODULE_EXPORT void 
cssed_plugin_convert_eols (CssedPlugin* plugin, gint eolmode);

G_MODULE_EXPORT gint
cssed_plugin_get_codepage (CssedPlugin* plugin);
G_MODULE_EXPORT void
cssed_plugin_set_codepage (CssedPlugin* plugin, gint codepage);


/* ******************************************************************* */
G_MODULE_EXPORT gboolean
cssed_plugin_get_line_endings_visible (CssedPlugin* plugin);
G_MODULE_EXPORT void
cssed_plugin_set_line_endings_visible (CssedPlugin* plugin, gboolean visible);
G_MODULE_EXPORT gboolean
cssed_plugin_get_lines_wrapped (CssedPlugin* plugin);
G_MODULE_EXPORT void
cssed_plugin_set_lines_wrapped (CssedPlugin* plugin, gboolean wrapped);
G_MODULE_EXPORT gboolean
cssed_plugin_get_white_spaces_visible (CssedPlugin* plugin);
G_MODULE_EXPORT void
cssed_plugin_set_white_spaces_visible (CssedPlugin* plugin, gboolean visible);


G_MODULE_EXPORT GtkWidget* 
cssed_plugin_get_pop_menu(CssedPlugin* plugin);

G_MODULE_EXPORT gpointer // to be wrapped by the plugin's interface
cssed_plugin_send_scintilla_message ( CssedPlugin* plugin, guint iMessage, gulong wParam, glong lParam );
