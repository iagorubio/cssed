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

#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <stdio.h>

#include <gdk/gdkkeysyms.h>
#include <gtk/gtk.h>

#include "cssedwindow.h"
#include "callbacks.h"
#include "interface.h"
#include "support.h"

#define GLADE_HOOKUP_OBJECT(component,widget,name) \
  g_object_set_data_full (G_OBJECT (component), name, \
    gtk_widget_ref (widget), (GDestroyNotify) gtk_widget_unref)

#define GLADE_HOOKUP_OBJECT_NO_REF(component,widget,name) \
  g_object_set_data (G_OBJECT (component), name, widget)

void on_button_switch_page_clicked (GtkButton * button, gpointer user_data);
void on_button_selector_add_clicked (GtkButton * button, gpointer user_data);
void on_button_selector_del_clicked (GtkButton * button, gpointer user_data);
void on_button_selector_clean_clicked (GtkButton * button, gpointer user_data);
void on_button_child_add_clicked (GtkButton * button, gpointer user_data);
void on_button_child_del_clicked (GtkButton * button, gpointer user_data);
void on_button_child_clear_clicked (GtkButton * button, gpointer user_data);
void on_entry_class_changed (GtkEditable * editable, gpointer user_data);
void on_button_class_add_clicked (GtkButton * button, gpointer user_data);
void on_button_class_del_clicked (GtkButton * button, gpointer user_data);
void on_button_class_clean_clicked (GtkButton * button, gpointer user_data);
void on_button_sibling_add_clicked (GtkButton * button, gpointer user_data);
void on_button_sibling_del_clicked (GtkButton * button, gpointer user_data);
void on_button_sibling_clean_clicked (GtkButton * button, gpointer user_data);
void on_button_contextual_add_clicked (GtkButton * button, gpointer user_data);
void on_button_contextual_del_clicked (GtkButton * button, gpointer user_data);
void on_button_contextual_clean_clicked (GtkButton * button, gpointer user_data);

/* generic function used by callbacks */
void
tree_view_store_clean_by_name_from_lookup (GtkWidget* same_top_level_widget, 
										gchar* name);
void
tree_view_delete_selected_by_name_from_lookup (GtkWidget* same_top_level_widget, 
												gchar* name);

void
tree_view_add_string_from_list_by_name (GtkWidget* same_top_level_widget,
										gchar* input_list_name,
										gchar* output_list_name);
void
tree_view_add_strings_from_two_lists_by_name(GtkWidget* same_top_level_widget,
												gchar* first_list_name,
												gchar* second_list_name,
												gchar* output_list_name,
												gchar* str_to_join);
void
tree_view_add_string_from_list_and_entry_by_name 
										(GtkWidget* same_top_level_widget,
										gchar* input_list_name,
										gchar* output_list_name,
										gchar* entry_name,
										gchar* str_to_join);
/* create UI */												
GtkWidget *
create_selector_wizard ()
{
	GtkWidget *selector_wizard;
	GtkWidget *dialog_vbox;
	GtkWidget *vbox_main;
	GtkWidget *hbox_main;
	GtkWidget *vbox_buttons;
	GtkWidget *vbuttonbox_main;
	GtkWidget *button_selector;
	GtkWidget *button_child;
	GtkWidget *button_class;
	GtkWidget *button_sibling;
	GtkWidget *button_contextual;
	GtkWidget *vbox_declaration_radio;
	GtkWidget *label_set_styles;
	GtkWidget *radiobutton_for_each;
	GSList *radiobutton_for_each_group = NULL;
	GtkWidget *radiobutton_for_all;
	GtkWidget *notebook_wizard;
	GtkWidget *vbox_selector;
	GtkWidget *label_selector_header;
	GtkWidget *label_selector_detail;
	GtkWidget *hbox_selector_lists;
	GtkWidget *scrolledwindow_selector;
	GtkWidget *treeview_selector;
	GtkWidget *vbuttonbox_selector;
	GtkWidget *button_selector_add;
	GtkWidget *button_selector_del;
	GtkWidget *button_selector_clean;
	GtkWidget *scrolledwindow_selector_selected;
	GtkWidget *treeview_selector_selected;
	GtkWidget *label_tab_first;
	GtkWidget *vbox_child;
	GtkWidget *label_child_header;
	GtkWidget *label_child_detail;
	GtkWidget *hbox_child_list;
	GtkWidget *hbox_child;
	GtkWidget *scrolledwindow_child_parent;
	GtkWidget *treeview_child_parent;
	GtkWidget *scrolledwindow_child_child;
	GtkWidget *treeview_child_child;
	GtkWidget *vbuttonbox_child;
	GtkWidget *button_child_add;
	GtkWidget *button_child_del;
	GtkWidget *button_child_clear;
	GtkWidget *scrolledwindow_child_selected;
	GtkWidget *treeview_child_selected;
	GtkWidget *label_tab_second;
	GtkWidget *vbox_class;
	GtkWidget *label_class_header;
	GtkWidget *label_class_detail;
	GtkWidget *hbox_class_list;
	GtkWidget *scrolledwindow_class;
	GtkWidget *treeview_class;
	GtkWidget *vbox_class_buttons;
	GtkWidget *entry_class;
	GtkWidget *vbuttonbox_class;
	GtkWidget *button_class_add;
	GtkWidget *button_class_del;
	GtkWidget *button_class_clean;
	GtkWidget *scrolledwindow_class_selected;
	GtkWidget *treeview_class_selected;
	GtkWidget *label_tab_third;
	GtkWidget *vbox_sibling;
	GtkWidget *label_sibling_header;
	GtkWidget *label_sibling_detail;
	GtkWidget *hbox_sibling_lists;
	GtkWidget *hbox_sibling_first_and_second;
	GtkWidget *scrolledwindow_sibling_first;
	GtkWidget *treeview_sibling_fist;
	GtkWidget *scrolledwindow_sibling_second;
	GtkWidget *treeview_sibling_second;
	GtkWidget *vbuttonbox_sibling;
	GtkWidget *button_sibling_add;
	GtkWidget *button_sibling_del;
	GtkWidget *button_sibling_clean;
	GtkWidget *scrolledwindow_sibling_selected;
	GtkWidget *treeview_sibling_selected;
	GtkWidget *label_tab_fourth;
	GtkWidget *vbox_contextual;
	GtkWidget *label_contextual_header;
	GtkWidget *label_contextual_detail;
	GtkWidget *hbox_contextual_lists;
	GtkWidget *hbox_contextual;
	GtkWidget *scrolledwindow_contextual_first;
	GtkWidget *treeview_contextual_first;
	GtkWidget *scrolledwindow_contextual_secont;
	GtkWidget *treeview_contextual_second;
	GtkWidget *vbuttonbox_contextual;
	GtkWidget *button_contextual_add;
	GtkWidget *button_contextual_del;
	GtkWidget *button_contextual_clean;
	GtkWidget *scrolledwindow_contextual_selected;
	GtkWidget *treeview_contextual_selected;
	GtkWidget *label_tab_five;
	GtkWidget *dialog_action_area;
	GtkWidget *okbutton;
	GtkWidget *cancelbutton;
	// selector list
	GtkListStore *selector_store;
	GtkTreeViewColumn *selector_column;
	GtkTreeViewColumn *child_parent_column;
	GtkTreeViewColumn *child_child_column;
	GtkTreeViewColumn *class_column;
	GtkTreeViewColumn *sibling_first_column;
	GtkTreeViewColumn *sibling_second_column;
	GtkTreeViewColumn *contextual_first_column;
	GtkTreeViewColumn *contextual_second_column;
	GtkCellRenderer *selector_render;
	// output stores
	GtkListStore *selector_selected_store;
	GtkListStore *child_selected_store;
	GtkListStore *class_selected_store;
	GtkListStore *sibling_selected_store;
	GtkListStore *contextual_selected_store;
	// output columns
	GtkTreeViewColumn *selector_selected_column;
	GtkTreeViewColumn *child_selected_column;
	GtkTreeViewColumn *class_selected_column;
	GtkTreeViewColumn *sibling_selected_column;
	GtkTreeViewColumn *contextual_selected_column;
	// iter
	GtkTreeIter iter;
	gint i;
	const gchar *selectors[] = {
		"a", "a:link", "a:active", "a:visited", "a:hover", "address", "blockquote",
		"body", "br", "button", "caption", "cite", "code", "col", "colgroup",
		"dd","dfn","div","dl","dt","em","fieldset","form","h1","h2","h3","h4","h5","h6",
		"hr","i","img","input","kbd","label","legend","li","link","ol","optgroup",
		"option","p","pre","q","s","samp","select","small","span","strike","strong",
		"sub","sup","table","tbody","td","textarea","tfoot","th","thead","title",
		"tr","tt","u","ul","var",		
		NULL		// must end in null        
	};
	// set up items to the selectr lists
	selector_store = gtk_list_store_new (1, G_TYPE_STRING);
	selector_render = gtk_cell_renderer_text_new ();
	// set output stores
	selector_selected_store = gtk_list_store_new (1, G_TYPE_STRING);
	child_selected_store = gtk_list_store_new (1, G_TYPE_STRING);
	class_selected_store = gtk_list_store_new (1, G_TYPE_STRING);
	sibling_selected_store = gtk_list_store_new (1, G_TYPE_STRING);
	contextual_selected_store = gtk_list_store_new (1, G_TYPE_STRING);

	// setup columns for all
	selector_selected_column =
		gtk_tree_view_column_new_with_attributes ("Current",
							  selector_render,
							  "markup", 0, NULL);
	child_selected_column =
		gtk_tree_view_column_new_with_attributes ("Current",
							  selector_render,
							  "markup", 0, NULL);
	class_selected_column =
		gtk_tree_view_column_new_with_attributes ("Current",
							  selector_render,
							  "markup", 0, NULL);
	sibling_selected_column =
		gtk_tree_view_column_new_with_attributes ("Current",
							  selector_render,
							  "markup", 0, NULL);
	contextual_selected_column =
		gtk_tree_view_column_new_with_attributes ("Current",
							  selector_render,
							  "markup", 0, NULL);
	selector_column =
		gtk_tree_view_column_new_with_attributes ("Selector",
							  selector_render,
							  "markup", 0, NULL);
	child_parent_column =
		gtk_tree_view_column_new_with_attributes ("Selector",
							  selector_render,
							  "markup", 0, NULL);
	child_child_column =
		gtk_tree_view_column_new_with_attributes ("Child",
							  selector_render,
							  "markup", 0, NULL);
	class_column =
		gtk_tree_view_column_new_with_attributes ("Selector",
							  selector_render,
							  "markup", 0, NULL);
	sibling_first_column =
		gtk_tree_view_column_new_with_attributes ("First element",
							  selector_render,
							  "markup", 0, NULL);
	sibling_second_column =
		gtk_tree_view_column_new_with_attributes ("Second element",
							  selector_render,
							  "markup", 0, NULL);
	contextual_first_column =
		gtk_tree_view_column_new_with_attributes ("Ancestor",
							  selector_render,
							  "markup", 0, NULL);
	contextual_second_column =
		gtk_tree_view_column_new_with_attributes ("Second element",
							  selector_render,
							  "markup", 0, NULL);
//
	for (i = 0; selectors[i]; i++)
	{
		gtk_list_store_append (GTK_LIST_STORE (selector_store), &iter);
		gtk_list_store_set (GTK_LIST_STORE (selector_store), &iter, 0,
				    selectors[i], -1);
	}
//
	selector_wizard = gtk_dialog_new ();
	gtk_window_set_title (GTK_WINDOW (selector_wizard), _("Selector wizard"));
	gtk_window_set_destroy_with_parent (GTK_WINDOW (selector_wizard),   TRUE);
	gtk_window_set_position (GTK_WINDOW (selector_wizard), GTK_WIN_POS_CENTER);

	dialog_vbox = GTK_DIALOG (selector_wizard)->vbox;
	gtk_widget_show (dialog_vbox);

	vbox_main = gtk_vbox_new (FALSE, 0);
	gtk_widget_show (vbox_main);
	gtk_box_pack_start (GTK_BOX (dialog_vbox), vbox_main, TRUE, TRUE, 0);

	hbox_main = gtk_hbox_new (FALSE, 0);
	gtk_widget_show (hbox_main);
	gtk_box_pack_start (GTK_BOX (vbox_main), hbox_main, TRUE, TRUE, 0);

	vbox_buttons = gtk_vbox_new (FALSE, 0);
	gtk_widget_show (vbox_buttons);
	gtk_box_pack_start (GTK_BOX (hbox_main), vbox_buttons, FALSE, FALSE,
			    5);

	vbuttonbox_main = gtk_vbutton_box_new ();
	gtk_widget_show (vbuttonbox_main);
	gtk_box_pack_start (GTK_BOX (vbox_buttons), vbuttonbox_main, TRUE,
			    TRUE, 0);
	gtk_button_box_set_layout (GTK_BUTTON_BOX (vbuttonbox_main),
				   GTK_BUTTONBOX_START);
	gtk_box_set_spacing (GTK_BOX (vbuttonbox_main), 2);


	button_selector = gtk_button_new_with_mnemonic (_("Selector"));
	gtk_widget_show (button_selector);
	gtk_container_add (GTK_CONTAINER (vbuttonbox_main), button_selector);
	GTK_WIDGET_SET_FLAGS (button_selector, GTK_CAN_DEFAULT);

	button_child = gtk_button_new_with_mnemonic (_("Child"));
	gtk_widget_show (button_child);
	gtk_container_add (GTK_CONTAINER (vbuttonbox_main), button_child);
	GTK_WIDGET_SET_FLAGS (button_child, GTK_CAN_DEFAULT);

	button_class = gtk_button_new_with_mnemonic (_("Class"));
	gtk_widget_show (button_class);
	gtk_container_add (GTK_CONTAINER (vbuttonbox_main), button_class);
	GTK_WIDGET_SET_FLAGS (button_class, GTK_CAN_DEFAULT);

	button_sibling = gtk_button_new_with_mnemonic (_("Sibling"));
	gtk_widget_show (button_sibling);
	gtk_container_add (GTK_CONTAINER (vbuttonbox_main), button_sibling);
	GTK_WIDGET_SET_FLAGS (button_sibling, GTK_CAN_DEFAULT);

	button_contextual = gtk_button_new_with_mnemonic (_("Contextual"));
	gtk_widget_show (button_contextual);
	gtk_container_add (GTK_CONTAINER (vbuttonbox_main),
			   button_contextual);
	GTK_WIDGET_SET_FLAGS (button_contextual, GTK_CAN_DEFAULT);
	 /**/ vbox_declaration_radio = gtk_vbox_new (FALSE, 0);
	gtk_widget_show (vbox_declaration_radio);
	gtk_box_pack_start (GTK_BOX (vbox_buttons), vbox_declaration_radio,
			    FALSE, FALSE, 0);

	label_set_styles = gtk_label_new (_("Declarations:"));
	gtk_widget_show (label_set_styles);
	gtk_box_pack_start (GTK_BOX (vbox_declaration_radio),
			    label_set_styles, FALSE, FALSE, 0);
	gtk_label_set_justify (GTK_LABEL (label_set_styles),
			       GTK_JUSTIFY_LEFT);

	radiobutton_for_each =
		gtk_radio_button_new_with_mnemonic (NULL, _("One for each"));
	gtk_widget_show (radiobutton_for_each);
	gtk_box_pack_start (GTK_BOX (vbox_declaration_radio),
			    radiobutton_for_each, FALSE, FALSE, 0);
	gtk_radio_button_set_group (GTK_RADIO_BUTTON (radiobutton_for_each),
				    radiobutton_for_each_group);
	radiobutton_for_each_group =
		gtk_radio_button_get_group (GTK_RADIO_BUTTON
					    (radiobutton_for_each));

	radiobutton_for_all =
		gtk_radio_button_new_with_mnemonic (NULL, _("One for all"));
	gtk_widget_show (radiobutton_for_all);
	gtk_box_pack_start (GTK_BOX (vbox_declaration_radio),
			    radiobutton_for_all, FALSE, FALSE, 0);
	gtk_radio_button_set_group (GTK_RADIO_BUTTON (radiobutton_for_all),
				    radiobutton_for_each_group);
	radiobutton_for_each_group =
		gtk_radio_button_get_group (GTK_RADIO_BUTTON
					    (radiobutton_for_all));

	notebook_wizard = gtk_notebook_new ();
	gtk_widget_show (notebook_wizard);
	gtk_box_pack_start (GTK_BOX (hbox_main), notebook_wizard, TRUE, TRUE,
			    0);
	GTK_WIDGET_UNSET_FLAGS (notebook_wizard, GTK_CAN_FOCUS);
	gtk_notebook_set_show_tabs (GTK_NOTEBOOK (notebook_wizard), FALSE);
	gtk_notebook_set_tab_pos (GTK_NOTEBOOK (notebook_wizard),
				  GTK_POS_LEFT);
	gtk_widget_set_size_request (GTK_WIDGET (notebook_wizard), 550, 350);

	vbox_selector = gtk_vbox_new (FALSE, 0);
	gtk_widget_show (vbox_selector);
	gtk_container_add (GTK_CONTAINER (notebook_wizard), vbox_selector);

	label_selector_header =
		gtk_label_new (_
			       ("<span size='larger'>Single Selector</span>"));
	gtk_widget_show (label_selector_header);
	gtk_box_pack_start (GTK_BOX (vbox_selector), label_selector_header,
			    FALSE, FALSE, 5);
	gtk_label_set_use_markup (GTK_LABEL (label_selector_header), TRUE);
	gtk_label_set_justify (GTK_LABEL (label_selector_header),
			       GTK_JUSTIFY_LEFT);

	label_selector_detail =
		gtk_label_new (_
			       ("<span size='small'>This style will be applied to all tags that match the selector.</span>"));
	gtk_widget_show (label_selector_detail);
	gtk_box_pack_start (GTK_BOX (vbox_selector), label_selector_detail,
			    FALSE, FALSE, 5);
	gtk_label_set_use_markup (GTK_LABEL (label_selector_detail), TRUE);
	gtk_label_set_justify (GTK_LABEL (label_selector_detail),
			       GTK_JUSTIFY_LEFT);
	gtk_label_set_line_wrap (GTK_LABEL (label_selector_detail), TRUE);

	hbox_selector_lists = gtk_hbox_new (FALSE, 0);
	gtk_widget_show (hbox_selector_lists);
	gtk_box_pack_start (GTK_BOX (vbox_selector), hbox_selector_lists,
			    TRUE, TRUE, 0);

	scrolledwindow_selector = gtk_scrolled_window_new (NULL, NULL);
	gtk_widget_show (scrolledwindow_selector);
	gtk_box_pack_start (GTK_BOX (hbox_selector_lists),
			    scrolledwindow_selector, TRUE, TRUE, 2);

	treeview_selector = gtk_tree_view_new ();
	gtk_widget_show (treeview_selector);
	gtk_container_add (GTK_CONTAINER (scrolledwindow_selector),
			   treeview_selector);
	gtk_tree_view_set_enable_search (GTK_TREE_VIEW (treeview_selector),
					 FALSE);
//
	gtk_tree_view_insert_column (GTK_TREE_VIEW (treeview_selector),
				     selector_column, 0);

	gtk_tree_view_set_model (GTK_TREE_VIEW (treeview_selector),
				 GTK_TREE_MODEL (selector_store));
	g_object_unref(selector_store);
	gtk_tree_view_set_headers_visible (GTK_TREE_VIEW (treeview_selector),
					   TRUE);
//

	vbuttonbox_selector = gtk_vbutton_box_new ();
	gtk_widget_show (vbuttonbox_selector);
	gtk_box_pack_start (GTK_BOX (hbox_selector_lists),
			    vbuttonbox_selector, FALSE, FALSE, 0);
	gtk_container_set_border_width (GTK_CONTAINER (vbuttonbox_selector),
					5);
	gtk_button_box_set_layout (GTK_BUTTON_BOX (vbuttonbox_selector),
				   GTK_BUTTONBOX_START);
	gtk_box_set_spacing (GTK_BOX (vbuttonbox_selector), 2);

	button_selector_add = gtk_button_new_from_stock ("gtk-add");
	gtk_widget_show (button_selector_add);
	gtk_container_add (GTK_CONTAINER (vbuttonbox_selector),
			   button_selector_add);
	GTK_WIDGET_SET_FLAGS (button_selector_add, GTK_CAN_DEFAULT);

	button_selector_del = gtk_button_new_from_stock ("gtk-delete");
	gtk_widget_show (button_selector_del);
	gtk_container_add (GTK_CONTAINER (vbuttonbox_selector),
			   button_selector_del);
	GTK_WIDGET_SET_FLAGS (button_selector_del, GTK_CAN_DEFAULT);

	button_selector_clean = gtk_button_new_from_stock ("gtk-clear");
	gtk_widget_show (button_selector_clean);
	gtk_container_add (GTK_CONTAINER (vbuttonbox_selector),
			   button_selector_clean);
	GTK_WIDGET_SET_FLAGS (button_selector_clean, GTK_CAN_DEFAULT);

	scrolledwindow_selector_selected =
		gtk_scrolled_window_new (NULL, NULL);
	gtk_widget_show (scrolledwindow_selector_selected);
	gtk_box_pack_start (GTK_BOX (hbox_selector_lists),
			    scrolledwindow_selector_selected, TRUE, TRUE, 2);

	treeview_selector_selected = gtk_tree_view_new ();
	gtk_widget_show (treeview_selector_selected);
	gtk_container_add (GTK_CONTAINER (scrolledwindow_selector_selected),
			   treeview_selector_selected);
	gtk_tree_view_set_enable_search (GTK_TREE_VIEW
					 (treeview_selector_selected), FALSE);
//      
	gtk_tree_view_insert_column (GTK_TREE_VIEW
				     (treeview_selector_selected),
				     selector_selected_column, 0);

	gtk_tree_view_set_model (GTK_TREE_VIEW (treeview_selector_selected),
				 GTK_TREE_MODEL (selector_selected_store));
	g_object_unref(selector_selected_store);
	gtk_tree_view_set_headers_visible (GTK_TREE_VIEW
					   (treeview_selector_selected),
					   TRUE);
//
	label_tab_first = gtk_label_new ("1");
	gtk_widget_show (label_tab_first);
	gtk_notebook_set_tab_label (GTK_NOTEBOOK (notebook_wizard),
				    gtk_notebook_get_nth_page (GTK_NOTEBOOK
							       (notebook_wizard),
							       0),
				    label_tab_first);
	gtk_label_set_justify (GTK_LABEL (label_tab_first), GTK_JUSTIFY_LEFT);

	vbox_child = gtk_vbox_new (FALSE, 0);
	gtk_widget_show (vbox_child);
	gtk_container_add (GTK_CONTAINER (notebook_wizard), vbox_child);

	label_child_header =
		gtk_label_new (_
			       ("<span size='larger'>Child selectors</span>"));
	gtk_widget_show (label_child_header);
	gtk_box_pack_start (GTK_BOX (vbox_child), label_child_header, FALSE,
			    FALSE, 5);
	gtk_label_set_use_markup (GTK_LABEL (label_child_header), TRUE);
	gtk_label_set_justify (GTK_LABEL (label_child_header),
			       GTK_JUSTIFY_LEFT);

	label_child_detail =
		gtk_label_new (_
			       ("<span size='small'>This style will be applied to all tags with a matching parent. <i>p &gt; u { style }</i>, will be applied to all <i>u</i> tags with a <i>p</i> parent</span>"));
	gtk_widget_show (label_child_detail);
	gtk_box_pack_start (GTK_BOX (vbox_child), label_child_detail, FALSE,
			    FALSE, 5);
	gtk_label_set_use_markup (GTK_LABEL (label_child_detail), TRUE);
	gtk_label_set_justify (GTK_LABEL (label_child_detail),
			       GTK_JUSTIFY_LEFT);
	gtk_label_set_line_wrap (GTK_LABEL (label_child_detail), TRUE);

	hbox_child_list = gtk_hbox_new (FALSE, 0);
	gtk_widget_show (hbox_child_list);
	gtk_box_pack_start (GTK_BOX (vbox_child), hbox_child_list, TRUE, TRUE,
			    0);

	hbox_child = gtk_hbox_new (FALSE, 0);
	gtk_widget_show (hbox_child);
	gtk_box_pack_start (GTK_BOX (hbox_child_list), hbox_child, TRUE, TRUE,
			    0);

	scrolledwindow_child_parent = gtk_scrolled_window_new (NULL, NULL);
	gtk_widget_show (scrolledwindow_child_parent);
	gtk_box_pack_start (GTK_BOX (hbox_child), scrolledwindow_child_parent,
			    TRUE, TRUE, 2);

	treeview_child_parent = gtk_tree_view_new ();
	gtk_widget_show (treeview_child_parent);
	gtk_container_add (GTK_CONTAINER (scrolledwindow_child_parent),
			   treeview_child_parent);
//      
	gtk_tree_view_insert_column (GTK_TREE_VIEW (treeview_child_parent),
				     child_parent_column, 0);

	gtk_tree_view_set_model (GTK_TREE_VIEW (treeview_child_parent),
				 GTK_TREE_MODEL (selector_store));
	gtk_tree_view_set_headers_visible (GTK_TREE_VIEW
					   (treeview_child_parent), TRUE);
//
	scrolledwindow_child_child = gtk_scrolled_window_new (NULL, NULL);
	gtk_widget_show (scrolledwindow_child_child);
	gtk_box_pack_start (GTK_BOX (hbox_child), scrolledwindow_child_child,
			    TRUE, TRUE, 2);

	treeview_child_child = gtk_tree_view_new ();
	gtk_widget_show (treeview_child_child);
	gtk_container_add (GTK_CONTAINER (scrolledwindow_child_child),
			   treeview_child_child);
//
	gtk_tree_view_insert_column (GTK_TREE_VIEW (treeview_child_child),
				     child_child_column, 0);

	gtk_tree_view_set_model (GTK_TREE_VIEW (treeview_child_child),
				 GTK_TREE_MODEL (selector_store));
	gtk_tree_view_set_headers_visible (GTK_TREE_VIEW
					   (treeview_child_child), TRUE);
//
	vbuttonbox_child = gtk_vbutton_box_new ();
	gtk_widget_show (vbuttonbox_child);
	gtk_box_pack_start (GTK_BOX (hbox_child_list), vbuttonbox_child,
			    FALSE, FALSE, 0);
	gtk_container_set_border_width (GTK_CONTAINER (vbuttonbox_child), 5);
	gtk_button_box_set_layout (GTK_BUTTON_BOX (vbuttonbox_child),
				   GTK_BUTTONBOX_START);
	gtk_box_set_spacing (GTK_BOX (vbuttonbox_child), 2);

	button_child_add = gtk_button_new_from_stock ("gtk-add");
	gtk_widget_show (button_child_add);
	gtk_container_add (GTK_CONTAINER (vbuttonbox_child),
			   button_child_add);
	GTK_WIDGET_SET_FLAGS (button_child_add, GTK_CAN_DEFAULT);

	button_child_del = gtk_button_new_from_stock ("gtk-delete");
	gtk_widget_show (button_child_del);
	gtk_container_add (GTK_CONTAINER (vbuttonbox_child),
			   button_child_del);
	GTK_WIDGET_SET_FLAGS (button_child_del, GTK_CAN_DEFAULT);

	button_child_clear = gtk_button_new_from_stock ("gtk-clear");
	gtk_widget_show (button_child_clear);
	gtk_container_add (GTK_CONTAINER (vbuttonbox_child),
			   button_child_clear);
	GTK_WIDGET_SET_FLAGS (button_child_clear, GTK_CAN_DEFAULT);

	scrolledwindow_child_selected = gtk_scrolled_window_new (NULL, NULL);
	gtk_widget_show (scrolledwindow_child_selected);
	gtk_box_pack_start (GTK_BOX (hbox_child_list),
			    scrolledwindow_child_selected, TRUE, TRUE, 2);

	treeview_child_selected = gtk_tree_view_new ();
	gtk_widget_show (treeview_child_selected);
	gtk_container_add (GTK_CONTAINER (scrolledwindow_child_selected),
			   treeview_child_selected);
//      
	gtk_tree_view_insert_column (GTK_TREE_VIEW (treeview_child_selected),
				     child_selected_column, 0);

	gtk_tree_view_set_model (GTK_TREE_VIEW (treeview_child_selected),
				 GTK_TREE_MODEL (child_selected_store));
	g_object_unref(child_selected_store);
	gtk_tree_view_set_headers_visible (GTK_TREE_VIEW
					   (treeview_child_selected), TRUE);
//

	label_tab_second = gtk_label_new ("2");
	gtk_widget_show (label_tab_second);
	gtk_notebook_set_tab_label (GTK_NOTEBOOK (notebook_wizard),
				    gtk_notebook_get_nth_page (GTK_NOTEBOOK
							       (notebook_wizard),
							       1),
				    label_tab_second);
	gtk_label_set_use_markup (GTK_LABEL (label_tab_second), TRUE);
	gtk_label_set_justify (GTK_LABEL (label_tab_second),
			       GTK_JUSTIFY_LEFT);
	gtk_label_set_line_wrap (GTK_LABEL (label_tab_second), TRUE);

	vbox_class = gtk_vbox_new (FALSE, 0);
	gtk_widget_show (vbox_class);
	gtk_container_add (GTK_CONTAINER (notebook_wizard), vbox_class);

	label_class_header =
		gtk_label_new (_("<span size='large'>Class</span>"));
	gtk_widget_show (label_class_header);
	gtk_box_pack_start (GTK_BOX (vbox_class), label_class_header, FALSE,
			    FALSE, 5);
	gtk_label_set_use_markup (GTK_LABEL (label_class_header), TRUE);
	gtk_label_set_justify (GTK_LABEL (label_class_header),
			       GTK_JUSTIFY_LEFT);

	label_class_detail =
		gtk_label_new (_
			       ("<span size='small'>This style will be applied to all tag matching the selector with a class attribute that matches. The tag <i>&lt;p class=&quot;test&quot;&gt;</i> will match the class <i>p.test</i></span>."));
	gtk_widget_show (label_class_detail);
	gtk_box_pack_start (GTK_BOX (vbox_class), label_class_detail, FALSE,
			    FALSE, 5);
	gtk_label_set_use_markup (GTK_LABEL (label_class_detail), TRUE);
	gtk_label_set_justify (GTK_LABEL (label_class_detail),
			       GTK_JUSTIFY_LEFT);
	gtk_label_set_line_wrap (GTK_LABEL (label_class_detail), TRUE);

	hbox_class_list = gtk_hbox_new (FALSE, 0);
	gtk_widget_show (hbox_class_list);
	gtk_box_pack_start (GTK_BOX (vbox_class), hbox_class_list, TRUE, TRUE,
			    0);

	scrolledwindow_class = gtk_scrolled_window_new (NULL, NULL);
	gtk_widget_show (scrolledwindow_class);
	gtk_box_pack_start (GTK_BOX (hbox_class_list), scrolledwindow_class,
			    TRUE, TRUE, 2);

	treeview_class = gtk_tree_view_new ();
	gtk_widget_show (treeview_class);
	gtk_container_add (GTK_CONTAINER (scrolledwindow_class),
			   treeview_class);
//
	gtk_tree_view_insert_column (GTK_TREE_VIEW (treeview_class),
				     class_column, 0);
	gtk_tree_view_set_model (GTK_TREE_VIEW (treeview_class),
				 GTK_TREE_MODEL (selector_store));
	gtk_tree_view_set_headers_visible (GTK_TREE_VIEW (treeview_class),
					   TRUE);
//
	vbox_class_buttons = gtk_vbox_new (FALSE, 0);
	gtk_widget_show (vbox_class_buttons);
	gtk_box_pack_start (GTK_BOX (hbox_class_list), vbox_class_buttons,
			    FALSE, FALSE, 10);

	entry_class = gtk_entry_new ();
	gtk_widget_show (entry_class);
	gtk_box_pack_start (GTK_BOX (vbox_class_buttons), entry_class, FALSE,
			    FALSE, 5);
	gtk_widget_set_size_request (entry_class, 80, 24);

	vbuttonbox_class = gtk_vbutton_box_new ();
	gtk_widget_show (vbuttonbox_class);
	gtk_box_pack_start (GTK_BOX (vbox_class_buttons), vbuttonbox_class,
			    FALSE, FALSE, 0);
	gtk_button_box_set_layout (GTK_BUTTON_BOX (vbuttonbox_class),
				   GTK_BUTTONBOX_START);
	gtk_box_set_spacing (GTK_BOX (vbuttonbox_class), 2);

	button_class_add = gtk_button_new_from_stock ("gtk-add");
	gtk_widget_show (button_class_add);
	gtk_container_add (GTK_CONTAINER (vbuttonbox_class),
			   button_class_add);
	GTK_WIDGET_SET_FLAGS (button_class_add, GTK_CAN_DEFAULT);

	button_class_del = gtk_button_new_from_stock ("gtk-delete");
	gtk_widget_show (button_class_del);
	gtk_container_add (GTK_CONTAINER (vbuttonbox_class),
			   button_class_del);
	GTK_WIDGET_SET_FLAGS (button_class_del, GTK_CAN_DEFAULT);

	button_class_clean = gtk_button_new_from_stock ("gtk-clear");
	gtk_widget_show (button_class_clean);
	gtk_container_add (GTK_CONTAINER (vbuttonbox_class),
			   button_class_clean);
	GTK_WIDGET_SET_FLAGS (button_class_clean, GTK_CAN_DEFAULT);

	scrolledwindow_class_selected = gtk_scrolled_window_new (NULL, NULL);
	gtk_widget_show (scrolledwindow_class_selected);
	gtk_box_pack_start (GTK_BOX (hbox_class_list),
			    scrolledwindow_class_selected, TRUE, TRUE, 2);

	treeview_class_selected = gtk_tree_view_new ();
	gtk_widget_show (treeview_class_selected);
	gtk_container_add (GTK_CONTAINER (scrolledwindow_class_selected),
			   treeview_class_selected);
//      
	gtk_tree_view_insert_column (GTK_TREE_VIEW (treeview_class_selected),
				     class_selected_column, 0);

	gtk_tree_view_set_model (GTK_TREE_VIEW (treeview_class_selected),
				 GTK_TREE_MODEL (class_selected_store));
	g_object_unref(class_selected_store);
	gtk_tree_view_set_headers_visible (GTK_TREE_VIEW
					   (treeview_class_selected), TRUE);
//
	label_tab_third = gtk_label_new ("3");
	gtk_widget_show (label_tab_third);
	gtk_notebook_set_tab_label (GTK_NOTEBOOK (notebook_wizard),
				    gtk_notebook_get_nth_page (GTK_NOTEBOOK
							       (notebook_wizard),
							       2),
				    label_tab_third);
	gtk_label_set_justify (GTK_LABEL (label_tab_third), GTK_JUSTIFY_LEFT);

	vbox_sibling = gtk_vbox_new (FALSE, 0);
	gtk_widget_show (vbox_sibling);
	gtk_container_add (GTK_CONTAINER (notebook_wizard), vbox_sibling);

	label_sibling_header =
		gtk_label_new (_
			       ("<span size='larger'>Sibling selectors</span>"));
	gtk_widget_show (label_sibling_header);
	gtk_box_pack_start (GTK_BOX (vbox_sibling), label_sibling_header,
			    FALSE, FALSE, 5);
	gtk_label_set_use_markup (GTK_LABEL (label_sibling_header), TRUE);
	gtk_label_set_justify (GTK_LABEL (label_sibling_header),
			       GTK_JUSTIFY_LEFT);

	label_sibling_detail =
		gtk_label_new (_
			       ("<span size='small'>This style will be applied to any tag preceding other tag with the same parent. <i>p + u { style }</i>, will be applied to all <i>u</i> tags preceded by a <i>p</i> tag with the same parent</span>"));
	gtk_widget_show (label_sibling_detail);
	gtk_box_pack_start (GTK_BOX (vbox_sibling), label_sibling_detail,
			    FALSE, FALSE, 5);
	gtk_label_set_use_markup (GTK_LABEL (label_sibling_detail), TRUE);
	gtk_label_set_justify (GTK_LABEL (label_sibling_detail),
			       GTK_JUSTIFY_LEFT);
	gtk_label_set_line_wrap (GTK_LABEL (label_sibling_detail), TRUE);

	hbox_sibling_lists = gtk_hbox_new (FALSE, 0);
	gtk_widget_show (hbox_sibling_lists);
	gtk_box_pack_start (GTK_BOX (vbox_sibling), hbox_sibling_lists, TRUE,
			    TRUE, 0);

	hbox_sibling_first_and_second = gtk_hbox_new (FALSE, 0);
	gtk_widget_show (hbox_sibling_first_and_second);
	gtk_box_pack_start (GTK_BOX (hbox_sibling_lists),
			    hbox_sibling_first_and_second, TRUE, TRUE, 0);

	scrolledwindow_sibling_first = gtk_scrolled_window_new (NULL, NULL);
	gtk_widget_show (scrolledwindow_sibling_first);
	gtk_box_pack_start (GTK_BOX (hbox_sibling_first_and_second),
			    scrolledwindow_sibling_first, TRUE, TRUE, 2);

	treeview_sibling_fist = gtk_tree_view_new ();
	gtk_widget_show (treeview_sibling_fist);
	gtk_container_add (GTK_CONTAINER (scrolledwindow_sibling_first),
			   treeview_sibling_fist);
//
	gtk_tree_view_insert_column (GTK_TREE_VIEW (treeview_sibling_fist),
				     sibling_first_column, 0);
	gtk_tree_view_set_model (GTK_TREE_VIEW (treeview_sibling_fist),
				 GTK_TREE_MODEL (selector_store));
	gtk_tree_view_set_headers_visible (GTK_TREE_VIEW
					   (treeview_sibling_fist), TRUE);
//
	scrolledwindow_sibling_second = gtk_scrolled_window_new (NULL, NULL);
	gtk_widget_show (scrolledwindow_sibling_second);
	gtk_box_pack_start (GTK_BOX (hbox_sibling_first_and_second),
			    scrolledwindow_sibling_second, TRUE, TRUE, 2);

	treeview_sibling_second = gtk_tree_view_new ();
	gtk_widget_show (treeview_sibling_second);
	gtk_container_add (GTK_CONTAINER (scrolledwindow_sibling_second),
			   treeview_sibling_second);
//
	gtk_tree_view_insert_column (GTK_TREE_VIEW (treeview_sibling_second),
				     sibling_second_column, 0);
	gtk_tree_view_set_model (GTK_TREE_VIEW (treeview_sibling_second),
				 GTK_TREE_MODEL (selector_store));
	gtk_tree_view_set_headers_visible (GTK_TREE_VIEW
					   (treeview_sibling_second), TRUE);
//
	vbuttonbox_sibling = gtk_vbutton_box_new ();
	gtk_widget_show (vbuttonbox_sibling);
	gtk_box_pack_start (GTK_BOX (hbox_sibling_lists), vbuttonbox_sibling,
			    FALSE, FALSE, 0);
	gtk_container_set_border_width (GTK_CONTAINER (vbuttonbox_sibling),
					5);
	gtk_button_box_set_layout (GTK_BUTTON_BOX (vbuttonbox_sibling),
				   GTK_BUTTONBOX_START);
	gtk_box_set_spacing (GTK_BOX (vbuttonbox_sibling), 2);

	button_sibling_add = gtk_button_new_from_stock ("gtk-add");
	gtk_widget_show (button_sibling_add);
	gtk_container_add (GTK_CONTAINER (vbuttonbox_sibling),
			   button_sibling_add);
	GTK_WIDGET_SET_FLAGS (button_sibling_add, GTK_CAN_DEFAULT);

	button_sibling_del = gtk_button_new_from_stock ("gtk-delete");
	gtk_widget_show (button_sibling_del);
	gtk_container_add (GTK_CONTAINER (vbuttonbox_sibling),
			   button_sibling_del);
	GTK_WIDGET_SET_FLAGS (button_sibling_del, GTK_CAN_DEFAULT);

	button_sibling_clean = gtk_button_new_from_stock ("gtk-clear");
	gtk_widget_show (button_sibling_clean);
	gtk_container_add (GTK_CONTAINER (vbuttonbox_sibling),
			   button_sibling_clean);
	GTK_WIDGET_SET_FLAGS (button_sibling_clean, GTK_CAN_DEFAULT);

	scrolledwindow_sibling_selected =
		gtk_scrolled_window_new (NULL, NULL);
	gtk_widget_show (scrolledwindow_sibling_selected);
	gtk_box_pack_start (GTK_BOX (hbox_sibling_lists),
			    scrolledwindow_sibling_selected, TRUE, TRUE, 2);

	treeview_sibling_selected = gtk_tree_view_new ();
	gtk_widget_show (treeview_sibling_selected);
	gtk_container_add (GTK_CONTAINER (scrolledwindow_sibling_selected),
			   treeview_sibling_selected);
//
	gtk_tree_view_insert_column (GTK_TREE_VIEW
				     (treeview_sibling_selected),
				     sibling_selected_column, 0);

	gtk_tree_view_set_model (GTK_TREE_VIEW (treeview_sibling_selected),
				 GTK_TREE_MODEL (sibling_selected_store));
	g_object_unref(sibling_selected_store);
	gtk_tree_view_set_headers_visible (GTK_TREE_VIEW
					   (treeview_sibling_selected), TRUE);
//
	label_tab_fourth = gtk_label_new ("4");
	gtk_widget_show (label_tab_fourth);
	gtk_notebook_set_tab_label (GTK_NOTEBOOK (notebook_wizard),
				    gtk_notebook_get_nth_page (GTK_NOTEBOOK
							       (notebook_wizard),
							       3),
				    label_tab_fourth);
	gtk_label_set_justify (GTK_LABEL (label_tab_fourth),
			       GTK_JUSTIFY_LEFT);

	vbox_contextual = gtk_vbox_new (FALSE, 0);
	gtk_widget_show (vbox_contextual);
	gtk_container_add (GTK_CONTAINER (notebook_wizard), vbox_contextual);

	label_contextual_header =
		gtk_label_new (_
			       ("<span size='larger'>Contextual selectors</span>"));
	gtk_widget_show (label_contextual_header);
	gtk_box_pack_start (GTK_BOX (vbox_contextual),
			    label_contextual_header, FALSE, FALSE, 5);
	gtk_label_set_use_markup (GTK_LABEL (label_contextual_header), TRUE);
	gtk_label_set_justify (GTK_LABEL (label_contextual_header),
			       GTK_JUSTIFY_LEFT);

	label_contextual_detail =
		gtk_label_new (_
			       ("<span size='small'>This style will be applied to all tags with a specific ancestor. <i>p  u { style }</i>, will be applied to all <i>u</i> tags within a <i>p</i> tag.</span>"));
	gtk_widget_show (label_contextual_detail);
	gtk_box_pack_start (GTK_BOX (vbox_contextual),
			    label_contextual_detail, FALSE, FALSE, 5);
	gtk_label_set_use_markup (GTK_LABEL (label_contextual_detail), TRUE);
	gtk_label_set_justify (GTK_LABEL (label_contextual_detail),
			       GTK_JUSTIFY_LEFT);
	gtk_label_set_line_wrap (GTK_LABEL (label_contextual_detail), TRUE);

	hbox_contextual_lists = gtk_hbox_new (FALSE, 0);
	gtk_widget_show (hbox_contextual_lists);
	gtk_box_pack_start (GTK_BOX (vbox_contextual), hbox_contextual_lists,
			    TRUE, TRUE, 0);

	hbox_contextual = gtk_hbox_new (FALSE, 0);
	gtk_widget_show (hbox_contextual);
	gtk_box_pack_start (GTK_BOX (hbox_contextual_lists), hbox_contextual,
			    TRUE, TRUE, 0);

	scrolledwindow_contextual_first =
		gtk_scrolled_window_new (NULL, NULL);
	gtk_widget_show (scrolledwindow_contextual_first);
	gtk_box_pack_start (GTK_BOX (hbox_contextual),
			    scrolledwindow_contextual_first, TRUE, TRUE, 2);

	treeview_contextual_first = gtk_tree_view_new ();
	gtk_widget_show (treeview_contextual_first);
	gtk_container_add (GTK_CONTAINER (scrolledwindow_contextual_first),
			   treeview_contextual_first);
//
	gtk_tree_view_insert_column (GTK_TREE_VIEW
				     (treeview_contextual_first),
				     contextual_first_column, 0);
	gtk_tree_view_set_model (GTK_TREE_VIEW (treeview_contextual_first),
				 GTK_TREE_MODEL (selector_store));
	gtk_tree_view_set_headers_visible (GTK_TREE_VIEW
					   (treeview_contextual_first), TRUE);
//
	scrolledwindow_contextual_secont =
		gtk_scrolled_window_new (NULL, NULL);
	gtk_widget_show (scrolledwindow_contextual_secont);
	gtk_box_pack_start (GTK_BOX (hbox_contextual),
			    scrolledwindow_contextual_secont, TRUE, TRUE, 2);

	treeview_contextual_second = gtk_tree_view_new ();
	gtk_widget_show (treeview_contextual_second);
	gtk_container_add (GTK_CONTAINER (scrolledwindow_contextual_secont),
			   treeview_contextual_second);
//
	gtk_tree_view_insert_column (GTK_TREE_VIEW
				     (treeview_contextual_second),
				     contextual_second_column, 0);
	gtk_tree_view_set_model (GTK_TREE_VIEW (treeview_contextual_second),
				 GTK_TREE_MODEL (selector_store));
	gtk_tree_view_set_headers_visible (GTK_TREE_VIEW
					   (treeview_contextual_second),
					   TRUE);
//
	vbuttonbox_contextual = gtk_vbutton_box_new ();
	gtk_widget_show (vbuttonbox_contextual);
	gtk_box_pack_start (GTK_BOX (hbox_contextual_lists),
			    vbuttonbox_contextual, FALSE, FALSE, 0);
	gtk_container_set_border_width (GTK_CONTAINER (vbuttonbox_contextual),
					5);
	gtk_button_box_set_layout (GTK_BUTTON_BOX (vbuttonbox_contextual),
				   GTK_BUTTONBOX_START);
	gtk_box_set_spacing (GTK_BOX (vbuttonbox_contextual), 2);

	button_contextual_add = gtk_button_new_from_stock ("gtk-add");
	gtk_widget_show (button_contextual_add);
	gtk_container_add (GTK_CONTAINER (vbuttonbox_contextual),
			   button_contextual_add);
	GTK_WIDGET_SET_FLAGS (button_contextual_add, GTK_CAN_DEFAULT);

	button_contextual_del = gtk_button_new_from_stock ("gtk-delete");
	gtk_widget_show (button_contextual_del);
	gtk_container_add (GTK_CONTAINER (vbuttonbox_contextual),
			   button_contextual_del);
	GTK_WIDGET_SET_FLAGS (button_contextual_del, GTK_CAN_DEFAULT);

	button_contextual_clean = gtk_button_new_from_stock ("gtk-clear");
	gtk_widget_show (button_contextual_clean);
	gtk_container_add (GTK_CONTAINER (vbuttonbox_contextual),
			   button_contextual_clean);
	GTK_WIDGET_SET_FLAGS (button_contextual_clean, GTK_CAN_DEFAULT);

	scrolledwindow_contextual_selected =
		gtk_scrolled_window_new (NULL, NULL);
	gtk_widget_show (scrolledwindow_contextual_selected);
	gtk_box_pack_start (GTK_BOX (hbox_contextual_lists),
			    scrolledwindow_contextual_selected, TRUE, TRUE,
			    2);

	treeview_contextual_selected = gtk_tree_view_new ();
	gtk_widget_show (treeview_contextual_selected);
	gtk_container_add (GTK_CONTAINER (scrolledwindow_contextual_selected),
			   treeview_contextual_selected);
//      
	gtk_tree_view_insert_column (GTK_TREE_VIEW
				     (treeview_contextual_selected),
				     contextual_selected_column, 0);

	gtk_tree_view_set_model (GTK_TREE_VIEW (treeview_contextual_selected),
				 GTK_TREE_MODEL (contextual_selected_store));
	gtk_tree_view_set_headers_visible (GTK_TREE_VIEW
					   (treeview_contextual_selected),
					   TRUE);
//
	label_tab_five = gtk_label_new ("5");
	gtk_widget_show (label_tab_five);
	gtk_notebook_set_tab_label (GTK_NOTEBOOK (notebook_wizard),
				    gtk_notebook_get_nth_page (GTK_NOTEBOOK
							       (notebook_wizard),
							       4),
				    label_tab_five);
	gtk_label_set_justify (GTK_LABEL (label_tab_five), GTK_JUSTIFY_LEFT);

	dialog_action_area = GTK_DIALOG (selector_wizard)->action_area;
	gtk_widget_show (dialog_action_area);
	gtk_button_box_set_layout (GTK_BUTTON_BOX (dialog_action_area),
				   GTK_BUTTONBOX_END);

	cancelbutton = gtk_button_new_from_stock ("gtk-cancel");
	gtk_widget_show (cancelbutton);
	gtk_dialog_add_action_widget (GTK_DIALOG (selector_wizard),
				      cancelbutton, GTK_RESPONSE_CANCEL);
	GTK_WIDGET_SET_FLAGS (cancelbutton, GTK_CAN_DEFAULT);

	okbutton = gtk_button_new_from_stock ("gtk-ok");
	gtk_widget_show (okbutton);
	gtk_dialog_add_action_widget (GTK_DIALOG (selector_wizard), okbutton,
				      GTK_RESPONSE_OK);
	GTK_WIDGET_SET_FLAGS (okbutton, GTK_CAN_DEFAULT);

	// side buttons will switch pages
	g_signal_connect ((gpointer) button_selector, "clicked",
			  G_CALLBACK (on_button_switch_page_clicked),
			  GINT_TO_POINTER (0));
	g_signal_connect ((gpointer) button_child, "clicked",
			  G_CALLBACK (on_button_switch_page_clicked),
			  GINT_TO_POINTER (1));
	g_signal_connect ((gpointer) button_class, "clicked",
			  G_CALLBACK (on_button_switch_page_clicked),
			  GINT_TO_POINTER (2));
	g_signal_connect ((gpointer) button_sibling, "clicked",
			  G_CALLBACK (on_button_switch_page_clicked),
			  GINT_TO_POINTER (3));
	g_signal_connect ((gpointer) button_contextual, "clicked",
			  G_CALLBACK (on_button_switch_page_clicked),
			  GINT_TO_POINTER (4));
	//
	g_signal_connect ((gpointer) button_selector_add, "clicked",
			  G_CALLBACK (on_button_selector_add_clicked), NULL);
	g_signal_connect ((gpointer) button_selector_del, "clicked",
			  G_CALLBACK (on_button_selector_del_clicked), NULL);
	g_signal_connect ((gpointer) button_selector_clean, "clicked",
			  G_CALLBACK (on_button_selector_clean_clicked),
			  NULL);
	g_signal_connect ((gpointer) button_child_add, "clicked",
			  G_CALLBACK (on_button_child_add_clicked), NULL);
	g_signal_connect ((gpointer) button_child_del, "clicked",
			  G_CALLBACK (on_button_child_del_clicked), NULL);
	g_signal_connect ((gpointer) button_child_clear, "clicked",
			  G_CALLBACK (on_button_child_clear_clicked), NULL);
	g_signal_connect ((gpointer) entry_class, "changed",
			  G_CALLBACK (on_entry_class_changed), NULL);
	g_signal_connect ((gpointer) button_class_add, "clicked",
			  G_CALLBACK (on_button_class_add_clicked), NULL);
	g_signal_connect ((gpointer) button_class_del, "clicked",
			  G_CALLBACK (on_button_class_del_clicked), NULL);
	g_signal_connect ((gpointer) button_class_clean, "clicked",
			  G_CALLBACK (on_button_class_clean_clicked), NULL);
	g_signal_connect ((gpointer) button_sibling_add, "clicked",
			  G_CALLBACK (on_button_sibling_add_clicked), NULL);
	g_signal_connect ((gpointer) button_sibling_del, "clicked",
			  G_CALLBACK (on_button_sibling_del_clicked), NULL);
	g_signal_connect ((gpointer) button_sibling_clean, "clicked",
			  G_CALLBACK (on_button_sibling_clean_clicked), NULL);
	g_signal_connect ((gpointer) button_contextual_add, "clicked",
			  G_CALLBACK (on_button_contextual_add_clicked),
			  NULL);
	g_signal_connect ((gpointer) button_contextual_del, "clicked",
			  G_CALLBACK (on_button_contextual_del_clicked),
			  NULL);
	g_signal_connect ((gpointer) button_contextual_clean, "clicked",
			  G_CALLBACK (on_button_contextual_clean_clicked),
			  NULL);

	/* Store pointers to some widgets, for use by lookup_widget(). */
	GLADE_HOOKUP_OBJECT_NO_REF (selector_wizard, selector_wizard,
				    "selector_wizard");
	GLADE_HOOKUP_OBJECT (selector_wizard, button_selector,
			     "button_selector");
	GLADE_HOOKUP_OBJECT (selector_wizard, button_child, "button_child");
	GLADE_HOOKUP_OBJECT (selector_wizard, button_class, "button_class");
	GLADE_HOOKUP_OBJECT (selector_wizard, button_sibling,
			     "button_sibling");
	GLADE_HOOKUP_OBJECT (selector_wizard, radiobutton_for_each,
			     "radiobutton_for_each");
	GLADE_HOOKUP_OBJECT (selector_wizard, radiobutton_for_all, 
			     "radiobutton_for_all");
	GLADE_HOOKUP_OBJECT (selector_wizard, button_contextual,
			     "button_contextual");
	GLADE_HOOKUP_OBJECT (selector_wizard, notebook_wizard,
			     "notebook_wizard");
	GLADE_HOOKUP_OBJECT (selector_wizard, treeview_selector,
			     "treeview_selector");
	GLADE_HOOKUP_OBJECT (selector_wizard, button_selector_add,
			     "button_selector_add");
	GLADE_HOOKUP_OBJECT (selector_wizard, button_selector_del,
			     "button_selector_del");
	GLADE_HOOKUP_OBJECT (selector_wizard, button_selector_clean,
			     "button_selector_clean");
	GLADE_HOOKUP_OBJECT (selector_wizard, treeview_selector_selected,
			     "treeview_selector_selected");
	GLADE_HOOKUP_OBJECT (selector_wizard, treeview_child_parent,
			     "treeview_child_parent");
	GLADE_HOOKUP_OBJECT (selector_wizard, treeview_child_child,
			     "treeview_child_child");
	GLADE_HOOKUP_OBJECT (selector_wizard, button_child_add,
			     "button_child_add");
	GLADE_HOOKUP_OBJECT (selector_wizard, button_child_del,
			     "button_child_del");
	GLADE_HOOKUP_OBJECT (selector_wizard, button_child_clear,
			     "button_child_clear");
	GLADE_HOOKUP_OBJECT (selector_wizard, treeview_child_selected,
			     "treeview_child_selected");
	GLADE_HOOKUP_OBJECT (selector_wizard, treeview_class,
			     "treeview_class");
	GLADE_HOOKUP_OBJECT (selector_wizard, entry_class, "entry_class");
	GLADE_HOOKUP_OBJECT (selector_wizard, button_class_add,
			     "button_class_add");
	GLADE_HOOKUP_OBJECT (selector_wizard, button_class_del,
			     "button_class_del");
	GLADE_HOOKUP_OBJECT (selector_wizard, button_class_clean,
			     "button_class_clean");
	GLADE_HOOKUP_OBJECT (selector_wizard, treeview_class_selected,
			     "treeview_class_selected");
	GLADE_HOOKUP_OBJECT (selector_wizard, treeview_sibling_fist,
			     "treeview_sibling_fist");
	GLADE_HOOKUP_OBJECT (selector_wizard, treeview_sibling_second,
			     "treeview_sibling_second");
	GLADE_HOOKUP_OBJECT (selector_wizard, button_sibling_add,
			     "button_sibling_add");
	GLADE_HOOKUP_OBJECT (selector_wizard, button_sibling_del,
			     "button_sibling_del");
	GLADE_HOOKUP_OBJECT (selector_wizard, button_sibling_clean,
			     "button_sibling_clean");
	GLADE_HOOKUP_OBJECT (selector_wizard, treeview_sibling_selected,
			     "treeview_sibling_selected");
	GLADE_HOOKUP_OBJECT (selector_wizard, treeview_contextual_first,
			     "treeview_contextual_first");
	GLADE_HOOKUP_OBJECT (selector_wizard, treeview_contextual_second,
			     "treeview_contextual_second");
	GLADE_HOOKUP_OBJECT (selector_wizard, button_contextual_add,
			     "button_contextual_add");
	GLADE_HOOKUP_OBJECT (selector_wizard, button_contextual_del,
			     "button_contextual_del");
	GLADE_HOOKUP_OBJECT (selector_wizard, button_contextual_clean,
			     "button_contextual_clean");
	GLADE_HOOKUP_OBJECT (selector_wizard, treeview_contextual_selected,
			     "treeview_contextual_selected");
	GLADE_HOOKUP_OBJECT (selector_wizard, okbutton, "okbutton");
	GLADE_HOOKUP_OBJECT (selector_wizard, cancelbutton, "cancelbutton");

	return selector_wizard;
}

/* callbacks */


void
on_button_switch_page_clicked (GtkButton * button, gpointer user_data)
{
	GtkWidget *notebook;
	gint page;

	notebook = lookup_widget (GTK_WIDGET (button), "notebook_wizard");
	page = GPOINTER_TO_INT (user_data);
	gtk_notebook_set_current_page (GTK_NOTEBOOK (notebook), page);
}

/* first page, single selector */
void
on_button_selector_add_clicked (GtkButton * button, gpointer user_data)
{
	tree_view_add_string_from_list_by_name (GTK_WIDGET(button),
											"treeview_selector",
											"treeview_selector_selected");
}


void
on_button_selector_del_clicked (GtkButton * button, gpointer user_data)
{
	tree_view_delete_selected_by_name_from_lookup(GTK_WIDGET(button),
												"treeview_selector_selected");
}


void
on_button_selector_clean_clicked (GtkButton * button, gpointer user_data)
{
	tree_view_store_clean_by_name_from_lookup (GTK_WIDGET(button), 
										"treeview_selector_selected");
}

/* second page, child selector */
void
on_button_child_add_clicked (GtkButton * button, gpointer user_data)
{
	tree_view_add_strings_from_two_lists_by_name( 	GTK_WIDGET(button),
													"treeview_child_parent",
													"treeview_child_child",
													"treeview_child_selected",
													">" );
}


void
on_button_child_del_clicked (GtkButton * button, gpointer user_data)
{
	tree_view_delete_selected_by_name_from_lookup(GTK_WIDGET(button),
												"treeview_child_selected");
}


void
on_button_child_clear_clicked (GtkButton * button, gpointer user_data)
{
	tree_view_store_clean_by_name_from_lookup (GTK_WIDGET(button), 
										"treeview_child_selected");
}

/*  third page classes */
void
on_entry_class_changed (GtkEditable * editable, gpointer user_data)
{

}


void
on_button_class_add_clicked (GtkButton * button, gpointer user_data)
{
	tree_view_add_string_from_list_and_entry_by_name 
										(GTK_WIDGET(button),
										 "treeview_class",
										 "treeview_class_selected",
										 "entry_class",
										 ".");
}


void
on_button_class_del_clicked (GtkButton * button, gpointer user_data)
{
	tree_view_delete_selected_by_name_from_lookup(GTK_WIDGET(button),
												"treeview_class_selected");
}


void
on_button_class_clean_clicked (GtkButton * button, gpointer user_data)
{
	tree_view_store_clean_by_name_from_lookup (GTK_WIDGET(button), 
										"treeview_class_selected");
}

/* sibling */
void
on_button_sibling_add_clicked (GtkButton * button, gpointer user_data)
{
	tree_view_add_strings_from_two_lists_by_name( 	GTK_WIDGET(button),
													"treeview_sibling_fist",
													"treeview_sibling_second",
													"treeview_sibling_selected",
													"+" );
	  
}


void
on_button_sibling_del_clicked (GtkButton * button, gpointer user_data)
{
	tree_view_delete_selected_by_name_from_lookup(GTK_WIDGET(button),
												"treeview_sibling_selected");
}


void
on_button_sibling_clean_clicked (GtkButton * button, gpointer user_data)
{
	tree_view_store_clean_by_name_from_lookup (GTK_WIDGET(button), 
										"treeview_sibling_selected");
}

/* contextual */
void
on_button_contextual_add_clicked (GtkButton * button, gpointer user_data)
{
	tree_view_add_strings_from_two_lists_by_name( 	GTK_WIDGET(button),
													"treeview_contextual_first",
													"treeview_contextual_second",
													"treeview_contextual_selected",
													NULL );
}


void
on_button_contextual_del_clicked (GtkButton * button, gpointer user_data)
{
	tree_view_delete_selected_by_name_from_lookup(GTK_WIDGET(button),
												"treeview_contextual_selected");
}


void
on_button_contextual_clean_clicked (GtkButton * button, gpointer user_data)
{
	tree_view_store_clean_by_name_from_lookup (GTK_WIDGET(button), 
										"treeview_contextual_selected");
}

/* generic, used by all calls */

void
tree_view_store_clean_by_name_from_lookup (GtkWidget* same_top_level_widget, 
												gchar* name)
{
	GtkWidget* tree_out;
	GtkTreeModel* model_out;
	
	tree_out = lookup_widget(same_top_level_widget,name);
	model_out = gtk_tree_view_get_model (GTK_TREE_VIEW(tree_out));
	gtk_list_store_clear (GTK_LIST_STORE(model_out));	
}

void
tree_view_delete_selected_by_name_from_lookup (GtkWidget* same_top_level_widget, 
												gchar* name)
{
	GtkWidget* tree_out;
	GtkTreeModel* model_out;
	GtkTreeViewColumn* col;
	GtkTreePath *path;
	GtkTreeIter iter;
	
	tree_out = lookup_widget(same_top_level_widget,name);
	gtk_tree_view_get_cursor( GTK_TREE_VIEW(tree_out), &path, &col );
	if( path != NULL ){
		model_out = gtk_tree_view_get_model (GTK_TREE_VIEW(tree_out));
		gtk_tree_model_get_iter(model_out, &iter, path);
		gtk_list_store_remove (GTK_LIST_STORE(model_out),&iter);
	}	
}

void
tree_view_add_string_from_list_by_name (GtkWidget* same_top_level_widget,
										gchar* input_list_name,
										gchar* output_list_name)
{
	GtkWidget* tree_in;
	GtkWidget* tree_out;
	GtkTreeModel* model_in;
	GtkTreeModel* model_out;
	GtkTreeViewColumn* col;
	GtkTreePath *path;
	GtkTreeIter iter;
	gchar *strtoadd;
	
	tree_in  = lookup_widget(same_top_level_widget,input_list_name);
	tree_out = lookup_widget(same_top_level_widget, output_list_name);
	
	gtk_tree_view_get_cursor( GTK_TREE_VIEW(tree_in), &path, &col );
	
	if( path != NULL ){
		model_in = gtk_tree_view_get_model (GTK_TREE_VIEW(tree_in));
		model_out = gtk_tree_view_get_model (GTK_TREE_VIEW(tree_out));// model must be set
		gtk_tree_model_get_iter(model_in, &iter, path);
		gtk_tree_model_get (model_in, &iter, 0, &strtoadd, -1);
		gtk_list_store_append(GTK_LIST_STORE(model_out),&iter);
		gtk_list_store_set (GTK_LIST_STORE(model_out), &iter, 0,
				    strtoadd, -1);
	}		
}

void
tree_view_add_string_from_list_and_entry_by_name (GtkWidget* same_top_level_widget,
										gchar* input_list_name,
										gchar* output_list_name,
										gchar* entry_name,
										gchar* str_to_join)
{
	GtkWidget* tree_in;
	GtkWidget* tree_out;
	GtkWidget* entry;
	GtkTreeModel* model_in;
	GtkTreeModel* model_out;
	GtkTreeViewColumn* col;
	GtkTreePath *path;
	GtkTreeIter iter;
	gchar *strtoadd;
	gchar* strclass;
	gchar* strcat;
	
	tree_in  = lookup_widget(same_top_level_widget,input_list_name);
	tree_out = lookup_widget(same_top_level_widget, output_list_name);
	entry = lookup_widget(same_top_level_widget, entry_name);
	
	gtk_tree_view_get_cursor( GTK_TREE_VIEW(tree_in), &path, &col );
	strclass = gtk_editable_get_chars(GTK_EDITABLE(entry),0,-1);
	if( path != NULL && strlen(strclass) > 0){
		model_in = gtk_tree_view_get_model (GTK_TREE_VIEW(tree_in));
		model_out = gtk_tree_view_get_model (GTK_TREE_VIEW(tree_out));// model must be set
		gtk_tree_model_get_iter(model_in, &iter, path);
		gtk_tree_model_get (model_in, &iter, 0, &strtoadd, -1);
		gtk_list_store_append(GTK_LIST_STORE(model_out),&iter);
		if( str_to_join != NULL )
			strcat = g_strconcat(strtoadd, str_to_join, strclass, NULL);
		else
			strcat = g_strconcat(strtoadd, strclass, NULL);
		gtk_list_store_set (GTK_LIST_STORE(model_out), &iter, 0,
				    strcat, -1);
		g_free(strcat);
	}	
	g_free(strclass);	
}

void
tree_view_add_strings_from_two_lists_by_name(GtkWidget* same_top_level_widget,
												gchar* first_list_name,
												gchar* second_list_name,
												gchar* output_list_name,
												gchar* str_to_join)
{
	GtkWidget* tree_in;
	GtkWidget* tree_in_second;
	GtkWidget* tree_out;	
	GtkTreeModel* model_in;
	GtkTreeModel* model_in_second;
	GtkTreeModel* model_out;
	GtkTreeViewColumn* col;
	GtkTreeViewColumn* col_second;
	GtkTreePath *path;
	GtkTreePath *path_second;	
	GtkTreeIter iter;
	gchar *strtoadd;
	gchar *strtoadd_second;
	gchar *strcat;
	
	tree_in  = lookup_widget(same_top_level_widget, first_list_name);
	tree_in_second = lookup_widget(same_top_level_widget, second_list_name);
	tree_out = lookup_widget(same_top_level_widget, output_list_name);
	
	gtk_tree_view_get_cursor( GTK_TREE_VIEW(tree_in), &path, &col );
	gtk_tree_view_get_cursor( GTK_TREE_VIEW(tree_in_second), &path_second, 
									&col_second );
	
	if( path != NULL && path_second != NULL ){
		model_in = gtk_tree_view_get_model (GTK_TREE_VIEW(tree_in));
		model_in_second = gtk_tree_view_get_model (GTK_TREE_VIEW(tree_in_second));
		model_out = gtk_tree_view_get_model (GTK_TREE_VIEW(tree_out));// model must be set
		gtk_tree_model_get_iter(model_in, &iter, path);
		gtk_tree_model_get (model_in, &iter, 0, &strtoadd, -1);
		gtk_tree_model_get_iter(model_in_second, &iter, path_second);
		gtk_tree_model_get (model_in_second, &iter, 0, &strtoadd_second, -1);
		if( str_to_join != NULL )
			strcat = g_strjoin(" ",strtoadd, str_to_join, strtoadd_second, NULL);
		else
			strcat = g_strjoin(" ",strtoadd, strtoadd_second, NULL);
		gtk_list_store_append(GTK_LIST_STORE(model_out),&iter);
		gtk_list_store_set (GTK_LIST_STORE(model_out), &iter, 0,
				    strcat, -1);
		g_free(strcat);
	}
}

