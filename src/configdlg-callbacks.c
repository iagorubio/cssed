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
#include <gtk/gtk.h>

#include "cssedwindow.h"
#include "configdlg-interface.h"
#include "configdlg-callbacks.h"
#include "utils.h"
#include "support.h"

void
config_dialog_change_style_and_button_markup( GtkButton* button, gint* style);

/* ********************* fore color buttons ********************* */
void
on_button_vproperty_fg_clicked         (GtkButton       *button,
                                        gpointer         user_data)
{
	gint *style;
	CssedWindow* window;
	CssedConfig* cfg;
	
	window = CSSED_WINDOW(user_data);
	cfg = cssed_window_get_config( window );
	style = &cfg->IDENTIFIER_fore_color;

	config_dialog_change_style_and_button_markup( button, style );
}


void
on_button_operator_fg_clicked          (GtkButton       *button,
                                        gpointer         user_data)
{
	gint *style;
	CssedWindow* window;
	CssedConfig* cfg;
	
	window = CSSED_WINDOW(user_data);
	cfg = cssed_window_get_config( window );
	style = &cfg->OPERATOR_fore_color;

	config_dialog_change_style_and_button_markup( button, style );
}


void
on_button_unk_pseudoclass_fg_clicked   (GtkButton       *button,
                                        gpointer         user_data)
{
	gint *style;
	CssedWindow* window;
	CssedConfig* cfg;
	
	window = CSSED_WINDOW(user_data);
	cfg = cssed_window_get_config( window );
	style = &cfg->UNKNOWN_PSEUDOCLASS_fore_color;

	config_dialog_change_style_and_button_markup( button, style );
}


void
on_button_pseudoclass_fg_clicked       (GtkButton       *button,
                                        gpointer         user_data)
{
	gint *style;
	CssedWindow* window;
	CssedConfig* cfg;
	
	window = CSSED_WINDOW(user_data);
	cfg = cssed_window_get_config( window );
	style = &cfg->PSEUDOCLASS_fore_color;

	config_dialog_change_style_and_button_markup( button, style );
}


void
on_button_tag_fg_clicked               (GtkButton       *button,
                                        gpointer         user_data)
{
	gint *style;
	CssedWindow* window;
	CssedConfig* cfg;
	
	window = CSSED_WINDOW(user_data);
	cfg = cssed_window_get_config( window );
	style = &cfg->TAG_fore_color;

	config_dialog_change_style_and_button_markup( button, style );
}


void
on_button_default_bg_clicked           (GtkButton       *button,
                                        gpointer         user_data)
{
	gint *style;
	CssedWindow* window;
	CssedConfig* cfg;
	
	window = CSSED_WINDOW(user_data);
	cfg = cssed_window_get_config( window );
	style = &cfg->DEFAULT_back_color;

	config_dialog_change_style_and_button_markup( button, style );
}


void
on_button_unk_property_fg_clicked      (GtkButton       *button,
                                        gpointer         user_data)
{
	gint *style;
	CssedWindow* window;
	CssedConfig* cfg;
	
	window = CSSED_WINDOW(user_data);
	cfg = cssed_window_get_config( window );
	style = &cfg->UNKNOWN_IDENTIFIER_fore_color;

	config_dialog_change_style_and_button_markup( button, style );
}


void
on_button_value_fg_clicked             (GtkButton       *button,
                                        gpointer         user_data)
{
	gint *style;
	CssedWindow* window;
	CssedConfig* cfg;
	
	window = CSSED_WINDOW(user_data);
	cfg = cssed_window_get_config( window );
	style = &cfg->VALID_VALUE_fore_color;

	config_dialog_change_style_and_button_markup( button, style );
}


void
on_button_unk_value_fg_clicked         (GtkButton       *button,
                                        gpointer         user_data)
{
	gint *style;
	CssedWindow* window;
	CssedConfig* cfg;
	
	window = CSSED_WINDOW(user_data);
	cfg = cssed_window_get_config( window );
	style = &cfg->VALUE_fore_color;

	config_dialog_change_style_and_button_markup( button, style );
}


void
on_button_important_fg_clicked         (GtkButton       *button,
                                        gpointer         user_data)
{
	gint *style;
	CssedWindow* window;
	CssedConfig* cfg;
	
	window = CSSED_WINDOW(user_data);
	cfg = cssed_window_get_config( window );
	style = &cfg->IMPORTANT_fore_color;

	config_dialog_change_style_and_button_markup( button, style );
}


void
on_button_directive_fg_clicked         (GtkButton       *button,
                                        gpointer         user_data)
{
	gint *style;
	CssedWindow* window;
	CssedConfig* cfg;
	
	window = CSSED_WINDOW(user_data);
	cfg = cssed_window_get_config( window );
	style = &cfg->DIRECTIVE_fore_color;

	config_dialog_change_style_and_button_markup( button, style );
}


void
on_button_string_double_fg_clicked     (GtkButton       *button,
                                        gpointer         user_data)
{
	gint *style;
	CssedWindow* window;
	CssedConfig* cfg;
	
	window = CSSED_WINDOW(user_data);
	cfg = cssed_window_get_config( window );
	style = &cfg->DOUBLESTRING_fore_color;

	config_dialog_change_style_and_button_markup( button, style );
}


void
on_button_string_single_fg_clicked     (GtkButton       *button,
                                        gpointer         user_data)
{
	gint *style;
	CssedWindow* window;
	CssedConfig* cfg;
	
	window = CSSED_WINDOW(user_data);
	cfg = cssed_window_get_config( window );
	style = &cfg->SINGLESTRING_fore_color;

	config_dialog_change_style_and_button_markup( button, style );
}


void
on_button_class_fg_clicked             (GtkButton       *button,
                                        gpointer         user_data)
{
	gint *style;
	CssedWindow* window;
	CssedConfig* cfg;
	
	window = CSSED_WINDOW(user_data);
	cfg = cssed_window_get_config( window );
	style = &cfg->CLASS_fore_color;

	config_dialog_change_style_and_button_markup( button, style );
}


void
on_button_id_fg_clicked                (GtkButton       *button,
                                        gpointer         user_data)
{
	gint *style;
	CssedWindow* window;
	CssedConfig* cfg;
	
	window = CSSED_WINDOW(user_data);
	cfg = cssed_window_get_config( window );
	style = &cfg->ID_fore_color;

	config_dialog_change_style_and_button_markup( button, style );
}


void
on_button_function_fg_clicked          (GtkButton       *button,
                                        gpointer         user_data)
{
	gint *style;
	CssedWindow* window;
	CssedConfig* cfg;
	
	window = CSSED_WINDOW(user_data);
	cfg = cssed_window_get_config( window );
	style = &cfg->FUNCTION_fore_color;

	config_dialog_change_style_and_button_markup( button, style );
}


void
on_button_number_fg_clicked            (GtkButton       *button,
                                        gpointer         user_data)
{
	gint *style;
	CssedWindow* window;
	CssedConfig* cfg;
	
	window = CSSED_WINDOW(user_data);
	cfg = cssed_window_get_config( window );
	style = &cfg->NUMBER_fore_color;

	config_dialog_change_style_and_button_markup( button, style );
}


void
on_button_unit_fg_clicked              (GtkButton       *button,
                                        gpointer         user_data)
{
	gint *style;
	CssedWindow* window;
	CssedConfig* cfg;
	
	window = CSSED_WINDOW(user_data);
	cfg = cssed_window_get_config( window );
	style = &cfg->UNIT_fore_color;

	config_dialog_change_style_and_button_markup( button, style );
}


void
on_button_color_fg_clicked             (GtkButton       *button,
                                        gpointer         user_data)
{
	gint *style;
	CssedWindow* window;
	CssedConfig* cfg;
	
	window = CSSED_WINDOW(user_data);
	cfg = cssed_window_get_config( window );
	style = &cfg->COLOR_fore_color;

	config_dialog_change_style_and_button_markup( button, style );
}


void
on_button_hexacolor_fg_clicked         (GtkButton       *button,
                                        gpointer         user_data)
{
	gint *style;
	CssedWindow* window;
	CssedConfig* cfg;
	
	window = CSSED_WINDOW(user_data);
	cfg = cssed_window_get_config( window );
	style = &cfg->HEXACOLOR_fore_color;

	config_dialog_change_style_and_button_markup( button, style );
}


void
on_button_attr_match_fg_clicked        (GtkButton       *button,
                                        gpointer         user_data)
{
	gint *style;
	CssedWindow* window;
	CssedConfig* cfg;
	
	window = CSSED_WINDOW(user_data);
	cfg = cssed_window_get_config( window );
	style = &cfg->ATTR_MATCH_fore_color;

	config_dialog_change_style_and_button_markup( button, style );
}


void
on_button_lang_fg_clicked              (GtkButton       *button,
                                        gpointer         user_data)
{
	gint *style;
	CssedWindow* window;
	CssedConfig* cfg;
	
	window = CSSED_WINDOW(user_data);
	cfg = cssed_window_get_config( window );
	style = &cfg->LANGUAGE_fore_color;

	config_dialog_change_style_and_button_markup( button, style );
}

/* ******************* bold toggle buttons ******************** */
void
on_checkbutton_default_bold_toggled    (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
	CssedWindow* window;
	CssedConfig* cfg;

	window = CSSED_WINDOW(user_data);
	cfg = cssed_window_get_config( window );
	
	cfg->DEFAULT_bold = gtk_toggle_button_get_active(togglebutton);
}


void
on_checkbutton_tag_bold_toggled        (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
	CssedWindow* window;
	CssedConfig* cfg;

	window = CSSED_WINDOW(user_data);
	cfg = cssed_window_get_config( window );
	

	cfg->TAG_bold = gtk_toggle_button_get_active(togglebutton);
}


void
on_checkbutton_pseudoclass_bold_toggled
                                        (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
	CssedWindow* window;
	CssedConfig* cfg;

	window = CSSED_WINDOW(user_data);
	cfg = cssed_window_get_config( window );
	
	cfg->PSEUDOCLASS_bold = gtk_toggle_button_get_active(togglebutton);
}


void
on_checkbutton_unk_pseudoclass_bold_toggled
                                        (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
	CssedWindow* window;
	CssedConfig* cfg;

	window = CSSED_WINDOW(user_data);
	cfg = cssed_window_get_config( window );
	
	cfg->UNKNOWN_PSEUDOCLASS_bold = gtk_toggle_button_get_active(togglebutton);
}


void
on_checkbutton_operator_bold_toggled   (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
	CssedWindow* window;
	CssedConfig* cfg;

	window = CSSED_WINDOW(user_data);
	cfg = cssed_window_get_config( window );
	
	cfg->OPERATOR_bold = gtk_toggle_button_get_active(togglebutton);
}


void
on_checkbutton_vproperty_bold_toggled  (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
	CssedWindow* window;
	CssedConfig* cfg;

	window = CSSED_WINDOW(user_data);
	cfg = cssed_window_get_config( window );
	
	cfg->IDENTIFIER_bold = gtk_toggle_button_get_active(togglebutton);
}


void
on_checkbutton_unk_property_bold_toggled(GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
	CssedWindow* window;
	CssedConfig* cfg;

	window = CSSED_WINDOW(user_data);
	cfg = cssed_window_get_config( window );
	
	cfg->UNKNOWN_IDENTIFIER_bold = gtk_toggle_button_get_active(togglebutton);
}


void
on_checkbutton_value_bold_toggled      (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
	CssedWindow* window;
	CssedConfig* cfg;

	window = CSSED_WINDOW(user_data);
	cfg = cssed_window_get_config( window );
	
	cfg->VALID_VALUE_bold = gtk_toggle_button_get_active(togglebutton);
}


void
on_checkbutton_unk_value_bold_toggled  (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
	CssedWindow* window;
	CssedConfig* cfg;

	window = CSSED_WINDOW(user_data);
	cfg = cssed_window_get_config( window );
	
	cfg->VALUE_bold = gtk_toggle_button_get_active(togglebutton);
}


void
on_checkbutton_important_bold_toggled  (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
	CssedWindow* window;
	CssedConfig* cfg;

	window = CSSED_WINDOW(user_data);
	cfg = cssed_window_get_config( window );
	
	cfg->IMPORTANT_bold = gtk_toggle_button_get_active(togglebutton);
}


void
on_checkbutton_directive_bold_toggled  (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
	CssedWindow* window;
	CssedConfig* cfg;

	window = CSSED_WINDOW(user_data);
	cfg = cssed_window_get_config( window );
	
	cfg->DIRECTIVE_bold = gtk_toggle_button_get_active(togglebutton);
}


void
on_checkbutton_string_double_bold_toggled
                                        (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
	CssedWindow* window;
	CssedConfig* cfg;

	window = CSSED_WINDOW(user_data);
	cfg = cssed_window_get_config( window );
	
	cfg->DOUBLESTRING_bold = gtk_toggle_button_get_active(togglebutton);
}


void
on_checkbutton_string_single_bold_toggled
                                        (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
	CssedWindow* window;
	CssedConfig* cfg;

	window = CSSED_WINDOW(user_data);
	cfg = cssed_window_get_config( window );
	
	cfg->SINGLESTRING_bold = gtk_toggle_button_get_active(togglebutton);
}


void
on_checkbutton_class_bold_toggled      (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
	CssedWindow* window;
	CssedConfig* cfg;

	window = CSSED_WINDOW(user_data);
	cfg = cssed_window_get_config( window );
	
	cfg->CLASS_bold = gtk_toggle_button_get_active(togglebutton);
}


void
on_checkbutton_id_bold_toggled         (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
	CssedWindow* window;
	CssedConfig* cfg;

	window = CSSED_WINDOW(user_data);
	cfg = cssed_window_get_config( window );
	
	cfg->ID_bold = gtk_toggle_button_get_active(togglebutton);
}


void
on_checkbutton_function_bold_toggled   (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
	CssedWindow* window;
	CssedConfig* cfg;

	window = CSSED_WINDOW(user_data);
	cfg = cssed_window_get_config( window );
	
	cfg->FUNCTION_bold = gtk_toggle_button_get_active(togglebutton);
}


void
on_checkbutton_number_bold_toggled     (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
	CssedWindow* window;
	CssedConfig* cfg;

	window = CSSED_WINDOW(user_data);
	cfg = cssed_window_get_config( window );
	
	cfg->NUMBER_bold = gtk_toggle_button_get_active(togglebutton);
}


void
on_checkbutton_unit_bold_toggled       (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
	CssedWindow* window;
	CssedConfig* cfg;

	window = CSSED_WINDOW(user_data);
	cfg = cssed_window_get_config( window );
	
	cfg->UNIT_bold = gtk_toggle_button_get_active(togglebutton);
}


void
on_checkbutton_color_bold_toggled      (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
	CssedWindow* window;
	CssedConfig* cfg;

	window = CSSED_WINDOW(user_data);
	cfg = cssed_window_get_config( window );
	
	cfg->COLOR_bold = gtk_toggle_button_get_active(togglebutton);
}


void
on_checkbutton_hexacolor_bold_toggled  (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
	CssedWindow* window;
	CssedConfig* cfg;

	window = CSSED_WINDOW(user_data);
	cfg = cssed_window_get_config( window );
	
	cfg->HEXACOLOR_bold = gtk_toggle_button_get_active(togglebutton);
}


void
on_checkbutton_attr_match_bold_toggled (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
	CssedWindow* window;
	CssedConfig* cfg;

	window = CSSED_WINDOW(user_data);
	cfg = cssed_window_get_config( window );
	
	cfg->ATTR_MATCH_bold = gtk_toggle_button_get_active(togglebutton);
}


void
on_checkbutton_lang_bold_toggled       (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
	CssedWindow* window;
	CssedConfig* cfg;

	window = CSSED_WINDOW(user_data);
	cfg = cssed_window_get_config( window );
	
	cfg->LANGUAGE_bold = gtk_toggle_button_get_active(togglebutton);
}

/* ******************* background buttons ******************** */
void
on_button_tag_bg_clicked               (GtkButton       *button,
                                        gpointer         user_data)
{
	gint *style;
	CssedWindow* window;
	CssedConfig* cfg;
	
	window = CSSED_WINDOW(user_data);
	cfg = cssed_window_get_config( window );
	style = &cfg->TAG_back_color;

	config_dialog_change_style_and_button_markup( button, style );
}


void
on_button_pseudoclass_bg_clicked       (GtkButton       *button,
                                        gpointer         user_data)
{
	gint *style;
	CssedWindow* window;
	CssedConfig* cfg;
	
	window = CSSED_WINDOW(user_data);
	cfg = cssed_window_get_config( window );
	style = &cfg->PSEUDOCLASS_back_color;

	config_dialog_change_style_and_button_markup( button, style );
}


void
on_button_unk_pseudoclass_bg_clicked   (GtkButton       *button,
                                        gpointer         user_data)
{
	gint *style;
	CssedWindow* window;
	CssedConfig* cfg;
	
	window = CSSED_WINDOW(user_data);
	cfg = cssed_window_get_config( window );
	style = &cfg->UNKNOWN_PSEUDOCLASS_back_color;

	config_dialog_change_style_and_button_markup( button, style );
}


void
on_button_operator_bg_clicked          (GtkButton       *button,
                                        gpointer         user_data)
{
	gint *style;
	CssedWindow* window;
	CssedConfig* cfg;
	
	window = CSSED_WINDOW(user_data);
	cfg = cssed_window_get_config( window );
	style = &cfg->OPERATOR_back_color;

	config_dialog_change_style_and_button_markup( button, style );
}


void
on_button_vproperty_bg_clicked         (GtkButton       *button,
                                        gpointer         user_data)
{
	gint *style;
	CssedWindow* window;
	CssedConfig* cfg;
	
	window = CSSED_WINDOW(user_data);
	cfg = cssed_window_get_config( window );
	style = &cfg->IDENTIFIER_back_color;

	config_dialog_change_style_and_button_markup( button, style );
}


void
on_button_unk_property_bg_clicked      (GtkButton       *button,
                                        gpointer         user_data)
{
	gint *style;
	CssedWindow* window;
	CssedConfig* cfg;
	
	window = CSSED_WINDOW(user_data);
	cfg = cssed_window_get_config( window );
	style = &cfg->UNKNOWN_IDENTIFIER_back_color;

	config_dialog_change_style_and_button_markup( button, style );
}


void
on_button_value_bg_clicked             (GtkButton       *button,
                                        gpointer         user_data)
{
	gint *style;
	CssedWindow* window;
	CssedConfig* cfg;
	
	window = CSSED_WINDOW(user_data);
	cfg = cssed_window_get_config( window );
	style = &cfg->VALID_VALUE_back_color;

	config_dialog_change_style_and_button_markup( button, style );
}


void
on_button_unk_value_bg_clicked         (GtkButton       *button,
                                        gpointer         user_data)
{
	gint *style;
	CssedWindow* window;
	CssedConfig* cfg;
	
	window = CSSED_WINDOW(user_data);
	cfg = cssed_window_get_config( window );
	style = &cfg->PSEUDOCLASS_back_color;

	config_dialog_change_style_and_button_markup( button, style );
}


void
on_button_important_bg_clicked         (GtkButton       *button,
                                        gpointer         user_data)
{
	gint *style;
	CssedWindow* window;
	CssedConfig* cfg;
	
	window = CSSED_WINDOW(user_data);
	cfg = cssed_window_get_config( window );
	style = &cfg->IMPORTANT_back_color;

	config_dialog_change_style_and_button_markup( button, style );
}


void
on_button_directive_bg_clicked         (GtkButton       *button,
                                        gpointer         user_data)
{
	gint *style;
	CssedWindow* window;
	CssedConfig* cfg;
	
	window = CSSED_WINDOW(user_data);
	cfg = cssed_window_get_config( window );
	style = &cfg->DIRECTIVE_back_color;

	config_dialog_change_style_and_button_markup( button, style );
}


void
on_button_string_double_bg_clicked     (GtkButton       *button,
                                        gpointer         user_data)
{
	gint *style;
	CssedWindow* window;
	CssedConfig* cfg;
	
	window = CSSED_WINDOW(user_data);
	cfg = cssed_window_get_config( window );
	style = &cfg->DOUBLESTRING_back_color;

	config_dialog_change_style_and_button_markup( button, style );
}


void
on_button_string_single_bg_clicked     (GtkButton       *button,
                                        gpointer         user_data)
{
	gint *style;
	CssedWindow* window;
	CssedConfig* cfg;
	
	window = CSSED_WINDOW(user_data);
	cfg = cssed_window_get_config( window );
	style = &cfg->SINGLESTRING_back_color;

	config_dialog_change_style_and_button_markup( button, style );
}


void
on_button_class_bg_clicked             (GtkButton       *button,
                                        gpointer         user_data)
{
	gint *style;
	CssedWindow* window;
	CssedConfig* cfg;
	
	window = CSSED_WINDOW(user_data);
	cfg = cssed_window_get_config( window );
	style = &cfg->CLASS_back_color;

	config_dialog_change_style_and_button_markup( button, style );
}


void
on_button_id_bg_clicked                (GtkButton       *button,
                                        gpointer         user_data)
{
	gint *style;
	CssedWindow* window;
	CssedConfig* cfg;
	
	window = CSSED_WINDOW(user_data);
	cfg = cssed_window_get_config( window );
	style = &cfg->ID_back_color;

	config_dialog_change_style_and_button_markup( button, style );
}


void
on_button_function_bg_clicked          (GtkButton       *button,
                                        gpointer         user_data)
{
	gint *style;
	CssedWindow* window;
	CssedConfig* cfg;
	
	window = CSSED_WINDOW(user_data);
	cfg = cssed_window_get_config( window );
	style = &cfg->FUNCTION_back_color;

	config_dialog_change_style_and_button_markup( button, style );
}


void
on_button_number_bg_clicked            (GtkButton       *button,
                                        gpointer         user_data)
{
	gint *style;
	CssedWindow* window;
	CssedConfig* cfg;
	
	window = CSSED_WINDOW(user_data);
	cfg = cssed_window_get_config( window );
	style = &cfg->NUMBER_back_color;

	config_dialog_change_style_and_button_markup( button, style );
}


void
on_button_unit_bg_clicked              (GtkButton       *button,
                                        gpointer         user_data)
{
	gint *style;
	CssedWindow* window;
	CssedConfig* cfg;
	
	window = CSSED_WINDOW(user_data);
	cfg = cssed_window_get_config( window );
	style = &cfg->UNIT_back_color;

	config_dialog_change_style_and_button_markup( button, style );
}


void
on_button_color_bg_clicked             (GtkButton       *button,
                                        gpointer         user_data)
{	
	gint *style;
	CssedWindow* window;
	CssedConfig* cfg;
	
	window = CSSED_WINDOW(user_data);
	cfg = cssed_window_get_config( window );
	style = &cfg->COLOR_back_color;

	config_dialog_change_style_and_button_markup( button, style );

}


void
on_button_hexacolor_bg_clicked         (GtkButton       *button,
                                        gpointer         user_data)
{
	gint *style;
	CssedWindow* window;
	CssedConfig* cfg;
	
	window = CSSED_WINDOW(user_data);
	cfg = cssed_window_get_config( window );
	style = &cfg->HEXACOLOR_back_color;

	config_dialog_change_style_and_button_markup( button, style );
}


void
on_button_attr_match_bg_clicked        (GtkButton       *button,
                                        gpointer         user_data)
{
	gint *style;
	CssedWindow* window;
	CssedConfig* cfg;
	
	window = CSSED_WINDOW(user_data);
	cfg = cssed_window_get_config( window );
	style = &cfg->ATTR_MATCH_back_color;

	config_dialog_change_style_and_button_markup( button, style );
}


void
on_button_lang_bg_clicked              (GtkButton       *button,
                                        gpointer         user_data)
{
	gint *style;
	CssedWindow* window;
	CssedConfig* cfg;
	
	window = CSSED_WINDOW(user_data);
	cfg = cssed_window_get_config( window );
	style = &cfg->LANGUAGE_back_color;

	config_dialog_change_style_and_button_markup( button, style );
}


void
on_button_default_fg_clicked           (GtkButton       *button,
                                        gpointer         user_data)
{	gint *style;
	CssedWindow* window;
	CssedConfig* cfg;
	
	window = CSSED_WINDOW(user_data);
	cfg = cssed_window_get_config( window );
	style = &cfg->DEFAULT_fore_color;

	config_dialog_change_style_and_button_markup( button, style );

}

void
on_button_comment_fg_clicked           (GtkButton       *button,
                                        gpointer         user_data)
{
	gint *style;
	CssedWindow* window;
	CssedConfig* cfg;
	
	window = CSSED_WINDOW(user_data);
	cfg = cssed_window_get_config( window );
	style = &cfg->COMMENT_fore_color;

	config_dialog_change_style_and_button_markup( button, style );
}


void
on_button_comment_bg_clicked           (GtkButton       *button,
                                        gpointer         user_data)
{
	gint *style;
	CssedWindow* window;
	CssedConfig* cfg;
	
	window = CSSED_WINDOW(user_data);
	cfg = cssed_window_get_config( window );
	style = &cfg->COMMENT_back_color;

	config_dialog_change_style_and_button_markup( button, style );
}

void
on_checkbutton_comment_bold_toggled    (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
	CssedWindow* window;
	CssedConfig* cfg;

	window = CSSED_WINDOW(user_data);
	cfg = cssed_window_get_config( window );
	
	cfg->COMMENT_bold = gtk_toggle_button_get_active(togglebutton);
}
/* **************** document settings *********************** */
void
on_checkbutton_line_numbers_toggled    (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
	CssedWindow* window;
	CssedConfig* cfg;

	window = CSSED_WINDOW(user_data);
	cfg = cssed_window_get_config( window );
	
	cfg->linenumbers = gtk_toggle_button_get_active(togglebutton);
}
void
on_checkbutton_line_endings_toggled    (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
	CssedWindow* window;
	CssedConfig* cfg;

	window = CSSED_WINDOW(user_data);
	cfg = cssed_window_get_config( window );
	
	cfg->lineendings = gtk_toggle_button_get_active(togglebutton);
}
void
on_checkbutton_white_spaces_toggled    (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
	CssedWindow* window;
	CssedConfig* cfg;

	window = CSSED_WINDOW(user_data);
	cfg = cssed_window_get_config( window );
	
	cfg->whitespaces = gtk_toggle_button_get_active(togglebutton);
}
void
on_checkbutton_line_wrap_toggled       (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
	CssedWindow* window;
	CssedConfig* cfg;

	window = CSSED_WINDOW(user_data);
	cfg = cssed_window_get_config( window );
	
	cfg->lineswraped = gtk_toggle_button_get_active(togglebutton);
}
void
on_checkbutton_auto_completion_toggled  (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
	CssedWindow* window;
	CssedConfig* cfg;

	window = CSSED_WINDOW(user_data);
	cfg = cssed_window_get_config( window );
	
	cfg->autocompletion = gtk_toggle_button_get_active(togglebutton);
}

void
on_checkbutton_folding_toggled        (GtkToggleButton *togglebutton,
                                       gpointer         user_data)
{
	CssedWindow* window;
	CssedConfig* cfg;

	window = CSSED_WINDOW(user_data);
	cfg = cssed_window_get_config( window );
	
	cfg->folding = gtk_toggle_button_get_active(togglebutton);
}
void
on_checkbutton_usetabs_toggled        (GtkToggleButton *togglebutton,
                                       gpointer         user_data)
{
	CssedWindow* window;
	CssedConfig* cfg;

	window = CSSED_WINDOW(user_data);
	cfg = cssed_window_get_config( window );
	
	cfg->usetabs = gtk_toggle_button_get_active(togglebutton);
}

void
on_spinbutton_tabswidth_value_changed   (GtkSpinButton *spinbutton,
                                       gpointer         user_data)
{
	CssedWindow* window;
	CssedConfig* cfg;

	window = CSSED_WINDOW(user_data);
	cfg = cssed_window_get_config( window );
	
	cfg->tabswidth = gtk_spin_button_get_value_as_int (spinbutton);
}

/* font */
void
on_apply_font_button_clicked         	(GtkButton       *button,
                                        gpointer         user_data)
{
	CssedWindow* window;
	CssedConfig* cfg;
	GtkWidget* fontsel;
	GtkWidget* label;
	gchar* font;
	gchar* family_name;
	gint size;
	gchar* labelstr;
	PangoFontDescription* pfd; 

	window = CSSED_WINDOW(user_data);
	cfg = cssed_window_get_config( window );	
	fontsel = lookup_widget(GTK_WIDGET(button),"fontsel");
	label = lookup_widget(GTK_WIDGET(button), "label_current_font");
	
	font = gtk_font_selection_get_font_name(GTK_FONT_SELECTION(fontsel));
	if( font != NULL){
		if( cfg->font != NULL) g_free(cfg->font);
		pfd = pango_font_description_from_string( font );
		family_name = (gchar*) pango_font_description_get_family ( pfd );
		size = pango_font_description_get_size ( pfd );
		cfg->font = g_strdup_printf("!%s",family_name);
		cfg->font_size = size/PANGO_SCALE;
		labelstr = g_strdup_printf("%s %d",family_name,cfg->font_size);
		gtk_label_set_text(GTK_LABEL(label),labelstr);
		pango_font_description_free (pfd);
		g_free(labelstr);
	}	
}

/* browser */
void
on_apply_browser_button_clicked         (GtkButton       *button,
                                        gpointer         user_data)
{
	CssedWindow* window;
	CssedConfig* cfg;
	GtkWidget* label;
	GtkWidget* entry;
	G_CONST_RETURN gchar* command;

	window = CSSED_WINDOW(user_data);
	cfg = cssed_window_get_config( window );	
	entry = lookup_widget(GTK_WIDGET(button),"entry_browser");
	label = lookup_widget(GTK_WIDGET(button), "label_current_browser");

	if( !(cfg && label) ) return;

	command = gtk_entry_get_text(GTK_ENTRY(entry));

	if( strlen(command) < 1 ) {
		return;
	}else{
		if( cfg->browser_command != NULL ) g_free(cfg->browser_command);
		cfg->browser_command = g_strdup(command);
		gtk_label_set_text(GTK_LABEL(label), command);
	}	
}

/* 
	called by all buttons to set the window->config fields value and update
	the pango markup in button's labels to show the current color 
*/

void
config_dialog_change_style_and_button_markup( GtkButton* button, gint* style)
{
	GtkWidget* colorselection;
	GtkWidget *colorsel;
	GdkColor color;
	gint resp;
	gint scicolor;
	GList* child;	
	GtkWidget* label;
	gchar* markup;
	gchar* colorstr;
	G_CONST_RETURN gchar* label_text;

	child = gtk_container_get_children (GTK_CONTAINER(button));
	label = g_list_first( child )->data;
	colorselection = gtk_color_selection_dialog_new  (_("Select a color for this style") );
	colorsel = GTK_COLOR_SELECTION_DIALOG(colorselection)->colorsel;

	label_text = gtk_label_get_label(GTK_LABEL(label)) ;
	colorstr = g_strndup (label_text + 18, 7);

	if(  gdk_color_parse (colorstr, &color) )
		gtk_color_selection_set_current_color( 
						GTK_COLOR_SELECTION(colorsel),
						&color );
	g_free( colorstr );

	resp = gtk_dialog_run( GTK_DIALOG(colorselection) );

	if( resp == GTK_RESPONSE_OK ){
		gtk_color_selection_get_current_color(
				GTK_COLOR_SELECTION(colorsel),
				&color);
		scicolor = gdk_color_to_scintilla_integer_color( &color );
		*style = scicolor;
		colorstr = gdk_color_to_rgb_triplet_string( &color );
		markup = g_strdup_printf("<span background='%s'>    </span>",colorstr);
		gtk_label_set_text(GTK_LABEL(label),markup);
		gtk_label_set_use_markup(GTK_LABEL(label), TRUE);
		g_list_free( child );
		g_free( colorstr );
		g_free( markup );		
	}
	gtk_widget_destroy( colorselection );
}
