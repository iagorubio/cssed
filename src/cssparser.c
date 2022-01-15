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

typedef struct _CssedCssParserData {
	CssedDoc *doc;
	gchar *current_selector;
	gboolean in_media;
	gint err_count;
	gchar *filename;
} CssedCssParserData;
	
/* *** UTILS *** */
CssedWindow *
get_window_fom_sac_handler_data (CRDocHandler * a_handler)
{
	CssedCssParserData* data;
	CssedDoc *document;
	CssedWindow *win;

	data = (CssedCssParserData*) a_handler->app_data;
	document = (CssedDoc *) data->doc;
	win = document_get_window (document);

	return win;
}

void 
change_current_sac_handler_selector(CRDocHandler * a_handler,gchar* selector)
{
	CssedCssParserData* data;	
	data = (CssedCssParserData*) a_handler->app_data;
	
	if( data->current_selector )
		g_free(data->current_selector);
	
	data->current_selector = g_strdup(selector);	
}

/* *** SAC HANDLERS START *** */
void
sac_handler_start_document (CRDocHandler * a_handler)
{
	CssedWindow* window;
	g_return_if_fail (a_handler);

	window = get_window_fom_sac_handler_data (a_handler);
	cssed_window_output_clear(window);
}

void
sac_handler_end_document (CRDocHandler * a_handler)
{
	g_return_if_fail (a_handler);
}

void
sac_handler_import_style (CRDocHandler *a_handler,
						  GList *a_media_list,
						  CRString *ar_uri,
						  CRString *ar_uri_default_ns,
						  CRParsingLocation *a_location)
{
	GList *cur = NULL;
	gchar *str;
	gchar *out;
	CssedCssParserData* data;
	GString *a_uri, *a_uri_default_ns;


	g_return_if_fail (ar_uri);
	g_return_if_fail (ar_uri_default_ns);	
	g_return_if_fail (a_handler);

	a_uri = ar_uri->stryng;
	a_uri_default_ns = ar_uri_default_ns->stryng;

	data = (CssedCssParserData*) a_handler->app_data;

	document_write_error_to_program_output ((CssedDoc *) data->doc,
						_("Input style from import found."),
						OUT_COLOR);

	if (a_media_list)
	{
		document_write_error_to_program_output ((CssedDoc *) data->doc,
							_("\tMedia list"),
							OUT_COLOR);

		for (cur = a_media_list; cur; cur = cur->next)
		{
			if (cur->data)
			{
				str = g_strndup
					((const gchar*)((GString *) cur->data)->str,
					 ((GString *) cur->data)->len);

				if (str)
				{
					out = g_strdup_printf ("\t\t%s", str);
					document_write_error_to_program_output((CssedDoc *) data->doc, out, OUT_COLOR);
					g_free (str);
					g_free (out);
					str = NULL;
					out = NULL;
				}
			}
		}

		document_write_error_to_program_output ((CssedDoc *)data->doc, _("\tDefault namespace"), OUT_COLOR);
		if (a_uri_default_ns && a_uri_default_ns->str)
		{
			str = g_strndup ((const gchar*)a_uri_default_ns->str, a_uri_default_ns->len);
			if (str)
			{
				out = g_strdup_printf ("\t\t%s", str);
				document_write_error_to_program_output ((CssedDoc *) data->doc, out, OUT_COLOR);
				g_free (str);
				g_free (out);
			}
		}
	}
}


void
sac_handler_namespace_declaration (CRDocHandler *a_handler,
								   CRString *ar_prefix,
								   CRString *ar_uri,
								   CRParsingLocation *a_location)

{
	GString *a_prefix, *a_uri;
	gchar *prefix = NULL;
	gchar *uri = NULL;
	gchar *out = NULL;
	CssedCssParserData* data;


	g_return_if_fail (ar_uri);
	g_return_if_fail (ar_prefix);	
	g_return_if_fail (a_handler);

	data = (CssedCssParserData*) a_handler->app_data;	
	a_prefix = ar_prefix->stryng;
	a_uri = ar_uri->stryng;

	document_write_error_to_program_output ((CssedDoc *) data->doc,
						_("parser: Namespace declaration"),	OUT_COLOR);

	if (a_prefix && a_prefix->str)
	{
		prefix = g_strndup ((const gchar*)a_prefix->str, a_prefix->len);

		if (prefix)
		{
			out = g_strdup_printf (_("\t\tprefix: %s"), prefix);
			document_write_error_to_program_output ((CssedDoc *)
								data->doc, out,	OUT_COLOR);
			g_free (prefix);
			g_free (out);
		}
	}

	if (a_uri && a_uri->str)
	{

		uri = g_strndup ((const gchar*)a_uri->str, a_uri->len);

		if (uri)
		{
			out = g_strdup_printf (_("\t\turi: %s"), uri);
			document_write_error_to_program_output ((CssedDoc *)
								data->
								doc, out,
								OUT_COLOR);
			g_free (prefix);
			g_free (out);
		}
	}
}


void
sac_handler_comment    (CRDocHandler *a_handler,
						CRString *ar_comment) 
{
	gchar *out = NULL;
	gchar *comment = NULL;
	CssedCssParserData* data;
	GString * a_comment;

	g_return_if_fail (ar_comment);	
	g_return_if_fail (a_handler);

	data = (CssedCssParserData*) a_handler->app_data;	
	a_comment = ar_comment->stryng;
	if (a_comment && a_comment->str)
	{
		comment = g_strndup ((const gchar*)a_comment->str, a_comment->len);

		if (comment)
		{
			out = g_strdup_printf (_("comment -> %s"), comment);
			document_write_error_to_program_output ((CssedDoc*) data->doc, out,	OUT_COLOR);
			g_free (comment);
			g_free (out);
			out = NULL;
			comment = NULL;
		}
	}
}


void
sac_handler_start_selector (CRDocHandler * a_handler,
							CRSelector * a_selector_list)
{
	gchar *tmp_buf = NULL;
	gchar *out = NULL;
	CssedCssParserData* data;


	g_return_if_fail (a_selector_list);	
	g_return_if_fail (a_handler);

	data = (CssedCssParserData*) a_handler->app_data;	

	if (a_selector_list)
	{
		tmp_buf = (gchar*)cr_selector_to_string (a_selector_list);
		if (tmp_buf)
		{
			data->current_selector = g_strdup(tmp_buf);
			out = g_strdup_printf (_("<span color='black'>%s</span>:<span color='dark green'>%d</span>:\n%s"), 
															data->filename, 
															a_selector_list->location.line,
															tmp_buf);
			document_write_error_to_program_output ((CssedDoc*) data->doc, out, SEL_COLOR);
			g_free (out);
			g_free (tmp_buf);
		}
	}
}


void
sac_handler_end_selector   (CRDocHandler * a_handler,
							CRSelector * a_selector_list)
{
	guchar *tmp_buf = NULL;
	gchar *out = NULL;
	CssedCssParserData* data;

	g_return_if_fail (a_selector_list);	
	g_return_if_fail (a_handler);

	data = (CssedCssParserData*) a_handler->app_data;	

	if (a_selector_list)
	{
		tmp_buf = cr_selector_to_string (a_selector_list);
		if (tmp_buf)
		{
			out = g_strdup_printf (_("%s end"), tmp_buf);
			document_write_error_to_program_output ((CssedDoc*) data-> doc, out, SEL_COLOR);
			g_free (out);
			g_free (tmp_buf);
			tmp_buf = NULL;
			out = NULL;
		}
	}
}

void
sac_handler_property (CRDocHandler *a_handler,
						CRString *ar_name,
						CRTerm *a_expr,
						gboolean a_is_important)
{
	gchar *out = NULL;
	gchar* val;	
	gchar *name ;
	CssedCssParserData* data;
	GString * a_name;

	g_return_if_fail (ar_name);	
	g_return_if_fail (a_expr);
	g_return_if_fail (a_handler);

	data = (CssedCssParserData*) a_handler->app_data;	
	a_name = ar_name->stryng;

	if (a_name && a_name->str)
	{
		name = g_strndup ((const gchar*)a_name->str, a_name->len);

		if (name && a_expr)
		{
			val = (gchar*)cr_term_to_string (a_expr) ;
			out=g_strdup_printf(_("\t%s: %s"), name, val);
			document_write_error_to_program_output ((CssedDoc *) data->doc,	out, PRO_COLOR);
			g_free(out);
			g_free(val);
		}
		else
		{
			if( name ){
				out=g_strdup_printf(_("\tempty property -> %s"),name);
				document_write_error_to_program_output ((CssedDoc *) data->doc, out, ERR_COLOR);
				g_free( out );
				
			}
			if( a_expr ){
				val=g_strdup_printf(_("\tempty value -> %s"),name);
				document_write_error_to_program_output ((CssedDoc *) data->doc, val, ERR_COLOR);
				g_free( val );				
			}
		}

		if (name)
			g_free (name);
	}
}

void
sac_handler_start_font_face (CRDocHandler * a_handler, CRParsingLocation *a_location)
{
	CssedCssParserData* data;
	g_return_if_fail (a_handler);

	data = (CssedCssParserData*) a_handler->app_data;	
	document_write_error_to_program_output ((CssedDoc *)
						data->doc, _("Start font face"),
						OUT_COLOR);
}


void
sac_handler_end_font_face (CRDocHandler * a_handler)
{
	CssedCssParserData* data;
	g_return_if_fail (a_handler);

	data = (CssedCssParserData*) a_handler->app_data;	
	document_write_error_to_program_output ((CssedDoc *)
						data->doc, _("End font face"),
						OUT_COLOR);
}

void
sac_handler_start_media (CRDocHandler * a_handler, GList * a_media_list, CRParsingLocation *a_location)
{
	CssedCssParserData* data;
	gchar *out;

	g_return_if_fail (a_handler);
	g_return_if_fail (a_location);

	data = (CssedCssParserData*) a_handler->app_data	;

	out = g_strdup_printf (_("<span color='black'>%s</span>:<span color='dark green'>%d</span>:\nMedia"), 
															data->filename, 
															a_location->line);
	document_write_error_to_program_output ((CssedDoc *) data->doc, out, MED_COLOR);
	g_free(out);
}


void
sac_handler_end_media (CRDocHandler * a_handler, GList * a_media_list)
{
	CssedCssParserData* data;

	g_return_if_fail (a_handler);
	data = (CssedCssParserData*) a_handler->app_data;

	document_write_error_to_program_output ((CssedDoc *) data->doc,
											_("End Media"), MED_COLOR);

}

void
sac_handler_start_page (CRDocHandler *a_handler,
						CRString *ar_name, 
						CRString *ar_pseudo_page,
						CRParsingLocation *a_location) 

{
	CssedCssParserData* data;
	gchar *out;

	g_return_if_fail (a_handler);
	data = (CssedCssParserData*) a_handler->app_data;
	out = g_strdup_printf (_("<span color='black'>%s</span>:<span color='dark green'>%d</span>:\nPage"), 
															data->filename, 
															a_location->line);
	document_write_error_to_program_output ((CssedDoc*) data->doc, out, MED_COLOR);	
	g_free(out);
}

void
sac_handler_end_page (CRDocHandler *a_handler,
					  CRString *ar_name,
					  CRString *ar_pseudo_page)
{
	CssedCssParserData* data;

	g_return_if_fail (a_handler);
	data = (CssedCssParserData*) a_handler->app_data;
	document_write_error_to_program_output ((CssedDoc*) data->doc, _("End Page"), MED_COLOR);
}


void
sac_handler_ignorable_at_rule (CRDocHandler * a_handler,  CRString *ar_name)
{
	gchar *name = NULL;
	GString * a_name;

	g_return_if_fail (a_handler);
	g_return_if_fail (ar_name);

	a_name = ar_name->stryng;

	fprintf (stdout, "*********************\n");
	fprintf (stdout, "ignorable_at_rule\n");

	if (a_name && a_name->str)
	{
		name = (gchar*)g_strndup ((const gchar*)a_name->str, a_name->len);
	}

	if (name)
	{
		fprintf (stdout, "%s\n", name);
		g_free(name);
	}

	fprintf (stdout, "*********************\n\n");
}

void
sac_handler_parse_error (CRDocHandler * a_handler, gint line)
{
	CssedCssParserData* data;
	gchar* err;
	
	data = (CssedCssParserData*) a_handler->app_data	;
	data->err_count++;	
	err = g_strdup_printf(_("%s:%d: syntax error (%d)"), data->filename, line+1, data->err_count);
	document_write_error_to_program_output ((CssedDoc *) data->doc, err, ERR_COLOR);
	g_free(err);
}

void
sac_handler_fatal_error (CRDocHandler * a_handler)
{
	CssedCssParserData* data;
	data = (CssedCssParserData*) a_handler->app_data	;

	document_write_error_to_program_output ((CssedDoc *) data->doc,	_("FATAL error"), ERR_COLOR);
}

void
init_sac_handler (CRDocHandler * a_handler)
{
	a_handler->start_document = sac_handler_start_document;
	a_handler->end_document = sac_handler_end_document;
	a_handler->import_style = sac_handler_import_style;
	a_handler->namespace_declaration = sac_handler_namespace_declaration;
	a_handler->comment = sac_handler_comment;
	a_handler->start_selector = sac_handler_start_selector;
	a_handler->end_selector = sac_handler_end_selector;
	a_handler->property = sac_handler_property;
	a_handler->start_font_face = sac_handler_start_font_face;
	a_handler->end_font_face = sac_handler_end_font_face;
	a_handler->start_media = sac_handler_start_media;
	a_handler->end_media = sac_handler_end_media;
	a_handler->start_page = sac_handler_start_page;
	a_handler->end_page = sac_handler_end_page;
	a_handler->ignorable_at_rule = sac_handler_ignorable_at_rule;
	a_handler->error = sac_handler_parse_error;
	a_handler->unrecoverable_error = sac_handler_fatal_error;
}

void
init_no_dump_sac_handler (CRDocHandler * a_handler)
{
	a_handler->start_document = sac_handler_start_document; // clean previous output
	a_handler->end_document = NULL;
	a_handler->import_style = NULL;
	a_handler->namespace_declaration = NULL;
	a_handler->comment = NULL;
	a_handler->start_selector = NULL;
	a_handler->end_selector = NULL;
	a_handler->property = NULL;
	a_handler->start_font_face = NULL;
	a_handler->end_font_face = NULL;
	a_handler->start_media = NULL;
	a_handler->end_media = NULL;
	a_handler->start_page = NULL;
	a_handler->end_page = NULL;
	a_handler->ignorable_at_rule = NULL;
	a_handler->error = sac_handler_parse_error;
	a_handler->unrecoverable_error = NULL;
}
/* *** SAC HANDLERS END *** */




void
cssed_cr_parser_parse_buffer (CssedDoc * doc, gchar * buffer, gint len, gboolean dump)
{
	enum CRStatus status = CR_OK;
	CRDocHandler *handler = { 0 };
	CRParser *parser = NULL;
	CssedCssParserData* data;
	gchar* val_buf;
	gchar *filename;
	GList* err_list;
	CRParserError* cr_err;
	CssedCodePage codepage;

	data = g_malloc(sizeof(CssedCssParserData));	
	data->doc = doc;
	data->err_count = 0;
	data->current_selector = NULL;
	data->in_media = FALSE;
	
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

	handler = cr_doc_handler_new ();

	if( dump )
		init_sac_handler (handler);
	else
		init_no_dump_sac_handler (handler);
	
	handler->app_data = data;
	status = cr_parser_set_sac_handler (parser, handler);

	if (status != CR_OK)
	{//FIXME say somethig to user
		cr_parser_destroy (parser);
		g_free(data);
		g_free(filename);
		return;
	}

	//g_print("validating buffer: %s\n", buffer);
	status = cr_parser_parse (parser);
	switch( status ){		
		case CR_OK:
			if( data->err_count > 0 ){
				cssed_error_message(_("Validation result ok with errors"), 
									_("The CSS document can be parsed, but has syntax errors (%d).\nCheck the program output window."),	
									data->err_count);
			}else{
				cssed_error_message(_("Validation result ok"),
									_("The CSS document is well formed and\ncan be parsed by a browser."));
			}
			break;
		case  CR_ERROR:	
			cssed_error_message(_("Validation fails"), _("The CSS document is not well formed"));
			break;	
		case  CR_BAD_PARAM_ERROR:
		case  CR_INSTANCIATION_FAILED_ERROR:
		case  CR_EMPTY_PARSER_INPUT_ERROR:
		case  CR_ENCODING_ERROR:
		case  CR_ENCODING_NOT_FOUND_ERROR:
			cssed_error_message(_("Validation fails"),
								_("The validator found an internal error\nand is unable to continue."));
			break;
		default:
			cssed_error_message(_("Validation fails"),
								_("The CSS document is not well formed"));
			break;
	}
	
	err_list = cr_parser_get_error_stack(parser);

    for ( ;err_list;err_list = err_list->next )
    {
		cr_err = (CRParserError*) err_list->data;
		if ( cr_err != NULL ){
			val_buf = g_strdup_printf("%s:%d: %s ", filename, ((int) cr_err->line + 1) ,cr_err->msg	);
			document_write_error_to_program_output(doc,val_buf,ERR_COLOR);
			g_free(val_buf);
		}
	}

	if( err_list ) g_list_free( err_list );
	cr_parser_destroy (parser);

	if( data->current_selector ) g_free(data->current_selector);
	g_free(data);
	g_free(filename);
}

