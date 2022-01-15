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

//void 				sci_set_css_style 			(ScintillaObject * sci);
ScintillaObject* 	sci_get_from_notebook_page	(GtkNotebook* notebook, gint 		page_number);
void 				sci_set_line_numbers		(ScintillaObject* sci,  gboolean 	set );
void 				sci_add_text 				(ScintillaObject* sci,  gchar* 		text);
gboolean			sci_can_redo				(ScintillaObject* sci);
gboolean			sci_can_undo				(ScintillaObject* sci);
void 				sci_undo					(ScintillaObject* sci);
void 				sci_redo					(ScintillaObject* sci);
void 				sci_empty_undo_buffer		(ScintillaObject* sci);
void 				sci_end_undo_action			(ScintillaObject* sci);
void 				sci_start_undo_action		(ScintillaObject* sci);
void				sci_set_visible_eols		(ScintillaObject* sci, gboolean set );
gboolean			sci_get_visible_eols		(ScintillaObject* sci);
void				sci_set_lines_wrapped		(ScintillaObject* sci, gboolean set );
gboolean 			sci_get_lines_wrapped		(ScintillaObject* sci);
void				sci_set_visible_white_spaces(ScintillaObject* sci, gboolean set );
gboolean 			sci_get_visible_white_spaces(ScintillaObject* sci);
void 				sci_convert_eols			(ScintillaObject* sci, gint eolmode);
gint				sci_get_eol_mode			(ScintillaObject* sci);
void 				sci_set_eol_mode			(ScintillaObject* sci, gint eolmode);
void 				sci_zoom_in					(ScintillaObject* sci);
void 				sci_zoom_out				(ScintillaObject* sci);
void 				sci_zoom_off				(ScintillaObject* sci);
void				sci_set_marker_at_line		(ScintillaObject* sci, gint line_number, gboolean set, gint marker );
gboolean 			sci_is_marker_set_at_line	(ScintillaObject* sci, gint line, gint marker);
gboolean 			sci_marker_next				(ScintillaObject* sci, gint line, gint marker_mask);
gboolean 			sci_marker_prev				(ScintillaObject* sci, gint line, gint marker_mask);

gint 				sci_get_line_from_position	(ScintillaObject* sci, gint position);
gint 				sci_get_position_from_line	(ScintillaObject* sci, gint line );
gint 				sci_get_current_position	(ScintillaObject* sci);
void 				sci_set_current_position	(ScintillaObject* sci, gint position);
void 				sci_set_current_line		(ScintillaObject* sci, gint line);

void 				sci_cut						(ScintillaObject* sci);
void 				sci_copy					(ScintillaObject* sci);
void 				sci_paste					(ScintillaObject* sci);

gint				sci_get_selection_start		(ScintillaObject* sci);
gint				sci_get_selection_end		(ScintillaObject* sci);
void 				sci_replace_sel				(ScintillaObject* sci, gchar* text);
void 				sci_replace_sel_re			(ScintillaObject* sci, gchar* text);
gint				sci_get_length				(ScintillaObject* sci);
void				sci_get_text				(ScintillaObject* sci,gint len,gchar* text);
gchar				sci_get_char_at				(ScintillaObject* sci, gint pos);
void				sci_get_selected_text		(ScintillaObject* sci, gchar* text);
void				sci_get_line				(ScintillaObject* sci, gint line, gchar* text);
gint 				sci_line_length				(ScintillaObject* sci, gint line);
gint				sci_get_line_count			( ScintillaObject* sci );
void 				sci_get_xy_from_position	(ScintillaObject* sci,gint pos, gint* x, gint* y);

void 				sci_set_undo_collection		(ScintillaObject* sci, gboolean set);
gboolean			sci_get_undo_collection		(ScintillaObject* sci);

void 				sci_set_selection_start		(ScintillaObject* sci, gint position);
void				sci_set_selection_end		(ScintillaObject* sci, gint position);
gint				sci_get_line_end_from_position	(ScintillaObject* sci, gint position);

void 				sci_toggle_fold				(ScintillaObject* sci, gint line);
gboolean			sci_get_line_is_visible		(ScintillaObject* sci, gint line);
void				sci_ensure_line_is_visible	(ScintillaObject* sci, gint line);
gint				sci_get_fold_level			(ScintillaObject* sci, gint line);

void 				sci_set_folding_margin_visible (ScintillaObject * sci, gboolean set );
gboolean			sci_get_folding_margin_visible(ScintillaObject * sci);
void 				sci_set_use_tabs (ScintillaObject * sci, gboolean set );
gboolean			sci_get_use_tabs (ScintillaObject * sci);
void 				sci_set_tabs_width (ScintillaObject * sci, gint set );
gint    			sci_get_tabs_width (ScintillaObject * sci);
gboolean			sci_get_fold_expanded		(ScintillaObject* sci, gint line);
void				sci_colourise				(ScintillaObject* sci, gint start, gint end);

gboolean			sci_get_line_numbers		(ScintillaObject * sci);

void				sci_calltipshow 		(ScintillaObject* sci, gint start, gchar* definition);
void				sci_calltipcancel 		(ScintillaObject* sci);
gboolean			sci_calltipactive 		(ScintillaObject* sci);
gint				sci_calltiposstart 		(ScintillaObject* sci);

void				sci_grab_focus			(ScintillaObject* sci);
