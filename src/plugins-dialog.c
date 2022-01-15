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
#include <gmodule.h>

#include "cssedwindow.h"
#include "plugin.h"
#include "support.h"
#include "utils.h"

#define GLADE_HOOKUP_OBJECT(component,widget,name) \
  g_object_set_data_full (G_OBJECT (component), name, \
    gtk_widget_ref (widget), (GDestroyNotify) gtk_widget_unref)

#define GLADE_HOOKUP_OBJECT_NO_REF(component,widget,name) \
  g_object_set_data (G_OBJECT (component), name, widget)

// util func
void
scan_and_output_dir (CssedWindow * window, gchar * dirname,
		     			GtkListStore * list_store)
{
	GDir *dir;
	GError *error = NULL;
	GPatternSpec *pattern;
	G_CONST_RETURN gchar *filename;
	GtkTreeIter iter;
	CssedPlugin *plugin;

#ifdef WIN32
	pattern = g_pattern_spec_new ("*.dll");
#else
	pattern = g_pattern_spec_new ("*.so");
#endif

	dir = g_dir_open (dirname, 0, &error);

	if (dir != NULL)
	{
		gtk_list_store_clear (list_store);
		filename = g_dir_read_name (dir);
		while (filename)
		{
			if (g_pattern_match (pattern, strlen (filename), filename, NULL))
			{
				gchar *fullname = g_build_filename (dirname, filename, NULL);
				if ((plugin =cssed_get_plugin_if_file_loaded (window, fullname)
					 ) == NULL )
				{
					gtk_list_store_append (list_store,
							       &iter);
					gtk_list_store_set (list_store, &iter,
							    0, fullname,
								1, "",
								2, "", 
								3, FALSE, 
								-1);
				}
				else
				{
					gtk_list_store_append (list_store,   &iter);
					gtk_list_store_set (list_store, &iter,
							    0, fullname, 1,
							    plugin->name, 2,
							    plugin->
							    description, 3,
							    TRUE, -1);
				}
				g_free (fullname);
			}
			filename = g_dir_read_name (dir);
		}
		g_dir_close (dir);
		g_pattern_spec_free (pattern);
	}else{
		g_error_free(error);
	}
}

// callbacks
void
on_dialog_close (GtkDialog * dialog, gpointer user_data)
{
	gtk_widget_destroy (GTK_WIDGET (dialog));
}


void
on_button_refresh_clicked (GtkButton * button, gpointer user_data)
{
	CssedWindow *window;
	GtkTreeModel *model;
	GtkWidget *view;
	gchar *plugins_dir;

	window = CSSED_WINDOW (user_data);
	view = lookup_widget (GTK_WIDGET (button), "treeview_plugins");
	model = gtk_tree_view_get_model (GTK_TREE_VIEW (view));

	plugins_dir = g_build_filename(PACKAGE_LIB_DIR,"plugins", NULL);
	scan_and_output_dir (window, plugins_dir, GTK_LIST_STORE (model));
	g_free(plugins_dir);
}


void
on_button_load_clicked (GtkButton * button, gpointer user_data)
{
	CssedWindow *window;
	CssedPlugin *plugin;
	GtkTreeSelection *select;
	GtkTreeModel *model;
	GtkTreeView *view;
	GtkTreeIter iter;
	gchar *path;


	window = CSSED_WINDOW (user_data);
	view = (GtkTreeView *) lookup_widget (GTK_WIDGET (button),
					      "treeview_plugins");
	model = gtk_tree_view_get_model (view);
	select = gtk_tree_view_get_selection (view);

	if (gtk_tree_selection_get_selected (select, NULL, &iter))
	{
		gtk_tree_model_get (model, &iter, 0, &path, -1);
		if (cssed_is_plugin_file_loaded (window, path))
		{
			//errmsg = g_strdup_printf (_("Plugin is loaded\n%s\n\nPlease click Refresh if the dialog shows inaccurate values"),
			//			  				path);
			cssed_error_message (_("Plugin loaded"), _("Plugin is loaded\n%s\n\nPlease click Refresh if the dialog shows inaccurate values"), path);
			//g_free (errmsg);
		}
		else
		{
			plugin = cssed_init_plugin (window, path);
			if (plugin)
			{
				cssed_load_plugin (plugin);
				gtk_list_store_set (GTK_LIST_STORE (model),
						    &iter, 0, path, 1,
						    plugin->name, 2,
						    plugin->description, 3,
						    TRUE, -1);
			}
		}
		g_free (path);
	}
}


void
on_button_unload_clicked (GtkButton * button, gpointer user_data)
{
	CssedWindow *window;
	CssedPlugin *plugin;
	GtkTreeSelection *select;
	GtkTreeModel *model;
	GtkTreeView *view;
	GtkTreeIter iter;
	gchar *path;


	window = CSSED_WINDOW (user_data);
	view = (GtkTreeView*) lookup_widget (GTK_WIDGET (button),"treeview_plugins");
	model = gtk_tree_view_get_model (view);
	select = gtk_tree_view_get_selection (view);

	if (gtk_tree_selection_get_selected (select, NULL, &iter))
	{
		gtk_tree_model_get (model, &iter, 0, &path, -1);
		if ((plugin =
		     cssed_get_plugin_if_file_loaded (window, path)) != NULL)
		{
			cssed_unload_plugin (plugin);
			gtk_list_store_set (GTK_LIST_STORE (model), &iter,
					    0, path,
					    1, "",
						2, "",
						3, FALSE,
						-1);
		}
		else
		{
			//errmsg = g_strdup_printf (_("Plugin is not loaded\n%s\n\nPlease click Refresh if the dialog shows false values"), path);
			cssed_error_message (_("Not loaded"), _("Plugin is not loaded\n%s\n\nPlease click Refresh if the dialog shows false values"));
			//g_free (errmsg);
		}
		g_free (path);
	}
}


void
on_button_close_clicked (GtkButton * button, gpointer user_data)
{
	gtk_widget_destroy (GTK_WIDGET (user_data));
}

// create UI
GtkWidget *
create_plugins_dialog (CssedWindow * window)
{
	GtkWidget *dialog;
	GtkWidget *dialog_vbox;
	GtkWidget *scrolledwindow;
	GtkWidget *treeview_plugins;
	GtkWidget *dialog_action_area;
	GtkWidget *button_refresh;
	GtkWidget *button_load;
	GtkWidget *alignment2;
	GtkWidget *hbox2;
	GtkWidget *image2;
	GtkWidget *label2;
	GtkWidget *button_unload;
	GtkWidget *alignment3;
	GtkWidget *hbox3;
	GtkWidget *image3;
	GtkWidget *label3;
	GtkWidget *button_close;
	// list store and stuff
	GtkListStore *store;
	GtkTreeViewColumn *col_file;
	GtkTreeViewColumn *col_name;
	GtkTreeViewColumn *col_description;
	GtkTreeViewColumn *col_loaded;
	GtkCellRenderer *text_renderer;
	GtkCellRenderer *bool_renderer;
	// window widget
	GtkWidget *main_window;
	gchar *plugins_dir;

	dialog = gtk_dialog_new ();
	gtk_window_set_title (GTK_WINDOW (dialog), _("Plugins dialog"));
	main_window = cssed_window_get_window_widget( window );
	gtk_window_set_transient_for (GTK_WINDOW (dialog), GTK_WINDOW (main_window));
	gtk_window_set_default_size (GTK_WINDOW (dialog), 650, 400);

	dialog_vbox = GTK_DIALOG (dialog)->vbox;
	gtk_widget_show (dialog_vbox);

	scrolledwindow = gtk_scrolled_window_new (NULL, NULL);
	gtk_widget_show (scrolledwindow);
	gtk_box_pack_start (GTK_BOX (dialog_vbox), scrolledwindow, TRUE, TRUE,
			    0);

	treeview_plugins = gtk_tree_view_new ();
	gtk_widget_show (treeview_plugins);
	gtk_container_add (GTK_CONTAINER (scrolledwindow), treeview_plugins);


	store = gtk_list_store_new (4, 	G_TYPE_STRING,
				    				G_TYPE_STRING,
				    				G_TYPE_STRING,
									G_TYPE_BOOLEAN);

	gtk_tree_view_set_model (GTK_TREE_VIEW (treeview_plugins),
				 					GTK_TREE_MODEL (store));
	g_object_unref(store);
	
	text_renderer = gtk_cell_renderer_text_new ();
	bool_renderer = gtk_cell_renderer_toggle_new ();

	col_file =
		gtk_tree_view_column_new_with_attributes (_("File"),
							  text_renderer,
							  "markup", 0, NULL);
	col_name =
		gtk_tree_view_column_new_with_attributes (_("Name"),
							  text_renderer,
							  "markup", 1, NULL);
	col_description =
		gtk_tree_view_column_new_with_attributes (_("Description"),
							  text_renderer,
							  "markup", 2, NULL);
	col_loaded =
		gtk_tree_view_column_new_with_attributes (_("Loaded"),
							  bool_renderer,
							  "active", 3, NULL);

	gtk_tree_view_set_headers_visible (GTK_TREE_VIEW (treeview_plugins),
					   TRUE);

	gtk_tree_view_insert_column (GTK_TREE_VIEW (treeview_plugins),
				     			col_file, 0);
	gtk_tree_view_insert_column (GTK_TREE_VIEW (treeview_plugins),
				     			col_name, 1);
	gtk_tree_view_insert_column (GTK_TREE_VIEW (treeview_plugins),
				     			col_description, 2);
	gtk_tree_view_insert_column (GTK_TREE_VIEW (treeview_plugins),
				     			col_loaded, 3);


	dialog_action_area = GTK_DIALOG (dialog)->action_area;
	gtk_widget_show (dialog_action_area);
	gtk_button_box_set_layout (GTK_BUTTON_BOX (dialog_action_area),
				   				GTK_BUTTONBOX_END);

	button_refresh = gtk_button_new_from_stock ("gtk-refresh");
	gtk_widget_show (button_refresh);
	gtk_dialog_add_action_widget (GTK_DIALOG (dialog), button_refresh, 0);
	GTK_WIDGET_SET_FLAGS (button_refresh, GTK_CAN_DEFAULT);

	button_load = gtk_button_new ();
	gtk_widget_show (button_load);
	gtk_dialog_add_action_widget (GTK_DIALOG (dialog), button_load, 0);
	GTK_WIDGET_SET_FLAGS (button_load, GTK_CAN_DEFAULT);

	alignment2 = gtk_alignment_new (0.5, 0.5, 0, 0);
	gtk_widget_show (alignment2);
	gtk_container_add (GTK_CONTAINER (button_load), alignment2);

	hbox2 = gtk_hbox_new (FALSE, 2);
	gtk_widget_show (hbox2);
	gtk_container_add (GTK_CONTAINER (alignment2), hbox2);

	image2 = gtk_image_new_from_stock ("gtk-yes", GTK_ICON_SIZE_BUTTON);
	gtk_widget_show (image2);
	gtk_box_pack_start (GTK_BOX (hbox2), image2, FALSE, FALSE, 0);

	label2 = gtk_label_new_with_mnemonic (_("Load"));
	gtk_widget_show (label2);
	gtk_box_pack_start (GTK_BOX (hbox2), label2, FALSE, FALSE, 0);
	gtk_label_set_justify (GTK_LABEL (label2), GTK_JUSTIFY_LEFT);

	button_unload = gtk_button_new ();
	gtk_widget_show (button_unload);
	gtk_dialog_add_action_widget (GTK_DIALOG (dialog), button_unload, 0);
	GTK_WIDGET_SET_FLAGS (button_unload, GTK_CAN_DEFAULT);

	alignment3 = gtk_alignment_new (0.5, 0.5, 0, 0);
	gtk_widget_show (alignment3);
	gtk_container_add (GTK_CONTAINER (button_unload), alignment3);

	hbox3 = gtk_hbox_new (FALSE, 2);
	gtk_widget_show (hbox3);
	gtk_container_add (GTK_CONTAINER (alignment3), hbox3);

	image3 = gtk_image_new_from_stock ("gtk-no", GTK_ICON_SIZE_BUTTON);
	gtk_widget_show (image3);
	gtk_box_pack_start (GTK_BOX (hbox3), image3, FALSE, FALSE, 0);

	label3 = gtk_label_new_with_mnemonic (_("Unload"));
	gtk_widget_show (label3);
	gtk_box_pack_start (GTK_BOX (hbox3), label3, FALSE, FALSE, 0);
	gtk_label_set_justify (GTK_LABEL (label3), GTK_JUSTIFY_LEFT);

	button_close = gtk_button_new_from_stock ("gtk-close");
	gtk_widget_show (button_close);
	gtk_dialog_add_action_widget (GTK_DIALOG (dialog), button_close,
				      GTK_RESPONSE_CLOSE);
	GTK_WIDGET_SET_FLAGS (button_close, GTK_CAN_DEFAULT);

	g_signal_connect ((gpointer) dialog, "close",
			  G_CALLBACK (on_dialog_close), NULL);
	g_signal_connect ((gpointer) button_refresh, "clicked",
			  G_CALLBACK (on_button_refresh_clicked), window);
	g_signal_connect ((gpointer) button_load, "clicked",
			  G_CALLBACK (on_button_load_clicked), window);
	g_signal_connect ((gpointer) button_unload, "clicked",
			  G_CALLBACK (on_button_unload_clicked), window);
	g_signal_connect ((gpointer) button_close, "clicked",
			  G_CALLBACK (on_button_close_clicked), dialog);

	/* Store pointers to all widgets, for use by lookup_widget(). */
	GLADE_HOOKUP_OBJECT_NO_REF (dialog, dialog, "dialog");
	GLADE_HOOKUP_OBJECT (dialog, treeview_plugins, "treeview_plugins");
	GLADE_HOOKUP_OBJECT (dialog, button_refresh, "button_refresh");
	GLADE_HOOKUP_OBJECT (dialog, button_load, "button_load");
	GLADE_HOOKUP_OBJECT (dialog, button_unload, "button_unload");
	GLADE_HOOKUP_OBJECT (dialog, button_close, "button_close");
	
	// fill the tree
	plugins_dir = g_build_filename(PACKAGE_LIB_DIR,"plugins", NULL);
	scan_and_output_dir (window, plugins_dir, GTK_LIST_STORE (store));
	g_free(plugins_dir);
	
	return dialog;
}

