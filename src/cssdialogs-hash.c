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

#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <string.h>
#include <stdio.h>

#include <gdk/gdkkeysyms.h>
#include <gtk/gtk.h>

#include "cssedwindow.h"
#include "document.h"
#include "cssdialogs-interface.h"
#include "cssdialogs-functions.h"
#include "support.h"
#include "cssdialogs-class.h"

void 
key_destroy_func(gpointer data)
{
	g_free(data);	
}

void 
value_destroy_func (gpointer data)
{
	g_free(data);	
}

gboolean
key_equal_func (gconstpointer a, gconstpointer b)
{
	if( strcmp( (gchar*) a, (gchar*) b) == 0 )
		return TRUE;
	else
		return FALSE;
}

GHashTable*
create_and_fill_cssdialogs_hash_table()
{
	GHashTable* hashtable;
	CssDialogClass* klass;
	hashtable = g_hash_table_new_full (g_str_hash,
                                       key_equal_func,
                                       key_destroy_func,
                                       value_destroy_func);
	
	klass = css_dialog_class_new();
	klass->keyword = g_strdup("(color)");
	klass->create_function = &create_color_selector_dialog;
	klass->get_string_function = &cssdialog_color_selector_get_string;
	g_hash_table_insert( hashtable, klass->keyword, klass );
	
	klass = css_dialog_class_new();
	klass->keyword = g_strdup("(angle)");
	klass->create_function = &create_angle_property_dialog;
	klass->get_string_function = &cssdialog_angle_property_get_string;
	g_hash_table_insert( hashtable, klass->keyword, klass );

	klass = css_dialog_class_new();
	klass->keyword = g_strdup("(background)");
	klass->create_function = &create_background_property_dialog;
	klass->get_string_function = &cssdialog_background_property_get_string;
	g_hash_table_insert( hashtable, klass->keyword, klass );
	
	klass = css_dialog_class_new();
	klass->keyword = g_strdup("(border)");
	klass->create_function = &create_border_properties_dialog;
	klass->get_string_function = &cssdialog_border_properties_get_string;
	g_hash_table_insert( hashtable, klass->keyword, klass );
	
	klass = css_dialog_class_new();
	klass->keyword = g_strdup("(border-width)");
	klass->create_function = &create_border_width_properties_dialog;
	klass->get_string_function = &cssdialog_border_width_properties_get_string;
	g_hash_table_insert( hashtable, klass->keyword, klass );
	
	klass = css_dialog_class_new();
	klass->keyword = g_strdup("(border-width-single)");
	klass->create_function = &create_border_width_single_properties_dialog;
	klass->get_string_function = &cssdialog_border_width_single_get_string;
	g_hash_table_insert( hashtable, klass->keyword, klass );
	
	klass = css_dialog_class_new();
	klass->keyword = g_strdup("(increment-reset)");
	klass->create_function = &create_counter_increment_reset_dialog;
	klass->get_string_function = &cssdialog_counter_increment_reset_get_string;
	g_hash_table_insert( hashtable, klass->keyword, klass );
	
	klass = css_dialog_class_new();
	klass->keyword = g_strdup("(string)");
	klass->create_function = &create_string_properties_dialog;
	klass->get_string_function = &cssdialog_string_properties_get_string;
	g_hash_table_insert( hashtable, klass->keyword, klass );
	
	klass = css_dialog_class_new();
	klass->keyword = g_strdup("(uri)");
	klass->create_function = &create_uri_value_dialog;
	klass->get_string_function = &cssdialog_uri_value_get_string;
	g_hash_table_insert( hashtable, klass->keyword, klass );
	
	klass = css_dialog_class_new();
	klass->keyword = g_strdup("(attr)");
	klass->create_function = &create_attr_value_dialog;
	klass->get_string_function = &cssdialog_attr_value_get_string;
	g_hash_table_insert( hashtable, klass->keyword, klass );
	
	klass = css_dialog_class_new();
	klass->keyword = g_strdup("(counter)");
	klass->create_function = &create_counter_property_dialog;
	klass->get_string_function = &cssdialog_counter_property_get_string;
	g_hash_table_insert( hashtable, klass->keyword, klass );
	
	klass = css_dialog_class_new();
	klass->keyword = g_strdup("(font)");
	klass->create_function = &create_font_property_dialog;
	klass->get_string_function = &cssdialog_font_property_get_string;
	g_hash_table_insert( hashtable, klass->keyword, klass );
	
	klass = css_dialog_class_new();
	klass->keyword = g_strdup("(font-family)");
	klass->create_function = &create_font_family_dialog;
	klass->get_string_function = &cssdialog_font_family_get_string;
	g_hash_table_insert( hashtable, klass->keyword, klass );	
	
	klass = css_dialog_class_new();
	klass->keyword = g_strdup("(font-size)");
	klass->create_function = &create_font_size_dialog;
	klass->get_string_function = &cssdialog_font_size_get_string;
	g_hash_table_insert( hashtable, klass->keyword, klass );	
	
	klass = css_dialog_class_new();
	klass->keyword = g_strdup("(frequency)");
	klass->create_function = &create_frecuency_property_dialog;
	klass->get_string_function = &cssdialog_frecuenzy_property_get_string;
	g_hash_table_insert( hashtable, klass->keyword, klass );	
	
	klass = css_dialog_class_new();
	klass->keyword = g_strdup("(identifier)");
	klass->create_function = &create_identifier_value_dialog;
	klass->get_string_function = &cssdialog_identifier_value_get_string;
	g_hash_table_insert( hashtable, klass->keyword, klass );	
	
	klass = css_dialog_class_new();
	klass->keyword = g_strdup("(integer)");
	klass->create_function = &create_integer_value_dialog;
	klass->get_string_function = &cssdialog_integer_value_get_string;
	g_hash_table_insert( hashtable, klass->keyword, klass );	
	
	klass = css_dialog_class_new();
	klass->keyword = g_strdup("(length)");
	klass->create_function = &create_length_value_dialog;
	klass->get_string_function = &cssdialog_length_value_get_string;
	g_hash_table_insert( hashtable, klass->keyword, klass );	
	
	klass = css_dialog_class_new();
	klass->keyword = g_strdup("(list-style)");
	klass->create_function = &create_list_style_dialog;
	klass->get_string_function = &cssdialog_list_style_get_string;
	g_hash_table_insert( hashtable, klass->keyword, klass );	
	
	klass = css_dialog_class_new();
	klass->keyword = g_strdup("(margin)");
	klass->create_function = &create_margin_property_dialog;
	klass->get_string_function = &cssdialog_margin_property_get_string;
	g_hash_table_insert( hashtable, klass->keyword, klass );	
	
	klass = css_dialog_class_new();
	klass->keyword = g_strdup("(margin-width)");
	klass->create_function = &create_margin_width_value_dialog;
	klass->get_string_function = &cssdialog_margin_width_get_string;
	g_hash_table_insert( hashtable, klass->keyword, klass );	
	
	klass = css_dialog_class_new();
	klass->keyword = g_strdup("(number)");
	klass->create_function = &create_number_value_dialog;
	klass->get_string_function = &cssdialog_number_value_get_string;
	g_hash_table_insert( hashtable, klass->keyword, klass );	
	
	klass = css_dialog_class_new();
	klass->keyword = g_strdup("(padding)");
	klass->create_function = &create_padding_property_dialog;
	klass->get_string_function = &cssdialog_padding_property_get_string;
	g_hash_table_insert( hashtable, klass->keyword, klass );	
	
	klass = css_dialog_class_new();
	klass->keyword = g_strdup("(outline)");
	klass->create_function = &create_outline_property_dialog;
	klass->get_string_function = &cssdialog_outline_property_get_string;
	g_hash_table_insert( hashtable, klass->keyword, klass );	
	
	klass = css_dialog_class_new();
	klass->keyword = g_strdup("(pause)");
	klass->create_function = &create_pause_property_dialog;
	klass->get_string_function = &cssdialog_pause_property_get_string;
	g_hash_table_insert( hashtable, klass->keyword, klass );	
	
	klass = css_dialog_class_new();
	klass->keyword = g_strdup("(percentage)");
	klass->create_function = &create_percentage_value_dialog;
	klass->get_string_function = &cssdialog_percentage_value_get_string;
	g_hash_table_insert( hashtable, klass->keyword, klass );	
	
	klass = css_dialog_class_new();
	klass->keyword = g_strdup("(percentage-length)");
	klass->create_function = &create_percentage_length_value_dialog;
	klass->get_string_function = &cssdialog_percentage_length_value_get_string;
	g_hash_table_insert( hashtable, klass->keyword, klass );	
	
	klass = css_dialog_class_new();
	klass->keyword = g_strdup("(play-during)");
	klass->create_function = &create_play_during_property_dialog;
	klass->get_string_function = &cssdialog_play_during_property_get_string;
	g_hash_table_insert( hashtable, klass->keyword, klass );	
	
	klass = css_dialog_class_new();
	klass->keyword = g_strdup("(quotes)");
	klass->create_function = &create_quotes_property_dialog;
	klass->get_string_function = &cssdialog_quotes_property_get_string;
	g_hash_table_insert( hashtable, klass->keyword, klass );	
	
	klass = css_dialog_class_new();
	klass->keyword = g_strdup("(shape)");
	klass->create_function = &create_shape_value_dialog;
	klass->get_string_function = &cssdialog_shape_value_get_string;
	g_hash_table_insert( hashtable, klass->keyword, klass );	
	
	klass = css_dialog_class_new();
	klass->keyword = g_strdup("(size-length)");
	klass->create_function = &create_size_length_value_dialog;
	klass->get_string_function = &cssdialog_size_length_value_get_string;
	g_hash_table_insert( hashtable, klass->keyword, klass );	
	
	klass = css_dialog_class_new();
	klass->keyword = g_strdup("(text-shadow)");
	klass->create_function = &create_text_shadow_value_dialog;
	klass->get_string_function = &cssdialog_text_shadow_value_get_string;
	g_hash_table_insert( hashtable, klass->keyword, klass );	
	
	klass = css_dialog_class_new();
	klass->keyword = g_strdup("(voice-family)");
	klass->create_function = &create_voice_family_value_dialog;
	klass->get_string_function = &cssdialog_voice_family_value_get_string;
	g_hash_table_insert( hashtable, klass->keyword, klass );	
	
	klass = css_dialog_class_new();
	klass->keyword = g_strdup("(width-style-color)");
	klass->create_function = &create_border_width_style_color_value_dialog;
	klass->get_string_function = &cssdialog_border_width_style_color_value_get_string;
	g_hash_table_insert( hashtable, klass->keyword, klass );	
	
	klass = css_dialog_class_new();
	klass->keyword = g_strdup("(border-style)");
	klass->create_function = &create_border_style_dialog;
	klass->get_string_function = &cssdialog_border_style_get_string;
	g_hash_table_insert( hashtable, klass->keyword, klass );	
	
	return hashtable;	
}
