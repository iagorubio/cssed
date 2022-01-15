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
filetype_diff_set_style( CssedDoc* doc, GtkWidget* editor )
{
	ScintillaObject* sci;
	GtkWidget* scintilla;
	CssedWindow* window;

	window = document_get_window(doc);
	
	if( editor ) scintilla = editor;
	else scintilla = document_get_scintilla_widget(doc);
	
	sci = SCINTILLA(scintilla);

	SSM (sci, SCI_STYLECLEARALL, 0, 0);
	SSM (sci, SCI_SETLEXER, SCLEX_DIFF, 0);

	SSM(sci, SCI_STYLESETFORE, SCE_DIFF_DEFAULT, 0x000000);
	SSM(sci, SCI_STYLESETBACK, SCE_DIFF_DEFAULT, 0xffffff);

	SSM(sci, SCI_STYLESETFORE, SCE_DIFF_COMMENT, 0x400000);
	SSM(sci, SCI_STYLESETBACK, SCE_DIFF_COMMENT, 0xffffff);

	SSM(sci, SCI_STYLESETFORE, SCE_DIFF_COMMAND, 0x606060);
	SSM(sci, SCI_STYLESETBACK, SCE_DIFF_COMMAND, 0xffffff);

	SSM(sci, SCI_STYLESETFORE, SCE_DIFF_HEADER, 0x990066);
	SSM(sci, SCI_STYLESETBACK, SCE_DIFF_HEADER, 0xffffff);

	SSM(sci, SCI_STYLESETFORE, SCE_DIFF_POSITION, 0x008040);
	SSM(sci, SCI_STYLESETBACK, SCE_DIFF_POSITION, 0xffffff);

	SSM(sci, SCI_STYLESETFORE, SCE_DIFF_DELETED, 0x0000ff);
	SSM(sci, SCI_STYLESETBACK, SCE_DIFF_DELETED, 0xffffff);

	SSM(sci, SCI_STYLESETFORE, SCE_DIFF_ADDED, 0x900000);
	SSM(sci, SCI_STYLESETBACK, SCE_DIFF_ADDED, 0xffffff);
}

void
create_and_register_diff_file_type (CssedWindow* window)
{
	CssedFileType* diff;
	GtkWidget* diff_menu;

	diff_menu = gtk_menu_item_new_with_label("Diff/Patch");

	diff = cssed_file_type_new();
	diff->char_added  = NULL;

	diff->modify_attemp_read_only = NULL;
	diff->double_click = NULL;
	diff->user_list_selection = NULL;
	diff->dwell_start = NULL;
	diff->dwell_end = NULL;
	diff->hot_spot_click = NULL;
	diff->hot_spot_doubleclick = NULL;
	diff->call_tip_click = NULL;

	diff->save_point_reached = NULL;
	diff->save_point_left = NULL;
	diff->uri_dropped = NULL;
	diff->margin_click = NULL;
	diff->modified = NULL;
	diff->key_pressed = NULL;

	diff->apply_style = filetype_diff_set_style;

	diff->get_config_page_widget = NULL;
	diff->save_configuration_from_widget = NULL;	
	diff->pop_menu = NULL;
	diff->clean_popmenu = NULL;
	diff->menu_item = diff_menu;
	diff->clean_menu = NULL;
	diff->load_ui = NULL;

	diff->open_file = NULL;
	diff->save_doc = NULL;

	diff->id = CSSED_FILETYPE_DIFF;
	diff->style_min = SCE_DIFF_DEFAULT;
	diff->style_max = SCE_DIFF_ADDED;
	diff->can_fold = FALSE;
	diff->label_language = g_strdup("Diff");
	diff->lable_file = g_strdup("Diff/Patch file");

	cssed_file_type_add_pattern_spec (diff, "*.diff");
	cssed_file_type_add_pattern_spec (diff, "*.patch");
	cssed_file_type_menu_set_default_callback (window, diff_menu, diff);
	cssed_window_add_filetype (window, diff, FALSE);
}
