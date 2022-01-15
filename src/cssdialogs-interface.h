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

GtkWidget* create_angle_property_dialog(CssedWindow * window,
							  gchar * previous_string, 
							  gboolean add_previous_string,
							  gboolean connect_callbacks);
GtkWidget* create_background_property_dialog (CssedWindow * window,
							  gchar * previous_string, 
							  gboolean add_previous_string,
							  gboolean connect_callbacks);
GtkWidget* create_border_properties_dialog (CssedWindow * window,
							  gchar * previous_string, 
							  gboolean add_previous_string,
							  gboolean connect_callbacks);
GtkWidget* create_border_width_properties_dialog (CssedWindow * window,
							  gchar * previous_string, 
							  gboolean add_previous_string,
							  gboolean connect_callbacks);
GtkWidget* create_border_width_single_properties_dialog (CssedWindow * window,
							  gchar * previous_string, 
							  gboolean add_previous_string,
							  gboolean connect_callbacks);
GtkWidget* create_counter_increment_reset_dialog (CssedWindow * window,
							  gchar * previous_string, 
							  gboolean add_previous_string,
							  gboolean connect_callbacks);
GtkWidget* create_string_properties_dialog (CssedWindow * window,
							  gchar * previous_string, 
							  gboolean add_previous_string,
							  gboolean connect_callbacks);
GtkWidget* create_uri_value_dialog (CssedWindow * window,
							  gchar * previous_string, 
							  gboolean add_previous_string,
							  gboolean connect_callbacks);
GtkWidget* create_attr_value_dialog (CssedWindow * window,
							  gchar * previous_string, 
							  gboolean add_previous_string,
							  gboolean connect_callbacks);
GtkWidget* create_counter_property_dialog (CssedWindow * window,
							  gchar * previous_string, 
							  gboolean add_previous_string,
							  gboolean connect_callbacks);
GtkWidget* create_font_property_dialog (CssedWindow * window,
							  gchar * previous_string, 
							  gboolean add_previous_string,
							  gboolean connect_callbacks);
GtkWidget* create_font_family_dialog (CssedWindow * window,
							  gchar * previous_string, 
							  gboolean add_previous_string,
							  gboolean connect_callbacks);
GtkWidget* create_font_size_dialog (CssedWindow * window,
							  gchar * previous_string, 
							  gboolean add_previous_string,
							  gboolean connect_callbacks);
GtkWidget* create_frecuency_property_dialog (CssedWindow * window,
							  gchar * previous_string, 
							  gboolean add_previous_string,
							  gboolean connect_callbacks);
GtkWidget* create_identifier_value_dialog (CssedWindow * window,
							  gchar * previous_string, 
							  gboolean add_previous_string,
							  gboolean connect_callbacks);
GtkWidget* create_integer_value_dialog (CssedWindow * window,
							  gchar * previous_string, 
							  gboolean add_previous_string,
							  gboolean connect_callbacks);
GtkWidget* create_length_value_dialog (CssedWindow * window,
							  gchar * previous_string, 
							  gboolean add_previous_string,
							  gboolean connect_callbacks);
GtkWidget* create_list_style_dialog (CssedWindow * window,
							  gchar * previous_string, 
							  gboolean add_previous_string,
							  gboolean connect_callbacks);
GtkWidget* create_margin_property_dialog (CssedWindow * window,
							  gchar * previous_string, 
							  gboolean add_previous_string,
							  gboolean connect_callbacks);
GtkWidget* create_margin_width_value_dialog (CssedWindow * window,
							  gchar * previous_string, 
							  gboolean add_previous_string,
							  gboolean connect_callbacks);
GtkWidget* create_number_value_dialog (CssedWindow * window,
							  gchar * previous_string, 
							  gboolean add_previous_string,
							  gboolean connect_callbacks);
GtkWidget* create_padding_property_dialog (CssedWindow * window,
							  gchar * previous_string, 
							  gboolean add_previous_string,
							  gboolean connect_callbacks);
GtkWidget* create_outline_property_dialog (CssedWindow * window,
							  gchar * previous_string, 
							  gboolean add_previous_string,
							  gboolean connect_callbacks);
GtkWidget* create_pause_property_dialog (CssedWindow * window,
							  gchar * previous_string, 
							  gboolean add_previous_string,
							  gboolean connect_callbacks);
GtkWidget* create_percentage_value_dialog (CssedWindow * window,
							  gchar * previous_string, 
							  gboolean add_previous_string,
							  gboolean connect_callbacks);
GtkWidget* create_percentage_length_value_dialog (CssedWindow * window,
							  gchar * previous_string, 
							  gboolean add_previous_string,
							  gboolean connect_callbacks);
GtkWidget* create_play_during_property_dialog (CssedWindow * window,
							  gchar * previous_string, 
							  gboolean add_previous_string,
							  gboolean connect_callbacks);
GtkWidget* create_quotes_property_dialog (CssedWindow * window,
							  gchar * previous_string, 
							  gboolean add_previous_string,
							  gboolean connect_callbacks);
GtkWidget* create_shape_value_dialog (CssedWindow * window,
							  gchar * previous_string, 
							  gboolean add_previous_string,
							  gboolean connect_callbacks);
GtkWidget* create_size_length_value_dialog (CssedWindow * window,
							  gchar * previous_string, 
							  gboolean add_previous_string,
							  gboolean connect_callbacks);
GtkWidget* create_text_shadow_value_dialog (CssedWindow * window,
							  gchar * previous_string, 
							  gboolean add_previous_string,
							  gboolean connect_callbacks);
GtkWidget* create_voice_family_value_dialog (CssedWindow * window,
							  gchar * previous_string, 
							  gboolean add_previous_string,
							  gboolean connect_callbacks);
GtkWidget* create_border_width_style_color_value_dialog (CssedWindow * window,
							  gchar * previous_string, 
							  gboolean add_previous_string,
							  gboolean connect_callbacks);
GtkWidget* create_color_selector_dialog (CssedWindow * window,
							  gchar * previous_string, 
							  gboolean add_previous_string,
							  gboolean connect_callbacks);
GtkWidget* create_border_style_dialog (CssedWindow * window,
							  gchar * previous_string, 
							  gboolean add_previous_string,
							  gboolean connect_callbacks);
