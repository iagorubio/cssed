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

#include <gtk/gtk.h>


void
on_button_vproperty_fg_clicked         (GtkButton       *button,
                                        gpointer         user_data);

void
on_button_operator_fg_clicked          (GtkButton       *button,
                                        gpointer         user_data);

void
on_button_unk_pseudoclass_fg_clicked   (GtkButton       *button,
                                        gpointer         user_data);

void
on_button_pseudoclass_fg_clicked       (GtkButton       *button,
                                        gpointer         user_data);

void
on_button_tag_fg_clicked               (GtkButton       *button,
                                        gpointer         user_data);

void
on_button_default_bg_clicked           (GtkButton       *button,
                                        gpointer         user_data);

void
on_button_unk_property_fg_clicked      (GtkButton       *button,
                                        gpointer         user_data);

void
on_button_value_fg_clicked             (GtkButton       *button,
                                        gpointer         user_data);

void
on_button_unk_value_fg_clicked         (GtkButton       *button,
                                        gpointer         user_data);

void
on_button_important_fg_clicked         (GtkButton       *button,
                                        gpointer         user_data);

void
on_button_directive_fg_clicked         (GtkButton       *button,
                                        gpointer         user_data);

void
on_button_string_double_fg_clicked     (GtkButton       *button,
                                        gpointer         user_data);

void
on_button_string_single_fg_clicked     (GtkButton       *button,
                                        gpointer         user_data);

void
on_button_class_fg_clicked             (GtkButton       *button,
                                        gpointer         user_data);

void
on_button_id_fg_clicked                (GtkButton       *button,
                                        gpointer         user_data);

void
on_button_function_fg_clicked          (GtkButton       *button,
                                        gpointer         user_data);

void
on_button_number_fg_clicked            (GtkButton       *button,
                                        gpointer         user_data);

void
on_button_unit_fg_clicked              (GtkButton       *button,
                                        gpointer         user_data);

void
on_button_color_fg_clicked             (GtkButton       *button,
                                        gpointer         user_data);

void
on_button_hexacolor_fg_clicked         (GtkButton       *button,
                                        gpointer         user_data);

void
on_button_attr_match_fg_clicked        (GtkButton       *button,
                                        gpointer         user_data);

void
on_button_lang_fg_clicked              (GtkButton       *button,
                                        gpointer         user_data);

void
on_checkbutton_default_bold_toggled    (GtkToggleButton *togglebutton,
                                        gpointer         user_data);

void
on_checkbutton_tag_bold_toggled        (GtkToggleButton *togglebutton,
                                        gpointer         user_data);

void
on_checkbutton_pseudoclass_bold_toggled
                                        (GtkToggleButton *togglebutton,
                                        gpointer         user_data);

void
on_checkbutton_unk_pseudoclass_bold_toggled
                                        (GtkToggleButton *togglebutton,
                                        gpointer         user_data);

void
on_checkbutton_operator_bold_toggled   (GtkToggleButton *togglebutton,
                                        gpointer         user_data);

void
on_checkbutton_vproperty_bold_toggled  (GtkToggleButton *togglebutton,
                                        gpointer         user_data);

void
on_checkbutton_unk_property_bold_toggled
                                        (GtkToggleButton *togglebutton,
                                        gpointer         user_data);

void
on_checkbutton_value_bold_toggled       (GtkToggleButton *togglebutton,
                                        gpointer         user_data);

void
on_checkbutton_unk_value_bold_toggled  (GtkToggleButton *togglebutton,
                                        gpointer         user_data);

void
on_checkbutton_important_bold_toggled  (GtkToggleButton *togglebutton,
                                        gpointer         user_data);

void
on_checkbutton_directive_bold_toggled  (GtkToggleButton *togglebutton,
                                        gpointer         user_data);

void
on_checkbutton_string_double_bold_toggled
                                        (GtkToggleButton *togglebutton,
                                        gpointer         user_data);

void
on_checkbutton_string_single_bold_toggled
                                        (GtkToggleButton *togglebutton,
                                        gpointer         user_data);

void
on_checkbutton_class_bold_toggled      (GtkToggleButton *togglebutton,
                                        gpointer         user_data);

void
on_checkbutton_id_bold_toggled         (GtkToggleButton *togglebutton,
                                        gpointer         user_data);

void
on_checkbutton_function_bold_toggled   (GtkToggleButton *togglebutton,
                                        gpointer         user_data);

void
on_checkbutton_number_bold_toggled     (GtkToggleButton *togglebutton,
                                        gpointer         user_data);

void
on_checkbutton_unit_bold_toggled       (GtkToggleButton *togglebutton,
                                        gpointer         user_data);

void
on_checkbutton_color_bold_toggled      (GtkToggleButton *togglebutton,
                                        gpointer         user_data);

void
on_checkbutton_hexacolor_bold_toggled  (GtkToggleButton *togglebutton,
                                        gpointer         user_data);

void
on_checkbutton_attr_match_bold_toggled (GtkToggleButton *togglebutton,
                                        gpointer         user_data);

void
on_checkbutton_lang_bold_toggled       (GtkToggleButton *togglebutton,
                                        gpointer         user_data);

void
on_button_tag_bg_clicked               (GtkButton       *button,
                                        gpointer         user_data);

void
on_button_pseudoclass_bg_clicked       (GtkButton       *button,
                                        gpointer         user_data);

void
on_button_unk_pseudoclass_bg_clicked   (GtkButton       *button,
                                        gpointer         user_data);

void
on_button_operator_bg_clicked          (GtkButton       *button,
                                        gpointer         user_data);

void
on_button_vproperty_bg_clicked         (GtkButton       *button,
                                        gpointer         user_data);

void
on_button_unk_property_bg_clicked      (GtkButton       *button,
                                        gpointer         user_data);

void
on_button_value_bg_clicked             (GtkButton       *button,
                                        gpointer         user_data);

void
on_button_unk_value_bg_clicked         (GtkButton       *button,
                                        gpointer         user_data);

void
on_button_important_bg_clicked         (GtkButton       *button,
                                        gpointer         user_data);

void
on_button_directive_bg_clicked         (GtkButton       *button,
                                        gpointer         user_data);

void
on_button_string_double_bg_clicked     (GtkButton       *button,
                                        gpointer         user_data);

void
on_button_string_single_bg_clicked     (GtkButton       *button,
                                        gpointer         user_data);

void
on_button_class_bg_clicked             (GtkButton       *button,
                                        gpointer         user_data);

void
on_button_id_bg_clicked                (GtkButton       *button,
                                        gpointer         user_data);

void
on_button_function_bg_clicked          (GtkButton       *button,
                                        gpointer         user_data);

void
on_button_number_bg_clicked            (GtkButton       *button,
                                        gpointer         user_data);

void
on_button_unit_bg_clicked              (GtkButton       *button,
                                        gpointer         user_data);

void
on_button_color_bg_clicked             (GtkButton       *button,
                                        gpointer         user_data);

void
on_button_hexacolor_bg_clicked         (GtkButton       *button,
                                        gpointer         user_data);

void
on_button_attr_match_bg_clicked        (GtkButton       *button,
                                        gpointer         user_data);

void
on_button_lang_bg_clicked              (GtkButton       *button,
                                        gpointer         user_data);

void
on_button_default_fg_clicked           (GtkButton       *button,
                                        gpointer         user_data);
void
on_checkbutton_comment_bold_toggled    (GtkToggleButton *togglebutton,
                                        gpointer         user_data);
void
on_button_comment_bg_clicked           (GtkButton       *button,
                                        gpointer         user_data);

void
on_button_comment_fg_clicked           (GtkButton       *button,
                                        gpointer         user_data);
/* document settings */
void
on_checkbutton_line_numbers_toggled    (GtkToggleButton *togglebutton,
                                        gpointer         user_data);										
void
on_checkbutton_line_endings_toggled    (GtkToggleButton *togglebutton,
                                        gpointer         user_data);
void
on_checkbutton_white_spaces_toggled    (GtkToggleButton *togglebutton,
                                        gpointer         user_data);
void
on_checkbutton_line_wrap_toggled       (GtkToggleButton *togglebutton,
                                        gpointer         user_data);
void
on_checkbutton_auto_completion_toggled  (GtkToggleButton *togglebutton,
                                        gpointer         user_data);									

void
on_checkbutton_folding_toggled        (GtkToggleButton *togglebutton,
                                       gpointer         user_data);

void
on_checkbutton_usetabs_toggled        (GtkToggleButton *togglebutton,
                                       gpointer         user_data); 
                                    
void
on_spinbutton_tabswidth_value_changed   (GtkSpinButton *spinbutton,
                                       gpointer         user_data);                                    
/* font */
void
on_apply_font_button_clicked         	(GtkButton       *button,
                                        gpointer         user_data);
/* browser */
void
on_apply_browser_button_clicked         (GtkButton       *button,
                                        gpointer         user_data);
