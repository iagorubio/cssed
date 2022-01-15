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
#include "document.h"
#include "callbacks.h"
#include "cssdialogs-interface.h"
#include "cssdialogs-callbacks.h"
#include "cssdialogs-functions.h"
#include "floating-list.h"
#include "support.h"
#include "debug.h"
#include "cssdialogs-class.h"

#define GLADE_HOOKUP_OBJECT(component,widget,name) \
  g_object_set_data_full (G_OBJECT (component), name, \
    gtk_widget_ref (widget), (GDestroyNotify) gtk_widget_unref)

#define GLADE_HOOKUP_OBJECT_NO_REF(component,widget,name) \
  g_object_set_data (G_OBJECT (component), name, widget)

typedef struct _cssed_floating_list_data
{
	GtkWidget* list_widget;		// the list window
	gchar* keyword;				// the keyword to scan the cssed def tree 
	CssedWindow* window;		// main window object
	GtkTreeModel* output_model; // where to refreash changes
	gchar* output_path; 	// path to the row to be changed
	GtkWidget* treeview;		// the tree view
} CssedFloatingListData;

#define LIST_DATA(x) (CssedFloatingListData*) x


void
on_tree_activate (GtkTreeView * t, 
								GtkTreePath * p, 
								GtkTreeViewColumn * c,
		  						gpointer user_data);
/*
void
curchanged (GtkTreeView * treeview, gpointer user_data);
*/
void
on_list_tree_activated (GtkTreeView * t, GtkTreePath * p,
								GtkTreeViewColumn * c, 
								gpointer user_data);
gboolean
on_list_press_event (GtkWidget * widget, 
								GdkEventButton * event,
			      				gpointer user_data);
gboolean
on_focus_out (GtkWidget * widget, 
								GdkEventFocus * event,
		       					gpointer user_data);
								
//
void
run_dialog_disconnected_from_string(CssedFloatingListData* data);

gboolean
list_set_focus_on_idle(gpointer data)
{
	CssedFloatingListData* list_data;
	
	list_data = LIST_DATA(data);
	gtk_window_set_focus(GTK_WINDOW(list_data->list_widget), list_data->treeview);
	return FALSE;
}
 
GtkWidget *
create_floating_list_from_keyword ( CssedDoc * doc, 
									gchar * key,
				   					gint x,
									gint y, 
									gint width,
									gint height,
									GtkTreeModel* output_model,
									gchar* output_path)
{
	CssedWindow *window;
	CssedFloatingListData* data;
	GtkWidget *treeview;
	GtkTreeModel *model;
	GtkTreeIter iter;
	gboolean valid;
	gchar *value;
	gchar *path_str;
	gint nrow;
	gchar *keyword;
	GtkWidget *id;
	GtkWidget *scroller;
	GtkWidget *list;
	GtkTreeIter listiter;
	GtkWidget *frame;
	GtkListStore *store;
	GtkTreeSelection *selection;
	GtkTreeViewColumn *column;
	GtkCellRenderer *renderer;
	GtkWidget *parent;
	//gint i;

	//parent = lookup_widget (GTK_WIDGET (doc->notebook), "window");
	window =document_get_window( doc );
	parent = cssed_window_get_window_widget( window );
	treeview = cssed_window_get_css_definition_treeview ( window );

	// prepare store and list
	store = gtk_list_store_new (2, GDK_TYPE_PIXBUF, G_TYPE_STRING);
	list = gtk_tree_view_new_with_model (GTK_TREE_MODEL (store));
	
	// check for matchs in the treeview
	// most of this is taken from document.c 
	// document_pop_submenu_from_keyword_in_treeview()
	model = gtk_tree_view_get_model (GTK_TREE_VIEW (treeview));

	if (model == NULL)
	{
		return NULL;		// FIXME say something to user
	}
	valid = gtk_tree_model_get_iter_from_string (model, &iter, "0:0");
	if (!valid)
	{
		return NULL;		// FIXME say something to user
	}

	nrow = 0;
	while (valid)
	{
		gtk_tree_model_get (model, &iter, 1, &keyword, -1);
		if (strcmp (keyword, key) == 0)
		{

			path_str = g_strdup_printf ("0:%d:0", nrow);
			valid = gtk_tree_model_get_iter_from_string (model,
								     &iter,
								     path_str);
			g_free (path_str);
			if (!valid)
			{
				g_free (keyword);
				return NULL;	// FIXME say something to user
			}
			while (valid)
			{
				gtk_tree_model_get (model, &iter, 2, &value, -1);
				gtk_list_store_append (store, &listiter);
				gtk_list_store_set (store, &listiter, 1, value, -1);
				g_free (value);
				valid = gtk_tree_model_iter_next (model, &iter);
			}
		}
		nrow++;
		g_free (keyword);
		valid = gtk_tree_model_iter_next (model, &iter);
	}

	data = g_malloc(sizeof(CssedFloatingListData));
	
	// create window and callbacks
	id = gtk_window_new (GTK_WINDOW_TOPLEVEL);
	gtk_window_set_type_hint(GTK_WINDOW (id), GDK_WINDOW_TYPE_HINT_UTILITY );// GDK_WINDOW_TYPE_HINT_MENU
#ifdef GTK_IS_2_2
	gtk_window_set_skip_taskbar_hint(GTK_WINDOW (id), TRUE);
	gtk_window_set_skip_pager_hint(GTK_WINDOW (id), TRUE);
#endif
	gtk_window_set_decorated (GTK_WINDOW (id), FALSE);
	frame = gtk_frame_new (NULL);
	gtk_widget_show (frame);
	gtk_container_add (GTK_CONTAINER (id), frame);
	gtk_frame_set_shadow_type (GTK_FRAME (frame), GTK_SHADOW_OUT);
	gtk_container_set_border_width (GTK_CONTAINER (frame), 0);

	scroller = gtk_scrolled_window_new (NULL, NULL);
	gtk_container_set_border_width (GTK_CONTAINER (scroller), 0);
	gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scroller),
					GTK_POLICY_NEVER,
					GTK_POLICY_AUTOMATIC);
	gtk_container_add (GTK_CONTAINER (frame), scroller);
	gtk_widget_show (GTK_WIDGET (scroller));

	selection = gtk_tree_view_get_selection (GTK_TREE_VIEW (list));
	gtk_tree_selection_set_mode (selection, GTK_SELECTION_SINGLE);
	gtk_tree_view_set_headers_visible (GTK_TREE_VIEW (list), FALSE);
	gtk_tree_view_set_reorderable (GTK_TREE_VIEW (list), FALSE);
	GTK_WIDGET_SET_FLAGS (list, GTK_CAN_DEFAULT);

	column = gtk_tree_view_column_new ();
	gtk_tree_view_column_set_sizing (column,
					 GTK_TREE_VIEW_COLUMN_AUTOSIZE);
	//gtk_tree_view_column_set_title (column, "Autocomplete");

	renderer = gtk_cell_renderer_pixbuf_new ();
	gtk_tree_view_column_pack_start (column, renderer, FALSE);
	gtk_tree_view_column_add_attribute (column, renderer, "pixbuf", 0);

	renderer = gtk_cell_renderer_text_new ();
	gtk_tree_view_column_pack_start (column, renderer, TRUE);
	gtk_tree_view_column_add_attribute (column, renderer, "text", 1);

	gtk_tree_view_append_column (GTK_TREE_VIEW (list), column);
	gtk_container_add (GTK_CONTAINER (scroller), GTK_WIDGET (list));
	gtk_widget_show (GTK_WIDGET (list));

	gtk_window_set_default_size (GTK_WINDOW (id), width, height);
	gtk_window_move (GTK_WINDOW (id), x, y);
	
	data->list_widget = id;
	data->keyword = NULL;
	data->window = window;
	data->output_model = output_model; 
	data->output_path = output_path; 
	data->treeview = list;
	
	g_signal_connect (G_OBJECT (list), "row-activated",
			  G_CALLBACK (on_list_tree_activated), data);

	g_signal_connect (G_OBJECT (id), "focus-out-event",
			  GTK_SIGNAL_FUNC (on_focus_out), data);

	g_signal_connect (G_OBJECT (list), "button-press-event",
			  GTK_SIGNAL_FUNC (on_list_press_event), data);

	gtk_window_set_transient_for (GTK_WINDOW (id), GTK_WINDOW (parent));
	gtk_window_set_destroy_with_parent (GTK_WINDOW (id), TRUE);
	gtk_window_set_accept_focus (GTK_WINDOW(id), TRUE);

	gtk_tree_model_get_iter_first (GTK_TREE_MODEL(store), &iter);
	gtk_tree_selection_select_iter  (selection,  &iter);
	
	gtk_widget_show (id);
	
	return id;
}

void
on_list_tree_activated (GtkTreeView * t, GtkTreePath * p,
			GtkTreeViewColumn * c, gpointer user_data)
{
	CssedFloatingListData* data;
	CssedFloatingListData* newdata;
	GtkTreeIter iter;
	GtkTreeModel *model;
	gchar *prop;
	
	data = LIST_DATA(user_data);
	
	model = gtk_tree_view_get_model (t);
	if (gtk_tree_model_get_iter (model, &iter, p))
	{
		gtk_tree_model_get (model, &iter, 1, &prop, -1);
		newdata = g_malloc(sizeof(CssedFloatingListData));
		newdata->list_widget=data->list_widget;
		newdata->keyword=prop;
		newdata->window=data->window;
		newdata->output_model=data->output_model;
		newdata->output_path=g_strdup(data->output_path);
		run_dialog_disconnected_from_string(newdata);		
	}
}


gboolean
on_list_press_event (GtkWidget * widget,
		     		 GdkEventButton * event, 
					 gpointer user_data)
{
	CssedFloatingListData* data;
	data = (CssedFloatingListData*) user_data;
	
	if (event->button != 1)
	{	
		gtk_widget_hide (GTK_WIDGET (data->list_widget));
	}
	return FALSE;
}

gboolean
on_focus_out (GtkWidget * widget, GdkEventFocus * event, gpointer user_data)
{
	CssedFloatingListData* data;

	if( user_data != NULL ){
		data = LIST_DATA(user_data);
		if( data != NULL){
			if( GTK_IS_WIDGET(data->list_widget) )
				gtk_widget_destroy (GTK_WIDGET (data->list_widget));
			if( data->keyword != NULL)
				g_free(data->keyword);
			if( data->output_path != NULL)
				g_free(data->output_path);
			if( user_data != NULL )
				g_free(user_data);
		}
	}
	return FALSE;
}

void
run_dialog_disconnected_from_string(CssedFloatingListData* data)
{
	GtkWidget* dialog;
	gint response;
	GtkTreeIter iter;
	gboolean valid;
	gchar* response_string;
	CssedDoc* doc;
	GtkWidget* list_widget;
	GtkWidget* hbutton;
	gchar* keyword;
	CssedWindow* window;
	GtkTreeModel* output_model;
	gchar* output_path;
	CssDialogClass* class;

	
	list_widget=data->list_widget;
	keyword=data->keyword;
	window=data->window;
	output_model=data->output_model;
	output_path=data->output_path;	
	
	gtk_widget_hide( list_widget );
	doc = document_get_current(window);

	valid = gtk_tree_model_get_iter_from_string (output_model, &iter, output_path);
	
	if( !valid ) { // FIXME say something to user
		DBGMSG (__FILE__,__LINE__,"Invalid iter from path %s, in model %p\n",	output_path	,output_model);
		g_free(data->keyword);
		g_free(data->output_path);
		g_free(data);
		return;
	}

	if( keyword[0] == '(' ){ // if it doesn't start with '(' it's not a dialog
		if( (class = (CssDialogClass*) cssed_window_css_dialog_lookup_by_keyword ( window, keyword )) != NULL ){
			dialog = class->create_function(window, NULL, FALSE, FALSE);
			hbutton = lookup_widget(dialog,"helpbutton"); // hide help buttton
			if( GTK_IS_BUTTON( hbutton ) ){
				gtk_widget_hide(hbutton);
			}
			response = gtk_dialog_run(GTK_DIALOG(dialog));
			if( response == GTK_RESPONSE_OK ){
				response_string = class->get_string_function( window, (gpointer) dialog );
				if( response_string != NULL ){
					g_strstrip( response_string );
					on_current_css_render_values_edited (  
													NULL,
													output_path,
													response_string,
													window
													);
 					gtk_widget_destroy(dialog);	
					g_free(	response_string	);			
				}
			}else{
				gtk_widget_destroy(dialog);	
			}
		}
		else
		{ // dialog doesn't exists yet
			on_current_css_render_values_edited  (  NULL,
													output_path,
													keyword,
													window);
		}			
	}else{ // is not a dialog
			on_current_css_render_values_edited  (  NULL,
													output_path,
													keyword,
													window);
	}

	g_free(data->keyword);
	g_free(data->output_path);
	g_free(data);
}
