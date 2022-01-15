/*  cssed (c) Iago Rubio 2003, 2004 - A tiny CSS editor.
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

#include "cssdialogs-strings.h"
#include "cssedwindow.h"
#include "document.h"
#include "cssdialogs-callbacks.h"
#include "cssdialogs-interface.h"
#include "support.h"

#define GLADE_HOOKUP_OBJECT(component,widget,name) \
  g_object_set_data_full (G_OBJECT (component), name, \
    gtk_widget_ref (widget), (GDestroyNotify) gtk_widget_unref)

#define GLADE_HOOKUP_OBJECT_NO_REF(component,widget,name) \
  g_object_set_data (G_OBJECT (component), name, widget)

GtkWidget *
create_angle_property_dialog (CssedWindow * window,
							  gchar * previous_string,
							  gboolean add_previous_string,
							  gboolean connect_callbacks)
{
	GtkWidget *angle_property_dialog;
	GtkWidget *dialog_vbox;
	GtkWidget *hbox;
	GtkWidget *vbox_left;
	GtkWidget *label_angle;
	GtkWidget *label_unit;
	GtkWidget *vbox_right;
	GtkWidget *entry_angle;
	GtkWidget *combo_unit;
	GList *combo_unit_items = NULL;
	GtkWidget *combo_entry_units;
	GtkWidget *dialog_action_area;
	GtkWidget *helpbutton;
	GtkWidget *cancelbutton;
	GtkWidget *okbutton;
	GtkAccelGroup *accel_group;
	GtkWidget *main_window;
	CssedCssDialogsData *data = NULL;

	if (connect_callbacks)
	{
		data = g_malloc (sizeof (CssedCssDialogsData));
		data->main_window = window;
		data->previous_string = previous_string;
		data->add_property_string = add_previous_string;
	}
	accel_group = gtk_accel_group_new ();

	angle_property_dialog = gtk_dialog_new ();
	gtk_window_set_title (GTK_WINDOW (angle_property_dialog),
			      previous_string);
	gtk_window_set_modal (GTK_WINDOW (angle_property_dialog), TRUE);

	main_window = cssed_window_get_window_widget( window );
	gtk_window_set_transient_for (GTK_WINDOW (angle_property_dialog),
				      GTK_WINDOW (main_window));

	dialog_vbox = GTK_DIALOG (angle_property_dialog)->vbox;
	gtk_widget_show (dialog_vbox);

	hbox = gtk_hbox_new (FALSE, 0);
	gtk_widget_show (hbox);
	gtk_box_pack_start (GTK_BOX (dialog_vbox), hbox, TRUE, TRUE, 0);

	vbox_left = gtk_vbox_new (TRUE, 0);
	gtk_widget_show (vbox_left);
	gtk_box_pack_start (GTK_BOX (hbox), vbox_left, TRUE, TRUE, 0);

	label_angle = gtk_label_new (_("Angle"));
	gtk_widget_show (label_angle);
	gtk_box_pack_start (GTK_BOX (vbox_left), label_angle, TRUE, FALSE, 0);
	gtk_label_set_justify (GTK_LABEL (label_angle), GTK_JUSTIFY_LEFT);

	label_unit = gtk_label_new (_("Unit"));
	gtk_widget_show (label_unit);
	gtk_box_pack_start (GTK_BOX (vbox_left), label_unit, TRUE, FALSE, 0);
	gtk_label_set_justify (GTK_LABEL (label_unit), GTK_JUSTIFY_LEFT);

	vbox_right = gtk_vbox_new (TRUE, 0);
	gtk_widget_show (vbox_right);
	gtk_box_pack_start (GTK_BOX (hbox), vbox_right, TRUE, FALSE, 0);

	entry_angle = gtk_entry_new ();
	gtk_widget_show (entry_angle);
	gtk_box_pack_start (GTK_BOX (vbox_right), entry_angle, FALSE, FALSE,
			    0);

	combo_unit = gtk_combo_new ();
	g_object_set_data (G_OBJECT (GTK_COMBO (combo_unit)->popwin),
			   "GladeParentKey", combo_unit);
	gtk_widget_show (combo_unit);
	gtk_box_pack_start (GTK_BOX (vbox_right), combo_unit, FALSE, FALSE,
			    0);
	combo_unit_items = g_list_append (combo_unit_items, (gpointer) "deg");
	combo_unit_items = g_list_append (combo_unit_items, (gpointer) "gra");
	combo_unit_items = g_list_append (combo_unit_items, (gpointer) "rad");
	gtk_combo_set_popdown_strings (GTK_COMBO (combo_unit), combo_unit_items);
	g_list_free (combo_unit_items);

	combo_entry_units = GTK_COMBO (combo_unit)->entry;
	gtk_widget_show (combo_entry_units);
	gtk_editable_set_editable (GTK_EDITABLE (combo_entry_units), FALSE);
	gtk_entry_set_text (GTK_ENTRY (combo_entry_units), "deg");

	dialog_action_area = GTK_DIALOG (angle_property_dialog)->action_area;
	gtk_widget_show (dialog_action_area);
	gtk_button_box_set_layout (GTK_BUTTON_BOX (dialog_action_area),
				   GTK_BUTTONBOX_END);

	helpbutton = gtk_button_new_from_stock ("gtk-help");
	gtk_widget_show (helpbutton);
	gtk_dialog_add_action_widget (GTK_DIALOG (angle_property_dialog),
				      helpbutton, GTK_RESPONSE_HELP);
	GTK_WIDGET_SET_FLAGS (helpbutton, GTK_CAN_DEFAULT);
	gtk_widget_add_accelerator (helpbutton, "clicked", accel_group,
				    GDK_h, GDK_CONTROL_MASK,
				    GTK_ACCEL_VISIBLE);
	gtk_widget_add_accelerator (helpbutton, "clicked", accel_group, GDK_y,
				    GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);

	cancelbutton = gtk_button_new_from_stock ("gtk-cancel");
	gtk_widget_show (cancelbutton);
	gtk_dialog_add_action_widget (GTK_DIALOG (angle_property_dialog),
				      cancelbutton, GTK_RESPONSE_CANCEL);
	GTK_WIDGET_SET_FLAGS (cancelbutton, GTK_CAN_DEFAULT);
	gtk_widget_add_accelerator (cancelbutton, "clicked", accel_group,
				    GDK_C, GDK_CONTROL_MASK,
				    GTK_ACCEL_VISIBLE);

	okbutton = gtk_button_new_from_stock ("gtk-ok");
	gtk_widget_show (okbutton);
	gtk_dialog_add_action_widget (GTK_DIALOG (angle_property_dialog),
				      okbutton, GTK_RESPONSE_OK);
	GTK_WIDGET_SET_FLAGS (okbutton, GTK_CAN_DEFAULT);
	gtk_widget_add_accelerator (okbutton, "clicked", accel_group,
				    GDK_o, GDK_CONTROL_MASK,
				    GTK_ACCEL_VISIBLE);

	if (connect_callbacks)
	{
		g_signal_connect ((gpointer) angle_property_dialog, "close",
				  G_CALLBACK (on_css_dialog_close), data);

		g_signal_connect ((gpointer) helpbutton, "clicked",
				  G_CALLBACK
				  (on_angle_property_dialog_helpbutton_clicked),
				  NULL);
		g_signal_connect ((gpointer) cancelbutton, "clicked",
				  G_CALLBACK
				  (on_css_dialog_cancel_button_clicked),
				  data);
		g_signal_connect ((gpointer) okbutton, "clicked",
				  G_CALLBACK
				  (on_angle_property_dialog_ok_button_clicked),
				  data);
	}

	GLADE_HOOKUP_OBJECT_NO_REF (angle_property_dialog,
				    angle_property_dialog,
				    "angle_property_dialog");
	GLADE_HOOKUP_OBJECT_NO_REF (angle_property_dialog, dialog_vbox,
				    "dialog_vbox");
	GLADE_HOOKUP_OBJECT (angle_property_dialog, hbox, "hbox");
	GLADE_HOOKUP_OBJECT (angle_property_dialog, vbox_left, "vbox_left");
	GLADE_HOOKUP_OBJECT (angle_property_dialog, label_angle,
			     "label_angle");
	GLADE_HOOKUP_OBJECT (angle_property_dialog, label_unit, "label_unit");
	GLADE_HOOKUP_OBJECT (angle_property_dialog, vbox_right, "vbox_right");
	GLADE_HOOKUP_OBJECT (angle_property_dialog, entry_angle,
			     "entry_angle");
	GLADE_HOOKUP_OBJECT (angle_property_dialog, combo_unit, "combo_unit");
	GLADE_HOOKUP_OBJECT (angle_property_dialog, combo_entry_units,
			     "combo_entry_units");
	GLADE_HOOKUP_OBJECT_NO_REF (angle_property_dialog, dialog_action_area,
				    "dialog_action_area");
	GLADE_HOOKUP_OBJECT (angle_property_dialog, helpbutton, "helpbutton");
	GLADE_HOOKUP_OBJECT (angle_property_dialog, cancelbutton,
			     "cancelbutton");
	GLADE_HOOKUP_OBJECT (angle_property_dialog, okbutton, "okbutton");

	gtk_window_add_accel_group (GTK_WINDOW (angle_property_dialog),
				    accel_group);

	return angle_property_dialog;
}

GtkWidget *
create_background_property_dialog (CssedWindow * window,
								   gchar * previous_string,
								   gboolean add_previous_string,
								   gboolean connect_callbacks)
{
	GtkWidget *background_property_dialog;
	GtkWidget *dialog_vbox_main;
	GtkWidget *vbox_main;
	GtkWidget *colorselection;
	GtkWidget *hbox_bg_color;
	GtkWidget *checkbutton_bg_color;
	GtkWidget *combo_bg_color;
	GList *combo_bg_color_items = NULL;
	GtkWidget *combo_entry_bg_color;
	GtkWidget *hbox_bg_position;
	GtkWidget *checkbutton_bg_position;
	GtkWidget *hbox_bg_position_radio;
	GtkWidget *radiobutton_bg_pos_fixed;
	GSList *radiobutton_bg_pos_fixed_group = NULL;
	GtkWidget *combo_bg_pos_fixed;
	GList *combo_bg_pos_fixed_items = NULL;
	GtkWidget *combo_entry_bg_pos_fixed;
	GtkWidget *hbox_bg_pclen_entries_upper;
	GtkWidget *hbox_bg_perclen_entries_top;
	GtkWidget *radiobutton_bg_pos_perclen;
	GtkWidget *label_uper_left;
	GtkObject *spinbutton_upper_left_adj;
	GtkWidget *spinbutton_upper_left;
	GtkWidget *label_lower_right;
	GtkObject *spinbutton_lower_right_adj;
	GtkWidget *spinbutton_lower_right;
	GtkWidget *combo_unit;
	GList *combo_unit_items = NULL;
	GtkWidget *combo_entry_unit;
	GtkWidget *hbox_bg_image;
	GtkWidget *checkbutton_bg_image;
	GtkWidget *label_bg_image_uri;
	GtkWidget *entry_uri;
	GtkWidget *hbox_bg_repeat;
	GtkWidget *checkbutton_bg_repeat;
	GtkWidget *combo_bg_repeat;
	GList *combo_bg_repeat_items = NULL;
	GtkWidget *combo_entry_bg_repeat;
	GtkWidget *hbox_bg_attachment;
	GtkWidget *checkbutton_bg_attachment;
	GtkWidget *combo_bg_attachment;
	GList *combo_bg_attachment_items = NULL;
	GtkWidget *combo_entry_bg_attachment;
	GtkWidget *dialog_action_area;
	GtkWidget *helpbutton;
	GtkWidget *cancelbutton;
	GtkWidget *okbutton;
	GtkAccelGroup *accel_group;
	GtkWidget *main_window;
	CssedCssDialogsData *data = NULL;

	if (connect_callbacks)
	{
		data = g_malloc (sizeof (CssedCssDialogsData));
		data->main_window = window;
		data->previous_string = previous_string;
		data->add_property_string = add_previous_string;
	}

	accel_group = gtk_accel_group_new ();

	background_property_dialog = gtk_dialog_new ();
	gtk_window_set_title (GTK_WINDOW (background_property_dialog),
			      previous_string);
	gtk_window_set_modal (GTK_WINDOW (background_property_dialog), TRUE);

	main_window = cssed_window_get_window_widget( window );
	gtk_window_set_transient_for (GTK_WINDOW (background_property_dialog),
				      GTK_WINDOW (main_window));

	dialog_vbox_main = GTK_DIALOG (background_property_dialog)->vbox;
	gtk_widget_show (dialog_vbox_main);

	vbox_main = gtk_vbox_new (FALSE, 0);
	gtk_widget_show (vbox_main);
	gtk_box_pack_start (GTK_BOX (dialog_vbox_main), vbox_main, FALSE,
			    FALSE, 5);

	colorselection = gtk_color_selection_new ();
	gtk_widget_show (colorselection);
	gtk_box_pack_start (GTK_BOX (vbox_main), colorselection, FALSE, FALSE,
			    5);
	gtk_container_set_border_width (GTK_CONTAINER (colorselection), 10);
	gtk_color_selection_set_has_opacity_control (GTK_COLOR_SELECTION
						     (colorselection), FALSE);
	gtk_color_selection_set_has_palette (GTK_COLOR_SELECTION
					     (colorselection), TRUE);
	hbox_bg_color = gtk_hbox_new (FALSE, 0);
	gtk_widget_show (hbox_bg_color);
	gtk_box_pack_start (GTK_BOX (vbox_main), hbox_bg_color, FALSE, FALSE,
			    10);

	checkbutton_bg_color =
		gtk_check_button_new_with_mnemonic (_("Background color"));
	gtk_widget_show (checkbutton_bg_color);
	gtk_box_pack_start (GTK_BOX (hbox_bg_color), checkbutton_bg_color,
			    FALSE, FALSE, 5);

	combo_bg_color = gtk_combo_new ();
	g_object_set_data (G_OBJECT (GTK_COMBO (combo_bg_color)->popwin),
			   "GladeParentKey", combo_bg_color);
	gtk_widget_show (combo_bg_color);
	gtk_box_pack_start (GTK_BOX (hbox_bg_color), combo_bg_color, FALSE,
			    FALSE, 5);
	combo_bg_color_items =
		g_list_append (combo_bg_color_items,
			       (gpointer) CSS_STR_USE_COLORSEL);
	combo_bg_color_items =
		g_list_append (combo_bg_color_items, (gpointer) "aqua");
	combo_bg_color_items =
		g_list_append (combo_bg_color_items, (gpointer) "black");
	combo_bg_color_items =
		g_list_append (combo_bg_color_items, (gpointer) "blue");
	combo_bg_color_items =
		g_list_append (combo_bg_color_items, (gpointer) "fuchsia");
	combo_bg_color_items =
		g_list_append (combo_bg_color_items, (gpointer) "gray");
	combo_bg_color_items =
		g_list_append (combo_bg_color_items, (gpointer) "green");
	combo_bg_color_items =
		g_list_append (combo_bg_color_items, (gpointer) "lime");
	combo_bg_color_items =
		g_list_append (combo_bg_color_items, (gpointer) "maroon");
	combo_bg_color_items =
		g_list_append (combo_bg_color_items, (gpointer) "navy");
	combo_bg_color_items =
		g_list_append (combo_bg_color_items, (gpointer) "olive");
	combo_bg_color_items =
		g_list_append (combo_bg_color_items, (gpointer) "purple");
	combo_bg_color_items =
		g_list_append (combo_bg_color_items, (gpointer) "red");
	combo_bg_color_items =
		g_list_append (combo_bg_color_items, (gpointer) "silver");
	combo_bg_color_items =
		g_list_append (combo_bg_color_items, (gpointer) "teal");
	combo_bg_color_items =
		g_list_append (combo_bg_color_items, (gpointer) "white");
	combo_bg_color_items =
		g_list_append (combo_bg_color_items, (gpointer) "yellow");
	combo_bg_color_items =
		g_list_append (combo_bg_color_items,
			       (gpointer) "ActiveBorder");
	combo_bg_color_items =
		g_list_append (combo_bg_color_items,
			       (gpointer) "ActiveCaption");
	combo_bg_color_items =
		g_list_append (combo_bg_color_items,
			       (gpointer) "AppWorkspace");
	combo_bg_color_items =
		g_list_append (combo_bg_color_items, (gpointer) "Background");
	combo_bg_color_items =
		g_list_append (combo_bg_color_items, (gpointer) "ButtonFace");
	combo_bg_color_items =
		g_list_append (combo_bg_color_items,
			       (gpointer) "ButtonHighlight");
	combo_bg_color_items =
		g_list_append (combo_bg_color_items,
			       (gpointer) "ButtonShadow");
	combo_bg_color_items =
		g_list_append (combo_bg_color_items, (gpointer) "ButtonText");
	combo_bg_color_items =
		g_list_append (combo_bg_color_items,
			       (gpointer) "CaptionText");
	combo_bg_color_items =
		g_list_append (combo_bg_color_items, (gpointer) "GrayText");
	combo_bg_color_items =
		g_list_append (combo_bg_color_items, (gpointer) "Highlight");
	combo_bg_color_items =
		g_list_append (combo_bg_color_items,
			       (gpointer) "HighlightText");
	combo_bg_color_items =
		g_list_append (combo_bg_color_items,
			       (gpointer) "InactiveBorder");
	combo_bg_color_items =
		g_list_append (combo_bg_color_items,
			       (gpointer) "InactiveCaption");
	combo_bg_color_items =
		g_list_append (combo_bg_color_items,
			       (gpointer) "InactiveCaptionText");
	combo_bg_color_items =
		g_list_append (combo_bg_color_items,
			       (gpointer) "InfoBackground");
	combo_bg_color_items =
		g_list_append (combo_bg_color_items, (gpointer) "InfoText");
	combo_bg_color_items =
		g_list_append (combo_bg_color_items, (gpointer) "Menu");
	combo_bg_color_items =
		g_list_append (combo_bg_color_items, (gpointer) "MenuText");
	combo_bg_color_items =
		g_list_append (combo_bg_color_items, (gpointer) "Scrollbar");
	combo_bg_color_items =
		g_list_append (combo_bg_color_items,
			       (gpointer) "ThreeDDarkShadow");
	combo_bg_color_items =
		g_list_append (combo_bg_color_items, (gpointer) "ThreeDFace");
	combo_bg_color_items =
		g_list_append (combo_bg_color_items,
			       (gpointer) "ThreeDHighlight");
	combo_bg_color_items =
		g_list_append (combo_bg_color_items,
			       (gpointer) "ThreeDLightShadow");
	combo_bg_color_items =
		g_list_append (combo_bg_color_items,
			       (gpointer) "ThreeDShadow");
	combo_bg_color_items =
		g_list_append (combo_bg_color_items, (gpointer) "Window");
	combo_bg_color_items =
		g_list_append (combo_bg_color_items,
			       (gpointer) "WindowFrame");
	combo_bg_color_items =
		g_list_append (combo_bg_color_items, (gpointer) "WindowText");
	combo_bg_color_items =
		g_list_append (combo_bg_color_items,
			       (gpointer) "transparent");
	combo_bg_color_items =
		g_list_append (combo_bg_color_items, (gpointer) "");
	gtk_combo_set_popdown_strings (GTK_COMBO (combo_bg_color),
				       combo_bg_color_items);
	g_list_free (combo_bg_color_items);

	combo_entry_bg_color = GTK_COMBO (combo_bg_color)->entry;
	gtk_widget_show (combo_entry_bg_color);
	gtk_entry_set_text (GTK_ENTRY (combo_entry_bg_color),
			    CSS_STR_USE_COLORSEL);

	hbox_bg_position = gtk_hbox_new (FALSE, 0);
	gtk_widget_show (hbox_bg_position);
	gtk_box_pack_start (GTK_BOX (vbox_main), hbox_bg_position, FALSE,
			    FALSE, 5);

	checkbutton_bg_position =
		gtk_check_button_new_with_mnemonic (_("Background position"));
	gtk_widget_show (checkbutton_bg_position);
	gtk_box_pack_start (GTK_BOX (hbox_bg_position),
			    checkbutton_bg_position, FALSE, FALSE, 5);

	hbox_bg_position_radio = gtk_hbox_new (FALSE, 0);
	gtk_widget_show (hbox_bg_position_radio);
	gtk_box_pack_start (GTK_BOX (vbox_main), hbox_bg_position_radio,
			    FALSE, FALSE, 5);

	radiobutton_bg_pos_fixed =
		gtk_radio_button_new_with_mnemonic (NULL, _("Fixed value"));
	gtk_widget_show (radiobutton_bg_pos_fixed);
	gtk_box_pack_start (GTK_BOX (hbox_bg_position_radio),
			    radiobutton_bg_pos_fixed, TRUE, TRUE, 0);
	gtk_radio_button_set_group (GTK_RADIO_BUTTON
				    (radiobutton_bg_pos_fixed),
				    radiobutton_bg_pos_fixed_group);
	radiobutton_bg_pos_fixed_group =
		gtk_radio_button_get_group (GTK_RADIO_BUTTON
					    (radiobutton_bg_pos_fixed));

	combo_bg_pos_fixed = gtk_combo_new ();
	g_object_set_data (G_OBJECT (GTK_COMBO (combo_bg_pos_fixed)->popwin),
			   "GladeParentKey", combo_bg_pos_fixed);
	gtk_widget_show (combo_bg_pos_fixed);
	gtk_box_pack_start (GTK_BOX (hbox_bg_position_radio),
			    combo_bg_pos_fixed, TRUE, TRUE, 5);
	combo_bg_pos_fixed_items =
		g_list_append (combo_bg_pos_fixed_items,
			       (gpointer) "top left");
	combo_bg_pos_fixed_items =
		g_list_append (combo_bg_pos_fixed_items,
			       (gpointer) "top right");
	combo_bg_pos_fixed_items =
		g_list_append (combo_bg_pos_fixed_items,
			       (gpointer) "bottom left");
	combo_bg_pos_fixed_items =
		g_list_append (combo_bg_pos_fixed_items,
			       (gpointer) "bottom right");
	combo_bg_pos_fixed_items =
		g_list_append (combo_bg_pos_fixed_items, (gpointer) "center");
	gtk_combo_set_popdown_strings (GTK_COMBO (combo_bg_pos_fixed),
				       combo_bg_pos_fixed_items);
	g_list_free (combo_bg_pos_fixed_items);

	combo_entry_bg_pos_fixed = GTK_COMBO (combo_bg_pos_fixed)->entry;
	gtk_widget_show (combo_entry_bg_pos_fixed);
	gtk_widget_set_size_request (combo_entry_bg_pos_fixed, 100, -1);
	gtk_entry_set_text (GTK_ENTRY (combo_entry_bg_pos_fixed), "top left");

	hbox_bg_pclen_entries_upper = gtk_hbox_new (FALSE, 0);
	gtk_widget_show (hbox_bg_pclen_entries_upper);
	gtk_box_pack_start (GTK_BOX (vbox_main), hbox_bg_pclen_entries_upper,
			    FALSE, FALSE, 0);

	hbox_bg_perclen_entries_top = gtk_hbox_new (FALSE, 0);
	gtk_widget_show (hbox_bg_perclen_entries_top);
	gtk_box_pack_start (GTK_BOX (hbox_bg_pclen_entries_upper),
			    hbox_bg_perclen_entries_top, TRUE, TRUE, 0);

	radiobutton_bg_pos_perclen =
		gtk_radio_button_new_with_mnemonic (NULL,
						    _
						    ("Percentage or length"));
	gtk_widget_show (radiobutton_bg_pos_perclen);
	gtk_box_pack_start (GTK_BOX (hbox_bg_perclen_entries_top),
			    radiobutton_bg_pos_perclen, FALSE, FALSE, 0);
	gtk_radio_button_set_group (GTK_RADIO_BUTTON
				    (radiobutton_bg_pos_perclen),
				    radiobutton_bg_pos_fixed_group);

	label_uper_left = gtk_label_new (_("Upper left"));
	gtk_widget_show (label_uper_left);
	gtk_box_pack_start (GTK_BOX (hbox_bg_perclen_entries_top),
			    label_uper_left, TRUE, TRUE, 0);
	gtk_label_set_justify (GTK_LABEL (label_uper_left), GTK_JUSTIFY_LEFT);

	spinbutton_upper_left_adj =
		gtk_adjustment_new (1, -35565, 35565, 1, 10, 10);
	spinbutton_upper_left =
		gtk_spin_button_new (GTK_ADJUSTMENT
				     (spinbutton_upper_left_adj), 1, 0);
	gtk_widget_show (spinbutton_upper_left);
	gtk_box_pack_start (GTK_BOX (hbox_bg_perclen_entries_top),
			    spinbutton_upper_left, FALSE, FALSE, 0);

	label_lower_right = gtk_label_new (_("Lower right"));
	gtk_widget_show (label_lower_right);
	gtk_box_pack_start (GTK_BOX (hbox_bg_pclen_entries_upper),
			    label_lower_right, TRUE, TRUE, 0);
	gtk_label_set_justify (GTK_LABEL (label_lower_right),
			       GTK_JUSTIFY_LEFT);

	spinbutton_lower_right_adj = gtk_adjustment_new (1, -35565, 35565, 1, 10, 10);
	spinbutton_lower_right = gtk_spin_button_new (GTK_ADJUSTMENT(spinbutton_lower_right_adj), 1, 0);
	gtk_widget_show (spinbutton_lower_right);
	gtk_box_pack_start (GTK_BOX (hbox_bg_pclen_entries_upper), spinbutton_lower_right, FALSE, FALSE, 0);

	combo_unit = gtk_combo_new ();
	g_object_set_data (G_OBJECT (GTK_COMBO (combo_unit)->popwin),  "GladeParentKey", combo_unit);
	gtk_widget_show (combo_unit);
	gtk_box_pack_start (GTK_BOX (hbox_bg_pclen_entries_upper), combo_unit, FALSE, FALSE, 5);
	combo_unit_items = g_list_append (combo_unit_items, (gpointer) "%");
	combo_unit_items = g_list_append (combo_unit_items, (gpointer) "em");
	combo_unit_items = g_list_append (combo_unit_items, (gpointer) "ex");
	combo_unit_items = g_list_append (combo_unit_items, (gpointer) "px");
	combo_unit_items = g_list_append (combo_unit_items, (gpointer) "in");
	combo_unit_items = g_list_append (combo_unit_items, (gpointer) "cm");
	combo_unit_items = g_list_append (combo_unit_items, (gpointer) "mm");
	combo_unit_items = g_list_append (combo_unit_items, (gpointer) "pt");
	combo_unit_items = g_list_append (combo_unit_items, (gpointer) "pc");

	gtk_combo_set_popdown_strings (GTK_COMBO (combo_unit),	combo_unit_items);
	g_list_free (combo_unit_items);

	combo_entry_unit = GTK_COMBO (combo_unit)->entry;
	gtk_widget_show (combo_entry_unit);
	gtk_widget_set_size_request (combo_entry_unit, 50, -1);
	gtk_editable_set_editable (GTK_EDITABLE (combo_entry_unit), FALSE);
	gtk_entry_set_text (GTK_ENTRY (combo_entry_unit), "px");

	hbox_bg_image = gtk_hbox_new (FALSE, 0);
	gtk_widget_show (hbox_bg_image);
	gtk_box_pack_start (GTK_BOX (vbox_main), hbox_bg_image, FALSE, FALSE,
			    5);

	checkbutton_bg_image =
		gtk_check_button_new_with_mnemonic (_("Background image"));
	gtk_widget_show (checkbutton_bg_image);
	gtk_box_pack_start (GTK_BOX (hbox_bg_image), checkbutton_bg_image,
			    FALSE, FALSE, 5);

	label_bg_image_uri = gtk_label_new (_("Enter uri of image"));
	gtk_widget_show (label_bg_image_uri);
	gtk_box_pack_start (GTK_BOX (hbox_bg_image), label_bg_image_uri, TRUE,
			    TRUE, 0);
	gtk_label_set_justify (GTK_LABEL (label_bg_image_uri),
			       GTK_JUSTIFY_LEFT);

	entry_uri = gtk_entry_new ();
	gtk_widget_show (entry_uri);
	gtk_box_pack_start (GTK_BOX (hbox_bg_image), entry_uri, TRUE, TRUE,
			    5);

	hbox_bg_repeat = gtk_hbox_new (FALSE, 0);
	gtk_widget_show (hbox_bg_repeat);
	gtk_box_pack_start (GTK_BOX (vbox_main), hbox_bg_repeat, FALSE, FALSE,
			    5);

	checkbutton_bg_repeat =
		gtk_check_button_new_with_mnemonic (_("Background repeat"));
	gtk_widget_show (checkbutton_bg_repeat);
	gtk_box_pack_start (GTK_BOX (hbox_bg_repeat), checkbutton_bg_repeat,
			    TRUE, TRUE, 5);

	combo_bg_repeat = gtk_combo_new ();
	g_object_set_data (G_OBJECT (GTK_COMBO (combo_bg_repeat)->popwin),
			   "GladeParentKey", combo_bg_repeat);
	gtk_widget_show (combo_bg_repeat);
	gtk_box_pack_start (GTK_BOX (hbox_bg_repeat), combo_bg_repeat, FALSE,
			    FALSE, 5);
	combo_bg_repeat_items =
		g_list_append (combo_bg_repeat_items, (gpointer) "repeat");
	combo_bg_repeat_items =
		g_list_append (combo_bg_repeat_items, (gpointer) "repeat-x");
	combo_bg_repeat_items =
		g_list_append (combo_bg_repeat_items, (gpointer) "repeat-y");
	combo_bg_repeat_items =
		g_list_append (combo_bg_repeat_items, (gpointer) "no-repeat");
	gtk_combo_set_popdown_strings (GTK_COMBO (combo_bg_repeat),
				       combo_bg_repeat_items);
	g_list_free (combo_bg_repeat_items);

	combo_entry_bg_repeat = GTK_COMBO (combo_bg_repeat)->entry;
	gtk_widget_show (combo_entry_bg_repeat);
	gtk_editable_set_editable (GTK_EDITABLE (combo_entry_bg_repeat),
				   FALSE);
	gtk_entry_set_text (GTK_ENTRY (combo_entry_bg_repeat), "repeat");

	hbox_bg_attachment = gtk_hbox_new (FALSE, 0);
	gtk_widget_show (hbox_bg_attachment);
	gtk_box_pack_start (GTK_BOX (vbox_main), hbox_bg_attachment, FALSE,
			    FALSE, 5);

	checkbutton_bg_attachment =
		gtk_check_button_new_with_mnemonic (_
						    ("Background attachment"));
	gtk_widget_show (checkbutton_bg_attachment);
	gtk_box_pack_start (GTK_BOX (hbox_bg_attachment),
			    checkbutton_bg_attachment, TRUE, TRUE, 5);

	combo_bg_attachment = gtk_combo_new ();
	g_object_set_data (G_OBJECT (GTK_COMBO (combo_bg_attachment)->popwin),
			   "GladeParentKey", combo_bg_attachment);
	gtk_widget_show (combo_bg_attachment);
	gtk_box_pack_start (GTK_BOX (hbox_bg_attachment), combo_bg_attachment,
			    FALSE, FALSE, 5);
	combo_bg_attachment_items =
		g_list_append (combo_bg_attachment_items,
			       (gpointer) "scroll");
	combo_bg_attachment_items =
		g_list_append (combo_bg_attachment_items, (gpointer) "fixed");
	gtk_combo_set_popdown_strings (GTK_COMBO (combo_bg_attachment),
				       combo_bg_attachment_items);
	g_list_free (combo_bg_attachment_items);

	combo_entry_bg_attachment = GTK_COMBO (combo_bg_attachment)->entry;
	gtk_widget_show (combo_entry_bg_attachment);
	gtk_editable_set_editable (GTK_EDITABLE (combo_entry_bg_attachment),
				   FALSE);
	gtk_entry_set_text (GTK_ENTRY (combo_entry_bg_attachment), "scroll");

	dialog_action_area =
		GTK_DIALOG (background_property_dialog)->action_area;
	gtk_widget_show (dialog_action_area);
	gtk_button_box_set_layout (GTK_BUTTON_BOX (dialog_action_area),
				   GTK_BUTTONBOX_END);

	helpbutton = gtk_button_new_from_stock ("gtk-help");
	gtk_widget_show (helpbutton);
	gtk_dialog_add_action_widget (GTK_DIALOG (background_property_dialog),
				      helpbutton, GTK_RESPONSE_HELP);
	GTK_WIDGET_SET_FLAGS (helpbutton, GTK_CAN_DEFAULT);
	gtk_widget_add_accelerator (helpbutton, "clicked", accel_group,
				    GDK_y, GDK_CONTROL_MASK,
				    GTK_ACCEL_VISIBLE);
	gtk_widget_add_accelerator (helpbutton, "clicked", accel_group, GDK_h,
				    GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);

	cancelbutton = gtk_button_new_from_stock ("gtk-cancel");
	gtk_widget_show (cancelbutton);
	gtk_dialog_add_action_widget (GTK_DIALOG (background_property_dialog),
				      cancelbutton, GTK_RESPONSE_CANCEL);
	GTK_WIDGET_SET_FLAGS (cancelbutton, GTK_CAN_DEFAULT);
	gtk_widget_add_accelerator (cancelbutton, "clicked", accel_group,
				    GDK_C, GDK_CONTROL_MASK,
				    GTK_ACCEL_VISIBLE);

	okbutton = gtk_button_new_from_stock ("gtk-ok");
	gtk_widget_show (okbutton);
	gtk_dialog_add_action_widget (GTK_DIALOG (background_property_dialog),
				      okbutton, GTK_RESPONSE_OK);
	GTK_WIDGET_SET_FLAGS (okbutton, GTK_CAN_DEFAULT);
	gtk_widget_add_accelerator (okbutton, "clicked", accel_group,
				    GDK_o, GDK_CONTROL_MASK,
				    GTK_ACCEL_VISIBLE);

	/*  to update the spin button to the unit selected */
	g_signal_connect ((gpointer) combo_entry_unit,
			  "changed", G_CALLBACK (on_css_dialog_unit_changed),
			  spinbutton_upper_left);
	g_signal_connect ((gpointer) combo_entry_unit,
			  "changed", G_CALLBACK (on_css_dialog_unit_changed),
			  spinbutton_lower_right);
			  
	/* to update the checkboxes when a related widget is changed */
	g_signal_connect ((gpointer) colorselection,
			  "color-changed", G_CALLBACK (on_widget_change_set_toggle_active),
			  checkbutton_bg_color);
	g_signal_connect ((gpointer) combo_entry_bg_color,
			  "changed", G_CALLBACK (on_widget_change_set_toggle_active),
			  checkbutton_bg_color);
			  
	g_signal_connect ((gpointer) radiobutton_bg_pos_fixed,
			  "toggled", G_CALLBACK (on_widget_change_set_toggle_active),
			  checkbutton_bg_position);
			  
	g_signal_connect ((gpointer) combo_entry_bg_pos_fixed,
			  "changed", G_CALLBACK (on_widget_change_set_toggle_active),
			  checkbutton_bg_position);
	g_signal_connect ((gpointer) radiobutton_bg_pos_perclen,
			  "toggled", G_CALLBACK (on_widget_change_set_toggle_active),
			  checkbutton_bg_position);	
			  
	g_signal_connect ((gpointer) spinbutton_upper_left,
			  "value-changed", G_CALLBACK (on_widget_change_set_toggle_active),
			  checkbutton_bg_position);				  		  			  			  	
	g_signal_connect ((gpointer) spinbutton_lower_right,
			  "value-changed", G_CALLBACK (on_widget_change_set_toggle_active),
			  checkbutton_bg_position);				  		  			  			  	
	g_signal_connect ((gpointer) combo_entry_unit,
			  "changed", G_CALLBACK (on_widget_change_set_toggle_active),
			  checkbutton_bg_position);		
			  
	g_signal_connect ((gpointer) combo_entry_bg_pos_fixed,
			  "changed", G_CALLBACK (on_widget_change_set_toggle_active),
			  radiobutton_bg_pos_fixed);
			  
	g_signal_connect ((gpointer) spinbutton_upper_left,
			  "value-changed", G_CALLBACK (on_widget_change_set_toggle_active),
			  radiobutton_bg_pos_perclen);				  		  			  			  	
	g_signal_connect ((gpointer) spinbutton_lower_right,
			  "value-changed", G_CALLBACK (on_widget_change_set_toggle_active),
			  radiobutton_bg_pos_perclen);				  		  			  			  	
	g_signal_connect ((gpointer) combo_entry_unit,
			  "changed", G_CALLBACK (on_widget_change_set_toggle_active),
			  radiobutton_bg_pos_perclen);
			  
			  	
	g_signal_connect ((gpointer) entry_uri,
			  "changed", G_CALLBACK (on_widget_change_set_toggle_active),
			  checkbutton_bg_image);	
			   	
	g_signal_connect ((gpointer) combo_entry_bg_attachment,
			  "changed", G_CALLBACK (on_widget_change_set_toggle_active),
			  checkbutton_bg_attachment);
 	
	g_signal_connect ((gpointer) combo_entry_bg_repeat,
			  "changed", G_CALLBACK (on_widget_change_set_toggle_active),
			  checkbutton_bg_repeat);	
			  	
			  
	if (connect_callbacks)
	{
		g_signal_connect ((gpointer) background_property_dialog,
				  "close", G_CALLBACK (on_css_dialog_close),
				  data);

		g_signal_connect ((gpointer) helpbutton, "clicked",
				  G_CALLBACK
				  (on_background_property_dialog_helpbutton_clicked),
				  NULL);
		g_signal_connect ((gpointer) cancelbutton, "clicked",
				  G_CALLBACK
				  (on_css_dialog_cancel_button_clicked),
				  data);
		g_signal_connect ((gpointer) okbutton, "clicked",
				  G_CALLBACK
				  (on_background_property_dialog_okbutton_clicked),
				  data);
	}

	GLADE_HOOKUP_OBJECT_NO_REF (background_property_dialog,
				    background_property_dialog,
				    "background_property_dialog");
	GLADE_HOOKUP_OBJECT_NO_REF (background_property_dialog,
				    dialog_vbox_main, "dialog_vbox_main");
	GLADE_HOOKUP_OBJECT (background_property_dialog, vbox_main,
			     "vbox_main");
	GLADE_HOOKUP_OBJECT (background_property_dialog, colorselection,
			     "colorselection");
	GLADE_HOOKUP_OBJECT (background_property_dialog, hbox_bg_color,
			     "hbox_bg_color");
	GLADE_HOOKUP_OBJECT (background_property_dialog, checkbutton_bg_color,
			     "checkbutton_bg_color");
	GLADE_HOOKUP_OBJECT (background_property_dialog, combo_bg_color,
			     "combo_bg_color");
	GLADE_HOOKUP_OBJECT (background_property_dialog, combo_entry_bg_color,
			     "combo_entry_bg_color");
	GLADE_HOOKUP_OBJECT (background_property_dialog, hbox_bg_position,
			     "hbox_bg_position");
	GLADE_HOOKUP_OBJECT (background_property_dialog,
			     checkbutton_bg_position,
			     "checkbutton_bg_position");
	GLADE_HOOKUP_OBJECT (background_property_dialog,
			     hbox_bg_position_radio,
			     "hbox_bg_position_radio");
	GLADE_HOOKUP_OBJECT (background_property_dialog,
			     radiobutton_bg_pos_fixed,
			     "radiobutton_bg_pos_fixed");
	GLADE_HOOKUP_OBJECT (background_property_dialog, combo_bg_pos_fixed,
			     "combo_bg_pos_fixed");
	GLADE_HOOKUP_OBJECT (background_property_dialog,
			     combo_entry_bg_pos_fixed,
			     "combo_entry_bg_pos_fixed");
	GLADE_HOOKUP_OBJECT (background_property_dialog,
			     hbox_bg_pclen_entries_upper,
			     "hbox_bg_pclen_entries_upper");
	GLADE_HOOKUP_OBJECT (background_property_dialog,
			     hbox_bg_perclen_entries_top,
			     "hbox_bg_perclen_entries_top");
	GLADE_HOOKUP_OBJECT (background_property_dialog,
			     radiobutton_bg_pos_perclen,
			     "radiobutton_bg_pos_perclen");
	GLADE_HOOKUP_OBJECT (background_property_dialog, label_uper_left,
			     "label_uper_left");
	GLADE_HOOKUP_OBJECT (background_property_dialog,
			     spinbutton_upper_left, "spinbutton_upper_left");
	GLADE_HOOKUP_OBJECT (background_property_dialog, label_lower_right,
			     "label_lower_right");
	GLADE_HOOKUP_OBJECT (background_property_dialog,
			     spinbutton_lower_right,
			     "spinbutton_lower_right");
	GLADE_HOOKUP_OBJECT (background_property_dialog, combo_unit,
			     "combo_unit");
	GLADE_HOOKUP_OBJECT (background_property_dialog, combo_entry_unit,
			     "combo_entry_unit");
	GLADE_HOOKUP_OBJECT (background_property_dialog, hbox_bg_image,
			     "hbox_bg_image");
	GLADE_HOOKUP_OBJECT (background_property_dialog, checkbutton_bg_image,
			     "checkbutton_bg_image");
	GLADE_HOOKUP_OBJECT (background_property_dialog, label_bg_image_uri,
			     "label_bg_image_uri");
	GLADE_HOOKUP_OBJECT (background_property_dialog, entry_uri,
			     "entry_uri");
	GLADE_HOOKUP_OBJECT (background_property_dialog, hbox_bg_repeat,
			     "hbox_bg_repeat");
	GLADE_HOOKUP_OBJECT (background_property_dialog,
			     checkbutton_bg_repeat, "checkbutton_bg_repeat");
	GLADE_HOOKUP_OBJECT (background_property_dialog, combo_bg_repeat,
			     "combo_bg_repeat");
	GLADE_HOOKUP_OBJECT (background_property_dialog,
			     combo_entry_bg_repeat, "combo_entry_bg_repeat");
	GLADE_HOOKUP_OBJECT (background_property_dialog, hbox_bg_attachment,
			     "hbox_bg_attachment");
	GLADE_HOOKUP_OBJECT (background_property_dialog,
			     checkbutton_bg_attachment,
			     "checkbutton_bg_attachment");
	GLADE_HOOKUP_OBJECT (background_property_dialog, combo_bg_attachment,
			     "combo_bg_attachment");
	GLADE_HOOKUP_OBJECT (background_property_dialog,
			     combo_entry_bg_attachment,
			     "combo_entry_bg_attachment");
	GLADE_HOOKUP_OBJECT_NO_REF (background_property_dialog,
				    dialog_action_area, "dialog_action_area");
	GLADE_HOOKUP_OBJECT (background_property_dialog, helpbutton,
			     "helpbutton");
	GLADE_HOOKUP_OBJECT (background_property_dialog, cancelbutton,
			     "cancelbutton");
	GLADE_HOOKUP_OBJECT (background_property_dialog, okbutton,
			     "okbutton");

	gtk_window_add_accel_group (GTK_WINDOW (background_property_dialog),
				    accel_group);

	return background_property_dialog;
}

GtkWidget *
create_border_properties_dialog (CssedWindow * window,
								 gchar * previous_string,
								 gboolean add_previous_string,
								 gboolean connect_callbacks )
{
	GtkWidget *border_properties_dialog;
	GtkWidget *dialog_vbox_main;
	GtkWidget *vbox_main;
	GtkWidget *hbox_border_length;
	GtkWidget *checkbutton_border_length;
	GtkWidget *combo_border_length_selector;
	GList *combo_border_length_selector_items = NULL;
	GtkWidget *combo_entry_border_length_selector;
	GtkObject *spinbutton_len_adj;
	GtkWidget *spinbutton_len;
	GtkWidget *combo_len;
	GList *combo_len_items = NULL;
	GtkWidget *combo_entry_len;
	GtkWidget *hbox_border_style;
	GtkWidget *checkbutton_border_style;
	GtkWidget *combo_border_style;
	GList *combo_border_style_items = NULL;
	GtkWidget *combo_entry_border_style;
	GtkWidget *hbox_border_color;
	GtkWidget *checkbutton_border_color;
	GtkWidget *combo_border_color;
	GList *combo_border_color_items = NULL;
	GtkWidget *combo_entry_border_color;
	GtkWidget *colorselection;
	GtkWidget *dialog_action_area;
	GtkWidget *helpbutton;
	GtkWidget *cancelbutton;
	GtkWidget *okbutton;
	GtkAccelGroup *accel_group;
	GtkWidget *main_window;
	CssedCssDialogsData *data = NULL;

	if (connect_callbacks)
	{
		data = g_malloc (sizeof (CssedCssDialogsData));
		data->main_window = window;
		data->previous_string = previous_string;
		data->add_property_string = add_previous_string;
	}

	accel_group = gtk_accel_group_new ();

	border_properties_dialog = gtk_dialog_new ();
	gtk_window_set_title (GTK_WINDOW (border_properties_dialog), "border");
	gtk_window_set_modal (GTK_WINDOW (border_properties_dialog), TRUE);

	main_window = cssed_window_get_window_widget( window );

	gtk_window_set_transient_for (GTK_WINDOW (border_properties_dialog),
				      GTK_WINDOW (main_window));

	dialog_vbox_main = GTK_DIALOG (border_properties_dialog)->vbox;
	gtk_widget_show (dialog_vbox_main);

	vbox_main = gtk_vbox_new (FALSE, 0);
	gtk_widget_show (vbox_main);
	gtk_box_pack_start (GTK_BOX (dialog_vbox_main), vbox_main, TRUE, TRUE,
			    0);

	hbox_border_length = gtk_hbox_new (FALSE, 0);
	gtk_widget_show (hbox_border_length);
	gtk_box_pack_start (GTK_BOX (vbox_main), hbox_border_length, FALSE,
			    TRUE, 5);

	checkbutton_border_length =
		gtk_check_button_new_with_mnemonic (_("Border length"));
	gtk_widget_show (checkbutton_border_length);
	gtk_box_pack_start (GTK_BOX (hbox_border_length),
			    checkbutton_border_length, TRUE, TRUE, 5);

	combo_border_length_selector = gtk_combo_new ();
	g_object_set_data (G_OBJECT
			   (GTK_COMBO (combo_border_length_selector)->popwin),
			   "GladeParentKey", combo_border_length_selector);
	gtk_widget_show (combo_border_length_selector);
	gtk_box_pack_start (GTK_BOX (hbox_border_length),
			    combo_border_length_selector, FALSE, FALSE, 2);
	combo_border_length_selector_items =
		g_list_append (combo_border_length_selector_items,
			       (gpointer) CSS_STR_USE_LENGTH);
	combo_border_length_selector_items =
		g_list_append (combo_border_length_selector_items,
			       (gpointer) "thin");
	combo_border_length_selector_items =
		g_list_append (combo_border_length_selector_items,
			       (gpointer) "medium");
	combo_border_length_selector_items =
		g_list_append (combo_border_length_selector_items,
			       (gpointer) "thick");
	gtk_combo_set_popdown_strings (GTK_COMBO
				       (combo_border_length_selector),
				       combo_border_length_selector_items);
	g_list_free (combo_border_length_selector_items);

	combo_entry_border_length_selector =
		GTK_COMBO (combo_border_length_selector)->entry;
	gtk_widget_show (combo_entry_border_length_selector);
	gtk_editable_set_editable (GTK_EDITABLE
				   (combo_entry_border_length_selector),
				   FALSE);
	gtk_entry_set_text (GTK_ENTRY (combo_entry_border_length_selector),
			    CSS_STR_USE_LENGTH);

	spinbutton_len_adj = gtk_adjustment_new (1, -35565, 35565, 1, 10, 10);
	spinbutton_len =
		gtk_spin_button_new (GTK_ADJUSTMENT (spinbutton_len_adj), 1,
				     0);
	gtk_widget_show (spinbutton_len);
	gtk_box_pack_start (GTK_BOX (hbox_border_length), spinbutton_len,
			    TRUE, TRUE, 5);

	combo_len = gtk_combo_new ();
	g_object_set_data (G_OBJECT (GTK_COMBO (combo_len)->popwin),
			   "GladeParentKey", combo_len);
	gtk_widget_show (combo_len);
	gtk_box_pack_start (GTK_BOX (hbox_border_length), combo_len, FALSE,
			    FALSE, 5);
	gtk_widget_set_size_request (combo_len, 65, -1);
	combo_len_items = g_list_append (combo_len_items, (gpointer) "em");
	combo_len_items = g_list_append (combo_len_items, (gpointer) "ex");
	combo_len_items = g_list_append (combo_len_items, (gpointer) "px");
	combo_len_items = g_list_append (combo_len_items, (gpointer) "in");
	combo_len_items = g_list_append (combo_len_items, (gpointer) "cm");
	combo_len_items = g_list_append (combo_len_items, (gpointer) "mm");
	combo_len_items = g_list_append (combo_len_items, (gpointer) "pt");
	combo_len_items = g_list_append (combo_len_items, (gpointer) "pc");
	gtk_combo_set_popdown_strings (GTK_COMBO (combo_len),
				       combo_len_items);
	g_list_free (combo_len_items);

	combo_entry_len = GTK_COMBO (combo_len)->entry;
	gtk_widget_show (combo_entry_len);
	gtk_editable_set_editable (GTK_EDITABLE (combo_entry_len), FALSE);
	gtk_entry_set_text (GTK_ENTRY (combo_entry_len), "px");

	hbox_border_style = gtk_hbox_new (TRUE, 0);
	gtk_widget_show (hbox_border_style);
	gtk_box_pack_start (GTK_BOX (vbox_main), hbox_border_style, FALSE,
			    TRUE, 5);

	checkbutton_border_style =
		gtk_check_button_new_with_mnemonic (_("Border style"));
	gtk_widget_show (checkbutton_border_style);
	gtk_box_pack_start (GTK_BOX (hbox_border_style),
			    checkbutton_border_style, TRUE, TRUE, 5);

	combo_border_style = gtk_combo_new ();
	g_object_set_data (G_OBJECT (GTK_COMBO (combo_border_style)->popwin),
			   "GladeParentKey", combo_border_style);
	gtk_widget_show (combo_border_style);
	gtk_box_pack_start (GTK_BOX (hbox_border_style), combo_border_style,
			    TRUE, TRUE, 2);
	combo_border_style_items =
		g_list_append (combo_border_style_items,
			       (gpointer) "hidden ");
	combo_border_style_items =
		g_list_append (combo_border_style_items,
			       (gpointer) "dotted ");
	combo_border_style_items =
		g_list_append (combo_border_style_items,
			       (gpointer) "dashed ");
	combo_border_style_items =
		g_list_append (combo_border_style_items, (gpointer) "solid ");
	combo_border_style_items =
		g_list_append (combo_border_style_items,
			       (gpointer) "double ");
	combo_border_style_items =
		g_list_append (combo_border_style_items,
			       (gpointer) "groove ");
	combo_border_style_items =
		g_list_append (combo_border_style_items, (gpointer) "ridge ");
	combo_border_style_items =
		g_list_append (combo_border_style_items, (gpointer) "inset ");
	combo_border_style_items =
		g_list_append (combo_border_style_items, (gpointer) "outset");
	gtk_combo_set_popdown_strings (GTK_COMBO (combo_border_style),
				       combo_border_style_items);
	g_list_free (combo_border_style_items);

	combo_entry_border_style = GTK_COMBO (combo_border_style)->entry;
	gtk_widget_show (combo_entry_border_style);
	gtk_editable_set_editable (GTK_EDITABLE (combo_entry_border_style),
				   FALSE);
	gtk_entry_set_text (GTK_ENTRY (combo_entry_border_style), "hidden ");

	hbox_border_color = gtk_hbox_new (TRUE, 0);
	gtk_widget_show (hbox_border_color);
	gtk_box_pack_start (GTK_BOX (vbox_main), hbox_border_color, FALSE,
			    FALSE, 5);

	checkbutton_border_color =
		gtk_check_button_new_with_mnemonic (_("Border color"));
	gtk_widget_show (checkbutton_border_color);
	gtk_box_pack_start (GTK_BOX (hbox_border_color),
			    checkbutton_border_color, TRUE, TRUE, 5);

	combo_border_color = gtk_combo_new ();
	g_object_set_data (G_OBJECT (GTK_COMBO (combo_border_color)->popwin),
			   "GladeParentKey", combo_border_color);
	gtk_widget_show (combo_border_color);
	gtk_box_pack_start (GTK_BOX (hbox_border_color), combo_border_color,
			    TRUE, TRUE, 2);
	combo_border_color_items =
		g_list_append (combo_border_color_items,
			       (gpointer) CSS_STR_USE_COLORSEL);
	combo_border_color_items =
		g_list_append (combo_border_color_items, (gpointer) "aqua");
	combo_border_color_items =
		g_list_append (combo_border_color_items, (gpointer) "black");
	combo_border_color_items =
		g_list_append (combo_border_color_items, (gpointer) "blue");
	combo_border_color_items =
		g_list_append (combo_border_color_items,
			       (gpointer) "fuchsia");
	combo_border_color_items =
		g_list_append (combo_border_color_items, (gpointer) "gray");
	combo_border_color_items =
		g_list_append (combo_border_color_items, (gpointer) "green");
	combo_border_color_items =
		g_list_append (combo_border_color_items, (gpointer) "lime");
	combo_border_color_items =
		g_list_append (combo_border_color_items, (gpointer) "maroon");
	combo_border_color_items =
		g_list_append (combo_border_color_items, (gpointer) "navy");
	combo_border_color_items =
		g_list_append (combo_border_color_items, (gpointer) "olive");
	combo_border_color_items =
		g_list_append (combo_border_color_items, (gpointer) "purple");
	combo_border_color_items =
		g_list_append (combo_border_color_items, (gpointer) "red");
	combo_border_color_items =
		g_list_append (combo_border_color_items, (gpointer) "silver");
	combo_border_color_items =
		g_list_append (combo_border_color_items, (gpointer) "teal");
	combo_border_color_items =
		g_list_append (combo_border_color_items, (gpointer) "white");
	combo_border_color_items =
		g_list_append (combo_border_color_items, (gpointer) "yellow");
	combo_border_color_items =
		g_list_append (combo_border_color_items,
			       (gpointer) "ActiveBorder");
	combo_border_color_items =
		g_list_append (combo_border_color_items,
			       (gpointer) "ActiveCaption");
	combo_border_color_items =
		g_list_append (combo_border_color_items,
			       (gpointer) "AppWorkspace");
	combo_border_color_items =
		g_list_append (combo_border_color_items,
			       (gpointer) "Background");
	combo_border_color_items =
		g_list_append (combo_border_color_items,
			       (gpointer) "ButtonFace");
	combo_border_color_items =
		g_list_append (combo_border_color_items,
			       (gpointer) "ButtonHighlight");
	combo_border_color_items =
		g_list_append (combo_border_color_items,
			       (gpointer) "ButtonShadow");
	combo_border_color_items =
		g_list_append (combo_border_color_items,
			       (gpointer) "ButtonText");
	combo_border_color_items =
		g_list_append (combo_border_color_items,
			       (gpointer) "CaptionText");
	combo_border_color_items =
		g_list_append (combo_border_color_items,
			       (gpointer) "GrayText");
	combo_border_color_items =
		g_list_append (combo_border_color_items,
			       (gpointer) "Highlight");
	combo_border_color_items =
		g_list_append (combo_border_color_items,
			       (gpointer) "HighlightText");
	combo_border_color_items =
		g_list_append (combo_border_color_items,
			       (gpointer) "InactiveBorder");
	combo_border_color_items =
		g_list_append (combo_border_color_items,
			       (gpointer) "InactiveCaption");
	combo_border_color_items =
		g_list_append (combo_border_color_items,
			       (gpointer) "InactiveCaptionText");
	combo_border_color_items =
		g_list_append (combo_border_color_items,
			       (gpointer) "InfoBackground");
	combo_border_color_items =
		g_list_append (combo_border_color_items,
			       (gpointer) "InfoText");
	combo_border_color_items =
		g_list_append (combo_border_color_items, (gpointer) "Menu");
	combo_border_color_items =
		g_list_append (combo_border_color_items,
			       (gpointer) "MenuText");
	combo_border_color_items =
		g_list_append (combo_border_color_items,
			       (gpointer) "Scrollbar");
	combo_border_color_items =
		g_list_append (combo_border_color_items,
			       (gpointer) "ThreeDDarkShadow");
	combo_border_color_items =
		g_list_append (combo_border_color_items,
			       (gpointer) "ThreeDFace");
	combo_border_color_items =
		g_list_append (combo_border_color_items,
			       (gpointer) "ThreeDHighlight");
	combo_border_color_items =
		g_list_append (combo_border_color_items,
			       (gpointer) "ThreeDLightShadow");
	combo_border_color_items =
		g_list_append (combo_border_color_items,
			       (gpointer) "ThreeDShadow");
	combo_border_color_items =
		g_list_append (combo_border_color_items, (gpointer) "Window");
	combo_border_color_items =
		g_list_append (combo_border_color_items,
			       (gpointer) "WindowFrame");
	combo_border_color_items =
		g_list_append (combo_border_color_items,
			       (gpointer) "WindowText");
	gtk_combo_set_popdown_strings (GTK_COMBO (combo_border_color),
				       combo_border_color_items);
	g_list_free (combo_border_color_items);

	combo_entry_border_color = GTK_COMBO (combo_border_color)->entry;
	gtk_widget_show (combo_entry_border_color);
	gtk_editable_set_editable (GTK_EDITABLE (combo_entry_border_color),
				   FALSE);
	gtk_entry_set_text (GTK_ENTRY (combo_entry_border_color),
			    CSS_STR_USE_COLORSEL);

	colorselection = gtk_color_selection_new ();
	gtk_widget_show (colorselection);
	gtk_box_pack_start (GTK_BOX (vbox_main), colorselection, FALSE, FALSE,
			    5);
	gtk_container_set_border_width (GTK_CONTAINER (colorselection), 5);
	gtk_color_selection_set_has_opacity_control (GTK_COLOR_SELECTION
						     (colorselection), FALSE);
	gtk_color_selection_set_has_palette (GTK_COLOR_SELECTION
					     (colorselection), TRUE);
	dialog_action_area =
		GTK_DIALOG (border_properties_dialog)->action_area;
	gtk_widget_show (dialog_action_area);
	gtk_button_box_set_layout (GTK_BUTTON_BOX (dialog_action_area),
				   GTK_BUTTONBOX_END);

	helpbutton = gtk_button_new_from_stock ("gtk-help");
	gtk_widget_show (helpbutton);
	gtk_dialog_add_action_widget (GTK_DIALOG (border_properties_dialog),
				      helpbutton, GTK_RESPONSE_HELP);
	GTK_WIDGET_SET_FLAGS (helpbutton, GTK_CAN_DEFAULT);
	gtk_widget_add_accelerator (helpbutton, "clicked", accel_group,
				    GDK_y, GDK_CONTROL_MASK,
				    GTK_ACCEL_VISIBLE);
	gtk_widget_add_accelerator (helpbutton, "clicked", accel_group, GDK_h,
				    GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);

	cancelbutton = gtk_button_new_from_stock ("gtk-cancel");
	gtk_widget_show (cancelbutton);
	gtk_dialog_add_action_widget (GTK_DIALOG (border_properties_dialog),
				      cancelbutton, GTK_RESPONSE_CANCEL);
	GTK_WIDGET_SET_FLAGS (cancelbutton, GTK_CAN_DEFAULT);
	gtk_widget_add_accelerator (cancelbutton, "clicked", accel_group,
				    GDK_c, GDK_CONTROL_MASK,
				    GTK_ACCEL_VISIBLE);

	okbutton = gtk_button_new_from_stock ("gtk-ok");
	gtk_widget_show (okbutton);
	gtk_dialog_add_action_widget (GTK_DIALOG (border_properties_dialog),
				      okbutton, GTK_RESPONSE_OK);
	GTK_WIDGET_SET_FLAGS (okbutton, GTK_CAN_DEFAULT);
	gtk_widget_add_accelerator (okbutton, "clicked", accel_group,
				    GDK_o, GDK_CONTROL_MASK,
				    GTK_ACCEL_VISIBLE);

	/*  to update the spin button to the unit selected */
	g_signal_connect ((gpointer) combo_entry_len,
			  "changed", G_CALLBACK (on_css_dialog_unit_changed),
			  spinbutton_len);

	/* to update the checkboxes when a related widget is changed */
	g_signal_connect ((gpointer) colorselection,
			  "color-changed", G_CALLBACK (on_widget_change_set_toggle_active),
			  checkbutton_border_color);
	g_signal_connect ((gpointer) combo_entry_border_color,
			  "changed", G_CALLBACK (on_widget_change_set_toggle_active),
			  checkbutton_border_color);
			  
	g_signal_connect ((gpointer) combo_entry_border_style,
			  "changed", G_CALLBACK (on_widget_change_set_toggle_active),
			  checkbutton_border_style);
			  
	g_signal_connect ((gpointer) combo_entry_border_length_selector,
			  "changed", G_CALLBACK (on_widget_change_set_toggle_active),
			  checkbutton_border_length);
	g_signal_connect ((gpointer) combo_entry_len,
			  "changed", G_CALLBACK (on_widget_change_set_toggle_active),
			  checkbutton_border_length);
	g_signal_connect ((gpointer) spinbutton_len,
			  "value-changed", G_CALLBACK (on_widget_change_set_toggle_active),
			  checkbutton_border_length);			  
			  	
	if (connect_callbacks)
	{
		g_signal_connect ((gpointer) border_properties_dialog,
				  "close", G_CALLBACK (on_css_dialog_close),
				  data);

		g_signal_connect ((gpointer) helpbutton, "clicked",
				  G_CALLBACK
				  (on_border_properties_helpbutton_clicked),
				  NULL);
		g_signal_connect ((gpointer) cancelbutton, "clicked",
				  G_CALLBACK
				  (on_css_dialog_cancel_button_clicked),
				  data);
		g_signal_connect ((gpointer) okbutton, "clicked",
				  G_CALLBACK
				  (on_border_properties_okbutton_clicked),
				  data);
	}

	GLADE_HOOKUP_OBJECT_NO_REF (border_properties_dialog,
				    border_properties_dialog,
				    "border_properties_dialog");
	GLADE_HOOKUP_OBJECT_NO_REF (border_properties_dialog,
				    dialog_vbox_main, "dialog_vbox_main");
	GLADE_HOOKUP_OBJECT (border_properties_dialog, vbox_main,
			     "vbox_main");
	GLADE_HOOKUP_OBJECT (border_properties_dialog, hbox_border_length,
			     "hbox_border_length");
	GLADE_HOOKUP_OBJECT (border_properties_dialog,
			     checkbutton_border_length,
			     "checkbutton_border_length");
	GLADE_HOOKUP_OBJECT (border_properties_dialog,
			     combo_border_length_selector,
			     "combo_border_length_selector");
	GLADE_HOOKUP_OBJECT (border_properties_dialog,
			     combo_entry_border_length_selector,
			     "combo_entry_border_length_selector");
	GLADE_HOOKUP_OBJECT (border_properties_dialog, spinbutton_len,
			     "spinbutton_len");
	GLADE_HOOKUP_OBJECT (border_properties_dialog, combo_len,
			     "combo_len");
	GLADE_HOOKUP_OBJECT (border_properties_dialog, combo_entry_len,
			     "combo_entry_len");
	GLADE_HOOKUP_OBJECT (border_properties_dialog, hbox_border_style,
			     "hbox_border_style");
	GLADE_HOOKUP_OBJECT (border_properties_dialog,
			     checkbutton_border_style,
			     "checkbutton_border_style");
	GLADE_HOOKUP_OBJECT (border_properties_dialog, combo_border_style,
			     "combo_border_style");
	GLADE_HOOKUP_OBJECT (border_properties_dialog,
			     combo_entry_border_style,
			     "combo_entry_border_style");
	GLADE_HOOKUP_OBJECT (border_properties_dialog, hbox_border_color,
			     "hbox_border_color");
	GLADE_HOOKUP_OBJECT (border_properties_dialog,
			     checkbutton_border_color,
			     "checkbutton_border_color");
	GLADE_HOOKUP_OBJECT (border_properties_dialog, combo_border_color,
			     "combo_border_color");
	GLADE_HOOKUP_OBJECT (border_properties_dialog,
			     combo_entry_border_color,
			     "combo_entry_border_color");
	GLADE_HOOKUP_OBJECT (border_properties_dialog, colorselection,
			     "colorselection");
	GLADE_HOOKUP_OBJECT_NO_REF (border_properties_dialog,
				    dialog_action_area, "dialog_action_area");
	GLADE_HOOKUP_OBJECT (border_properties_dialog, helpbutton,
			     "helpbutton");
	GLADE_HOOKUP_OBJECT (border_properties_dialog, cancelbutton,
			     "cancelbutton");
	GLADE_HOOKUP_OBJECT (border_properties_dialog, okbutton, "okbutton");

	gtk_window_add_accel_group (GTK_WINDOW (border_properties_dialog),
				    accel_group);

	return border_properties_dialog;
}

GtkWidget *
create_border_width_properties_dialog (CssedWindow * window,
				       gchar * previous_string,
				       gboolean
				       add_previous_string,
				       gboolean connect_callbacks)
{
	GtkWidget *border_width_properties_dialog;
	GtkWidget *dialog_vbox;
	GtkWidget *table_main;
	GtkWidget *checkbutton_border_width_top;
	GtkWidget *checkbutton_border_width_right;
	GtkWidget *checkbutton_border_width_bottom;
	GtkWidget *checkbutton_border_width_left;
	GtkWidget *combo_border_width_top_units;
	GList *combo_border_width_top_units_items = NULL;
	GtkWidget *combo_entry_border_width_top_units;
	GtkWidget *combo_border_width_right_units;
	GList *combo_border_width_right_units_items = NULL;
	GtkWidget *combo_entry_border_width_right_units;
	GtkWidget *combo_border_width_bottom_units;
	GList *combo_border_width_bottom_units_items = NULL;
	GtkWidget *combo_entry_border_width_bottom_units;
	GtkWidget *combo_border_width_left_units;
	GList *combo_border_width_left_units_items = NULL;
	GtkWidget *combo_entry_border_width_left_units;
	GtkWidget *combo_border_width_top;
	GList *combo_border_width_top_items = NULL;
	GtkWidget *combo_entry_border_width_top_selector;
	GtkWidget *combo_border_width_right;
	GList *combo_border_width_right_items = NULL;
	GtkWidget *combo_entry_border_width_right_selector;
	GtkWidget *combo_border_width_bottom;
	GList *combo_border_width_bottom_items = NULL;
	GtkWidget *combo_entry_border_width_bottom_selector;
	GtkWidget *combo_border_width_left;
	GList *combo_border_width_left_items = NULL;
	GtkWidget *combo_entry_border_width_left_selector;
	GtkObject *spinbutton_border_width_top_adj;
	GtkWidget *spinbutton_border_width_top;
	GtkObject *spinbutton_border_width_right_adj;
	GtkWidget *spinbutton_border_width_right;
	GtkObject *spinbutton_border_width_bottom_adj;
	GtkWidget *spinbutton_border_width_bottom;
	GtkObject *spinbutton_border_width_left_adj;
	GtkWidget *spinbutton_border_width_left;
	GtkWidget *dialog_action_area;
	GtkWidget *helpbutton;
	GtkWidget *cancelbutton;
	GtkWidget *okbutton;
	GtkAccelGroup *accel_group;
	GtkWidget *main_window;
	CssedCssDialogsData *data = NULL;

	if (connect_callbacks)
	{
		data = g_malloc (sizeof (CssedCssDialogsData));
		data->main_window = window;
		data->previous_string = previous_string;
		data->add_property_string = add_previous_string;
	}

	accel_group = gtk_accel_group_new ();

	border_width_properties_dialog = gtk_dialog_new ();
	gtk_window_set_title (GTK_WINDOW (border_width_properties_dialog), "border-width");
	gtk_window_set_modal (GTK_WINDOW (border_width_properties_dialog), TRUE);

	main_window = cssed_window_get_window_widget( window );	
	gtk_window_set_transient_for (GTK_WINDOW(border_width_properties_dialog), GTK_WINDOW (main_window));

	dialog_vbox = GTK_DIALOG (border_width_properties_dialog)->vbox;
	gtk_widget_show (dialog_vbox);

	table_main = gtk_table_new (4, 4, FALSE);
	gtk_widget_show (table_main);
	gtk_box_pack_start (GTK_BOX (dialog_vbox), table_main, FALSE, FALSE,
			    0);
	gtk_container_set_border_width (GTK_CONTAINER (table_main), 2);
	gtk_table_set_row_spacings (GTK_TABLE (table_main), 2);
	gtk_table_set_col_spacings (GTK_TABLE (table_main), 6);

	checkbutton_border_width_top =
		gtk_check_button_new_with_mnemonic (_("Top"));
	gtk_widget_show (checkbutton_border_width_top);
	gtk_table_attach (GTK_TABLE (table_main),
			  checkbutton_border_width_top, 0, 1, 0, 1,
			  (GtkAttachOptions) (GTK_FILL),
			  (GtkAttachOptions) (0), 0, 0);

	checkbutton_border_width_right =
		gtk_check_button_new_with_mnemonic (_("Right"));
	gtk_widget_show (checkbutton_border_width_right);
	gtk_table_attach (GTK_TABLE (table_main),
			  checkbutton_border_width_right, 0, 1, 1, 2,
			  (GtkAttachOptions) (GTK_FILL),
			  (GtkAttachOptions) (0), 0, 0);

	checkbutton_border_width_bottom =
		gtk_check_button_new_with_mnemonic (_("Bottom"));
	gtk_widget_show (checkbutton_border_width_bottom);
	gtk_table_attach (GTK_TABLE (table_main),
			  checkbutton_border_width_bottom, 0, 1, 2, 3,
			  (GtkAttachOptions) (GTK_FILL),
			  (GtkAttachOptions) (0), 0, 0);

	checkbutton_border_width_left =
		gtk_check_button_new_with_mnemonic (_("Left"));
	gtk_widget_show (checkbutton_border_width_left);
	gtk_table_attach (GTK_TABLE (table_main),
			  checkbutton_border_width_left, 0, 1, 3, 4,
			  (GtkAttachOptions) (GTK_FILL),
			  (GtkAttachOptions) (0), 0, 0);

	combo_border_width_top_units = gtk_combo_new ();
	g_object_set_data (G_OBJECT
			   (GTK_COMBO (combo_border_width_top_units)->popwin),
			   "GladeParentKey", combo_border_width_top_units);
	gtk_widget_show (combo_border_width_top_units);
	gtk_table_attach (GTK_TABLE (table_main),
			  combo_border_width_top_units, 3, 4, 0, 1,
			  (GtkAttachOptions) (0), (GtkAttachOptions) (0), 0,
			  0);
	combo_border_width_top_units_items =
		g_list_append (combo_border_width_top_units_items,
			       (gpointer) "em");
	combo_border_width_top_units_items =
		g_list_append (combo_border_width_top_units_items,
			       (gpointer) "ex");
	combo_border_width_top_units_items =
		g_list_append (combo_border_width_top_units_items,
			       (gpointer) "px");
	combo_border_width_top_units_items =
		g_list_append (combo_border_width_top_units_items,
			       (gpointer) "in");
	combo_border_width_top_units_items =
		g_list_append (combo_border_width_top_units_items,
			       (gpointer) "cm");
	combo_border_width_top_units_items =
		g_list_append (combo_border_width_top_units_items,
			       (gpointer) "mm");
	combo_border_width_top_units_items =
		g_list_append (combo_border_width_top_units_items,
			       (gpointer) "pt");
	combo_border_width_top_units_items =
		g_list_append (combo_border_width_top_units_items,
			       (gpointer) "pc");
	gtk_combo_set_popdown_strings (GTK_COMBO
				       (combo_border_width_top_units),
				       combo_border_width_top_units_items);
	g_list_free (combo_border_width_top_units_items);

	combo_entry_border_width_top_units =
		GTK_COMBO (combo_border_width_top_units)->entry;
	gtk_widget_show (combo_entry_border_width_top_units);
	gtk_widget_set_size_request (combo_entry_border_width_top_units, 40,
				     -1);
	gtk_editable_set_editable (GTK_EDITABLE
				   (combo_entry_border_width_top_units),
				   FALSE);
	gtk_entry_set_text (GTK_ENTRY (combo_entry_border_width_top_units),
			    "px");

	combo_border_width_right_units = gtk_combo_new ();
	g_object_set_data (G_OBJECT
			   (GTK_COMBO (combo_border_width_right_units)->
			    popwin), "GladeParentKey",
			   combo_border_width_right_units);
	gtk_widget_show (combo_border_width_right_units);
	gtk_table_attach (GTK_TABLE (table_main),
			  combo_border_width_right_units, 3, 4, 1, 2,
			  (GtkAttachOptions) (0), (GtkAttachOptions) (0), 0,
			  0);
	combo_border_width_right_units_items =
		g_list_append (combo_border_width_right_units_items,
			       (gpointer) "em");
	combo_border_width_right_units_items =
		g_list_append (combo_border_width_right_units_items,
			       (gpointer) "ex");
	combo_border_width_right_units_items =
		g_list_append (combo_border_width_right_units_items,
			       (gpointer) "px");
	combo_border_width_right_units_items =
		g_list_append (combo_border_width_right_units_items,
			       (gpointer) "in");
	combo_border_width_right_units_items =
		g_list_append (combo_border_width_right_units_items,
			       (gpointer) "cm");
	combo_border_width_right_units_items =
		g_list_append (combo_border_width_right_units_items,
			       (gpointer) "mm");
	combo_border_width_right_units_items =
		g_list_append (combo_border_width_right_units_items,
			       (gpointer) "pt");
	combo_border_width_right_units_items =
		g_list_append (combo_border_width_right_units_items,
			       (gpointer) "pc");
	gtk_combo_set_popdown_strings (GTK_COMBO
				       (combo_border_width_right_units),
				       combo_border_width_right_units_items);
	g_list_free (combo_border_width_right_units_items);

	combo_entry_border_width_right_units =
		GTK_COMBO (combo_border_width_right_units)->entry;
	gtk_widget_show (combo_entry_border_width_right_units);
	gtk_widget_set_size_request (combo_entry_border_width_right_units, 40,
				     -1);
	gtk_editable_set_editable (GTK_EDITABLE
				   (combo_entry_border_width_right_units),
				   FALSE);
	gtk_entry_set_text (GTK_ENTRY (combo_entry_border_width_right_units),
			    "px");

	combo_border_width_bottom_units = gtk_combo_new ();
	g_object_set_data (G_OBJECT
			   (GTK_COMBO (combo_border_width_bottom_units)->
			    popwin), "GladeParentKey",
			   combo_border_width_bottom_units);
	gtk_widget_show (combo_border_width_bottom_units);
	gtk_table_attach (GTK_TABLE (table_main),
			  combo_border_width_bottom_units, 3, 4, 2, 3,
			  (GtkAttachOptions) (0), (GtkAttachOptions) (0), 0,
			  0);
	combo_border_width_bottom_units_items =
		g_list_append (combo_border_width_bottom_units_items,
			       (gpointer) "em");
	combo_border_width_bottom_units_items =
		g_list_append (combo_border_width_bottom_units_items,
			       (gpointer) "ex");
	combo_border_width_bottom_units_items =
		g_list_append (combo_border_width_bottom_units_items,
			       (gpointer) "px");
	combo_border_width_bottom_units_items =
		g_list_append (combo_border_width_bottom_units_items,
			       (gpointer) "in");
	combo_border_width_bottom_units_items =
		g_list_append (combo_border_width_bottom_units_items,
			       (gpointer) "cm");
	combo_border_width_bottom_units_items =
		g_list_append (combo_border_width_bottom_units_items,
			       (gpointer) "mm");
	combo_border_width_bottom_units_items =
		g_list_append (combo_border_width_bottom_units_items,
			       (gpointer) "pt");
	combo_border_width_bottom_units_items =
		g_list_append (combo_border_width_bottom_units_items,
			       (gpointer) "pc");
	gtk_combo_set_popdown_strings (GTK_COMBO
				       (combo_border_width_bottom_units),
				       combo_border_width_bottom_units_items);
	g_list_free (combo_border_width_bottom_units_items);

	combo_entry_border_width_bottom_units =
		GTK_COMBO (combo_border_width_bottom_units)->entry;
	gtk_widget_show (combo_entry_border_width_bottom_units);
	gtk_widget_set_size_request (combo_entry_border_width_bottom_units,
				     40, -1);
	gtk_editable_set_editable (GTK_EDITABLE
				   (combo_entry_border_width_bottom_units),
				   FALSE);
	gtk_entry_set_text (GTK_ENTRY (combo_entry_border_width_bottom_units),
			    "px");

	combo_border_width_left_units = gtk_combo_new ();
	g_object_set_data (G_OBJECT
			   (GTK_COMBO (combo_border_width_left_units)->
			    popwin), "GladeParentKey",
			   combo_border_width_left_units);
	gtk_widget_show (combo_border_width_left_units);
	gtk_table_attach (GTK_TABLE (table_main),
			  combo_border_width_left_units, 3, 4, 3, 4,
			  (GtkAttachOptions) (0), (GtkAttachOptions) (0), 0,
			  0);
	combo_border_width_left_units_items =
		g_list_append (combo_border_width_left_units_items,
			       (gpointer) "em");
	combo_border_width_left_units_items =
		g_list_append (combo_border_width_left_units_items,
			       (gpointer) "ex");
	combo_border_width_left_units_items =
		g_list_append (combo_border_width_left_units_items,
			       (gpointer) "px");
	combo_border_width_left_units_items =
		g_list_append (combo_border_width_left_units_items,
			       (gpointer) "in");
	combo_border_width_left_units_items =
		g_list_append (combo_border_width_left_units_items,
			       (gpointer) "cm");
	combo_border_width_left_units_items =
		g_list_append (combo_border_width_left_units_items,
			       (gpointer) "mm");
	combo_border_width_left_units_items =
		g_list_append (combo_border_width_left_units_items,
			       (gpointer) "pt");
	combo_border_width_left_units_items =
		g_list_append (combo_border_width_left_units_items,
			       (gpointer) "pc");
	gtk_combo_set_popdown_strings (GTK_COMBO
				       (combo_border_width_left_units),
				       combo_border_width_left_units_items);
	g_list_free (combo_border_width_left_units_items);

	combo_entry_border_width_left_units =
		GTK_COMBO (combo_border_width_left_units)->entry;
	gtk_widget_show (combo_entry_border_width_left_units);
	gtk_widget_set_size_request (combo_entry_border_width_left_units, 40,
				     -1);
	gtk_editable_set_editable (GTK_EDITABLE
				   (combo_entry_border_width_left_units),
				   FALSE);
	gtk_entry_set_text (GTK_ENTRY (combo_entry_border_width_left_units),
			    "px");

	combo_border_width_top = gtk_combo_new ();
	g_object_set_data (G_OBJECT
			   (GTK_COMBO (combo_border_width_top)->popwin),
			   "GladeParentKey", combo_border_width_top);
	gtk_widget_show (combo_border_width_top);
	gtk_table_attach (GTK_TABLE (table_main), combo_border_width_top, 1,
			  2, 0, 1, (GtkAttachOptions) (0),
			  (GtkAttachOptions) (0), 0, 0);
	combo_border_width_top_items =
		g_list_append (combo_border_width_top_items,
			       (gpointer) CSS_STR_USE_LENGTH);
	combo_border_width_top_items =
		g_list_append (combo_border_width_top_items,
			       (gpointer) "thin");
	combo_border_width_top_items =
		g_list_append (combo_border_width_top_items,
			       (gpointer) "medium");
	combo_border_width_top_items =
		g_list_append (combo_border_width_top_items,
			       (gpointer) "thick");
	gtk_combo_set_popdown_strings (GTK_COMBO (combo_border_width_top),
				       combo_border_width_top_items);
	g_list_free (combo_border_width_top_items);

	combo_entry_border_width_top_selector =
		GTK_COMBO (combo_border_width_top)->entry;
	gtk_widget_show (combo_entry_border_width_top_selector);
	gtk_editable_set_editable (GTK_EDITABLE
				   (combo_entry_border_width_top_selector),
				   FALSE);
	gtk_entry_set_text (GTK_ENTRY (combo_entry_border_width_top_selector),
			    CSS_STR_USE_LENGTH);

	combo_border_width_right = gtk_combo_new ();
	g_object_set_data (G_OBJECT
			   (GTK_COMBO (combo_border_width_right)->popwin),
			   "GladeParentKey", combo_border_width_right);
	gtk_widget_show (combo_border_width_right);
	gtk_table_attach (GTK_TABLE (table_main), combo_border_width_right, 1,
			  2, 1, 2, (GtkAttachOptions) (0),
			  (GtkAttachOptions) (0), 0, 0);
	combo_border_width_right_items =
		g_list_append (combo_border_width_right_items,
			       (gpointer) CSS_STR_USE_LENGTH);
	combo_border_width_right_items =
		g_list_append (combo_border_width_right_items,
			       (gpointer) "thin");
	combo_border_width_right_items =
		g_list_append (combo_border_width_right_items,
			       (gpointer) "medium");
	combo_border_width_right_items =
		g_list_append (combo_border_width_right_items,
			       (gpointer) "thick");
	gtk_combo_set_popdown_strings (GTK_COMBO (combo_border_width_right),
				       combo_border_width_right_items);
	g_list_free (combo_border_width_right_items);

	combo_entry_border_width_right_selector =
		GTK_COMBO (combo_border_width_right)->entry;
	gtk_widget_show (combo_entry_border_width_right_selector);
	gtk_editable_set_editable (GTK_EDITABLE
				   (combo_entry_border_width_right_selector),
				   FALSE);
	gtk_entry_set_text (GTK_ENTRY
			    (combo_entry_border_width_right_selector),
			    CSS_STR_USE_LENGTH);

	combo_border_width_bottom = gtk_combo_new ();
	g_object_set_data (G_OBJECT
			   (GTK_COMBO (combo_border_width_bottom)->popwin),
			   "GladeParentKey", combo_border_width_bottom);
	gtk_widget_show (combo_border_width_bottom);
	gtk_table_attach (GTK_TABLE (table_main), combo_border_width_bottom,
			  1, 2, 2, 3, (GtkAttachOptions) (0),
			  (GtkAttachOptions) (0), 0, 0);
	combo_border_width_bottom_items =
		g_list_append (combo_border_width_bottom_items,
			       (gpointer) CSS_STR_USE_LENGTH);
	combo_border_width_bottom_items =
		g_list_append (combo_border_width_bottom_items,
			       (gpointer) "thin");
	combo_border_width_bottom_items =
		g_list_append (combo_border_width_bottom_items,
			       (gpointer) "medium");
	combo_border_width_bottom_items =
		g_list_append (combo_border_width_bottom_items,
			       (gpointer) "thick");
	gtk_combo_set_popdown_strings (GTK_COMBO (combo_border_width_bottom),
				       combo_border_width_bottom_items);
	g_list_free (combo_border_width_bottom_items);

	combo_entry_border_width_bottom_selector =
		GTK_COMBO (combo_border_width_bottom)->entry;
	gtk_widget_show (combo_entry_border_width_bottom_selector);
	gtk_editable_set_editable (GTK_EDITABLE
				   (combo_entry_border_width_bottom_selector),
				   FALSE);
	gtk_entry_set_text (GTK_ENTRY
			    (combo_entry_border_width_bottom_selector),
			    CSS_STR_USE_LENGTH);

	combo_border_width_left = gtk_combo_new ();
	g_object_set_data (G_OBJECT
			   (GTK_COMBO (combo_border_width_left)->popwin),
			   "GladeParentKey", combo_border_width_left);
	gtk_widget_show (combo_border_width_left);
	gtk_table_attach (GTK_TABLE (table_main), combo_border_width_left, 1,
			  2, 3, 4, (GtkAttachOptions) (0),
			  (GtkAttachOptions) (0), 0, 0);
	combo_border_width_left_items =
		g_list_append (combo_border_width_left_items,
			       (gpointer) CSS_STR_USE_LENGTH);
	combo_border_width_left_items =
		g_list_append (combo_border_width_left_items,
			       (gpointer) "thin");
	combo_border_width_left_items =
		g_list_append (combo_border_width_left_items,
			       (gpointer) "medium");
	combo_border_width_left_items =
		g_list_append (combo_border_width_left_items,
			       (gpointer) "thick");
	gtk_combo_set_popdown_strings (GTK_COMBO (combo_border_width_left),
				       combo_border_width_left_items);
	g_list_free (combo_border_width_left_items);

	combo_entry_border_width_left_selector =
		GTK_COMBO (combo_border_width_left)->entry;
	gtk_widget_show (combo_entry_border_width_left_selector);
	gtk_editable_set_editable (GTK_EDITABLE
				   (combo_entry_border_width_left_selector),
				   FALSE);
	gtk_entry_set_text (GTK_ENTRY
			    (combo_entry_border_width_left_selector),
			    CSS_STR_USE_LENGTH);

	spinbutton_border_width_top_adj =
		gtk_adjustment_new (1, -35565, 35565, 1, 10, 10);
	spinbutton_border_width_top =
		gtk_spin_button_new (GTK_ADJUSTMENT
				     (spinbutton_border_width_top_adj), 1, 0);
	gtk_widget_show (spinbutton_border_width_top);
	gtk_table_attach (GTK_TABLE (table_main), spinbutton_border_width_top,
			  2, 3, 0, 1, (GtkAttachOptions) (0),
			  (GtkAttachOptions) (0), 0, 0);

	spinbutton_border_width_right_adj =
		gtk_adjustment_new (1, -35565, 35565, 1, 10, 10);
	spinbutton_border_width_right =
		gtk_spin_button_new (GTK_ADJUSTMENT
				     (spinbutton_border_width_right_adj), 1,
				     0);
	gtk_widget_show (spinbutton_border_width_right);
	gtk_table_attach (GTK_TABLE (table_main),
			  spinbutton_border_width_right, 2, 3, 1, 2,
			  (GtkAttachOptions) (0), (GtkAttachOptions) (0), 0,
			  0);

	spinbutton_border_width_bottom_adj =
		gtk_adjustment_new (1, -35565, 35565, 1, 10, 10);
	spinbutton_border_width_bottom =
		gtk_spin_button_new (GTK_ADJUSTMENT
				     (spinbutton_border_width_bottom_adj), 1,
				     0);
	gtk_widget_show (spinbutton_border_width_bottom);
	gtk_table_attach (GTK_TABLE (table_main),
			  spinbutton_border_width_bottom, 2, 3, 2, 3,
			  (GtkAttachOptions) (0), (GtkAttachOptions) (0), 0,
			  0);

	spinbutton_border_width_left_adj =
		gtk_adjustment_new (1, -35565, 35565, 1, 10, 10);
	spinbutton_border_width_left =
		gtk_spin_button_new (GTK_ADJUSTMENT
				     (spinbutton_border_width_left_adj), 1,
				     0);
	gtk_widget_show (spinbutton_border_width_left);
	gtk_table_attach (GTK_TABLE (table_main),
			  spinbutton_border_width_left, 2, 3, 3, 4,
			  (GtkAttachOptions) (0), (GtkAttachOptions) (0), 0,
			  0);

	dialog_action_area =
		GTK_DIALOG (border_width_properties_dialog)->action_area;
	gtk_widget_show (dialog_action_area);
	gtk_button_box_set_layout (GTK_BUTTON_BOX (dialog_action_area),
				   GTK_BUTTONBOX_END);

	helpbutton = gtk_button_new_from_stock ("gtk-help");
	gtk_widget_show (helpbutton);
	gtk_dialog_add_action_widget (GTK_DIALOG
				      (border_width_properties_dialog),
				      helpbutton, GTK_RESPONSE_HELP);
	GTK_WIDGET_SET_FLAGS (helpbutton, GTK_CAN_DEFAULT);
	gtk_widget_add_accelerator (helpbutton, "clicked", accel_group,
				    GDK_y, GDK_CONTROL_MASK,
				    GTK_ACCEL_VISIBLE);
	gtk_widget_add_accelerator (helpbutton, "clicked", accel_group, GDK_h,
				    GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);

	cancelbutton = gtk_button_new_from_stock ("gtk-cancel");
	gtk_widget_show (cancelbutton);
	gtk_dialog_add_action_widget (GTK_DIALOG
				      (border_width_properties_dialog),
				      cancelbutton, GTK_RESPONSE_CANCEL);
	GTK_WIDGET_SET_FLAGS (cancelbutton, GTK_CAN_DEFAULT);
	gtk_widget_add_accelerator (cancelbutton, "clicked", accel_group,
				    GDK_c, GDK_CONTROL_MASK,
				    GTK_ACCEL_VISIBLE);

	okbutton = gtk_button_new_from_stock ("gtk-ok");
	gtk_widget_show (okbutton);
	gtk_dialog_add_action_widget (GTK_DIALOG
				      (border_width_properties_dialog),
				      okbutton, GTK_RESPONSE_OK);
	GTK_WIDGET_SET_FLAGS (okbutton, GTK_CAN_DEFAULT);
	gtk_widget_add_accelerator (okbutton, "clicked", accel_group,
				    GDK_o, GDK_CONTROL_MASK,
				    GTK_ACCEL_VISIBLE);
				    
	/*  to update the spin button to the unit selected */
	g_signal_connect ((gpointer) combo_entry_border_width_top_units,
			  "changed", G_CALLBACK (on_css_dialog_unit_changed),
			  spinbutton_border_width_top);
	g_signal_connect ((gpointer) combo_entry_border_width_right_units,
			  "changed", G_CALLBACK (on_css_dialog_unit_changed),
			  spinbutton_border_width_right);
	g_signal_connect ((gpointer) combo_entry_border_width_bottom_units,
			  "changed", G_CALLBACK (on_css_dialog_unit_changed),
			  spinbutton_border_width_bottom);
	g_signal_connect ((gpointer) combo_entry_border_width_left_units,
			  "changed", G_CALLBACK (on_css_dialog_unit_changed),
			  spinbutton_border_width_left);
			  
	/* to update the checkboxes when a related widget is changed */
	g_signal_connect ((gpointer) combo_entry_border_width_top_units,
			  "changed", G_CALLBACK (on_widget_change_set_toggle_active),
			  checkbutton_border_width_top);
	g_signal_connect ((gpointer) combo_entry_border_width_top_selector,
			  "changed", G_CALLBACK (on_widget_change_set_toggle_active),
			  checkbutton_border_width_top);
	g_signal_connect ((gpointer) spinbutton_border_width_top,
			  "value-changed", G_CALLBACK (on_widget_change_set_toggle_active),
			  checkbutton_border_width_top);	
	
	g_signal_connect ((gpointer) combo_entry_border_width_right_units,
			  "changed", G_CALLBACK (on_widget_change_set_toggle_active),
			  checkbutton_border_width_right);
	g_signal_connect ((gpointer) combo_entry_border_width_right_selector,
			  "changed", G_CALLBACK (on_widget_change_set_toggle_active),
			  checkbutton_border_width_right);
	g_signal_connect ((gpointer) spinbutton_border_width_right,
			  "value-changed", G_CALLBACK (on_widget_change_set_toggle_active),
			  checkbutton_border_width_right);	

	g_signal_connect ((gpointer) combo_entry_border_width_bottom_units,
			  "changed", G_CALLBACK (on_widget_change_set_toggle_active),
			  checkbutton_border_width_bottom);
	g_signal_connect ((gpointer) combo_entry_border_width_bottom_selector,
			  "changed", G_CALLBACK (on_widget_change_set_toggle_active),
			  checkbutton_border_width_bottom);
	g_signal_connect ((gpointer) spinbutton_border_width_bottom,
			  "value-changed", G_CALLBACK (on_widget_change_set_toggle_active),
			  checkbutton_border_width_bottom);	


	g_signal_connect ((gpointer) combo_entry_border_width_left_units,
			  "changed", G_CALLBACK (on_widget_change_set_toggle_active),
			  checkbutton_border_width_left);
	g_signal_connect ((gpointer) combo_entry_border_width_left_selector,
			  "changed", G_CALLBACK (on_widget_change_set_toggle_active),
			  checkbutton_border_width_left);
	g_signal_connect ((gpointer) spinbutton_border_width_left,
			  "value-changed", G_CALLBACK (on_widget_change_set_toggle_active),
			  checkbutton_border_width_left);	
			  
	if (connect_callbacks)
	{
		g_signal_connect ((gpointer) border_width_properties_dialog,
				  "close", G_CALLBACK (on_css_dialog_close),
				  data);

		g_signal_connect ((gpointer) helpbutton, "clicked",
				  G_CALLBACK
				  (on_border_with_properties_helpbutton_clicked),
				  NULL);
		g_signal_connect ((gpointer) cancelbutton, "clicked",
				  G_CALLBACK
				  (on_css_dialog_cancel_button_clicked),
				  data);
		g_signal_connect ((gpointer) okbutton, "clicked",
				  G_CALLBACK
				  (on_border_width_properties_dialog_okbutton_clicked),
				  data);
	}

	GLADE_HOOKUP_OBJECT_NO_REF (border_width_properties_dialog,
				    border_width_properties_dialog,
				    "border_width_properties_dialog");
	GLADE_HOOKUP_OBJECT_NO_REF (border_width_properties_dialog,
				    dialog_vbox, "dialog_vbox");
	GLADE_HOOKUP_OBJECT (border_width_properties_dialog, table_main,
			     "table_main");
	GLADE_HOOKUP_OBJECT (border_width_properties_dialog,
			     checkbutton_border_width_top,
			     "checkbutton_border_width_top");
	GLADE_HOOKUP_OBJECT (border_width_properties_dialog,
			     checkbutton_border_width_right,
			     "checkbutton_border_width_right");
	GLADE_HOOKUP_OBJECT (border_width_properties_dialog,
			     checkbutton_border_width_bottom,
			     "checkbutton_border_width_bottom");
	GLADE_HOOKUP_OBJECT (border_width_properties_dialog,
			     checkbutton_border_width_left,
			     "checkbutton_border_width_left");
	GLADE_HOOKUP_OBJECT (border_width_properties_dialog,
			     combo_border_width_top_units,
			     "combo_border_width_top_units");
	GLADE_HOOKUP_OBJECT (border_width_properties_dialog,
			     combo_entry_border_width_top_units,
			     "combo_entry_border_width_top_units");
	GLADE_HOOKUP_OBJECT (border_width_properties_dialog,
			     combo_border_width_right_units,
			     "combo_border_width_right_units");
	GLADE_HOOKUP_OBJECT (border_width_properties_dialog,
			     combo_entry_border_width_right_units,
			     "combo_entry_border_width_right_units");
	GLADE_HOOKUP_OBJECT (border_width_properties_dialog,
			     combo_border_width_bottom_units,
			     "combo_border_width_bottom_units");
	GLADE_HOOKUP_OBJECT (border_width_properties_dialog,
			     combo_entry_border_width_bottom_units,
			     "combo_entry_border_width_bottom_units");
	GLADE_HOOKUP_OBJECT (border_width_properties_dialog,
			     combo_border_width_left_units,
			     "combo_border_width_left_units");
	GLADE_HOOKUP_OBJECT (border_width_properties_dialog,
			     combo_entry_border_width_left_units,
			     "combo_entry_border_width_left_units");
	GLADE_HOOKUP_OBJECT (border_width_properties_dialog,
			     combo_border_width_top,
			     "combo_border_width_top");
	GLADE_HOOKUP_OBJECT (border_width_properties_dialog,
			     combo_entry_border_width_top_selector,
			     "combo_entry_border_width_top_selector");
	GLADE_HOOKUP_OBJECT (border_width_properties_dialog,
			     combo_border_width_right,
			     "combo_border_width_right");
	GLADE_HOOKUP_OBJECT (border_width_properties_dialog,
			     combo_entry_border_width_right_selector,
			     "combo_entry_border_width_right_selector");
	GLADE_HOOKUP_OBJECT (border_width_properties_dialog,
			     combo_border_width_bottom,
			     "combo_border_width_bottom");
	GLADE_HOOKUP_OBJECT (border_width_properties_dialog,
			     combo_entry_border_width_bottom_selector,
			     "combo_entry_border_width_bottom_selector");
	GLADE_HOOKUP_OBJECT (border_width_properties_dialog,
			     combo_border_width_left,
			     "combo_border_width_left");
	GLADE_HOOKUP_OBJECT (border_width_properties_dialog,
			     combo_entry_border_width_left_selector,
			     "combo_entry_border_width_left_selector");
	GLADE_HOOKUP_OBJECT (border_width_properties_dialog,
			     spinbutton_border_width_top,
			     "spinbutton_border_width_top");
	GLADE_HOOKUP_OBJECT (border_width_properties_dialog,
			     spinbutton_border_width_right,
			     "spinbutton_border_width_right");
	GLADE_HOOKUP_OBJECT (border_width_properties_dialog,
			     spinbutton_border_width_bottom,
			     "spinbutton_border_width_bottom");
	GLADE_HOOKUP_OBJECT (border_width_properties_dialog,
			     spinbutton_border_width_left,
			     "spinbutton_border_width_left");
	GLADE_HOOKUP_OBJECT_NO_REF (border_width_properties_dialog,
				    dialog_action_area, "dialog_action_area");
	GLADE_HOOKUP_OBJECT (border_width_properties_dialog, helpbutton,
			     "helpbutton");
	GLADE_HOOKUP_OBJECT (border_width_properties_dialog, cancelbutton,
			     "cancelbutton");
	GLADE_HOOKUP_OBJECT (border_width_properties_dialog, okbutton,
			     "okbutton");

	gtk_window_add_accel_group (GTK_WINDOW
				    (border_width_properties_dialog),
				    accel_group);

	return border_width_properties_dialog;
}

GtkWidget *
create_border_width_single_properties_dialog (CssedWindow *
					      window,
					      gchar *
					      previous_string,
					      gboolean
					      add_previous_string,
					      gboolean connect_callbacks)
{
	GtkWidget *border_width_single_properties_dialog;
	GtkWidget *dialog_vbox;
	GtkWidget *hbox;
	GtkWidget *label_property;
	GtkWidget *combo_selector;
	GList *combo_selector_items = NULL;
	GtkWidget *combo_entry_selector;
	GtkObject *spinbutton_length_adj;
	GtkWidget *spinbutton_length;
	GtkWidget *combo__units;
	GList *combo__units_items = NULL;
	GtkWidget *combo_entry_units;
	GtkWidget *dialog_action_area;
	GtkWidget *helpbutton;
	GtkWidget *cancelbutton;
	GtkWidget *okbutton;
	GtkAccelGroup *accel_group;
	GtkWidget *main_window;
	CssedCssDialogsData *data = NULL;

	if (connect_callbacks)
	{
		data = g_malloc (sizeof (CssedCssDialogsData));
		data->main_window = window;
		data->previous_string = previous_string;
		data->add_property_string = add_previous_string;
	}


	accel_group = gtk_accel_group_new ();

	border_width_single_properties_dialog = gtk_dialog_new ();
	gtk_window_set_title (GTK_WINDOW
			      (border_width_single_properties_dialog),
			      previous_string);
	gtk_window_set_modal (GTK_WINDOW
			      (border_width_single_properties_dialog), TRUE);

	main_window = cssed_window_get_window_widget( window );

	gtk_window_set_transient_for (GTK_WINDOW
				      (border_width_single_properties_dialog),
				      GTK_WINDOW (main_window));

	dialog_vbox =
		GTK_DIALOG (border_width_single_properties_dialog)->vbox;
	gtk_widget_show (dialog_vbox);

	hbox = gtk_hbox_new (FALSE, 0);
	gtk_widget_show (hbox);
	gtk_box_pack_start (GTK_BOX (dialog_vbox), hbox, TRUE, TRUE, 0);

	label_property = gtk_label_new (previous_string);
	gtk_widget_show (label_property);
	gtk_box_pack_start (GTK_BOX (hbox), label_property, TRUE, TRUE, 0);
	gtk_label_set_justify (GTK_LABEL (label_property), GTK_JUSTIFY_LEFT);

	combo_selector = gtk_combo_new ();
	g_object_set_data (G_OBJECT (GTK_COMBO (combo_selector)->popwin),
			   "GladeParentKey", combo_selector);
	gtk_widget_show (combo_selector);
	gtk_box_pack_start (GTK_BOX (hbox), combo_selector, FALSE, FALSE, 5);
	combo_selector_items =
		g_list_append (combo_selector_items,
			       (gpointer) CSS_STR_USE_PERCLEN);
	combo_selector_items =
		g_list_append (combo_selector_items, (gpointer) "thin");
	combo_selector_items =
		g_list_append (combo_selector_items, (gpointer) "medium");
	combo_selector_items =
		g_list_append (combo_selector_items, (gpointer) "thick");
	gtk_combo_set_popdown_strings (GTK_COMBO (combo_selector),
				       combo_selector_items);
	g_list_free (combo_selector_items);

	combo_entry_selector = GTK_COMBO (combo_selector)->entry;
	gtk_widget_show (combo_entry_selector);
	gtk_editable_set_editable (GTK_EDITABLE (combo_entry_selector),
				   FALSE);
	gtk_entry_set_text (GTK_ENTRY (combo_entry_selector),
			    CSS_STR_USE_PERCLEN);

	spinbutton_length_adj =
		gtk_adjustment_new (1, -35565, 35565, 1, 10, 10);
	spinbutton_length =
		gtk_spin_button_new (GTK_ADJUSTMENT (spinbutton_length_adj),
				     1, 0);
	gtk_widget_show (spinbutton_length);
	gtk_box_pack_start (GTK_BOX (hbox), spinbutton_length, FALSE, FALSE,
			    5);
	gtk_widget_set_size_request (spinbutton_length, 46, -1);

	combo__units = gtk_combo_new ();
	g_object_set_data (G_OBJECT (GTK_COMBO (combo__units)->popwin),
			   "GladeParentKey", combo__units);
	gtk_widget_show (combo__units);
	gtk_box_pack_start (GTK_BOX (hbox), combo__units, FALSE, FALSE, 5);
	gtk_widget_set_size_request (combo__units, 50, -1);
	combo__units_items =
		g_list_append (combo__units_items, (gpointer) "%");
	combo__units_items =
		g_list_append (combo__units_items, (gpointer) "em");
	combo__units_items =
		g_list_append (combo__units_items, (gpointer) "ex");
	combo__units_items =
		g_list_append (combo__units_items, (gpointer) "px");
	combo__units_items =
		g_list_append (combo__units_items, (gpointer) "in");
	combo__units_items =
		g_list_append (combo__units_items, (gpointer) "cm");
	combo__units_items =
		g_list_append (combo__units_items, (gpointer) "mm");
	combo__units_items =
		g_list_append (combo__units_items, (gpointer) "pt");
	combo__units_items =
		g_list_append (combo__units_items, (gpointer) "pc");

	gtk_combo_set_popdown_strings (GTK_COMBO (combo__units),
				       combo__units_items);
	g_list_free (combo__units_items);

	combo_entry_units = GTK_COMBO (combo__units)->entry;
	gtk_widget_show (combo_entry_units);
	gtk_editable_set_editable (GTK_EDITABLE (combo_entry_units), FALSE);
	gtk_entry_set_text (GTK_ENTRY (combo_entry_units), "px");

	dialog_action_area =
		GTK_DIALOG (border_width_single_properties_dialog)->
		action_area;
	gtk_widget_show (dialog_action_area);
	gtk_button_box_set_layout (GTK_BUTTON_BOX (dialog_action_area),
				   GTK_BUTTONBOX_END);

	helpbutton = gtk_button_new_from_stock ("gtk-help");
	gtk_widget_show (helpbutton);
	gtk_dialog_add_action_widget (GTK_DIALOG
				      (border_width_single_properties_dialog),
				      helpbutton, GTK_RESPONSE_HELP);
	GTK_WIDGET_SET_FLAGS (helpbutton, GTK_CAN_DEFAULT);
	gtk_widget_add_accelerator (helpbutton, "clicked", accel_group,
				    GDK_y, GDK_CONTROL_MASK,
				    GTK_ACCEL_VISIBLE);
	gtk_widget_add_accelerator (helpbutton, "clicked", accel_group, GDK_h,
				    GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);

	cancelbutton = gtk_button_new_from_stock ("gtk-cancel");
	gtk_widget_show (cancelbutton);
	gtk_dialog_add_action_widget (GTK_DIALOG
				      (border_width_single_properties_dialog),
				      cancelbutton, GTK_RESPONSE_CANCEL);
	GTK_WIDGET_SET_FLAGS (cancelbutton, GTK_CAN_DEFAULT);
	gtk_widget_add_accelerator (cancelbutton, "clicked", accel_group,
				    GDK_c, GDK_CONTROL_MASK,
				    GTK_ACCEL_VISIBLE);

	okbutton = gtk_button_new_from_stock ("gtk-ok");
	gtk_widget_show (okbutton);
	gtk_dialog_add_action_widget (GTK_DIALOG
				      (border_width_single_properties_dialog),
				      okbutton, GTK_RESPONSE_OK);
	GTK_WIDGET_SET_FLAGS (okbutton, GTK_CAN_DEFAULT);
	gtk_widget_add_accelerator (okbutton, "clicked", accel_group,
				    GDK_o, GDK_CONTROL_MASK,
				    GTK_ACCEL_VISIBLE);


	g_signal_connect ((gpointer) combo_entry_units,
			  "changed", G_CALLBACK (on_css_dialog_unit_changed),
			  spinbutton_length);

	if (connect_callbacks)
	{
		g_signal_connect ((gpointer)
				  border_width_single_properties_dialog,
				  "close", G_CALLBACK (on_css_dialog_close),
				  data);

		g_signal_connect ((gpointer) helpbutton, "clicked",
				  G_CALLBACK
				  (on_border_width_single_helpbutton_clicked),
				  NULL);
		g_signal_connect ((gpointer) cancelbutton, "clicked",
				  G_CALLBACK
				  (on_css_dialog_cancel_button_clicked),
				  data);
		g_signal_connect ((gpointer) okbutton, "clicked",
				  G_CALLBACK
				  (on_border_width_single_okbutton_clicked),
				  data);
	}

	GLADE_HOOKUP_OBJECT_NO_REF (border_width_single_properties_dialog,
				    border_width_single_properties_dialog,
				    "border_width_single_properties_dialog");
	GLADE_HOOKUP_OBJECT_NO_REF (border_width_single_properties_dialog,
				    dialog_vbox, "dialog_vbox");
	GLADE_HOOKUP_OBJECT (border_width_single_properties_dialog, hbox,
			     "hbox");
	GLADE_HOOKUP_OBJECT (border_width_single_properties_dialog,
			     label_property, "label_property");
	GLADE_HOOKUP_OBJECT (border_width_single_properties_dialog,
			     combo_selector, "combo_selector");
	GLADE_HOOKUP_OBJECT (border_width_single_properties_dialog,
			     combo_entry_selector, "combo_entry_selector");
	GLADE_HOOKUP_OBJECT (border_width_single_properties_dialog,
			     spinbutton_length, "spinbutton_length");
	GLADE_HOOKUP_OBJECT (border_width_single_properties_dialog,
			     combo__units, "combo__units");
	GLADE_HOOKUP_OBJECT (border_width_single_properties_dialog,
			     combo_entry_units, "combo_entry_units");
	GLADE_HOOKUP_OBJECT_NO_REF (border_width_single_properties_dialog,
				    dialog_action_area, "dialog_action_area");
	GLADE_HOOKUP_OBJECT (border_width_single_properties_dialog,
			     helpbutton, "helpbutton");
	GLADE_HOOKUP_OBJECT (border_width_single_properties_dialog,
			     cancelbutton, "cancelbutton");
	GLADE_HOOKUP_OBJECT (border_width_single_properties_dialog, okbutton,
			     "okbutton");

	gtk_window_add_accel_group (GTK_WINDOW
				    (border_width_single_properties_dialog),
				    accel_group);

	return border_width_single_properties_dialog;
}

GtkWidget *
create_counter_increment_reset_dialog (CssedWindow * window,
				       gchar * previous_string,
				       gboolean
				       add_previous_string,
				       gboolean connect_callbacks)
{
	GtkWidget *counter_increment_reset_dialog;
	GtkWidget *dialog_vbox;
	GtkWidget *vbox;
	GtkWidget *hbox;
	GtkWidget *label_counter_increment_reset_property;
	GtkWidget *entry_counter_increment_reset_identifyer;
	GtkWidget *label_blank;
	GtkObject *spinbutton_counter_increment_reset_step_adj;
	GtkWidget *spinbutton_counter_increment_reset_step;
	GtkWidget *button_counter_increment_reset_add;
	GtkWidget *button_counter_increment_reset_del;
	GtkWidget *scrolledwindow;
	GtkWidget *treeview;
	GtkWidget *dialog_action_area;
	GtkWidget *helpbutton;
	GtkWidget *cancelbutton;
	GtkWidget *okbutton;
	GtkAccelGroup *accel_group;
	GtkWidget *main_window;
	CssedCssDialogsData *data = NULL;
	GtkTreeViewColumn *col_identifier;
	GtkTreeViewColumn *col_increment_index;
	GtkCellRenderer *identifier_renderer;
	GtkCellRenderer *increment_renderer;
	GtkListStore *store;


	if (connect_callbacks)
	{
		data = g_malloc (sizeof (CssedCssDialogsData));
		data->main_window = window;
		data->previous_string = previous_string;
		data->add_property_string = add_previous_string;
	}

	accel_group = gtk_accel_group_new ();

	counter_increment_reset_dialog = gtk_dialog_new ();
	gtk_window_set_title (GTK_WINDOW (counter_increment_reset_dialog),
			      previous_string);
	gtk_window_set_modal (GTK_WINDOW (counter_increment_reset_dialog),
			      TRUE);

	main_window = cssed_window_get_window_widget( window );

	gtk_window_set_transient_for (GTK_WINDOW
				      (counter_increment_reset_dialog),
				      GTK_WINDOW (main_window));

	dialog_vbox = GTK_DIALOG (counter_increment_reset_dialog)->vbox;
	gtk_widget_show (dialog_vbox);

	vbox = gtk_vbox_new (FALSE, 0);
	gtk_widget_show (vbox);
	gtk_box_pack_start (GTK_BOX (dialog_vbox), vbox, TRUE, TRUE, 0);

	hbox = gtk_hbox_new (FALSE, 0);
	gtk_widget_show (hbox);
	gtk_box_pack_start (GTK_BOX (vbox), hbox, FALSE, FALSE, 0);

	label_counter_increment_reset_property =
		gtk_label_new (previous_string);
	gtk_widget_show (label_counter_increment_reset_property);
	gtk_box_pack_start (GTK_BOX (hbox),
			    label_counter_increment_reset_property, TRUE,
			    TRUE, 0);
	gtk_label_set_justify (GTK_LABEL
			       (label_counter_increment_reset_property),
			       GTK_JUSTIFY_LEFT);

	entry_counter_increment_reset_identifyer = gtk_entry_new ();
	gtk_widget_show (entry_counter_increment_reset_identifyer);
	gtk_box_pack_start (GTK_BOX (hbox),
			    entry_counter_increment_reset_identifyer, FALSE,
			    FALSE, 5);

	label_blank = gtk_label_new (" ");
	gtk_widget_show (label_blank);
	gtk_box_pack_start (GTK_BOX (hbox), label_blank, FALSE, FALSE, 0);
	gtk_label_set_justify (GTK_LABEL (label_blank), GTK_JUSTIFY_LEFT);

	spinbutton_counter_increment_reset_step_adj =
		gtk_adjustment_new (1, -35565, 35565, 1, 10, 10);
	spinbutton_counter_increment_reset_step =
		gtk_spin_button_new (GTK_ADJUSTMENT
				     (spinbutton_counter_increment_reset_step_adj),
				     1, 0);
	gtk_widget_show (spinbutton_counter_increment_reset_step);
	gtk_box_pack_start (GTK_BOX (hbox),
			    spinbutton_counter_increment_reset_step, FALSE,
			    TRUE, 5);

	button_counter_increment_reset_add =
		gtk_button_new_from_stock ("gtk-add");
	gtk_widget_show (button_counter_increment_reset_add);
	gtk_box_pack_start (GTK_BOX (hbox),
			    button_counter_increment_reset_add, FALSE, FALSE,
			    4);
	gtk_widget_add_accelerator (button_counter_increment_reset_add,
				    "clicked", accel_group, GDK_A,
				    GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);

	button_counter_increment_reset_del =
		gtk_button_new_from_stock ("gtk-remove");
	gtk_widget_show (button_counter_increment_reset_del);
	gtk_box_pack_start (GTK_BOX (hbox),
			    button_counter_increment_reset_del, FALSE, FALSE,
			    0);
	gtk_widget_add_accelerator (button_counter_increment_reset_del,
				    "clicked", accel_group, GDK_Q,
				    GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);

	scrolledwindow = gtk_scrolled_window_new (NULL, NULL);
	gtk_widget_show (scrolledwindow);
	gtk_box_pack_start (GTK_BOX (vbox), scrolledwindow, TRUE, TRUE, 0);
	gtk_container_set_border_width (GTK_CONTAINER (scrolledwindow), 5);
	gtk_widget_set_size_request (scrolledwindow, -1, 125);

	store = gtk_list_store_new (2, G_TYPE_STRING, G_TYPE_INT);
	treeview = gtk_tree_view_new_with_model (GTK_TREE_MODEL (store));
	gtk_widget_show (treeview);
	gtk_container_add (GTK_CONTAINER (scrolledwindow), treeview);

	identifier_renderer = gtk_cell_renderer_text_new ();
	increment_renderer = gtk_cell_renderer_text_new ();

	col_identifier =
		gtk_tree_view_column_new_with_attributes ("Identifier",
							  identifier_renderer,
							  "text", 0, NULL);
	col_increment_index =
		gtk_tree_view_column_new_with_attributes ("Increment",
							  increment_renderer,
							  "text", 1, NULL);
	gtk_tree_view_insert_column (GTK_TREE_VIEW (treeview), col_identifier,
				     0);
	gtk_tree_view_insert_column (GTK_TREE_VIEW (treeview),
				     col_increment_index, 1);

	gtk_tree_view_set_headers_visible (GTK_TREE_VIEW (treeview), TRUE);

	dialog_action_area =
		GTK_DIALOG (counter_increment_reset_dialog)->action_area;
	gtk_widget_show (dialog_action_area);
	gtk_button_box_set_layout (GTK_BUTTON_BOX (dialog_action_area),
				   GTK_BUTTONBOX_END);

	helpbutton = gtk_button_new_from_stock ("gtk-help");
	gtk_widget_show (helpbutton);
	gtk_dialog_add_action_widget (GTK_DIALOG
				      (counter_increment_reset_dialog),
				      helpbutton, GTK_RESPONSE_HELP);
	GTK_WIDGET_SET_FLAGS (helpbutton, GTK_CAN_DEFAULT);
	gtk_widget_add_accelerator (helpbutton, "clicked", accel_group,
				    GDK_h, GDK_CONTROL_MASK,
				    GTK_ACCEL_VISIBLE);
	gtk_widget_add_accelerator (helpbutton, "clicked", accel_group, GDK_y,
				    GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);

	cancelbutton = gtk_button_new_from_stock ("gtk-cancel");
	gtk_widget_show (cancelbutton);
	gtk_dialog_add_action_widget (GTK_DIALOG
				      (counter_increment_reset_dialog),
				      cancelbutton, GTK_RESPONSE_CANCEL);
	GTK_WIDGET_SET_FLAGS (cancelbutton, GTK_CAN_DEFAULT);
	gtk_widget_add_accelerator (cancelbutton, "clicked", accel_group,
				    GDK_c, GDK_CONTROL_MASK,
				    GTK_ACCEL_VISIBLE);

	okbutton = gtk_button_new_from_stock ("gtk-ok");
	gtk_widget_show (okbutton);
	gtk_dialog_add_action_widget (GTK_DIALOG
				      (counter_increment_reset_dialog),
				      okbutton, GTK_RESPONSE_OK);
	GTK_WIDGET_SET_FLAGS (okbutton, GTK_CAN_DEFAULT);
	gtk_widget_add_accelerator (okbutton, "clicked", accel_group,
				    GDK_O, GDK_CONTROL_MASK,
				    GTK_ACCEL_VISIBLE);
	if (connect_callbacks)
	{
		g_signal_connect ((gpointer) counter_increment_reset_dialog,
				  "close", G_CALLBACK (on_css_dialog_close),
				  data);

		g_signal_connect ((gpointer) helpbutton, "clicked",
				  G_CALLBACK
				  (on_dialog_counter_increment_reset_helpbutton_clicked),
				  NULL);
		g_signal_connect ((gpointer) cancelbutton, "clicked",
				  G_CALLBACK
				  (on_css_dialog_cancel_button_clicked),
				  data);
		g_signal_connect ((gpointer) okbutton, "clicked",
				  G_CALLBACK
				  (on_dialog_counter_increment_reset_okbutton_clicked),
				  data);
	}

	g_signal_connect ((gpointer) button_counter_increment_reset_add,
			  "clicked",
			  G_CALLBACK
			  (on_button_counter_increment_reset_add_clicked),
			  store);
	g_signal_connect ((gpointer) button_counter_increment_reset_del,
			  "clicked",
			  G_CALLBACK
			  (on_button_counter_increment_reset_del_clicked),
			  store);

	GLADE_HOOKUP_OBJECT_NO_REF (counter_increment_reset_dialog,
				    counter_increment_reset_dialog,
				    "counter_increment_reset_dialog");
	GLADE_HOOKUP_OBJECT_NO_REF (counter_increment_reset_dialog,
				    dialog_vbox, "dialog_vbox");
	GLADE_HOOKUP_OBJECT (counter_increment_reset_dialog, vbox, "vbox");
	GLADE_HOOKUP_OBJECT (counter_increment_reset_dialog, hbox, "hbox");
	GLADE_HOOKUP_OBJECT (counter_increment_reset_dialog,
			     label_counter_increment_reset_property,
			     "label_counter_increment_reset_property");
	GLADE_HOOKUP_OBJECT (counter_increment_reset_dialog,
			     entry_counter_increment_reset_identifyer,
			     "entry_counter_increment_reset_identifyer");
	GLADE_HOOKUP_OBJECT (counter_increment_reset_dialog, label_blank,
			     "label_blank");
	GLADE_HOOKUP_OBJECT (counter_increment_reset_dialog,
			     spinbutton_counter_increment_reset_step,
			     "spinbutton_counter_increment_reset_step");
	GLADE_HOOKUP_OBJECT (counter_increment_reset_dialog,
			     button_counter_increment_reset_add,
			     "button_counter_increment_reset_add");
	GLADE_HOOKUP_OBJECT (counter_increment_reset_dialog,
			     button_counter_increment_reset_del,
			     "button_counter_increment_reset_del");
	GLADE_HOOKUP_OBJECT (counter_increment_reset_dialog, scrolledwindow,
			     "scrolledwindow");
	GLADE_HOOKUP_OBJECT (counter_increment_reset_dialog, treeview,
			     "treeview");
	GLADE_HOOKUP_OBJECT_NO_REF (counter_increment_reset_dialog,
				    dialog_action_area, "dialog_action_area");
	GLADE_HOOKUP_OBJECT (counter_increment_reset_dialog, helpbutton,
			     "helpbutton");
	GLADE_HOOKUP_OBJECT (counter_increment_reset_dialog, cancelbutton,
			     "cancelbutton");
	GLADE_HOOKUP_OBJECT (counter_increment_reset_dialog, okbutton,
			     "okbutton");

	gtk_window_add_accel_group (GTK_WINDOW
				    (counter_increment_reset_dialog),
				    accel_group);

	return counter_increment_reset_dialog;
}

GtkWidget *
create_string_properties_dialog (CssedWindow * window,
				 gchar * previous_string,
				 gboolean add_previous_string,
				 gboolean connect_callbacks)
{
	GtkWidget *string_properties_dialog;
	GtkWidget *dialog_vbox;
	GtkWidget *hbox;
	GtkWidget *label_property;
	GtkWidget *entry_string_property_dialog;
	GtkWidget *dialog_action_area;
	GtkWidget *helpbutton;
	GtkWidget *cancelbutton;
	GtkWidget *okbutton;
	GtkAccelGroup *accel_group;
	GtkWidget *main_window;
	CssedCssDialogsData *data = NULL;

	if (connect_callbacks)
	{
		data = g_malloc (sizeof (CssedCssDialogsData));
		data->main_window = window;
		data->previous_string = previous_string;
		data->add_property_string = add_previous_string;
	}


	accel_group = gtk_accel_group_new ();

	string_properties_dialog = gtk_dialog_new ();
	gtk_window_set_title (GTK_WINDOW (string_properties_dialog),
			      previous_string);
	gtk_window_set_modal (GTK_WINDOW (string_properties_dialog), TRUE);

	main_window = cssed_window_get_window_widget( window );

	gtk_window_set_transient_for (GTK_WINDOW (string_properties_dialog),
				      GTK_WINDOW (main_window));

	dialog_vbox = GTK_DIALOG (string_properties_dialog)->vbox;
	gtk_widget_show (dialog_vbox);

	hbox = gtk_hbox_new (TRUE, 0);
	gtk_widget_show (hbox);
	gtk_box_pack_start (GTK_BOX (dialog_vbox), hbox, TRUE, TRUE, 0);

	label_property = gtk_label_new (previous_string);
	gtk_widget_show (label_property);
	gtk_box_pack_start (GTK_BOX (hbox), label_property, TRUE, TRUE, 0);
	gtk_label_set_justify (GTK_LABEL (label_property), GTK_JUSTIFY_LEFT);

	entry_string_property_dialog = gtk_entry_new ();
	gtk_widget_show (entry_string_property_dialog);
	gtk_box_pack_start (GTK_BOX (hbox), entry_string_property_dialog,
			    TRUE, TRUE, 5);
	gtk_entry_set_activates_default (GTK_ENTRY
					 (entry_string_property_dialog),
					 TRUE);

	dialog_action_area =
		GTK_DIALOG (string_properties_dialog)->action_area;
	gtk_widget_show (dialog_action_area);
	gtk_button_box_set_layout (GTK_BUTTON_BOX (dialog_action_area),
				   GTK_BUTTONBOX_END);

	helpbutton = gtk_button_new_from_stock ("gtk-help");
	gtk_widget_show (helpbutton);
	gtk_dialog_add_action_widget (GTK_DIALOG (string_properties_dialog),
				      helpbutton, GTK_RESPONSE_HELP);
	GTK_WIDGET_SET_FLAGS (helpbutton, GTK_CAN_DEFAULT);
	gtk_widget_add_accelerator (helpbutton, "clicked", accel_group,
				    GDK_y, GDK_CONTROL_MASK,
				    GTK_ACCEL_VISIBLE);
	gtk_widget_add_accelerator (helpbutton, "clicked", accel_group, GDK_h,
				    GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);

	cancelbutton = gtk_button_new_from_stock ("gtk-cancel");
	gtk_widget_show (cancelbutton);
	gtk_dialog_add_action_widget (GTK_DIALOG (string_properties_dialog),
				      cancelbutton, GTK_RESPONSE_CANCEL);
	GTK_WIDGET_SET_FLAGS (cancelbutton, GTK_CAN_DEFAULT);
	gtk_widget_add_accelerator (cancelbutton, "clicked", accel_group,
				    GDK_c, GDK_CONTROL_MASK,
				    GTK_ACCEL_VISIBLE);

	okbutton = gtk_button_new_from_stock ("gtk-ok");
	gtk_widget_show (okbutton);
	gtk_dialog_add_action_widget (GTK_DIALOG (string_properties_dialog),
				      okbutton, GTK_RESPONSE_OK);
	GTK_WIDGET_SET_FLAGS (okbutton, GTK_CAN_DEFAULT);
	gtk_widget_add_accelerator (okbutton, "clicked", accel_group,
				    GDK_o, GDK_CONTROL_MASK,
				    GTK_ACCEL_VISIBLE);
	if (connect_callbacks)
	{
		g_signal_connect ((gpointer) string_properties_dialog,
				  "close", G_CALLBACK (on_css_dialog_close),
				  data);

		g_signal_connect ((gpointer) helpbutton, "clicked",
				  G_CALLBACK
				  (on_string_properties_dialog_helpbutton_clicked),
				  NULL);
		g_signal_connect ((gpointer) cancelbutton, "clicked",
				  G_CALLBACK
				  (on_css_dialog_cancel_button_clicked),
				  data);
		g_signal_connect ((gpointer) okbutton, "clicked",
				  G_CALLBACK
				  (on_string_properties_dialog_okbutton_clicked),
				  data);
	}

	GLADE_HOOKUP_OBJECT_NO_REF (string_properties_dialog,
				    string_properties_dialog,
				    "string_properties_dialog");
	GLADE_HOOKUP_OBJECT_NO_REF (string_properties_dialog, dialog_vbox,
				    "dialog_vbox");
	GLADE_HOOKUP_OBJECT (string_properties_dialog, hbox, "hbox");
	GLADE_HOOKUP_OBJECT (string_properties_dialog, label_property,
			     "label_property");
	GLADE_HOOKUP_OBJECT (string_properties_dialog,
			     entry_string_property_dialog,
			     "entry_string_property_dialog");
	GLADE_HOOKUP_OBJECT_NO_REF (string_properties_dialog,
				    dialog_action_area, "dialog_action_area");
	GLADE_HOOKUP_OBJECT (string_properties_dialog, helpbutton,
			     "helpbutton");
	GLADE_HOOKUP_OBJECT (string_properties_dialog, cancelbutton,
			     "cancelbutton");
	GLADE_HOOKUP_OBJECT (string_properties_dialog, okbutton, "okbutton");

	gtk_window_add_accel_group (GTK_WINDOW (string_properties_dialog),
				    accel_group);

	return string_properties_dialog;
}

GtkWidget *
create_uri_value_dialog (CssedWindow * window,
			 gchar * previous_string,
			 gboolean add_previous_string,
			 gboolean connect_callbacks)
{
	GtkWidget *uri_value_dialog;
	GtkWidget *dialog_vbox;
	GtkWidget *hbox26;
	GtkWidget *label_uri;
	GtkWidget *entry_uri_value_dialog;
	GtkWidget *dialog_action_area;
	GtkWidget *helpbutton;
	GtkWidget *cancelbutton;
	GtkWidget *okbutton;
	GtkAccelGroup *accel_group;
	GtkWidget *main_window;
	CssedCssDialogsData *data = NULL;

	if (connect_callbacks)
	{
		data = g_malloc (sizeof (CssedCssDialogsData));
		data->main_window = window;
		data->previous_string = previous_string;
		data->add_property_string = add_previous_string;
	}

	accel_group = gtk_accel_group_new ();

	uri_value_dialog = gtk_dialog_new ();
	gtk_window_set_title (GTK_WINDOW (uri_value_dialog), previous_string);
	gtk_window_set_modal (GTK_WINDOW (uri_value_dialog), TRUE);

	main_window = cssed_window_get_window_widget( window );

	gtk_window_set_transient_for (GTK_WINDOW (uri_value_dialog),
				      GTK_WINDOW (main_window));

	dialog_vbox = GTK_DIALOG (uri_value_dialog)->vbox;
	gtk_widget_show (dialog_vbox);

	hbox26 = gtk_hbox_new (FALSE, 0);
	gtk_widget_show (hbox26);
	gtk_box_pack_start (GTK_BOX (dialog_vbox), hbox26, TRUE, TRUE, 0);

	label_uri = gtk_label_new ("uri");
	gtk_widget_show (label_uri);
	gtk_box_pack_start (GTK_BOX (hbox26), label_uri, TRUE, TRUE, 0);
	gtk_label_set_justify (GTK_LABEL (label_uri), GTK_JUSTIFY_LEFT);

	entry_uri_value_dialog = gtk_entry_new ();
	gtk_widget_show (entry_uri_value_dialog);
	gtk_box_pack_start (GTK_BOX (hbox26), entry_uri_value_dialog, TRUE,
			    TRUE, 5);
	gtk_entry_set_activates_default (GTK_ENTRY (entry_uri_value_dialog),
					 TRUE);

	dialog_action_area = GTK_DIALOG (uri_value_dialog)->action_area;
	gtk_widget_show (dialog_action_area);
	gtk_button_box_set_layout (GTK_BUTTON_BOX (dialog_action_area),
				   GTK_BUTTONBOX_END);

	helpbutton = gtk_button_new_from_stock ("gtk-help");
	gtk_widget_show (helpbutton);
	gtk_dialog_add_action_widget (GTK_DIALOG (uri_value_dialog),
				      helpbutton, GTK_RESPONSE_HELP);
	GTK_WIDGET_SET_FLAGS (helpbutton, GTK_CAN_DEFAULT);
	gtk_widget_add_accelerator (helpbutton, "clicked", accel_group,
				    GDK_y, GDK_CONTROL_MASK,
				    GTK_ACCEL_VISIBLE);
	gtk_widget_add_accelerator (helpbutton, "clicked", accel_group, GDK_h,
				    GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);

	cancelbutton = gtk_button_new_from_stock ("gtk-cancel");
	gtk_widget_show (cancelbutton);
	gtk_dialog_add_action_widget (GTK_DIALOG (uri_value_dialog),
				      cancelbutton, GTK_RESPONSE_CANCEL);
	GTK_WIDGET_SET_FLAGS (cancelbutton, GTK_CAN_DEFAULT);
	gtk_widget_add_accelerator (cancelbutton, "clicked", accel_group,
				    GDK_c, GDK_CONTROL_MASK,
				    GTK_ACCEL_VISIBLE);

	okbutton = gtk_button_new_from_stock ("gtk-ok");
	gtk_widget_show (okbutton);
	gtk_dialog_add_action_widget (GTK_DIALOG (uri_value_dialog), okbutton,
				      GTK_RESPONSE_OK);
	GTK_WIDGET_SET_FLAGS (okbutton, GTK_CAN_DEFAULT);
	gtk_widget_add_accelerator (okbutton, "clicked", accel_group,
				    GDK_o, GDK_CONTROL_MASK,
				    GTK_ACCEL_VISIBLE);
	if (connect_callbacks)
	{
		g_signal_connect ((gpointer) uri_value_dialog, "close",
				  G_CALLBACK (on_css_dialog_close), data);

		g_signal_connect ((gpointer) helpbutton, "clicked",
				  G_CALLBACK
				  (on_uri_value_dialog_helpbutton_clicked),
				  NULL);
		g_signal_connect ((gpointer) cancelbutton, "clicked",
				  G_CALLBACK
				  (on_css_dialog_cancel_button_clicked),
				  data);
		g_signal_connect ((gpointer) okbutton, "clicked",
				  G_CALLBACK
				  (on_uri_value_dialog_okbutton_clicked),
				  data);
	}
	GLADE_HOOKUP_OBJECT_NO_REF (uri_value_dialog, uri_value_dialog,
				    "uri_value_dialog");
	GLADE_HOOKUP_OBJECT_NO_REF (uri_value_dialog, dialog_vbox,
				    "dialog_vbox");
	GLADE_HOOKUP_OBJECT (uri_value_dialog, hbox26, "hbox26");
	GLADE_HOOKUP_OBJECT (uri_value_dialog, label_uri, "label_uri");
	GLADE_HOOKUP_OBJECT (uri_value_dialog, entry_uri_value_dialog,
			     "entry_uri_value_dialog");
	GLADE_HOOKUP_OBJECT_NO_REF (uri_value_dialog, dialog_action_area,
				    "dialog_action_area");
	GLADE_HOOKUP_OBJECT (uri_value_dialog, helpbutton, "helpbutton");
	GLADE_HOOKUP_OBJECT (uri_value_dialog, cancelbutton, "cancelbutton");
	GLADE_HOOKUP_OBJECT (uri_value_dialog, okbutton, "okbutton");

	gtk_window_add_accel_group (GTK_WINDOW (uri_value_dialog),
				    accel_group);

	return uri_value_dialog;
}

GtkWidget *
create_attr_value_dialog (CssedWindow * window,
			  gchar * previous_string,
			  gboolean add_previous_string,
			  gboolean connect_callbacks)
{
	GtkWidget *attr_value_dialog;
	GtkWidget *dialog_vbox;
	GtkWidget *hbox;
	GtkWidget *label_attribute;
	GtkWidget *entry_attribute_value_dialog;
	GtkWidget *dialog_action_area;
	GtkWidget *helpbutton;
	GtkWidget *cancelbutton;
	GtkWidget *okbutton;
	GtkAccelGroup *accel_group;
	GtkWidget *main_window;
	CssedCssDialogsData *data = NULL;

	if (connect_callbacks)
	{
		data = g_malloc (sizeof (CssedCssDialogsData));
		data->main_window = window;
		data->previous_string = previous_string;
		data->add_property_string = add_previous_string;
	}

	accel_group = gtk_accel_group_new ();

	attr_value_dialog = gtk_dialog_new ();
	gtk_window_set_title (GTK_WINDOW (attr_value_dialog),
			      previous_string);
	gtk_window_set_modal (GTK_WINDOW (attr_value_dialog), TRUE);

	main_window = cssed_window_get_window_widget( window );

	gtk_window_set_transient_for (GTK_WINDOW (attr_value_dialog),
				      GTK_WINDOW (main_window));

	dialog_vbox = GTK_DIALOG (attr_value_dialog)->vbox;
	gtk_widget_show (dialog_vbox);

	hbox = gtk_hbox_new (FALSE, 0);
	gtk_widget_show (hbox);
	gtk_box_pack_start (GTK_BOX (dialog_vbox), hbox, TRUE, TRUE, 0);

	label_attribute = gtk_label_new (_("Attribute"));
	gtk_widget_show (label_attribute);
	gtk_box_pack_start (GTK_BOX (hbox), label_attribute, TRUE, TRUE, 0);
	gtk_label_set_justify (GTK_LABEL (label_attribute), GTK_JUSTIFY_LEFT);

	entry_attribute_value_dialog = gtk_entry_new ();
	gtk_widget_show (entry_attribute_value_dialog);
	gtk_box_pack_start (GTK_BOX (hbox), entry_attribute_value_dialog,
			    TRUE, TRUE, 4);
	gtk_entry_set_activates_default (GTK_ENTRY
					 (entry_attribute_value_dialog),
					 TRUE);

	dialog_action_area = GTK_DIALOG (attr_value_dialog)->action_area;
	gtk_widget_show (dialog_action_area);
	gtk_button_box_set_layout (GTK_BUTTON_BOX (dialog_action_area),
				   GTK_BUTTONBOX_END);

	helpbutton = gtk_button_new_from_stock ("gtk-help");
	gtk_widget_show (helpbutton);
	gtk_dialog_add_action_widget (GTK_DIALOG (attr_value_dialog),
				      helpbutton, GTK_RESPONSE_HELP);
	GTK_WIDGET_SET_FLAGS (helpbutton, GTK_CAN_DEFAULT);
	gtk_widget_add_accelerator (helpbutton, "clicked", accel_group,
				    GDK_y, GDK_CONTROL_MASK,
				    GTK_ACCEL_VISIBLE);
	gtk_widget_add_accelerator (helpbutton, "clicked", accel_group, GDK_h,
				    GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);

	cancelbutton = gtk_button_new_from_stock ("gtk-cancel");
	gtk_widget_show (cancelbutton);
	gtk_dialog_add_action_widget (GTK_DIALOG (attr_value_dialog),
				      cancelbutton, GTK_RESPONSE_CANCEL);
	GTK_WIDGET_SET_FLAGS (cancelbutton, GTK_CAN_DEFAULT);
	gtk_widget_add_accelerator (cancelbutton, "clicked", accel_group,
				    GDK_c, GDK_CONTROL_MASK,
				    GTK_ACCEL_VISIBLE);

	okbutton = gtk_button_new_from_stock ("gtk-ok");
	gtk_widget_show (okbutton);
	gtk_dialog_add_action_widget (GTK_DIALOG (attr_value_dialog),
				      okbutton, GTK_RESPONSE_OK);
	GTK_WIDGET_SET_FLAGS (okbutton, GTK_CAN_DEFAULT);
	gtk_widget_add_accelerator (okbutton, "clicked", accel_group,
				    GDK_o, GDK_CONTROL_MASK,
				    GTK_ACCEL_VISIBLE);
	if (connect_callbacks)
	{
		g_signal_connect ((gpointer) attr_value_dialog, "close",
				  G_CALLBACK (on_css_dialog_close), data);

		g_signal_connect ((gpointer) helpbutton, "clicked",
				  G_CALLBACK
				  (on_attr_value_dialog_helpbutton_clicked),
				  NULL);
		g_signal_connect ((gpointer) cancelbutton, "clicked",
				  G_CALLBACK
				  (on_css_dialog_cancel_button_clicked),
				  data);
		g_signal_connect ((gpointer) okbutton, "clicked",
				  G_CALLBACK
				  (on_attr_value_dialog_okbutton_clicked),
				  data);
	}

	GLADE_HOOKUP_OBJECT_NO_REF (attr_value_dialog, attr_value_dialog,
				    "attr_value_dialog");
	GLADE_HOOKUP_OBJECT_NO_REF (attr_value_dialog, dialog_vbox,
				    "dialog_vbox");
	GLADE_HOOKUP_OBJECT (attr_value_dialog, hbox, "hbox");
	GLADE_HOOKUP_OBJECT (attr_value_dialog, label_attribute,
			     "label_attribute");
	GLADE_HOOKUP_OBJECT (attr_value_dialog, entry_attribute_value_dialog,
			     "entry_attribute_value_dialog");
	GLADE_HOOKUP_OBJECT_NO_REF (attr_value_dialog, dialog_action_area,
				    "dialog_action_area");
	GLADE_HOOKUP_OBJECT (attr_value_dialog, helpbutton, "helpbutton");
	GLADE_HOOKUP_OBJECT (attr_value_dialog, cancelbutton, "cancelbutton");
	GLADE_HOOKUP_OBJECT (attr_value_dialog, okbutton, "okbutton");

	gtk_window_add_accel_group (GTK_WINDOW (attr_value_dialog),
				    accel_group);

	return attr_value_dialog;
}

GtkWidget *
create_counter_property_dialog (CssedWindow * window,
				gchar * previous_string,
				gboolean add_previous_string,
				gboolean connect_callbacks)
{
	GtkWidget *counter_property_dialog;
	GtkWidget *dialog_vbox;
	GtkWidget *vbox_main;
	GtkWidget *hbox_counter;
	GtkWidget *label_counter;
	GtkWidget *entry_counter_identifier;
	GtkWidget *hbox_after;
	GtkWidget *checkbutton_after;
	GtkWidget *entry_string_after;
	GtkWidget *hbox_before;
	GtkWidget *checkbutton_before;
	GtkWidget *entry_string_before;
	GtkWidget *hbox_list_style;
	GtkWidget *checkbutton_list_style_tyle;
	GtkWidget *combo_list_style;
	GList *combo_list_style_items = NULL;
	GtkWidget *combo_entry_list_style_type;
	GtkWidget *dialog_action_area;
	GtkWidget *helpbutton;
	GtkWidget *cancelbutton;
	GtkWidget *okbutton;
	GtkAccelGroup *accel_group;
	GtkWidget *main_window;
	CssedCssDialogsData *data = NULL;

	if (connect_callbacks)
	{
		data = g_malloc (sizeof (CssedCssDialogsData));
		data->main_window = window;
		data->previous_string = previous_string;
		data->add_property_string = add_previous_string;
	}


	accel_group = gtk_accel_group_new ();

	counter_property_dialog = gtk_dialog_new ();
	gtk_window_set_title (GTK_WINDOW (counter_property_dialog),
			      previous_string);
	gtk_window_set_modal (GTK_WINDOW (counter_property_dialog), TRUE);

	main_window = cssed_window_get_window_widget( window );

	gtk_window_set_transient_for (GTK_WINDOW (counter_property_dialog),
				      GTK_WINDOW (main_window));

	dialog_vbox = GTK_DIALOG (counter_property_dialog)->vbox;
	gtk_widget_show (dialog_vbox);

	vbox_main = gtk_vbox_new (FALSE, 0);
	gtk_widget_show (vbox_main);
	gtk_box_pack_start (GTK_BOX (dialog_vbox), vbox_main, TRUE, TRUE, 0);

	hbox_counter = gtk_hbox_new (TRUE, 0);
	gtk_widget_show (hbox_counter);
	gtk_box_pack_start (GTK_BOX (vbox_main), hbox_counter, TRUE, TRUE, 0);

	label_counter = gtk_label_new (_("Counter identifier"));
	gtk_widget_show (label_counter);
	gtk_box_pack_start (GTK_BOX (hbox_counter), label_counter, TRUE, TRUE,
			    0);
	gtk_label_set_justify (GTK_LABEL (label_counter), GTK_JUSTIFY_LEFT);

	entry_counter_identifier = gtk_entry_new ();
	gtk_widget_show (entry_counter_identifier);
	gtk_box_pack_start (GTK_BOX (hbox_counter), entry_counter_identifier,
			    TRUE, TRUE, 5);

	hbox_after = gtk_hbox_new (TRUE, 0);
	gtk_widget_show (hbox_after);
	gtk_box_pack_start (GTK_BOX (vbox_main), hbox_after, TRUE, TRUE, 0);

	checkbutton_after =
		gtk_check_button_new_with_mnemonic (_("String after"));
	gtk_widget_show (checkbutton_after);
	gtk_box_pack_start (GTK_BOX (hbox_after), checkbutton_after, TRUE,
			    TRUE, 5);

	entry_string_after = gtk_entry_new ();
	gtk_widget_show (entry_string_after);
	gtk_box_pack_start (GTK_BOX (hbox_after), entry_string_after, TRUE,
			    TRUE, 5);

	hbox_before = gtk_hbox_new (TRUE, 0);
	gtk_widget_show (hbox_before);
	gtk_box_pack_start (GTK_BOX (vbox_main), hbox_before, TRUE, TRUE, 0);

	checkbutton_before =
		gtk_check_button_new_with_mnemonic (_("String before"));
	gtk_widget_show (checkbutton_before);
	gtk_box_pack_start (GTK_BOX (hbox_before), checkbutton_before, TRUE,
			    TRUE, 5);

	entry_string_before = gtk_entry_new ();
	gtk_widget_show (entry_string_before);
	gtk_box_pack_start (GTK_BOX (hbox_before), entry_string_before, TRUE,
			    TRUE, 5);

	hbox_list_style = gtk_hbox_new (TRUE, 0);
	gtk_widget_show (hbox_list_style);
	gtk_box_pack_start (GTK_BOX (vbox_main), hbox_list_style, TRUE, TRUE,
			    0);

	checkbutton_list_style_tyle =
		gtk_check_button_new_with_mnemonic (_("list style type"));
	gtk_widget_show (checkbutton_list_style_tyle);
	gtk_box_pack_start (GTK_BOX (hbox_list_style),
			    checkbutton_list_style_tyle, TRUE, TRUE, 5);

	combo_list_style = gtk_combo_new ();
	g_object_set_data (G_OBJECT (GTK_COMBO (combo_list_style)->popwin),
			   "GladeParentKey", combo_list_style);
	gtk_widget_show (combo_list_style);
	gtk_box_pack_start (GTK_BOX (hbox_list_style), combo_list_style, TRUE,
			    TRUE, 5);
	combo_list_style_items =
		g_list_append (combo_list_style_items, (gpointer) "disc");
	combo_list_style_items =
		g_list_append (combo_list_style_items, (gpointer) "circle");
	combo_list_style_items =
		g_list_append (combo_list_style_items, (gpointer) "square");
	combo_list_style_items =
		g_list_append (combo_list_style_items, (gpointer) "decimal");
	combo_list_style_items =
		g_list_append (combo_list_style_items,
			       (gpointer) "decimal-leading-zero");
	combo_list_style_items =
		g_list_append (combo_list_style_items,
			       (gpointer) "lower-roman ");
	combo_list_style_items =
		g_list_append (combo_list_style_items,
			       (gpointer) "upper-roman");
	combo_list_style_items =
		g_list_append (combo_list_style_items,
			       (gpointer) "lower-greek");
	combo_list_style_items =
		g_list_append (combo_list_style_items,
			       (gpointer) "lower-alpha");
	combo_list_style_items =
		g_list_append (combo_list_style_items,
			       (gpointer) "lower-latin");
	combo_list_style_items =
		g_list_append (combo_list_style_items,
			       (gpointer) "upper-alpha");
	combo_list_style_items =
		g_list_append (combo_list_style_items,
			       (gpointer) "upper-latin");
	combo_list_style_items =
		g_list_append (combo_list_style_items, (gpointer) "hebrew");
	combo_list_style_items =
		g_list_append (combo_list_style_items, (gpointer) "armenian");
	combo_list_style_items =
		g_list_append (combo_list_style_items, (gpointer) "georgian");
	combo_list_style_items =
		g_list_append (combo_list_style_items,
			       (gpointer) "cjk-ideographic");
	combo_list_style_items =
		g_list_append (combo_list_style_items, (gpointer) "hiragana");
	combo_list_style_items =
		g_list_append (combo_list_style_items, (gpointer) "katakana");
	combo_list_style_items =
		g_list_append (combo_list_style_items,
			       (gpointer) "hiragana-iroha ");
	combo_list_style_items =
		g_list_append (combo_list_style_items,
			       (gpointer) "katakana-iroha");
	combo_list_style_items =
		g_list_append (combo_list_style_items, (gpointer) "none");
	combo_list_style_items =
		g_list_append (combo_list_style_items, (gpointer) "inherit");
	gtk_combo_set_popdown_strings (GTK_COMBO (combo_list_style),
				       combo_list_style_items);
	g_list_free (combo_list_style_items);

	combo_entry_list_style_type = GTK_COMBO (combo_list_style)->entry;
	gtk_widget_show (combo_entry_list_style_type);
	gtk_entry_set_text (GTK_ENTRY (combo_entry_list_style_type), "disc");

	dialog_action_area =
		GTK_DIALOG (counter_property_dialog)->action_area;
	gtk_widget_show (dialog_action_area);
	gtk_button_box_set_layout (GTK_BUTTON_BOX (dialog_action_area),
				   GTK_BUTTONBOX_END);

	helpbutton = gtk_button_new_from_stock ("gtk-help");
	gtk_widget_show (helpbutton);
	gtk_dialog_add_action_widget (GTK_DIALOG (counter_property_dialog),
				      helpbutton, GTK_RESPONSE_HELP);
	GTK_WIDGET_SET_FLAGS (helpbutton, GTK_CAN_DEFAULT);
	gtk_widget_add_accelerator (helpbutton, "clicked", accel_group,
				    GDK_y, GDK_CONTROL_MASK,
				    GTK_ACCEL_VISIBLE);
	gtk_widget_add_accelerator (helpbutton, "clicked", accel_group, GDK_h,
				    GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);

	cancelbutton = gtk_button_new_from_stock ("gtk-cancel");
	gtk_widget_show (cancelbutton);
	gtk_dialog_add_action_widget (GTK_DIALOG (counter_property_dialog),
				      cancelbutton, GTK_RESPONSE_CANCEL);
	GTK_WIDGET_SET_FLAGS (cancelbutton, GTK_CAN_DEFAULT);
	gtk_widget_add_accelerator (cancelbutton, "clicked", accel_group,
				    GDK_c, GDK_CONTROL_MASK,
				    GTK_ACCEL_VISIBLE);

	okbutton = gtk_button_new_from_stock ("gtk-ok");
	gtk_widget_show (okbutton);
	gtk_dialog_add_action_widget (GTK_DIALOG (counter_property_dialog),
				      okbutton, GTK_RESPONSE_OK);
	GTK_WIDGET_SET_FLAGS (okbutton, GTK_CAN_DEFAULT);
	gtk_widget_add_accelerator (okbutton, "clicked", accel_group,
				    GDK_o, GDK_CONTROL_MASK,
				    GTK_ACCEL_VISIBLE);
				    
				    
	g_signal_connect ((gpointer) entry_string_after,
			  "changed", G_CALLBACK (on_widget_change_set_toggle_active),
			  checkbutton_after);
			  
	g_signal_connect ((gpointer) entry_string_before,
			  "changed", G_CALLBACK (on_widget_change_set_toggle_active),
			  checkbutton_before);
			  
	g_signal_connect ((gpointer) combo_entry_list_style_type,
			  "changed", G_CALLBACK (on_widget_change_set_toggle_active),
			  checkbutton_list_style_tyle);	
			  		  
	if (connect_callbacks)
	{
		g_signal_connect ((gpointer) counter_property_dialog, "close",
				  G_CALLBACK (on_css_dialog_close), data);

		g_signal_connect ((gpointer) helpbutton, "clicked",
				  G_CALLBACK
				  (on_counter_property_dialog_helpbutton_clicked),
				  NULL);
		g_signal_connect ((gpointer) cancelbutton, "clicked",
				  G_CALLBACK
				  (on_css_dialog_cancel_button_clicked),
				  data);
		g_signal_connect ((gpointer) okbutton, "clicked",
				  G_CALLBACK
				  (on_counter_property_dialog_okbutton_clicked),
				  data);
	}

	GLADE_HOOKUP_OBJECT_NO_REF (counter_property_dialog,
				    counter_property_dialog,
				    "counter_property_dialog");
	GLADE_HOOKUP_OBJECT_NO_REF (counter_property_dialog, dialog_vbox,
				    "dialog_vbox");
	GLADE_HOOKUP_OBJECT (counter_property_dialog, vbox_main, "vbox_main");
	GLADE_HOOKUP_OBJECT (counter_property_dialog, hbox_counter,
			     "hbox_counter");
	GLADE_HOOKUP_OBJECT (counter_property_dialog, label_counter,
			     "label_counter");
	GLADE_HOOKUP_OBJECT (counter_property_dialog,
			     entry_counter_identifier,
			     "entry_counter_identifier");
	GLADE_HOOKUP_OBJECT (counter_property_dialog, hbox_after,
			     "hbox_after");
	GLADE_HOOKUP_OBJECT (counter_property_dialog, checkbutton_after,
			     "checkbutton_after");
	GLADE_HOOKUP_OBJECT (counter_property_dialog, entry_string_after,
			     "entry_string_after");
	GLADE_HOOKUP_OBJECT (counter_property_dialog, hbox_before,
			     "hbox_before");
	GLADE_HOOKUP_OBJECT (counter_property_dialog, checkbutton_before,
			     "checkbutton_before");
	GLADE_HOOKUP_OBJECT (counter_property_dialog, entry_string_before,
			     "entry_string_before");
	GLADE_HOOKUP_OBJECT (counter_property_dialog, hbox_list_style,
			     "hbox_list_style");
	GLADE_HOOKUP_OBJECT (counter_property_dialog,
			     checkbutton_list_style_tyle,
			     "checkbutton_list_style_tyle");
	GLADE_HOOKUP_OBJECT (counter_property_dialog, combo_list_style,
			     "combo_list_style");
	GLADE_HOOKUP_OBJECT (counter_property_dialog,
			     combo_entry_list_style_type,
			     "combo_entry_list_style_type");
	GLADE_HOOKUP_OBJECT_NO_REF (counter_property_dialog,
				    dialog_action_area, "dialog_action_area");
	GLADE_HOOKUP_OBJECT (counter_property_dialog, helpbutton,
			     "helpbutton");
	GLADE_HOOKUP_OBJECT (counter_property_dialog, cancelbutton,
			     "cancelbutton");
	GLADE_HOOKUP_OBJECT (counter_property_dialog, okbutton, "okbutton");

	gtk_window_add_accel_group (GTK_WINDOW (counter_property_dialog),
				    accel_group);

	return counter_property_dialog;
}

GtkWidget *
create_font_property_dialog (CssedWindow * window,
			     gchar * previous_string,
			     gboolean add_previous_string,
			     gboolean connect_callbacks)
{
	GtkWidget *font_property_dialog;
	GtkWidget *dialog_vbox;
	GtkWidget *vbox_main;
	GtkWidget *hbox_system_fonts;
	GtkWidget *radiobutton_system_fonts;
	GSList *radiobutton_system_fonts_group = NULL;
	GtkWidget *combo_system_fonts;
	GList *combo_system_fonts_items = NULL;
	GtkWidget *combo_entry_system_fonts;
	GtkWidget *hbox_typographical;
	GtkWidget *radiobutton_typographical_notation;
	GtkWidget *label_blank;
	GtkWidget *hbox_size;
	GtkWidget *checkbutton_size;
	GtkWidget *combo_font_size_selector;
	GList *combo_font_size_selector_items = NULL;
	GtkWidget *combo_entry_font_size_selector;
	GtkObject *spinbutton_font_size_adj;
	GtkWidget *spinbutton_font_size;
	GtkWidget *combo_font_size_units;
	GList *combo_font_size_units_items = NULL;
	GtkWidget *combo_entry_font_size_units;
	GtkWidget *hbox_line_height;
	GtkWidget *checkbutton_line_height;
	GtkObject *spinbutton_line_height_adj;
	GtkWidget *spinbutton_line_height;
	GtkWidget *combo_line_height;
	GList *combo_line_height_items = NULL;
	GtkWidget *combo_entry_line_height_selector_and_units;
	GtkWidget *hbox_style;
	GtkWidget *checkbutton_style;
	GtkWidget *combo_style;
	GList *combo_style_items = NULL;
	GtkWidget *combo_entry_style;
	GtkWidget *hbox_variant;
	GtkWidget *checkbutton_variant;
	GtkWidget *combo_variant;
	GList *combo_variant_items = NULL;
	GtkWidget *combo_entry_variant;
	GtkWidget *hbox_weight;
	GtkWidget *checkbutton_weight;
	GtkWidget *combo_weight;
	GList *combo_weight_items = NULL;
	GtkWidget *combo_entry_weight;
	GtkWidget *hbox_font_family;
	GtkWidget *checkbutton_font_family;
	GtkWidget *combo_font_family;
	GList *combo_font_family_items = NULL;
	GtkWidget *combo_entry_font_family;
	GtkWidget *hbox_generic_family;
	GtkWidget *checkbutton_generic_family;
	GtkWidget *combo_generic_family;
	GList *combo_generic_family_items = NULL;
	GtkWidget *combo_entry_generic_family;
	GtkWidget *hbox_use_font_set;
	GtkWidget *checkbutton_use_font_set;
	GtkWidget *hbuttonbox;
	GtkWidget *button_font_set_add;
	GtkWidget *button_font_set_del;
	GtkWidget *scrolledwindow_font_set;
	GtkWidget *treeview;
	GtkWidget *dialog_action_area;
	GtkWidget *helpbutton;
	GtkWidget *cancelbutton;
	GtkWidget *okbutton;
	GtkAccelGroup *accel_group;
	GtkWidget *main_window;
	CssedCssDialogsData *data = NULL;
	GtkTreeViewColumn *col_font_family;
	GtkTreeViewColumn *col_generic_family;
	GtkCellRenderer *ff_renderer;
	GtkCellRenderer *gf_renderer;
	GtkListStore *store;

	if (connect_callbacks)
	{
		data = g_malloc (sizeof (CssedCssDialogsData));
		data->main_window = window;
		data->previous_string = previous_string;
		data->add_property_string = add_previous_string;
	}


	accel_group = gtk_accel_group_new ();

	font_property_dialog = gtk_dialog_new ();
	gtk_window_set_title (GTK_WINDOW (font_property_dialog), "font");
	gtk_window_set_modal (GTK_WINDOW (font_property_dialog), TRUE);

	main_window = cssed_window_get_window_widget( window );

	gtk_window_set_transient_for (GTK_WINDOW (font_property_dialog),
				      GTK_WINDOW (main_window));

	dialog_vbox = GTK_DIALOG (font_property_dialog)->vbox;
	gtk_widget_show (dialog_vbox);

	vbox_main = gtk_vbox_new (FALSE, 0);
	gtk_widget_show (vbox_main);
	gtk_box_pack_start (GTK_BOX (dialog_vbox), vbox_main, TRUE, TRUE, 0);

	hbox_system_fonts = gtk_hbox_new (TRUE, 0);
	gtk_widget_show (hbox_system_fonts);
	gtk_box_pack_start (GTK_BOX (vbox_main), hbox_system_fonts, FALSE,
			    FALSE, 10);

	radiobutton_system_fonts =
		gtk_radio_button_new_with_mnemonic (NULL,
						    _("Use system fonts"));
	gtk_widget_show (radiobutton_system_fonts);
	gtk_box_pack_start (GTK_BOX (hbox_system_fonts),
			    radiobutton_system_fonts, TRUE, TRUE, 5);
	gtk_radio_button_set_group (GTK_RADIO_BUTTON
				    (radiobutton_system_fonts),
				    radiobutton_system_fonts_group);
	radiobutton_system_fonts_group =
		gtk_radio_button_get_group (GTK_RADIO_BUTTON
					    (radiobutton_system_fonts));

	combo_system_fonts = gtk_combo_new ();
	g_object_set_data (G_OBJECT (GTK_COMBO (combo_system_fonts)->popwin),
			   "GladeParentKey", combo_system_fonts);
	gtk_widget_show (combo_system_fonts);
	gtk_box_pack_start (GTK_BOX (hbox_system_fonts), combo_system_fonts,
			    TRUE, TRUE, 5);
	combo_system_fonts_items =
		g_list_append (combo_system_fonts_items,
			       (gpointer) "caption");
	combo_system_fonts_items =
		g_list_append (combo_system_fonts_items, (gpointer) "icon");
	combo_system_fonts_items =
		g_list_append (combo_system_fonts_items, (gpointer) "menu");
	combo_system_fonts_items =
		g_list_append (combo_system_fonts_items,
			       (gpointer) "message-box");
	combo_system_fonts_items =
		g_list_append (combo_system_fonts_items,
			       (gpointer) "small-caption");
	combo_system_fonts_items =
		g_list_append (combo_system_fonts_items,
			       (gpointer) "status-bar");
	gtk_combo_set_popdown_strings (GTK_COMBO (combo_system_fonts),
				       combo_system_fonts_items);
	g_list_free (combo_system_fonts_items);

	combo_entry_system_fonts = GTK_COMBO (combo_system_fonts)->entry;
	gtk_widget_show (combo_entry_system_fonts);
	gtk_editable_set_editable (GTK_EDITABLE (combo_entry_system_fonts),
				   FALSE);
	gtk_entry_set_text (GTK_ENTRY (combo_entry_system_fonts), "caption");

	hbox_typographical = gtk_hbox_new (TRUE, 0);
	gtk_widget_show (hbox_typographical);
	gtk_box_pack_start (GTK_BOX (vbox_main), hbox_typographical, FALSE,
			    FALSE, 1);

	radiobutton_typographical_notation =
		gtk_radio_button_new_with_mnemonic (NULL,
						    _
						    ("Use typographical notation"));
	gtk_widget_show (radiobutton_typographical_notation);
	gtk_box_pack_start (GTK_BOX (hbox_typographical),
			    radiobutton_typographical_notation, TRUE, TRUE,
			    5);
	gtk_radio_button_set_group (GTK_RADIO_BUTTON
				    (radiobutton_typographical_notation),
				    radiobutton_system_fonts_group);
	radiobutton_system_fonts_group =
		gtk_radio_button_get_group (GTK_RADIO_BUTTON
					    (radiobutton_typographical_notation));

	label_blank = gtk_label_new ("");
	gtk_widget_show (label_blank);
	gtk_box_pack_start (GTK_BOX (hbox_typographical), label_blank, FALSE,
			    FALSE, 0);
	gtk_label_set_justify (GTK_LABEL (label_blank), GTK_JUSTIFY_LEFT);

	hbox_size = gtk_hbox_new (FALSE, 0);
	gtk_widget_show (hbox_size);
	gtk_box_pack_start (GTK_BOX (vbox_main), hbox_size, FALSE, FALSE, 1);

	checkbutton_size = gtk_check_button_new_with_mnemonic (_("_size"));
	gtk_widget_show (checkbutton_size);
	gtk_box_pack_start (GTK_BOX (hbox_size), checkbutton_size, TRUE, TRUE,
			    5);

	combo_font_size_selector = gtk_combo_new ();
	g_object_set_data (G_OBJECT
			   (GTK_COMBO (combo_font_size_selector)->popwin),
			   "GladeParentKey", combo_font_size_selector);
	gtk_widget_show (combo_font_size_selector);
	gtk_box_pack_start (GTK_BOX (hbox_size), combo_font_size_selector,
			    FALSE, FALSE, 0);
	combo_font_size_selector_items =
		g_list_append (combo_font_size_selector_items,
			       (gpointer) CSS_STR_USE_PERCLEN);
	combo_font_size_selector_items =
		g_list_append (combo_font_size_selector_items,
			       (gpointer) "xx-small");
	combo_font_size_selector_items =
		g_list_append (combo_font_size_selector_items,
			       (gpointer) "x-small");
	combo_font_size_selector_items =
		g_list_append (combo_font_size_selector_items,
			       (gpointer) "small");
	combo_font_size_selector_items =
		g_list_append (combo_font_size_selector_items,
			       (gpointer) "medium");
	combo_font_size_selector_items =
		g_list_append (combo_font_size_selector_items,
			       (gpointer) "large");
	combo_font_size_selector_items =
		g_list_append (combo_font_size_selector_items,
			       (gpointer) "x-large");
	combo_font_size_selector_items =
		g_list_append (combo_font_size_selector_items,
			       (gpointer) "xx-large");
	combo_font_size_selector_items =
		g_list_append (combo_font_size_selector_items,
			       (gpointer) "larger");
	combo_font_size_selector_items =
		g_list_append (combo_font_size_selector_items,
			       (gpointer) "smaller ");
	gtk_combo_set_popdown_strings (GTK_COMBO (combo_font_size_selector),
				       combo_font_size_selector_items);
	g_list_free (combo_font_size_selector_items);

	combo_entry_font_size_selector =
		GTK_COMBO (combo_font_size_selector)->entry;
	gtk_widget_show (combo_entry_font_size_selector);
	gtk_editable_set_editable (GTK_EDITABLE
				   (combo_entry_font_size_selector), FALSE);
	gtk_entry_set_text (GTK_ENTRY (combo_entry_font_size_selector),
			    CSS_STR_USE_PERCLEN);

	spinbutton_font_size_adj =
		gtk_adjustment_new (8, -35565, 35565, 1, 10, 10);
	spinbutton_font_size =
		gtk_spin_button_new (GTK_ADJUSTMENT
				     (spinbutton_font_size_adj), 1, 0);
	gtk_widget_show (spinbutton_font_size);
	gtk_box_pack_start (GTK_BOX (hbox_size), spinbutton_font_size, FALSE,
			    FALSE, 5);
	gtk_widget_set_size_request (spinbutton_font_size, 46, -1);

	combo_font_size_units = gtk_combo_new ();
	g_object_set_data (G_OBJECT
			   (GTK_COMBO (combo_font_size_units)->popwin),
			   "GladeParentKey", combo_font_size_units);
	gtk_widget_show (combo_font_size_units);
	gtk_box_pack_end (GTK_BOX (hbox_size), combo_font_size_units, FALSE,
			  FALSE, 5);
	combo_font_size_units_items =
		g_list_append (combo_font_size_units_items, (gpointer) "%");
	combo_font_size_units_items =
		g_list_append (combo_font_size_units_items, (gpointer) "em");
	combo_font_size_units_items =
		g_list_append (combo_font_size_units_items, (gpointer) "ex");
	combo_font_size_units_items =
		g_list_append (combo_font_size_units_items, (gpointer) "px");
	combo_font_size_units_items =
		g_list_append (combo_font_size_units_items, (gpointer) "in");
	combo_font_size_units_items =
		g_list_append (combo_font_size_units_items, (gpointer) "cm");
	combo_font_size_units_items =
		g_list_append (combo_font_size_units_items, (gpointer) "mm");
	combo_font_size_units_items =
		g_list_append (combo_font_size_units_items, (gpointer) "pt");
	combo_font_size_units_items =
		g_list_append (combo_font_size_units_items, (gpointer) "pc");

	gtk_combo_set_popdown_strings (GTK_COMBO (combo_font_size_units),
				       combo_font_size_units_items);
	g_list_free (combo_font_size_units_items);

	combo_entry_font_size_units =
		GTK_COMBO (combo_font_size_units)->entry;
	gtk_widget_show (combo_entry_font_size_units);
	gtk_widget_set_size_request (combo_entry_font_size_units, 30, -1);
	gtk_editable_set_editable (GTK_EDITABLE (combo_entry_font_size_units),
				   FALSE);
	gtk_entry_set_text (GTK_ENTRY (combo_entry_font_size_units), "px");

	hbox_line_height = gtk_hbox_new (FALSE, 0);
	gtk_widget_show (hbox_line_height);
	gtk_box_pack_start (GTK_BOX (vbox_main), hbox_line_height, FALSE,
			    FALSE, 1);

	checkbutton_line_height =
		gtk_check_button_new_with_mnemonic (_("line height"));
	gtk_widget_show (checkbutton_line_height);
	gtk_box_pack_start (GTK_BOX (hbox_line_height),
			    checkbutton_line_height, TRUE, TRUE, 5);

	spinbutton_line_height_adj =
		gtk_adjustment_new (1, -35565, 35565, 1, 10, 10);
	spinbutton_line_height =
		gtk_spin_button_new (GTK_ADJUSTMENT
				     (spinbutton_line_height_adj), 1, 0);
	gtk_widget_show (spinbutton_line_height);
	gtk_box_pack_start (GTK_BOX (hbox_line_height),
			    spinbutton_line_height, FALSE, FALSE, 5);
	gtk_widget_set_size_request (spinbutton_line_height, 50, -1);

	combo_line_height = gtk_combo_new ();
	g_object_set_data (G_OBJECT (GTK_COMBO (combo_line_height)->popwin),
			   "GladeParentKey", combo_line_height);
	gtk_widget_show (combo_line_height);
	gtk_box_pack_start (GTK_BOX (hbox_line_height), combo_line_height,
			    FALSE, FALSE, 5);
	combo_line_height_items =
		g_list_append (combo_line_height_items,
			       (gpointer) CSS_STR_USE_NUM);
	combo_line_height_items =
		g_list_append (combo_line_height_items, (gpointer) "%");
	combo_line_height_items =
		g_list_append (combo_line_height_items, (gpointer) "em");
	combo_line_height_items =
		g_list_append (combo_line_height_items, (gpointer) "ex");
	combo_line_height_items =
		g_list_append (combo_line_height_items, (gpointer) "px");
	combo_line_height_items =
		g_list_append (combo_line_height_items, (gpointer) "in");
	combo_line_height_items =
		g_list_append (combo_line_height_items, (gpointer) "cm");
	combo_line_height_items =
		g_list_append (combo_line_height_items, (gpointer) "mm");
	combo_line_height_items =
		g_list_append (combo_line_height_items, (gpointer) "pt");
	combo_line_height_items =
		g_list_append (combo_line_height_items, (gpointer) "pc");

	gtk_combo_set_popdown_strings (GTK_COMBO (combo_line_height),
				       combo_line_height_items);
	g_list_free (combo_line_height_items);

	combo_entry_line_height_selector_and_units =
		GTK_COMBO (combo_line_height)->entry;
	gtk_widget_show (combo_entry_line_height_selector_and_units);
	gtk_editable_set_editable (GTK_EDITABLE
				   (combo_entry_line_height_selector_and_units),
				   FALSE);
	gtk_entry_set_text (GTK_ENTRY
			    (combo_entry_line_height_selector_and_units),
			    CSS_STR_USE_NUM);

	hbox_style = gtk_hbox_new (TRUE, 0);
	gtk_widget_show (hbox_style);
	gtk_box_pack_start (GTK_BOX (vbox_main), hbox_style, FALSE, FALSE, 1);

	checkbutton_style = gtk_check_button_new_with_mnemonic (_("style"));
	gtk_widget_show (checkbutton_style);
	gtk_box_pack_start (GTK_BOX (hbox_style), checkbutton_style, TRUE,
			    TRUE, 5);

	combo_style = gtk_combo_new ();
	g_object_set_data (G_OBJECT (GTK_COMBO (combo_style)->popwin),
			   "GladeParentKey", combo_style);
	gtk_widget_show (combo_style);
	gtk_box_pack_start (GTK_BOX (hbox_style), combo_style, TRUE, TRUE, 4);
	combo_style_items =
		g_list_append (combo_style_items, (gpointer) "normal");
	combo_style_items =
		g_list_append (combo_style_items, (gpointer) "italic");
	combo_style_items =
		g_list_append (combo_style_items, (gpointer) "oblique");
	gtk_combo_set_popdown_strings (GTK_COMBO (combo_style),
				       combo_style_items);
	g_list_free (combo_style_items);

	combo_entry_style = GTK_COMBO (combo_style)->entry;
	gtk_widget_show (combo_entry_style);
	gtk_editable_set_editable (GTK_EDITABLE (combo_entry_style), FALSE);
	gtk_entry_set_text (GTK_ENTRY (combo_entry_style), "normal");

	hbox_variant = gtk_hbox_new (TRUE, 0);
	gtk_widget_show (hbox_variant);
	gtk_box_pack_start (GTK_BOX (vbox_main), hbox_variant, FALSE, FALSE,
			    1);

	checkbutton_variant = gtk_check_button_new_with_mnemonic ("variant");
	gtk_widget_show (checkbutton_variant);
	gtk_box_pack_start (GTK_BOX (hbox_variant), checkbutton_variant, TRUE,
			    TRUE, 5);

	combo_variant = gtk_combo_new ();
	g_object_set_data (G_OBJECT (GTK_COMBO (combo_variant)->popwin),
			   "GladeParentKey", combo_variant);
	gtk_widget_show (combo_variant);
	gtk_box_pack_start (GTK_BOX (hbox_variant), combo_variant, TRUE, TRUE,
			    5);
	combo_variant_items =
		g_list_append (combo_variant_items, (gpointer) "normal");
	combo_variant_items =
		g_list_append (combo_variant_items, (gpointer) "small-caps");
	gtk_combo_set_popdown_strings (GTK_COMBO (combo_variant),
				       combo_variant_items);
	g_list_free (combo_variant_items);

	combo_entry_variant = GTK_COMBO (combo_variant)->entry;
	gtk_widget_show (combo_entry_variant);
	gtk_editable_set_editable (GTK_EDITABLE (combo_entry_variant), FALSE);
	gtk_entry_set_text (GTK_ENTRY (combo_entry_variant), "normal");

	hbox_weight = gtk_hbox_new (TRUE, 0);
	gtk_widget_show (hbox_weight);
	gtk_box_pack_start (GTK_BOX (vbox_main), hbox_weight, FALSE, FALSE,
			    1);

	checkbutton_weight = gtk_check_button_new_with_mnemonic ("weight");
	gtk_widget_show (checkbutton_weight);
	gtk_box_pack_start (GTK_BOX (hbox_weight), checkbutton_weight, TRUE,
			    TRUE, 5);

	combo_weight = gtk_combo_new ();
	g_object_set_data (G_OBJECT (GTK_COMBO (combo_weight)->popwin),
			   "GladeParentKey", combo_weight);
	gtk_widget_show (combo_weight);
	gtk_box_pack_start (GTK_BOX (hbox_weight), combo_weight, TRUE, TRUE,
			    5);
	combo_weight_items =
		g_list_append (combo_weight_items, (gpointer) "normal");
	combo_weight_items =
		g_list_append (combo_weight_items, (gpointer) "bold");
	combo_weight_items =
		g_list_append (combo_weight_items, (gpointer) "bolder");
	combo_weight_items =
		g_list_append (combo_weight_items, (gpointer) "lighter");
	combo_weight_items =
		g_list_append (combo_weight_items, (gpointer) "100");
	combo_weight_items =
		g_list_append (combo_weight_items, (gpointer) "200");
	combo_weight_items =
		g_list_append (combo_weight_items, (gpointer) "300");
	combo_weight_items =
		g_list_append (combo_weight_items, (gpointer) "400");
	combo_weight_items =
		g_list_append (combo_weight_items, (gpointer) "500");
	combo_weight_items =
		g_list_append (combo_weight_items, (gpointer) "600");
	combo_weight_items =
		g_list_append (combo_weight_items, (gpointer) "700");
	combo_weight_items =
		g_list_append (combo_weight_items, (gpointer) "800");
	combo_weight_items =
		g_list_append (combo_weight_items, (gpointer) "900");
	gtk_combo_set_popdown_strings (GTK_COMBO (combo_weight),
				       combo_weight_items);
	g_list_free (combo_weight_items);

	combo_entry_weight = GTK_COMBO (combo_weight)->entry;
	gtk_widget_show (combo_entry_weight);
	gtk_editable_set_editable (GTK_EDITABLE (combo_entry_weight), FALSE);
	gtk_entry_set_text (GTK_ENTRY (combo_entry_weight), "normal");

	hbox_font_family = gtk_hbox_new (TRUE, 0);
	gtk_widget_show (hbox_font_family);
	gtk_box_pack_start (GTK_BOX (vbox_main), hbox_font_family, FALSE,
			    FALSE, 1);

	checkbutton_font_family =
		gtk_check_button_new_with_mnemonic (_("font family"));
	gtk_widget_show (checkbutton_font_family);
	gtk_box_pack_start (GTK_BOX (hbox_font_family),
			    checkbutton_font_family, TRUE, TRUE, 5);

	combo_font_family = gtk_combo_new ();
	g_object_set_data (G_OBJECT (GTK_COMBO (combo_font_family)->popwin),
			   "GladeParentKey", combo_font_family);
	gtk_widget_show (combo_font_family);
	gtk_box_pack_start (GTK_BOX (hbox_font_family), combo_font_family,
			    TRUE, TRUE, 5);
	combo_font_family_items =
		g_list_append (combo_font_family_items, (gpointer) "Arial");
	combo_font_family_items =
		g_list_append (combo_font_family_items,
			       (gpointer) "Arial Black");
	combo_font_family_items =
		g_list_append (combo_font_family_items, (gpointer) "Charter");
	combo_font_family_items =
		g_list_append (combo_font_family_items, (gpointer) "Chicago");
	combo_font_family_items =
		g_list_append (combo_font_family_items, (gpointer) "Clean");
	combo_font_family_items =
		g_list_append (combo_font_family_items,
			       (gpointer) "Comic Sans MS");
	combo_font_family_items =
		g_list_append (combo_font_family_items, (gpointer) "Courier");
	combo_font_family_items =
		g_list_append (combo_font_family_items,
			       (gpointer) "Courier New");
	combo_font_family_items =
		g_list_append (combo_font_family_items,
			       (gpointer) "Courier Regular");
	combo_font_family_items =
		g_list_append (combo_font_family_items, (gpointer) "Fixed");
	combo_font_family_items =
		g_list_append (combo_font_family_items, (gpointer) "Geneva");
	combo_font_family_items =
		g_list_append (combo_font_family_items,
			       (gpointer) "Helvetica");
	combo_font_family_items =
		g_list_append (combo_font_family_items, (gpointer) "Monaco");
	combo_font_family_items =
		g_list_append (combo_font_family_items,
			       (gpointer) "New York");
	combo_font_family_items =
		g_list_append (combo_font_family_items,
			       (gpointer) "Palatino");
	combo_font_family_items =
		g_list_append (combo_font_family_items, (gpointer) "Impact");
	combo_font_family_items =
		g_list_append (combo_font_family_items, (gpointer) "Lucida");
	combo_font_family_items =
		g_list_append (combo_font_family_items,
			       (gpointer) "Lucida Bright");
	combo_font_family_items =
		g_list_append (combo_font_family_items,
			       (gpointer) "Lucidatypewriter");
	combo_font_family_items =
		g_list_append (combo_font_family_items,
			       (gpointer) "Lucida Sans");
	combo_font_family_items =
		g_list_append (combo_font_family_items,
			       (gpointer) "Lucida Console");
	combo_font_family_items =
		g_list_append (combo_font_family_items,
			       (gpointer) "New Century Schoolbook");
	combo_font_family_items =
		g_list_append (combo_font_family_items, (gpointer) "Symbol");
	combo_font_family_items =
		g_list_append (combo_font_family_items, (gpointer) "Times");
	combo_font_family_items =
		g_list_append (combo_font_family_items,
			       (gpointer) "Times New Roman");
	combo_font_family_items =
		g_list_append (combo_font_family_items, (gpointer) "Utopia");
	combo_font_family_items =
		g_list_append (combo_font_family_items, (gpointer) "Verdana");
	combo_font_family_items =
		g_list_append (combo_font_family_items,
			       (gpointer) "Webdings");
	combo_font_family_items =
		g_list_append (combo_font_family_items,
			       (gpointer) "Windings");
	gtk_combo_set_popdown_strings (GTK_COMBO (combo_font_family),
				       combo_font_family_items);
	g_list_free (combo_font_family_items);

	combo_entry_font_family = GTK_COMBO (combo_font_family)->entry;
	gtk_widget_show (combo_entry_font_family);
	gtk_entry_set_text (GTK_ENTRY (combo_entry_font_family), "");

	hbox_generic_family = gtk_hbox_new (TRUE, 0);
	gtk_widget_show (hbox_generic_family);
	gtk_box_pack_start (GTK_BOX (vbox_main), hbox_generic_family, FALSE,
			    FALSE, 1);

	checkbutton_generic_family =
		gtk_check_button_new_with_mnemonic (_("generic family"));
	gtk_widget_show (checkbutton_generic_family);
	gtk_box_pack_start (GTK_BOX (hbox_generic_family),
			    checkbutton_generic_family, TRUE, TRUE, 5);

	combo_generic_family = gtk_combo_new ();
	g_object_set_data (G_OBJECT
			   (GTK_COMBO (combo_generic_family)->popwin),
			   "GladeParentKey", combo_generic_family);
	gtk_widget_show (combo_generic_family);
	gtk_box_pack_start (GTK_BOX (hbox_generic_family),
			    combo_generic_family, TRUE, TRUE, 5);
	combo_generic_family_items =
		g_list_append (combo_generic_family_items,
			       (gpointer) "serif");
	combo_generic_family_items =
		g_list_append (combo_generic_family_items,
			       (gpointer) "sans-serif");
	combo_generic_family_items =
		g_list_append (combo_generic_family_items,
			       (gpointer) "cursive");
	combo_generic_family_items =
		g_list_append (combo_generic_family_items,
			       (gpointer) "fantasy");
	combo_generic_family_items =
		g_list_append (combo_generic_family_items,
			       (gpointer) "monospace   ");
	gtk_combo_set_popdown_strings (GTK_COMBO (combo_generic_family),
				       combo_generic_family_items);
	g_list_free (combo_generic_family_items);

	combo_entry_generic_family = GTK_COMBO (combo_generic_family)->entry;
	gtk_widget_show (combo_entry_generic_family);
	gtk_entry_set_text (GTK_ENTRY (combo_entry_generic_family), "");

	hbox_use_font_set = gtk_hbox_new (TRUE, 0);
	gtk_widget_show (hbox_use_font_set);
	gtk_box_pack_start (GTK_BOX (vbox_main), hbox_use_font_set, FALSE,
			    FALSE, 0);

	checkbutton_use_font_set =
		gtk_check_button_new_with_mnemonic (_("use font set"));
	gtk_widget_show (checkbutton_use_font_set);
	gtk_box_pack_start (GTK_BOX (hbox_use_font_set),
			    checkbutton_use_font_set, TRUE, TRUE, 5);

	hbuttonbox = gtk_hbutton_box_new ();
	gtk_widget_show (hbuttonbox);
	gtk_box_pack_start (GTK_BOX (hbox_use_font_set), hbuttonbox, TRUE,
			    TRUE, 0);
	gtk_container_set_border_width (GTK_CONTAINER (hbuttonbox), 5);

	button_font_set_add = gtk_button_new_from_stock ("gtk-add");
	gtk_widget_show (button_font_set_add);
	gtk_container_add (GTK_CONTAINER (hbuttonbox), button_font_set_add);
	GTK_WIDGET_SET_FLAGS (button_font_set_add, GTK_CAN_DEFAULT);

	button_font_set_del = gtk_button_new_from_stock ("gtk-remove");
	gtk_widget_show (button_font_set_del);
	gtk_container_add (GTK_CONTAINER (hbuttonbox), button_font_set_del);
	GTK_WIDGET_SET_FLAGS (button_font_set_del, GTK_CAN_DEFAULT);
	gtk_widget_add_accelerator (button_font_set_del, "clicked",
				    accel_group, GDK_q, GDK_CONTROL_MASK,
				    GTK_ACCEL_VISIBLE);
	gtk_widget_add_accelerator (button_font_set_del, "clicked",
				    accel_group, GDK_d, GDK_CONTROL_MASK,
				    GTK_ACCEL_VISIBLE);

	scrolledwindow_font_set = gtk_scrolled_window_new (NULL, NULL);
	gtk_widget_show (scrolledwindow_font_set);
	gtk_box_pack_start (GTK_BOX (vbox_main), scrolledwindow_font_set,
			    TRUE, TRUE, 0);
	gtk_container_set_border_width (GTK_CONTAINER
					(scrolledwindow_font_set), 5);
	gtk_widget_set_size_request (scrolledwindow_font_set, -1, 125);

	store = gtk_list_store_new (2, G_TYPE_STRING, G_TYPE_STRING);
	treeview = gtk_tree_view_new_with_model (GTK_TREE_MODEL (store));
	gtk_widget_show (treeview);
	gtk_container_add (GTK_CONTAINER (scrolledwindow_font_set), treeview);

	ff_renderer = gtk_cell_renderer_text_new ();
	gf_renderer = gtk_cell_renderer_text_new ();

	col_font_family =
		gtk_tree_view_column_new_with_attributes (_("Font family"),
							  ff_renderer, "text",
							  0, NULL);
	col_generic_family =
		gtk_tree_view_column_new_with_attributes (_("Generic family"),
							  gf_renderer, "text",
							  1, NULL);
	gtk_tree_view_insert_column (GTK_TREE_VIEW (treeview),
				     col_font_family, 0);
	gtk_tree_view_insert_column (GTK_TREE_VIEW (treeview),
				     col_generic_family, 1);

	gtk_tree_view_set_headers_visible (GTK_TREE_VIEW (treeview), TRUE);

	dialog_action_area = GTK_DIALOG (font_property_dialog)->action_area;
	gtk_widget_show (dialog_action_area);
	gtk_button_box_set_layout (GTK_BUTTON_BOX (dialog_action_area),
				   GTK_BUTTONBOX_END);

	helpbutton = gtk_button_new_from_stock ("gtk-help");
	gtk_widget_show (helpbutton);
	gtk_dialog_add_action_widget (GTK_DIALOG (font_property_dialog),
				      helpbutton, GTK_RESPONSE_HELP);
	GTK_WIDGET_SET_FLAGS (helpbutton, GTK_CAN_DEFAULT);
	gtk_widget_add_accelerator (helpbutton, "clicked", accel_group,
				    GDK_y, GDK_CONTROL_MASK,
				    GTK_ACCEL_VISIBLE);

	cancelbutton = gtk_button_new_from_stock ("gtk-cancel");
	gtk_widget_show (cancelbutton);
	gtk_dialog_add_action_widget (GTK_DIALOG (font_property_dialog),
				      cancelbutton, GTK_RESPONSE_CANCEL);
	GTK_WIDGET_SET_FLAGS (cancelbutton, GTK_CAN_DEFAULT);
	gtk_widget_add_accelerator (cancelbutton, "clicked", accel_group,
				    GDK_c, GDK_CONTROL_MASK,
				    GTK_ACCEL_VISIBLE);

	okbutton = gtk_button_new_from_stock ("gtk-ok");
	gtk_widget_show (okbutton);
	gtk_dialog_add_action_widget (GTK_DIALOG (font_property_dialog),
				      okbutton, GTK_RESPONSE_OK);
	GTK_WIDGET_SET_FLAGS (okbutton, GTK_CAN_DEFAULT);
	gtk_widget_add_accelerator (okbutton, "clicked", accel_group,
				    GDK_o, GDK_CONTROL_MASK,
				    GTK_ACCEL_VISIBLE);


	g_signal_connect ((gpointer) combo_entry_font_size_units,
			  "changed", G_CALLBACK (on_css_dialog_unit_changed),
			  spinbutton_font_size);
	g_signal_connect ((gpointer)
			  combo_entry_line_height_selector_and_units,
			  "changed", G_CALLBACK (on_css_dialog_unit_changed),
			  spinbutton_line_height);

	/* to update the checkboxes when a related widget is changed */			  
	g_signal_connect ((gpointer) combo_entry_system_fonts,
			  "changed", G_CALLBACK (on_widget_change_set_toggle_active),
			  radiobutton_system_fonts);		
			  	  
	g_signal_connect ((gpointer) combo_entry_font_size_selector,
			  "changed", G_CALLBACK (on_widget_change_set_toggle_active),
			  radiobutton_typographical_notation);			  
	g_signal_connect ((gpointer) combo_entry_font_size_units,
			  "changed", G_CALLBACK (on_widget_change_set_toggle_active),
			  radiobutton_typographical_notation);			  	  			  
	g_signal_connect ((gpointer) combo_entry_line_height_selector_and_units,
			  "changed", G_CALLBACK (on_widget_change_set_toggle_active),
			  radiobutton_typographical_notation);				  
	g_signal_connect ((gpointer) combo_entry_style,
			  "changed", G_CALLBACK (on_widget_change_set_toggle_active),
			  radiobutton_typographical_notation);				  
	g_signal_connect ((gpointer) combo_entry_variant,
			  "changed", G_CALLBACK (on_widget_change_set_toggle_active),
			  radiobutton_typographical_notation); 
	g_signal_connect ((gpointer) combo_entry_weight,
			  "changed", G_CALLBACK (on_widget_change_set_toggle_active),
			  radiobutton_typographical_notation);
	g_signal_connect ((gpointer) combo_entry_font_family,
			  "changed", G_CALLBACK (on_widget_change_set_toggle_active),
			  radiobutton_typographical_notation);
	g_signal_connect ((gpointer) combo_entry_generic_family,
			  "changed", G_CALLBACK (on_widget_change_set_toggle_active),
			  radiobutton_typographical_notation);
	g_signal_connect ((gpointer) spinbutton_font_size,
			  "value-changed", G_CALLBACK (on_widget_change_set_toggle_active),
			  radiobutton_typographical_notation);	
	g_signal_connect ((gpointer) spinbutton_line_height,
			  "value-changed", G_CALLBACK (on_widget_change_set_toggle_active),
			  radiobutton_typographical_notation);				  	    			  		

	g_signal_connect ((gpointer) combo_entry_font_size_selector,
			  "changed", G_CALLBACK (on_widget_change_set_toggle_active),
			  checkbutton_size);
	g_signal_connect ((gpointer) combo_entry_font_size_units,
			  "changed", G_CALLBACK (on_widget_change_set_toggle_active),
			  checkbutton_size);
	g_signal_connect ((gpointer) spinbutton_font_size,
			  "value-changed", G_CALLBACK (on_widget_change_set_toggle_active),
			  checkbutton_size);	
			  
	g_signal_connect ((gpointer) combo_entry_line_height_selector_and_units,
			  "changed", G_CALLBACK (on_widget_change_set_toggle_active),
			  checkbutton_line_height);
	g_signal_connect ((gpointer) spinbutton_line_height,
			  "value-changed", G_CALLBACK (on_widget_change_set_toggle_active),
			  checkbutton_line_height);	
			  
	g_signal_connect ((gpointer) combo_entry_style,
			  "changed", G_CALLBACK (on_widget_change_set_toggle_active),
			  checkbutton_style);	
			  
  
	g_signal_connect ((gpointer) combo_entry_variant,
			  "changed", G_CALLBACK (on_widget_change_set_toggle_active),
			  checkbutton_variant);				  	
			  				
	g_signal_connect ((gpointer) combo_entry_weight,
			  "changed", G_CALLBACK (on_widget_change_set_toggle_active),
			  checkbutton_weight);	
			  
  
	g_signal_connect ((gpointer) combo_entry_font_family,
			  "changed", G_CALLBACK (on_widget_change_set_toggle_active),
			  checkbutton_font_family);	
	g_signal_connect ((gpointer) combo_entry_font_family,
			  "changed", G_CALLBACK (on_widget_change_unset_toggle_active),
			  checkbutton_use_font_set);				  			  	
			  				
	g_signal_connect ((gpointer) combo_entry_generic_family,
			  "changed", G_CALLBACK (on_widget_change_set_toggle_active),
			  checkbutton_generic_family);			  
	g_signal_connect ((gpointer) combo_entry_generic_family,
			  "changed", G_CALLBACK (on_widget_change_unset_toggle_active),
			  checkbutton_use_font_set);	
			  		  				
	if (connect_callbacks)
	{
		g_signal_connect ((gpointer) font_property_dialog, "close",
				  G_CALLBACK (on_css_dialog_close), data);
		g_signal_connect ((gpointer) helpbutton, "clicked",
				  G_CALLBACK
				  (on_font_property_dialog_helpbutton_clicked),
				  NULL);
		g_signal_connect ((gpointer) cancelbutton, "clicked",
				  G_CALLBACK
				  (on_css_dialog_cancel_button_clicked),
				  data);
		g_signal_connect ((gpointer) okbutton, "clicked",
				  G_CALLBACK
				  (on_font_property_dialog_okbutton_clicked),
				  data);
	}

	g_signal_connect ((gpointer) button_font_set_add, "clicked",
			  G_CALLBACK (on_button_font_set_add_clicked), store);

	g_signal_connect ((gpointer) button_font_set_del, "clicked",
			  G_CALLBACK (on_button_font_set_del_clicked), store);
	GLADE_HOOKUP_OBJECT_NO_REF (font_property_dialog,
				    font_property_dialog,
				    "font_property_dialog");
	GLADE_HOOKUP_OBJECT_NO_REF (font_property_dialog, dialog_vbox,
				    "dialog_vbox");
	GLADE_HOOKUP_OBJECT (font_property_dialog, vbox_main, "vbox_main");
	GLADE_HOOKUP_OBJECT (font_property_dialog, hbox_system_fonts,
			     "hbox_system_fonts");
	GLADE_HOOKUP_OBJECT (font_property_dialog, radiobutton_system_fonts,
			     "radiobutton_system_fonts");
	GLADE_HOOKUP_OBJECT (font_property_dialog, combo_system_fonts,
			     "combo_system_fonts");
	GLADE_HOOKUP_OBJECT (font_property_dialog, combo_entry_system_fonts,
			     "combo_entry_system_fonts");
	GLADE_HOOKUP_OBJECT (font_property_dialog, hbox_typographical,
			     "hbox_typographical");
	GLADE_HOOKUP_OBJECT (font_property_dialog,
			     radiobutton_typographical_notation,
			     "radiobutton_typographical_notation");
	GLADE_HOOKUP_OBJECT (font_property_dialog, label_blank,
			     "label_blank");
	GLADE_HOOKUP_OBJECT (font_property_dialog, hbox_size, "hbox_size");
	GLADE_HOOKUP_OBJECT (font_property_dialog, checkbutton_size,
			     "checkbutton_size");
	GLADE_HOOKUP_OBJECT (font_property_dialog, combo_font_size_selector,
			     "combo_font_size_selector");
	GLADE_HOOKUP_OBJECT (font_property_dialog,
			     combo_entry_font_size_selector,
			     "combo_entry_font_size_selector");
	GLADE_HOOKUP_OBJECT (font_property_dialog, spinbutton_font_size,
			     "spinbutton_font_size");
	GLADE_HOOKUP_OBJECT (font_property_dialog, combo_font_size_units,
			     "combo_font_size_units");
	GLADE_HOOKUP_OBJECT (font_property_dialog,
			     combo_entry_font_size_units,
			     "combo_entry_font_size_units");
	GLADE_HOOKUP_OBJECT (font_property_dialog, hbox_line_height,
			     "hbox_line_height");
	GLADE_HOOKUP_OBJECT (font_property_dialog, checkbutton_line_height,
			     "checkbutton_line_height");
	GLADE_HOOKUP_OBJECT (font_property_dialog, spinbutton_line_height,
			     "spinbutton_line_height");
	GLADE_HOOKUP_OBJECT (font_property_dialog, combo_line_height,
			     "combo_line_height");
	GLADE_HOOKUP_OBJECT (font_property_dialog,
			     combo_entry_line_height_selector_and_units,
			     "combo_entry_line_height_selector_and_units");
	GLADE_HOOKUP_OBJECT (font_property_dialog, hbox_style, "hbox_style");
	GLADE_HOOKUP_OBJECT (font_property_dialog, checkbutton_style,
			     "checkbutton_style");
	GLADE_HOOKUP_OBJECT (font_property_dialog, combo_style,
			     "combo_style");
	GLADE_HOOKUP_OBJECT (font_property_dialog, combo_entry_style,
			     "combo_entry_style");
	GLADE_HOOKUP_OBJECT (font_property_dialog, hbox_variant,
			     "hbox_variant");
	GLADE_HOOKUP_OBJECT (font_property_dialog, checkbutton_variant,
			     "checkbutton_variant");
	GLADE_HOOKUP_OBJECT (font_property_dialog, combo_variant,
			     "combo_variant");
	GLADE_HOOKUP_OBJECT (font_property_dialog, combo_entry_variant,
			     "combo_entry_variant");
	GLADE_HOOKUP_OBJECT (font_property_dialog, hbox_weight,
			     "hbox_weight");
	GLADE_HOOKUP_OBJECT (font_property_dialog, checkbutton_weight,
			     "checkbutton_weight");
	GLADE_HOOKUP_OBJECT (font_property_dialog, combo_weight,
			     "combo_weight");
	GLADE_HOOKUP_OBJECT (font_property_dialog, combo_entry_weight,
			     "combo_entry_weight");
	GLADE_HOOKUP_OBJECT (font_property_dialog, hbox_font_family,
			     "hbox_font_family");
	GLADE_HOOKUP_OBJECT (font_property_dialog, checkbutton_font_family,
			     "checkbutton_font_family");
	GLADE_HOOKUP_OBJECT (font_property_dialog, combo_font_family,
			     "combo_font_family");
	GLADE_HOOKUP_OBJECT (font_property_dialog, combo_entry_font_family,
			     "combo_entry_font_family");
	GLADE_HOOKUP_OBJECT (font_property_dialog, hbox_generic_family,
			     "hbox_generic_family");
	GLADE_HOOKUP_OBJECT (font_property_dialog, checkbutton_generic_family,
			     "checkbutton_generic_family");
	GLADE_HOOKUP_OBJECT (font_property_dialog, combo_generic_family,
			     "combo_generic_family");
	GLADE_HOOKUP_OBJECT (font_property_dialog, combo_entry_generic_family,
			     "combo_entry_generic_family");
	GLADE_HOOKUP_OBJECT (font_property_dialog, hbox_use_font_set,
			     "hbox_use_font_set");
	GLADE_HOOKUP_OBJECT (font_property_dialog, checkbutton_use_font_set,
			     "checkbutton_use_font_set");
	GLADE_HOOKUP_OBJECT (font_property_dialog, hbuttonbox, "hbuttonbox");
	GLADE_HOOKUP_OBJECT (font_property_dialog, button_font_set_add,
			     "button_font_set_add");
	GLADE_HOOKUP_OBJECT (font_property_dialog, button_font_set_del,
			     "button_font_set_del");
	GLADE_HOOKUP_OBJECT (font_property_dialog, scrolledwindow_font_set,
			     "scrolledwindow_font_set");
	GLADE_HOOKUP_OBJECT (font_property_dialog, treeview, "treeview");
	GLADE_HOOKUP_OBJECT_NO_REF (font_property_dialog, dialog_action_area,
				    "dialog_action_area");
	GLADE_HOOKUP_OBJECT (font_property_dialog, helpbutton, "helpbutton");
	GLADE_HOOKUP_OBJECT (font_property_dialog, cancelbutton,
			     "cancelbutton");
	GLADE_HOOKUP_OBJECT (font_property_dialog, okbutton, "okbutton");

	gtk_window_add_accel_group (GTK_WINDOW (font_property_dialog),
				    accel_group);

	return font_property_dialog;
}

GtkWidget *
create_font_family_dialog (CssedWindow * window,
			   gchar * previous_string,
			   gboolean add_previous_string,
			   gboolean connect_callbacks)
{
	GtkWidget *font_family_dialog;
	GtkWidget *dialog_vbox;
	GtkWidget *vbox_main;
	GtkWidget *hbox_system_fonts;
	GtkWidget *radiobutton_system_fonts;
	GSList *radiobutton_system_fonts_group = NULL;
	GtkWidget *combo_system_fonts;
	GList *combo_system_fonts_items = NULL;
	GtkWidget *combo_entry_system_fonts;
	GtkWidget *hbox_typo;
	GtkWidget *radiobutton_typographical_notation;
	GtkWidget *label_blank;
	GtkWidget *hbox_font_family;
	GtkWidget *checkbutton_font_family;
	GtkWidget *combo_font_family;
	GList *combo_font_family_items = NULL;
	GtkWidget *combo_entry_font_family;
	GtkWidget *hbox_generic_family;
	GtkWidget *checkbutton_generic_family;
	GtkWidget *combo_generic_family;
	GList *combo_generic_family_items = NULL;
	GtkWidget *combo_entry_generic_family;
	GtkWidget *hbox_font_set;
	GtkWidget *checkbutton_font_set;
	GtkWidget *hbuttonbox_fs;
	GtkWidget *button_font_family_dialog_add;
	GtkWidget *button_font_family_dialog_del;
	GtkWidget *scrolledwindow_font_set;
	GtkWidget *treeview;
	GtkWidget *dialog_action_area;
	GtkWidget *helpbutton;
	GtkWidget *cancelbutton;
	GtkWidget *okbutton;
	GtkWidget *main_window;
	GtkTreeViewColumn *col_font_family;
	GtkTreeViewColumn *col_generic_family;
	GtkCellRenderer *ff_renderer;
	GtkCellRenderer *gf_renderer;
	GtkListStore *store;
	CssedCssDialogsData *data = NULL;

	if (connect_callbacks)
	{
		data = g_malloc (sizeof (CssedCssDialogsData));
		data->main_window = window;
		data->previous_string = previous_string;
		data->add_property_string = add_previous_string;
	}

	font_family_dialog = gtk_dialog_new ();
	gtk_window_set_title (GTK_WINDOW (font_family_dialog), "font-family");
	gtk_window_set_modal (GTK_WINDOW (font_family_dialog), TRUE);

	main_window = cssed_window_get_window_widget( window );

	gtk_window_set_transient_for (GTK_WINDOW (font_family_dialog),
				      GTK_WINDOW (main_window));

	dialog_vbox = GTK_DIALOG (font_family_dialog)->vbox;
	gtk_widget_show (dialog_vbox);

	vbox_main = gtk_vbox_new (FALSE, 0);
	gtk_widget_show (vbox_main);
	gtk_box_pack_start (GTK_BOX (dialog_vbox), vbox_main, TRUE, TRUE, 0);

	hbox_system_fonts = gtk_hbox_new (TRUE, 0);
	gtk_widget_show (hbox_system_fonts);
	gtk_box_pack_start (GTK_BOX (vbox_main), hbox_system_fonts, FALSE,
			    TRUE, 0);

	radiobutton_system_fonts =
		gtk_radio_button_new_with_mnemonic (NULL,
						    _("Use system fonts"));
	gtk_widget_show (radiobutton_system_fonts);
	gtk_box_pack_start (GTK_BOX (hbox_system_fonts),
			    radiobutton_system_fonts, TRUE, TRUE, 5);
	gtk_radio_button_set_group (GTK_RADIO_BUTTON
				    (radiobutton_system_fonts),
				    radiobutton_system_fonts_group);
	radiobutton_system_fonts_group =
		gtk_radio_button_get_group (GTK_RADIO_BUTTON
					    (radiobutton_system_fonts));

	combo_system_fonts = gtk_combo_new ();
	g_object_set_data (G_OBJECT (GTK_COMBO (combo_system_fonts)->popwin),
			   "GladeParentKey", combo_system_fonts);
	gtk_widget_show (combo_system_fonts);
	gtk_box_pack_start (GTK_BOX (hbox_system_fonts), combo_system_fonts,
			    TRUE, TRUE, 5);
	combo_system_fonts_items =
		g_list_append (combo_system_fonts_items,
			       (gpointer) "caption");
	combo_system_fonts_items =
		g_list_append (combo_system_fonts_items, (gpointer) "icon");
	combo_system_fonts_items =
		g_list_append (combo_system_fonts_items, (gpointer) "menu");
	combo_system_fonts_items =
		g_list_append (combo_system_fonts_items,
			       (gpointer) "message-box");
	combo_system_fonts_items =
		g_list_append (combo_system_fonts_items,
			       (gpointer) "small-caption");
	combo_system_fonts_items =
		g_list_append (combo_system_fonts_items,
			       (gpointer) "status-bar");
	gtk_combo_set_popdown_strings (GTK_COMBO (combo_system_fonts),
				       combo_system_fonts_items);
	g_list_free (combo_system_fonts_items);

	combo_entry_system_fonts = GTK_COMBO (combo_system_fonts)->entry;
	gtk_widget_show (combo_entry_system_fonts);
	gtk_editable_set_editable (GTK_EDITABLE (combo_entry_system_fonts),
				   FALSE);
	gtk_entry_set_text (GTK_ENTRY (combo_entry_system_fonts), "caption");

	hbox_typo = gtk_hbox_new (FALSE, 0);
	gtk_widget_show (hbox_typo);
	gtk_box_pack_start (GTK_BOX (vbox_main), hbox_typo, FALSE, TRUE, 0);

	radiobutton_typographical_notation =
		gtk_radio_button_new_with_mnemonic (NULL,
						    _
						    ("Use typographical notation"));
	gtk_widget_show (radiobutton_typographical_notation);
	gtk_box_pack_start (GTK_BOX (hbox_typo),
			    radiobutton_typographical_notation, FALSE, FALSE,
			    5);
	gtk_radio_button_set_group (GTK_RADIO_BUTTON
				    (radiobutton_typographical_notation),
				    radiobutton_system_fonts_group);
	radiobutton_system_fonts_group =
		gtk_radio_button_get_group (GTK_RADIO_BUTTON
					    (radiobutton_typographical_notation));

	label_blank = gtk_label_new ("");
	gtk_widget_show (label_blank);
	gtk_box_pack_start (GTK_BOX (hbox_typo), label_blank, TRUE, TRUE, 0);
	gtk_label_set_justify (GTK_LABEL (label_blank), GTK_JUSTIFY_LEFT);

	hbox_font_family = gtk_hbox_new (TRUE, 0);
	gtk_widget_show (hbox_font_family);
	gtk_box_pack_start (GTK_BOX (vbox_main), hbox_font_family, FALSE,
			    TRUE, 0);

	checkbutton_font_family =
		gtk_check_button_new_with_mnemonic ("font family");
	gtk_widget_show (checkbutton_font_family);
	gtk_box_pack_start (GTK_BOX (hbox_font_family),
			    checkbutton_font_family, TRUE, TRUE, 5);

	combo_font_family = gtk_combo_new ();
	g_object_set_data (G_OBJECT (GTK_COMBO (combo_font_family)->popwin),
			   "GladeParentKey", combo_font_family);
	gtk_widget_show (combo_font_family);
	gtk_box_pack_start (GTK_BOX (hbox_font_family), combo_font_family,
			    TRUE, TRUE, 5);
	combo_font_family_items =
		g_list_append (combo_font_family_items, (gpointer) "Arial");
	combo_font_family_items =
		g_list_append (combo_font_family_items,
			       (gpointer) "Arial Black");
	combo_font_family_items =
		g_list_append (combo_font_family_items, (gpointer) "Charter");
	combo_font_family_items =
		g_list_append (combo_font_family_items, (gpointer) "Chicago");
	combo_font_family_items =
		g_list_append (combo_font_family_items, (gpointer) "Clean");
	combo_font_family_items =
		g_list_append (combo_font_family_items,
			       (gpointer) "Comic Sans MS");
	combo_font_family_items =
		g_list_append (combo_font_family_items, (gpointer) "Courier");
	combo_font_family_items =
		g_list_append (combo_font_family_items,
			       (gpointer) "Courier New");
	combo_font_family_items =
		g_list_append (combo_font_family_items,
			       (gpointer) "Courier Regular");
	combo_font_family_items =
		g_list_append (combo_font_family_items, (gpointer) "Fixed");
	combo_font_family_items =
		g_list_append (combo_font_family_items, (gpointer) "Geneva");
	combo_font_family_items =
		g_list_append (combo_font_family_items,
			       (gpointer) "Helvetica");
	combo_font_family_items =
		g_list_append (combo_font_family_items, (gpointer) "Monaco");
	combo_font_family_items =
		g_list_append (combo_font_family_items,
			       (gpointer) "New York");
	combo_font_family_items =
		g_list_append (combo_font_family_items,
			       (gpointer) "Palatino");
	combo_font_family_items =
		g_list_append (combo_font_family_items, (gpointer) "Impact");
	combo_font_family_items =
		g_list_append (combo_font_family_items, (gpointer) "Lucida");
	combo_font_family_items =
		g_list_append (combo_font_family_items,
			       (gpointer) "Lucida Bright");
	combo_font_family_items =
		g_list_append (combo_font_family_items,
			       (gpointer) "Lucidatypewriter");
	combo_font_family_items =
		g_list_append (combo_font_family_items,
			       (gpointer) "Lucida Sans");
	combo_font_family_items =
		g_list_append (combo_font_family_items,
			       (gpointer) "Lucida Console");
	combo_font_family_items =
		g_list_append (combo_font_family_items,
			       (gpointer) "New Century Schoolbook");
	combo_font_family_items =
		g_list_append (combo_font_family_items, (gpointer) "Symbol");
	combo_font_family_items =
		g_list_append (combo_font_family_items, (gpointer) "Times");
	combo_font_family_items =
		g_list_append (combo_font_family_items,
			       (gpointer) "Times New Roman");
	combo_font_family_items =
		g_list_append (combo_font_family_items, (gpointer) "Utopia");
	combo_font_family_items =
		g_list_append (combo_font_family_items, (gpointer) "Verdana");
	combo_font_family_items =
		g_list_append (combo_font_family_items,
			       (gpointer) "Webdings");
	combo_font_family_items =
		g_list_append (combo_font_family_items,
			       (gpointer) "Windings");
	gtk_combo_set_popdown_strings (GTK_COMBO (combo_font_family),
				       combo_font_family_items);
	g_list_free (combo_font_family_items);

	combo_entry_font_family = GTK_COMBO (combo_font_family)->entry;
	gtk_widget_show (combo_entry_font_family);
	gtk_entry_set_text (GTK_ENTRY (combo_entry_font_family), "");

	hbox_generic_family = gtk_hbox_new (TRUE, 0);
	gtk_widget_show (hbox_generic_family);
	gtk_box_pack_start (GTK_BOX (vbox_main), hbox_generic_family, FALSE,
			    TRUE, 0);

	checkbutton_generic_family =
		gtk_check_button_new_with_mnemonic (_("generic family"));
	gtk_widget_show (checkbutton_generic_family);
	gtk_box_pack_start (GTK_BOX (hbox_generic_family),
			    checkbutton_generic_family, TRUE, TRUE, 5);

	combo_generic_family = gtk_combo_new ();
	g_object_set_data (G_OBJECT
			   (GTK_COMBO (combo_generic_family)->popwin),
			   "GladeParentKey", combo_generic_family);
	gtk_widget_show (combo_generic_family);
	gtk_box_pack_start (GTK_BOX (hbox_generic_family),
			    combo_generic_family, TRUE, TRUE, 5);
	combo_generic_family_items =
		g_list_append (combo_generic_family_items,
			       (gpointer) "serif");
	combo_generic_family_items =
		g_list_append (combo_generic_family_items,
			       (gpointer) "sans-serif");
	combo_generic_family_items =
		g_list_append (combo_generic_family_items,
			       (gpointer) "cursive");
	combo_generic_family_items =
		g_list_append (combo_generic_family_items,
			       (gpointer) "fantasy");
	combo_generic_family_items =
		g_list_append (combo_generic_family_items,
			       (gpointer) "monospace ");
	gtk_combo_set_popdown_strings (GTK_COMBO (combo_generic_family),
				       combo_generic_family_items);
	g_list_free (combo_generic_family_items);

	combo_entry_generic_family = GTK_COMBO (combo_generic_family)->entry;
	gtk_widget_show (combo_entry_generic_family);
	gtk_editable_set_editable (GTK_EDITABLE (combo_entry_generic_family),
				   TRUE);
	gtk_entry_set_text (GTK_ENTRY (combo_entry_generic_family), "");

	hbox_font_set = gtk_hbox_new (TRUE, 0);
	gtk_widget_show (hbox_font_set);
	gtk_box_pack_start (GTK_BOX (vbox_main), hbox_font_set, FALSE, TRUE,
			    5);

	checkbutton_font_set =
		gtk_check_button_new_with_mnemonic (_("Use font set"));
	gtk_widget_show (checkbutton_font_set);
	gtk_box_pack_start (GTK_BOX (hbox_font_set), checkbutton_font_set,
			    TRUE, TRUE, 5);

	hbuttonbox_fs = gtk_hbutton_box_new ();
	gtk_widget_show (hbuttonbox_fs);
	gtk_box_pack_start (GTK_BOX (hbox_font_set), hbuttonbox_fs, TRUE,
			    TRUE, 5);

	button_font_family_dialog_add = gtk_button_new_from_stock ("gtk-add");
	gtk_widget_show (button_font_family_dialog_add);
	gtk_container_add (GTK_CONTAINER (hbuttonbox_fs),
			   button_font_family_dialog_add);
	GTK_WIDGET_SET_FLAGS (button_font_family_dialog_add, GTK_CAN_DEFAULT);

	button_font_family_dialog_del =
		gtk_button_new_from_stock ("gtk-remove");
	gtk_widget_show (button_font_family_dialog_del);
	gtk_container_add (GTK_CONTAINER (hbuttonbox_fs),
			   button_font_family_dialog_del);
	GTK_WIDGET_SET_FLAGS (button_font_family_dialog_del, GTK_CAN_DEFAULT);

	scrolledwindow_font_set = gtk_scrolled_window_new (NULL, NULL);
	gtk_widget_show (scrolledwindow_font_set);
	gtk_box_pack_start (GTK_BOX (vbox_main), scrolledwindow_font_set,
			    TRUE, TRUE, 5);
	gtk_container_set_border_width (GTK_CONTAINER
					(scrolledwindow_font_set), 5);
	gtk_widget_set_size_request (scrolledwindow_font_set, -1, 125);

	store = gtk_list_store_new (2, G_TYPE_STRING, G_TYPE_STRING);
	treeview = gtk_tree_view_new_with_model (GTK_TREE_MODEL (store));
	gtk_widget_show (treeview);
	gtk_container_add (GTK_CONTAINER (scrolledwindow_font_set), treeview);

	ff_renderer = gtk_cell_renderer_text_new ();
	gf_renderer = gtk_cell_renderer_text_new ();

	col_font_family =
		gtk_tree_view_column_new_with_attributes (_("Font family"),
							  ff_renderer, "text",
							  0, NULL);
	col_generic_family =
		gtk_tree_view_column_new_with_attributes (_("Generic family"),
							  gf_renderer, "text",
							  1, NULL);
	gtk_tree_view_insert_column (GTK_TREE_VIEW (treeview),
				     col_font_family, 0);
	gtk_tree_view_insert_column (GTK_TREE_VIEW (treeview),
				     col_generic_family, 1);

	gtk_tree_view_set_headers_visible (GTK_TREE_VIEW (treeview), TRUE);

	dialog_action_area = GTK_DIALOG (font_family_dialog)->action_area;
	gtk_widget_show (dialog_action_area);
	gtk_button_box_set_layout (GTK_BUTTON_BOX (dialog_action_area),
				   GTK_BUTTONBOX_END);

	helpbutton = gtk_button_new_from_stock ("gtk-help");
	gtk_widget_show (helpbutton);
	gtk_dialog_add_action_widget (GTK_DIALOG (font_family_dialog),
				      helpbutton, GTK_RESPONSE_HELP);
	GTK_WIDGET_SET_FLAGS (helpbutton, GTK_CAN_DEFAULT);

	cancelbutton = gtk_button_new_from_stock ("gtk-cancel");
	gtk_widget_show (cancelbutton);
	gtk_dialog_add_action_widget (GTK_DIALOG (font_family_dialog),
				      cancelbutton, GTK_RESPONSE_CANCEL);
	GTK_WIDGET_SET_FLAGS (cancelbutton, GTK_CAN_DEFAULT);

	okbutton = gtk_button_new_from_stock ("gtk-ok");
	gtk_widget_show (okbutton);
	gtk_dialog_add_action_widget (GTK_DIALOG (font_family_dialog),
				      okbutton, GTK_RESPONSE_OK);
	GTK_WIDGET_SET_FLAGS (okbutton, GTK_CAN_DEFAULT);
	
	/* to update the checkboxes when a related widget is changed */			  
	g_signal_connect ((gpointer) combo_entry_system_fonts,
			  "changed", G_CALLBACK (on_widget_change_set_toggle_active),
			  radiobutton_system_fonts);		

	g_signal_connect ((gpointer) combo_entry_font_family,
			  "changed", G_CALLBACK (on_widget_change_set_toggle_active),
			  radiobutton_typographical_notation);
	g_signal_connect ((gpointer) combo_entry_generic_family,
			  "changed", G_CALLBACK (on_widget_change_set_toggle_active),
			  radiobutton_typographical_notation);

	g_signal_connect ((gpointer) combo_entry_font_family,
			  "changed", G_CALLBACK (on_widget_change_set_toggle_active),
			  checkbutton_font_family);	
	g_signal_connect ((gpointer) combo_entry_font_family,
			  "changed", G_CALLBACK (on_widget_change_unset_toggle_active),
			  checkbutton_font_set);				  			  	
			  				
	g_signal_connect ((gpointer) combo_entry_generic_family,
			  "changed", G_CALLBACK (on_widget_change_set_toggle_active),
			  checkbutton_generic_family);			  
	g_signal_connect ((gpointer) combo_entry_generic_family,
			  "changed", G_CALLBACK (on_widget_change_unset_toggle_active),
			  checkbutton_font_set);
			    
	if (connect_callbacks)
	{
		g_signal_connect ((gpointer) font_family_dialog, "close",
				  G_CALLBACK (on_css_dialog_close), data);
		g_signal_connect ((gpointer) helpbutton, "clicked",
				  G_CALLBACK
				  (on_font_family_dialog_helpbutton_clicked),
				  NULL);
		g_signal_connect ((gpointer) cancelbutton, "clicked",
				  G_CALLBACK
				  (on_css_dialog_cancel_button_clicked),
				  data);
		g_signal_connect ((gpointer) okbutton, "clicked",
				  G_CALLBACK
				  (on_font_family_dialog_okbutton_clicked),
				  data);
	}
	
	g_signal_connect ((gpointer) checkbutton_font_set, "toggled",
			  G_CALLBACK (on_button_font_fam_dialog_set_toggled),
			  radiobutton_typographical_notation);
	g_signal_connect ((gpointer) button_font_family_dialog_add, "clicked",
			  G_CALLBACK
			  (on_button_font_family_dialog_add_clicked), store);
	g_signal_connect ((gpointer) button_font_family_dialog_del, "clicked",
			  G_CALLBACK
			  (on_button_font_family_dialog_del_clicked), store);

	GLADE_HOOKUP_OBJECT_NO_REF (font_family_dialog, font_family_dialog,
				    "font_family_dialog");
	GLADE_HOOKUP_OBJECT (font_family_dialog, radiobutton_system_fonts,
			     "radiobutton_system_fonts");
	GLADE_HOOKUP_OBJECT (font_family_dialog, combo_system_fonts,
			     "combo_system_fonts");
	GLADE_HOOKUP_OBJECT (font_family_dialog, combo_entry_system_fonts,
			     "combo_entry_system_fonts");
	GLADE_HOOKUP_OBJECT (font_family_dialog,
			     radiobutton_typographical_notation,
			     "radiobutton_typographical_notation");
	GLADE_HOOKUP_OBJECT (font_family_dialog, checkbutton_font_family,
			     "checkbutton_font_family");
	GLADE_HOOKUP_OBJECT (font_family_dialog, combo_font_family,
			     "combo_font_family");
	GLADE_HOOKUP_OBJECT (font_family_dialog, combo_entry_font_family,
			     "combo_entry_font_family");
	GLADE_HOOKUP_OBJECT (font_family_dialog, checkbutton_generic_family,
			     "checkbutton_generic_family");
	GLADE_HOOKUP_OBJECT (font_family_dialog, combo_generic_family,
			     "combo_generic_family");
	GLADE_HOOKUP_OBJECT (font_family_dialog, combo_entry_generic_family,
			     "combo_entry_generic_family");
	GLADE_HOOKUP_OBJECT (font_family_dialog, checkbutton_font_set,
			     "checkbutton_font_set");
	GLADE_HOOKUP_OBJECT (font_family_dialog, hbuttonbox_fs,
			     "hbuttonbox_fs");
	GLADE_HOOKUP_OBJECT (font_family_dialog,
			     button_font_family_dialog_add,
			     "button_font_family_dialog_add");
	GLADE_HOOKUP_OBJECT (font_family_dialog,
			     button_font_family_dialog_del,
			     "button_font_family_dialog_del");
	GLADE_HOOKUP_OBJECT (font_family_dialog, scrolledwindow_font_set,
			     "scrolledwindow_font_set");
	GLADE_HOOKUP_OBJECT (font_family_dialog, treeview, "treeview");
	GLADE_HOOKUP_OBJECT (font_family_dialog, helpbutton, "helpbutton");
	GLADE_HOOKUP_OBJECT (font_family_dialog, cancelbutton,
			     "cancelbutton");
	GLADE_HOOKUP_OBJECT (font_family_dialog, okbutton, "okbutton");

	return font_family_dialog;
}

GtkWidget *
create_font_size_dialog (CssedWindow * window,
			 gchar * previous_string,
			 gboolean add_previous_string,
			 gboolean connect_callbacks)
{
	GtkWidget *font_size_dialog;
	GtkWidget *dialog_vbox;
	GtkWidget *hbox;
	GtkWidget *label_font_size;
	GtkWidget *combo_font_size;
	GList *combo_font_size_items = NULL;
	GtkWidget *combo_entry_font_size;
	GtkObject *spinbutton_font_size_adj;
	GtkWidget *spinbutton_font_size;
	GtkWidget *combo_font_size_units;
	GList *combo_font_size_units_items = NULL;
	GtkWidget *combo_entry_font_size_units;
	GtkWidget *dialog_action_area;
	GtkWidget *helpbutton;
	GtkWidget *cancelbutton;
	GtkWidget *okbutton;
	GtkAccelGroup *accel_group;
	GtkWidget *main_window;
	CssedCssDialogsData *data = NULL;

	if (connect_callbacks)
	{
		data = g_malloc (sizeof (CssedCssDialogsData));
		data->main_window = window;
		data->previous_string = previous_string;
		data->add_property_string = add_previous_string;
	}


	accel_group = gtk_accel_group_new ();

	font_size_dialog = gtk_dialog_new ();
	gtk_window_set_title (GTK_WINDOW (font_size_dialog), previous_string);
	gtk_window_set_modal (GTK_WINDOW (font_size_dialog), TRUE);

	main_window = cssed_window_get_window_widget( window );

	gtk_window_set_transient_for (GTK_WINDOW (font_size_dialog),
				      GTK_WINDOW (main_window));

	dialog_vbox = GTK_DIALOG (font_size_dialog)->vbox;
	gtk_widget_show (dialog_vbox);

	hbox = gtk_hbox_new (FALSE, 0);
	gtk_widget_show (hbox);
	gtk_box_pack_start (GTK_BOX (dialog_vbox), hbox, TRUE, TRUE, 0);

	label_font_size = gtk_label_new (previous_string);
	gtk_widget_show (label_font_size);
	gtk_box_pack_start (GTK_BOX (hbox), label_font_size, FALSE, FALSE, 5);
	gtk_label_set_justify (GTK_LABEL (label_font_size), GTK_JUSTIFY_LEFT);

	combo_font_size = gtk_combo_new ();
	g_object_set_data (G_OBJECT (GTK_COMBO (combo_font_size)->popwin),
			   "GladeParentKey", combo_font_size);
	gtk_widget_show (combo_font_size);
	gtk_box_pack_start (GTK_BOX (hbox), combo_font_size, TRUE, TRUE, 0);
	combo_font_size_items =
		g_list_append (combo_font_size_items,
			       (gpointer) CSS_STR_USE_PERCLEN);
	combo_font_size_items =
		g_list_append (combo_font_size_items, (gpointer) "xx-small");
	combo_font_size_items =
		g_list_append (combo_font_size_items, (gpointer) "x-small");
	combo_font_size_items =
		g_list_append (combo_font_size_items, (gpointer) "small");
	combo_font_size_items =
		g_list_append (combo_font_size_items, (gpointer) "medium");
	combo_font_size_items =
		g_list_append (combo_font_size_items, (gpointer) "large");
	combo_font_size_items =
		g_list_append (combo_font_size_items, (gpointer) "x-large");
	combo_font_size_items =
		g_list_append (combo_font_size_items, (gpointer) "xx-large");
	combo_font_size_items =
		g_list_append (combo_font_size_items, (gpointer) "larger");
	combo_font_size_items =
		g_list_append (combo_font_size_items, (gpointer) "smaller ");
	gtk_combo_set_popdown_strings (GTK_COMBO (combo_font_size),
				       combo_font_size_items);
	g_list_free (combo_font_size_items);

	combo_entry_font_size = GTK_COMBO (combo_font_size)->entry;
	gtk_widget_show (combo_entry_font_size);
	gtk_editable_set_editable (GTK_EDITABLE (combo_entry_font_size),
				   FALSE);
	gtk_entry_set_text (GTK_ENTRY (combo_entry_font_size),
			    CSS_STR_USE_PERCLEN);

	spinbutton_font_size_adj =
		gtk_adjustment_new (8, -35565, 35565, 1, 10, 10);
	spinbutton_font_size =
		gtk_spin_button_new (GTK_ADJUSTMENT
				     (spinbutton_font_size_adj), 1, 0);
	gtk_widget_show (spinbutton_font_size);
	gtk_box_pack_start (GTK_BOX (hbox), spinbutton_font_size, FALSE,
			    FALSE, 0);
	gtk_widget_set_size_request (spinbutton_font_size, 50, -1);

	combo_font_size_units = gtk_combo_new ();
	g_object_set_data (G_OBJECT
			   (GTK_COMBO (combo_font_size_units)->popwin),
			   "GladeParentKey", combo_font_size_units);
	gtk_widget_show (combo_font_size_units);
	gtk_box_pack_start (GTK_BOX (hbox), combo_font_size_units, FALSE,
			    FALSE, 0);
	combo_font_size_units_items =
		g_list_append (combo_font_size_units_items, (gpointer) "%");
	combo_font_size_units_items =
		g_list_append (combo_font_size_units_items, (gpointer) "em");
	combo_font_size_units_items =
		g_list_append (combo_font_size_units_items, (gpointer) "ex");
	combo_font_size_units_items =
		g_list_append (combo_font_size_units_items, (gpointer) "px");
	combo_font_size_units_items =
		g_list_append (combo_font_size_units_items, (gpointer) "in");
	combo_font_size_units_items =
		g_list_append (combo_font_size_units_items, (gpointer) "cm");
	combo_font_size_units_items =
		g_list_append (combo_font_size_units_items, (gpointer) "mm");
	combo_font_size_units_items =
		g_list_append (combo_font_size_units_items, (gpointer) "pt");
	combo_font_size_units_items =
		g_list_append (combo_font_size_units_items, (gpointer) "pc");
	gtk_combo_set_popdown_strings (GTK_COMBO (combo_font_size_units),
				       combo_font_size_units_items);
	g_list_free (combo_font_size_units_items);

	combo_entry_font_size_units =
		GTK_COMBO (combo_font_size_units)->entry;
	gtk_widget_show (combo_entry_font_size_units);
	gtk_widget_set_size_request (combo_entry_font_size_units, 40, -1);
	gtk_editable_set_editable (GTK_EDITABLE (combo_entry_font_size_units),
				   FALSE);
	gtk_entry_set_text (GTK_ENTRY (combo_entry_font_size_units), "px");

	dialog_action_area = GTK_DIALOG (font_size_dialog)->action_area;
	gtk_widget_show (dialog_action_area);
	gtk_button_box_set_layout (GTK_BUTTON_BOX (dialog_action_area),
				   GTK_BUTTONBOX_END);

	helpbutton = gtk_button_new_from_stock ("gtk-help");
	gtk_widget_show (helpbutton);
	gtk_dialog_add_action_widget (GTK_DIALOG (font_size_dialog),
				      helpbutton, GTK_RESPONSE_HELP);
	GTK_WIDGET_SET_FLAGS (helpbutton, GTK_CAN_DEFAULT);
	gtk_widget_add_accelerator (helpbutton, "clicked", accel_group,
				    GDK_y, GDK_CONTROL_MASK,
				    GTK_ACCEL_VISIBLE);
	gtk_widget_add_accelerator (helpbutton, "clicked", accel_group, GDK_h,
				    GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);

	cancelbutton = gtk_button_new_from_stock ("gtk-cancel");
	gtk_widget_show (cancelbutton);
	gtk_dialog_add_action_widget (GTK_DIALOG (font_size_dialog),
				      cancelbutton, GTK_RESPONSE_CANCEL);
	GTK_WIDGET_SET_FLAGS (cancelbutton, GTK_CAN_DEFAULT);
	gtk_widget_add_accelerator (cancelbutton, "clicked", accel_group,
				    GDK_c, GDK_CONTROL_MASK,
				    GTK_ACCEL_VISIBLE);

	okbutton = gtk_button_new_from_stock ("gtk-ok");
	gtk_widget_show (okbutton);
	gtk_dialog_add_action_widget (GTK_DIALOG (font_size_dialog), okbutton,
				      GTK_RESPONSE_OK);
	GTK_WIDGET_SET_FLAGS (okbutton, GTK_CAN_DEFAULT);
	gtk_widget_add_accelerator (okbutton, "clicked", accel_group,
				    GDK_o, GDK_CONTROL_MASK,
				    GTK_ACCEL_VISIBLE);

	g_signal_connect ((gpointer) combo_entry_font_size_units,
			  "changed", G_CALLBACK (on_css_dialog_unit_changed),
			  spinbutton_font_size);

	if (connect_callbacks)
	{
		g_signal_connect ((gpointer) font_size_dialog, "close",
				  G_CALLBACK (on_css_dialog_close), data);

		g_signal_connect ((gpointer) helpbutton, "clicked",
				  G_CALLBACK
				  (on_font_size_dialog_helpbutton_clicked),
				  NULL);
		g_signal_connect ((gpointer) cancelbutton, "clicked",
				  G_CALLBACK
				  (on_css_dialog_cancel_button_clicked),
				  data);
		g_signal_connect ((gpointer) okbutton, "clicked",
				  G_CALLBACK
				  (on_font_size_dialog_okbutton_clicked),
				  data);
	}

	GLADE_HOOKUP_OBJECT_NO_REF (font_size_dialog, font_size_dialog,
				    "font_size_dialog");
	GLADE_HOOKUP_OBJECT_NO_REF (font_size_dialog, dialog_vbox,
				    "dialog_vbox");
	GLADE_HOOKUP_OBJECT (font_size_dialog, hbox, "hbox");
	GLADE_HOOKUP_OBJECT (font_size_dialog, label_font_size,
			     "label_font_size");
	GLADE_HOOKUP_OBJECT (font_size_dialog, combo_font_size,
			     "combo_font_size");
	GLADE_HOOKUP_OBJECT (font_size_dialog, combo_entry_font_size,
			     "combo_entry_font_size");
	GLADE_HOOKUP_OBJECT (font_size_dialog, spinbutton_font_size,
			     "spinbutton_font_size");
	GLADE_HOOKUP_OBJECT (font_size_dialog, combo_font_size_units,
			     "combo_font_size_units");
	GLADE_HOOKUP_OBJECT (font_size_dialog, combo_entry_font_size_units,
			     "combo_entry_font_size_units");
	GLADE_HOOKUP_OBJECT_NO_REF (font_size_dialog, dialog_action_area,
				    "dialog_action_area");
	GLADE_HOOKUP_OBJECT (font_size_dialog, helpbutton, "helpbutton");
	GLADE_HOOKUP_OBJECT (font_size_dialog, cancelbutton, "cancelbutton");
	GLADE_HOOKUP_OBJECT (font_size_dialog, okbutton, "okbutton");

	gtk_window_add_accel_group (GTK_WINDOW (font_size_dialog),
				    accel_group);

	return font_size_dialog;
}

GtkWidget *
create_frecuency_property_dialog (CssedWindow * window,
				  gchar * previous_string,
				  gboolean add_previous_string,
				  gboolean connect_callbacks)
{
	GtkWidget *frecuency_property_dialog;
	GtkWidget *dialog_vbox;
	GtkWidget *hbox50;
	GtkWidget *label_frecuency;
	GtkObject *spinbutton_frecuency_adj;
	GtkWidget *spinbutton_frecuency;
	GtkWidget *combo_frecuency;
	GList *combo_frecuency_items = NULL;
	GtkWidget *combo_entry_frecuency;
	GtkWidget *dialog_action_area;
	GtkWidget *helpbutton;
	GtkWidget *cancelbutton;
	GtkWidget *okbutton;
	GtkAccelGroup *accel_group;
	GtkWidget *main_window;
	CssedCssDialogsData *data = NULL;

	if (connect_callbacks)
	{
		data = g_malloc (sizeof (CssedCssDialogsData));
		data->main_window = window;
		data->previous_string = previous_string;
		data->add_property_string = add_previous_string;
	}


	accel_group = gtk_accel_group_new ();

	frecuency_property_dialog = gtk_dialog_new ();
	gtk_window_set_title (GTK_WINDOW (frecuency_property_dialog),
			      previous_string);
	gtk_window_set_modal (GTK_WINDOW (frecuency_property_dialog), TRUE);

	main_window = cssed_window_get_window_widget( window );

	gtk_window_set_transient_for (GTK_WINDOW (frecuency_property_dialog),
				      GTK_WINDOW (main_window));

	dialog_vbox = GTK_DIALOG (frecuency_property_dialog)->vbox;
	gtk_widget_show (dialog_vbox);

	hbox50 = gtk_hbox_new (FALSE, 0);
	gtk_widget_show (hbox50);
	gtk_box_pack_start (GTK_BOX (dialog_vbox), hbox50, TRUE, TRUE, 0);

	label_frecuency = gtk_label_new ("frequency");
	gtk_widget_show (label_frecuency);
	gtk_box_pack_start (GTK_BOX (hbox50), label_frecuency, TRUE, TRUE, 0);
	gtk_label_set_justify (GTK_LABEL (label_frecuency), GTK_JUSTIFY_LEFT);

	spinbutton_frecuency_adj =gtk_adjustment_new (1, -35565, 35565, 1, 10, 10);
	spinbutton_frecuency =	gtk_spin_button_new (GTK_ADJUSTMENT(spinbutton_frecuency_adj), 1, 0);
	gtk_widget_show (spinbutton_frecuency);
	gtk_box_pack_start (GTK_BOX (hbox50), spinbutton_frecuency, FALSE,
			    FALSE, 5);

	combo_frecuency = gtk_combo_new ();
	g_object_set_data (G_OBJECT (GTK_COMBO (combo_frecuency)->popwin),
			   "GladeParentKey", combo_frecuency);
	gtk_widget_show (combo_frecuency);
	gtk_box_pack_start (GTK_BOX (hbox50), combo_frecuency, FALSE, FALSE,
			    5);
	combo_frecuency_items =
		g_list_append (combo_frecuency_items, (gpointer) "Hz");
	combo_frecuency_items =
		g_list_append (combo_frecuency_items, (gpointer) "kHz");
	gtk_combo_set_popdown_strings (GTK_COMBO (combo_frecuency),
				       combo_frecuency_items);
	g_list_free (combo_frecuency_items);

	combo_entry_frecuency = GTK_COMBO (combo_frecuency)->entry;
	gtk_widget_show (combo_entry_frecuency);
	gtk_widget_set_size_request (combo_entry_frecuency, 50, -1);
	gtk_editable_set_editable (GTK_EDITABLE (combo_entry_frecuency),
				   FALSE);
	gtk_entry_set_text (GTK_ENTRY (combo_entry_frecuency), "Hz");

	dialog_action_area =
		GTK_DIALOG (frecuency_property_dialog)->action_area;
	gtk_widget_show (dialog_action_area);
	gtk_button_box_set_layout (GTK_BUTTON_BOX (dialog_action_area),
				   GTK_BUTTONBOX_END);

	helpbutton = gtk_button_new_from_stock ("gtk-help");
	gtk_widget_show (helpbutton);
	gtk_dialog_add_action_widget (GTK_DIALOG (frecuency_property_dialog),
				      helpbutton, GTK_RESPONSE_HELP);
	GTK_WIDGET_SET_FLAGS (helpbutton, GTK_CAN_DEFAULT);
	gtk_widget_add_accelerator (helpbutton, "clicked", accel_group,
				    GDK_y, GDK_CONTROL_MASK,
				    GTK_ACCEL_VISIBLE);
	gtk_widget_add_accelerator (helpbutton, "clicked", accel_group, GDK_h,
				    GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);

	cancelbutton = gtk_button_new_from_stock ("gtk-cancel");
	gtk_widget_show (cancelbutton);
	gtk_dialog_add_action_widget (GTK_DIALOG (frecuency_property_dialog),
				      cancelbutton, GTK_RESPONSE_CANCEL);
	GTK_WIDGET_SET_FLAGS (cancelbutton, GTK_CAN_DEFAULT);

	okbutton = gtk_button_new_from_stock ("gtk-ok");
	gtk_widget_show (okbutton);
	gtk_dialog_add_action_widget (GTK_DIALOG (frecuency_property_dialog),
				      okbutton, GTK_RESPONSE_OK);
	GTK_WIDGET_SET_FLAGS (okbutton, GTK_CAN_DEFAULT);

	if (connect_callbacks)
	{
		g_signal_connect ((gpointer) frecuency_property_dialog,
				  "close", G_CALLBACK (on_css_dialog_close),
				  data);

		g_signal_connect ((gpointer) helpbutton, "clicked",
				  G_CALLBACK
				  (on_dialog_frecuency_helpbutton_clicked),
				  NULL);
		g_signal_connect ((gpointer) cancelbutton, "clicked",
				  G_CALLBACK
				  (on_css_dialog_cancel_button_clicked),
				  data);
		g_signal_connect ((gpointer) okbutton, "clicked",
				  G_CALLBACK
				  (on_frecuency_property_dialog_okbutton_clicked),
				  data);
	}

	GLADE_HOOKUP_OBJECT_NO_REF (frecuency_property_dialog,
				    frecuency_property_dialog,
				    "frecuency_property_dialog");
	GLADE_HOOKUP_OBJECT_NO_REF (frecuency_property_dialog, dialog_vbox,
				    "dialog_vbox");
	GLADE_HOOKUP_OBJECT (frecuency_property_dialog, hbox50, "hbox50");
	GLADE_HOOKUP_OBJECT (frecuency_property_dialog, label_frecuency,
			     "label_frecuency");
	GLADE_HOOKUP_OBJECT (frecuency_property_dialog, spinbutton_frecuency,
			     "spinbutton_frecuency");
	GLADE_HOOKUP_OBJECT (frecuency_property_dialog, combo_frecuency,
			     "combo_frecuency");
	GLADE_HOOKUP_OBJECT (frecuency_property_dialog, combo_entry_frecuency,
			     "combo_entry_frecuency");
	GLADE_HOOKUP_OBJECT_NO_REF (frecuency_property_dialog,
				    dialog_action_area, "dialog_action_area");
	GLADE_HOOKUP_OBJECT (frecuency_property_dialog, helpbutton,
			     "helpbutton");
	GLADE_HOOKUP_OBJECT (frecuency_property_dialog, cancelbutton,
			     "cancelbutton");
	GLADE_HOOKUP_OBJECT (frecuency_property_dialog, okbutton, "okbutton");

	gtk_window_add_accel_group (GTK_WINDOW (frecuency_property_dialog),
				    accel_group);

	return frecuency_property_dialog;
}



GtkWidget *
create_identifier_value_dialog (CssedWindow * window,
				gchar * previous_string,
				gboolean add_previous_string,
				gboolean connect_callbacks)
{
	GtkWidget *identifier_value_dialog;
	GtkWidget *dialog_vbox;
	GtkWidget *hbox;
	GtkWidget *label;
	GtkWidget *entry_identifier;
	GtkWidget *dialog_action_area;
	GtkWidget *helpbutton;
	GtkWidget *cancelbutton;
	GtkWidget *okbutton;
	GtkAccelGroup *accel_group;
	GtkWidget *main_window;
	CssedCssDialogsData *data = NULL;

	if (connect_callbacks)
	{
		data = g_malloc (sizeof (CssedCssDialogsData));
		data->main_window = window;
		data->previous_string = previous_string;
		data->add_property_string = add_previous_string;
	}

	accel_group = gtk_accel_group_new ();

	identifier_value_dialog = gtk_dialog_new ();
	gtk_window_set_title (GTK_WINDOW (identifier_value_dialog),
			      previous_string);
	gtk_window_set_modal (GTK_WINDOW (identifier_value_dialog), TRUE);

	main_window = cssed_window_get_window_widget( window );

	gtk_window_set_transient_for (GTK_WINDOW (identifier_value_dialog),
				      GTK_WINDOW (main_window));

	dialog_vbox = GTK_DIALOG (identifier_value_dialog)->vbox;
	gtk_widget_show (dialog_vbox);

	hbox = gtk_hbox_new (TRUE, 0);
	gtk_widget_show (hbox);
	gtk_box_pack_start (GTK_BOX (dialog_vbox), hbox, TRUE, TRUE, 0);

	label = gtk_label_new (_("Identifier"));
	gtk_widget_show (label);
	gtk_box_pack_start (GTK_BOX (hbox), label, TRUE, TRUE, 0);
	gtk_label_set_justify (GTK_LABEL (label), GTK_JUSTIFY_LEFT);

	entry_identifier = gtk_entry_new ();
	gtk_widget_show (entry_identifier);
	gtk_box_pack_start (GTK_BOX (hbox), entry_identifier, TRUE, TRUE, 5);

	dialog_action_area =
		GTK_DIALOG (identifier_value_dialog)->action_area;
	gtk_widget_show (dialog_action_area);
	gtk_button_box_set_layout (GTK_BUTTON_BOX (dialog_action_area),
				   GTK_BUTTONBOX_END);

	helpbutton = gtk_button_new_from_stock ("gtk-help");
	gtk_widget_show (helpbutton);
	gtk_dialog_add_action_widget (GTK_DIALOG (identifier_value_dialog),
				      helpbutton, GTK_RESPONSE_HELP);
	GTK_WIDGET_SET_FLAGS (helpbutton, GTK_CAN_DEFAULT);
	gtk_widget_add_accelerator (helpbutton, "clicked", accel_group,
				    GDK_y, GDK_CONTROL_MASK,
				    GTK_ACCEL_VISIBLE);

	cancelbutton = gtk_button_new_from_stock ("gtk-cancel");
	gtk_widget_show (cancelbutton);
	gtk_dialog_add_action_widget (GTK_DIALOG (identifier_value_dialog),
				      cancelbutton, GTK_RESPONSE_CANCEL);
	GTK_WIDGET_SET_FLAGS (cancelbutton, GTK_CAN_DEFAULT);
	gtk_widget_add_accelerator (cancelbutton, "clicked", accel_group,
				    GDK_c, GDK_CONTROL_MASK,
				    GTK_ACCEL_VISIBLE);

	okbutton = gtk_button_new_from_stock ("gtk-ok");
	gtk_widget_show (okbutton);
	gtk_dialog_add_action_widget (GTK_DIALOG (identifier_value_dialog),
				      okbutton, GTK_RESPONSE_OK);
	GTK_WIDGET_SET_FLAGS (okbutton, GTK_CAN_DEFAULT);
	gtk_widget_add_accelerator (okbutton, "clicked", accel_group,
				    GDK_o, GDK_CONTROL_MASK,
				    GTK_ACCEL_VISIBLE);
	if (connect_callbacks)
	{
		g_signal_connect ((gpointer) identifier_value_dialog, "close",
				  G_CALLBACK (on_css_dialog_close), data);
		g_signal_connect ((gpointer) helpbutton, "clicked",
				  G_CALLBACK
				  (on_identifier_value_helpbutton_clicked),
				  NULL);
		g_signal_connect ((gpointer) cancelbutton, "clicked",
				  G_CALLBACK
				  (on_css_dialog_cancel_button_clicked),
				  data);
		g_signal_connect ((gpointer) okbutton, "clicked",
				  G_CALLBACK
				  (on_identifier_value_dialog_okbutton_clicked),
				  data);
	}
	/*
	 * Store pointers to all widgets, for use by lookup_widget(). 
	 */
	GLADE_HOOKUP_OBJECT_NO_REF (identifier_value_dialog,
				    identifier_value_dialog,
				    "identifier_value_dialog");
	GLADE_HOOKUP_OBJECT_NO_REF (identifier_value_dialog, dialog_vbox,
				    "dialog_vbox");
	GLADE_HOOKUP_OBJECT (identifier_value_dialog, hbox, "hbox");
	GLADE_HOOKUP_OBJECT (identifier_value_dialog, label, "label");
	GLADE_HOOKUP_OBJECT (identifier_value_dialog, entry_identifier,
			     "entry_identifier");
	GLADE_HOOKUP_OBJECT_NO_REF (identifier_value_dialog,
				    dialog_action_area, "dialog_action_area");
	GLADE_HOOKUP_OBJECT (identifier_value_dialog, helpbutton,
			     "helpbutton");
	GLADE_HOOKUP_OBJECT (identifier_value_dialog, cancelbutton,
			     "cancelbutton");
	GLADE_HOOKUP_OBJECT (identifier_value_dialog, okbutton, "okbutton");

	gtk_window_add_accel_group (GTK_WINDOW (identifier_value_dialog),
				    accel_group);

	return identifier_value_dialog;
}

GtkWidget *
create_integer_value_dialog (CssedWindow * window,
			     gchar * previous_string,
			     gboolean add_previous_string,
			     gboolean connect_callbacks)
{
	GtkWidget *integer_value_dialog;
	GtkWidget *dialog_vbox;
	GtkWidget *hbox;
	GtkWidget *label_integer;
	GtkObject *spinbutton_integer_adj;
	GtkWidget *spinbutton_integer;
	GtkWidget *dialog_action_area;
	GtkWidget *helpbutton;
	GtkWidget *cancelbutton;
	GtkWidget *okbutton;
	GtkAccelGroup *accel_group;
	GtkWidget *main_window;
	CssedCssDialogsData *data = NULL;

	if (connect_callbacks)
	{
		data = g_malloc (sizeof (CssedCssDialogsData));
		data->main_window = window;
		data->previous_string = previous_string;
		data->add_property_string = add_previous_string;
	}

	accel_group = gtk_accel_group_new ();

	integer_value_dialog = gtk_dialog_new ();
	gtk_window_set_title (GTK_WINDOW (integer_value_dialog),
			      previous_string);
	// ~ gtk_window_set_position (GTK_WINDOW (integer_value_dialog),
	// ~ GTK_WIN_POS_CENTER);
	gtk_window_set_modal (GTK_WINDOW (integer_value_dialog), TRUE);

	main_window = cssed_window_get_window_widget( window );

	gtk_window_set_transient_for (GTK_WINDOW (integer_value_dialog),
				      GTK_WINDOW (main_window));

	dialog_vbox = GTK_DIALOG (integer_value_dialog)->vbox;
	gtk_widget_show (dialog_vbox);

	hbox = gtk_hbox_new (FALSE, 0);
	gtk_widget_show (hbox);
	gtk_box_pack_start (GTK_BOX (dialog_vbox), hbox, TRUE, TRUE, 0);

	label_integer = gtk_label_new (previous_string);
	gtk_widget_show (label_integer);
	gtk_box_pack_start (GTK_BOX (hbox), label_integer, TRUE, TRUE, 0);
	gtk_label_set_justify (GTK_LABEL (label_integer), GTK_JUSTIFY_LEFT);

	spinbutton_integer_adj =
		gtk_adjustment_new (1, -35565, 35565, 1, 10, 10);
	spinbutton_integer =
		gtk_spin_button_new (GTK_ADJUSTMENT (spinbutton_integer_adj),
				     1, 0);
	gtk_widget_show (spinbutton_integer);
	gtk_box_pack_start (GTK_BOX (hbox), spinbutton_integer, FALSE, TRUE,
			    5);

	dialog_action_area = GTK_DIALOG (integer_value_dialog)->action_area;
	gtk_widget_show (dialog_action_area);
	gtk_button_box_set_layout (GTK_BUTTON_BOX (dialog_action_area),
				   GTK_BUTTONBOX_END);

	helpbutton = gtk_button_new_from_stock ("gtk-help");
	gtk_widget_show (helpbutton);
	gtk_dialog_add_action_widget (GTK_DIALOG (integer_value_dialog),
				      helpbutton, GTK_RESPONSE_HELP);
	GTK_WIDGET_SET_FLAGS (helpbutton, GTK_CAN_DEFAULT);
	gtk_widget_add_accelerator (helpbutton, "clicked", accel_group,
				    GDK_y, GDK_CONTROL_MASK,
				    GTK_ACCEL_VISIBLE);
	gtk_widget_add_accelerator (helpbutton, "clicked", accel_group, GDK_h,
				    GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);

	cancelbutton = gtk_button_new_from_stock ("gtk-cancel");
	gtk_widget_show (cancelbutton);
	gtk_dialog_add_action_widget (GTK_DIALOG (integer_value_dialog),
				      cancelbutton, GTK_RESPONSE_CANCEL);
	GTK_WIDGET_SET_FLAGS (cancelbutton, GTK_CAN_DEFAULT);
	gtk_widget_add_accelerator (cancelbutton, "clicked", accel_group,
				    GDK_c, GDK_CONTROL_MASK,
				    GTK_ACCEL_VISIBLE);

	okbutton = gtk_button_new_from_stock ("gtk-ok");
	gtk_widget_show (okbutton);
	gtk_dialog_add_action_widget (GTK_DIALOG (integer_value_dialog),
				      okbutton, GTK_RESPONSE_OK);
	GTK_WIDGET_SET_FLAGS (okbutton, GTK_CAN_DEFAULT);
	gtk_widget_add_accelerator (okbutton, "clicked", accel_group,
				    GDK_o, GDK_CONTROL_MASK,
				    GTK_ACCEL_VISIBLE);
	if (connect_callbacks)
	{
		g_signal_connect ((gpointer) integer_value_dialog, "close",
				  G_CALLBACK (on_css_dialog_close), data);

		g_signal_connect ((gpointer) helpbutton, "clicked",
				  G_CALLBACK
				  (on_dialog_integer_helpbutton_clicked),
				  NULL);
		g_signal_connect ((gpointer) cancelbutton, "clicked",
				  G_CALLBACK
				  (on_css_dialog_cancel_button_clicked),
				  data);
		g_signal_connect ((gpointer) okbutton, "clicked",
				  G_CALLBACK
				  (on_integer_value_dialog_okbutton_clicked),
				  data);
	}
	/*
	 * Store pointers to all widgets, for use by lookup_widget(). 
	 */
	GLADE_HOOKUP_OBJECT_NO_REF (integer_value_dialog,
				    integer_value_dialog,
				    "integer_value_dialog");
	GLADE_HOOKUP_OBJECT_NO_REF (integer_value_dialog, dialog_vbox,
				    "dialog_vbox");
	GLADE_HOOKUP_OBJECT (integer_value_dialog, hbox, "hbox");
	GLADE_HOOKUP_OBJECT (integer_value_dialog, label_integer,
			     "label_integer");
	GLADE_HOOKUP_OBJECT (integer_value_dialog, spinbutton_integer,
			     "spinbutton_integer");
	GLADE_HOOKUP_OBJECT_NO_REF (integer_value_dialog, dialog_action_area,
				    "dialog_action_area");
	GLADE_HOOKUP_OBJECT (integer_value_dialog, helpbutton, "helpbutton");
	GLADE_HOOKUP_OBJECT (integer_value_dialog, cancelbutton,
			     "cancelbutton");
	GLADE_HOOKUP_OBJECT (integer_value_dialog, okbutton, "okbutton");

	gtk_window_add_accel_group (GTK_WINDOW (integer_value_dialog),
				    accel_group);

	return integer_value_dialog;
}

GtkWidget *
create_length_value_dialog (CssedWindow * window,
			    gchar * previous_string,
			    gboolean add_previous_string,
			    gboolean connect_callbacks)
{
	GtkWidget *length_value_dialog;
	GtkWidget *dialog_vbox;
	GtkWidget *hbox;
	GtkWidget *label_length;
	GtkObject *spinbutton_length_adj;
	GtkWidget *spinbutton_length;
	GtkWidget *combo_length;
	GList *combo_length_items = NULL;
	GtkWidget *combo_entry_length;
	GtkWidget *dialog_action_area;
	GtkWidget *helpbutton;
	GtkWidget *cancelbutton;
	GtkWidget *okbutton;
	GtkAccelGroup *accel_group;
	GtkWidget *main_window;
	CssedCssDialogsData *data = NULL;

	if (connect_callbacks)
	{
		data = g_malloc (sizeof (CssedCssDialogsData));
		data->main_window = window;
		data->previous_string = previous_string;
		data->add_property_string = add_previous_string;
	}

	accel_group = gtk_accel_group_new ();

	length_value_dialog = gtk_dialog_new ();
	gtk_window_set_title (GTK_WINDOW (length_value_dialog),
			      previous_string);
	// ~ gtk_window_set_position (GTK_WINDOW (length_value_dialog),
	// ~ GTK_WIN_POS_CENTER);
	gtk_window_set_modal (GTK_WINDOW (length_value_dialog), TRUE);

	main_window = cssed_window_get_window_widget( window );

	gtk_window_set_transient_for (GTK_WINDOW (length_value_dialog),
				      GTK_WINDOW (main_window));

	dialog_vbox = GTK_DIALOG (length_value_dialog)->vbox;
	gtk_widget_show (dialog_vbox);

	hbox = gtk_hbox_new (FALSE, 0);
	gtk_widget_show (hbox);
	gtk_box_pack_start (GTK_BOX (dialog_vbox), hbox, TRUE, TRUE, 0);

	label_length = gtk_label_new (previous_string);
	gtk_widget_show (label_length);
	gtk_box_pack_start (GTK_BOX (hbox), label_length, TRUE, TRUE, 0);
	gtk_label_set_justify (GTK_LABEL (label_length), GTK_JUSTIFY_LEFT);

	spinbutton_length_adj =
		gtk_adjustment_new (1, -35565, 35565, 1, 10, 10);
	spinbutton_length =
		gtk_spin_button_new (GTK_ADJUSTMENT (spinbutton_length_adj),
				     1, 0);
	gtk_widget_show (spinbutton_length);
	gtk_box_pack_start (GTK_BOX (hbox), spinbutton_length, FALSE, FALSE,
			    5);

	combo_length = gtk_combo_new ();
	g_object_set_data (G_OBJECT (GTK_COMBO (combo_length)->popwin),
			   "GladeParentKey", combo_length);
	gtk_widget_show (combo_length);
	gtk_box_pack_start (GTK_BOX (hbox), combo_length, FALSE, FALSE, 5);
	combo_length_items =
		g_list_append (combo_length_items, (gpointer) "em");
	combo_length_items =
		g_list_append (combo_length_items, (gpointer) "ex");
	combo_length_items =
		g_list_append (combo_length_items, (gpointer) "px");
	combo_length_items =
		g_list_append (combo_length_items, (gpointer) "in");
	combo_length_items =
		g_list_append (combo_length_items, (gpointer) "cm");
	combo_length_items =
		g_list_append (combo_length_items, (gpointer) "mm");
	combo_length_items =
		g_list_append (combo_length_items, (gpointer) "pt");
	combo_length_items =
		g_list_append (combo_length_items, (gpointer) "pc");
	gtk_combo_set_popdown_strings (GTK_COMBO (combo_length),
				       combo_length_items);
	g_list_free (combo_length_items);

	combo_entry_length = GTK_COMBO (combo_length)->entry;
	gtk_widget_show (combo_entry_length);
	gtk_widget_set_size_request (combo_entry_length, 25, -1);
	gtk_entry_set_text (GTK_ENTRY (combo_entry_length), "px");

	dialog_action_area = GTK_DIALOG (length_value_dialog)->action_area;
	gtk_widget_show (dialog_action_area);
	gtk_button_box_set_layout (GTK_BUTTON_BOX (dialog_action_area),
				   GTK_BUTTONBOX_END);

	helpbutton = gtk_button_new_from_stock ("gtk-help");
	gtk_widget_show (helpbutton);
	gtk_dialog_add_action_widget (GTK_DIALOG (length_value_dialog),
				      helpbutton, GTK_RESPONSE_HELP);
	GTK_WIDGET_SET_FLAGS (helpbutton, GTK_CAN_DEFAULT);
	gtk_widget_add_accelerator (helpbutton, "clicked", accel_group,
				    GDK_y, GDK_CONTROL_MASK,
				    GTK_ACCEL_VISIBLE);
	gtk_widget_add_accelerator (helpbutton, "clicked", accel_group, GDK_h,
				    GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);

	cancelbutton = gtk_button_new_from_stock ("gtk-cancel");
	gtk_widget_show (cancelbutton);
	gtk_dialog_add_action_widget (GTK_DIALOG (length_value_dialog),
				      cancelbutton, GTK_RESPONSE_CANCEL);
	GTK_WIDGET_SET_FLAGS (cancelbutton, GTK_CAN_DEFAULT);
	gtk_widget_add_accelerator (cancelbutton, "clicked", accel_group,
				    GDK_c, GDK_CONTROL_MASK,
				    GTK_ACCEL_VISIBLE);

	okbutton = gtk_button_new_from_stock ("gtk-ok");
	gtk_widget_show (okbutton);
	gtk_dialog_add_action_widget (GTK_DIALOG (length_value_dialog),
				      okbutton, GTK_RESPONSE_OK);
	GTK_WIDGET_SET_FLAGS (okbutton, GTK_CAN_DEFAULT);
	gtk_widget_add_accelerator (okbutton, "clicked", accel_group,
				    GDK_o, GDK_CONTROL_MASK,
				    GTK_ACCEL_VISIBLE);

	g_signal_connect ((gpointer) combo_entry_length,
			  "changed", G_CALLBACK (on_css_dialog_unit_changed),
			  spinbutton_length);

	if (connect_callbacks)
	{
		g_signal_connect ((gpointer) length_value_dialog, "close",
				  G_CALLBACK (on_css_dialog_close), data);

		g_signal_connect ((gpointer) helpbutton, "clicked",
				  G_CALLBACK
				  (on_length_value_dialog_helpbutton_clicked),
				  NULL);
		g_signal_connect ((gpointer) cancelbutton, "clicked",
				  G_CALLBACK
				  (on_css_dialog_cancel_button_clicked),
				  data);
		g_signal_connect ((gpointer) okbutton, "clicked",
				  G_CALLBACK
				  (on_length_value_dialog_okbutton_clicked),
				  data);
	}
	/*
	 * Store pointers to all widgets, for use by lookup_widget(). 
	 */
	GLADE_HOOKUP_OBJECT_NO_REF (length_value_dialog, length_value_dialog,
				    "length_value_dialog");
	GLADE_HOOKUP_OBJECT_NO_REF (length_value_dialog, dialog_vbox,
				    "dialog_vbox");
	GLADE_HOOKUP_OBJECT (length_value_dialog, hbox, "hbox");
	GLADE_HOOKUP_OBJECT (length_value_dialog, label_length,
			     "label_length");
	GLADE_HOOKUP_OBJECT (length_value_dialog, spinbutton_length,
			     "spinbutton_length");
	GLADE_HOOKUP_OBJECT (length_value_dialog, combo_length,
			     "combo_length");
	GLADE_HOOKUP_OBJECT (length_value_dialog, combo_entry_length,
			     "combo_entry_length");
	GLADE_HOOKUP_OBJECT_NO_REF (length_value_dialog, dialog_action_area,
				    "dialog_action_area");
	GLADE_HOOKUP_OBJECT (length_value_dialog, helpbutton, "helpbutton");
	GLADE_HOOKUP_OBJECT (length_value_dialog, cancelbutton,
			     "cancelbutton");
	GLADE_HOOKUP_OBJECT (length_value_dialog, okbutton, "okbutton");

	gtk_window_add_accel_group (GTK_WINDOW (length_value_dialog),
				    accel_group);

	return length_value_dialog;
}

GtkWidget *
create_list_style_dialog (CssedWindow * window,
			  gchar * previous_string,
			  gboolean add_previous_string,
			  gboolean connect_callbacks)
{
	GtkWidget *list_style_dialog;
	GtkWidget *dialog_vbox;
	GtkWidget *vbox;
	GtkWidget *hbox_position;
	GtkWidget *checkbutton_position;
	GtkWidget *combo_position;
	GList *combo_position_items = NULL;
	GtkWidget *combo_entry_position;
	GtkWidget *hbox_type;
	GtkWidget *checkbutton_type;
	GtkWidget *combo_type;
	GList *combo_type_items = NULL;
	GtkWidget *combo_entry_type;
	GtkWidget *hbox_image;
	GtkWidget *checkbutton_image;
	GtkWidget *entry_image;
	GtkWidget *dialog_action_area;
	GtkWidget *helpbutton;
	GtkWidget *cancelbutton;
	GtkWidget *okbutton;
	GtkAccelGroup *accel_group;
	GtkWidget *main_window;
	CssedCssDialogsData *data = NULL;

	if (connect_callbacks)
	{
		data = g_malloc (sizeof (CssedCssDialogsData));
		data->main_window = window;
		data->previous_string = previous_string;
		data->add_property_string = add_previous_string;
	}

	accel_group = gtk_accel_group_new ();

	list_style_dialog = gtk_dialog_new ();
	gtk_window_set_title (GTK_WINDOW (list_style_dialog),
			      previous_string);

	gtk_window_set_modal (GTK_WINDOW (list_style_dialog), TRUE);
	main_window = cssed_window_get_window_widget( window );

	gtk_window_set_transient_for (GTK_WINDOW (list_style_dialog),
				      GTK_WINDOW (main_window));

	dialog_vbox = GTK_DIALOG (list_style_dialog)->vbox;
	gtk_widget_show (dialog_vbox);

	vbox = gtk_vbox_new (FALSE, 0);
	gtk_widget_show (vbox);
	gtk_box_pack_start (GTK_BOX (dialog_vbox), vbox, TRUE, TRUE, 0);

	hbox_position = gtk_hbox_new (TRUE, 0);
	gtk_widget_show (hbox_position);
	gtk_box_pack_start (GTK_BOX (vbox), hbox_position, TRUE, TRUE, 0);

	checkbutton_position =
		gtk_check_button_new_with_mnemonic (_("list style position"));
	gtk_widget_show (checkbutton_position);
	gtk_box_pack_start (GTK_BOX (hbox_position), checkbutton_position,
			    TRUE, TRUE, 5);

	combo_position = gtk_combo_new ();
	g_object_set_data (G_OBJECT (GTK_COMBO (combo_position)->popwin),
			   "GladeParentKey", combo_position);
	gtk_widget_show (combo_position);
	gtk_box_pack_start (GTK_BOX (hbox_position), combo_position, TRUE,
			    TRUE, 5);
	combo_position_items =
		g_list_append (combo_position_items, (gpointer) "inside");
	combo_position_items =
		g_list_append (combo_position_items, (gpointer) "outside");
	gtk_combo_set_popdown_strings (GTK_COMBO (combo_position),
				       combo_position_items);
	g_list_free (combo_position_items);

	combo_entry_position = GTK_COMBO (combo_position)->entry;
	gtk_widget_show (combo_entry_position);
	gtk_entry_set_text (GTK_ENTRY (combo_entry_position), "inside");

	hbox_type = gtk_hbox_new (TRUE, 0);
	gtk_widget_show (hbox_type);
	gtk_box_pack_start (GTK_BOX (vbox), hbox_type, TRUE, TRUE, 0);

	checkbutton_type =
		gtk_check_button_new_with_mnemonic (_("list style type"));
	gtk_widget_show (checkbutton_type);
	gtk_box_pack_start (GTK_BOX (hbox_type), checkbutton_type, TRUE, TRUE,
			    5);

	combo_type = gtk_combo_new ();
	g_object_set_data (G_OBJECT (GTK_COMBO (combo_type)->popwin),
			   "GladeParentKey", combo_type);
	gtk_widget_show (combo_type);
	gtk_box_pack_start (GTK_BOX (hbox_type), combo_type, FALSE, FALSE, 0);
	combo_type_items =
		g_list_append (combo_type_items, (gpointer) "disc");
	combo_type_items =
		g_list_append (combo_type_items, (gpointer) "circle");
	combo_type_items =
		g_list_append (combo_type_items, (gpointer) "square");
	combo_type_items =
		g_list_append (combo_type_items, (gpointer) "decimal");
	combo_type_items =
		g_list_append (combo_type_items,
			       (gpointer) "decimal-leading-zero");
	combo_type_items =
		g_list_append (combo_type_items, (gpointer) "lower-roman");
	combo_type_items =
		g_list_append (combo_type_items, (gpointer) "upper-roman");
	combo_type_items =
		g_list_append (combo_type_items, (gpointer) "lower-greek");
	combo_type_items =
		g_list_append (combo_type_items, (gpointer) "lower-alpha");
	combo_type_items =
		g_list_append (combo_type_items, (gpointer) "lower-latin");
	combo_type_items =
		g_list_append (combo_type_items, (gpointer) "upper-alpha");
	combo_type_items =
		g_list_append (combo_type_items, (gpointer) "upper-latin");
	combo_type_items =
		g_list_append (combo_type_items, (gpointer) "hebrew");
	combo_type_items =
		g_list_append (combo_type_items, (gpointer) "armenian");
	combo_type_items =
		g_list_append (combo_type_items, (gpointer) "georgian");
	combo_type_items =
		g_list_append (combo_type_items,
			       (gpointer) "cjk-ideographic");
	combo_type_items =
		g_list_append (combo_type_items, (gpointer) "hiragana");
	combo_type_items =
		g_list_append (combo_type_items, (gpointer) "katakana");
	combo_type_items =
		g_list_append (combo_type_items, (gpointer) "hiragana-iroha");
	combo_type_items =
		g_list_append (combo_type_items, (gpointer) "katakana-iroha");
	gtk_combo_set_popdown_strings (GTK_COMBO (combo_type),
				       combo_type_items);
	g_list_free (combo_type_items);

	combo_entry_type = GTK_COMBO (combo_type)->entry;
	gtk_widget_show (combo_entry_type);
	gtk_editable_set_editable (GTK_EDITABLE (combo_entry_type), FALSE);
	gtk_entry_set_text (GTK_ENTRY (combo_entry_type), "disc");

	hbox_image = gtk_hbox_new (TRUE, 0);
	gtk_widget_show (hbox_image);
	gtk_box_pack_start (GTK_BOX (vbox), hbox_image, TRUE, TRUE, 0);

	checkbutton_image =
		gtk_check_button_new_with_mnemonic (_
						    ("list style image (uri)"));
	gtk_widget_show (checkbutton_image);
	gtk_box_pack_start (GTK_BOX (hbox_image), checkbutton_image, TRUE,
			    TRUE, 5);

	entry_image = gtk_entry_new ();
	gtk_widget_show (entry_image);
	gtk_box_pack_start (GTK_BOX (hbox_image), entry_image, TRUE, TRUE, 5);

	dialog_action_area = GTK_DIALOG (list_style_dialog)->action_area;
	gtk_widget_show (dialog_action_area);
	gtk_button_box_set_layout (GTK_BUTTON_BOX (dialog_action_area),
				   GTK_BUTTONBOX_END);

	helpbutton = gtk_button_new_from_stock ("gtk-help");
	gtk_widget_show (helpbutton);
	gtk_dialog_add_action_widget (GTK_DIALOG (list_style_dialog),
				      helpbutton, GTK_RESPONSE_HELP);
	GTK_WIDGET_SET_FLAGS (helpbutton, GTK_CAN_DEFAULT);
	gtk_widget_add_accelerator (helpbutton, "clicked", accel_group,
				    GDK_y, GDK_CONTROL_MASK,
				    GTK_ACCEL_VISIBLE);
	gtk_widget_add_accelerator (helpbutton, "clicked", accel_group, GDK_h,
				    GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);

	cancelbutton = gtk_button_new_from_stock ("gtk-cancel");
	gtk_widget_show (cancelbutton);
	gtk_dialog_add_action_widget (GTK_DIALOG (list_style_dialog),
				      cancelbutton, GTK_RESPONSE_CANCEL);
	GTK_WIDGET_SET_FLAGS (cancelbutton, GTK_CAN_DEFAULT);
	gtk_widget_add_accelerator (cancelbutton, "clicked", accel_group,
				    GDK_c, GDK_CONTROL_MASK,
				    GTK_ACCEL_VISIBLE);

	okbutton = gtk_button_new_from_stock ("gtk-ok");
	gtk_widget_show (okbutton);
	gtk_dialog_add_action_widget (GTK_DIALOG (list_style_dialog),
				      okbutton, GTK_RESPONSE_OK);
	GTK_WIDGET_SET_FLAGS (okbutton, GTK_CAN_DEFAULT);
	gtk_widget_add_accelerator (okbutton, "clicked", accel_group,
				    GDK_o, GDK_CONTROL_MASK,
				    GTK_ACCEL_VISIBLE);
				    
	/* to update the checkboxes when a related widget is changed */				    
	g_signal_connect ((gpointer) combo_entry_position,
			  "changed", G_CALLBACK (on_widget_change_set_toggle_active),
			  checkbutton_position );
	g_signal_connect ((gpointer) combo_entry_type,
			  "changed", G_CALLBACK (on_widget_change_set_toggle_active),
			  checkbutton_type);
	g_signal_connect ((gpointer) entry_image,
			  "changed", G_CALLBACK (on_widget_change_set_toggle_active),
			  checkbutton_image );
			  	  
	if (connect_callbacks)
	{
		g_signal_connect ((gpointer) list_style_dialog, "close",
				  G_CALLBACK (on_css_dialog_close), data);

		g_signal_connect ((gpointer) helpbutton, "clicked",
				  G_CALLBACK
				  (on_list_style_dialog_helpbutton_clicked),
				  NULL);
		g_signal_connect ((gpointer) cancelbutton, "clicked",
				  G_CALLBACK
				  (on_css_dialog_cancel_button_clicked),
				  data);
		g_signal_connect ((gpointer) okbutton, "clicked",
				  G_CALLBACK
				  (on_list_style_dialog_okbutton_clicked),
				  data);
	}
	/*
	 * Store pointers to all widgets, for use by lookup_widget(). 
	 */
	GLADE_HOOKUP_OBJECT_NO_REF (list_style_dialog, list_style_dialog,
				    "list_style_dialog");
	GLADE_HOOKUP_OBJECT (list_style_dialog, checkbutton_position,
			     "checkbutton_position");
	GLADE_HOOKUP_OBJECT (list_style_dialog, combo_position,
			     "combo_position");
	GLADE_HOOKUP_OBJECT (list_style_dialog, combo_entry_position,
			     "combo_entry_position");
	GLADE_HOOKUP_OBJECT (list_style_dialog, checkbutton_type,
			     "checkbutton_type");
	GLADE_HOOKUP_OBJECT (list_style_dialog, combo_type, "combo_type");
	GLADE_HOOKUP_OBJECT (list_style_dialog, combo_entry_type,
			     "combo_entry_type");
	GLADE_HOOKUP_OBJECT (list_style_dialog, checkbutton_image,
			     "checkbutton_image");
	GLADE_HOOKUP_OBJECT (list_style_dialog, entry_image, "entry_image");
	GLADE_HOOKUP_OBJECT (list_style_dialog, helpbutton, "helpbutton");
	GLADE_HOOKUP_OBJECT (list_style_dialog, cancelbutton, "cancelbutton");
	GLADE_HOOKUP_OBJECT (list_style_dialog, okbutton, "okbutton");

	gtk_window_add_accel_group (GTK_WINDOW (list_style_dialog),
				    accel_group);

	return list_style_dialog;
}

GtkWidget *
create_margin_property_dialog (CssedWindow * window,
			       gchar * previous_string,
			       gboolean add_previous_string,
			       gboolean connect_callbacks)
{
	GtkWidget *margin_property_dialog;
	GtkWidget *dialog_vbox;
	GtkWidget *vbox;
	GtkWidget *hbox_top;
	GtkWidget *checkbutton_top;
	GtkObject *spinbutton_top_adj;
	GtkWidget *spinbutton_top;
	GtkWidget *combo_top;
	GList *combo_top_items = NULL;
	GtkWidget *combo_entry_top;
	GtkWidget *hbox_right;
	GtkWidget *checkbutton_right;
	GtkObject *spinbutton_right_adj;
	GtkWidget *spinbutton_right;
	GtkWidget *combo_right;
	GList *combo_right_items = NULL;
	GtkWidget *combo_entry_right;
	GtkWidget *hbox_bottom;
	GtkWidget *checkbutton_bottom;
	GtkObject *spinbutton_bottom_adj;
	GtkWidget *spinbutton_bottom;
	GtkWidget *combo_bottom;
	GList *combo_bottom_items = NULL;
	GtkWidget *combo_entry_bottom;
	GtkWidget *hbox_left;
	GtkWidget *checkbutton_left;
	GtkObject *spinbutton_left_adj;
	GtkWidget *spinbutton_left;
	GtkWidget *combo_left;
	GList *combo_left_items = NULL;
	GtkWidget *combo_entry_left;
	GtkWidget *dialog_action_area;
	GtkWidget *helpbutton;
	GtkWidget *cancelbutton;
	GtkWidget *okbutton;
	GtkAccelGroup *accel_group;
	GtkWidget *main_window;
	CssedCssDialogsData *data = NULL;

	if (connect_callbacks)
	{
		data = g_malloc (sizeof (CssedCssDialogsData));
		data->main_window = window;
		data->previous_string = previous_string;
		data->add_property_string = add_previous_string;
	}

	accel_group = gtk_accel_group_new ();

	margin_property_dialog = gtk_dialog_new ();
	gtk_window_set_title (GTK_WINDOW (margin_property_dialog),
			      previous_string);

	gtk_window_set_modal (GTK_WINDOW (margin_property_dialog), TRUE);
	main_window = cssed_window_get_window_widget( window );

	gtk_window_set_transient_for (GTK_WINDOW (margin_property_dialog),
				      GTK_WINDOW (main_window));

	dialog_vbox = GTK_DIALOG (margin_property_dialog)->vbox;
	gtk_widget_show (dialog_vbox);

	vbox = gtk_vbox_new (FALSE, 0);
	gtk_widget_show (vbox);
	gtk_box_pack_start (GTK_BOX (dialog_vbox), vbox, TRUE, TRUE, 0);

	hbox_top = gtk_hbox_new (TRUE, 0);
	gtk_widget_show (hbox_top);
	gtk_box_pack_start (GTK_BOX (vbox), hbox_top, TRUE, TRUE, 0);

	checkbutton_top =
		gtk_check_button_new_with_mnemonic ("margin-top");
	gtk_widget_show (checkbutton_top);
	gtk_box_pack_start (GTK_BOX (hbox_top), checkbutton_top, TRUE, TRUE,
			    5);

	spinbutton_top_adj = gtk_adjustment_new (1, -35565, 35565, 1, 10, 10);
	spinbutton_top =
		gtk_spin_button_new (GTK_ADJUSTMENT (spinbutton_top_adj), 1,
				     0);
	gtk_widget_show (spinbutton_top);
	gtk_box_pack_start (GTK_BOX (hbox_top), spinbutton_top, FALSE, FALSE,
			    5);

	combo_top = gtk_combo_new ();
	g_object_set_data (G_OBJECT (GTK_COMBO (combo_top)->popwin),
			   "GladeParentKey", combo_top);
	gtk_widget_show (combo_top);
	gtk_box_pack_start (GTK_BOX (hbox_top), combo_top, TRUE, FALSE, 5);
	combo_top_items = g_list_append (combo_top_items, (gpointer) "%");
	combo_top_items = g_list_append (combo_top_items, (gpointer) "em");
	combo_top_items = g_list_append (combo_top_items, (gpointer) "ex");
	combo_top_items = g_list_append (combo_top_items, (gpointer) "px");
	combo_top_items = g_list_append (combo_top_items, (gpointer) "in");
	combo_top_items = g_list_append (combo_top_items, (gpointer) "cm");
	combo_top_items = g_list_append (combo_top_items, (gpointer) "mm");
	combo_top_items = g_list_append (combo_top_items, (gpointer) "pt");
	combo_top_items = g_list_append (combo_top_items, (gpointer) "pc");

	gtk_combo_set_popdown_strings (GTK_COMBO (combo_top),
				       combo_top_items);
	g_list_free (combo_top_items);

	combo_entry_top = GTK_COMBO (combo_top)->entry;
	gtk_widget_show (combo_entry_top);
	gtk_widget_set_size_request (combo_entry_top, 40, -1);
	gtk_entry_set_text (GTK_ENTRY (combo_entry_top), "px");

	hbox_right = gtk_hbox_new (TRUE, 0);
	gtk_widget_show (hbox_right);
	gtk_box_pack_start (GTK_BOX (vbox), hbox_right, TRUE, TRUE, 0);

	checkbutton_right =
		gtk_check_button_new_with_mnemonic ("margin-right");
	gtk_widget_show (checkbutton_right);
	gtk_box_pack_start (GTK_BOX (hbox_right), checkbutton_right, TRUE,
			    TRUE, 5);

	spinbutton_right_adj =
		gtk_adjustment_new (1, -35565, 35565, 1, 10, 10);
	spinbutton_right =
		gtk_spin_button_new (GTK_ADJUSTMENT (spinbutton_right_adj), 1,
				     0);
	gtk_widget_show (spinbutton_right);
	gtk_box_pack_start (GTK_BOX (hbox_right), spinbutton_right, FALSE,
			    FALSE, 5);

	combo_right = gtk_combo_new ();
	g_object_set_data (G_OBJECT (GTK_COMBO (combo_right)->popwin),
			   "GladeParentKey", combo_right);
	gtk_widget_show (combo_right);
	gtk_box_pack_start (GTK_BOX (hbox_right), combo_right, TRUE, FALSE,
			    5);
	combo_right_items = g_list_append (combo_right_items, (gpointer) "%");
	combo_right_items =
		g_list_append (combo_right_items, (gpointer) "em");
	combo_right_items =
		g_list_append (combo_right_items, (gpointer) "ex");
	combo_right_items =
		g_list_append (combo_right_items, (gpointer) "px");
	combo_right_items =
		g_list_append (combo_right_items, (gpointer) "in");
	combo_right_items =
		g_list_append (combo_right_items, (gpointer) "cm");
	combo_right_items =
		g_list_append (combo_right_items, (gpointer) "mm");
	combo_right_items =
		g_list_append (combo_right_items, (gpointer) "pt");
	combo_right_items =
		g_list_append (combo_right_items, (gpointer) "pc");

	gtk_combo_set_popdown_strings (GTK_COMBO (combo_right),
				       combo_right_items);
	g_list_free (combo_right_items);

	combo_entry_right = GTK_COMBO (combo_right)->entry;
	gtk_widget_show (combo_entry_right);
	gtk_widget_set_size_request (combo_entry_right, 40, -1);
	gtk_entry_set_text (GTK_ENTRY (combo_entry_right), "px");

	hbox_bottom = gtk_hbox_new (TRUE, 0);
	gtk_widget_show (hbox_bottom);
	gtk_box_pack_start (GTK_BOX (vbox), hbox_bottom, TRUE, TRUE, 0);

	checkbutton_bottom =
		gtk_check_button_new_with_mnemonic ("margin-bottom");
	gtk_widget_show (checkbutton_bottom);
	gtk_box_pack_start (GTK_BOX (hbox_bottom), checkbutton_bottom, TRUE,
			    TRUE, 5);

	spinbutton_bottom_adj =
		gtk_adjustment_new (1, -35565, 35565, 1, 10, 10);
	spinbutton_bottom =
		gtk_spin_button_new (GTK_ADJUSTMENT (spinbutton_bottom_adj),
				     1, 0);
	gtk_widget_show (spinbutton_bottom);
	gtk_box_pack_start (GTK_BOX (hbox_bottom), spinbutton_bottom, FALSE,
			    FALSE, 5);

	combo_bottom = gtk_combo_new ();
	g_object_set_data (G_OBJECT (GTK_COMBO (combo_bottom)->popwin),
			   "GladeParentKey", combo_bottom);
	gtk_widget_show (combo_bottom);
	gtk_box_pack_start (GTK_BOX (hbox_bottom), combo_bottom, TRUE, FALSE,
			    5);
	combo_bottom_items =
		g_list_append (combo_bottom_items, (gpointer) "%");
	combo_bottom_items =
		g_list_append (combo_bottom_items, (gpointer) "em");
	combo_bottom_items =
		g_list_append (combo_bottom_items, (gpointer) "ex");
	combo_bottom_items =
		g_list_append (combo_bottom_items, (gpointer) "px");
	combo_bottom_items =
		g_list_append (combo_bottom_items, (gpointer) "in");
	combo_bottom_items =
		g_list_append (combo_bottom_items, (gpointer) "cm");
	combo_bottom_items =
		g_list_append (combo_bottom_items, (gpointer) "mm");
	combo_bottom_items =
		g_list_append (combo_bottom_items, (gpointer) "pt");
	combo_bottom_items =
		g_list_append (combo_bottom_items, (gpointer) "pc");

	gtk_combo_set_popdown_strings (GTK_COMBO (combo_bottom),
				       combo_bottom_items);
	g_list_free (combo_bottom_items);

	combo_entry_bottom = GTK_COMBO (combo_bottom)->entry;
	gtk_widget_show (combo_entry_bottom);
	gtk_widget_set_size_request (combo_entry_bottom, 40, -1);
	gtk_entry_set_text (GTK_ENTRY (combo_entry_bottom), "px");

	hbox_left = gtk_hbox_new (TRUE, 0);
	gtk_widget_show (hbox_left);
	gtk_box_pack_start (GTK_BOX (vbox), hbox_left, TRUE, TRUE, 0);

	checkbutton_left =
		gtk_check_button_new_with_mnemonic ("margin-left");
	gtk_widget_show (checkbutton_left);
	gtk_box_pack_start (GTK_BOX (hbox_left), checkbutton_left, TRUE, TRUE,
			    5);

	spinbutton_left_adj =
		gtk_adjustment_new (1, -35565, 35565, 1, 10, 10);
	spinbutton_left =
		gtk_spin_button_new (GTK_ADJUSTMENT (spinbutton_left_adj), 1,
				     0);
	gtk_widget_show (spinbutton_left);
	gtk_box_pack_start (GTK_BOX (hbox_left), spinbutton_left, FALSE,
			    FALSE, 5);

	combo_left = gtk_combo_new ();
	g_object_set_data (G_OBJECT (GTK_COMBO (combo_left)->popwin),
			   "GladeParentKey", combo_left);
	gtk_widget_show (combo_left);
	gtk_box_pack_start (GTK_BOX (hbox_left), combo_left, TRUE, FALSE, 5);
	combo_left_items = g_list_append (combo_left_items, (gpointer) "%");
	combo_left_items = g_list_append (combo_left_items, (gpointer) "em");
	combo_left_items = g_list_append (combo_left_items, (gpointer) "ex");
	combo_left_items = g_list_append (combo_left_items, (gpointer) "px");
	combo_left_items = g_list_append (combo_left_items, (gpointer) "in");
	combo_left_items = g_list_append (combo_left_items, (gpointer) "cm");
	combo_left_items = g_list_append (combo_left_items, (gpointer) "mm");
	combo_left_items = g_list_append (combo_left_items, (gpointer) "pt");
	combo_left_items = g_list_append (combo_left_items, (gpointer) "pc");

	gtk_combo_set_popdown_strings (GTK_COMBO (combo_left),
				       combo_left_items);
	g_list_free (combo_left_items);

	combo_entry_left = GTK_COMBO (combo_left)->entry;
	gtk_widget_show (combo_entry_left);
	gtk_widget_set_size_request (combo_entry_left, 40, -1);
	gtk_entry_set_text (GTK_ENTRY (combo_entry_left), "px");

	dialog_action_area = GTK_DIALOG (margin_property_dialog)->action_area;
	gtk_widget_show (dialog_action_area);
	gtk_button_box_set_layout (GTK_BUTTON_BOX (dialog_action_area),
				   GTK_BUTTONBOX_END);

	helpbutton = gtk_button_new_from_stock ("gtk-help");
	gtk_widget_show (helpbutton);
	gtk_dialog_add_action_widget (GTK_DIALOG (margin_property_dialog),
				      helpbutton, GTK_RESPONSE_HELP);
	GTK_WIDGET_SET_FLAGS (helpbutton, GTK_CAN_DEFAULT);
	gtk_widget_add_accelerator (helpbutton, "clicked", accel_group,
				    GDK_y, GDK_CONTROL_MASK,
				    GTK_ACCEL_VISIBLE);
	gtk_widget_add_accelerator (helpbutton, "clicked", accel_group, GDK_h,
				    GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);

	cancelbutton = gtk_button_new_from_stock ("gtk-cancel");
	gtk_widget_show (cancelbutton);
	gtk_dialog_add_action_widget (GTK_DIALOG (margin_property_dialog),
				      cancelbutton, GTK_RESPONSE_CANCEL);
	GTK_WIDGET_SET_FLAGS (cancelbutton, GTK_CAN_DEFAULT);
	gtk_widget_add_accelerator (cancelbutton, "clicked", accel_group,
				    GDK_c, GDK_CONTROL_MASK,
				    GTK_ACCEL_VISIBLE);

	okbutton = gtk_button_new_from_stock ("gtk-ok");
	gtk_widget_show (okbutton);
	gtk_dialog_add_action_widget (GTK_DIALOG (margin_property_dialog),
				      okbutton, GTK_RESPONSE_OK);
	GTK_WIDGET_SET_FLAGS (okbutton, GTK_CAN_DEFAULT);
	gtk_widget_add_accelerator (okbutton, "clicked", accel_group,
				    GDK_o, GDK_CONTROL_MASK,
				    GTK_ACCEL_VISIBLE);

	g_signal_connect ((gpointer) combo_entry_top,
			  "changed", G_CALLBACK (on_css_dialog_unit_changed),
			  spinbutton_top);
	g_signal_connect ((gpointer) combo_entry_right,
			  "changed", G_CALLBACK (on_css_dialog_unit_changed),
			  spinbutton_right);
	g_signal_connect ((gpointer) combo_entry_bottom,
			  "changed", G_CALLBACK (on_css_dialog_unit_changed),
			  spinbutton_bottom);
	g_signal_connect ((gpointer) combo_entry_left,
			  "changed", G_CALLBACK (on_css_dialog_unit_changed),
			  spinbutton_left);
			  
	/* to update the checkboxes when a related widget is changed */				    
	g_signal_connect ((gpointer) spinbutton_top,
			  "value-changed", G_CALLBACK (on_widget_change_set_toggle_active),
			  checkbutton_top );
	g_signal_connect ((gpointer) combo_entry_top,
			  "changed", G_CALLBACK (on_widget_change_set_toggle_active),
			  checkbutton_top);

	g_signal_connect ((gpointer) spinbutton_right,
			  "value-changed", G_CALLBACK (on_widget_change_set_toggle_active),
			  checkbutton_right );
	g_signal_connect ((gpointer) combo_entry_right,
			  "changed", G_CALLBACK (on_widget_change_set_toggle_active),
			  checkbutton_right);

	g_signal_connect ((gpointer) spinbutton_bottom,
			  "value-changed", G_CALLBACK (on_widget_change_set_toggle_active),
			  checkbutton_bottom );
	g_signal_connect ((gpointer) combo_entry_bottom,
			  "changed", G_CALLBACK (on_widget_change_set_toggle_active),
			  checkbutton_bottom);			  

	g_signal_connect ((gpointer) spinbutton_left,
			  "value-changed", G_CALLBACK (on_widget_change_set_toggle_active),
			  checkbutton_left );
	g_signal_connect ((gpointer) combo_entry_left,
			  "changed", G_CALLBACK (on_widget_change_set_toggle_active),
			  checkbutton_left);			  
			  
	if (connect_callbacks)
	{
		g_signal_connect ((gpointer) margin_property_dialog, "close",
				  G_CALLBACK (on_css_dialog_close), data);

		g_signal_connect ((gpointer) helpbutton, "clicked",
				  G_CALLBACK
				  (on_margin_property_dialog_helpbutton_clicked),
				  NULL);
		g_signal_connect ((gpointer) cancelbutton, "clicked",
				  G_CALLBACK
				  (on_css_dialog_cancel_button_clicked),
				  data);
		g_signal_connect ((gpointer) okbutton, "clicked",
				  G_CALLBACK
				  (on_margin_property_dialog_okbutton_clicked),
				  data);
	}
	/*
	 * Store pointers to all widgets, for use by lookup_widget(). 
	 */
	GLADE_HOOKUP_OBJECT_NO_REF (margin_property_dialog,
				    margin_property_dialog,
				    "margin_property_dialog");
	GLADE_HOOKUP_OBJECT (margin_property_dialog, checkbutton_top,
			     "checkbutton_top");
	GLADE_HOOKUP_OBJECT (margin_property_dialog, spinbutton_top,
			     "spinbutton_top");
	GLADE_HOOKUP_OBJECT (margin_property_dialog, combo_top, "combo_top");
	GLADE_HOOKUP_OBJECT (margin_property_dialog, combo_entry_top,
			     "combo_entry_top");
	GLADE_HOOKUP_OBJECT (margin_property_dialog, checkbutton_right,
			     "checkbutton_right");
	GLADE_HOOKUP_OBJECT (margin_property_dialog, spinbutton_right,
			     "spinbutton_right");
	GLADE_HOOKUP_OBJECT (margin_property_dialog, combo_right,
			     "combo_right");
	GLADE_HOOKUP_OBJECT (margin_property_dialog, combo_entry_right,
			     "combo_entry_right");
	GLADE_HOOKUP_OBJECT (margin_property_dialog, checkbutton_bottom,
			     "checkbutton_bottom");
	GLADE_HOOKUP_OBJECT (margin_property_dialog, spinbutton_bottom,
			     "spinbutton_bottom");
	GLADE_HOOKUP_OBJECT (margin_property_dialog, combo_bottom,
			     "combo_bottom");
	GLADE_HOOKUP_OBJECT (margin_property_dialog, combo_entry_bottom,
			     "combo_entry_bottom");
	GLADE_HOOKUP_OBJECT (margin_property_dialog, checkbutton_left,
			     "checkbutton_left");
	GLADE_HOOKUP_OBJECT (margin_property_dialog, spinbutton_left,
			     "spinbutton_left");
	GLADE_HOOKUP_OBJECT (margin_property_dialog, combo_left,
			     "combo_left");
	GLADE_HOOKUP_OBJECT (margin_property_dialog, combo_entry_left,
			     "combo_entry_left");
	GLADE_HOOKUP_OBJECT (margin_property_dialog, helpbutton,
			     "helpbutton");
	GLADE_HOOKUP_OBJECT (margin_property_dialog, cancelbutton,
			     "cancelbutton");
	GLADE_HOOKUP_OBJECT (margin_property_dialog, okbutton, "okbutton");

	gtk_window_add_accel_group (GTK_WINDOW (margin_property_dialog),
				    accel_group);

	return margin_property_dialog;
}

GtkWidget *
create_margin_width_value_dialog (CssedWindow * window,
				  gchar * previous_string,
				  gboolean add_previous_string,
				  gboolean connect_callbacks)
{
	GtkWidget *margin_width_value_dialog;
	GtkWidget *dialog_vbox;
	GtkWidget *hbox;
	GtkWidget *label_margin_width;
	GtkObject *spinbutton_margin_width_adj;
	GtkWidget *spinbutton_margin_width;
	GtkWidget *combo_margin_width;
	GList *combo_margin_width_items = NULL;
	GtkWidget *combo_entry_margin_width_units;
	GtkWidget *dialog_action_area;
	GtkWidget *helpbutton;
	GtkWidget *cancelbutton;
	GtkWidget *okbutton;
	GtkAccelGroup *accel_group;
	GtkWidget *main_window;
	CssedCssDialogsData *data = NULL;

	if (connect_callbacks)
	{
		data = g_malloc (sizeof (CssedCssDialogsData));
		data->main_window = window;
		data->previous_string = previous_string;
		data->add_property_string = add_previous_string;
	}

	accel_group = gtk_accel_group_new ();

	margin_width_value_dialog = gtk_dialog_new ();
	gtk_window_set_title (GTK_WINDOW (margin_width_value_dialog),
			      previous_string);
	// ~ gtk_window_set_position (GTK_WINDOW (margin_width_value_dialog),
	// ~ GTK_WIN_POS_CENTER);
	gtk_window_set_modal (GTK_WINDOW (margin_width_value_dialog), TRUE);

	main_window = cssed_window_get_window_widget( window );

	gtk_window_set_transient_for (GTK_WINDOW (margin_width_value_dialog),
				      GTK_WINDOW (main_window));

	dialog_vbox = GTK_DIALOG (margin_width_value_dialog)->vbox;
	gtk_widget_show (dialog_vbox);

	hbox = gtk_hbox_new (TRUE, 0);
	gtk_widget_show (hbox);
	gtk_box_pack_start (GTK_BOX (dialog_vbox), hbox, TRUE, TRUE, 0);

	label_margin_width = gtk_label_new (previous_string);
	gtk_widget_show (label_margin_width);
	gtk_box_pack_start (GTK_BOX (hbox), label_margin_width, TRUE, TRUE,
			    0);
	gtk_label_set_justify (GTK_LABEL (label_margin_width),
			       GTK_JUSTIFY_LEFT);

	spinbutton_margin_width_adj =
		gtk_adjustment_new (1, -35565, 35565, 1, 10, 10);
	spinbutton_margin_width =
		gtk_spin_button_new (GTK_ADJUSTMENT
				     (spinbutton_margin_width_adj), 1, 0);
	gtk_widget_show (spinbutton_margin_width);
	gtk_box_pack_start (GTK_BOX (hbox), spinbutton_margin_width, FALSE,
			    FALSE, 0);

	combo_margin_width = gtk_combo_new ();
	g_object_set_data (G_OBJECT (GTK_COMBO (combo_margin_width)->popwin),
			   "GladeParentKey", combo_margin_width);
	gtk_widget_show (combo_margin_width);
	gtk_box_pack_start (GTK_BOX (hbox), combo_margin_width, FALSE, FALSE,
			    0);
	combo_margin_width_items =
		g_list_append (combo_margin_width_items, (gpointer) "%");
	combo_margin_width_items =
		g_list_append (combo_margin_width_items, (gpointer) "em");
	combo_margin_width_items =
		g_list_append (combo_margin_width_items, (gpointer) "ex");
	combo_margin_width_items =
		g_list_append (combo_margin_width_items, (gpointer) "px");
	combo_margin_width_items =
		g_list_append (combo_margin_width_items, (gpointer) "in");
	combo_margin_width_items =
		g_list_append (combo_margin_width_items, (gpointer) "cm");
	combo_margin_width_items =
		g_list_append (combo_margin_width_items, (gpointer) "mm");
	combo_margin_width_items =
		g_list_append (combo_margin_width_items, (gpointer) "pt");
	combo_margin_width_items =
		g_list_append (combo_margin_width_items, (gpointer) "pc");


	gtk_combo_set_popdown_strings (GTK_COMBO (combo_margin_width),
				       combo_margin_width_items);
	g_list_free (combo_margin_width_items);

	combo_entry_margin_width_units =
		GTK_COMBO (combo_margin_width)->entry;
	gtk_widget_show (combo_entry_margin_width_units);
	gtk_widget_set_size_request (combo_entry_margin_width_units, 40, -1);
	gtk_editable_set_editable (GTK_EDITABLE
				   (combo_entry_margin_width_units), FALSE);
	gtk_entry_set_text (GTK_ENTRY (combo_entry_margin_width_units), "px");

	dialog_action_area =
		GTK_DIALOG (margin_width_value_dialog)->action_area;
	gtk_widget_show (dialog_action_area);
	gtk_button_box_set_layout (GTK_BUTTON_BOX (dialog_action_area),
				   GTK_BUTTONBOX_END);

	helpbutton = gtk_button_new_from_stock ("gtk-help");
	gtk_widget_show (helpbutton);
	gtk_dialog_add_action_widget (GTK_DIALOG (margin_width_value_dialog),
				      helpbutton, GTK_RESPONSE_HELP);
	GTK_WIDGET_SET_FLAGS (helpbutton, GTK_CAN_DEFAULT);
	gtk_widget_add_accelerator (helpbutton, "clicked", accel_group,
				    GDK_y, GDK_CONTROL_MASK,
				    GTK_ACCEL_VISIBLE);
	gtk_widget_add_accelerator (helpbutton, "clicked", accel_group, GDK_h,
				    GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);

	cancelbutton = gtk_button_new_from_stock ("gtk-cancel");
	gtk_widget_show (cancelbutton);
	gtk_dialog_add_action_widget (GTK_DIALOG (margin_width_value_dialog),
				      cancelbutton, GTK_RESPONSE_CANCEL);
	GTK_WIDGET_SET_FLAGS (cancelbutton, GTK_CAN_DEFAULT);

	okbutton = gtk_button_new_from_stock ("gtk-ok");
	gtk_widget_show (okbutton);
	gtk_dialog_add_action_widget (GTK_DIALOG (margin_width_value_dialog),
				      okbutton, GTK_RESPONSE_OK);
	GTK_WIDGET_SET_FLAGS (okbutton, GTK_CAN_DEFAULT);
	gtk_widget_add_accelerator (okbutton, "clicked", accel_group,
				    GDK_o, GDK_CONTROL_MASK,
				    GTK_ACCEL_VISIBLE);

	g_signal_connect ((gpointer) combo_entry_margin_width_units,
			  "changed", G_CALLBACK (on_css_dialog_unit_changed),
			  spinbutton_margin_width);

	if (connect_callbacks)
	{
		g_signal_connect ((gpointer) margin_width_value_dialog,
				  "close", G_CALLBACK (on_css_dialog_close),
				  data);

		g_signal_connect ((gpointer) helpbutton, "clicked",
				  G_CALLBACK
				  (on_margin_width_value_dialog_helpbutton_clicked),
				  NULL);
		g_signal_connect ((gpointer) cancelbutton, "clicked",
				  G_CALLBACK
				  (on_css_dialog_cancel_button_clicked),
				  data);
		g_signal_connect ((gpointer) okbutton, "clicked",
				  G_CALLBACK
				  (on_margin_width_dialog_okbutton_clicked),
				  data);
	}

	GLADE_HOOKUP_OBJECT_NO_REF (margin_width_value_dialog,
				    margin_width_value_dialog,
				    "margin_width_value_dialog");
	GLADE_HOOKUP_OBJECT_NO_REF (margin_width_value_dialog, dialog_vbox,
				    "dialog_vbox");
	GLADE_HOOKUP_OBJECT (margin_width_value_dialog, hbox, "hbox");
	GLADE_HOOKUP_OBJECT (margin_width_value_dialog, label_margin_width,
			     "label_margin_width");
	GLADE_HOOKUP_OBJECT (margin_width_value_dialog,
			     spinbutton_margin_width,
			     "spinbutton_margin_width");
	GLADE_HOOKUP_OBJECT (margin_width_value_dialog, combo_margin_width,
			     "combo_margin_width");
	GLADE_HOOKUP_OBJECT (margin_width_value_dialog,
			     combo_entry_margin_width_units,
			     "combo_entry_margin_width_units");
	GLADE_HOOKUP_OBJECT_NO_REF (margin_width_value_dialog,
				    dialog_action_area, "dialog_action_area");
	GLADE_HOOKUP_OBJECT (margin_width_value_dialog, helpbutton,
			     "helpbutton");
	GLADE_HOOKUP_OBJECT (margin_width_value_dialog, cancelbutton,
			     "cancelbutton");
	GLADE_HOOKUP_OBJECT (margin_width_value_dialog, okbutton, "okbutton");

	gtk_window_add_accel_group (GTK_WINDOW (margin_width_value_dialog),
				    accel_group);

	return margin_width_value_dialog;
}

GtkWidget *
create_number_value_dialog (CssedWindow * window,
			    gchar * previous_string,
			    gboolean add_previous_string,
			    gboolean connect_callbacks)
{
	GtkWidget *number_value_dialog;
	GtkWidget *dialog_vbox;
	GtkWidget *hbox;
	GtkWidget *label_number;
	GtkObject *spinbutton_number_adj;
	GtkWidget *spinbutton_number;
	GtkWidget *dialog_action_area;
	GtkWidget *helpbutton;
	GtkWidget *cancelbutton;
	GtkWidget *okbutton;
	GtkAccelGroup *accel_group;
	GtkWidget *main_window;
	CssedCssDialogsData *data = NULL;

	if (connect_callbacks)
	{
		data = g_malloc (sizeof (CssedCssDialogsData));
		data->main_window = window;
		data->previous_string = previous_string;
		data->add_property_string = add_previous_string;
	}

	accel_group = gtk_accel_group_new ();

	number_value_dialog = gtk_dialog_new ();
	gtk_window_set_title (GTK_WINDOW (number_value_dialog),
			      previous_string);
	// ~ gtk_window_set_position (GTK_WINDOW (number_value_dialog),
	// ~ GTK_WIN_POS_CENTER);
	gtk_window_set_modal (GTK_WINDOW (number_value_dialog), TRUE);

	main_window = cssed_window_get_window_widget( window );

	gtk_window_set_transient_for (GTK_WINDOW (number_value_dialog),
				      GTK_WINDOW (main_window));

	dialog_vbox = GTK_DIALOG (number_value_dialog)->vbox;
	gtk_widget_show (dialog_vbox);

	hbox = gtk_hbox_new (TRUE, 0);
	gtk_widget_show (hbox);
	gtk_box_pack_start (GTK_BOX (dialog_vbox), hbox, TRUE, TRUE, 0);

	label_number = gtk_label_new ("number");
	gtk_widget_show (label_number);
	gtk_box_pack_start (GTK_BOX (hbox), label_number, TRUE, TRUE, 0);
	gtk_label_set_justify (GTK_LABEL (label_number), GTK_JUSTIFY_LEFT);

	spinbutton_number_adj =
		gtk_adjustment_new (1, -35565, 35565, 1, 10, 10);
	spinbutton_number =
		gtk_spin_button_new (GTK_ADJUSTMENT (spinbutton_number_adj),
				     1, 2);
	gtk_widget_show (spinbutton_number);
	gtk_box_pack_end (GTK_BOX (hbox), spinbutton_number, FALSE, FALSE, 0);

	dialog_action_area = GTK_DIALOG (number_value_dialog)->action_area;
	gtk_widget_show (dialog_action_area);
	gtk_button_box_set_layout (GTK_BUTTON_BOX (dialog_action_area),
				   GTK_BUTTONBOX_END);

	helpbutton = gtk_button_new_from_stock ("gtk-help");
	gtk_widget_show (helpbutton);
	gtk_dialog_add_action_widget (GTK_DIALOG (number_value_dialog),
				      helpbutton, GTK_RESPONSE_HELP);
	GTK_WIDGET_SET_FLAGS (helpbutton, GTK_CAN_DEFAULT);
	gtk_widget_add_accelerator (helpbutton, "clicked", accel_group,
				    GDK_y, GDK_CONTROL_MASK,
				    GTK_ACCEL_VISIBLE);
	gtk_widget_add_accelerator (helpbutton, "clicked", accel_group, GDK_h,
				    GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);

	cancelbutton = gtk_button_new_from_stock ("gtk-cancel");
	gtk_widget_show (cancelbutton);
	gtk_dialog_add_action_widget (GTK_DIALOG (number_value_dialog),
				      cancelbutton, GTK_RESPONSE_CANCEL);
	GTK_WIDGET_SET_FLAGS (cancelbutton, GTK_CAN_DEFAULT);
	gtk_widget_add_accelerator (cancelbutton, "clicked", accel_group,
				    GDK_c, GDK_CONTROL_MASK,
				    GTK_ACCEL_VISIBLE);

	okbutton = gtk_button_new_from_stock ("gtk-ok");
	gtk_widget_show (okbutton);
	gtk_dialog_add_action_widget (GTK_DIALOG (number_value_dialog),
				      okbutton, GTK_RESPONSE_OK);
	GTK_WIDGET_SET_FLAGS (okbutton, GTK_CAN_DEFAULT);
	gtk_widget_add_accelerator (okbutton, "clicked", accel_group,
				    GDK_o, GDK_CONTROL_MASK,
				    GTK_ACCEL_VISIBLE);
	if (connect_callbacks)
	{
		g_signal_connect ((gpointer) number_value_dialog, "close",
				  G_CALLBACK (on_css_dialog_close), data);

		g_signal_connect ((gpointer) helpbutton, "clicked",
				  G_CALLBACK
				  (on_number_value_dialog_helpbutton_clicked),
				  NULL);
		g_signal_connect ((gpointer) cancelbutton, "clicked",
				  G_CALLBACK
				  (on_css_dialog_cancel_button_clicked),
				  data);
		g_signal_connect ((gpointer) okbutton, "clicked",
				  G_CALLBACK
				  (on_number_value_dialog_okbutton_clicked),
				  data);
	}

	GLADE_HOOKUP_OBJECT_NO_REF (number_value_dialog, number_value_dialog,
				    "number_value_dialog");
	GLADE_HOOKUP_OBJECT_NO_REF (number_value_dialog, dialog_vbox,
				    "dialog_vbox");
	GLADE_HOOKUP_OBJECT (number_value_dialog, hbox, "hbox");
	GLADE_HOOKUP_OBJECT (number_value_dialog, label_number,
			     "label_number");
	GLADE_HOOKUP_OBJECT (number_value_dialog, spinbutton_number,
			     "spinbutton_number");
	GLADE_HOOKUP_OBJECT_NO_REF (number_value_dialog, dialog_action_area,
				    "dialog_action_area");
	GLADE_HOOKUP_OBJECT (number_value_dialog, helpbutton, "helpbutton");
	GLADE_HOOKUP_OBJECT (number_value_dialog, cancelbutton,
			     "cancelbutton");
	GLADE_HOOKUP_OBJECT (number_value_dialog, okbutton, "okbutton");

	gtk_window_add_accel_group (GTK_WINDOW (number_value_dialog),
				    accel_group);

	return number_value_dialog;
}

GtkWidget *
create_padding_property_dialog (CssedWindow * window,
				gchar * previous_string,
				gboolean add_previous_string,
				gboolean connect_callbacks)
{
	GtkWidget *padding_property_dialog;
	GtkWidget *dialog_vbox;
	GtkWidget *vbox;
	GtkWidget *hbox_top;
	GtkWidget *checkbutton_top;
	GtkObject *spinbutton_top_adj;
	GtkWidget *spinbutton_top;
	GtkWidget *combo_top;
	GList *combo_top_items = NULL;
	GtkWidget *combo_entry_top;
	GtkWidget *hbox_left;
	GtkWidget *checkbutton_left;
	GtkObject *spinbutton_left_adj;
	GtkWidget *spinbutton_left;
	GtkWidget *combo_left;
	GList *combo_left_items = NULL;
	GtkWidget *combo_entry_left;
	GtkWidget *hbox_bottom;
	GtkWidget *checkbutton_bottom;
	GtkObject *spinbutton_bottom_adj;
	GtkWidget *spinbutton_bottom;
	GtkWidget *combo_bottom;
	GList *combo_bottom_items = NULL;
	GtkWidget *combo_entry_bottom;
	GtkWidget *hbox_right;
	GtkWidget *checkbutton_right;
	GtkObject *spinbutton_right_adj;
	GtkWidget *spinbutton_right;
	GtkWidget *combo_right;
	GList *combo_right_items = NULL;
	GtkWidget *combo_entry_right;
	GtkWidget *dialog_action_area;
	GtkWidget *helpbutton;
	GtkWidget *cancelbutton;
	GtkWidget *okbutton;
	GtkAccelGroup *accel_group;
	GtkWidget *main_window;
	CssedCssDialogsData *data = NULL;

	if (connect_callbacks)
	{
		data = g_malloc (sizeof (CssedCssDialogsData));
		data->main_window = window;
		data->previous_string = previous_string;
		data->add_property_string = add_previous_string;
	}

	accel_group = gtk_accel_group_new ();

	padding_property_dialog = gtk_dialog_new ();
	gtk_window_set_title (GTK_WINDOW (padding_property_dialog),
			      previous_string);

	gtk_window_set_modal (GTK_WINDOW (padding_property_dialog), TRUE);

	main_window = cssed_window_get_window_widget( window );

	gtk_window_set_transient_for (GTK_WINDOW (padding_property_dialog),
				      GTK_WINDOW (main_window));

	dialog_vbox = GTK_DIALOG (padding_property_dialog)->vbox;
	gtk_widget_show (dialog_vbox);

	vbox = gtk_vbox_new (FALSE, 0);
	gtk_widget_show (vbox);
	gtk_box_pack_start (GTK_BOX (dialog_vbox), vbox, TRUE, TRUE, 0);
/*top*/
	hbox_top = gtk_hbox_new (TRUE, 0);
	gtk_widget_show (hbox_top);
	gtk_box_pack_start (GTK_BOX (vbox), hbox_top, FALSE, FALSE, 5);

	checkbutton_top =
		gtk_check_button_new_with_mnemonic ("padding-top");
	gtk_widget_show (checkbutton_top);
	gtk_box_pack_start (GTK_BOX (hbox_top), checkbutton_top, TRUE, TRUE,
			    5);

	spinbutton_top_adj = gtk_adjustment_new (1, -35565, 35565, 1, 10, 10);
	spinbutton_top =
		gtk_spin_button_new (GTK_ADJUSTMENT (spinbutton_top_adj), 1,
				     0);
	gtk_widget_show (spinbutton_top);
	gtk_box_pack_start (GTK_BOX (hbox_top), spinbutton_top, FALSE, FALSE,
			    0);

	combo_top = gtk_combo_new ();
	g_object_set_data (G_OBJECT (GTK_COMBO (combo_top)->popwin),
			   "GladeParentKey", combo_top);
	gtk_widget_show (combo_top);
	gtk_box_pack_start (GTK_BOX (hbox_top), combo_top, FALSE, FALSE, 0);
	combo_top_items = g_list_append (combo_top_items, (gpointer) "em");
	combo_top_items = g_list_append (combo_top_items, (gpointer) "ex");
	combo_top_items = g_list_append (combo_top_items, (gpointer) "px");
	combo_top_items = g_list_append (combo_top_items, (gpointer) "in");
	combo_top_items = g_list_append (combo_top_items, (gpointer) "cm");
	combo_top_items = g_list_append (combo_top_items, (gpointer) "mm");
	combo_top_items = g_list_append (combo_top_items, (gpointer) "pt");
	combo_top_items = g_list_append (combo_top_items, (gpointer) "pc");
	gtk_combo_set_popdown_strings (GTK_COMBO (combo_top),
				       combo_top_items);
	g_list_free (combo_top_items);

	combo_entry_top = GTK_COMBO (combo_top)->entry;
	gtk_widget_show (combo_entry_top);
	gtk_widget_set_size_request (combo_entry_top, 50, -1);
	gtk_entry_set_text (GTK_ENTRY (combo_entry_top), "px");
/*right*/
	hbox_right = gtk_hbox_new (TRUE, 0);
	gtk_widget_show (hbox_right);
	gtk_box_pack_start (GTK_BOX (vbox), hbox_right, FALSE, FALSE, 5);

	checkbutton_right =
		gtk_check_button_new_with_mnemonic ("padding-right");
	gtk_widget_show (checkbutton_right);
	gtk_box_pack_start (GTK_BOX (hbox_right), checkbutton_right, TRUE,
			    TRUE, 5);

	spinbutton_right_adj =
		gtk_adjustment_new (1, -35565, 35565, 1, 10, 10);
	spinbutton_right =
		gtk_spin_button_new (GTK_ADJUSTMENT (spinbutton_right_adj), 1,
				     0);
	gtk_widget_show (spinbutton_right);
	gtk_box_pack_start (GTK_BOX (hbox_right), spinbutton_right, FALSE,
			    FALSE, 0);

	combo_right = gtk_combo_new ();
	g_object_set_data (G_OBJECT (GTK_COMBO (combo_right)->popwin),
			   "GladeParentKey", combo_right);
	gtk_widget_show (combo_right);
	gtk_box_pack_start (GTK_BOX (hbox_right), combo_right, FALSE, FALSE,
			    0);
	combo_right_items =
		g_list_append (combo_right_items, (gpointer) "em");
	combo_right_items =
		g_list_append (combo_right_items, (gpointer) "ex");
	combo_right_items =
		g_list_append (combo_right_items, (gpointer) "px");
	combo_right_items =
		g_list_append (combo_right_items, (gpointer) "in");
	combo_right_items =
		g_list_append (combo_right_items, (gpointer) "cm");
	combo_right_items =
		g_list_append (combo_right_items, (gpointer) "mm");
	combo_right_items =
		g_list_append (combo_right_items, (gpointer) "pt");
	combo_right_items =
		g_list_append (combo_right_items, (gpointer) "pc");
	gtk_combo_set_popdown_strings (GTK_COMBO (combo_right),
				       combo_right_items);
	g_list_free (combo_right_items);

	combo_entry_right = GTK_COMBO (combo_right)->entry;
	gtk_widget_show (combo_entry_right);
	gtk_widget_set_size_request (combo_entry_right, 50, -1);
	gtk_entry_set_text (GTK_ENTRY (combo_entry_right), "px");

/*bottom*/
	hbox_bottom = gtk_hbox_new (TRUE, 0);
	gtk_widget_show (hbox_bottom);
	gtk_box_pack_start (GTK_BOX (vbox), hbox_bottom, FALSE, FALSE, 5);

	checkbutton_bottom =
		gtk_check_button_new_with_mnemonic ("padding-bottom");
	gtk_widget_show (checkbutton_bottom);
	gtk_box_pack_start (GTK_BOX (hbox_bottom), checkbutton_bottom, TRUE,
			    TRUE, 5);

	spinbutton_bottom_adj =
		gtk_adjustment_new (1, -35565, 35565, 1, 10, 10);
	spinbutton_bottom =
		gtk_spin_button_new (GTK_ADJUSTMENT (spinbutton_bottom_adj),
				     1, 0);
	gtk_widget_show (spinbutton_bottom);
	gtk_box_pack_start (GTK_BOX (hbox_bottom), spinbutton_bottom, FALSE,
			    FALSE, 0);

	combo_bottom = gtk_combo_new ();
	g_object_set_data (G_OBJECT (GTK_COMBO (combo_bottom)->popwin),
			   "GladeParentKey", combo_bottom);
	gtk_widget_show (combo_bottom);
	gtk_box_pack_start (GTK_BOX (hbox_bottom), combo_bottom, FALSE, FALSE,
			    0);
	combo_bottom_items =
		g_list_append (combo_bottom_items, (gpointer) "em");
	combo_bottom_items =
		g_list_append (combo_bottom_items, (gpointer) "ex");
	combo_bottom_items =
		g_list_append (combo_bottom_items, (gpointer) "px");
	combo_bottom_items =
		g_list_append (combo_bottom_items, (gpointer) "in");
	combo_bottom_items =
		g_list_append (combo_bottom_items, (gpointer) "cm");
	combo_bottom_items =
		g_list_append (combo_bottom_items, (gpointer) "mm");
	combo_bottom_items =
		g_list_append (combo_bottom_items, (gpointer) "pt");
	combo_bottom_items =
		g_list_append (combo_bottom_items, (gpointer) "pc");
	gtk_combo_set_popdown_strings (GTK_COMBO (combo_bottom),
				       combo_bottom_items);
	g_list_free (combo_bottom_items);

	combo_entry_bottom = GTK_COMBO (combo_bottom)->entry;
	gtk_widget_show (combo_entry_bottom);
	gtk_widget_set_size_request (combo_entry_bottom, 50, -1);
	gtk_entry_set_text (GTK_ENTRY (combo_entry_bottom), "px");
/*left*/
	hbox_left = gtk_hbox_new (TRUE, 0);
	gtk_widget_show (hbox_left);
	gtk_box_pack_start (GTK_BOX (vbox), hbox_left, FALSE, FALSE, 5);

	checkbutton_left =
		gtk_check_button_new_with_mnemonic ("padding-left");
	gtk_widget_show (checkbutton_left);
	gtk_box_pack_start (GTK_BOX (hbox_left), checkbutton_left, TRUE, TRUE,
			    5);

	spinbutton_left_adj =
		gtk_adjustment_new (1, -35565, 35565, 1, 10, 10);
	spinbutton_left =
		gtk_spin_button_new (GTK_ADJUSTMENT (spinbutton_left_adj), 1,
				     0);
	gtk_widget_show (spinbutton_left);
	gtk_box_pack_start (GTK_BOX (hbox_left), spinbutton_left, FALSE,
			    FALSE, 0);

	combo_left = gtk_combo_new ();
	g_object_set_data (G_OBJECT (GTK_COMBO (combo_left)->popwin),
			   "GladeParentKey", combo_left);
	gtk_widget_show (combo_left);
	gtk_box_pack_start (GTK_BOX (hbox_left), combo_left, FALSE, FALSE, 0);
	combo_left_items = g_list_append (combo_left_items, (gpointer) "em");
	combo_left_items = g_list_append (combo_left_items, (gpointer) "ex");
	combo_left_items = g_list_append (combo_left_items, (gpointer) "px");
	combo_left_items = g_list_append (combo_left_items, (gpointer) "in");
	combo_left_items = g_list_append (combo_left_items, (gpointer) "cm");
	combo_left_items = g_list_append (combo_left_items, (gpointer) "mm");
	combo_left_items = g_list_append (combo_left_items, (gpointer) "pt");
	combo_left_items = g_list_append (combo_left_items, (gpointer) "pc");
	gtk_combo_set_popdown_strings (GTK_COMBO (combo_left),
				       combo_left_items);
	g_list_free (combo_left_items);

	combo_entry_left = GTK_COMBO (combo_left)->entry;
	gtk_widget_show (combo_entry_left);
	gtk_widget_set_size_request (combo_entry_left, 50, -1);
	gtk_entry_set_text (GTK_ENTRY (combo_entry_left), "px");


	 /**/ dialog_action_area =
		GTK_DIALOG (padding_property_dialog)->action_area;
	gtk_widget_show (dialog_action_area);
	gtk_button_box_set_layout (GTK_BUTTON_BOX (dialog_action_area),
				   GTK_BUTTONBOX_END);

	helpbutton = gtk_button_new_from_stock ("gtk-help");
	gtk_widget_show (helpbutton);
	gtk_dialog_add_action_widget (GTK_DIALOG (padding_property_dialog),
				      helpbutton, GTK_RESPONSE_HELP);
	GTK_WIDGET_SET_FLAGS (helpbutton, GTK_CAN_DEFAULT);
	gtk_widget_add_accelerator (helpbutton, "clicked", accel_group,
				    GDK_y, GDK_CONTROL_MASK,
				    GTK_ACCEL_VISIBLE);
	gtk_widget_add_accelerator (helpbutton, "clicked", accel_group, GDK_h,
				    GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);

	cancelbutton = gtk_button_new_from_stock ("gtk-cancel");
	gtk_widget_show (cancelbutton);
	gtk_dialog_add_action_widget (GTK_DIALOG (padding_property_dialog),
				      cancelbutton, GTK_RESPONSE_CANCEL);
	GTK_WIDGET_SET_FLAGS (cancelbutton, GTK_CAN_DEFAULT);
	gtk_widget_add_accelerator (cancelbutton, "clicked", accel_group,
				    GDK_c, GDK_CONTROL_MASK,
				    GTK_ACCEL_VISIBLE);

	okbutton = gtk_button_new_from_stock ("gtk-ok");
	gtk_widget_show (okbutton);
	gtk_dialog_add_action_widget (GTK_DIALOG (padding_property_dialog),
				      okbutton, GTK_RESPONSE_OK);
	GTK_WIDGET_SET_FLAGS (okbutton, GTK_CAN_DEFAULT);
	gtk_widget_add_accelerator (okbutton, "clicked", accel_group,
				    GDK_o, GDK_CONTROL_MASK,
				    GTK_ACCEL_VISIBLE);

	g_signal_connect ((gpointer) combo_entry_top,
			  "changed", G_CALLBACK (on_css_dialog_unit_changed),
			  spinbutton_top);
	g_signal_connect ((gpointer) combo_entry_right,
			  "changed", G_CALLBACK (on_css_dialog_unit_changed),
			  spinbutton_right);
	g_signal_connect ((gpointer) combo_entry_bottom,
			  "changed", G_CALLBACK (on_css_dialog_unit_changed),
			  spinbutton_bottom);
	g_signal_connect ((gpointer) combo_entry_left,
			  "changed", G_CALLBACK (on_css_dialog_unit_changed),
			  spinbutton_left);

	/* to update the checkboxes when a related widget is changed */				    
	g_signal_connect ((gpointer) spinbutton_top,
			  "value-changed", G_CALLBACK (on_widget_change_set_toggle_active),
			  checkbutton_top );
	g_signal_connect ((gpointer) combo_entry_top,
			  "changed", G_CALLBACK (on_widget_change_set_toggle_active),
			  checkbutton_top);

	g_signal_connect ((gpointer) spinbutton_right,
			  "value-changed", G_CALLBACK (on_widget_change_set_toggle_active),
			  checkbutton_right );
	g_signal_connect ((gpointer) combo_entry_right,
			  "changed", G_CALLBACK (on_widget_change_set_toggle_active),
			  checkbutton_right);

	g_signal_connect ((gpointer) spinbutton_bottom,
			  "value-changed", G_CALLBACK (on_widget_change_set_toggle_active),
			  checkbutton_bottom );
	g_signal_connect ((gpointer) combo_entry_bottom,
			  "changed", G_CALLBACK (on_widget_change_set_toggle_active),
			  checkbutton_bottom);			  

	g_signal_connect ((gpointer) spinbutton_left,
			  "value-changed", G_CALLBACK (on_widget_change_set_toggle_active),
			  checkbutton_left );
	g_signal_connect ((gpointer) combo_entry_left,
			  "changed", G_CALLBACK (on_widget_change_set_toggle_active),
			  checkbutton_left);			

			  
	if (connect_callbacks)
	{
		g_signal_connect ((gpointer) padding_property_dialog, "close",
				  G_CALLBACK (on_css_dialog_close), data);

		g_signal_connect ((gpointer) helpbutton, "clicked",
				  G_CALLBACK
				  (on_padding_property_dialog_helpbutton_clicked),
				  NULL);
		g_signal_connect ((gpointer) cancelbutton, "clicked",
				  G_CALLBACK
				  (on_css_dialog_cancel_button_clicked),
				  data);
		g_signal_connect ((gpointer) okbutton, "clicked",
				  G_CALLBACK
				  (on_padding_property_dialog_okbutton_clicked),
				  data);
	}

	GLADE_HOOKUP_OBJECT_NO_REF (padding_property_dialog,
				    padding_property_dialog,
				    "padding_property_dialog");
	GLADE_HOOKUP_OBJECT (padding_property_dialog, checkbutton_top,
			     "checkbutton_top");
	GLADE_HOOKUP_OBJECT (padding_property_dialog, spinbutton_top,
			     "spinbutton_top");
	GLADE_HOOKUP_OBJECT (padding_property_dialog, combo_top, "combo_top");
	GLADE_HOOKUP_OBJECT (padding_property_dialog, combo_entry_top,
			     "combo_entry_top");
	GLADE_HOOKUP_OBJECT (padding_property_dialog, checkbutton_left,
			     "checkbutton_left");
	GLADE_HOOKUP_OBJECT (padding_property_dialog, spinbutton_left,
			     "spinbutton_left");
	GLADE_HOOKUP_OBJECT (padding_property_dialog, combo_left,
			     "combo_left");
	GLADE_HOOKUP_OBJECT (padding_property_dialog, combo_entry_left,
			     "combo_entry_left");
	GLADE_HOOKUP_OBJECT (padding_property_dialog, hbox_bottom,
			     "hbox_bottom");
	GLADE_HOOKUP_OBJECT (padding_property_dialog, checkbutton_bottom,
			     "checkbutton_bottom");
	GLADE_HOOKUP_OBJECT (padding_property_dialog, spinbutton_bottom,
			     "spinbutton_bottom");
	GLADE_HOOKUP_OBJECT (padding_property_dialog, combo_bottom,
			     "combo_bottom");
	GLADE_HOOKUP_OBJECT (padding_property_dialog, combo_entry_bottom,
			     "combo_entry_bottom");
	GLADE_HOOKUP_OBJECT (padding_property_dialog, checkbutton_right,
			     "checkbutton_right");
	GLADE_HOOKUP_OBJECT (padding_property_dialog, spinbutton_right,
			     "spinbutton_right");
	GLADE_HOOKUP_OBJECT (padding_property_dialog, combo_right,
			     "combo_right");
	GLADE_HOOKUP_OBJECT (padding_property_dialog, combo_entry_right,
			     "combo_entry_right");
	GLADE_HOOKUP_OBJECT_NO_REF (padding_property_dialog,
				    dialog_action_area, "dialog_action_area");
	GLADE_HOOKUP_OBJECT (padding_property_dialog, helpbutton,
			     "helpbutton");
	GLADE_HOOKUP_OBJECT (padding_property_dialog, cancelbutton,
			     "cancelbutton");
	GLADE_HOOKUP_OBJECT (padding_property_dialog, okbutton, "okbutton");

	gtk_window_add_accel_group (GTK_WINDOW (padding_property_dialog),
				    accel_group);

	return padding_property_dialog;
}

GtkWidget *
create_outline_property_dialog (CssedWindow * window,
				gchar * previous_string,
				gboolean add_previous_string,
				gboolean connect_callbacks)
{
	GtkWidget *outline_property_dialog;
	GtkWidget *dialog_vbox;
	GtkWidget *vbox;
	GtkWidget *hbox_length;
	GtkWidget *checkbutton_length;
	GtkWidget *combo_length_selector;
	GList *combo_length_selector_items = NULL;
	GtkWidget *combo_entry_length_selector;
	GtkObject *spinbutton_length_adj;
	GtkWidget *spinbutton_length;
	GtkWidget *combo_length_units;
	GList *combo_length_units_items = NULL;
	GtkWidget *combo_entry_length_units;
	GtkWidget *hbox_style;
	GtkWidget *checkbutton_style;
	GtkWidget *combo_style;
	GList *combo_style_items = NULL;
	GtkWidget *combo_entry_style;
	GtkWidget *hbox_color;
	GtkWidget *checkbutton_color;
	GtkWidget *combo_color;
	GList *combo_color_items = NULL;
	GtkWidget *combo_entry_color;
	GtkWidget *colorselection;
	GtkWidget *dialog_action_area;
	GtkWidget *helpbutton;
	GtkWidget *cancelbutton;
	GtkWidget *okbutton;
	GtkAccelGroup *accel_group;
	GtkWidget *main_window;
	CssedCssDialogsData *data = NULL;

	if (connect_callbacks)
	{
		data = g_malloc (sizeof (CssedCssDialogsData));
		data->main_window = window;
		data->previous_string = previous_string;
		data->add_property_string = add_previous_string;
	}

	accel_group = gtk_accel_group_new ();

	outline_property_dialog = gtk_dialog_new ();
	gtk_window_set_title (GTK_WINDOW (outline_property_dialog),
			      previous_string);
	gtk_window_set_modal (GTK_WINDOW (outline_property_dialog), TRUE);

	main_window = cssed_window_get_window_widget( window );

	gtk_window_set_transient_for (GTK_WINDOW (outline_property_dialog),
				      GTK_WINDOW (main_window));

	dialog_vbox = GTK_DIALOG (outline_property_dialog)->vbox;
	gtk_widget_show (dialog_vbox);

	vbox = gtk_vbox_new (FALSE, 0);
	gtk_widget_show (vbox);
	gtk_box_pack_start (GTK_BOX (dialog_vbox), vbox, TRUE, TRUE, 0);

	hbox_length = gtk_hbox_new (FALSE, 0);
	gtk_widget_show (hbox_length);
	gtk_box_pack_start (GTK_BOX (vbox), hbox_length, TRUE, TRUE, 0);

	checkbutton_length =
		gtk_check_button_new_with_mnemonic ("outline-length");
	gtk_widget_show (checkbutton_length);
	gtk_box_pack_start (GTK_BOX (hbox_length), checkbutton_length, FALSE,
			    FALSE, 5);

	combo_length_selector = gtk_combo_new ();
	g_object_set_data (G_OBJECT
			   (GTK_COMBO (combo_length_selector)->popwin),
			   "GladeParentKey", combo_length_selector);
	gtk_widget_show (combo_length_selector);
	gtk_box_pack_start (GTK_BOX (hbox_length), combo_length_selector,
			    TRUE, FALSE, 5);
	combo_length_selector_items =
		g_list_append (combo_length_selector_items,
			       (gpointer) CSS_STR_USE_PERCLEN);
	combo_length_selector_items =
		g_list_append (combo_length_selector_items,
			       (gpointer) "thin");
	combo_length_selector_items =
		g_list_append (combo_length_selector_items,
			       (gpointer) "medium");
	combo_length_selector_items =
		g_list_append (combo_length_selector_items,
			       (gpointer) "thick");
	gtk_combo_set_popdown_strings (GTK_COMBO (combo_length_selector),
				       combo_length_selector_items);
	g_list_free (combo_length_selector_items);

	combo_entry_length_selector =
		GTK_COMBO (combo_length_selector)->entry;
	gtk_widget_show (combo_entry_length_selector);
	gtk_editable_set_editable (GTK_EDITABLE (combo_entry_length_selector),
				   FALSE);
	gtk_entry_set_text (GTK_ENTRY (combo_entry_length_selector),
			    CSS_STR_USE_PERCLEN);

	spinbutton_length_adj =
		gtk_adjustment_new (1, -35565, 35565, 1, 10, 10);
	spinbutton_length =
		gtk_spin_button_new (GTK_ADJUSTMENT (spinbutton_length_adj),
				     1, 0);
	gtk_widget_show (spinbutton_length);
	gtk_box_pack_start (GTK_BOX (hbox_length), spinbutton_length, TRUE,
			    TRUE, 5);

	combo_length_units = gtk_combo_new ();
	g_object_set_data (G_OBJECT (GTK_COMBO (combo_length_units)->popwin),
			   "GladeParentKey", combo_length_units);
	gtk_widget_show (combo_length_units);
	gtk_box_pack_start (GTK_BOX (hbox_length), combo_length_units, FALSE,
			    FALSE, 5);
	combo_length_units_items =
		g_list_append (combo_length_units_items, (gpointer) "%");
	combo_length_units_items =
		g_list_append (combo_length_units_items, (gpointer) "em");
	combo_length_units_items =
		g_list_append (combo_length_units_items, (gpointer) "ex");
	combo_length_units_items =
		g_list_append (combo_length_units_items, (gpointer) "px");
	combo_length_units_items =
		g_list_append (combo_length_units_items, (gpointer) "in");
	combo_length_units_items =
		g_list_append (combo_length_units_items, (gpointer) "cm");
	combo_length_units_items =
		g_list_append (combo_length_units_items, (gpointer) "mm");
	combo_length_units_items =
		g_list_append (combo_length_units_items, (gpointer) "pt");
	combo_length_units_items =
		g_list_append (combo_length_units_items, (gpointer) "pc");
	gtk_combo_set_popdown_strings (GTK_COMBO (combo_length_units),
				       combo_length_units_items);
	g_list_free (combo_length_units_items);

	combo_entry_length_units = GTK_COMBO (combo_length_units)->entry;
	gtk_widget_show (combo_entry_length_units);
	gtk_widget_set_size_request (combo_entry_length_units, 25, -1);
	gtk_editable_set_editable (GTK_EDITABLE (combo_entry_length_units),
				   FALSE);
	gtk_entry_set_text (GTK_ENTRY (combo_entry_length_units), "px");

	hbox_style = gtk_hbox_new (TRUE, 0);
	gtk_widget_show (hbox_style);
	gtk_box_pack_start (GTK_BOX (vbox), hbox_style, TRUE, TRUE, 0);

	checkbutton_style =
		gtk_check_button_new_with_mnemonic ("outline-style");
	gtk_widget_show (checkbutton_style);
	gtk_box_pack_start (GTK_BOX (hbox_style), checkbutton_style, TRUE,
			    TRUE, 5);

	combo_style = gtk_combo_new ();
	g_object_set_data (G_OBJECT (GTK_COMBO (combo_style)->popwin),
			   "GladeParentKey", combo_style);
	gtk_widget_show (combo_style);
	gtk_box_pack_start (GTK_BOX (hbox_style), combo_style, TRUE, TRUE, 5);
	combo_style_items =
		g_list_append (combo_style_items, (gpointer) "none");
	combo_style_items =
		g_list_append (combo_style_items, (gpointer) "hidden ");
	combo_style_items =
		g_list_append (combo_style_items, (gpointer) "dotted ");
	combo_style_items =
		g_list_append (combo_style_items, (gpointer) "dashed ");
	combo_style_items =
		g_list_append (combo_style_items, (gpointer) "solid ");
	combo_style_items =
		g_list_append (combo_style_items, (gpointer) "double ");
	combo_style_items =
		g_list_append (combo_style_items, (gpointer) "groove ");
	combo_style_items =
		g_list_append (combo_style_items, (gpointer) "ridge ");
	combo_style_items =
		g_list_append (combo_style_items, (gpointer) "inset ");
	combo_style_items =
		g_list_append (combo_style_items, (gpointer) "outset");
	gtk_combo_set_popdown_strings (GTK_COMBO (combo_style),
				       combo_style_items);
	g_list_free (combo_style_items);

	combo_entry_style = GTK_COMBO (combo_style)->entry;
	gtk_widget_show (combo_entry_style);
	gtk_editable_set_editable (GTK_EDITABLE (combo_entry_style), FALSE);
	gtk_entry_set_text (GTK_ENTRY (combo_entry_style), "none");

	hbox_color = gtk_hbox_new (TRUE, 0);
	gtk_widget_show (hbox_color);
	gtk_box_pack_start (GTK_BOX (vbox), hbox_color, TRUE, TRUE, 0);

	checkbutton_color =
		gtk_check_button_new_with_mnemonic ("outline-color");
	gtk_widget_show (checkbutton_color);
	gtk_box_pack_start (GTK_BOX (hbox_color), checkbutton_color, TRUE,
			    TRUE, 5);

	combo_color = gtk_combo_new ();
	g_object_set_data (G_OBJECT (GTK_COMBO (combo_color)->popwin),
			   "GladeParentKey", combo_color);
	gtk_widget_show (combo_color);
	gtk_box_pack_start (GTK_BOX (hbox_color), combo_color, TRUE, TRUE, 5);
	combo_color_items =
		g_list_append (combo_color_items,
			       (gpointer) CSS_STR_USE_COLORSEL);
	combo_color_items =
		g_list_append (combo_color_items, (gpointer) "aqua");
	combo_color_items =
		g_list_append (combo_color_items, (gpointer) "black");
	combo_color_items =
		g_list_append (combo_color_items, (gpointer) "blue");
	combo_color_items =
		g_list_append (combo_color_items, (gpointer) "fuchsia");
	combo_color_items =
		g_list_append (combo_color_items, (gpointer) "gray");
	combo_color_items =
		g_list_append (combo_color_items, (gpointer) "green");
	combo_color_items =
		g_list_append (combo_color_items, (gpointer) "lime");
	combo_color_items =
		g_list_append (combo_color_items, (gpointer) "maroon ");
	combo_color_items =
		g_list_append (combo_color_items, (gpointer) "navy");
	combo_color_items =
		g_list_append (combo_color_items, (gpointer) "olive");
	combo_color_items =
		g_list_append (combo_color_items, (gpointer) "purple");
	combo_color_items =
		g_list_append (combo_color_items, (gpointer) "red");
	combo_color_items =
		g_list_append (combo_color_items, (gpointer) "silver");
	combo_color_items =
		g_list_append (combo_color_items, (gpointer) "teal");
	combo_color_items =
		g_list_append (combo_color_items, (gpointer) "white");
	combo_color_items =
		g_list_append (combo_color_items, (gpointer) "yellow");
	combo_color_items =
		g_list_append (combo_color_items, (gpointer) "ActiveBorder");
	combo_color_items =
		g_list_append (combo_color_items, (gpointer) "ActiveCaption");
	combo_color_items =
		g_list_append (combo_color_items, (gpointer) "AppWorkspace");
	combo_color_items =
		g_list_append (combo_color_items, (gpointer) "Background");
	combo_color_items =
		g_list_append (combo_color_items, (gpointer) "ButtonFace");
	combo_color_items =
		g_list_append (combo_color_items,
			       (gpointer) "ButtonHighlight");
	combo_color_items =
		g_list_append (combo_color_items, (gpointer) "ButtonShadow");
	combo_color_items =
		g_list_append (combo_color_items, (gpointer) "ButtonText");
	combo_color_items =
		g_list_append (combo_color_items, (gpointer) "CaptionText");
	combo_color_items =
		g_list_append (combo_color_items, (gpointer) "GrayText");
	combo_color_items =
		g_list_append (combo_color_items, (gpointer) "Highlight");
	combo_color_items =
		g_list_append (combo_color_items, (gpointer) "HighlightText");
	combo_color_items =
		g_list_append (combo_color_items,
			       (gpointer) "InactiveBorder");
	combo_color_items =
		g_list_append (combo_color_items,
			       (gpointer) "InactiveCaption");
	combo_color_items =
		g_list_append (combo_color_items,
			       (gpointer) "InactiveCaptionText");
	combo_color_items =
		g_list_append (combo_color_items,
			       (gpointer) "InfoBackground");
	combo_color_items =
		g_list_append (combo_color_items, (gpointer) "InfoText");
	combo_color_items =
		g_list_append (combo_color_items, (gpointer) "Menu");
	combo_color_items =
		g_list_append (combo_color_items, (gpointer) "MenuText");
	combo_color_items =
		g_list_append (combo_color_items, (gpointer) "Scrollbar");
	combo_color_items =
		g_list_append (combo_color_items,
			       (gpointer) "ThreeDDarkShadow");
	combo_color_items =
		g_list_append (combo_color_items, (gpointer) "ThreeDFace");
	combo_color_items =
		g_list_append (combo_color_items,
			       (gpointer) "ThreeDHighlight");
	combo_color_items =
		g_list_append (combo_color_items,
			       (gpointer) "ThreeDLightShadow");
	combo_color_items =
		g_list_append (combo_color_items, (gpointer) "ThreeDShadow");
	combo_color_items =
		g_list_append (combo_color_items, (gpointer) "Window");
	combo_color_items =
		g_list_append (combo_color_items, (gpointer) "WindowFrame");
	combo_color_items =
		g_list_append (combo_color_items, (gpointer) "WindowText");
	gtk_combo_set_popdown_strings (GTK_COMBO (combo_color),
				       combo_color_items);
	g_list_free (combo_color_items);

	combo_entry_color = GTK_COMBO (combo_color)->entry;
	gtk_widget_show (combo_entry_color);
	gtk_editable_set_editable (GTK_EDITABLE (combo_entry_color), FALSE);
	gtk_entry_set_text (GTK_ENTRY (combo_entry_color),
			    CSS_STR_USE_COLORSEL);

	colorselection = gtk_color_selection_new ();
	gtk_widget_show (colorselection);
	gtk_box_pack_start (GTK_BOX (vbox), colorselection, FALSE, FALSE, 0);
	gtk_container_set_border_width (GTK_CONTAINER (colorselection), 5);
	gtk_color_selection_set_has_opacity_control (GTK_COLOR_SELECTION
						     (colorselection), FALSE);
	gtk_color_selection_set_has_palette (GTK_COLOR_SELECTION
					     (colorselection), TRUE);

	dialog_action_area =
		GTK_DIALOG (outline_property_dialog)->action_area;
	gtk_widget_show (dialog_action_area);
	gtk_button_box_set_layout (GTK_BUTTON_BOX (dialog_action_area),
				   GTK_BUTTONBOX_END);

	helpbutton = gtk_button_new_from_stock ("gtk-help");
	gtk_widget_show (helpbutton);
	gtk_dialog_add_action_widget (GTK_DIALOG (outline_property_dialog),
				      helpbutton, GTK_RESPONSE_HELP);
	GTK_WIDGET_SET_FLAGS (helpbutton, GTK_CAN_DEFAULT);
	gtk_widget_add_accelerator (helpbutton, "clicked", accel_group,
				    GDK_y, GDK_CONTROL_MASK,
				    GTK_ACCEL_VISIBLE);
	gtk_widget_add_accelerator (helpbutton, "clicked", accel_group, GDK_h,
				    GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);

	cancelbutton = gtk_button_new_from_stock ("gtk-cancel");
	gtk_widget_show (cancelbutton);
	gtk_dialog_add_action_widget (GTK_DIALOG (outline_property_dialog),
				      cancelbutton, GTK_RESPONSE_CANCEL);
	GTK_WIDGET_SET_FLAGS (cancelbutton, GTK_CAN_DEFAULT);
	gtk_widget_add_accelerator (cancelbutton, "clicked", accel_group,
				    GDK_c, GDK_CONTROL_MASK,
				    GTK_ACCEL_VISIBLE);

	okbutton = gtk_button_new_from_stock ("gtk-ok");
	gtk_widget_show (okbutton);
	gtk_dialog_add_action_widget (GTK_DIALOG (outline_property_dialog),
				      okbutton, GTK_RESPONSE_OK);
	GTK_WIDGET_SET_FLAGS (okbutton, GTK_CAN_DEFAULT);
	gtk_widget_add_accelerator (okbutton, "clicked", accel_group,
				    GDK_o, GDK_CONTROL_MASK,
				    GTK_ACCEL_VISIBLE);

	g_signal_connect ((gpointer) combo_entry_length_units,
			  "changed", G_CALLBACK (on_css_dialog_unit_changed),
			  spinbutton_length);
			  
	/* to update the checkboxes when a related widget is changed */				    
	g_signal_connect ((gpointer) spinbutton_length,
			  "value-changed", G_CALLBACK (on_widget_change_set_toggle_active),
			  checkbutton_length );
	g_signal_connect ((gpointer) combo_entry_length_selector,
			  "changed", G_CALLBACK (on_widget_change_set_toggle_active),
			  checkbutton_length);
	g_signal_connect ((gpointer) combo_entry_length_selector,
			  "changed", G_CALLBACK (on_widget_change_set_toggle_active),
			  checkbutton_length);
			  
	g_signal_connect ((gpointer) combo_entry_style,
			  "changed", G_CALLBACK (on_widget_change_set_toggle_active),
			  checkbutton_style);

	g_signal_connect ((gpointer) combo_entry_color,
			  "changed", G_CALLBACK (on_widget_change_set_toggle_active),
			  checkbutton_color);
	g_signal_connect ((gpointer) colorselection,
			  "color-changed", G_CALLBACK (on_widget_change_set_toggle_active),
			  checkbutton_color);
		  			  
			  
	if (connect_callbacks)
	{
		g_signal_connect ((gpointer) outline_property_dialog, "close",
				  G_CALLBACK (on_css_dialog_close), data);

		g_signal_connect ((gpointer) helpbutton, "clicked",
				  G_CALLBACK
				  (on_outline_property_dialog_helpbutton_clicked),
				  NULL);
		g_signal_connect ((gpointer) cancelbutton, "clicked",
				  G_CALLBACK
				  (on_css_dialog_cancel_button_clicked),
				  data);
		g_signal_connect ((gpointer) okbutton, "clicked",
				  G_CALLBACK
				  (on_outline_property_dialog_okbutton_clicked),
				  data);
	}

	GLADE_HOOKUP_OBJECT_NO_REF (outline_property_dialog,
				    outline_property_dialog,
				    "outline_property_dialog");
	GLADE_HOOKUP_OBJECT_NO_REF (outline_property_dialog, dialog_vbox,
				    "dialog_vbox");
	GLADE_HOOKUP_OBJECT (outline_property_dialog, vbox, "vbox");
	GLADE_HOOKUP_OBJECT (outline_property_dialog, hbox_length,
			     "hbox_length");
	GLADE_HOOKUP_OBJECT (outline_property_dialog, checkbutton_length,
			     "checkbutton_length");
	GLADE_HOOKUP_OBJECT (outline_property_dialog, combo_length_selector,
			     "combo_length_selector");
	GLADE_HOOKUP_OBJECT (outline_property_dialog,
			     combo_entry_length_selector,
			     "combo_entry_length_selector");
	GLADE_HOOKUP_OBJECT (outline_property_dialog, spinbutton_length,
			     "spinbutton_length");
	GLADE_HOOKUP_OBJECT (outline_property_dialog, combo_length_units,
			     "combo_length_units");
	GLADE_HOOKUP_OBJECT (outline_property_dialog,
			     combo_entry_length_units,
			     "combo_entry_length_units");
	GLADE_HOOKUP_OBJECT (outline_property_dialog, hbox_style,
			     "hbox_style");
	GLADE_HOOKUP_OBJECT (outline_property_dialog, checkbutton_style,
			     "checkbutton_style");
	GLADE_HOOKUP_OBJECT (outline_property_dialog, combo_style,
			     "combo_style");
	GLADE_HOOKUP_OBJECT (outline_property_dialog, combo_entry_style,
			     "combo_entry_style");
	GLADE_HOOKUP_OBJECT (outline_property_dialog, hbox_color,
			     "hbox_color");
	GLADE_HOOKUP_OBJECT (outline_property_dialog, checkbutton_color,
			     "checkbutton_color");
	GLADE_HOOKUP_OBJECT (outline_property_dialog, combo_color,
			     "combo_color");
	GLADE_HOOKUP_OBJECT (outline_property_dialog, combo_entry_color,
			     "combo_entry_color");
	GLADE_HOOKUP_OBJECT (outline_property_dialog, colorselection,
			     "colorselection");
	GLADE_HOOKUP_OBJECT_NO_REF (outline_property_dialog,
				    dialog_action_area, "dialog_action_area");
	GLADE_HOOKUP_OBJECT (outline_property_dialog, helpbutton,
			     "helpbutton");
	GLADE_HOOKUP_OBJECT (outline_property_dialog, cancelbutton,
			     "cancelbutton");
	GLADE_HOOKUP_OBJECT (outline_property_dialog, okbutton, "okbutton");

	gtk_window_add_accel_group (GTK_WINDOW (outline_property_dialog),
				    accel_group);

	return outline_property_dialog;
}

GtkWidget *
create_pause_property_dialog (CssedWindow * window,
			      gchar * previous_string,
			      gboolean add_previous_string,
			      gboolean connect_callbacks)
{
	GtkWidget *pause_property_dialog;
	GtkWidget *dialog_vbox;
	GtkWidget *vbox;
	GtkWidget *hbox_after;
	GtkWidget *checkbutton_after;
	GtkObject *spinbutton_after_adj;
	GtkWidget *spinbutton_after;
	GtkWidget *combo_after;
	GList *combo_after_items = NULL;
	GtkWidget *combo_entry_after;
	GtkWidget *hbox_before;
	GtkWidget *checkbutton_before;
	GtkObject *spinbutton_before_adj;
	GtkWidget *spinbutton_before;
	GtkWidget *combo_before;
	GList *combo_before_items = NULL;
	GtkWidget *combo_entry_before;
	GtkWidget *dialog_action_area;
	GtkWidget *helpbutton;
	GtkWidget *cancelbutton;
	GtkWidget *okbutton;
	GtkAccelGroup *accel_group;
	GtkWidget *main_window;
	CssedCssDialogsData *data = NULL;

	if (connect_callbacks)
	{
		data = g_malloc (sizeof (CssedCssDialogsData));
		data->main_window = window;
		data->previous_string = previous_string;
		data->add_property_string = add_previous_string;
	}

	accel_group = gtk_accel_group_new ();

	pause_property_dialog = gtk_dialog_new ();
	gtk_window_set_title (GTK_WINDOW (pause_property_dialog),
			      previous_string);
	gtk_window_set_modal (GTK_WINDOW (pause_property_dialog), TRUE);

	main_window = cssed_window_get_window_widget( window );

	gtk_window_set_transient_for (GTK_WINDOW (pause_property_dialog),
				      GTK_WINDOW (main_window));

	dialog_vbox = GTK_DIALOG (pause_property_dialog)->vbox;
	gtk_widget_show (dialog_vbox);

	vbox = gtk_vbox_new (TRUE, 0);
	gtk_widget_show (vbox);
	gtk_box_pack_start (GTK_BOX (dialog_vbox), vbox, TRUE, TRUE, 0);

	hbox_after = gtk_hbox_new (TRUE, 0);
	gtk_widget_show (hbox_after);
	gtk_box_pack_start (GTK_BOX (vbox), hbox_after, TRUE, TRUE, 0);

	checkbutton_after =
		gtk_check_button_new_with_mnemonic ("pause-after");
	gtk_widget_show (checkbutton_after);
	gtk_box_pack_start (GTK_BOX (hbox_after), checkbutton_after, TRUE,
			    TRUE, 5);

	spinbutton_after_adj =
		gtk_adjustment_new (1, -35565, 35565, 1, 10, 10);
	spinbutton_after =
		gtk_spin_button_new (GTK_ADJUSTMENT (spinbutton_after_adj), 1,
				     0);
	gtk_widget_show (spinbutton_after);
	gtk_box_pack_start (GTK_BOX (hbox_after), spinbutton_after, FALSE,
			    FALSE, 0);

	combo_after = gtk_combo_new ();
	g_object_set_data (G_OBJECT (GTK_COMBO (combo_after)->popwin),
			   "GladeParentKey", combo_after);
	gtk_widget_show (combo_after);
	gtk_box_pack_start (GTK_BOX (hbox_after), combo_after, FALSE, FALSE,
			    0);
	combo_after_items = g_list_append (combo_after_items, (gpointer) "%");
	combo_after_items =
		g_list_append (combo_after_items, (gpointer) "ms");
	combo_after_items = g_list_append (combo_after_items, (gpointer) "s");
	gtk_combo_set_popdown_strings (GTK_COMBO (combo_after),
				       combo_after_items);
	g_list_free (combo_after_items);

	combo_entry_after = GTK_COMBO (combo_after)->entry;
	gtk_widget_show (combo_entry_after);
	gtk_widget_set_size_request (combo_entry_after, 40, -1);
	gtk_entry_set_text (GTK_ENTRY (combo_entry_after), "%");

	hbox_before = gtk_hbox_new (TRUE, 0);
	gtk_widget_show (hbox_before);
	gtk_box_pack_start (GTK_BOX (vbox), hbox_before, TRUE, TRUE, 0);

	checkbutton_before =
		gtk_check_button_new_with_mnemonic ("pause-before");
	gtk_widget_show (checkbutton_before);
	gtk_box_pack_start (GTK_BOX (hbox_before), checkbutton_before, TRUE,
			    TRUE, 5);

	spinbutton_before_adj =
		gtk_adjustment_new (1, -35565, 35565, 1, 10, 10);
	spinbutton_before =
		gtk_spin_button_new (GTK_ADJUSTMENT (spinbutton_before_adj),
				     1, 0);
	gtk_widget_show (spinbutton_before);
	gtk_box_pack_start (GTK_BOX (hbox_before), spinbutton_before, FALSE,
			    FALSE, 0);

	combo_before = gtk_combo_new ();
	g_object_set_data (G_OBJECT (GTK_COMBO (combo_before)->popwin),
			   "GladeParentKey", combo_before);
	gtk_widget_show (combo_before);
	gtk_box_pack_start (GTK_BOX (hbox_before), combo_before, FALSE, FALSE,
			    0);
	combo_before_items =
		g_list_append (combo_before_items, (gpointer) "%");
	combo_before_items =
		g_list_append (combo_before_items, (gpointer) "ms");
	combo_before_items =
		g_list_append (combo_before_items, (gpointer) "s");
	gtk_combo_set_popdown_strings (GTK_COMBO (combo_before),
				       combo_before_items);
	g_list_free (combo_before_items);

	combo_entry_before = GTK_COMBO (combo_before)->entry;
	gtk_widget_show (combo_entry_before);
	gtk_widget_set_size_request (combo_entry_before, 40, -1);
	gtk_entry_set_text (GTK_ENTRY (combo_entry_before), "%");

	dialog_action_area = GTK_DIALOG (pause_property_dialog)->action_area;
	gtk_widget_show (dialog_action_area);
	gtk_button_box_set_layout (GTK_BUTTON_BOX (dialog_action_area),
				   GTK_BUTTONBOX_END);

	helpbutton = gtk_button_new_from_stock ("gtk-help");
	gtk_widget_show (helpbutton);
	gtk_dialog_add_action_widget (GTK_DIALOG (pause_property_dialog),
				      helpbutton, GTK_RESPONSE_HELP);
	GTK_WIDGET_SET_FLAGS (helpbutton, GTK_CAN_DEFAULT);
	gtk_widget_add_accelerator (helpbutton, "clicked", accel_group,
				    GDK_y, GDK_CONTROL_MASK,
				    GTK_ACCEL_VISIBLE);
	gtk_widget_add_accelerator (helpbutton, "clicked", accel_group, GDK_h,
				    GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);

	cancelbutton = gtk_button_new_from_stock ("gtk-cancel");
	gtk_widget_show (cancelbutton);
	gtk_dialog_add_action_widget (GTK_DIALOG (pause_property_dialog),
				      cancelbutton, GTK_RESPONSE_CANCEL);
	GTK_WIDGET_SET_FLAGS (cancelbutton, GTK_CAN_DEFAULT);
	gtk_widget_add_accelerator (cancelbutton, "clicked", accel_group,
				    GDK_c, GDK_CONTROL_MASK,
				    GTK_ACCEL_VISIBLE);

	okbutton = gtk_button_new_from_stock ("gtk-ok");
	gtk_widget_show (okbutton);
	gtk_dialog_add_action_widget (GTK_DIALOG (pause_property_dialog),
				      okbutton, GTK_RESPONSE_OK);
	GTK_WIDGET_SET_FLAGS (okbutton, GTK_CAN_DEFAULT);
	gtk_widget_add_accelerator (okbutton, "clicked", accel_group,
				    GDK_o, GDK_CONTROL_MASK,
				    GTK_ACCEL_VISIBLE);
				    
	/* to update the checkboxes when a related widget is changed */				    
	g_signal_connect ((gpointer) spinbutton_after,
			  "value-changed", G_CALLBACK (on_widget_change_set_toggle_active),
			  checkbutton_after );
	g_signal_connect ((gpointer) combo_entry_after,
			  "changed", G_CALLBACK (on_widget_change_set_toggle_active),
			  checkbutton_after);
			  
	g_signal_connect ((gpointer) spinbutton_before,
			  "value-changed", G_CALLBACK (on_widget_change_set_toggle_active),
			  checkbutton_before );
	g_signal_connect ((gpointer) combo_entry_before,
			  "changed", G_CALLBACK (on_widget_change_set_toggle_active),
			  checkbutton_before);
			  
	if (connect_callbacks)
	{
		g_signal_connect ((gpointer) pause_property_dialog, "close",
				  G_CALLBACK (on_css_dialog_close), data);

		g_signal_connect ((gpointer) helpbutton, "clicked",
				  G_CALLBACK
				  (on_pause_property_dialog_helpbutton_clicked),
				  NULL);
		g_signal_connect ((gpointer) cancelbutton, "clicked",
				  G_CALLBACK
				  (on_css_dialog_cancel_button_clicked),
				  data);
		g_signal_connect ((gpointer) okbutton, "clicked",
				  G_CALLBACK
				  (on_pause_property_dialog_okbutton_clicked),
				  data);
	}

	GLADE_HOOKUP_OBJECT_NO_REF (pause_property_dialog,
				    pause_property_dialog,
				    "pause_property_dialog");
	GLADE_HOOKUP_OBJECT (pause_property_dialog, checkbutton_after,
			     "checkbutton_after");
	GLADE_HOOKUP_OBJECT (pause_property_dialog, spinbutton_after,
			     "spinbutton_after");
	GLADE_HOOKUP_OBJECT (pause_property_dialog, combo_after,
			     "combo_after");
	GLADE_HOOKUP_OBJECT (pause_property_dialog, combo_entry_after,
			     "combo_entry_after");
	GLADE_HOOKUP_OBJECT (pause_property_dialog, hbox_before,
			     "hbox_before");
	GLADE_HOOKUP_OBJECT (pause_property_dialog, checkbutton_before,
			     "checkbutton_before");
	GLADE_HOOKUP_OBJECT (pause_property_dialog, spinbutton_before,
			     "spinbutton_before");
	GLADE_HOOKUP_OBJECT (pause_property_dialog, combo_before,
			     "combo_before");
	GLADE_HOOKUP_OBJECT (pause_property_dialog, combo_entry_before,
			     "combo_entry_before");
	GLADE_HOOKUP_OBJECT_NO_REF (pause_property_dialog, dialog_action_area,
				    "dialog_action_area");
	GLADE_HOOKUP_OBJECT (pause_property_dialog, helpbutton, "helpbutton");
	GLADE_HOOKUP_OBJECT (pause_property_dialog, cancelbutton,
			     "cancelbutton");
	GLADE_HOOKUP_OBJECT (pause_property_dialog, okbutton, "okbutton");

	gtk_window_add_accel_group (GTK_WINDOW (pause_property_dialog),
				    accel_group);

	return pause_property_dialog;
}

GtkWidget *
create_percentage_value_dialog (CssedWindow * window,
								gchar * previous_string,
								gboolean add_previous_string,
								gboolean connect_callbacks)
{
	GtkWidget *percentage_value_dialog;
	GtkWidget *dialog_vbox;
	GtkWidget *hbox_pc;
	GtkWidget *label_property;
	GtkObject *spinbutton_percentage_adj;
	GtkWidget *spinbutton_percentage;
	GtkWidget *label_pc;
	GtkWidget *dialog_action_area;
	GtkWidget *helpbutton;
	GtkWidget *cancelbutton;
	GtkWidget *okbutton;
	GtkAccelGroup *accel_group;
	GtkWidget *main_window;
	CssedCssDialogsData *data = NULL;

	if (connect_callbacks)
	{
		data = g_malloc (sizeof (CssedCssDialogsData));
		data->main_window = window;
		data->previous_string = previous_string;
		data->add_property_string = add_previous_string;
	}

	accel_group = gtk_accel_group_new ();

	percentage_value_dialog = gtk_dialog_new ();
	gtk_window_set_modal (GTK_WINDOW (percentage_value_dialog), TRUE);
	gtk_window_set_title (GTK_WINDOW (percentage_value_dialog),
			      previous_string);

	main_window = cssed_window_get_window_widget( window );

	gtk_window_set_transient_for (GTK_WINDOW (percentage_value_dialog),
				      GTK_WINDOW (main_window));

	dialog_vbox = GTK_DIALOG (percentage_value_dialog)->vbox;
	gtk_widget_show (dialog_vbox);

	hbox_pc = gtk_hbox_new (FALSE, 0);
	gtk_widget_show (hbox_pc);
	gtk_box_pack_start (GTK_BOX (dialog_vbox), hbox_pc, TRUE, TRUE, 0);

	label_property = gtk_label_new ("percentage");
	gtk_widget_show (label_property);
	gtk_box_pack_start (GTK_BOX (hbox_pc), label_property, TRUE, TRUE, 0);
	gtk_label_set_justify (GTK_LABEL (label_property), GTK_JUSTIFY_LEFT);

	spinbutton_percentage_adj =
		gtk_adjustment_new (1, -35565, 35565, 1, 10, 10);
	spinbutton_percentage =
		gtk_spin_button_new (GTK_ADJUSTMENT
				     (spinbutton_percentage_adj), 1, 0);
	gtk_widget_show (spinbutton_percentage);
	gtk_box_pack_start (GTK_BOX (hbox_pc), spinbutton_percentage, FALSE,
			    FALSE, 5);

	label_pc = gtk_label_new ("%");
	gtk_widget_show (label_pc);
	gtk_box_pack_start (GTK_BOX (hbox_pc), label_pc, FALSE, FALSE, 10);
	gtk_label_set_justify (GTK_LABEL (label_pc), GTK_JUSTIFY_LEFT);

	dialog_action_area =
		GTK_DIALOG (percentage_value_dialog)->action_area;
	gtk_widget_show (dialog_action_area);
	gtk_button_box_set_layout (GTK_BUTTON_BOX (dialog_action_area),
				   GTK_BUTTONBOX_END);

	helpbutton = gtk_button_new_from_stock ("gtk-help");
	gtk_widget_show (helpbutton);
	gtk_dialog_add_action_widget (GTK_DIALOG (percentage_value_dialog),
				      helpbutton, GTK_RESPONSE_HELP);
	GTK_WIDGET_SET_FLAGS (helpbutton, GTK_CAN_DEFAULT);
	gtk_widget_add_accelerator (helpbutton, "clicked", accel_group,
				    GDK_y, GDK_CONTROL_MASK,
				    GTK_ACCEL_VISIBLE);
	gtk_widget_add_accelerator (helpbutton, "clicked", accel_group, GDK_h,
				    GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);

	cancelbutton = gtk_button_new_from_stock ("gtk-cancel");
	gtk_widget_show (cancelbutton);
	gtk_dialog_add_action_widget (GTK_DIALOG (percentage_value_dialog),
				      cancelbutton, GTK_RESPONSE_CANCEL);
	GTK_WIDGET_SET_FLAGS (cancelbutton, GTK_CAN_DEFAULT);
	gtk_widget_add_accelerator (cancelbutton, "clicked", accel_group,
				    GDK_c, GDK_CONTROL_MASK,
				    GTK_ACCEL_VISIBLE);

	okbutton = gtk_button_new_from_stock ("gtk-ok");
	gtk_widget_show (okbutton);
	gtk_dialog_add_action_widget (GTK_DIALOG (percentage_value_dialog),
				      okbutton, GTK_RESPONSE_OK);
	GTK_WIDGET_SET_FLAGS (okbutton, GTK_CAN_DEFAULT);
	gtk_widget_add_accelerator (okbutton, "clicked", accel_group,
				    GDK_o, GDK_CONTROL_MASK,
				    GTK_ACCEL_VISIBLE);
	if (connect_callbacks)
	{
		g_signal_connect ((gpointer) percentage_value_dialog, "close",
				  G_CALLBACK (on_css_dialog_close), data);

		g_signal_connect ((gpointer) helpbutton, "clicked",
				  G_CALLBACK
				  (on_percentage_value_dialog_helpbutton_clicked),
				  NULL);
		g_signal_connect ((gpointer) cancelbutton, "clicked",
				  G_CALLBACK
				  (on_css_dialog_cancel_button_clicked),
				  data);
		g_signal_connect ((gpointer) okbutton, "clicked",
				  G_CALLBACK
				  (on_percentage_value_dialog_okbutton_clicked),
				  data);
	}

	GLADE_HOOKUP_OBJECT_NO_REF (percentage_value_dialog,
				    percentage_value_dialog,
				    "percentage_value_dialog");
	GLADE_HOOKUP_OBJECT_NO_REF (percentage_value_dialog, dialog_vbox,
				    "dialog_vbox");
	GLADE_HOOKUP_OBJECT (percentage_value_dialog, hbox_pc, "hbox_pc");
	GLADE_HOOKUP_OBJECT (percentage_value_dialog, label_property,
			     "label_property");
	GLADE_HOOKUP_OBJECT (percentage_value_dialog, spinbutton_percentage,
			     "spinbutton_percentage");
	GLADE_HOOKUP_OBJECT (percentage_value_dialog, label_pc, "label_pc");
	GLADE_HOOKUP_OBJECT_NO_REF (percentage_value_dialog,
				    dialog_action_area, "dialog_action_area");
	GLADE_HOOKUP_OBJECT (percentage_value_dialog, helpbutton,
			     "helpbutton");
	GLADE_HOOKUP_OBJECT (percentage_value_dialog, cancelbutton,
			     "cancelbutton");
	GLADE_HOOKUP_OBJECT (percentage_value_dialog, okbutton, "okbutton");

	gtk_window_add_accel_group (GTK_WINDOW (percentage_value_dialog),
				    accel_group);

	return percentage_value_dialog;
}

GtkWidget *
create_percentage_length_value_dialog (CssedWindow * window,
				       gchar * previous_string,
				       gboolean add_previous_string,
				       gboolean connect_callbacks)
{
	GtkWidget *percentage_length_value_dialog;
	GtkWidget *dialog_vbox;
	GtkWidget *hbox;
	GtkWidget *label_property;
	GtkObject *spinbutton_perclen_adj;
	GtkWidget *spinbutton_perclen;
	GtkWidget *combo_perclen_units;
	GList *combo_perclen_units_items = NULL;
	GtkWidget *combo_entry_perclen_units;
	GtkWidget *dialog_action_area;
	GtkWidget *helpbutton;
	GtkWidget *cancelbutton;
	GtkWidget *okbutton;
	GtkAccelGroup *accel_group;
	GtkWidget *main_window;
	CssedCssDialogsData *data = NULL;

	if (connect_callbacks)
	{
		data = g_malloc (sizeof (CssedCssDialogsData));
		data->main_window = window;
		data->previous_string = previous_string;
		data->add_property_string = add_previous_string;
	}

	accel_group = gtk_accel_group_new ();

	percentage_length_value_dialog = gtk_dialog_new ();
	gtk_window_set_title (GTK_WINDOW (percentage_length_value_dialog),
			      previous_string);
	gtk_window_set_modal (GTK_WINDOW (percentage_length_value_dialog),
			      TRUE);

	main_window = cssed_window_get_window_widget( window );

	gtk_window_set_transient_for (GTK_WINDOW
				      (percentage_length_value_dialog),
				      GTK_WINDOW (main_window));

	dialog_vbox = GTK_DIALOG (percentage_length_value_dialog)->vbox;
	gtk_widget_show (dialog_vbox);

	hbox = gtk_hbox_new (FALSE, 0);
	gtk_widget_show (hbox);
	gtk_box_pack_start (GTK_BOX (dialog_vbox), hbox, TRUE, TRUE, 0);

	label_property = gtk_label_new (previous_string);
	gtk_widget_show (label_property);
	gtk_box_pack_start (GTK_BOX (hbox), label_property, TRUE, TRUE, 0);
	gtk_label_set_justify (GTK_LABEL (label_property), GTK_JUSTIFY_LEFT);

	spinbutton_perclen_adj =
		gtk_adjustment_new (1, -35565, 35565, 1, 10, 10);
	spinbutton_perclen =
		gtk_spin_button_new (GTK_ADJUSTMENT (spinbutton_perclen_adj),
				     1, 0);
	gtk_widget_show (spinbutton_perclen);
	gtk_box_pack_start (GTK_BOX (hbox), spinbutton_perclen, FALSE, FALSE,
			    5);

	combo_perclen_units = gtk_combo_new ();
	g_object_set_data (G_OBJECT (GTK_COMBO (combo_perclen_units)->popwin),
			   "GladeParentKey", combo_perclen_units);
	gtk_widget_show (combo_perclen_units);
	gtk_box_pack_start (GTK_BOX (hbox), combo_perclen_units, FALSE, FALSE,
			    5);
	gtk_widget_set_size_request (combo_perclen_units, 50, -1);
	combo_perclen_units_items =
		g_list_append (combo_perclen_units_items, (gpointer) "%");
	combo_perclen_units_items =
		g_list_append (combo_perclen_units_items, (gpointer) "em");
	combo_perclen_units_items =
		g_list_append (combo_perclen_units_items, (gpointer) "ex");
	combo_perclen_units_items =
		g_list_append (combo_perclen_units_items, (gpointer) "px");
	combo_perclen_units_items =
		g_list_append (combo_perclen_units_items, (gpointer) "in");
	combo_perclen_units_items =
		g_list_append (combo_perclen_units_items, (gpointer) "cm");
	combo_perclen_units_items =
		g_list_append (combo_perclen_units_items, (gpointer) "mm");
	combo_perclen_units_items =
		g_list_append (combo_perclen_units_items, (gpointer) "pt");
	combo_perclen_units_items =
		g_list_append (combo_perclen_units_items, (gpointer) "pc");


	gtk_combo_set_popdown_strings (GTK_COMBO (combo_perclen_units),
				       combo_perclen_units_items);
	g_list_free (combo_perclen_units_items);

	combo_entry_perclen_units = GTK_COMBO (combo_perclen_units)->entry;
	gtk_widget_show (combo_entry_perclen_units);
	gtk_entry_set_text (GTK_ENTRY (combo_entry_perclen_units), "px");

	dialog_action_area =
		GTK_DIALOG (percentage_length_value_dialog)->action_area;
	gtk_widget_show (dialog_action_area);
	gtk_button_box_set_layout (GTK_BUTTON_BOX (dialog_action_area),
				   GTK_BUTTONBOX_END);

	helpbutton = gtk_button_new_from_stock ("gtk-help");
	gtk_widget_show (helpbutton);
	gtk_dialog_add_action_widget (GTK_DIALOG
				      (percentage_length_value_dialog),
				      helpbutton, GTK_RESPONSE_HELP);
	GTK_WIDGET_SET_FLAGS (helpbutton, GTK_CAN_DEFAULT);
	gtk_widget_add_accelerator (helpbutton, "clicked", accel_group,
				    GDK_y, GDK_CONTROL_MASK,
				    GTK_ACCEL_VISIBLE);
	gtk_widget_add_accelerator (helpbutton, "clicked", accel_group, GDK_h,
				    GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);

	cancelbutton = gtk_button_new_from_stock ("gtk-cancel");
	gtk_widget_show (cancelbutton);
	gtk_dialog_add_action_widget (GTK_DIALOG
				      (percentage_length_value_dialog),
				      cancelbutton, GTK_RESPONSE_CANCEL);
	GTK_WIDGET_SET_FLAGS (cancelbutton, GTK_CAN_DEFAULT);
	gtk_widget_add_accelerator (cancelbutton, "clicked", accel_group,
				    GDK_c, GDK_CONTROL_MASK,
				    GTK_ACCEL_VISIBLE);

	okbutton = gtk_button_new_from_stock ("gtk-ok");
	gtk_widget_show (okbutton);
	gtk_dialog_add_action_widget (GTK_DIALOG
				      (percentage_length_value_dialog),
				      okbutton, GTK_RESPONSE_OK);
	GTK_WIDGET_SET_FLAGS (okbutton, GTK_CAN_DEFAULT);
	gtk_widget_add_accelerator (okbutton, "clicked", accel_group,
				    GDK_o, GDK_CONTROL_MASK,
				    GTK_ACCEL_VISIBLE);

	g_signal_connect ((gpointer) combo_entry_perclen_units,
			  "changed", G_CALLBACK (on_css_dialog_unit_changed),
			  spinbutton_perclen);

	if (connect_callbacks)
	{
		g_signal_connect ((gpointer) percentage_length_value_dialog,
				  "close", G_CALLBACK (on_css_dialog_close),
				  data);

		g_signal_connect ((gpointer) helpbutton, "clicked",
				  G_CALLBACK
				  (on_percentage_length_value_dialog_helpbutton_clicked),
				  NULL);
		g_signal_connect ((gpointer) cancelbutton, "clicked",
				  G_CALLBACK
				  (on_css_dialog_cancel_button_clicked),
				  data);
		g_signal_connect ((gpointer) okbutton, "clicked",
				  G_CALLBACK
				  (on_percentage_length_value_dialog_okbutton_clicked),
				  data);
	}

	GLADE_HOOKUP_OBJECT_NO_REF (percentage_length_value_dialog,
				    percentage_length_value_dialog,
				    "percentage_length_value_dialog");
	GLADE_HOOKUP_OBJECT_NO_REF (percentage_length_value_dialog,
				    dialog_vbox, "dialog_vbox");
	GLADE_HOOKUP_OBJECT (percentage_length_value_dialog, hbox, "hbox");
	GLADE_HOOKUP_OBJECT (percentage_length_value_dialog, label_property,
			     "label_property");
	GLADE_HOOKUP_OBJECT (percentage_length_value_dialog,
			     spinbutton_perclen, "spinbutton_perclen");
	GLADE_HOOKUP_OBJECT (percentage_length_value_dialog,
			     combo_perclen_units, "combo_perclen_units");
	GLADE_HOOKUP_OBJECT (percentage_length_value_dialog,
			     combo_entry_perclen_units,
			     "combo_entry_perclen_units");
	GLADE_HOOKUP_OBJECT_NO_REF (percentage_length_value_dialog,
				    dialog_action_area, "dialog_action_area");
	GLADE_HOOKUP_OBJECT (percentage_length_value_dialog, helpbutton,
			     "helpbutton");
	GLADE_HOOKUP_OBJECT (percentage_length_value_dialog, cancelbutton,
			     "cancelbutton");
	GLADE_HOOKUP_OBJECT (percentage_length_value_dialog, okbutton,
			     "okbutton");

	gtk_window_add_accel_group (GTK_WINDOW
				    (percentage_length_value_dialog),
				    accel_group);

	return percentage_length_value_dialog;
}

GtkWidget *
create_play_during_property_dialog (CssedWindow * window,
				    gchar * previous_string,
				    gboolean add_previous_string,
				    gboolean connect_callbacks)
{
	GtkWidget *play_during_property_dialog;
	GtkWidget *dialog_vbox;
	GtkWidget *vbox;
	GtkWidget *hbox_uri;
	GtkWidget *label_uri;
	GtkWidget *entry_uri;
	GtkWidget *hbox_choices;
	GtkWidget *checkbutton_repeat;
	GtkWidget *checkbutton_mix;
	GtkWidget *dialog_action_area;
	GtkWidget *helpbutton;
	GtkWidget *cancelbutton;
	GtkWidget *okbutton;
	GtkWidget *main_window;
	CssedCssDialogsData *data = NULL;

	if (connect_callbacks)
	{
		data = g_malloc (sizeof (CssedCssDialogsData));
		data->main_window = window;
		data->previous_string = previous_string;
		data->add_property_string = add_previous_string;
	}

	play_during_property_dialog = gtk_dialog_new ();
	gtk_window_set_title (GTK_WINDOW (play_during_property_dialog),
			      previous_string);
	gtk_window_set_modal (GTK_WINDOW (play_during_property_dialog), TRUE);

	main_window = cssed_window_get_window_widget( window );

	gtk_window_set_transient_for (GTK_WINDOW
				      (play_during_property_dialog),
				      GTK_WINDOW (main_window));

	dialog_vbox = GTK_DIALOG (play_during_property_dialog)->vbox;
	gtk_widget_show (dialog_vbox);

	vbox = gtk_vbox_new (TRUE, 0);
	gtk_widget_show (vbox);
	gtk_box_pack_start (GTK_BOX (dialog_vbox), vbox, TRUE, TRUE, 0);

	hbox_uri = gtk_hbox_new (FALSE, 0);
	gtk_widget_show (hbox_uri);
	gtk_box_pack_start (GTK_BOX (vbox), hbox_uri, TRUE, TRUE, 0);

	label_uri = gtk_label_new ("Uri");
	gtk_widget_show (label_uri);
	gtk_box_pack_start (GTK_BOX (hbox_uri), label_uri, FALSE, FALSE, 5);

	entry_uri = gtk_entry_new ();
	gtk_widget_show (entry_uri);
	gtk_box_pack_start (GTK_BOX (hbox_uri), entry_uri, TRUE, TRUE, 5);

	hbox_choices = gtk_hbox_new (TRUE, 0);
	gtk_widget_show (hbox_choices);
	gtk_box_pack_start (GTK_BOX (vbox), hbox_choices, TRUE, TRUE, 0);

	checkbutton_repeat = gtk_check_button_new_with_mnemonic ("repeat");
	gtk_widget_show (checkbutton_repeat);
	gtk_box_pack_start (GTK_BOX (hbox_choices), checkbutton_repeat, FALSE,
			    TRUE, 5);

	checkbutton_mix = gtk_check_button_new_with_mnemonic ("mix");
	gtk_widget_show (checkbutton_mix);
	gtk_box_pack_start (GTK_BOX (hbox_choices), checkbutton_mix, FALSE,
			    TRUE, 5);

	dialog_action_area =
		GTK_DIALOG (play_during_property_dialog)->action_area;
	gtk_widget_show (dialog_action_area);
	gtk_button_box_set_layout (GTK_BUTTON_BOX (dialog_action_area),
				   GTK_BUTTONBOX_END);

	helpbutton = gtk_button_new_from_stock ("gtk-help");
	gtk_widget_show (helpbutton);
	gtk_dialog_add_action_widget (GTK_DIALOG
				      (play_during_property_dialog),
				      helpbutton, GTK_RESPONSE_HELP);
	GTK_WIDGET_SET_FLAGS (helpbutton, GTK_CAN_DEFAULT);

	cancelbutton = gtk_button_new_from_stock ("gtk-cancel");
	gtk_widget_show (cancelbutton);
	gtk_dialog_add_action_widget (GTK_DIALOG
				      (play_during_property_dialog),
				      cancelbutton, GTK_RESPONSE_CANCEL);
	GTK_WIDGET_SET_FLAGS (cancelbutton, GTK_CAN_DEFAULT);

	okbutton = gtk_button_new_from_stock ("gtk-ok");
	gtk_widget_show (okbutton);
	gtk_dialog_add_action_widget (GTK_DIALOG
				      (play_during_property_dialog), okbutton,
				      GTK_RESPONSE_OK);
	GTK_WIDGET_SET_FLAGS (okbutton, GTK_CAN_DEFAULT);
	if (connect_callbacks)
	{
		g_signal_connect ((gpointer) play_during_property_dialog,
				  "close", G_CALLBACK (on_css_dialog_close),
				  data);

		g_signal_connect ((gpointer) helpbutton, "clicked",
				  G_CALLBACK
				  (on_play_during_property_dialog_helpbutton_clicked),
				  NULL);
		g_signal_connect ((gpointer) cancelbutton, "clicked",
				  G_CALLBACK
				  (on_css_dialog_cancel_button_clicked),
				  data);
		g_signal_connect ((gpointer) okbutton, "clicked",
				  G_CALLBACK
				  (on_play_during_property_dialog_okbutton_clicked),
				  data);
	}

	GLADE_HOOKUP_OBJECT_NO_REF (play_during_property_dialog,
				    play_during_property_dialog,
				    "play_during_property_dialog");
	GLADE_HOOKUP_OBJECT_NO_REF (play_during_property_dialog, dialog_vbox,
				    "dialog_vbox");
	GLADE_HOOKUP_OBJECT (play_during_property_dialog, vbox, "vbox");
	GLADE_HOOKUP_OBJECT (play_during_property_dialog, hbox_uri,
			     "hbox_uri");
	GLADE_HOOKUP_OBJECT (play_during_property_dialog, label_uri,
			     "label_uri");
	GLADE_HOOKUP_OBJECT (play_during_property_dialog, entry_uri,
			     "entry_uri");
	GLADE_HOOKUP_OBJECT (play_during_property_dialog, hbox_choices,
			     "hbox_choices");
	GLADE_HOOKUP_OBJECT (play_during_property_dialog, checkbutton_repeat,
			     "checkbutton_repeat");
	GLADE_HOOKUP_OBJECT (play_during_property_dialog, checkbutton_mix,
			     "checkbutton_mix");
	GLADE_HOOKUP_OBJECT_NO_REF (play_during_property_dialog,
				    dialog_action_area, "dialog_action_area");
	GLADE_HOOKUP_OBJECT (play_during_property_dialog, helpbutton,
			     "helpbutton");
	GLADE_HOOKUP_OBJECT (play_during_property_dialog, cancelbutton,
			     "cancelbutton");
	GLADE_HOOKUP_OBJECT (play_during_property_dialog, okbutton,
			     "okbutton");

	return play_during_property_dialog;
}

GtkWidget *
create_quotes_property_dialog (CssedWindow * window,
			       gchar * previous_string,
			       gboolean add_previous_string,
			       gboolean connect_callbacks)
{
	GtkWidget *quotes_property_dialog;
	GtkWidget *dialog_vbox;
	GtkWidget *vbox;
	GtkWidget *hbox_open_close_and_buttons;
	GtkWidget *hbox_open;
	GtkWidget *label_open;
	GtkWidget *entry_open;
	GtkWidget *hbox_close;
	GtkWidget *label_close;
	GtkWidget *entry_close;
	GtkWidget *hbuttonbox;
	GtkWidget *button_quotes_property_dialog_add;
	GtkWidget *button_quotes_property_dialog_del;
	GtkWidget *scrolledwindow;
	GtkWidget *treeview;
	GtkWidget *dialog_action_area;
	GtkWidget *helpbutton;
	GtkWidget *cancelbutton;
	GtkWidget *okbutton;
	GtkAccelGroup *accel_group;
	GtkWidget *main_window;
	GtkTreeViewColumn *col_open_quote;
	GtkTreeViewColumn *col_close_quote;
	GtkCellRenderer *oq_renderer;
	GtkCellRenderer *cq_renderer;
	GtkListStore *store;
	CssedCssDialogsData *data = NULL;

	if (connect_callbacks)
	{
		data = g_malloc (sizeof (CssedCssDialogsData));
		data->main_window = window;
		data->previous_string = previous_string;
		data->add_property_string = add_previous_string;
	}

	accel_group = gtk_accel_group_new ();

	quotes_property_dialog = gtk_dialog_new ();
	gtk_window_set_title (GTK_WINDOW (quotes_property_dialog),
			      previous_string);
	gtk_window_set_modal (GTK_WINDOW (quotes_property_dialog), TRUE);

	main_window = cssed_window_get_window_widget( window );

	gtk_window_set_transient_for (GTK_WINDOW (quotes_property_dialog),
				      GTK_WINDOW (main_window));

	dialog_vbox = GTK_DIALOG (quotes_property_dialog)->vbox;
	gtk_widget_show (dialog_vbox);

	vbox = gtk_vbox_new (FALSE, 0);
	gtk_widget_show (vbox);
	gtk_box_pack_start (GTK_BOX (dialog_vbox), vbox, TRUE, TRUE, 0);

	hbox_open_close_and_buttons = gtk_hbox_new (FALSE, 0);
	gtk_widget_show (hbox_open_close_and_buttons);
	gtk_box_pack_start (GTK_BOX (vbox), hbox_open_close_and_buttons,
			    FALSE, TRUE, 5);

	hbox_open = gtk_hbox_new (TRUE, 0);
	gtk_widget_show (hbox_open);
	gtk_box_pack_start (GTK_BOX (hbox_open_close_and_buttons), hbox_open,
			    TRUE, TRUE, 0);

	label_open = gtk_label_new (_("open"));
	gtk_widget_show (label_open);
	gtk_box_pack_start (GTK_BOX (hbox_open), label_open, TRUE, TRUE, 5);
	gtk_label_set_justify (GTK_LABEL (label_open), GTK_JUSTIFY_LEFT);

	entry_open = gtk_entry_new ();
	gtk_widget_show (entry_open);
	gtk_box_pack_start (GTK_BOX (hbox_open), entry_open, FALSE, FALSE, 0);
	gtk_widget_set_size_request (entry_open, 25, -1);
	gtk_entry_set_max_length (GTK_ENTRY (entry_open), 1);

	hbox_close = gtk_hbox_new (TRUE, 0);
	gtk_widget_show (hbox_close);
	gtk_box_pack_start (GTK_BOX (hbox_open_close_and_buttons), hbox_close,
			    TRUE, TRUE, 0);

	label_close = gtk_label_new (_("close"));
	gtk_widget_show (label_close);
	gtk_box_pack_start (GTK_BOX (hbox_close), label_close, TRUE, TRUE, 0);
	gtk_label_set_justify (GTK_LABEL (label_close), GTK_JUSTIFY_LEFT);

	entry_close = gtk_entry_new ();
	gtk_widget_show (entry_close);
	gtk_box_pack_start (GTK_BOX (hbox_close), entry_close, FALSE, FALSE,
			    0);
	gtk_widget_set_size_request (entry_close, 25, -1);
	gtk_entry_set_max_length (GTK_ENTRY (entry_close), 1);

	hbuttonbox = gtk_hbutton_box_new ();
	gtk_widget_show (hbuttonbox);
	gtk_box_pack_start (GTK_BOX (hbox_open_close_and_buttons), hbuttonbox,
			    TRUE, TRUE, 5);

	button_quotes_property_dialog_add =
		gtk_button_new_from_stock ("gtk-add");
	gtk_widget_show (button_quotes_property_dialog_add);
	gtk_container_add (GTK_CONTAINER (hbuttonbox),
			   button_quotes_property_dialog_add);
	GTK_WIDGET_SET_FLAGS (button_quotes_property_dialog_add,
			      GTK_CAN_DEFAULT);

	button_quotes_property_dialog_del =
		gtk_button_new_from_stock ("gtk-remove");
	gtk_widget_show (button_quotes_property_dialog_del);
	gtk_container_add (GTK_CONTAINER (hbuttonbox),
			   button_quotes_property_dialog_del);
	GTK_WIDGET_SET_FLAGS (button_quotes_property_dialog_del,
			      GTK_CAN_DEFAULT);

	scrolledwindow = gtk_scrolled_window_new (NULL, NULL);
	gtk_widget_show (scrolledwindow);
	gtk_box_pack_start (GTK_BOX (vbox), scrolledwindow, TRUE, TRUE, 0);
	gtk_container_set_border_width (GTK_CONTAINER (scrolledwindow), 5);
	gtk_widget_set_size_request (scrolledwindow, -1, 150);

	store = gtk_list_store_new (2, G_TYPE_STRING, G_TYPE_STRING);
	treeview = gtk_tree_view_new_with_model (GTK_TREE_MODEL (store));

	oq_renderer = gtk_cell_renderer_text_new ();
	cq_renderer = gtk_cell_renderer_text_new ();

	col_open_quote =
		gtk_tree_view_column_new_with_attributes (_("Open quote"),
							  oq_renderer, "text",
							  0, NULL);
	col_close_quote =
		gtk_tree_view_column_new_with_attributes (_("Close quote"),
							  cq_renderer, "text",
							  1, NULL);
	gtk_tree_view_insert_column (GTK_TREE_VIEW (treeview), col_open_quote,
				     0);
	gtk_tree_view_insert_column (GTK_TREE_VIEW (treeview),
				     col_close_quote, 1);

	gtk_widget_show (treeview);
	gtk_container_add (GTK_CONTAINER (scrolledwindow), treeview);
	gtk_tree_view_set_headers_visible (GTK_TREE_VIEW (treeview), TRUE);

	dialog_action_area = GTK_DIALOG (quotes_property_dialog)->action_area;
	gtk_widget_show (dialog_action_area);
	gtk_button_box_set_layout (GTK_BUTTON_BOX (dialog_action_area),
				   GTK_BUTTONBOX_END);

	helpbutton = gtk_button_new_from_stock ("gtk-help");
	gtk_widget_show (helpbutton);
	gtk_dialog_add_action_widget (GTK_DIALOG (quotes_property_dialog),
				      helpbutton, GTK_RESPONSE_HELP);
	GTK_WIDGET_SET_FLAGS (helpbutton, GTK_CAN_DEFAULT);
	gtk_widget_add_accelerator (helpbutton, "clicked", accel_group,
				    GDK_y, GDK_CONTROL_MASK,
				    GTK_ACCEL_VISIBLE);
	gtk_widget_add_accelerator (helpbutton, "clicked", accel_group, GDK_h,
				    GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);

	cancelbutton = gtk_button_new_from_stock ("gtk-cancel");
	gtk_widget_show (cancelbutton);
	gtk_dialog_add_action_widget (GTK_DIALOG (quotes_property_dialog),
				      cancelbutton, GTK_RESPONSE_CANCEL);
	GTK_WIDGET_SET_FLAGS (cancelbutton, GTK_CAN_DEFAULT);

	okbutton = gtk_button_new_from_stock ("gtk-ok");
	gtk_widget_show (okbutton);
	gtk_dialog_add_action_widget (GTK_DIALOG (quotes_property_dialog),
				      okbutton, GTK_RESPONSE_OK);
	GTK_WIDGET_SET_FLAGS (okbutton, GTK_CAN_DEFAULT);
	gtk_widget_add_accelerator (okbutton, "clicked", accel_group,
				    GDK_o, GDK_CONTROL_MASK,
				    GTK_ACCEL_VISIBLE);
	if (connect_callbacks)
	{
		g_signal_connect ((gpointer) quotes_property_dialog, "close",
				  G_CALLBACK (on_css_dialog_close), data);

		g_signal_connect ((gpointer) helpbutton, "clicked",
				  G_CALLBACK
				  (on_quotes_property_dialog_helpbutton_clicked),
				  NULL);
		g_signal_connect ((gpointer) cancelbutton, "clicked",
				  G_CALLBACK
				  (on_css_dialog_cancel_button_clicked),
				  data);
		g_signal_connect ((gpointer) okbutton, "clicked",
				  G_CALLBACK
				  (on_quotes_property_dialog_okbutton_clicked),
				  data);
	}
	g_signal_connect ((gpointer) button_quotes_property_dialog_add,
			  "clicked",
			  G_CALLBACK
			  (on_button_quotes_property_dialog_add_clicked),
			  store);
	g_signal_connect ((gpointer) button_quotes_property_dialog_del,
			  "clicked",
			  G_CALLBACK
			  (on_button_quotes_property_dialog_del_clicked),
			  store);

	GLADE_HOOKUP_OBJECT_NO_REF (quotes_property_dialog,
				    quotes_property_dialog,
				    "quotes_property_dialog");
	GLADE_HOOKUP_OBJECT_NO_REF (quotes_property_dialog, dialog_vbox,
				    "dialog_vbox");
	GLADE_HOOKUP_OBJECT (quotes_property_dialog, vbox, "vbox");
	GLADE_HOOKUP_OBJECT (quotes_property_dialog,
			     hbox_open_close_and_buttons,
			     "hbox_open_close_and_buttons");
	GLADE_HOOKUP_OBJECT (quotes_property_dialog, hbox_open, "hbox_open");
	GLADE_HOOKUP_OBJECT (quotes_property_dialog, label_open,
			     "label_open");
	GLADE_HOOKUP_OBJECT (quotes_property_dialog, entry_open,
			     "entry_open");
	GLADE_HOOKUP_OBJECT (quotes_property_dialog, hbox_close,
			     "hbox_close");
	GLADE_HOOKUP_OBJECT (quotes_property_dialog, label_close,
			     "label_close");
	GLADE_HOOKUP_OBJECT (quotes_property_dialog, entry_close,
			     "entry_close");
	GLADE_HOOKUP_OBJECT (quotes_property_dialog, hbuttonbox,
			     "hbuttonbox");
	GLADE_HOOKUP_OBJECT (quotes_property_dialog,
			     button_quotes_property_dialog_add,
			     "button_quotes_property_dialog_add");
	GLADE_HOOKUP_OBJECT (quotes_property_dialog,
			     button_quotes_property_dialog_del,
			     "button_quotes_property_dialog_del");
	GLADE_HOOKUP_OBJECT (quotes_property_dialog, scrolledwindow,
			     "scrolledwindow");
	GLADE_HOOKUP_OBJECT (quotes_property_dialog, treeview, "treeview");
	GLADE_HOOKUP_OBJECT_NO_REF (quotes_property_dialog,
				    dialog_action_area, "dialog_action_area");
	GLADE_HOOKUP_OBJECT (quotes_property_dialog, helpbutton,
			     "helpbutton");
	GLADE_HOOKUP_OBJECT (quotes_property_dialog, cancelbutton,
			     "cancelbutton");
	GLADE_HOOKUP_OBJECT (quotes_property_dialog, okbutton, "okbutton");

	gtk_window_add_accel_group (GTK_WINDOW (quotes_property_dialog),
				    accel_group);

	return quotes_property_dialog;
}

GtkWidget *
create_shape_value_dialog (CssedWindow * window,
			   gchar * previous_string,
			   gboolean add_previous_string,
			   gboolean connect_callbacks)
{
	GtkWidget *shape_value_dialog;
	GtkWidget *dialog_vbox;
	GtkWidget *vbox;
	GtkWidget *hbox_top;
	GtkWidget *label_top;
	GtkObject *spinbutton_top_adj;
	GtkWidget *spinbutton_top;
	GtkWidget *combo_top;
	GList *combo_top_items = NULL;
	GtkWidget *combo_entry_top;
	GtkWidget *hbox_right;
	GtkWidget *label_right;
	GtkObject *spinbutton_right_adj;
	GtkWidget *spinbutton_right;
	GtkWidget *combo_right;
	GList *combo_right_items = NULL;
	GtkWidget *combo_entry_right;
	GtkWidget *hbox_bottom;
	GtkWidget *label_bottom;
	GtkObject *spinbutton_bottom_adj;
	GtkWidget *spinbutton_bottom;
	GtkWidget *combo_bottom;
	GList *combo_bottom_items = NULL;
	GtkWidget *combo_entry_bottom;
	GtkWidget *hbox_left;
	GtkWidget *label_left;
	GtkObject *spinbutton_left_adj;
	GtkWidget *spinbutton_left;
	GtkWidget *combo_left;
	GList *combo_left_items = NULL;
	GtkWidget *combo_entry_left;
	GtkWidget *dialog_action_area;
	GtkWidget *helpbutton;
	GtkWidget *cancelbutton;
	GtkWidget *okbutton;
	GtkAccelGroup *accel_group;
	GtkWidget *main_window;
	CssedCssDialogsData *data = NULL;

	if (connect_callbacks)
	{
		data = g_malloc (sizeof (CssedCssDialogsData));
		data->main_window = window;
		data->previous_string = previous_string;
		data->add_property_string = add_previous_string;
	}

	accel_group = gtk_accel_group_new ();

	shape_value_dialog = gtk_dialog_new ();
	gtk_window_set_title (GTK_WINDOW (shape_value_dialog),
			      previous_string);
	gtk_window_set_modal (GTK_WINDOW (shape_value_dialog), TRUE);

	main_window = cssed_window_get_window_widget( window );

	gtk_window_set_transient_for (GTK_WINDOW (shape_value_dialog),
				      GTK_WINDOW (main_window));

	dialog_vbox = GTK_DIALOG (shape_value_dialog)->vbox;
	gtk_widget_show (dialog_vbox);

	vbox = gtk_vbox_new (FALSE, 0);
	gtk_widget_show (vbox);
	gtk_box_pack_start (GTK_BOX (dialog_vbox), vbox, TRUE, TRUE, 0);

	hbox_top = gtk_hbox_new (TRUE, 0);
	gtk_widget_show (hbox_top);
	gtk_box_pack_start (GTK_BOX (vbox), hbox_top, TRUE, TRUE, 0);

	label_top = gtk_label_new (_("Top"));
	gtk_widget_show (label_top);
	gtk_box_pack_start (GTK_BOX (hbox_top), label_top, TRUE, TRUE, 5);
	gtk_label_set_justify (GTK_LABEL (label_top), GTK_JUSTIFY_LEFT);

	spinbutton_top_adj = gtk_adjustment_new (1, -35565, 35565, 1, 10, 10);
	spinbutton_top =
		gtk_spin_button_new (GTK_ADJUSTMENT (spinbutton_top_adj), 1,
				     0);
	gtk_widget_show (spinbutton_top);
	gtk_box_pack_start (GTK_BOX (hbox_top), spinbutton_top, TRUE, FALSE,
			    0);

	combo_top = gtk_combo_new ();
	g_object_set_data (G_OBJECT (GTK_COMBO (combo_top)->popwin),
			   "GladeParentKey", combo_top);
	gtk_widget_show (combo_top);
	gtk_box_pack_start (GTK_BOX (hbox_top), combo_top, FALSE, FALSE, 5);
	combo_top_items = g_list_append (combo_top_items, (gpointer) "em");
	combo_top_items = g_list_append (combo_top_items, (gpointer) "ex");
	combo_top_items = g_list_append (combo_top_items, (gpointer) "px");
	combo_top_items = g_list_append (combo_top_items, (gpointer) "in");
	combo_top_items = g_list_append (combo_top_items, (gpointer) "cm");
	combo_top_items = g_list_append (combo_top_items, (gpointer) "mm");
	combo_top_items = g_list_append (combo_top_items, (gpointer) "pt");
	combo_top_items = g_list_append (combo_top_items, (gpointer) "pc");

	gtk_combo_set_popdown_strings (GTK_COMBO (combo_top),
				       combo_top_items);
	g_list_free (combo_top_items);

	combo_entry_top = GTK_COMBO (combo_top)->entry;
	gtk_widget_show (combo_entry_top);
	gtk_widget_set_size_request (combo_entry_top, 50, -1);
	gtk_entry_set_text (GTK_ENTRY (combo_entry_top), "px");

	hbox_right = gtk_hbox_new (TRUE, 0);
	gtk_widget_show (hbox_right);
	gtk_box_pack_start (GTK_BOX (vbox), hbox_right, TRUE, TRUE, 5);

	label_right = gtk_label_new (_("Right"));
	gtk_widget_show (label_right);
	gtk_box_pack_start (GTK_BOX (hbox_right), label_right, TRUE, TRUE, 5);
	gtk_label_set_justify (GTK_LABEL (label_right), GTK_JUSTIFY_LEFT);

	spinbutton_right_adj =
		gtk_adjustment_new (1, -35565, 35565, 1, 10, 10);
	spinbutton_right =
		gtk_spin_button_new (GTK_ADJUSTMENT (spinbutton_right_adj), 1,
				     0);
	gtk_widget_show (spinbutton_right);
	gtk_box_pack_start (GTK_BOX (hbox_right), spinbutton_right, TRUE,
			    FALSE, 0);

	combo_right = gtk_combo_new ();
	g_object_set_data (G_OBJECT (GTK_COMBO (combo_right)->popwin),
			   "GladeParentKey", combo_right);
	gtk_widget_show (combo_right);
	gtk_box_pack_start (GTK_BOX (hbox_right), combo_right, FALSE, FALSE,
			    5);
	combo_right_items =
		g_list_append (combo_right_items, (gpointer) "em");
	combo_right_items =
		g_list_append (combo_right_items, (gpointer) "ex");
	combo_right_items =
		g_list_append (combo_right_items, (gpointer) "px");
	combo_right_items =
		g_list_append (combo_right_items, (gpointer) "in");
	combo_right_items =
		g_list_append (combo_right_items, (gpointer) "cm");
	combo_right_items =
		g_list_append (combo_right_items, (gpointer) "mm");
	combo_right_items =
		g_list_append (combo_right_items, (gpointer) "pt");
	combo_right_items =
		g_list_append (combo_right_items, (gpointer) "pc");

	gtk_combo_set_popdown_strings (GTK_COMBO (combo_right),
				       combo_right_items);
	g_list_free (combo_right_items);

	combo_entry_right = GTK_COMBO (combo_right)->entry;
	gtk_widget_show (combo_entry_right);
	gtk_widget_set_size_request (combo_entry_right, 50, -1);
	gtk_entry_set_text (GTK_ENTRY (combo_entry_right), "px");

	hbox_bottom = gtk_hbox_new (TRUE, 0);
	gtk_widget_show (hbox_bottom);
	gtk_box_pack_start (GTK_BOX (vbox), hbox_bottom, TRUE, TRUE, 0);

	label_bottom = gtk_label_new (_("Bottom"));
	gtk_widget_show (label_bottom);
	gtk_box_pack_start (GTK_BOX (hbox_bottom), label_bottom, TRUE, TRUE,
			    5);
	gtk_label_set_justify (GTK_LABEL (label_bottom), GTK_JUSTIFY_LEFT);

	spinbutton_bottom_adj =
		gtk_adjustment_new (1, -35565, 35565, 1, 10, 10);
	spinbutton_bottom =
		gtk_spin_button_new (GTK_ADJUSTMENT (spinbutton_bottom_adj),
				     1, 0);
	gtk_widget_show (spinbutton_bottom);
	gtk_box_pack_start (GTK_BOX (hbox_bottom), spinbutton_bottom, TRUE,
			    FALSE, 0);

	combo_bottom = gtk_combo_new ();
	g_object_set_data (G_OBJECT (GTK_COMBO (combo_bottom)->popwin),
			   "GladeParentKey", combo_bottom);
	gtk_widget_show (combo_bottom);
	gtk_box_pack_start (GTK_BOX (hbox_bottom), combo_bottom, FALSE, FALSE,
			    5);
	combo_bottom_items =
		g_list_append (combo_bottom_items, (gpointer) "em");
	combo_bottom_items =
		g_list_append (combo_bottom_items, (gpointer) "ex");
	combo_bottom_items =
		g_list_append (combo_bottom_items, (gpointer) "px");
	combo_bottom_items =
		g_list_append (combo_bottom_items, (gpointer) "in");
	combo_bottom_items =
		g_list_append (combo_bottom_items, (gpointer) "cm");
	combo_bottom_items =
		g_list_append (combo_bottom_items, (gpointer) "mm");
	combo_bottom_items =
		g_list_append (combo_bottom_items, (gpointer) "pt");
	combo_bottom_items =
		g_list_append (combo_bottom_items, (gpointer) "pc");


	gtk_combo_set_popdown_strings (GTK_COMBO (combo_bottom),
				       combo_bottom_items);
	g_list_free (combo_bottom_items);

	combo_entry_bottom = GTK_COMBO (combo_bottom)->entry;
	gtk_widget_show (combo_entry_bottom);
	gtk_widget_set_size_request (combo_entry_bottom, 50, -1);
	gtk_entry_set_text (GTK_ENTRY (combo_entry_bottom), "px");

	hbox_left = gtk_hbox_new (TRUE, 0);
	gtk_widget_show (hbox_left);
	gtk_box_pack_start (GTK_BOX (vbox), hbox_left, TRUE, TRUE, 5);

	label_left = gtk_label_new (_("Left"));
	gtk_widget_show (label_left);
	gtk_box_pack_start (GTK_BOX (hbox_left), label_left, TRUE, TRUE, 5);
	gtk_label_set_justify (GTK_LABEL (label_left), GTK_JUSTIFY_LEFT);

	spinbutton_left_adj =
		gtk_adjustment_new (1, -35565, 35565, 1, 10, 10);
	spinbutton_left =
		gtk_spin_button_new (GTK_ADJUSTMENT (spinbutton_left_adj), 1,
				     0);
	gtk_widget_show (spinbutton_left);
	gtk_box_pack_start (GTK_BOX (hbox_left), spinbutton_left, TRUE, FALSE,
			    0);

	combo_left = gtk_combo_new ();
	g_object_set_data (G_OBJECT (GTK_COMBO (combo_left)->popwin),
			   "GladeParentKey", combo_left);
	gtk_widget_show (combo_left);
	gtk_box_pack_start (GTK_BOX (hbox_left), combo_left, FALSE, FALSE, 5);
	combo_left_items = g_list_append (combo_left_items, (gpointer) "em");
	combo_left_items = g_list_append (combo_left_items, (gpointer) "ex");
	combo_left_items = g_list_append (combo_left_items, (gpointer) "px");
	combo_left_items = g_list_append (combo_left_items, (gpointer) "in");
	combo_left_items = g_list_append (combo_left_items, (gpointer) "cm");
	combo_left_items = g_list_append (combo_left_items, (gpointer) "mm");
	combo_left_items = g_list_append (combo_left_items, (gpointer) "pt");
	combo_left_items = g_list_append (combo_left_items, (gpointer) "pc");

	gtk_combo_set_popdown_strings (GTK_COMBO (combo_left),
				       combo_left_items);
	g_list_free (combo_left_items);

	combo_entry_left = GTK_COMBO (combo_left)->entry;
	gtk_widget_show (combo_entry_left);
	gtk_widget_set_size_request (combo_entry_left, 50, -1);
	gtk_entry_set_text (GTK_ENTRY (combo_entry_left), "px");

	dialog_action_area = GTK_DIALOG (shape_value_dialog)->action_area;
	gtk_widget_show (dialog_action_area);
	gtk_button_box_set_layout (GTK_BUTTON_BOX (dialog_action_area),
				   GTK_BUTTONBOX_END);

	helpbutton = gtk_button_new_from_stock ("gtk-help");
	gtk_widget_show (helpbutton);
	gtk_dialog_add_action_widget (GTK_DIALOG (shape_value_dialog),
				      helpbutton, GTK_RESPONSE_HELP);
	GTK_WIDGET_SET_FLAGS (helpbutton, GTK_CAN_DEFAULT);
	gtk_widget_add_accelerator (helpbutton, "clicked", accel_group,
				    GDK_y, GDK_CONTROL_MASK,
				    GTK_ACCEL_VISIBLE);
	gtk_widget_add_accelerator (helpbutton, "clicked", accel_group, GDK_h,
				    GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);

	cancelbutton = gtk_button_new_from_stock ("gtk-cancel");
	gtk_widget_show (cancelbutton);
	gtk_dialog_add_action_widget (GTK_DIALOG (shape_value_dialog),
				      cancelbutton, GTK_RESPONSE_CANCEL);
	GTK_WIDGET_SET_FLAGS (cancelbutton, GTK_CAN_DEFAULT);
	gtk_widget_add_accelerator (cancelbutton, "clicked", accel_group,
				    GDK_c, GDK_CONTROL_MASK,
				    GTK_ACCEL_VISIBLE);

	okbutton = gtk_button_new_from_stock ("gtk-ok");
	gtk_widget_show (okbutton);
	gtk_dialog_add_action_widget (GTK_DIALOG (shape_value_dialog),
				      okbutton, GTK_RESPONSE_OK);
	GTK_WIDGET_SET_FLAGS (okbutton, GTK_CAN_DEFAULT);
	gtk_widget_add_accelerator (okbutton, "clicked", accel_group,
				    GDK_o, GDK_CONTROL_MASK,
				    GTK_ACCEL_VISIBLE);

	g_signal_connect ((gpointer) combo_entry_top,
			  "changed", G_CALLBACK (on_css_dialog_unit_changed),
			  spinbutton_top);
	g_signal_connect ((gpointer) combo_entry_right,
			  "changed", G_CALLBACK (on_css_dialog_unit_changed),
			  spinbutton_right);
	g_signal_connect ((gpointer) combo_entry_bottom,
			  "changed", G_CALLBACK (on_css_dialog_unit_changed),
			  spinbutton_bottom);
	g_signal_connect ((gpointer) combo_entry_left,
			  "changed", G_CALLBACK (on_css_dialog_unit_changed),
			  spinbutton_left);

	if (connect_callbacks)
	{
		g_signal_connect ((gpointer) shape_value_dialog, "close",
				  G_CALLBACK (on_css_dialog_close), data);

		g_signal_connect ((gpointer) helpbutton, "clicked",
				  G_CALLBACK
				  (on_shape_value_dialog_helpbutton_clicked),
				  NULL);
		g_signal_connect ((gpointer) cancelbutton, "clicked",
				  G_CALLBACK
				  (on_css_dialog_cancel_button_clicked),
				  data);
		g_signal_connect ((gpointer) okbutton, "clicked",
				  G_CALLBACK
				  (on_shape_value_dialog_okbutton_clicked),
				  data);
	}

	GLADE_HOOKUP_OBJECT_NO_REF (shape_value_dialog, shape_value_dialog,
				    "shape_value_dialog");
	GLADE_HOOKUP_OBJECT (shape_value_dialog, label_top, "label_top");
	GLADE_HOOKUP_OBJECT (shape_value_dialog, spinbutton_top,
			     "spinbutton_top");
	GLADE_HOOKUP_OBJECT (shape_value_dialog, combo_top, "combo_top");
	GLADE_HOOKUP_OBJECT (shape_value_dialog, combo_entry_top,
			     "combo_entry_top");
	GLADE_HOOKUP_OBJECT (shape_value_dialog, label_right, "label_right");
	GLADE_HOOKUP_OBJECT (shape_value_dialog, spinbutton_right,
			     "spinbutton_right");
	GLADE_HOOKUP_OBJECT (shape_value_dialog, combo_right, "combo_right");
	GLADE_HOOKUP_OBJECT (shape_value_dialog, combo_entry_right,
			     "combo_entry_right");
	GLADE_HOOKUP_OBJECT (shape_value_dialog, hbox_bottom, "hbox_bottom");
	GLADE_HOOKUP_OBJECT (shape_value_dialog, label_bottom,
			     "label_bottom");
	GLADE_HOOKUP_OBJECT (shape_value_dialog, spinbutton_bottom,
			     "spinbutton_bottom");
	GLADE_HOOKUP_OBJECT (shape_value_dialog, combo_bottom,
			     "combo_bottom");
	GLADE_HOOKUP_OBJECT (shape_value_dialog, combo_entry_bottom,
			     "combo_entry_bottom");
	GLADE_HOOKUP_OBJECT (shape_value_dialog, label_left, "label_left");
	GLADE_HOOKUP_OBJECT (shape_value_dialog, spinbutton_left,
			     "spinbutton_left");
	GLADE_HOOKUP_OBJECT (shape_value_dialog, combo_left, "combo_left");
	GLADE_HOOKUP_OBJECT (shape_value_dialog, combo_entry_left,
			     "combo_entry_left");
	GLADE_HOOKUP_OBJECT_NO_REF (shape_value_dialog, dialog_action_area,
				    "dialog_action_area");
	GLADE_HOOKUP_OBJECT (shape_value_dialog, helpbutton, "helpbutton");
	GLADE_HOOKUP_OBJECT (shape_value_dialog, cancelbutton,
			     "cancelbutton");
	GLADE_HOOKUP_OBJECT (shape_value_dialog, okbutton, "okbutton");

	gtk_window_add_accel_group (GTK_WINDOW (shape_value_dialog),
				    accel_group);

	return shape_value_dialog;
}

GtkWidget*
create_size_length_value_dialog (CssedWindow * window,
								 gchar * previous_string,
								 gboolean add_previous_string,
								 gboolean connect_callbacks)
{
	GtkWidget *size_length_value_dialog;
	GtkWidget *dialog_vbox;
	GtkWidget *vbox;
	GtkWidget *hbox_width;
	GtkWidget *label_width;
	GtkObject *spinbutton_width_adj;
	GtkWidget *spinbutton_width;
	GtkWidget *combo_width;
	GList *combo_width_items = NULL;
	GtkWidget *combo_entry_width;
	GtkWidget *hbox_height;
	GtkWidget *label_height;
	GtkObject *spinbutton_height_adj;
	GtkWidget *spinbutton_height;
	GtkWidget *combo_height;
	GList *combo_height_items = NULL;
	GtkWidget *combo_entry_height;
	GtkWidget *dialog_action_area;
	GtkWidget *helpbutton;
	GtkWidget *cancelbutton;
	GtkWidget *okbutton;
	GtkAccelGroup *accel_group;
	GtkWidget *main_window;
	CssedCssDialogsData *data = NULL;

	if (connect_callbacks)
	{
		data = g_malloc (sizeof (CssedCssDialogsData));
		data->main_window = window;
		data->previous_string = previous_string;
		data->add_property_string = add_previous_string;
	}

	accel_group = gtk_accel_group_new ();

	size_length_value_dialog = gtk_dialog_new ();
	gtk_window_set_title (GTK_WINDOW (size_length_value_dialog),
			      previous_string);
	gtk_window_set_modal (GTK_WINDOW (size_length_value_dialog), TRUE);

	main_window = cssed_window_get_window_widget( window );

	gtk_window_set_transient_for (GTK_WINDOW (size_length_value_dialog),
				      GTK_WINDOW (main_window));

	dialog_vbox = GTK_DIALOG (size_length_value_dialog)->vbox;
	gtk_widget_show (dialog_vbox);

	vbox = gtk_vbox_new (FALSE, 0);
	gtk_widget_show (vbox);
	gtk_box_pack_start (GTK_BOX (dialog_vbox), vbox, TRUE, TRUE, 0);

	hbox_width = gtk_hbox_new (TRUE, 0);
	gtk_widget_show (hbox_width);
	gtk_box_pack_start (GTK_BOX (vbox), hbox_width, TRUE, TRUE, 0);

	label_width = gtk_label_new (_("Width"));
	gtk_widget_show (label_width);
	gtk_box_pack_start (GTK_BOX (hbox_width), label_width, TRUE, TRUE, 0);
	gtk_label_set_justify (GTK_LABEL (label_width), GTK_JUSTIFY_LEFT);

	spinbutton_width_adj =
		gtk_adjustment_new (1, -35565, 35565, 1, 10, 10);
	spinbutton_width =
		gtk_spin_button_new (GTK_ADJUSTMENT (spinbutton_width_adj), 1,
				     0);
	gtk_widget_show (spinbutton_width);
	gtk_box_pack_start (GTK_BOX (hbox_width), spinbutton_width, FALSE,
			    FALSE, 0);

	combo_width = gtk_combo_new ();
	g_object_set_data (G_OBJECT (GTK_COMBO (combo_width)->popwin),
			   "GladeParentKey", combo_width);
	gtk_widget_show (combo_width);
	gtk_box_pack_start (GTK_BOX (hbox_width), combo_width, FALSE, FALSE,
			    0);
	gtk_widget_set_size_request (combo_width, 50, -1);
	combo_width_items =
		g_list_append (combo_width_items, (gpointer) "em");
	combo_width_items =
		g_list_append (combo_width_items, (gpointer) "ex");
	combo_width_items =
		g_list_append (combo_width_items, (gpointer) "px");
	combo_width_items =
		g_list_append (combo_width_items, (gpointer) "in");
	combo_width_items =
		g_list_append (combo_width_items, (gpointer) "cm");
	combo_width_items =
		g_list_append (combo_width_items, (gpointer) "mm");
	combo_width_items =
		g_list_append (combo_width_items, (gpointer) "pt");
	combo_width_items =
		g_list_append (combo_width_items, (gpointer) "pc");
	gtk_combo_set_popdown_strings (GTK_COMBO (combo_width),
				       combo_width_items);
	g_list_free (combo_width_items);

	combo_entry_width = GTK_COMBO (combo_width)->entry;
	gtk_widget_show (combo_entry_width);
	gtk_entry_set_text (GTK_ENTRY (combo_entry_width), "px");

	hbox_height = gtk_hbox_new (TRUE, 0);
	gtk_widget_show (hbox_height);
	gtk_box_pack_start (GTK_BOX (vbox), hbox_height, TRUE, TRUE, 0);

	label_height = gtk_label_new (_("Height"));
	gtk_widget_show (label_height);
	gtk_box_pack_start (GTK_BOX (hbox_height), label_height, TRUE, TRUE,
			    0);
	gtk_label_set_justify (GTK_LABEL (label_height), GTK_JUSTIFY_LEFT);

	spinbutton_height_adj =
		gtk_adjustment_new (1, -35565, 35565, 1, 10, 10);
	spinbutton_height =
		gtk_spin_button_new (GTK_ADJUSTMENT (spinbutton_height_adj),
				     1, 0);
	gtk_widget_show (spinbutton_height);
	gtk_box_pack_start (GTK_BOX (hbox_height), spinbutton_height, FALSE,
			    FALSE, 0);

	combo_height = gtk_combo_new ();
	g_object_set_data (G_OBJECT (GTK_COMBO (combo_height)->popwin),
			   "GladeParentKey", combo_height);
	gtk_widget_show (combo_height);
	gtk_box_pack_start (GTK_BOX (hbox_height), combo_height, FALSE, FALSE,
			    0);
	gtk_widget_set_size_request (combo_height, 50, -1);
	combo_height_items =
		g_list_append (combo_height_items, (gpointer) "em");
	combo_height_items =
		g_list_append (combo_height_items, (gpointer) "ex");
	combo_height_items =
		g_list_append (combo_height_items, (gpointer) "px");
	combo_height_items =
		g_list_append (combo_height_items, (gpointer) "in");
	combo_height_items =
		g_list_append (combo_height_items, (gpointer) "cm");
	combo_height_items =
		g_list_append (combo_height_items, (gpointer) "mm");
	combo_height_items =
		g_list_append (combo_height_items, (gpointer) "pt");
	combo_height_items =
		g_list_append (combo_height_items, (gpointer) "pc");
	gtk_combo_set_popdown_strings (GTK_COMBO (combo_height),
				       combo_height_items);
	g_list_free (combo_height_items);

	combo_entry_height = GTK_COMBO (combo_height)->entry;
	gtk_widget_show (combo_entry_height);
	gtk_entry_set_text (GTK_ENTRY (combo_entry_height), "px");

	dialog_action_area =
		GTK_DIALOG (size_length_value_dialog)->action_area;
	gtk_widget_show (dialog_action_area);
	gtk_button_box_set_layout (GTK_BUTTON_BOX (dialog_action_area),
				   GTK_BUTTONBOX_END);

	helpbutton = gtk_button_new_from_stock ("gtk-help");
	gtk_widget_show (helpbutton);
	gtk_dialog_add_action_widget (GTK_DIALOG (size_length_value_dialog),
				      helpbutton, GTK_RESPONSE_HELP);
	GTK_WIDGET_SET_FLAGS (helpbutton, GTK_CAN_DEFAULT);
	gtk_widget_add_accelerator (helpbutton, "clicked", accel_group,
				    GDK_y, GDK_CONTROL_MASK,
				    GTK_ACCEL_VISIBLE);
	gtk_widget_add_accelerator (helpbutton, "clicked", accel_group, GDK_h,
				    GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);

	cancelbutton = gtk_button_new_from_stock ("gtk-cancel");
	gtk_widget_show (cancelbutton);
	gtk_dialog_add_action_widget (GTK_DIALOG (size_length_value_dialog),
				      cancelbutton, GTK_RESPONSE_CANCEL);
	GTK_WIDGET_SET_FLAGS (cancelbutton, GTK_CAN_DEFAULT);
	gtk_widget_add_accelerator (cancelbutton, "clicked", accel_group,
				    GDK_c, GDK_CONTROL_MASK,
				    GTK_ACCEL_VISIBLE);

	okbutton = gtk_button_new_from_stock ("gtk-ok");
	gtk_widget_show (okbutton);
	gtk_dialog_add_action_widget (GTK_DIALOG (size_length_value_dialog),
				      okbutton, GTK_RESPONSE_OK);
	GTK_WIDGET_SET_FLAGS (okbutton, GTK_CAN_DEFAULT);
	gtk_widget_add_accelerator (okbutton, "clicked", accel_group,
				    GDK_o, GDK_CONTROL_MASK,
				    GTK_ACCEL_VISIBLE);

	g_signal_connect ((gpointer) combo_entry_width,
			  "changed", G_CALLBACK (on_css_dialog_unit_changed),
			  spinbutton_width);
	g_signal_connect ((gpointer) combo_entry_height,
			  "changed", G_CALLBACK (on_css_dialog_unit_changed),
			  spinbutton_height);

	if (connect_callbacks)
	{
		g_signal_connect ((gpointer) size_length_value_dialog,
				  "close", G_CALLBACK (on_css_dialog_close),
				  data);

		g_signal_connect ((gpointer) helpbutton, "clicked",
				  G_CALLBACK
				  (on_size_lenght_value_dialog_helpbutton_clicked),
				  NULL);
		g_signal_connect ((gpointer) cancelbutton, "clicked",
				  G_CALLBACK
				  (on_css_dialog_cancel_button_clicked),
				  data);
		g_signal_connect ((gpointer) okbutton, "clicked",
				  G_CALLBACK
				  (on_size_length_value_dialog_okbutton_clicked),
				  data);
	}

	GLADE_HOOKUP_OBJECT_NO_REF (size_length_value_dialog, size_length_value_dialog, "size_length_value_dialog");
	GLADE_HOOKUP_OBJECT (size_length_value_dialog, label_width, "label_width");
	GLADE_HOOKUP_OBJECT (size_length_value_dialog, spinbutton_width, "spinbutton_width");
	GLADE_HOOKUP_OBJECT (size_length_value_dialog, combo_width, "combo_width");
	GLADE_HOOKUP_OBJECT (size_length_value_dialog, combo_entry_width, "combo_entry_width");
	GLADE_HOOKUP_OBJECT (size_length_value_dialog, label_height, "label_height");
	GLADE_HOOKUP_OBJECT (size_length_value_dialog, spinbutton_height, "spinbutton_height");
	GLADE_HOOKUP_OBJECT (size_length_value_dialog, combo_height, "combo_height");
	GLADE_HOOKUP_OBJECT (size_length_value_dialog, combo_entry_height, "combo_entry_height");
	GLADE_HOOKUP_OBJECT (size_length_value_dialog, helpbutton, "helpbutton");
	GLADE_HOOKUP_OBJECT (size_length_value_dialog, cancelbutton, "cancelbutton");
	GLADE_HOOKUP_OBJECT (size_length_value_dialog, okbutton, "okbutton");

	gtk_window_add_accel_group (GTK_WINDOW (size_length_value_dialog), accel_group);

	return size_length_value_dialog;
}

GtkWidget *
create_text_shadow_value_dialog (CssedWindow * window,
								 gchar * previous_string,
								 gboolean add_previous_string,
								 gboolean connect_callbacks)
{
	GtkWidget *text_shadow_value_dialog;
	GtkWidget *dialog_vbox;
	GtkWidget *vbox;
	GtkWidget *hbox_right;
	GtkWidget *label_right;
	GtkObject *spinbutton_right_adj;
	GtkWidget *spinbutton_right;
	GtkWidget *combo_right;
	GList *combo_right_items = NULL;
	GtkWidget *combo_entry_right;
	GtkWidget *hbox_below;
	GtkWidget *label_below;
	GtkObject *spinbutton_below_adj;
	GtkWidget *spinbutton_below;
	GtkWidget *combo_below;
	GList *combo_below_items = NULL;
	GtkWidget *combo_entry_below;
	GtkWidget *hbox_blur;
	GtkWidget *checkbutton_blur;
	GtkObject *spinbutton_blur_adj;
	GtkWidget *spinbutton_blur;
	GtkWidget *combo_blur;
	GList *combo_blur_items = NULL;
	GtkWidget *combo_entry_blur;
	GtkWidget *hbox_use_color;
	GtkWidget *checkbutton_use_color;
	GtkWidget *combo_use_color;
	GList *combo_use_color_items = NULL;
	GtkWidget *combo_entry_use_color;
	GtkWidget *colorselection;
	GtkWidget *hbox_shadow_list;
	GtkWidget *checkbutton_use_shadow_list;
	GtkWidget *hbuttonbox;
	GtkWidget *button_text_shadow_value_dialog_add;
	GtkWidget *button_text_shadow_value_dialog_del;
	GtkWidget *scrolledwindow;
	GtkWidget *treeview;
	GtkWidget *dialog_action_area;
	GtkWidget *helpbutton;
	GtkWidget *cancelbutton;
	GtkWidget *okbutton;
	GtkAccelGroup *accel_group;
	GtkWidget *main_window;
	GtkTreeViewColumn *col_right;
	GtkTreeViewColumn *col_below;
	GtkTreeViewColumn *col_blur;
	GtkTreeViewColumn *col_color;
	GtkCellRenderer *ri_renderer;
	GtkCellRenderer *be_renderer;
	GtkCellRenderer *bl_renderer;
	GtkCellRenderer *co_renderer;
	GtkListStore *store;
	CssedCssDialogsData *data = NULL;

	enum
	{
		COL_RIGHT,
		COL_BELOW,
		COL_BLUR,
		COL_COLOR,
		N_COLS
	};

	if (connect_callbacks)
	{
		data = g_malloc (sizeof (CssedCssDialogsData));
		data->main_window = window;
		data->previous_string = previous_string;
		data->add_property_string = add_previous_string;
	}

	accel_group = gtk_accel_group_new ();

	text_shadow_value_dialog = gtk_dialog_new ();
	gtk_window_set_title (GTK_WINDOW (text_shadow_value_dialog),
			      previous_string);
	gtk_window_set_modal (GTK_WINDOW (text_shadow_value_dialog), TRUE);

	main_window = cssed_window_get_window_widget( window );

	gtk_window_set_transient_for (GTK_WINDOW (text_shadow_value_dialog),
				      GTK_WINDOW (main_window));

	dialog_vbox = GTK_DIALOG (text_shadow_value_dialog)->vbox;
	gtk_widget_show (dialog_vbox);

	vbox = gtk_vbox_new (FALSE, 0);
	gtk_widget_show (vbox);
	gtk_box_pack_start (GTK_BOX (dialog_vbox), vbox, TRUE, TRUE, 0);
	gtk_container_set_border_width (GTK_CONTAINER (vbox), 5);

	hbox_right = gtk_hbox_new (FALSE, 0);
	gtk_widget_show (hbox_right);
	gtk_box_pack_start (GTK_BOX (vbox), hbox_right, FALSE, FALSE, 5);

	label_right =
		gtk_label_new (_("Shadow right  ( this field is required )"));
	gtk_widget_show (label_right);
	gtk_box_pack_start (GTK_BOX (hbox_right), label_right, TRUE, TRUE, 0);
	gtk_label_set_justify (GTK_LABEL (label_right), GTK_JUSTIFY_LEFT);

	spinbutton_right_adj =
		gtk_adjustment_new (1, -35565, 35565, 1, 10, 10);
	spinbutton_right =
		gtk_spin_button_new (GTK_ADJUSTMENT (spinbutton_right_adj),
				     1, 0);
	gtk_widget_show (spinbutton_right);
	gtk_box_pack_start (GTK_BOX (hbox_right), spinbutton_right, FALSE,
			    FALSE, 5);

	combo_right = gtk_combo_new ();
	g_object_set_data (G_OBJECT (GTK_COMBO (combo_right)->popwin),
			   "GladeParentKey", combo_right);
	gtk_widget_show (combo_right);
	gtk_box_pack_start (GTK_BOX (hbox_right), combo_right, FALSE, FALSE,
			    5);
	gtk_widget_set_size_request (combo_right, 50, -1);
	combo_right_items =
		g_list_append (combo_right_items, (gpointer) "em");
	combo_right_items =
		g_list_append (combo_right_items, (gpointer) "ex");
	combo_right_items =
		g_list_append (combo_right_items, (gpointer) "px");
	combo_right_items =
		g_list_append (combo_right_items, (gpointer) "in");
	combo_right_items =
		g_list_append (combo_right_items, (gpointer) "cm");
	combo_right_items =
		g_list_append (combo_right_items, (gpointer) "mm");
	combo_right_items =
		g_list_append (combo_right_items, (gpointer) "pt");
	combo_right_items =
		g_list_append (combo_right_items, (gpointer) "pc");
	gtk_combo_set_popdown_strings (GTK_COMBO (combo_right),
				       combo_right_items);
	g_list_free (combo_right_items);

	combo_entry_right = GTK_COMBO (combo_right)->entry;
	gtk_widget_show (combo_entry_right);
	gtk_entry_set_text (GTK_ENTRY (combo_entry_right), "px");

	hbox_below = gtk_hbox_new (FALSE, 0);
	gtk_widget_show (hbox_below);
	gtk_box_pack_start (GTK_BOX (vbox), hbox_below, FALSE, FALSE, 5);

	label_below =
		gtk_label_new (_("Shadow below ( this field is required )"));
	gtk_widget_show (label_below);
	gtk_box_pack_start (GTK_BOX (hbox_below), label_below, TRUE, TRUE, 0);
	gtk_label_set_justify (GTK_LABEL (label_below), GTK_JUSTIFY_LEFT);

	spinbutton_below_adj =
		gtk_adjustment_new (1, -35565, 35565, 1, 10, 10);
	spinbutton_below =
		gtk_spin_button_new (GTK_ADJUSTMENT (spinbutton_below_adj),
				     1, 0);
	gtk_widget_show (spinbutton_below);
	gtk_box_pack_start (GTK_BOX (hbox_below), spinbutton_below, FALSE,
			    FALSE, 5);

	combo_below = gtk_combo_new ();
	g_object_set_data (G_OBJECT (GTK_COMBO (combo_below)->popwin),
			   "GladeParentKey", combo_below);
	gtk_widget_show (combo_below);
	gtk_box_pack_start (GTK_BOX (hbox_below), combo_below, FALSE, FALSE,
			    5);
	gtk_widget_set_size_request (combo_below, 50, -1);
	combo_below_items =
		g_list_append (combo_below_items, (gpointer) "em");
	combo_below_items =
		g_list_append (combo_below_items, (gpointer) "ex");
	combo_below_items =
		g_list_append (combo_below_items, (gpointer) "px");
	combo_below_items =
		g_list_append (combo_below_items, (gpointer) "in");
	combo_below_items =
		g_list_append (combo_below_items, (gpointer) "cm");
	combo_below_items =
		g_list_append (combo_below_items, (gpointer) "mm");
	combo_below_items =
		g_list_append (combo_below_items, (gpointer) "pt");
	combo_below_items =
		g_list_append (combo_below_items, (gpointer) "pc");
	gtk_combo_set_popdown_strings (GTK_COMBO (combo_below),
				       combo_below_items);
	g_list_free (combo_below_items);

	combo_entry_below = GTK_COMBO (combo_below)->entry;
	gtk_widget_show (combo_entry_below);
	gtk_entry_set_text (GTK_ENTRY (combo_entry_below), "px");

	hbox_blur = gtk_hbox_new (FALSE, 0);
	gtk_widget_show (hbox_blur);
	gtk_box_pack_start (GTK_BOX (vbox), hbox_blur, FALSE, FALSE, 5);

	checkbutton_blur = gtk_check_button_new_with_mnemonic (_("Blur"));
	gtk_widget_show (checkbutton_blur);
	gtk_box_pack_start (GTK_BOX (hbox_blur), checkbutton_blur, TRUE, TRUE,
			    5);

	spinbutton_blur_adj =
		gtk_adjustment_new (1.00, -1000.00, 1000.00, 0.10, 0.10,
				    0.10);
	spinbutton_blur =
		gtk_spin_button_new (GTK_ADJUSTMENT (spinbutton_blur_adj),
				     1, 0);
	gtk_widget_show (spinbutton_blur);
	gtk_box_pack_start (GTK_BOX (hbox_blur), spinbutton_blur, FALSE,
			    FALSE, 5);

	combo_blur = gtk_combo_new ();
	g_object_set_data (G_OBJECT (GTK_COMBO (combo_blur)->popwin),
			   "GladeParentKey", combo_blur);

	gtk_widget_show (combo_blur);
	gtk_box_pack_start (GTK_BOX (hbox_blur), combo_blur, FALSE, FALSE, 5);
	gtk_widget_set_size_request (combo_blur, 50, -1);
	combo_blur_items = g_list_append (combo_blur_items, (gpointer) "em");
	combo_blur_items = g_list_append (combo_blur_items, (gpointer) "ex");
	combo_blur_items = g_list_append (combo_blur_items, (gpointer) "px");
	combo_blur_items = g_list_append (combo_blur_items, (gpointer) "in");
	combo_blur_items = g_list_append (combo_blur_items, (gpointer) "cm");
	combo_blur_items = g_list_append (combo_blur_items, (gpointer) "mm");
	combo_blur_items = g_list_append (combo_blur_items, (gpointer) "pt");
	combo_blur_items = g_list_append (combo_blur_items, (gpointer) "pc");
	gtk_combo_set_popdown_strings (GTK_COMBO (combo_blur),
				       combo_blur_items);
	g_list_free (combo_blur_items);

	combo_entry_blur = GTK_COMBO (combo_blur)->entry;
	gtk_widget_show (combo_entry_blur);
	gtk_entry_set_text (GTK_ENTRY (combo_entry_blur), "px");

	hbox_use_color = gtk_hbox_new (TRUE, 0);
	gtk_widget_show (hbox_use_color);
	gtk_box_pack_start (GTK_BOX (vbox), hbox_use_color, FALSE, FALSE, 5);

	checkbutton_use_color =
		gtk_check_button_new_with_mnemonic (_("Use color"));
	gtk_widget_show (checkbutton_use_color);
	gtk_box_pack_start (GTK_BOX (hbox_use_color), checkbutton_use_color,
			    TRUE, TRUE, 5);

	combo_use_color = gtk_combo_new ();
	g_object_set_data (G_OBJECT (GTK_COMBO (combo_use_color)->popwin),
			   "GladeParentKey", combo_use_color);
	gtk_widget_show (combo_use_color);
	gtk_box_pack_start (GTK_BOX (hbox_use_color), combo_use_color, TRUE,
			    TRUE, 5);
	combo_use_color_items =
		g_list_append (combo_use_color_items,
			       (gpointer) CSS_STR_USE_COLORSEL);
	combo_use_color_items =
		g_list_append (combo_use_color_items, (gpointer) "aqua");
	combo_use_color_items =
		g_list_append (combo_use_color_items, (gpointer) "black");
	combo_use_color_items =
		g_list_append (combo_use_color_items, (gpointer) "blue");
	combo_use_color_items =
		g_list_append (combo_use_color_items, (gpointer) "fuchsia");
	combo_use_color_items =
		g_list_append (combo_use_color_items, (gpointer) "gray");
	combo_use_color_items =
		g_list_append (combo_use_color_items, (gpointer) "green");
	combo_use_color_items =
		g_list_append (combo_use_color_items, (gpointer) "lime");
	combo_use_color_items =
		g_list_append (combo_use_color_items, (gpointer) "maroon");
	combo_use_color_items =
		g_list_append (combo_use_color_items, (gpointer) "navy");
	combo_use_color_items =
		g_list_append (combo_use_color_items, (gpointer) "olive");
	combo_use_color_items =
		g_list_append (combo_use_color_items, (gpointer) "purple");
	combo_use_color_items =
		g_list_append (combo_use_color_items, (gpointer) "red");
	combo_use_color_items =
		g_list_append (combo_use_color_items, (gpointer) "silver");
	combo_use_color_items =
		g_list_append (combo_use_color_items, (gpointer) "teal");
	combo_use_color_items =
		g_list_append (combo_use_color_items, (gpointer) "white");
	combo_use_color_items =
		g_list_append (combo_use_color_items, (gpointer) "yellow");
	combo_use_color_items =
		g_list_append (combo_use_color_items,
			       (gpointer) "ActiveBorder");
	combo_use_color_items =
		g_list_append (combo_use_color_items,
			       (gpointer) "ActiveCaption");
	combo_use_color_items =
		g_list_append (combo_use_color_items,
			       (gpointer) "AppWorkspace");
	combo_use_color_items =
		g_list_append (combo_use_color_items,
			       (gpointer) "Background");
	combo_use_color_items =
		g_list_append (combo_use_color_items,
			       (gpointer) "ButtonFace");
	combo_use_color_items =
		g_list_append (combo_use_color_items,
			       (gpointer) "ButtonHighlight");
	combo_use_color_items =
		g_list_append (combo_use_color_items,
			       (gpointer) "ButtonShadow");
	combo_use_color_items =
		g_list_append (combo_use_color_items,
			       (gpointer) "ButtonText");
	combo_use_color_items =
		g_list_append (combo_use_color_items,
			       (gpointer) "CaptionText");
	combo_use_color_items =
		g_list_append (combo_use_color_items, (gpointer) "GrayText");
	combo_use_color_items =
		g_list_append (combo_use_color_items, (gpointer) "Highlight");
	combo_use_color_items =
		g_list_append (combo_use_color_items,
			       (gpointer) "HighlightText");
	combo_use_color_items =
		g_list_append (combo_use_color_items,
			       (gpointer) "InactiveBorder");
	combo_use_color_items =
		g_list_append (combo_use_color_items,
			       (gpointer) "InactiveCaption");
	combo_use_color_items =
		g_list_append (combo_use_color_items,
			       (gpointer) "InactiveCaptionText");
	combo_use_color_items =
		g_list_append (combo_use_color_items,
			       (gpointer) "InfoBackground");
	combo_use_color_items =
		g_list_append (combo_use_color_items, (gpointer) "InfoText");
	combo_use_color_items =
		g_list_append (combo_use_color_items, (gpointer) "Menu");
	combo_use_color_items =
		g_list_append (combo_use_color_items, (gpointer) "MenuText");
	combo_use_color_items =
		g_list_append (combo_use_color_items, (gpointer) "Scrollbar");
	combo_use_color_items =
		g_list_append (combo_use_color_items,
			       (gpointer) "ThreeDDarkShadow");
	combo_use_color_items =
		g_list_append (combo_use_color_items,
			       (gpointer) "ThreeDFace");
	combo_use_color_items =
		g_list_append (combo_use_color_items,
			       (gpointer) "ThreeDHighlight");
	combo_use_color_items =
		g_list_append (combo_use_color_items,
			       (gpointer) "ThreeDLightShadow");
	combo_use_color_items =
		g_list_append (combo_use_color_items,
			       (gpointer) "ThreeDShadow");
	combo_use_color_items =
		g_list_append (combo_use_color_items, (gpointer) "Window");
	combo_use_color_items =
		g_list_append (combo_use_color_items,
			       (gpointer) "WindowFrame");
	combo_use_color_items =
		g_list_append (combo_use_color_items,
			       (gpointer) "WindowText");
	gtk_combo_set_popdown_strings (GTK_COMBO (combo_use_color),
				       combo_use_color_items);
	g_list_free (combo_use_color_items);

	combo_entry_use_color = GTK_COMBO (combo_use_color)->entry;
	gtk_widget_show (combo_entry_use_color);
	gtk_entry_set_text (GTK_ENTRY (combo_entry_use_color),
			    CSS_STR_USE_COLORSEL);

	colorselection = gtk_color_selection_new ();
	gtk_widget_show (colorselection);
	gtk_box_pack_start (GTK_BOX (vbox), colorselection, FALSE, FALSE, 0);
	gtk_container_set_border_width (GTK_CONTAINER (colorselection), 15);
	gtk_color_selection_set_has_opacity_control (GTK_COLOR_SELECTION
						     (colorselection), FALSE);
	gtk_color_selection_set_has_palette (GTK_COLOR_SELECTION
					     (colorselection), TRUE);

	hbox_shadow_list = gtk_hbox_new (TRUE, 0);
	gtk_widget_show (hbox_shadow_list);
	gtk_box_pack_start (GTK_BOX (vbox), hbox_shadow_list, FALSE, FALSE,
			    0);

	checkbutton_use_shadow_list =
		gtk_check_button_new_with_mnemonic (_
						    ("Use shadow list (more than one)"));
	gtk_widget_show (checkbutton_use_shadow_list);
	gtk_box_pack_start (GTK_BOX (hbox_shadow_list),
			    checkbutton_use_shadow_list, TRUE, TRUE, 0);

	hbuttonbox = gtk_hbutton_box_new ();
	gtk_widget_show (hbuttonbox);
	gtk_box_pack_start (GTK_BOX (hbox_shadow_list), hbuttonbox, TRUE,
			    TRUE, 0);
	gtk_button_box_set_layout (GTK_BUTTON_BOX (hbuttonbox),
				   GTK_BUTTONBOX_END);

	button_text_shadow_value_dialog_add =
		gtk_button_new_from_stock ("gtk-add");
	gtk_widget_show (button_text_shadow_value_dialog_add);
	gtk_container_add (GTK_CONTAINER (hbuttonbox),
			   button_text_shadow_value_dialog_add);
	gtk_container_set_border_width (GTK_CONTAINER
					(button_text_shadow_value_dialog_add),
					5);
	GTK_WIDGET_SET_FLAGS (button_text_shadow_value_dialog_add,
			      GTK_CAN_DEFAULT);
	gtk_widget_add_accelerator (button_text_shadow_value_dialog_add,
				    "clicked", accel_group, GDK_a,
				    GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);

	button_text_shadow_value_dialog_del =
		gtk_button_new_from_stock ("gtk-remove");
	gtk_widget_show (button_text_shadow_value_dialog_del);
	gtk_container_add (GTK_CONTAINER (hbuttonbox),
			   button_text_shadow_value_dialog_del);
	gtk_container_set_border_width (GTK_CONTAINER
					(button_text_shadow_value_dialog_del),
					5);
	GTK_WIDGET_SET_FLAGS (button_text_shadow_value_dialog_del,
			      GTK_CAN_DEFAULT);
	gtk_widget_add_accelerator (button_text_shadow_value_dialog_del,
				    "clicked", accel_group, GDK_q,
				    GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);
	gtk_widget_add_accelerator (button_text_shadow_value_dialog_del,
				    "clicked", accel_group, GDK_d,
				    GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);

	scrolledwindow = gtk_scrolled_window_new (NULL, NULL);
	gtk_widget_show (scrolledwindow);
	gtk_box_pack_start (GTK_BOX (vbox), scrolledwindow, TRUE, TRUE, 5);
	gtk_widget_set_size_request (scrolledwindow, -1, 125);
	// 
	store = gtk_list_store_new (N_COLS, G_TYPE_STRING, G_TYPE_STRING,
				    G_TYPE_STRING, G_TYPE_STRING);
	treeview = gtk_tree_view_new_with_model (GTK_TREE_MODEL (store));

	ri_renderer = gtk_cell_renderer_text_new ();
	be_renderer = gtk_cell_renderer_text_new ();
	bl_renderer = gtk_cell_renderer_text_new ();
	co_renderer = gtk_cell_renderer_text_new ();

	col_right =
		gtk_tree_view_column_new_with_attributes (_("Right"),
							  ri_renderer,
							  "text", COL_RIGHT,
							  NULL);
	col_below =
		gtk_tree_view_column_new_with_attributes (_("Below"),
							  be_renderer, "text",
							  COL_BELOW, NULL);

	col_blur =
		gtk_tree_view_column_new_with_attributes (_("Blur"),
							  bl_renderer,
							  "text", COL_BLUR,
							  NULL);
	col_color =
		gtk_tree_view_column_new_with_attributes (_("Color"),
							  co_renderer, "text",
							  COL_COLOR, NULL);

	gtk_tree_view_insert_column (GTK_TREE_VIEW (treeview),
				     col_right, COL_RIGHT);
	gtk_tree_view_insert_column (GTK_TREE_VIEW (treeview),
				     col_below, COL_BELOW);
	gtk_tree_view_insert_column (GTK_TREE_VIEW (treeview), col_blur,
				     COL_BLUR);
	gtk_tree_view_insert_column (GTK_TREE_VIEW (treeview), col_color,
				     COL_COLOR);

	gtk_widget_show (treeview);
	gtk_container_add (GTK_CONTAINER (scrolledwindow), treeview);
	gtk_tree_view_set_headers_visible (GTK_TREE_VIEW (treeview), TRUE);
	// 
	dialog_action_area =
		GTK_DIALOG (text_shadow_value_dialog)->action_area;
	gtk_widget_show (dialog_action_area);
	gtk_button_box_set_layout (GTK_BUTTON_BOX (dialog_action_area),
				   GTK_BUTTONBOX_END);

	helpbutton = gtk_button_new_from_stock ("gtk-help");
	gtk_widget_show (helpbutton);
	gtk_dialog_add_action_widget (GTK_DIALOG (text_shadow_value_dialog),
				      helpbutton, GTK_RESPONSE_HELP);
	GTK_WIDGET_SET_FLAGS (helpbutton, GTK_CAN_DEFAULT);
	gtk_widget_add_accelerator (helpbutton, "clicked", accel_group,
				    GDK_y, GDK_CONTROL_MASK,
				    GTK_ACCEL_VISIBLE);
	gtk_widget_add_accelerator (helpbutton, "clicked", accel_group, GDK_h,
				    GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);

	cancelbutton = gtk_button_new_from_stock ("gtk-cancel");
	gtk_widget_show (cancelbutton);
	gtk_dialog_add_action_widget (GTK_DIALOG (text_shadow_value_dialog),
				      cancelbutton, GTK_RESPONSE_CANCEL);
	GTK_WIDGET_SET_FLAGS (cancelbutton, GTK_CAN_DEFAULT);
	gtk_widget_add_accelerator (cancelbutton, "clicked", accel_group,
				    GDK_c, GDK_CONTROL_MASK,
				    GTK_ACCEL_VISIBLE);

	okbutton = gtk_button_new_from_stock ("gtk-ok");
	gtk_widget_show (okbutton);
	gtk_dialog_add_action_widget (GTK_DIALOG (text_shadow_value_dialog),
				      okbutton, GTK_RESPONSE_OK);
	GTK_WIDGET_SET_FLAGS (okbutton, GTK_CAN_DEFAULT);
	gtk_widget_add_accelerator (okbutton, "clicked", accel_group,
				    GDK_o, GDK_CONTROL_MASK,
				    GTK_ACCEL_VISIBLE);

	g_signal_connect ((gpointer) combo_entry_right,
			  "changed", G_CALLBACK (on_css_dialog_unit_changed),
			  spinbutton_right);
	g_signal_connect ((gpointer) combo_entry_below,
			  "changed", G_CALLBACK (on_css_dialog_unit_changed),
			  spinbutton_below);
	g_signal_connect ((gpointer) combo_entry_blur,
			  "changed", G_CALLBACK (on_css_dialog_unit_changed),
			  spinbutton_blur);

	/* to update the checkboxes when a related widget is changed */				    
	g_signal_connect ((gpointer) spinbutton_blur,
			  "value-changed", G_CALLBACK (on_widget_change_set_toggle_active),
			  checkbutton_blur );
	g_signal_connect ((gpointer) combo_entry_blur,
			  "changed", G_CALLBACK (on_widget_change_set_toggle_active),
			  checkbutton_blur);
			  
	    
	g_signal_connect ((gpointer) combo_entry_use_color,
			  "changed", G_CALLBACK (on_widget_change_set_toggle_active),
			  checkbutton_use_color );
	g_signal_connect ((gpointer) colorselection,
			  "color-changed", G_CALLBACK (on_widget_change_set_toggle_active),
			  checkbutton_use_color);			  

	g_signal_connect ((gpointer) button_text_shadow_value_dialog_add,
			  "clicked", G_CALLBACK (on_widget_change_set_toggle_active),
			  checkbutton_use_shadow_list );
			  
	if (connect_callbacks)
	{
		g_signal_connect ((gpointer) text_shadow_value_dialog,
				  "close", G_CALLBACK (on_css_dialog_close),
				  data);
		g_signal_connect ((gpointer) cancelbutton, "clicked",
				  G_CALLBACK
				  (on_css_dialog_cancel_button_clicked),
				  data);
		g_signal_connect ((gpointer) okbutton, "clicked",
				  G_CALLBACK
				  (on_text_shadow_value_dialog_okbutton_clicked),
				  data);
	}

	g_signal_connect ((gpointer) button_text_shadow_value_dialog_add,
			  "clicked",
			  G_CALLBACK
			  (on_button_text_shadow_value_dialog_add_clicked),
			  store);
	g_signal_connect ((gpointer) button_text_shadow_value_dialog_del,
			  "clicked",
			  G_CALLBACK
			  (on_button_text_shadow_value_dialog_del_clicked),
			  store);

	GLADE_HOOKUP_OBJECT_NO_REF (text_shadow_value_dialog,
				    text_shadow_value_dialog,
				    "text_shadow_value_dialog");
	GLADE_HOOKUP_OBJECT (text_shadow_value_dialog, label_right,
			     "label_right");
	GLADE_HOOKUP_OBJECT (text_shadow_value_dialog, spinbutton_right,
			     "spinbutton_right");
	GLADE_HOOKUP_OBJECT (text_shadow_value_dialog, combo_right,
			     "combo_right");
	GLADE_HOOKUP_OBJECT (text_shadow_value_dialog, combo_entry_right,
			     "combo_entry_right");
	GLADE_HOOKUP_OBJECT (text_shadow_value_dialog, hbox_below,
			     "hbox_below");
	GLADE_HOOKUP_OBJECT (text_shadow_value_dialog, label_below,
			     "label_below");
	GLADE_HOOKUP_OBJECT (text_shadow_value_dialog, spinbutton_below,
			     "spinbutton_below");
	GLADE_HOOKUP_OBJECT (text_shadow_value_dialog, combo_below,
			     "combo_below");
	GLADE_HOOKUP_OBJECT (text_shadow_value_dialog, combo_entry_below,
			     "combo_entry_below");
	GLADE_HOOKUP_OBJECT (text_shadow_value_dialog, checkbutton_blur,
			     "checkbutton_blur");
	GLADE_HOOKUP_OBJECT (text_shadow_value_dialog, spinbutton_blur,
			     "spinbutton_blur");
	GLADE_HOOKUP_OBJECT (text_shadow_value_dialog, combo_blur,
			     "combo_blur");
	GLADE_HOOKUP_OBJECT (text_shadow_value_dialog, combo_entry_blur,
			     "combo_entry_blur");
	GLADE_HOOKUP_OBJECT (text_shadow_value_dialog, hbox_use_color,
			     "hbox_use_color");
	GLADE_HOOKUP_OBJECT (text_shadow_value_dialog, checkbutton_use_color,
			     "checkbutton_use_color");
	GLADE_HOOKUP_OBJECT (text_shadow_value_dialog, combo_use_color,
			     "combo_use_color");
	GLADE_HOOKUP_OBJECT (text_shadow_value_dialog, combo_entry_use_color,
			     "combo_entry_use_color");
	GLADE_HOOKUP_OBJECT (text_shadow_value_dialog, colorselection,
			     "colorselection");
	GLADE_HOOKUP_OBJECT (text_shadow_value_dialog,
			     checkbutton_use_shadow_list,
			     "checkbutton_use_shadow_list");
	GLADE_HOOKUP_OBJECT (text_shadow_value_dialog, hbuttonbox,
			     "hbuttonbox");
	GLADE_HOOKUP_OBJECT (text_shadow_value_dialog,
			     button_text_shadow_value_dialog_add,
			     "button_text_shadow_value_dialog_add");
	GLADE_HOOKUP_OBJECT (text_shadow_value_dialog,
			     button_text_shadow_value_dialog_del,
			     "button_text_shadow_value_dialog_del");
	GLADE_HOOKUP_OBJECT (text_shadow_value_dialog, treeview, "treeview");
	GLADE_HOOKUP_OBJECT (text_shadow_value_dialog, helpbutton,
			     "helpbutton");
	GLADE_HOOKUP_OBJECT (text_shadow_value_dialog, cancelbutton,
			     "cancelbutton");
	GLADE_HOOKUP_OBJECT (text_shadow_value_dialog, okbutton, "okbutton");

	gtk_window_add_accel_group (GTK_WINDOW (text_shadow_value_dialog),
				    accel_group);

	return text_shadow_value_dialog;
}

GtkWidget *
create_voice_family_value_dialog (CssedWindow * window,
				  gchar * previous_string,
				  gboolean add_previous_string,
				  gboolean connect_callbacks)
{
	GtkWidget *voice_family_value_dialog;
	GtkWidget *dialog_vbox;
	GtkWidget *vbox;
	GtkWidget *hbox_generic;
	GtkWidget *label_generic;
	GtkWidget *combo_generic;
	GList *combo_generic_items = NULL;
	GtkWidget *combo_entry_generic;
	GtkWidget *hbox_specific;
	GtkWidget *label_specific;
	GtkWidget *entry_specific_voice;
	GtkWidget *hbox_voice_set;
	GtkWidget *checkbutton_voice_set;
	GtkWidget *hbuttonbox;
	GtkWidget *button_voice_family_value_dialog_add;
	GtkWidget *button_voice_family_value_dialog_del;
	GtkWidget *scrolledwindow;
	GtkWidget *treeview;
	GtkWidget *dialog_action_area;
	GtkWidget *helpbutton;
	GtkWidget *cancelbutton;
	GtkWidget *okbutton;
	GtkAccelGroup *accel_group;
	GtkWidget *main_window;
	GtkTreeViewColumn *col_generic_voice;
	GtkTreeViewColumn *col_specific_voice;
	GtkCellRenderer *gv_renderer;
	GtkCellRenderer *sv_renderer;
	GtkListStore *store;

	CssedCssDialogsData *data = NULL;

	enum
	{
		COL_GENERIC,
		COL_SPECIFIC,
		N_COLS
	};

	if (connect_callbacks)
	{
		data = g_malloc (sizeof (CssedCssDialogsData));
		data->main_window = window;
		data->previous_string = previous_string;
		data->add_property_string = add_previous_string;
	}

	accel_group = gtk_accel_group_new ();

	voice_family_value_dialog = gtk_dialog_new ();
	gtk_window_set_title (GTK_WINDOW (voice_family_value_dialog),
			      previous_string);
	gtk_window_set_modal (GTK_WINDOW (voice_family_value_dialog), TRUE);

	main_window = cssed_window_get_window_widget( window );

	gtk_window_set_transient_for (GTK_WINDOW (voice_family_value_dialog),
				      GTK_WINDOW (main_window));

	dialog_vbox = GTK_DIALOG (voice_family_value_dialog)->vbox;
	gtk_widget_show (dialog_vbox);

	vbox = gtk_vbox_new (FALSE, 0);
	gtk_widget_show (vbox);
	gtk_box_pack_start (GTK_BOX (dialog_vbox), vbox, TRUE, TRUE, 0);

	hbox_generic = gtk_hbox_new (TRUE, 0);
	gtk_widget_show (hbox_generic);
	gtk_box_pack_start (GTK_BOX (vbox), hbox_generic, FALSE, FALSE, 5);

	label_generic = gtk_label_new (_("Generic voice"));
	gtk_widget_show (label_generic);
	gtk_box_pack_start (GTK_BOX (hbox_generic), label_generic, TRUE, TRUE,
			    0);
	gtk_label_set_justify (GTK_LABEL (label_generic), GTK_JUSTIFY_LEFT);

	combo_generic = gtk_combo_new ();
	g_object_set_data (G_OBJECT (GTK_COMBO (combo_generic)->popwin),
			   "GladeParentKey", combo_generic);
	gtk_widget_show (combo_generic);
	gtk_box_pack_start (GTK_BOX (hbox_generic), combo_generic, TRUE, TRUE,
			    5);

	combo_generic_items =
		g_list_append (combo_generic_items, (gpointer) "male");
	combo_generic_items =
		g_list_append (combo_generic_items, (gpointer) "female");
	combo_generic_items =
		g_list_append (combo_generic_items, (gpointer) "child");

	gtk_combo_set_popdown_strings (GTK_COMBO (combo_generic),
				       combo_generic_items);
	g_list_free (combo_generic_items);

	combo_entry_generic = GTK_COMBO (combo_generic)->entry;
	gtk_widget_show (combo_entry_generic);
	gtk_entry_set_text (GTK_ENTRY (combo_entry_generic), "male");
	gtk_editable_set_editable (GTK_EDITABLE (combo_entry_generic), FALSE);

	hbox_specific = gtk_hbox_new (TRUE, 0);
	gtk_widget_show (hbox_specific);
	gtk_box_pack_start (GTK_BOX (vbox), hbox_specific, FALSE, FALSE, 5);

	label_specific = gtk_label_new (_("Specific voice"));
	gtk_widget_show (label_specific);
	gtk_box_pack_start (GTK_BOX (hbox_specific), label_specific, TRUE,
			    TRUE, 10);
	gtk_label_set_justify (GTK_LABEL (label_specific), GTK_JUSTIFY_LEFT);

	entry_specific_voice = gtk_entry_new ();
	gtk_widget_show (entry_specific_voice);
	gtk_box_pack_start (GTK_BOX (hbox_specific), entry_specific_voice,
			    TRUE, TRUE, 5);
	gtk_editable_set_editable (GTK_EDITABLE (entry_specific_voice), TRUE);

	hbox_voice_set = gtk_hbox_new (TRUE, 0);
	gtk_widget_show (hbox_voice_set);
	gtk_box_pack_start (GTK_BOX (vbox), hbox_voice_set, FALSE, FALSE, 0);

	checkbutton_voice_set =
		gtk_check_button_new_with_mnemonic (_("Use voice set"));
	gtk_widget_show (checkbutton_voice_set);
	gtk_box_pack_start (GTK_BOX (hbox_voice_set), checkbutton_voice_set,
			    TRUE, TRUE, 5);

	hbuttonbox = gtk_hbutton_box_new ();
	gtk_widget_show (hbuttonbox);
	gtk_box_pack_start (GTK_BOX (hbox_voice_set), hbuttonbox, TRUE, TRUE,
			    0);
	gtk_button_box_set_layout (GTK_BUTTON_BOX (hbuttonbox),
				   GTK_BUTTONBOX_END);

	button_voice_family_value_dialog_add =
		gtk_button_new_from_stock ("gtk-add");
	gtk_widget_show (button_voice_family_value_dialog_add);
	gtk_container_add (GTK_CONTAINER (hbuttonbox),
			   button_voice_family_value_dialog_add);
	gtk_container_set_border_width (GTK_CONTAINER
					(button_voice_family_value_dialog_add),
					5);
	GTK_WIDGET_SET_FLAGS (button_voice_family_value_dialog_add,
			      GTK_CAN_DEFAULT);
	gtk_widget_add_accelerator (button_voice_family_value_dialog_add,
				    "clicked", accel_group, GDK_a,
				    GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);

	button_voice_family_value_dialog_del =
		gtk_button_new_from_stock ("gtk-remove");
	gtk_widget_show (button_voice_family_value_dialog_del);
	gtk_container_add (GTK_CONTAINER (hbuttonbox),
			   button_voice_family_value_dialog_del);
	gtk_container_set_border_width (GTK_CONTAINER
					(button_voice_family_value_dialog_del),
					5);
	GTK_WIDGET_SET_FLAGS (button_voice_family_value_dialog_del,
			      GTK_CAN_DEFAULT);
	gtk_widget_add_accelerator (button_voice_family_value_dialog_del,
				    "clicked", accel_group, GDK_q,
				    GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);
	gtk_widget_add_accelerator (button_voice_family_value_dialog_del,
				    "clicked", accel_group, GDK_d,
				    GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);

	scrolledwindow = gtk_scrolled_window_new (NULL, NULL);
	gtk_widget_show (scrolledwindow);
	gtk_box_pack_start (GTK_BOX (vbox), scrolledwindow, TRUE, TRUE, 0);
	gtk_container_set_border_width (GTK_CONTAINER (scrolledwindow), 5);
	gtk_widget_set_size_request (scrolledwindow, -1, 125);
	// 
	store = gtk_list_store_new (N_COLS, G_TYPE_STRING, G_TYPE_STRING);
	treeview = gtk_tree_view_new_with_model (GTK_TREE_MODEL (store));

	gv_renderer = gtk_cell_renderer_text_new ();
	sv_renderer = gtk_cell_renderer_text_new ();

	col_generic_voice =
		gtk_tree_view_column_new_with_attributes (_("Generic voice"),
							  gv_renderer,
							  "text", COL_GENERIC,
							  NULL);
	col_specific_voice =
		gtk_tree_view_column_new_with_attributes (_("Specific voice"),
							  sv_renderer, "text",
							  COL_SPECIFIC, NULL);
	gtk_tree_view_insert_column (GTK_TREE_VIEW (treeview),
				     col_generic_voice, COL_GENERIC);
	gtk_tree_view_insert_column (GTK_TREE_VIEW (treeview),
				     col_specific_voice, COL_SPECIFIC);

	gtk_widget_show (treeview);
	gtk_container_add (GTK_CONTAINER (scrolledwindow), treeview);
	gtk_tree_view_set_headers_visible (GTK_TREE_VIEW (treeview), TRUE);

	dialog_action_area =
		GTK_DIALOG (voice_family_value_dialog)->action_area;
	gtk_widget_show (dialog_action_area);
	gtk_button_box_set_layout (GTK_BUTTON_BOX (dialog_action_area),
				   GTK_BUTTONBOX_END);

	helpbutton = gtk_button_new_from_stock ("gtk-help");
	gtk_widget_show (helpbutton);
	gtk_dialog_add_action_widget (GTK_DIALOG (voice_family_value_dialog),
				      helpbutton, GTK_RESPONSE_HELP);
	GTK_WIDGET_SET_FLAGS (helpbutton, GTK_CAN_DEFAULT);
	gtk_widget_add_accelerator (helpbutton, "clicked", accel_group,
				    GDK_y, GDK_CONTROL_MASK,
				    GTK_ACCEL_VISIBLE);
	gtk_widget_add_accelerator (helpbutton, "clicked", accel_group, GDK_h,
				    GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);

	cancelbutton = gtk_button_new_from_stock ("gtk-cancel");
	gtk_widget_show (cancelbutton);
	gtk_dialog_add_action_widget (GTK_DIALOG (voice_family_value_dialog),
				      cancelbutton, GTK_RESPONSE_CANCEL);
	GTK_WIDGET_SET_FLAGS (cancelbutton, GTK_CAN_DEFAULT);
	gtk_widget_add_accelerator (cancelbutton, "clicked", accel_group,
				    GDK_c, GDK_CONTROL_MASK,
				    GTK_ACCEL_VISIBLE);

	okbutton = gtk_button_new_from_stock ("gtk-ok");
	gtk_widget_show (okbutton);
	gtk_dialog_add_action_widget (GTK_DIALOG (voice_family_value_dialog),
				      okbutton, GTK_RESPONSE_OK);
	GTK_WIDGET_SET_FLAGS (okbutton, GTK_CAN_DEFAULT);
	gtk_widget_add_accelerator (okbutton, "clicked", accel_group,
				    GDK_o, GDK_CONTROL_MASK,
				    GTK_ACCEL_VISIBLE);
				    
	g_signal_connect ((gpointer) button_voice_family_value_dialog_add,
			  "clicked", G_CALLBACK (on_widget_change_set_toggle_active),
			  checkbutton_voice_set );
	
	if (connect_callbacks)
	{
		g_signal_connect ((gpointer) voice_family_value_dialog,
				  "close", G_CALLBACK (on_css_dialog_close),
				  data);

		g_signal_connect ((gpointer) cancelbutton, "clicked",
				  G_CALLBACK
				  (on_css_dialog_cancel_button_clicked),
				  data);
		g_signal_connect ((gpointer) okbutton, "clicked",
				  G_CALLBACK
				  (on_voice_family_value_dialog_okbutton_clicked),
				  data);
	}
	g_signal_connect ((gpointer) button_voice_family_value_dialog_add,
			  "clicked",
			  G_CALLBACK
			  (on_button_voice_family_value_dialog_add_clicked),
			  store);
	g_signal_connect ((gpointer) button_voice_family_value_dialog_del,
			  "clicked",
			  G_CALLBACK
			  (on_button_voice_family_value_dialog_del_clicked),
			  store);

	GLADE_HOOKUP_OBJECT_NO_REF (voice_family_value_dialog, voice_family_value_dialog,
														  "voice_family_value_dialog");
	GLADE_HOOKUP_OBJECT (voice_family_value_dialog, label_generic, "label_generic");
	GLADE_HOOKUP_OBJECT (voice_family_value_dialog, combo_generic, "combo_generic");
	GLADE_HOOKUP_OBJECT (voice_family_value_dialog, combo_entry_generic, "combo_entry_generic");
	GLADE_HOOKUP_OBJECT (voice_family_value_dialog, label_specific, "label_specific");
	GLADE_HOOKUP_OBJECT (voice_family_value_dialog, entry_specific_voice, "entry_specific_voice");
	GLADE_HOOKUP_OBJECT (voice_family_value_dialog, checkbutton_voice_set, "checkbutton_voice_set");
	GLADE_HOOKUP_OBJECT (voice_family_value_dialog, button_voice_family_value_dialog_add, 
													"button_voice_family_value_dialog_add");
	GLADE_HOOKUP_OBJECT (voice_family_value_dialog,  button_voice_family_value_dialog_del,
													"button_voice_family_value_dialog_del");
	GLADE_HOOKUP_OBJECT (voice_family_value_dialog, scrolledwindow, "scrolledwindow");
	GLADE_HOOKUP_OBJECT (voice_family_value_dialog, treeview, "treeview");
	GLADE_HOOKUP_OBJECT (voice_family_value_dialog, helpbutton, "helpbutton");
	GLADE_HOOKUP_OBJECT (voice_family_value_dialog, cancelbutton, "cancelbutton");
	GLADE_HOOKUP_OBJECT (voice_family_value_dialog, okbutton, "okbutton");

	gtk_window_add_accel_group (GTK_WINDOW (voice_family_value_dialog), accel_group);

	return voice_family_value_dialog;
}

GtkWidget*
create_border_width_style_color_value_dialog (CssedWindow* window,
											  gchar* previous_string,
											  gboolean add_previous_string,
											  gboolean connect_callbacks)
{
	GtkWidget *border_width_style_color_value_dialog;
	GtkWidget *dialog_vbox;
	GtkWidget *vbox;
	GtkWidget *hbox_length;
	GtkWidget *checkbutton_length;
	GtkWidget *combo_length;
	GList *combo_length_items = NULL;
	GtkWidget *combo_entry_length;
	GtkObject *spinbutton_length_adj;
	GtkWidget *spinbutton_length;
	GtkWidget *alignment_length_units;
	GtkWidget *combo_length_units;
	GList *combo_lengthunits_items = NULL;
	GtkWidget *combo_entry_length_units;
	GtkWidget *hbox_color;
	GtkWidget *checkbutton_color;
	GtkWidget *combo_color;
	GList *combo_color_items = NULL;
	GtkWidget *combo_entry_color;
	GtkWidget *colorselection;
	GtkWidget *hbox_style;
	GtkWidget *checkbutton_style;
	GtkWidget *combo_style;
	GList *combo_style_items = NULL;
	GtkWidget *combo_entry_style;
	GtkWidget *dialog_action_area;
	GtkWidget *helpbutton;
	GtkWidget *cancelbutton;
	GtkWidget *okbutton;
	GtkAccelGroup *accel_group;
	GtkWidget *main_window;
	CssedCssDialogsData *data = NULL;

	if (connect_callbacks)
	{
		data = g_malloc (sizeof (CssedCssDialogsData));
		data->main_window = window;
		data->previous_string = previous_string;
		data->add_property_string = add_previous_string;
	}

	accel_group = gtk_accel_group_new ();

	border_width_style_color_value_dialog = gtk_dialog_new ();
	gtk_window_set_title (GTK_WINDOW
			      (border_width_style_color_value_dialog),
			      previous_string);
	gtk_window_set_modal (GTK_WINDOW
			      (border_width_style_color_value_dialog), TRUE);

	main_window = cssed_window_get_window_widget( window );

	gtk_window_set_transient_for (GTK_WINDOW
				      (border_width_style_color_value_dialog),
				      GTK_WINDOW (main_window));

	dialog_vbox =
		GTK_DIALOG (border_width_style_color_value_dialog)->vbox;
	gtk_widget_show (dialog_vbox);

	vbox = gtk_vbox_new (FALSE, 0);
	gtk_widget_show (vbox);
	gtk_box_pack_start (GTK_BOX (dialog_vbox), vbox, TRUE, TRUE, 0);

	hbox_length = gtk_hbox_new (FALSE, 0);
	gtk_widget_show (hbox_length);
	gtk_box_pack_start (GTK_BOX (vbox), hbox_length, TRUE, TRUE, 5);

	checkbutton_length =
		gtk_check_button_new_with_mnemonic (_("Border width"));
	gtk_widget_show (checkbutton_length);
	gtk_box_pack_start (GTK_BOX (hbox_length), checkbutton_length, TRUE,
			    TRUE, 5);

	combo_length = gtk_combo_new ();
	g_object_set_data (G_OBJECT (GTK_COMBO (combo_length)->popwin),
			   "GladeParentKey", combo_length);
	gtk_widget_show (combo_length);
	gtk_box_pack_start (GTK_BOX (hbox_length), combo_length, FALSE, FALSE,
			    5);
	combo_length_items =
		g_list_append (combo_length_items,
			       (gpointer) CSS_STR_USE_LENGTH);
	combo_length_items =
		g_list_append (combo_length_items, (gpointer) "thin");
	combo_length_items =
		g_list_append (combo_length_items, (gpointer) "medium");
	combo_length_items =
		g_list_append (combo_length_items, (gpointer) "thick");
	gtk_combo_set_popdown_strings (GTK_COMBO (combo_length),
				       combo_length_items);
	g_list_free (combo_length_items);

	combo_entry_length = GTK_COMBO (combo_length)->entry;
	gtk_widget_show (combo_entry_length);
	gtk_entry_set_text (GTK_ENTRY (combo_entry_length),
			    CSS_STR_USE_LENGTH);

	spinbutton_length_adj =
		gtk_adjustment_new (1, -35565, 35565, 1, 10, 10);
	spinbutton_length =
		gtk_spin_button_new (GTK_ADJUSTMENT (spinbutton_length_adj),
				     1, 0);
	gtk_widget_show (spinbutton_length);
	gtk_box_pack_start (GTK_BOX (hbox_length), spinbutton_length, FALSE,
			    FALSE, 5);

	alignment_length_units = gtk_alignment_new (0.5, 0.5, 1, 1);
	gtk_widget_show (alignment_length_units);
	gtk_box_pack_start (GTK_BOX (hbox_length), alignment_length_units,
			    FALSE, FALSE, 0);
	// 
	combo_length_units = gtk_combo_new ();
	g_object_set_data (G_OBJECT (GTK_COMBO (combo_length_units)->popwin),
			   "GladeParentKey", combo_length_units);
	gtk_widget_show (combo_length_units);
	gtk_container_add (GTK_CONTAINER (alignment_length_units),
			   combo_length_units);
	gtk_widget_set_size_request (combo_length_units, 60, -1);
	gtk_container_set_border_width (GTK_CONTAINER (combo_length_units),
					5);
	combo_lengthunits_items =
		g_list_append (combo_lengthunits_items, (gpointer) "em");
	combo_lengthunits_items =
		g_list_append (combo_lengthunits_items, (gpointer) "ex");
	combo_lengthunits_items =
		g_list_append (combo_lengthunits_items, (gpointer) "px");
	combo_lengthunits_items =
		g_list_append (combo_lengthunits_items, (gpointer) "in");
	combo_lengthunits_items =
		g_list_append (combo_lengthunits_items, (gpointer) "cm");
	combo_lengthunits_items =
		g_list_append (combo_lengthunits_items, (gpointer) "mm");
	combo_lengthunits_items =
		g_list_append (combo_lengthunits_items, (gpointer) "pt");
	combo_lengthunits_items =
		g_list_append (combo_lengthunits_items, (gpointer) "pc");
	gtk_combo_set_popdown_strings (GTK_COMBO (combo_length_units),
				       combo_lengthunits_items);
	g_list_free (combo_lengthunits_items);

	combo_entry_length_units = GTK_COMBO (combo_length_units)->entry;
	gtk_widget_show (combo_entry_length_units);
	gtk_editable_set_editable (GTK_EDITABLE (combo_entry_length_units),
				   FALSE);
	gtk_entry_set_text (GTK_ENTRY (combo_entry_length_units), "px");

	hbox_color = gtk_hbox_new (TRUE, 0);
	gtk_widget_show (hbox_color);
	gtk_box_pack_start (GTK_BOX (vbox), hbox_color, TRUE, TRUE, 5);

	checkbutton_color =
		gtk_check_button_new_with_mnemonic (_("Border color"));
	gtk_widget_show (checkbutton_color);
	gtk_box_pack_start (GTK_BOX (hbox_color), checkbutton_color, TRUE,
			    TRUE, 5);

	combo_color = gtk_combo_new ();
	g_object_set_data (G_OBJECT (GTK_COMBO (combo_color)->popwin),
			   "GladeParentKey", combo_color);
	gtk_widget_show (combo_color);
	gtk_box_pack_start (GTK_BOX (hbox_color), combo_color, TRUE, TRUE, 5);
	combo_color_items =
		g_list_append (combo_color_items,
			       (gpointer) CSS_STR_USE_COLORSEL);
	combo_color_items =
		g_list_append (combo_color_items, (gpointer) "aqua");
	combo_color_items =
		g_list_append (combo_color_items, (gpointer) "black");
	combo_color_items =
		g_list_append (combo_color_items, (gpointer) "blue");
	combo_color_items =
		g_list_append (combo_color_items, (gpointer) "fuchsia");
	combo_color_items =
		g_list_append (combo_color_items, (gpointer) "gray");
	combo_color_items =
		g_list_append (combo_color_items, (gpointer) "green");
	combo_color_items =
		g_list_append (combo_color_items, (gpointer) "lime");
	combo_color_items =
		g_list_append (combo_color_items, (gpointer) "maroon ");
	combo_color_items =
		g_list_append (combo_color_items, (gpointer) "navy");
	combo_color_items =
		g_list_append (combo_color_items, (gpointer) "olive");
	combo_color_items =
		g_list_append (combo_color_items, (gpointer) "purple");
	combo_color_items =
		g_list_append (combo_color_items, (gpointer) "red");
	combo_color_items =
		g_list_append (combo_color_items, (gpointer) "silver");
	combo_color_items =
		g_list_append (combo_color_items, (gpointer) "teal");
	combo_color_items =
		g_list_append (combo_color_items, (gpointer) "white");
	combo_color_items =
		g_list_append (combo_color_items, (gpointer) "yellow");
	combo_color_items =
		g_list_append (combo_color_items, (gpointer) "ActiveBorder");
	combo_color_items =
		g_list_append (combo_color_items, (gpointer) "ActiveCaption");
	combo_color_items =
		g_list_append (combo_color_items, (gpointer) "AppWorkspace");
	combo_color_items =
		g_list_append (combo_color_items, (gpointer) "Background");
	combo_color_items =
		g_list_append (combo_color_items, (gpointer) "ButtonFace");
	combo_color_items =
		g_list_append (combo_color_items,
			       (gpointer) "ButtonHighlight");
	combo_color_items =
		g_list_append (combo_color_items, (gpointer) "ButtonShadow");
	combo_color_items =
		g_list_append (combo_color_items, (gpointer) "ButtonText");
	combo_color_items =
		g_list_append (combo_color_items, (gpointer) "CaptionText");
	combo_color_items =
		g_list_append (combo_color_items, (gpointer) "GrayText");
	combo_color_items =
		g_list_append (combo_color_items, (gpointer) "Highlight");
	combo_color_items =
		g_list_append (combo_color_items, (gpointer) "HighlightText");
	combo_color_items =
		g_list_append (combo_color_items, (gpointer) "InactiveBorder");
	combo_color_items =	g_list_append (combo_color_items,
			       (gpointer) "InactiveCaption");
	combo_color_items =	g_list_append (combo_color_items,
			       (gpointer) "InactiveCaptionText");
	combo_color_items =
		g_list_append (combo_color_items,
			       (gpointer) "InfoBackground");
	combo_color_items =
		g_list_append (combo_color_items, (gpointer) "InfoText");
	combo_color_items =
		g_list_append (combo_color_items, (gpointer) "Menu");
	combo_color_items =
		g_list_append (combo_color_items, (gpointer) "MenuText");
	combo_color_items =
		g_list_append (combo_color_items, (gpointer) "Scrollbar");
	combo_color_items =
		g_list_append (combo_color_items,
			       (gpointer) "ThreeDDarkShadow");
	combo_color_items =
		g_list_append (combo_color_items, (gpointer) "ThreeDFace");
	combo_color_items =
		g_list_append (combo_color_items,
			       (gpointer) "ThreeDHighlight");
	combo_color_items =
		g_list_append (combo_color_items,
			       (gpointer) "ThreeDLightShadow");
	combo_color_items =
		g_list_append (combo_color_items, (gpointer) "ThreeDShadow");
	combo_color_items =
		g_list_append (combo_color_items, (gpointer) "Window");
	combo_color_items =
		g_list_append (combo_color_items, (gpointer) "WindowFrame");
	combo_color_items =
		g_list_append (combo_color_items, (gpointer) "WindowText");
	gtk_combo_set_popdown_strings (GTK_COMBO (combo_color),
				       combo_color_items);
	g_list_free (combo_color_items);

	combo_entry_color = GTK_COMBO (combo_color)->entry;
	gtk_widget_show (combo_entry_color);
	gtk_editable_set_editable (GTK_EDITABLE (combo_entry_color), FALSE);
	gtk_entry_set_text (GTK_ENTRY (combo_entry_color),
			    CSS_STR_USE_COLORSEL);

	colorselection = gtk_color_selection_new ();
	gtk_widget_show (colorselection);
	gtk_box_pack_start (GTK_BOX (vbox), colorselection, TRUE, TRUE, 0);
	gtk_container_set_border_width (GTK_CONTAINER (colorselection), 10);
	gtk_color_selection_set_has_opacity_control (GTK_COLOR_SELECTION
						     (colorselection), FALSE);
	gtk_color_selection_set_has_palette (GTK_COLOR_SELECTION
					     (colorselection), TRUE);

	hbox_style = gtk_hbox_new (TRUE, 0);
	gtk_widget_show (hbox_style);
	gtk_box_pack_start (GTK_BOX (vbox), hbox_style, TRUE, TRUE, 0);

	checkbutton_style =
		gtk_check_button_new_with_mnemonic (_("Border style"));
	gtk_widget_show (checkbutton_style);
	gtk_box_pack_start (GTK_BOX (hbox_style), checkbutton_style, TRUE,
			    TRUE, 5);

	combo_style = gtk_combo_new ();
	g_object_set_data (G_OBJECT (GTK_COMBO (combo_style)->popwin),
			   "GladeParentKey", combo_style);
	gtk_widget_show (combo_style);
	gtk_box_pack_start (GTK_BOX (hbox_style), combo_style, TRUE, TRUE, 5);
	combo_style_items =
		g_list_append (combo_style_items, (gpointer) "none");
	combo_style_items =
		g_list_append (combo_style_items, (gpointer) "hidden ");
	combo_style_items =
		g_list_append (combo_style_items, (gpointer) "dotted ");
	combo_style_items =
		g_list_append (combo_style_items, (gpointer) "dashed ");
	combo_style_items =
		g_list_append (combo_style_items, (gpointer) "solid ");
	combo_style_items =
		g_list_append (combo_style_items, (gpointer) "double ");
	combo_style_items =
		g_list_append (combo_style_items, (gpointer) "groove ");
	combo_style_items =
		g_list_append (combo_style_items, (gpointer) "ridge ");
	combo_style_items =
		g_list_append (combo_style_items, (gpointer) "inset ");
	combo_style_items =
		g_list_append (combo_style_items, (gpointer) "outset");
	gtk_combo_set_popdown_strings (GTK_COMBO (combo_style),
				       combo_style_items);
	g_list_free (combo_style_items);

	combo_entry_style = GTK_COMBO (combo_style)->entry;
	gtk_widget_show (combo_entry_style);
	gtk_entry_set_text (GTK_ENTRY (combo_entry_style), "none");

	dialog_action_area =
		GTK_DIALOG (border_width_style_color_value_dialog)->
		action_area;
	gtk_widget_show (dialog_action_area);
	gtk_button_box_set_layout (GTK_BUTTON_BOX (dialog_action_area),
				   GTK_BUTTONBOX_END);

	helpbutton = gtk_button_new_from_stock ("gtk-help");
	gtk_widget_show (helpbutton);
	gtk_dialog_add_action_widget (GTK_DIALOG
				      (border_width_style_color_value_dialog),
				      helpbutton, GTK_RESPONSE_HELP);
	GTK_WIDGET_SET_FLAGS (helpbutton, GTK_CAN_DEFAULT);
	gtk_widget_add_accelerator (helpbutton, "clicked", accel_group,
				    GDK_y, GDK_CONTROL_MASK,
				    GTK_ACCEL_VISIBLE);
	gtk_widget_add_accelerator (helpbutton, "clicked", accel_group, GDK_h,
				    GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);

	cancelbutton = gtk_button_new_from_stock ("gtk-cancel");
	gtk_widget_show (cancelbutton);
	gtk_dialog_add_action_widget (GTK_DIALOG
				      (border_width_style_color_value_dialog),
				      cancelbutton, GTK_RESPONSE_CANCEL);
	GTK_WIDGET_SET_FLAGS (cancelbutton, GTK_CAN_DEFAULT);
	gtk_widget_add_accelerator (cancelbutton, "clicked", accel_group,
				    GDK_c, GDK_CONTROL_MASK,
				    GTK_ACCEL_VISIBLE);

	okbutton = gtk_button_new_from_stock ("gtk-ok");
	gtk_widget_show (okbutton);
	gtk_dialog_add_action_widget (GTK_DIALOG
				      (border_width_style_color_value_dialog),
				      okbutton, GTK_RESPONSE_OK);
	GTK_WIDGET_SET_FLAGS (okbutton, GTK_CAN_DEFAULT);
	gtk_widget_add_accelerator (okbutton, "clicked", accel_group,
				    GDK_o, GDK_CONTROL_MASK,
				    GTK_ACCEL_VISIBLE);
					
	g_signal_connect ((gpointer) combo_entry_length_units,
			  "changed", G_CALLBACK (on_css_dialog_unit_changed),
			  spinbutton_length);
			  
	/* to update the checkboxes when a related widget is changed */				    
	g_signal_connect ((gpointer) spinbutton_length,
			  "value-changed", G_CALLBACK (on_widget_change_set_toggle_active),
			  checkbutton_length );
	g_signal_connect ((gpointer) combo_entry_length,
			  "changed", G_CALLBACK (on_widget_change_set_toggle_active),
			  checkbutton_length);
	g_signal_connect ((gpointer) combo_entry_length_units,
			  "changed", G_CALLBACK (on_widget_change_set_toggle_active),
			  checkbutton_length);	
			  
	g_signal_connect ((gpointer) colorselection,
			  "color-changed", G_CALLBACK (on_widget_change_set_toggle_active),
			  checkbutton_color );
	g_signal_connect ((gpointer) combo_entry_color,
			  "changed", G_CALLBACK (on_widget_change_set_toggle_active),
			  checkbutton_color);	
			  
	g_signal_connect ((gpointer) combo_entry_style,
			  "changed", G_CALLBACK (on_widget_change_set_toggle_active),
			  checkbutton_style);	  
			  		  
			  
	if (connect_callbacks)
	{
		g_signal_connect ((gpointer)
				  border_width_style_color_value_dialog,
				  "close", G_CALLBACK (on_css_dialog_close),
				  data);

		g_signal_connect ((gpointer) helpbutton, "clicked",
				  G_CALLBACK
				  (on_border_width_style_color_value_dialog_helpbutton_clicked),
				  NULL);
		g_signal_connect ((gpointer) cancelbutton, "clicked",
				  G_CALLBACK
				  (on_css_dialog_cancel_button_clicked),
				  data);
		g_signal_connect ((gpointer) okbutton, "clicked",
				  G_CALLBACK
				  (on_border_width_style_color_value_dialog_okbutton_clicked),
				  data);
	}

	GLADE_HOOKUP_OBJECT_NO_REF (border_width_style_color_value_dialog,
				    border_width_style_color_value_dialog,
				    "border_width_style_color_value_dialog");
	GLADE_HOOKUP_OBJECT (border_width_style_color_value_dialog,
			     checkbutton_length, "checkbutton_length");
	GLADE_HOOKUP_OBJECT (border_width_style_color_value_dialog,
			     combo_length, "combo_length");
	GLADE_HOOKUP_OBJECT (border_width_style_color_value_dialog,
			     combo_entry_length, "combo_entry_length");
	GLADE_HOOKUP_OBJECT (border_width_style_color_value_dialog,
			     spinbutton_length, "spinbutton_length");
	GLADE_HOOKUP_OBJECT (border_width_style_color_value_dialog,
			     combo_entry_length, "combo_entry_length");
	GLADE_HOOKUP_OBJECT (border_width_style_color_value_dialog,
			     combo_entry_length_units,
			     "combo_entry_length_units");
	GLADE_HOOKUP_OBJECT (border_width_style_color_value_dialog,
			     checkbutton_color, "checkbutton_color");
	GLADE_HOOKUP_OBJECT (border_width_style_color_value_dialog,
			     combo_color, "combo_color");
	GLADE_HOOKUP_OBJECT (border_width_style_color_value_dialog,
			     combo_entry_color, "combo_entry_color");
	GLADE_HOOKUP_OBJECT (border_width_style_color_value_dialog,
			     colorselection, "colorselection");
	GLADE_HOOKUP_OBJECT (border_width_style_color_value_dialog,
			     checkbutton_style, "checkbutton_style");
	GLADE_HOOKUP_OBJECT (border_width_style_color_value_dialog,
			     combo_style, "combo_style");
	GLADE_HOOKUP_OBJECT (border_width_style_color_value_dialog,
			     combo_entry_style, "combo_entry_style");
	GLADE_HOOKUP_OBJECT_NO_REF (border_width_style_color_value_dialog,
				    dialog_action_area, "dialog_action_area");
	GLADE_HOOKUP_OBJECT (border_width_style_color_value_dialog,
			     helpbutton, "helpbutton");
	GLADE_HOOKUP_OBJECT (border_width_style_color_value_dialog,
			     cancelbutton, "cancelbutton");
	GLADE_HOOKUP_OBJECT (border_width_style_color_value_dialog, okbutton,
			     "okbutton");

	gtk_window_add_accel_group (GTK_WINDOW
				    (border_width_style_color_value_dialog),
				    accel_group);

	return border_width_style_color_value_dialog;
}

GtkWidget *
create_color_selector_dialog (CssedWindow * window,
			      gchar * previous_string,
			      gboolean add_previous_string,
			      gboolean connect_callbacks)
{
	GtkWidget *color_selector_dialog;
	GtkWidget *dialog_vbox;
	GtkWidget *vbox;
	GtkWidget *hbox_named;
	GtkWidget *radiobutton_named;
	GSList *radiobutton_named_group = NULL;
	GtkWidget *combo_named;
	GList *combo_named_items = NULL;
	GtkWidget *combo_entry_named;
	GtkWidget *radiobutton_colorsel;
	GtkWidget *hbox_colorsel;
	GtkWidget *colorselection;
	GtkWidget *dialog_action_area;
	GtkWidget *helpbutton;
	GtkWidget *cancelbutton;
	GtkWidget *okbutton;
	GtkAccelGroup *accel_group;
	GtkWidget *main_window;
	CssedCssDialogsData *data = NULL;

	if (connect_callbacks)
	{
		data = g_malloc (sizeof (CssedCssDialogsData));
		data->main_window = window;
		data->previous_string = previous_string;
		data->add_property_string = add_previous_string;
	}

	accel_group = gtk_accel_group_new ();

	color_selector_dialog = gtk_dialog_new ();
	gtk_window_set_title (GTK_WINDOW (color_selector_dialog),
			      previous_string);
	gtk_window_set_modal (GTK_WINDOW (color_selector_dialog), TRUE);

	main_window = cssed_window_get_window_widget( window );

	gtk_window_set_transient_for (GTK_WINDOW (color_selector_dialog),
				      GTK_WINDOW (main_window));

	dialog_vbox = GTK_DIALOG (color_selector_dialog)->vbox;
	gtk_widget_show (dialog_vbox);

	vbox = gtk_vbox_new (FALSE, 0);
	gtk_widget_show (vbox);
	gtk_box_pack_start (GTK_BOX (dialog_vbox), vbox, TRUE, FALSE, 0);

	hbox_named = gtk_hbox_new (TRUE, 0);
	gtk_widget_show (hbox_named);
	gtk_box_pack_start (GTK_BOX (vbox), hbox_named, TRUE, TRUE, 0);

	radiobutton_named =
		gtk_radio_button_new_with_mnemonic (NULL,
						    _("Use named color"));
	gtk_widget_show (radiobutton_named);
	gtk_box_pack_start (GTK_BOX (hbox_named), radiobutton_named, TRUE,
			    TRUE, 5);
	gtk_radio_button_set_group (GTK_RADIO_BUTTON (radiobutton_named),
				    radiobutton_named_group);
	radiobutton_named_group =
		gtk_radio_button_get_group (GTK_RADIO_BUTTON
					    (radiobutton_named));

	combo_named = gtk_combo_new ();
	g_object_set_data (G_OBJECT (GTK_COMBO (combo_named)->popwin),
			   "GladeParentKey", combo_named);
	gtk_widget_show (combo_named);
	gtk_box_pack_start (GTK_BOX (hbox_named), combo_named, TRUE, TRUE, 5);
	combo_named_items =
		g_list_append (combo_named_items, (gpointer) "aqua");
	combo_named_items =
		g_list_append (combo_named_items, (gpointer) "black");
	combo_named_items =
		g_list_append (combo_named_items, (gpointer) "blue");
	combo_named_items =
		g_list_append (combo_named_items, (gpointer) "fuchsia");
	combo_named_items =
		g_list_append (combo_named_items, (gpointer) "gray");
	combo_named_items =
		g_list_append (combo_named_items, (gpointer) "green");
	combo_named_items =
		g_list_append (combo_named_items, (gpointer) "lime");
	combo_named_items =
		g_list_append (combo_named_items, (gpointer) "maroon");
	combo_named_items =
		g_list_append (combo_named_items, (gpointer) "navy");
	combo_named_items =
		g_list_append (combo_named_items, (gpointer) "olive");
	combo_named_items =
		g_list_append (combo_named_items, (gpointer) "purple");
	combo_named_items =
		g_list_append (combo_named_items, (gpointer) "red");
	combo_named_items =
		g_list_append (combo_named_items, (gpointer) "silver");
	combo_named_items =
		g_list_append (combo_named_items, (gpointer) "teal");
	combo_named_items =
		g_list_append (combo_named_items, (gpointer) "white");
	combo_named_items =
		g_list_append (combo_named_items, (gpointer) "yellow");
	combo_named_items =
		g_list_append (combo_named_items, (gpointer) "ActiveBorder");
	combo_named_items =
		g_list_append (combo_named_items, (gpointer) "ActiveCaption");
	combo_named_items =
		g_list_append (combo_named_items, (gpointer) "AppWorkspace");
	combo_named_items =
		g_list_append (combo_named_items, (gpointer) "Background");
	combo_named_items =
		g_list_append (combo_named_items, (gpointer) "ButtonFace");
	combo_named_items =
		g_list_append (combo_named_items,
			       (gpointer) "ButtonHighlight");
	combo_named_items =
		g_list_append (combo_named_items, (gpointer) "ButtonShadow");
	combo_named_items =
		g_list_append (combo_named_items, (gpointer) "ButtonText");
	combo_named_items =
		g_list_append (combo_named_items, (gpointer) "CaptionText");
	combo_named_items =
		g_list_append (combo_named_items, (gpointer) "GrayText");
	combo_named_items =
		g_list_append (combo_named_items, (gpointer) "Highlight");
	combo_named_items =
		g_list_append (combo_named_items, (gpointer) "HighlightText");
	combo_named_items =
		g_list_append (combo_named_items,
			       (gpointer) "InactiveBorder");
	combo_named_items =
		g_list_append (combo_named_items,
			       (gpointer) "InactiveCaption");
	combo_named_items =
		g_list_append (combo_named_items,
			       (gpointer) "InactiveCaptionText");
	combo_named_items =
		g_list_append (combo_named_items,
			       (gpointer) "InfoBackground");
	combo_named_items =
		g_list_append (combo_named_items, (gpointer) "InfoText");
	combo_named_items =
		g_list_append (combo_named_items, (gpointer) "Menu");
	combo_named_items =
		g_list_append (combo_named_items, (gpointer) "MenuText");
	combo_named_items =
		g_list_append (combo_named_items, (gpointer) "Scrollbar");
	combo_named_items =
		g_list_append (combo_named_items,
			       (gpointer) "ThreeDDarkShadow");
	combo_named_items =
		g_list_append (combo_named_items, (gpointer) "ThreeDFace");
	combo_named_items =
		g_list_append (combo_named_items,
			       (gpointer) "ThreeDHighlight");
	combo_named_items =
		g_list_append (combo_named_items,
			       (gpointer) "ThreeDLightShadow");
	combo_named_items =
		g_list_append (combo_named_items, (gpointer) "ThreeDShadow");
	combo_named_items =
		g_list_append (combo_named_items, (gpointer) "Window");
	combo_named_items =
		g_list_append (combo_named_items, (gpointer) "WindowFrame");
	combo_named_items =
		g_list_append (combo_named_items, (gpointer) "WindowText");
	combo_named_items =
		g_list_append (combo_named_items, (gpointer) "transparent");
	gtk_combo_set_popdown_strings (GTK_COMBO (combo_named),
				       combo_named_items);
	g_list_free (combo_named_items);

	combo_entry_named = GTK_COMBO (combo_named)->entry;
	gtk_widget_show (combo_entry_named);
	gtk_entry_set_text (GTK_ENTRY (combo_entry_named), "aqua");

	radiobutton_colorsel =
		gtk_radio_button_new_with_mnemonic (NULL,
						    _("Use color selector"));
	gtk_widget_show (radiobutton_colorsel);
	gtk_box_pack_start (GTK_BOX (vbox), radiobutton_colorsel, TRUE, TRUE,
			    5);
	gtk_container_set_border_width (GTK_CONTAINER (radiobutton_colorsel),
					5);
	gtk_radio_button_set_group (GTK_RADIO_BUTTON (radiobutton_colorsel),
				    radiobutton_named_group);
	radiobutton_named_group =
		gtk_radio_button_get_group (GTK_RADIO_BUTTON
					    (radiobutton_colorsel));
	gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON
				      (radiobutton_colorsel), TRUE);

	hbox_colorsel = gtk_hbox_new (TRUE, 0);
	gtk_widget_show (hbox_colorsel);
	gtk_box_pack_start (GTK_BOX (vbox), hbox_colorsel, TRUE, TRUE, 0);

	colorselection = gtk_color_selection_new ();
	gtk_widget_show (colorselection);
	gtk_box_pack_start (GTK_BOX (hbox_colorsel), colorselection, TRUE,
			    TRUE, 0);
	gtk_container_set_border_width (GTK_CONTAINER (colorselection), 10);
	gtk_color_selection_set_has_opacity_control (GTK_COLOR_SELECTION
						     (colorselection), FALSE);
	gtk_color_selection_set_has_palette (GTK_COLOR_SELECTION
					     (colorselection), TRUE);

	dialog_action_area = GTK_DIALOG (color_selector_dialog)->action_area;
	gtk_widget_show (dialog_action_area);
	gtk_button_box_set_layout (GTK_BUTTON_BOX (dialog_action_area),
				   GTK_BUTTONBOX_END);

	helpbutton = gtk_button_new_from_stock ("gtk-help");
	gtk_widget_show (helpbutton);
	gtk_dialog_add_action_widget (GTK_DIALOG (color_selector_dialog),
				      helpbutton, GTK_RESPONSE_HELP);
	GTK_WIDGET_SET_FLAGS (helpbutton, GTK_CAN_DEFAULT);
	gtk_widget_add_accelerator (helpbutton, "clicked", accel_group, GDK_h,
				    GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);

	cancelbutton = gtk_button_new_from_stock ("gtk-cancel");
	gtk_widget_show (cancelbutton);
	gtk_dialog_add_action_widget (GTK_DIALOG (color_selector_dialog),
				      cancelbutton, GTK_RESPONSE_CANCEL);
	GTK_WIDGET_SET_FLAGS (cancelbutton, GTK_CAN_DEFAULT);
	gtk_widget_add_accelerator (cancelbutton, "clicked", accel_group,
				    GDK_c, GDK_CONTROL_MASK,
				    GTK_ACCEL_VISIBLE);

	okbutton = gtk_button_new_from_stock ("gtk-ok");
	gtk_widget_show (okbutton);
	gtk_dialog_add_action_widget (GTK_DIALOG (color_selector_dialog),
				      okbutton, GTK_RESPONSE_OK);
	GTK_WIDGET_SET_FLAGS (okbutton, GTK_CAN_DEFAULT);
	gtk_widget_add_accelerator (okbutton, "clicked", accel_group,
				    GDK_o, GDK_CONTROL_MASK,
				    GTK_ACCEL_VISIBLE);
				    
	/* to update the checkboxes when a related widget is changed */				    

	g_signal_connect ((gpointer) combo_entry_named,
			  "changed", G_CALLBACK (on_widget_change_set_toggle_active),
			  radiobutton_named);
			  
	g_signal_connect ((gpointer) colorselection,
			  "color-changed", G_CALLBACK (on_widget_change_set_toggle_active),
			  radiobutton_colorsel );


	if (connect_callbacks)
	{
		g_signal_connect ((gpointer) color_selector_dialog, "close",
				  G_CALLBACK (on_css_dialog_close), data);
		g_signal_connect ((gpointer) helpbutton, "clicked",
				  G_CALLBACK
				  (on_color_selector_dialog_helpbutton_clicked),
				  NULL);
		g_signal_connect ((gpointer) cancelbutton, "clicked",
				  G_CALLBACK
				  (on_css_dialog_cancel_button_clicked),
				  data);
		g_signal_connect ((gpointer) okbutton, "clicked",
				  G_CALLBACK
				  (on_color_selector_dialog_okbutton_clicked),
				  data);
	}

	GLADE_HOOKUP_OBJECT_NO_REF (color_selector_dialog,
				    color_selector_dialog,
				    "color_selector_dialog");
	GLADE_HOOKUP_OBJECT (color_selector_dialog, radiobutton_named,
			     "radiobutton_named");
	GLADE_HOOKUP_OBJECT (color_selector_dialog, combo_named,
			     "combo_named");
	GLADE_HOOKUP_OBJECT (color_selector_dialog, combo_entry_named,
			     "combo_entry_named");
	GLADE_HOOKUP_OBJECT (color_selector_dialog, radiobutton_colorsel,
			     "radiobutton_colorsel");
	GLADE_HOOKUP_OBJECT (color_selector_dialog, colorselection,
			     "colorselection");
	GLADE_HOOKUP_OBJECT (color_selector_dialog, helpbutton, "helpbutton");
	GLADE_HOOKUP_OBJECT (color_selector_dialog, cancelbutton,
			     "cancelbutton");
	GLADE_HOOKUP_OBJECT (color_selector_dialog, okbutton, "okbutton");

	gtk_window_add_accel_group (GTK_WINDOW (color_selector_dialog),
				    accel_group);

	return color_selector_dialog;
}

GtkWidget*
create_border_style_dialog (CssedWindow* window,
			    gchar* previous_string,
			    gboolean add_previous_string,
			    gboolean connect_callbacks)
{
	GtkWidget *border_style_dialog;
	GtkWidget *dialog_vbox;
	GtkWidget *hbox;
	GtkWidget *label_property;
	GtkWidget *combo_style;
	GList *combo_style_items = NULL;
	GtkWidget *combo_entry_style;
	GtkWidget *dialog_action_area;
	GtkWidget *helpbutton;
	GtkWidget *cancelbutton;
	GtkWidget *okbutton;
	GtkAccelGroup *accel_group;
	GtkWidget *main_window;
	CssedCssDialogsData *data = NULL;

	if (connect_callbacks)
	{
		data = g_malloc (sizeof (CssedCssDialogsData));
		data->main_window = window;
		data->previous_string = previous_string;
		data->add_property_string = add_previous_string;
	}

	accel_group = gtk_accel_group_new ();

	border_style_dialog = gtk_dialog_new ();
	gtk_window_set_title (GTK_WINDOW (border_style_dialog),
			      previous_string);
	gtk_window_set_modal (GTK_WINDOW (border_style_dialog), TRUE);

	main_window = cssed_window_get_window_widget( window );

	gtk_window_set_transient_for (GTK_WINDOW (border_style_dialog),
				      GTK_WINDOW (main_window));

	dialog_vbox = GTK_DIALOG (border_style_dialog)->vbox;
	gtk_widget_show (dialog_vbox);

	hbox = gtk_hbox_new (TRUE, 0);
	gtk_widget_show (hbox);
	gtk_box_pack_start (GTK_BOX (dialog_vbox), hbox, TRUE, TRUE, 0);

	label_property = gtk_label_new (previous_string);
	gtk_widget_show (label_property);
	gtk_box_pack_start (GTK_BOX (hbox), label_property, TRUE, TRUE, 0);
	gtk_label_set_justify (GTK_LABEL (label_property), GTK_JUSTIFY_LEFT);

	combo_style = gtk_combo_new ();
	g_object_set_data (G_OBJECT (GTK_COMBO (combo_style)->popwin),
			   "GladeParentKey", combo_style);
	gtk_widget_show (combo_style);
	gtk_box_pack_start (GTK_BOX (hbox), combo_style, TRUE, TRUE, 0);
	combo_style_items =
		g_list_append (combo_style_items, (gpointer) "hidden");
	combo_style_items =
		g_list_append (combo_style_items, (gpointer) "dotted");
	combo_style_items =
		g_list_append (combo_style_items, (gpointer) "dashed");
	combo_style_items =
		g_list_append (combo_style_items, (gpointer) "solid");
	combo_style_items =
		g_list_append (combo_style_items, (gpointer) "double");
	combo_style_items =
		g_list_append (combo_style_items, (gpointer) "groove");
	combo_style_items =
		g_list_append (combo_style_items, (gpointer) "ridge");
	combo_style_items =
		g_list_append (combo_style_items, (gpointer) "inset");
	combo_style_items =
		g_list_append (combo_style_items, (gpointer) "outset");
	gtk_combo_set_popdown_strings (GTK_COMBO (combo_style),
				       combo_style_items);
	g_list_free (combo_style_items);

	combo_entry_style = GTK_COMBO (combo_style)->entry;
	gtk_widget_show (combo_entry_style);
	gtk_entry_set_text (GTK_ENTRY (combo_entry_style), "hidden ");
	gtk_editable_set_editable (GTK_EDITABLE (combo_entry_style), FALSE);

	dialog_action_area = GTK_DIALOG (border_style_dialog)->action_area;
	gtk_widget_show (dialog_action_area);
	gtk_button_box_set_layout (GTK_BUTTON_BOX (dialog_action_area),
				   GTK_BUTTONBOX_END);

	helpbutton = gtk_button_new_from_stock ("gtk-help");
	gtk_widget_show (helpbutton);
	gtk_dialog_add_action_widget (GTK_DIALOG (border_style_dialog),
				      helpbutton, GTK_RESPONSE_HELP);

	GTK_WIDGET_SET_FLAGS (helpbutton, GTK_CAN_DEFAULT);
	gtk_widget_add_accelerator (helpbutton, "clicked", accel_group,
				    GDK_h, GDK_CONTROL_MASK,
				    GTK_ACCEL_VISIBLE);

	cancelbutton = gtk_button_new_from_stock ("gtk-cancel");
	gtk_widget_show (cancelbutton);
	gtk_dialog_add_action_widget (GTK_DIALOG (border_style_dialog),
				      cancelbutton, GTK_RESPONSE_CANCEL);
	GTK_WIDGET_SET_FLAGS (cancelbutton, GTK_CAN_DEFAULT);
	gtk_widget_add_accelerator (cancelbutton, "clicked", accel_group,
				    GDK_c, GDK_CONTROL_MASK,
				    GTK_ACCEL_VISIBLE);

	okbutton = gtk_button_new_from_stock ("gtk-ok");
	gtk_widget_show (okbutton);
	gtk_dialog_add_action_widget (GTK_DIALOG (border_style_dialog),
				      okbutton, GTK_RESPONSE_OK);
	GTK_WIDGET_SET_FLAGS (okbutton, GTK_CAN_DEFAULT);
	gtk_widget_add_accelerator (okbutton, "clicked", accel_group,
				    GDK_o, GDK_CONTROL_MASK,
				    GTK_ACCEL_VISIBLE);
	if (connect_callbacks)
	{
		g_signal_connect ((gpointer) border_style_dialog, "close",
				  G_CALLBACK (on_css_dialog_close), data);
		g_signal_connect ((gpointer) helpbutton, "clicked",
				  G_CALLBACK
				  (on_border_style_dialog_helpbutton_clicked),
				  NULL);
		g_signal_connect ((gpointer) cancelbutton, "clicked",
				  G_CALLBACK
				  (on_css_dialog_cancel_button_clicked),
				  data);
		g_signal_connect ((gpointer) okbutton, "clicked",
				  G_CALLBACK
				  (on_border_style_dialog_okbutton_clicked),
				  data);

	}

	GLADE_HOOKUP_OBJECT_NO_REF (border_style_dialog, border_style_dialog,
				    "border_style_dialog");
	GLADE_HOOKUP_OBJECT_NO_REF (border_style_dialog, dialog_vbox,
				    "dialog_vbox");
	GLADE_HOOKUP_OBJECT (border_style_dialog, hbox, "hbox");
	GLADE_HOOKUP_OBJECT (border_style_dialog, label_property,
			     "label_property");
	GLADE_HOOKUP_OBJECT (border_style_dialog, combo_style, "combo_style");
	GLADE_HOOKUP_OBJECT (border_style_dialog, combo_entry_style,
			     "combo_entry_style");
	GLADE_HOOKUP_OBJECT_NO_REF (border_style_dialog, dialog_action_area,
				    "dialog_action_area");
	GLADE_HOOKUP_OBJECT (border_style_dialog, helpbutton, "helpbutton");
	GLADE_HOOKUP_OBJECT (border_style_dialog, cancelbutton,
			     "cancelbutton");
	GLADE_HOOKUP_OBJECT (border_style_dialog, okbutton, "okbutton");

	gtk_window_add_accel_group (GTK_WINDOW (border_style_dialog),
				    accel_group);

	return border_style_dialog;
}

