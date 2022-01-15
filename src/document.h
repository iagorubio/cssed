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

#define SSM(s, m, w, l) scintilla_send_message(s, m, w, l)
//#define WITH_SPLITTER_WINDOW

typedef struct _CssedCssDialogsData {
	CssedWindow* main_window;
	gchar* previous_string;
	gboolean add_property_string;
} CssedCssDialogsData;

typedef struct _CssedPopmenuData {
	CssedWindow* main_window;
	CssedDoc* document;
	gchar* attribute;
	gchar* property; // previous_string
	gboolean add_property_string;
} CssedPopmenuData;

typedef struct _CssedPoint {
	CssedWindow* main_window;
	gint x;
	gint y;
} CssedPoint;

#ifdef WITH_SPLITTER_WINDOW
typedef enum {
	CSSED_DOC_SPLIT_NONE,
	CSSED_DOC_SPLIT_HORIZONTAL,
	CSSED_DOC_SPLIT_VERTICAL
} CssedSplitType;
#endif
/*
GtkWidget *
create_initial_blank_editor  (CssedWindow* window);
*/
CssedDoc*
create_and_attach_new_doc(CssedWindow* window ,const gchar* title);

CssedDoc *
cssed_create_new_doc (CssedWindow* window);

gboolean
document_save(CssedDoc* doc);

gboolean
document_save_as(CssedDoc* doc, gchar* filename);

void
document_close (CssedDoc* doc);

void 
document_add_text (CssedDoc* doc, gchar* text);

// no need to be NULL terminated
void
document_add_text_buffer (CssedDoc* doc, gchar* buffer , gint len);

void
document_add_text_line (CssedDoc * doc, gchar* text);

void 
document_scroll_to_caret (CssedDoc* doc);

CssedDoc*
document_get_from_notebook_page_num(CssedWindow* window, gint page_number);

CssedDoc*
document_get_current(CssedWindow* window);

gint
document_get_index (CssedDoc* doc);

void
document_set_current(CssedDoc* doc);

gboolean
document_get_modified (CssedDoc* doc);

gboolean
document_dispose (CssedDoc* doc);

gboolean 
document_get_line_numbers_visible (CssedDoc* doc);

void 
document_set_line_numbers_visible (CssedDoc* doc, gboolean visible);

gint
document_get_position_from_line (CssedDoc* doc, gint line);

void
document_set_white_spaces_visible (CssedDoc* doc, gboolean visible);

void
document_set_line_endings_visible (CssedDoc* doc, gboolean visible);

gboolean
document_get_lines_wrapped (CssedDoc* doc);

void
document_set_lines_wrapped (CssedDoc* doc, gboolean wrapped);

CssedEolMode
document_get_eol_mode (CssedDoc* doc);

void 
document_set_eol_mode(CssedDoc* doc, CssedEolMode eolmode);

void 
document_convert_eols(CssedDoc* doc, CssedEolMode eolmode);

gboolean
document_get_line_endings_visible (CssedDoc* doc);

gboolean
document_get_lines_wrapped (CssedDoc* doc);

gboolean
document_get_white_spaces_visible (CssedDoc* doc);

CssedDoc*
document_get_from_editor (CssedWindow* window, GtkWidget* editor);

gboolean 
document_marker_next(CssedDoc* doc, gint line, gint marker_mask);

gboolean 
document_marker_prev(CssedDoc* doc, gint line, gint marker_mask);

/* filetype */
void
document_set_filetype (CssedDoc* doc, CssedFileType* filetype);
void
document_set_filetype_by_id (CssedDoc* doc, CssedFileTypeId id);

CssedFileType*
document_get_filetype_from_filename (CssedDoc* doc, gchar* filename);

CssedFileType*
document_get_filetype (CssedDoc* doc);

CssedFileTypeId
document_get_filetype_id (CssedDoc* doc);
/* */
void
document_set_font_by_name(CssedDoc* doc, gchar* font_name);

void
document_set_font(CssedDoc* doc);

void
document_set_codepage(CssedDoc* doc, CssedCodePage codepage);

CssedCodePage
document_get_codepage(CssedDoc* doc);

void
document_set_character_set(CssedDoc* doc, gint charset);

void
document_set_font_size(CssedDoc* doc, gint size);

void
document_set_current_font_size(CssedDoc* doc);

void
document_set_default_configuration (CssedDoc* doc);

gboolean			
document_can_redo(CssedDoc* doc);

gboolean			
document_can_undo(CssedDoc* doc);

void 
document_undo(CssedDoc* doc);

void 
document_redo(CssedDoc* doc);

void 
document_empty_undo_buffer(CssedDoc* doc);

void 
document_end_undo_action(CssedDoc* doc);

void 
document_new_undo_action(CssedDoc* doc);

void 
document_set_undo_collection(CssedDoc* doc, gboolean set);

gboolean
document_get_undo_collection(CssedDoc* doc);

void 
document_start_undo_action (CssedDoc* doc);

void 
document_search_next (CssedDoc* doc, gchar* text, gint flags);

void
document_search_prev (CssedDoc* doc, gchar* text, gint flags);

void 
document_zoom_in(CssedDoc* doc);

void 
document_zoom_off(CssedDoc* doc);

void 
document_zoom_out(CssedDoc* doc);

void 
document_set_marker_at_line (CssedDoc* doc, gint line, gboolean set, gint marker);

gboolean
document_is_marker_set_at_line (CssedDoc* doc, gint line, gint marker);

gint
document_get_line_from_position (CssedDoc* doc, gint position);

gint 
document_get_current_position (CssedDoc* doc);

void
document_set_current_position	(CssedDoc* doc, gint position);

void
document_set_current_line (CssedDoc* doc, gint line);

void 
document_paste (CssedDoc* doc);

void 
document_copy (CssedDoc* doc);

void 
document_cut (CssedDoc* doc);

gint
document_get_selection_end(CssedDoc* doc);

gint
document_get_selection_start(CssedDoc* doc);

void
document_set_selection_start(CssedDoc* doc, gint position);

void
document_set_selection_end(CssedDoc* doc, gint position);

gint 
document_get_line_end_from_position(CssedDoc* doc, gint position);

void 
document_set_selection_range(CssedDoc* doc,gint start, gint end);

void
document_replace_sel(CssedDoc* doc, gchar* text);

void
document_replace_sel_re(CssedDoc* doc, gchar* text);

void
document_clear_sel(CssedDoc* doc);

gint
document_get_length	(CssedDoc* doc);

void
document_get_text(CssedDoc* doc, gint len, gchar* text);

void
document_get_selected_text(CssedDoc* doc, gchar* text);
void
document_set_modified (CssedDoc* doc , gboolean modified);

void
document_set_save_point (CssedDoc* doc);

void 
document_write_error_to_program_output(	CssedDoc* doc, 
										gchar* error_message,
										gchar* color);
										
void 
document_show_user_list (CssedDoc* doc, gint identifier, gchar* list);

void
document_show_autocompletion_list (CssedDoc* doc, gint len, gchar* list);

void 
document_autocompletion_cancel (CssedDoc* doc);

gboolean
document_is_autocompletion_active (CssedDoc* doc);

void 
document_autocompletion_select (CssedDoc* doc, gchar* select);

gboolean
document_current_pos_is_into_selector(CssedDoc* doc);
	
void
document_get_text_line(CssedDoc* doc, gint line, gchar* buffer);

gchar
document_get_char_at(CssedDoc* doc, gint pos);

gint 
document_get_line_length(CssedDoc* doc, gint line);

gint
document_get_line_count (CssedDoc* doc);

void 
fire_dialog_from_strings_or_write_output(	CssedWindow* window,
											CssedDoc* tmpdoc, 
											gchar* attribute, 
											gchar* property,
											gboolean add_previous);
											
void
document_toggle_fold_at_line(CssedDoc* doc, gint line);

gboolean
document_get_line_is_visible(CssedDoc* doc, gint line);

void
document_ensure_line_is_visible (CssedDoc* doc, gint line);

gint
document_get_fold_level (CssedDoc* doc, gint line);

gint
document_get_fold_depth(CssedDoc* doc, gint line);

void 
document_set_folding_margin_visible (CssedDoc* doc, gboolean visible);

gboolean 
document_get_folding_margin_visible (CssedDoc* doc);

void
document_fold_all (CssedDoc* doc);

void
document_unfold_all (CssedDoc* doc);


gboolean
document_get_folding_enabled (CssedDoc* doc);

void
document_set_use_tabs (CssedDoc* doc, gboolean set );

gboolean
document_get_use_tabs (CssedDoc* doc);

void
document_set_tabs_width (CssedDoc* doc, gint set );

gint
document_get_tabs_width (CssedDoc* doc);

gboolean 
document_get_fold_expanded_at_line (CssedDoc* doc, gint line);

void				
document_style_clear_all(CssedDoc* doc) ;

gint
document_get_style_at(CssedDoc* doc, gint pos);

void				
document_colourise_all(CssedDoc* doc);

void				
document_colourise(CssedDoc* doc, gint start, gint end);

void
document_set_validator_arrow_at_line (CssedDoc* doc, gint line);

void
document_set_validator_arrow_at_line_on_idle (CssedDoc* doc, gint line);

void
document_clear_validator_arrow (CssedDoc* doc);

// access to document object fields //
CssedWindow*
document_get_window (CssedDoc* doc);

gchar* document_get_filename (CssedDoc* doc);

void document_set_filename (CssedDoc* doc , gchar* filename);
gboolean document_get_autocompletion_enabled (CssedDoc* doc);

void
document_set_autocompletion_enabled  (CssedDoc* doc, gboolean enabled);

// for awfull auto-indentation, not clever but at leat as stupid as predictable.
void
document_indent_as_last_line (CssedDoc* doc);


gboolean // returns true is the previous line starts in whitespace
document_last_line_is_indented (CssedDoc* doc);

// scintilla control access 
GtkWidget* // you must use the SCINTILLA() macro to get the ScintillaObject and include scintilla headers 
document_get_scintilla_widget (CssedDoc* doc);
#ifdef WITH_SPLITTER_WINDOW
GtkWidget*
document_get_primary_scintilla_widget ( CssedDoc* doc);
GtkWidget*
document_get_secondary_scintilla_widget ( CssedDoc* doc);
#endif
// The container widget attached to the notebook
GtkWidget* 
document_get_container_widget (CssedDoc* doc);

#ifdef WITH_SPLITTER_WINDOW
void
document_split( CssedDoc* doc, CssedSplitType type );
void
document_unsplit( CssedDoc* doc );
gboolean
document_splitted ( CssedDoc* doc );
GtkWidget*
document_get_active_editor(CssedDoc* doc);
void
document_set_active_editor(CssedDoc* doc, GtkWidget* editor);
#endif
// new open function - returns NULL and sets error on failure */
CssedDoc*
document_open (CssedWindow* window , gchar *filename, GError **error);
void
document_open_with_error_check (CssedWindow* window , gchar *filename);
gboolean
document_add_file (CssedDoc* doc , gchar *filename, GError **err);
// calltip
void
document_calltipshow (CssedDoc* doc, gint start, gchar* definition);
void
document_calltipcancel (CssedDoc* doc);
gboolean
document_calltipactive (CssedDoc* doc);
gint
document_calltiposstart (CssedDoc* doc);
//
void
document_grab_focus	(CssedDoc* doc);
//////////////// THIS IS TEMPORARY //////////////////////
void  
document_pop_submenu_from_keyword_in_treeview(CssedDoc* doc,gchar* text);

gpointer // to be wrapped by the plugin's interface
document_send_scintilla_message ( CssedDoc* doc, guint iMessage, gulong wParam, glong lParam );
