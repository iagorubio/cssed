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
 * 
 *  - SPECIAL EXCEPTION -
 * 
 *  Linking cssed statically or dynamically with other modules is making a
 *  combined work based on cssed.  Thus, the terms and conditions of the GNU
 *  General Public License cover the whole combination.
 * 
 *  As a special exception, the copyright holders of cssed give you
 *  permission to link cssed with independent modules that communicate with
 *  cssed solely through this file's plugable interface, regardless of the
 *  license terms of these independent modules, and to copy and distribute the
 *  resulting combined work under terms of your choice, provided that
 *  every copy of the combined work is accompanied by a complete copy of
 *  the source code of cssed (the version of cssed used to produce the
 *  combined work), being distributed under the terms of the GNU General
 *  Public License plus this exception.  An independent module is a module
 *  which is not derived from or based on cssed.
 * 
 *  Note that people who make modified versions of cssed are NOT obligated
 *  to grant this special exception for their modified versions; it is
 *  their choice whether to do so.  The GNU General Public License gives
 *  permission to release a modified version without this exception; this
 *  exception also makes it possible to release a modified version which
 *  carries forward this exception.
 */
 
/** @file plugin.h
 *  @brief Header file for cssed plugin interface. 
 *  Those functions are the plugable interface to extend cssed.
 */

typedef enum {
        // those match with GFileError
        CSSED_FILE_ERROR_EXIST,
        CSSED_FILE_ERROR_ISDIR,
        CSSED_FILE_ERROR_ACCES,
        CSSED_FILE_ERROR_NAMETOOLONG,
        CSSED_FILE_ERROR_NOENT,
        CSSED_FILE_ERROR_NOTDIR,
        CSSED_FILE_ERROR_NXIO,
        CSSED_FILE_ERROR_NODEV,
        CSSED_FILE_ERROR_ROFS,
        CSSED_FILE_ERROR_TXTBSY,
        CSSED_FILE_ERROR_FAULT,
        CSSED_FILE_ERROR_LOOP,
        CSSED_FILE_ERROR_NOSPC,
        CSSED_FILE_ERROR_NOMEM,
        CSSED_FILE_ERROR_MFILE,
        CSSED_FILE_ERROR_NFILE,
        CSSED_FILE_ERROR_BADF,
        CSSED_FILE_ERROR_INVAL,
        CSSED_FILE_ERROR_PIPE,
        CSSED_FILE_ERROR_AGAIN,
        CSSED_FILE_ERROR_INTR,
        CSSED_FILE_ERROR_IO,
        CSSED_FILE_ERROR_PERM,
        CSSED_FILE_ERROR_FAILED,
        // this is not an error code it's just
        // to know were the GFIleError matching ends
        CSSED_FILE_ERROR_LAST,
        // specific cssed error codes
        CSSED_ERROR_FILE_IS_OPENED, // the file is already opened
        CSSED_ERROR_FILE_NOT_REGULAR, // the file is not a regular file, may be a device or whatever
        CSSED_ERROR_DOC_IS_NULL // a NULL document have been passed or returned
} CssedError;

/** @enum CssedEolMode cssed's end of line modes */
typedef enum {
        /** Windows end of line - Carriage return / Line feed */
        EOL_MODE_CRLF,
        /** Mac end of line - Carriage return */
        EOL_MODE_CR,
        /** Unix end of line - Line feed */
        EOL_MODE_LF
} CssedEolMode;

/** @enum CssedCodePage cssed's code pages */
typedef enum {
        /** Default code page */
        CSSED_CODEPAGE_DEFAULT,
        /** UTF8 code page */
        CSSED_CODEPAGE_UTF8,
        /** DBCS code page (Thai, Hangul) */
        CSSED_CODEPAGE_DBCS
} CssedCodePage;

#define CSSED_FILETYPE_DEFAULT CSSED_FILETYPE_TEXT

typedef enum {
        CSSED_FILETYPE_UNKNOW,          // internall use. Never use it. 
        CSSED_FILETYPE_TEXT,            // The default file type, plain text
        CSSED_FILETYPE_BYNARY,          // not used ,but will be link insertion
        CSSED_FILETYPE_CSS,                     // core language
        CSSED_FILETYPE_HTML,            // this is not just HTML but ASP and PHP
        CSSED_FILETYPE_PHP,                     // This is standalon PHP not client side PHP (for sh-like scripts)
        CSSED_FILETYPE_XML,                     // XML and SVG
        CSSED_FILETYPE_SH,                      // shell and bash scripts
        CSSED_FILETYPE_C,                       // not just C but CPP
        CSSED_FILETYPE_APACHE_CONF,     // apache conf files
        CSSED_FILETYPE_LISP,            // mostly for emacs.el files (not ready)
        CSSED_FILETYPE_PERL,            // perl scripts
        CSSED_FILETYPE_PYTHON,          // python
        CSSED_FILETYPE_SQL,                     // SQL (Ansi) (not ready)
        CSSED_FILETYPE_DIFF,            // diff and patch files
        CSSED_FILETYPE_JAVA,            // java
        CSSED_FILETYPE_JS,                      // javascript
        CSSED_FILETYPE_MAKE,            // Makefiles - I use make for almost everything :)
        // Those are file types supported by scintilla, so they have it's place here
        CSSED_FILETYPE_APDL,
        CSSED_FILETYPE_AU3,
        CSSED_FILETYPE_AVE,
        CSSED_FILETYPE_ADA,
        CSSED_FILETYPE_ASM,
        CSSED_FILETYPE_ASN1,
        CSSED_FILETYPE_BAAN,
        CSSED_FILETYPE_BASIC,
        CSSED_FILETYPE_BULLANT,
        CSSED_FILETYPE_CLW,
        CSSED_FILETYPE_CAML,
        CSSED_FILETYPE_CRONTAB,
        CSSED_FILETYPE_ESCRIPT,
        CSSED_FILETYPE_EIFFEL,
        CSSED_FILETYPE_ERLANG,
        CSSED_FILETYPE_FORTH,
        CSSED_FILETYPE_FORTRAN,
        CSSED_FILETYPE_GUI4CLI,
        CSSED_FILETYPE_KIX,
        CSSED_FILETYPE_LOUT,
        CSSED_FILETYPE_LUA,
        CSSED_FILETYPE_MMIXAL,
        CSSED_FILETYPE_MPT,
        CSSED_FILETYPE_MSSQL,
        CSSED_FILETYPE_MATLAB,
        CSSED_FILETYPE_METAPOST,
        CSSED_FILETYPE_NSIS,
        CSSED_FILETYPE_PB,
        CSSED_FILETYPE_POV,
        CSSED_FILETYPE_PS,
        CSSED_FILETYPE_PASCAL,
        CSSED_FILETYPE_RUBY,
        CSSED_FILETYPE_SCRIPTOL,
        CSSED_FILETYPE_SPECMAN,
        CSSED_FILETYPE_TEX,
        CSSED_FILETYPE_VB,
        CSSED_FILETYPE_VHDL,
        CSSED_FILETYPE_VERILOG,
        CSSED_FILETYPE_YAML,
        // if you miss something here, just request  for inclusion
        CSSED_FILETYPE_USR,                     // user defined file types
        CSSED_FILETYPE_USR1,
        CSSED_FILETYPE_USR2,
        CSSED_FILETYPE_USR3,
        CSSED_FILETYPE_USR4,
        CSSED_FILETYPE_USR5,
        CSSED_FILETYPE_USR6,
        CSSED_FILETYPE_USR7,
        CSSED_FILETYPE_USR8,
        CSSED_FILETYPE_USR9
} CssedFileTypeId;



/** @struct _CssedPluginPriv.  
 *  @brief This object is opaque and should be ignored.
 */	
typedef struct _CssedPluginPriv CssedPluginPriv;

/** @typedef CssedPlugin  
 *  @brief A data type for ::_CssedPlugin structures.
 */		
typedef struct _CssedPlugin CssedPlugin;

/** Returns the character position at the start of a line.  
 *	It returns the character position as an integer, in the first character of 
 *	the line number passed.
 *	@param plugin - The plugin object.
 *  @param line - The line number to get the character position.
 *	@return an integer with the character position.
 *  @deprecated This function is deprecated, use ::cssed_plugin_get_position_from_line instead.
 */
G_MODULE_IMPORT gint
cssed_plugin_get_position_at_line ( CssedPlugin* plugin, gint line );

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


/** To be used as a widget callback.  
 *	This function lets you unload the plugin from a signal, using it as the signal's callback.
 *	@param widget - The widget that caused the signal.
 *	@param plugin - A pointer to the plugin object.
 */
G_MODULE_IMPORT void 
cssed_plugin_unload_callback( GtkWidget* widget, gpointer plugin );

/** Adds a menu item to the plugin's menu.    
 *	This function lets you add an item to the plugin's menu, you must set the callbacks.
 *	yourself.
 *	@param plugin - A pointer to the plugin object.
 *  @param menu_item - The menu item widget to add.
 */ 
G_MODULE_IMPORT gboolean
cssed_add_menu_item_to_plugins_menu( CssedPlugin* plugin, GtkWidget* menu_item);

/** Adds a menu item to the toolbar.  
 *	This function lets you add an item to the plugin's menu, you must set the callbacks.
 *	yourself.
 *	@param plugin - A pointer to the plugin object.
 *  @param widget - The widget to add.
 *	@param tooltip_text - The text shown to the user in a tool tip box.
 *	@param tooltip_private_text - A biggest description to show to users.
 */
G_MODULE_IMPORT gboolean
cssed_plugin_add_widget_to_toolbar( CssedPlugin* plugin, 
							 GtkWidget* widget,
							 const char *tooltip_text,
							 const char *tooltip_private_text );
/** Adds a widget to the application's footer panel.  
 *	This function lets you add an item to the footer's panel, you must set the callbacks.
 *	yourself.
 *	@param plugin - A pointer to the plugin object.
 *  @param widget - The menu item widget to add.
 *  @param label_text - A text to be shown in the notebook's tab label. 
 */							 
G_MODULE_IMPORT gboolean
cssed_plugin_add_page_with_widget_to_footer( CssedPlugin* plugin,
											GtkWidget* widget,
											gchar* label_text );
/** Removes a widget from the application's footer panel. 
 *	This function lets you remove a widget from the footer's panel
 *	@param plugin - A pointer to the plugin object.
 *  @param widget - The widget to be removed.
 */
G_MODULE_IMPORT gboolean
cssed_plugin_remove_page_with_widget_in_footer( CssedPlugin* plugin,
											GtkWidget* widget );

/** Adds a widget to the sidebar.  
 *	This function lets you add a widget to the sidebar, you must set the callbacks.
 *	yourself.
 *	@param plugin - A pointer to the plugin object.
 *  @param widget - The widget to add.
 *  @param label_text - A text to be shown in the notebook's tab label. 
 */		
G_MODULE_IMPORT gboolean
cssed_plugin_add_page_with_widget_to_sidebar( CssedPlugin* plugin,
											GtkWidget* widget,
											gchar* label_text );

/** Removes a widget from the application's side bar. 
 *	This function lets you remove a widget from the side bar.
 *	@param plugin - A pointer to the plugin object.
 *  @param widget - The widget to be removed.
 */										
G_MODULE_IMPORT gboolean
cssed_plugin_remove_page_with_widget_in_sidebar( CssedPlugin* plugin,
											GtkWidget* widget );

/** Selects the page containing a widget. 
 *	This function selects the page containing the widget passed in the sidebar notebook.
 *	@param plugin - A pointer to the plugin object.
 *  @param widget - The widget to be selected.
 */	
G_MODULE_IMPORT gboolean
cssed_plugin_select_page_with_widget_in_sidebar( CssedPlugin* plugin,
											GtkWidget* widget );

/** Selects the page containing a widget. 
 *	This function selects the page containing the widget passed in the footer notebook.
 *	@param plugin - A pointer to the plugin object.
 *  @param widget - The widget to be selected.
 */	
G_MODULE_IMPORT gboolean
cssed_plugin_select_page_with_widget_in_footer( CssedPlugin* plugin,
											GtkWidget* widget );

/** Shows an error message dialog.  
 *	It prompts a dialog to show an error to user, It may use
 *	printf style format on the message argument and a variable
 *	argument list matching this format. See the documentation
 *	on printf in your system.
 *  @param title - Brief error message. 
 *	@param message - Explained error message.
 */ 
G_MODULE_IMPORT void
cssed_plugin_error_message ( gchar* title, gchar* message, ...);

/** Shows a confirmation dialog.  
 *	It prompts a confirmation dialog asking the user to make a choice. It may use
 *	printf style format on the message argument and a variable
 *	argument list matching this format. See the documentation
 *	on printf on your system.
 *  @param title - Brief message.
 *	@param message - Explained message.
 *	@return a boolean with the user's choice. TRUE if accepts the option
 *	explained in the message FALSE otherwise.
 */
G_MODULE_IMPORT gboolean
cssed_plugin_confirm_dialog ( gchar* title, gchar* message, ... );

/** Opens a file.  
 *	This function opens the file passed as "filename" in cssed.
 *	@param plugin - The plugin object.
 *  @param filename - Full path to the file to be opened.
 */
G_MODULE_IMPORT void
cssed_plugin_open_file( CssedPlugin* plugin, gchar* filename );

/** Shows a file selector dialog.  
 *	It prompts a dialog to let the user choose a file to be opened.
 *	@param plugin - The plugin object.
 */
G_MODULE_IMPORT void
cssed_plugin_open_file_with_fileselector( CssedPlugin* plugin );

/** Adds the text passed to the document.  
 *	It adds the text passed to the document at the anchor position. The whole
 *	operation will be flagged as a single undo action.
 *	@param plugin - The plugin object.
 *  @param text - The text to add to the document.
 */
G_MODULE_IMPORT void 
cssed_plugin_add_text_to_document( CssedPlugin* plugin, gchar* text );

/** Gets a null terminated string with the current document's text.  
 *	You must free the returned valued if not NULL, with g_free.
 *	@param plugin - The plugin object.
 *  @return - A pointer to the text.
 *	@warning You must free the returned value with g_free when no longer needed.
 */
G_MODULE_IMPORT gchar*
cssed_plugin_get_text( CssedPlugin* plugin );

/** Selects a text range.  
 *	The selection is done from character position "start", to character 
 *	position "end".
 *	@param plugin - The plugin object.
 *  @param start - An integer containing the start of selection.
 *	@param end - An integer containing the end of selection.
 *  @deprecated This function is deprecated and should not be used in newly written code, use ::cssed_plugin_set_selection_range instead.
 */
G_MODULE_IMPORT void
cssed_plugin_select_text_range( CssedPlugin* plugin, gint start, gint end );

/** Gets a null terminated string with selected text.  
 *	You must free the returned valued if not NULL, with g_free.
 *	@param plugin - The plugin object.
 *  @return - A pointer to the text or NULL if nothing is selected.
 *	@warning You must free the returned value with g_free when no longer needed.
 */
G_MODULE_IMPORT gchar*
cssed_plugin_get_selected_text(  CssedPlugin* plugin  );

/** Deletes the text selected.  
 *	If there's no selection this function has no effect.
 *	@param plugin - The plugin object.
 */
G_MODULE_IMPORT void
cssed_plugin_clear_selected_text( CssedPlugin* plugin );

/** Undoes one previous action.  
 *	It undoes the last text or character insertion in the document.
 *	@param plugin - The plugin object.
 */
G_MODULE_IMPORT void
cssed_plugin_undo_document_action ( CssedPlugin* plugin );

/** Redoes one previous action.  
 *	It redoes the last action.
 *	@param plugin - The plugin object.
 */
G_MODULE_IMPORT void
cssed_plugin_redo_document_action ( CssedPlugin* plugin );

/** Gets if a document have actions in the redo queue.  
 *	This function informs if the current document have actions in the  redo queue.
 *	@param plugin - The plugin object.
 *	@return A boolean indicating if there are actions in the redo queue.
 *  @since 0.2 (cssed-0.3.1)
 */
G_MODULE_IMPORT gboolean
cssed_plugin_document_can_redo ( CssedPlugin* plugin );

/** Gets if a document have actions in the undo queue.  
 *	This function informs if the current document have actions in the  undo queue.
 *	@param plugin - The plugin object.
 *	@return A boolean indicating if there are actions in the undo queue.
 *  @since 0.2 (cssed-0.3.1)
 */
G_MODULE_IMPORT gboolean
cssed_plugin_document_can_undo ( CssedPlugin* plugin );

/** Backwards searches and selects some text. 
 *	It searches and selects the text passed second parameter going from caret position 
 *	to the document's start.
 *	@param plugin - The plugin object. 
 *  @param text_to_search - The text to search for.
 *  @since 0.2 (cssed-0.3.1)
 */
G_MODULE_IMPORT void
cssed_plugin_search_prev( CssedPlugin* plugin, gchar* text_to_search );

/** Searches and selects some text.
 *	It searchs and selects the text passed as second parameter, searching from caret position 
 *	to the document's end.
 *	@param plugin - The plugin object. 
 *  @param text_to_search - The text to search for.
 */
G_MODULE_IMPORT void
cssed_plugin_search_next( CssedPlugin* plugin, gchar* text_to_search );

////////////////////////////////////////////////////////////////////////////////////////////
/** Goes to next bookmark if any.
 *	Moves the caret to the next bookmark in current document. Or shows a message if no
 *	bookmarks exists.
 *	@param plugin - The plugin object. 
 */
G_MODULE_IMPORT void
cssed_plugin_bookmark_next ( CssedPlugin* plugin );

/** Goes to previous bookmark if any.
 *	Moves the caret to the provious bookmark in current document. Or shows a message if no
 *	bookmarks exists.
 *	@param plugin - The plugin object. 
 */
G_MODULE_IMPORT void
cssed_plugin_bookmark_prev ( CssedPlugin* plugin );


/** Set or unset a bookmark.
 *	It sets or unset a bookmark at a given line. It's recommended not to deal directly
 *	with bookmarks but to use markers instead. Bookmarks should be managed by the
 *	user and to change them - but in really speciallized cases - may be bothering.
 *	@param plugin - The plugin object. 
 *  @param set - TRUE to set the bookmar, FALSE to unset it.
 *  @param line - The 0 based index of the line where the bookmark is set.
 */
G_MODULE_IMPORT  void
cssed_plugin_set_bookmark_at_line  ( CssedPlugin* plugin, gboolean set, gint line );

/** Queries if a bookmark is set on a given line.
 *	It queries the bookmark state at a given line. It's recommended not to deal directly
 *	with bookmarks but to use markers instead. Bookmarks should be managed by the
 *	user and to change them - but in really speciallized cases - may be bothering.
 *	@param plugin - The plugin object. 
 *  @param line - The 0 based index of the line to query.
 *	@return a boolean idicating if the bookmar is set.
 */
G_MODULE_IMPORT gboolean
cssed_plugin_is_bookmark_set_at_line ( CssedPlugin* plugin, gint line );

// for marker 1 arrow (validator arrow)
/** Goes to next marker if any.
 *	Moves the caret to the next marker in current document. Or shows a message if no
 *	marker exists.
 *	@param plugin - The plugin object. 
 */
G_MODULE_IMPORT gboolean
cssed_plugin_marker_next ( CssedPlugin* plugin );
/** Goes to previous marker if any.
 *	Moves the caret to the provious marker in current document. Or shows a message if no
 *	marker exists.
 *	@param plugin - The plugin object. 
 */
G_MODULE_IMPORT gboolean
cssed_plugin_marker_prev ( CssedPlugin* plugin );

/** Set or unset a marker.
 *	It sets or unset a marker at a given line. 
 *	@param plugin - The plugin object. 
 *  @param set - TRUE to set the bookmar, FALSE to unset it.
 *  @param line - The 0 based index of the line where the bookmark is set.
 */
G_MODULE_IMPORT void
cssed_plugin_set_marker_at_line  ( CssedPlugin* plugin, gboolean set, gint line );

/** Queries if a marker is set on a given line.
 *	It queries the marker state at a given line.
 *	@param plugin - The plugin object. 
 *  @param line - The 0 based index of the line to query.
 *	@return a boolean idicating if the bookmar is set.
 */
G_MODULE_IMPORT gboolean
cssed_plugin_is_marker_set_at_line ( CssedPlugin* plugin, gint line );

/** Send text to cssed's output.
 *	It sends text to cssed's output window, the text will be taken as pango markup
 *	so if you're sending an external command output to it, you should escape the text
 *	to be valid pango markup. It aceepts printf style format. 
 *	@param plugin - The plugin object. 
 *  @param markup - The pango markup to show in the output window.
 */
G_MODULE_IMPORT void
cssed_plugin_output_write ( CssedPlugin* plugin, gchar *markup, ... );

/** Clears cssed's output.
 *	It deletes all text on cssed's output window,.
 *	@param plugin - The plugin object. 
 */
G_MODULE_IMPORT void
cssed_plugin_output_clear ( CssedPlugin* plugin );

/** Returns the line number.
 *	It returns the line number - 0 based index - at a given character position.
 *	@param plugin - The plugin object. 
 *  @param pos - The  character position.
 *	@return an integer with the line's number.
 */
G_MODULE_IMPORT gint
cssed_plugin_get_line_from_position ( CssedPlugin* plugin, gint pos );

/** Returns character position.
 *	It returns the character position - 0 based index - at a given line number.
 *	@param plugin - The plugin object. 
 *  @param line - The 0 based index of the line to query.
 *	@return an integer with the character position
 */
G_MODULE_IMPORT gint
cssed_plugin_get_position_from_line ( CssedPlugin* plugin, gint line );

/** Returns character position at the caret.
 *	It returns the character position - 0 based index - at the text insertion point.
 *	@param plugin - The plugin object. 
 *	@return an integer with the caret position
 */
G_MODULE_IMPORT gint
cssed_plugin_get_current_position ( CssedPlugin* plugin );

/** Sets the current position .
 *	It moves the text insertion point to any given position.
 *	@param plugin - The plugin object. 
 *  @param pos - The position where to move the caret.
 */
G_MODULE_IMPORT void
cssed_plugin_set_current_position ( CssedPlugin* plugin, gint pos );

/** Sets the current line.
 *	It moves the text insertion point to the given line.
 *	@param plugin - The plugin object. 
 *  @param line - The line where to move the caret.
 */
G_MODULE_IMPORT void
cssed_plugin_set_current_line ( CssedPlugin* plugin , gint line );

/** Returns the current line number.
 *	It returns the line number where the caret is.
 *	@param plugin - The plugin object. 
 *	@return an integer with the caret line.
 */
G_MODULE_IMPORT gint
cssed_plugin_get_current_line ( CssedPlugin* plugin );

/** Returns the character position at the end of the line. 
 *	It returns the character position at the end of the line where 
 *	the character shich position is passed as argument is.
 *	@param plugin - The plugin object. 
 *  @param pos - The position from where to search the end of line. 
 *	@return an integer with the character position at the end of the line.
 */
G_MODULE_IMPORT gint
cssed_plugin_get_line_end_from_position ( CssedPlugin* plugin, gint pos);


/** Cuts the selected text to the clipboard. 
 *	It copies the selected text to the clipboard, and delete the selection. 
 *	The word "clipboard" will have a different meanings depending on the platform
 *	you're using.
 *	@param plugin - The plugin object. 
 */
G_MODULE_IMPORT void
cssed_plugin_cut ( CssedPlugin* plugin );

/** Copy the selected text to the clipboard. 
 *	It copies the selected text to the clipboard. 
 *	The word "clipboard" will have a different meanings depending on the platform
 *	you're using.
 *	@param plugin - The plugin object. 
 */
G_MODULE_IMPORT void
cssed_plugin_copy ( CssedPlugin* plugin );

/** Paste the clipboard contents. 
 *	It writes the text on the clipboard at text insertion point, deleting any selection. 
 *	The word "clipboard" will have a different meanings depending on the platform
 *	you're using.
 *	@param plugin - The plugin object. 
 */
G_MODULE_IMPORT void
cssed_plugin_paste ( CssedPlugin* plugin );

/** Returns the selection's end. 
 *	It returns the end of the selected text. If there are no text selected
 *	this function returns de same value as ::cssed_plugin_get_selection_start
 *	@param plugin - The plugin object. 
 *	@return an integer with the character position at selection's end.
 */
G_MODULE_IMPORT gint
cssed_plugin_get_selection_end ( CssedPlugin* plugin );

/** Returns the selection's start. 
 *	It returns the start of the selected text. If there are no text selected
 *	this function returns de same value as ::cssed_plugin_get_selection_end
 *	@param plugin - The plugin object. 
 *	@return an integer with the character position at selection's start.
 */
G_MODULE_IMPORT gint
cssed_plugin_get_selection_start ( CssedPlugin* plugin );

/** Sets the selection's start. 
 *	It sets the start of the selected text. you need to call ::cssed_plugin_set_selection_end
 *	to select a range of text.
 *	@param plugin - The plugin object. 
 *	@param start - The selection's start.
 */
G_MODULE_IMPORT void
cssed_plugin_set_selection_start ( CssedPlugin* plugin, gint start);

/** Sets the selection's end. 
 *	It sets the end of the selected text. It'll select the text from current 
 *	selection's start or caret position if a call to ::cssed_plugin_set_selection_start
 *	was not made.
 *	@param plugin - The plugin object. 
 *	@param end - The selection's end.
 */
G_MODULE_IMPORT void
cssed_plugin_set_selection_end ( CssedPlugin* plugin, gint end );

/** Selects a range of text. 
 *	It selects a range of text from start to end. Is as to call ::cssed_plugin_set_selection_start
 *	::cssed_plugin_set_selection_end in one single call.
 *	@param plugin - The plugin object. 
 *	@param start - The selection's end.
 *	@param end - The selection's end.
 */
G_MODULE_IMPORT void
cssed_plugin_set_selection_range ( CssedPlugin* plugin, gint start, gint end );

/** Replaces the selected text. 
 *	It replaces the selected text on current document, with the text passed as argument.
 *	@param plugin - The plugin object. 
 *	@param text - The replacement text.
 */
G_MODULE_IMPORT void
cssed_plugin_replace_sel  ( CssedPlugin* plugin, gchar *text );

/** Deletes the selected text. 
 *	It deletes the selected text on current document.
 *	@param plugin - The plugin object. 
 */
G_MODULE_IMPORT void
cssed_plugin_clear_sel ( CssedPlugin* plugin );

/** It gets the document's text length. 
 *	It gets the length of current document, without the leading NULL character. You should allocate
 *	cssed_plugin_get_length + 1 bytes to store all document's text.
 *	@param plugin - The plugin object. 
 */
G_MODULE_IMPORT gint
cssed_plugin_get_length	( CssedPlugin* plugin );

/** It shows a user list. 
 *	It shows a pop window with the elements passed as argument shown as a list
 *	to let the user to choose among one of them.
 *	@param plugin - The plugin object. 
 *	@param list - An string containing the space separated elements of the list. 
 */
G_MODULE_IMPORT void
cssed_plugin_show_user_list ( CssedPlugin* plugin, gchar* list );

/** It shows an auto-completion list. 
 *	It shows a pop window with the elements passed as argument shown as a list
 *	to let the user to choose among one of them. As the user types, the list is
 *	scrolled to show the best match if any.
 *	@param plugin - The plugin object. 
 *	@param len - the length of the text entered by the user - to auto complete it.
 *	@param list - An string containing the space separated elements of the list. 
 */
G_MODULE_IMPORT void
cssed_plugin_show_autocompletion_list (CssedPlugin* plugin, gint len, gchar* list);

/** It hides any user list or auto-completion list being shown. 
 *	It hides any user list as well as any auto-completion list being shown. It does nothing
 *	if no user list nor auto-completion lists are running.
 *	@param plugin - The plugin object. 
 */
G_MODULE_IMPORT void 
cssed_plugin_autocompletion_cancel (CssedPlugin* plugin);

/** It tells you if there's any list being shown. 
 *	It returns a boolean indicating if there's any user or auto-completionm list active.
 *	@param plugin - The plugin object. 
 *	@return A boolean idicating if there's a list active.
 */
G_MODULE_IMPORT gboolean
cssed_plugin_is_autocompletion_active (CssedPlugin* plugin);

/** It selects an item on a list if active. 
 *	It selects the an item matching with the text passed as parameter
 *	if it exists.
 *	@param plugin - The plugin object.
 *	@param select - The text to be selected on the list. 
 */
G_MODULE_IMPORT void 
cssed_plugin_autocompletion_select (CssedPlugin* plugin, gchar* select);

/** Shows a call tip. 
 *	Shows an small windown with information as function signatures.
 *	@param plugin - The plugin object.
 *	@param start - The character position where to show the call tip. 
  *	@param definition - The text to be shown. 
 */
G_MODULE_IMPORT void
cssed_plugin_show_calltip ( CssedPlugin* plugin, gint start, gchar* definition );

/** Cancels a call tip. 
 *	If a call tip is being show it hides it..
 *	@param plugin - The plugin object.
 */
G_MODULE_IMPORT void
cssed_plugin_cancel_calltip ( CssedPlugin* plugin );

/** Informs wheter a calltip is currently active. 
 *	Returns a boolean indicating if there's a calltip active or not
 *	@param plugin - The plugin object.
 */
G_MODULE_IMPORT gboolean
cssed_plugin_calltipactive ( CssedPlugin* plugin );

/** Returns the start point of a calltip. 
 *	Returns an integer indicating the position where a calltip have been shown
 *	@param plugin - The plugin object.
 */
G_MODULE_IMPORT gint
cssed_plugin_calltiposstart ( CssedPlugin* plugin );

//
G_MODULE_IMPORT void
cssed_plugin_document_grab_focus ( CssedPlugin* plugin );
// line related
G_MODULE_IMPORT gchar*
cssed_plugin_get_text_line (CssedPlugin* plugin, gint line);
G_MODULE_IMPORT gint
cssed_plugin_get_line_length (CssedPlugin* plugin, gint line);
G_MODULE_IMPORT gint
cssed_plugin_get_line_count (CssedPlugin* plugin);
// folding
G_MODULE_IMPORT void
cssed_plugin_toggle_fold_at_line (CssedPlugin* plugin, gint line);
G_MODULE_IMPORT gboolean
cssed_plugin_get_line_is_visible (CssedPlugin* plugin, gint line);
G_MODULE_IMPORT void
cssed_plugin_ensure_line_is_visible (CssedPlugin* plugin, gint line);
G_MODULE_IMPORT gint
cssed_plugin_get_fold_level (CssedPlugin* plugin, gint line);
G_MODULE_IMPORT gint
cssed_plugin_get_fold_depth (CssedPlugin* plugin, gint line);
G_MODULE_IMPORT void
cssed_plugin_set_folding_margin_visible (CssedPlugin* plugin, gboolean set);
G_MODULE_IMPORT gboolean
cssed_plugin_get_folding_margin_visible (CssedPlugin* plugin);
G_MODULE_IMPORT gboolean
cssed_plugin_get_folding_enabled (CssedPlugin* plugin);
G_MODULE_IMPORT void
cssed_plugin_fold_all (CssedPlugin* plugin);
G_MODULE_IMPORT void
cssed_plugin_unfold_all (CssedPlugin* plugin);
// validator arrow marker
G_MODULE_IMPORT void
cssed_plugin_set_arrow_marker_at_line (CssedPlugin* plugin, gint line);
G_MODULE_IMPORT void
cssed_plugin_clear_arrow_marker (CssedPlugin* plugin);
// zooming
G_MODULE_IMPORT void
cssed_plugin_zoom_in (CssedPlugin* plugin);
G_MODULE_IMPORT void
cssed_plugin_zoom_out (CssedPlugin* plugin);
G_MODULE_IMPORT void
cssed_plugin_zoom_off (CssedPlugin* plugin);
//
G_MODULE_IMPORT gboolean
cssed_plugin_get_modified (CssedPlugin* plugin);
G_MODULE_IMPORT gboolean
cssed_plugin_is_file_opened (CssedPlugin* plugin, gchar* filename);
G_MODULE_IMPORT gboolean
cssed_plugin_save_document (CssedPlugin* plugin);
G_MODULE_IMPORT gboolean
cssed_plugin_save_document_as (CssedPlugin* plugin, gchar* filename);
G_MODULE_IMPORT gint
cssed_plugin_get_document_index (CssedPlugin* plugin);
G_MODULE_IMPORT void
cssed_plugin_set_document_by_index (CssedPlugin* plugin, gint index);
G_MODULE_IMPORT gint
cssed_plugin_get_num_docs (CssedPlugin* plugin);
G_MODULE_IMPORT gboolean
cssed_plugin_new_document (CssedPlugin* plugin);
G_MODULE_IMPORT gchar*
cssed_plugin_document_get_filename (CssedPlugin* plugin);
G_MODULE_IMPORT gint
cssed_plugin_get_style_at (CssedPlugin* plugin, gint pos);
G_MODULE_IMPORT gchar*
cssed_plugin_prompt_for_file_to_open (CssedPlugin* plugin, gchar* dialog_title, gchar* basedir);
G_MODULE_IMPORT gchar*
cssed_plugin_prompt_for_file_to_save (CssedPlugin* plugin, gchar* dialog_title, gchar* basedir);
G_MODULE_IMPORT gchar*
cssed_plugin_prompt_for_directory_to_open (CssedPlugin* plugin, gchar* dialog_title, gchar* basedir);
G_MODULE_IMPORT gchar*
cssed_plugin_prompt_for_directory_to_create (CssedPlugin* plugin, gchar* dialog_title, gchar* basedir);
///////////////////////////////////////////////////////////////////////////////////////////
/** Inserts a css dialog.  
 *	It inserts a css dialog defining a CSS property's value to be used by auto completion
 *	and quick insertion.
 *	@param plugin - The plugin object. 
 *  @param key - A key defining this value, it must be surrounded by parens - as example (border).
 *	@param klass - A ::CssedDialogClass defining the dialog.
 */
G_MODULE_IMPORT void 
cssed_plugin_css_dialog_insert (CssedPlugin* plugin, gchar* key, gpointer klass);
	
/** Removes a css dialog.  
 *	It removes a css dialog defining a CSS property's value to be used by auto completion
 *	and quick insertion.
 *	@param plugin - The plugin object. 
 *  @param key - The key used to define this value.
 */	
G_MODULE_IMPORT void 
cssed_plugin_css_dialog_remove_by_keyword( CssedPlugin* plugin, gchar* key );

/** Returns a ::CssDialogClass defining a css dialog.  
 *	It returns a ::CssDialogClass defining a CSS proerty's value to be used by
 *	auto completion.
 *	@param plugin - The plugin object. 
 *  @param key - The key used to define this value.
 *	@return A pointer to the ::CssDialogClass you must cast it with (CssedDialogClass*)
 */	
G_MODULE_IMPORT gpointer 
cssed_plugin_css_dialog_lookup_by_keyword( CssedPlugin* plugin, gchar* key );

/** Returns a boolean indicating if the given keyword is being used.  
 *	The keyword is the trigger for most auto completion features.
 *	@param plugin - The plugin object. 
 *  @param key - The keyword.
 *  @return - A pointer to the ::CssDialogClass, or NULL if the key is not found.
 */	
G_MODULE_IMPORT gboolean 
cssed_plugin_css_dialog_keyword_in_use( CssedPlugin* plugin, gchar* key );

/** Returns a pointer to the main ::CssedWindow.  
 *	You must not free or modify this pointer. You must cast it to ::CssedWindow pointer - use (::CssedWindow*) or 
 *	CSSED_WINDOW() at your convenience. You should not free nor modify this pointer and use it only as
 *	parameter on function that require it.
 *	@param plugin - The plugin object. 
 *  @return - The main cssed window object.
 */	
G_MODULE_IMPORT gpointer
cssed_plugin_get_window( CssedPlugin* plugin );

/* ******************************************************************* */
G_MODULE_IMPORT gint
cssed_plugin_get_filetype_id (CssedPlugin* plugin);
G_MODULE_IMPORT void
cssed_plugin_set_filetype_by_id (CssedPlugin* plugin, gint id);

G_MODULE_IMPORT gint
cssed_plugin_get_eol_mode (CssedPlugin* plugin);
G_MODULE_IMPORT void
cssed_plugin_set_eol_mode (CssedPlugin* plugin, gint eol_mode);
G_MODULE_IMPORT void 
cssed_plugin_convert_eols (CssedPlugin* plugin, gint eolmode);

G_MODULE_IMPORT gint
cssed_plugin_get_codepage (CssedPlugin* plugin);
G_MODULE_IMPORT void
cssed_plugin_set_codepage (CssedPlugin* plugin, gint codepage);


/* ******************************************************************* */
G_MODULE_IMPORT gboolean
cssed_plugin_get_line_endings_visible (CssedPlugin* plugin);
G_MODULE_IMPORT void
cssed_plugin_set_line_endings_visible (CssedPlugin* plugin, gboolean visible);
G_MODULE_IMPORT gboolean
cssed_plugin_get_lines_wrapped (CssedPlugin* plugin);
G_MODULE_IMPORT void
cssed_plugin_set_lines_wrapped (CssedPlugin* plugin, gboolean wrapped);
G_MODULE_IMPORT gboolean
cssed_plugin_get_white_spaces_visible (CssedPlugin* plugin);
G_MODULE_IMPORT void
cssed_plugin_set_white_spaces_visible (CssedPlugin* plugin, gboolean visible);


G_MODULE_IMPORT GtkWidget* 
cssed_plugin_get_pop_menu (CssedPlugin* plugin);

/** Sends a message to the underlying scintilla editor control.  
 *	With this function you've got direct access to the scintilla editor control.
 *	You may visit scintilla.org for more information on this function.
 *	@param plugin - The plugin object. 
 *	@param iMessage - The message identifier object. 
 *	@param wParam - message parameter.
  *	@param lParam - message parameter.
 *  @return - The return value depends on the message sent.
 */	
G_MODULE_IMPORT gpointer 
cssed_plugin_send_scintilla_message (CssedPlugin* plugin, guint iMessage, gulong wParam, glong lParam);
