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

#include <libxml/parser.h>
#include <stdlib.h>
#include <string.h>
#include <gdk/gdkkeysyms.h>
#include <gtk/gtk.h>

#include "cssedwindow.h"
#include "document.h"
#include "utils.h"
#include "support.h"
#include "document-list.h"

typedef struct _CssedDocListData {
	GtkWidget *dialog;
	CssedWindow *window;
	GtkTreeView *treeview;
	gchar *filename;
} CssedDocListData;	

// used by the doclist xml parser
typedef struct _CssedDocListXmlData {
	CssedDocListData* data;
	gint depth;
} CssedDocListXmlData;

enum {
	COL_NAME,
	COL_FILE,
	COL_SAVED,
	COL_INDEX,
	NCOLS
};

// setup the SAX handler

CssedDocListXmlData*
cssed_doclist_xml_data_new(CssedDocListData* data)
{
	CssedDocListXmlData *xmldata;

	xmldata = g_malloc0(sizeof(CssedDocListXmlData));

	xmldata->data = data;
	xmldata->depth = 0;

	return xmldata;
}

void
cssed_doclist_xml_data_free( CssedDocListXmlData *xmldata )
{
	g_free(xmldata);
}

// load functions
void
load_doclist_on_store (CssedDocListData *data)
{
	CssedWindow *window;
	GtkTreeView *treeview;
	GtkListStore *store;
	CssedDoc *doc;
	GList *list;
	GtkTreeIter iter;
	gchar *file_name;
	gchar *file_path;
	gchar *file_index;
	gboolean saved;
	gint index = 0;
	
	window = data->window;
	treeview = data->treeview;
	store = GTK_LIST_STORE(gtk_tree_view_get_model(treeview));

	list = cssed_window_get_document_list_first(window);
	
	gtk_list_store_clear (store);
	while( list != NULL ){
		doc = (CssedDoc*) list->data;
		file_path = document_get_filename( doc );
		if( file_path == NULL ){
			file_path = g_strdup("");
			file_name = g_strdup(_("Untitled"));
			saved = FALSE;			
		}else{
			file_name = g_path_get_basename((const gchar*) file_path);	
			saved = !document_get_modified(doc);
		}		
		file_index = g_strdup_printf("%d", ++index);
		gtk_list_store_append (store, &iter);
		gtk_list_store_set (store, &iter,
							0, file_name,
							1, file_path,
							2, saved, 
							3, file_index,
							-1);
		g_free(file_name);
		g_free(file_path);
		g_free(file_index);
		list = g_list_next(list);
	}		
}

/* file loading */
static void
doclist_file_start_tag_cb (void* data, const xmlChar *el, const xmlChar **attr)
{
	CssedDocListXmlData *xmldata;
	GtkTreeIter iter;
	GtkListStore *store;
	GtkTreeView *treeview;	
	CssedWindow *window;
	gchar* filename;
	guint i;

	xmldata = (CssedDocListXmlData*) data;
	window = xmldata->data->window;
	treeview = xmldata->data->treeview;
	store = GTK_LIST_STORE(gtk_tree_view_get_model(treeview));

	switch(xmldata->depth){
		case 0:
			if( strcmp((const gchar*)el, "doclist") != 0 ){
				cssed_window_output_write( window, _("Doc list file is bogus, no doclist tag at level one"));
			}
		break;
		case 1:
			for (i = 0; attr[i]; i += 2)
			{
				if( strcmp((const gchar*)attr[i], "path")==0){
					filename = g_path_get_basename((const gchar*) attr[i + 1]);	
					gtk_list_store_append (store, &iter);
					gtk_list_store_set ( store, &iter,
										0, filename,
										1, attr[i + 1],
										2, TRUE, 
										3, "",
										-1);
					g_free( filename );
				}
			}
		break;
		default:
			cssed_window_output_write( window, _("Document list file is bogus, too much nested tags (max level 2)"));
		break;
	}
	++(xmldata->depth);
}

static void
doclist_file_end_tag_cb (void *data, const xmlChar *el)
{
	CssedDocListXmlData *xmldata;	
	xmldata = (CssedDocListXmlData*) data;
	--(xmldata->depth);
}

static void
doclist_xml_error(void *user_data, const char *msg, ...) {
    va_list args;
	CssedWindow *window;
	CssedDocListXmlData *data;
	gchar *xmlerror, *message;

	data = (CssedDocListXmlData*) user_data;
	window = CSSED_WINDOW(data->data->window);
	
    va_start(args, msg);
    xmlerror = g_strdup_vprintf(msg, args);
    va_end(args);

	g_strstrip(xmlerror);
	message = g_strdup_printf( "%s: %s", data->data->filename, xmlerror);
	cssed_window_output_write(window, message);
	g_free(message);
	g_free(xmlerror);
}

void
initialize_document_list_handler(xmlSAXHandler *handler)
{
    handler->internalSubset = NULL; 
    handler->isStandalone = NULL;
    handler->hasInternalSubset = NULL;
    handler->hasExternalSubset = NULL;
    handler->resolveEntity = NULL;
    handler->getEntity = NULL;
    handler->entityDecl = NULL;
    handler->notationDecl = NULL;
    handler->attributeDecl = NULL;
    handler->elementDecl = NULL;
    handler->unparsedEntityDecl = NULL;
    handler->setDocumentLocator = NULL;
    handler->startDocument = NULL;
    handler->endDocument = NULL;
    handler->startElement = doclist_file_start_tag_cb;
    handler->endElement = doclist_file_end_tag_cb;
    handler->reference = NULL;
    handler->characters = NULL;
    handler->ignorableWhitespace = NULL;
    handler->processingInstruction = NULL;
    handler->comment = NULL;
    handler->warning = NULL;
    handler->error = &doclist_xml_error;
    handler->fatalError = NULL; /* unused error() get all the errors */
    handler->getParameterEntity = NULL;
    handler->cdataBlock = NULL;
    handler->externalSubset = NULL;
    handler->initialized = 0;
    handler->_private = NULL;
    handler->startElementNs = NULL;
    handler->endElementNs = NULL;
    handler->serror = NULL;
}

void
load_doclist_from_file(CssedDocListData* data, gchar* filename, gboolean clean_store)
{
	GError* error = NULL;
	GtkTreeView *treeview;
	GtkListStore *store;
	gchar* buffer;
	gint retval;
	CssedDocListXmlData *xmldata;
	gchar* file_to_open;
	xmlSAXHandler handler;

	initialize_document_list_handler(&handler);
	treeview = data->treeview;
	store = GTK_LIST_STORE(gtk_tree_view_get_model(treeview));

	if( filename ) file_to_open = filename;
	else file_to_open = data->filename;

	if( g_file_test(file_to_open, G_FILE_TEST_EXISTS) )
	{
		if( g_file_get_contents(file_to_open, &buffer, NULL,  &error) ){
			xmldata = cssed_doclist_xml_data_new(data);
			if ( (retval = xmlSAXUserParseMemory(&handler, xmldata, buffer, strlen(buffer))) != 0) {
				//err = g_strdup_printf("The parser found error on the document list file\n%s\n\nError code: %d\nCheck the output window for details.", file_to_open, retval);
				cssed_error_message( _("Error parsing document list file"), "The parser found error on the document list file\n%s\n\nError code: %d\nCheck the output window for details.", file_to_open, retval);
				//g_free(err);
				return;
			} 
			cssed_doclist_xml_data_free(xmldata);
		}else{
			//err = g_strdup_printf(_("The system was unable to open the file\n%s\n\n%s"), file_to_open, error->message);
			cssed_error_message(_("Error reading file."), _("The system was unable to open the file\n%s\n\n%s"), file_to_open, error->message);
			g_error_free(error);
			//g_free(err);
		}
	}else{
		cssed_error_message( _("Error opening file"), _("The file does not exist\n%s"), file_to_open);
	}
}

// callbacks
void
on_doclist_treeview_row_activated       (GtkTreeView     *treeview,
                                        GtkTreePath     *path,
                                        GtkTreeViewColumn *column,
                                        gpointer         user_data)
{
	GtkTreeModel *model;
	GtkTreeIter iter;
	gchar *file;
	gint index;
	CssedWindow *window;
	CssedDocListData *data;

	data = (CssedDocListData*) user_data;
	window = data->window;

	model = gtk_tree_view_get_model (treeview);

	if (gtk_tree_model_get_iter (model, &iter, path))
	{
		gtk_tree_model_get (model, &iter, 1, &file, -1);	// columns 0,1,2
		if( strcmp("", file) == 0 ){
			g_free(file);
			gtk_tree_model_get (model, &iter, 3, &file, -1);
			index = atoi(file);
			gtk_notebook_set_current_page (
						GTK_NOTEBOOK(
							cssed_window_get_document_notebook(window)
						),
						--index);
			cssed_window_output_write(window, 
									_("<span color='brown'>document list: unnamed document, searching by index.</span>")
									);
		}else{
			if( !cssed_window_is_file_opened(window, file) )
				document_open_with_error_check(window, file);
		}
		g_free(file);
	}
}

// generic save function
gboolean
document_list_file_save (CssedDocListData *data, gchar *filename)
{
	CssedWindow *window;
	GtkWidget *dialog; 
	GtkListStore *store;
	GtkTreeView *treeview;
	GtkTreeIter iter;
	gchar *path;
	gboolean valid;
	gchar *strout;
	FILE* f;

	window = data->window;
	treeview = data->treeview;
	dialog = data->dialog;
	store = GTK_LIST_STORE(gtk_tree_view_get_model(treeview));

	// open the file
	f = fopen (filename, "wb");
	if (f == NULL)
	{
		cssed_error_message (_("Cannot open file"), _("Unable to open the file\n%s\nfor writing."), filename);
		return FALSE;
	}
	else
	{
		const gchar *xml_header = 	"<?xml version=\"1.0\" encoding=\"UTF-8\" ?>\n"
									"<!DOCTYPE doclist [\n"
									"<!ELEMENT doclist (document+)>\n"
									"<!ELEMENT document EMPTY>\n"
									"<!ATTLIST document path CDATA #REQUIRED>\n"
									"]>\n<doclist>\n";
		const gchar *xml_footer = 	"</doclist>\n";
		gchar* title;

		fwrite (xml_header, sizeof (gchar), strlen(xml_header), f);
		// pick all items from the store and write them down <document /> tag
		valid = gtk_tree_model_get_iter_first (GTK_TREE_MODEL(store), &iter);
		
		while (valid)
		{
			gtk_tree_model_get (GTK_TREE_MODEL(store), &iter, 1, &path, -1);
			if( path != NULL && (strlen(path) > 0) ){
				strout = g_strdup_printf("<document path=\"%s\" />\n",path);
				fwrite (strout, sizeof (gchar), strlen(strout), f);
				g_free (strout);
				g_free (path);
			}
			valid = gtk_tree_model_iter_next (GTK_TREE_MODEL(store), &iter);
		}

		fwrite (xml_footer, sizeof (gchar), strlen(xml_footer), f);	
		title = g_strdup_printf(_("Document list: %s"), filename);
		gtk_window_set_title (GTK_WINDOW (dialog), title);
		g_free(title);
		fclose(f);
		return TRUE;
	}
}

void
on_doclist_button_save_as_clicked( GtkButton       *button,
                                gpointer         user_data)
{
	CssedWindow *window;
	CssedDocListData *data;
	gchar *filename;
	gchar* path;

	data = (CssedDocListData*) user_data;
	window = data->window;
	path = data->filename;

	filename = cssed_prompt_for_file(window, _("Select file name to save"), path, CSSED_FILESELECTOR_SAVE);
	if( !filename ) return;

	if( document_list_file_save(data, filename) ){
		if( data->filename )
			g_free(data->filename);
			
		data->filename = filename;
	}
}

void
on_doclist_button_save_clicked( GtkButton       *button,
                                gpointer         user_data)
{
	CssedWindow *window;
	CssedDocListData *data;
	gchar *filename;
	gboolean new_filename = FALSE;

	data = (CssedDocListData*) user_data;
	window = data->window;
	filename = data->filename;

	if( !data->filename){
		filename = cssed_prompt_for_file(window,  _("Select file name to save"), data->filename, CSSED_FILESELECTOR_SAVE);
		if( !filename ) return;
		new_filename = TRUE;
	}

	if( document_list_file_save(data, filename) )
		if( new_filename )
			data->filename = filename;	
}

// loads the currently opened documents on the list
void
on_doclist_button_refresh_clicked     ( GtkButton       *button,
                                        gpointer         user_data)
{
	CssedDocListData *data;
	
	data = (CssedDocListData*) user_data;
	load_doclist_on_store (data);
}

void
on_doclist_button_add_file_clicked ( GtkButton       *button,
                                        gpointer         user_data)
{
	CssedDocListData *data;
	CssedWindow *window;
	GtkListStore *store;
	GtkTreeView *treeview;
	GtkTreeIter iter;
	gchar *filename;
	gchar *basename;

	
	data = (CssedDocListData*) user_data;
	treeview = data->treeview;
	window = data->window;
	store = GTK_LIST_STORE(gtk_tree_view_get_model(treeview));

	filename = cssed_prompt_for_file(window, _("Select file to add to list"),  NULL, CSSED_FILESELECTOR_OPEN); 
	
	if( !filename ) return;

	basename = g_path_get_basename((const gchar*) filename);	
	gtk_list_store_append (store, &iter);
	gtk_list_store_set ( store, &iter,
						0, basename,
						1, filename,
						2, TRUE, 
						3, "",
						-1);
	g_free(filename);
}

void
on_doclist_button_delete_selected_clicked ( GtkButton       *button,
                                        gpointer         user_data)
{
	GtkTreeView *treeview;
	GtkTreeModel* model;
	GtkTreeSelection *selection;
	GtkTreeIter iter;

	treeview = GTK_TREE_VIEW(user_data);
	selection = gtk_tree_view_get_selection(treeview);

	if( gtk_tree_selection_get_selected(selection, &model, &iter) )
		gtk_list_store_remove( GTK_LIST_STORE(model), &iter);
}

void
on_doclist_button_append_doclist_clicked ( GtkButton       *button,
                                        gpointer         user_data)
{
	CssedDocListData *data;
	GtkListStore *store;
	GtkTreeView *treeview;
	CssedWindow *window;
	gchar *filename;

	data = (CssedDocListData*) user_data;
	window = data->window;
	treeview = data->treeview;
	store = GTK_LIST_STORE(gtk_tree_view_get_model(treeview));

	filename = cssed_prompt_for_file(window, _("Select document list file"),  NULL, CSSED_FILESELECTOR_OPEN);

	if( filename ){
		load_doclist_from_file(data, filename, FALSE);	
		g_free(filename);
	}
}

void
on_doclist_button_revert_to_saved_clicked ( GtkButton       *button,
                                        gpointer         user_data)
{
	CssedDocListData *data;

	data = (CssedDocListData*) user_data;	

	if( data->filename ){
		load_doclist_from_file(data, NULL, TRUE);
	}else{
		cssed_error_message (_("This document list is not saved"), _("I'm unable to revert this document list\nbecause it have not been saved.\n\nTry the \"Save\" button instead" ));
	}
}


void
on_doclist_button_close_clicked        (GtkButton       *button,
                                        gpointer         user_data)
{
	gtk_widget_destroy( GTK_WIDGET(user_data) );
}


void        
on_doclist_response      (  GtkDialog *dialog,
							gint response,
							gpointer user_data )
{
	CssedDocListData *data;

	data = (CssedDocListData*) user_data;
	
	if( response == GTK_RESPONSE_CLOSE || 
		response == GTK_RESPONSE_DELETE_EVENT )
	{
		if( data->filename ) g_free(data->filename);
		g_free( user_data );
	}
}

#ifdef 	GTK_ATLEAST_2_4
 void        
on_checkbox_transient_toggled      (GtkToggleButton *togglebutton,
                                            gpointer user_data)
{
	CssedDocListData *data;
	CssedWindow *window;
	GtkWidget *window_widget;
	GtkWidget *dialog;
	gboolean transient;

	data = (CssedDocListData*) user_data;
	window = data->window;
	dialog = data->dialog;
	window_widget = cssed_window_get_window_widget(window);

	transient = gtk_toggle_button_get_active(togglebutton);
	//gtk_window_set_keep_above (GTK_WINDOW(dialog), transient);
	if( transient )
		gtk_window_set_transient_for (GTK_WINDOW(dialog),
									GTK_WINDOW(cssed_window_get_window_widget(window)));
	else
		gtk_window_set_transient_for  (GTK_WINDOW(dialog),NULL);
}
#endif /* GTK_ATLEAST_2_4 */

// drag and drop
gboolean
on_doclist_treeview_drag_motion(GtkWidget *widget,
								GdkDragContext *drag_context,
								gint x,
								gint y,
								guint time,
								gpointer user_data)

{
	GtkTreeView *treeview;
	GtkTreeModel *model;
	GtkTreeSelection *selection;
	GtkTreeIter iter;
	gchar *file;
	
	// don't allow Untitled docs to be dragged
	treeview = GTK_TREE_VIEW(user_data);	
	selection = gtk_tree_view_get_selection(treeview);
	if (gtk_tree_selection_get_selected(selection, &model, &iter))
	{
		gtk_tree_model_get (model, &iter, 1, &file, -1);
		if( strlen(file) == 0 )
			return FALSE;
		g_free(file);
	}

	return TRUE;
}

void
on_doclist_treeview_drag_data_get ( GtkWidget *widget,
									GdkDragContext *drag_context,
									GtkSelectionData *data,
									guint info,
									guint time,
									gpointer user_data)
{
	GtkTreeView *treeview;
	GtkTreeModel *model;
	GtkTreeSelection *selection;
	GtkTreeIter iter;
	gchar *file;

	treeview = GTK_TREE_VIEW(user_data);	
	selection = gtk_tree_view_get_selection(treeview);
	if (gtk_tree_selection_get_selected(selection, &model, &iter))
	{
		gtk_tree_model_get (model, &iter, 1, &file, -1);
		gtk_selection_data_set(
				data,
				GDK_SELECTION_TYPE_STRING,
				8,
				(const guchar*)file, 
				strlen(file)
			);
		g_free(file);
	}
}

void
on_doclist_treeview_drag_data_received (GtkWidget *widget,
										GdkDragContext *drag_context,
										gint x,
										gint y,
										GtkSelectionData *data,
										guint info,
										guint time,
										gpointer user_data)
{
	GtkTreeView *treeview;
	GtkTreeModel *model;
	GtkTreeIter iter;
	gchar *file;
	gchar *filename;
 	GtkWidget *source;

 	// dont't allow d'n'd on the same treeview
	source = gtk_drag_get_source_widget (drag_context);
	if( source == widget )	return;

	treeview = GTK_TREE_VIEW(user_data);	
	model = gtk_tree_view_get_model(treeview);

	file = (gchar*)data->data;

	// don't allow untitled or unnamed docs
	if( strlen(file) == 0 ) return;

	filename = g_path_get_basename((const gchar*) file);	
	gtk_list_store_append (GTK_LIST_STORE(model), &iter);
	gtk_list_store_set ( GTK_LIST_STORE(model), &iter,
							0, filename,
							1, file,
							2, TRUE, 
							3, "",
							-1);
	g_free( filename );
}

/* sorting function */
gint
document_list_tree_sort_column (GtkTreeModel *model,
								GtkTreeIter *a,
								GtkTreeIter *b,
								gpointer user_data)
{
	gint return_value;
	gboolean will_return = FALSE;
	gchar* first_string;
	gchar* second_string;
	gchar* cleaned_first;
	gchar* cleaned_second;
	gboolean bool_first;
	gboolean bool_second;
	gint index_first;
	gint index_second;
	gint col = GPOINTER_TO_INT(user_data);

	if( col != COL_SAVED ){
		gtk_tree_model_get(model, a, col, &first_string, -1);
		gtk_tree_model_get(model, b, col, &second_string, -1);
	
		if( (first_string==NULL) && (second_string==NULL) )
		{
			will_return = TRUE;
			return_value = 0;
		}
		else if( (first_string==NULL) && (second_string!=NULL) )
		{
			will_return = TRUE;
			return_value = 1;	
		}
		else if( (first_string!=NULL) && (second_string==NULL) )
		{
			will_return = TRUE;
			return_value = -1;
		}

		if( will_return ){
			g_free(first_string);
			g_free(second_string);			
			return return_value;
		}
		// just used to clean the markup
		pango_parse_markup (first_string, strlen( first_string ), 0,NULL,
							&cleaned_first,NULL, NULL);
		pango_parse_markup (second_string, strlen( second_string ), 0,NULL,
							&cleaned_second,NULL, NULL);
	
		g_free(first_string);
		g_free(second_string);
	}else{
		gtk_tree_model_get(model, a, COL_SAVED, &bool_first, -1);
		gtk_tree_model_get(model, b, COL_SAVED, &bool_second, -1);
	}


	switch( col ){
		case COL_NAME: // clean the number in the form <span>[111]</span> and convert to int
		case COL_FILE:
			return_value = strcmp(cleaned_first, cleaned_second);
			g_free(cleaned_first);
			g_free(cleaned_second);	
			return return_value;
		break;
		case COL_SAVED:
			if( bool_first == bool_second )
				return 0;
			else if( bool_first && !bool_second )
				return -1;
			else
				return 1;
		break;
		case COL_INDEX:
			index_first = atoi(cleaned_first);
			index_second = atoi(cleaned_second);			
			if ( index_first > index_second )
				return_value = 1;
			else if ( index_first < index_second )
				return_value =  -1;
			else
				return_value =  0;
			g_free(cleaned_first);
			g_free(cleaned_second);
			return return_value;
		break;

	}
	return 0;
}

/* generic short function when clicked column headers */
void
doclist_col_clicked(GtkTreeViewColumn *treeviewcolumn, GtkTreeView *treeview, gint colid )
{
	GtkSortType type;
	GtkTreeSortable *sortable;

	GList *cols=NULL, *list=NULL;
	
	cols = gtk_tree_view_get_columns (treeview);
	
	list = g_list_first(cols);
	while(list){
		if ( g_list_position (cols, list) != colid ){
			gtk_tree_view_column_set_sort_indicator(list->data, FALSE);
		}else{
			gtk_tree_view_column_set_sort_indicator(list->data, TRUE);
		}
		list = g_list_next(list);
	}

	type = gtk_tree_view_column_get_sort_order  (treeviewcolumn);
	sortable = GTK_TREE_SORTABLE(gtk_tree_view_get_model(treeview));
	gtk_tree_sortable_set_sort_column_id(sortable, colid, type==0?1:0);
	gtk_tree_view_column_set_sort_order  (treeviewcolumn, type==0?1:0);

	g_list_free(cols);
}

void
on_doclist_treeview_col_file_clicked      (GtkTreeViewColumn *treeviewcolumn,
                                            gpointer user_data)
{
	doclist_col_clicked(treeviewcolumn, GTK_TREE_VIEW(user_data),COL_FILE);
}

void
on_doclist_treeview_col_name_clicked      (GtkTreeViewColumn *treeviewcolumn,
                                            gpointer user_data)
{
	doclist_col_clicked(treeviewcolumn, GTK_TREE_VIEW(user_data),COL_NAME);
}

void
on_doclist_treeview_col_saved_clicked      (GtkTreeViewColumn *treeviewcolumn,
                                            gpointer user_data)
{
	doclist_col_clicked(treeviewcolumn, GTK_TREE_VIEW(user_data),COL_SAVED);
}

void
on_doclist_treeview_col_index_clicked      (GtkTreeViewColumn *treeviewcolumn,
                                            gpointer user_data)
{
	doclist_col_clicked(treeviewcolumn, GTK_TREE_VIEW(user_data), COL_INDEX);
}

// user interface, opens a doclist dialog. 
// gboolean filename if NULL it will load the current document_list
// if not null it will try to open a document list file
GtkWidget*
create_doclist_dialog (CssedWindow *window, CssedDocumentListType type, gchar* filename)
{
	GtkWidget *doclist_dialog;
	GtkWidget *dialog_vbox;
	GtkWidget *buttonbar_hbox;
	GtkWidget *scrolledwindow;
	GtkWidget *treeview;
	GtkWidget *dialog_action_area;
	GtkWidget *button_refresh;
	GtkWidget *button_close;
	GtkWidget *button_save;
	GtkWidget *button_save_as;
	GtkWidget *button_add_file;
	GtkWidget *button_delete_selected;
	GtkWidget *button_append_doclist;
	GtkWidget *button_revert_to_saved;
#ifdef GTK_ATLEAST_2_4
	GtkWidget *checkbox_transient;
#endif
	GtkListStore *store;
	GtkTreeViewColumn *col_file;
	GtkTreeViewColumn *col_name;
	GtkTreeViewColumn *col_saved;
	GtkTreeViewColumn *col_index;
	GtkCellRenderer *text_renderer;
	GtkCellRenderer *bool_renderer;
	CssedDocListData *data;

	static const GtkTargetEntry targets[] = {
		{ "STRING", 0, 0 },
		{ "text/plain", 0, 0 }
	};

	doclist_dialog = gtk_dialog_new ();

	if( filename ){
		gchar* title = g_strdup_printf(_("Document list: %s"), filename);
		gtk_window_set_title (GTK_WINDOW (doclist_dialog), title);
		g_free(title);
	}else{
		gtk_window_set_title (GTK_WINDOW (doclist_dialog), _("Document list"));
	}

	gtk_window_set_destroy_with_parent (GTK_WINDOW (doclist_dialog), TRUE);
	gtk_window_set_default_size (GTK_WINDOW (doclist_dialog), 650, 450);
	gtk_window_set_type_hint    (GTK_WINDOW (doclist_dialog), GDK_WINDOW_TYPE_HINT_NORMAL);
	gtk_window_set_position     (GTK_WINDOW (doclist_dialog), GTK_WIN_POS_CENTER);

	dialog_vbox = GTK_DIALOG (doclist_dialog)->vbox;
	gtk_widget_show (dialog_vbox);
	
	buttonbar_hbox = gtk_hbutton_box_new();
	gtk_widget_show (buttonbar_hbox);	
	gtk_box_pack_start (GTK_BOX (dialog_vbox), buttonbar_hbox, FALSE, FALSE, 5);
	gtk_button_box_set_layout (GTK_BUTTON_BOX (buttonbar_hbox), GTK_BUTTONBOX_START);

	scrolledwindow = gtk_scrolled_window_new (NULL, NULL);
	gtk_widget_show (scrolledwindow);
	gtk_box_pack_end (GTK_BOX (dialog_vbox), scrolledwindow, TRUE, TRUE, 0);
	
	// setup treeview
	treeview = gtk_tree_view_new ();
	gtk_widget_show (treeview);
	gtk_container_add (GTK_CONTAINER (scrolledwindow), treeview);
	
	store = gtk_list_store_new (NCOLS, 	G_TYPE_STRING, G_TYPE_STRING, G_TYPE_BOOLEAN, G_TYPE_STRING);
	gtk_tree_view_set_model (GTK_TREE_VIEW(treeview), GTK_TREE_MODEL(store));	
	g_object_unref(store);

	text_renderer = gtk_cell_renderer_text_new ();
	bool_renderer = gtk_cell_renderer_toggle_new ();

	
	col_name =
		gtk_tree_view_column_new_with_attributes (_("Name"),
							  text_renderer,
							  "markup", COL_NAME, NULL);
	col_file =
		gtk_tree_view_column_new_with_attributes (_("Path"),
							  text_renderer,
							  "markup", COL_FILE, NULL);							  
	col_saved =
		gtk_tree_view_column_new_with_attributes (_("Saved"),
							  bool_renderer,
							  "active", COL_SAVED, NULL);
	col_index =
		gtk_tree_view_column_new_with_attributes (_("Index"),
							  text_renderer,
							  "markup", COL_INDEX, NULL);	
				  
	gtk_tree_view_insert_column (GTK_TREE_VIEW (treeview), col_name, COL_NAME);
	gtk_tree_view_insert_column (GTK_TREE_VIEW (treeview), col_file, COL_FILE);
	gtk_tree_view_insert_column (GTK_TREE_VIEW (treeview), col_saved, COL_SAVED);
	gtk_tree_view_insert_column (GTK_TREE_VIEW (treeview), col_index, COL_INDEX);	
	
	gtk_tree_view_set_headers_visible (GTK_TREE_VIEW (treeview), TRUE);
	gtk_tree_view_set_headers_clickable(GTK_TREE_VIEW(treeview), TRUE);

	gtk_tree_sortable_set_sort_func(GTK_TREE_SORTABLE(store), COL_NAME, 
									document_list_tree_sort_column, GINT_TO_POINTER(COL_NAME), NULL);
	gtk_tree_sortable_set_sort_func(GTK_TREE_SORTABLE(store), COL_FILE,
									document_list_tree_sort_column, GINT_TO_POINTER(COL_FILE), NULL);	
	gtk_tree_sortable_set_sort_func(GTK_TREE_SORTABLE(store), COL_SAVED,
									document_list_tree_sort_column,	GINT_TO_POINTER(COL_SAVED), NULL);
	gtk_tree_sortable_set_sort_func(GTK_TREE_SORTABLE(store), COL_INDEX,
									document_list_tree_sort_column,	GINT_TO_POINTER(COL_INDEX), NULL);			
	// D'n'D
	gtk_drag_source_set (GTK_WIDGET (treeview),
			   GDK_BUTTON1_MASK,
			   targets,
			   G_N_ELEMENTS (targets),
			   GDK_ACTION_COPY | GDK_ACTION_MOVE);
	
	gtk_drag_dest_set (GTK_WIDGET (treeview),
			   GTK_DEST_DEFAULT_ALL,
			   targets,
			   G_N_ELEMENTS (targets),
			   GDK_ACTION_COPY | GDK_ACTION_MOVE);

#ifdef GTK_ATLEAST_2_4
	// checkbox
	checkbox_transient = gtk_check_button_new_with_mnemonic  ("_Always on top of main window");
	gtk_widget_show (checkbox_transient);
	gtk_box_pack_end (GTK_BOX (dialog_vbox), checkbox_transient, FALSE, FALSE, 5);
#endif

	dialog_action_area = GTK_DIALOG (doclist_dialog)->action_area;
	gtk_widget_show (dialog_action_area);
	gtk_button_box_set_layout (GTK_BUTTON_BOX (dialog_action_area), GTK_BUTTONBOX_END);
	gtk_box_set_homogeneous (GTK_BOX (dialog_action_area), FALSE);

// Those are the top bar buttons
	button_add_file = gtk_button_new_from_stock ("gtk-add");
	gtk_widget_show (button_add_file);
	gtk_box_pack_start (GTK_BOX (buttonbar_hbox), button_add_file, FALSE, FALSE, 0);
	GTK_WIDGET_SET_FLAGS (button_add_file, GTK_CAN_DEFAULT);

	button_delete_selected = gtk_button_new_from_stock ("gtk-delete");
	gtk_widget_show (button_delete_selected);
	gtk_box_pack_start (GTK_BOX (buttonbar_hbox), button_delete_selected, FALSE, FALSE, 0);
	GTK_WIDGET_SET_FLAGS (button_delete_selected, GTK_CAN_DEFAULT);

	button_append_doclist = create_custom_button_from_stock(_("Append list"), "gtk-index");
	gtk_widget_show (button_append_doclist);
	gtk_box_pack_start (GTK_BOX (buttonbar_hbox), button_append_doclist, FALSE, FALSE, 0);
	GTK_WIDGET_SET_FLAGS (button_append_doclist, GTK_CAN_DEFAULT);

	button_revert_to_saved= gtk_button_new_from_stock ("gtk-revert-to-saved");
	gtk_widget_show (button_revert_to_saved);
	gtk_box_pack_start (GTK_BOX (buttonbar_hbox), button_revert_to_saved, FALSE, FALSE, 0);
	GTK_WIDGET_SET_FLAGS (button_revert_to_saved, GTK_CAN_DEFAULT);
	gtk_button_box_set_child_secondary  (GTK_BUTTON_BOX(buttonbar_hbox), button_revert_to_saved, TRUE);

	button_save_as = gtk_button_new_from_stock ("gtk-save-as");
	gtk_widget_show (button_save_as);
	gtk_box_pack_start (GTK_BOX (buttonbar_hbox), button_save_as, FALSE, FALSE, 0);
	GTK_WIDGET_SET_FLAGS (button_save_as, GTK_CAN_DEFAULT);
	gtk_button_box_set_child_secondary  (GTK_BUTTON_BOX(buttonbar_hbox), button_save_as, TRUE);

	button_save = gtk_button_new_from_stock ("gtk-save");
	gtk_widget_show (button_save);
	gtk_box_pack_start (GTK_BOX (buttonbar_hbox), button_save, FALSE, FALSE, 0);
	GTK_WIDGET_SET_FLAGS (button_save, GTK_CAN_DEFAULT);
	gtk_button_box_set_child_secondary  (GTK_BUTTON_BOX(buttonbar_hbox), button_save, TRUE);

// Those are the action area buttons
	button_refresh = create_custom_button_from_stock(_("Reload Current"), "gtk-refresh");
	gtk_widget_show (button_refresh);
	gtk_dialog_add_action_widget (GTK_DIALOG (doclist_dialog), button_refresh, 0);
	GTK_WIDGET_SET_FLAGS (button_refresh, GTK_CAN_DEFAULT);

	button_close = gtk_button_new_from_stock ("gtk-close");
	gtk_widget_show (button_close);
	gtk_dialog_add_action_widget (GTK_DIALOG (doclist_dialog), button_close, GTK_RESPONSE_CLOSE);
	GTK_WIDGET_SET_FLAGS (button_close, GTK_CAN_DEFAULT);

	// allocate room for callbacks data
	data = g_malloc0(sizeof(CssedDocListData));
	data->dialog = doclist_dialog;
	data->window = window;	
	data->treeview = GTK_TREE_VIEW(treeview);
	
	// load initiall data
	switch( type ){
		case CSSED_DOCLIST_CURRENT:
			data->filename = NULL;
			load_doclist_on_store (data);
			break;

		case CSSED_DOCLIST_OPEN:
			if( filename ){
					data->filename = g_strdup(filename);
					load_doclist_from_file(data, NULL, TRUE);
				}else{
					data->filename = NULL;
					g_warning("Passed NULL filename to open document list");
				}
			break;
		case CSSED_DOCLIST_NEW:
				data->filename = NULL;
			break;
		default: // open current by default and warn
			data->filename = NULL;
			load_doclist_on_store (data);			
			g_warning("CssedDocumentListType out of range");
			break;
	}
	
	g_signal_connect (G_OBJECT (treeview), "row_activated",
			G_CALLBACK (on_doclist_treeview_row_activated),	data);
	g_signal_connect (G_OBJECT (button_refresh), "clicked",
			G_CALLBACK (on_doclist_button_refresh_clicked),	data);
	g_signal_connect (G_OBJECT (button_save), "clicked",
			G_CALLBACK (on_doclist_button_save_clicked), data);
	g_signal_connect (G_OBJECT (button_save_as), "clicked",
			G_CALLBACK (on_doclist_button_save_as_clicked), data);
	g_signal_connect (G_OBJECT (button_close), "clicked",
			G_CALLBACK (on_doclist_button_close_clicked), doclist_dialog);
	g_signal_connect (G_OBJECT (doclist_dialog), "response",
			G_CALLBACK (on_doclist_response), data);
#ifdef GTK_ATLEAST_2_4
	g_signal_connect (G_OBJECT (checkbox_transient), "toggled",
			G_CALLBACK (on_checkbox_transient_toggled), data);	
#endif
	g_signal_connect (G_OBJECT (button_add_file), "clicked",
			G_CALLBACK (on_doclist_button_add_file_clicked),	data);
	g_signal_connect (G_OBJECT (button_delete_selected), "clicked",
			G_CALLBACK (on_doclist_button_delete_selected_clicked), treeview);
	g_signal_connect (G_OBJECT (button_append_doclist), "clicked",
			G_CALLBACK (on_doclist_button_append_doclist_clicked), data);
	g_signal_connect (G_OBJECT (button_revert_to_saved), "clicked",
			G_CALLBACK (on_doclist_button_revert_to_saved_clicked), data);
	// D'n'D
	g_signal_connect (G_OBJECT (treeview), "drag_data_get",
			G_CALLBACK (on_doclist_treeview_drag_data_get), treeview);
	g_signal_connect (G_OBJECT (treeview), "drag_data_received",
			G_CALLBACK (on_doclist_treeview_drag_data_received), treeview);
	g_signal_connect (G_OBJECT (treeview), "drag_motion",
			G_CALLBACK (on_doclist_treeview_drag_motion), treeview);

	// sort on clicking col headers
	g_signal_connect (G_OBJECT (col_file), "clicked",
			G_CALLBACK (on_doclist_treeview_col_file_clicked), treeview);
	g_signal_connect (G_OBJECT (col_name), "clicked",
			G_CALLBACK (on_doclist_treeview_col_name_clicked), treeview);
	g_signal_connect (G_OBJECT (col_saved), "clicked",
			G_CALLBACK (on_doclist_treeview_col_saved_clicked), treeview);
	g_signal_connect (G_OBJECT (col_index), "clicked",
			G_CALLBACK (on_doclist_treeview_col_index_clicked), treeview);

	return doclist_dialog;
}




