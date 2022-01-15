#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#ifndef PLAT_GTK
#   define PLAT_GTK 1
#endif

#include <gtk/gtk.h>

#include "cssedwindow.h"
#include "document.h"
#include "file-type-manager.h"
#include "support.h"
#include "debug.h"
#include "utils.h"

#include <Scintilla.h>
#include <SciLexer.h>
#include <ScintillaWidget.h>

void
filetype_xml_set_style(CssedDoc* doc, GtkWidget *editor)
{
	ScintillaObject * sci;
	CssedWindow* window;
	CssedConfig* cfg;
	GtkWidget *scintilla;

	const gchar sgmlKeyWords[] = "ELEMENT DOCTYPE ATTLIST ENTITY NOTATION xml";

	window = document_get_window(doc);
	cfg = cssed_window_get_config(window);
	
	if( editor ) scintilla = editor;
	else scintilla = document_get_scintilla_widget(doc);
	
	sci = SCINTILLA(scintilla);

	SSM (sci, SCI_SETPROPERTY, (sptr_t) "fold.html", (sptr_t) "1");
	SSM (sci, SCI_SETPROPERTY, (sptr_t) "fold.html.preprocessor", (sptr_t) "1");

	SSM (sci, SCI_STYLECLEARALL, 0, 0);
	SSM (sci, SCI_SETLEXER, SCLEX_XML, 0);

	SSM(sci, SCI_SETKEYWORDS, 5, (sptr_t) sgmlKeyWords);

	SSM(sci, SCI_STYLESETFORE, SCE_H_DEFAULT, 0x000000);
	SSM(sci, SCI_STYLESETBACK, SCE_H_DEFAULT, 0xffffff);

	SSM(sci, SCI_STYLESETFORE, SCE_H_TAG, 0x990000);
	SSM(sci, SCI_STYLESETBACK, SCE_H_TAG, 0xffffff);

	SSM(sci, SCI_STYLESETFORE, SCE_H_TAGUNKNOWN, 0x990000);
	SSM(sci, SCI_STYLESETBACK, SCE_H_TAGUNKNOWN, 0xffffff);

	SSM(sci, SCI_STYLESETFORE, SCE_H_ATTRIBUTE, 0x990066);
	SSM(sci, SCI_STYLESETBACK, SCE_H_ATTRIBUTE, 0xffffff);

	SSM(sci, SCI_STYLESETFORE, SCE_H_ATTRIBUTEUNKNOWN, 0x990066);
	SSM(sci, SCI_STYLESETBACK, SCE_H_ATTRIBUTEUNKNOWN, 0xffffff);

	SSM(sci, SCI_STYLESETFORE, SCE_H_NUMBER, 0x008040);
	SSM(sci, SCI_STYLESETBACK, SCE_H_NUMBER, 0xffffff);

	SSM(sci, SCI_STYLESETFORE, SCE_H_DOUBLESTRING, 0x008000);
	SSM(sci, SCI_STYLESETBACK, SCE_H_DOUBLESTRING, 0xffffff);

	SSM(sci, SCI_STYLESETFORE, SCE_H_SINGLESTRING, 0x008000);
	SSM(sci, SCI_STYLESETBACK, SCE_H_SINGLESTRING, 0xffffff);

	SSM(sci, SCI_STYLESETFORE, SCE_H_OTHER, 0x800080);
	SSM(sci, SCI_STYLESETBACK, SCE_H_OTHER, 0xffffff);

	SSM(sci, SCI_STYLESETFORE, SCE_H_COMMENT, 0x808080);
	SSM(sci, SCI_STYLESETBACK, SCE_H_COMMENT, 0xffffff);

	SSM(sci, SCI_STYLESETFORE, SCE_H_ENTITY, 0x800080);
	SSM(sci, SCI_STYLESETBACK, SCE_H_ENTITY, 0xffffff);

	SSM(sci, SCI_STYLESETFORE, SCE_H_TAGEND, 0x800000);
	SSM(sci, SCI_STYLESETBACK, SCE_H_TAGEND, 0xffffff);

	SSM(sci, SCI_STYLESETFORE, SCE_H_XMLSTART, 0x990000);// <?
	SSM(sci, SCI_STYLESETBACK, SCE_H_XMLSTART, 0xf0f0f0);
	SSM(sci, SCI_STYLESETEOLFILLED, SCE_H_XMLSTART, 1);

	SSM(sci, SCI_STYLESETFORE, SCE_H_XMLEND, 0x990000);// ?>
	SSM(sci, SCI_STYLESETBACK, SCE_H_XMLEND, 0xf0f0f0);

	SSM(sci, SCI_STYLESETFORE, SCE_H_SGML_DEFAULT, 0x990066);
	SSM(sci, SCI_STYLESETBACK, SCE_H_SGML_DEFAULT, 0xffffff);

	SSM(sci, SCI_STYLESETFORE, SCE_H_SGML_COMMAND, 0x990000);
	SSM(sci, SCI_STYLESETBACK, SCE_H_SGML_COMMAND, 0xffffff);

	SSM(sci, SCI_STYLESETFORE, SCE_H_SGML_1ST_PARAM, 0x808000);
	SSM(sci, SCI_STYLESETBACK, SCE_H_SGML_1ST_PARAM, 0xf0f0f0);

	SSM(sci, SCI_STYLESETFORE, SCE_H_SGML_DOUBLESTRING, 0x008000);
	SSM(sci, SCI_STYLESETBACK, SCE_H_SGML_DOUBLESTRING, 0xffffff);

	SSM(sci, SCI_STYLESETFORE, SCE_H_SGML_SIMPLESTRING, 0x008000);
	SSM(sci, SCI_STYLESETBACK, SCE_H_SGML_SIMPLESTRING, 0xffffff);

	SSM(sci, SCI_STYLESETFORE, SCE_H_SGML_ERROR, 0x000080);
	SSM(sci, SCI_STYLESETBACK, SCE_H_SGML_ERROR, 0xffffff);

	SSM(sci, SCI_STYLESETFORE, SCE_H_SGML_SPECIAL, 0x008080);
	SSM(sci, SCI_STYLESETBACK, SCE_H_SGML_SPECIAL, 0xffffff);

	SSM(sci, SCI_STYLESETFORE, SCE_H_SGML_ENTITY, 0x808060);
	SSM(sci, SCI_STYLESETBACK, SCE_H_SGML_ENTITY, 0xffffff);

	SSM(sci, SCI_STYLESETFORE, SCE_H_SGML_COMMENT, 0x808080);
	SSM(sci, SCI_STYLESETBACK, SCE_H_SGML_COMMENT, 0xf0f0f0);

	SSM(sci, SCI_STYLESETFORE, SCE_H_SGML_1ST_PARAM_COMMENT, 0x808080);
	SSM(sci, SCI_STYLESETBACK, SCE_H_SGML_1ST_PARAM_COMMENT, 0xf0f0f0);

	SSM(sci, SCI_STYLESETFORE, SCE_H_SGML_BLOCK_DEFAULT, 0x808040);
	SSM(sci, SCI_STYLESETBACK, SCE_H_SGML_BLOCK_DEFAULT, 0xffffff);
}
/* ********************************** */

void
create_and_register_xml_file_type (CssedWindow* window)
{
	CssedFileType* xml;
	GtkWidget* xml_menu;

	xml_menu = gtk_menu_item_new_with_label("XML");

	xml = cssed_file_type_new();
	xml->char_added  = NULL;

	xml->modify_attemp_read_only = NULL;
	xml->double_click = NULL;
	xml->user_list_selection = NULL;
	xml->dwell_start = NULL;
	xml->dwell_end = NULL;
	xml->hot_spot_click = NULL;
	xml->hot_spot_doubleclick = NULL;
	xml->call_tip_click = NULL;

	xml->save_point_reached = NULL;
	xml->save_point_left = NULL;
	xml->uri_dropped = NULL;
	xml->margin_click = NULL;
	xml->modified = NULL;
	xml->key_pressed = NULL;

	xml->apply_style = filetype_xml_set_style;

	xml->get_config_page_widget = NULL;
	xml->save_configuration_from_widget = NULL;	
	xml->pop_menu = NULL;
	xml->clean_popmenu = NULL;
	xml->menu_item = xml_menu;
	xml->clean_menu = NULL;
	xml->load_ui = NULL;

	// file related issues, those are mostly to avoid to open or save files
	xml->open_file = NULL;
	xml->save_doc = NULL;

	xml->id = CSSED_FILETYPE_XML;
	xml->style_min = SCE_H_DEFAULT;
	xml->style_max = SCE_H_SGML_BLOCK_DEFAULT;
	xml->can_fold = TRUE;
	xml->label_language = g_strdup("XML");
	xml->lable_file = g_strdup("XML");

	cssed_file_type_add_pattern_spec (xml, "*.xml");
	cssed_file_type_add_pattern_spec (xml, "*.svg");
	cssed_file_type_add_pattern_spec (xml, "*.sgml");
	cssed_file_type_add_pattern_spec (xml, "*.glade");	
	
	cssed_file_type_menu_set_default_callback (window, xml_menu, xml);
	cssed_window_add_filetype (window, xml, FALSE);
}
