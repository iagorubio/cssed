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

#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <stdio.h>

#include <gdk/gdkkeysyms.h>
#include <gtk/gtk.h>

#include "cssedwindow.h"
#include "document.h"
#include "cssdialogs-interface.h"
#include "support.h"

#define GLADE_HOOKUP_OBJECT(component,widget,name) \
  g_object_set_data_full (G_OBJECT (component), name, \
    gtk_widget_ref (widget), (GDestroyNotify) gtk_widget_unref)

#define GLADE_HOOKUP_OBJECT_NO_REF(component,widget,name) \
  g_object_set_data (G_OBJECT (component), name, widget)
  
void
on_css_dialogs_angle_activate          (GtkMenuItem     *menuitem,
                                        gpointer         user_data);
void
on_css_dialogs_border_all_activate     (GtkMenuItem     *menuitem,
                                        gpointer         user_data);
void
on_css_menu_background_activate        (GtkMenuItem     *menuitem,
                                        gpointer         user_data);
void
on_css_menu_background_pair_activate   (GtkMenuItem     *menuitem,
                                        gpointer         user_data);
void
on_css_menu_border_all_activate        (GtkMenuItem     *menuitem,
                                        gpointer         user_data);
void
on_css_menu_border_width_all_activate  (GtkMenuItem     *menuitem,
                                        gpointer         user_data);
void
on_css_menu_border_width_one_activate  (GtkMenuItem     *menuitem,
                                        gpointer         user_data);
										
void										
on_css_menu_border_width_one_add_value_activate(GtkMenuItem     *menuitem,
                                        gpointer         user_data);
void
on_css_menu_border_wsc_activate        (GtkMenuItem     *menuitem,
                                        gpointer         user_data);
void
on_css_menu_counter_prop_activate      (GtkMenuItem     *menuitem,
                                        gpointer         user_data);
void
on_css_menu_increment_or_reset_activate
                                        (GtkMenuItem     *menuitem,
                                        gpointer         user_data);
void
on_css_menu_color_activate             (GtkMenuItem     *menuitem,
                                        gpointer         user_data);
void
on_css_menu_font_value_activate        (GtkMenuItem     *menuitem,
                                        gpointer         user_data);
void
on_css_menu_font_family_activate       (GtkMenuItem     *menuitem,
                                        gpointer         user_data)	;
void
on_css_menu_font_size_activate         (GtkMenuItem     *menuitem,
                                        gpointer         user_data)	;
void
on_css_menu_list_style_activate        (GtkMenuItem     *menuitem,
                                        gpointer         user_data)	;
void
on_css_menu_margin_value_activate      (GtkMenuItem     *menuitem,
                                        gpointer         user_data)	;
void
on_css_menu_margin_width_activate      (GtkMenuItem     *menuitem,
                                        gpointer         user_data)	;
void
on_css_menu_padding_activate           (GtkMenuItem     *menuitem,
                                        gpointer         user_data)	;
void
on_css_menu_pause_activate             (GtkMenuItem     *menuitem,
                                        gpointer         user_data)	;
void
on_css_menu_play_during_activate       (GtkMenuItem     *menuitem,
                                        gpointer         user_data)	;
void
on_css_manu_outline_activate           (GtkMenuItem     *menuitem,
                                        gpointer         user_data);
void
on_css_menu_quotes_activate            (GtkMenuItem     *menuitem,
                                        gpointer         user_data);
void
on_css_menu_shape_activate             (GtkMenuItem     *menuitem,
                                        gpointer         user_data)	;
void
on_css_dialogs_text_shadow_activate    (GtkMenuItem     *menuitem,
                                        gpointer         user_data)	;
void
on_css_menu_voice_family_activate      (GtkMenuItem     *menuitem,
                                        gpointer         user_data)	;
void
on_css_menu_percentage_activate        (GtkMenuItem     *menuitem,
                                        gpointer         user_data)	;
										
void
on_css_menu_percentage_or_length_activate
                                        (GtkMenuItem     *menuitem,
                                        gpointer         user_data);
void
on_css_menu_size_or_length_activate    (GtkMenuItem     *menuitem,
                                        gpointer         user_data);
void
on_css_menu_string_activate            (GtkMenuItem     *menuitem,
                                        gpointer         user_data)	;
void
on_css_menu_number_activate            (GtkMenuItem     *menuitem,
                                        gpointer         user_data)	;
void
on_css_menu_frequency_activate         (GtkMenuItem     *menuitem,
                                        gpointer         user_data)	;
void
on_css_menu_identifier_activate        (GtkMenuItem     *menuitem,
                                        gpointer         user_data)	;
void
on_css_menu_integer_activate           (GtkMenuItem     *menuitem,
                                        gpointer         user_data)	;
void
on_css_menu_length_activate            (GtkMenuItem     *menuitem,
                                        gpointer         user_data);
void
on_css_menu_url_activate               (GtkMenuItem     *menuitem,
                                        gpointer         user_data)	;
void
on_css_menu_attribute_activate         (GtkMenuItem     *menuitem,
                                        gpointer         user_data)	;
void
css_menu_border_pair_activate     (GtkMenuItem     *menuitem,
                                   gpointer         user_data);
void
on_css_menu_border_width_all_pair_activate
                                        (GtkMenuItem     *menuitem,
                                        gpointer         user_data);
void
on_css_menu_border_top_width_activate
                                        (GtkMenuItem     *menuitem,
                                        gpointer         user_data);
void
on_css_menu_border_right_width_activate
                                        (GtkMenuItem     *menuitem,
                                        gpointer         user_data);
void
on_css_menu_border_bottom_width_activate
                                        (GtkMenuItem     *menuitem,
                                        gpointer         user_data);
void
on_css_menu_border_left_width_activate
                                        (GtkMenuItem     *menuitem,
                                        gpointer         user_data);
void
on_css_menu_wsc_border_top_activate
                                        (GtkMenuItem     *menuitem,
                                        gpointer         user_data);
void
on_css_menu_border_right_activate (GtkMenuItem     *menuitem,
                                        gpointer         user_data);
void
on_css_menu_border_bottom_activate
                                        (GtkMenuItem     *menuitem,
                                        gpointer         user_data);
void
on_on_css_menu_border_left_activate
                                        (GtkMenuItem     *menuitem,
                                        gpointer         user_data);
void
on_css_menu_counter_pair_activate (GtkMenuItem     *menuitem,
                                        gpointer         user_data);
void
on_css_menu_increment             (GtkMenuItem     *menuitem,
                                        gpointer         user_data);
void
on_css_menu_counter_reset_activate
                                        (GtkMenuItem     *menuitem,
                                        gpointer         user_data);
void
on_css_menu_background_attachment_color_activate
                                        (GtkMenuItem     *menuitem,
                                        gpointer         user_data);
										void
on_css_menu_background_color_activate
                                        (GtkMenuItem     *menuitem,
                                        gpointer         user_data);
void
on_css_menu_border_color_activate (GtkMenuItem     *menuitem,
                                        gpointer         user_data);
void
on_css_menu_border_top_color_activate
                                        (GtkMenuItem     *menuitem,
                                        gpointer         user_data);
void
on_css_menu_border_right_color_activate
                                        (GtkMenuItem     *menuitem,
                                        gpointer         user_data);
void
on_css_menu_border_bottom_color_activate
                                        (GtkMenuItem     *menuitem,
                                        gpointer         user_data);
void
on_css_menu_border_left_color_activate
                                        (GtkMenuItem     *menuitem,
                                        gpointer         user_data);
void
on_css_menu_color_pair_activate   (GtkMenuItem     *menuitem,
                                        gpointer         user_data);
void
on_css_menu_outline_color_activate
                                        (GtkMenuItem     *menuitem,
                                        gpointer         user_data);
void
on_css_menu_font_pair_activate    (GtkMenuItem     *menuitem,
                                        gpointer         user_data);
void
on_css_menu_font_family_pair_activate
                                        (GtkMenuItem     *menuitem,
                                        gpointer         user_data);
void
on_css_menu_font_size_pair_activate
                                        (GtkMenuItem     *menuitem,
                                        gpointer         user_data);
void
on_css_menu_list_style_pair_activate
                                        (GtkMenuItem     *menuitem,
                                        gpointer         user_data);
void
on_css_menu_nargin_pair_activate  (GtkMenuItem     *menuitem,
                                        gpointer         user_data);
void
on_css_menu_margin_width_pair_activate
                                        (GtkMenuItem     *menuitem,
                                        gpointer         user_data);
void
on_css_menu_margin_top_activate   (GtkMenuItem     *menuitem,
                                        gpointer         user_data);
void
on_css_menu_margin_right_activate (GtkMenuItem     *menuitem,
                                        gpointer         user_data);
void
on_css_menu_margin_bottom_activate
                                        (GtkMenuItem     *menuitem,
                                        gpointer         user_data);
void
on_css_menu_margin_left_activate  (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
on_css_menu_pading_pair_activate  (GtkMenuItem     *menuitem,
                                        gpointer         user_data);
void
on_css_menu_padding_top_activate  (GtkMenuItem     *menuitem,
                                        gpointer         user_data);
void
on_css_menu_padding_right_activate
                                        (GtkMenuItem     *menuitem,
                                        gpointer         user_data);
void
on_css_menu_padding_bottom_activate
                                        (GtkMenuItem     *menuitem,
                                        gpointer         user_data);
void
on_css_menu_padding_left_activate (GtkMenuItem     *menuitem,
                                        gpointer         user_data);
void
on_css_menu_pause_pair_activate   (GtkMenuItem     *menuitem,
                                        gpointer         user_data);
										void
on_css_menu_play_during_pair_activate
                                        (GtkMenuItem     *menuitem,
                                        gpointer         user_data);
void
on_css_menu_outline_pair_activate (GtkMenuItem     *menuitem,
                                        gpointer         user_data);
void
on_css_menu_quotes_pair_activate  (GtkMenuItem     *menuitem,
                                        gpointer         user_data);
void
on_css_menu_clip_activate         (GtkMenuItem     *menuitem,
                                        gpointer         user_data);
void
on_css_menu_text_shadow_pair_activate
                                        (GtkMenuItem     *menuitem,
                                        gpointer         user_data);
void
on_voice_family_activate          (GtkMenuItem     *menuitem,
                                        gpointer         user_data);
										

GtkWidget*
create_css_dialogs_menu (CssedWindow* window_object, GtkAccelGroup* accels )
{

  GtkWidget *css_dialogs_menu;
  GtkWidget *css_menu_property_values;
  GtkWidget *css_menu_property_values_menu;
  GtkWidget *css_menu_background;
  GtkWidget *css_menu_background_menu;
  GtkWidget *css_menu_background_add_value;
  GtkWidget *css_menu_background_pair;
  GtkWidget *css_menu_border;
  GtkWidget *css_menu_border_menu;
  GtkWidget *css_menu_border_all;
  GtkWidget *css_menu_border_all_menu;
  GtkWidget *css_menu_border_add_value;
  GtkWidget *css_menu_border_pair;
  GtkWidget *css_menu_border_width_all;
  GtkWidget *css_menu_border_width_all_menu;
  GtkWidget *css_menu_border_width_all_add_value;
  GtkWidget *css_menu_border_width_all_pair;
  GtkWidget *css_menu_border_width_one;
  GtkWidget *css_menu_border_width_one_menu;
  GtkWidget *css_menu_border_width_one_add_value;
  GtkWidget *css_menu_border_top_width;
  GtkWidget *css_menu_border_right_width;
  GtkWidget *css_menu_border_bottom_width;
  GtkWidget *css_menu_border_left_width;
  GtkWidget *css_menu_border_wsc;
  GtkWidget *css_menu_border_wsc_menu;
  GtkWidget *css_menu_wsc_add_values;
  GtkWidget *css_menu_wsc_border_top;
  GtkWidget *css_menu_border_right;
  GtkWidget *css_menu_border_bottom;
  GtkWidget *on_css_menu_border_left;
  GtkWidget *css_menu_counter;
  GtkWidget *css_menu_counter_menu;
  GtkWidget *css_menu_counter_value;
  GtkWidget *css_menu_counter_pair;
  GtkWidget *css_menu_increment;
  GtkWidget *css_menu_counter_reset;
  GtkWidget *css_menu_color;
  GtkWidget *css_menu_color_menu;
  GtkWidget *css_menu_color_add_value;
  GtkWidget *css_menu_background_attachment_color;
  GtkWidget *css_menu_background_color;
  GtkWidget *css_menu_border_color;
  GtkWidget *css_menu_border_top_color;
  GtkWidget *css_menu_border_right_color;
  GtkWidget *css_menu_border_bottom_color;
  GtkWidget *css_menu_border_left_color;
  GtkWidget *css_menu_color_pair;
  GtkWidget *css_menu_outline_color;
  GtkWidget *css_menu_font;
  GtkWidget *css_menu_font_menu;
  GtkWidget *css_menu_font_value;
  GtkWidget *css_menu_font_value_menu;
  GtkWidget *css_menu_font_add_value;
  GtkWidget *css_menu_font_pair;
  GtkWidget *css_menu_font_family;
  GtkWidget *css_menu_font_family_menu;
  GtkWidget *css_menu_font_family_add_value;
  GtkWidget *css_menu_font_family_pair;
  GtkWidget *css_menu_font_size;
  GtkWidget *css_menu_font_size_menu;
  GtkWidget *css_menu_font_size_add_value;
  GtkWidget *css_menu_font_size_pair;
  GtkWidget *css_menu_list_style;
  GtkWidget *css_menu_list_style_menu;
  GtkWidget *css_menu_list_style_add_value;
  GtkWidget *css_menu_list_style_pair;
  GtkWidget *css_menu_margin;
  GtkWidget *css_menu_margin_menu;
  GtkWidget *css_menu_margin_value;
  GtkWidget *css_menu_margin_value_menu;
  GtkWidget *css_menu_margin_add_value;
  GtkWidget *css_menu_nargin_pair;
  GtkWidget *css_menu_margin_width;
  GtkWidget *css_menu_margin_width_menu;
  GtkWidget *css_menu_margin_width_add_value;
  GtkWidget *css_menu_margin_width_pair;
  GtkWidget *css_menu_margin_top;
  GtkWidget *css_menu_margin_right;
  GtkWidget *css_menu_margin_bottom;
  GtkWidget *css_menu_margin_left;
  GtkWidget *css_menu_padding;
  GtkWidget *css_menu_padding_menu;
  GtkWidget *css_menu_padding_add_value;
  GtkWidget *css_menu_pading_pair;
  GtkWidget *css_menu_padding_top;
  GtkWidget *css_menu_padding_right;
  GtkWidget *css_menu_padding_bottom;
  GtkWidget *css_menu_padding_left;
  GtkWidget *css_menu_pause;
  GtkWidget *css_menu_pause_menu;
  GtkWidget *css_menu_pause_add_value;
  GtkWidget *css_menu_pause_pair;
  GtkWidget *css_menu_play_during;
  GtkWidget *css_menu_play_during_menu;
  GtkWidget *on_css_play_during_add_value;
  GtkWidget *css_menu_play_during_pair;
  GtkWidget *css_manu_outline;
  GtkWidget *css_manu_outline_menu;
  GtkWidget *css_menu_outline_add_value;
  GtkWidget *css_menu_outline_pair;
  GtkWidget *css_menu_quotes;
  GtkWidget *css_menu_quotes_menu;
  GtkWidget *css_menu_quotes_add_value;
  GtkWidget *css_menu_quotes_pair;
  GtkWidget *css_menu_shape;
  GtkWidget *css_menu_shape_menu;
  GtkWidget *css_menu_shape_add_value;
  GtkWidget *css_menu_clip;
  GtkWidget *css_dialogs_text_shadow;
  GtkWidget *css_dialogs_text_shadow_menu;
  GtkWidget *css_menu_text_shadow_add_value;
  GtkWidget *css_menu_text_shadow_pair;
  GtkWidget *css_menu_voice_family;
  GtkWidget *css_menu_voice_family_menu;
  GtkWidget *css_menu_voice_family_add_value;
  GtkWidget *voice_family_pair;
  GtkWidget *css_menu_common_values;
  GtkWidget *css_menu_common_values_menu;
  GtkWidget *css_dialogs_angle;
  GtkWidget *css_menu_percentage;
  GtkWidget *css_menu_percentage_or_length;
  GtkWidget *css_menu_size_or_length;
  GtkWidget *css_menu_string;
  GtkWidget *css_menu_number;
  GtkWidget *css_menu_frequency;
  GtkWidget *css_menu_identifier;
  GtkWidget *css_menu_integer;
  GtkWidget *css_menu_length;
  GtkWidget *css_menu_functions;
  GtkWidget *css_menu_functions_menu;
  GtkWidget *css_menu_url;
  GtkWidget *css_menu_attribute;
  GtkTooltips *tooltips;

  tooltips = gtk_tooltips_new ();
  css_dialogs_menu = gtk_menu_new ();

  css_menu_property_values = gtk_menu_item_new_with_mnemonic (_("Properties and values"));
  gtk_widget_show (css_menu_property_values);
  gtk_container_add (GTK_CONTAINER (css_dialogs_menu), css_menu_property_values);

  css_menu_property_values_menu = gtk_menu_new ();
  gtk_menu_item_set_submenu (GTK_MENU_ITEM (css_menu_property_values), css_menu_property_values_menu);

  css_menu_background = gtk_menu_item_new_with_mnemonic (_("Background"));
  gtk_widget_show (css_menu_background);
  gtk_container_add (GTK_CONTAINER (css_menu_property_values_menu), css_menu_background);
  
 css_menu_background_menu = gtk_menu_new ();
  gtk_menu_item_set_submenu (GTK_MENU_ITEM (css_menu_background), css_menu_background_menu);

  css_menu_background_add_value = gtk_menu_item_new_with_mnemonic (_("Add value"));
  gtk_widget_show (css_menu_background_add_value);
  gtk_container_add (GTK_CONTAINER (css_menu_background_menu), css_menu_background_add_value);
  gtk_tooltips_set_tip (tooltips, css_menu_background_add_value, _("Set the background property's value"), NULL);
	
  css_menu_background_pair = gtk_menu_item_new_with_mnemonic (_("background"));
  gtk_widget_show (css_menu_background_pair);
  gtk_container_add (GTK_CONTAINER (css_menu_background_menu), css_menu_background_pair);
  gtk_tooltips_set_tip (tooltips, css_menu_background_pair, _("Set the width of all borders"), NULL);  

  css_menu_border = gtk_menu_item_new_with_mnemonic (_("Border"));
  gtk_widget_show (css_menu_border);
  gtk_container_add (GTK_CONTAINER (css_menu_property_values_menu), css_menu_border);
  gtk_tooltips_set_tip (tooltips, css_menu_border, _("Common values for borders"), NULL);

  css_menu_border_menu = gtk_menu_new ();
  gtk_menu_item_set_submenu (GTK_MENU_ITEM (css_menu_border), css_menu_border_menu);

  css_menu_border_all = gtk_menu_item_new_with_mnemonic (_("Border (all)"));
  gtk_widget_show (css_menu_border_all);
  gtk_container_add (GTK_CONTAINER (css_menu_border_menu), css_menu_border_all);
  gtk_tooltips_set_tip (tooltips, css_menu_border_all, _("All values used by the border property"), NULL);

  css_menu_border_all_menu = gtk_menu_new ();
  gtk_menu_item_set_submenu (GTK_MENU_ITEM (css_menu_border_all), css_menu_border_all_menu);

  css_menu_border_add_value = gtk_menu_item_new_with_mnemonic (_("Add value"));
  gtk_widget_show (css_menu_border_add_value);
  gtk_container_add (GTK_CONTAINER (css_menu_border_all_menu), css_menu_border_add_value);
  gtk_tooltips_set_tip (tooltips, css_menu_border_add_value, _("Add the value to the border property."), NULL);

  css_menu_border_pair = gtk_menu_item_new_with_mnemonic ("border");
  gtk_widget_show (css_menu_border_pair);
  gtk_container_add (GTK_CONTAINER (css_menu_border_all_menu), css_menu_border_pair);
  gtk_tooltips_set_tip (tooltips, css_menu_border_pair, _("Set the width, style and color for all borders."), NULL);

  css_menu_border_width_all = gtk_menu_item_new_with_mnemonic (_("Border width (all)"));
  gtk_widget_show (css_menu_border_width_all);
  gtk_container_add (GTK_CONTAINER (css_menu_border_menu), css_menu_border_width_all);
  gtk_tooltips_set_tip (tooltips, css_menu_border_width_all, _("Set border properties or values."), NULL);

  css_menu_border_width_all_menu = gtk_menu_new ();
  gtk_menu_item_set_submenu (GTK_MENU_ITEM (css_menu_border_width_all), css_menu_border_width_all_menu);

  css_menu_border_width_all_add_value = gtk_menu_item_new_with_mnemonic (_("Add value"));
  gtk_widget_show (css_menu_border_width_all_add_value);
  gtk_container_add (GTK_CONTAINER (css_menu_border_width_all_menu), css_menu_border_width_all_add_value);

  css_menu_border_width_all_pair = gtk_menu_item_new_with_mnemonic ("border-width");
  gtk_widget_show (css_menu_border_width_all_pair);
  gtk_container_add (GTK_CONTAINER (css_menu_border_width_all_menu), css_menu_border_width_all_pair);
  gtk_tooltips_set_tip (tooltips, css_menu_border_width_all_pair, _("Set the width of all borders"), NULL);

  css_menu_border_width_one = gtk_menu_item_new_with_mnemonic (_("Border width (one)"));
  gtk_widget_show (css_menu_border_width_one);
  gtk_container_add (GTK_CONTAINER (css_menu_border_menu), css_menu_border_width_one);
  gtk_tooltips_set_tip (tooltips, css_menu_border_width_one, _("Set width of one border, used by border-width-(top|left|right|bottom)"), NULL);

  css_menu_border_width_one_menu = gtk_menu_new ();
  gtk_menu_item_set_submenu (GTK_MENU_ITEM (css_menu_border_width_one), css_menu_border_width_one_menu);

  css_menu_border_width_one_add_value = gtk_menu_item_new_with_mnemonic (_("Add value"));
  gtk_widget_show (css_menu_border_width_one_add_value);
  gtk_container_add (GTK_CONTAINER (css_menu_border_width_one_menu), css_menu_border_width_one_add_value);
  gtk_tooltips_set_tip (tooltips, css_menu_border_width_one_add_value, _("Add just the value"), NULL);

  css_menu_border_top_width = gtk_menu_item_new_with_mnemonic ("border-top-width");
  gtk_widget_show (css_menu_border_top_width);
  gtk_container_add (GTK_CONTAINER (css_menu_border_width_one_menu), css_menu_border_top_width);
  gtk_tooltips_set_tip (tooltips, css_menu_border_top_width, _("Set the border width to the top border."), NULL);

  css_menu_border_right_width = gtk_menu_item_new_with_mnemonic ("border-right-width");
  gtk_widget_show (css_menu_border_right_width);
  gtk_container_add (GTK_CONTAINER (css_menu_border_width_one_menu), css_menu_border_right_width);
  gtk_tooltips_set_tip (tooltips, css_menu_border_right_width, _("Set the border width to the right border."), NULL);

  css_menu_border_bottom_width = gtk_menu_item_new_with_mnemonic ("border-bottom-width");
  gtk_widget_show (css_menu_border_bottom_width);
  gtk_container_add (GTK_CONTAINER (css_menu_border_width_one_menu), css_menu_border_bottom_width);
  gtk_tooltips_set_tip (tooltips, css_menu_border_bottom_width, _("Set the border width to the bottom border."), NULL);

  css_menu_border_left_width = gtk_menu_item_new_with_mnemonic ("border-left-width");
  gtk_widget_show (css_menu_border_left_width);
  gtk_container_add (GTK_CONTAINER (css_menu_border_width_one_menu), css_menu_border_left_width);
  gtk_tooltips_set_tip (tooltips, css_menu_border_left_width, _("Set the border width to the left border."), NULL);

  css_menu_border_wsc = gtk_menu_item_new_with_mnemonic (_("Border width style and color"));
  gtk_widget_show (css_menu_border_wsc);
  gtk_container_add (GTK_CONTAINER (css_menu_border_menu), css_menu_border_wsc);
  gtk_tooltips_set_tip (tooltips, css_menu_border_wsc, _("Set the width style and color of one border."), NULL);

  css_menu_border_wsc_menu = gtk_menu_new ();
  gtk_menu_item_set_submenu (GTK_MENU_ITEM (css_menu_border_wsc), css_menu_border_wsc_menu);

  css_menu_wsc_add_values = gtk_menu_item_new_with_mnemonic (_("Add value"));
  gtk_widget_show (css_menu_wsc_add_values);
  gtk_container_add (GTK_CONTAINER (css_menu_border_wsc_menu), css_menu_wsc_add_values);
  gtk_tooltips_set_tip (tooltips, css_menu_wsc_add_values, _("Add just the value "), NULL);

  css_menu_wsc_border_top = gtk_menu_item_new_with_mnemonic ("border-top");
  gtk_widget_show (css_menu_wsc_border_top);
  gtk_container_add (GTK_CONTAINER (css_menu_border_wsc_menu), css_menu_wsc_border_top);
  gtk_tooltips_set_tip (tooltips, css_menu_wsc_border_top, _("Set the width style and color in the top border"), NULL);

  css_menu_border_right = gtk_menu_item_new_with_mnemonic ("border-right");
  gtk_widget_show (css_menu_border_right);
  gtk_container_add (GTK_CONTAINER (css_menu_border_wsc_menu), css_menu_border_right);
  gtk_tooltips_set_tip (tooltips, css_menu_border_right, _("Set the width style and color to the right border"), NULL);

  css_menu_border_bottom = gtk_menu_item_new_with_mnemonic ("border-bottom");
  gtk_widget_show (css_menu_border_bottom);
  gtk_container_add (GTK_CONTAINER (css_menu_border_wsc_menu), css_menu_border_bottom);
  gtk_tooltips_set_tip (tooltips, css_menu_border_bottom, _("Set the width style and color to the bottom border"), NULL);

  on_css_menu_border_left = gtk_menu_item_new_with_mnemonic ("border-left");
  gtk_widget_show (on_css_menu_border_left);
  gtk_container_add (GTK_CONTAINER (css_menu_border_wsc_menu), on_css_menu_border_left);
  gtk_tooltips_set_tip (tooltips, on_css_menu_border_left, _("Set the width style and color to the left border"), NULL);

  css_menu_counter = gtk_menu_item_new_with_mnemonic (_("Counter"));
  gtk_widget_show (css_menu_counter);
  gtk_container_add (GTK_CONTAINER (css_menu_property_values_menu), css_menu_counter);

  css_menu_counter_menu = gtk_menu_new ();
  gtk_menu_item_set_submenu (GTK_MENU_ITEM (css_menu_counter), css_menu_counter_menu);

  css_menu_counter_value = gtk_menu_item_new_with_mnemonic (_("Add value"));
  gtk_widget_show (css_menu_counter_value);
  gtk_container_add (GTK_CONTAINER (css_menu_counter_menu), css_menu_counter_value);
  gtk_tooltips_set_tip (tooltips, css_menu_counter_value, _("Set the values of the counter property"), NULL);

  css_menu_counter_pair = gtk_menu_item_new_with_mnemonic ("counter");
  gtk_widget_show (css_menu_counter_pair);
  gtk_container_add (GTK_CONTAINER (css_menu_counter_menu), css_menu_counter_pair);

  css_menu_increment = gtk_menu_item_new_with_mnemonic ("counter-increment");
  gtk_widget_show (css_menu_increment);
  gtk_container_add (GTK_CONTAINER (css_menu_counter_menu), css_menu_increment);
  gtk_tooltips_set_tip (tooltips, css_menu_increment, _("Set the increment of a counter, used by counter-(increment|reset)"), NULL);

  css_menu_counter_reset = gtk_menu_item_new_with_mnemonic ("counter-reset");
  gtk_widget_show (css_menu_counter_reset);
  gtk_container_add (GTK_CONTAINER (css_menu_counter_menu), css_menu_counter_reset);

  css_menu_color = gtk_menu_item_new_with_mnemonic (_("Color"));
  gtk_widget_show (css_menu_color);
  gtk_container_add (GTK_CONTAINER (css_menu_property_values_menu), css_menu_color);
  gtk_tooltips_set_tip (tooltips, css_menu_color, _("Set a color"), NULL);

  css_menu_color_menu = gtk_menu_new ();
  gtk_menu_item_set_submenu (GTK_MENU_ITEM (css_menu_color), css_menu_color_menu);

  css_menu_color_add_value = gtk_menu_item_new_with_mnemonic (_("Add value"));
  gtk_widget_show (css_menu_color_add_value);
  gtk_container_add (GTK_CONTAINER (css_menu_color_menu), css_menu_color_add_value);
  gtk_tooltips_set_tip (tooltips, css_menu_color_add_value, _("Add a color value to an existing property."), NULL);

  css_menu_background_attachment_color = gtk_menu_item_new_with_mnemonic ("background-attachment");
  gtk_widget_show (css_menu_background_attachment_color);
  gtk_container_add (GTK_CONTAINER (css_menu_color_menu), css_menu_background_attachment_color);
  gtk_tooltips_set_tip (tooltips, css_menu_background_attachment_color, _("Set the color of the background attachment"), NULL);

  css_menu_background_color = gtk_menu_item_new_with_mnemonic ("background-color");
  gtk_widget_show (css_menu_background_color);
  gtk_container_add (GTK_CONTAINER (css_menu_color_menu), css_menu_background_color);
  gtk_tooltips_set_tip (tooltips, css_menu_background_color, _("Set the background's color"), NULL);

  css_menu_border_color = gtk_menu_item_new_with_mnemonic ("border-color");
  gtk_widget_show (css_menu_border_color);
  gtk_container_add (GTK_CONTAINER (css_menu_color_menu), css_menu_border_color);
  gtk_tooltips_set_tip (tooltips, css_menu_border_color, _("Set the color of all borders"), NULL);

  css_menu_border_top_color = gtk_menu_item_new_with_mnemonic ("border-top-color");
  gtk_widget_show (css_menu_border_top_color);
  gtk_container_add (GTK_CONTAINER (css_menu_color_menu), css_menu_border_top_color);
  gtk_tooltips_set_tip (tooltips, css_menu_border_top_color, _("Set the color of the top border"), NULL);

  css_menu_border_right_color = gtk_menu_item_new_with_mnemonic ("border-right-color");
  gtk_widget_show (css_menu_border_right_color);
  gtk_container_add (GTK_CONTAINER (css_menu_color_menu), css_menu_border_right_color);
  gtk_tooltips_set_tip (tooltips, css_menu_border_right_color, _("Set the color of the right border"), NULL);

  css_menu_border_bottom_color = gtk_menu_item_new_with_mnemonic ("border-bottom-color");
  gtk_widget_show (css_menu_border_bottom_color);
  gtk_container_add (GTK_CONTAINER (css_menu_color_menu), css_menu_border_bottom_color);
  gtk_tooltips_set_tip (tooltips, css_menu_border_bottom_color, _("Set the color of the bottom border"), NULL);

  css_menu_border_left_color = gtk_menu_item_new_with_mnemonic ("border-left-color");
  gtk_widget_show (css_menu_border_left_color);
  gtk_container_add (GTK_CONTAINER (css_menu_color_menu), css_menu_border_left_color);
  gtk_tooltips_set_tip (tooltips, css_menu_border_left_color, _("Set the color of the left border"), NULL);

  css_menu_color_pair = gtk_menu_item_new_with_mnemonic ("color");
  gtk_widget_show (css_menu_color_pair);
  gtk_container_add (GTK_CONTAINER (css_menu_color_menu), css_menu_color_pair);
  gtk_tooltips_set_tip (tooltips, css_menu_color_pair, _("Set the text color"), NULL);

  css_menu_outline_color = gtk_menu_item_new_with_mnemonic ("outline-color");
  gtk_widget_show (css_menu_outline_color);
  gtk_container_add (GTK_CONTAINER (css_menu_color_menu), css_menu_outline_color);
  gtk_tooltips_set_tip (tooltips, css_menu_outline_color, _("Set the outline color"), NULL);

  css_menu_font = gtk_menu_item_new_with_mnemonic (_("Font"));
  gtk_widget_show (css_menu_font);
  gtk_container_add (GTK_CONTAINER (css_menu_property_values_menu), css_menu_font);

  css_menu_font_menu = gtk_menu_new ();
  gtk_menu_item_set_submenu (GTK_MENU_ITEM (css_menu_font), css_menu_font_menu);

  css_menu_font_value = gtk_menu_item_new_with_mnemonic (_("Font"));
  gtk_widget_show (css_menu_font_value);
  gtk_container_add (GTK_CONTAINER (css_menu_font_menu), css_menu_font_value);
  gtk_tooltips_set_tip (tooltips, css_menu_font_value, _("Set the font values, used by font property"), NULL);

  css_menu_font_value_menu = gtk_menu_new ();
  gtk_menu_item_set_submenu (GTK_MENU_ITEM (css_menu_font_value), css_menu_font_value_menu);

  css_menu_font_add_value = gtk_menu_item_new_with_mnemonic (_("Add value"));
  gtk_widget_show (css_menu_font_add_value);
  gtk_container_add (GTK_CONTAINER (css_menu_font_value_menu), css_menu_font_add_value);

  css_menu_font_pair = gtk_menu_item_new_with_mnemonic ("font");
  gtk_widget_show (css_menu_font_pair);
  gtk_container_add (GTK_CONTAINER (css_menu_font_value_menu), css_menu_font_pair);

  css_menu_font_family = gtk_menu_item_new_with_mnemonic (_("Font family"));
  gtk_widget_show (css_menu_font_family);
  gtk_container_add (GTK_CONTAINER (css_menu_font_menu), css_menu_font_family);
  gtk_tooltips_set_tip (tooltips, css_menu_font_family, _("Set the font's family, used by the font-family property"), NULL);

  css_menu_font_family_menu = gtk_menu_new ();
  gtk_menu_item_set_submenu (GTK_MENU_ITEM (css_menu_font_family), css_menu_font_family_menu);

  css_menu_font_family_add_value = gtk_menu_item_new_with_mnemonic (_("Add value"));
  gtk_widget_show (css_menu_font_family_add_value);
  gtk_container_add (GTK_CONTAINER (css_menu_font_family_menu), css_menu_font_family_add_value);
  gtk_tooltips_set_tip (tooltips, css_menu_font_family_add_value, _("Add the value to an existing font-family property"), NULL);

  css_menu_font_family_pair = gtk_menu_item_new_with_mnemonic ("font-family");
  gtk_widget_show (css_menu_font_family_pair);
  gtk_container_add (GTK_CONTAINER (css_menu_font_family_menu), css_menu_font_family_pair);
  gtk_tooltips_set_tip (tooltips, css_menu_font_family_pair, _("Set the font family"), NULL);

  css_menu_font_size = gtk_menu_item_new_with_mnemonic (_("Font size"));
  gtk_widget_show (css_menu_font_size);
  gtk_container_add (GTK_CONTAINER (css_menu_font_menu), css_menu_font_size);
  gtk_tooltips_set_tip (tooltips, css_menu_font_size, _("Set the font size, used by the font-size property."), NULL);

  css_menu_font_size_menu = gtk_menu_new ();
  gtk_menu_item_set_submenu (GTK_MENU_ITEM (css_menu_font_size), css_menu_font_size_menu);

  css_menu_font_size_add_value = gtk_menu_item_new_with_mnemonic (_("Add value"));
  gtk_widget_show (css_menu_font_size_add_value);
  gtk_container_add (GTK_CONTAINER (css_menu_font_size_menu), css_menu_font_size_add_value);
  gtk_tooltips_set_tip (tooltips, css_menu_font_size_add_value, _("Add the value to an existing font-size property"), NULL);

  css_menu_font_size_pair = gtk_menu_item_new_with_mnemonic ("font-size");
  gtk_widget_show (css_menu_font_size_pair);
  gtk_container_add (GTK_CONTAINER (css_menu_font_size_menu), css_menu_font_size_pair);
  gtk_tooltips_set_tip (tooltips, css_menu_font_size_pair, _("Set the font size"), NULL);

  css_menu_list_style = gtk_menu_item_new_with_mnemonic (_("List style"));
  gtk_widget_show (css_menu_list_style);
  gtk_container_add (GTK_CONTAINER (css_menu_property_values_menu), css_menu_list_style);
  gtk_tooltips_set_tip (tooltips, css_menu_list_style, _("Set the list style, used by list-style"), NULL);

  css_menu_list_style_menu = gtk_menu_new ();
  gtk_menu_item_set_submenu (GTK_MENU_ITEM (css_menu_list_style), css_menu_list_style_menu);

  css_menu_list_style_add_value = gtk_menu_item_new_with_mnemonic (_("Add value"));
  gtk_widget_show (css_menu_list_style_add_value);
  gtk_container_add (GTK_CONTAINER (css_menu_list_style_menu), css_menu_list_style_add_value);

  css_menu_list_style_pair = gtk_menu_item_new_with_mnemonic ("list-style");
  gtk_widget_show (css_menu_list_style_pair);
  gtk_container_add (GTK_CONTAINER (css_menu_list_style_menu), css_menu_list_style_pair);
  gtk_tooltips_set_tip (tooltips, css_menu_list_style_pair, _("Set the list style"), NULL);

  css_menu_margin = gtk_menu_item_new_with_mnemonic (_("Margin"));
  gtk_widget_show (css_menu_margin);
  gtk_container_add (GTK_CONTAINER (css_menu_property_values_menu), css_menu_margin);

  css_menu_margin_menu = gtk_menu_new ();
  gtk_menu_item_set_submenu (GTK_MENU_ITEM (css_menu_margin), css_menu_margin_menu);

  css_menu_margin_value = gtk_menu_item_new_with_mnemonic (_("Margin"));
  gtk_widget_show (css_menu_margin_value);
  gtk_container_add (GTK_CONTAINER (css_menu_margin_menu), css_menu_margin_value);
  gtk_tooltips_set_tip (tooltips, css_menu_margin_value, _("Set the value of all margins, used by margin"), NULL);

  css_menu_margin_value_menu = gtk_menu_new ();
  gtk_menu_item_set_submenu (GTK_MENU_ITEM (css_menu_margin_value), css_menu_margin_value_menu);

  css_menu_margin_add_value = gtk_menu_item_new_with_mnemonic (_("Add value"));
  gtk_widget_show (css_menu_margin_add_value);
  gtk_container_add (GTK_CONTAINER (css_menu_margin_value_menu), css_menu_margin_add_value);
  gtk_tooltips_set_tip (tooltips, css_menu_margin_add_value, _("Add the value to existing margin property."), NULL);

  css_menu_nargin_pair = gtk_menu_item_new_with_mnemonic ("margin");
  gtk_widget_show (css_menu_nargin_pair);
  gtk_container_add (GTK_CONTAINER (css_menu_margin_value_menu), css_menu_nargin_pair);

  css_menu_margin_width = gtk_menu_item_new_with_mnemonic (_("Margin width"));
  gtk_widget_show (css_menu_margin_width);
  gtk_container_add (GTK_CONTAINER (css_menu_margin_menu), css_menu_margin_width);
  gtk_tooltips_set_tip (tooltips, css_menu_margin_width, _("Set the width of one margin, used by margin-(top|left|bottom|right)"), NULL);

  css_menu_margin_width_menu = gtk_menu_new ();
  gtk_menu_item_set_submenu (GTK_MENU_ITEM (css_menu_margin_width), css_menu_margin_width_menu);

  css_menu_margin_width_add_value = gtk_menu_item_new_with_mnemonic (_("Add value"));
  gtk_widget_show (css_menu_margin_width_add_value);
  gtk_container_add (GTK_CONTAINER (css_menu_margin_width_menu), css_menu_margin_width_add_value);
  gtk_tooltips_set_tip (tooltips, css_menu_margin_width_add_value, _("Add the margin width value to an existing margin-width property"), NULL);

  css_menu_margin_width_pair = gtk_menu_item_new_with_mnemonic ("margin-width");
  gtk_widget_show (css_menu_margin_width_pair);
  gtk_container_add (GTK_CONTAINER (css_menu_margin_width_menu), css_menu_margin_width_pair);
  gtk_tooltips_set_tip (tooltips, css_menu_margin_width_pair, _("Set the width for all margins"), NULL);

  css_menu_margin_top = gtk_menu_item_new_with_mnemonic ("margin-top");
  gtk_widget_show (css_menu_margin_top);
  gtk_container_add (GTK_CONTAINER (css_menu_margin_width_menu), css_menu_margin_top);
  gtk_tooltips_set_tip (tooltips, css_menu_margin_top, _("Set the top margin's width."), NULL);

  css_menu_margin_right = gtk_menu_item_new_with_mnemonic ("margin-right");
  gtk_widget_show (css_menu_margin_right);
  gtk_container_add (GTK_CONTAINER (css_menu_margin_width_menu), css_menu_margin_right);
  gtk_tooltips_set_tip (tooltips, css_menu_margin_right, _("Set the right margin's width."), NULL);

  css_menu_margin_bottom = gtk_menu_item_new_with_mnemonic ("margin-bottom");
  gtk_widget_show (css_menu_margin_bottom);
  gtk_container_add (GTK_CONTAINER (css_menu_margin_width_menu), css_menu_margin_bottom);
  gtk_tooltips_set_tip (tooltips, css_menu_margin_bottom, _("Set the bottom margin's width."), NULL);

  css_menu_margin_left = gtk_menu_item_new_with_mnemonic ("margin-left");
  gtk_widget_show (css_menu_margin_left);
  gtk_container_add (GTK_CONTAINER (css_menu_margin_width_menu), css_menu_margin_left);
  gtk_tooltips_set_tip (tooltips, css_menu_margin_left, _("Set the left margin's width."), NULL);

  css_menu_padding = gtk_menu_item_new_with_mnemonic (_("Padding"));
  gtk_widget_show (css_menu_padding);
  gtk_container_add (GTK_CONTAINER (css_menu_property_values_menu), css_menu_padding);
  gtk_tooltips_set_tip (tooltips, css_menu_padding, _("Used by padding-(top|left|bottom|right)"), NULL);

  css_menu_padding_menu = gtk_menu_new ();
  gtk_menu_item_set_submenu (GTK_MENU_ITEM (css_menu_padding), css_menu_padding_menu);

  css_menu_padding_add_value = gtk_menu_item_new_with_mnemonic (_("Add value"));
  gtk_widget_show (css_menu_padding_add_value);
  gtk_container_add (GTK_CONTAINER (css_menu_padding_menu), css_menu_padding_add_value);
  gtk_tooltips_set_tip (tooltips, css_menu_padding_add_value, _("on_css_menu_padding_activate"), NULL);

  css_menu_pading_pair = gtk_menu_item_new_with_mnemonic ("padding");
  gtk_widget_show (css_menu_pading_pair);
  gtk_container_add (GTK_CONTAINER (css_menu_padding_menu), css_menu_pading_pair);
  gtk_tooltips_set_tip (tooltips, css_menu_pading_pair, _("Set the padding for all sides."), NULL);

  css_menu_padding_top = gtk_menu_item_new_with_mnemonic ("padding-top");
  gtk_widget_show (css_menu_padding_top);
  gtk_container_add (GTK_CONTAINER (css_menu_padding_menu), css_menu_padding_top);

  css_menu_padding_right = gtk_menu_item_new_with_mnemonic ("padding-right");
  gtk_widget_show (css_menu_padding_right);
  gtk_container_add (GTK_CONTAINER (css_menu_padding_menu), css_menu_padding_right);

  css_menu_padding_bottom = gtk_menu_item_new_with_mnemonic ("padding-bottom");
  gtk_widget_show (css_menu_padding_bottom);
  gtk_container_add (GTK_CONTAINER (css_menu_padding_menu), css_menu_padding_bottom);

  css_menu_padding_left = gtk_menu_item_new_with_mnemonic ("padding-left");
  gtk_widget_show (css_menu_padding_left);
  gtk_container_add (GTK_CONTAINER (css_menu_padding_menu), css_menu_padding_left);

  css_menu_pause = gtk_menu_item_new_with_mnemonic (_("Pause"));
  gtk_widget_show (css_menu_pause);
  gtk_container_add (GTK_CONTAINER (css_menu_property_values_menu), css_menu_pause);
  gtk_tooltips_set_tip (tooltips, css_menu_pause, _("Set a pause in aural stylesheets"), NULL);

  css_menu_pause_menu = gtk_menu_new ();
  gtk_menu_item_set_submenu (GTK_MENU_ITEM (css_menu_pause), css_menu_pause_menu);

  css_menu_pause_add_value = gtk_menu_item_new_with_mnemonic (_("Add value"));
  gtk_widget_show (css_menu_pause_add_value);
  gtk_container_add (GTK_CONTAINER (css_menu_pause_menu), css_menu_pause_add_value);
  gtk_tooltips_set_tip (tooltips, css_menu_pause_add_value, _("Add the value to an existing pause property."), NULL);

  css_menu_pause_pair = gtk_menu_item_new_with_mnemonic ("pause");
  gtk_widget_show (css_menu_pause_pair);
  gtk_container_add (GTK_CONTAINER (css_menu_pause_menu), css_menu_pause_pair);
  gtk_tooltips_set_tip (tooltips, css_menu_pause_pair, _("Add the pause property and value"), NULL);

  css_menu_play_during = gtk_menu_item_new_with_mnemonic (_("Play during"));
  gtk_widget_show (css_menu_play_during);
  gtk_container_add (GTK_CONTAINER (css_menu_property_values_menu), css_menu_play_during);
  gtk_tooltips_set_tip (tooltips, css_menu_play_during, _("Play a sound file"), NULL);

  css_menu_play_during_menu = gtk_menu_new ();
  gtk_menu_item_set_submenu (GTK_MENU_ITEM (css_menu_play_during), css_menu_play_during_menu);

  on_css_play_during_add_value = gtk_menu_item_new_with_mnemonic (_("Add value"));
  gtk_widget_show (on_css_play_during_add_value);
  gtk_container_add (GTK_CONTAINER (css_menu_play_during_menu), on_css_play_during_add_value);
  gtk_tooltips_set_tip (tooltips, on_css_play_during_add_value, _("Add the value to an existing play-during property"), NULL);

  css_menu_play_during_pair = gtk_menu_item_new_with_mnemonic ("play-during");
  gtk_widget_show (css_menu_play_during_pair);
  gtk_container_add (GTK_CONTAINER (css_menu_play_during_menu), css_menu_play_during_pair);

  css_manu_outline = gtk_menu_item_new_with_mnemonic (_("Outline"));
  gtk_widget_show (css_manu_outline);
  gtk_container_add (GTK_CONTAINER (css_menu_property_values_menu), css_manu_outline);
  gtk_tooltips_set_tip (tooltips, css_manu_outline, _("Set the outline value, used by outline"), NULL);

  css_manu_outline_menu = gtk_menu_new ();
  gtk_menu_item_set_submenu (GTK_MENU_ITEM (css_manu_outline), css_manu_outline_menu);

  css_menu_outline_add_value = gtk_menu_item_new_with_mnemonic (_("Add value"));
  gtk_widget_show (css_menu_outline_add_value);
  gtk_container_add (GTK_CONTAINER (css_manu_outline_menu), css_menu_outline_add_value);

  css_menu_outline_pair = gtk_menu_item_new_with_mnemonic ("outline");
  gtk_widget_show (css_menu_outline_pair);
  gtk_container_add (GTK_CONTAINER (css_manu_outline_menu), css_menu_outline_pair);

  css_menu_quotes = gtk_menu_item_new_with_mnemonic (_("Quotes"));
  gtk_widget_show (css_menu_quotes);
  gtk_container_add (GTK_CONTAINER (css_menu_property_values_menu), css_menu_quotes);
  gtk_tooltips_set_tip (tooltips, css_menu_quotes, _("Set the open and closing quotes. Usually used with languages."), NULL);

  css_menu_quotes_menu = gtk_menu_new ();
  gtk_menu_item_set_submenu (GTK_MENU_ITEM (css_menu_quotes), css_menu_quotes_menu);

  css_menu_quotes_add_value = gtk_menu_item_new_with_mnemonic (_("Add value"));
  gtk_widget_show (css_menu_quotes_add_value);
  gtk_container_add (GTK_CONTAINER (css_menu_quotes_menu), css_menu_quotes_add_value);
  gtk_tooltips_set_tip (tooltips, css_menu_quotes_add_value, _("Add the value to the opening and closing quotes to existing quotes property"), NULL);

  css_menu_quotes_pair = gtk_menu_item_new_with_mnemonic ("quotes");
  gtk_widget_show (css_menu_quotes_pair);
  gtk_container_add (GTK_CONTAINER (css_menu_quotes_menu), css_menu_quotes_pair);

  css_menu_shape = gtk_menu_item_new_with_mnemonic (_("Shape"));
  gtk_widget_show (css_menu_shape);
  gtk_container_add (GTK_CONTAINER (css_menu_property_values_menu), css_menu_shape);
  gtk_tooltips_set_tip (tooltips, css_menu_shape, _("Set the shape's function parameters. Used by the clip property."), NULL);

  css_menu_shape_menu = gtk_menu_new ();
  gtk_menu_item_set_submenu (GTK_MENU_ITEM (css_menu_shape), css_menu_shape_menu);

  css_menu_shape_add_value = gtk_menu_item_new_with_mnemonic (_("Add value"));
  gtk_widget_show (css_menu_shape_add_value);
  gtk_container_add (GTK_CONTAINER (css_menu_shape_menu), css_menu_shape_add_value);
  gtk_tooltips_set_tip (tooltips, css_menu_shape_add_value, _("Add the value to an existing clip property."), NULL);

  css_menu_clip = gtk_menu_item_new_with_mnemonic ("clip");
  gtk_widget_show (css_menu_clip);
  gtk_container_add (GTK_CONTAINER (css_menu_shape_menu), css_menu_clip);
  gtk_tooltips_set_tip (tooltips, css_menu_clip, _("Add the clip property's shape value."), NULL);

  css_dialogs_text_shadow = gtk_menu_item_new_with_mnemonic (_("Text shadow"));
  gtk_widget_show (css_dialogs_text_shadow);
  gtk_container_add (GTK_CONTAINER (css_menu_property_values_menu), css_dialogs_text_shadow);
  gtk_tooltips_set_tip (tooltips, css_dialogs_text_shadow, _("Set the text's shadow"), NULL);

  css_dialogs_text_shadow_menu = gtk_menu_new ();
  gtk_menu_item_set_submenu (GTK_MENU_ITEM (css_dialogs_text_shadow), css_dialogs_text_shadow_menu);

  css_menu_text_shadow_add_value = gtk_menu_item_new_with_mnemonic (_("Add value"));
  gtk_widget_show (css_menu_text_shadow_add_value);
  gtk_container_add (GTK_CONTAINER (css_dialogs_text_shadow_menu), css_menu_text_shadow_add_value);
  gtk_tooltips_set_tip (tooltips, css_menu_text_shadow_add_value, _("Add the value to an existing text-shadow property"), NULL);

  css_menu_text_shadow_pair = gtk_menu_item_new_with_mnemonic ("text-shadow");
  gtk_widget_show (css_menu_text_shadow_pair);
  gtk_container_add (GTK_CONTAINER (css_dialogs_text_shadow_menu), css_menu_text_shadow_pair);

  css_menu_voice_family = gtk_menu_item_new_with_mnemonic (_("Voice family"));
  gtk_widget_show (css_menu_voice_family);
  gtk_container_add (GTK_CONTAINER (css_menu_property_values_menu), css_menu_voice_family);
  gtk_tooltips_set_tip (tooltips, css_menu_voice_family, _("Set the voice-family in aural stylesheets for speech systems"), NULL);

  css_menu_voice_family_menu = gtk_menu_new ();
  gtk_menu_item_set_submenu (GTK_MENU_ITEM (css_menu_voice_family), css_menu_voice_family_menu);

  css_menu_voice_family_add_value = gtk_menu_item_new_with_mnemonic (_("Add value"));
  gtk_widget_show (css_menu_voice_family_add_value);
  gtk_container_add (GTK_CONTAINER (css_menu_voice_family_menu), css_menu_voice_family_add_value);
  gtk_tooltips_set_tip (tooltips, css_menu_voice_family_add_value, _("Add the value to an existing voice-family property"), NULL);

  voice_family_pair = gtk_menu_item_new_with_mnemonic ("voice-family");
  gtk_widget_show (voice_family_pair);
  gtk_container_add (GTK_CONTAINER (css_menu_voice_family_menu), voice_family_pair);

  css_menu_common_values = gtk_menu_item_new_with_mnemonic (_("Common values"));
  gtk_widget_show (css_menu_common_values);
  gtk_container_add (GTK_CONTAINER (css_dialogs_menu), css_menu_common_values);

  css_menu_common_values_menu = gtk_menu_new ();
  gtk_menu_item_set_submenu (GTK_MENU_ITEM (css_menu_common_values), css_menu_common_values_menu);

  css_dialogs_angle = gtk_menu_item_new_with_mnemonic (_("Angle"));
  gtk_widget_show (css_dialogs_angle);
  gtk_container_add (GTK_CONTAINER (css_menu_common_values_menu), css_dialogs_angle);

  css_menu_percentage = gtk_menu_item_new_with_mnemonic (_("Percentage"));
  gtk_widget_show (css_menu_percentage);
  gtk_container_add (GTK_CONTAINER (css_menu_common_values_menu), css_menu_percentage);

  css_menu_percentage_or_length = gtk_menu_item_new_with_mnemonic (_("Percentage or length"));
  gtk_widget_show (css_menu_percentage_or_length);
  gtk_container_add (GTK_CONTAINER (css_menu_common_values_menu), css_menu_percentage_or_length);

  css_menu_size_or_length = gtk_menu_item_new_with_mnemonic (_("Size or length"));
  gtk_widget_show (css_menu_size_or_length);
  gtk_container_add (GTK_CONTAINER (css_menu_common_values_menu), css_menu_size_or_length);

  css_menu_string = gtk_menu_item_new_with_mnemonic (_("String"));
  gtk_widget_show (css_menu_string);
  gtk_container_add (GTK_CONTAINER (css_menu_common_values_menu), css_menu_string);

  css_menu_number = gtk_menu_item_new_with_mnemonic (_("Number"));
  gtk_widget_show (css_menu_number);
  gtk_container_add (GTK_CONTAINER (css_menu_common_values_menu), css_menu_number);

  css_menu_frequency = gtk_menu_item_new_with_mnemonic (_("Frequency"));
  gtk_widget_show (css_menu_frequency);
  gtk_container_add (GTK_CONTAINER (css_menu_common_values_menu), css_menu_frequency);

  css_menu_identifier = gtk_menu_item_new_with_mnemonic (_("Identifier"));
  gtk_widget_show (css_menu_identifier);
  gtk_container_add (GTK_CONTAINER (css_menu_common_values_menu), css_menu_identifier);

  css_menu_integer = gtk_menu_item_new_with_mnemonic (_("Integer"));
  gtk_widget_show (css_menu_integer);
  gtk_container_add (GTK_CONTAINER (css_menu_common_values_menu), css_menu_integer);

  css_menu_length = gtk_menu_item_new_with_mnemonic (_("Length"));
  gtk_widget_show (css_menu_length);
  gtk_container_add (GTK_CONTAINER (css_menu_common_values_menu), css_menu_length);

  css_menu_functions = gtk_menu_item_new_with_mnemonic (_("Function values"));
  gtk_widget_show (css_menu_functions);
  gtk_container_add (GTK_CONTAINER (css_dialogs_menu), css_menu_functions);

  css_menu_functions_menu = gtk_menu_new ();
  gtk_menu_item_set_submenu (GTK_MENU_ITEM (css_menu_functions), css_menu_functions_menu);

  css_menu_url = gtk_menu_item_new_with_mnemonic (_("Url"));
  gtk_widget_show (css_menu_url);
  gtk_container_add (GTK_CONTAINER (css_menu_functions_menu), css_menu_url);
  gtk_tooltips_set_tip (tooltips, css_menu_url, _("Set a url, used by various properties as background, background-image and others"), NULL);

  css_menu_attribute = gtk_menu_item_new_with_mnemonic (_("Attribute"));
  gtk_widget_show (css_menu_attribute);
  gtk_container_add (GTK_CONTAINER (css_menu_functions_menu), css_menu_attribute);
  gtk_tooltips_set_tip (tooltips, css_menu_attribute, _("Set the value of the attr() function, it is only used by content, to add one tag attribute - as id, or width - to the content header."), NULL);

  g_signal_connect ((gpointer) css_menu_border_pair, "activate",
					G_CALLBACK (css_menu_border_pair_activate),
                    window_object);

  g_signal_connect ((gpointer) css_menu_border_width_all_pair, "activate",
                    G_CALLBACK (on_css_menu_border_width_all_pair_activate),
                    window_object);

  g_signal_connect ((gpointer) css_menu_border_top_width, "activate",
                    G_CALLBACK (on_css_menu_border_top_width_activate),
                    window_object);
  g_signal_connect ((gpointer) css_menu_border_right_width, "activate",
                    G_CALLBACK (on_css_menu_border_right_width_activate),
                    window_object);
  g_signal_connect ((gpointer) css_menu_border_bottom_width, "activate",
                    G_CALLBACK (on_css_menu_border_bottom_width_activate),
                    window_object);
  g_signal_connect ((gpointer) css_menu_border_left_width, "activate",
                    G_CALLBACK (on_css_menu_border_left_width_activate),
                    window_object);

  g_signal_connect ((gpointer) css_menu_wsc_border_top, "activate",
                    G_CALLBACK (on_css_menu_wsc_border_top_activate),
                    window_object);
  g_signal_connect ((gpointer) css_menu_border_right, "activate",
                    G_CALLBACK (on_css_menu_border_right_activate),
                    window_object);
  g_signal_connect ((gpointer) css_menu_border_bottom, "activate",
                    G_CALLBACK (on_css_menu_border_bottom_activate),
                    window_object);
  g_signal_connect ((gpointer) on_css_menu_border_left, "activate",
                    G_CALLBACK (on_on_css_menu_border_left_activate),
                    window_object);
  g_signal_connect ((gpointer) css_menu_counter_pair, "activate",
                    G_CALLBACK (on_css_menu_counter_pair_activate),
                    window_object);
  g_signal_connect ((gpointer) css_menu_increment, "activate",
                    G_CALLBACK (on_css_menu_increment),
                    window_object);
  g_signal_connect ((gpointer) css_menu_counter_reset, "activate",
                    G_CALLBACK (on_css_menu_counter_reset_activate),
                    window_object);
  g_signal_connect ((gpointer) css_menu_background_attachment_color, "activate",
                    G_CALLBACK (on_css_menu_background_attachment_color_activate),
                    window_object);
  g_signal_connect ((gpointer) css_menu_background_color, "activate",
                    G_CALLBACK (on_css_menu_background_color_activate),
                    window_object);
  g_signal_connect ((gpointer) css_menu_border_color, "activate",
                    G_CALLBACK (on_css_menu_border_color_activate),
                    window_object);
  g_signal_connect ((gpointer) css_menu_border_top_color, "activate",
                    G_CALLBACK (on_css_menu_border_top_color_activate),
                    window_object);
  g_signal_connect ((gpointer) css_menu_border_right_color, "activate",
                    G_CALLBACK (on_css_menu_border_right_color_activate),
                    window_object);
  g_signal_connect ((gpointer) css_menu_border_bottom_color, "activate",
                    G_CALLBACK (on_css_menu_border_bottom_color_activate),
                    window_object);
  g_signal_connect ((gpointer) css_menu_border_left_color, "activate",
                    G_CALLBACK (on_css_menu_border_left_color_activate),
                    window_object);
  g_signal_connect ((gpointer) css_menu_color_pair, "activate",
                    G_CALLBACK (on_css_menu_color_pair_activate),
                    window_object);
  g_signal_connect ((gpointer) css_menu_outline_color, "activate",
                    G_CALLBACK (on_css_menu_outline_color_activate),
                    window_object);
  g_signal_connect ((gpointer) css_menu_font_pair, "activate",
                    G_CALLBACK (on_css_menu_font_pair_activate),
                    window_object);
  g_signal_connect ((gpointer) css_menu_font_family_pair, "activate",
                    G_CALLBACK (on_css_menu_font_family_pair_activate),
                    window_object);
  g_signal_connect ((gpointer) css_menu_font_size_pair, "activate",
                    G_CALLBACK (on_css_menu_font_size_pair_activate),
                    window_object);
  g_signal_connect ((gpointer) css_menu_list_style_pair, "activate",
                    G_CALLBACK (on_css_menu_list_style_pair_activate),
                    window_object);
  g_signal_connect ((gpointer) css_menu_nargin_pair, "activate",
                    G_CALLBACK (on_css_menu_nargin_pair_activate),
                    window_object);
  g_signal_connect ((gpointer) css_menu_margin_width_pair, "activate",
                    G_CALLBACK (on_css_menu_margin_width_pair_activate),
                    window_object);
  g_signal_connect ((gpointer) css_menu_margin_top, "activate",
                    G_CALLBACK (on_css_menu_margin_top_activate),
                    window_object);
  g_signal_connect ((gpointer) css_menu_margin_right, "activate",
                    G_CALLBACK (on_css_menu_margin_right_activate),
                    window_object);
  g_signal_connect ((gpointer) css_menu_margin_bottom, "activate",
                    G_CALLBACK (on_css_menu_margin_bottom_activate),
                    window_object);
  g_signal_connect ((gpointer) css_menu_margin_left, "activate",
                    G_CALLBACK (on_css_menu_margin_left_activate),
                    window_object);
  g_signal_connect ((gpointer) css_menu_pading_pair, "activate",
                    G_CALLBACK (on_css_menu_pading_pair_activate),
                    window_object);
  g_signal_connect ((gpointer) css_menu_padding_top, "activate",
                    G_CALLBACK (on_css_menu_padding_top_activate),
                    window_object);
  g_signal_connect ((gpointer) css_menu_padding_right, "activate",
                    G_CALLBACK (on_css_menu_padding_right_activate),
                    window_object);
  g_signal_connect ((gpointer) css_menu_padding_bottom, "activate",
                    G_CALLBACK (on_css_menu_padding_bottom_activate),
                    window_object);
  g_signal_connect ((gpointer) css_menu_padding_left, "activate",
                    G_CALLBACK (on_css_menu_padding_left_activate),
                    window_object);
  g_signal_connect ((gpointer) css_menu_pause_pair, "activate",
                    G_CALLBACK (on_css_menu_pause_pair_activate),
                    window_object);
  g_signal_connect ((gpointer) css_menu_play_during_pair, "activate",
                    G_CALLBACK (on_css_menu_play_during_pair_activate),
                    window_object);
  g_signal_connect ((gpointer) css_menu_outline_pair, "activate",
                    G_CALLBACK (on_css_menu_outline_pair_activate),
                    window_object);
  g_signal_connect ((gpointer) css_menu_quotes_pair, "activate",
                    G_CALLBACK (on_css_menu_quotes_pair_activate),
                    window_object);

  g_signal_connect ((gpointer) css_menu_clip, "activate",
                    G_CALLBACK (on_css_menu_clip_activate),
                    window_object);

  g_signal_connect ((gpointer) css_menu_text_shadow_pair, "activate",
                    G_CALLBACK (on_css_menu_text_shadow_pair_activate),
                    window_object);
  g_signal_connect ((gpointer) voice_family_pair, "activate",
                    G_CALLBACK (on_voice_family_activate),
                    window_object);
				
  g_signal_connect ((gpointer) css_menu_background_add_value, "activate",
                    G_CALLBACK (on_css_menu_background_activate),
                    window_object);
  g_signal_connect ((gpointer) css_menu_background_pair, "activate",
                    G_CALLBACK (on_css_menu_background_pair_activate),
                    window_object);					

  g_signal_connect ((gpointer) css_menu_border_add_value, "activate",
                    G_CALLBACK (on_css_menu_border_all_activate),
                    window_object);				
  g_signal_connect ((gpointer) css_menu_border_width_all_add_value, "activate",
                    G_CALLBACK (on_css_menu_border_width_all_activate),
                    window_object);	
			
  g_signal_connect ((gpointer) css_menu_border_width_one_add_value, "activate",
                    G_CALLBACK (on_css_menu_border_width_one_add_value_activate),
                    window_object);					
  g_signal_connect ((gpointer) css_menu_wsc_add_values, "activate",
                    G_CALLBACK (on_css_menu_border_wsc_activate),
                    window_object);					
  g_signal_connect ((gpointer) css_menu_counter_value, "activate",
                    G_CALLBACK (on_css_menu_counter_prop_activate),
                    window_object);					
  g_signal_connect ((gpointer) css_menu_color_add_value, "activate",
                    G_CALLBACK (on_css_menu_color_activate),
                    window_object);				
  g_signal_connect ((gpointer) css_menu_font_add_value, "activate",
                    G_CALLBACK (on_css_menu_font_value_activate),
                    window_object);				
  g_signal_connect ((gpointer) css_menu_font_family_add_value, "activate",
                    G_CALLBACK (on_css_menu_font_family_activate),
                    window_object);
					
   g_signal_connect ((gpointer) css_menu_font_size_add_value, "activate",
                    G_CALLBACK (on_css_menu_font_size_activate),
                    window_object);				
  g_signal_connect ((gpointer) css_menu_list_style_add_value, "activate",
                    G_CALLBACK (on_css_menu_list_style_activate),
                    window_object);				
  g_signal_connect ((gpointer) css_menu_margin_add_value, "activate",
                    G_CALLBACK (on_css_menu_margin_value_activate),
                    window_object);				
  g_signal_connect ((gpointer) css_menu_margin_width_add_value, "activate",
                    G_CALLBACK (on_css_menu_margin_width_activate),
                    window_object);					
	  g_signal_connect ((gpointer) css_menu_padding_add_value, "activate",
                    G_CALLBACK (on_css_menu_padding_activate),
                    window_object);				
  g_signal_connect ((gpointer) css_menu_pause_add_value, "activate",
                    G_CALLBACK (on_css_menu_pause_activate),
                    window_object);					
	g_signal_connect ((gpointer) on_css_play_during_add_value, "activate",
                    G_CALLBACK (on_css_menu_play_during_activate),
                    window_object);				
	g_signal_connect ((gpointer) css_menu_outline_add_value, "activate",
                    G_CALLBACK (on_css_manu_outline_activate),
                    window_object);				
	g_signal_connect ((gpointer) css_menu_quotes_add_value, "activate",
                    G_CALLBACK (on_css_menu_quotes_activate),
                    window_object);				
g_signal_connect ((gpointer) css_menu_shape_add_value, "activate",
                    G_CALLBACK (on_css_menu_shape_activate),
                    window_object);					
  g_signal_connect ((gpointer) css_menu_voice_family_add_value, "activate",
                    G_CALLBACK (on_css_menu_voice_family_activate),
                    window_object);	
	  g_signal_connect ((gpointer) css_menu_text_shadow_add_value, "activate",
                    G_CALLBACK (on_css_dialogs_text_shadow_activate),
                    window_object);				
  g_signal_connect ((gpointer) css_dialogs_angle, "activate",
                    G_CALLBACK (on_css_dialogs_angle_activate),
                    window_object);
  g_signal_connect ((gpointer) css_menu_percentage, "activate",
                    G_CALLBACK (on_css_menu_percentage_activate),
                    window_object);
  g_signal_connect ((gpointer) css_menu_percentage_or_length, "activate",
                    G_CALLBACK (on_css_menu_percentage_or_length_activate),
                    window_object);
  g_signal_connect ((gpointer) css_menu_size_or_length, "activate",
                    G_CALLBACK (on_css_menu_size_or_length_activate),
                    window_object);
  g_signal_connect ((gpointer) css_menu_string, "activate",
                    G_CALLBACK (on_css_menu_string_activate),
                    window_object);
  g_signal_connect ((gpointer) css_menu_number, "activate",
                    G_CALLBACK (on_css_menu_number_activate),
                    window_object);
  g_signal_connect ((gpointer) css_menu_frequency, "activate",
                    G_CALLBACK (on_css_menu_frequency_activate),
                    window_object);
  g_signal_connect ((gpointer) css_menu_identifier, "activate",
                    G_CALLBACK (on_css_menu_identifier_activate),
                    window_object);
  g_signal_connect ((gpointer) css_menu_integer, "activate",
                    G_CALLBACK (on_css_menu_integer_activate),
                    window_object);
  g_signal_connect ((gpointer) css_menu_length, "activate",
                    G_CALLBACK (on_css_menu_length_activate),
                    window_object);
  g_signal_connect ((gpointer) css_menu_url, "activate",
                    G_CALLBACK (on_css_menu_url_activate),
                    window_object);
  g_signal_connect ((gpointer) css_menu_attribute, "activate",
                    G_CALLBACK (on_css_menu_attribute_activate),
                    window_object);

  return css_dialogs_menu;
  
}
// callbacks

void
on_css_dialogs_angle_activate          (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
	GtkWidget* dlg;

	dlg = create_angle_property_dialog (CSSED_WINDOW(user_data),
				 	 g_strdup (_("Angle")), FALSE  ,TRUE);
	gtk_widget_show( dlg );
}


void
on_css_menu_background_activate        (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
	 
	GtkWidget* dlg;

	dlg = create_background_property_dialog (CSSED_WINDOW(user_data),
				 	 g_strdup (_("Background")), FALSE  ,TRUE);
	gtk_widget_show( dlg );
}

void
on_css_menu_background_pair_activate   (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
	GtkWidget* dlg;

	dlg = create_background_property_dialog (CSSED_WINDOW(user_data),
				 	 g_strdup ("background"), TRUE  ,TRUE);
	gtk_widget_show( dlg );	
}
void
on_css_menu_border_all_activate        (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
	GtkWidget* dlg;

	dlg = create_border_properties_dialog (CSSED_WINDOW(user_data),
				 	 g_strdup (_("Border")), FALSE  ,TRUE);
	gtk_widget_show( dlg );
}



void
on_css_menu_border_width_all_activate  (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
	GtkWidget* dlg;

	dlg = create_border_width_properties_dialog (CSSED_WINDOW(user_data),
				 	 g_strdup (_("Border width (all)")), FALSE  ,TRUE);
	gtk_widget_show( dlg );
}


void
on_css_menu_border_width_one_add_value_activate  (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
	GtkWidget* dlg;

	dlg = create_border_width_single_properties_dialog (CSSED_WINDOW(user_data),
				 	 g_strdup (_("Border width (one)")), FALSE  ,TRUE);
	gtk_widget_show( dlg );
}


void
on_css_menu_border_wsc_activate        (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{

	GtkWidget* dlg;

	dlg = create_border_width_style_color_value_dialog (CSSED_WINDOW(user_data),
				 	 g_strdup (_("Border width style and color")), FALSE  ,TRUE);
	gtk_widget_show( dlg );	
}
void
on_css_menu_counter_prop_activate      (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{

	GtkWidget* dlg;

	dlg = create_counter_property_dialog (CSSED_WINDOW(user_data),
				 	 g_strdup (_("Counter")), FALSE ,TRUE );
	gtk_widget_show( dlg );		
}


void
on_css_menu_increment_or_reset_activate
                                        (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
	GtkWidget* dlg;

	dlg = create_counter_increment_reset_dialog (CSSED_WINDOW(user_data),
				 	 g_strdup (_("Counter increment/reset")), FALSE, TRUE);
	gtk_widget_show( dlg );
}


void
on_css_menu_color_activate             (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
	GtkWidget* dlg;

	dlg = create_color_selector_dialog (CSSED_WINDOW(user_data),
				 	 g_strdup (_("Color")), FALSE ,TRUE );
	gtk_widget_show( dlg );	
}


void
on_css_menu_font_value_activate        (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
	GtkWidget* dlg;

	dlg = create_font_property_dialog (CSSED_WINDOW(user_data),
				 	 g_strdup (_("Font")), FALSE ,TRUE );
	gtk_widget_show( dlg );		
}

void
on_css_menu_font_family_activate       (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
	GtkWidget* dlg;

	dlg = create_font_family_dialog (CSSED_WINDOW(user_data),
				 	 g_strdup (_("Font family")), FALSE ,TRUE );
	gtk_widget_show( dlg );	
}


void
on_css_menu_font_size_activate         (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
	GtkWidget* dlg;

	dlg = create_font_size_dialog (CSSED_WINDOW(user_data),
				 	 g_strdup (_("Font size")), FALSE ,TRUE );
	gtk_widget_show( dlg );	
}


void
on_css_menu_list_style_activate        (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
	GtkWidget* dlg;

	dlg = create_list_style_dialog (CSSED_WINDOW(user_data),
				 	 g_strdup (_("List style")), FALSE ,TRUE );
	gtk_widget_show( dlg );	
}


void
on_css_menu_margin_value_activate      (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
	GtkWidget* dlg;

	dlg = create_margin_property_dialog (CSSED_WINDOW(user_data),
				 	 g_strdup (_("Margin")), FALSE ,TRUE );
	gtk_widget_show( dlg );		
}


void
on_css_menu_margin_width_activate      (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
	GtkWidget* dlg;

	dlg = create_margin_width_value_dialog (CSSED_WINDOW(user_data),
				 	 g_strdup (_("Margin width")), FALSE ,TRUE );
	gtk_widget_show( dlg );
}


void
on_css_menu_padding_activate           (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{

	GtkWidget* dlg;

	dlg = create_padding_property_dialog (CSSED_WINDOW(user_data),
				 	 g_strdup (_("Padding")), FALSE ,TRUE );
	gtk_widget_show( dlg );	
}


void
on_css_menu_pause_activate             (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
	GtkWidget* dlg;

	dlg = create_pause_property_dialog (CSSED_WINDOW(user_data),
				 	 g_strdup (_("Pause")), FALSE ,TRUE );
	gtk_widget_show( dlg );	
}


void
on_css_menu_play_during_activate       (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
	GtkWidget* dlg;

	dlg = create_play_during_property_dialog (CSSED_WINDOW(user_data),
				 	 g_strdup (_("Play during")), FALSE ,TRUE );
	gtk_widget_show( dlg );		
}


void
on_css_manu_outline_activate           (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
	GtkWidget* dlg;

	dlg = create_outline_property_dialog (CSSED_WINDOW(user_data),
				 	 g_strdup (_("Outline")), FALSE ,TRUE );
	gtk_widget_show( dlg );		
}

void
on_css_menu_quotes_activate            (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
	GtkWidget* dlg;

	dlg = create_quotes_property_dialog (CSSED_WINDOW(user_data),
				 	 g_strdup (_("Outline")), FALSE ,TRUE );
	gtk_widget_show( dlg );		
	
}


void
on_css_menu_shape_activate             (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
	GtkWidget* dlg;

	dlg = create_shape_value_dialog (CSSED_WINDOW(user_data),
				 	 g_strdup (_("Shape")), FALSE ,TRUE );
	gtk_widget_show( dlg );		
}


void
on_css_dialogs_text_shadow_activate    (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
	GtkWidget* dlg;

	dlg = create_text_shadow_value_dialog (CSSED_WINDOW(user_data),
				 	 g_strdup (_("Text shadow")), FALSE ,TRUE );
	gtk_widget_show( dlg );	
}


void
on_css_menu_voice_family_activate      (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
	GtkWidget* dlg;

	dlg = create_voice_family_value_dialog (CSSED_WINDOW(user_data),
				 	 g_strdup (_("Voice family")), FALSE ,TRUE );
	gtk_widget_show( dlg );	
}

void
on_css_menu_percentage_activate        (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
	GtkWidget* dlg;

	dlg = create_percentage_value_dialog (CSSED_WINDOW(user_data),
				 	 g_strdup (_("Percentage")), FALSE ,TRUE );
	gtk_widget_show( dlg );	
}


void
on_css_menu_percentage_or_length_activate
                                        (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
	GtkWidget* dlg;

	dlg = create_percentage_length_value_dialog (CSSED_WINDOW(user_data),
				 	 g_strdup (_("Percentage or length")), FALSE ,TRUE );
	gtk_widget_show( dlg );	
}


void
on_css_menu_size_or_length_activate    (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
	GtkWidget* dlg;

	dlg = create_size_length_value_dialog (CSSED_WINDOW(user_data),
				 	 g_strdup (_("Size or length")), FALSE ,TRUE );
	gtk_widget_show( dlg );	
}


void
on_css_menu_string_activate            (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
	GtkWidget* dlg;

	dlg = create_string_properties_dialog (CSSED_WINDOW(user_data),
				 	 g_strdup (_("Size or length")), FALSE ,TRUE );
	gtk_widget_show( dlg );

}

void
on_css_menu_number_activate            (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
	GtkWidget* dlg;

	dlg = create_number_value_dialog (CSSED_WINDOW(user_data),
				 	 g_strdup (_("Size or length")), FALSE ,TRUE );
	gtk_widget_show( dlg );
}


void
on_css_menu_frequency_activate         (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
	GtkWidget* dlg;

	dlg = create_frecuency_property_dialog (CSSED_WINDOW(user_data),
				 	 g_strdup (_("Frequency")), FALSE ,TRUE );
	gtk_widget_show( dlg );
}


void
on_css_menu_identifier_activate        (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
	GtkWidget* dlg;

	dlg = create_identifier_value_dialog (CSSED_WINDOW(user_data),
				 	 g_strdup (_("Identifier")), FALSE ,TRUE );
	gtk_widget_show( dlg );
}


void
on_css_menu_integer_activate           (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
	GtkWidget* dlg;

	dlg = create_integer_value_dialog (CSSED_WINDOW(user_data),
				 	 g_strdup (_("Integer")), FALSE ,TRUE );
	gtk_widget_show( dlg );
}


void
on_css_menu_length_activate            (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
	GtkWidget* dlg;

	dlg = create_length_value_dialog (CSSED_WINDOW(user_data),
				 	 g_strdup (_("Length")), FALSE ,TRUE );
	gtk_widget_show( dlg );
}

void
on_css_menu_url_activate               (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
	GtkWidget* dlg;

	dlg = create_uri_value_dialog (CSSED_WINDOW(user_data),
				 	 g_strdup (_("Url")), FALSE ,TRUE );
	gtk_widget_show( dlg );

}


void
on_css_menu_attribute_activate         (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
	GtkWidget* dlg;

	dlg = create_attr_value_dialog (CSSED_WINDOW(user_data),
				 	 g_strdup (_("Attribute")), FALSE ,TRUE );
	gtk_widget_show( dlg );
}

/* ****************** */ 


void
css_menu_border_pair_activate     (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{ 
	GtkWidget* dlg;

	dlg = create_border_properties_dialog (CSSED_WINDOW(user_data),
				 	 g_strdup ("border"), TRUE, TRUE );
	gtk_widget_show( dlg );
}

void
on_css_menu_border_width_all_pair_activate
                                        (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{ 
	GtkWidget* dlg;

	dlg = create_border_width_properties_dialog (CSSED_WINDOW(user_data),
				 	 g_strdup ("border-width"), TRUE, TRUE );
	gtk_widget_show( dlg );	
}

void
on_css_menu_border_top_width_activate
                                        (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{ 
	GtkWidget* dlg;

	dlg = create_border_width_single_properties_dialog (CSSED_WINDOW(user_data),
				 	 g_strdup ("border-top-width"), TRUE, TRUE );
	gtk_widget_show( dlg );		
}

void
on_css_menu_border_right_width_activate
                                        (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
	GtkWidget* dlg;

	dlg = create_border_width_single_properties_dialog (CSSED_WINDOW(user_data),
				 	 g_strdup ("border-right-width"), TRUE, TRUE );
	gtk_widget_show( dlg );	
}

void
on_css_menu_border_bottom_width_activate
                                        (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
	GtkWidget* dlg;

	dlg = create_border_width_single_properties_dialog (CSSED_WINDOW(user_data),
				 	 g_strdup ("border-bottom-width"), TRUE, TRUE );
	gtk_widget_show( dlg );	
}

void
on_css_menu_border_left_width_activate
                                        (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
	GtkWidget* dlg;

	dlg = create_border_width_single_properties_dialog (CSSED_WINDOW(user_data),
				 	 g_strdup ("border-left-width"), TRUE, TRUE );
	gtk_widget_show( dlg );	
}

void
on_css_menu_wsc_border_top_activate
                                        (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
	GtkWidget* dlg;

	dlg = create_border_width_style_color_value_dialog (CSSED_WINDOW(user_data),
				 	 g_strdup ("border-top"), TRUE, TRUE );
	gtk_widget_show( dlg );		
}

void
on_css_menu_border_right_activate (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
	GtkWidget* dlg;

	dlg = create_border_width_style_color_value_dialog (CSSED_WINDOW(user_data),
				 	 g_strdup ("border-right"), TRUE, TRUE );
	gtk_widget_show( dlg );	
}

void
on_css_menu_border_bottom_activate
                                        (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{ 
	GtkWidget* dlg;

	dlg = create_border_width_style_color_value_dialog (CSSED_WINDOW(user_data),
				 	 g_strdup ("border-bottom"), TRUE, TRUE );
	gtk_widget_show( dlg );		
}

void
on_on_css_menu_border_left_activate
                                        (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
	GtkWidget* dlg;

	dlg = create_border_width_style_color_value_dialog (CSSED_WINDOW(user_data),
				 	 g_strdup ("border-left"), TRUE, TRUE );
	gtk_widget_show( dlg );	
}

void
on_css_menu_counter_pair_activate (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
	GtkWidget* dlg;

	dlg = create_counter_property_dialog (CSSED_WINDOW(user_data),
				 	 g_strdup ("counter"), TRUE, TRUE );
	gtk_widget_show( dlg );	
}

void
on_css_menu_increment             (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
	GtkWidget* dlg;

	dlg = create_counter_increment_reset_dialog (CSSED_WINDOW(user_data),
				 	 g_strdup ("counter-increment"), TRUE, TRUE );
	gtk_widget_show( dlg );
}

void
on_css_menu_counter_reset_activate
                                        (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
	GtkWidget* dlg;

	dlg = create_counter_increment_reset_dialog (CSSED_WINDOW(user_data),
				 	 g_strdup ("counter-reset"), TRUE, TRUE );
	gtk_widget_show( dlg );
}

void
on_css_menu_background_attachment_color_activate
                                        (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
	GtkWidget* dlg;

	dlg = create_color_selector_dialog (CSSED_WINDOW(user_data),
				 	 g_strdup ("background-attachment"), TRUE, TRUE );
	gtk_widget_show( dlg );	
}

void
on_css_menu_background_color_activate
                                        (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
	GtkWidget* dlg;

	dlg = create_color_selector_dialog (CSSED_WINDOW(user_data),
				 	 g_strdup ("background-color"), TRUE, TRUE );
	gtk_widget_show( dlg );		
}

void
on_css_menu_border_color_activate (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
	GtkWidget* dlg;

	dlg = create_color_selector_dialog (CSSED_WINDOW(user_data),
				 	 g_strdup ("border-color"), TRUE, TRUE );
	gtk_widget_show( dlg );	
}

void
on_css_menu_border_top_color_activate
                                        (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
	GtkWidget* dlg;

	dlg = create_color_selector_dialog (CSSED_WINDOW(user_data),
				 	 g_strdup ("border-top-color"), TRUE, TRUE );
	gtk_widget_show( dlg );		
}

void
on_css_menu_border_right_color_activate
                                        (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
	GtkWidget* dlg;

	dlg = create_color_selector_dialog (CSSED_WINDOW(user_data),
				 	 g_strdup ("border-right-color"), TRUE, TRUE );
	gtk_widget_show( dlg );		
}

void
on_css_menu_border_bottom_color_activate
                                        (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
	GtkWidget* dlg;

	dlg = create_color_selector_dialog (CSSED_WINDOW(user_data),
				 	 g_strdup ("border-bottom-color"), TRUE, TRUE );
	gtk_widget_show( dlg );			
}

void
on_css_menu_border_left_color_activate
                                        (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
	GtkWidget* dlg;

	dlg = create_color_selector_dialog (CSSED_WINDOW(user_data),
				 	 g_strdup ("border-left-color"), TRUE, TRUE );
	gtk_widget_show( dlg );	
}

void
on_css_menu_color_pair_activate   (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
	GtkWidget* dlg;

	dlg = create_color_selector_dialog (CSSED_WINDOW(user_data),
				 	 g_strdup ("color"), TRUE, TRUE );
	gtk_widget_show( dlg );		
}

void
on_css_menu_outline_color_activate
                                        (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
	GtkWidget* dlg;

	dlg = create_color_selector_dialog (CSSED_WINDOW(user_data),
				 	 g_strdup ("outline-color"), TRUE, TRUE );
	gtk_widget_show( dlg );		
}

void
on_css_menu_font_pair_activate    (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
	GtkWidget* dlg;

	dlg = create_font_property_dialog(CSSED_WINDOW(user_data),
				 	 g_strdup ("font"), TRUE, TRUE );
	gtk_widget_show( dlg );	
}

void
on_css_menu_font_family_pair_activate
                                        (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
	GtkWidget* dlg;

	dlg = create_font_family_dialog(CSSED_WINDOW(user_data),
				 	 g_strdup ("font-family"), TRUE, TRUE );
	gtk_widget_show( dlg );		
}

void
on_css_menu_font_size_pair_activate
                                        (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
	GtkWidget* dlg;

	dlg = create_font_size_dialog(CSSED_WINDOW(user_data),
				 	 g_strdup ("font-size"), TRUE, TRUE );
	gtk_widget_show( dlg );		
}


void
on_css_menu_list_style_pair_activate
                                        (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
	GtkWidget* dlg;

	dlg = create_list_style_dialog(CSSED_WINDOW(user_data),
				 	 g_strdup ("list-style"), TRUE, TRUE );
	gtk_widget_show( dlg );		
}

void
on_css_menu_nargin_pair_activate  (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
	GtkWidget* dlg;

	dlg = create_margin_property_dialog(CSSED_WINDOW(user_data),
				 	 g_strdup ("margin"), TRUE, TRUE );
	gtk_widget_show( dlg );	
}

void
on_css_menu_margin_width_pair_activate
                                        (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
	GtkWidget* dlg;

	dlg = create_margin_width_value_dialog(CSSED_WINDOW(user_data),
				 	 g_strdup ("margin-width"), TRUE, TRUE );
	gtk_widget_show( dlg );	
}

void
on_css_menu_margin_top_activate   (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
	GtkWidget* dlg;

	dlg = create_margin_width_value_dialog(CSSED_WINDOW(user_data),
				 	 g_strdup ("margin-top"), TRUE, TRUE );
	gtk_widget_show( dlg );		
}

void
on_css_menu_margin_right_activate (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
	GtkWidget* dlg;

	dlg = create_margin_width_value_dialog(CSSED_WINDOW(user_data),
				 	 g_strdup ("margin-right"), TRUE, TRUE );
	gtk_widget_show( dlg );		
}

void
on_css_menu_margin_bottom_activate
                                        (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
	GtkWidget* dlg;

	dlg = create_margin_width_value_dialog(CSSED_WINDOW(user_data),
				 	 g_strdup ("margin-bottom"), TRUE, TRUE );
	gtk_widget_show( dlg );		
}

void
on_css_menu_margin_left_activate  (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
	GtkWidget* dlg;

	dlg = create_margin_width_value_dialog(CSSED_WINDOW(user_data),
				 	 g_strdup ("margin-left"), TRUE, TRUE );
	gtk_widget_show( dlg );	
}


void
on_css_menu_pading_pair_activate  (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
	GtkWidget* dlg;

	dlg = create_padding_property_dialog(CSSED_WINDOW(user_data),
				 	 g_strdup ("padding"), TRUE, TRUE );
	gtk_widget_show( dlg );		
}

void
on_css_menu_padding_top_activate  (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
	GtkWidget* dlg;

	dlg = create_length_value_dialog(CSSED_WINDOW(user_data),
				 	 g_strdup ("padding-top"), TRUE, TRUE );
	gtk_widget_show( dlg );		
}

void
on_css_menu_padding_right_activate
                                        (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
	GtkWidget* dlg;

	dlg = create_length_value_dialog(CSSED_WINDOW(user_data),
				 	 g_strdup ("padding-right"), TRUE, TRUE );
	gtk_widget_show( dlg );		
}

void
on_css_menu_padding_bottom_activate
                                        (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
	GtkWidget* dlg;

	dlg = create_length_value_dialog(CSSED_WINDOW(user_data),
				 	 g_strdup ("padding-bottom"), TRUE, TRUE );
	gtk_widget_show( dlg );		
}

void
on_css_menu_padding_left_activate (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
	GtkWidget* dlg;

	dlg = create_length_value_dialog(CSSED_WINDOW(user_data),
				 	 g_strdup ("padding-left"), TRUE, TRUE );
	gtk_widget_show( dlg );		
}


void
on_css_menu_pause_pair_activate   (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
	GtkWidget* dlg;

	dlg = create_pause_property_dialog(CSSED_WINDOW(user_data),
				 	 g_strdup ("pause"), TRUE, TRUE );
	gtk_widget_show( dlg );		
}


void
on_css_menu_play_during_pair_activate
                                        (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
	GtkWidget* dlg;

	dlg = create_play_during_property_dialog(CSSED_WINDOW(user_data),
				 	 g_strdup ("play-during"), TRUE, TRUE );
	gtk_widget_show( dlg );			
}


void
on_css_menu_outline_pair_activate (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
	GtkWidget* dlg;

	dlg = create_outline_property_dialog(CSSED_WINDOW(user_data),
				 	 g_strdup ("outline"), TRUE, TRUE );
	gtk_widget_show( dlg );		
}


void
on_css_menu_quotes_pair_activate  (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
	GtkWidget* dlg;

	dlg = create_quotes_property_dialog(CSSED_WINDOW(user_data),
				 	 g_strdup ("quotes"), TRUE, TRUE );
	gtk_widget_show( dlg );		
}


void
on_css_menu_clip_activate         (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
	GtkWidget* dlg;

	dlg = create_shape_value_dialog(CSSED_WINDOW(user_data),
				 	 g_strdup ("clip"), TRUE, TRUE );
	gtk_widget_show( dlg );		
}


void
on_css_menu_text_shadow_pair_activate (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
	GtkWidget* dlg;

	dlg = create_text_shadow_value_dialog(CSSED_WINDOW(user_data),
				 	 g_strdup ("text-shadow"), TRUE, TRUE );
	gtk_widget_show( dlg );			
}

void
on_voice_family_activate          (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
	GtkWidget* dlg;

	dlg = create_voice_family_value_dialog(CSSED_WINDOW(user_data),
				 	 g_strdup ("voice-family"), TRUE, TRUE );
	gtk_widget_show( dlg );		
}
