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

#include <gtk/gtk.h>
#include <string.h>

#include "cssedwindow.h"
#include "document.h"
#include "support.h"
#include "debug.h"

enum {
	HAVE_NO_QUOTE ,
	HAVE_DOUBLE_QUOTES,
	HAVE_SINGLE_QUOTES
};
/* Not used yet, but don't delete please 
enum {
	CSSED_TOKEN_DOSP = G_TOKEN_LAST + 1,
	CSSED_TOKEN_PC = G_TOKEN_LAST + 2
};
// symbol array 
static const struct {
  gchar *symbol_name;
  guint  symbol_token;
} symbols[] = {
  { ":", CSSED_TOKEN_, },
  { ";", CSSED_TOKEN_, },
  { NULL, 0, }
}, *p_symbol = symbols;
*/
static const GScannerConfig css_scanner_config =
{
	("\t\r\n" )             	/* cset_skip_characters */,
	(
		G_CSET_a_2_z
		"0123456789#-!"
		 G_CSET_A_2_Z
	)                        	/* cset_identifier_first */,
	(
		G_CSET_a_2_z
		"_-.0123456789#%/"       // the dot must be moved in the future to symbols
		G_CSET_A_2_Z
	)                        	/* cset_identifier_nth */,
	( "" )         		 		/* cpair_comment_single */,				   
	TRUE                     	/* case_sensitive */,
	TRUE                      	/* skip_comment_multi */,
	TRUE                      	/* skip_comment_single */,
	TRUE                      	/* scan_comment_multi */,
	TRUE                      	/* scan_identifier */,
	TRUE                     	/* scan_identifier_1char */,
	TRUE                     	/* scan_identifier_NULL */,
	FALSE                     	/* scan_symbols */,
	TRUE                      	/* scan_binary */,
	TRUE                      	/* scan_octal */,
	TRUE                      	/* scan_float */,
	TRUE                      	/* scan_hex */,
	FALSE                      	/* scan_hex_dollar */,
	TRUE                 	  	/* scan_string_sq */,
	TRUE                 	  	/* scan_string_dq */,
	TRUE                      	/* numbers_2_int - all nums converted to int*/,
	FALSE                     	/* int_2_float */,
	FALSE                     	/* identifier_2_string */,
	TRUE                 	  	/* char_2_token */,	
	TRUE                     	/* symbol_2_token - don't return the simbol but it's identifier*/,
	FALSE                     	/* scope_0_fallback */,
};
/* prototypes */
gchar* 		css_buffer_clean			(CssedWindow* window, gchar* text);
GScanner*	css_scanner_new				(void);
gchar*		css_buffer_add_string		(gchar* string, gchar* buffer);
void 		css_parser_write_error		(CssedDoc* doc, gchar* message, 
										 gint buffer_line, 
										 gint buffer_start_line);
gint		css_buffer_have_quotes		(gchar* buffer);

/* This is called from the "Scan selector" menu "activate" event. Scans the selector
	and fill the list store data. */
void 
parser_css_selector_buffer_to_list( CssedWindow* window, gchar* buffer )
{
	gchar** property_and_value;
	gchar** tokens;
	gint index = 0;
	
	// cleanup the store
	cssed_window_selector_scanner_clear( window );
	// it will clean up the buffer and other tasks
	buffer = css_buffer_clean(window, buffer);
	
	tokens = g_strsplit(buffer,";",-1);
	
	while( tokens[index] != NULL ){
		if( strlen(	tokens[index] ) > 0 ){ // avoid empty strings	
			g_strstrip(tokens[index]);
			property_and_value = g_strsplit(tokens[index],":",2);
			if( (property_and_value[0] != NULL) && (property_and_value[1] != NULL) ){
				if( (strlen(property_and_value[0]) > 0) || (strlen(property_and_value[1]) > 0) ){	
					cssed_window_selector_scanner_write( window, 
														property_and_value[0], 
														g_strstrip(property_and_value[1]));
					DBGMSG (__FILE__,__LINE__,"Property - %s Value - %s",
									property_and_value[1],
									property_and_value[0]
							);
				}
			}		
			g_strfreev(property_and_value);
		}	
		index++;	
	}
	g_strfreev (tokens);
	if(buffer) g_free(buffer);
	cssed_window_ensure_selector_scanner_visible( window );
}

/* Will clean the buffer, and check if well formed ( syntax only )
	needs to strip out comments */
gchar* // prototype at top of this file
css_buffer_clean(CssedWindow* window, gchar* text)
{
	GScanner* scanner;
	GTokenType type;
	gboolean done = FALSE;
	gboolean declaration_done;
	gchar* cleaned;
	gint line;
	gint pos;
	gint document_line;
	gint document_pos;
	CssedDoc* doc;
	
	doc = document_get_current( window );
	cssed_window_output_clear( window );
	
	document_pos = document_get_selection_start(doc);
	document_line = document_get_line_from_position(doc, document_pos);
	
	scanner = css_scanner_new();
	g_scanner_input_text (scanner,text,strlen(text) );
	
	type = g_scanner_get_next_token (scanner);
	scanner->input_name = "cssed scanner";
		
	// Symbols are not used yet, but will be used. 
	// DON'T DELETE ME PLEASE, THANK YOU ;-)	
	/* load symbols into the scanner 
	while (p_symbol->symbol_name)
	{
		  g_scanner_add_symbol (scanner,
								p_symbol->symbol_name,
								GINT_TO_POINTER (p_symbol->symbol_token));
		  p_symbol++;
	}
	//g_scanner_set_scope(scanner,0);
*/
	
	cleaned = g_strdup(""); // start with empty - but not NULL - string
	while (!done)
	{		
		declaration_done = FALSE;
		
		if( type == G_TOKEN_EOF ){
			g_scanner_destroy (scanner);
			g_free( text );
			return cleaned;
		}
		g_scanner_peek_next_token (scanner);
		if (scanner->next_token == G_TOKEN_EOF) {
			done = TRUE;
		}	
	
		line = g_scanner_cur_line(scanner);
		pos = g_scanner_cur_position(scanner);
		
		if( (type == G_TOKEN_COMMENT_MULTI) ){
			type = g_scanner_get_next_token (scanner);
			g_scanner_peek_next_token (scanner);
			if (scanner->next_token == G_TOKEN_EOF) {
				done = TRUE;
			}		
		}		
		
		while( type == ' ' ){ // skip white chars at start
			type = g_scanner_get_next_token (scanner);
			g_scanner_peek_next_token (scanner);		
		}
		
		if( type == G_TOKEN_IDENTIFIER ) {
				cleaned = css_buffer_add_string	(scanner->value.v_identifier, cleaned);
		}else{ // just for info
			switch( type ){
				case G_TOKEN_LEFT_PAREN:
				case G_TOKEN_RIGHT_PAREN:
				case G_TOKEN_RIGHT_CURLY:	
				case G_TOKEN_LEFT_CURLY:
				case G_TOKEN_LEFT_BRACE:
				case G_TOKEN_RIGHT_BRACE:	
				case G_TOKEN_COMMA:
				case G_TOKEN_EQUAL_SIGN:
					css_parser_write_error(doc,_("special character found where must be an identifier. Stop scanning .."), line, document_line);
				break;
				case G_TOKEN_INT:
				case G_TOKEN_FLOAT:				
					css_parser_write_error(doc,_("number found where must be an identifier. Stop scanning .."), line, document_line);
				break;	
				case G_TOKEN_STRING:
					css_parser_write_error(doc,_("quoted strings are not valid identifiers. Stop scanning .."), line, document_line);
				break;
				default:					
					switch(scanner->token) {
						case ':':
							css_parser_write_error(doc,_("sentence separator found but no property identifier. Stop scanning .."), line, document_line);
						break;						
						case ';':			
							css_parser_write_error(doc,_("end of sentence found but no identifier. Stop scanning .."), line, document_line);
						break;
						default:
							css_parser_write_error(doc,_("bad property is not a valid identifier. Stop scanning .."), line, document_line);
						break;
					}
					break;
			}
			g_scanner_destroy (scanner);
			g_free( text );
			return cleaned;
		}
		
		type = g_scanner_get_next_token (scanner);
		if( type == ':' ){
			cleaned = css_buffer_add_string	(":", cleaned);
		}else{
			css_parser_write_error(doc,_("bad declaration separator, must be \":\""), line, document_line);
			g_scanner_destroy (scanner);
			g_free( text );
			g_free(cleaned);
			return g_strdup("");
		}
		if( g_scanner_peek_next_token (scanner) == G_TOKEN_EOF ){
			g_scanner_destroy (scanner);
			g_free( text );
			return cleaned;
		}
		type = g_scanner_get_next_token (scanner);
		while( !declaration_done )  {
			if( type == G_TOKEN_EOF ){
				g_scanner_destroy (scanner);
				g_free( text );
				return cleaned;
			}else{
				switch( type ) {	
					case G_TOKEN_LEFT_PAREN:
						cleaned = css_buffer_add_string	("(", cleaned); 
						/* 
						This function must be striped in parts. One of them
						must go for the right paren or end of declaration
						*/	
						break;
					case G_TOKEN_RIGHT_PAREN:
						cleaned = css_buffer_add_string	(")", cleaned);
						break;		
					case  G_TOKEN_LEFT_CURLY:
						cleaned = css_buffer_add_string	("{", cleaned);
						css_parser_write_error(doc,_("opening curly braces are not allowed into a selector"), line, document_line);
						break;
					case  G_TOKEN_RIGHT_CURLY:
						cleaned = css_buffer_add_string	("}", cleaned);
						css_parser_write_error(doc,_("closing curly braces are not allowed into a selector"), line, document_line);
						break;
					case  G_TOKEN_LEFT_BRACE:
						cleaned = css_buffer_add_string	("[", cleaned);
						break;
					case  G_TOKEN_RIGHT_BRACE:
						g_strconcat(cleaned, "]",NULL );
						break;					
					case  G_TOKEN_EQUAL_SIGN:
						cleaned = css_buffer_add_string	( "=", cleaned);
						css_parser_write_error(doc,_("equal sign not allowed into a selector"), line, document_line);
						break;
					case  G_TOKEN_COMMA:
						cleaned = css_buffer_add_string	( ",", cleaned);							
						break;		
					case  G_TOKEN_INT:
						{
							gchar* value;
							value = g_strdup_printf("%d", (gint) scanner->value.v_int );
							cleaned = css_buffer_add_string	( value, cleaned);
							g_free( value );
						}							
						break;	
					case  G_TOKEN_FLOAT:
						{
							gchar* value;
							value = g_strdup_printf("%f", scanner->value.v_float);
							cleaned = css_buffer_add_string	( value, cleaned);
							g_free( value );
						}		
						break;
					case  G_TOKEN_STRING:
						{
							gint have_quotes = css_buffer_have_quotes( scanner->value.v_string );

							switch( have_quotes ){
								case HAVE_SINGLE_QUOTES:									
								case HAVE_NO_QUOTE:
									{
										gchar* value = g_strdup_printf("\"%s\"", scanner->value.v_string );
										cleaned = css_buffer_add_string	( value, cleaned);
										g_free( value );
									}		
									break;
								case HAVE_DOUBLE_QUOTES:
									{
										gchar* value = g_strdup_printf("'%s'", scanner->value.v_string );
										cleaned = css_buffer_add_string	( value, cleaned);
										g_free( value );
									}	
									break;
							}
						}
						break;
					case  G_TOKEN_SYMBOL:
						DBGMSG (__FILE__,__LINE__,"found SYMBOL at line %d pos %d\n",line, pos);					
						break;				
					case  G_TOKEN_IDENTIFIER:
						{
							gchar* value;
							value = g_strdup_printf("%s", scanner->value.v_identifier );
							cleaned = css_buffer_add_string	(value, cleaned);
							g_free( value );
						}	
						break;
					case G_TOKEN_COMMENT_SINGLE: // avoid comments
					case G_TOKEN_COMMENT_MULTI:
						break;
					default:					
						switch(type) {
							case ':':
									css_parser_write_error(doc,
										_("error, more than one sentence separator into one sentence"),
										line, document_line);
								break;
							case ';':			
									cleaned = css_buffer_add_string	( ";", cleaned);								
									declaration_done = TRUE;
								break;
							case '/':			
									cleaned = css_buffer_add_string	( ";", cleaned);								
									declaration_done = TRUE;
								break;		
							case '-':			
									cleaned = css_buffer_add_string	( "-", cleaned);								
									declaration_done = TRUE;
								break;
							case ' ':
									cleaned = css_buffer_add_string	( " ", cleaned);
								break;								
							default:
									{
										gchar* msg = g_strdup_printf(_("invalid token type (%d). Ignoring."),type);
										css_parser_write_error(doc,msg, line, document_line);
										g_free(msg);
									}
							break;
						}
						break;											
					}
					if(!declaration_done){
						if( (g_scanner_peek_next_token (scanner) == G_TOKEN_EOF) || 
							(g_scanner_peek_next_token (scanner) == G_TOKEN_NONE)  ){
							declaration_done = TRUE;
						}else{
							type = g_scanner_get_next_token (scanner);
						}								
					}
				}
			}					
			type = g_scanner_get_next_token (scanner);			
	}	
	g_scanner_destroy (scanner);
	g_free( text );
	return cleaned;
}

GScanner* // prototype at top of this file
css_scanner_new()
{
	return  g_scanner_new (&css_scanner_config);
}
/*  This function is only to be used with the scanner as it clean up and tokenize
	the strings sending only each string content without nested quotes:
	When the scanner finds "1'2'3"'4"5''6"7"'"8" it will tokenize
		1'2'3
		4"5
		6"7"
		8
	If you send strings containing nested quotes it will be bad evaluated returning
	the first quotes it find.
*/
gint
css_buffer_have_quotes( gchar* buffer )
{
	gchar* buffer_end;
	gchar* iterator;
	
	iterator = buffer;
	buffer_end = buffer + (strlen(buffer) * sizeof(gchar));
	
	while( iterator <= buffer_end ){
		if( *iterator == '"' )	return HAVE_DOUBLE_QUOTES;
		if( *iterator == '\'' )	return HAVE_SINGLE_QUOTES;
		iterator++;
	}
	return HAVE_NO_QUOTE;
}

gchar*
css_buffer_add_string( gchar* string, gchar* buffer )
{
	gchar* previous;
	
	previous = g_strdup( buffer );
	g_free( buffer );
	buffer = g_strconcat(previous, string ,NULL );
	g_free( previous );	
	return buffer; // return new buffer address	
}

void 
css_parser_write_error(CssedDoc* doc, gchar* message, gint buffer_line, gint buffer_start_line)
{
	gchar* msg;
	gint line;
	gchar *filename;

	line = buffer_line + buffer_start_line + 1;
	filename = document_get_filename(doc);
	if( filename ){
		msg	= g_strdup_printf(_("%s:%d - selector scanner: %s"), filename, line, message );
		g_free(filename);
	}else{
		msg	= g_strdup_printf(_("[%d] - selector scanner: %s"), line, message );
	}
	document_write_error_to_program_output(doc, msg, ERROR_COLOR_STRING);
	g_free(msg);	
}
