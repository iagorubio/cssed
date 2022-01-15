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

#include "cssdialogs-strings.h"
#include "cssedwindow.h"
#include "document.h"
#include "utils.h"
#include "support.h"

enum {
	COL_RIGHT,
	COL_BELOW,
	COL_BLUR,
	COL_COLOR
};

enum {
	COL_GENERIC,
	COL_SPECIFIC
};

gchar*
cssdialog_angle_property_get_string	( CssedWindow* window, gpointer data )
{
	GtkWidget *entry_angle;
	GtkWidget *combo_entry_units;
	gchar* output_string;
	gchar *angle;
	gchar *units;
	GtkWidget* parent;	
	CssedDoc* doc;
	
	parent = GTK_WIDGET(data);
	doc = document_get_current( window );
	
	entry_angle = lookup_widget ( parent, "entry_angle");
	combo_entry_units =
		lookup_widget (parent, "combo_entry_units");
	
	angle = gtk_editable_get_chars (GTK_EDITABLE (entry_angle), 0, -1);
	units = gtk_editable_get_chars (GTK_EDITABLE (combo_entry_units), 0,-1);
	
	if (strlen (angle) > 0)
		output_string = g_strdup_printf("%s", angle);
	else
		output_string = g_strdup_printf("0");

	output_string = allocated_buffer_add_string(output_string, units);

	g_free(angle);
	g_free(units);
	
	return output_string;	
}

gchar*
cssdialog_background_property_get_string	( CssedWindow* window, gpointer data )
{
	GtkWidget *colorselection;
	GtkWidget *checkbutton_bg_color;
	GtkWidget *combo_entry_bg_color;
	GtkWidget *checkbutton_bg_position;
	GtkWidget *radiobutton_bg_pos_perclen;
//	GtkWidget *radiobutton_bg_pos_fixed;
	GtkWidget *spinbutton_upper_left;
	GtkWidget *combo_entry_bg_perclen_upper_left;
	GtkWidget *combo_entry_bg_pos_fixed;
	GtkWidget *spinbutton_lower_right;
//	GtkWidget *combo_entry_lower_right;
	GtkWidget *checkbutton_bg_image;
//	GtkWidget *combo_entry_bg_image;
	GtkWidget *entry_uri;
	GtkWidget *checkbutton_bg_repeat;
	GtkWidget *combo_entry_bg_repeat;
	GtkWidget *checkbutton_bg_attachment;
	GtkWidget *combo_entry_bg_attachment;	
	gchar *output_string;
	gchar *colorstr;
	GdkColor gdkcolor;
	gchar *position;
	gchar *position_upper_left;
	gchar *position_lower_right;
	gchar *position_upper_left_unit;
	gint nposition_uper_left;
	gint nposition_lower_right;
	gfloat fposition_uper_left;
	gfloat fposition_lower_right;
	gchar *imagestr;
	gchar *repeatstr;
	gchar *attachmentstr;
	gboolean bimage;
	gboolean bcolor;
	gboolean bposition;
	gboolean bposperclen;
	gboolean brepeat;
	gboolean battachment;
	GtkWidget* parent;	
	CssedDoc* doc;
	
	doc = document_get_current( window );	
	parent = GTK_WIDGET(data);
	
	checkbutton_bg_image =
		lookup_widget (parent, "checkbutton_bg_image");
	bimage = gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON
					       					(checkbutton_bg_image));
	checkbutton_bg_color =
		lookup_widget (parent, "checkbutton_bg_color");
	bcolor = gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON
					       					(checkbutton_bg_color));
	checkbutton_bg_position = lookup_widget (parent,"checkbutton_bg_position");
	bposition =
		gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON(checkbutton_bg_position));
	checkbutton_bg_repeat = lookup_widget (parent, "checkbutton_bg_repeat");
	brepeat = gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON
					      					(checkbutton_bg_repeat));
	checkbutton_bg_attachment =
		lookup_widget (parent,
			       "checkbutton_bg_attachment");
	battachment =
		gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON
					      (checkbutton_bg_attachment));
						  
	if (!bimage && !bcolor && !bposition && !brepeat && !battachment)
	{
		// background none then
		output_string = g_strdup("none");
		document_write_error_to_program_output(doc,_("background dialog: no value selected, defaulting to none"),WARNING_COLOR_STRING);
		return output_string;
	}
	output_string = g_strdup("");
	// image
	if (bimage)
	{
		entry_uri = lookup_widget ( parent, "entry_uri");
		imagestr =
			gtk_editable_get_chars (GTK_EDITABLE (entry_uri), 0, -1);
		output_string = allocated_buffer_add_string(output_string, "url(\"");
		output_string = allocated_buffer_add_string(output_string, imagestr);
		output_string = allocated_buffer_add_string(output_string, "\") ");
		g_free (imagestr);
	}

	// bgcolor
	if (bcolor)
	{
		combo_entry_bg_color =
			lookup_widget (parent,
				       "combo_entry_bg_color");
		colorstr =
			gtk_editable_get_chars (GTK_EDITABLE
						(combo_entry_bg_color), 0,
						-1);

		if (strcmp (colorstr, CSS_STR_USE_COLORSEL) == 0)
		{
			g_free (colorstr);
			colorselection =
				lookup_widget (parent,
					       "colorselection");
			gtk_color_selection_get_current_color
				(GTK_COLOR_SELECTION (colorselection),
				 &gdkcolor);
			colorstr =
				gdk_color_to_rgb_triplet_string (&gdkcolor);
		}
		output_string = allocated_buffer_add_string(output_string, colorstr);
		output_string = allocated_buffer_add_string(output_string, " ");
		g_free (colorstr);
	}
	//position
	if (bposition)
	{
		// perclen
		radiobutton_bg_pos_perclen =
			lookup_widget (parent,
				       "radiobutton_bg_pos_perclen");
		bposperclen =
			gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON
						      (radiobutton_bg_pos_perclen));
		if (bposperclen)
		{
			spinbutton_upper_left =
				lookup_widget (parent,
					       "spinbutton_upper_left");
			spinbutton_lower_right =
				lookup_widget (parent,
					       "spinbutton_lower_right");
			combo_entry_bg_perclen_upper_left =
				lookup_widget (parent,
					       "combo_entry_unit");

			position_upper_left_unit =
				gtk_editable_get_chars (GTK_EDITABLE
							(combo_entry_bg_perclen_upper_left),
							0, -1);
/**/		if( strcmp( position_upper_left_unit, "em") == 0 ||
				strcmp( position_upper_left_unit, "in") == 0 )
			{
				fposition_uper_left = (gfloat)
					gtk_spin_button_get_value_as_float
					(GTK_SPIN_BUTTON (spinbutton_upper_left));
				fposition_lower_right = (gfloat)
					gtk_spin_button_get_value_as_float
					(GTK_SPIN_BUTTON (spinbutton_lower_right));
	
				position_upper_left =
					g_strdup_printf ("%0.2f%s", fposition_uper_left,
							 position_upper_left_unit);
				position_lower_right =
					g_strdup_printf ("%0.2f%s", fposition_lower_right,
							 position_upper_left_unit);			
			}else{
				nposition_uper_left =
					gtk_spin_button_get_value_as_int
					(GTK_SPIN_BUTTON (spinbutton_upper_left));
				nposition_lower_right =
					gtk_spin_button_get_value_as_int
					(GTK_SPIN_BUTTON (spinbutton_lower_right));
	
				position_upper_left =
					g_strdup_printf ("%d%s", nposition_uper_left,
							 position_upper_left_unit);
				position_lower_right =
					g_strdup_printf ("%d%s",
							 nposition_lower_right,
							 position_upper_left_unit);			
			}

			//g_strdup_printf
			output_string = allocated_buffer_add_string(output_string, position_upper_left);
			output_string = allocated_buffer_add_string(output_string, " ");
			output_string = allocated_buffer_add_string(output_string, position_lower_right);
			output_string = allocated_buffer_add_string(output_string, " ");

			g_free (position_upper_left_unit);
			g_free (position_lower_right);
			g_free (position_upper_left);
		}
		else
		{
			combo_entry_bg_pos_fixed =
				lookup_widget (parent,
					       "combo_entry_bg_pos_fixed");
			position =
				gtk_editable_get_chars (GTK_EDITABLE
							(combo_entry_bg_pos_fixed),
							-0, -1);
			output_string = allocated_buffer_add_string(output_string, position);
			output_string = allocated_buffer_add_string(output_string, " ");
			g_free (position);
		}
	}
	//repeat
	if (brepeat)
	{
		combo_entry_bg_repeat =	lookup_widget (parent, "combo_entry_bg_repeat");
		repeatstr =	gtk_editable_get_chars (GTK_EDITABLE(combo_entry_bg_repeat),
							-0,	-1);
		output_string = allocated_buffer_add_string(output_string, repeatstr);
		output_string = allocated_buffer_add_string(output_string, " ");
		g_free (repeatstr);
	}
	//attachment
	if (battachment)
	{
		combo_entry_bg_attachment =
			lookup_widget (parent,
				       "combo_entry_bg_attachment");
		attachmentstr =
			gtk_editable_get_chars (GTK_EDITABLE
						(combo_entry_bg_attachment),
						-0, -1);
		
		output_string = allocated_buffer_add_string(output_string, attachmentstr);
		g_free (attachmentstr);
	}
	
	return output_string;
}

gchar*
cssdialog_border_properties_get_string	( CssedWindow* window, gpointer data )
{
	GtkWidget *checkbutton_border_length;
	GtkWidget *combo_entry_border_length_selector;
	GtkWidget *spinbutton_len;
	GtkWidget *combo_entry_len;
	GtkWidget *checkbutton_border_style;
	GtkWidget *combo_entry_border_style;
	GtkWidget *checkbutton_border_color;
	GtkWidget *combo_entry_border_color;
	GtkWidget *colorselection;
	GdkColor gdkcolor;
	gboolean blength;
	gboolean bstyle;
	gboolean bcolor;
	gchar *strlength;
	gchar *strunit;
	gchar *strlenunit;
	gchar *strstyle;
	gchar *strcolor;
	gchar* output_string;
	gint len;
	gfloat flen;
	GtkWidget* parent;	
	CssedDoc* doc;
	
	parent = GTK_WIDGET(data);
	doc = document_get_current( window );	
	
	checkbutton_border_length =
		lookup_widget (parent,
			       "checkbutton_border_length");
	checkbutton_border_style =
		lookup_widget (parent,
			       "checkbutton_border_style");
	checkbutton_border_color =
		lookup_widget (parent,
			       "checkbutton_border_color");

	blength =
		gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON
					      (checkbutton_border_length));
	bstyle = gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON
					       (checkbutton_border_style));
	bcolor = gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON
					       (checkbutton_border_color));
						   
	if (!blength && !bstyle && !bcolor)
	{
		output_string = g_strdup("none");
		document_write_error_to_program_output(doc,_("border dialog: no value selected defaulting to \"none\""),WARNING_COLOR_STRING);
	}
	else
	{
		output_string = g_strdup(""); // empty but not NULL string
		// length
		if (blength)
		{
			combo_entry_border_length_selector =
				lookup_widget (parent,
					       "combo_entry_border_length_selector");
			strlength =
				gtk_editable_get_chars (GTK_EDITABLE
							(combo_entry_border_length_selector),
							-0, -1);

			if (strcmp (strlength, CSS_STR_USE_LENGTH) == 0)
			{
				spinbutton_len =
					lookup_widget (parent,
						       "spinbutton_len");

				combo_entry_len =
					lookup_widget (parent,
						       "combo_entry_len");
				strunit =
					gtk_editable_get_chars (GTK_EDITABLE
								(combo_entry_len),
								-0, -1);
				
				if( strcmp( strunit, "em" ) == 0 ||
					strcmp( strunit, "in" ) == 0  ){
					flen = (gfloat) gtk_spin_button_get_value_as_float( GTK_SPIN_BUTTON 
															(spinbutton_len) );
					strlenunit =g_strdup_printf ("%0.2f%s ", flen, strunit);		
				}else{
					len = gtk_spin_button_get_value_as_int( GTK_SPIN_BUTTON 
															(spinbutton_len) );
					strlenunit =g_strdup_printf ("%d%s ", len, strunit);					
				}

				output_string = allocated_buffer_add_string(output_string, strlenunit);
				g_free (strlenunit);
				g_free (strunit);
			}
			else
			{
				output_string = allocated_buffer_add_string(output_string, strlength);
				output_string = allocated_buffer_add_string(output_string, " ");
			}
			g_free (strlength);
		}
		// style
		if (bstyle)
		{
			combo_entry_border_style =
				lookup_widget (parent,
					       "combo_entry_border_style");
			strstyle =
				gtk_editable_get_chars (GTK_EDITABLE
							(combo_entry_border_style),
							-0, -1);
			output_string = allocated_buffer_add_string(output_string, strstyle);
			output_string = allocated_buffer_add_string(output_string, " ");
			g_free (strstyle);
		}
		// color
		if (bcolor)
		{
			combo_entry_border_color =
				lookup_widget (parent,
					       "combo_entry_border_color");
			strcolor =
				gtk_editable_get_chars (GTK_EDITABLE
							(combo_entry_border_color),
							-0, -1);

			if (strcmp (strcolor, CSS_STR_USE_COLORSEL) == 0)
			{
				g_free (strcolor);
				colorselection =
					lookup_widget (parent,
						       "colorselection");
				gtk_color_selection_get_current_color
					(GTK_COLOR_SELECTION (colorselection),
					 &gdkcolor);
				strcolor =
					gdk_color_to_rgb_triplet_string
					(&gdkcolor);
				output_string = allocated_buffer_add_string(output_string, strcolor);
			}
			else
			{
				output_string = allocated_buffer_add_string(output_string, strcolor);
			}
			g_free (strcolor);
		}
	} 
	return output_string;
}

gchar*
cssdialog_border_width_properties_get_string ( CssedWindow* window, gpointer data )
{
	GtkWidget *checkbutton_border_width_top;
	GtkWidget *combo_entry_border_width_top_units;
	GtkWidget *combo_entry_border_width_top_selector;
	GtkWidget *spinbutton_border_width_top;
	GtkWidget *checkbutton_border_width_right;
	GtkWidget *combo_entry_border_width_right_units;
	GtkWidget *combo_entry_border_width_right_selector;
	GtkWidget *spinbutton_border_width_right;
	GtkWidget *checkbutton_border_width_bottom;
	GtkWidget *combo_entry_border_width_bottom_units;
	GtkWidget *combo_entry_border_width_bottom_selector;
	GtkWidget *spinbutton_border_width_bottom;
	GtkWidget *checkbutton_border_width_left;
	GtkWidget *combo_entry_border_width_left_units;
	GtkWidget *combo_entry_border_width_left_selector;
	GtkWidget *spinbutton_border_width_left;
	gboolean btop;
	gboolean bright;
	gboolean bbottom;
	gboolean bleft;
	gchar *strout_top;
	gchar *strlength_top;
	gchar *strunit_top;
	gchar *strout_right;
	gchar *strlength_right;
	gchar *strunit_right;
	gchar *strout_bottom;
	gchar *strlength_bottom;
	gchar *strunit_bottom;
	gchar *strout_left;
	gchar *strlength_left;
	gchar *strunit_left;
	gint nlength;
	gfloat flength;
	gchar* output_string;
	GtkWidget* parent;	
	CssedDoc* doc;
	
	parent = GTK_WIDGET(data);
	doc = document_get_current( window );	
	
	checkbutton_border_width_top =
		lookup_widget (parent,
			       "checkbutton_border_width_top");
	checkbutton_border_width_right =
		lookup_widget (parent,
			       "checkbutton_border_width_right");
	checkbutton_border_width_bottom =
		lookup_widget (parent,
			       "checkbutton_border_width_bottom");
	checkbutton_border_width_left =
		lookup_widget (parent,
			       "checkbutton_border_width_left");

	btop = gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON
					     (checkbutton_border_width_top));
	bright = gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON
					       (checkbutton_border_width_right));
	bbottom =
		gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON
					      (checkbutton_border_width_bottom));
	bleft = gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON
					      (checkbutton_border_width_left));
						  
	if (!btop && !bright && !bbottom && !bleft)
	{
		output_string = g_strdup("none");
		document_write_error_to_program_output(doc,_("border width single dialog: no value selected defaulting to \"none\""),WARNING_COLOR_STRING);
	}
	else
	{
		output_string = g_strdup("");
		// TOP
		if ( btop )
		{
			combo_entry_border_width_top_selector =
				lookup_widget (parent,
					       "combo_entry_border_width_top_selector");
			strlength_top =
				gtk_editable_get_chars (GTK_EDITABLE
							(combo_entry_border_width_top_selector),
							-0, -1);
			if (strcmp (strlength_top, CSS_STR_USE_LENGTH) == 0)
			{
				combo_entry_border_width_top_units =
					lookup_widget (parent,
						       "combo_entry_border_width_top_units");
				strunit_top =
					gtk_editable_get_chars (GTK_EDITABLE
								(combo_entry_border_width_top_units),
								-0, -1);
				spinbutton_border_width_top =
					lookup_widget (parent,
						       "spinbutton_border_width_top");
				
				if( strcmp( strunit_top, "em" ) == 0 ||
					strcmp( strunit_top, "in" ) == 0 ){
					flength = (gfloat)
						gtk_spin_button_get_value_as_float
						(GTK_SPIN_BUTTON
						 (spinbutton_border_width_top));
					strout_top =
						g_strdup_printf ("%0.2f%s", flength,
								 strunit_top);		
				}else{
					nlength =
						gtk_spin_button_get_value_as_int
						(GTK_SPIN_BUTTON
						 (spinbutton_border_width_top));
					strout_top =
						g_strdup_printf ("%d%s", nlength,
								 strunit_top);
				}				
				
				output_string = allocated_buffer_add_string(output_string, strout_top);
				output_string = allocated_buffer_add_string(output_string, " ");
				g_free (strout_top);
				g_free (strunit_top);
			}
			else
			{
				output_string = allocated_buffer_add_string(output_string, strlength_top);
				output_string = allocated_buffer_add_string(output_string, " ");
			}
			g_free (strlength_top);
		}
		// RIGHT
		if (bright)
		{
			combo_entry_border_width_right_selector =
				lookup_widget (parent,
					       "combo_entry_border_width_right_selector");
			strlength_right =
				gtk_editable_get_chars (GTK_EDITABLE
							(combo_entry_border_width_right_selector),
							-0, -1);
			if (strcmp (strlength_right, CSS_STR_USE_LENGTH) == 0)
			{
				combo_entry_border_width_right_units =
					lookup_widget (parent,
						       "combo_entry_border_width_right_units");
				strunit_right =
					gtk_editable_get_chars (GTK_EDITABLE
								(combo_entry_border_width_right_units),
								-0, -1);
				spinbutton_border_width_right =
					lookup_widget (parent,
						       "spinbutton_border_width_right");
				nlength =
					gtk_spin_button_get_value_as_int
					(GTK_SPIN_BUTTON
					 (spinbutton_border_width_right));
				strout_right =
					g_strdup_printf ("%d%s", nlength,
							 strunit_right);
				output_string = allocated_buffer_add_string(output_string, strout_right);
				output_string = allocated_buffer_add_string(output_string, " ");
				g_free (strout_right);
				g_free (strunit_right);
			}
			else
			{
				output_string = allocated_buffer_add_string(output_string, strlength_right);
				output_string = allocated_buffer_add_string(output_string, " ");
			}
			g_free (strlength_right);
		}
		// BOTTOM
		if (bbottom)
		{
			combo_entry_border_width_bottom_selector =
				lookup_widget (parent,
					       "combo_entry_border_width_bottom_selector");
			strlength_bottom =
				gtk_editable_get_chars (GTK_EDITABLE
							(combo_entry_border_width_bottom_selector),
							-0, -1);
			if (strcmp (strlength_bottom, CSS_STR_USE_LENGTH) == 0)
			{
				combo_entry_border_width_bottom_units =
					lookup_widget (parent,
						       "combo_entry_border_width_bottom_units");
				strunit_bottom =
					gtk_editable_get_chars (GTK_EDITABLE
								(combo_entry_border_width_bottom_units),
								-0, -1);
				spinbutton_border_width_bottom =
					lookup_widget (parent,
						       "spinbutton_border_width_bottom");
				nlength =
					gtk_spin_button_get_value_as_int
					(GTK_SPIN_BUTTON
					 (spinbutton_border_width_bottom));
				strout_bottom =
					g_strdup_printf ("%d%s", nlength,
							 strunit_bottom);
				output_string = allocated_buffer_add_string(output_string, strout_bottom);
				output_string = allocated_buffer_add_string(output_string, " ");
				g_free (strout_bottom);
				g_free (strunit_bottom);
			}
			else
			{
				output_string = allocated_buffer_add_string(output_string, strlength_bottom);
				output_string = allocated_buffer_add_string(output_string, " ");
			}
			g_free (strlength_bottom);
		}
		// LEFT
		if (bleft)
		{
			combo_entry_border_width_left_selector =
				lookup_widget (parent,
					       "combo_entry_border_width_left_selector");
			strlength_left =
				gtk_editable_get_chars (GTK_EDITABLE
							(combo_entry_border_width_left_selector),
							-0, -1);
			if (strcmp (strlength_left, CSS_STR_USE_LENGTH) == 0)
			{
				combo_entry_border_width_left_units =
					lookup_widget (parent,
						       "combo_entry_border_width_left_units");
				strunit_left =
					gtk_editable_get_chars (GTK_EDITABLE
								(combo_entry_border_width_left_units),
								-0, -1);
				spinbutton_border_width_left =
					lookup_widget (parent,
						       "spinbutton_border_width_left");
				nlength =
					gtk_spin_button_get_value_as_int
					(GTK_SPIN_BUTTON
					 (spinbutton_border_width_left));
				strout_left =
					g_strdup_printf ("%d%s", nlength,
							 strunit_left);
				output_string = allocated_buffer_add_string(output_string, strout_left);
				output_string = allocated_buffer_add_string(output_string, " ");
				g_free (strout_left);
				g_free (strunit_left);
			}
			else
			{
				output_string = allocated_buffer_add_string(output_string, strlength_left);
				output_string = allocated_buffer_add_string(output_string, " ");
			}
			g_free (strlength_left);
		}
	}
	
	return output_string;
}
gchar*
cssdialog_border_width_single_get_string ( CssedWindow* window, gpointer data )
{
	GtkWidget *combo_entry_selector;
	GtkWidget *spinbutton_length;
	GtkWidget *combo_entry_units;
	gchar *strwidth;
	gchar *strunit;
	gchar *strout;
	gchar* output_string;
	gint len;
	GtkWidget* parent;	
	CssedDoc* doc;
	
	parent = GTK_WIDGET(data);
	doc = document_get_current( window );
	
	combo_entry_selector =
		lookup_widget (parent, "combo_entry_selector");
	
	strwidth =
		gtk_editable_get_chars (GTK_EDITABLE (combo_entry_selector), 0, -1);
	
	output_string = g_strdup("");
	
	if (strcmp (strwidth,CSS_STR_USE_PERCLEN) == 0)
	{
		spinbutton_length =
			lookup_widget (parent,
				       "spinbutton_length");
		combo_entry_units =
			lookup_widget (parent,
				       "combo_entry_units");
		len = gtk_spin_button_get_value_as_int (GTK_SPIN_BUTTON
							(spinbutton_length));
		strunit =
			gtk_editable_get_chars (GTK_EDITABLE
						(combo_entry_units), 0, -1);
		strout = g_strdup_printf ("%d%s", len, strunit);
		output_string = allocated_buffer_add_string(output_string, strout);
		g_free (strout);
		g_free (strunit);
	}
	else
	{
		output_string = allocated_buffer_add_string(output_string, strwidth);
	}
	g_free (strwidth);
	
	return output_string;
}

gchar*
cssdialog_counter_increment_reset_get_string ( CssedWindow* window, gpointer data )
{
	GtkTreeModel *list_store;
	GtkWidget *spinbutton;
	GtkWidget *entry;
	GtkWidget *treeview;
	GtkTreeIter iter;
	gboolean valid;
	gchar *strout;
	gchar *str_data;
	gint int_data;
	gchar* output_string;
	GtkWidget* parent;	
	CssedDoc* doc;
	
	parent = GTK_WIDGET(data);
	doc = document_get_current( window );
	
	spinbutton =
		lookup_widget (parent,
			       "spinbutton_counter_increment_reset_step");
	entry = lookup_widget (parent,
			       "entry_counter_increment_reset_identifyer");
	
	treeview = lookup_widget (parent, "treeview");
	list_store = gtk_tree_view_get_model (GTK_TREE_VIEW (treeview));
	valid = gtk_tree_model_get_iter_first (list_store, &iter);
	
	if (valid)
	{	// there are values in the store
		// parse the list
		output_string = g_strdup("");
		while (valid)
		{
			gtk_tree_model_get (list_store, &iter,
					    0, &str_data, 1, &int_data, -1);
			
			strout = g_strdup_printf(" %s %d",str_data,int_data);
			output_string = allocated_buffer_add_string(output_string, strout);

			g_free (strout);
			g_free (str_data);
			valid = gtk_tree_model_iter_next (list_store, &iter);
		}
	}
	else
	{
		output_string = g_strdup("");
		spinbutton =
			lookup_widget (parent,
				       "spinbutton_counter_increment_reset_step");
		entry = lookup_widget (parent,
				       "entry_counter_increment_reset_identifyer");

		str_data =
			gtk_editable_get_chars (GTK_EDITABLE (entry), 0, -1);
		int_data =
			gtk_spin_button_get_value_as_int (GTK_SPIN_BUTTON
							  (spinbutton));
		
		strout = g_strdup_printf("%s %d ",str_data,int_data);
		output_string = allocated_buffer_add_string(output_string, strout);

		g_free (strout);
		g_free (str_data);
	}
	return output_string;
}

gchar*
cssdialog_string_properties_get_string( CssedWindow* window, gpointer data )
{
	GtkWidget *entry_string_property_dialog;
	gchar *string;
	gchar* output_string;
	GtkWidget* parent;	
	CssedDoc* doc;
	
	parent = GTK_WIDGET(data);
	doc = document_get_current( window );
	
	entry_string_property_dialog =
		lookup_widget (parent, "entry_string_property_dialog");

	string = gtk_editable_get_chars (GTK_EDITABLE
					 (entry_string_property_dialog), 0,
					 -1);
	if (strlen (string) > 0)
	{
		output_string = g_strdup_printf("\"%s\"",string);
		g_free(string);
		return output_string;
	}else{
		return NULL;
	}	
}

gchar*
cssdialog_uri_value_get_string ( CssedWindow* window, gpointer data )
{
	GtkWidget *entry_uri_value_dialog;
	gchar *string;
	gchar* output_string;
	GtkWidget* parent;	
	CssedDoc* doc;
	
	parent = GTK_WIDGET(data);
	doc = document_get_current( window );
	
	entry_uri_value_dialog =
	lookup_widget (parent, "entry_uri_value_dialog");

	string = gtk_editable_get_chars (GTK_EDITABLE
					 (entry_uri_value_dialog), 0, -1);
	
	if (strlen (string) > 0)
	{
		output_string = g_strdup_printf("url(\"%s\")",string);
		g_free(string);
		return output_string;
	}else{
		return NULL;
	}	
}

gchar*
cssdialog_attr_value_get_string ( CssedWindow* window, gpointer data )
{
	GtkWidget *entry_attribute_value_dialog;
	gchar *string;
	gchar* output_string;
	GtkWidget* parent;	
	CssedDoc* doc;
	
	parent = GTK_WIDGET(data);
	doc = document_get_current( window );
	
	entry_attribute_value_dialog =
		lookup_widget (parent,
			       "entry_attribute_value_dialog");

	string = gtk_editable_get_chars (GTK_EDITABLE
					 (entry_attribute_value_dialog), 0,
					 -1);
	
	if (strlen (string) > 0)
	{
		output_string = g_strdup_printf("attr(\"%s\")",string);
		g_free(string);
		return output_string;
	}else{
		return NULL;
	}	
}

gchar*
cssdialog_counter_property_get_string ( CssedWindow* window, gpointer data )
{
	GtkWidget *entry_counter_identifier;
	GtkWidget *checkbutton_after;
	GtkWidget *entry_string_after;
	GtkWidget *checkbutton_before;
	GtkWidget *entry_string_before;
	GtkWidget *checkbutton_list_style_tyle;
	GtkWidget *combo_entry_list_style_type;
	gchar *stridentifier;
	gchar *strafter;
	gchar *strbefore;
	gchar *strstyle;
	gboolean bafter;
	gboolean bbefore;
	gboolean bstyle;
	gchar* output_string;
	GtkWidget* parent;	
	CssedDoc* doc;
	
	parent = GTK_WIDGET(data);
	doc = document_get_current( window );
	
	entry_counter_identifier =
		lookup_widget (parent,
			       "entry_counter_identifier");
	stridentifier =
		gtk_editable_get_chars (GTK_EDITABLE
					(entry_counter_identifier), 0, -1);

	if(strlen(stridentifier) > 0){
		output_string = g_strdup("");
		
		checkbutton_after =
			lookup_widget (parent,
				       "checkbutton_after");
		checkbutton_before =
			lookup_widget (parent,
				       "checkbutton_before");
		checkbutton_list_style_tyle =
			lookup_widget (parent,
				       "checkbutton_list_style_tyle");

		bafter = gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON
						       (checkbutton_after));
		bbefore =
			gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON
						      (checkbutton_before));
		bstyle = gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON
						       (checkbutton_list_style_tyle));

		if (bafter)
		{
			entry_string_after =
				lookup_widget (parent,
					       "entry_string_after");
			strafter =
				gtk_editable_get_chars (GTK_EDITABLE
							(entry_string_after),
							0, -1);
			output_string = allocated_buffer_add_string(output_string, " \"");
			output_string = allocated_buffer_add_string(output_string, strafter);
			output_string = allocated_buffer_add_string(output_string, "\" ");
			g_free (strafter);
		}

		// add the identifier
		output_string = allocated_buffer_add_string(output_string, " counter( ");
		output_string = allocated_buffer_add_string(output_string, stridentifier);

		if (bstyle)
		{
			combo_entry_list_style_type =
				lookup_widget (parent,
					       "combo_entry_list_style_type");
			strstyle =
				gtk_editable_get_chars (GTK_EDITABLE
							(combo_entry_list_style_type),
							0, -1);
			output_string = allocated_buffer_add_string(output_string, ", ");
			output_string = allocated_buffer_add_string(output_string, strstyle);
			g_free (strstyle);
		}

		output_string = allocated_buffer_add_string(output_string, " )");

		if (bbefore)
		{
			entry_string_before =
				lookup_widget (parent,
					       "entry_string_before");
			strbefore =
				gtk_editable_get_chars (GTK_EDITABLE
							(entry_string_before),
							0, -1);
			output_string = allocated_buffer_add_string(output_string, " \"");
			output_string = allocated_buffer_add_string(output_string, strbefore);
			output_string = allocated_buffer_add_string(output_string, "\" ");
			g_free (strbefore);
		}
		
		g_free(stridentifier);
		return output_string;
	}else{
		return NULL;
	}
}

gchar*
cssdialog_font_property_get_string ( CssedWindow* window, gpointer data )
{
	GtkWidget *radiobutton_system_fonts;
	GtkWidget *combo_entry_system_fonts;
	GtkWidget *checkbutton_size;
	GtkWidget *combo_entry_font_size_selector;
	GtkWidget *spinbutton_font_size;
	GtkWidget *combo_entry_font_size_units;
	GtkWidget *checkbutton_line_height;
	GtkWidget *spinbutton_line_height;
	GtkWidget *combo_entry_line_height_selector_and_units;
	GtkWidget *checkbutton_style;
	GtkWidget *combo_entry_style;
	GtkWidget *checkbutton_variant;
	GtkWidget *combo_entry_variant;
	GtkWidget *checkbutton_weight;
	GtkWidget *combo_entry_weight;
	GtkWidget *checkbutton_font_family;
	GtkWidget *combo_entry_font_family;
	GtkWidget *checkbutton_generic_family;
	GtkWidget *combo_entry_generic_family;
	GtkWidget *checkbutton_use_font_set;
	GtkWidget *treeview;	
	GtkTreeModel *list_store;
	GtkTreeIter iter;
	gboolean valid;
	gboolean bsystemfonts;
	gboolean bsize;
	gboolean bheight;
	gboolean bstyle;
	gboolean bvariant;
	gboolean bweight;
	gboolean bfontfamily;
	gboolean bgenericfamily;
	gboolean bfontset;
	gchar* strfullsize;	
	gchar* strsize;
	gchar* strsize_units;
	gint nsize;
	gfloat fsize;
	gchar* strlheight;
	gchar* strfullheight;
	gint nheight;
	gfloat fheight;
	gchar* strstyle;
	gchar* strvariant;
	gchar* strweight;	
	gchar* str_set_font;
	gchar* str_set_generic;	
	gchar* str_font_family;
	gchar* str_gen_family;	
	gchar* output_string;
	GtkWidget* parent;	
	CssedDoc* doc;
	
	parent = GTK_WIDGET(data);
	doc = document_get_current( window );
	
	radiobutton_system_fonts =
		lookup_widget (parent,
			       "radiobutton_system_fonts");
				   
	bsystemfonts = gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON
				      (radiobutton_system_fonts));

	if( bsystemfonts ){
		combo_entry_system_fonts = lookup_widget (parent, 
									"combo_entry_system_fonts");		
		output_string = gtk_editable_get_chars(
							GTK_EDITABLE(combo_entry_system_fonts),
							0,
							-1);
		return output_string;
	}else {
		checkbutton_size =
			lookup_widget (parent, "checkbutton_size");
		checkbutton_line_height =
			lookup_widget (parent, "checkbutton_line_height");
		checkbutton_style =
			lookup_widget (parent, "checkbutton_style");
		checkbutton_variant =
			lookup_widget (parent, "checkbutton_variant");
		checkbutton_weight =
			lookup_widget (parent, "checkbutton_weight");
		checkbutton_font_family =
			lookup_widget (parent,"checkbutton_font_family");
		checkbutton_generic_family =
			lookup_widget (parent, "checkbutton_generic_family");
		checkbutton_use_font_set =
			lookup_widget (parent, "checkbutton_use_font_set");
		
		bsize = gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON
				      (checkbutton_size));
		bheight = gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON
				      (checkbutton_line_height));
		bstyle = gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON
				      (checkbutton_style));
		bvariant = gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON
				      (checkbutton_variant));
		bweight = gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON
				      (checkbutton_weight));
		bfontfamily = gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON
				      (checkbutton_font_family));
		bgenericfamily = gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON
				      (checkbutton_generic_family));
		bfontset = gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON
				      (checkbutton_use_font_set));
					  
		if( bsize || bheight || bstyle || bvariant || bweight || bfontfamily ||
			bgenericfamily || bfontset)
		{ 
			output_string = g_strdup("");
			// font size
			if( bsize ){
				combo_entry_font_size_selector =
				lookup_widget (parent,
							   "combo_entry_font_size_selector");
				
				strsize = gtk_editable_get_chars(
							GTK_EDITABLE(combo_entry_font_size_selector),
							0,-1);
				if( strcmp(strsize,CSS_STR_USE_PERCLEN) == 0){
					spinbutton_font_size =
						lookup_widget (parent, "spinbutton_font_size");
					combo_entry_font_size_units =
						lookup_widget (parent,
								"combo_entry_font_size_units");							
					strsize_units = gtk_editable_get_chars(
								GTK_EDITABLE(combo_entry_font_size_units),0,-1);	
					
					if( strcmp(strsize_units, "em") == 0  ||
						strcmp(strsize_units, "em") == 0  ){
						fsize = (gfloat) gtk_spin_button_get_value_as_float(
										GTK_SPIN_BUTTON(spinbutton_font_size));					
						strfullsize = g_strdup_printf("%0.2f%s ", fsize, 
										strsize_units);							
					}else{
						nsize = gtk_spin_button_get_value_as_int(
										GTK_SPIN_BUTTON(spinbutton_font_size));					
						strfullsize = g_strdup_printf("%d%s ", nsize, 
										strsize_units);								
					}
					
					output_string = allocated_buffer_add_string(output_string, strfullsize);
					
					g_free( strfullsize );
					g_free(	strsize_units );
									
				}else{
					output_string = allocated_buffer_add_string(output_string, strsize);
				}
				output_string = allocated_buffer_add_string(output_string, " ");
				g_free(strsize);
			}
			
			// line height "use number"
			if( bheight ){
				combo_entry_line_height_selector_and_units =
					lookup_widget (parent, 
									"combo_entry_line_height_selector_and_units");
				strlheight = gtk_editable_get_chars(
					GTK_EDITABLE(combo_entry_line_height_selector_and_units),0,-1);
				
				spinbutton_line_height = lookup_widget (parent, 
										"spinbutton_line_height");
				if( strcmp(strlheight, CSS_STR_USE_NUM) == 0){
					nheight = gtk_spin_button_get_value_as_int(
								GTK_SPIN_BUTTON(spinbutton_line_height));				
					strfullheight = g_strdup_printf("%d", nheight);	
					
					output_string = allocated_buffer_add_string(output_string, strfullheight);						
					g_free(strfullheight);				
				}else{					
					if( strcmp(strlheight, "em") == 0  ||
						strcmp(strlheight, "in") == 0  ){
						fheight = (gfloat) gtk_spin_button_get_value_as_float(
										GTK_SPIN_BUTTON(spinbutton_line_height));					
						strfullheight = g_strdup_printf("%0.2f%s ", fheight, 
														strlheight);							
					}else{
						nheight = gtk_spin_button_get_value_as_int(
									GTK_SPIN_BUTTON(spinbutton_line_height));				
						strfullheight = g_strdup_printf("%d%s", nheight, 
														strlheight);							
					}	
					
					output_string = allocated_buffer_add_string(output_string, strfullheight);		
				}
	
				output_string = allocated_buffer_add_string(output_string, " ");
				g_free(strlheight);
			}
			
			// style
			if( bstyle ){
				combo_entry_style =
					lookup_widget (parent, "combo_entry_style");
				strstyle = gtk_editable_get_chars(
					GTK_EDITABLE(combo_entry_style),0,-1);
				output_string = allocated_buffer_add_string(output_string, strstyle);	
				output_string = allocated_buffer_add_string(output_string, " ");
				g_free(strstyle);			
			}		
			// variant		
			if( bvariant ){
				combo_entry_variant =
					lookup_widget (parent, "combo_entry_variant");
				strvariant = gtk_editable_get_chars(
					GTK_EDITABLE(combo_entry_variant),0,-1);
				output_string = allocated_buffer_add_string(output_string, strvariant);	
				output_string = allocated_buffer_add_string(output_string, " ");
				g_free(strvariant);
			}
			// weight	
			if( bweight ){
				combo_entry_weight =
					lookup_widget (parent, "combo_entry_weight");
				strweight = gtk_editable_get_chars(
					GTK_EDITABLE(combo_entry_weight),0,-1);
				output_string = allocated_buffer_add_string(output_string, strweight);	
				output_string = allocated_buffer_add_string(output_string, " ");
				g_free(strweight);
			}
			// font set
			if ( bfontset ){
				treeview = lookup_widget (parent, "treeview");
				list_store = gtk_tree_view_get_model (GTK_TREE_VIEW (treeview));
				valid = gtk_tree_model_get_iter_first (list_store, &iter);
	
				if (valid)
				{	// there are values in the store
					// parser the list
					while (valid)
					{
						gtk_tree_model_get (list_store, &iter,
									0, &str_set_font, 1, &str_set_generic, -1);
			
						if( strlen(str_set_font) > 0){
							output_string = allocated_buffer_add_string(output_string, "\"");
							output_string = allocated_buffer_add_string(output_string, str_set_font);
							output_string = allocated_buffer_add_string(output_string, "\"");
						}
						
						if( ( strlen(str_set_generic) > 0 ) && 
							( strlen(str_set_font)    > 0 )  ){
							output_string = allocated_buffer_add_string(output_string, " , ");
						}
						
						if( strlen(str_set_generic) > 0){
							output_string = allocated_buffer_add_string(output_string, str_set_generic);
						}					
						
						g_free (str_set_font);		
						g_free (str_set_generic);
						valid = gtk_tree_model_iter_next (list_store, &iter);
						if( valid ){
							output_string = allocated_buffer_add_string(output_string, " , ");
						}else{
							output_string = allocated_buffer_add_string(output_string, " ");
						}
					}
				}
			}else{
				// font family			
				if( bfontfamily ){
					combo_entry_font_family =
						lookup_widget (parent, "combo_entry_font_family");
					str_font_family = gtk_editable_get_chars(
							GTK_EDITABLE(combo_entry_font_family),0,-1);
					output_string = allocated_buffer_add_string(output_string, "\"");
					output_string = allocated_buffer_add_string(output_string, str_font_family);
					output_string = allocated_buffer_add_string(output_string, "\"");
					g_free(str_font_family);		
				}
				
				if( bgenericfamily ){
					if (bfontfamily) { 
						output_string = allocated_buffer_add_string(output_string, " , "); 
					}
					combo_entry_generic_family =
						lookup_widget (parent, "combo_entry_generic_family");
					str_gen_family = gtk_editable_get_chars(
							GTK_EDITABLE(combo_entry_generic_family),0,-1);
					output_string = allocated_buffer_add_string(output_string, str_gen_family);	
					g_free(str_gen_family);
				}	
			}	
		}else{
			return NULL;
		}
	}
	return output_string;
}


gchar*
cssdialog_font_family_get_string ( CssedWindow* window, gpointer data )
{
	GtkWidget *radiobutton_system_fonts;
	GtkWidget *combo_entry_system_fonts;	
	GtkWidget *checkbutton_font_family;
	GtkWidget *combo_entry_font_family;
	GtkWidget *checkbutton_generic_family;
	GtkWidget *combo_entry_generic_family;
	GtkWidget *checkbutton_font_set;
	GtkWidget *treeview;	
	GtkTreeModel *list_store;
	GtkTreeIter iter;
	gboolean valid;	
	gboolean bsystemfonts;
	gboolean bfontfamily;
	gboolean bgenericfamily;
	gboolean bfontset;	
	gchar* str_set_font;
	gchar* str_set_generic;	
	gchar* str_font_family;
	gchar* str_gen_family;	
	gchar* output_string;
	GtkWidget* parent;	
	CssedDoc* doc;
	
	parent = GTK_WIDGET(data);
	doc = document_get_current( window );
	
	radiobutton_system_fonts = lookup_widget(parent,"radiobutton_system_fonts");		
	checkbutton_font_family = lookup_widget(parent,"checkbutton_font_family");	
	checkbutton_generic_family = lookup_widget(parent,"checkbutton_generic_family");
	checkbutton_font_set = lookup_widget(parent,"checkbutton_font_set");
	
	bsystemfonts = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(radiobutton_system_fonts));	
	bfontfamily = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(checkbutton_font_family));
	bgenericfamily = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(checkbutton_generic_family));
	bfontset = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(checkbutton_font_set));
	
	if( bsystemfonts ){
		combo_entry_system_fonts = lookup_widget(parent,"combo_entry_system_fonts");
		output_string = gtk_editable_get_chars(GTK_EDITABLE(combo_entry_system_fonts),0,-1);
		return output_string;
	}else{
		if( bfontset ){
			treeview = lookup_widget (parent, "treeview");
			list_store = gtk_tree_view_get_model (GTK_TREE_VIEW (treeview));
			valid = gtk_tree_model_get_iter_first (list_store, &iter);

			if (valid)
			{	// there are values in the store
				// parser the list
				output_string = g_strdup("");
				while (valid)
				{
					gtk_tree_model_get (list_store, &iter,
								0, &str_set_font, 1, &str_set_generic, -1);
		
					if( strlen(str_set_font) > 0){
						output_string = allocated_buffer_add_string(output_string, "\"");
						output_string = allocated_buffer_add_string(output_string, str_set_font);
						output_string = allocated_buffer_add_string(output_string, "\"");
					}
					
					if( ( strlen(str_set_generic) > 0 ) && 
						( strlen(str_set_font)    > 0 )  ){
						output_string = allocated_buffer_add_string(output_string, " , ");
					}
					
					if( strlen(str_set_generic) > 0){
						output_string = allocated_buffer_add_string(output_string, str_set_generic);
					}					
					
					g_free (str_set_font);		
					g_free (str_set_generic);
					valid = gtk_tree_model_iter_next (list_store, &iter);
					if( valid ){
						output_string = allocated_buffer_add_string(output_string, " , ");
					}else{
						output_string = allocated_buffer_add_string(output_string, " ");
					}
				}
				return output_string;
			}else { // not valid, nothing in the list
				return NULL;
			}
		}else {				
			if( bfontfamily || bgenericfamily){
				output_string = g_strdup("");
				
				combo_entry_font_family = lookup_widget(parent,"combo_entry_font_family");
				combo_entry_generic_family = lookup_widget(parent,"combo_entry_generic_family");
				str_font_family = gtk_editable_get_chars(GTK_EDITABLE(combo_entry_font_family),0,-1);
				str_gen_family = gtk_editable_get_chars(GTK_EDITABLE(combo_entry_generic_family),0,-1);	

				if( bfontfamily ){
					output_string = allocated_buffer_add_string(output_string, "\"");
					output_string = allocated_buffer_add_string(output_string, str_font_family);
					output_string = allocated_buffer_add_string(output_string, "\"");
				}
				if( bfontfamily && bgenericfamily){
					output_string = allocated_buffer_add_string(output_string, " , ");
				}
				if( bgenericfamily ){
					output_string = allocated_buffer_add_string(output_string, str_gen_family);
				}
				g_free(str_font_family);
				g_free(str_gen_family);
				
				return output_string;
			}else{
				return NULL;
			}
		}
	}	
}

gchar*
cssdialog_font_size_get_string( CssedWindow* window, gpointer data )
{
	GtkWidget *combo_entry_font_size;
	GtkWidget *spinbutton_font_size;
	GtkWidget *combo_entry_font_size_units;
	gchar* strsize;
	gchar* strunit;
	gint nsize;
	gfloat fsize;	
	gchar* output_string;
	GtkWidget* parent;	
	CssedDoc* doc;
	
	parent = GTK_WIDGET(data);
	doc = document_get_current( window );	
	
	combo_entry_font_size = lookup_widget(parent,"combo_entry_font_size");	
	strsize = gtk_editable_get_chars(GTK_EDITABLE(combo_entry_font_size),0,-1);
	
	if( strcmp(CSS_STR_USE_PERCLEN,strsize) == 0 ){
		output_string = g_strdup("");
		spinbutton_font_size = lookup_widget(parent,"spinbutton_font_size");
		combo_entry_font_size_units = lookup_widget(parent,"combo_entry_font_size_units");
		strunit = gtk_editable_get_chars(GTK_EDITABLE(combo_entry_font_size_units),0,-1);
		
		if( strcmp(strunit, "em") == 0  ||
			strcmp(strunit, "in") == 0  ){
			fsize = (gfloat) gtk_spin_button_get_value_as_float(GTK_SPIN_BUTTON(spinbutton_font_size));
			output_string = g_strdup_printf("%0.2f%s",fsize,strunit);					
		}else{
			nsize = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(spinbutton_font_size));
			output_string = g_strdup_printf("%d%s",nsize,strunit);	
		}			
				
		g_free(strunit);
		g_free(strsize);
		return output_string;
	}else{
		return strsize;
	}
}
gchar*
cssdialog_frecuenzy_property_get_string( CssedWindow* window, gpointer data )
{
	GtkWidget *spinbutton_frecuency;
	GtkWidget *combo_entry_frecuency;
	gint nfreq;
	gchar* output_string;
	gchar* unit;
	GtkWidget* parent;	
	CssedDoc* doc;
	
	parent = GTK_WIDGET(data);
	doc = document_get_current( window );
	
	spinbutton_frecuency = lookup_widget(parent,"spinbutton_frecuency");
	combo_entry_frecuency = lookup_widget(parent,"combo_entry_frecuency");
	
	unit = gtk_editable_get_chars(GTK_EDITABLE(combo_entry_frecuency),0,-1);
	nfreq = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON
				(spinbutton_frecuency));
	output_string = g_strdup_printf("%d%s",nfreq,unit);
	
	g_free(unit);
	return output_string;
}

gchar*
cssdialog_identifier_value_get_string( CssedWindow* window, gpointer data )
{
	GtkWidget *entry_identifier;
	gchar* output_string;
	GtkWidget* parent;	
	CssedDoc* doc;
	
	parent = GTK_WIDGET(data);
	doc = document_get_current( window );
	
	entry_identifier = lookup_widget(parent,"entry_identifier");
	output_string = gtk_editable_get_chars(GTK_EDITABLE(entry_identifier),0,-1);
	
	if( strlen(output_string) > 0){
		return output_string;
	}else{
		g_free( output_string );
		return NULL;
	}
}

gchar*
cssdialog_integer_value_get_string( CssedWindow* window, gpointer data )
{
	GtkWidget *spinbutton_integer;
	gchar* output_string;
	gint ninteger;
	GtkWidget* parent;	
	CssedDoc* doc;
	
	parent = GTK_WIDGET(data);
	doc = document_get_current( window );	
	
	spinbutton_integer = lookup_widget(parent,"spinbutton_integer");	
	ninteger = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(spinbutton_integer));
	
	output_string = g_strdup_printf(" %d", ninteger );
	
	return output_string;
}	

gchar*
cssdialog_length_value_get_string ( CssedWindow* window, gpointer data )
{	
	GtkWidget *spinbutton_length;
	GtkWidget *combo_entry_length;
	gchar* strunit;
	gint nlength;
	gfloat flength;
	gchar* output_string;
	GtkWidget* parent;	
	CssedDoc* doc;
	
	parent = GTK_WIDGET(data);
	doc = document_get_current( window );
	
	spinbutton_length = lookup_widget(parent,"spinbutton_length");
	combo_entry_length = lookup_widget(parent,"combo_entry_length");
	strunit = gtk_editable_get_chars(GTK_EDITABLE(combo_entry_length),0,-1);
	
	if( strcmp(strunit, "em") == 0  ||
		strcmp(strunit, "in") == 0  ){
		flength = (gfloat) gtk_spin_button_get_value_as_float(GTK_SPIN_BUTTON(spinbutton_length));
		output_string = g_strdup_printf("%0.2f%s",flength,strunit);					
	}else{
		nlength = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(spinbutton_length));
		output_string = g_strdup_printf(" %d%s", nlength,strunit);
	}	
	
	g_free (strunit);
	return 	output_string;	
}

gchar*
cssdialog_list_style_get_string ( CssedWindow* window, gpointer data )
{
	GtkWidget *checkbutton_type;
	GtkWidget *combo_entry_type;
	GtkWidget *checkbutton_position;
	GtkWidget *combo_entry_position;
	GtkWidget *checkbutton_image;
	GtkWidget *entry_image;
	gboolean bpos;
	gboolean btype;
	gboolean bimage;
	gchar* strytype;
	gchar* strpos;
	gchar* strimage;
	gchar* output_string;
	GtkWidget* parent;	
	CssedDoc* doc;
	
	parent = GTK_WIDGET(data);
	doc = document_get_current( window );
	
	checkbutton_type = lookup_widget(parent,"checkbutton_type");	
	checkbutton_position = lookup_widget(parent,"checkbutton_position"); 
	checkbutton_image = lookup_widget(parent,"checkbutton_image");
	
	btype = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(checkbutton_type));
	bpos = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(checkbutton_position));
	bimage = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(checkbutton_image));
	
	if( btype || bpos || bimage ){
		output_string = g_strdup("");
		if( btype || bpos ){
			if( btype ){
				combo_entry_type = lookup_widget(parent,"combo_entry_type");
				strytype = gtk_editable_get_chars(GTK_EDITABLE(combo_entry_type),
							0,-1);
				output_string = allocated_buffer_add_string(output_string, strytype);
				g_free (strytype);
			}
			
			if( bpos ){
				combo_entry_position = lookup_widget(parent,"combo_entry_position");
				strpos = gtk_editable_get_chars(GTK_EDITABLE(combo_entry_position),
							0,-1);
				if( btype ) output_string = allocated_buffer_add_string(output_string, " ");
				output_string = allocated_buffer_add_string(output_string, strpos);
				g_free (strpos);
			}
		}
		if( bimage ){
			entry_image = lookup_widget(parent,"entry_image");
			strimage = gtk_editable_get_chars(GTK_EDITABLE(entry_image),
							0,-1);
			if( strlen(strimage) > 0) {
					output_string = allocated_buffer_add_string(output_string, " url(\"");
					output_string = allocated_buffer_add_string(output_string, strimage);
					output_string = allocated_buffer_add_string(output_string, "\")");
			}else{
				document_write_error_to_program_output( 
							doc,
							_("list style dialog: image is selected but empty, ignoring ..."),
							WARNING_COLOR_STRING);
			}
			g_free (strimage);
		}
		return output_string;
	}else{
		return NULL;
	}
}
gchar*
cssdialog_margin_property_get_string ( CssedWindow* window, gpointer data )
{
	GtkWidget *checkbutton_top;
	GtkWidget *spinbutton_top;
	GtkWidget *combo_entry_top;
	GtkWidget *checkbutton_right;
	GtkWidget *spinbutton_right;
	GtkWidget *combo_entry_right;
	GtkWidget *checkbutton_bottom;
	GtkWidget *spinbutton_bottom;
	GtkWidget *combo_entry_bottom;
	GtkWidget *checkbutton_left;
	GtkWidget *spinbutton_left;
	GtkWidget *combo_entry_left;
	gboolean b_top;
	gboolean b_right;
	gboolean b_bottom;
	gboolean b_left;
	gchar* strtop;
	gchar* strtop_unit;
	gint ntop;
	gfloat ftop;
	gchar* strright;
	gchar* strright_unit;	
	gint nright;
	gfloat fright;	
	gchar* strbottom;
	gchar* strbottom_unit = NULL;	
	gint nbottom;
	gfloat fbottom;		
	gchar* strleft;
	gchar* strleft_unit;	
	gint nleft;
	gfloat fleft;		
	gint count;
	gchar* output_string;
	GtkWidget* parent;	
	CssedDoc* doc;
	
	parent = GTK_WIDGET(data);
	doc = document_get_current( window );
	
	checkbutton_top = lookup_widget(parent,"checkbutton_top");
	checkbutton_right = lookup_widget(parent,"checkbutton_right");
	checkbutton_bottom = lookup_widget(parent,"checkbutton_bottom");
	checkbutton_left = lookup_widget(parent,"checkbutton_left");
	
	b_top = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(checkbutton_top));
	b_right = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(checkbutton_right));
	b_bottom = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(checkbutton_bottom));
	b_left = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(checkbutton_left));	
	
	if( !b_top && !b_right && !b_bottom && !b_left ){
		return NULL;
	}else{
		count = 0;
		output_string = g_strdup("");
		if( b_top ){
			count++;
			spinbutton_top = lookup_widget(parent,"spinbutton_top");
			combo_entry_top = lookup_widget(parent,"combo_entry_top");
			strtop_unit = gtk_editable_get_chars(GTK_EDITABLE(combo_entry_top),0,-1);
			
			if( strcmp(strtop_unit, "em") == 0 ||
				strcmp(strtop_unit, "in") == 0 )
			{
				ftop = (gfloat) gtk_spin_button_get_value_as_float(GTK_SPIN_BUTTON(spinbutton_top));			
				strtop = g_strdup_printf("%0.2f%s ", ftop, strtop_unit);				
			}else{
				ntop = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(spinbutton_top));			
				strtop = g_strdup_printf("%d%s ", ntop, strtop_unit);
			}
			
			output_string = allocated_buffer_add_string(output_string,  strtop);
			g_free(strtop);
			g_free(strtop_unit);			
		}

		if( b_right ){
			count++;
			spinbutton_right = lookup_widget(parent,"spinbutton_right");
			combo_entry_right = lookup_widget(parent,"combo_entry_right");
			strright_unit = gtk_editable_get_chars(GTK_EDITABLE(combo_entry_right),0,-1);	

			if( strcmp(strright_unit, "em") == 0 ||
				strcmp(strright_unit, "in") == 0 )
			{
				fright = (gfloat) gtk_spin_button_get_value_as_float(GTK_SPIN_BUTTON(spinbutton_right));			
				strright = g_strdup_printf("%0.2f%s ", fright, strright_unit);				
			}else{
				nright = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(spinbutton_right));			
				strright = g_strdup_printf("%d%s ", nright, strright_unit);
			}			
			
			output_string = allocated_buffer_add_string(output_string,  strright);
			g_free(strright);
			g_free(strright_unit);			
		}
		if( b_bottom ){
			count++;
			spinbutton_bottom = lookup_widget(parent,"spinbutton_bottom");
			combo_entry_bottom = lookup_widget(parent,"combo_entry_bottom");
			strbottom_unit = gtk_editable_get_chars(GTK_EDITABLE(combo_entry_bottom),0,-1);	

			if( strcmp(strbottom_unit, "em") == 0 ||
				strcmp(strbottom_unit, "in") == 0 )
			{
				fbottom = (gfloat) gtk_spin_button_get_value_as_float(GTK_SPIN_BUTTON(spinbutton_bottom));			
				strbottom = g_strdup_printf("%0.2f%s ",  fbottom, strbottom_unit);				
			}else{
				nbottom =  gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(spinbutton_bottom));
				strbottom = g_strdup_printf("%d%s ", nbottom, strbottom_unit);
			}	
			
			output_string = allocated_buffer_add_string(output_string,  strbottom);
			g_free(strbottom);
			g_free(strbottom_unit);			
		}
		if( b_left ){
			count++;
			spinbutton_left = lookup_widget(parent,"spinbutton_left");
			combo_entry_left = lookup_widget(parent,"combo_entry_left");
			strleft_unit = gtk_editable_get_chars(GTK_EDITABLE(combo_entry_left),0,-1);
			
			if( strcmp(strbottom_unit, "em") == 0 ||
				strcmp(strbottom_unit, "in") == 0 )
			{
				fleft = (gfloat) gtk_spin_button_get_value_as_float(GTK_SPIN_BUTTON(spinbutton_left));			
				strleft = g_strdup_printf("%0.2f%s ",  fleft, strleft_unit);				
			}else{
				nleft = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(spinbutton_left));
				strleft = g_strdup_printf("%d%s ", nleft, strleft_unit);
			}	
			
			output_string = allocated_buffer_add_string(output_string, strleft);
			g_free(strleft);
			g_free(strleft_unit);			
		}	


		if( count == 1){
			document_write_error_to_program_output(
					doc,
					_("margin: only one value used. Will be applied to all margins."),
					WARNING_COLOR_STRING
					);
		}else if( count == 2 ){
			document_write_error_to_program_output(
					doc,
					_("margin: two values used. First will be applied to top/bottom, second to left/right."),
					WARNING_COLOR_STRING
					);
		}else if(count == 3 ){
			document_write_error_to_program_output(
					doc,
					_("margin: three values used. First will be used to top, second to left/right and third to bottom."),
					WARNING_COLOR_STRING
					);
		}		
		return output_string;
	}
}

gchar*
cssdialog_margin_width_get_string ( CssedWindow* window, gpointer data )
{
	GtkWidget *spinbutton_margin_width;
	GtkWidget *combo_entry_margin_width_units;
	gchar* output_string;
	gchar* strunit;
	gint nwidth;
	gfloat fwidth;
	GtkWidget* parent;	
	CssedDoc* doc;
	
	parent = GTK_WIDGET(data);
	doc = document_get_current( window );
	
	spinbutton_margin_width = lookup_widget(parent, "spinbutton_margin_width");
	combo_entry_margin_width_units = lookup_widget(parent, "combo_entry_margin_width_units");	
	strunit = gtk_editable_get_chars(GTK_EDITABLE(combo_entry_margin_width_units),0,-1);
	
	if( strcmp(strunit, "em") == 0  ||
		strcmp(strunit, "in") == 0  ){
		fwidth = (gfloat) gtk_spin_button_get_value_as_float(GTK_SPIN_BUTTON(spinbutton_margin_width));
		output_string = g_strdup_printf("%0.2f%s", fwidth, strunit);	
	}else{
		nwidth = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(spinbutton_margin_width));
		output_string = g_strdup_printf("%d%s",nwidth,strunit);
	}
	
	g_free (strunit);	
	return output_string;
}

gchar*
cssdialog_number_value_get_string ( CssedWindow* window, gpointer data )
{
	GtkWidget *spinbutton_number;
	gfloat number;
	gchar* output_string;
	GtkWidget* parent;	
	CssedDoc* doc;
	
	parent = GTK_WIDGET(data);
	doc = document_get_current( window );
	
	spinbutton_number = lookup_widget(parent,"spinbutton_number");
	
	number = (gfloat) gtk_spin_button_get_value_as_float(GTK_SPIN_BUTTON(spinbutton_number));
	output_string = g_strdup_printf(" %0.2f", number);
	return output_string;
}

gchar*
cssdialog_padding_property_get_string ( CssedWindow* window, gpointer data )
{
	GtkWidget *checkbutton_top;
	GtkWidget *spinbutton_top;
	GtkWidget *combo_entry_top;
	GtkWidget *checkbutton_left;
	GtkWidget *spinbutton_left;
	GtkWidget *combo_entry_left;
	GtkWidget *checkbutton_bottom;
	GtkWidget *spinbutton_bottom;
	GtkWidget *combo_entry_bottom;
	GtkWidget *checkbutton_right;
	GtkWidget *spinbutton_right;
	GtkWidget *combo_entry_right;
	gboolean b_top;
	gboolean b_left;
	gboolean b_bottom;
	gboolean b_right;
	gint ntop;
	gint nleft;
	gint nbottom;
	gint nright;
	gfloat ftop;
	gfloat fleft;
	gfloat fbottom;
	gfloat fright;	
	gchar* strtop;
	gchar* strleft;
	gchar* strbottom;
	gchar* strright;
	gchar* strtop_unit;
	gchar* strleft_unit;
	gchar* strbottom_unit = NULL;
	gchar* strright_unit;	
	gint count;
	gchar* output_string;
	GtkWidget* parent;	
	CssedDoc* doc;
	
	parent = GTK_WIDGET(data);
	doc = document_get_current( window );
	
	checkbutton_top = lookup_widget(parent, "checkbutton_top");
	checkbutton_left = lookup_widget(parent, "checkbutton_left");
	checkbutton_bottom = lookup_widget(parent, "checkbutton_bottom");
	checkbutton_right = lookup_widget(parent, "checkbutton_right");

	b_top = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(checkbutton_top));
	b_left = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(checkbutton_left));
	b_bottom = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(checkbutton_bottom));
	b_right = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(checkbutton_right));
	
	if( !b_top && !b_right && !b_bottom && !b_left ){
		return NULL;
	}else{
		count = 0;
		output_string = g_strdup("");
		if( b_top ){
			count++;
			spinbutton_top  = lookup_widget(parent, "spinbutton_top");
			combo_entry_top = lookup_widget(parent, "combo_entry_top");
			strtop_unit = gtk_editable_get_chars(GTK_EDITABLE(combo_entry_top),0,-1);
			
			if( strcmp(strtop_unit, "em") == 0 ||
				strcmp(strtop_unit, "in") == 0 )
			{
				ftop = (gfloat) gtk_spin_button_get_value_as_float(GTK_SPIN_BUTTON(spinbutton_top));			
				strtop = g_strdup_printf("%0.2f%s ", ftop, strtop_unit);				
			}else{
				ntop = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(spinbutton_top));			
				strtop = g_strdup_printf("%d%s ", ntop, strtop_unit);
			}
			
			output_string = allocated_buffer_add_string(output_string,  strtop);
			g_free(strtop);
			g_free(strtop_unit);			
		}
		if( b_right ){
			count++;
			spinbutton_right = lookup_widget(parent,"spinbutton_right");
			combo_entry_right = lookup_widget(parent,"combo_entry_right");
			strright_unit = gtk_editable_get_chars(GTK_EDITABLE(combo_entry_right),0,-1);

			if( strcmp(strright_unit, "em") == 0 ||
				strcmp(strright_unit, "in") == 0 )
			{
				fright = (gfloat) gtk_spin_button_get_value_as_float(GTK_SPIN_BUTTON(spinbutton_right));			
				strright = g_strdup_printf("%0.2f%s ", fright, strright_unit);				
			}else{
				nright = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(spinbutton_right));			
				strright = g_strdup_printf("%d%s ", nright, strright_unit);
			}	
			
			output_string = allocated_buffer_add_string(output_string,  strright);
			g_free(strright);
			g_free(strright_unit);			
		}
		if( b_bottom ){
			count++;
			spinbutton_bottom = lookup_widget(parent,"spinbutton_bottom");
			combo_entry_bottom = lookup_widget(parent,"combo_entry_bottom");
			strbottom_unit = gtk_editable_get_chars(GTK_EDITABLE(combo_entry_bottom),0,-1);	

			if( strcmp(strbottom_unit, "em") == 0 ||
				strcmp(strbottom_unit, "in") == 0 )
			{
				fbottom = (gfloat) gtk_spin_button_get_value_as_float(GTK_SPIN_BUTTON(spinbutton_bottom));			
				strbottom = g_strdup_printf("%0.2f%s ",  fbottom, strbottom_unit);				
			}else{
				nbottom =  gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(spinbutton_bottom));
				strbottom = g_strdup_printf("%d%s ", nbottom, strbottom_unit);
			}
			
			output_string = allocated_buffer_add_string(output_string, strbottom);
			g_free(strbottom);
			g_free(strbottom_unit);			
		}
		if( b_left ){
			count++;
			spinbutton_left = lookup_widget(parent,"spinbutton_left");
			combo_entry_left = lookup_widget(parent,"combo_entry_left");
			strleft_unit = gtk_editable_get_chars(GTK_EDITABLE(combo_entry_left),0,-1);
			
			if( strcmp(strleft_unit, "em") == 0 ||
				strcmp(strleft_unit, "in") == 0 )
			{
				fleft = (gfloat) gtk_spin_button_get_value_as_float(GTK_SPIN_BUTTON(spinbutton_left));			
				strleft = g_strdup_printf("%0.2f%s ",  fleft, strleft_unit);				
			}else{
				nleft = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(spinbutton_left));
				strleft = g_strdup_printf("%d%s ", nleft, strleft_unit);
			}	
			
			output_string = allocated_buffer_add_string(output_string, strleft);
			g_free(strleft);
			g_free(strleft_unit);			
		}	

		if( count == 1){
			document_write_error_to_program_output(doc,_("padding: only one value used. Will be applied to all sides."),WARNING_COLOR_STRING);
		}else if( count == 2 ){
			document_write_error_to_program_output(doc,_("padding: two values used. First will be applied to top/bottom, second to left/right."),WARNING_COLOR_STRING);
		}else if(count == 3 ){
			document_write_error_to_program_output(doc,_("padding: three values used. First will be used to top, second to left/right and third to bottom."),WARNING_COLOR_STRING);
		}

		return output_string;	
	}	
}

gchar*
cssdialog_outline_property_get_string ( CssedWindow* window, gpointer data )
{
	GtkWidget *checkbutton_length;
	GtkWidget *combo_entry_length_selector;
	GtkWidget *spinbutton_length;
	GtkWidget *combo_entry_length_units;
	GtkWidget *checkbutton_style;
	GtkWidget *combo_entry_style;
	GtkWidget *checkbutton_color;
	GtkWidget *combo_entry_color;
	GtkWidget *colorselection;	
	gboolean b_length;
	gboolean b_style;
	gboolean b_color;
	gint nlength;
	gfloat flength;
	gchar* strlength_units;
	gchar* strlength;
	gchar* strlength_and_unit;	
	gchar* strstyle;	
	gchar* strcolor;
	gchar* gdk_color_to_string;
	GdkColor color; 
	gchar* output_string;
	GtkWidget* parent;	
	CssedDoc* doc;
	
	parent = GTK_WIDGET(data);
	doc = document_get_current( window );
	
	checkbutton_length  = lookup_widget(parent,"checkbutton_length");
	checkbutton_style = lookup_widget(parent,"checkbutton_style");
	checkbutton_color = lookup_widget(parent,"checkbutton_color");
	
	b_length = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(checkbutton_length)); 
	b_style = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(checkbutton_style));
	b_color = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(checkbutton_color));	
	
	if ( !b_length && !b_style && !b_color ){
		return NULL;
	}else{		
		output_string = g_strdup("");
		if( b_length ){
			combo_entry_length_selector = lookup_widget(parent,"combo_entry_length_selector") ;	
			strlength = gtk_editable_get_chars(GTK_EDITABLE(combo_entry_length_selector),
								0,-1);
			if( strcmp(strlength,CSS_STR_USE_PERCLEN) == 0){
				spinbutton_length  = lookup_widget(parent,"spinbutton_length");
				combo_entry_length_units = lookup_widget(parent,"combo_entry_length_units");
				strlength_units = gtk_editable_get_chars(GTK_EDITABLE(combo_entry_length_units),
								0,-1);
				
				if( strcmp(strlength_units, "em") == 0 ||
					strcmp(strlength_units, "in") == 0 )
				{
					flength = (gfloat) gtk_spin_button_get_value_as_float(GTK_SPIN_BUTTON(spinbutton_length));				
					strlength_and_unit = g_strdup_printf("%0.2f%s", flength, strlength_units);
				}else{
					nlength = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(spinbutton_length));				
					strlength_and_unit = g_strdup_printf("%d%s", nlength, strlength_units);
				}
				
				output_string = allocated_buffer_add_string(output_string, strlength_and_unit);
				g_free(strlength_units);
				g_free(strlength_and_unit);
			}else{
				output_string = allocated_buffer_add_string(output_string, strlength);
			}			
			g_free(strlength);
			if( b_style || b_color ){
				output_string = allocated_buffer_add_string(output_string," ");
			}			
		}
		if( b_style ){
			combo_entry_style = lookup_widget(parent,"combo_entry_style");	
			strstyle = gtk_editable_get_chars(GTK_EDITABLE(combo_entry_style),0,-1);
			output_string = allocated_buffer_add_string(output_string,strstyle);
			if( b_color ){
				output_string = allocated_buffer_add_string(output_string, " ");
			}
			g_free(strstyle);
		}		
		if( b_color ){
			combo_entry_color = lookup_widget(parent,"combo_entry_color");
			strcolor = gtk_editable_get_chars(GTK_EDITABLE(combo_entry_color),0,-1);
			if( strcmp(strcolor,CSS_STR_USE_COLORSEL) == 0 ){
				colorselection = lookup_widget(parent,"colorselection");
				gtk_color_selection_get_current_color(GTK_COLOR_SELECTION(colorselection),&color);
				gdk_color_to_string = gdk_color_to_rgb_triplet_string(&color);
				output_string = allocated_buffer_add_string(output_string, gdk_color_to_string );
				g_free(gdk_color_to_string);
			}else{
				output_string = allocated_buffer_add_string(output_string, strcolor);
			}		
			g_free(strcolor);
		}		
		return output_string;		
	}
}

gchar*
cssdialog_pause_property_get_string( CssedWindow* window, gpointer data )
{
	GtkWidget *checkbutton_after;
	GtkWidget *spinbutton_after;
	GtkWidget *combo_entry_after;
	GtkWidget *checkbutton_before;
	GtkWidget *spinbutton_before;
	GtkWidget *combo_entry_before;
	gboolean b_after;
	gboolean b_before;
	gchar* strafter;
	gchar* strafter_unit;
	gint nafter;
	gchar* strbefore;
	gchar* strbefore_unit;
	gint nbefore;	
	gchar* output_string;
	GtkWidget* parent;	
	CssedDoc* doc;
	
	parent = GTK_WIDGET(data);
	doc = document_get_current( window );
	
	checkbutton_after = lookup_widget(parent,"checkbutton_after");
	checkbutton_before = lookup_widget(parent,"checkbutton_before");
	
	b_after = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(checkbutton_after));
	b_before = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(checkbutton_before));
	
	if(!b_after && !b_before){
		return NULL;
	}else{	
		output_string = g_strdup("");
		if( b_after ){
			spinbutton_after = lookup_widget(parent,"spinbutton_after");
			combo_entry_after = lookup_widget(parent,"combo_entry_after");
			strafter_unit = gtk_editable_get_chars(GTK_EDITABLE(combo_entry_after),0,-1);
			nafter = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(spinbutton_after));
			strafter = g_strdup_printf("%d%s",nafter,strafter_unit);
			output_string = allocated_buffer_add_string(output_string,strafter);
			if( b_before ){
				output_string = allocated_buffer_add_string(output_string," ");
			}
			g_free(strafter_unit);
			g_free(strafter);
		}
		if( b_before ){
			if( !b_after ){
				output_string = allocated_buffer_add_string(output_string, " 0ms ");
				document_write_error_to_program_output(doc,_("pause dialog: only pause before selected, defaulting pause-after to 0 millisecond."),WARNING_COLOR_STRING);
			}
			spinbutton_before = lookup_widget(parent,"spinbutton_before");
			combo_entry_before = lookup_widget(parent,"combo_entry_before");			
			strbefore_unit = gtk_editable_get_chars(GTK_EDITABLE(combo_entry_before),0,-1);
			nbefore = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(spinbutton_before));
			strbefore = g_strdup_printf("%d%s",nbefore,strbefore_unit);
			output_string = allocated_buffer_add_string(output_string, strbefore);
			g_free(strbefore_unit);
			g_free(strbefore);
		}
		return output_string;
	}	
}

gchar*
cssdialog_percentage_value_get_string( CssedWindow* window, gpointer data )
{
	GtkWidget *spinbutton_percentage;
	gchar* output_string;
	gint npercentage;
	GtkWidget* parent;	
	CssedDoc* doc;
	
	parent = GTK_WIDGET(data);
	doc = document_get_current( window );
	
	spinbutton_percentage = lookup_widget(parent,"spinbutton_percentage");
	npercentage = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(spinbutton_percentage));
	output_string = g_strdup_printf(" %d",npercentage);

	return output_string;
}
gchar*
cssdialog_percentage_length_value_get_string( CssedWindow* window, gpointer data )
{
	GtkWidget *spinbutton_perclen;
	GtkWidget *combo_entry_perclen_units;
	gchar* output_string;
	gchar* strunit;
	gint npercentage;
	gfloat fpercentage;
	GtkWidget* parent;	
	CssedDoc* doc;
	
	parent = GTK_WIDGET(data);
	doc = document_get_current( window );
	
	spinbutton_perclen = lookup_widget(parent,"spinbutton_perclen");
	combo_entry_perclen_units = lookup_widget(parent,"combo_entry_perclen_units");
	strunit = gtk_editable_get_chars(GTK_EDITABLE(combo_entry_perclen_units),0,-1);	
	
	if( strcmp(strunit, "em") == 0 ||
		strcmp(strunit, "in") == 0 )
	{
		fpercentage = (gfloat) gtk_spin_button_get_value_as_float(GTK_SPIN_BUTTON(spinbutton_perclen));
		output_string = g_strdup_printf(" %0.2f%s", fpercentage, strunit);
	}else{
		npercentage = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(spinbutton_perclen));
		output_string = g_strdup_printf(" %d%s", npercentage, strunit);
	}
	
	g_free (strunit);
	return output_string;
}
gchar*
cssdialog_play_during_property_get_string ( CssedWindow* window, gpointer data )
{
	GtkWidget *entry_uri;
	GtkWidget *checkbutton_repeat;
	GtkWidget *checkbutton_mix;	
	gchar* struri;
	gboolean b_repeat;
	gboolean b_mix;
	gchar* output_string;
	GtkWidget* parent;	
	CssedDoc* doc;
	
	parent = GTK_WIDGET(data);
	doc = document_get_current( window );
	
	entry_uri = lookup_widget(parent,"entry_uri");	
	struri = gtk_editable_get_chars(GTK_EDITABLE(entry_uri),0,-1);
	
	if( strlen(struri) > 0){	
		output_string = g_strdup("");	
		checkbutton_repeat = lookup_widget(parent,"checkbutton_repeat");
		checkbutton_mix = lookup_widget(parent,"checkbutton_mix");
		b_repeat = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(checkbutton_repeat));
		b_mix = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(checkbutton_mix));

		output_string = allocated_buffer_add_string(output_string, " url(\"");
		output_string = allocated_buffer_add_string(output_string, struri);
		output_string = allocated_buffer_add_string(output_string, "\")");		
		
		if( b_repeat ) output_string = allocated_buffer_add_string(output_string, " repeat");
		if( b_mix ) output_string = allocated_buffer_add_string(output_string, " mix");
		
		g_free(struri);
		return output_string;
	}else{
		g_free(struri);
		return NULL;
	}	
}

gchar*
cssdialog_quotes_property_get_string( CssedWindow* window, gpointer data )
{
	GtkWidget *treeview;	
	GtkTreeModel *list_store;
	GtkTreeIter iter;
	gboolean valid;	
	gchar* stropen;
	gchar* strclose;
	gchar* output_string;
	GtkWidget* parent;	
	CssedDoc* doc;
	
	parent = GTK_WIDGET(data);
	doc = document_get_current( window );
	
	treeview = lookup_widget (parent, "treeview");
	list_store = gtk_tree_view_get_model (GTK_TREE_VIEW (treeview));
	valid = gtk_tree_model_get_iter_first (list_store, &iter);

	if (valid)
	{	// there are values in the store
		// parse the list
		output_string = g_strdup("");
		while (valid)
		{
			gtk_tree_model_get (list_store, &iter,
						0, &stropen, 1, &strclose, -1);

			// the callback on_button_quotes_property_dialog_add_clicked()
			// ensures that no empty values can be passed.
			if( strcmp(stropen,"\"") == 0 ){
				output_string = allocated_buffer_add_string(output_string, " '");
				output_string = allocated_buffer_add_string(output_string, stropen);
				output_string = allocated_buffer_add_string(output_string, "'");
			}else{
				output_string = allocated_buffer_add_string(output_string, " \"");
				output_string = allocated_buffer_add_string(output_string, stropen);
				output_string = allocated_buffer_add_string(output_string, "\"");					
			}		

			if( strcmp(strclose,"\"") == 0 ){
				output_string = allocated_buffer_add_string(output_string, " '");
				output_string = allocated_buffer_add_string(output_string, strclose);
				output_string = allocated_buffer_add_string(output_string, "'");
			}else{
				output_string = allocated_buffer_add_string(output_string, " \"");
				output_string = allocated_buffer_add_string(output_string, strclose);
				output_string = allocated_buffer_add_string(output_string, "\"");					
			}	
			
			g_free (stropen);		
			g_free (strclose);
			valid = gtk_tree_model_iter_next (list_store, &iter);
			if( valid ){
				output_string = allocated_buffer_add_string(output_string, " , ");
			}
		}		
		return output_string;
	}else { // not valid nothing in the list
		return NULL;
	}
}	

gchar*
cssdialog_shape_value_get_string ( CssedWindow* window, gpointer data )
{
	GtkWidget *spinbutton_top;
	GtkWidget *combo_entry_top;
	GtkWidget *spinbutton_right;
	GtkWidget *combo_entry_right;
	GtkWidget *spinbutton_bottom;
	GtkWidget *combo_entry_bottom;
	GtkWidget *spinbutton_left;
	GtkWidget *combo_entry_left;
	gchar* output_string;
	gchar* str_top_units;
	gchar* str_left_units;
	gchar* str_bottom_units;
	gchar* str_right_units;
	gint ntop;
	gint nleft;
	gint nbottom;
	gint nright;
	gfloat ftop;
	gfloat fleft;
	gfloat fbottom;
	gfloat fright;
	gchar* helper_str;	
	GtkWidget* parent;	
	CssedDoc* doc;
	
	parent = GTK_WIDGET(data);
	doc = document_get_current( window );
	
	spinbutton_top = lookup_widget(parent,"spinbutton_top");
	combo_entry_top = lookup_widget(parent,"combo_entry_top");
	spinbutton_right = lookup_widget(parent,"spinbutton_right");
	combo_entry_right = lookup_widget(parent,"combo_entry_right");
	spinbutton_bottom = lookup_widget(parent,"spinbutton_bottom");
	combo_entry_bottom = lookup_widget(parent,"combo_entry_bottom");
	spinbutton_left = lookup_widget(parent,"spinbutton_left");
	combo_entry_left = lookup_widget(parent,"combo_entry_left");
	
	str_top_units = gtk_editable_get_chars(GTK_EDITABLE(combo_entry_top),0,-1);
	str_left_units = gtk_editable_get_chars(GTK_EDITABLE(combo_entry_right),0,-1);
	str_bottom_units = gtk_editable_get_chars(GTK_EDITABLE(combo_entry_bottom),0,-1);
	str_right_units = gtk_editable_get_chars(GTK_EDITABLE(combo_entry_left),0,-1);
	
	output_string =  g_strdup("rect(  ");	
	
	if( strcmp(str_top_units, "em") == 0 ||
		strcmp(str_top_units, "in") == 0 )
	{
		ftop = (gfloat) gtk_spin_button_get_value_as_float(GTK_SPIN_BUTTON(spinbutton_top));
		helper_str= g_strdup_printf("%0.2f%s, ", ftop, str_top_units);
		output_string = allocated_buffer_add_string(output_string, helper_str);
		g_free(helper_str);
	}else{
		ntop = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(spinbutton_top));
		helper_str= g_strdup_printf("%d%s, ", ntop, str_top_units);
		output_string = allocated_buffer_add_string(output_string, helper_str);
		g_free(helper_str);
	}
	
	if( strcmp(str_left_units, "em") == 0 ||
		strcmp(str_left_units, "in") == 0 )
	{
		fleft = (gfloat) gtk_spin_button_get_value_as_float(GTK_SPIN_BUTTON(spinbutton_left));
		helper_str= g_strdup_printf("%0.2f%s, ", fleft, str_left_units);
		output_string = allocated_buffer_add_string(output_string, helper_str);
		g_free(helper_str);
	}else{
		nleft = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(spinbutton_left));
		helper_str= g_strdup_printf("%d%s, ", nleft, str_left_units);
		output_string = allocated_buffer_add_string(output_string, helper_str);
		g_free(helper_str);
	}
	
	if( strcmp(str_bottom_units, "em") == 0 ||
		strcmp(str_bottom_units, "in") == 0 )
	{
		fbottom = (gfloat) gtk_spin_button_get_value_as_float(GTK_SPIN_BUTTON(spinbutton_bottom));
		helper_str= g_strdup_printf("%0.2f%s, ", fbottom, str_bottom_units);
		output_string = allocated_buffer_add_string(output_string, helper_str);
		g_free(helper_str);
	}else{
		nbottom = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(spinbutton_bottom));
		helper_str= g_strdup_printf("%d%s, ", nbottom, str_bottom_units);
		output_string = allocated_buffer_add_string(output_string, helper_str);
		g_free(helper_str);
	}
	
	if( strcmp(str_right_units, "em") == 0 ||
		strcmp(str_right_units, "in") == 0 )
	{
		fright = (gfloat) gtk_spin_button_get_value_as_float(GTK_SPIN_BUTTON(spinbutton_right));
		helper_str= g_strdup_printf("%0.2f%s )", fright, str_right_units);
		output_string = allocated_buffer_add_string(output_string, helper_str);
		g_free(helper_str);
	}else{
		nright = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(spinbutton_right));
		helper_str= g_strdup_printf("%d%s )", nright, str_right_units);
		output_string = allocated_buffer_add_string(output_string, helper_str);
		g_free(helper_str);
	}	
			
	g_free (str_top_units);
	g_free (str_left_units);
	g_free (str_bottom_units);
	g_free (str_right_units);
	
	return output_string;
}

gchar*
cssdialog_size_length_value_get_string( CssedWindow* window, gpointer data )
{
	GtkWidget *spinbutton_width;
	GtkWidget *combo_entry_width;
	GtkWidget *spinbutton_height;
	GtkWidget *combo_entry_height;
	gchar* output_string;
	gchar* strwidth_units;
	gchar* strheight_units;	
	gfloat fwidth;
//	gfloat fheight;
	gint nwidth;
//	gint nheight;	
	gchar* helper_str;
	GtkWidget* parent;	
	CssedDoc* doc;
	
	parent = GTK_WIDGET(data);
	doc = document_get_current( window );
	
	spinbutton_width = lookup_widget(parent,"spinbutton_width");
	combo_entry_width = lookup_widget(parent,"combo_entry_width");
	spinbutton_height = lookup_widget(parent,"spinbutton_height");
	combo_entry_height = lookup_widget(parent,"combo_entry_height");

	strwidth_units = gtk_editable_get_chars(GTK_EDITABLE(combo_entry_width ),0,-1);
	strheight_units  = gtk_editable_get_chars(GTK_EDITABLE(combo_entry_height ),0,-1) ;
	
	output_string = g_strdup("");
	
	if( strcmp(strwidth_units, "em") == 0 ||
		strcmp(strwidth_units, "in") == 0 )
	{
		fwidth = (gfloat) gtk_spin_button_get_value_as_float(GTK_SPIN_BUTTON(spinbutton_width));
		helper_str= g_strdup_printf("%0.2f%s ", fwidth, strwidth_units);
		output_string = allocated_buffer_add_string(output_string, helper_str);
		g_free(helper_str);
	}else{
		nwidth = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(spinbutton_width));
		helper_str= g_strdup_printf("%d%s ", nwidth, strwidth_units);
		output_string = allocated_buffer_add_string(output_string, helper_str);
		g_free(helper_str);
	}	
	
	if( strcmp(strheight_units, "em") == 0 ||
		strcmp(strheight_units, "in") == 0 )
	{
		fwidth = (gfloat) gtk_spin_button_get_value_as_float(GTK_SPIN_BUTTON(spinbutton_width));
		helper_str= g_strdup_printf("%0.2f%s", fwidth, strheight_units);
		output_string = allocated_buffer_add_string(output_string, helper_str);
		g_free(helper_str);
	}else{
		nwidth = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(spinbutton_width));
		helper_str= g_strdup_printf("%d%s", nwidth, strheight_units);
		output_string = allocated_buffer_add_string(output_string, helper_str);
		g_free(helper_str);
	}		
	
	g_free (strwidth_units);
	g_free (strheight_units);
	
	return output_string;
}
gchar*
cssdialog_text_shadow_value_get_string ( CssedWindow* window, gpointer data )

{
	GtkTreeModel *list_store;
	GtkTreeIter iter;
	GtkWidget *spinbutton_right;
	GtkWidget *combo_entry_right;
	GtkWidget *spinbutton_below;
	GtkWidget *combo_entry_below;
	GtkWidget *checkbutton_blur;
	GtkWidget *spinbutton_blur;
	GtkWidget *combo_entry_blur;
	GtkWidget *checkbutton_use_color;
	GtkWidget *combo_entry_use_color;
	GtkWidget *colorselection;
	GtkWidget *checkbutton_use_shadow_list;
	GtkWidget *treeview;
	gboolean b_use_list;
	gchar* str_right_units;
	gchar* str_right;
	gfloat fright;	
	gint nright;
	gchar* str_below_units;
	gchar* str_below;
	gfloat fbelow;	
	gint nbelow;
	gchar* str_blur_units;
	gchar* str_blur;
	gfloat fblur;	
	gint nblur;
	gboolean b_blur;	
	gchar* strcolor;
	gchar* strrgbcolor;
	GdkColor gdkcolor;
	gboolean b_color;
	gboolean valid;
	gchar* output_string;
	GtkWidget* parent;	
	CssedDoc* doc;
	
	parent = GTK_WIDGET(data);
	doc = document_get_current( window );
	
	checkbutton_use_shadow_list = lookup_widget(parent,"checkbutton_use_shadow_list");
	b_use_list = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(checkbutton_use_shadow_list));
	
	if( b_use_list ){
		treeview = lookup_widget(parent,"treeview");
		document_write_error_to_program_output(doc,_("text shadow dialog: using list values, ignoring other dialog choices."),WARNING_COLOR_STRING);
		list_store = gtk_tree_view_get_model (GTK_TREE_VIEW (treeview));
		valid = gtk_tree_model_get_iter_first (list_store, &iter);
		output_string = g_strdup("");
		if (valid)
		{	// there are values in the store
			// parse the list
			output_string = g_strdup("");
			while (valid)
			{
				gtk_tree_model_get (list_store, &iter,
							COL_RIGHT, &str_right, 
							COL_BELOW, &str_below, 
							COL_BLUR, &str_blur, 
							COL_COLOR, &strcolor, 	-1);
				
				output_string = allocated_buffer_add_string(output_string, str_right);
				output_string = allocated_buffer_add_string(output_string, " ");
				output_string = allocated_buffer_add_string(output_string, str_below);
				
				if ( str_blur != NULL ){
					if(  strlen(str_blur) > 0 ){
						output_string = allocated_buffer_add_string(output_string, " ");
						output_string = allocated_buffer_add_string(output_string, str_blur);	
					}
					g_free (str_blur);	
				}
				
				if( strcolor != NULL ){
					if( strlen(strcolor) > 0  ){
						output_string = allocated_buffer_add_string(output_string, " ");
						output_string = allocated_buffer_add_string(output_string, strcolor);	
					}					
				}
				
				g_free (str_right);		
				g_free (str_below);
			
				valid = gtk_tree_model_iter_next (list_store, &iter);
				if( valid ){
					output_string = allocated_buffer_add_string(output_string, " , ");
				}
			}		
			return output_string;		
		}else { // nothing in the list
			return NULL;
		}		
	}else{ // don't use list
		output_string = g_strdup("");
		checkbutton_blur = lookup_widget(parent,"checkbutton_blur");	
		checkbutton_use_color = lookup_widget(parent,"checkbutton_use_color");
		
		b_blur = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(checkbutton_blur));
		b_color = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(checkbutton_use_color));
		
		spinbutton_right = lookup_widget(parent,"spinbutton_right");
		combo_entry_right = lookup_widget(parent,"combo_entry_right");
		spinbutton_below = lookup_widget(parent,"spinbutton_below");
		combo_entry_below = lookup_widget(parent,"combo_entry_below");	
		spinbutton_blur = lookup_widget(parent,"spinbutton_blur");
		combo_entry_blur = lookup_widget(parent,"combo_entry_blur");	
		combo_entry_use_color = lookup_widget(parent,"combo_entry_use_color");
		colorselection = lookup_widget(parent,"colorselection");
		
		str_right_units = gtk_editable_get_chars(GTK_EDITABLE(combo_entry_right),0,-1);		
		if( strcmp(str_right_units, "em") == 0 ||
			strcmp(str_right_units, "in") == 0 )
		{
			fright = (gfloat) gtk_spin_button_get_value_as_float(GTK_SPIN_BUTTON(spinbutton_right));
			str_right= g_strdup_printf("%0.2f%s", fright, str_right_units);
		}else{
			nright = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(spinbutton_right));
			str_right = g_strdup_printf("%d%s", nright, str_right_units);
		}	
		
		str_below_units = gtk_editable_get_chars(GTK_EDITABLE(combo_entry_below),0,-1);		
		if( strcmp(str_below_units, "em") == 0 ||
			strcmp(str_below_units, "in") == 0 )
		{
			fbelow = (gfloat) gtk_spin_button_get_value_as_float(GTK_SPIN_BUTTON(spinbutton_below));
			str_below = g_strdup_printf("%0.2f%s", fbelow, str_below_units);
		}else{
			nbelow = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(spinbutton_below));
			str_below = g_strdup_printf("%d%s", nbelow, str_below_units);
		}	
	
		output_string = allocated_buffer_add_string(output_string, str_right);
		output_string = allocated_buffer_add_string(output_string, " ");
		output_string = allocated_buffer_add_string(output_string, str_below);
				
		if( b_blur ){
			spinbutton_blur = lookup_widget(parent,"spinbutton_blur");
			combo_entry_blur = lookup_widget(parent,"combo_entry_blur");
			
			str_blur_units = gtk_editable_get_chars(GTK_EDITABLE(combo_entry_blur),0,-1);			
			if( strcmp(str_blur_units, "em") == 0 ||
				strcmp(str_blur_units, "in") == 0 )
			{
				fblur = (gfloat) gtk_spin_button_get_value_as_float(GTK_SPIN_BUTTON(spinbutton_blur));
				str_blur = g_strdup_printf("%0.2f%s", fblur, str_blur_units);
			}else{
				nblur = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(spinbutton_blur));
				str_blur = g_strdup_printf("%d%s", nblur, str_blur_units);
			}	
		
			output_string = allocated_buffer_add_string(output_string, " ");
			output_string = allocated_buffer_add_string(output_string, str_blur);
			
			g_free( str_blur_units );
			g_free( str_blur );		
		}
		
		if( b_color ){
			combo_entry_use_color = lookup_widget(parent,"combo_entry_use_color");
			colorselection = lookup_widget(parent,"colorselection");	
			strcolor = gtk_editable_get_chars(GTK_EDITABLE(combo_entry_use_color),0,-1);
			if( strcmp(strcolor,CSS_STR_USE_COLORSEL) == 0){
				gtk_color_selection_get_current_color(GTK_COLOR_SELECTION(colorselection),
					&gdkcolor);
				strrgbcolor = gdk_color_to_rgb_triplet_string(&gdkcolor);
			}else{
				strrgbcolor = g_strdup(strcolor);
			}
			output_string = allocated_buffer_add_string(output_string, " ");
			output_string = allocated_buffer_add_string(output_string, strrgbcolor);
			g_free(strcolor);
			g_free( strrgbcolor );
		}

		g_free( str_right_units );
		g_free( str_below_units );
		g_free( str_below );
		g_free( str_right );
		
		return output_string;
	}	
}

gchar*
cssdialog_voice_family_value_get_string ( CssedWindow* window, gpointer data )
{
	GtkWidget *combo_entry_generic;
	GtkWidget *entry_specific_voice;
	GtkWidget *checkbutton_voice_set;
	GtkWidget *treeview;
	GtkTreeModel *list_store;
	GtkTreeIter iter;
	gboolean valid;
	gboolean b_set;
	gchar* str_generic;
	gchar* str_specific;
	gchar* output_string;
	GtkWidget* parent;	
	CssedDoc* doc;
	
	parent = GTK_WIDGET(data);
	doc = document_get_current( window );
	
	checkbutton_voice_set = lookup_widget(parent,"checkbutton_voice_set");	
	b_set = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(checkbutton_voice_set));
	
	if( b_set ){
		treeview = lookup_widget(parent,"treeview");
		list_store = gtk_tree_view_get_model (GTK_TREE_VIEW (treeview));
		valid = gtk_tree_model_get_iter_first (list_store, &iter);
	
		if (valid)
		{	
			output_string = g_strdup("");
			while (valid)
			{
				gtk_tree_model_get (list_store, &iter,
							COL_GENERIC, &str_generic, 
							COL_SPECIFIC, &str_specific, 	-1);				
				
				if ( str_specific != NULL ){
					if( strlen(str_specific) > 0  ){
						output_string = allocated_buffer_add_string(output_string, " \"");
						output_string = allocated_buffer_add_string(output_string, str_specific);	
						output_string = allocated_buffer_add_string(output_string, "\"");
						if( (strlen(str_generic) > 0 ) && (str_generic != NULL) ){
							output_string = allocated_buffer_add_string(output_string, ",");
						}	
					}						
				}
				
				if( str_generic != NULL ){
					if( strlen(str_generic) > 0  ){
						output_string = allocated_buffer_add_string(output_string, " ");
						output_string = allocated_buffer_add_string(output_string, str_generic);	
					}					
				}
				
				g_free (str_generic);		
				g_free (str_specific);
			
				valid = gtk_tree_model_iter_next (list_store, &iter);
				if( valid ){
					output_string = allocated_buffer_add_string(output_string,  ",");
				}
			}		
		
			return output_string;
		}else { 
			return NULL;
		}	
	}else{
		output_string = g_strdup("");
		
		combo_entry_generic = lookup_widget(parent,"combo_entry_generic");
		entry_specific_voice = lookup_widget(parent,"entry_specific_voice");
		
		str_generic = gtk_editable_get_chars(GTK_EDITABLE(combo_entry_generic),0,-1);
		str_specific = gtk_editable_get_chars(GTK_EDITABLE(entry_specific_voice),0,-1);
		
			
		if( (  strlen(str_generic) > 0 ) || ( strlen(str_specific) > 0  ) ){
			if( strlen(str_specific) > 0  ){
				output_string = allocated_buffer_add_string(output_string,  " \"");
				output_string = allocated_buffer_add_string(output_string,  str_specific);	
				output_string = allocated_buffer_add_string(output_string,  "\"");
				
				if( (strlen(str_generic) > 0 ) && (str_generic != NULL) ){
					output_string = allocated_buffer_add_string(output_string,  ",");
				}
			}
			if(  strlen(str_generic) > 0 ){
				output_string = allocated_buffer_add_string(output_string,  " ");
				output_string = allocated_buffer_add_string(output_string,  str_generic);	
			}							
		}else{
			g_free (str_generic);		
			g_free (str_specific);
			return NULL;
		}		

		g_free (str_generic);		
		g_free (str_specific);
		
		return output_string;
	}	
}
gchar*
cssdialog_border_width_style_color_value_get_string ( CssedWindow* window, gpointer data )
{
	GtkWidget *checkbutton_length;
	GtkWidget *combo_entry_length;
	GtkWidget *spinbutton_length;
	GtkWidget *combo_entry_length_units;
	GtkWidget *checkbutton_color;
	GtkWidget *combo_entry_color;
	GtkWidget *colorselection;
	GtkWidget *checkbutton_style;
	GtkWidget *combo_entry_style;
	gboolean b_length;
	gboolean b_style;
	gboolean b_color;
	gchar* str_length_units;
	gchar* str_length;
	gchar* str_legth_and_units;
	gint nlength;
	gfloat flength;
	gchar* strstyle;
	gchar* str_color;
	gchar* str_rgb_color;
	GdkColor gdkcolor;
	gchar* output_string;
	GtkWidget* parent;	
	CssedDoc* doc;
	
	parent = GTK_WIDGET(data);
	doc = document_get_current( window );
	
	checkbutton_length = lookup_widget(parent,"checkbutton_length");
	checkbutton_color = lookup_widget(parent,"checkbutton_color");
	checkbutton_style = lookup_widget(parent,"checkbutton_style");
	
	b_length = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(checkbutton_length));
	b_style = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(checkbutton_style));
	b_color =  gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(checkbutton_color));
	
	if( !b_length && !b_style && !b_color ){
		return NULL;
	}else{	
		output_string = g_strdup("");
		if( b_length ){			
			combo_entry_length = lookup_widget(parent,"combo_entry_length");

			str_length	= gtk_editable_get_chars(GTK_EDITABLE(combo_entry_length),0,-1);
			if( strcmp(str_length,CSS_STR_USE_LENGTH) == 0 ){
				spinbutton_length = lookup_widget(parent,"spinbutton_length");
				combo_entry_length_units = lookup_widget(parent,"combo_entry_length_units");
				
				str_length_units = gtk_editable_get_chars(GTK_EDITABLE(combo_entry_length_units),0,-1);
				
				if( strcmp(str_length_units, "em") == 0 ||
					strcmp(str_length_units, "in") == 0 )
				{
					flength = (gfloat) gtk_spin_button_get_value_as_float(GTK_SPIN_BUTTON(spinbutton_length));
					str_legth_and_units = g_strdup_printf("%0.2f%s", flength, str_length_units);
				}else{
					nlength = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(spinbutton_length));
					str_legth_and_units = g_strdup_printf("%d%s", nlength, str_length_units);
				}	
				
				output_string = allocated_buffer_add_string(output_string,  str_legth_and_units);
				
				if( b_style || b_color ){
					output_string = allocated_buffer_add_string(output_string,  " ");
				}				
				
				g_free(str_length_units);
				g_free(str_legth_and_units);		
				
			}else{
				output_string = allocated_buffer_add_string(output_string,  str_length);
				if( b_style || b_color ){
					output_string = allocated_buffer_add_string(output_string,  " ");
				}
			}
			g_free( str_length );
		}
		
		if( b_style ){
			combo_entry_style = lookup_widget(parent,"combo_entry_style");	
			strstyle = gtk_editable_get_chars(GTK_EDITABLE(combo_entry_style),0,-1);	
			output_string = allocated_buffer_add_string(output_string,  strstyle);
			if(  b_color ){
				output_string = allocated_buffer_add_string(output_string,  " ");
			}	
			g_free(strstyle);		
		}
		
		if( b_color ){
			combo_entry_color = lookup_widget(parent,"combo_entry_color");	
			str_color= gtk_editable_get_chars(GTK_EDITABLE(combo_entry_color),0,-1);
			
			if( strcmp(str_color,CSS_STR_USE_COLORSEL) == 0 ){
				colorselection = lookup_widget(parent,"colorselection");
				gtk_color_selection_get_current_color(GTK_COLOR_SELECTION(colorselection),&gdkcolor);				
				str_rgb_color = gdk_color_to_rgb_triplet_string(&gdkcolor);
				output_string = allocated_buffer_add_string(output_string,  str_rgb_color);
				g_free(str_rgb_color);
			}else{
				output_string = allocated_buffer_add_string(output_string,  str_color);
			}
			g_free(	str_color );
		}

		return output_string;
	}
}
gchar*
cssdialog_color_selector_get_string ( CssedWindow* window, gpointer data )
{
	GtkWidget *radiobutton_named;
	GtkWidget *combo_entry_named;
	GtkWidget *colorselection;
	gboolean b_use_named_color;
	GdkColor gdkcolor;
	gchar* output_string;
	GtkWidget* parent;	
	CssedDoc* doc;
	
	parent = GTK_WIDGET(data);
	doc = document_get_current( window );
	
	radiobutton_named = lookup_widget(parent,"radiobutton_named");
	combo_entry_named = lookup_widget(parent,"combo_entry_named");
	colorselection = lookup_widget(parent,"colorselection");
	
	b_use_named_color = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(radiobutton_named));
	
	if( b_use_named_color ){
		output_string = gtk_editable_get_chars(GTK_EDITABLE(combo_entry_named),0,-1);
	}else{
		gtk_color_selection_get_current_color(GTK_COLOR_SELECTION(colorselection),
			&gdkcolor );		
		output_string = gdk_color_to_rgb_triplet_string(&gdkcolor);
	}
	return output_string;
}

gchar*
cssdialog_border_style_get_string( CssedWindow* window, gpointer data )
{
	GtkWidget *combo_entry_style;
	gchar* output_string;
	GtkWidget* parent;	
	CssedDoc* doc;
	
	parent = GTK_WIDGET(data);
	doc = document_get_current( window );
	
	combo_entry_style = lookup_widget(parent,"combo_entry_style");	
	output_string = gtk_editable_get_chars(GTK_EDITABLE(combo_entry_style),0,-1);
	return 	output_string;
}

