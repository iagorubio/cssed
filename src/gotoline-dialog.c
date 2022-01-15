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
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <gdk/gdkkeysyms.h>
#include <gtk/gtk.h>

#include "cssedwindow.h"
#include "document.h"
#include "support.h"
#include "utils.h"

typedef struct _CssedGotoDlgData {
	CssedWindow *window;
	GtkWidget *spin;
} CssedGotoLineDlgData;

void
gotoline_dialog_go(CssedGotoLineDlgData *data)
{
	GtkWidget *spin;
	CssedWindow *window;
	CssedDoc *doc;
	gdouble value;

	window = data->window;
	spin = data->spin;
	doc = document_get_current(window);
	value = gtk_spin_button_get_value(GTK_SPIN_BUTTON(spin));
	document_set_validator_arrow_at_line(doc, (gint) value);	
}

void 
on_button_goto_line_apply_clicked(GtkButton *button, gpointer user_data)
{
	gotoline_dialog_go((CssedGotoLineDlgData*) user_data);
}

gboolean
on_spinbutton_goto_line_press_event		(GtkWidget       *widget,
                                        GdkEventKey     *event,
                                        gpointer         user_data)
{
	if( event->keyval == GDK_KP_Enter ){
		gotoline_dialog_go((CssedGotoLineDlgData*) user_data);
	}
	return FALSE;
}

void
on_goto_line_dialog_destroy (GtkWidget *dialog, gpointer user_data)
{
	g_free(user_data);
}

void 
on_button_goto_line_cancel_clicked(GtkButton *button, gpointer user_data)
{
	gtk_widget_destroy(GTK_WIDGET(user_data));
}

GtkWidget*
create_goto_line_dialog (CssedWindow *window)
{
	GtkWidget *goto_line_dialog;
	GtkWidget *dialog_vbox;
	GtkWidget *hbox;
	GtkWidget *label;
	GtkWidget *spin;
	GtkWidget *dialog_action_area;
	GtkWidget *button_goto_line_cancel;
	GtkWidget *button_goto_line_apply;
	CssedGotoLineDlgData *data;

	data = g_malloc0(sizeof(CssedGotoLineDlgData));
	goto_line_dialog = gtk_dialog_new ();
	
	gtk_window_set_type_hint (GTK_WINDOW(goto_line_dialog), GDK_WINDOW_TYPE_HINT_UTILITY);
	gtk_window_set_transient_for (GTK_WINDOW(goto_line_dialog), GTK_WINDOW(cssed_window_get_window_widget(window)));

	dialog_vbox = GTK_DIALOG (goto_line_dialog)->vbox;
	gtk_widget_show (dialog_vbox);
	
	hbox = gtk_hbox_new (FALSE, 0);
	gtk_widget_show (hbox);
	gtk_box_pack_start (GTK_BOX (dialog_vbox), hbox, TRUE, TRUE, 0);
	
	label = gtk_label_new (_("Go to line"));
	gtk_widget_show (label);
	gtk_box_pack_start (GTK_BOX (hbox), label, TRUE, TRUE, 0);
	
	spin = gtk_spin_button_new_with_range (1, 1000000, 1); 
	gtk_spin_button_set_increments  (GTK_SPIN_BUTTON(spin), 1, 25);
	gtk_widget_show (spin);
	gtk_box_pack_start (GTK_BOX (hbox), spin, TRUE, TRUE, 10);
	gtk_editable_set_editable (GTK_EDITABLE(spin), TRUE);

	dialog_action_area = GTK_DIALOG (goto_line_dialog)->action_area;
	gtk_widget_show (dialog_action_area);
	gtk_button_box_set_layout (GTK_BUTTON_BOX (dialog_action_area), GTK_BUTTONBOX_END);
	
	button_goto_line_cancel = gtk_button_new_from_stock ("gtk-cancel");
	gtk_widget_show (button_goto_line_cancel);
	gtk_dialog_add_action_widget (GTK_DIALOG (goto_line_dialog), button_goto_line_cancel, GTK_RESPONSE_CANCEL);
	GTK_WIDGET_SET_FLAGS (button_goto_line_cancel, GTK_CAN_DEFAULT);
	
	button_goto_line_apply = gtk_button_new_from_stock ("gtk-apply");
	gtk_widget_show (button_goto_line_apply);
	gtk_dialog_add_action_widget (GTK_DIALOG (goto_line_dialog), button_goto_line_apply, GTK_RESPONSE_APPLY);
	GTK_WIDGET_SET_FLAGS (button_goto_line_apply, GTK_CAN_DEFAULT);

	data->window = window;
	data->spin = spin;

	g_signal_connect ((gpointer) goto_line_dialog, "destroy",
					G_CALLBACK (on_goto_line_dialog_destroy),
					data);
	g_signal_connect ((gpointer) button_goto_line_cancel, "clicked",
					G_CALLBACK (on_button_goto_line_cancel_clicked),
					goto_line_dialog);
	g_signal_connect ((gpointer) button_goto_line_apply, "clicked",
					G_CALLBACK (on_button_goto_line_apply_clicked),
					data);

	gtk_widget_grab_default (button_goto_line_apply);	
	gtk_widget_show(goto_line_dialog);

	gtk_widget_grab_focus (spin);
	
	return goto_line_dialog;
}
