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
#include "utils.h"
#include "support.h"

#define GLADE_HOOKUP_OBJECT(component,widget,name) \
  g_object_set_data_full (G_OBJECT (component), name, \
    gtk_widget_ref (widget), (GDestroyNotify) gtk_widget_unref)

#define GLADE_HOOKUP_OBJECT_NO_REF(component,widget,name) \
  g_object_set_data (G_OBJECT (component), name, widget)

void on_color_wizard_dialog_close(GtkDialog *dialog, gpointer user_data);
void on_button_fore_clicked(GtkButton *button, gpointer user_data);
void on_button_back_clicked (GtkButton *button, gpointer user_data);
void on_button_attach_clicked(GtkButton *button, gpointer user_data);					
void on_button_border_top_clicked(GtkButton *button, gpointer user_data);	
void on_button_border_right_clicked(GtkButton *button, gpointer user_data);
void on_button_border_bottom_clicked(GtkButton *button, gpointer user_data);
void on_button_border_left_clicked(GtkButton *button, gpointer user_data);
void on_cancelbutton_clicked (GtkButton *button, gpointer user_data);
void on_button_outline_clicked (GtkButton *button, gpointer user_data);
void on_applybutton_clicked (GtkButton *button, gpointer user_data);
void on_okbutton_clicked (GtkButton *button, gpointer user_data);
void set_color_and_label(GtkWidget* entry, GtkWidget* label);									
void write_results_to_document(GtkWidget* parent, CssedWindow* window);




GtkWidget*
create_color_wizard_dialog (CssedWindow* window)
{
  GtkWidget *color_wizard_dialog;
  GtkWidget *dialog_vbox;
  GtkWidget *table;
  GtkWidget *combo_fore;
  GList *combo_items = NULL;
  GtkWidget *combo_entry_fore;
  GtkWidget *combo_back;
  GtkWidget *combo_entry_back;
  GtkWidget *combo_attach;
  GtkWidget *combo_entry_attach;
  GtkWidget *combo_border_top;
  GtkWidget *combo_entry_border_top;
  GtkWidget *combo_border_right;
  GtkWidget *combo_entry_border_right;
  GtkWidget *combo_border_bottom;
  GtkWidget *combo_entry_border_bottom;
  GtkWidget *combo_border_left;
  GtkWidget *combo_entry_border_left;
  GtkWidget *combo_outline;
  GtkWidget *combo_entry_outline;
  GtkWidget *checkbutton_fore;
  GtkWidget *checkbutton_back;
  GtkWidget *checkbutton_attach;
  GtkWidget *checkbutton_border_top;
  GtkWidget *checkbutton_border_right;
  GtkWidget *checkbutton_border_bottom;
  GtkWidget *checkbutton_border_left;
  GtkWidget *checkbutton_outline;
  GtkWidget *button_fore;
  GtkWidget *alignment_fore;
  GtkWidget *hbox_fore;
  GtkWidget *image_fore;
  GtkWidget *label_fore;
  GtkWidget *button_back;
  GtkWidget *alignment_back;
  GtkWidget *hbox_back;
  GtkWidget *image_back;
  GtkWidget *label_back;
  GtkWidget *button_attach;
  GtkWidget *alignment_attach;
  GtkWidget *hbox_attach;
  GtkWidget *image_attach;
  GtkWidget *label_attach;
  GtkWidget *button_border_top;
  GtkWidget *alignment_border_top;
  GtkWidget *hbox_border_top;
  GtkWidget *image_border_top;
  GtkWidget *label_border_top;
  GtkWidget *button_border_right;
  GtkWidget *alignment_border_right;
  GtkWidget *hbox_border_right;
  GtkWidget *image_border_right;
  GtkWidget *label_border_right;
  GtkWidget *button_border_bottom;
  GtkWidget *alignment_border_bottom;
  GtkWidget *hbox_border_botom;
  GtkWidget *image_border_bottom;
  GtkWidget *label_border_bottom;
  GtkWidget *button_border_left;
  GtkWidget *alignment_border_left;
  GtkWidget *hbox_border_left;
  GtkWidget *image_border_left;
  GtkWidget *label_border_left;
  GtkWidget *button_outline;
  GtkWidget *alignment_outline;
  GtkWidget *hbox_outline;
  GtkWidget *image_outline;
  GtkWidget *label_outline;
  GtkWidget *dialog_action_area;
  GtkWidget *cancelbutton;
  GtkWidget *applybutton;
  GtkWidget *okbutton;
  


  color_wizard_dialog = gtk_dialog_new ();
  gtk_window_set_title (GTK_WINDOW (color_wizard_dialog), _("Color wizard"));
  gtk_window_set_default_size (GTK_WINDOW (color_wizard_dialog), 340, 250);
  gtk_window_set_destroy_with_parent (GTK_WINDOW (color_wizard_dialog), TRUE);
  gtk_window_set_transient_for ( GTK_WINDOW(color_wizard_dialog),
				      GTK_WINDOW ( cssed_window_get_window_widget(window) ));

  dialog_vbox = GTK_DIALOG (color_wizard_dialog)->vbox;
  gtk_widget_show (dialog_vbox);

  table = gtk_table_new (8, 3, FALSE);
  gtk_widget_show (table);
  gtk_box_pack_start (GTK_BOX (dialog_vbox), table, TRUE, TRUE, 0);

  combo_fore = gtk_combo_new ();
  g_object_set_data (G_OBJECT (GTK_COMBO (combo_fore)->popwin),
                     "GladeParentKey", combo_fore);
  gtk_widget_show (combo_fore);
  gtk_table_attach (GTK_TABLE (table), combo_fore, 1, 2, 0, 1,
                    (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  combo_items = g_list_append (combo_items, (gpointer) "aqua");
  combo_items = g_list_append (combo_items, (gpointer) "black");
  combo_items = g_list_append (combo_items, (gpointer) "blue");
  combo_items = g_list_append (combo_items, (gpointer) "fuchsia");
  combo_items = g_list_append (combo_items, (gpointer) "gray");
  combo_items = g_list_append (combo_items, (gpointer) "green");
  combo_items = g_list_append (combo_items, (gpointer) "lime");
  combo_items = g_list_append (combo_items, (gpointer) "maroon");
  combo_items = g_list_append (combo_items, (gpointer) "navy");
  combo_items = g_list_append (combo_items, (gpointer) "olive");
  combo_items = g_list_append (combo_items, (gpointer) "purple");
  combo_items = g_list_append (combo_items, (gpointer) "red");
  combo_items = g_list_append (combo_items, (gpointer) "silver");
  combo_items = g_list_append (combo_items, (gpointer) "teal");
  combo_items = g_list_append (combo_items, (gpointer) "white");
  combo_items = g_list_append (combo_items, (gpointer) "yellow");
  combo_items = g_list_append (combo_items, (gpointer) "activeborder ");
  combo_items = g_list_append (combo_items, (gpointer) "activecaption");
  combo_items = g_list_append (combo_items, (gpointer) "appworkspace");
  combo_items = g_list_append (combo_items, (gpointer) "background");
  combo_items = g_list_append (combo_items, (gpointer) "buttonface");
  combo_items = g_list_append (combo_items, (gpointer) "buttonhighlight");
  combo_items = g_list_append (combo_items, (gpointer) "buttonshadow");
  combo_items = g_list_append (combo_items, (gpointer) "buttontext");
  combo_items = g_list_append (combo_items, (gpointer) "captiontext");
  combo_items = g_list_append (combo_items, (gpointer) "graytext");
  combo_items = g_list_append (combo_items, (gpointer) "highlight");
  combo_items = g_list_append (combo_items, (gpointer) "highlighttext");
  combo_items = g_list_append (combo_items, (gpointer) "inactiveborder");
  combo_items = g_list_append (combo_items, (gpointer) "inactivecaption");
  combo_items = g_list_append (combo_items, (gpointer) "inactivecaptiontext");
  combo_items = g_list_append (combo_items, (gpointer) "infobackground");
  combo_items = g_list_append (combo_items, (gpointer) "infotext");
  combo_items = g_list_append (combo_items, (gpointer) "menu");
  combo_items = g_list_append (combo_items, (gpointer) "menutext");
  combo_items = g_list_append (combo_items, (gpointer) "scrollbar");
  combo_items = g_list_append (combo_items, (gpointer) "threeddarkshadow");
  combo_items = g_list_append (combo_items, (gpointer) "threedface");
  combo_items = g_list_append (combo_items, (gpointer) "threedhighlight");
  combo_items = g_list_append (combo_items, (gpointer) "threedlightshadow");
  combo_items = g_list_append (combo_items, (gpointer) "threedshadow");
  combo_items = g_list_append (combo_items, (gpointer) "window");
  combo_items = g_list_append (combo_items, (gpointer) "windowframe");
  combo_items = g_list_append (combo_items, (gpointer) "windowtext");
  gtk_combo_set_popdown_strings (GTK_COMBO (combo_fore), combo_items);


  combo_entry_fore = GTK_COMBO (combo_fore)->entry;
  gtk_widget_show (combo_entry_fore);
  gtk_entry_set_text (GTK_ENTRY (combo_entry_fore), "");

  combo_back = gtk_combo_new ();
  g_object_set_data (G_OBJECT (GTK_COMBO (combo_back)->popwin),
                     "GladeParentKey", combo_back);
  gtk_widget_show (combo_back);
  gtk_table_attach (GTK_TABLE (table), combo_back, 1, 2, 1, 2,
                    (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_combo_set_popdown_strings (GTK_COMBO (combo_back), combo_items);
  
  combo_entry_back = GTK_COMBO (combo_back)->entry;
  gtk_widget_show (combo_entry_back);
  gtk_entry_set_text (GTK_ENTRY (combo_entry_back), "");
  
  combo_attach = gtk_combo_new ();
  g_object_set_data (G_OBJECT (GTK_COMBO (combo_attach)->popwin),
                     "GladeParentKey", combo_attach);
  gtk_widget_show (combo_attach);
  gtk_table_attach (GTK_TABLE (table), combo_attach, 1, 2, 2, 3,
                    (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);					
  gtk_combo_set_popdown_strings (GTK_COMBO (combo_attach), combo_items);
  
  combo_entry_attach = GTK_COMBO (combo_attach)->entry;
  gtk_widget_show (combo_entry_attach);
  gtk_entry_set_text (GTK_ENTRY (combo_entry_attach), "");
  
  combo_border_top = gtk_combo_new ();
  g_object_set_data (G_OBJECT (GTK_COMBO (combo_border_top)->popwin),
                     "GladeParentKey", combo_border_top);
  gtk_widget_show (combo_border_top);
  gtk_table_attach (GTK_TABLE (table), combo_border_top, 1, 2, 3, 4,
                    (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_combo_set_popdown_strings (GTK_COMBO (combo_border_top), combo_items);
  
  combo_entry_border_top = GTK_COMBO (combo_border_top)->entry;
  gtk_widget_show (combo_entry_border_top);
  gtk_entry_set_text (GTK_ENTRY (combo_entry_border_top), "");
  
  combo_border_right = gtk_combo_new ();
  g_object_set_data (G_OBJECT (GTK_COMBO (combo_border_right)->popwin),
                     "GladeParentKey", combo_border_right);
  gtk_widget_show (combo_border_right);
  gtk_table_attach (GTK_TABLE (table), combo_border_right, 1, 2, 4, 5,
                    (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_combo_set_popdown_strings (GTK_COMBO (combo_border_right), combo_items);
  
  combo_entry_border_right = GTK_COMBO (combo_border_right)->entry;
  gtk_widget_show (combo_entry_border_right);
   gtk_entry_set_text (GTK_ENTRY (combo_entry_border_right), ""); 

  combo_border_bottom = gtk_combo_new ();
  g_object_set_data (G_OBJECT (GTK_COMBO (combo_border_bottom)->popwin),
                     "GladeParentKey", combo_border_bottom);
  gtk_widget_show (combo_border_bottom);
  gtk_table_attach (GTK_TABLE (table), combo_border_bottom, 1, 2, 5, 6,
                    (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_combo_set_popdown_strings (GTK_COMBO (combo_border_bottom), combo_items);
  
  combo_entry_border_bottom = GTK_COMBO (combo_border_bottom)->entry;
  gtk_widget_show (combo_entry_border_bottom);
   gtk_entry_set_text (GTK_ENTRY (combo_entry_border_bottom), ""); 
   
  combo_border_left = gtk_combo_new ();
  g_object_set_data (G_OBJECT (GTK_COMBO (combo_border_left)->popwin),
                     "GladeParentKey", combo_border_left);
  gtk_widget_show (combo_border_left);
  gtk_table_attach (GTK_TABLE (table), combo_border_left, 1, 2, 6, 7,
                    (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_combo_set_popdown_strings (GTK_COMBO (combo_border_left), combo_items);
  
  combo_entry_border_left = GTK_COMBO (combo_border_left)->entry;
  gtk_widget_show (combo_entry_border_left);
   gtk_entry_set_text (GTK_ENTRY (combo_entry_border_left), ""); 
   
  combo_outline = gtk_combo_new ();
  g_object_set_data (G_OBJECT (GTK_COMBO (combo_outline)->popwin),
                     "GladeParentKey", combo_border_left);
  gtk_widget_show (combo_outline);
  gtk_table_attach (GTK_TABLE (table), combo_outline, 1, 2, 7, 8,
                    (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_combo_set_popdown_strings (GTK_COMBO (combo_outline), combo_items);
  
  // free combo_items as it's not needed anymore
  g_list_free (combo_items);
  
  combo_entry_outline = GTK_COMBO (combo_outline)->entry;
  gtk_widget_show (combo_entry_outline);
   gtk_entry_set_text (GTK_ENTRY (combo_entry_outline), ""); 
   
  checkbutton_fore = gtk_check_button_new_with_mnemonic (_("foreground"));
  gtk_widget_show (checkbutton_fore);
  gtk_table_attach (GTK_TABLE (table), checkbutton_fore, 0, 1, 0, 1,
                    (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                    (GtkAttachOptions) (GTK_EXPAND), 5, 0);

  checkbutton_back = gtk_check_button_new_with_mnemonic (_("background"));
  gtk_widget_show (checkbutton_back);
  gtk_table_attach (GTK_TABLE (table), checkbutton_back, 0, 1, 1, 2,
                    (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                    (GtkAttachOptions) (GTK_EXPAND), 5, 0);

  checkbutton_attach = gtk_check_button_new_with_mnemonic (_("background attachment"));
  gtk_widget_show (checkbutton_attach);
  gtk_table_attach (GTK_TABLE (table), checkbutton_attach, 0, 1, 2, 3,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (GTK_EXPAND), 5, 0);

  checkbutton_border_top = gtk_check_button_new_with_mnemonic (_("border top"));
  gtk_widget_show (checkbutton_border_top);
  gtk_table_attach (GTK_TABLE (table), checkbutton_border_top, 0, 1, 3, 4,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (GTK_EXPAND), 5, 0);

  checkbutton_border_right = gtk_check_button_new_with_mnemonic (_("border right"));
  gtk_widget_show (checkbutton_border_right);
  gtk_table_attach (GTK_TABLE (table), checkbutton_border_right, 0, 1, 4, 5,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (GTK_EXPAND), 5, 0);

  checkbutton_border_bottom = gtk_check_button_new_with_mnemonic (_("border bottom"));
  gtk_widget_show (checkbutton_border_bottom);
  gtk_table_attach (GTK_TABLE (table), checkbutton_border_bottom, 0, 1, 5, 6,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (GTK_EXPAND), 5, 0);

  checkbutton_border_left = gtk_check_button_new_with_mnemonic (_("border left"));
  gtk_widget_show (checkbutton_border_left);
  gtk_table_attach (GTK_TABLE (table), checkbutton_border_left, 0, 1, 6, 7,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (GTK_EXPAND), 5, 0);

  checkbutton_outline = gtk_check_button_new_with_mnemonic (_("outline"));
  gtk_widget_show (checkbutton_outline);
  gtk_table_attach (GTK_TABLE (table), checkbutton_outline, 0, 1, 7, 8,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (GTK_EXPAND), 5, 0);

  button_fore = gtk_button_new ();
  gtk_widget_show (button_fore);
  gtk_table_attach (GTK_TABLE (table), button_fore, 2, 3, 0, 1,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 5, 0);

  alignment_fore = gtk_alignment_new (0.5, 0.5, 0, 0);
  gtk_widget_show (alignment_fore);
  gtk_container_add (GTK_CONTAINER (button_fore), alignment_fore);

  hbox_fore = gtk_hbox_new (FALSE, 2);
  gtk_widget_show (hbox_fore);
  gtk_container_add (GTK_CONTAINER (alignment_fore), hbox_fore);

  image_fore = gtk_image_new_from_stock ("gtk-select-color", GTK_ICON_SIZE_BUTTON);
  gtk_widget_show (image_fore);
  gtk_box_pack_start (GTK_BOX (hbox_fore), image_fore, FALSE, FALSE, 0);

  label_fore = gtk_label_new_with_mnemonic (_("   "));
  gtk_widget_show (label_fore);
  gtk_box_pack_start (GTK_BOX (hbox_fore), label_fore, FALSE, FALSE, 0);
  gtk_label_set_justify (GTK_LABEL (label_fore), GTK_JUSTIFY_LEFT);

  button_back = gtk_button_new ();
  gtk_widget_show (button_back);
  gtk_table_attach (GTK_TABLE (table), button_back, 2, 3, 1, 2,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 5, 0);

  alignment_back = gtk_alignment_new (0.5, 0.5, 0, 0);
  gtk_widget_show (alignment_back);
  gtk_container_add (GTK_CONTAINER (button_back), alignment_back);

  hbox_back = gtk_hbox_new (FALSE, 2);
  gtk_widget_show (hbox_back);
  gtk_container_add (GTK_CONTAINER (alignment_back), hbox_back);

  image_back = gtk_image_new_from_stock ("gtk-select-color", GTK_ICON_SIZE_BUTTON);
  gtk_widget_show (image_back);
  gtk_box_pack_start (GTK_BOX (hbox_back), image_back, FALSE, FALSE, 0);

  label_back = gtk_label_new_with_mnemonic (_("   "));
  gtk_widget_show (label_back);
  gtk_box_pack_start (GTK_BOX (hbox_back), label_back, FALSE, FALSE, 0);
  gtk_label_set_justify (GTK_LABEL (label_back), GTK_JUSTIFY_LEFT);

  button_attach = gtk_button_new ();
  gtk_widget_show (button_attach);
  gtk_table_attach (GTK_TABLE (table), button_attach, 2, 3, 2, 3,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 5, 0);

  alignment_attach = gtk_alignment_new (0.5, 0.5, 0, 0);
  gtk_widget_show (alignment_attach);
  gtk_container_add (GTK_CONTAINER (button_attach), alignment_attach);

  hbox_attach = gtk_hbox_new (FALSE, 2);
  gtk_widget_show (hbox_attach);
  gtk_container_add (GTK_CONTAINER (alignment_attach), hbox_attach);

  image_attach = gtk_image_new_from_stock ("gtk-select-color", GTK_ICON_SIZE_BUTTON);
  gtk_widget_show (image_attach);
  gtk_box_pack_start (GTK_BOX (hbox_attach), image_attach, FALSE, FALSE, 0);

  label_attach = gtk_label_new_with_mnemonic (_("   "));
  gtk_widget_show (label_attach);
  gtk_box_pack_start (GTK_BOX (hbox_attach), label_attach, FALSE, FALSE, 0);
  gtk_label_set_justify (GTK_LABEL (label_attach), GTK_JUSTIFY_LEFT);
/**/
  button_border_top = gtk_button_new ();
  gtk_widget_show (button_border_top);
  gtk_table_attach (GTK_TABLE (table), button_border_top, 2, 3, 3, 4,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 5, 0);

  alignment_border_top = gtk_alignment_new (0.5, 0.5, 0, 0);
  gtk_widget_show (alignment_border_top);
  gtk_container_add (GTK_CONTAINER (button_border_top), alignment_border_top);

  hbox_border_top = gtk_hbox_new (FALSE, 2);
  gtk_widget_show (hbox_border_top);
  gtk_container_add (GTK_CONTAINER (alignment_border_top), hbox_border_top);

  image_border_top = gtk_image_new_from_stock ("gtk-select-color", GTK_ICON_SIZE_BUTTON);
  gtk_widget_show (image_border_top);
  gtk_box_pack_start (GTK_BOX (hbox_border_top), image_border_top, FALSE, FALSE, 0);

  label_border_top = gtk_label_new_with_mnemonic (_("   "));
  gtk_widget_show (label_border_top);
  gtk_box_pack_start (GTK_BOX (hbox_border_top), label_border_top, FALSE, FALSE, 0);
  gtk_label_set_justify (GTK_LABEL (label_border_top), GTK_JUSTIFY_LEFT);

  button_border_right = gtk_button_new ();
  gtk_widget_show (button_border_right);
  gtk_table_attach (GTK_TABLE (table), button_border_right, 2, 3, 4, 5,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 5, 0);

  alignment_border_right = gtk_alignment_new (0.5, 0.5, 0, 0);
  gtk_widget_show (alignment_border_right);
  gtk_container_add (GTK_CONTAINER (button_border_right), alignment_border_right);

  hbox_border_right = gtk_hbox_new (FALSE, 2);
  gtk_widget_show (hbox_border_right);
  gtk_container_add (GTK_CONTAINER (alignment_border_right), hbox_border_right);

  image_border_right = gtk_image_new_from_stock ("gtk-select-color", GTK_ICON_SIZE_BUTTON);
  gtk_widget_show (image_border_right);
  gtk_box_pack_start (GTK_BOX (hbox_border_right), image_border_right, FALSE, FALSE, 0);

  label_border_right = gtk_label_new_with_mnemonic (_("   "));
  gtk_widget_show (label_border_right);
  gtk_box_pack_start (GTK_BOX (hbox_border_right), label_border_right, FALSE, FALSE, 0);
  gtk_label_set_justify (GTK_LABEL (label_border_right), GTK_JUSTIFY_LEFT);

  button_border_bottom = gtk_button_new ();
  gtk_widget_show (button_border_bottom);
  gtk_table_attach (GTK_TABLE (table), button_border_bottom, 2, 3, 5, 6,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 5, 0);

  alignment_border_bottom = gtk_alignment_new (0.5, 0.5, 0, 0);
  gtk_widget_show (alignment_border_bottom);
  gtk_container_add (GTK_CONTAINER (button_border_bottom), alignment_border_bottom);

  hbox_border_botom = gtk_hbox_new (FALSE, 2);
  gtk_widget_show (hbox_border_botom);
  gtk_container_add (GTK_CONTAINER (alignment_border_bottom), hbox_border_botom);

  image_border_bottom = gtk_image_new_from_stock ("gtk-select-color", GTK_ICON_SIZE_BUTTON);
  gtk_widget_show (image_border_bottom);
  gtk_box_pack_start (GTK_BOX (hbox_border_botom), image_border_bottom, FALSE, FALSE, 0);

  label_border_bottom = gtk_label_new_with_mnemonic (_("   "));
  gtk_widget_show (label_border_bottom);
  gtk_box_pack_start (GTK_BOX (hbox_border_botom), label_border_bottom, FALSE, FALSE, 0);
  gtk_label_set_justify (GTK_LABEL (label_border_bottom), GTK_JUSTIFY_LEFT);

  button_border_left = gtk_button_new ();
  gtk_widget_show (button_border_left);
  gtk_table_attach (GTK_TABLE (table), button_border_left, 2, 3, 6, 7,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 5, 0);

  alignment_border_left = gtk_alignment_new (0.5, 0.5, 0, 0);
  gtk_widget_show (alignment_border_left);
  gtk_container_add (GTK_CONTAINER (button_border_left), alignment_border_left);

  hbox_border_left = gtk_hbox_new (FALSE, 2);
  gtk_widget_show (hbox_border_left);
  gtk_container_add (GTK_CONTAINER (alignment_border_left), hbox_border_left);

  image_border_left = gtk_image_new_from_stock ("gtk-select-color", GTK_ICON_SIZE_BUTTON);
  gtk_widget_show (image_border_left);
  gtk_box_pack_start (GTK_BOX (hbox_border_left), image_border_left, FALSE, FALSE, 0);

  label_border_left = gtk_label_new_with_mnemonic (_("   "));
  gtk_widget_show (label_border_left);
  gtk_box_pack_start (GTK_BOX (hbox_border_left), label_border_left, FALSE, FALSE, 0);
  gtk_label_set_justify (GTK_LABEL (label_border_left), GTK_JUSTIFY_LEFT);

  button_outline = gtk_button_new ();
  gtk_widget_show (button_outline);
  gtk_table_attach (GTK_TABLE (table), button_outline, 2, 3, 7, 8,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 5, 0);

  alignment_outline = gtk_alignment_new (0.5, 0.5, 0, 0);
  gtk_widget_show (alignment_outline);
  gtk_container_add (GTK_CONTAINER (button_outline), alignment_outline);

  hbox_outline = gtk_hbox_new (FALSE, 2);
  gtk_widget_show (hbox_outline);
  gtk_container_add (GTK_CONTAINER (alignment_outline), hbox_outline);

  image_outline = gtk_image_new_from_stock ("gtk-select-color", GTK_ICON_SIZE_BUTTON);
  gtk_widget_show (image_outline);
  gtk_box_pack_start (GTK_BOX (hbox_outline), image_outline, FALSE, FALSE, 0);

  label_outline = gtk_label_new_with_mnemonic (_("   "));
  gtk_widget_show (label_outline);
  gtk_box_pack_start (GTK_BOX (hbox_outline), label_outline, FALSE, FALSE, 0);
  gtk_label_set_justify (GTK_LABEL (label_outline), GTK_JUSTIFY_LEFT);

  dialog_action_area = GTK_DIALOG (color_wizard_dialog)->action_area;
  gtk_widget_show (dialog_action_area);
  gtk_button_box_set_layout (GTK_BUTTON_BOX (dialog_action_area), GTK_BUTTONBOX_END);

  cancelbutton = gtk_button_new_from_stock ("gtk-cancel");
  gtk_widget_show (cancelbutton);
  gtk_dialog_add_action_widget (GTK_DIALOG (color_wizard_dialog), cancelbutton, GTK_RESPONSE_CANCEL);
  GTK_WIDGET_SET_FLAGS (cancelbutton, GTK_CAN_DEFAULT);

  applybutton = gtk_button_new_from_stock ("gtk-apply");
  gtk_widget_show (applybutton);
  gtk_dialog_add_action_widget (GTK_DIALOG (color_wizard_dialog), applybutton, GTK_RESPONSE_APPLY);
  GTK_WIDGET_SET_FLAGS (applybutton, GTK_CAN_DEFAULT);

  okbutton = gtk_button_new_from_stock ("gtk-ok");
  gtk_widget_show (okbutton);
  gtk_dialog_add_action_widget (GTK_DIALOG (color_wizard_dialog), okbutton, GTK_RESPONSE_OK);
  GTK_WIDGET_SET_FLAGS (okbutton, GTK_CAN_DEFAULT);

/*
  g_signal_connect ((gpointer) color_wizard_dialog, "close",
                    G_CALLBACK (on_color_wizard_dialog_close),
                    NULL);
*/
  g_signal_connect ((gpointer) button_fore, "clicked",
                    G_CALLBACK (on_button_fore_clicked),
                    NULL);
  g_signal_connect ((gpointer) button_back, "clicked",
                    G_CALLBACK (on_button_back_clicked),
                    NULL);
  g_signal_connect ((gpointer) button_attach, "clicked",
                    G_CALLBACK (on_button_attach_clicked),
                    NULL);					
  g_signal_connect ((gpointer) button_border_top, "clicked",
                    G_CALLBACK (on_button_border_top_clicked),
                    NULL);				
  g_signal_connect ((gpointer) button_border_right, "clicked",
                    G_CALLBACK (on_button_border_right_clicked),
                    NULL);					
  g_signal_connect ((gpointer) button_border_bottom, "clicked",
                    G_CALLBACK (on_button_border_bottom_clicked),
                    NULL);									
  g_signal_connect ((gpointer) button_border_left, "clicked",
                    G_CALLBACK (on_button_border_left_clicked),
                    NULL);				
  g_signal_connect ((gpointer) button_outline, "clicked",
                    G_CALLBACK (on_button_outline_clicked),
                    NULL);			
  g_signal_connect ((gpointer) cancelbutton, "clicked",
                    G_CALLBACK (on_cancelbutton_clicked),
                    color_wizard_dialog);
  g_signal_connect ((gpointer) applybutton, "clicked",
                    G_CALLBACK (on_applybutton_clicked),
                    window);
  g_signal_connect ((gpointer) okbutton, "clicked",
                    G_CALLBACK (on_okbutton_clicked),
                    window);
					

	GLADE_HOOKUP_OBJECT_NO_REF (color_wizard_dialog, color_wizard_dialog, "color_wizard_dialog");
  GLADE_HOOKUP_OBJECT (color_wizard_dialog, combo_entry_fore, "combo_entry_fore");
  GLADE_HOOKUP_OBJECT (color_wizard_dialog, combo_entry_back, "combo_entry_back");
  GLADE_HOOKUP_OBJECT (color_wizard_dialog, combo_entry_attach, "combo_entry_attach");
  GLADE_HOOKUP_OBJECT (color_wizard_dialog, combo_entry_border_top, "combo_entry_border_top");
  GLADE_HOOKUP_OBJECT (color_wizard_dialog, combo_entry_border_right, "combo_entry_border_right");
  GLADE_HOOKUP_OBJECT (color_wizard_dialog, combo_entry_border_bottom, "combo_entry_border_bottom");
  GLADE_HOOKUP_OBJECT (color_wizard_dialog, combo_entry_border_left, "combo_entry_border_left");
  GLADE_HOOKUP_OBJECT (color_wizard_dialog, combo_entry_border_left, "combo_entry_border_left");
  GLADE_HOOKUP_OBJECT (color_wizard_dialog, checkbutton_fore, "checkbutton_fore");
  GLADE_HOOKUP_OBJECT (color_wizard_dialog, checkbutton_back, "checkbutton_back");
  GLADE_HOOKUP_OBJECT (color_wizard_dialog, checkbutton_attach, "checkbutton_attach");
  GLADE_HOOKUP_OBJECT (color_wizard_dialog, checkbutton_border_top, "checkbutton_border_top");
  GLADE_HOOKUP_OBJECT (color_wizard_dialog, checkbutton_border_right, "checkbutton_border_right");
  GLADE_HOOKUP_OBJECT (color_wizard_dialog, checkbutton_border_bottom, "checkbutton_border_bottom");
  GLADE_HOOKUP_OBJECT (color_wizard_dialog, checkbutton_border_left, "checkbutton_border_left");
  GLADE_HOOKUP_OBJECT (color_wizard_dialog, checkbutton_border_left, "checkbutton_border_left");
  GLADE_HOOKUP_OBJECT (color_wizard_dialog, checkbutton_outline,"checkbutton_outline");
  GLADE_HOOKUP_OBJECT (color_wizard_dialog, combo_entry_outline,"combo_entry_outline");
  GLADE_HOOKUP_OBJECT (color_wizard_dialog, button_fore, "button_fore");
  GLADE_HOOKUP_OBJECT (color_wizard_dialog, label_fore, "label_fore");
  GLADE_HOOKUP_OBJECT (color_wizard_dialog, button_back, "button_back");
  GLADE_HOOKUP_OBJECT (color_wizard_dialog, label_back, "label_back");
  GLADE_HOOKUP_OBJECT (color_wizard_dialog, button_attach, "button_attach");
  GLADE_HOOKUP_OBJECT (color_wizard_dialog, label_attach, "label_attach");
  GLADE_HOOKUP_OBJECT (color_wizard_dialog, button_border_top, "button_border_top");
  GLADE_HOOKUP_OBJECT (color_wizard_dialog, label_border_top, "label_border_top");
  GLADE_HOOKUP_OBJECT (color_wizard_dialog, button_border_right, "button_border_right");
  GLADE_HOOKUP_OBJECT (color_wizard_dialog, label_border_right, "label_border_right");
  GLADE_HOOKUP_OBJECT (color_wizard_dialog, button_border_bottom, "button_border_bottom");
  GLADE_HOOKUP_OBJECT (color_wizard_dialog, label_border_bottom, "label_border_bottom");
  GLADE_HOOKUP_OBJECT (color_wizard_dialog, button_border_left, "button_border_left");
  GLADE_HOOKUP_OBJECT (color_wizard_dialog, label_border_left, "label_border_left");
  GLADE_HOOKUP_OBJECT (color_wizard_dialog, button_outline, "button_outline");
  GLADE_HOOKUP_OBJECT (color_wizard_dialog, label_outline, "label_outline");
  GLADE_HOOKUP_OBJECT (color_wizard_dialog, cancelbutton, "cancelbutton");
  GLADE_HOOKUP_OBJECT (color_wizard_dialog, applybutton, "applybutton");
  GLADE_HOOKUP_OBJECT (color_wizard_dialog, okbutton, "okbutton");

  return color_wizard_dialog;
}

/* callbacks */
void
on_color_wizard_dialog_close           (GtkDialog       *dialog,
                                        gpointer         user_data)
{
	gtk_widget_destroy(GTK_WIDGET(dialog));
}


void
on_button_fore_clicked                 (GtkButton       *button,
                                        gpointer         user_data)
{
	GtkWidget* entry;
	GtkWidget* label;
	GtkWidget* toggle;
	
	entry = lookup_widget(GTK_WIDGET(button),"combo_entry_fore");
	label = lookup_widget(GTK_WIDGET(button),"label_fore");
	toggle =  lookup_widget(GTK_WIDGET(button),"checkbutton_fore");
	
	set_color_and_label( entry, label);
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(toggle),TRUE);
}


void
on_button_back_clicked                 (GtkButton       *button,
                                        gpointer         user_data)
{
	GtkWidget* entry;
	GtkWidget* label;
	GtkWidget* toggle;
	
	entry = lookup_widget(GTK_WIDGET(button),"combo_entry_back");
	label = lookup_widget(GTK_WIDGET(button),"label_back");
	toggle =  lookup_widget(GTK_WIDGET(button),"checkbutton_back");
	
	set_color_and_label( entry, label);
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(toggle),TRUE);
}

void
on_button_attach_clicked(GtkButton *button, gpointer user_data)		
{
	GtkWidget* entry;
	GtkWidget* label;
	GtkWidget* toggle;
	
	entry = lookup_widget(GTK_WIDGET(button),"combo_entry_attach");
	label = lookup_widget(GTK_WIDGET(button),"label_attach");
	toggle =  lookup_widget(GTK_WIDGET(button),"checkbutton_attach");
	
	set_color_and_label( entry, label);
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(toggle),TRUE);	
}

void 
on_button_border_top_clicked(GtkButton *button, gpointer user_data)		
{
	GtkWidget* entry;
	GtkWidget* label;
	GtkWidget* toggle;
	
	entry = lookup_widget(GTK_WIDGET(button),"combo_entry_border_top");
	label = lookup_widget(GTK_WIDGET(button),"label_border_top");
	toggle =  lookup_widget(GTK_WIDGET(button),"checkbutton_border_top");
	
	set_color_and_label( entry, label);
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(toggle),TRUE);		
}	

void 
on_button_border_right_clicked(GtkButton *button, gpointer user_data)		
{
	GtkWidget* entry;
	GtkWidget* label;
	GtkWidget* toggle;
	
	entry = lookup_widget(GTK_WIDGET(button),"combo_entry_border_right");
	label = lookup_widget(GTK_WIDGET(button),"label_border_right");
	toggle =  lookup_widget(GTK_WIDGET(button),"checkbutton_border_right");
	
	set_color_and_label( entry, label);
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(toggle),TRUE);	
}

void 
on_button_border_bottom_clicked(GtkButton *button, gpointer user_data)		
{
	GtkWidget* entry;
	GtkWidget* label;
	GtkWidget* toggle;
	
	entry = lookup_widget(GTK_WIDGET(button),"combo_entry_border_bottom");
	label = lookup_widget(GTK_WIDGET(button),"label_border_bottom");
	toggle =  lookup_widget(GTK_WIDGET(button),"checkbutton_border_bottom");
	
	set_color_and_label( entry, label);
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(toggle),TRUE);	
}

void 
on_button_border_left_clicked(GtkButton *button, gpointer user_data)		
{
	GtkWidget* entry;
	GtkWidget* label;
	GtkWidget* toggle;
	
	entry = lookup_widget(GTK_WIDGET(button),"combo_entry_border_left");
	label = lookup_widget(GTK_WIDGET(button),"label_border_left");
	toggle =  lookup_widget(GTK_WIDGET(button),"checkbutton_border_left");
	
	set_color_and_label( entry, label);
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(toggle),TRUE);	
}

void 
on_button_outline_clicked (GtkButton *button, gpointer user_data)		
{
	GtkWidget* entry;
	GtkWidget* label;
	GtkWidget* toggle;
	
	entry = lookup_widget(GTK_WIDGET(button),"combo_entry_outline");
	label = lookup_widget(GTK_WIDGET(button),"label_outline");
	toggle =  lookup_widget(GTK_WIDGET(button),"checkbutton_outline");
	
	set_color_and_label( entry, label);
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(toggle),TRUE);	
}

void
on_cancelbutton_clicked                (GtkButton       *button,
                                        gpointer         user_data)
{
	gtk_widget_destroy(GTK_WIDGET(user_data));
}


void
on_applybutton_clicked                 (GtkButton       *button,
                                        gpointer         user_data)
{
	write_results_to_document(GTK_WIDGET(button), CSSED_WINDOW(user_data));	
}


void
on_okbutton_clicked                    (GtkButton       *button,
                                        gpointer         user_data)
{	
	GtkWidget* dialog;
	
	dialog = lookup_widget(GTK_WIDGET(button),"color_wizard_dialog");
	write_results_to_document(GTK_WIDGET(button), CSSED_WINDOW(user_data));
	gtk_widget_destroy(dialog);
}

void
set_color_and_label( GtkWidget* entry, GtkWidget* label)
{
	GtkWidget* colorselection;
	GtkWidget *colorsel;
	GdkColor color;
	gint resp;
	gchar* markup;
	gchar* colorstr;
	
	colorselection = gtk_color_selection_dialog_new  (_("Select a color") );
	colorsel = GTK_COLOR_SELECTION_DIALOG(colorselection)->colorsel;
	gtk_color_selection_set_has_palette( GTK_COLOR_SELECTION(colorsel), TRUE );
	
	resp = gtk_dialog_run( GTK_DIALOG(colorselection) );
	
	if( resp == GTK_RESPONSE_OK ){
		gtk_color_selection_get_current_color(
				GTK_COLOR_SELECTION(colorsel),
				&color);
		colorstr = gdk_color_to_rgb_triplet_string( &color );
		gtk_entry_set_text(GTK_ENTRY(entry),colorstr);
		markup = g_strdup_printf("<span background='%s'>   </span>",colorstr);
		gtk_label_set_text(GTK_LABEL(label),markup);
		gtk_label_set_use_markup(GTK_LABEL(label), TRUE);
		g_free( colorstr );
		g_free( markup );		
	}
	gtk_widget_destroy( colorselection );
}

void
write_results_to_document(GtkWidget* parent, CssedWindow* window)
{
	GtkWidget* checkbutton_fore;	
	GtkWidget* checkbutton_back;
	GtkWidget* checkbutton_attach;
	GtkWidget* checkbutton_border_top;	
	GtkWidget* checkbutton_border_right;	
	GtkWidget* checkbutton_border_bottom;	
	GtkWidget* checkbutton_border_left;
	GtkWidget* checkbutton_outline;	
	GtkWidget* combo_entry_fore;	
	GtkWidget* combo_entry_back;	
	GtkWidget* combo_entry_attach;	
	GtkWidget* combo_entry_border_top;
	GtkWidget* combo_entry_border_right;
	GtkWidget* combo_entry_border_bottom;	
	GtkWidget* combo_entry_border_left;	
	GtkWidget* combo_entry_outline;	
	gboolean bfore;	
	gboolean bback;
	gboolean battach;
	gboolean bborder_top;	
	gboolean bborder_right;	
	gboolean bborder_bottom;	
	gboolean bborder_left;
	gboolean boutline;	
	gchar* color;
	CssedDoc* doc;	
	
	doc = document_get_current(window);
	
	checkbutton_fore = lookup_widget(parent, "checkbutton_fore");
	checkbutton_back = lookup_widget(parent, "checkbutton_back");
	checkbutton_attach = lookup_widget(parent, "checkbutton_attach");
	checkbutton_border_top = lookup_widget(parent, "checkbutton_border_top");
	checkbutton_border_right = lookup_widget(parent, "checkbutton_border_right");
	checkbutton_border_bottom = lookup_widget(parent, "checkbutton_border_bottom");
	checkbutton_border_left = lookup_widget(parent, "checkbutton_border_left");
	checkbutton_outline = lookup_widget(parent, "checkbutton_outline");

	bfore = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(checkbutton_fore));	
	bback = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(checkbutton_back));
	battach = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(checkbutton_attach));
	bborder_top = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(checkbutton_border_top));	
	bborder_right = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(checkbutton_border_right));	
	bborder_bottom = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(checkbutton_border_bottom));	
	bborder_left = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(checkbutton_border_left));
	boutline = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(checkbutton_outline));	
	
	if(!bfore && !bback && !battach && !bborder_top && 
	   !bborder_right && !bborder_bottom && !bborder_left && !boutline )
	{
		cssed_error_message("No value to add", "Nothing was selected.");
		return;		
	}
	
	if(bfore){
		combo_entry_fore = lookup_widget(parent,  "combo_entry_fore");
		color = gtk_editable_get_chars(GTK_EDITABLE(combo_entry_fore), 0, -1);
		document_add_text(doc, "\tcolor: ");
		document_add_text(doc, color);
		document_add_text_line(doc, ";");
		g_free(color);
		color = NULL;
	}
	
	if(bback){
		combo_entry_back = lookup_widget(parent, "combo_entry_back");
		color = gtk_editable_get_chars(GTK_EDITABLE(combo_entry_back), 0, -1);	
		document_add_text(doc,"\tbackground-color: ");
		document_add_text(doc,color);
		document_add_text_line(doc,";");
		g_free(color);
		color = NULL;
	}	
	
	if(battach){
		combo_entry_attach = lookup_widget(parent, "combo_entry_attach");
		color = gtk_editable_get_chars(GTK_EDITABLE(combo_entry_attach), 0, -1);		
		document_add_text(doc,"\tbackground-attachment: ");
		document_add_text(doc,color);
		document_add_text_line(doc,";");
		g_free(color);
		color = NULL;
	}

	if(bborder_top){
		combo_entry_border_top = lookup_widget(parent, "combo_entry_border_top");
		color = gtk_editable_get_chars(GTK_EDITABLE(combo_entry_border_top), 0, -1);		
		document_add_text(doc,"\tborder-top-color: ");
		document_add_text(doc,color);
		document_add_text_line(doc,";");
		g_free(color);
		color = NULL;
	}	
	
	if(bborder_right){
		combo_entry_border_right = lookup_widget(parent, "combo_entry_border_right");
		color = gtk_editable_get_chars(GTK_EDITABLE(combo_entry_border_right), 0, -1);		
		document_add_text(doc,"\tborder-right-color: ");
		document_add_text(doc,color);
		document_add_text_line(doc,";");
		g_free(color);
		color = NULL;
	}		
	
	if(bborder_bottom){
		combo_entry_border_bottom = lookup_widget(parent, "combo_entry_border_bottom");
		color = gtk_editable_get_chars(GTK_EDITABLE(combo_entry_border_bottom), 0, -1);		
		document_add_text(doc,"\tborder-bottom-color: ");
		document_add_text(doc,color);
		document_add_text_line(doc,";");
		g_free(color);
		color = NULL;
	}	
	
	if(bborder_left){
		combo_entry_border_left = lookup_widget(parent, "combo_entry_border_left");
		color = gtk_editable_get_chars(GTK_EDITABLE(combo_entry_border_left), 0, -1);	
		document_add_text(doc,"\tborder-left-color: ");
		document_add_text(doc,color);
		document_add_text_line(doc,";");
		g_free(color);
		color = NULL;
	}	

	if(boutline){
		combo_entry_outline = lookup_widget(parent, "combo_entry_outline");
		color = gtk_editable_get_chars(GTK_EDITABLE(combo_entry_outline), 0, -1);
		document_add_text(doc,"\toutline-color: ");
		document_add_text(doc,color);
		document_add_text_line(doc,";");
		g_free(color);
		color = NULL;
	}	
}
