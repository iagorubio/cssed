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
#include "cssdialogs-functions.h"
#include "support.h"

#define GLADE_HOOKUP_OBJECT(component,widget,name) \
  g_object_set_data_full (G_OBJECT (component), name, \
    gtk_widget_ref (widget), (GDestroyNotify) gtk_widget_unref)

#define GLADE_HOOKUP_OBJECT_NO_REF(component,widget,name) \
  g_object_set_data (G_OBJECT (component), name, widget)

/* callbacks */

void
on_box_wizard_close                    (GtkDialog       *dialog,
                                        gpointer         user_data)
{
	gtk_widget_destroy(GTK_WIDGET(user_data));
}

void 
run_css_dialog(CssedWindow *window, GtkWidget *parent_widget,
				char *entry_name,char *title,
				GtkWidget *(*create_function) (CssedWindow*,gchar *,gboolean, gboolean),
				gchar *(*get_string_function) (CssedWindow*, gpointer ) )
{
	GtkWidget *dialog;
	gint response;
	gchar *string;
	CssedDoc *doc;
	GtkWidget *entry;
	
	doc = document_get_current(window);
	entry = lookup_widget(GTK_WIDGET(parent_widget), entry_name);
	
	dialog = create_function(window, title,	FALSE, FALSE);	
	response = gtk_dialog_run(GTK_DIALOG(dialog));
	
	if( response == GTK_RESPONSE_OK ){
		string = get_string_function( window, (gpointer) dialog );
		gtk_entry_set_text(GTK_ENTRY(entry),string);
		g_free(string);
	}
	gtk_widget_destroy(dialog);	
}

void
on_button_all_width_clicked            (GtkButton       *button,
                                        gpointer         user_data)
{
	CssedWindow *window;
	GtkWidget *(*create_function) (CssedWindow*, gchar*, gboolean,   gboolean);
	char *(*get_string_function) ( CssedWindow*, gpointer  );	
	
	window = CSSED_WINDOW(user_data);	
	create_function = &create_percentage_length_value_dialog;
	get_string_function = &cssdialog_percentage_length_value_get_string;	
	run_css_dialog(window, GTK_WIDGET(button), "entry_all_width", "width",
											create_function, 
											get_string_function);
}


void
on_button_all_height_clicked           (GtkButton       *button,
                                        gpointer         user_data)
{
	CssedWindow *window;
	GtkWidget *(*create_function) (CssedWindow*, gchar*, gboolean,   gboolean);
	char *(*get_string_function) ( CssedWindow*, gpointer  );	
	
	window = CSSED_WINDOW(user_data);	
	create_function = &create_percentage_length_value_dialog;
	get_string_function = &cssdialog_percentage_length_value_get_string;	
	run_css_dialog(window, GTK_WIDGET(button), "entry_all_height", "height",
											create_function, 
											get_string_function);
}


void
on_button_all_border_top_clicked       (GtkButton       *button,
                                        gpointer         user_data)
{
	CssedWindow *window;
	GtkWidget *(*create_function) (CssedWindow*, gchar*, gboolean,   gboolean);
	char *(*get_string_function) ( CssedWindow*, gpointer  );	
	
	window = CSSED_WINDOW(user_data);	
	create_function = &create_border_properties_dialog;
	get_string_function = &cssdialog_border_properties_get_string;	
	run_css_dialog(window, GTK_WIDGET(button), "entry_all_border_top", 
											"border-top",
											create_function, 
											get_string_function);
}


void
on_button_all_border_left_clicked      (GtkButton       *button,
                                        gpointer         user_data)
{
	CssedWindow *window;
	GtkWidget *(*create_function) (CssedWindow*, gchar*, gboolean,   gboolean);
	char *(*get_string_function) ( CssedWindow*, gpointer  );	
	
	window = CSSED_WINDOW(user_data);	
	create_function = &create_border_properties_dialog;
	get_string_function = &cssdialog_border_properties_get_string;	
	run_css_dialog(window, GTK_WIDGET(button), "entry_all_border_left", 
											"border-left",
											create_function, 
											get_string_function);
}


void
on_button_all_border_bottom_clicked    (GtkButton       *button,
                                        gpointer         user_data)
{
	CssedWindow *window;
	GtkWidget *(*create_function) (CssedWindow*, gchar*, gboolean,   gboolean);
	char *(*get_string_function) ( CssedWindow*, gpointer  );	
	
	window = CSSED_WINDOW(user_data);	
	create_function = &create_border_properties_dialog;
	get_string_function = &cssdialog_border_properties_get_string;	
	run_css_dialog(window, GTK_WIDGET(button), "entry_all_border_bottom", 
											"border-bottom",
											create_function, 
											get_string_function);
}


void
on_button_all_border_right_clicked     (GtkButton       *button,
                                        gpointer         user_data)
{
	CssedWindow *window;
	GtkWidget *(*create_function) (CssedWindow*, gchar*, gboolean,   gboolean);
	char *(*get_string_function) ( CssedWindow*, gpointer  );	
	
	window = CSSED_WINDOW(user_data);	
	create_function = &create_border_properties_dialog;
	get_string_function = &cssdialog_border_properties_get_string;	
	run_css_dialog(window, GTK_WIDGET(button), "entry_all_border_right", 
											"border-right",
											create_function, 
											get_string_function);
}


void
on_button_all_padding_top_clicked      (GtkButton       *button,
                                        gpointer         user_data)
{
	CssedWindow *window;
	GtkWidget *(*create_function) (CssedWindow*, gchar*, gboolean,   gboolean);
	char *(*get_string_function) ( CssedWindow*, gpointer  );	
	
	window = CSSED_WINDOW(user_data);	
	create_function = &create_percentage_length_value_dialog;
	get_string_function = &cssdialog_percentage_length_value_get_string;	
	run_css_dialog(window, GTK_WIDGET(button), "entry_all_padding_top", 
											"padding-top",
											create_function, 
											get_string_function);
}


void
on_button_all_padding_left_clicked     (GtkButton       *button,
                                        gpointer         user_data)
{
	CssedWindow *window;
	GtkWidget *(*create_function) (CssedWindow*, gchar*, gboolean,   gboolean);
	char *(*get_string_function) ( CssedWindow*, gpointer  );	
	
	window = CSSED_WINDOW(user_data);	
	create_function = &create_percentage_length_value_dialog;
	get_string_function = &cssdialog_percentage_length_value_get_string;	
	run_css_dialog(window, GTK_WIDGET(button), "entry_all_padding_left", 
											"padding-left",
											create_function, 
											get_string_function);
}


void
on_button_all_margin_right_clicked     (GtkButton       *button,
                                        gpointer         user_data)
{
	CssedWindow *window;
	GtkWidget *(*create_function) (CssedWindow*, gchar*, gboolean,   gboolean);
	char *(*get_string_function) ( CssedWindow*, gpointer  );	
	
	window = CSSED_WINDOW(user_data);	
	create_function = &create_percentage_length_value_dialog;
	get_string_function = &cssdialog_percentage_length_value_get_string;	
	run_css_dialog(window, GTK_WIDGET(button), "entry_all_padding_right", 
											"padding-right",
											create_function, 
											get_string_function);
}


void
on_button_all_padding_bottom_clicked   (GtkButton       *button,
                                        gpointer         user_data)
{
	CssedWindow *window;
	GtkWidget *(*create_function) (CssedWindow*, gchar*, gboolean,   gboolean);
	char *(*get_string_function) ( CssedWindow*, gpointer  );	
	
	window = CSSED_WINDOW(user_data);	
	create_function = &create_percentage_length_value_dialog;
	get_string_function = &cssdialog_percentage_length_value_get_string;	
	run_css_dialog(window, GTK_WIDGET(button), "entry_all_padding_bottom", 
											"padding-bottom",
											create_function, 
											get_string_function);
}


void
on_button_all_padding_right_clicked    (GtkButton       *button,
                                        gpointer         user_data)
{
	CssedWindow *window;
	GtkWidget *(*create_function) (CssedWindow*, gchar*, gboolean,   gboolean);
	char *(*get_string_function) ( CssedWindow*, gpointer  );	
	
	window = CSSED_WINDOW(user_data);	
	create_function = &create_percentage_length_value_dialog;
	get_string_function = &cssdialog_percentage_length_value_get_string;	
	run_css_dialog(window, GTK_WIDGET(button), "entry_all_margin_right", 
											"margin-right",
											create_function, 
											get_string_function);
}


void
on_button_all_margin_bottom_clicked    (GtkButton       *button,
                                        gpointer         user_data)
{
	CssedWindow *window;
	GtkWidget *(*create_function) (CssedWindow*, gchar*, gboolean,   gboolean);
	char *(*get_string_function) ( CssedWindow*, gpointer  );	
	
	window = CSSED_WINDOW(user_data);	
	create_function = &create_percentage_length_value_dialog;
	get_string_function = &cssdialog_percentage_length_value_get_string;	
	run_css_dialog(window, GTK_WIDGET(button), "entry_all_margin_bottom", 
											"margin-bottom",
											create_function, 
											get_string_function);
}


void
on_button_all_margin_top_clicked       (GtkButton       *button,
                                        gpointer         user_data)
{
	CssedWindow *window;
	GtkWidget *(*create_function) (CssedWindow*, gchar*, gboolean,   gboolean);
	char *(*get_string_function) ( CssedWindow*, gpointer  );	
	
	window = CSSED_WINDOW(user_data);	
	create_function = &create_percentage_length_value_dialog;
	get_string_function = &cssdialog_percentage_length_value_get_string;	
	run_css_dialog(window, GTK_WIDGET(button), "entry_all_margin_top", 
											"margin-top",
											create_function, 
											get_string_function);
}


void
on_button_all_margin_left_clicked      (GtkButton       *button,
                                        gpointer         user_data)
{
	CssedWindow *window;
	GtkWidget *(*create_function) (CssedWindow*, gchar*, gboolean,   gboolean);
	char *(*get_string_function) ( CssedWindow*, gpointer  );	
	
	window = CSSED_WINDOW(user_data);	
	create_function = &create_percentage_length_value_dialog;
	get_string_function = &cssdialog_percentage_length_value_get_string;	
	run_css_dialog(window, GTK_WIDGET(button), "entry_all_margin_left", 
											"margin-left",
											create_function, 
											get_string_function);
}


void
on_button_short_width_clicked          (GtkButton       *button,
                                        gpointer         user_data)
{
	CssedWindow *window;
	GtkWidget *(*create_function) (CssedWindow*, gchar*, gboolean,   gboolean);
	char *(*get_string_function) ( CssedWindow*, gpointer  );	
	
	window = CSSED_WINDOW(user_data);	
	create_function = &create_percentage_length_value_dialog;
	get_string_function = &cssdialog_percentage_length_value_get_string;	
	run_css_dialog(window, GTK_WIDGET(button), "entry_short_width", "width",
											create_function, 
											get_string_function);
}


void
on_button_short_height_clicked         (GtkButton       *button,
                                        gpointer         user_data)
{
	CssedWindow *window;
	GtkWidget *(*create_function) (CssedWindow*, gchar*, gboolean,   gboolean);
	char *(*get_string_function) ( CssedWindow*, gpointer  );	
	
	window = CSSED_WINDOW(user_data);	
	create_function = &create_percentage_length_value_dialog;
	get_string_function = &cssdialog_percentage_length_value_get_string;	
	run_css_dialog(window, GTK_WIDGET(button), "entry_short_height", "height",
											create_function, 
											get_string_function);
}


void
on_button_short_border_clicked         (GtkButton       *button,
                                        gpointer         user_data)
{
	CssedWindow *window;
	GtkWidget *(*create_function) (CssedWindow*, gchar*, gboolean,   gboolean);
	char *(*get_string_function) ( CssedWindow*, gpointer  );	
	
	window = CSSED_WINDOW(user_data);	
	create_function = &create_border_properties_dialog;
	get_string_function = &cssdialog_border_properties_get_string;	
	run_css_dialog(window, GTK_WIDGET(button), "entry_short_border", 
											"border",
											create_function, 
											get_string_function);
}


void
on_button_short_margin_clicked         (GtkButton       *button,
                                        gpointer         user_data)
{
	CssedWindow *window;
	GtkWidget *(*create_function) (CssedWindow*, gchar*, gboolean,   gboolean);
	char *(*get_string_function) ( CssedWindow*, gpointer  );	
	
	window = CSSED_WINDOW(user_data);	
	create_function = &create_margin_property_dialog;
	get_string_function = &cssdialog_margin_property_get_string;	
	run_css_dialog(window, GTK_WIDGET(button), "entry_short_margin", 
											"margin",
											create_function, 
											get_string_function);
}


void
on_button_short_padding_clicked        (GtkButton       *button,
                                        gpointer         user_data)
{
	CssedWindow *window;
	GtkWidget *(*create_function) (CssedWindow*, gchar*, gboolean,   gboolean);
	char *(*get_string_function) ( CssedWindow*, gpointer  );	
	
	window = CSSED_WINDOW(user_data);	
	create_function = &create_padding_property_dialog;
	get_string_function = &cssdialog_padding_property_get_string;	
	run_css_dialog(window, GTK_WIDGET(button), "entry_short_padding", 
											"padding",
											create_function, 
											get_string_function);
}


void
on_box_wizard_cancelbutton_clicked     (GtkButton       *button,
                                        gpointer         user_data)
{
	gtk_widget_destroy(GTK_WIDGET(user_data));
}

void
write_entry_contents_if_any		(CssedDoc *doc, gchar *previous_string,
								GtkWidget *entry)
{
	gchar *entry_contents;
	gchar *output_string;
	
	entry_contents = gtk_editable_get_chars(GTK_EDITABLE(entry),
                                             0,-1);
	if( strlen( entry_contents ) > 0 ){
		g_strstrip( entry_contents );
		output_string = g_strdup_printf("\t%s: %s;\n", previous_string, 
									entry_contents);
		document_add_text(doc,output_string);
		g_free(output_string);
	}
	g_free( entry_contents );
}

void
on_box_wizard_applybutton_clicked      (GtkButton       *button,
                                        gpointer         user_data)
{
	CssedWindow *window;
	CssedDoc *doc;
	GtkWidget *entry_all_width;
	GtkWidget *entry_all_height;
	GtkWidget *entry_all_border_top;
	GtkWidget *entry_all_border_left;
	GtkWidget *entry_all_border_bottom;
	GtkWidget *entry_all_border_right;
	GtkWidget *entry_all_padding_top;
	GtkWidget *entry_all_padding_left;
	GtkWidget *entry_all_margin_right;
	GtkWidget *entry_all_padding_bottom;
	GtkWidget *entry_all_margin_bottom;
	GtkWidget *entry_all_padding_right;
	GtkWidget *entry_all_margin_top;
	GtkWidget *entry_all_margin_left;
	GtkWidget *entry_short_width;
	GtkWidget *entry_short_height;
	GtkWidget *entry_short_border;
	GtkWidget *entry_short_margin;
	GtkWidget *entry_short_padding;
	GtkWidget *notebook;
	gint pagenum;
	
	window = CSSED_WINDOW(user_data);
	doc = document_get_current(window);
	
	notebook = lookup_widget(GTK_WIDGET(button), "notebook");
	
	g_return_if_fail(GTK_IS_NOTEBOOK(notebook));
		
	pagenum = gtk_notebook_get_current_page(GTK_NOTEBOOK(notebook));
		
	switch( pagenum ){
		case 0: // first page
			entry_all_width = lookup_widget(GTK_WIDGET(button), "entry_all_width");
			write_entry_contents_if_any(doc, "width", entry_all_width);
			
			entry_all_height = lookup_widget(GTK_WIDGET(button), "entry_all_height");
			write_entry_contents_if_any(doc, "height", entry_all_height);
		
			entry_all_border_top= lookup_widget(GTK_WIDGET(button), "entry_all_border_top");
			write_entry_contents_if_any(doc, "border-top", entry_all_border_top);
		
			entry_all_border_left= lookup_widget(GTK_WIDGET(button), "entry_all_border_left");
			write_entry_contents_if_any(doc, "border-left", entry_all_border_left);
		
			entry_all_border_bottom = lookup_widget(GTK_WIDGET(button), "entry_all_border_bottom");
			write_entry_contents_if_any(doc, "border-bottom", entry_all_border_bottom);
		
			entry_all_border_right = lookup_widget(GTK_WIDGET(button), "entry_all_border_right");
			write_entry_contents_if_any(doc, "border-right", entry_all_border_right);
		
			entry_all_padding_top = lookup_widget(GTK_WIDGET(button), "entry_all_padding_top");
			write_entry_contents_if_any(doc, "padding-top", entry_all_padding_top);
		
			entry_all_padding_left = lookup_widget(GTK_WIDGET(button), "entry_all_padding_left");
			write_entry_contents_if_any(doc, "padding-left", entry_all_padding_left);
		
			entry_all_padding_bottom = lookup_widget(GTK_WIDGET(button), "entry_all_padding_bottom");
			write_entry_contents_if_any(doc, "padding-bottom", entry_all_padding_bottom);
		
			entry_all_padding_right = lookup_widget(GTK_WIDGET(button), "entry_all_padding_right");
			write_entry_contents_if_any(doc, "padding-right", entry_all_padding_right);	
		
			entry_all_margin_top = lookup_widget(GTK_WIDGET(button), "entry_all_margin_top");
			write_entry_contents_if_any(doc, "margin-top", entry_all_margin_top);	
			
			entry_all_margin_left = lookup_widget(GTK_WIDGET(button), "entry_all_margin_left");	
			write_entry_contents_if_any(doc, "margin-left", entry_all_margin_left);	
			
			entry_all_margin_bottom = lookup_widget(GTK_WIDGET(button), "entry_all_margin_bottom");	
			write_entry_contents_if_any(doc, "margin-bottom", entry_all_margin_bottom);	
			
			entry_all_margin_right = lookup_widget(GTK_WIDGET(button), "entry_all_margin_right");
			write_entry_contents_if_any(doc, "margin-right", entry_all_margin_right);	

			break;
		case 1: // shorthand page
			entry_short_width = lookup_widget(GTK_WIDGET(button), "entry_short_width");
			write_entry_contents_if_any(doc, "width", entry_short_width);
		
			entry_short_height = lookup_widget(GTK_WIDGET(button), "entry_short_height");
			write_entry_contents_if_any(doc, "height", entry_short_height);
		
			entry_short_border = lookup_widget(GTK_WIDGET(button), "entry_short_border");
			write_entry_contents_if_any(doc, "border", entry_short_border);
		
			entry_short_margin = lookup_widget(GTK_WIDGET(button), "entry_short_margin");
			write_entry_contents_if_any(doc, "margin", entry_short_margin);
		
			entry_short_padding = lookup_widget(GTK_WIDGET(button), "entry_short_padding");
			write_entry_contents_if_any(doc, "padding", entry_short_padding);
		
			break;
		//case 2:// FIXME, do nothing but must say something to user.
		default:
		break;
	}
}


void
on_box_wizard_okbutton_clicked          (GtkButton       *button,
                                        gpointer         user_data)
{
	GtkWidget *dialog;
	
	dialog = lookup_widget(GTK_WIDGET(button), "box_wizard");
	on_box_wizard_applybutton_clicked (button, user_data);
	gtk_widget_destroy(dialog);
}

/* drawing functions */
void
draw_area(GtkWidget       *widget)
{
  const gchar *border = _("<span color='black' size='smaller'>border</span>");
  const gchar *margin = _("<span color='blue' size='smaller'>margin (transparent)</span>");
  const gchar *padding = _("<span color='blue' size='smaller'>padding (transparent)</span>");
  const gchar *content = _("<span color='white' size='smaller'>content</span>");
  const gchar *top = _("<span color='navy' size='smaller'>TOP</span>");
  const gchar *left = _("<span color='navy' size='smaller'>LEFT</span>");
  const gchar *bottom = _("<span color='navy' size='smaller'>BOTTOM</span>");
  const gchar *right = _("<span color='navy' size='smaller'>RIGHT</span>");

  PangoContext *context;
  PangoLayout *layout;
  
  context = gtk_widget_get_pango_context(widget);  
  layout = pango_layout_new( context );


  gdk_draw_rectangle            (widget->window,
                                 widget->style->fg_gc[GTK_WIDGET_STATE (widget)],
                                 FALSE,
                                 40,20, widget->allocation.width - 80, widget->allocation.height - 50);

  gdk_draw_rectangle            (widget->window,
                                 widget->style->fg_gc[GTK_WIDGET_STATE (widget)],
                                 FALSE,
                                 60,40, widget->allocation.width - 120, widget->allocation.height - 90);

  gdk_draw_rectangle            (widget->window,
                                 widget->style->fg_gc[GTK_WIDGET_STATE (widget)],
                                 FALSE,
                                 80,60, widget->allocation.width - 165, widget->allocation.height - 130);
  
  gdk_draw_rectangle            (widget->window,
                                 widget->style->fg_gc[GTK_WIDGET_STATE (widget)],
                                 TRUE,
                                 100,80, widget->allocation.width - 200, widget->allocation.height - 170);
								 
  pango_layout_set_markup(layout,top ,strlen(top));

  gdk_draw_layout    (widget->window,
                      widget->style->fg_gc[GTK_WIDGET_STATE (widget)],
                      widget->allocation.width/2 - 15,
                      5,layout);

  pango_layout_set_markup(layout,margin,strlen(margin));
  gdk_draw_layout    (widget->window,
                      widget->style->fg_gc[GTK_WIDGET_STATE (widget)],
                      widget->allocation.width/2 - 100,
                      24,layout);

  pango_layout_set_markup(layout,border,strlen(border));
  gdk_draw_layout    (widget->window,
                      widget->style->fg_gc[GTK_WIDGET_STATE (widget)],
                      widget->allocation.width/2 - 15,
                      40,layout);

  pango_layout_set_markup(layout,padding,strlen(padding));
  gdk_draw_layout    (widget->window,
                      widget->style->fg_gc[GTK_WIDGET_STATE (widget)],
                      widget->allocation.width/2 - 80,
                      60,layout);

  pango_layout_set_markup (layout,content,strlen(content));
  gdk_draw_layout    (widget->window,
                      widget->style->fg_gc[GTK_WIDGET_STATE (widget)],
                      widget->allocation.width/2 - 15,
                      widget->allocation.height/2 - 10,layout); 
 
  pango_layout_set_markup (layout,right,strlen(right));
  gdk_draw_layout    (widget->window,
                      widget->style->fg_gc[GTK_WIDGET_STATE (widget)],
                      0,
                      widget->allocation.height/2 - 40,layout); 

  pango_layout_set_markup (layout,bottom,strlen(bottom));

  gdk_draw_layout    (widget->window,
                      widget->style->fg_gc[GTK_WIDGET_STATE (widget)],
                      widget->allocation.width/2 - 5,
                      widget->allocation.height - 20,layout); 

  pango_layout_set_markup (layout,left,strlen(left));

  gdk_draw_layout    (widget->window,
                      widget->style->fg_gc[GTK_WIDGET_STATE (widget)],
                      widget->allocation.width - 30,
                      widget->allocation.height/2 - 40,layout); 

}



gboolean
on_drawingarea_expose_event            (GtkWidget       *widget,
                                        GdkEventExpose  *event,
                                        gpointer         user_data)
{
  draw_area(widget);
  return FALSE;
}
/* draw end */


/* UI */
GtkWidget*
create_box_wizard (CssedWindow *window)
{
  GtkWidget *label_blank;
  GtkWidget *box_wizard;
  //GdkPixbuf *box_wizard_icon_pixbuf;
  GtkWidget *dialog_vbox;
  GtkWidget *notebook;
  GtkWidget *table_all_border_right;
  GtkWidget *button_all_width;
  GtkWidget *entry_all_width;
  GtkWidget *label_all_width;
  GtkWidget *entry_all_height;
  GtkWidget *entry_all_border_top;
  GtkWidget *entry_all_border_left;
  GtkWidget *entry_all_border_bottom;
  GtkWidget *entry_all_border_right;
  GtkWidget *entry_all_padding_top;
  GtkWidget *entry_all_padding_left;
  GtkWidget *entry_all_margin_right;
  GtkWidget *button_all_height;
  GtkWidget *button_all_border_top;
  GtkWidget *button_all_border_left;
  GtkWidget *button_all_border_bottom;
  GtkWidget *button_all_border_right;
  GtkWidget *button_all_padding_top;
  GtkWidget *button_all_padding_left;
  GtkWidget *button_all_margin_right;
  GtkWidget *label_all_height;
  GtkWidget *label_all_border_left;
  GtkWidget *label_all_border_bottom;
  GtkWidget *label_all_border_right;
  GtkWidget *label_all_padding_top;
  GtkWidget *label_all_padding_left;
  GtkWidget *button_all_padding_bottom;
  GtkWidget *button_all_padding_right;
  GtkWidget *button_all_margin_bottom;
  GtkWidget *entry_all_padding_bottom;
  GtkWidget *entry_all_margin_bottom;
  GtkWidget *entry_all_padding_right;
  GtkWidget *label_all_padding_bottom;
  GtkWidget *label_all_padding_right;
  GtkWidget *label_all_margin_bottom;
  GtkWidget *label_all_margin_right;
  GtkWidget *label_all_border_top;
  GtkWidget *label_all_margin_top;
  GtkWidget *label_all_margin_left;
  GtkWidget *entry_all_margin_top;
  GtkWidget *entry_all_margin_left;
  GtkWidget *button_all_margin_top;
  GtkWidget *button_all_margin_left;
  GtkWidget *label_all_padding_big;
  GtkWidget *label_all_margin_big;
  GtkWidget *label_all_border_big;
  GtkWidget *label_all_box_big;
  GtkWidget *label_prop;
  GtkWidget *table_shorthand;
  GtkWidget *entry_short_width;
  GtkWidget *button_short_width;
  GtkWidget *entry_short_height;
  GtkWidget *entry_short_border;
  GtkWidget *entry_short_margin;
  GtkWidget *entry_short_padding;
  GtkWidget *button_short_height;
  GtkWidget *button_short_border;
  GtkWidget *button_short_margin;
  GtkWidget *button_short_padding;
  GtkWidget *label_short_box_big;
  GtkWidget *label_short_width;
  GtkWidget *label_short_height;
  GtkWidget *label_short_border;
  GtkWidget *label_short_margin;
  GtkWidget *label_short_padding;
  GtkWidget *label_short_box_big_prop;
  GtkWidget *label_shorthand;
  GtkWidget *hbox_help;
  GtkWidget *vbox_help;
  GtkWidget *scrolledwindow;
  GtkWidget *textview;
  GtkWidget *drawingarea;
  GtkWidget *label_help;
  GtkWidget *dialog_action_area;
  GtkWidget *cancelbutton;
  GtkWidget *applybutton;
  GtkWidget *okbutton;

  label_blank = gtk_label_new ("");
  
  box_wizard = gtk_dialog_new ();
  gtk_window_set_title (GTK_WINDOW (box_wizard), _("Box properties dialog"));
  gtk_window_set_destroy_with_parent (GTK_WINDOW (box_wizard), TRUE);
  gtk_window_set_transient_for ( GTK_WINDOW(box_wizard),
				      GTK_WINDOW ( cssed_window_get_window_widget(window) ));

  /*
  box_wizard_icon_pixbuf = create_pixbuf ("dlg_border.png");
  if (box_wizard_icon_pixbuf)
    {
      gtk_window_set_icon (GTK_WINDOW (box_wizard), box_wizard_icon_pixbuf);
      gdk_pixbuf_unref (box_wizard_icon_pixbuf);
    }
  */
  dialog_vbox = GTK_DIALOG (box_wizard)->vbox;
  gtk_widget_show (dialog_vbox);

  notebook = gtk_notebook_new ();
  gtk_widget_show (notebook);
  gtk_box_pack_start (GTK_BOX (dialog_vbox), notebook, TRUE, TRUE, 0);

  table_all_border_right = gtk_table_new (21, 3, FALSE);
  gtk_widget_show (table_all_border_right);
  gtk_container_add (GTK_CONTAINER (notebook), table_all_border_right);
  gtk_container_set_border_width (GTK_CONTAINER (table_all_border_right), 5);

  button_all_width = gtk_button_new_with_mnemonic (_("Set"));
  gtk_widget_show (button_all_width);
  gtk_table_attach (GTK_TABLE (table_all_border_right), button_all_width, 2, 3, 1, 2,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 5, 0);

  entry_all_width = gtk_entry_new ();
  gtk_widget_show (entry_all_width);
  gtk_table_attach (GTK_TABLE (table_all_border_right), entry_all_width, 1, 2, 1, 2,
                    (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                    (GtkAttachOptions) (0), 5, 0);

  label_all_width = gtk_label_new ("width");
  gtk_widget_show (label_all_width);
  gtk_table_attach (GTK_TABLE (table_all_border_right), label_all_width, 0, 1, 1, 2,
                    (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                    (GtkAttachOptions) (GTK_EXPAND), 10, 0);
  gtk_label_set_justify (GTK_LABEL (label_all_width), GTK_JUSTIFY_LEFT);
  gtk_misc_set_alignment (GTK_MISC (label_all_width), 0, 0.5);

  entry_all_height = gtk_entry_new ();
  gtk_widget_show (entry_all_height);
  gtk_table_attach (GTK_TABLE (table_all_border_right), entry_all_height, 1, 2, 2, 3,
                    (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                    (GtkAttachOptions) (0), 5, 0);

  entry_all_border_top = gtk_entry_new ();
  gtk_widget_show (entry_all_border_top);
  gtk_table_attach (GTK_TABLE (table_all_border_right), entry_all_border_top, 1, 2, 5, 6,
                    (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                    (GtkAttachOptions) (0), 5, 0);

  entry_all_border_left = gtk_entry_new ();
  gtk_widget_show (entry_all_border_left);
  gtk_table_attach (GTK_TABLE (table_all_border_right), entry_all_border_left, 1, 2, 6, 7,
                    (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                    (GtkAttachOptions) (0), 5, 0);

  entry_all_border_bottom = gtk_entry_new ();
  gtk_widget_show (entry_all_border_bottom);
  gtk_table_attach (GTK_TABLE (table_all_border_right), entry_all_border_bottom, 1, 2, 7, 8,
                    (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                    (GtkAttachOptions) (0), 5, 0);

  entry_all_border_right = gtk_entry_new ();
  gtk_widget_show (entry_all_border_right);
  gtk_table_attach (GTK_TABLE (table_all_border_right), entry_all_border_right, 1, 2, 8, 9,
                    (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                    (GtkAttachOptions) (0), 5, 0);

  entry_all_padding_top = gtk_entry_new ();
  gtk_widget_show (entry_all_padding_top);
  gtk_table_attach (GTK_TABLE (table_all_border_right), entry_all_padding_top, 1, 2, 11, 12,
                    (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                    (GtkAttachOptions) (0), 5, 0);

  entry_all_padding_left = gtk_entry_new ();
  gtk_widget_show (entry_all_padding_left);
  gtk_table_attach (GTK_TABLE (table_all_border_right), entry_all_padding_left, 1, 2, 12, 13,
                    (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                    (GtkAttachOptions) (0), 5, 0);

  entry_all_margin_right = gtk_entry_new ();
  gtk_widget_show (entry_all_margin_right);
  gtk_table_attach (GTK_TABLE (table_all_border_right), entry_all_margin_right, 1, 2, 20, 21,
                    (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                    (GtkAttachOptions) (0), 5, 0);

  button_all_height = gtk_button_new_with_mnemonic (_("Set"));
  gtk_widget_show (button_all_height);
  gtk_table_attach (GTK_TABLE (table_all_border_right), button_all_height, 2, 3, 2, 3,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 5, 0);

  button_all_border_top = gtk_button_new_with_mnemonic (_("Set"));
  gtk_widget_show (button_all_border_top);
  gtk_table_attach (GTK_TABLE (table_all_border_right), button_all_border_top, 2, 3, 5, 6,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 5, 0);

  button_all_border_left = gtk_button_new_with_mnemonic (_("Set"));
  gtk_widget_show (button_all_border_left);
  gtk_table_attach (GTK_TABLE (table_all_border_right), button_all_border_left, 2, 3, 6, 7,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 5, 0);

  button_all_border_bottom = gtk_button_new_with_mnemonic (_("Set"));
  gtk_widget_show (button_all_border_bottom);
  gtk_table_attach (GTK_TABLE (table_all_border_right), button_all_border_bottom, 2, 3, 7, 8,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 5, 0);

  button_all_border_right = gtk_button_new_with_mnemonic (_("Set"));
  gtk_widget_show (button_all_border_right);
  gtk_table_attach (GTK_TABLE (table_all_border_right), button_all_border_right, 2, 3, 8, 9,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 5, 0);

  button_all_padding_top = gtk_button_new_with_mnemonic (_("Set"));
  gtk_widget_show (button_all_padding_top);
  gtk_table_attach (GTK_TABLE (table_all_border_right), button_all_padding_top, 2, 3, 11, 12,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 5, 0);

  button_all_padding_left = gtk_button_new_with_mnemonic (_("Set"));
  gtk_widget_show (button_all_padding_left);
  gtk_table_attach (GTK_TABLE (table_all_border_right), button_all_padding_left, 2, 3, 12, 13,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 5, 0);

  button_all_margin_right = gtk_button_new_with_mnemonic (_("Set"));
  gtk_widget_show (button_all_margin_right);
  gtk_table_attach (GTK_TABLE (table_all_border_right), button_all_margin_right, 2, 3, 20, 21,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 5, 0);

  label_all_height = gtk_label_new ("height");
  gtk_widget_show (label_all_height);
  gtk_table_attach (GTK_TABLE (table_all_border_right), label_all_height, 0, 1, 2, 3,
                    (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                    (GtkAttachOptions) (GTK_EXPAND), 10, 0);
  gtk_label_set_justify (GTK_LABEL (label_all_height), GTK_JUSTIFY_LEFT);
  gtk_misc_set_alignment (GTK_MISC (label_all_height), 0, 0.5);

  label_all_border_left = gtk_label_new ("border-left");
  gtk_widget_show (label_all_border_left);
  gtk_table_attach (GTK_TABLE (table_all_border_right), label_all_border_left, 0, 1, 6, 7,
                    (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                    (GtkAttachOptions) (GTK_EXPAND), 10, 0);
  gtk_label_set_justify (GTK_LABEL (label_all_border_left), GTK_JUSTIFY_LEFT);
  gtk_misc_set_alignment (GTK_MISC (label_all_border_left), 0, 0.5);

  label_all_border_bottom = gtk_label_new ("border-bottom");
  gtk_widget_show (label_all_border_bottom);
  gtk_table_attach (GTK_TABLE (table_all_border_right), label_all_border_bottom, 0, 1, 7, 8,
                    (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                    (GtkAttachOptions) (GTK_EXPAND), 10, 0);
  gtk_label_set_justify (GTK_LABEL (label_all_border_bottom), GTK_JUSTIFY_LEFT);
  gtk_misc_set_alignment (GTK_MISC (label_all_border_bottom), 0, 0.5);

  label_all_border_right = gtk_label_new ("border-right");
  gtk_widget_show (label_all_border_right);
  gtk_table_attach (GTK_TABLE (table_all_border_right), label_all_border_right, 0, 1, 8, 9,
                    (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                    (GtkAttachOptions) (GTK_EXPAND), 10, 0);
  gtk_label_set_justify (GTK_LABEL (label_all_border_right), GTK_JUSTIFY_LEFT);
  gtk_misc_set_alignment (GTK_MISC (label_all_border_right), 0, 0.5);

  label_all_padding_top = gtk_label_new ("padding-top");
  gtk_widget_show (label_all_padding_top);
  gtk_table_attach (GTK_TABLE (table_all_border_right), label_all_padding_top, 0, 1, 11, 12,
                    (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                    (GtkAttachOptions) (GTK_EXPAND), 10, 0);
  gtk_label_set_justify (GTK_LABEL (label_all_padding_top), GTK_JUSTIFY_LEFT);
  gtk_misc_set_alignment (GTK_MISC (label_all_padding_top), 0, 0.5);

  label_all_padding_left = gtk_label_new ("padding-left");
  gtk_widget_show (label_all_padding_left);
  gtk_table_attach (GTK_TABLE (table_all_border_right), label_all_padding_left, 0, 1, 12, 13,
                    (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                    (GtkAttachOptions) (GTK_EXPAND), 10, 0);
  gtk_label_set_justify (GTK_LABEL (label_all_padding_left), GTK_JUSTIFY_LEFT);
  gtk_misc_set_alignment (GTK_MISC (label_all_padding_left), 0, 0.5);

  button_all_padding_bottom = gtk_button_new_with_mnemonic (_("Set"));
  gtk_widget_show (button_all_padding_bottom);
  gtk_table_attach (GTK_TABLE (table_all_border_right), button_all_padding_bottom, 2, 3, 13, 14,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 5, 0);

  button_all_padding_right = gtk_button_new_with_mnemonic (_("Set"));
  gtk_widget_show (button_all_padding_right);
  gtk_table_attach (GTK_TABLE (table_all_border_right), button_all_padding_right, 2, 3, 14, 15,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 5, 0);

  button_all_margin_bottom = gtk_button_new_with_mnemonic (_("Set"));
  gtk_widget_show (button_all_margin_bottom);
  gtk_table_attach (GTK_TABLE (table_all_border_right), button_all_margin_bottom, 2, 3, 19, 20,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 5, 0);

  entry_all_padding_bottom = gtk_entry_new ();
  gtk_widget_show (entry_all_padding_bottom);
  gtk_table_attach (GTK_TABLE (table_all_border_right), entry_all_padding_bottom, 1, 2, 13, 14,
                    (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                    (GtkAttachOptions) (0), 5, 0);

  entry_all_margin_bottom = gtk_entry_new ();
  gtk_widget_show (entry_all_margin_bottom);
  gtk_table_attach (GTK_TABLE (table_all_border_right), entry_all_margin_bottom, 1, 2, 19, 20,
                    (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                    (GtkAttachOptions) (0), 5, 0);

  entry_all_padding_right = gtk_entry_new ();
  gtk_widget_show (entry_all_padding_right);
  gtk_table_attach (GTK_TABLE (table_all_border_right), entry_all_padding_right, 1, 2, 14, 15,
                    (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                    (GtkAttachOptions) (0), 5, 0);

  label_all_padding_bottom = gtk_label_new ("padding-bottom");
  gtk_widget_show (label_all_padding_bottom);
  gtk_table_attach (GTK_TABLE (table_all_border_right), label_all_padding_bottom, 0, 1, 13, 14,
                    (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                    (GtkAttachOptions) (GTK_EXPAND), 10, 0);
  gtk_label_set_justify (GTK_LABEL (label_all_padding_bottom), GTK_JUSTIFY_LEFT);
  gtk_misc_set_alignment (GTK_MISC (label_all_padding_bottom), 0, 0.5);

  label_all_padding_right = gtk_label_new ("padding-right");
  gtk_widget_show (label_all_padding_right);
  gtk_table_attach (GTK_TABLE (table_all_border_right), label_all_padding_right, 0, 1, 14, 15,
                    (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                    (GtkAttachOptions) (GTK_EXPAND), 10, 0);
  gtk_label_set_justify (GTK_LABEL (label_all_padding_right), GTK_JUSTIFY_LEFT);
  gtk_misc_set_alignment (GTK_MISC (label_all_padding_right), 0, 0.5);

  label_all_margin_bottom = gtk_label_new ("margin-bottom");
  gtk_widget_show (label_all_margin_bottom);
  gtk_table_attach (GTK_TABLE (table_all_border_right), label_all_margin_bottom, 0, 1, 19, 20,
                    (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                    (GtkAttachOptions) (GTK_EXPAND), 10, 0);
  gtk_label_set_justify (GTK_LABEL (label_all_margin_bottom), GTK_JUSTIFY_LEFT);
  gtk_misc_set_alignment (GTK_MISC (label_all_margin_bottom), 0, 0.5);

  label_all_margin_right = gtk_label_new ("margin-right");
  gtk_widget_show (label_all_margin_right);
  gtk_table_attach (GTK_TABLE (table_all_border_right), label_all_margin_right, 0, 1, 20, 21,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 10, 0);
  gtk_label_set_justify (GTK_LABEL (label_all_margin_right), GTK_JUSTIFY_LEFT);
  gtk_misc_set_alignment (GTK_MISC (label_all_margin_right), 0, 0.5);

  label_all_border_top = gtk_label_new ("border-top");
  gtk_widget_show (label_all_border_top);
  gtk_table_attach (GTK_TABLE (table_all_border_right), label_all_border_top, 0, 1, 5, 6,
                    (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                    (GtkAttachOptions) (GTK_EXPAND), 10, 0);
  gtk_label_set_justify (GTK_LABEL (label_all_border_top), GTK_JUSTIFY_LEFT);
  gtk_misc_set_alignment (GTK_MISC (label_all_border_top), 0, 0.5);
  
  label_blank = gtk_label_new ("");
  gtk_widget_show (label_blank);
  gtk_table_attach (GTK_TABLE (table_all_border_right), label_blank, 1, 2, 4, 5,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_label_set_justify (GTK_LABEL (label_blank), GTK_JUSTIFY_LEFT);
  gtk_misc_set_alignment (GTK_MISC (label_blank), 0, 0.5);
  
  label_blank = gtk_label_new ("");
  gtk_widget_show (label_blank);
  gtk_table_attach (GTK_TABLE (table_all_border_right), label_blank, 2, 3, 4, 5,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_label_set_justify (GTK_LABEL (label_blank), GTK_JUSTIFY_LEFT);
  gtk_misc_set_alignment (GTK_MISC (label_blank), 0, 0.5);
  
  label_blank = gtk_label_new ("");
  gtk_widget_show (label_blank);
  gtk_table_attach (GTK_TABLE (table_all_border_right), label_blank, 1, 2, 10, 11,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_label_set_justify (GTK_LABEL (label_blank), GTK_JUSTIFY_LEFT);
  gtk_misc_set_alignment (GTK_MISC (label_blank), 0, 0.5);
  
  label_blank = gtk_label_new ("");
  gtk_widget_show (label_blank);
  gtk_table_attach (GTK_TABLE (table_all_border_right), label_blank, 2, 3, 10, 11,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_label_set_justify (GTK_LABEL (label_blank), GTK_JUSTIFY_LEFT);
  gtk_misc_set_alignment (GTK_MISC (label_blank), 0, 0.5);
  
  label_blank = gtk_label_new ("");
  gtk_widget_show (label_blank);
  gtk_table_attach (GTK_TABLE (table_all_border_right), label_blank, 1, 2, 16, 17,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_label_set_justify (GTK_LABEL (label_blank), GTK_JUSTIFY_LEFT);
  gtk_misc_set_alignment (GTK_MISC (label_blank), 0, 0.5);

  
  label_blank = gtk_label_new ("");
  gtk_widget_show (label_blank);
  gtk_table_attach (GTK_TABLE (table_all_border_right), label_blank, 2, 3, 16, 17,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_label_set_justify (GTK_LABEL (label_blank), GTK_JUSTIFY_LEFT);
  gtk_misc_set_alignment (GTK_MISC (label_blank), 0, 0.5);

  label_all_margin_top = gtk_label_new (_("margin-top"));
  gtk_widget_show (label_all_margin_top);
  gtk_table_attach (GTK_TABLE (table_all_border_right), label_all_margin_top, 0, 1, 17, 18,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 10, 0);
  gtk_label_set_justify (GTK_LABEL (label_all_margin_top), GTK_JUSTIFY_LEFT);
  gtk_misc_set_alignment (GTK_MISC (label_all_margin_top), 0, 0.5);

  label_all_margin_left = gtk_label_new (_("margin-left"));
  gtk_widget_show (label_all_margin_left);
  gtk_table_attach (GTK_TABLE (table_all_border_right), label_all_margin_left, 0, 1, 18, 19,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 10, 0);
  gtk_label_set_justify (GTK_LABEL (label_all_margin_left), GTK_JUSTIFY_LEFT);
  gtk_misc_set_alignment (GTK_MISC (label_all_margin_left), 0, 0.5);

  entry_all_margin_top = gtk_entry_new ();
  gtk_widget_show (entry_all_margin_top);
  gtk_table_attach (GTK_TABLE (table_all_border_right), entry_all_margin_top, 1, 2, 17, 18,
                    (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                    (GtkAttachOptions) (0), 5, 0);

  entry_all_margin_left = gtk_entry_new ();
  gtk_widget_show (entry_all_margin_left);
  gtk_table_attach (GTK_TABLE (table_all_border_right), entry_all_margin_left, 1, 2, 18, 19,
                    (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                    (GtkAttachOptions) (0), 5, 0);

  button_all_margin_top = gtk_button_new_with_mnemonic (_("Set"));
  gtk_widget_show (button_all_margin_top);
  gtk_table_attach (GTK_TABLE (table_all_border_right), button_all_margin_top, 2, 3, 17, 18,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 5, 0);

  button_all_margin_left = gtk_button_new_with_mnemonic (_("Set"));
  gtk_widget_show (button_all_margin_left);
  gtk_table_attach (GTK_TABLE (table_all_border_right), button_all_margin_left, 2, 3, 18, 19,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 5, 0);

  label_all_padding_big = gtk_label_new (_("<span size='large'>Padding</span>"));
  gtk_widget_show (label_all_padding_big);
  gtk_table_attach (GTK_TABLE (table_all_border_right), label_all_padding_big, 0, 1, 10, 11,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_label_set_use_markup (GTK_LABEL (label_all_padding_big), TRUE);
  gtk_label_set_justify (GTK_LABEL (label_all_padding_big), GTK_JUSTIFY_LEFT);
  gtk_misc_set_alignment (GTK_MISC (label_all_padding_big), 0, 0.5);

  label_all_margin_big = gtk_label_new (_("<span size='large'>Margin</span>"));
  gtk_widget_show (label_all_margin_big);
  gtk_table_attach (GTK_TABLE (table_all_border_right), label_all_margin_big, 0, 1, 16, 17,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_label_set_use_markup (GTK_LABEL (label_all_margin_big), TRUE);
  gtk_label_set_justify (GTK_LABEL (label_all_margin_big), GTK_JUSTIFY_LEFT);
  gtk_misc_set_alignment (GTK_MISC (label_all_margin_big), 0, 0.5);

  label_all_border_big = gtk_label_new (_("<span size='large'>Border</span>"));
  gtk_widget_show (label_all_border_big);
  gtk_table_attach (GTK_TABLE (table_all_border_right), label_all_border_big, 0, 1, 4, 5,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_label_set_use_markup (GTK_LABEL (label_all_border_big), TRUE);
  gtk_label_set_justify (GTK_LABEL (label_all_border_big), GTK_JUSTIFY_LEFT);
  gtk_misc_set_alignment (GTK_MISC (label_all_border_big), 0, 0.5);
  
  label_blank = gtk_label_new ("");
  gtk_widget_show (label_blank);
  gtk_table_attach (GTK_TABLE (table_all_border_right), label_blank, 0, 1, 15, 16,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_label_set_justify (GTK_LABEL (label_blank), GTK_JUSTIFY_LEFT);
  gtk_misc_set_alignment (GTK_MISC (label_blank), 0, 0.5);
  
  label_blank = gtk_label_new ("");
  gtk_widget_show (label_blank);
  gtk_table_attach (GTK_TABLE (table_all_border_right), label_blank, 1, 2, 15, 16,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_label_set_justify (GTK_LABEL (label_blank), GTK_JUSTIFY_LEFT);
  gtk_misc_set_alignment (GTK_MISC (label_blank), 0, 0.5);
  
  label_blank = gtk_label_new ("");
  gtk_widget_show (label_blank);
  gtk_table_attach (GTK_TABLE (table_all_border_right), label_blank, 2, 3, 15, 16,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_label_set_justify (GTK_LABEL (label_blank), GTK_JUSTIFY_LEFT);
  gtk_misc_set_alignment (GTK_MISC (label_blank), 0, 0.5);
  
  label_blank = gtk_label_new ("");
  gtk_widget_show (label_blank);
  gtk_table_attach (GTK_TABLE (table_all_border_right), label_blank, 0, 1, 9, 10,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_label_set_justify (GTK_LABEL (label_blank), GTK_JUSTIFY_LEFT);
  gtk_misc_set_alignment (GTK_MISC (label_blank), 0, 0.5);
  
  label_blank = gtk_label_new ("");
  gtk_widget_show (label_blank);
  gtk_table_attach (GTK_TABLE (table_all_border_right), label_blank, 1, 2, 9, 10,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_label_set_justify (GTK_LABEL (label_blank), GTK_JUSTIFY_LEFT);
  gtk_misc_set_alignment (GTK_MISC (label_blank), 0, 0.5);
  
  label_blank = gtk_label_new ("");
  gtk_widget_show (label_blank);
  gtk_table_attach (GTK_TABLE (table_all_border_right), label_blank, 2, 3, 9, 10,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_label_set_justify (GTK_LABEL (label_blank), GTK_JUSTIFY_LEFT);
  gtk_misc_set_alignment (GTK_MISC (label_blank), 0, 0.5);
  
  label_blank = gtk_label_new ("");
  gtk_widget_show (label_blank);
  gtk_table_attach (GTK_TABLE (table_all_border_right), label_blank, 0, 1, 3, 4,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_label_set_justify (GTK_LABEL (label_blank), GTK_JUSTIFY_LEFT);
  gtk_misc_set_alignment (GTK_MISC (label_blank), 0, 0.5);
  
  label_blank = gtk_label_new ("");
  gtk_widget_show (label_blank);
  gtk_table_attach (GTK_TABLE (table_all_border_right), label_blank, 1, 2, 3, 4,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_label_set_justify (GTK_LABEL (label_blank), GTK_JUSTIFY_LEFT);
  gtk_misc_set_alignment (GTK_MISC (label_blank), 0, 0.5);
  
  label_blank = gtk_label_new ("");
  gtk_widget_show (label_blank);
  gtk_table_attach (GTK_TABLE (table_all_border_right), label_blank, 2, 3, 3, 4,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_label_set_justify (GTK_LABEL (label_blank), GTK_JUSTIFY_LEFT);
  gtk_misc_set_alignment (GTK_MISC (label_blank), 0, 0.5);

  label_all_box_big = gtk_label_new (_("<span size='large'>Box</span>"));
  gtk_widget_show (label_all_box_big);
  gtk_table_attach (GTK_TABLE (table_all_border_right), label_all_box_big, 0, 1, 0, 1,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_label_set_use_markup (GTK_LABEL (label_all_box_big), TRUE);
  gtk_label_set_justify (GTK_LABEL (label_all_box_big), GTK_JUSTIFY_LEFT);
  gtk_misc_set_alignment (GTK_MISC (label_all_box_big), 0, 0.5);
  
  label_blank = gtk_label_new ("");
  gtk_widget_show (label_blank);
  gtk_table_attach (GTK_TABLE (table_all_border_right), label_blank, 1, 2, 0, 1,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_label_set_justify (GTK_LABEL (label_blank), GTK_JUSTIFY_LEFT);
  gtk_misc_set_alignment (GTK_MISC (label_blank), 0, 0.5);
  
  label_blank = gtk_label_new ("");
  gtk_widget_show (label_blank);
  gtk_table_attach (GTK_TABLE (table_all_border_right), label_blank, 2, 3, 0, 1,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_label_set_justify (GTK_LABEL (label_blank), GTK_JUSTIFY_LEFT);
  gtk_misc_set_alignment (GTK_MISC (label_blank), 0, 0.5);

  label_prop = gtk_label_new (_("All properties"));
  gtk_widget_show (label_prop);
  gtk_notebook_set_tab_label (GTK_NOTEBOOK (notebook), gtk_notebook_get_nth_page (GTK_NOTEBOOK (notebook), 0), label_prop);
  gtk_label_set_justify (GTK_LABEL (label_prop), GTK_JUSTIFY_LEFT);

  table_shorthand = gtk_table_new (8, 3, FALSE);
  gtk_widget_show (table_shorthand);
  gtk_container_add (GTK_CONTAINER (notebook), table_shorthand);
  gtk_container_set_border_width (GTK_CONTAINER (table_shorthand), 5);

  entry_short_width = gtk_entry_new ();
  gtk_widget_show (entry_short_width);
  gtk_table_attach (GTK_TABLE (table_shorthand), entry_short_width, 1, 2, 1, 2,
                    (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                    (GtkAttachOptions) (0), 5, 0);

  button_short_width = gtk_button_new_with_mnemonic (_("Set"));
  gtk_widget_show (button_short_width);
  gtk_table_attach (GTK_TABLE (table_shorthand), button_short_width, 2, 3, 1, 2,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 5, 0);

  entry_short_height = gtk_entry_new ();
  gtk_widget_show (entry_short_height);
  gtk_table_attach (GTK_TABLE (table_shorthand), entry_short_height, 1, 2, 2, 3,
                    (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                    (GtkAttachOptions) (0), 5, 0);

  entry_short_border = gtk_entry_new ();
  gtk_widget_show (entry_short_border);
  gtk_table_attach (GTK_TABLE (table_shorthand), entry_short_border, 1, 2, 5, 6,
                    (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                    (GtkAttachOptions) (0), 5, 0);

  entry_short_margin = gtk_entry_new ();
  gtk_widget_show (entry_short_margin);
  gtk_table_attach (GTK_TABLE (table_shorthand), entry_short_margin, 1, 2, 6, 7,
                    (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                    (GtkAttachOptions) (0), 5, 0);

  entry_short_padding = gtk_entry_new ();
  gtk_widget_show (entry_short_padding);
  gtk_table_attach (GTK_TABLE (table_shorthand), entry_short_padding, 1, 2, 7, 8,
                    (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                    (GtkAttachOptions) (0), 5, 0);

  button_short_height = gtk_button_new_with_mnemonic (_("Set"));
  gtk_widget_show (button_short_height);
  gtk_table_attach (GTK_TABLE (table_shorthand), button_short_height, 2, 3, 2, 3,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 5, 0);

  button_short_border = gtk_button_new_with_mnemonic (_("Set"));
  gtk_widget_show (button_short_border);
  gtk_table_attach (GTK_TABLE (table_shorthand), button_short_border, 2, 3, 5, 6,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 5, 0);

  button_short_margin = gtk_button_new_with_mnemonic (_("Set"));
  gtk_widget_show (button_short_margin);
  gtk_table_attach (GTK_TABLE (table_shorthand), button_short_margin, 2, 3, 6, 7,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 5, 0);

  button_short_padding = gtk_button_new_with_mnemonic (_("Set"));
  gtk_widget_show (button_short_padding);
  gtk_table_attach (GTK_TABLE (table_shorthand), button_short_padding, 2, 3, 7, 8,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 5, 0);

  label_short_box_big = gtk_label_new (_("<span size='large'>Box</span>"));
  gtk_widget_show (label_short_box_big);
  gtk_table_attach (GTK_TABLE (table_shorthand), label_short_box_big, 0, 1, 0, 1,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_label_set_use_markup (GTK_LABEL (label_short_box_big), TRUE);
  gtk_label_set_justify (GTK_LABEL (label_short_box_big), GTK_JUSTIFY_LEFT);
  gtk_misc_set_alignment (GTK_MISC (label_short_box_big), 0, 0.5);
  
  label_blank = gtk_label_new ("");
  gtk_widget_show (label_blank);
  gtk_table_attach (GTK_TABLE (table_shorthand), label_blank, 1, 2, 0, 1,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_label_set_justify (GTK_LABEL (label_blank), GTK_JUSTIFY_LEFT);
  gtk_misc_set_alignment (GTK_MISC (label_blank), 0, 0.5);
  
  label_blank = gtk_label_new ("");
  gtk_widget_show (label_blank);
  gtk_table_attach (GTK_TABLE (table_shorthand), label_blank, 2, 3, 0, 1,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_label_set_justify (GTK_LABEL (label_blank), GTK_JUSTIFY_LEFT);
  gtk_misc_set_alignment (GTK_MISC (label_blank), 0, 0.5);
  
  label_blank = gtk_label_new ("");
  gtk_widget_show (label_blank);
  gtk_table_attach (GTK_TABLE (table_shorthand), label_blank, 1, 2, 4, 5,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_label_set_justify (GTK_LABEL (label_blank), GTK_JUSTIFY_LEFT);
  gtk_misc_set_alignment (GTK_MISC (label_blank), 0, 0.5);
  
  label_blank = gtk_label_new ("");
  gtk_widget_show (label_blank);
  gtk_table_attach (GTK_TABLE (table_shorthand), label_blank, 2, 3, 4, 5,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_label_set_justify (GTK_LABEL (label_blank), GTK_JUSTIFY_LEFT);
  gtk_misc_set_alignment (GTK_MISC (label_blank), 0, 0.5);

  label_short_width = gtk_label_new (_("width"));
  gtk_widget_show (label_short_width);
  gtk_table_attach (GTK_TABLE (table_shorthand), label_short_width, 0, 1, 1, 2,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 10, 0);
  gtk_label_set_justify (GTK_LABEL (label_short_width), GTK_JUSTIFY_LEFT);
  gtk_misc_set_alignment (GTK_MISC (label_short_width), 0, 0.5);

  label_short_height = gtk_label_new (_("height"));
  gtk_widget_show (label_short_height);
  gtk_table_attach (GTK_TABLE (table_shorthand), label_short_height, 0, 1, 2, 3,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 10, 0);
  gtk_label_set_justify (GTK_LABEL (label_short_height), GTK_JUSTIFY_LEFT);
  gtk_misc_set_alignment (GTK_MISC (label_short_height), 0, 0.5);

  label_short_border = gtk_label_new (_("border"));
  gtk_widget_show (label_short_border);
  gtk_table_attach (GTK_TABLE (table_shorthand), label_short_border, 0, 1, 5, 6,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 10, 0);
  gtk_label_set_justify (GTK_LABEL (label_short_border), GTK_JUSTIFY_LEFT);
  gtk_misc_set_alignment (GTK_MISC (label_short_border), 0, 0.5);

  label_short_margin = gtk_label_new (_("margin"));
  gtk_widget_show (label_short_margin);
  gtk_table_attach (GTK_TABLE (table_shorthand), label_short_margin, 0, 1, 6, 7,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 10, 0);
  gtk_label_set_justify (GTK_LABEL (label_short_margin), GTK_JUSTIFY_LEFT);
  gtk_misc_set_alignment (GTK_MISC (label_short_margin), 0, 0.5);

  label_short_padding = gtk_label_new (_("padding"));
  gtk_widget_show (label_short_padding);
  gtk_table_attach (GTK_TABLE (table_shorthand), label_short_padding, 0, 1, 7, 8,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 10, 0);
  gtk_label_set_justify (GTK_LABEL (label_short_padding), GTK_JUSTIFY_LEFT);
  gtk_misc_set_alignment (GTK_MISC (label_short_padding), 0, 0.5);

  label_short_box_big_prop = gtk_label_new (_("<span size='large'>Box properties</span>"));
  gtk_widget_show (label_short_box_big_prop);
  gtk_table_attach (GTK_TABLE (table_shorthand), label_short_box_big_prop, 0, 1, 4, 5,
                    (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_label_set_use_markup (GTK_LABEL (label_short_box_big_prop), TRUE);
  gtk_label_set_justify (GTK_LABEL (label_short_box_big_prop), GTK_JUSTIFY_LEFT);
  gtk_misc_set_alignment (GTK_MISC (label_short_box_big_prop), 0, 0.5);
  
  label_blank = gtk_label_new ("");
  gtk_widget_show (label_blank);
  gtk_table_attach (GTK_TABLE (table_shorthand), label_blank, 0, 1, 3, 4,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_label_set_justify (GTK_LABEL (label_blank), GTK_JUSTIFY_LEFT);
  gtk_misc_set_alignment (GTK_MISC (label_blank), 0, 0.5);
  
  label_blank = gtk_label_new ("");
  gtk_widget_show (label_blank);
  gtk_table_attach (GTK_TABLE (table_shorthand), label_blank, 1, 2, 3, 4,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_label_set_justify (GTK_LABEL (label_blank), GTK_JUSTIFY_LEFT);
  gtk_misc_set_alignment (GTK_MISC (label_blank), 0, 0.5);
  
  label_blank = gtk_label_new ("");
  gtk_widget_show (label_blank);
  gtk_table_attach (GTK_TABLE (table_shorthand), label_blank, 2, 3, 3, 4,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_label_set_justify (GTK_LABEL (label_blank), GTK_JUSTIFY_LEFT);
  gtk_misc_set_alignment (GTK_MISC (label_blank), 0, 0.5);

  label_shorthand = gtk_label_new (_("Shorthand properties"));
  gtk_widget_show (label_shorthand);
  gtk_notebook_set_tab_label (GTK_NOTEBOOK (notebook), gtk_notebook_get_nth_page (GTK_NOTEBOOK (notebook), 1), label_shorthand);
  gtk_label_set_justify (GTK_LABEL (label_shorthand), GTK_JUSTIFY_LEFT);

  hbox_help = gtk_hbox_new (FALSE, 0);
  gtk_widget_show (hbox_help);
  gtk_container_add (GTK_CONTAINER (notebook), hbox_help);

  vbox_help = gtk_vbox_new (FALSE, 0);
  gtk_widget_show (vbox_help);
  gtk_box_pack_start (GTK_BOX (hbox_help), vbox_help, TRUE, TRUE, 0);

  scrolledwindow = gtk_scrolled_window_new (NULL, NULL);
  gtk_widget_show (scrolledwindow);
  gtk_box_pack_start (GTK_BOX (vbox_help), scrolledwindow, TRUE, TRUE, 0);

  textview = gtk_text_view_new ();
  gtk_widget_show (textview);
  gtk_container_add (GTK_CONTAINER (scrolledwindow), textview);
  gtk_container_set_border_width (GTK_CONTAINER (textview), 5);
  gtk_text_view_set_wrap_mode (GTK_TEXT_VIEW (textview), GTK_WRAP_WORD);
  gtk_text_buffer_set_text (gtk_text_view_get_buffer (GTK_TEXT_VIEW (textview)),
	_("From: http://www.w3.org/TR/CSS2/box.html\n\nEach box has a content area (e.g., text, an image, etc.) and optional surrounding padding, border, and margin areas; the size of each area is specified by properties defined below. The following diagram shows how these areas relate and the terminology used to refer to pieces of margin, border, and padding.\n\nThe perimeter of each of the four areas (content, padding, border, and margin) is called an \"edge\", so each box has four edges:\n\n- content edge or inner edge:\n    The content edge surrounds the element's rendered content. \n\n- padding edge:\n    The padding edge surrounds the box padding. If the padding has 0 width, the padding edge is the same as the content edge. The padding edge of a box defines the edges of the containing block established by the box. \n\n- border edge:\n    The border edge surrounds the box's border. If the border has 0 width, the border edge is the same as the padding edge. \n\n- margin edge or outer edge:\n    The margin edge surrounds the box margin. If the margin has 0 width, the margin edge is the same as the border edge. "), -1);

  drawingarea = gtk_drawing_area_new();
  gtk_widget_show (drawingarea);
  gtk_box_pack_start (GTK_BOX (vbox_help), drawingarea, TRUE, TRUE, 0);

  label_help = gtk_label_new (_("Quick help"));
  gtk_widget_show (label_help);
  gtk_notebook_set_tab_label (GTK_NOTEBOOK (notebook), gtk_notebook_get_nth_page (GTK_NOTEBOOK (notebook), 2), label_help);
  gtk_label_set_justify (GTK_LABEL (label_help), GTK_JUSTIFY_LEFT);

  dialog_action_area = GTK_DIALOG (box_wizard)->action_area;
  gtk_widget_show (dialog_action_area);
  gtk_button_box_set_layout (GTK_BUTTON_BOX (dialog_action_area), GTK_BUTTONBOX_END);

  cancelbutton = gtk_button_new_from_stock ("gtk-cancel");
  gtk_widget_show (cancelbutton);
  gtk_dialog_add_action_widget (GTK_DIALOG (box_wizard), cancelbutton, GTK_RESPONSE_CANCEL);
  GTK_WIDGET_SET_FLAGS (cancelbutton, GTK_CAN_DEFAULT);

  applybutton = gtk_button_new_from_stock ("gtk-apply");
  gtk_widget_show (applybutton);
  gtk_dialog_add_action_widget (GTK_DIALOG (box_wizard), applybutton, GTK_RESPONSE_APPLY);
  GTK_WIDGET_SET_FLAGS (applybutton, GTK_CAN_DEFAULT);

  okbutton = gtk_button_new_from_stock ("gtk-ok");
  gtk_widget_show (okbutton);
  gtk_dialog_add_action_widget (GTK_DIALOG (box_wizard), okbutton, GTK_RESPONSE_OK);
  GTK_WIDGET_SET_FLAGS (okbutton, GTK_CAN_DEFAULT);

//
  g_signal_connect ((gpointer) drawingarea, "expose_event",
                    G_CALLBACK (on_drawingarea_expose_event),
                    NULL);
/*
  g_signal_connect ((gpointer) box_wizard, "close",
                    G_CALLBACK (on_box_wizard_close),
                    box_wizard);
*/
  g_signal_connect ((gpointer) button_all_width, "clicked",
                    G_CALLBACK (on_button_all_width_clicked),
                    window);
  g_signal_connect ((gpointer) button_all_height, "clicked",
                    G_CALLBACK (on_button_all_height_clicked),
                    window);
  g_signal_connect ((gpointer) button_all_border_top, "clicked",
                    G_CALLBACK (on_button_all_border_top_clicked),
                    window);
  g_signal_connect ((gpointer) button_all_border_left, "clicked",
                    G_CALLBACK (on_button_all_border_left_clicked),
                    window);
  g_signal_connect ((gpointer) button_all_border_bottom, "clicked",
                    G_CALLBACK (on_button_all_border_bottom_clicked),
                    window);
  g_signal_connect ((gpointer) button_all_border_right, "clicked",
                    G_CALLBACK (on_button_all_border_right_clicked),
                    window);
  g_signal_connect ((gpointer) button_all_padding_top, "clicked",
                    G_CALLBACK (on_button_all_padding_top_clicked),
                    window);
  g_signal_connect ((gpointer) button_all_padding_left, "clicked",
                    G_CALLBACK (on_button_all_padding_left_clicked),
                    window);
  g_signal_connect ((gpointer) button_all_margin_right, "clicked",
                    G_CALLBACK (on_button_all_margin_right_clicked),
                    window);
  g_signal_connect ((gpointer) button_all_padding_bottom, "clicked",
                    G_CALLBACK (on_button_all_padding_bottom_clicked),
                    window);
  g_signal_connect ((gpointer) button_all_padding_right, "clicked",
                    G_CALLBACK (on_button_all_padding_right_clicked),
                    window);
  g_signal_connect ((gpointer) button_all_margin_bottom, "clicked",
                    G_CALLBACK (on_button_all_margin_bottom_clicked),
                    window);
  g_signal_connect ((gpointer) button_all_margin_top, "clicked",
                    G_CALLBACK (on_button_all_margin_top_clicked),
                    window);
  g_signal_connect ((gpointer) button_all_margin_left, "clicked",
                    G_CALLBACK (on_button_all_margin_left_clicked),
                    window);
  g_signal_connect ((gpointer) button_short_width, "clicked",
                    G_CALLBACK (on_button_short_width_clicked),
                    window);
  g_signal_connect ((gpointer) button_short_height, "clicked",
                    G_CALLBACK (on_button_short_height_clicked),
                    window);
  g_signal_connect ((gpointer) button_short_border, "clicked",
                    G_CALLBACK (on_button_short_border_clicked),
                    window);
  g_signal_connect ((gpointer) button_short_margin, "clicked",
                    G_CALLBACK (on_button_short_margin_clicked),
                    window);
  g_signal_connect ((gpointer) button_short_padding, "clicked",
                    G_CALLBACK (on_button_short_padding_clicked),
                    window);
  g_signal_connect ((gpointer) cancelbutton, "clicked",
                    G_CALLBACK (on_box_wizard_cancelbutton_clicked),
                    box_wizard);
  g_signal_connect ((gpointer) applybutton, "clicked",
                    G_CALLBACK (on_box_wizard_applybutton_clicked),
                    window);
  g_signal_connect ((gpointer) okbutton, "clicked",
                    G_CALLBACK (on_box_wizard_okbutton_clicked),
                    window);

  /* Store pointers to all widgets, for use by lookup_widget(). */
  GLADE_HOOKUP_OBJECT_NO_REF (box_wizard, box_wizard, 			"box_wizard");
  GLADE_HOOKUP_OBJECT (box_wizard, notebook, 					"notebook");
  GLADE_HOOKUP_OBJECT (box_wizard, entry_all_width, 			"entry_all_width");
  GLADE_HOOKUP_OBJECT (box_wizard, entry_all_height, 			"entry_all_height");
  GLADE_HOOKUP_OBJECT (box_wizard, entry_all_border_top, 		"entry_all_border_top");
  GLADE_HOOKUP_OBJECT (box_wizard, entry_all_border_left, 		"entry_all_border_left");
  GLADE_HOOKUP_OBJECT (box_wizard, entry_all_border_bottom, 	"entry_all_border_bottom");
  GLADE_HOOKUP_OBJECT (box_wizard, entry_all_border_right, 		"entry_all_border_right");
  GLADE_HOOKUP_OBJECT (box_wizard, entry_all_padding_top, 		"entry_all_padding_top");
  GLADE_HOOKUP_OBJECT (box_wizard, entry_all_padding_left, 		"entry_all_padding_left");
  GLADE_HOOKUP_OBJECT (box_wizard, entry_all_margin_right, 		"entry_all_margin_right");
  GLADE_HOOKUP_OBJECT (box_wizard, entry_all_padding_bottom, 	"entry_all_padding_bottom");
  GLADE_HOOKUP_OBJECT (box_wizard, entry_all_margin_bottom, 	"entry_all_margin_bottom");
  GLADE_HOOKUP_OBJECT (box_wizard, entry_all_padding_right, 	"entry_all_padding_right");
  GLADE_HOOKUP_OBJECT (box_wizard, entry_all_margin_top, 		"entry_all_margin_top");
  GLADE_HOOKUP_OBJECT (box_wizard, entry_all_margin_left, 		"entry_all_margin_left");
  GLADE_HOOKUP_OBJECT (box_wizard, entry_short_width, 			"entry_short_width");
  GLADE_HOOKUP_OBJECT (box_wizard, entry_short_height, 			"entry_short_height");
  GLADE_HOOKUP_OBJECT (box_wizard, entry_short_border, 			"entry_short_border");
  GLADE_HOOKUP_OBJECT (box_wizard, entry_short_margin, 			"entry_short_margin");
  GLADE_HOOKUP_OBJECT (box_wizard, entry_short_padding, 		"entry_short_padding");
  GLADE_HOOKUP_OBJECT (box_wizard, cancelbutton, 				"cancelbutton");
  GLADE_HOOKUP_OBJECT (box_wizard, applybutton, 				"applybutton");
  GLADE_HOOKUP_OBJECT (box_wizard, okbutton, 					"okbutton");

  return box_wizard;
}
