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
on_widget_change_set_toggle_active (GtkWidget*widget,  gpointer user_data);

void
on_widget_change_unset_toggle_active (GtkWidget*widget,  gpointer user_data);

void
on_angle_property_dialog_helpbutton_clicked	(GtkButton * button, 
											gpointer user_data);

void
on_angle_property_dialog_ok_button_clicked(GtkButton * button, 
											gpointer user_data);

void
on_background_property_dialog_helpbutton_clicked(GtkButton * button, 
											gpointer user_data);

void
on_background_property_dialog_okbutton_clicked(GtkButton * button, 
											gpointer user_data);

void
on_border_properties_helpbutton_clicked	(GtkButton * button, 
											gpointer user_data);
	
void
on_border_properties_okbutton_clicked (GtkButton * button,
				       						gpointer user_data);
					   
void
on_border_with_properties_helpbutton_clicked(GtkButton * button,
											gpointer user_data);

void
on_border_width_properties_dialog_okbutton_clicked(GtkButton * button, 
											gpointer user_data);

void
on_border_width_single_helpbutton_clicked(GtkButton * button,
											gpointer user_data);

void
on_border_width_single_okbutton_clicked	(GtkButton * button, 
										gpointer user_data);

void
on_button_counter_increment_reset_add_clicked(GtkButton * button,
										gpointer user_data);

void
on_button_counter_increment_reset_del_clicked(GtkButton * button, 
										gpointer user_data);

void
on_dialog_counter_increment_reset_helpbutton_clicked(GtkButton * button, 
										gpointer user_data);

void on_dialog_counter_increment_reset_okbutton_clicked	(GtkButton * button, 
										gpointer user_data);

void
on_string_properties_dialog_helpbutton_clicked(GtkButton * button, 
										gpointer user_data);

void
on_string_properties_dialog_okbutton_clicked(GtkButton * button, 
										gpointer user_data);

void
on_uri_value_dialog_helpbutton_clicked (GtkButton * button,
										gpointer user_data);

void
on_uri_value_dialog_okbutton_clicked (GtkButton * button, 
										gpointer user_data);

void on_attr_value_dialog_helpbutton_clicked(GtkButton * button, 
										gpointer user_data);

void
on_attr_value_dialog_okbutton_clicked (GtkButton * button,
				       					gpointer user_data);

void
on_counter_property_dialog_helpbutton_clicked(GtkButton * button, 
										gpointer user_data);

void
on_counter_property_dialog_okbutton_clicked	(GtkButton * button, 
										gpointer user_data);

void
on_font_property_dialog_helpbutton_clicked	(GtkButton * button, 
										gpointer user_data);

void
on_font_property_dialog_okbutton_clicked(GtkButton * button, 
										gpointer user_data);
void
on_button_font_set_add_clicked (GtkButton * button, 
										gpointer user_data);


void
on_button_font_set_del_clicked (GtkButton * button, gpointer user_data);

void
on_button_font_family_dialog_add_clicked(GtkButton * button, 
							gpointer user_data);

void on_button_font_family_dialog_del_clicked(GtkButton * button, 
							gpointer user_data);

void
on_font_family_dialog_helpbutton_clicked(GtkButton * button, 
							gpointer user_data);

void
on_font_family_dialog_okbutton_clicked (GtkButton * button,
							gpointer user_data);

void
on_font_size_dialog_helpbutton_clicked (GtkButton * button,
							gpointer user_data);

void
on_font_size_dialog_okbutton_clicked (GtkButton * button,
							gpointer user_data);

void
on_dialog_frecuency_helpbutton_clicked (GtkButton * button,
							gpointer user_data);

void on_frecuency_property_dialog_okbutton_clicked(GtkButton * button, 
							gpointer user_data);

void on_identifier_value_helpbutton_clicked	(GtkButton * button, 
							gpointer user_data);
void on_identifier_value_dialog_okbutton_clicked(GtkButton * button, 
							gpointer user_data);
void
on_dialog_integer_helpbutton_clicked (GtkButton * button, 
							gpointer user_data);

void
on_integer_value_dialog_okbutton_clicked(GtkButton * button, 
							gpointer user_data);

void
on_length_value_dialog_helpbutton_clicked(GtkButton * button, 
							gpointer user_data);

void
on_length_value_dialog_okbutton_clicked	(GtkButton * button, 
							gpointer user_data);

void
on_list_style_dialog_helpbutton_clicked(GtkButton * button, 
							gpointer user_data);

void
on_list_style_dialog_okbutton_clicked (GtkButton * button,
							gpointer user_data);

void
on_margin_property_dialog_helpbutton_clicked(GtkButton * button,
							gpointer user_data);

void
on_margin_property_dialog_okbutton_clicked(GtkButton * button,
							gpointer user_data);

void
on_margin_width_value_dialog_helpbutton_clicked(GtkButton * button,
							gpointer user_data);

void
on_margin_width_dialog_okbutton_clicked(GtkButton * button,
							gpointer user_data);

void on_number_value_dialog_helpbutton_clicked(GtkButton * button,
							gpointer user_data);

void
on_number_value_dialog_okbutton_clicked(GtkButton * button,
							gpointer user_data);

void
on_padding_property_dialog_helpbutton_clicked(GtkButton * button,
							gpointer user_data);

void
on_padding_property_dialog_okbutton_clicked(GtkButton * button,
							gpointer user_data);

void
on_outline_property_dialog_helpbutton_clicked(GtkButton * button,
							gpointer user_data);

void
on_outline_property_dialog_okbutton_clicked(GtkButton * button,
							gpointer user_data);

void
on_pause_property_dialog_helpbutton_clicked(GtkButton * button,
							gpointer user_data);

void
on_pause_property_dialog_okbutton_clicked(GtkButton * button,
							gpointer user_data);

void
on_percentage_value_dialog_helpbutton_clicked(GtkButton * button,
							gpointer user_data);
	
void
on_percentage_value_dialog_okbutton_clicked(GtkButton * button,
							gpointer user_data);
	
void
on_percentage_length_value_dialog_helpbutton_clicked(GtkButton * button,
							gpointer user_data);

void
on_percentage_length_value_dialog_okbutton_clicked(GtkButton * button,
							gpointer user_data);

void
on_play_during_property_dialog_helpbutton_clicked(GtkButton * button,
							gpointer user_data);

void on_play_during_property_dialog_okbutton_clicked(GtkButton * button,
							gpointer user_data);
void
on_button_quotes_property_dialog_add_clicked(GtkButton * button,
							gpointer user_data);

void
on_button_quotes_property_dialog_del_clicked (GtkButton * button,
							gpointer user_data);

void on_quotes_property_dialog_helpbutton_clicked (GtkButton * button,
							gpointer user_data);

void
on_quotes_property_dialog_okbutton_clicked (GtkButton * button,
							gpointer user_data);

void on_shape_value_dialog_helpbutton_clicked (GtkButton * button,
							gpointer user_data);

void
on_shape_value_dialog_okbutton_clicked (GtkButton * button,
							gpointer user_data);

void
on_size_lenght_value_dialog_helpbutton_clicked (GtkButton * button,
							gpointer user_data);

void
on_size_length_value_dialog_okbutton_clicked (GtkButton * button,
							gpointer user_data);

void
on_button_text_shadow_value_dialog_add_clicked (GtkButton * button,
							gpointer user_data);

void
on_button_text_shadow_value_dialog_del_clicked (GtkButton * button,
							gpointer user_data);

void
on_text_shadow_value_dialog_okbutton_clicked (GtkButton * button,
							gpointer user_data);

void
on_button_voice_family_value_dialog_add_clicked (GtkButton * button,
							gpointer user_data);
	
void
on_button_voice_family_value_dialog_del_clicked (GtkButton * button,
							gpointer user_data);

void
on_voice_family_value_dialog_okbutton_clicked (GtkButton * button,
							gpointer user_data);

void
on_border_width_style_color_value_dialog_helpbutton_clicked	(GtkButton * button, 
							gpointer user_data);

void
on_border_width_style_color_value_dialog_okbutton_clicked (GtkButton * button,
							gpointer user_data);

void
on_color_selector_dialog_helpbutton_clicked (GtkButton * button,
							gpointer user_data);

void
on_color_selector_dialog_okbutton_clicked (GtkButton * button,
							gpointer user_data);
	
void
on_button_font_fam_dialog_set_toggled     (GtkToggleButton *togglebutton,
                                            gpointer user_data);
											
void
on_border_style_dialog_okbutton_clicked(GtkButton * button,
							gpointer user_data);
void
on_border_style_dialog_helpbutton_clicked (GtkButton * button,
							gpointer user_data);

/* called by all dialogs */
void
on_css_dialog_cancel_button_clicked (GtkButton * button,
							gpointer user_data);

void
on_css_dialog_close                    (GtkDialog       *dialog,
                                        gpointer         user_data);
										
/* called to change the unit in a spin box */
void
on_css_dialog_unit_changed				(GtkEditable *editable,
                                            gpointer user_data);
