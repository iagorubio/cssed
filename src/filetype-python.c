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
filetype_python_char_added(CssedDoc* doc, gchar  lastchar)
{
	if( lastchar == '\n')
		document_indent_as_last_line(doc);
}

void
filetype_python_set_style(CssedDoc* doc, GtkWidget* editor)
{
	ScintillaObject * sci;
	CssedWindow* window;
	CssedConfig* cfg;
	GtkWidget *scintilla;

	const gchar pyKeyWords[] = "and assert break class continue def del elif else except exec finally for from global if import in is lambda not or pass print raise return try while yield";

	window = document_get_window(doc);
	cfg = cssed_window_get_config(window);
	
	if( editor ) scintilla = editor;
	else scintilla = document_get_scintilla_widget(doc);
	
	sci = SCINTILLA(scintilla);

	SSM (sci, SCI_SETPROPERTY, (sptr_t) "fold.commnent.python", (sptr_t) "1");
	SSM (sci, SCI_SETPROPERTY, (sptr_t) "fold.quotes.python", (sptr_t) "1");

	SSM (sci, SCI_STYLECLEARALL, 0, 0);
	SSM (sci, SCI_SETLEXER, SCLEX_PYTHON, 0);

	SSM(sci, SCI_SETKEYWORDS, 0, (sptr_t) pyKeyWords);
	
	// on python indentation matters
	SSM(sci, SCI_SETINDENTATIONGUIDES, TRUE, 0);

	SSM(sci, SCI_STYLESETFORE, SCE_P_DEFAULT, 0x000000);
	SSM(sci, SCI_STYLESETBACK, SCE_P_DEFAULT, 0xffffff);

	SSM(sci, SCI_STYLESETFORE, SCE_P_COMMENTLINE, 0x808080);
	SSM(sci, SCI_STYLESETBACK, SCE_P_COMMENTLINE, 0xffffff);

	SSM(sci, SCI_STYLESETFORE, SCE_P_NUMBER, 0x800040);
	SSM(sci, SCI_STYLESETBACK, SCE_P_NUMBER, 0xffffff);

	SSM(sci, SCI_STYLESETFORE, SCE_P_STRING, 0x008000);
	SSM(sci, SCI_STYLESETBACK, SCE_P_STRING, 0xffffff);

	SSM(sci, SCI_STYLESETFORE, SCE_P_CHARACTER, 0x008000);
	SSM(sci, SCI_STYLESETBACK, SCE_P_CHARACTER, 0xffffff);

	SSM(sci, SCI_STYLESETFORE, SCE_P_WORD, 0x800060);
	SSM(sci, SCI_STYLESETBACK, SCE_P_WORD, 0xffffff);

	SSM(sci, SCI_STYLESETFORE, SCE_P_TRIPLE, 0x208000);
	SSM(sci, SCI_STYLESETBACK, SCE_P_TRIPLE, 0xffffff);

	SSM(sci, SCI_STYLESETFORE, SCE_P_TRIPLEDOUBLE, 0x004040);
	SSM(sci, SCI_STYLESETBACK, SCE_P_TRIPLEDOUBLE, 0xffffff);

	SSM(sci, SCI_STYLESETFORE, SCE_P_CLASSNAME, 0x303000);
	SSM(sci, SCI_STYLESETBACK, SCE_P_CLASSNAME, 0xffffff);

	SSM(sci, SCI_STYLESETFORE, SCE_P_DEFNAME, 0x800000);
	SSM(sci, SCI_STYLESETBACK, SCE_P_DEFNAME, 0xffffff);

	SSM(sci, SCI_STYLESETFORE, SCE_P_OPERATOR, 0x800030);
	SSM(sci, SCI_STYLESETBACK, SCE_P_OPERATOR, 0xffffff);

	SSM(sci, SCI_STYLESETFORE, SCE_P_IDENTIFIER, 0x000000);
	SSM(sci, SCI_STYLESETBACK, SCE_P_IDENTIFIER, 0xffffff);

	SSM(sci, SCI_STYLESETFORE, SCE_P_COMMENTBLOCK, 0x808080);
	SSM(sci, SCI_STYLESETBACK, SCE_P_COMMENTBLOCK, 0xffffff);

	SSM(sci, SCI_STYLESETFORE, SCE_P_STRINGEOL, 0x000000);
	SSM(sci, SCI_STYLESETBACK, SCE_P_STRINGEOL, 0xffffff);

}
/* ********************************** */

void
create_and_register_python_file_type (CssedWindow* window)
{
	CssedFileType* python;
	GtkWidget* python_menu;

	python_menu = gtk_menu_item_new_with_label("Python");

	python = cssed_file_type_new();
	python->char_added  = filetype_python_char_added;

	python->modify_attemp_read_only = NULL;
	python->double_click = NULL;
	python->user_list_selection = NULL;
	python->dwell_start = NULL;
	python->dwell_end = NULL;
	python->hot_spot_click = NULL;
	python->hot_spot_doubleclick = NULL;
	python->call_tip_click = NULL;

	python->save_point_reached = NULL;
	python->save_point_left = NULL;
	python->uri_dropped = NULL;
	python->margin_click = NULL;
	python->modified = NULL;
	python->key_pressed = NULL;

	// configuraton and styling
	python->apply_style = filetype_python_set_style;

	// THOSE ARE NOT USED TEMPORALY
	python->get_config_page_widget = NULL;
	python->save_configuration_from_widget = NULL;	
	python->pop_menu = NULL;
	python->clean_popmenu = NULL;
	python->menu_item = python_menu;
	python->clean_menu = NULL;
	python->load_ui = NULL;

	// file related issues, those are mostly to avoid to open or save files
	python->open_file = NULL;
	python->save_doc = NULL;

	python->id = CSSED_FILETYPE_PYTHON;
	python->style_min = SCE_P_DEFAULT;//(document_set_font_size)  (document_set_font_by_name) (Read/Write const only)
	python->style_max = SCE_P_STRINGEOL; //(document_set_font_size) (document_set_font_by_name)(Read/Write const only)
	python->can_fold = TRUE;
	python->label_language = g_strdup("Python"); // a name for the menu as CSS or XML
	python->lable_file = g_strdup("Python");

	cssed_file_type_add_pattern_spec (python, "*.py");
	cssed_file_type_menu_set_default_callback (window, python_menu, python);
	cssed_window_add_filetype (window, python, FALSE);
}
