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

#include <libxml/parser.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdarg.h>
#include <fcntl.h>
#include <string.h>
#include <gtk/gtk.h>

#ifndef WIN32
# include <sys/mman.h>
#endif

#include "cssedwindow.h"
#include "xmlparser.h"
#include "support.h"
#include "utils.h"
#include "plugin.h"
#include "debug.h"

enum {
	PARSER_STATE_IN_START,
	PARSER_STATE_IN_ROOT,
	PARSER_STATE_IN_WINDOW,
	PARSER_STATE_IN_DOCUMENT,
	PARSER_STATE_IN_OPENED_DOCS,
	PARSER_STATE_IN_HIGHLIGHTING,
	PARSER_STATE_IN_PLUGINS,
	PARSER_STATE_ERROR
};

/* protos */
static void
config_start_tag_cb (void* data, const xmlChar *el, const xmlChar **attr);
static void
config_end_tag_cb (void *data, const xmlChar *el);
void
parse_style_attributes (CssedConfigParserData* data, const xmlChar **attr);
static void
xml_error(void *user_data, const char *msg, ...);

void
initialize_config_handler(xmlSAXHandler *handler)
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
	handler->startElement = config_start_tag_cb;
	handler->endElement = config_end_tag_cb;
	handler->reference = NULL;
	handler->characters = NULL;
	handler->ignorableWhitespace = NULL;
	handler->processingInstruction = NULL;
	handler->comment = NULL;
	handler->warning = NULL;
	handler->error = &xml_error;
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

static void
xml_error(void *user_data, const char *msg, ...) {
	va_list args;
	CssedWindow *window;
	Tcssed_tp_data* data;
	gchar *xmlerror;


	data = (Tcssed_tp_data*) user_data;
	window = CSSED_WINDOW(data->window);

	va_start(args, msg);
	xmlerror = g_strdup_vprintf(msg, args);
	va_end(args);

	g_strstrip(xmlerror);
	cssed_error_message("Configuration file error", xmlerror);
	g_free(xmlerror);
}

/* parse the css definition, it doesn't validate against the DTD yet
	so ANY well formed XML will be parsed.
	Will return a plugins list to be loaded when window is created.
	FIXME: This must change to a separated function.
*/
GList*
parse_configuration_file (CssedWindow* window)
{
	gint retval;
	gchar* configfile;
	CssedConfigParserData* parser_data;
	GList* plugins_list;
	G_CONST_RETURN gchar* home;
	xmlSAXHandler handler;

	initialize_config_handler(&handler);

	parser_data = g_malloc(sizeof(CssedConfigParserData));
	parser_data->window = window;
	parser_data->depth = 0;
	parser_data->state = PARSER_STATE_IN_START;
	parser_data->plugins_list = NULL;

	home = g_get_home_dir();
	if( home != NULL ){
		configfile = g_strconcat(home, G_DIR_SEPARATOR_S, ".cssed", G_DIR_SEPARATOR_S, "cssed-cfg.xml",NULL );
	if( !g_file_test(configfile,G_FILE_TEST_IS_REGULAR| G_FILE_TEST_EXISTS) )
			return NULL; // no configuration yet
	}else{
		return NULL; // no home so no configuration file
	}
	DBGMSG (__FILE__,__LINE__,"User's config exist: %s", configfile );
	if ( (retval=xmlSAXUserParseFile(&handler, parser_data, configfile)) != 0 ) {
		cssed_error_message(_("Error parsing XML definition file"), 
							_("The parser found error on the xml file\n%s\n\nError code: %d."),
							configfile,
							retval);
		return NULL;
	} 

	if( parser_data->state != PARSER_STATE_ERROR && parser_data->plugins_list != NULL ){
		DBGMSG (__FILE__,__LINE__,"config plugins: %d", g_list_length(parser_data->plugins_list));
		plugins_list = parser_data->plugins_list;
		DBGMSG (__FILE__,__LINE__,"config will return: %d - %p", g_list_length( parser_data->plugins_list ), plugins_list);
		g_free(parser_data);
		g_free(configfile);
		return plugins_list;
	}else{
		g_free(parser_data);
		g_free(configfile);
		return NULL;
	}
}

static void
config_start_tag_cb (void* data, const xmlChar *el, const xmlChar **attr)
{
	gint i;
	CssedWindow* window;
	CssedConfigParserData* tp_data;
	CssedConfig* cfg;

	tp_data = (CssedConfigParserData*) data;
	window = tp_data->window;
	cfg = cssed_window_get_config(window);

	if( tp_data->state == PARSER_STATE_ERROR) return;

	switch( tp_data->depth ){
		case 0: //
				if( strcmp((const gchar*)el, "cssed-conf")!=0 )
					tp_data->state = PARSER_STATE_ERROR;
				else
					tp_data->state = PARSER_STATE_IN_ROOT;
		break;
		case 1: //
			if( tp_data->state != PARSER_STATE_IN_ROOT )
				tp_data->state = PARSER_STATE_ERROR;

			if( strcmp((const gchar*)el, "window") == 0){
				tp_data->state = PARSER_STATE_IN_WINDOW;				
				for (i = 0; attr[i]; i += 2)
				{  
					if( strcmp((const gchar*)attr[i], "x")==0){	
						if( char_ptr_is_decimal_num((const gchar*)attr[i + 1]) )
							cfg->window_x = atoi((const gchar*)attr[i + 1]);
					}else if( strcmp((const gchar*)attr[i], "y")==0){	
						if( char_ptr_is_decimal_num((const gchar*)attr[i + 1]) )
							cfg->window_y = atoi((const gchar*)attr[i + 1]);
					}else if( strcmp((const gchar*)attr[i], "width")==0){
						if( char_ptr_is_decimal_num((const gchar*)attr[i + 1]) )
							cfg->window_width = atoi((const gchar*)attr[i + 1]);
					}else if( strcmp((const gchar*)attr[i], "height")==0){
						if( char_ptr_is_decimal_num((const gchar*)attr[i + 1]) )
							cfg->window_height = atoi((const gchar*)attr[i + 1]);
					}else if( strcmp((const gchar*)attr[i], "maximized")==0){
						if( strcmp((const gchar*)attr[i + 1], "true" ) == 0 )
							cfg->initiall_state_maximized = TRUE;
						else
							cfg->initiall_state_maximized = FALSE;
					}else{
						tp_data->state = PARSER_STATE_ERROR;
					}
				}
			}else if( strcmp((const gchar*)el, "document") == 0 )
				tp_data->state = PARSER_STATE_IN_DOCUMENT;
			else if( strcmp((const gchar*)el, "opened-docs") == 0 )
				tp_data->state = PARSER_STATE_IN_OPENED_DOCS;
			else if( strcmp((const gchar*)el, "highlighting") == 0 )
				tp_data->state = PARSER_STATE_IN_HIGHLIGHTING;
			else if( strcmp((const gchar*)el, "plugins") == 0 )
				tp_data->state = PARSER_STATE_IN_PLUGINS;
			else if( strcmp((const gchar*)el, "browser") == 0 ){
				for (i = 0; attr[i]; i += 2)
				{  
					if( strcmp((const gchar*)attr[i], "command") == 0){
						if( cfg->browser_command ) g_free( cfg->browser_command );
						cfg->browser_command = g_strdup((const gchar*)attr[i + 1]);
					}
					tp_data->state = PARSER_STATE_IN_ROOT;
				}
			}
			else
				tp_data->state = PARSER_STATE_ERROR;
		break;
		case 2: // 
			switch( tp_data->state ){
				case PARSER_STATE_IN_WINDOW:
					if( strcmp((const gchar*)el, "footer") == 0){
						if( strcmp((const gchar*)attr[0], "position") == 0 ){
							if( char_ptr_is_decimal_num((const gchar*)attr[1]) )	cfg->footer_pos = atoi((const gchar*)attr[1]);
						}else{
							tp_data->state = PARSER_STATE_ERROR;
						}
					}else if( strcmp((const gchar*)el, "sidebar" ) == 0){
						if( strcmp((const gchar*)attr[0], "position") == 0 ){
							if( char_ptr_is_decimal_num((const gchar*)attr[1]) ) cfg->sidebar_pos = atoi((const gchar*)attr[1]);
						}else{
							tp_data->state = PARSER_STATE_ERROR;
						}
					}else if( strcmp((const gchar*)el, "csstreeview" ) == 0){	
						if( strcmp((const gchar*)attr[0], "position") == 0 ){
							if( char_ptr_is_decimal_num((const gchar*)attr[1]) ) cfg->css_treeview_pos = atoi((const gchar*)attr[1]);
						}else{
							tp_data->state = PARSER_STATE_ERROR;
						}
					}else{
						tp_data->state = PARSER_STATE_ERROR;
					}
					break;
				case PARSER_STATE_IN_DOCUMENT:
					if( strcmp((const gchar*)el, "linenumbers" ) == 0){
						if( strcmp((const gchar*)attr[0], "show") == 0 ){
							if( strcmp((const gchar*)attr[1], "true" ) == 0 )
								cfg->linenumbers = TRUE;
							else
								cfg->linenumbers = FALSE;
						}else{
							tp_data->state = PARSER_STATE_ERROR;
						}
					}else if( strcmp((const gchar*)el, "autocompletion" ) == 0){
						if( strcmp((const gchar*)attr[0], "enabled") == 0 ){
							if( strcmp((const gchar*)attr[1], "true" ) == 0 )
								cfg->autocompletion = TRUE;
							else
								cfg->autocompletion = FALSE;
						}else{
							tp_data->state = PARSER_STATE_ERROR;
						}
					}else if( strcmp((const gchar*)el, "lineendings" ) == 0){	
						if( strcmp((const gchar*)attr[0], "show") == 0 ){
							if( strcmp((const gchar*)attr[1], "true" ) == 0 )
								cfg->lineendings = TRUE;
							else
								cfg->lineendings = FALSE;
						}else{
							tp_data->state = PARSER_STATE_ERROR;
						}
					}else if( strcmp((const gchar*)el, "whitespaces" ) == 0){	
						if( strcmp((const gchar*)attr[0], "show") == 0 ){
							if( strcmp((const gchar*)attr[1], "true" ) == 0 )
								cfg->whitespaces = TRUE;
							else
								cfg->whitespaces = FALSE;
						}else{
							tp_data->state = PARSER_STATE_ERROR;
						}
					}else if( strcmp((const gchar*)el, "folding" ) == 0){	
						if( strcmp((const gchar*)attr[0], "enabled") == 0 ){
							if( strcmp((const gchar*)attr[1], "true" ) == 0 )
								cfg->folding = TRUE;
							else
								cfg->folding = FALSE;
						}else{
							tp_data->state = PARSER_STATE_ERROR;
						}	
					}else if( strcmp((const gchar*)el, "lineswraped" ) == 0){	
						if( strcmp((const gchar*)attr[0], "show") == 0 ){
							if( strcmp((const gchar*)attr[1], "true" ) == 0 )
								cfg->lineswraped = TRUE;
							else
								cfg->lineswraped = FALSE;
						}else{
							tp_data->state = PARSER_STATE_ERROR;
						}							
					}else if( strcmp((const gchar*)el, "font" ) == 0){	
						for (i = 0; attr[i]; i += 2)
						{ 
							if( strcmp((const gchar*)attr[i], "name") == 0 )
								cfg->font = g_strdup((const gchar*)attr[i+1]);
							else if( strcmp((const gchar*)attr[i], "size") == 0 )
								cfg->font_size = atoi((const gchar*)attr[i+1]);
							else
								tp_data->state = PARSER_STATE_ERROR;
						}
					}else if( strcmp((const gchar*)el, "usetabs" ) == 0){	
						if( strcmp((const gchar*)attr[0], "value") == 0 ){
							if( strcmp((const gchar*)attr[1], "true" ) == 0 )
								cfg->usetabs = TRUE;
							else
								cfg->usetabs = FALSE;
						}else{
							tp_data->state = PARSER_STATE_ERROR;
						}							
					}else if( strcmp((const gchar*)el, "tabswidth" ) == 0){	
						if( strcmp((const gchar*)attr[0], "value") == 0 ){
                            if( char_ptr_is_decimal_num((const gchar*)attr[1]) ) cfg->tabswidth = atoi((const gchar*)attr[1]);
						}else{
							tp_data->state = PARSER_STATE_ERROR;
						}							
					}else{
						tp_data->state = PARSER_STATE_ERROR;
					}
					break;
				case PARSER_STATE_IN_OPENED_DOCS: // UNFINISHED FIXME 
					if( strcmp((const gchar*)el, "doc") == 0){
						if( strcmp((const gchar*)attr[0], "path") == 0 ){
							// value attr[1] true
						}else{
							tp_data->state = PARSER_STATE_ERROR;
						}
					}else {
						tp_data->state = PARSER_STATE_ERROR;
					}
					break;
				case PARSER_STATE_IN_PLUGINS:
					if( strcmp((const gchar*)el, "plugin" ) == 0){
						if( strcmp((const gchar*)attr[0], "file") == 0 ){
							DBGMSG (__FILE__,__LINE__, "config read plugin found file: %s", attr[1] );
							tp_data->plugins_list = g_list_append( tp_data->plugins_list, g_strdup((const gchar*) attr[1] ) );
						}else{
							tp_data->state = PARSER_STATE_ERROR;
						}
					}else {
						tp_data->state = PARSER_STATE_ERROR;
					}
					break;					
				case PARSER_STATE_IN_HIGHLIGHTING:
					if( strcmp((const gchar*)el, "style" ) == 0)
						parse_style_attributes( tp_data, attr );
					else
						tp_data->state = PARSER_STATE_ERROR;
					break;
				default:
					tp_data->state = PARSER_STATE_ERROR;
				break;				
			}
		break;
		default:
			cssed_error_message (_("Configuration file error"), 
								 _("Bad XML configuration file\nDepth %d at element %s\nMax deptph must be 3"),
								 tp_data->depth, el);
			tp_data->state = PARSER_STATE_ERROR;
		break;
	}
	tp_data->depth++;
}

static void
config_end_tag_cb (void *data, const xmlChar *el)
{
	CssedConfigParserData* pdata;
	
	pdata = (CssedConfigParserData*) data; 
	pdata->depth--;
}

void
parse_style_attributes( CssedConfigParserData* data, const xmlChar **attr)
{
	CssedWindow* window;
	CssedConfig* cfg;
	gchar* value = NULL;
	long int fore = -1;
	long int back = -1;
	gboolean bold = FALSE;
	gboolean bold_found = FALSE;
	gint i;
	
	window = data->window;
	cfg = cssed_window_get_config( window );	
	
	for (i = 0; attr[i]; i += 2)
	{
		if( strcmp((const gchar*)attr[i], "name")==0){				
			value = (gchar*)attr[i + 1];			
		}else if( strcmp((const gchar*)attr[i], "fore")==0 ){			
			fore = strtol((const gchar*)attr[i + 1], NULL, 16);
			if( fore == LONG_MIN || fore == LONG_MAX ){
				fore = -1; // out of range
			}
		}else if( strcmp((const gchar*)attr[i], "back")==0 ){
			back = strtol((const gchar*)attr[i + 1], NULL, 16);
			if( back == LONG_MIN || back == LONG_MAX ){
				back = -1; // out of range
			}			
		}else if( strcmp((const gchar*)attr[i], "bold")==0 ){
			bold = (strcmp((const gchar*)attr[i + 1], "true" )==0)?TRUE:FALSE;
			bold_found = TRUE;
		}
	}	

	if( value == NULL){
		data->state = PARSER_STATE_ERROR;
		return;
	}
	
	if( strcmp((const gchar*)value,"DEFAULT") == 0){
		if( fore >= 0 && fore <= 0xffffff )
			cfg->DEFAULT_fore_color = fore;
		if( back >= 0 && back <= 0xffffff )
			cfg->DEFAULT_back_color = back;
		if( bold_found )
			cfg->DEFAULT_bold = bold;
	}else if( strcmp((const gchar*)value,"TAG") == 0){
		if( fore >= 0 && fore <= 0xffffff )
			cfg->TAG_fore_color = fore;
		if( back >= 0 && back <= 0xffffff )
			cfg->TAG_back_color = back;
		if( bold_found )
			cfg->TAG_bold = bold;
	}else if( strcmp((const gchar*)value,"PSEUDOCLASS") == 0){
		if( fore >= 0 && fore <= 0xffffff )
			cfg->PSEUDOCLASS_fore_color = fore;
		if( back >= 0 && back <= 0xffffff )
			cfg->PSEUDOCLASS_back_color = back;
		if( bold_found )
			cfg->PSEUDOCLASS_bold = bold;	
	}else if( strcmp((const gchar*)value,"UNKNOWN_PSEUDOCLASS") == 0){
		if( fore >= 0 && fore <= 0xffffff )
			cfg->UNKNOWN_PSEUDOCLASS_fore_color = fore;
		if( back >= 0 && back <= 0xffffff )
			cfg->UNKNOWN_PSEUDOCLASS_back_color = back;
		if( bold_found )
			cfg->UNKNOWN_PSEUDOCLASS_bold = bold;	
	}else if( strcmp((const gchar*)value,"OPERATOR") == 0){
		if( fore >= 0 && fore <= 0xffffff )
			cfg->OPERATOR_fore_color = fore;
		if( back >= 0 && back <= 0xffffff )
			cfg->OPERATOR_back_color = back;
		if( bold_found )
			cfg->OPERATOR_bold = bold;
	}else if( strcmp((const gchar*)value,"IDENTIFIER") == 0){
		if( fore >= 0 && fore <= 0xffffff )
			cfg->IDENTIFIER_fore_color = fore;
		if( back >= 0 && back <= 0xffffff )
			cfg->IDENTIFIER_back_color = back;
		if( bold_found )
			cfg->IDENTIFIER_bold = bold;
	}else if( strcmp((const gchar*)value,"UNKNOWN_IDENTIFIER") == 0){
		if( fore >= 0 && fore <= 0xffffff )
			cfg->UNKNOWN_IDENTIFIER_fore_color = fore;
		if( back >= 0 && back <= 0xffffff )
			cfg->UNKNOWN_IDENTIFIER_back_color = back;
		if( bold_found )
			cfg->UNKNOWN_IDENTIFIER_bold = bold;
	}else if( strcmp((const gchar*)value,"VALUE") == 0){
		if( fore >= 0 && fore <= 0xffffff )
			cfg->VALUE_fore_color = fore;
		if( back >= 0 && back <= 0xffffff )
			cfg->VALUE_back_color = back;
		if( bold_found )
			cfg->VALUE_bold = bold;
	}else if( strcmp((const gchar*)value,"COMMENT") == 0){
		if( fore >= 0 && fore <= 0xffffff )
			cfg->COMMENT_fore_color = fore;
		if( back >= 0 && back <= 0xffffff )
			cfg->COMMENT_back_color = back;
		if( bold_found )
			cfg->COMMENT_bold = bold;
	}else if( strcmp((const gchar*)value,"IMPORTANT") == 0){
		if( fore >= 0 && fore <= 0xffffff )
			cfg->IMPORTANT_fore_color = fore;
		if( back >= 0 && back <= 0xffffff )
			cfg->IMPORTANT_back_color = back;
		if( bold_found )
			cfg->IMPORTANT_bold = bold;
	}else if( strcmp((const gchar*)value,"DIRECTIVE") == 0){
		if( fore >= 0 && fore <= 0xffffff )
			cfg->DIRECTIVE_fore_color = fore;
		if( back >= 0 && back <= 0xffffff )
			cfg->DIRECTIVE_back_color = back;
		if( bold_found )
			cfg->DIRECTIVE_bold = bold;
	}else if( strcmp((const gchar*)value,"DOUBLESTRING") == 0){
		if( fore >= 0 && fore <= 0xffffff )
			cfg->DOUBLESTRING_fore_color = fore;
		if( back >= 0 && back <= 0xffffff )
			cfg->DOUBLESTRING_back_color = back;
		if( bold_found )
			cfg->DOUBLESTRING_bold = bold;
	}else if( strcmp((const gchar*)value,"SINGLESTRING") == 0){
		if( fore >= 0 && fore <= 0xffffff )
			cfg->SINGLESTRING_fore_color = fore;
		if( back >= 0 && back <= 0xffffff )
			cfg->SINGLESTRING_back_color = back;
		if( bold_found )
			cfg->SINGLESTRING_bold = bold;
	}else if( strcmp((const gchar*)value,"CLASS") == 0){
		if( fore >= 0 && fore <= 0xffffff )
			cfg->CLASS_fore_color = fore;
		if( back >= 0 && back <= 0xffffff )
			cfg->CLASS_back_color = back;
		if( bold_found )
			cfg->CLASS_bold = bold;
	}else if( strcmp((const gchar*)value,"ID") == 0){
		if( fore >= 0 && fore <= 0xffffff )
			cfg->ID_fore_color = fore;
		if( back >= 0 && back <= 0xffffff )
			cfg->ID_back_color = back;
		if( bold_found )
			cfg->ID_bold = bold;
	}else if( strcmp((const gchar*)value,"VALID_VALUE") == 0){
		if( fore >= 0 && fore <= 0xffffff )
			cfg->VALID_VALUE_fore_color = fore;
		if( back >= 0 && back <= 0xffffff )
			cfg->VALID_VALUE_back_color = back;
		if( bold_found )
			cfg->VALID_VALUE_bold = bold;
	}else if( strcmp((const gchar*)value,"FUNCTION") == 0){
		if( fore >= 0 && fore <= 0xffffff )
			cfg->DEFAULT_fore_color = fore;
		if( back >= 0 && back <= 0xffffff )
			cfg->DEFAULT_back_color = back;
		if( bold_found )
			cfg->DEFAULT_bold = bold;
	}else if( strcmp((const gchar*)value,"NUMBER") == 0){
		if( fore >= 0 && fore <= 0xffffff )
			cfg->NUMBER_fore_color = fore;
		if( back >= 0 && back <= 0xffffff )
			cfg->NUMBER_back_color = back;
		if( bold_found )
			cfg->NUMBER_bold = bold;
	}else if( strcmp((const gchar*)value,"UNIT") == 0){
		if( fore >= 0 && fore <= 0xffffff )
			cfg->UNIT_fore_color = fore;
		if( back >= 0 && back <= 0xffffff )
			cfg->UNIT_back_color = back;
		if( bold_found )
			cfg->UNIT_bold = bold;
	}else if( strcmp((const gchar*)value,"COLOR") == 0){
		if( fore >= 0 && fore <= 0xffffff )
			cfg->COLOR_fore_color = fore;
		if( back >= 0 && back <= 0xffffff )
			cfg->COLOR_back_color = back;
		if( bold_found )
			cfg->COLOR_bold = bold;
	}else if( strcmp((const gchar*)value,"HEXACOLOR") == 0){
		if( fore >= 0 && fore <= 0xffffff )
			cfg->HEXACOLOR_fore_color = fore;
		if( back >= 0 && back <= 0xffffff )
			cfg->HEXACOLOR_back_color = back;
		if( bold_found )
			cfg->HEXACOLOR_bold = bold;
	}else if( strcmp((const gchar*)value,"ATTR_MATCH") == 0){
		if( fore >= 0 && fore <= 0xffffff )
			cfg->ATTR_MATCH_fore_color = fore;
		if( back >= 0 && back <= 0xffffff )
			cfg->ATTR_MATCH_back_color = back;
		if( bold_found )
			cfg->ATTR_MATCH_bold = bold;
	}else if( strcmp((const gchar*)value,"LANGUAGE") == 0){
		if( fore >= 0 && fore <= 0xffffff )
			cfg->LANGUAGE_fore_color = fore;
		if( back >= 0 && back <= 0xffffff )
			cfg->LANGUAGE_back_color = back;
		if( bold_found )
			cfg->LANGUAGE_bold = bold;
	}else{
		data->state = PARSER_STATE_ERROR;
		return;
	}	
}

