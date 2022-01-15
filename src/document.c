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

#ifndef PLAT_GTK
#   define PLAT_GTK 1
#endif

// this is the line numbers width template
#define MARGIN_LINES_WIDTH_STRING "_99999"
// markers margin width in pixels
#define MARGIN_MARKERS_WIDTH_PIXELS 14
// folding margin width in pixels
#define MARGIN_FOLDING_WIDTH_PIXELS 12

// drag and drop on sheets' buttons
#ifndef WIN32
# define WITH_DRAGABLE_SHEETS
#endif

//#define WITH_SPLITTER_WINDOW
#define WITH_EXPERIMENTAL_CHANGE_STATE
#define WITH_RECENT_FILE_MANAGER

#include <gtk/gtk.h>

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <ctype.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>

#ifndef WIN32
# include <sys/mman.h>
# include <libgen.h>
# include <unistd.h>
#endif

#include <Scintilla.h>
#include <SciLexer.h>
#include <ScintillaWidget.h>

#include "cssedwindow.h"
#include "document.h"
#include "sciwrappers.h"
#include "file-type-manager.h"
#include "callbacks.h"
#include "cssdialogs-interface.h"
#include "interface.h"
#include "support.h"
#include "debug.h"
#include "cssdialogs-class.h"
#include "utils.h"
#include "xpm-icons.h"
#include <pixmaps/images.h>

#ifdef WITH_DRAGABLE_SHEETS
static const char *arrow_xpm[] = {
"18 22 2 1",
" 	c None",
".	c #FF0000",
"     ........     ",
"     ........     ",
"     ........     ",
"     ........     ",
"     ........     ",
"     ........     ",
"     ........     ",
"     ........     ",
"     ........     ",
"     ........     ",
"     ........     ",
"     ........     ",
"     ........     ",
"     ........     ",
"     ........     ",
"     ........     ",
" ................ ",
"  ..............  ",
"    ..........    ",
"     ........     ",
"       ....       ",
"        ..        "};
# endif

struct _CssedDoc
{
	CssedWindow* window;	// main window object
	GtkWidget* editor;		// scintilla editor
	gint scid;				// scintilla id
	GtkWidget* notebook; 	// main notebook where this doc is attached
	gchar* filename;		// full path to file, it is dynamically allocated so dispose mem when not needed
	CssedFileType *filetype;	// one of the supported filetypes
	gboolean modified; 		// modified flag, will be updated when the document changes (DEPRECATED)
	gboolean autocompletion;// set autocompletion mode for the document
	GtkWidget *container_box;
#ifdef WITH_SPLITTER_WINDOW
	GtkWidget *split_editor;
	GtkWidget *split_paned;
	CssedSplitType split_type;
	gpointer sci_document;
	GtkWidget *active_editor;
#endif
};

typedef struct _CssedDocLine {
	CssedDoc *doc;
	gint line;
} CssedDocLine;

/* some prototypes will be moved to sciwrappers.h and functions to sciwrappers.c */
void 				on_editor_notification		(GtkWidget* editor,gint scn, gpointer lscn, gpointer user_data);
int					get_new_sci_number ( void );
ScintillaObject*	sci_get_from_document		(CssedDoc* doc);

/*
	create an empty doc with a scintilla editor
*/

CssedDoc *
cssed_create_new_doc ( CssedWindow* window )
{
	GtkWidget* editor;
	ScintillaObject *sci;
	CssedDoc *newdoc;
	CssedConfig* cfg;
	CssedFileTypeManager* manager;
	gint sciid;
	
	newdoc = g_malloc0(sizeof (CssedDoc));
	cfg = cssed_window_get_config( window );
	manager = cssed_window_get_filetype_manager (window);

	editor = scintilla_new ();
	sci = SCINTILLA (editor);
	sciid = get_new_sci_number ();
	scintilla_set_id (sci, sciid);
#ifdef WITH_SPLITTER_WINDOW
	newdoc->sci_document = (gpointer) SSM(sci, SCI_GETDOCPOINTER, 0, 0);
	newdoc->split_type = CSSED_DOC_SPLIT_NONE;
#endif
	
	DBGMSG (__FILE__,__LINE__,"in list %d", cssed_window_get_document_list_length ( window ));

	g_signal_connect( editor, "sci-notify", G_CALLBACK(on_editor_notification), (gpointer) window );
	/* This is unused now but will be used in the future to track function key press events and others
	 * The callback func is in callbacks.c
	g_signal_connect( editor, "key-press-event", G_CALLBACK(on_editor_key_press_event), (gpointer) window );
	*/
	// for the pop menu
	g_signal_connect( editor, "button-press-event", G_CALLBACK(on_editor_button_press_event), (gpointer) window );
	// to delete values on the digest tree view - FIXME, should be part of a CssedFileType
	g_signal_connect( editor, "grab-focus" , G_CALLBACK(on_editor_grab_focus), (gpointer) window );

	gtk_widget_set_size_request (editor, 100, 100);
	newdoc->editor = editor;
	newdoc->scid = sciid;
	newdoc->notebook = cssed_window_get_document_notebook (window);
	newdoc->filename = NULL; 
	//newdoc->modified = FALSE;
	newdoc->autocompletion = cfg->autocompletion;
	newdoc->window = window;
	
	if( manager )
		newdoc->filetype = cssed_file_type_manager_get_filetype_from_id(manager, CSSED_FILETYPE_TEXT); 
	document_set_default_configuration( newdoc );
	document_set_undo_collection( newdoc, TRUE );
	document_start_undo_action(newdoc);
	document_set_font(newdoc);
	document_set_current_font_size(newdoc);

#ifdef WIN32
	document_set_eol_mode(newdoc, EOL_MODE_CRLF);
#else
	document_set_eol_mode(newdoc, EOL_MODE_LF);
#endif

	return newdoc;
}

gboolean
document_save(CssedDoc* doc)
{
	CssedWindow* window;
	GtkWidget *window_widget = NULL;
	gint doclen;
	FILE *f;
	gchar *filename = NULL;
	GtkWidget *notebook = NULL;
	GtkWidget *container;
	GtkWidget* new_label;
	gchar *doc_buffer;
	gint current_index;
	gchar* dir;
	CssedFileType *filetype;
	gchar* basename;
	GList* list;
	GtkWidget* hbox;
	size_t nwriten;	
	gchar* locale_filename = NULL;
	gchar* utf8_filename = NULL;
	G_CONST_RETURN char *charset;


	window = CSSED_WINDOW(doc->window);
	notebook = cssed_window_get_document_notebook(window);


	if (doc->filename == NULL)
	{
		filename = cssed_prompt_for_file (window, _("Select file name to save"), NULL, CSSED_FILESELECTOR_SAVE);
		if( filename == NULL ) return FALSE;
		f = fopen (filename, "wb");
		if( !g_get_charset(&charset) ){
			utf8_filename = g_filename_to_utf8(filename, -1, NULL, NULL, NULL);
		}else{
			utf8_filename = filename;
		}
	}else{
		if( !g_get_charset(&charset) ){
			locale_filename = g_filename_from_utf8(doc->filename, -1, NULL, NULL, NULL);
			f = fopen (locale_filename, "wb");
			g_free( locale_filename );
		}else{
			f = fopen (doc->filename, "wb");
		}
		
	}
	DBGMSG (__FILE__,__LINE__,"Trying to save a document called %s", doc->filename);
	if (f == NULL)
	{ 
		cssed_error_message ( _("Cannot open file"), (gchar*) g_strerror(errno) );
		return FALSE;
	}
	else
	{
		doclen = document_get_length (doc);
		doc_buffer = g_malloc0 (++doclen);
		document_get_text (doc, doclen, doc_buffer);
		nwriten = fwrite (doc_buffer, sizeof (gchar), strlen(doc_buffer), f);

		if ( nwriten != strlen(doc_buffer) )
		{
			cssed_error_message
				(_("Error while writing to file"),
				 _("There is a mismatch between the file size\nand the bytes written.\nThe file may be damaged.") );
		}

		//document_empty_undo_buffer(doc);
		document_set_save_point (doc);

		if (doc->filename == NULL)
		{	
			current_index =	gtk_notebook_get_current_page (GTK_NOTEBOOK(notebook));
			container = gtk_notebook_get_nth_page (GTK_NOTEBOOK (notebook), current_index);		
			hbox = gtk_notebook_get_tab_label(GTK_NOTEBOOK (notebook), container);
			list = gtk_container_get_children( GTK_CONTAINER(hbox) );
			list = g_list_first( list );
			new_label = GTK_WIDGET( list->data );
		
			if( new_label != NULL ){
				basename = g_path_get_basename (utf8_filename);
				gtk_label_set_text(GTK_LABEL(new_label),basename);
				g_free(basename);
			}	
			
			gtk_notebook_set_menu_label_text (GTK_NOTEBOOK (notebook), container, filename);	
			gtk_misc_set_alignment (GTK_MISC(gtk_notebook_get_menu_label (GTK_NOTEBOOK (notebook), container)), 0, 0.5);
			document_set_filename (doc, utf8_filename);		
			filetype = document_get_filetype_from_filename (doc, filename);
			document_set_filetype (doc, filetype);
			gtk_window_set_title (GTK_WINDOW(window_widget),utf8_filename);
		}

		dir = g_strconcat (g_dirname (doc->filename), G_DIR_SEPARATOR_S, NULL );
		cssed_window_set_last_save_dir (window, dir);

		if( utf8_filename && (utf8_filename != filename)) g_free (utf8_filename);
		if( filename ) g_free( filename );	
		g_free (dir);
		g_free (doc_buffer);
		fclose (f);
		return TRUE;
	}
}

gboolean
document_save_as(CssedDoc* doc, gchar* filename)
{
	CssedWindow *window;	// main window object
	GtkWidget *window_widget;
	GtkWidget *notebook;
	GtkWidget *container;
	GtkWidget* new_label;
	GtkWidget* dialog;
	gint response;
	gchar *doc_buffer;
	gint doclen;
	gint current_index;
	FILE *f;
	gchar* dir;
	CssedFileType *filetype;
	gchar* basename;
	GList* list;
	GtkWidget* hbox;
	gchar* utf8_filename = NULL;
	G_CONST_RETURN char *charset;
#ifdef WIN32
	int nwriten;
#else
	size_t nwriten;
#endif

	window = document_get_window(doc) ;
	window_widget = cssed_window_get_window_widget(window);
	notebook = cssed_window_get_document_notebook(window);

	if( g_file_test( filename , G_FILE_TEST_EXISTS) ){
		dialog = create_yes_no_cancel_dialog (_("File exists and will be overwritten"),
										  _("Do you want to overwrite the existing file with the current one?") );
		response = gtk_dialog_run(GTK_DIALOG(dialog));
		gtk_widget_destroy(dialog);
		if( response != GTK_RESPONSE_YES ){
			return FALSE;
		}
	}
	
	f = fopen (filename, "wb");
	if (f == NULL)
	{
		cssed_error_message (_("Cannot open file"), _("The file %s\ncannot be opened for writing."), filename);
		return FALSE;
	}
	else
	{
		doclen = document_get_length (doc);
		doc_buffer = g_malloc (doclen+1);
		document_get_text (doc, doclen+1, doc_buffer);
	
		nwriten = fwrite (doc_buffer, sizeof (gchar), doclen, f);
		if (nwriten != doclen)
		{
			cssed_error_message
			(
			_("Error while writing to file"),
			_("There is a mismatch between the file size\nand the bytes written.\nThe file may be damaged.")
			);
		}
		document_empty_undo_buffer(doc);
		document_set_save_point (doc);
		current_index =	gtk_notebook_get_current_page (GTK_NOTEBOOK(notebook));
		container = gtk_notebook_get_nth_page (GTK_NOTEBOOK (notebook), current_index);		
		hbox = gtk_notebook_get_tab_label(GTK_NOTEBOOK (notebook), container);
		list = gtk_container_get_children( GTK_CONTAINER(hbox) );
		list = g_list_first (list);
		new_label = GTK_WIDGET (list->data);

		if( !g_get_charset(&charset) ){
			utf8_filename = g_filename_to_utf8(filename, -1, NULL, NULL, NULL);
		}else{
			utf8_filename = filename;
		}
		if( new_label != NULL ){
			basename = g_path_get_basename  (utf8_filename);
			gtk_label_set_text(GTK_LABEL(new_label),basename);
			g_free(basename);
		}
	
		gtk_notebook_set_menu_label_text(GTK_NOTEBOOK (notebook), container, utf8_filename );
		gtk_misc_set_alignment (GTK_MISC(gtk_notebook_get_menu_label (GTK_NOTEBOOK(notebook), container)), 0, 0.5);		
		document_set_filename (doc, utf8_filename);		
		filetype = document_get_filetype_from_filename (doc, filename);
		
		if( filetype->id != CSSED_FILETYPE_UNKNOW ){
			document_set_filetype (doc, filetype);
		}
	
		gtk_window_set_title(GTK_WINDOW(window_widget),filename);
		dir = g_strconcat(g_dirname(filename), G_DIR_SEPARATOR_S, NULL);	
		cssed_window_set_last_save_dir( window, dir);		

		if( utf8_filename && (utf8_filename != filename) ) g_free(utf8_filename);
		g_free (dir);
		g_free (doc_buffer);
		fclose (f);
		return TRUE;
	}
}

/* Drag and drop callbacks, will be moved when tests ends. */
#ifdef WITH_DRAGABLE_SHEETS
void
on_document_button_drag_begin(GtkWidget *widget,
								GdkDragContext *drag_context,
								GtkSelectionData *data,
								guint info,
								guint time,
								gpointer user_data)
{
	GtkWidget *arrow, *image;
	GdkPixbuf *pixbuf;
	GdkBitmap *bitmap;
	
	arrow = gtk_window_new (GTK_WINDOW_POPUP);
	gtk_widget_realize (arrow);
	pixbuf = gdk_pixbuf_new_from_xpm_data (arrow_xpm);
	image = gtk_image_new_from_pixbuf (pixbuf);
	gtk_widget_show (image);
	gtk_container_add (GTK_CONTAINER (arrow), image);
	gdk_pixbuf_render_pixmap_and_mask (pixbuf, NULL, &bitmap, 128);
	g_object_unref (G_OBJECT (pixbuf));
	gtk_widget_shape_combine_mask (arrow, bitmap, 0, 0);
	gdk_bitmap_unref (bitmap);
	g_object_ref (G_OBJECT (arrow));
	gtk_object_sink (GTK_OBJECT (arrow));
	g_object_set_data (G_OBJECT (widget), "arrow", arrow);
}

void
on_document_button_drag_end(GtkWidget *widget,
							GdkDragContext *drag_context,
							gpointer user_data)
{
	GtkWidget *arrow;

	arrow = g_object_get_data (G_OBJECT (widget), "arrow");
	gtk_object_destroy (GTK_OBJECT (arrow));
	g_object_set_data (G_OBJECT (widget), "arrow", NULL);
}

void
on_document_button_drag_data_get   (GtkWidget *widget,
									GdkDragContext *drag_context,
									GtkSelectionData *data,
									guint info,
									guint time,
									gpointer user_data)
{
	CssedDoc* doc;
	CssedWindow* win;
	gint index;

	doc = (CssedDoc*) user_data;
	win = document_get_window( doc );

	index = cssed_window_get_document_list_index(win, doc);

	gtk_selection_data_set(
			data,
			GDK_SELECTION_TYPE_STRING,
			sizeof (gint),
			(const guchar*) &index, 
			sizeof(index)
		);
}

void
on_document_button_drag_data_received  (GtkWidget *widget,
										GdkDragContext *drag_context,
										gint x,
										gint y,
										GtkSelectionData *data,
										guint info,
										guint time,
										gpointer user_data)
{
	GtkWidget* source;
	CssedDoc* doc, *src_doc;
	CssedWindow* window;
	guint n, i;
 
	doc = (CssedDoc*) user_data;
	window = document_get_window( doc );

	source = gtk_drag_get_source_widget (drag_context);
	src_doc = (CssedDoc*) g_object_get_data(G_OBJECT(source), "sheet_button_doc");
	n = cssed_window_get_document_list_index(window, doc);

	if( n == -1 ) return;
	i = (int)  *(data->data);
	if( i == -1 ) return;
	if(i == n) return;

	cssed_window_set_document_list_index(window, src_doc, n);
}

gboolean
on_document_button_drag_motion (GtkWidget *widget,
								GdkDragContext *drag_context,
								gint x,
								gint y,
								guint time,
								gpointer user_data)
{
	GtkWidget *w_source, *arrow, *window;
	gint *source_is_sheet_button, *dest_is_sheet_button, root_x, root_y, pos_x, pos_y;

	w_source = gtk_drag_get_source_widget (drag_context);
	source_is_sheet_button = g_object_get_data( G_OBJECT(w_source), "cssed_sheet_button");
	dest_is_sheet_button = g_object_get_data( G_OBJECT(widget), "cssed_sheet_button");
	if (!source_is_sheet_button ||!dest_is_sheet_button)
		return FALSE;

	// Move the arrow to the correct position and show it. 
	arrow = g_object_get_data (G_OBJECT (w_source), "arrow");
	window = gtk_widget_get_ancestor (widget, GTK_TYPE_WINDOW);
	gtk_window_get_position (GTK_WINDOW (window), &root_x, &root_y);
	pos_x = root_x + widget->allocation.x;
	pos_y = root_y + widget->allocation.y;
	gtk_window_move (GTK_WINDOW (arrow), pos_x, pos_y);
	gtk_widget_show (arrow);

	return TRUE;
}

// to free button's data
void
on_document_button_destroy     (GtkObject *object,
                                gpointer user_data)
{
	g_free( user_data );
}

#endif // WITH_DRAGABLE_SHEETS

/*  create an empty doc with an scintilla editor and
	attach to the notebook passed as argument, puts the
	string title on notebook label. FIXME must be window func
*/
CssedDoc*
create_and_attach_new_doc (CssedWindow* window , const gchar * title)
{
	CssedDoc *newdoc;
	GtkWidget *label;
	GtkWidget *notebook;
	// testing here
	GtkWidget *hbox;
	GtkWidget *close_button;
	GtkWidget *icon;
	GtkWidget *label_menu;
	
#ifdef WITH_DRAGABLE_SHEETS
	static const GtkTargetEntry targets[] = {
		{ "CSSED_SHEET", 0, 0 }
	}; 
	gint* data_value = g_new0(gint,1);
#endif // WITH_DRAGABLE_SHEETS

	newdoc = cssed_create_new_doc (window);
	if (newdoc == NULL)
	{ /// FIXME say something to user
		DBGMSG (__FILE__,__LINE__,"passed null doc !!!");
	}
	else
	{
		DBGMSG (__FILE__,__LINE__,"doc passed %p", newdoc);
		DBGMSG (__FILE__,__LINE__,"in list %d", cssed_window_get_document_list_length ( window ));
		//document_set_filetype( newdoc, CSSED_FILETYPE_TEXT );
		
		document_set_codepage( newdoc, CSSED_CODEPAGE_UTF8 );// empty docs are UTF8 by default

		cssed_window_add_document_to_list (window, newdoc);
		gtk_widget_show (newdoc->editor);

		hbox = gtk_hbox_new (FALSE, 0);
		gtk_widget_show (hbox);
		close_button = gtk_button_new();
		gtk_widget_show (close_button);
		icon = create_pixmap_from_data (doc_close);
		gtk_widget_show (icon);
		gtk_container_add (GTK_CONTAINER(close_button), icon);

		label = gtk_label_new (title);
		gtk_widget_show (label);
		label_menu = gtk_label_new (title);
		gtk_widget_show (label_menu);
		gtk_misc_set_alignment (GTK_MISC(label_menu), 0, 0.5);
		
#ifdef WITH_DRAGABLE_SHEETS
		*data_value = 1;
		
		g_object_set_data (G_OBJECT(close_button), "cssed_sheet_button", GINT_TO_POINTER(1)); //(gpointer) data_value);
		g_object_set_data (G_OBJECT(close_button), "sheet_button_doc", (gpointer) newdoc);
		
		gtk_drag_source_set (GTK_WIDGET (close_button),
				   GDK_BUTTON1_MASK,
				   targets,
				   G_N_ELEMENTS (targets),
				   GDK_ACTION_MOVE);
		
		gtk_drag_dest_set (GTK_WIDGET (close_button),
				   GTK_DEST_DEFAULT_ALL,
				   targets,
				   G_N_ELEMENTS (targets),
				   GDK_ACTION_MOVE);
		
		g_signal_connect(G_OBJECT(close_button),
						 "destroy",
						 G_CALLBACK(on_document_button_destroy),
						 data_value	);	
						 	
		g_signal_connect (G_OBJECT(close_button), "drag_begin",
				G_CALLBACK (on_document_button_drag_begin), newdoc);
		g_signal_connect (G_OBJECT(close_button), "drag_end",
				G_CALLBACK (on_document_button_drag_end), newdoc);
		g_signal_connect (G_OBJECT(close_button), "drag_data_get",
				G_CALLBACK (on_document_button_drag_data_get), newdoc);
		g_signal_connect (G_OBJECT(close_button), "drag_data_received",
				G_CALLBACK (on_document_button_drag_data_received), newdoc);
		g_signal_connect (G_OBJECT(close_button), "drag_motion",
				G_CALLBACK (on_document_button_drag_motion), newdoc);
#endif // WITH_DRAGABLE_SHEETS

		gtk_box_pack_start  (GTK_BOX(hbox), label,FALSE,FALSE,0);
		gtk_box_pack_end    (GTK_BOX(hbox), close_button,FALSE,FALSE,0);
		g_signal_connect_swapped ((gpointer) close_button, "clicked",  G_CALLBACK (document_close), newdoc);
		notebook = cssed_window_get_document_notebook (window);
		
		newdoc->container_box = gtk_hbox_new(TRUE,0);
		gtk_widget_show(newdoc->container_box);

		gtk_box_pack_start_defaults (GTK_BOX(newdoc->container_box), newdoc->editor);
		gtk_notebook_append_page_menu ( GTK_NOTEBOOK( notebook ), newdoc->container_box, hbox, label_menu);
		gtk_notebook_set_current_page (GTK_NOTEBOOK( notebook ), -1);
		cssed_window_disable_undo (window);
		cssed_window_disable_redo (window);
		gtk_widget_queue_draw (notebook);
	}
	return newdoc;
}

///// new open functions /////////////////////////////////////////////
void
document_open_with_error_check(CssedWindow* window , gchar *filename)
{
	GError *error = NULL;
	CssedDoc *doc;

	if( cssed_window_is_file_opened(window, filename) ) return;	
	doc = document_open(window, filename, &error);
	if( doc == NULL )
		cssed_show_message_from_error (error, _("Cannot open file"), TRUE);
}

CssedDoc*
document_open (CssedWindow* window , gchar *filename, GError **err)
{
	CssedDoc *newdoc;
	CssedDoc *curdoc;
	GtkWidget *label;
	GtkWidget *notebook;
	struct stat ststat;
	gint _retval;
	gchar *block_end;
	gchar *basename;
	gchar *curdocname;
	gboolean needs_new;
	gboolean is_empty;
	GtkWidget *window_widget;
	gint current_index;
	GtkWidget *container;
	gchar *dir;
	CssedEolMode mode;
	GList *list;
	GtkWidget *hbox;
	gchar* utf8_filename = NULL;
	gchar* locale_filename = NULL;
	G_CONST_RETURN char *charset;
#ifdef WIN32
	gchar *map;
#else
	gint filedes;
	void *map = NULL;
#endif
#ifdef WITH_RECENT_FILE_MANAGER
	GtkRecentManager *manager;
	GtkRecentData *recent_data;
	gchar *recent_file_uri = NULL;
#endif

	if( !g_get_charset(&charset) ){
		if( !g_utf8_validate (filename, -1, NULL) ){
			utf8_filename = g_filename_to_utf8 (filename, -1, NULL, NULL, NULL);
			locale_filename = filename;
		}else{
			utf8_filename = filename;
			locale_filename = g_filename_from_utf8 (filename, -1, NULL, NULL, NULL);
		}
	}else{
		utf8_filename = locale_filename = filename;
	}
	
	if( cssed_window_is_file_opened(window, utf8_filename) ){
		g_set_error (err,
			   CSSED_ERROR_DOMAIN,                    
			   CSSED_ERROR_FILE_IS_OPENED,            
			   _("The file is already opened.")
		);
		return NULL;
	}

	notebook = cssed_window_get_document_notebook( window );

	DBGMSG (__FILE__,__LINE__,"Opening document, filename: %s", filename);
	if (!g_file_test (locale_filename, G_FILE_TEST_EXISTS)) 
	{
		g_set_error (err,
			CSSED_ERROR_DOMAIN,                 					
			CSSED_FILE_ERROR_NOENT,		
			_("File does not exist: %s"), 			
			utf8_filename);
		return NULL;
	}
	if (!g_file_test (locale_filename, G_FILE_TEST_IS_REGULAR)) 
	{
		g_set_error (err,
			CSSED_ERROR_DOMAIN,                 					
			CSSED_ERROR_FILE_NOT_REGULAR,		
			_("File is not a regular file: %s"), 			
			utf8_filename);
		return NULL;
	}
	_retval = stat (locale_filename, &ststat);
	if (_retval != 0)
	{
		g_set_error (err,
			CSSED_ERROR_DOMAIN,                 			
			(CssedError) g_file_error_from_errno (errno),	
			_("Cannot get information about file: %s"), 	
			g_strerror (errno));
		return NULL;
	}
	
#ifdef WIN32
	if( !g_file_get_contents (locale_filename, &map, NULL,  err) ) return NULL; // error set by g_file_get_contents

	is_empty = FALSE;
#else
	if ((filedes = open (locale_filename, O_RDONLY)) < 0)
	{
		g_set_error (err,
			CSSED_ERROR_DOMAIN,                 			
			(CssedError) g_file_error_from_errno (errno),	
			_("%s"), 			
			g_strerror (errno));
		return NULL;
	}

	if( ststat.st_size == 0 ){
		cssed_window_output_write(window, _("open file: the file '%s' is empty"), utf8_filename);
		is_empty = TRUE;
	}else{	
		if ((map =
			 mmap (0, ststat.st_size, PROT_READ, MAP_SHARED, filedes, 0)) == MAP_FAILED)
		{
			g_set_error (err,
				CSSED_ERROR_DOMAIN,                 					
				(CssedError) g_file_error_from_errno (errno),		
				_("Cannot map file to memory: %s"), 			
				g_strerror (errno));
			return NULL;
		}
		is_empty = FALSE;
	}
#endif

	block_end = map + ststat.st_size;	     // end of memory block
	curdoc = document_get_current(window);
	curdocname = document_get_filename(curdoc);

	if ( curdocname == NULL && document_get_length(curdoc) == 0){
		needs_new = FALSE;
	}else{
		needs_new = TRUE;
		g_free(curdocname);
	}

	basename = g_path_get_basename(utf8_filename);

	if( needs_new )
		newdoc = create_and_attach_new_doc (window, basename);
	else
		newdoc = curdoc;

	if (newdoc == NULL) // sanity check
	{ // 
		g_warning("%s:%d - NULL document passed.", __FILE__, __LINE__);
		g_set_error (err,
			CSSED_ERROR_DOC_IS_NULL,                 					
			(CssedError) g_file_error_from_errno (errno),		
			_("Internal error detected: the system may be running out of memory and could become unstable.\nUnable to create new document window.\n\nPlease save your work and close the application.\nIf you could report this error at http://cssed.sf.net it would be really appreciated."));
#ifndef WIN32
		close (filedes);
		munmap(map, ststat.st_size);
#else
		g_free(map);
#endif
		g_free (basename);
		
		return NULL;
	}
	else
	{
		DBGMSG (__FILE__,__LINE__,"Doc passed %p, index in list %d", newdoc, cssed_window_get_document_list_length(window));

#ifndef WIN32
		if( !is_empty )	document_add_text_buffer(newdoc, map, strlen(map));
#else
		if( !is_empty )	document_add_text_buffer(newdoc, map, ststat.st_size);
#endif

		// get and set eol mode
		if( map ){
			mode = buffer_get_first_eol(map, block_end);
			document_set_eol_mode(newdoc, mode);
		}else{
#ifndef WIN32                
			document_set_eol_mode(newdoc, EOL_MODE_LF ); // default Unix/Linux/OsX
#else          
			document_set_eol_mode(newdoc, EOL_MODE_CRLF ); // default Win32       
#endif		
		}
		
		document_set_filetype(newdoc, document_get_filetype_from_filename(newdoc, filename));

		if( needs_new ){
			newdoc->notebook = notebook;
			newdoc->filename = g_strdup(utf8_filename);
		}else{
			newdoc->filename = g_strdup(utf8_filename);
		}

		dir = g_strconcat( g_dirname( locale_filename ), G_DIR_SEPARATOR_S, NULL );
		
		if( !is_empty ){
			if( g_utf8_validate(map, ststat.st_size-1, NULL) )
				document_set_codepage( newdoc, CSSED_CODEPAGE_UTF8 );
			else
				document_set_codepage( newdoc, CSSED_CODEPAGE_DEFAULT );
		}else{
			document_set_codepage( newdoc, CSSED_CODEPAGE_UTF8 );
		}
		
		current_index =	gtk_notebook_get_current_page (GTK_NOTEBOOK(notebook));
		container = gtk_notebook_get_nth_page (GTK_NOTEBOOK (notebook), current_index);

		hbox = gtk_notebook_get_tab_label (GTK_NOTEBOOK (notebook), container);
		list = gtk_container_get_children (GTK_CONTAINER(hbox));
		list = g_list_first (list);
		label = GTK_WIDGET (list->data);

		gtk_label_set_text (GTK_LABEL(label), basename);
		gtk_notebook_set_menu_label_text (GTK_NOTEBOOK (notebook), container, utf8_filename );
		gtk_misc_set_alignment (GTK_MISC(gtk_notebook_get_menu_label (GTK_NOTEBOOK (notebook), container)), 0, 0.5);

		window_widget = cssed_window_get_window_widget (window);
		gtk_window_set_title (GTK_WINDOW(window_widget), utf8_filename);
		document_colourise_all (newdoc);
		document_empty_undo_buffer (newdoc);
		document_set_save_point (newdoc);		

		cssed_window_set_last_open_dir (window, dir);
		cssed_window_disable_save (window);
		cssed_window_disable_undo (window);
		cssed_window_disable_redo (window);
		document_set_current_position (newdoc, 0);
		g_free(dir);
	}
#ifndef WIN32
	close (filedes);
	if( !is_empty ) munmap (map, ststat.st_size);
#else
	g_free(map);
#endif
  
#ifdef WITH_RECENT_FILE_MANAGER
	recent_data = g_new0 (GtkRecentData, 1);
	recent_data->display_name = basename;
	recent_data->mime_type = "text/plain";
	recent_data->app_name = "cssed";
	recent_data->app_exec = g_strjoin (" ", g_get_prgname (), "%f", NULL);
	manager = gtk_recent_manager_get_default ();	

	recent_file_uri = g_filename_to_uri ((const gchar*)utf8_filename, NULL, NULL);
	
	if( !gtk_recent_manager_add_full (manager, recent_file_uri, recent_data) )
		g_warning(_("Unable to add file to recent files' registry"));

	g_free (recent_file_uri);
	g_free (recent_data->app_exec);
	g_free (recent_data);
#endif

	if( basename ) g_free(basename);		
	if( utf8_filename != filename ) g_free (utf8_filename);
	if( locale_filename != filename ) g_free (locale_filename);
	
	return newdoc;
}

gboolean
document_add_file (CssedDoc* doc , gchar *filename, GError **err)
{
	struct stat ststat;
	gint _retval;
	gchar *errormsg;
#ifdef WIN32
	gchar *map;
#else
	gint filedes;
	void *map;
#endif

	DBGMSG (__FILE__,__LINE__,"Opening document, filename: %s", filename);
	if (!g_file_test (filename, G_FILE_TEST_EXISTS)) 
	{
		g_set_error (err,
			CSSED_ERROR_DOMAIN,                 					
			CSSED_FILE_ERROR_NOENT,		
			_("File does not exist: %s"), 			
			filename);
		return FALSE;
	}
	if (!g_file_test (filename, G_FILE_TEST_IS_REGULAR)) 
	{
		g_set_error (err,
			CSSED_ERROR_DOMAIN,                 					
			CSSED_ERROR_FILE_NOT_REGULAR,		
			_("File is not a regular file: %s"), 			
			filename);
		return FALSE;
	}
	_retval = stat (filename, &ststat);
	if (_retval != 0)
	{
		g_set_error (err,
			CSSED_ERROR_DOMAIN,                 					
			(CssedError) g_file_error_from_errno (errno),		
			_("Cannot get information about file: %s"), 			
			g_strerror (errno));
		return FALSE;
	}
	
	if( ststat.st_size == 0 ){
		errormsg = g_strdup_printf(_("open file: the file '%s' is empty"), filename);
		document_write_error_to_program_output(doc, errormsg, ERROR_COLOR_STRING);
		g_free(errormsg);
		return TRUE; // as the document is empty and we've added nothing, we've added doc's content.
	}
	
#ifdef WIN32
	if( !g_file_get_contents (filename, &map, NULL,  err) ){
		// error is set by g_file_get_contents
		return FALSE;
	}
#else
	if ((filedes = open (filename, O_RDONLY)) < 0)
	{
		g_set_error (err,
			CSSED_ERROR_DOMAIN,                 					
			(CssedError) g_file_error_from_errno (errno),		
			_("%s"), 			
			g_strerror (errno));
		return FALSE;
	}
	if ((map =
	     mmap (0, ststat.st_size, PROT_READ, MAP_SHARED, filedes, 0)) == MAP_FAILED)
	{
		g_set_error (err,
			CSSED_ERROR_DOMAIN,                 					
			(CssedError) g_file_error_from_errno (errno),		
			_("Cannot map file to memory: %s"), 			
			g_strerror (errno));
		return FALSE;
	}
#endif

#ifndef WIN32
	document_add_text_buffer(doc, map, strlen(map));
#else
	document_add_text_buffer(doc, map, ststat.st_size);
#endif

#ifndef WIN32
	close (filedes);
	munmap(map, ststat.st_size);
#else
	g_free(map);
#endif		
	return TRUE;
}

/*  Get diferent - incrementally - integer as sci ids
	so, right now ,we are limited to 65535 docs opened 
	in a single editing session. 
	Even if you closed them. FIXME 
*/
int
get_new_sci_number ()
{
	static unsigned int count = 0;
	return ++count;	// id 0 created on startup
}

/* close doc - check if it's saved and ask to user if not */
void
document_close(CssedDoc* doc)
{
	GtkWidget* dialog;
	gint response;

	DBGMSG (__FILE__,__LINE__, "Closing document: Will close %p", doc );

	if ( document_get_modified(doc) )
	{
		dialog = create_yes_no_cancel_dialog(
				_("File is not saved, do you want to save it?"),
				_("The file has changed and the changes will be lost if you close it. Click \"Yes\" to save the file before closing it. ")
				);
		response = gtk_dialog_run(GTK_DIALOG(dialog));
		gtk_widget_destroy(GTK_WIDGET(dialog));
		switch(response)
		{
			case GTK_RESPONSE_NO:
				document_dispose (doc);
			break;
			case GTK_RESPONSE_YES:
				if( document_save(doc) )
					document_dispose(doc);
			break;
		}
	}else{
		document_dispose (doc);
	}
}

/* validator arrow */
void
document_set_validator_arrow_at_line( CssedDoc* doc, gint line )
{
	ScintillaObject * sci;
	sci =  sci_get_from_document( doc );

	document_clear_validator_arrow(doc);
	SSM(sci, SCI_MARKERADD, --line, 1);
	document_ensure_line_is_visible(doc, line);
	document_set_current_line (doc, line);
}

gboolean    
cssed_validator_arrow_on_idle_func (gpointer user_data)
{
	CssedDocLine* data = (CssedDocLine*) user_data;
	document_set_validator_arrow_at_line(data->doc, data->line);
	return FALSE;
}	

void
document_set_validator_arrow_at_line_on_idle ( CssedDoc* doc, gint line )
{
	CssedDocLine *data;
	ScintillaObject * sci;

	sci =  sci_get_from_document( doc );
	data = g_malloc0(sizeof(CssedDocLine));
	data->doc = doc;
	data->line = line;				
	g_idle_add_full (G_PRIORITY_DEFAULT_IDLE,
                     cssed_validator_arrow_on_idle_func,
                     data, g_free);  
}

void
document_clear_validator_arrow( CssedDoc* doc )
{
	ScintillaObject * sci;
	sci =  sci_get_from_document( doc );
	SSM(sci, SCI_MARKERDELETEALL, 1, 0);
}
/* scroll to the caret
	when you adds text, to the scintilla control, if the
	edge bound of text is down the editor window, no automatic
	scrolling is performed so you must call this func
	to scroll to the caret before text insertion.
*/
void
document_scroll_to_caret( CssedDoc* doc )
{
	ScintillaObject * sci;
	sci = SCINTILLA( doc->editor );
	SSM (sci, SCI_MOVECARETINSIDEVIEW, 0, 0);
}
/* recolourise the document from start to end */
void
document_colourise(CssedDoc* doc, gint start, gint end)
{
	ScintillaObject * sci;
	sci = (ScintillaObject*) sci_get_from_document(doc);
	sci_colourise(sci, start, end);
}
void
document_colourise_all(CssedDoc* doc)
{
	gint end;
#ifdef WITH_SPLITTER_WINDOW
	ScintillaObject * sci;
#endif

	end = document_get_length(doc);
	
#ifdef WITH_SPLITTER_WINDOW
	if( document_splitted(doc) && doc->split_editor != NULL ){
		sci = SCINTILLA(doc->editor);
		sci_colourise(sci, 0, end);
		sci = SCINTILLA(doc->split_editor);
		sci_colourise(sci, 0, end);
	}else{
		document_colourise(doc , 0, end);
	}
#else
	document_colourise(doc , 0, end);
#endif	
}
void
document_style_clear_all(CssedDoc* doc) // no wrapper for this yet
{
	ScintillaObject * sci;
#ifdef WITH_SPLITTER_WINDOW
	GtkWidget *editor;
	
	if( document_splitted(doc) && doc->split_editor != NULL ){
		editor = doc->editor;
		sci = SCINTILLA(editor);
		SSM(sci, SCI_STYLECLEARALL, 0 , 0);
		editor = doc->split_editor;
		sci = SCINTILLA(editor);
		SSM(sci, SCI_STYLECLEARALL, 0 , 0);		
	}else{
		sci = (ScintillaObject *) sci_get_from_document(doc);
		SSM(sci, SCI_STYLECLEARALL, 0 , 0);	
	}
#else
	sci = (ScintillaObject *) sci_get_from_document(doc);
	SSM(sci, SCI_STYLECLEARALL, 0 , 0);
#endif
}

gint
document_get_style_at(CssedDoc* doc, gint pos)
{
	ScintillaObject * sci;
	sci = (ScintillaObject *) sci_get_from_document(doc);
	return SSM(sci, SCI_GETSTYLEAT, pos, 0);
}

void
document_set_font_by_name(CssedDoc* doc, gchar* font_name)
{
	ScintillaObject * sci;
	CssedFileType *filetype;
	GtkWidget *editor;
	gint style;
	
#ifdef WITH_SPLITTER_WINDOW
	if( document_splitted(doc) && doc->split_editor != NULL ){
		editor = doc->editor;
		sci = SCINTILLA( editor );
		filetype = document_get_filetype(doc);
	
		if( filetype->style_max > filetype->style_min ){
			for( style = filetype->style_min; style <= filetype->style_max; style++)
				SSM (sci, SCI_STYLESETFONT, style,(sptr_t) font_name );	
			// line number
			SSM (sci, SCI_STYLESETFONT, STYLE_LINENUMBER, (sptr_t) font_name);
		}else{
			SSM (sci, SCI_STYLESETFONT, STYLE_LINENUMBER, (sptr_t) font_name);
			SSM (sci, SCI_STYLESETFONT, STYLE_DEFAULT, (sptr_t) font_name);
		}
		
		editor = doc->split_editor;
		sci = SCINTILLA( editor );
	
		if( filetype->style_max > filetype->style_min ){
			for( style = filetype->style_min; style <= filetype->style_max; style++)
				SSM (sci, SCI_STYLESETFONT, style,(sptr_t) font_name );	
			// line number
			SSM (sci, SCI_STYLESETFONT, STYLE_LINENUMBER, (sptr_t) font_name);
		}else{
			SSM (sci, SCI_STYLESETFONT, STYLE_LINENUMBER, (sptr_t) font_name);
			SSM (sci, SCI_STYLESETFONT, STYLE_DEFAULT, (sptr_t) font_name);
		}		
	}else{
		editor = document_get_scintilla_widget(doc);
		sci = SCINTILLA( editor );
		filetype = document_get_filetype(doc);
	
		if( filetype->style_max > filetype->style_min ){
			for( style = filetype->style_min; style <= filetype->style_max; style++)
				SSM (sci, SCI_STYLESETFONT, style,(sptr_t) font_name );	
			// line number
			SSM (sci, SCI_STYLESETFONT, STYLE_LINENUMBER, (sptr_t) font_name);
		}else{
			SSM (sci, SCI_STYLESETFONT, STYLE_LINENUMBER, (sptr_t) font_name);
			SSM (sci, SCI_STYLESETFONT, STYLE_DEFAULT, (sptr_t) font_name);
		}
	}
#else 
	editor = document_get_scintilla_widget(doc);
	sci = SCINTILLA( editor );
	filetype = document_get_filetype(doc);

	if( filetype ){
		for( style = filetype->style_min; style <= filetype->style_max; style++)
			SSM (sci, SCI_STYLESETFONT, style,(sptr_t) font_name );	
		// line number
		SSM (sci, SCI_STYLESETFONT, STYLE_LINENUMBER, (sptr_t) font_name);
	}
#endif
}

void
document_set_codepage( CssedDoc* doc, CssedCodePage codepage )
{
	ScintillaObject * sci;
	GtkWidget *editor;
#ifdef WITH_SPLITTER_WINDOW
	ScintillaObject * secondary_sci = NULL;
	gboolean splitted;
	
	editor = document_get_primary_scintilla_widget(doc);
	sci = SCINTILLA( editor );
	splitted = document_splitted(doc);
	
	if( splitted ) {
		editor = document_get_secondary_scintilla_widget(doc);
		secondary_sci = SCINTILLA( editor );
	}
	
	switch( codepage ){
		case CSSED_CODEPAGE_DEFAULT:
			SSM( sci, SCI_SETCODEPAGE, 0, 0);
			if( splitted ) SSM( secondary_sci, SCI_SETCODEPAGE, 0, 0);
			break;
		case CSSED_CODEPAGE_UTF8:
			SSM( sci, SCI_SETCODEPAGE, SC_CP_UTF8, 0);
			if( splitted ) SSM( secondary_sci, SCI_SETCODEPAGE, SC_CP_UTF8, 0);
			break;
		case CSSED_CODEPAGE_DBCS:
			SSM( sci, SCI_SETCODEPAGE, SC_CP_DBCS, 0);
			if( splitted ) SSM( secondary_sci, SCI_SETCODEPAGE, SC_CP_DBCS, 0);			
			break;
	}
#else
	editor = document_get_scintilla_widget(doc);
	sci = SCINTILLA( editor );

	switch( codepage ){
		case CSSED_CODEPAGE_DEFAULT:
			SSM( sci, SCI_SETCODEPAGE, 0, 0);
			break;
		case CSSED_CODEPAGE_UTF8:
			SSM( sci, SCI_SETCODEPAGE, SC_CP_UTF8, 0);
			break;
		case CSSED_CODEPAGE_DBCS:
			SSM( sci, SCI_SETCODEPAGE, SC_CP_DBCS, 0);
			break;
	}
#endif
}

CssedCodePage
document_get_codepage( CssedDoc* doc )
{
	ScintillaObject * sci;
	gint scintilla_codepage;
	GtkWidget *editor;
	
	editor = document_get_scintilla_widget(doc);
	sci = SCINTILLA( editor );

	scintilla_codepage = SSM(sci,  SCI_GETCODEPAGE, 0,0);

	switch( scintilla_codepage ){
		case 0:
			return CSSED_CODEPAGE_DEFAULT;
		case SC_CP_UTF8:
			return CSSED_CODEPAGE_UTF8;
		case SC_CP_DBCS:
			return CSSED_CODEPAGE_DBCS;
	}
	return CSSED_CODEPAGE_DEFAULT;
}

void
document_set_character_set( CssedDoc* doc, gint charset )
{
	ScintillaObject * sci;
	CssedFileType *filetype;
	GtkWidget *editor;
	gint style;
	
	editor = document_get_scintilla_widget(doc);
	sci = SCINTILLA( editor );
	
	filetype = document_get_filetype(doc);
	if( filetype ){
		for( style = filetype->style_min; style <= filetype->style_max; style++)
			SSM (sci, SCI_STYLESETCHARACTERSET, style, charset );
	}
}

void
document_set_font(CssedDoc* doc)
{
	CssedWindow* window;
	CssedConfig* cfg;

	window = CSSED_WINDOW(doc->window);
	cfg = cssed_window_get_config( window );
	document_set_font_by_name(doc, cfg->font);
}

void
document_set_current_font_size(CssedDoc* doc)
{
	CssedWindow* window;
	CssedConfig* cfg;

	window = CSSED_WINDOW(doc->window);
	cfg = cssed_window_get_config( window );
	document_set_font_size(doc, cfg->font_size);
}

void
document_set_font_size(CssedDoc* doc, gint size)
{
	ScintillaObject * sci;
	CssedFileType *filetype;
	gint style;
	GtkWidget *editor;
	
#ifdef WITH_SPLITTER_WINDOW	
	editor = document_get_primary_scintilla_widget(doc);
	sci = SCINTILLA( editor );

	filetype = document_get_filetype(doc);

	if( filetype->style_max > filetype->style_min ){
		for( style = filetype->style_min; style <= filetype->style_max; style++)
			SSM (sci, SCI_STYLESETSIZE, style, size);
		// line number
		SSM (sci, SCI_STYLESETSIZE, STYLE_LINENUMBER, ((size>2)?size-2:size));
	}else{
			SSM (sci, SCI_STYLESETSIZE, STYLE_LINENUMBER, ((size>2)?size-2:size));
			SSM (sci, SCI_STYLESETSIZE, STYLE_DEFAULT, size);	
	}
	
	if( !document_splitted(doc) ) return;
	
	editor = document_get_secondary_scintilla_widget(doc);
	sci = SCINTILLA( editor );

	if( filetype->style_max > filetype->style_min ){
		for( style = filetype->style_min; style <= filetype->style_max; style++)
			SSM (sci, SCI_STYLESETSIZE, style, size);
		// line number
		SSM (sci, SCI_STYLESETSIZE, STYLE_LINENUMBER, ((size>2)?size-2:size));
	} else{
		SSM (sci, SCI_STYLESETSIZE, STYLE_LINENUMBER, ((size>2)?size-2:size));
		SSM (sci, SCI_STYLESETSIZE, STYLE_DEFAULT, size);	
	}
	
#else
	editor = document_get_scintilla_widget(doc);
	sci = SCINTILLA( editor );

	filetype = document_get_filetype(doc);

	if( filetype ){
		for( style = filetype->style_min; style <= filetype->style_max; style++)
			SSM (sci, SCI_STYLESETSIZE, style, size);
		// line number
		SSM (sci, SCI_STYLESETSIZE, STYLE_LINENUMBER, size);
	}
#endif
}


void
document_editor_set_default_configuration( CssedDoc* doc, GtkWidget *editor )
{
	ScintillaObject * sci;
	CssedWindow *window;
	CssedConfig* cfg;
	
	window = document_get_window( doc );
	cfg = cssed_window_get_config( window );

	sci = SCINTILLA( editor );
	SSM (sci, SCI_SETREADONLY, 0, 0);
	SSM (sci, SCI_USEPOPUP, FALSE, 0);
	SSM (sci, SCI_SETMARGINTYPEN, 0, SC_MARGIN_NUMBER);
	SSM (sci, SCI_SETMARGINTYPEN, 1, SC_MARGIN_SYMBOL);
	SSM (sci, SCI_SETMARGINWIDTHN, 1, MARGIN_MARKERS_WIDTH_PIXELS);
	SSM (sci, SCI_SETMARGINSENSITIVEN,1, TRUE);
	//SSM (sci, SCI_MARKERDEFINE, 0, SC_MARK_ROUNDRECT); // marker 0 for bookmarks
	SSM (sci, SCI_MARKERDEFINEPIXMAP, 0, (sptr_t) arrow_2_blue_xpm); // marker 0 for bookmarks
	//SSM (sci, SCI_MARKERDEFINE, 1, SC_MARK_ARROWS); // marker 1 for validator
	//SSM (sci, SCI_MARKERSETFORE, 1, 0x0000ff);
	SSM (sci, SCI_MARKERDEFINEPIXMAP, 1, (sptr_t) red_arrow_xpm);// marker 1 for validator
	//SSM (sci, SCI_MARKERDEFINE, 2, SC_MARK_CIRCLE); // marker 2 for user markers (plugins and python)
	//SSM (sci, SCI_MARKERSETFORE, 2, 0xff0000);
	SSM (sci, SCI_MARKERDEFINEPIXMAP, 2, (sptr_t) arrow_2_green_xpm);	// marker 2 for user markers (plugins and python)
	SSM (sci, SCI_SETUNDOCOLLECTION, 1,0);
	//SSM (sci, SCI_SETYCARETPOLICY, CARET_SLOP|CARET_EVEN, 3);
	SSM (sci, SCI_SETWRAPVISUALFLAGS, SC_WRAPVISUALFLAG_END, 0);
	SSM (sci,SCI_SETMODEVENTMASK, 	SC_MODEVENTMASKALL, 0 );
	SSM (sci, SCI_SETPROPERTY, (sptr_t) "fold", (sptr_t) "1");
	SSM (sci, SCI_SETPROPERTY, (sptr_t) "fold.comment", (sptr_t) "1");
	SSM (sci, SCI_SETPROPERTY, (sptr_t) "fold.compact", (sptr_t) "1");
 	SSM (sci, SCI_SETFOLDFLAGS, 16,0);
    SSM (sci,SCI_MARKERDEFINE,  SC_MARKNUM_FOLDEROPEN, SC_MARK_BOXMINUS);
	SSM (sci,SCI_MARKERSETFORE, SC_MARKNUM_FOLDEROPEN, 0xffffff);
	SSM (sci,SCI_MARKERSETBACK, SC_MARKNUM_FOLDEROPEN, 0x000000);
    SSM (sci,SCI_MARKERDEFINE,  SC_MARKNUM_FOLDER, SC_MARK_BOXPLUS);
	SSM (sci,SCI_MARKERSETFORE, SC_MARKNUM_FOLDER, 0xffffff);
	SSM (sci,SCI_MARKERSETBACK, SC_MARKNUM_FOLDER, 0x000000);
    SSM (sci,SCI_MARKERDEFINE,  SC_MARKNUM_FOLDERSUB, SC_MARK_VLINE);
	SSM (sci,SCI_MARKERSETFORE, SC_MARKNUM_FOLDERSUB, 0xffffff);
	SSM (sci,SCI_MARKERSETBACK, SC_MARKNUM_FOLDERSUB, 0x000000);
    SSM (sci,SCI_MARKERDEFINE,  SC_MARKNUM_FOLDERTAIL, SC_MARK_LCORNER);
	SSM (sci,SCI_MARKERSETFORE, SC_MARKNUM_FOLDERTAIL, 0xffffff);
	SSM (sci,SCI_MARKERSETBACK, SC_MARKNUM_FOLDERTAIL, 0x000000);
    SSM (sci,SCI_MARKERDEFINE,  SC_MARKNUM_FOLDEREND, SC_MARK_BOXPLUSCONNECTED);
	SSM (sci,SCI_MARKERSETFORE, SC_MARKNUM_FOLDEREND, 0xffffff);
	SSM (sci,SCI_MARKERSETBACK, SC_MARKNUM_FOLDEREND, 0x000000);
    SSM (sci,SCI_MARKERDEFINE,  SC_MARKNUM_FOLDEROPENMID, SC_MARK_BOXMINUSCONNECTED);
	SSM (sci,SCI_MARKERSETFORE, SC_MARKNUM_FOLDEROPENMID, 0xffffff);
	SSM (sci,SCI_MARKERSETBACK, SC_MARKNUM_FOLDEROPENMID, 0x000000);
    SSM (sci,SCI_MARKERDEFINE,  SC_MARKNUM_FOLDERMIDTAIL, SC_MARK_TCORNER);
	SSM (sci,SCI_MARKERSETFORE, SC_MARKNUM_FOLDERMIDTAIL, 0xffffff);
	SSM (sci,SCI_MARKERSETBACK, SC_MARKNUM_FOLDERMIDTAIL, 0x000000);
	
#ifdef WITH_EXPERIMENTAL_CHANGE_STATE
	SSM (sci,SCI_MARKERDEFINE, SC_MARKNUM_CHANGEUNSAVED, SC_MARK_LEFTRECT);
	SSM (sci,SCI_MARKERDEFINE, SC_MARKNUM_CHANGESAVED, SC_MARK_LEFTRECT);
	
	SSM (sci,SCI_MARKERSETFORE, SC_MARKNUM_CHANGEUNSAVED, 0x00ffff);
	SSM (sci,SCI_MARKERSETBACK, SC_MARKNUM_CHANGEUNSAVED, 0x00ffff);
	
	SSM (sci,SCI_MARKERSETFORE, SC_MARKNUM_CHANGESAVED, 0x00ff00);
	SSM (sci,SCI_MARKERSETBACK, SC_MARKNUM_CHANGESAVED, 0x00ff00);
#endif

	SSM (sci, SCI_SETMARGINSENSITIVEN,2, TRUE);
	SSM (sci, SCI_SETMARGINMASKN,2, SC_MASK_FOLDERS);

/*
	if( cfg->folding ){
		SSM (sci, SCI_SETMARGINWIDTHN, 2, MARGIN_FOLDING_WIDTH_PIXELS );
	}else{
		SSM (sci, SCI_SETMARGINWIDTHN, 2, 0 );
	}
*/
    if( cfg->tabswidth )
        SSM (sci, SCI_SETTABWIDTH, cfg->tabswidth, 0);
    
	if( cfg->linenumbers ){
		SSM (sci, SCI_SETMARGINWIDTHN, 0,
			SSM(sci, SCI_TEXTWIDTH, STYLE_LINENUMBER,(sptr_t) MARGIN_LINES_WIDTH_STRING)
			);
	}else{
		SSM (sci, SCI_SETMARGINWIDTHN, 0, 0 );
	}

	if( cfg->lineendings ){
		sci_set_visible_eols( sci, TRUE );
	}

	if( cfg->whitespaces )
		sci_set_visible_white_spaces(sci, TRUE);
		
	if( cfg->lineswraped )
		sci_set_lines_wrapped( sci, TRUE );

	if( cfg->autocompletion )
		document_set_autocompletion_enabled( doc, TRUE );
	
	if( !cfg->usetabs )
        SSM (sci, SCI_SETUSETABS, 0, 0);

}

void
document_set_default_configuration( CssedDoc* doc )
{
	document_editor_set_default_configuration( doc, doc->editor );	
}

#ifdef WITH_SPLITTER_WINDOW
void
document_view_set_parent_configuration( CssedDoc* doc, GtkWidget* editor )
{
	CssedWindow *window;
	CssedFileType *filetype = doc->filetype;
	
	window = document_get_window(doc);

	filetype->apply_style(doc, editor);
	document_set_folding_margin_visible(doc, sci_get_folding_margin_visible( SCINTILLA(doc->editor) ));
	document_set_font(doc);
	document_set_current_font_size(doc);
	document_colourise_all(doc);	
}
#endif

CssedDoc*
document_get_from_container( CssedWindow* window, GtkWidget* container )
{
	GList* tmplist;
	CssedDoc* tmpdoc;

	tmplist = cssed_window_get_document_list_first( window );
	while(tmplist){
		tmpdoc = (CssedDoc*) tmplist->data;
		if( container == (GtkWidget*) tmpdoc->container_box ){
			return tmpdoc;
		}else{
			tmplist = g_list_next(tmplist);
		}
	}
	return NULL;
}

CssedDoc*
document_get_from_editor( CssedWindow* window, GtkWidget* editor )
{
	GList* tmplist;
	CssedDoc* tmpdoc;

	tmplist = cssed_window_get_document_list_first( window );
	while(tmplist){
		tmpdoc = (CssedDoc*) tmplist->data;
#ifdef WITH_SPLITTER_WINDOW
		if( editor == tmpdoc->editor || editor == tmpdoc->split_editor){
#else
		if( editor == tmpdoc->editor ){
#endif
			return tmpdoc;
		}else{
			tmplist = g_list_next(tmplist);
		}

	}
	return NULL;
}

CssedDoc*
document_get_from_notebook_page_num(CssedWindow* window, gint page_number)
{
	GtkWidget* container;
	GtkWidget* notebook;

	notebook = cssed_window_get_document_notebook( window );

	container = gtk_notebook_get_nth_page (GTK_NOTEBOOK(notebook), page_number);
	return document_get_from_container( window, container );
}

CssedDoc*
document_get_current(CssedWindow* window)
{
	GtkWidget* notebook;
	CssedDoc* tmpdoc;
	gint current_index;

	notebook = cssed_window_get_document_notebook( window );

	current_index = gtk_notebook_get_current_page   ( GTK_NOTEBOOK(notebook) );
	tmpdoc = document_get_from_notebook_page_num(window, current_index);

	return tmpdoc;
}

gint
document_get_index (CssedDoc* doc)
{
	CssedWindow* win;
	GtkWidget* notebook;

	win = document_get_window( doc );
	notebook = cssed_window_get_document_notebook( win );

	return gtk_notebook_page_num  ( GTK_NOTEBOOK(notebook), doc->container_box );
}

void
document_set_current(CssedDoc* doc)
{
	CssedWindow* win;
	GtkWidget* notebook;
	gint doc_index;

	win = document_get_window( doc );
	notebook = cssed_window_get_document_notebook( win );
	doc_index = gtk_notebook_page_num  ( GTK_NOTEBOOK(notebook), doc->container_box );

	if( doc_index > -1 )
		gtk_notebook_set_current_page   (GTK_NOTEBOOK(notebook), doc_index);
}

gboolean
document_get_modified( CssedDoc* doc )
{
	ScintillaObject * sci;

	sci = (ScintillaObject *) sci_get_from_document(  doc );
	return SSM (sci, SCI_GETMODIFY, 0, 0);
}

void/// FIXME: it's not being used (?)
document_set_modified( CssedDoc* doc , gboolean modified)
{
	ScintillaObject * sci;

	if( modified ){
		//doc->modified = TRUE; /// FIXME: it's not being used (?)
	}else{
		sci = (ScintillaObject *) sci_get_from_document(  doc );
		SSM (sci, SCI_SETSAVEPOINT, 0, 0);
		//doc->modified = FALSE; /// FIXME: it's not being used (?)
	}
}

void
document_set_save_point( CssedDoc* doc )
{
	ScintillaObject * sci;
	CssedWindow* window;

	sci = (ScintillaObject *) sci_get_from_document(  doc );
	window = document_get_window( doc );

	SSM (sci, SCI_SETSAVEPOINT, 0, 0);
	cssed_window_disable_save( window );
}

/*  Detach the doc from the notebook , pops it out the list
	and clear all memory allocated. If last doc in queue it
	must open a clean one.
*/
gboolean
document_dispose( CssedDoc* doc )
{
	GtkWidget* notebook;
	GtkWidget* editor;
	CssedWindow* window;
	gint document_page_number;
	gint notebook_total_pages;
	gboolean needs_new = FALSE;

	notebook = doc->notebook;
	editor = doc->editor;

	DBGMSG (__FILE__,__LINE__,"document_dispose() -  editor %p document %p", editor, doc);
	document_page_number = gtk_notebook_page_num(GTK_NOTEBOOK(notebook), doc->container_box);
	DBGMSG (__FILE__,__LINE__,"will delete page %d from %p", document_page_number, notebook);

	if( document_page_number == -1 ){ // editor does not exist
		return FALSE;
	}else{
		gtk_notebook_remove_page(GTK_NOTEBOOK(notebook), document_page_number);
#ifdef GTK_IS_2_2
		notebook_total_pages = gtk_notebook_get_n_pages (GTK_NOTEBOOK(notebook));
#else
		notebook_total_pages = g_list_length(GTK_NOTEBOOK(notebook)->children);
#endif
		if( notebook_total_pages == 0 ){
			needs_new = TRUE;
			DBGMSG (__FILE__,__LINE__,"document_dispose() - Last notebook page. Will create new one ...");
		}
		// clear doc allocated mem and pop out the doc list
		if( doc->filename != NULL ){
			g_free(doc->filename);
		}
		window = doc->window;
		cssed_window_delete_document_from_list( window ,doc );
		
		g_free(doc);
	}

	if( needs_new )	create_and_attach_new_doc (window,_("Untitled"));
	return TRUE;
}

/* line numbers visibility */
gboolean
document_get_line_numbers_visible( CssedDoc* doc )
{
	ScintillaObject* sci;
	sci = sci_get_from_document(doc);
	return sci_get_line_numbers(sci);
}

void
document_set_line_numbers_visible( CssedDoc* doc, gboolean visible )
{
	ScintillaObject* sci;

	DBGMSG (__FILE__,__LINE__,"Going to set line numbers visibility to %s in doc %p",visible?"true":"false",doc);
	sci = sci_get_from_document(doc);
	sci_set_line_numbers(sci,visible);
}
/* folding marging visibility */
gboolean
document_get_folding_margin_visible( CssedDoc* doc )
{
	ScintillaObject* sci;
	sci = sci_get_from_document(doc);
	return sci_get_folding_margin_visible(sci);
}

void
document_set_folding_margin_visible( CssedDoc* doc, gboolean visible )
{
	ScintillaObject* sci;

	DBGMSG (__FILE__,__LINE__,"Going to set folding margin visibility to %s in doc %p",visible?"true":"false",doc);
	sci = sci_get_from_document(doc);
	sci_set_folding_margin_visible(sci,visible);
}
/* tabs */

/* line endings visibility */
gboolean
document_get_line_endings_visible( CssedDoc* doc)
{
	ScintillaObject* sci;
	sci = sci_get_from_document(doc);
	return sci_get_visible_eols(sci);
}
void
document_set_line_endings_visible( CssedDoc* doc, gboolean visible)
{
	ScintillaObject* sci;
	sci = sci_get_from_document(doc);
	sci_set_visible_eols(sci,visible);
}
/* line wrapping */
gboolean
document_get_lines_wrapped( CssedDoc* doc)
{
	ScintillaObject* sci;
	sci = sci_get_from_document(doc);
	return sci_get_lines_wrapped(sci);
}
void
document_set_lines_wrapped( CssedDoc* doc, gboolean wrapped)
{
	ScintillaObject* sci;
	sci = sci_get_from_document(doc);
	sci_set_lines_wrapped(sci,wrapped);
}
/* white spaces */
gboolean
document_get_white_spaces_visible( CssedDoc* doc)
{
	ScintillaObject* sci;
	sci = sci_get_from_document(doc);
	return sci_get_visible_white_spaces(sci);
}
void
document_set_white_spaces_visible( CssedDoc* doc, gboolean visible)
{
	ScintillaObject* sci;
	sci = sci_get_from_document(doc);
	sci_set_visible_white_spaces(sci,visible);
}

/* EOL mode */
CssedEolMode
document_get_eol_mode( CssedDoc* doc )
{
	ScintillaObject* sci;
	sci = sci_get_from_document(doc);
	return (CssedEolMode) sci_get_eol_mode	(sci);
}

void
document_set_eol_mode(CssedDoc* doc, CssedEolMode eolmode)
{
	ScintillaObject* sci;
	
	sci = sci_get_from_document(doc);
	sci_set_eol_mode(sci, (gint) eolmode);
}

void
document_convert_eols(CssedDoc* doc, CssedEolMode eolmode)
{
	ScintillaObject* sci;
	sci = sci_get_from_document(doc);
	sci_convert_eols(sci, (gint) eolmode);
}
/* text */
void
document_add_text( CssedDoc* doc, gchar* text )
{
	ScintillaObject* sci;
	sci = sci_get_from_document( doc );
	sci_add_text(sci,text);
}

// no need to be NULL terminated
void
document_add_text_buffer( CssedDoc* doc, gchar* buffer , gint len)
{
	ScintillaObject * sci;
	sci = SCINTILLA( doc->editor );
	SSM (sci, SCI_ADDTEXT, len, (sptr_t) buffer );
}

void/* adds text to the document editor ending in line feed */
document_add_text_line( CssedDoc * doc, char* text )
{
	ScintillaObject * sci;
	sci = SCINTILLA( doc->editor );

	SSM (sci, SCI_ADDTEXT, strlen(text) , (sptr_t) text );
	SSM (sci, SCI_ADDTEXT, strlen("\n") , (sptr_t) "\n" );
}

// find differs to search as find returns the location
// but search moves the caret to the position or shows an error
// it the text was not found
gint // no wrapper function for this
document_find_next( CssedDoc* doc, gchar* text, gint flags )
{
	ScintillaObject* sci;
	gint selection_start;
	gint selection_end;
#ifdef DEBUG_OUTPUT
	gint current_pos;
	gint anchor_pos;
#endif

	sci = sci_get_from_document( doc );
	// look if any selection, if so move the caret out it can
	// it can be selected by a prevous search.
	// right now, cssed cannot search into a block of text selected by
	// the user. The user must know that.

	selection_start = SSM(sci, SCI_GETSELECTIONSTART, 0, 0);
	selection_end   = SSM(sci, SCI_GETSELECTIONEND, 0, 0);

#ifdef DEBUG_OUTPUT
	current_pos		= SSM(sci, SCI_GETCURRENTPOS, 0, 0);
	anchor_pos		= SSM(sci, SCI_GETANCHOR, 0, 0);
	DBGMSG (__FILE__,__LINE__, "document_find_next(): Selection start %d end %d pos %d anchor %d",selection_start,selection_end,current_pos,anchor_pos);
#endif

	if( selection_end > selection_start ){ // there's selection
		/// FIXME: build a wrapper function
		SSM( sci, SCI_GOTOPOS, selection_end + 1, 0);
	}
	// set the anchor
	//// FIXME: build a wrapper function
	SSM( sci, SCI_SEARCHANCHOR,0,0);
	/// FIXME: build a wrapper function
	return SSM( sci, SCI_SEARCHNEXT,flags,(sptr_t) text );
}

void // no wrapper function for this
document_search_next( CssedDoc* doc, gchar* text, gint flags )
{
	ScintillaObject* sci;
	gint search_pos;
	gint line;

	sci = sci_get_from_document( doc );

	search_pos = document_find_next(doc, text, flags);
	
	if( search_pos != -1 ){
		line = document_get_line_from_position( doc, search_pos );
		if( !document_get_line_is_visible( doc, line ) ){
			document_ensure_line_is_visible( doc, line );
		}
		/// FIXME: build a wrapper function
		SSM( sci, SCI_SCROLLCARET,0,0);
	}else{
		cssed_error_message
		(
		_("End of document reached"), 
		_("The text cannot be found or end of document reached.\n\nIf you are sure the text exists, then the document is inactive.\nClick on a document to make it active")
		);
	}
}

gint // no wrapper function for this
document_find_prev( CssedDoc* doc, gchar* text, gint flags )
{
	ScintillaObject* sci;
	gint selection_start;
	gint selection_end;
#ifdef DEBUG_OUTPUT
	gint current_pos;
	gint anchor_pos;
#endif

	sci = sci_get_from_document( doc );
	// look if any selection, if so move the caret out it can
	// it can be selected by a prevous search.
	// right now, cssed cannot search into a block of text selected by
	// the user. The user must know that.

	selection_start = SSM( sci, SCI_GETSELECTIONSTART, 0, 0);
	selection_end   = SSM( sci, SCI_GETSELECTIONEND, 0, 0);

#ifdef DEBUG_OUTPUT
	current_pos		= SSM( sci, SCI_GETCURRENTPOS, 0, 0);
	anchor_pos		= SSM( sci, SCI_GETANCHOR, 0, 0);
	DBGMSG (__FILE__,__LINE__, "document_search_prev(): Selection start %d end %d pos %d anchor %d",selection_start,selection_end,current_pos,anchor_pos);
#endif

	if( selection_end > selection_start ){ // there's selection
		/// FIXME: build a wrapper function
		SSM( sci, SCI_GOTOPOS, selection_start - 1, 0);
	}
	// set the anchor
	/// FIXME: build a wrapper function
	SSM( sci, SCI_SEARCHANCHOR,0,0);
	/// FIXME: build a wrapper function
	return SSM( sci, SCI_SEARCHPREV, flags,(sptr_t) text );
}

void // no wrapper function for this
document_search_prev( CssedDoc* doc, gchar* text, gint flags )
{
	ScintillaObject* sci;
	gint search_pos;
	gint line;

	sci = sci_get_from_document( doc );

	search_pos = document_find_prev( doc, text, flags);
	if( search_pos != -1 ){
		line = document_get_line_from_position( doc, search_pos );
		if( !document_get_line_is_visible( doc, line ) ){
			document_ensure_line_is_visible( doc, line );
		}
		/// FIXME: build a wrapper function
		SSM( sci, SCI_SCROLLCARET,0,0);
	}else{
		cssed_error_message
		(
		_("Start of document reached"),
		_("The text cannot be found or start of document reached.\n\nIf you are \
sure the text exists, then the document is inactive.\nClick on a document to make it active")
		);
	}
}

gboolean
document_can_redo(CssedDoc* doc)
{
	ScintillaObject* sci;

	sci = sci_get_from_document( doc );
	return sci_can_redo( sci );
}
gboolean
document_can_undo(CssedDoc* doc)
{
	ScintillaObject* sci;

	sci = sci_get_from_document( doc );
	return sci_can_undo( sci );
}

void
document_undo(CssedDoc* doc)
{
	ScintillaObject* sci;

	sci = sci_get_from_document( doc );
	sci_undo(sci);
}
void
document_redo(CssedDoc* doc)
{
	ScintillaObject* sci;

	sci = sci_get_from_document( doc );
	sci_redo(sci);
}

void
document_end_undo_action(CssedDoc* doc)
{
	ScintillaObject* sci;

	sci = sci_get_from_document( doc );
	sci_end_undo_action(sci);
}

void
document_start_undo_action (CssedDoc* doc)
{
	ScintillaObject* sci;

	sci = sci_get_from_document( doc );
	sci_start_undo_action(sci);
}

void
document_new_undo_action(CssedDoc* doc)
{
	document_end_undo_action(doc);
	document_start_undo_action (doc);
}

void
document_empty_undo_buffer(CssedDoc* doc)
{
	ScintillaObject* sci;

	sci = sci_get_from_document( doc );
	sci_empty_undo_buffer(sci);
}

void
document_set_undo_collection(CssedDoc* doc, gboolean set)
{
	ScintillaObject* sci;

	sci = sci_get_from_document( doc );
	sci_set_undo_collection(sci,set);
}

gboolean
document_get_undo_collection(CssedDoc* doc)
{
	ScintillaObject* sci;

	sci = sci_get_from_document( doc );
	return sci_get_undo_collection(sci);
}

void
document_zoom_in(CssedDoc* doc)
{
	ScintillaObject* sci;

	sci = sci_get_from_document( doc );
	sci_zoom_in(sci);
}

void
document_zoom_out(CssedDoc* doc)
{
	ScintillaObject* sci;

	sci = sci_get_from_document( doc );
	sci_zoom_out(sci);
}

void
document_zoom_off(CssedDoc* doc)
{
	ScintillaObject* sci;

	sci = sci_get_from_document( doc );
	sci_zoom_off(sci);
}

void
document_cut (CssedDoc* doc)
{
	ScintillaObject* sci;

	sci = sci_get_from_document( doc );
	sci_cut (sci);
}
void
document_copy (CssedDoc* doc)
{
	ScintillaObject* sci;

	sci = sci_get_from_document( doc );
	sci_copy (sci);
}
void
document_paste (CssedDoc* doc)
{
	ScintillaObject* sci;

	sci = sci_get_from_document( doc );
	sci_paste (sci);
}

void
document_set_marker_at_line( CssedDoc* doc, gint line, gboolean set, gint marker)
{
	ScintillaObject* sci;

	sci = sci_get_from_document( doc );
	sci_set_marker_at_line( sci, line, set, marker);
}
gboolean
document_is_marker_set_at_line( CssedDoc* doc, gint line, gint marker)
{
	ScintillaObject* sci;
	gboolean set;
 
	sci = sci_get_from_document (doc);
	set = sci_is_marker_set_at_line (sci, line, marker);
	return set;
}
gboolean
document_marker_next( CssedDoc* doc, gint line, gint marker_mask )

{
	ScintillaObject* sci;

	sci = sci_get_from_document( doc );
	return sci_marker_next( sci, line, marker_mask );
}


gboolean
document_marker_prev( CssedDoc* doc, gint line, gint marker_mask )
{
	ScintillaObject* sci;

	sci = sci_get_from_document( doc );
	return sci_marker_prev( sci, line, marker_mask );
}

gint
document_get_line_from_position( CssedDoc* doc, gint position)
{
	ScintillaObject* sci;

	sci = sci_get_from_document( doc );
	return sci_get_line_from_position(sci, position );
}
gint
document_get_position_from_line( CssedDoc* doc, gint line)
{
	ScintillaObject* sci;

	sci = sci_get_from_document( doc );
	return sci_get_position_from_line(sci, line );
}
gint
document_get_current_position	(CssedDoc* doc )
{
	ScintillaObject* sci;

	sci = sci_get_from_document( doc );
	return sci_get_current_position	(sci);
}

void
document_set_current_position	(CssedDoc* doc, gint position)
{
	ScintillaObject* sci;

	sci = sci_get_from_document(doc);
	sci_set_current_position(sci, position);
	document_scroll_to_caret(doc);
}

void
document_set_current_line(CssedDoc* doc, gint line)
{
	ScintillaObject* sci;

	sci = sci_get_from_document(doc);
	sci_set_current_line(sci, line);
}


gint
document_get_selection_start(CssedDoc* doc)
{
	ScintillaObject* sci;

	sci = sci_get_from_document(doc);
	return sci_get_selection_start (sci);
}

gint
document_get_selection_end(CssedDoc* doc)
{
	ScintillaObject* sci;

	sci = sci_get_from_document(doc);
	return sci_get_selection_end(sci);
}

void
document_set_selection_start(CssedDoc* doc, gint position)
{
	ScintillaObject* sci;

	sci = sci_get_from_document(doc);
	sci_set_selection_start	(sci, position);
}

void
document_set_selection_end(CssedDoc* doc, gint position)
{
	ScintillaObject* sci;

	sci = sci_get_from_document(doc);
	sci_set_selection_end(sci, position);
}

gint
document_get_line_end_from_position(CssedDoc* doc, gint position)
{
	ScintillaObject* sci;

	sci = sci_get_from_document(doc);
	return sci_get_line_end_from_position(sci, position);
}

void  // make a sci wrapper for this FIXME
document_set_selection_range(CssedDoc* doc,gint start, gint end)
{
	ScintillaObject* sci;

	sci = sci_get_from_document( doc );
	SSM(sci, SCI_SETSEL, end, start);
}

void
document_replace_sel(CssedDoc* doc, gchar* text)
{
	ScintillaObject* sci;

	sci = sci_get_from_document( doc );
	sci_replace_sel(sci, text);
}

void
document_replace_sel_re(CssedDoc* doc, gchar* text)
{
	ScintillaObject* sci;

	sci = sci_get_from_document( doc );
	sci_replace_sel_re(sci, text);
}

void
document_clear_sel(CssedDoc* doc)
{
	ScintillaObject* sci;

	sci = sci_get_from_document( doc );
	sci_replace_sel(sci, "");
}


gint
document_get_length	(CssedDoc* doc)
{
	ScintillaObject* sci;

	sci = sci_get_from_document( doc );
	return sci_get_length(sci);
}
/* you should allocate a buffer enought to contain the text plus a null char */
void
document_get_text(CssedDoc* doc, gint len, gchar* text)
{
	ScintillaObject* sci;

	sci = sci_get_from_document (doc);
	sci_get_text (sci, len, text);
}

gchar
document_get_char_at(CssedDoc* doc, gint pos)
{
	ScintillaObject* sci;

	sci = sci_get_from_document (doc);
	return sci_get_char_at (sci, pos);
}

/* you should allocate a buffer enought to contain the text plus a null char */
void
document_get_selected_text(CssedDoc* doc, gchar* text)
{
	ScintillaObject* sci;

	sci = sci_get_from_document (doc);
	sci_get_selected_text (sci, text);
}

void
document_write_error_to_program_output(CssedDoc* doc, gchar* error_message, gchar* color_desc)
{
	CssedWindow *window;
	gchar* markup;

	window = CSSED_WINDOW(doc->window);

	markup = g_strdup_printf ("<span color='%s'>%s</span>", color_desc, error_message);
	cssed_window_output_write (window, markup);
	g_free (markup);

	gtk_notebook_set_current_page (GTK_NOTEBOOK(cssed_window_get_footer_notebook( window )),0);
}
/// FIXME fails if a string with a brace is present in the declaration
gboolean
document_current_pos_is_into_selector(CssedDoc* doc)
{
	ScintillaObject *sci;
	gint position;
	gint open_brace;
	gint close_brace;
	//gchar *buffer;

	sci = (ScintillaObject *) sci_get_from_document (doc);
	position = document_get_current_position (doc);

	SSM (sci, SCI_SEARCHANCHOR, 0, 0);
	open_brace = SSM (sci, SCI_SEARCHPREV, 0, (sptr_t) "{");

#ifdef DEBUG_OUTPUT
	if (open_brace == -1)
		DBGMSG (__FILE__,__LINE__,"document_current_pos_is_into_selector() - No opening brace before");
	else
		DBGMSG (__FILE__,__LINE__,"document_current_pos_is_into_selector() - Opening brace at %d", open_brace);
#endif

	SSM (sci, SCI_SEARCHANCHOR, 0, 0);
	close_brace = SSM (sci, SCI_SEARCHNEXT, 0, (sptr_t) "}");

#ifdef DEBUG_OUTPUT
	if (close_brace == -1)
		DBGMSG (__FILE__,__LINE__,"document_current_pos_is_into_selector() -No closing brace after");
	else
		DBGMSG (__FILE__,__LINE__,"document_current_pos_is_into_selector() -Closing brace at %d", close_brace);
#endif

	if (close_brace < position)
	{
		document_set_current_position(doc,position);
		return FALSE;
	}
	else
	{
		if ((open_brace != -1) && (close_brace != -1))
		{
			document_set_current_position(doc,position);
			return TRUE;
		}else{
			document_set_current_position(doc,position);
			return FALSE;
		}
	}
}
gint
document_get_line_length(CssedDoc* doc, gint line)
{
	ScintillaObject *sci;
	sci = sci_get_from_document (doc);
	return sci_line_length(sci,line);
}

gint
document_get_line_count( CssedDoc* doc )
{
	ScintillaObject *sci;
	sci = sci_get_from_document (doc);
	return sci_get_line_count( sci );
}

void
document_get_text_line(CssedDoc* doc, gint line, gchar* buffer)
{
	ScintillaObject *sci;

	sci = (ScintillaObject *) sci_get_from_document (doc);
	sci_get_line(sci, line, buffer);
}
void  // not in document.h
document_get_xy_from_position(CssedDoc* doc,gint pos, gint* x, gint* y)
{
	ScintillaObject *sci;

	sci = (ScintillaObject *) sci_get_from_document (doc);
	sci_get_xy_from_position(sci, pos, x, y);
}
void   // not in document.h
document_gtk_popmenu_position_func(GtkMenu *menu, gint *x, gint *y,
				gboolean *push_in,  gpointer user_data)
{
	CssedPoint* point;
	CssedWindow* window;
	CssedDoc* doc;
	GtkWidget* notebook;
	GtkWidget* editor;
	GtkWidget* window_widget;
	GdkWindow* gdkwindow;
	gint new_x;
	gint new_y;
	gint root_x;
	gint root_y;

	point = (CssedPoint*) user_data;
	window = CSSED_WINDOW(point->main_window);
	doc = document_get_current(window);
	notebook = cssed_window_get_document_notebook( window );
	editor = document_get_scintilla_widget(doc);
	//window_widget = gtk_widget_get_toplevel(notebook);
	window_widget = cssed_window_get_window_widget( window );
	gdkwindow = gtk_widget_get_parent_window (doc->container_box);//(editor);
	DBGMSG (__FILE__,__LINE__," document_gtk_popmenu_position_func - is notebook %d",GTK_IS_NOTEBOOK(notebook));
	gtk_widget_translate_coordinates        (editor,
                                             window_widget,
                                             point->x,
                                             point->y,
                                             &new_x,
                                             &new_y);
	DBGMSG (__FILE__,__LINE__," document_gtk_popmenu_position_func -Translated coordinates from editor to window x = %d , y = %d",
			new_x, new_y);
	gdk_window_get_root_origin(gdkwindow, &root_x, &root_y);
	DBGMSG (__FILE__,__LINE__," document_gtk_popmenu_position_func -Root window coordinates x = %d , y = %d",
			root_x, root_y);
	*x = new_x + root_x + 5;  // +5  add a little padding to right
	*y = new_y + root_y + 30; // +30 add a little padding down
	DBGMSG (__FILE__,__LINE__," document_gtk_popmenu_position_func -Menu coordinates x = %d , y = %d",
			*x, *y);

	g_free(user_data);
}
/* It will scan the second treeview colum ( in the css definitions treeview )
	trying to find the text passed. If it's found, it builds a popmenu with the
	third column contents and connect the appropiated callbacks to get
	autocompletion to work */
void /// FIXME to future CSS support code
document_pop_submenu_from_keyword_in_treeview(CssedDoc* doc,gchar* text)
{
	CssedWindow* window;
	CssedPoint* point;
	CssedPopmenuData* popmenu_data;
	GtkWidget* treeview;
	GtkTreeModel* model;
	GtkWidget* menu;
	GtkWidget* menu_items;
	GtkTreeIter iter;
	gboolean valid;
	gchar* keyword;
	gchar* value;
	gchar* path_str;
	GList* menu_item_list = NULL;
	gint nrow = 0;
	gint x;
	gint y;

	window = CSSED_WINDOW(doc->window);
	treeview = cssed_window_get_css_definition_treeview( window );

	model = gtk_tree_view_get_model(GTK_TREE_VIEW(treeview));

	if ( model == NULL ){
		return;
	}
	valid = gtk_tree_model_get_iter_from_string(model,&iter,"0:0");
	if(!valid)	return;

	while(valid){
		gtk_tree_model_get (model, &iter, 1, &keyword, -1);
        DBGMSG (__FILE__,__LINE__,"scanning %s", keyword);
		if( strcmp(keyword,text)==0 ){
			DBGMSG (__FILE__,__LINE__,"found !!");
			path_str = g_strdup_printf("0:%d:0",nrow);
			valid =  gtk_tree_model_get_iter_from_string(model,&iter,path_str);
			DBGMSG (__FILE__,__LINE__,"trying to get child on %s",path_str);
			g_free(path_str);
			if(!valid ){
				DBGMSG (__FILE__,__LINE__,"Cannot get child.");
				g_free(keyword);
				return;
			}
			DBGMSG (__FILE__,__LINE__,"got 3rd child!");
			menu = gtk_menu_new ();

			while( valid ){
				popmenu_data = g_malloc(sizeof(	CssedPopmenuData ));
				gtk_tree_model_get (model, &iter, 2, &value, -1);
				menu_items = gtk_menu_item_new_with_label (value);
				popmenu_data->main_window = window;
				popmenu_data->document = document_get_current(window);
				popmenu_data->attribute = g_strdup(value);
				popmenu_data->property = g_strdup(keyword);
				popmenu_data->add_property_string = FALSE;

				menu_item_list = g_list_append(menu_item_list, popmenu_data);
				gtk_menu_shell_append (GTK_MENU_SHELL (menu), menu_items);

				// callback will call fire_dialog_from_strings_or_write_output()
				g_signal_connect (G_OBJECT (menu_items), "activate",
                                      G_CALLBACK (popmenu_autocomp_activate_cb),
                                      popmenu_data );

				gtk_widget_show (menu_items);
				g_free(value);
				valid = gtk_tree_model_iter_next (model,&iter);
			}

			// this will dispose all memory allocated parsing the list
			// passed on unmap event (event->type ==  GDK_UNMAP).
			g_signal_connect (G_OBJECT (menu), "event-after",
                               G_CALLBACK (popmenu_autocomp_event_after_cb),
                               menu_item_list );
			DBGMSG (__FILE__,__LINE__,"Sending list %p as user_data for callback",menu_item_list);
			document_get_xy_from_position(doc, document_get_current_position(doc),&x,&y);
			DBGMSG (__FILE__,__LINE__,"Current point into editor x=%d,y=%d",x,y);
			point = g_malloc(sizeof(CssedPoint));
			point->main_window = window;
			point->x = x;
			point->y = y;
			gtk_menu_popup (GTK_MENU(menu),NULL, NULL,
										 document_gtk_popmenu_position_func  ,
                                         (gpointer) point,// freed by callback
                                         0,
                                         gtk_get_current_event_time());
			g_free(keyword);
			return;
		}
		nrow++;
        g_free (keyword);
		valid = gtk_tree_model_iter_next (model, &iter);
     }
}

void // if identifier > 0 it's a user list and it'll be tracked on notifications
document_show_user_list (CssedDoc* doc, gint identifier, gchar* list)
{
	ScintillaObject* sci;
	sci = sci_get_from_document(doc);
	SSM(sci, SCI_USERLISTSHOW, identifier, (sptr_t) list);
}

// call tips 
void
document_calltipshow (CssedDoc* doc, gint start, gchar* definition)
{
	ScintillaObject* sci;

	g_return_if_fail( definition != NULL );

	sci = sci_get_from_document(doc);
	sci_calltipshow( sci, start, definition );
}

void
document_calltipcancel (CssedDoc* doc)
{
	ScintillaObject* sci;
	sci = sci_get_from_document(doc);
	sci_calltipcancel( sci );
}

gboolean
document_calltipactive (CssedDoc* doc)
{
	ScintillaObject* sci;
	sci = sci_get_from_document(doc);
	return sci_calltipactive( sci );
}

gint
document_calltiposstart (CssedDoc* doc)
{
	ScintillaObject* sci;
	sci = sci_get_from_document(doc);
	return sci_calltiposstart (sci);
}

//
void
document_grab_focus	(CssedDoc* doc)
{
	ScintillaObject* sci;
	sci = sci_get_from_document(doc);
	sci_grab_focus (sci);
}
// auto completion //////////////////////////////////////////////////////

void
document_show_autocompletion_list (CssedDoc* doc, gint len, gchar* list)
{
	ScintillaObject* sci;
	sci = sci_get_from_document(doc);
	SSM(sci, SCI_AUTOCSHOW, len, (sptr_t) list);
}

void 
document_autocompletion_cancel (CssedDoc* doc)
{	
	ScintillaObject* sci;
	sci = sci_get_from_document(doc);
	SSM(sci, SCI_AUTOCCANCEL, 0 , 0);
}

gboolean
document_is_autocompletion_active (CssedDoc* doc)
{
	ScintillaObject* sci;
	sci = sci_get_from_document(doc);
	return SSM(sci, SCI_AUTOCACTIVE, 0 , 0);	
}

void 
document_autocompletion_select (CssedDoc* doc, gchar* select)
{
	ScintillaObject* sci;
	sci = sci_get_from_document(doc);
	SSM(sci, SCI_AUTOCSELECT, 0 ,  (sptr_t) select);
}
// call tips

/////////////////////////////////////////////////////////////////////////////////
void
document_toggle_fold_at_line(CssedDoc* doc, gint line)
{
	ScintillaObject* sci;
	sci = sci_get_from_document(doc);
	sci_toggle_fold(sci,line);
}

gboolean
document_get_line_is_visible(CssedDoc* doc, gint line)
{
	ScintillaObject* sci;
	sci = sci_get_from_document(doc);
	return sci_get_line_is_visible(sci,line);
}

void
document_ensure_line_is_visible(CssedDoc* doc, gint line)
{
	ScintillaObject* sci;
	sci = sci_get_from_document(doc);
	sci_ensure_line_is_visible( sci, line);
}

gint
document_get_fold_level(CssedDoc* doc, gint line)
{
	ScintillaObject* sci;
	sci = sci_get_from_document(doc);
	return sci_get_fold_level(sci,line);
}
gint
document_get_fold_depth(CssedDoc* doc, gint line)
{
	ScintillaObject* sci;
	sci = sci_get_from_document(doc);
	return (sci_get_fold_level(sci,line) & SC_FOLDLEVELNUMBERMASK) - SC_FOLDLEVELBASE;
}
void
document_unfold_all( CssedDoc* doc)
{
	gint lines;
	gint pos;
	int i;
	lines = document_get_line_count(doc);
	pos = document_get_current_position(doc);
	for( i=0; i<lines;i++){
		document_ensure_line_is_visible( doc, i );
	}
	//document_set_current_position( doc,  0 );
}

void
document_fold_all( CssedDoc* doc)
{
	gint lines;
	gint pos;
	int i;

	if(!document_get_folding_enabled(doc) ){
		document_set_folding_margin_visible(doc,TRUE );
	}

	lines = document_get_line_count(doc);
	pos = document_get_current_position(doc);
	for( i=0; i<lines;i++){
		int level =document_get_fold_level(doc,i);
		if (level & SC_FOLDLEVELHEADERFLAG) {
			if( document_get_fold_expanded_at_line(doc, i) ){
				document_toggle_fold_at_line(doc, i);
			}
		}
	}
}
gboolean
document_get_fold_expanded_at_line( CssedDoc* doc, gint line)
{
	ScintillaObject* sci;
	sci = sci_get_from_document(doc);
	return sci_get_fold_expanded(sci, line);
}

gboolean
document_get_folding_enabled( CssedDoc* doc)
{
	return document_get_folding_margin_visible(doc);
}

// tabs

void
document_set_use_tabs (CssedDoc* doc, gboolean set )
{
	ScintillaObject* sci;
	sci = sci_get_from_document(doc);
	sci_set_use_tabs (sci, set);
}

gboolean
document_get_use_tabs (CssedDoc* doc)
{
	ScintillaObject* sci;
	sci = sci_get_from_document(doc);
	return sci_get_use_tabs(sci);
}

void
document_set_tabs_width (CssedDoc* doc, gint set )
{
	ScintillaObject* sci;
	sci = sci_get_from_document(doc);
	sci_set_tabs_width (sci, set);
}

gint
document_get_tabs_width (CssedDoc* doc)
{
	ScintillaObject* sci;
	sci = sci_get_from_document(doc);
	return sci_get_tabs_width(sci);
}



//filetype
CssedFileType*
document_get_filetype( CssedDoc* doc)
{
	return doc->filetype;
}

CssedFileTypeId
document_get_filetype_id( CssedDoc* doc)
{
	return doc->filetype->id;
}

CssedFileType*
document_get_filetype_from_filename( CssedDoc* newdoc, gchar* filename )
{
	CssedWindow* window;
	CssedFileTypeManager* manager;

	window = document_get_window(newdoc);
	manager = cssed_window_get_filetype_manager(window);
	return cssed_file_type_manager_get_filetype_from_filename(manager, filename);
}

/* this sets the style and the filetype  */
void
document_set_filetype( CssedDoc* doc, CssedFileType* filetype )
{
	CssedWindow* window;
	CssedConfig* cfg;

	if( filetype == NULL ) return;

	window = document_get_window(doc);
	cfg = cssed_window_get_config(window);
	document_style_clear_all(doc);

	doc->filetype = filetype;

#ifdef WITH_SPLITTER_WINDOW
	if( document_splitted(doc) && doc->split_editor != NULL ){
		filetype->apply_style(doc, doc->editor);
		filetype->apply_style(doc, doc->split_editor);
	}else{
		filetype->apply_style(doc, NULL);
	}		
#else
	filetype->apply_style(doc, NULL);
#endif

	document_set_folding_margin_visible(doc, cfg->folding && filetype->can_fold);

	document_set_font(doc);
	document_set_current_font_size(doc);
	document_colourise_all(doc);
}

void
document_set_filetype_by_id ( CssedDoc* doc, CssedFileTypeId id )
{
	CssedWindow* window;
	CssedConfig* cfg;
	CssedFileType* filetype;
	CssedFileTypeManager* manager;

	window = document_get_window(doc);
	cfg = cssed_window_get_config(window);
	document_style_clear_all(doc);
	manager = cssed_window_get_filetype_manager(window);

	filetype = cssed_file_type_manager_get_filetype_from_id (manager, id);
	
	if( filetype ){
		doc->filetype = filetype;
#ifdef WITH_SPLITTER_WINDOW
		if( document_splitted(doc) && doc->split_editor != NULL ){
			filetype->apply_style(doc, doc->editor);
			filetype->apply_style(doc, doc->split_editor);
		}else{
			filetype->apply_style(doc, NULL);
		}		
#else
		filetype->apply_style(doc, NULL);
#endif
		document_set_folding_margin_visible(doc, cfg->folding && filetype->can_fold);	
		document_set_font(doc);
		document_set_current_font_size(doc);
		document_colourise_all(doc);
	}
}

// ACCESS TO DOCUMENT'S FIELDS
CssedWindow*
document_get_window( CssedDoc* doc)
{
	return doc->window;
}


gchar*
document_get_filename( CssedDoc* doc)
{
	if(doc== NULL ) return NULL;

	if( doc->filename != NULL )
		return g_strdup(doc->filename);
	else
		return NULL;
}

void
document_set_filename( CssedDoc* doc, gchar* filename )
{
	g_return_if_fail( filename != NULL );
	if( doc->filename != NULL ) g_free(doc->filename);
	doc->filename = g_strdup(filename);
}

gboolean
document_get_autocompletion_enabled( CssedDoc* doc)
{
	CssedFileType *type;

	type = document_get_filetype(doc);
	if( type == NULL ) return FALSE; // if no file type no autocompletion
	return ( doc->autocompletion && type->char_added );
}

void
document_set_autocompletion_enabled ( CssedDoc* doc, gboolean enabled )
{
	doc->autocompletion = enabled;
}

// new 
void
document_indent_as_last_line ( CssedDoc* doc )
{
	gint pos, line, len;
	gchar *last_line, *cur_char;

	pos = document_get_current_position (doc);
	line = document_get_line_from_position (doc, pos);

	if( line == 0 ) return; // line 0 bail out

	len = document_get_line_length(doc, --line);

	if( len == 0 ) return; // length 0 bail out

	last_line = g_malloc0(sizeof(gchar)*(len+1));
	document_get_text_line (doc, line, last_line);
	
	if( !last_line ) return;

	cur_char = last_line;
	while( *cur_char  ){
		if( !isspace(*cur_char) || 
			*cur_char == '\n'  ||
			*cur_char == '\r'  
		){
			*cur_char = '\0';
			if( strlen(last_line) > 0 )
				document_add_text (doc, last_line);
			g_free( last_line );
			return;
		}
		++cur_char;
	}	
}

gboolean // returns true is the previous line starts in whitespace
document_last_line_is_indented ( CssedDoc* doc )
{
	gint pos, line;
	gchar cur_char;

	pos = document_get_current_position (doc);
	line = document_get_line_from_position (doc, pos);

	if( line == 0 ) return FALSE; // line 0 bail out

	cur_char = document_get_char_at(doc, document_get_position_from_line(doc, line-1));
	return  cur_char != '0' && 
		    ( isspace(cur_char) &&
			cur_char != '\n' &&
			cur_char != '\r' );

}
/// FIXME: this must be moved to a window function
void
fire_dialog_from_strings_or_write_output(CssedWindow* window,
										CssedDoc* tmpdoc,
										gchar* attribute,
										gchar* property ,
										gboolean add_previous)
{
	gchar *line_to_insert;
	gchar* strdialog;
	GtkWidget *dialog;
	CssDialogClass* klass;

	if( attribute[0] == '(' ){ // if it does not start with '(' is not a dialog
		if( (klass = (CssDialogClass*) cssed_window_css_dialog_lookup_by_keyword (window, attribute)) != NULL ){
			dialog = klass->create_function(window, g_strdup (property), add_previous, TRUE);
			gtk_widget_show (dialog);
		}
		else
		{ // dialog does not exist yet
			strdialog = g_strdup_printf(_("The dialog %s does not exist."),attribute);
			document_write_error_to_program_output(tmpdoc, strdialog, "black");

			if( add_previous ){
				line_to_insert =
				g_strconcat (property, ": ", attribute, ";",
					     	NULL);
				document_add_text_line (tmpdoc, line_to_insert);
				document_indent_as_last_line (tmpdoc);
				g_free (line_to_insert);
			}else{
				document_add_text (tmpdoc, " ");
				document_add_text (tmpdoc, attribute);
			}

			g_free(strdialog);
		}
	}
	else
	{// no dialog to show, write the selected strings
		if (tmpdoc != NULL)
		{
			if( add_previous ){
				line_to_insert = g_strconcat (property, ": ", attribute, ";", NULL);
				document_add_text_line (tmpdoc, line_to_insert);
				document_indent_as_last_line (tmpdoc);
				g_free (line_to_insert);
			}else{
				document_add_text (tmpdoc, " ");
				document_add_text (tmpdoc, attribute);
			}
		}
	}
}

// scintilla control access
GtkWidget* // you must use the SCINTILLA() macro to get the ScintillaObject and include scintilla headers
document_get_scintilla_widget ( CssedDoc* doc)
{
#ifdef WITH_SPLITTER_WINDOW
	if( document_splitted(doc) && doc->active_editor != NULL )
		return doc->active_editor;
	else
		return doc->editor;
#else		
	return doc->editor;
#endif
}

#ifdef WITH_SPLITTER_WINDOW
GtkWidget* // you must use the SCINTILLA() macro to get the ScintillaObject and include scintilla headers
document_get_primary_scintilla_widget ( CssedDoc* doc)
{
	return doc->editor;
}

GtkWidget* // you must use the SCINTILLA() macro to get the ScintillaObject and include scintilla headers
document_get_secondary_scintilla_widget ( CssedDoc* doc)
{
	return doc->split_editor;
}
#endif
// the widget attached to the notebook (needed for spiltter windows)
GtkWidget* 
document_get_container_widget (CssedDoc* doc) 
{
	return doc->container_box;
}

// callback func called by all editors when a signals arises
void
on_editor_notification( GtkWidget* editor,gint scn, gpointer lscn, gpointer user_data)
{
	struct SCNotification *nt;
	CssedDoc* tmpdoc;
	CssedWindow* window;
	CssedFileType* filetype;
	// used if lastchar == ':' in Autocompletion mode
	gchar* buffer;
	gint line;
	gint position;
	gint margin;
	gchar** uris;
	gint uris_index;
	gchar* uri;

	window = CSSED_WINDOW( user_data );
	tmpdoc = document_get_from_editor( window, editor );
	filetype = document_get_filetype(tmpdoc);

	nt = lscn;
	switch (nt->nmhdr.code){
		// open "file:///" uris dropped from file browser
		case SCN_URIDROPPED:
			uris_index = 0;
			uris = g_strsplit(nt->text,"\n",-1);
			while( uris[uris_index] != NULL ){		
#ifdef GTK_IS_2_2
				if( g_str_has_prefix(uris[uris_index],"file://") ){
#else
				if( strncmp(uris[uris_index], "file://",7 * sizeof(gchar)) == 0 ){
#endif	
#ifdef WIN32 // windows passes file:/// as unix but, before that there's a drive letter
					g_strstrip(uris[uris_index]);
					if( strstr(uris[uris_index], "%") ){
						cssed_string_unescape_uri( uris[uris_index] + 8, &uri );
					}else{
						uri = g_strdup(uris[uris_index] + 8);
					}
#else
					uri = g_strdup(uris[uris_index] + 7);
					g_strstrip(uri);
#endif

					if( g_file_test(uri, G_FILE_TEST_IS_REGULAR ) ){
						document_open_with_error_check(window, uri);
					}else{
						if( g_file_test(uri, G_FILE_TEST_IS_DIR ) ){
							buffer = g_strdup_printf(_("Cannot open %s, is a directory"),uri);
							document_write_error_to_program_output(tmpdoc,buffer,"brown");
							g_free(buffer);
						}else{
							buffer = g_strdup_printf(_("Cannot open %s, is not a regular file"),uri);
							document_write_error_to_program_output(tmpdoc,buffer,"brown");
							g_free(buffer);
						}
					}
					g_free(uri);
				}else{
					if( strlen(uris[uris_index]) > 1 ){
						buffer = g_strdup_printf(_("Cannot open file %s, protocol not supported"),uris[uris_index]);
						document_write_error_to_program_output(tmpdoc,buffer,"brown");
						g_free(buffer);
					}
				}
				uris_index++;
			}
			g_strfreev(uris);
			break;
		case SCN_CHARADDED:
			if( filetype != NULL )
				if( filetype->char_added != NULL )
					filetype->char_added(tmpdoc, (gchar) nt->ch);
			break;
		case SCN_UPDATEUI:
			document_new_undo_action(tmpdoc);
			break;
		case SCN_KEY:
			if( filetype != NULL )
				if( filetype->key_pressed != NULL )
					filetype->key_pressed(tmpdoc, (gchar) nt->ch, nt->modifiers);
			DBGMSG (__FILE__,__LINE__,"Pressed char %d modifier %d", nt->ch, nt->modifiers );
			break;
		case SCN_MODIFIED:
			if (nt->modificationType & SC_LASTSTEPINUNDOREDO) {
				if( !document_can_undo(tmpdoc) ){
					cssed_window_disable_save ( window );
					cssed_window_disable_undo( window );
				}else{
					cssed_window_enable_undo( window );
				}
				if( !document_can_redo(tmpdoc) ){
					cssed_window_disable_redo( window );
				}else{
					cssed_window_enable_redo( window );
				}
			} else if (nt->modificationType & (SC_MOD_INSERTTEXT | SC_MOD_DELETETEXT)) {
				cssed_window_enable_save (window);
				cssed_window_enable_undo (window);
				if( document_can_redo(tmpdoc) ){
					cssed_window_enable_redo (window);
				}else{
					cssed_window_disable_redo(window);
				}
			} else if ( nt->modificationType & SC_PERFORMED_UNDO ){
				if( document_get_modified(tmpdoc) ){
					cssed_window_enable_save(window);
				}
				if( document_can_redo(tmpdoc) ){
					cssed_window_enable_redo (window);
				}else{
					cssed_window_disable_redo(window);
				}
			} else if ( nt->modificationType & SC_PERFORMED_REDO ){
				if( document_get_modified(tmpdoc) ){
					cssed_window_enable_save(window);
				}
				if( document_can_redo(tmpdoc) ){
					cssed_window_enable_redo (window);
				}else{
					cssed_window_disable_redo(window);
				}
			}
			break;
		case SCN_MARGINCLICK:
			position = nt->position;
			margin = nt->margin;
			DBGMSG (__FILE__,__LINE__,"Clicked margin %d",margin);
			line = document_get_line_from_position( tmpdoc, position );
			// margin 0 (numbers), 1 (bookmarks) and 2 (folding) are 
			// managed here, higher margins are passed to file type's hook
			if( margin == 1){
				if( document_is_marker_set_at_line ( tmpdoc, line, 0 ) ){
					document_set_marker_at_line (tmpdoc, line, FALSE, 0 );
				}else{
					document_set_marker_at_line (tmpdoc, line, TRUE, 0 );
				}
			}else if( margin == 2 ){
				int level =document_get_fold_level(tmpdoc,line);
				if (level & SC_FOLDLEVELHEADERFLAG) {
					DBGMSG (__FILE__,__LINE__,"Fold toggled at line %d",line);
					document_toggle_fold_at_line (tmpdoc, line);
                }
 			}else if( margin > 2 ){ 
				if( filetype != NULL ){
					if( filetype->margin_click != NULL )
						filetype->margin_click (tmpdoc, position, margin);
				}
			}
			break;
		/* 	call file type funcs if any - those are never managed by cssed but
			by its file type managers */
		case SCN_USERLISTSELECTION:
			if( filetype != NULL ){
				if( filetype->user_list_selection != NULL )
					filetype->user_list_selection (tmpdoc, nt->ch, (gchar*) nt->text);
			}
			break;
		case SCN_SAVEPOINTREACHED:
			if( filetype != NULL ){
				if( filetype->save_point_reached != NULL )
					filetype->save_point_reached (tmpdoc);
			}
			break;
		case SCN_SAVEPOINTLEFT:
			if( filetype != NULL ){
				if( filetype->save_point_left != NULL )
					filetype->save_point_left (tmpdoc);
			}
			break;
		case SCN_MODIFYATTEMPTRO:
			if( filetype != NULL ){
				if( filetype->modify_attemp_read_only != NULL )
					filetype->modify_attemp_read_only (tmpdoc);
			}
			break;
		case SCN_DOUBLECLICK:
			if( filetype != NULL ){
				if( filetype->double_click != NULL )
					filetype->double_click(tmpdoc);
			}
			break;
		case SCN_MACRORECORD:
			break;
		case SCN_DWELLSTART:
			if( filetype != NULL ){
				if( filetype->dwell_start != NULL )
					filetype->dwell_start (tmpdoc, nt->position, nt->x, nt->y);
			}
			break;
		case SCN_DWELLEND:
			if( filetype != NULL ){
				if( filetype->dwell_end != NULL )
					filetype->dwell_end (tmpdoc, nt->position, nt->x, nt->y);
			}
			break;
		case SCN_HOTSPOTCLICK:
			if( filetype != NULL ){
				if( filetype->hot_spot_click != NULL )
					filetype->hot_spot_click (tmpdoc, nt->position, nt->modifiers);
			}
			break;
		case SCN_HOTSPOTDOUBLECLICK:
			if( filetype != NULL ){
				if( filetype->hot_spot_doubleclick != NULL )
					filetype->hot_spot_doubleclick (tmpdoc, nt->position, nt->modifiers);
			}
			break;
		case SCN_CALLTIPCLICK:
			if( filetype != NULL ){
				if( filetype->call_tip_click != NULL )
					filetype->call_tip_click (tmpdoc, nt->position);
			}
			break;
	}
}


#ifdef WITH_SPLITTER_WINDOW
gboolean
document_splitted_set_paned_position (gpointer data)
{
	CssedDoc* doc;
	
	doc = (CssedDoc*) data;
	SSM( SCINTILLA(doc->editor), SCI_SETCURSOR, SC_CURSORWAIT, 0 );
	SSM( SCINTILLA(doc->editor), SCI_SETCURSOR, SC_CURSORNORMAL, 0 );
	// set vpaned position	
	if( doc->split_type == CSSED_DOC_SPLIT_HORIZONTAL)
		gtk_paned_set_position(GTK_PANED(doc->split_paned), doc->container_box->allocation.height/2);
	else if( doc->split_type == CSSED_DOC_SPLIT_VERTICAL )
		gtk_paned_set_position(GTK_PANED(doc->split_paned), doc->container_box->allocation.width/2);	
		
	return FALSE;
}

void
document_split( CssedDoc* doc, CssedSplitType type )
{
	CssedWindow *window;
	ScintillaObject *split_sci;	
	
	if( doc->split_type == type ) return;
	
	window = document_get_window(doc);
	
	switch( type ){
		case CSSED_DOC_SPLIT_NONE:
			if( document_splitted (doc) ){
				g_object_ref(G_OBJECT(doc->split_paned));
				g_object_ref(G_OBJECT(doc->editor));
				gtk_widget_reparent(doc->editor, doc->container_box);
				g_object_unref(G_OBJECT(doc->split_paned));
				g_object_unref(G_OBJECT(doc->editor));				
				gtk_widget_destroy(doc->split_paned);			
				doc->split_paned = NULL;
				doc->split_editor = NULL;
				doc->split_type = CSSED_DOC_SPLIT_NONE;				
			}
			break;
		case CSSED_DOC_SPLIT_VERTICAL:
		case CSSED_DOC_SPLIT_HORIZONTAL:	
			if( !document_splitted (doc) ){

				if( type == CSSED_DOC_SPLIT_HORIZONTAL)
					doc->split_paned = gtk_vpaned_new();
				else if( type == CSSED_DOC_SPLIT_VERTICAL )
					doc->split_paned = gtk_hpaned_new();
					
				gtk_widget_set_size_request (doc->split_paned, 100, 100);
				gtk_widget_show(doc->split_paned);
				
				doc->split_editor = scintilla_new ();
				gtk_widget_set_size_request (doc->split_editor, 100, 100);
				split_sci = SCINTILLA (doc->split_editor);
				SSM(split_sci, SCI_SETDOCPOINTER, 0, (gint)doc->sci_document);	
								
				// add paned to vbox
				gtk_box_pack_start (GTK_BOX(doc->container_box),
									doc->split_paned,
									TRUE,
									TRUE,
									0);
				// add primary editor to paned
				gtk_widget_reparent (doc->editor,  doc->split_paned);
				
				// add secondary editor to paned
				gtk_paned_pack2 (GTK_PANED(doc->split_paned),
								doc->split_editor,
								TRUE,
								TRUE);	
				gtk_widget_show(doc->split_editor);
				// SET SPLIT STATE
				doc->split_type = type;
				doc->active_editor = doc->editor;				
				// connect callbacks
				g_signal_connect( doc->split_editor, "sci-notify", G_CALLBACK(on_editor_notification), (gpointer) window );
				/* This is unused now but will be used in the future to track function key press events and others
				 * The callback func is in callbacks.c
				g_signal_connect( editor, "key-press-event", G_CALLBACK(on_editor_key_press_event), (gpointer) window );
				*/
				// for the pop menu
				g_signal_connect( doc->split_editor, "button-press-event", G_CALLBACK(on_editor_button_press_event), (gpointer) window );
				// to delete values on the digest tree view - FIXME, should be part of a CssedFileType
				g_signal_connect( doc->split_editor, "grab-focus" , G_CALLBACK(on_editor_grab_focus), (gpointer) window );
				// configure secondary editor
				document_editor_set_default_configuration( doc, doc->split_editor );
				document_view_set_parent_configuration( doc, doc->split_editor );
				
				g_idle_add(document_splitted_set_paned_position, (gpointer) doc);
			}
			break;
		default:
			return;
	}
}

void
document_unsplit( CssedDoc* doc )
{
	document_split( doc, CSSED_DOC_SPLIT_NONE );
}

gboolean
document_splitted ( CssedDoc* doc )
{
	return doc->split_type != CSSED_DOC_SPLIT_NONE;
}
GtkWidget*
document_get_active_editor(CssedDoc* doc)
{
	if( document_splitted(doc) && doc->active_editor != NULL )
		return doc->active_editor;
	else
		return doc->editor;
}
void
document_set_active_editor(CssedDoc* doc, GtkWidget* editor)
{
	doc->active_editor = editor;
}
#endif

///////////////////////////////////////////////////////
ScintillaObject*
sci_get_from_document( CssedDoc* doc )
{
	ScintillaObject * sci;
#ifdef WITH_SPLITTER_WINDOW
	if( document_splitted(doc) && doc->active_editor )
		sci = SCINTILLA(doc->active_editor);
	else
		sci = SCINTILLA(doc->editor);	
#else
	sci = SCINTILLA(doc->editor);
#endif
	return sci;
}

gpointer // to be wrapped by the plugin's interface
document_send_scintilla_message ( CssedDoc* doc, guint iMessage, gulong wParam, glong lParam )
{
	ScintillaObject * sci;
	sci = SCINTILLA(document_get_scintilla_widget(doc));
	return (gpointer) SSM( sci, iMessage, wParam, lParam );
}
