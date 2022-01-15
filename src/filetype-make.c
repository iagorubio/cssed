#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#ifndef PLAT_GTK
#   define PLAT_GTK 1
#endif

#include <gtk/gtk.h>
#include <Scintilla.h>
#include <SciLexer.h>
#include <ScintillaWidget.h>

#include "cssedwindow.h"
#include "document.h"
#include "file-type-manager.h"
#include "support.h"
#include "debug.h"
#include "utils.h"

void
filetype_make_set_style( CssedDoc* doc, GtkWidget *editor )
{
	ScintillaObject* sci;
	GtkWidget* scintilla;
	CssedWindow* window;

	window = document_get_window(doc);
	
	if( editor ) scintilla = editor;
	else scintilla = document_get_scintilla_widget(doc);
	
	sci = SCINTILLA(scintilla);

	SSM (sci, SCI_STYLECLEARALL, 0, 0);
	SSM (sci, SCI_SETLEXER, SCLEX_MAKEFILE, 0);

	SSM(sci, SCI_STYLESETFORE, SCE_MAKE_DEFAULT, 0x000000);
	SSM(sci, SCI_STYLESETBACK, SCE_MAKE_DEFAULT, 0xffffff);

	SSM(sci, SCI_STYLESETFORE, SCE_MAKE_COMMENT, 0x808080);
	SSM(sci, SCI_STYLESETBACK, SCE_MAKE_COMMENT, 0xffffff);

	SSM(sci, SCI_STYLESETFORE, SCE_MAKE_PREPROCESSOR, 0x606060);
	SSM(sci, SCI_STYLESETBACK, SCE_MAKE_PREPROCESSOR, 0xffffff);

	SSM(sci, SCI_STYLESETFORE, SCE_MAKE_IDENTIFIER, 0x990066);
	SSM(sci, SCI_STYLESETBACK, SCE_MAKE_IDENTIFIER, 0xffffff);

	SSM(sci, SCI_STYLESETFORE, SCE_MAKE_OPERATOR, 0x008040);
	SSM(sci, SCI_STYLESETBACK, SCE_MAKE_OPERATOR, 0xffffff);

	SSM(sci, SCI_STYLESETFORE, SCE_MAKE_TARGET, 0x600000);
	SSM(sci, SCI_STYLESETBACK, SCE_MAKE_TARGET, 0xffffff);

	SSM(sci, SCI_STYLESETFORE, SCE_MAKE_IDEOL, 0x008000);
	SSM(sci, SCI_STYLESETBACK, SCE_MAKE_IDEOL, 0xffffff);
}

void
create_and_register_make_file_type (CssedWindow* window)
{
	CssedFileType* make;
	GtkWidget* make_menu;

	make_menu = gtk_menu_item_new_with_label("Make");

	make = cssed_file_type_new();
	make->char_added  = NULL;

	make->modify_attemp_read_only = NULL;
	make->double_click = NULL;
	make->user_list_selection = NULL;
	make->dwell_start = NULL;
	make->dwell_end = NULL;
	make->hot_spot_click = NULL;
	make->hot_spot_doubleclick = NULL;
	make->call_tip_click = NULL;

	make->save_point_reached = NULL;
	make->save_point_left = NULL;
	make->uri_dropped = NULL;
	make->margin_click = NULL;
	make->modified = NULL;
	make->key_pressed = NULL;

	make->apply_style = filetype_make_set_style;

	make->get_config_page_widget = NULL;
	make->save_configuration_from_widget = NULL;	
	make->pop_menu = NULL;
	make->clean_popmenu = NULL;
	make->menu_item = make_menu;
	make->clean_menu = NULL;
	make->load_ui = NULL;

	make->open_file = NULL;
	make->save_doc = NULL;

	make->id = CSSED_FILETYPE_MAKE;
	make->style_min = SCE_MAKE_DEFAULT;
	make->style_max = SCE_MAKE_IDEOL;
	make->can_fold = FALSE;
	make->label_language = g_strdup("Make");
	make->lable_file = g_strdup("Makefile");

	cssed_file_type_add_pattern_spec (make, "*akefile");
	cssed_file_type_add_pattern_spec (make, "*akefile*");
	cssed_file_type_menu_set_default_callback (window, make_menu, make);
	cssed_window_add_filetype (window, make, FALSE);
}
