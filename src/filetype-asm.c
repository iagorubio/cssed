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
asm_filetype_char_added(CssedDoc* doc, gchar  lastchar)
{
	if( lastchar == '\n')
		document_indent_as_last_line(doc);
}

void
filetype_asm_set_style(CssedDoc* doc, GtkWidget *editor)
{
	ScintillaObject * sci;
	CssedWindow* window;
	CssedConfig* cfg;
	GtkWidget *scintilla;
	
	const gchar cpuInstructions[] = "addlw addwf andlw andwf bcf bsf btfsc btfss call clrf clrw clrwdt comf decf decfsz goto incf incfsz iorlw iorwf movf movlw movwf nop option retlw rlf rrf sleep subwf swapf tris xorlw xorwf";
	const gchar fpuInstructions[] = "ra2 ra3 ra4 mclr vss rb0 rb1 rb2 rb3 rb4 rb5 rb6 rb7 vdd osc2 osc1 ra2 ra1"; // pins in fact
	const gchar registers[] = "tmr0 rpo option pcl status fsr trisa trisb porta portb eedata eecon1 eecon2 eeadr pclath intcon";
	const gchar directives[] = "upper higher low";
	const gchar directiveOperands[] = "equ end"; 
	const gchar extendedInstructions[] = "addcf b bc bz bnc bnz clrc clrz setc setz movfw negf skpc skpz skpnc skpnz subcf tstf"; // exit inst.
	
	window = document_get_window(doc);
	cfg = cssed_window_get_config(window);
	
	if( editor ) scintilla = editor;
	else scintilla = document_get_scintilla_widget(doc);
	
	sci = SCINTILLA(scintilla);

	SSM (sci, SCI_STYLECLEARALL, 0, 0);
	SSM (sci, SCI_SETLEXER, SCLEX_ASM, 0);

	SSM(sci, SCI_SETKEYWORDS, 0, (sptr_t) cpuInstructions );
	SSM(sci, SCI_SETKEYWORDS, 1, (sptr_t) fpuInstructions );
	SSM(sci, SCI_SETKEYWORDS, 2, (sptr_t) registers );
	SSM(sci, SCI_SETKEYWORDS, 3, (sptr_t) directives );	
	SSM(sci, SCI_SETKEYWORDS, 4, (sptr_t) directiveOperands );
	SSM(sci, SCI_SETKEYWORDS, 5, (sptr_t) extendedInstructions );	
	
	SSM(sci, SCI_STYLESETFORE, SCE_ASM_DEFAULT, 0x000000);
	SSM(sci, SCI_STYLESETBACK, SCE_ASM_DEFAULT, 0xffffff);

	SSM(sci, SCI_STYLESETFORE, SCE_ASM_COMMENT, 0x808080);
	SSM(sci, SCI_STYLESETBACK, SCE_ASM_COMMENT, 0xffffff);

	SSM(sci, SCI_STYLESETFORE, SCE_ASM_NUMBER, 0x408000);
	SSM(sci, SCI_STYLESETBACK, SCE_ASM_NUMBER, 0xffffff);

	SSM(sci, SCI_STYLESETFORE, SCE_ASM_STRING, 0x404000);
	SSM(sci, SCI_STYLESETBACK, SCE_ASM_STRING, 0xffffff);

	SSM(sci, SCI_STYLESETFORE, SCE_ASM_DIRECTIVE, 0x004040);
	SSM(sci, SCI_STYLESETBACK, SCE_ASM_DIRECTIVE, 0xffffff);

	SSM(sci, SCI_STYLESETFORE, SCE_ASM_IDENTIFIER, 0x804040);
	SSM(sci, SCI_STYLESETBACK, SCE_ASM_IDENTIFIER, 0xffffff);

	SSM(sci, SCI_STYLESETFORE, SCE_ASM_CPUINSTRUCTION, 0x802080);
	SSM(sci, SCI_STYLESETBACK, SCE_ASM_CPUINSTRUCTION, 0xffffff);

	SSM(sci, SCI_STYLESETFORE, SCE_ASM_MATHINSTRUCTION, 0x804060);
	SSM(sci, SCI_STYLESETBACK, SCE_ASM_MATHINSTRUCTION, 0xffffff);

	SSM(sci, SCI_STYLESETFORE, SCE_ASM_REGISTER, 0x800020);
	SSM(sci, SCI_STYLESETBACK, SCE_ASM_REGISTER, 0xffffff);
	
	SSM(sci, SCI_STYLESETFORE, SCE_ASM_OPERATOR, 0x004040);
	SSM(sci, SCI_STYLESETBACK, SCE_ASM_OPERATOR, 0xffffff);

	SSM(sci, SCI_STYLESETFORE, SCE_ASM_DIRECTIVEOPERAND, 0xff2020);
	SSM(sci, SCI_STYLESETBACK, SCE_ASM_DIRECTIVEOPERAND, 0xffffff);
	
	SSM(sci, SCI_STYLESETFORE, SCE_ASM_COMMENTBLOCK, 0x005020);
	SSM(sci, SCI_STYLESETBACK, SCE_ASM_COMMENTBLOCK, 0xffffff);

	SSM(sci, SCI_STYLESETFORE, SCE_ASM_STRINGEOL, 0x104010);
	SSM(sci, SCI_STYLESETBACK, SCE_ASM_STRINGEOL, 0xffffff);

	SSM(sci, SCI_STYLESETFORE, SCE_ASM_COMMENTBLOCK, 0x400010);
	SSM(sci, SCI_STYLESETBACK, SCE_ASM_COMMENTBLOCK, 0xffffff);

	SSM(sci, SCI_STYLESETFORE, SCE_ASM_EXTINSTRUCTION, 0x109040);
	SSM(sci, SCI_STYLESETBACK, SCE_ASM_EXTINSTRUCTION, 0xffffff);
}
/* ********************************** */

void
create_and_register_asm_file_type (CssedWindow* window)
{
	CssedFileType* asmfiletype;
	GtkWidget* asmfiletype_menu;

	asmfiletype_menu = gtk_menu_item_new_with_label("RISC ASM");

	asmfiletype = cssed_file_type_new();
	asmfiletype->char_added  = asm_filetype_char_added;

	asmfiletype->modify_attemp_read_only = NULL;
	asmfiletype->double_click = NULL;
	asmfiletype->user_list_selection = NULL;
	asmfiletype->dwell_start = NULL;
	asmfiletype->dwell_end = NULL;
	asmfiletype->hot_spot_click = NULL;
	asmfiletype->hot_spot_doubleclick = NULL;
	asmfiletype->call_tip_click = NULL;

	asmfiletype->save_point_reached = NULL;
	asmfiletype->save_point_left = NULL;
	asmfiletype->uri_dropped = NULL;
	asmfiletype->margin_click = NULL;
	asmfiletype->modified = NULL;
	asmfiletype->key_pressed = NULL;

	// configuraton and styling
	asmfiletype->apply_style = filetype_asm_set_style;

	// THOSE ARE NOT USED TEMPORALY
	asmfiletype->get_config_page_widget = NULL;
	asmfiletype->save_configuration_from_widget = NULL;	
	asmfiletype->pop_menu = NULL;
	asmfiletype->clean_popmenu = NULL;
	asmfiletype->menu_item = asmfiletype_menu;
	asmfiletype->clean_menu = NULL;
	asmfiletype->load_ui = NULL;

	// file related issues, those are mostly to avoid to open or save files
	asmfiletype->open_file = NULL;
	asmfiletype->save_doc = NULL;

	asmfiletype->id = CSSED_FILETYPE_ASM;
	asmfiletype->style_min = SCE_ASM_DEFAULT;//(document_set_font_size)  (document_set_font_by_name) (Read/Write const only)
	asmfiletype->style_max = SCE_ASM_EXTINSTRUCTION; //(document_set_font_size) (document_set_font_by_name)(Read/Write const only)
	asmfiletype->can_fold = FALSE;
	asmfiletype->label_language = g_strdup("ASM"); // a name for the menu as CSS or XML
	asmfiletype->lable_file = g_strdup("ASSEMBLER");

	cssed_file_type_add_pattern_spec(asmfiletype, "*.asm");
	
	cssed_file_type_menu_set_default_callback (window, asmfiletype_menu, asmfiletype);
	cssed_window_add_filetype (window, asmfiletype, FALSE);
}
