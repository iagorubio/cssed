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

#include <gtk/gtk.h>
#include <string.h>

#include "cssdialogs-strings.h"
#include "cssedwindow.h"
#include "document.h"
#include "support.h"
#include "utils.h"
#include "cssdialogs-functions.h"

// generic it changs the state og checkbuttons when an related widget is changed
void
on_widget_change_set_toggle_active (GtkWidget*widget,  gpointer user_data)
{
	gtk_toggle_button_set_active    (GTK_TOGGLE_BUTTON(user_data), TRUE);
}

void
on_widget_change_unset_toggle_active (GtkWidget*widget,  gpointer user_data)
{
	gtk_toggle_button_set_active    (GTK_TOGGLE_BUTTON(user_data), FALSE);
}  
                                                                                                           
void
on_angle_property_dialog_ok_button_clicked (GtkButton * button, gpointer user_data)
{
	CssedCssDialogsData *data;
	CssedWindow *window;
	CssedDoc* tmpdoc;
	GtkWidget *angle_property_dialog;
	gboolean add_previous;
	//gchar *previous_string;
	gchar *output_string;

	data = (CssedCssDialogsData *) user_data;
	window = CSSED_WINDOW (data->main_window);
	tmpdoc = document_get_current( window );
	add_previous = data->add_property_string;

	angle_property_dialog =
		lookup_widget (GTK_WIDGET (button), "angle_property_dialog");
	output_string =	cssdialog_angle_property_get_string ( window, (gpointer) button );
	if (output_string != NULL)
	{
		//document_new_undo_action(tmpdoc);
		if (add_previous)
		{
			document_add_text (tmpdoc,
					   (gchar *) data->previous_string);
			document_add_text (tmpdoc, ": ");
		}
		document_add_text (tmpdoc, output_string);
		if (add_previous)
		{
			document_add_text (tmpdoc, ";\n");
			if( document_last_line_is_indented(tmpdoc) )
				document_indent_as_last_line(tmpdoc);
			else
				document_add_text (tmpdoc, "\t");
		}
		//document_new_undo_action(tmpdoc);
		g_free (output_string);
	}

	g_free (data->previous_string);
	g_free (user_data);
	gtk_widget_destroy (angle_property_dialog);
}

void on_background_property_dialog_okbutton_clicked
	(GtkButton * button, gpointer user_data)
{
	CssedCssDialogsData *data;
	CssedWindow *window;
	CssedDoc* tmpdoc;	
	gboolean add_previous;
	GtkWidget *background_property_dialog;
	gchar *output_string;

	data = (CssedCssDialogsData *) user_data;
	window = CSSED_WINDOW (data->main_window);
	tmpdoc = document_get_current( window );
	add_previous = data->add_property_string;

	background_property_dialog =
		lookup_widget (GTK_WIDGET (button), "background_property_dialog");

	output_string =
		cssdialog_background_property_get_string ( window, (gpointer) button );
	if (output_string != NULL)
	{
		//document_new_undo_action(tmpdoc);
		if (add_previous)
		{
			document_add_text (tmpdoc, (gchar *) data->previous_string);
			document_add_text (tmpdoc, ": ");
		}
		document_add_text (tmpdoc, output_string);    
		if (add_previous)
		{
			document_add_text (tmpdoc, ";\n");
			if( document_last_line_is_indented(tmpdoc) )
				document_indent_as_last_line(tmpdoc);
			else
				document_add_text (tmpdoc, "\t");
		}
		//document_new_undo_action(tmpdoc);
		g_free (output_string);
	}

	g_free (data->previous_string);
	g_free (user_data);
	gtk_widget_destroy (background_property_dialog);
}

void
on_border_properties_okbutton_clicked (GtkButton * button, gpointer user_data)
{
	CssedCssDialogsData *data;
	CssedWindow *window;
	CssedDoc* tmpdoc;	
	gboolean add_previous;
	GtkWidget *border_properties_dialog;
	gchar *output_string;

	data = (CssedCssDialogsData *) user_data;
	window = CSSED_WINDOW (data->main_window);
	tmpdoc = document_get_current( window );
	add_previous = data->add_property_string;

	border_properties_dialog =
		lookup_widget (GTK_WIDGET (button), "border_properties_dialog");


	output_string =
		cssdialog_border_properties_get_string  ( window, (gpointer) button );
	if (output_string != NULL)
	{
		//document_new_undo_action(tmpdoc);
		if (add_previous)
		{
			document_add_text (tmpdoc,
					   (gchar *) data->previous_string);
			document_add_text (tmpdoc, ": ");
		}

		document_add_text (tmpdoc, output_string);

		if (add_previous)
		{
			document_add_text (tmpdoc, ";\n");
			if( document_last_line_is_indented(tmpdoc) )
				document_indent_as_last_line(tmpdoc);
			else
				document_add_text (tmpdoc, "\t");
		}
		//document_new_undo_action(tmpdoc);
		g_free (output_string);
	}

	g_free (data->previous_string);
	g_free (user_data);
	gtk_widget_destroy (border_properties_dialog);
}

void on_border_width_properties_dialog_okbutton_clicked
	(GtkButton * button, gpointer user_data)
{
	CssedCssDialogsData *data;
	CssedWindow *window;
	CssedDoc* tmpdoc;	
	gboolean add_previous;
	GtkWidget *border_width_properties_dialog;
	gchar *output_string;

	data = (CssedCssDialogsData *) user_data;
	window = CSSED_WINDOW (data->main_window);
	tmpdoc = document_get_current( window );
	add_previous = data->add_property_string;

	border_width_properties_dialog =
		lookup_widget (GTK_WIDGET (button),
			       "border_width_properties_dialog");

	output_string =
		cssdialog_border_width_properties_get_string  ( window, (gpointer) button );
	if (output_string != NULL)
	{
		//document_new_undo_action(tmpdoc);
		if (add_previous)
		{
			document_add_text (tmpdoc,
					   (gchar *) data->previous_string);
			document_add_text (tmpdoc, ": ");
		}

		document_add_text (tmpdoc, output_string);

		if (add_previous)
		{
			document_add_text (tmpdoc, ";\n");
			if( document_last_line_is_indented(tmpdoc) )
				document_indent_as_last_line(tmpdoc);
			else
				document_add_text (tmpdoc, "\t");
		}
		//document_new_undo_action(tmpdoc);
		g_free (output_string);
	}
	g_free (data->previous_string);
	g_free (user_data);
	gtk_widget_destroy (border_width_properties_dialog);
}

void on_border_width_single_okbutton_clicked
	(GtkButton * button, gpointer user_data)
{
	CssedCssDialogsData *data;
	CssedWindow *window;
	CssedDoc* tmpdoc;	
	gboolean add_previous;
	GtkWidget *border_width_single_properties_dialog;
	gchar *output_string;

	data = (CssedCssDialogsData *) user_data;
	window = CSSED_WINDOW (data->main_window);
	tmpdoc = document_get_current( window );
	add_previous = data->add_property_string;

	border_width_single_properties_dialog =
		lookup_widget (GTK_WIDGET (button),
			       "border_width_single_properties_dialog");

	output_string =
		cssdialog_border_width_properties_get_string  ( window, (gpointer) button );
	if (output_string != NULL)
	{
		//document_new_undo_action(tmpdoc);
		if (add_previous)
		{
			document_add_text (tmpdoc,
					   (gchar *) data->previous_string);
			document_add_text (tmpdoc, ": ");
		}

		document_add_text (tmpdoc, output_string);

		if (add_previous)
		{
			document_add_text (tmpdoc, ";\n");
			if( document_last_line_is_indented(tmpdoc) )
				document_indent_as_last_line(tmpdoc);
			else
				document_add_text (tmpdoc, "\t");
		}
		//document_new_undo_action(tmpdoc);
		g_free (output_string);
	}

	g_free (data->previous_string);
	g_free (user_data);
	gtk_widget_destroy (border_width_single_properties_dialog);
}

/* This add counters, and needs three callbacks     
	 as various counters can be added to the list.                             
* ************************************************************************** */
void on_button_counter_increment_reset_add_clicked
	(GtkButton * button, gpointer user_data)
{
	GtkListStore *store;
	GtkWidget *spinbutton;
	GtkWidget *entry;
	GtkTreeIter iter;
	gchar *strident;
	gint increment;

	spinbutton =
		lookup_widget (GTK_WIDGET (button),
			       "spinbutton_counter_increment_reset_step");
	entry = lookup_widget (GTK_WIDGET (button),
			       "entry_counter_increment_reset_identifyer");

	strident = gtk_editable_get_chars (GTK_EDITABLE (entry), 0, -1);
	increment =
		gtk_spin_button_get_value_as_int (GTK_SPIN_BUTTON
						  (spinbutton));

	store = (GtkListStore *) user_data;
	gtk_list_store_append (store, &iter);
	gtk_list_store_set (store, &iter, 0, strident, 1, increment, -1);

	g_free (strident);
}


void on_button_counter_increment_reset_del_clicked
	(GtkButton * button, gpointer user_data)
{
	GtkListStore *store;
	GtkWidget *treeview;
	GtkTreeSelection *selection;
	gboolean isselected;
	GtkTreeIter iter;

	store = (GtkListStore *) user_data;
	treeview = lookup_widget (GTK_WIDGET (button), "treeview");
	selection = gtk_tree_view_get_selection (GTK_TREE_VIEW (treeview));
	isselected = gtk_tree_selection_get_selected (selection, NULL, &iter);

	if (isselected)
	{
		gtk_list_store_remove (store, &iter);
	}
}

void on_dialog_counter_increment_reset_okbutton_clicked
	(GtkButton * button, gpointer user_data)
{
	CssedCssDialogsData *data;
	CssedWindow *window;
	CssedDoc* tmpdoc;	
	gboolean add_previous;
	GtkWidget *counter_increment_reset_dialog;
	gchar *output_string;

	data = (CssedCssDialogsData *) user_data;
	window = CSSED_WINDOW (data->main_window);
	tmpdoc = document_get_current( window );
	add_previous = data->add_property_string;


	counter_increment_reset_dialog =
		lookup_widget (GTK_WIDGET (button),
			       "counter_increment_reset_dialog");
	output_string =
		cssdialog_counter_increment_reset_get_string  ( window, (gpointer) button );
	if (output_string != NULL)
	{
		//document_new_undo_action(tmpdoc);
		if (add_previous)
		{
			document_add_text (tmpdoc,
					   (gchar *) data->previous_string);
			document_add_text (tmpdoc, ": ");
		}

		document_add_text (tmpdoc, output_string);

		if (add_previous)
		{
			document_add_text (tmpdoc, ";\n");
			if( document_last_line_is_indented(tmpdoc) )
				document_indent_as_last_line(tmpdoc);
			else
				document_add_text (tmpdoc, "\t");
		}
		//document_new_undo_action(tmpdoc);
		g_free (output_string);
	}
	g_free (data->previous_string);
	g_free (user_data);
	gtk_widget_destroy (counter_increment_reset_dialog);
}

void on_string_properties_dialog_okbutton_clicked
	(GtkButton * button, gpointer user_data)
{
	CssedCssDialogsData *data;
	CssedWindow *window;
	CssedDoc* tmpdoc;	
	gboolean add_previous;
	GtkWidget *string_properties_dialog;
	gchar *output_string;


	data = (CssedCssDialogsData *) user_data;
	window = CSSED_WINDOW (data->main_window);
	tmpdoc = document_get_current( window );
	add_previous = data->add_property_string;

	string_properties_dialog =
		lookup_widget (GTK_WIDGET (button),
			       "string_properties_dialog");

	output_string =
		cssdialog_string_properties_get_string  ( window, (gpointer) button );
	if (output_string != NULL)
	{
		//document_new_undo_action(tmpdoc);
		if (add_previous)
		{
			document_add_text (tmpdoc,
					   (gchar *) data->previous_string);
			document_add_text (tmpdoc, ": ");
		}
		document_add_text (tmpdoc, output_string);
		if (add_previous)
		{
			document_add_text (tmpdoc, ";\n");
			if( document_last_line_is_indented(tmpdoc) )
				document_indent_as_last_line(tmpdoc);
			else
				document_add_text (tmpdoc, "\t");
		}
		//document_new_undo_action(tmpdoc);
		g_free (output_string);
		g_free (data->previous_string);
		g_free (user_data);
		gtk_widget_destroy (string_properties_dialog);
	}
	else
	{
		document_write_error_to_program_output (tmpdoc,
							_
							("string dialog: refusing to add empty values."),
							ERROR_COLOR_STRING);
	}
}

void
on_uri_value_dialog_okbutton_clicked (GtkButton * button, gpointer user_data)
{
	CssedCssDialogsData *data;
	CssedWindow *window;
	CssedDoc* tmpdoc;	
	gboolean add_previous;
	GtkWidget *uri_value_dialog;
	gchar *output_string;

	data = (CssedCssDialogsData *) user_data;
	window = CSSED_WINDOW (data->main_window);
	tmpdoc = document_get_current( window );
	add_previous = data->add_property_string;

	uri_value_dialog =
		lookup_widget (GTK_WIDGET (button), "uri_value_dialog");

	output_string = cssdialog_uri_value_get_string ( window, (gpointer) button );
	if (output_string != NULL)
	{
		//document_new_undo_action(tmpdoc);
		if (add_previous)
		{
			document_add_text (tmpdoc,
					   (gchar *) data->previous_string);
			document_add_text (tmpdoc, ": ");
		}

		document_add_text (tmpdoc, output_string);

		if (add_previous)
		{
			document_add_text (tmpdoc, ";\n");
			if( document_last_line_is_indented(tmpdoc) )
				document_indent_as_last_line(tmpdoc);
			else
				document_add_text (tmpdoc, "\t");
		}
		//document_new_undo_action(tmpdoc);
		g_free (output_string);
		g_free (data->previous_string);
		g_free (user_data);
		gtk_widget_destroy (uri_value_dialog);
	}
	else
	{
		document_write_error_to_program_output (tmpdoc,
							_("uri dialog: rejecting to add empty values"),
							ERROR_COLOR_STRING);
	}
}

void
on_attr_value_dialog_okbutton_clicked (GtkButton * button, gpointer user_data)
{
	CssedCssDialogsData *data;
	CssedWindow *window;
	CssedDoc *tmpdoc;
	gboolean add_previous;
	gchar *output_string;
	GtkWidget *attr_value_dialog;


	data = (CssedCssDialogsData *) user_data;
	window = CSSED_WINDOW (data->main_window);
	tmpdoc = document_get_current (window);
	add_previous = data->add_property_string;

	attr_value_dialog =
		lookup_widget (GTK_WIDGET (button), "attr_value_dialog");

	output_string = cssdialog_attr_value_get_string ( window, (gpointer) button );
	if (output_string != NULL)
	{
		//document_new_undo_action(tmpdoc);
		if (add_previous)
		{
			document_add_text (tmpdoc,
					   (gchar *) data->previous_string);
			document_add_text (tmpdoc, ": ");
		}
		document_add_text (tmpdoc, output_string);
		if (add_previous)
		{
			document_add_text (tmpdoc, ";\n");
			if( document_last_line_is_indented(tmpdoc) )
				document_indent_as_last_line(tmpdoc);
			else
				document_add_text (tmpdoc, "\t");
		}
		//document_new_undo_action(tmpdoc);
		g_free (output_string);
		g_free (data->previous_string);
		g_free (user_data);
		gtk_widget_destroy (attr_value_dialog);
	}
	else
	{
		document_write_error_to_program_output (tmpdoc,
							_
							("attribute dialog: rejecting to add empty values"),
							ERROR_COLOR_STRING);
	}
}

void on_counter_property_dialog_okbutton_clicked
	(GtkButton * button, gpointer user_data)
{
	CssedCssDialogsData *data;
	CssedWindow *window;
	CssedDoc *tmpdoc;
	gboolean add_previous;
	GtkWidget *counter_property_dialog;
	gchar *output_string;

	data = (CssedCssDialogsData *) user_data;
	window = CSSED_WINDOW (data->main_window);
	tmpdoc = document_get_current (window);
	add_previous = data->add_property_string;

	counter_property_dialog =
		lookup_widget (GTK_WIDGET (button),
			       "counter_property_dialog");

	output_string =
		cssdialog_counter_property_get_string ( window, (gpointer) button );

	if (output_string != NULL)
	{
		//document_new_undo_action(tmpdoc);
		if (add_previous)
		{
			document_add_text (tmpdoc,
					   (gchar *) data->previous_string);
			document_add_text (tmpdoc, ": ");
		}
		document_add_text (tmpdoc, output_string);
		if (add_previous)
		{
			document_add_text (tmpdoc, ";\n");
			if( document_last_line_is_indented(tmpdoc) )
				document_indent_as_last_line(tmpdoc);
			else
				document_add_text (tmpdoc, "\t");
		}
		//document_new_undo_action(tmpdoc);
		g_free (data->previous_string);
		g_free (user_data);
		gtk_widget_destroy (counter_property_dialog);
	}
	else
	{
		document_write_error_to_program_output (tmpdoc,
							_
							("counter dialog: rejecting to add empty identifier. Value required."),
							ERROR_COLOR_STRING);
	}
}

/* This needs three callbacks                                                
 * ************************************************************************* */
void on_font_property_dialog_okbutton_clicked
	(GtkButton * button, gpointer user_data)
{
	CssedCssDialogsData *data;
	CssedWindow *window;
	CssedDoc *tmpdoc;
	gboolean add_previous;
	GtkWidget *font_property_dialog;
	gchar *output_string;


	data = (CssedCssDialogsData *) user_data;
	window = CSSED_WINDOW (data->main_window);
	tmpdoc = document_get_current (window);
	add_previous = data->add_property_string;

	font_property_dialog =
		lookup_widget (GTK_WIDGET (button), "font_property_dialog");

	output_string =
		cssdialog_font_property_get_string ( window, (gpointer) button );
	if (output_string != NULL)
	{
		if (add_previous)
		{
			document_add_text (tmpdoc,
					   (gchar *) data->previous_string);
			document_add_text (tmpdoc, ": ");
		}
		document_add_text (tmpdoc, output_string);
		if (add_previous)
		{
			document_add_text (tmpdoc, ";\n");
			if( document_last_line_is_indented(tmpdoc) )
				document_indent_as_last_line(tmpdoc);
			else
				document_add_text (tmpdoc, "\t");
		}
		g_free (output_string);
	}
	else
	{
		document_write_error_to_program_output (tmpdoc,
							_
							("font dialog: rejecting to add empty values."),
							ERROR_COLOR_STRING);
	}

	g_free (data->previous_string);
	g_free (user_data);
	gtk_widget_destroy (font_property_dialog);
}

void
on_button_font_set_add_clicked (GtkButton * button, gpointer user_data)
{
	GtkWidget *combo_entry_font_family;
	GtkWidget *combo_entry_generic_family;
	GtkListStore *store;
	GtkWidget *checkbuton_fontset;
	GtkWidget  *checkbutton_font_family;
	GtkWidget  *checkbutton_generic_family;
	GtkTreeIter iter;
	gchar *strgf;
	gchar *strff;

	combo_entry_font_family = lookup_widget (GTK_WIDGET (button),
							"combo_entry_font_family");
	combo_entry_generic_family = lookup_widget (GTK_WIDGET (button),
						    "combo_entry_generic_family");
	checkbuton_fontset = lookup_widget (GTK_WIDGET (button),
						    "checkbutton_use_font_set");
	checkbutton_font_family = lookup_widget (GTK_WIDGET (button),
								"checkbutton_font_family");
	checkbutton_generic_family = lookup_widget (GTK_WIDGET (button),
								"checkbutton_generic_family");
								
	strgf = gtk_editable_get_chars (GTK_EDITABLE
					(combo_entry_font_family), 0, -1);
	strff = gtk_editable_get_chars (GTK_EDITABLE
					(combo_entry_generic_family), 0, -1);

	store = (GtkListStore *) user_data;
	gtk_list_store_append (store, &iter);
	gtk_list_store_set (store, &iter, 0, strgf, 1, strff, -1);

	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(checkbuton_fontset), TRUE);
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(checkbutton_font_family), FALSE);
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(checkbutton_generic_family), FALSE);
	
	g_free (strgf);
	g_free (strff);
}


void
on_button_font_set_del_clicked (GtkButton * button, gpointer user_data)
{
	GtkListStore *store;
	GtkWidget *treeview;
	GtkTreeSelection *selection;
	gboolean isselected;
	GtkTreeIter iter;

	store = (GtkListStore *) user_data;
	treeview = lookup_widget (GTK_WIDGET (button), "treeview");
	selection = gtk_tree_view_get_selection (GTK_TREE_VIEW (treeview));
	isselected = gtk_tree_selection_get_selected (selection, NULL, &iter);

	if (isselected)
	{
		gtk_list_store_remove (store, &iter);
	}
}

/* ************************************************************************* */
void on_font_family_dialog_okbutton_clicked
	(GtkButton * button, gpointer user_data)
{
	CssedCssDialogsData *data;
	CssedWindow *window;
	CssedDoc *tmpdoc;
	gboolean add_previous;
	GtkWidget *font_family_dialog;
	gchar *output_string;

	data = (CssedCssDialogsData *) user_data;
	window = CSSED_WINDOW (data->main_window);
	tmpdoc = document_get_current (window);
	add_previous = data->add_property_string;

	font_family_dialog =
		lookup_widget (GTK_WIDGET (button), "font_family_dialog");
	output_string =
		cssdialog_font_family_get_string ( window, (gpointer) button );

	if (output_string != NULL)
	{
		//document_new_undo_action(tmpdoc);
		if (add_previous)
		{
			document_add_text (tmpdoc,
					   (gchar *) data->previous_string);
			document_add_text (tmpdoc, ": ");
		}
		document_add_text (tmpdoc, output_string);
		if (add_previous)
		{
			document_add_text (tmpdoc, ";\n");
			if( document_last_line_is_indented(tmpdoc) )
				document_indent_as_last_line(tmpdoc);
			else
				document_add_text (tmpdoc, "\t");
		}
		//document_new_undo_action(tmpdoc);
		g_free (output_string);
		g_free (data->previous_string);
		g_free (user_data);
		gtk_widget_destroy (font_family_dialog);
	}
	else
	{
		document_write_error_to_program_output (tmpdoc,
							_
							("font family dialog: rejecting to add empty values."),
							ERROR_COLOR_STRING);
	}
}

void on_button_font_family_dialog_add_clicked
	(GtkButton * button, gpointer user_data)
{
	GtkWidget *combo_entry_font_family;
	GtkWidget *combo_entry_generic_family;	
	GtkWidget *checkbutton_font_family;
	GtkWidget *checkbutton_generic_family;
	GtkWidget *checkbutton_font_set;
	GtkListStore *store;
	GtkTreeIter iter;
	gchar *strgf;
	gchar *strff;

	combo_entry_font_family = lookup_widget (GTK_WIDGET (button),
							"combo_entry_font_family");
	combo_entry_generic_family = lookup_widget (GTK_WIDGET (button),
						    "combo_entry_generic_family");
	checkbutton_font_family= lookup_widget (GTK_WIDGET (button),
						    "checkbutton_font_family");
	checkbutton_generic_family = lookup_widget (GTK_WIDGET (button),
						    "checkbutton_generic_family");
	checkbutton_font_set = lookup_widget (GTK_WIDGET (button),
						    "checkbutton_font_set");  

	strgf = gtk_editable_get_chars (GTK_EDITABLE
					(combo_entry_font_family), 0, -1);
	strff = gtk_editable_get_chars (GTK_EDITABLE
					(combo_entry_generic_family), 0, -1);

	store = (GtkListStore *) user_data;
	gtk_list_store_append (store, &iter);
	gtk_list_store_set (store, &iter, 0, strgf, 1, strff, -1);

	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(checkbutton_font_set), TRUE);
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(checkbutton_font_family), FALSE);
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(checkbutton_generic_family), FALSE);
	
	g_free (strgf);
	g_free (strff);
}


void on_button_font_family_dialog_del_clicked
	(GtkButton * button, gpointer user_data)
{
	GtkListStore *store;
	GtkWidget *treeview;
	GtkTreeSelection *selection;
	gboolean isselected;
	GtkTreeIter iter;

	store = (GtkListStore *) user_data;
	treeview = lookup_widget (GTK_WIDGET (button), "treeview");
	selection = gtk_tree_view_get_selection (GTK_TREE_VIEW (treeview));
	isselected = gtk_tree_selection_get_selected (selection, NULL, &iter);

	if (isselected)
	{
		gtk_list_store_remove (store, &iter);
	}
}
void on_button_font_fam_dialog_set_toggled
	(GtkToggleButton * togglebutton, gpointer user_data)
{
	GtkWidget *radiobutton;
	gboolean isactive;

	radiobutton = GTK_WIDGET (user_data);
	isactive = gtk_toggle_button_get_active (togglebutton);
	if (isactive)
		gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (radiobutton),
					      TRUE);
}

void on_font_size_dialog_okbutton_clicked
	(GtkButton * button, gpointer user_data)
{
	CssedCssDialogsData *data;
	CssedWindow *window;
	CssedDoc *tmpdoc;
	gboolean add_previous;
	GtkWidget *font_size_dialog;
	gchar *output_string;

	data = (CssedCssDialogsData *) user_data;
	window = CSSED_WINDOW (data->main_window);
	tmpdoc = document_get_current (window);
	add_previous = data->add_property_string;

	font_size_dialog =
		lookup_widget (GTK_WIDGET (button), "font_size_dialog");

	output_string = cssdialog_font_size_get_string ( window, (gpointer) button );
	//document_new_undo_action(tmpdoc);
	if (add_previous)
	{
		document_add_text (tmpdoc, (gchar *) data->previous_string);
		document_add_text (tmpdoc, ": ");
	}

	document_add_text (tmpdoc, output_string);

	if (add_previous)
	{
		document_add_text (tmpdoc, " ;\n\t");
	}
	//document_new_undo_action(tmpdoc);
	g_free (output_string);
	g_free (data->previous_string);
	g_free (user_data);
	gtk_widget_destroy (font_size_dialog);
}

void on_frecuency_property_dialog_okbutton_clicked
	(GtkButton * button, gpointer user_data)
{
	CssedCssDialogsData *data;
	CssedWindow *window;
	CssedDoc *tmpdoc;
	gboolean add_previous;
	GtkWidget *frecuency_property_dialog;
	gchar *output_string;

	data = (CssedCssDialogsData *) user_data;
	window = CSSED_WINDOW (data->main_window);
	tmpdoc = document_get_current (window);
	add_previous = data->add_property_string;

	frecuency_property_dialog =
		lookup_widget (GTK_WIDGET (button),
			       "frecuency_property_dialog");

	output_string =
		cssdialog_frecuenzy_property_get_string ( window, (gpointer) button );

	//document_new_undo_action(tmpdoc);
	if (add_previous)
	{
		document_add_text (tmpdoc, (gchar *) data->previous_string);
		document_add_text (tmpdoc, ": ");
	}

	document_add_text (tmpdoc, output_string);

	if (add_previous)
	{
		document_add_text (tmpdoc, ";\n\t");
	}

	//document_new_undo_action(tmpdoc);
	g_free (output_string);
	g_free (data->previous_string);
	g_free (user_data);
	gtk_widget_destroy (frecuency_property_dialog);
}

void on_identifier_value_dialog_okbutton_clicked
	(GtkButton * button, gpointer user_data)
{
	CssedCssDialogsData *data;
	CssedWindow *window;
	CssedDoc *tmpdoc;
	gboolean add_previous;
	GtkWidget *identifier_value_dialog;
	gchar *output_string;

	data = (CssedCssDialogsData *) user_data;
	window = CSSED_WINDOW (data->main_window);
	tmpdoc = document_get_current (window);
	add_previous = data->add_property_string;

	identifier_value_dialog =
		lookup_widget (GTK_WIDGET (button),
			       "identifier_value_dialog");

	output_string =
		cssdialog_identifier_value_get_string ( window, (gpointer) button );
	if (output_string != NULL)
	{
		//document_new_undo_action(tmpdoc);
		if (add_previous)
		{
			document_add_text (tmpdoc,
					   (gchar *) data->previous_string);
			document_add_text (tmpdoc, ": ");
		}
		document_add_text (tmpdoc, output_string);
		if (add_previous)
		{
			document_add_text (tmpdoc, ";\n");
			if( document_last_line_is_indented(tmpdoc) )
				document_indent_as_last_line(tmpdoc);
			else
				document_add_text (tmpdoc, "\t");
		}
		//document_new_undo_action(tmpdoc);
		g_free (output_string);
		g_free (data->previous_string);
		g_free (user_data);
		gtk_widget_destroy (identifier_value_dialog);
	}
	else
	{
		document_write_error_to_program_output (tmpdoc,
							_
							("identifier dialog: rejecting to add empty values. Identifier required."),
							ERROR_COLOR_STRING);
	}
}

/* AQUI AHORA */
void on_integer_value_dialog_okbutton_clicked
	(GtkButton * button, gpointer user_data)
{
	CssedCssDialogsData *data;
	CssedWindow *window;
	CssedDoc *tmpdoc;
	gboolean add_previous;
	GtkWidget *integer_value_dialog;
	gchar *output_string;
	gchar *integer;

	data = (CssedCssDialogsData *) user_data;
	window = CSSED_WINDOW (data->main_window);
	tmpdoc = document_get_current (window);
	add_previous = data->add_property_string;

	integer_value_dialog =
		lookup_widget (GTK_WIDGET (button), "integer_value_dialog");

	output_string =
		cssdialog_integer_value_get_string ( window, (gpointer) button );
	if (output_string != NULL)
	{
		//document_new_undo_action(tmpdoc);
		if (add_previous)
		{
			integer = g_strdup_printf ("%s: %s;\n\t",
						   (gchar *) data->
						   previous_string,
						   output_string);
			document_add_text (tmpdoc, integer);
			g_free (integer);
		}
		else
		{
			document_add_text (tmpdoc, output_string);
		}
		//document_new_undo_action(tmpdoc);
		g_free (output_string);
	}

	g_free (data->previous_string);
	g_free (user_data);
	gtk_widget_destroy (integer_value_dialog);
}

void on_length_value_dialog_okbutton_clicked
	(GtkButton * button, gpointer user_data)
{
	CssedCssDialogsData *data;
	CssedWindow *window;
	CssedDoc *tmpdoc;
	gboolean add_previous;
	GtkWidget *length_value_dialog;
	gchar *output_string;
	gchar *strlength;

	data = (CssedCssDialogsData *) user_data;
	window = CSSED_WINDOW (data->main_window);
	tmpdoc = document_get_current (window);
	add_previous = data->add_property_string;

	length_value_dialog =
		lookup_widget (GTK_WIDGET (button), "length_value_dialog");

	output_string =
		cssdialog_length_value_get_string ( window, (gpointer) button );

	if (output_string != NULL)
	{
		//document_new_undo_action(tmpdoc);
		if (add_previous)
		{
			strlength =
				g_strdup_printf ("%s: %s;\n\t",
						 (gchar *) data->
						 previous_string,
						 output_string);

			document_add_text (tmpdoc, strlength);
			g_free (output_string);
			g_free (strlength);
		}
		else
		{
			document_add_text (tmpdoc, output_string);
			g_free (output_string);
		}
		//document_new_undo_action(tmpdoc);
	}

	g_free (data->previous_string);
	g_free (user_data);
	gtk_widget_destroy (length_value_dialog);
}

void on_list_style_dialog_okbutton_clicked
	(GtkButton * button, gpointer user_data)
{
	CssedCssDialogsData *data;
	CssedWindow *window;
	CssedDoc *tmpdoc;
	gboolean add_previous;
	GtkWidget *list_style_dialog;
	gchar *output_string;

	data = (CssedCssDialogsData *) user_data;
	window = CSSED_WINDOW (data->main_window);
	tmpdoc = document_get_current (window);
	add_previous = data->add_property_string;

	list_style_dialog =
		lookup_widget (GTK_WIDGET (button), "list_style_dialog");

	output_string = cssdialog_list_style_get_string ( window, (gpointer) button );
	if (output_string != NULL)
	{
		//document_new_undo_action(tmpdoc);
		if (add_previous)
		{
			document_add_text (tmpdoc,
					   (gchar *) data->previous_string);
			document_add_text (tmpdoc, ": ");
		}

		document_add_text (tmpdoc, output_string);

		if (add_previous)
		{
			document_add_text (tmpdoc, ";\n");
			if( document_last_line_is_indented(tmpdoc) )
				document_indent_as_last_line(tmpdoc);
			else
				document_add_text (tmpdoc, "\t");
		}
		//document_new_undo_action(tmpdoc);                                     

		g_free (output_string);
		g_free (data->previous_string);
		g_free (user_data);
		gtk_widget_destroy (list_style_dialog);
	}
	else
	{
		document_write_error_to_program_output (tmpdoc,
							_
							("list style dialog: rejecting to add empty values"),
							ERROR_COLOR_STRING);
	}
}

void on_margin_property_dialog_okbutton_clicked
	(GtkButton * button, gpointer user_data)
{
	CssedCssDialogsData *data;
	CssedWindow *window;
	CssedDoc *tmpdoc;
	gboolean add_previous;
	GtkWidget *margin_property_dialog;
	gchar *output_string;

	data = (CssedCssDialogsData *) user_data;
	window = CSSED_WINDOW (data->main_window);
	tmpdoc = document_get_current (window);
	add_previous = data->add_property_string;

	margin_property_dialog =
		lookup_widget (GTK_WIDGET (button), "margin_property_dialog");

	output_string =
		cssdialog_margin_property_get_string ( window, (gpointer) button );

	if (output_string == NULL)
	{
		document_write_error_to_program_output (tmpdoc,
							_
							("margin property dialog: rejecting to add empty values."),
							ERROR_COLOR_STRING);
	}
	else
	{

		//document_new_undo_action(tmpdoc);
		if (add_previous)
		{
			document_add_text (tmpdoc,
					   (gchar *) data->previous_string);
			document_add_text (tmpdoc, ": ");
		}

		document_add_text (tmpdoc, output_string);

		if (add_previous)
		{
			document_add_text (tmpdoc, ";\n");
			if( document_last_line_is_indented(tmpdoc) )
				document_indent_as_last_line(tmpdoc);
			else
				document_add_text (tmpdoc, "\t");
		}
		//document_new_undo_action(tmpdoc);

		g_free (output_string);
		g_free (data->previous_string);
		g_free (user_data);
		gtk_widget_destroy (margin_property_dialog);
	}
}

void on_margin_width_dialog_okbutton_clicked
	(GtkButton * button, gpointer user_data)
{
	CssedCssDialogsData *data;
	CssedWindow *window;
	CssedDoc *tmpdoc;
	gboolean add_previous;
	GtkWidget *margin_width_value_dialog;
	gchar *output_string;

	data = (CssedCssDialogsData *) user_data;
	window = CSSED_WINDOW (data->main_window);
	tmpdoc = document_get_current (window);
	add_previous = data->add_property_string;

	margin_width_value_dialog =
		lookup_widget (GTK_WIDGET (button),
			       "margin_width_value_dialog");

	output_string =
		cssdialog_margin_width_get_string ( window, (gpointer) button );
	//document_new_undo_action(tmpdoc);
	if (add_previous)
	{
		document_add_text (tmpdoc, (gchar *) data->previous_string);
		document_add_text (tmpdoc, ": ");
	}

	document_add_text (tmpdoc, output_string);

	if (add_previous)
	{
		document_add_text (tmpdoc, ";\n\t");
	}
	//document_new_undo_action(tmpdoc);

	g_free (output_string);
	g_free (data->previous_string);
	g_free (user_data);
	gtk_widget_destroy (margin_width_value_dialog);
}

void on_number_value_dialog_okbutton_clicked
	(GtkButton * button, gpointer user_data)
{
	CssedCssDialogsData *data;
	CssedWindow *window;
	CssedDoc *tmpdoc;
	gboolean add_previous;
	gchar *output_string;
	GtkWidget *number_value_dialog;

	data = (CssedCssDialogsData *) user_data;
	window = CSSED_WINDOW (data->main_window);
	tmpdoc = document_get_current (window);
	add_previous = data->add_property_string;

	number_value_dialog =
		lookup_widget (GTK_WIDGET (button), "number_value_dialog");

	output_string =
		cssdialog_number_value_get_string ( window, (gpointer) button );

	//document_new_undo_action(tmpdoc);
	if (add_previous)
	{
		document_add_text (tmpdoc, (gchar *) data->previous_string);
		document_add_text (tmpdoc, ": ");
	}

	document_add_text (tmpdoc, output_string);

	if (add_previous)
	{
		document_add_text (tmpdoc, ";\n\t");
	}
	//document_new_undo_action(tmpdoc);

	g_free (output_string);
	g_free (data->previous_string);
	g_free (user_data);
	gtk_widget_destroy (number_value_dialog);
}

void on_padding_property_dialog_okbutton_clicked
	(GtkButton * button, gpointer user_data)
{
	CssedCssDialogsData *data;
	CssedWindow *window;
	CssedDoc *tmpdoc;
	gboolean add_previous;
	GtkWidget *padding_property_dialog;
	gchar *output_string;

	data = (CssedCssDialogsData *) user_data;
	window = CSSED_WINDOW (data->main_window);
	tmpdoc = document_get_current (window);
	add_previous = data->add_property_string;

	padding_property_dialog =
		lookup_widget (GTK_WIDGET (button),
			       "padding_property_dialog");

	output_string =
		cssdialog_padding_property_get_string ( window, (gpointer) button );
	if (output_string == NULL)
	{
		document_write_error_to_program_output (tmpdoc,
							_
							("padding property dialog: no value in dialog, defaulting to zero."),
							WARNING_COLOR_STRING);
		//document_new_undo_action(tmpdoc);
		if (add_previous)
		{
			document_add_text (tmpdoc,
					   (gchar *) data->previous_string);
			document_add_text (tmpdoc, ": ");
		}
		document_add_text (tmpdoc, "0");
		if (add_previous)
		{
			document_add_text (tmpdoc, ";\n");
			if( document_last_line_is_indented(tmpdoc) )
				document_indent_as_last_line(tmpdoc);
			else
				document_add_text (tmpdoc, "\t");
		}
		//document_new_undo_action(tmpdoc);
	}
	else
	{
		//document_new_undo_action(tmpdoc);
		if (add_previous)
		{
			document_add_text (tmpdoc,
					   (gchar *) data->previous_string);
			document_add_text (tmpdoc, ": ");
		}
		document_add_text (tmpdoc, output_string);
		if (add_previous)
		{
			document_add_text (tmpdoc, ";\n");
			if( document_last_line_is_indented(tmpdoc) )
				document_indent_as_last_line(tmpdoc);
			else
				document_add_text (tmpdoc, "\t");
		}
		//document_new_undo_action(tmpdoc);
		g_free (output_string);
	}

	g_free (data->previous_string);
	g_free (user_data);
	gtk_widget_destroy (padding_property_dialog);
}

void on_outline_property_dialog_okbutton_clicked
	(GtkButton * button, gpointer user_data)
{
	CssedCssDialogsData *data;
	CssedWindow *window;
	CssedDoc *tmpdoc;
	gboolean add_previous;
	GtkWidget *outline_property_dialog;
	gchar *output_string;

	data = (CssedCssDialogsData *) user_data;
	window = CSSED_WINDOW (data->main_window);
	tmpdoc = document_get_current (window);
	add_previous = data->add_property_string;

	outline_property_dialog =
		lookup_widget (GTK_WIDGET (button),
			       "outline_property_dialog");

	output_string =
		cssdialog_outline_property_get_string( window, (gpointer) button );

	if (output_string == NULL)
	{
		document_write_error_to_program_output (tmpdoc,
							_
							("outline dialog: rejecting to add empty values."),
							ERROR_COLOR_STRING);
	}
	else
	{
		//document_new_undo_action(tmpdoc);
		if (add_previous)
		{
			document_add_text (tmpdoc,
					   (gchar *) data->previous_string);
			document_add_text (tmpdoc, ": ");
		}
		document_add_text (tmpdoc, output_string);
		if (add_previous)
		{
			document_add_text (tmpdoc, ";\n");
			if( document_last_line_is_indented(tmpdoc) )
				document_indent_as_last_line(tmpdoc);
			else
				document_add_text (tmpdoc, "\t");
		}
		//document_new_undo_action(tmpdoc);

		g_free (output_string);
		g_free (data->previous_string);
		g_free (user_data);
		gtk_widget_destroy (outline_property_dialog);
	}
}

void on_pause_property_dialog_okbutton_clicked
	(GtkButton * button, gpointer user_data)
{
	CssedCssDialogsData *data;
	CssedWindow *window;
	CssedDoc *tmpdoc;
	gboolean add_previous;
	GtkWidget *pause_property_dialog;
	gchar *output_string;

	data = (CssedCssDialogsData *) user_data;
	window = CSSED_WINDOW (data->main_window);
	tmpdoc = document_get_current (window);
	add_previous = data->add_property_string;

	pause_property_dialog =
		lookup_widget (GTK_WIDGET (button), "pause_property_dialog");

	output_string =
		cssdialog_pause_property_get_string ( window, (gpointer) button );

	if (output_string == NULL)
	{
		document_write_error_to_program_output (tmpdoc,
							_
							("pause dialog: rejecting to add empty values"),
							ERROR_COLOR_STRING);
	}
	else
	{
		//document_new_undo_action(tmpdoc);
		if (add_previous)
		{
			document_add_text (tmpdoc,
					   (gchar *) data->previous_string);
			document_add_text (tmpdoc, ": ");
		}
		document_add_text (tmpdoc, output_string);
		if (add_previous)
		{
			document_add_text (tmpdoc, ";\n");
			if( document_last_line_is_indented(tmpdoc) )
				document_indent_as_last_line(tmpdoc);
			else
				document_add_text (tmpdoc, "\t");
		}
		//document_new_undo_action(tmpdoc);

		g_free (output_string);
		g_free (data->previous_string);
		g_free (user_data);
		gtk_widget_destroy (pause_property_dialog);
	}
}

void on_percentage_value_dialog_okbutton_clicked
	(GtkButton * button, gpointer user_data)
{
	CssedCssDialogsData *data;
	CssedWindow *window;
	CssedDoc *tmpdoc;
	gboolean add_previous;
	GtkWidget *percentage_value_dialog;
	gchar *output_string;

	data = (CssedCssDialogsData *) user_data;
	window = CSSED_WINDOW (data->main_window);
	tmpdoc = document_get_current (window);
	add_previous = data->add_property_string;

	percentage_value_dialog =
		lookup_widget (GTK_WIDGET (button),
			       "percentage_value_dialog");

	output_string =
		cssdialog_percentage_value_get_string ( window, (gpointer) button );

	//document_new_undo_action(tmpdoc);
	if (add_previous)
	{
		document_add_text (tmpdoc, (gchar *) data->previous_string);
		document_add_text (tmpdoc, ": ");
	}

	document_add_text (tmpdoc, output_string);

	document_add_text (tmpdoc, "%");
	if (add_previous)
	{
		document_add_text (tmpdoc, ";\n\t");
	}
	//document_new_undo_action(tmpdoc);
	g_free (output_string);
	g_free (data->previous_string);
	g_free (user_data);
	gtk_widget_destroy (percentage_value_dialog);
}

void on_percentage_length_value_dialog_okbutton_clicked
	(GtkButton * button, gpointer user_data)
{
	CssedCssDialogsData *data;
	CssedWindow *window;
	CssedDoc *tmpdoc;
	gboolean add_previous;
	GtkWidget *percentage_length_value_dialog;
	gchar *output_string;

	data = (CssedCssDialogsData *) user_data;
	window = CSSED_WINDOW (data->main_window);
	tmpdoc = document_get_current (window);
	add_previous = data->add_property_string;

	percentage_length_value_dialog =
		lookup_widget (GTK_WIDGET (button),
			       "percentage_length_value_dialog");
	output_string =
		cssdialog_percentage_length_value_get_string ( window, (gpointer) button );

	//document_new_undo_action(tmpdoc);
	if (add_previous)
	{
		document_add_text (tmpdoc, (gchar *) data->previous_string);
		document_add_text (tmpdoc, ": ");
	}

	document_add_text (tmpdoc, output_string);

	if (add_previous)
	{
		document_add_text (tmpdoc, ";\n\t");
	}
	//document_new_undo_action(tmpdoc);
	g_free (output_string);
	g_free (data->previous_string);
	g_free (user_data);
	gtk_widget_destroy (percentage_length_value_dialog);
}

void on_play_during_property_dialog_okbutton_clicked
	(GtkButton * button, gpointer user_data)
{
	CssedCssDialogsData *data;
	CssedWindow *window;
	CssedDoc *tmpdoc;
	gboolean add_previous;
	GtkWidget *play_during_property_dialog;
	gchar *output_string;

	data = (CssedCssDialogsData *) user_data;
	window = CSSED_WINDOW (data->main_window);
	tmpdoc = document_get_current (window);
	add_previous = data->add_property_string;

	play_during_property_dialog =
		lookup_widget (GTK_WIDGET (button),
			       "play_during_property_dialog");
	output_string =
		cssdialog_play_during_property_get_string ( window, (gpointer) button );
	if (output_string != NULL)
	{

		//document_new_undo_action(tmpdoc);
		if (add_previous)
		{
			document_add_text (tmpdoc,
					   (gchar *) data->previous_string);
			document_add_text (tmpdoc, ":");
		}

		document_add_text (tmpdoc, output_string);

		if (add_previous)
		{
			document_add_text (tmpdoc, ";\n");
			if( document_last_line_is_indented(tmpdoc) )
				document_indent_as_last_line(tmpdoc);
			else
				document_add_text (tmpdoc, "\t");
		}
		//document_new_undo_action(tmpdoc);

		g_free (output_string);
		g_free (data->previous_string);
		g_free (user_data);
		gtk_widget_destroy (play_during_property_dialog);
	}
	else
	{
		document_write_error_to_program_output (tmpdoc,
							_
							("play during dialog: value \"uri\" required, rejecting to add invalid values"),
							ERROR_COLOR_STRING);
	}
}

/* this needs three callbacks */
void on_button_quotes_property_dialog_add_clicked
	(GtkButton * button, gpointer user_data)
{
	GtkWidget *entry_open;
	GtkWidget *entry_close;

	GtkListStore *store;
	GtkTreeIter iter;
	gchar *stroq;
	gchar *strcq;

	store = (GtkListStore *) user_data;

	entry_open = lookup_widget (GTK_WIDGET (button), "entry_open");
	entry_close = lookup_widget (GTK_WIDGET (button), "entry_close");

	stroq = gtk_editable_get_chars (GTK_EDITABLE (entry_open), 0, -1);
	strcq = gtk_editable_get_chars (GTK_EDITABLE (entry_close), 0, -1);

	if ((strlen (stroq) > 0) && (strlen (strcq) > 0))
	{
		gtk_list_store_append (store, &iter);
		gtk_list_store_set (store, &iter, 0, stroq, 1, strcq, -1);
	}
	else
	{			// this will default to the same open and close quotes if only 
		// one value is added. nothing will be added to the store if both
		// values are empty.
		if (strlen (stroq) > 0)
		{
			gtk_list_store_append (store, &iter);
			gtk_list_store_set (store, &iter, 0, stroq, 1, stroq,
					    -1);
		}
		if (strlen (strcq) > 0)
		{
			gtk_list_store_append (store, &iter);
			gtk_list_store_set (store, &iter, 0, strcq, 1, strcq,
					    -1);
		}
	}

	g_free (stroq);
	g_free (strcq);
}


void on_button_quotes_property_dialog_del_clicked
	(GtkButton * button, gpointer user_data)
{
	GtkListStore *store;
	GtkWidget *treeview;
	GtkTreeSelection *selection;
	gboolean isselected;
	GtkTreeIter iter;

	store = (GtkListStore *) user_data;
	treeview = lookup_widget (GTK_WIDGET (button), "treeview");
	selection = gtk_tree_view_get_selection (GTK_TREE_VIEW (treeview));
	isselected = gtk_tree_selection_get_selected (selection, NULL, &iter);

	if (isselected)
	{
		gtk_list_store_remove (store, &iter);
	}
}

void on_quotes_property_dialog_okbutton_clicked
	(GtkButton * button, gpointer user_data)
{
	CssedCssDialogsData *data;
	CssedWindow *window;
	CssedDoc *tmpdoc;
	GtkWidget *quotes_property_dialog;
	gboolean add_previous;
	gchar *output_string;

	data = (CssedCssDialogsData *) user_data;
	window = CSSED_WINDOW (data->main_window);
	tmpdoc = document_get_current (window);
	add_previous = data->add_property_string;

	quotes_property_dialog =
		lookup_widget (GTK_WIDGET (button), "quotes_property_dialog");

	output_string =
		cssdialog_quotes_property_get_string ( window, (gpointer) button );
	if (output_string != NULL)
	{
		//document_new_undo_action(tmpdoc);
		if (add_previous)
		{
			document_add_text (tmpdoc,
					   (gchar *) data->previous_string);
			document_add_text (tmpdoc, ": ");
		}
		document_add_text (tmpdoc, output_string);
		if (add_previous)
		{
			document_add_text (tmpdoc, ";\n");
			if( document_last_line_is_indented(tmpdoc) )
				document_indent_as_last_line(tmpdoc);
			else
				document_add_text (tmpdoc, "\t");
		}
		//document_new_undo_action(tmpdoc);
		g_free (output_string);
		g_free (data->previous_string);
		g_free (user_data);
		gtk_widget_destroy (quotes_property_dialog);
	}
	else
	{			// not valid nothing in the list
		document_write_error_to_program_output (tmpdoc,
							_
							("quotes dialog: nothing in the list, rejecting to add empty values."),
							WARNING_COLOR_STRING);
	}
}

/* end quotes */

void
on_shape_value_dialog_okbutton_clicked (GtkButton * button,
					gpointer user_data)
{
	CssedCssDialogsData *data;
	CssedWindow *window;
	CssedDoc *tmpdoc;
	gboolean add_previous;
	GtkWidget *shape_value_dialog;
	gchar *output_string;

	data = (CssedCssDialogsData *) user_data;
	window = CSSED_WINDOW (data->main_window);
	tmpdoc = document_get_current (window);
	add_previous = data->add_property_string;

	shape_value_dialog =
		lookup_widget (GTK_WIDGET (button), "shape_value_dialog");
	output_string =
		cssdialog_shape_value_get_string ( window, (gpointer) button );
	//document_new_undo_action(tmpdoc);
	if (add_previous)
	{
		document_add_text (tmpdoc, (gchar *) data->previous_string);
		document_add_text (tmpdoc, ": ");
	}

	document_add_text (tmpdoc, output_string);

	if (add_previous)
	{
		document_add_text (tmpdoc, ";\n\t");
	}
	//document_new_undo_action(tmpdoc);

	g_free (output_string);
	g_free (data->previous_string);
	g_free (user_data);
	gtk_widget_destroy (shape_value_dialog);
}


void on_size_length_value_dialog_okbutton_clicked
	(GtkButton * button, gpointer user_data)
{
	CssedCssDialogsData *data;
	CssedWindow *window;
	CssedDoc *tmpdoc;
	gboolean add_previous;
	GtkWidget *size_length_value_dialog;
	gchar *output_string;

	data = (CssedCssDialogsData *) user_data;
	window = CSSED_WINDOW (data->main_window);
	tmpdoc = document_get_current (window);
	add_previous = data->add_property_string;

	size_length_value_dialog =
		lookup_widget (GTK_WIDGET (button),
			       "size_length_value_dialog");
	output_string =
		cssdialog_size_length_value_get_string ( window, (gpointer) button );
	//document_new_undo_action(tmpdoc);
	if (add_previous)
	{
		document_add_text (tmpdoc, (gchar *) data->previous_string);
		document_add_text (tmpdoc, ": ");
	}

	document_add_text (tmpdoc, output_string);

	if (add_previous)
	{
		document_add_text (tmpdoc, ";\n\t");
	}
	//document_new_undo_action(tmpdoc);

	g_free (output_string);
	g_free (data->previous_string);
	g_free (user_data);
	gtk_widget_destroy (size_length_value_dialog);
}


/* needs three callbacks */
/* **************  I'm working here right now  ****************************** */
void on_button_text_shadow_value_dialog_add_clicked
	(GtkButton * button, gpointer user_data)
{
	GtkListStore *store;
	GtkTreeIter iter;
	GtkWidget *spinbutton_right;
	GtkWidget *combo_entry_right;
	GtkWidget *spinbutton_below;
	GtkWidget *combo_entry_below;
	GtkWidget *checkbutton_use_color;
	GtkWidget *checkbutton_blur;
	GtkWidget *spinbutton_blur;
	GtkWidget *combo_entry_blur;
	GtkWidget *combo_entry_use_color;
	GtkWidget *colorselection;
	GtkWidget *checkbutton_use_shadow_list;
	gchar *str_right_units;
	gchar *str_right;
	gdouble nright;
	gchar *str_below_units;
	gchar *str_below;
	gdouble nbelow;
	gchar *str_blur_units = NULL;
	gchar *str_blur;
	gdouble nblur;
	gboolean b_blur;
	gchar *strcolor = NULL;
	gchar *strrgbcolor = NULL;
	GdkColor gdkcolor;
	gboolean b_color;

	enum
	{
		COL_RIGHT,
		COL_BELOW,
		COL_BLUR,
		COL_COLOR,
		N_COLS
	};
	// if the're adding valus to the list is for using it so check the
	// use shadow list on bahalve of the user.
	checkbutton_use_shadow_list =
		lookup_widget (GTK_WIDGET (button),
			       "checkbutton_use_shadow_list");
	gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON
				      (checkbutton_use_shadow_list), TRUE);

	store = (GtkListStore *) user_data;

	spinbutton_right =
		lookup_widget (GTK_WIDGET (button), "spinbutton_right");
	combo_entry_right =
		lookup_widget (GTK_WIDGET (button), "combo_entry_right");
	spinbutton_below =
		lookup_widget (GTK_WIDGET (button), "spinbutton_below");
	combo_entry_below =
		lookup_widget (GTK_WIDGET (button), "combo_entry_below");
	checkbutton_blur =
		lookup_widget (GTK_WIDGET (button), "checkbutton_blur");
	checkbutton_use_color =
		lookup_widget (GTK_WIDGET (button), "checkbutton_use_color");

	b_blur = gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON
					       (checkbutton_blur));
	b_color =
		gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON
					      (checkbutton_use_color));

	nright = gtk_spin_button_get_value (GTK_SPIN_BUTTON
						     (spinbutton_right));
	str_right_units =
		gtk_editable_get_chars (GTK_EDITABLE (combo_entry_right), 0,
					-1);
	str_right = g_strdup_printf ("%0.2f%s", nright, str_right_units);

	nbelow = gtk_spin_button_get_value (GTK_SPIN_BUTTON
						     (spinbutton_below));
	str_below_units =
		gtk_editable_get_chars (GTK_EDITABLE (combo_entry_below), 0,
					-1);
	str_below = g_strdup_printf ("%0.2f%s", nbelow, str_below_units);

	if (b_blur)
	{
		spinbutton_blur =
			lookup_widget (GTK_WIDGET (button),
				       "spinbutton_blur");
		combo_entry_blur =
			lookup_widget (GTK_WIDGET (button),
				       "combo_entry_blur");
		nblur = gtk_spin_button_get_value (GTK_SPIN_BUTTON
							    (spinbutton_blur));
		str_blur_units =
			gtk_editable_get_chars (GTK_EDITABLE
						(combo_entry_blur), 0, -1);
		str_blur = g_strdup_printf ("%0.2f%s", nblur, str_blur_units);
	}
	else
	{
		str_blur = g_strdup ("");
	}
	if (b_color)
	{
		combo_entry_use_color =
			lookup_widget (GTK_WIDGET (button),
				       "combo_entry_use_color");
		colorselection =
			lookup_widget (GTK_WIDGET (button), "colorselection");
		strcolor =
			gtk_editable_get_chars (GTK_EDITABLE
						(combo_entry_use_color), 0,
						-1);
		if (strcmp (strcolor, CSS_STR_USE_COLORSEL) == 0)
		{
			gtk_color_selection_get_current_color
				(GTK_COLOR_SELECTION (colorselection),
				 &gdkcolor);
			strrgbcolor =
				gdk_color_to_rgb_triplet_string (&gdkcolor);
		}
		else
		{
			strrgbcolor = g_strdup (strcolor);
		}
	}

	gtk_list_store_append (store, &iter);
	gtk_list_store_set (store, &iter, COL_RIGHT, str_right, COL_BELOW,
			    str_below, COL_BLUR, str_blur, COL_COLOR,
			    strrgbcolor, -1);

	g_free (strcolor);
	g_free (strrgbcolor);
	g_free (str_blur_units);
	g_free (str_blur);
	g_free (str_right_units);
	g_free (str_below_units);
	g_free (str_below);
	g_free (str_right);
}


void on_button_text_shadow_value_dialog_del_clicked
	(GtkButton * button, gpointer user_data)
{
	GtkListStore *store;
	GtkWidget *treeview;
	GtkTreeSelection *selection;
	gboolean isselected;
	GtkTreeIter iter;

	store = (GtkListStore *) user_data;
	treeview = lookup_widget (GTK_WIDGET (button), "treeview");
	selection = gtk_tree_view_get_selection (GTK_TREE_VIEW (treeview));
	isselected = gtk_tree_selection_get_selected (selection, NULL, &iter);

	if (isselected)
	{
		gtk_list_store_remove (store, &iter);
	}
}


void on_text_shadow_value_dialog_okbutton_clicked
	(GtkButton * button, gpointer user_data)
{
	CssedCssDialogsData *data;
	CssedWindow *window;
	CssedDoc *tmpdoc;
	gboolean add_previous;
	GtkWidget *text_shadow_value_dialog;
	gchar *output_string;

	data = (CssedCssDialogsData *) user_data;
	window = CSSED_WINDOW (data->main_window);
	tmpdoc = document_get_current (window);
	add_previous = data->add_property_string;

	text_shadow_value_dialog =
		lookup_widget (GTK_WIDGET (button),
			       "text_shadow_value_dialog");

	output_string =
		cssdialog_text_shadow_value_get_string ( window, (gpointer) button );
	if (output_string != NULL)
	{
		//document_new_undo_action(tmpdoc);
		if (add_previous)
		{
			document_add_text (tmpdoc,
					   (gchar *) data->previous_string);
			document_add_text (tmpdoc, ": ");
		}
		document_add_text (tmpdoc, output_string);
		if (add_previous)
		{
			document_add_text (tmpdoc, ";\n");
			if( document_last_line_is_indented(tmpdoc) )
				document_indent_as_last_line(tmpdoc);
			else
				document_add_text (tmpdoc, "\t");
		}
		//document_new_undo_action(tmpdoc);
		g_free (output_string);
		g_free (data->previous_string);
		g_free (user_data);
		gtk_widget_destroy (text_shadow_value_dialog);
	}
	else
	{			// nothing in the list
		document_write_error_to_program_output (tmpdoc,
							_
							("text shadow dialog: shadow list selected but nothing in the list. Rejecting to add empty values."),
							WARNING_COLOR_STRING);
	}
}

// end text shadow
/* needs three callbacks */
void on_button_voice_family_value_dialog_add_clicked
	(GtkButton * button, gpointer user_data)
{
	GtkWidget *entry_specific_voice;
	GtkWidget *combo_entry_generic;
	GtkWidget *checkbutton_voice_set;
	GtkListStore *store;
	GtkTreeIter iter;
	gchar *str_gen;
	gchar *str_spec;

	enum
	{
		COL_GENERIC,
		COL_SPECIFIC,
		N_COLS
	};

	store = (GtkListStore *) user_data;

	entry_specific_voice =
		lookup_widget (GTK_WIDGET (button), "entry_specific_voice");
	combo_entry_generic = lookup_widget (GTK_WIDGET (button),
					     "combo_entry_generic");

	checkbutton_voice_set = lookup_widget (GTK_WIDGET (button),
					       "checkbutton_voice_set");

	str_spec = gtk_editable_get_chars (GTK_EDITABLE
					   (entry_specific_voice), 0, -1);
	str_gen = gtk_editable_get_chars (GTK_EDITABLE
					  (combo_entry_generic), 0, -1);
	// add only if there is at least one valid value
	if ((strlen (str_spec) > 0) || (strlen (str_gen) > 0))
	{
		gtk_list_store_append (store, &iter);
		gtk_list_store_set (store, &iter, COL_GENERIC, str_gen,
				    COL_SPECIFIC, str_spec, -1);
		// set the "use list" toggle active as they are adding values to the list
		gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON
					      (checkbutton_voice_set), TRUE);
	}
	g_free (str_spec);
	g_free (str_gen);
}

void on_button_voice_family_value_dialog_del_clicked
	(GtkButton * button, gpointer user_data)
{
	GtkListStore *store;
	GtkWidget *treeview;
	GtkTreeSelection *selection;
	gboolean isselected;
	GtkTreeIter iter;

	store = (GtkListStore *) user_data;
	treeview = lookup_widget (GTK_WIDGET (button), "treeview");
	selection = gtk_tree_view_get_selection (GTK_TREE_VIEW (treeview));
	isselected = gtk_tree_selection_get_selected (selection, NULL, &iter);

	if (isselected)
	{
		gtk_list_store_remove (store, &iter);
	}
}

void on_voice_family_value_dialog_okbutton_clicked
	(GtkButton * button, gpointer user_data)
{
	CssedCssDialogsData *data;
	CssedWindow *window;
	CssedDoc *tmpdoc;
	gboolean add_previous;
	GtkWidget *voice_family_value_dialog;
	gchar *output_string;

	data = (CssedCssDialogsData *) user_data;
	window = CSSED_WINDOW (data->main_window);
	tmpdoc = document_get_current (window);
	add_previous = data->add_property_string;

	voice_family_value_dialog =
		lookup_widget (GTK_WIDGET (button),
			       "voice_family_value_dialog");
	output_string =
		cssdialog_voice_family_value_get_string ( window, (gpointer) button );
	if (output_string != NULL)
	{
		//document_new_undo_action(tmpdoc);
		if (add_previous)
		{
			document_add_text (tmpdoc,
					   (gchar *) data->previous_string);
			document_add_text (tmpdoc, ": ");
		}
		document_add_text (tmpdoc, output_string);
		if (add_previous)
		{
			document_add_text (tmpdoc, ";\n");
			if( document_last_line_is_indented(tmpdoc) )
				document_indent_as_last_line(tmpdoc);
			else
				document_add_text (tmpdoc, "\t");
		}
		g_free (output_string);
		g_free (data->previous_string);
		g_free (user_data);
		gtk_widget_destroy (voice_family_value_dialog);
	}
	else
	{			// nothing in the list
		document_write_error_to_program_output (tmpdoc,
							_
							("voice family dialog: rejecting to add empty values"),
							ERROR_COLOR_STRING);
	}
}

// end voice family
void on_border_width_style_color_value_dialog_okbutton_clicked
	(GtkButton * button, gpointer user_data)
{
	CssedCssDialogsData *data;
	CssedWindow *window;
	CssedDoc *tmpdoc;
	gboolean add_previous;
	GtkWidget *border_width_style_color_value_dialog;
	gchar *output_string;

	data = (CssedCssDialogsData *) user_data;
	window = CSSED_WINDOW (data->main_window);
	tmpdoc = document_get_current (window);
	add_previous = data->add_property_string;

	border_width_style_color_value_dialog =
		lookup_widget (GTK_WIDGET (button),
			       "border_width_style_color_value_dialog");

	output_string =
		cssdialog_border_width_style_color_value_get_string( window, (gpointer) button );
	if (output_string == NULL)
	{
		document_write_error_to_program_output (tmpdoc,
							_
							("width style and color dialog: nothing selected. rejecting to add empty values."),
							ERROR_COLOR_STRING);
	}
	else
	{
		//document_new_undo_action(tmpdoc);
		if (add_previous)
		{
			document_add_text (tmpdoc,
					   (gchar *) data->previous_string);
			document_add_text (tmpdoc, ": ");
		}
		document_add_text (tmpdoc, output_string);

		if (add_previous)
		{
			document_add_text (tmpdoc, ";\n");
			if( document_last_line_is_indented(tmpdoc) )
				document_indent_as_last_line(tmpdoc);
			else
				document_add_text (tmpdoc, "\t");
		}
		//document_new_undo_action(tmpdoc);

		g_free (output_string);
		g_free (data->previous_string);
		g_free (user_data);
		gtk_widget_destroy (border_width_style_color_value_dialog);
	}
}

void on_color_selector_dialog_okbutton_clicked
	(GtkButton * button, gpointer user_data)
{
	CssedCssDialogsData *data;
	CssedWindow *window;
	CssedDoc *tmpdoc;
	gboolean add_previous;
	GtkWidget *color_selector_dialog;
	gchar *output_string;

	data = (CssedCssDialogsData *) user_data;
	window = CSSED_WINDOW (data->main_window);
	tmpdoc = document_get_current (window);
	add_previous = data->add_property_string;

	color_selector_dialog =
		lookup_widget (GTK_WIDGET (button), "color_selector_dialog");

	output_string =
		cssdialog_color_selector_get_string ( window, (gpointer) button );
	//document_new_undo_action(tmpdoc);
	if (add_previous)
	{
		document_add_text (tmpdoc, (gchar *) data->previous_string);
		document_add_text (tmpdoc, ": ");
	}

	document_add_text (tmpdoc, output_string);

	if (add_previous)
	{
		document_add_text (tmpdoc, ";\n\t");
	}
	//document_new_undo_action(tmpdoc);

	g_free (output_string);
	g_free (data->previous_string);
	g_free (user_data);
	gtk_widget_destroy (color_selector_dialog);
}

void on_border_style_dialog_okbutton_clicked
	(GtkButton * button, gpointer user_data)
{
	CssedCssDialogsData *data;
	CssedWindow *window;
	CssedDoc *tmpdoc;
	gboolean add_previous;
	GtkWidget *border_style_dialog;
	gchar *output_string;

	data = (CssedCssDialogsData *) user_data;
	window = CSSED_WINDOW (data->main_window);
	tmpdoc = document_get_current (window);
	add_previous = data->add_property_string;

	border_style_dialog =
		lookup_widget (GTK_WIDGET (button), "border_style_dialog");
	output_string =
		cssdialog_border_style_get_string ( window, (gpointer) button );

	if (add_previous)
	{
		document_add_text (tmpdoc, (gchar *) data->previous_string);
		document_add_text (tmpdoc, ": ");
	}
	document_add_text (tmpdoc, output_string);
	if (add_previous)
	{
		document_add_text (tmpdoc, ";\n\t");
	}

	g_free (output_string);
	g_free (data->previous_string);
	g_free (user_data);
	gtk_widget_destroy (border_style_dialog);
}

/* ---------------- CALLED BY ALL DIALOGS ----------------------------------  */
/* this will be called on all cancel button click events                      */
void
on_css_dialog_cancel_button_clicked (GtkButton * button, gpointer user_data)
{
	GtkWidget *dialog;
	CssedCssDialogsData *data;

	data = (CssedCssDialogsData *) user_data;

	dialog = gtk_widget_get_toplevel (GTK_WIDGET (button));
	gtk_widget_destroy (dialog);
	g_free (data->previous_string);
	g_free (user_data);
}
void
on_css_dialog_unit_changed				(GtkEditable *editable,
                                            gpointer user_data)
{
	gchar* unit;
	
	unit = gtk_editable_get_chars(editable,0,-1);
	if( strlen(unit) != 0 ){
		if( strcmp(unit,"em") == 0 ||
			strcmp(unit,"in")==0 ){
			gtk_spin_button_set_digits (GTK_SPIN_BUTTON(user_data),     2);
			gtk_spin_button_set_increments  (GTK_SPIN_BUTTON(user_data), 0.1, 1);
		}else{
			gtk_spin_button_set_digits (GTK_SPIN_BUTTON(user_data), 0);
			gtk_spin_button_set_increments  (GTK_SPIN_BUTTON(user_data), 1, 1);			
		}

	}
	g_free(unit);	
}
/* this is called when any dialog is closed                                   */
void
on_css_dialog_close (GtkDialog * dialog, gpointer user_data)
{
	CssedCssDialogsData *data;

	data = (CssedCssDialogsData *) user_data;

	g_free (data->previous_string);
	g_free (user_data);
}

/* *************************** HELP BUTTONS ********************************* */
void on_angle_property_dialog_helpbutton_clicked
	(GtkButton * button, gpointer user_data)
{


}
void on_background_property_dialog_helpbutton_clicked
	(GtkButton * button, gpointer user_data)
{

}
void on_border_properties_helpbutton_clicked
	(GtkButton * button, gpointer user_data)
{

}
void on_border_with_properties_helpbutton_clicked
	(GtkButton * button, gpointer user_data)
{

}
void on_border_width_single_helpbutton_clicked
	(GtkButton * button, gpointer user_data)
{

}
void on_dialog_counter_increment_reset_helpbutton_clicked
	(GtkButton * button, gpointer user_data)
{

}
void on_string_properties_dialog_helpbutton_clicked
	(GtkButton * button, gpointer user_data)
{

}
void
on_uri_value_dialog_helpbutton_clicked (GtkButton * button,
					gpointer user_data)
{

}
void on_attr_value_dialog_helpbutton_clicked
	(GtkButton * button, gpointer user_data)
{

}
void on_counter_property_dialog_helpbutton_clicked
	(GtkButton * button, gpointer user_data)
{

}
void on_font_property_dialog_helpbutton_clicked
	(GtkButton * button, gpointer user_data)
{

}
void on_font_family_dialog_helpbutton_clicked
	(GtkButton * button, gpointer user_data)
{

}
void
on_font_size_dialog_helpbutton_clicked (GtkButton * button,
					gpointer user_data)
{

}
void
on_dialog_frecuency_helpbutton_clicked (GtkButton * button,
					gpointer user_data)
{

}
void
on_dialog_integer_helpbutton_clicked (GtkButton * button, gpointer user_data)
{

}
void on_length_value_dialog_helpbutton_clicked
	(GtkButton * button, gpointer user_data)
{

}
void on_list_style_dialog_helpbutton_clicked
	(GtkButton * button, gpointer user_data)
{

}
void on_margin_property_dialog_helpbutton_clicked
	(GtkButton * button, gpointer user_data)
{

}
void on_margin_width_value_dialog_helpbutton_clicked
	(GtkButton * button, gpointer user_data)
{

}
void on_number_value_dialog_helpbutton_clicked
	(GtkButton * button, gpointer user_data)
{

}
void on_padding_property_dialog_helpbutton_clicked
	(GtkButton * button, gpointer user_data)
{

}
void on_outline_property_dialog_helpbutton_clicked
	(GtkButton * button, gpointer user_data)
{

}
void on_pause_property_dialog_helpbutton_clicked
	(GtkButton * button, gpointer user_data)
{

}
void on_percentage_value_dialog_helpbutton_clicked
	(GtkButton * button, gpointer user_data)
{

}
void on_percentage_length_value_dialog_helpbutton_clicked
	(GtkButton * button, gpointer user_data)
{

}
void on_play_during_property_dialog_helpbutton_clicked
	(GtkButton * button, gpointer user_data)
{

}
void on_quotes_property_dialog_helpbutton_clicked
	(GtkButton * button, gpointer user_data)
{

}
void on_shape_value_dialog_helpbutton_clicked
	(GtkButton * button, gpointer user_data)
{

}
void on_size_lenght_value_dialog_helpbutton_clicked
	(GtkButton * button, gpointer user_data)
{

}
void on_border_width_style_color_value_dialog_helpbutton_clicked
	(GtkButton * button, gpointer user_data)
{

}
void on_color_selector_dialog_helpbutton_clicked
	(GtkButton * button, gpointer user_data)
{

}
void on_identifier_value_helpbutton_clicked
	(GtkButton * button, gpointer user_data)
{

}

void on_border_style_dialog_helpbutton_clicked
	(GtkButton * button, gpointer user_data)
{

}

