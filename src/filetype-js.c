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
js_filetype_char_added (CssedDoc* doc, gchar  lastchar)
{
	if( lastchar == '\n')
		document_indent_as_last_line(doc);
}

void
filetype_js_set_style (CssedDoc* doc, GtkWidget *editor)
{
	ScintillaObject * sci;
	CssedWindow* window;
	CssedConfig* cfg;
	GtkWidget *scintilla;

	const gchar primaryKeyWords[] = "boolean break byte case catch char const continue \
debugger default delete do double else enum false \
final finally float for goto if implements in \
instanceof int long native new null private \
protected public return short static super switch synchronized \
this throw throws transient true try typeof var void volatile \
while with";

	const gchar secondaryKeyWords[] = "abstract class export extends function import interface package" ;

	const gchar docCommentKeyWords[] = "";

	const gchar typedefsKeyWords[] = "";

	window = document_get_window(doc);
	cfg = cssed_window_get_config(window);
	
	if( editor ) scintilla = editor;
	else scintilla = document_get_scintilla_widget(doc);
	
	sci = SCINTILLA(scintilla);

	SSM (sci, SCI_SETPROPERTY, (sptr_t) "fold.preprocessor", (sptr_t) "1");
	SSM (sci, SCI_SETPROPERTY, (sptr_t) "fold.at.else", (sptr_t) "1");

	SSM (sci, SCI_STYLECLEARALL, 0, 0);
	SSM (sci, SCI_SETLEXER, SCLEX_CPP, 0);

	SSM(sci, SCI_SETKEYWORDS, 0, (sptr_t) primaryKeyWords);
	SSM(sci, SCI_SETKEYWORDS, 1, (sptr_t) secondaryKeyWords);
	SSM(sci, SCI_SETKEYWORDS, 2, (sptr_t) docCommentKeyWords);
	SSM(sci, SCI_SETKEYWORDS, 4, (sptr_t) typedefsKeyWords);

	SSM(sci, SCI_STYLESETFORE, SCE_C_DEFAULT, 0x000000);
	SSM(sci, SCI_STYLESETBACK, SCE_C_DEFAULT, 0xffffff);

	SSM(sci, SCI_STYLESETFORE, SCE_C_COMMENT, 0x808080);
	SSM(sci, SCI_STYLESETBACK, SCE_C_COMMENT, 0xffffff);

	SSM(sci, SCI_STYLESETFORE, SCE_C_COMMENTLINE, 0x808080);
	SSM(sci, SCI_STYLESETBACK, SCE_C_COMMENTLINE, 0xffffff);

	SSM(sci, SCI_STYLESETFORE, SCE_C_COMMENTDOC, 0x808080);
	SSM(sci, SCI_STYLESETBACK, SCE_C_COMMENTDOC, 0xffffff);

	SSM(sci, SCI_STYLESETFORE, SCE_C_NUMBER, 0x408000);
	SSM(sci, SCI_STYLESETBACK, SCE_C_NUMBER, 0xffffff);

	SSM(sci, SCI_STYLESETFORE, SCE_C_WORD, 0x802080);
	SSM(sci, SCI_STYLESETBACK, SCE_C_WORD, 0xffffff);

	SSM(sci, SCI_STYLESETFORE, SCE_C_WORD2, 0x800020);
	SSM(sci, SCI_STYLESETBACK, SCE_C_WORD2, 0xffffff);

	SSM(sci, SCI_STYLESETFORE, SCE_C_STRING, 0x404000);
	SSM(sci, SCI_STYLESETBACK, SCE_C_STRING, 0xffffff);

	SSM(sci, SCI_STYLESETFORE, SCE_C_CHARACTER, 0x004040);
	SSM(sci, SCI_STYLESETBACK, SCE_C_CHARACTER, 0xffffff);

	SSM(sci, SCI_STYLESETFORE, SCE_C_UUID, 0x804040);
	SSM(sci, SCI_STYLESETBACK, SCE_C_UUID, 0xffffff);

	SSM(sci, SCI_STYLESETFORE, SCE_C_PREPROCESSOR, 0x804060);
	SSM(sci, SCI_STYLESETBACK, SCE_C_PREPROCESSOR, 0xffffff);

	SSM(sci, SCI_STYLESETFORE, SCE_C_OPERATOR, 0x101030);
	SSM(sci, SCI_STYLESETBACK, SCE_C_OPERATOR, 0xffffff);

	SSM(sci, SCI_STYLESETFORE, SCE_C_IDENTIFIER, 0x100000);
	SSM(sci, SCI_STYLESETBACK, SCE_C_IDENTIFIER, 0xffffff);

	SSM(sci, SCI_STYLESETFORE, SCE_C_STRINGEOL, 0x104010);
	SSM(sci, SCI_STYLESETBACK, SCE_C_STRINGEOL, 0xffffff);

	SSM(sci, SCI_STYLESETFORE, SCE_C_VERBATIM, 0x906040);
	SSM(sci, SCI_STYLESETBACK, SCE_C_VERBATIM, 0xffffff);

	SSM(sci, SCI_STYLESETFORE, SCE_C_REGEX, 0x905010);
	SSM(sci, SCI_STYLESETBACK, SCE_C_REGEX, 0xffffff);

	SSM(sci, SCI_STYLESETFORE, SCE_C_COMMENTLINEDOC, 0x005020);
	SSM(sci, SCI_STYLESETBACK, SCE_C_COMMENTLINEDOC, 0xffffff);

	SSM(sci, SCI_STYLESETFORE, SCE_C_COMMENTDOCKEYWORD, 0x400010);
	SSM(sci, SCI_STYLESETBACK, SCE_C_COMMENTDOCKEYWORD, 0xffffff);

	SSM(sci, SCI_STYLESETFORE, SCE_C_COMMENTDOCKEYWORDERROR, 0x001020);
	SSM(sci, SCI_STYLESETBACK, SCE_C_COMMENTDOCKEYWORDERROR, 0xffffff);

	SSM(sci, SCI_STYLESETFORE, SCE_C_GLOBALCLASS, 0x109040);
	SSM(sci, SCI_STYLESETBACK, SCE_C_GLOBALCLASS, 0xffffff);
}
/* ********************************** */

void
create_and_register_js_file_type (CssedWindow* window)
{
	CssedFileType* jsfiletype;
	GtkWidget* jsfiletype_menu;

	jsfiletype_menu = gtk_menu_item_new_with_label("Javascript");

	jsfiletype = cssed_file_type_new();
	jsfiletype->char_added = js_filetype_char_added;

	jsfiletype->modify_attemp_read_only = NULL;
	jsfiletype->double_click = NULL;
	jsfiletype->user_list_selection = NULL;
	jsfiletype->dwell_start = NULL;
	jsfiletype->dwell_end = NULL;
	jsfiletype->hot_spot_click = NULL;
	jsfiletype->hot_spot_doubleclick = NULL;
	jsfiletype->call_tip_click = NULL;

	jsfiletype->save_point_reached = NULL;
	jsfiletype->save_point_left = NULL;
	jsfiletype->uri_dropped = NULL;
	jsfiletype->margin_click = NULL;
	jsfiletype->modified = NULL;
	jsfiletype->key_pressed = NULL;

	// configuraton and styling
	jsfiletype->apply_style = filetype_js_set_style;

	// THOSE ARE NOT USED TEMPORALY
	jsfiletype->get_config_page_widget = NULL;
	jsfiletype->save_configuration_from_widget = NULL;	
	jsfiletype->pop_menu = NULL;
	jsfiletype->clean_popmenu = NULL;
	jsfiletype->menu_item = jsfiletype_menu;
	jsfiletype->clean_menu = NULL;
	jsfiletype->load_ui = NULL;

	// file related issues, those are mostly to avoid to open or save files
	jsfiletype->open_file = NULL;
	jsfiletype->save_doc = NULL;
	jsfiletype->id = CSSED_FILETYPE_JS;
	jsfiletype->style_min = SCE_C_DEFAULT;//(document_set_font_size)  (document_set_font_by_name) (Read/Write const only)
	jsfiletype->style_max = SCE_C_GLOBALCLASS; //(document_set_font_size) (document_set_font_by_name)(Read/Write const only)
	jsfiletype->can_fold = TRUE;
	jsfiletype->label_language = g_strdup("Javascript"); // a name for the menu as CSS or XML
	jsfiletype->lable_file = g_strdup("JS");

	cssed_file_type_add_pattern_spec(jsfiletype, "*.js");
	cssed_file_type_menu_set_default_callback (window, jsfiletype_menu, jsfiletype);
	cssed_window_add_filetype (window, jsfiletype, FALSE);
}
