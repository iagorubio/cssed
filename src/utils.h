/*  cssed (c) Iago Rubio, 2003-2005 - A tiny CSS editor.
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
 
/*  Those match GtkFileChooserAction but are defined here for gtk < 2.4 systems. 
	They're used in the cssed_prompt_for_file  function 
*/
typedef enum {
	CSSED_FILESELECTOR_OPEN,
	CSSED_FILESELECTOR_SAVE, 
	CSSED_FILESELECTOR_SELECT_FOLDER,
	CSSED_FILESELECTOR_CREATE_FOLDER,
} CssedFileSelectorType ;

void
cssed_error_message (gchar *title, gchar *message, ...);

gchar*
gdk_color_to_rgb_triplet_string( GdkColor *color);

gint
gdk_color_to_scintilla_integer_color( GdkColor *color );

GtkWidget*
create_custom_button_from_stock(gchar *label_text,gchar *stock_id);

gchar*
allocated_buffer_add_string(gchar *buffer, const gchar *string);

GtkWidget*
create_ok_cancel_dialog (gchar *title, gchar* format, ...);

GtkWidget*
create_yes_no_cancel_dialog (gchar* title, gchar* format, ...);

GtkWidget*
create_yes_no_save_dialog (gchar* title, gchar* format, ...);

GtkWidget*
create_unsaved_files_dialog (void);
/* this must be changed to a window function FIXME */
void
init_default_config( CssedWindow *window );
/* this must be changed to a window function FIXME */
void
save_config_to_file(CssedWindow *window);

void 
cssed_config_copy_style_and_defaults( CssedConfig *source, CssedConfig *target );

gboolean
char_ptr_is_decimal_num(const gchar *ptr);
/* this must be changed to a window function FIXME */
void
load_all_plugins( CssedWindow *cssedwin, GList *plugins_to_load );

// compare two tree view iters containing a markup string
gint
tree_iter_doc_scanner_compare_string(	GtkTreeModel *model,
                                  		GtkTreeIter *a,
                                  		GtkTreeIter *b,
                                  		gpointer user_data);
										
// to load and save a GtkTextBuffer into a file without error checking
void
text_buffer_load_contents( GtkTextBuffer *buffer, gchar *filename );
void
text_buffer_save_contents( GtkTextBuffer *buffer, gchar *filename );

// to load to and from the scratch pad
void
scratch_pad_load_contents( GtkTextBuffer *scratch_pad_buffer );
void
scratch_pad_save_contents( GtkTextBuffer *scratch_pad_buffer );
CssedEolMode
buffer_get_first_eol( gchar *buffer_start, gchar *buffer_end );

// initall code for a new generic gtk-2.0 to gtk-2.4 file selector dialog
gchar*
cssed_prompt_for_file (CssedWindow *window, gchar *title, gchar *path, CssedFileSelectorType type);
// show an error to the user
void
cssed_show_message_from_error (GError *error, gchar *title, gboolean free_error);
// for variable substitution on strings
gchar* 
cssed_string_replace_all(gchar *string, gchar *replaced_str, gchar *replacement_str);
gint
cssed_string_unescape_uri( const char *source, char **retval );

