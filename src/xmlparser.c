/*  cssed (c) Iago Rubio 2003-2005 - A tiny CSS editor.
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
#include <fcntl.h>
#include <gtk/gtk.h>
#include <string.h>

#ifndef WIN32
# include <sys/mman.h>
#endif

#include "cssedwindow.h"
#include "xmlparser.h"
#include "callbacks.h"
#include "support.h"
#include "debug.h"

/* globals */
int depth;

/* protos */
static void
fill_tree_view_start_tag_cb (void* data, const xmlChar *el, const xmlChar **attr);
static void
fill_tree_view_end_tag_cb (void *data, const xmlChar *el);
static void
xml_error(void *user_data, const char *msg, ...);

void
initialize_handler(xmlSAXHandler *handler)
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
    handler->startElement = fill_tree_view_start_tag_cb;
    handler->endElement = fill_tree_view_end_tag_cb;
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
	gchar *xmlerror, *message;

	data = (Tcssed_tp_data*) user_data;
	window = CSSED_WINDOW(data->window);
	
    va_start(args, msg);
    xmlerror = g_strdup_vprintf(msg, args);
    va_end(args);

	g_strstrip(xmlerror);
	message = g_strdup_printf( "%s: %s", data->filename, xmlerror);
	cssed_window_output_write(window, message);
	g_free(message);
	g_free(xmlerror);
}

/* parse the css definition, it don't validates against the DTD
	so ANY well formed XML will be parsed. Take in account that
	you can put garbled values on the treeview if you parse
	a not cssed related XML doc, resulting in a real mess !!
	// FIXME: change me to accept a filename as parameter.
*/
void
parse_css_definition_and_fill_tree_view( Tcssed_tp_data* data )
{
	CssedWindow* window;
	gchar* userfile = NULL;
	gchar* systemfile;
	G_CONST_RETURN gchar* home;
	gchar* globalfile;
	GtkTreeView* treeview ;
	gint retval;
	xmlSAXHandler handler;
#ifdef WIN32
	gchar* install_dir; 
#endif

	initialize_handler(&handler);
	window = CSSED_WINDOW(data->window);
	treeview = GTK_TREE_VIEW(cssed_window_get_css_definition_treeview(window));

#ifdef WIN32
	install_dir = g_win32_get_package_installation_directory(CSSED_REGISTRY_NAME, NULL);
	if( install_dir != NULL ){
		globalfile = g_strconcat(install_dir, "\\data\\cssed-def.xml", NULL);
		g_free(install_dir);
	}else{
		globalfile = g_strdup("\\data\\cssed-def.xml");
	}
#else  // _WIN32
	globalfile = PACKAGE_DATA_DIR "/" PACKAGE "/data/cssed-def.xml";
#endif
	// check if the user have a local definition file
	home = g_get_home_dir();
	if( home != NULL ){
		userfile = g_strconcat(home, G_DIR_SEPARATOR_S, ".cssed", G_DIR_SEPARATOR_S, "cssed-def.xml", NULL);
		if( g_file_test(userfile, G_FILE_TEST_IS_REGULAR| G_FILE_TEST_EXISTS) )
			systemfile = userfile;
		else
			systemfile = globalfile;		
	}else{
		systemfile = globalfile;
	}
	if( !g_file_test(systemfile, G_FILE_TEST_IS_REGULAR| G_FILE_TEST_EXISTS) ){
        cssed_error_message(_("XML definition file does not exists"), _("The definition file does not exists.\n%s\n\nThe CSS tree and CSS highlighting and auto completion will fail.\n Try to put a definition file in the .cssed directory in your home dir."),  systemfile) ;
#ifdef WIN32
		g_free( globalfile );
#endif 
		if(userfile) g_free(userfile);
		return;
	}

	DBGMSG (__FILE__,__LINE__,"Parsing CSS definition: Will try to use: %s", systemfile );
	data->filename = systemfile;

    if ( (retval=xmlSAXUserParseFile(&handler, data, systemfile)) != 0 ) {
        cssed_error_message(_("Error parsing XML definition file"), _("The parser found error on the xml file\n%s\n\nError code: %d\nCheck the output window for details."), systemfile, retval);
    } 
#ifdef WIN32
	g_free( globalfile );
#endif 
	if(userfile) g_free(userfile);
}

static void
fill_tree_view_start_tag_cb (void* data, const xmlChar *el, const xmlChar **attr)
{
	gint i;
	gchar* err_depth;
	GtkWidget* tree;
	GtkTreeStore *model;
	GtkTreeIter* toplevel, *child, *third;
	CssedWindow* window;
	Tcssed_tp_data* tp_data = (Tcssed_tp_data*) data;
	gchar* key_copy;

	window = CSSED_WINDOW(tp_data->window);
	toplevel =	tp_data->first_col;
	child =  	tp_data->second_col;
	third =  	tp_data->thrid_col;
	model =  	tp_data->store;
	tree = cssed_window_get_css_definition_treeview ( window );

	switch( depth ){
		case 0: // in depth 0 we've got the imp def and name
				for (i = 0; attr[i]; i += 2)
				{
					if( strcmp((const gchar*)attr[i], "implementation")==0){
						gtk_tree_store_append (model, toplevel, NULL);
						gtk_tree_store_set(model, toplevel, depth, attr[i + 1], -1);
					}
				}
		break;
		case 1: // in depth 1 will find reserved words ( properties )
				for (i = 0; attr[i]; i += 2)
				{
					if( strcmp( (const gchar*)attr[i], "name")==0){
						gtk_tree_store_append (model, child, toplevel);
						gtk_tree_store_set (model, child, depth, attr[i + 1] , -1);
						// add to keywordlist
						key_copy = g_strdup (cssed_window_get_keyword_list( window ));
						cssed_window_set_keyword_list (window, g_strjoin (" ",key_copy, attr[i + 1], NULL ));
						g_free (key_copy);
					}
				}
		break;
		case 2: // in depth 3 there are the properties valid values
				for (i = 0; attr[i]; i += 2)
				{
					if( strcmp((const gchar*) attr[i], "name")==0){
						gtk_tree_store_append (model, third, child);
						gtk_tree_store_set (model, third, depth, attr[i + 1], -1);
					}
				}
		break;
		default:
				// it must go out here
				err_depth = g_strdup_printf(
						_("<span color='brown'>* Bad XML definition file depth %d at element \"%s\". Max depth must be 3.</span>"),
						depth, el);
				cssed_window_output_write( window, err_depth );
				g_free(err_depth);
		break;
	}
	depth++;
}

static void
fill_tree_view_end_tag_cb (void *data, const xmlChar *el)
{
	depth--;
}
