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
 
/* 
	This parser is implemented using a subset of the libcroco (the parser)
	and using just it's SAC handler. Don't use it as a libcroco example as
	the libcroco used IS MODIFIED. Some functions doesn't exists or are private
	in the original libcroco. 
	
	Download the original libcroco from http://www.freespiders.org/
*/


#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#define PLAT_GTK 1

#include <stdio.h>
#include <gtk/gtk.h>

#include "cssedwindow.h"
#include "document.h"
#include "callbacks.h"
#include "interface.h"
#include "support.h"
#include "utils.h"

#include <libcroco.h>
#include <string.h>

#define OUT_COLOR "navy"
#define ERR_COLOR "red"
#define SEL_COLOR "blue"
#define PRO_COLOR "maroon"
#define MED_COLOR "dark green"

typedef struct _CssedDigestParserData {
	CRParser *parser;
	CssedDoc* doc;
	gboolean in_media;
	gint err_count;
	CssedWindow* window;
	gchar *filename;
} CssedDigestParserData;

/* *** UTILS *** */
void 
write_to_scanner_output(CRDocHandler *a_handler, gchar* style, gchar* color)
{
	CssedWindow* window;
	CssedDigestParserData* data;
	CRParser *parser;
	gulong line;
	gchar *linestr;
	gchar *stylestr;
	gchar *basename;
	GtkTreeView* view;
	GtkListStore* store;
	GtkTreeIter iter;
	GtkTreePath* path;
	
	g_return_if_fail (a_handler);	
	data = (CssedDigestParserData*) a_handler->app_data;
	window = data->window;
	parser = data->parser;
	
	line = cr_parser_get_pos(parser);
	
	linestr = g_strdup_printf("<span color='darkgreen'>%d</span>", (gint) line);
	stylestr = g_strdup_printf("<span color='%s'>%s</span>", color, style);
	basename = g_path_get_basename (data->filename);
	
	view = GTK_TREE_VIEW(cssed_window_get_digest_treeview(window));
	store = GTK_LIST_STORE(gtk_tree_view_get_model(view));
	gtk_list_store_append (store, &iter);
	gtk_list_store_set (store, &iter, 0, stylestr, 1, linestr, 2, basename, 3, data->filename, -1);	
	
	path = gtk_tree_model_get_path (GTK_TREE_MODEL(store),&iter);
	gtk_tree_view_scroll_to_cell (GTK_TREE_VIEW(view), path, NULL, FALSE, 0, 0);	
	gtk_tree_path_free(path);

	//cssed_window_digest_write( window, linestr,  string );
	g_free(linestr);
	g_free(stylestr);
	g_free(basename);
}

/* *** SAC HANDLERS START *** */
void
scanner_sac_handler_start_document (CRDocHandler * a_handler)
{
	CssedWindow* window;
	CssedDigestParserData* data;

	g_return_if_fail (a_handler);
	
	data = (CssedDigestParserData*) a_handler->app_data;
	window = data->window;	

	//cssed_window_digest_clear( window );
}

void
scanner_sac_handler_start_selector (CRDocHandler * a_handler,
			    CRSelector * a_selector_list)
{
	gchar *tmp_buf = NULL;
	gchar *out = NULL;
	gchar **selectors, **iterator;

	CssedDigestParserData* data;

	g_return_if_fail (a_handler);
	data = (CssedDigestParserData*) a_handler->app_data;	

	if (a_selector_list)
	{
		tmp_buf = (gchar*)cr_selector_to_string (a_selector_list);
		if (tmp_buf)
		{
			if( index(tmp_buf, ',') ){
				   iterator = selectors = g_strsplit(tmp_buf, ",", 0);
				   while( *iterator ){
						   out = g_strstrip(g_strdup_printf ("%s", *iterator));
						   write_to_scanner_output (a_handler, out, OUT_COLOR);
						   g_free (out);
						   iterator++;
				   }
				   g_strfreev(selectors);
			}else{
				   out = g_strdup_printf ("%s", tmp_buf);
				   write_to_scanner_output (a_handler, out, OUT_COLOR);
				   g_free (out);
			}
		}
	}
}

void
scanner_sac_handler_start_media (CRDocHandler * a_handler, GList * a_media_list, CRParsingLocation *a_location)
{
	g_return_if_fail (a_handler);
	write_to_scanner_output (a_handler, _("Media"), PRO_COLOR);
}

void
scanner_sac_handler_start_page (CRDocHandler *a_handler,
			    CRString *ar_name, 
			    CRString *ar_pseudo_page,
			    CRParsingLocation *a_location)
{
	g_return_if_fail (a_handler);
	write_to_scanner_output (a_handler, _("Page"), PRO_COLOR);
}

void
scanner_sac_handler_parse_error (CRDocHandler * a_handler, gint line)
{
	CssedDigestParserData* data;
	gchar* err;
	
	data = (CssedDigestParserData*) a_handler->app_data	;
	data->err_count++;	
	err = g_strdup_printf("syntax error (%d)",data->err_count);
	write_to_scanner_output (a_handler, err, ERR_COLOR);
	g_free(err);
}

void
scanner_init_sac_handler (CRDocHandler* a_handler)
{
	a_handler->start_document = scanner_sac_handler_start_document;
	a_handler->start_selector = scanner_sac_handler_start_selector;
	a_handler->start_media = scanner_sac_handler_start_media;
	a_handler->start_page = scanner_sac_handler_start_page;
	a_handler->error = scanner_sac_handler_parse_error;
	a_handler->end_document = NULL;
	a_handler->import_style = NULL; //
	a_handler->namespace_declaration = NULL;
	a_handler->comment = NULL;
	a_handler->end_selector = NULL;
	a_handler->property = NULL;
	a_handler->start_font_face = NULL; //
	a_handler->end_font_face = NULL;
	a_handler->end_media = NULL;
	a_handler->end_page = NULL;
	a_handler->ignorable_at_rule = NULL;
	a_handler->unrecoverable_error = NULL;
}

void
cssed_cr_parser_scan_doc_buffer (CssedDoc* doc, gchar* buffer, gint len)
{
	enum CRStatus status = CR_OK;
	CRDocHandler *handler = { 0 };
	CRParser *parser = NULL;
	CssedDigestParserData* data;
	CssedCodePage codepage;
	gchar* val_buf;
	GList* err_list;
	CRParserError* cr_err;
	CssedWindow* window;
	gchar* filename;
	
	data = g_malloc(sizeof(CssedDigestParserData));	
	data->doc = doc;
	data->err_count = 0;
	
	window = document_get_window (doc);
	data->window = window;

	filename = document_get_filename(doc);

	if( !filename )
		filename = g_strdup(_("Untitled"));
	data->filename = filename;

	codepage = document_get_codepage(doc);

	switch( codepage ){
		case CSSED_CODEPAGE_DEFAULT:
			parser = cr_parser_new_from_buf ((guchar*)buffer, len, CR_ISO_8859_1, FALSE);
			break;
		case CSSED_CODEPAGE_UTF8:
			parser = cr_parser_new_from_buf ((guchar*)buffer, len, CR_UTF_8, FALSE);
			break;
		case CSSED_CODEPAGE_DBCS:
			parser = cr_parser_new_from_buf ((guchar*)buffer, len, CR_UCS_4, FALSE);	
			break;		
		default:
			parser = cr_parser_new_from_buf ((guchar*)buffer, len, CR_AUTO, FALSE);
	}

	data->parser = parser;
	
	handler = cr_doc_handler_new ();
	scanner_init_sac_handler(handler);
	
	handler->app_data = data;
	status = cr_parser_set_sac_handler (parser, handler);

	if (status != CR_OK)
	{
		cr_parser_destroy (parser);
		g_free(data);
		g_free(filename);		
		return;
	}

	status = cr_parser_parse (parser);
	
	switch( status ){		
		case CR_OK:
			break;
		case  CR_ERROR:	
			cssed_error_message(_("Scanning fails"), _("Cannot scan bad formed document"));
			break;	
		default:
			cssed_error_message(_("Scanning fails"), _("Cannot scan bad formed document"));
			break;
	}
	
	 err_list = cr_parser_get_error_stack(parser);

    for ( ;err_list;err_list = err_list->next )
    {		
		cr_err = (CRParserError*) err_list->data;
		if ( cr_err != NULL ){
			val_buf = g_strdup_printf("%s:%d: %s ", filename, (gint) (cr_err->line + 1) ,cr_err->msg	);
			document_write_error_to_program_output( doc, val_buf, ERR_COLOR );
			g_free(val_buf);
		}
	}	
	cr_parser_destroy (parser);
	g_free(data);
	g_free(filename);
}
