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
filetype_text_set_style( CssedDoc* doc, GtkWidget *editor )
{
	ScintillaObject* sci;
	GtkWidget* scintilla;
	CssedWindow* window;

	window = document_get_window(doc);
	
	if( editor ) scintilla = editor;
	else scintilla = document_get_scintilla_widget(doc);
	
	sci = SCINTILLA(scintilla);

	SSM (sci, SCI_STYLECLEARALL, 0, 0);
	SSM (sci, SCI_SETLEXER, SCLEX_NULL, 0);
	SSM(sci, SCI_STYLESETFORE, STYLE_DEFAULT, 0x000000);
	SSM(sci, SCI_STYLESETBACK, STYLE_DEFAULT, 0xffffff);
}

void
create_and_register_text_file_type (CssedWindow* window)
{
	CssedFileType* text;
	GtkWidget* text_menu;

	text_menu = gtk_menu_item_new_with_label("Text");

	text = cssed_file_type_new();
	text->char_added  = NULL;

	text->modify_attemp_read_only = NULL;
	text->double_click = NULL;
	text->user_list_selection = NULL;
	text->dwell_start = NULL;
	text->dwell_end = NULL;
	text->hot_spot_click = NULL;
	text->hot_spot_doubleclick = NULL;
	text->call_tip_click = NULL;

	text->save_point_reached = NULL;
	text->save_point_left = NULL;
	text->uri_dropped = NULL;
	text->margin_click = NULL;
	text->modified = NULL;
	text->key_pressed = NULL;

	text->apply_style = filetype_text_set_style;

	text->get_config_page_widget = NULL;
	text->save_configuration_from_widget = NULL;	
	text->pop_menu = NULL;
	text->clean_popmenu = NULL;
	text->menu_item = text_menu;
	text->clean_menu = NULL;
	text->load_ui = NULL;

	text->open_file = NULL;
	text->save_doc = NULL;

	text->id = CSSED_FILETYPE_TEXT;
	text->style_min = STYLE_DEFAULT;
	text->style_max = STYLE_DEFAULT;
	text->can_fold = FALSE;
	text->label_language = g_strdup("Text");
	text->lable_file = g_strdup("Text");

	cssed_file_type_menu_set_default_callback (window, text_menu, text);
	cssed_window_add_filetype (window, text, FALSE);
}
