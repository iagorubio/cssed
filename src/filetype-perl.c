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
filetype_perl_set_style(CssedDoc* doc, GtkWidget *editor)
{
	ScintillaObject * sci;
	CssedWindow* window;
	CssedConfig* cfg;
	GtkWidget *scintilla;


	const gchar plKeyWords[] = "if else elsif for while until loop foreach do label last use my sub package defined require bless shift chomp chop chr crypt hex index lc lcfirst length oct ord pack reverse rindex sprintf substr uc ucfirst pos quotemeta split study abs atan2 cos exp hex int log oct rand sin sqrt srandpop push shift splice unshift grep join map reverse sort unpack delete each exists keys values binmode close closedir dbmclose dbmopen die eof fileno flock format getc print printf read readdir rewinddir seek seekdir select syscall sysread sysseek syswrite tell telldir truncate warn write pack read syscall sysread syswrite unpack vec chdir chmod chown chroot fcntl glob ioctl link lstat mkdir open opendir readlink rename rmdir stat symlink sysopen umask unlink utime caller continue die do dump eval exit goto last next redo return sub wantarray caller import local my our package use defined dump eval formline local my our reset scalar undef wantarray alarm exec fork getpgrp getppid getpriority kill pipe setpgrp setpriority sleep system times wait waitpid do import no package require use bless dbmclose dbmopen package ref tie tied untie use accept bind connect getpeername getsockname getsockopt listen recv send setsockopt shutdown socket socketpair msgctl msgget msgrcv msgsnd semctl semget semop shmctl shmget shmread shmwrite endgrent endhostent endnetent endpwent getgrent getgrgid getgrnam getlogin getpwent getpwnam getpwuid setgrent setpwent endprotoent endservent gethostbyaddr gethostbyname gethostent getnetbyaddr getnetbyname getnetent getprotobyname getprotobynumber getprotoent getservbyname getservbyport getservent sethostent setnetent setprotoent setservent gmtime localtime time times abs bless chomp chr exists formline glob import lc lcfirst map my no our prototype qx qw readline readpipe ref sub sysopen tie tied uc ucfirst untie us";

	window = document_get_window(doc);
	cfg = cssed_window_get_config(window);
	
	if( editor ) scintilla = editor;
	else scintilla = document_get_scintilla_widget(doc);
	
	sci = SCINTILLA(scintilla);


	SSM (sci, SCI_STYLECLEARALL, 0, 0);
	SSM (sci, SCI_SETLEXER, SCLEX_PERL, 0);

	SSM(sci, SCI_SETKEYWORDS, 0, (sptr_t) plKeyWords);

	SSM(sci, SCI_STYLESETFORE, SCE_PL_DEFAULT, 0x000000);
	SSM(sci, SCI_STYLESETBACK, SCE_PL_DEFAULT, 0xffffff);

	SSM(sci, SCI_STYLESETFORE, SCE_PL_ERROR, 0x0000ff);
	SSM(sci, SCI_STYLESETBACK, SCE_PL_ERROR, 0xffffff);

	SSM(sci, SCI_STYLESETFORE, SCE_PL_COMMENTLINE, 0x808080);
	SSM(sci, SCI_STYLESETBACK, SCE_PL_COMMENTLINE, 0xffffff);

	SSM(sci, SCI_STYLESETFORE, SCE_PL_POD, 0x004080);
	SSM(sci, SCI_STYLESETBACK, SCE_PL_POD, 0xffffff);

	SSM(sci, SCI_STYLESETFORE, SCE_PL_NUMBER, 0x800040);
	SSM(sci, SCI_STYLESETBACK, SCE_PL_NUMBER, 0xffffff);

	SSM(sci, SCI_STYLESETFORE, SCE_PL_WORD, 0x800000);
	SSM(sci, SCI_STYLESETBACK, SCE_PL_WORD, 0xffffff);

	SSM(sci, SCI_STYLESETFORE, SCE_PL_STRING, 0x008000);
	SSM(sci, SCI_STYLESETBACK, SCE_PL_STRING, 0xffffff);

	SSM(sci, SCI_STYLESETFORE, SCE_PL_CHARACTER, 0x006000);
	SSM(sci, SCI_STYLESETBACK, SCE_PL_CHARACTER, 0xffffff);
	SSM(sci, SCI_STYLESETFORE, SCE_PL_PUNCTUATION, 0x804000);
	SSM(sci, SCI_STYLESETBACK, SCE_PL_PUNCTUATION, 0xffffff);

	SSM(sci, SCI_STYLESETFORE, SCE_PL_PREPROCESSOR, 0x606000);
	SSM(sci, SCI_STYLESETBACK, SCE_PL_PREPROCESSOR, 0xffffff);

	SSM(sci, SCI_STYLESETFORE, SCE_PL_OPERATOR, 0x101010);
	SSM(sci, SCI_STYLESETBACK, SCE_PL_OPERATOR, 0xffffff);

	SSM(sci, SCI_STYLESETFORE, SCE_PL_IDENTIFIER, 0x000000);
	SSM(sci, SCI_STYLESETBACK, SCE_PL_IDENTIFIER, 0xffffff);

	SSM(sci, SCI_STYLESETFORE, SCE_PL_SCALAR, 0x800050);
	SSM(sci, SCI_STYLESETBACK, SCE_PL_SCALAR, 0xffffff);

	SSM(sci, SCI_STYLESETFORE, SCE_PL_ARRAY, 0x001050);
	SSM(sci, SCI_STYLESETBACK, SCE_PL_ARRAY, 0xffffff);

	SSM(sci, SCI_STYLESETFORE, SCE_PL_HASH, 0x000080);
	SSM(sci, SCI_STYLESETBACK, SCE_PL_HASH, 0xffffff);

	SSM(sci, SCI_STYLESETFORE, SCE_PL_SYMBOLTABLE, 0x106020);
	SSM(sci, SCI_STYLESETBACK, SCE_PL_SYMBOLTABLE, 0xffffff);

	SSM(sci, SCI_STYLESETFORE, SCE_PL_REGEX, 0x404000);
	SSM(sci, SCI_STYLESETBACK, SCE_PL_REGEX, 0xffffff);

	SSM(sci, SCI_STYLESETFORE, SCE_PL_REGSUBST, 0x404000);
	SSM(sci, SCI_STYLESETBACK, SCE_PL_REGSUBST, 0xffffff);

	SSM(sci, SCI_STYLESETFORE, SCE_PL_LONGQUOTE, 0x601030);
	SSM(sci, SCI_STYLESETBACK, SCE_PL_LONGQUOTE, 0xffffff);

	SSM(sci, SCI_STYLESETFORE, SCE_PL_BACKTICKS, 0x309090);
	SSM(sci, SCI_STYLESETBACK, SCE_PL_BACKTICKS, 0xffffff);

	SSM(sci, SCI_STYLESETFORE, SCE_PL_DATASECTION, 0x104000);
	SSM(sci, SCI_STYLESETBACK, SCE_PL_DATASECTION, 0xffffff);

	SSM(sci, SCI_STYLESETFORE, SCE_PL_HERE_DELIM, 0x007020);
	SSM(sci, SCI_STYLESETBACK, SCE_PL_HERE_DELIM, 0xffffff);

	SSM(sci, SCI_STYLESETFORE, SCE_PL_HERE_Q, 0x004040);
	SSM(sci, SCI_STYLESETBACK, SCE_PL_HERE_Q, 0xffffff);

	SSM(sci, SCI_STYLESETFORE, SCE_PL_HERE_QQ, 0x104040);
	SSM(sci, SCI_STYLESETBACK, SCE_PL_HERE_QQ, 0xffffff);

	SSM(sci, SCI_STYLESETFORE, SCE_PL_HERE_QX, 0x204040);
	SSM(sci, SCI_STYLESETBACK, SCE_PL_HERE_QX, 0xffffff);

	SSM(sci, SCI_STYLESETFORE, SCE_PL_STRING_Q, 0x008010);
	SSM(sci, SCI_STYLESETBACK, SCE_PL_STRING_Q, 0xffffff);

	SSM(sci, SCI_STYLESETFORE, SCE_PL_STRING_QQ, 0x108010);
	SSM(sci, SCI_STYLESETBACK, SCE_PL_STRING_QQ, 0xffffff);

	SSM(sci, SCI_STYLESETFORE, SCE_PL_STRING_QX, 0x208010);
	SSM(sci, SCI_STYLESETBACK, SCE_PL_STRING_QX, 0xffffff);

	SSM(sci, SCI_STYLESETFORE, SCE_PL_STRING_QR, 0x308010);
	SSM(sci, SCI_STYLESETBACK, SCE_PL_STRING_QR, 0xffffff);

	SSM(sci, SCI_STYLESETFORE, SCE_PL_STRING_QW, 0x408010);
	SSM(sci, SCI_STYLESETBACK, SCE_PL_STRING_QW, 0xffffff);
}
/* ********************************** */

void
create_and_register_perl_file_type (CssedWindow* window)
{
	CssedFileType* perl;
	GtkWidget* perl_menu;

	perl_menu = gtk_menu_item_new_with_label("Perl");

	perl = cssed_file_type_new();
	perl->char_added  = NULL;

	perl->modify_attemp_read_only = NULL;
	perl->double_click = NULL;
	perl->user_list_selection = NULL;
	perl->dwell_start = NULL;
	perl->dwell_end = NULL;
	perl->hot_spot_click = NULL;
	perl->hot_spot_doubleclick = NULL;
	perl->call_tip_click = NULL;

	perl->save_point_reached = NULL;
	perl->save_point_left = NULL;
	perl->uri_dropped = NULL;
	perl->margin_click = NULL;
	perl->modified = NULL;
	perl->key_pressed = NULL;

	// configuraton and styling
	perl->apply_style = filetype_perl_set_style;

	// THOSE ARE NOT USED TEMPORALY
	perl->get_config_page_widget = NULL;
	perl->save_configuration_from_widget = NULL;	
	perl->pop_menu = NULL;
	perl->clean_popmenu = NULL;
	perl->menu_item = perl_menu;
	perl->clean_menu = NULL;
	perl->load_ui = NULL;

	// file related issues, those are mostly to avoid to open or save files
	perl->open_file = NULL;
	perl->save_doc = NULL;

	perl->id = CSSED_FILETYPE_PERL;
	perl->style_min = SCE_PL_DEFAULT;//(document_set_font_size)  (document_set_font_by_name) (Read/Write const only)
	perl->style_max = SCE_PL_STRING_QW; //(document_set_font_size) (document_set_font_by_name)(Read/Write const only)
	perl->can_fold = TRUE;
	perl->label_language = g_strdup("Perl"); // a name for the menu as CSS or XML
	perl->lable_file = g_strdup("Perl");

	cssed_file_type_add_pattern_spec (perl, "*.pl");
	cssed_file_type_add_pattern_spec (perl, "*.cgi");
	cssed_file_type_menu_set_default_callback (window, perl_menu, perl);
	cssed_window_add_filetype (window, perl, FALSE);
}
