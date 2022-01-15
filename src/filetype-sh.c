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
filetype_sh_set_style(CssedDoc* doc, GtkWidget *editor)
{
	ScintillaObject * sci;
	GtkWidget *scintilla;

	const gchar shKeyWords[] = "[ ] test function bash alias bg bind break builtin cd command compgen \
complete continue declare dirs disown echo  enable  eval  exec \
exit  export  for fc  fg getopts hash help history in jobs kill let \
local logout popd printf pushd pwd read readonly  return  set \
shift  shopt  source  suspend  test  times  trap  type typeset \
ulimit umask unalias unset wait if then else while do case done in esac fi";
	
	if( editor ) scintilla = editor;
	else scintilla = document_get_scintilla_widget(doc);
	
	sci = SCINTILLA(scintilla);

	SSM (sci, SCI_STYLECLEARALL, 0, 0);
	SSM (sci, SCI_SETLEXER, SCLEX_BASH, 0);

	SSM(sci, SCI_SETKEYWORDS, 0, (sptr_t) shKeyWords);

	SSM(sci, SCI_STYLESETFORE, SCE_SH_DEFAULT, 0x000000);
	SSM(sci, SCI_STYLESETBACK, SCE_SH_DEFAULT, 0xffffff);

	SSM(sci, SCI_STYLESETFORE, SCE_SH_ERROR, 0x0000ff);
	SSM(sci, SCI_STYLESETBACK, SCE_SH_ERROR, 0x0000ff);

	SSM(sci, SCI_STYLESETFORE, SCE_SH_COMMENTLINE, 0x808080);
	SSM(sci, SCI_STYLESETBACK, SCE_SH_COMMENTLINE, 0xffffff);

	SSM(sci, SCI_STYLESETFORE, SCE_SH_NUMBER, 0x800080);
	SSM(sci, SCI_STYLESETBACK, SCE_SH_NUMBER, 0xffffff);

	SSM(sci, SCI_STYLESETFORE, SCE_SH_WORD, 0x800000);
	SSM(sci, SCI_STYLESETBACK, SCE_SH_WORD, 0xffffff);

	SSM(sci, SCI_STYLESETFORE, SCE_SH_STRING, 0x404000);
	SSM(sci, SCI_STYLESETBACK, SCE_SH_STRING, 0xffffff);

	SSM(sci, SCI_STYLESETFORE, SCE_SH_CHARACTER, 0x004040);
	SSM(sci, SCI_STYLESETBACK, SCE_SH_CHARACTER, 0xffffff);

	SSM(sci, SCI_STYLESETFORE, SCE_SH_OPERATOR, 0x004000);
	SSM(sci, SCI_STYLESETBACK, SCE_SH_OPERATOR, 0xffffff);

	SSM(sci, SCI_STYLESETFORE, SCE_SH_IDENTIFIER, 0x000000);
	SSM(sci, SCI_STYLESETBACK, SCE_SH_IDENTIFIER, 0xffffff);

	SSM(sci, SCI_STYLESETFORE, SCE_SH_SCALAR, 0x204070);
	SSM(sci, SCI_STYLESETBACK, SCE_SH_SCALAR, 0xffffff);

	SSM(sci, SCI_STYLESETFORE, SCE_SH_PARAM, 0x704020);
	SSM(sci, SCI_STYLESETBACK, SCE_SH_PARAM, 0xffffff);

	SSM(sci, SCI_STYLESETFORE, SCE_SH_BACKTICKS, 0x205020);
	SSM(sci, SCI_STYLESETBACK, SCE_SH_BACKTICKS, 0xffffff);

	SSM(sci, SCI_STYLESETFORE, SCE_SH_HERE_DELIM, 0x601060);
	SSM(sci, SCI_STYLESETBACK, SCE_SH_HERE_DELIM, 0xffffff);

	SSM(sci, SCI_STYLESETFORE, SCE_SH_HERE_Q, 0x303090);
	SSM(sci, SCI_STYLESETBACK, SCE_SH_HERE_Q, 0xffffff);
}
/* ********************************** */

void
create_and_register_sh_file_type (CssedWindow* window)
{
	CssedFileType* sh;
	GtkWidget* sh_menu;

	sh_menu = gtk_menu_item_new_with_label("SH");

	sh = cssed_file_type_new();
	sh->char_added  = NULL;

	sh->modify_attemp_read_only = NULL;
	sh->double_click = NULL;
	sh->user_list_selection = NULL;
	sh->dwell_start = NULL;
	sh->dwell_end = NULL;
	sh->hot_spot_click = NULL;
	sh->hot_spot_doubleclick = NULL;
	sh->call_tip_click = NULL;

	sh->save_point_reached = NULL;
	sh->save_point_left = NULL;
	sh->uri_dropped = NULL;
	sh->margin_click = NULL;
	sh->modified = NULL;
	sh->key_pressed = NULL;

	sh->apply_style = filetype_sh_set_style;

	sh->get_config_page_widget = NULL;
	sh->save_configuration_from_widget = NULL;	
	sh->pop_menu = NULL;
	sh->clean_popmenu = NULL;
	sh->menu_item = sh_menu;
	sh->clean_menu = NULL;
	sh->load_ui = NULL;

	// file related issues, those are mostly to avoid to open or save files
	sh->open_file = NULL;
	sh->save_doc = NULL;

	sh->id = CSSED_FILETYPE_SH;
	sh->style_min = SCE_SH_DEFAULT;
	sh->style_max = SCE_SH_HERE_Q;
	sh->can_fold = FALSE;
	sh->label_language = g_strdup("SH");
	sh->lable_file = g_strdup("Bash scripts");

	cssed_file_type_menu_set_default_callback (window, sh_menu, sh);
	cssed_window_add_filetype (window, sh, FALSE);
}
