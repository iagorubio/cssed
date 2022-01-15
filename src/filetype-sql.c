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
filetype_sql_set_style( CssedDoc* doc, GtkWidget *editor) 
{
	ScintillaObject * sci;
	GtkWidget *scintilla;
	CssedWindow* window;

/*
	"Keywords",
	"Database Objects",
	"PLDoc",
	"SQL*Plus",
	"User Keywords 1",
	"User Keywords 2",
	"User Keywords 3",
	"User Keywords 4",
*/

	const gchar primaryKeyWords[] = "absolute action add all allocate alter and any are as asc assertion at authorization avg begin between bit bit_length both by cascade cascaded case cast catalog character charset char_length character_length check close coalesce collate collation column commit connect connection constraint constraints continue convert corresponding count create cross current current_date current_time current_timestamp current_user cursor day deallocate dec declare default deferrable deferred delete desc describe descriptor diagnostics disconnect distinct domain drop else end end-exec engine escape except exception exec execute exists external extract false fetch first for foreign found from full get global go goto grant group having hour identity immediate in indicator initially inner input insensitive insert intersect interval into is isolation if join key language last leading left level like local lock lower match max min minute module month names national natural nchar next no not null nullif octet_length of on only open option or order outer output overlaps pad partial position prepare preserve primary prior privileges procedure public read references relative restrict revoke right rollback rows schema scroll second" \
		"section select session session_user size some space sql sqlcode sqlerror sqlstate substring sum system_user temporary then timezone_hour timezone_minute to trailing transaction translate translation trim true union unique unknown unlock update upper usage user using value values varying view when whenever where with work write zone ada c catalog_name character_set_catalog character_set_name character_set_schema class_origin cobol collation_catalog collation_name collation_schema column_name command_function committed condition_number connection_name constraint_catalog constraint_name constraint_schema cursor_name data datetime_interval_code datetime_interval_precision dynamic_function fortran length message_length message_octet_length message_text more mumps name nullable number" \
        "pascal pli repeatable returned_length returned_octet_length returned_sqlstate row_count scale schema_name serializable server_name subclass_origin table_name type uncommitted unnamed";

	const gchar secondaryKeyWords[] = "show table tables database";
	
	const gchar typeKeyWords[] = "tinyint smallint mediumint int integer bigint float double double precision real decimal numeric date datetime timestamp time year char varchar tinyblob tinytext blob text mediumblob mediumtext longblob longtext enum set";
	
	window = document_get_window(doc);
	
	if( editor ) scintilla = editor;
	else scintilla = document_get_scintilla_widget(doc);
	
	sci = SCINTILLA(scintilla);

	SSM (sci, SCI_SETPROPERTY, (sptr_t) "fold.comment", (sptr_t) "1");
	SSM (sci, SCI_SETPROPERTY, (sptr_t) "fold.compact", (sptr_t) "1");
	SSM (sci, SCI_SETPROPERTY, (sptr_t) "sql.backslash.escapes", (sptr_t) "1");
	SSM (sci, SCI_SETPROPERTY, (sptr_t) "sql.backticks.identifier", (sptr_t) "1");

	SSM (sci, SCI_STYLECLEARALL, 0, 0);
	SSM (sci, SCI_SETLEXER, SCLEX_SQL, 0);

	SSM(sci, SCI_SETKEYWORDS, 0, (sptr_t) primaryKeyWords);
	SSM(sci, SCI_SETKEYWORDS, 1, (sptr_t) secondaryKeyWords);
	SSM(sci, SCI_SETKEYWORDS, 3, (sptr_t) typeKeyWords);

	SSM(sci, SCI_STYLESETFORE, SCE_SQL_DEFAULT, 0x000000);
	SSM(sci, SCI_STYLESETBACK, SCE_SQL_DEFAULT, 0xffffff);

	SSM(sci, SCI_STYLESETFORE, SCE_SQL_COMMENT, 0x808080);
	SSM(sci, SCI_STYLESETBACK, SCE_SQL_COMMENT, 0xffffff);

	SSM(sci, SCI_STYLESETFORE, SCE_SQL_COMMENTLINE, 0x808080);
	SSM(sci, SCI_STYLESETBACK, SCE_SQL_COMMENTLINE, 0xffffff);
	
	SSM(sci, SCI_STYLESETFORE, SCE_SQL_COMMENTDOC, 0x808080);
	SSM(sci, SCI_STYLESETBACK, SCE_SQL_COMMENTDOC, 0xffffff);

	SSM(sci, SCI_STYLESETFORE, SCE_SQL_NUMBER, 0x408000);
	SSM(sci, SCI_STYLESETBACK, SCE_SQL_NUMBER, 0xffffff);

	SSM(sci, SCI_STYLESETFORE, SCE_SQL_WORD, 0x802080);
	SSM(sci, SCI_STYLESETBACK, SCE_SQL_WORD, 0xffffff);
	
	SSM(sci, SCI_STYLESETFORE, SCE_SQL_STRING, 0x008000);
	SSM(sci, SCI_STYLESETBACK, SCE_SQL_STRING, 0xffffff);

	SSM(sci, SCI_STYLESETFORE, SCE_SQL_CHARACTER, 0x008000);
	SSM(sci, SCI_STYLESETBACK, SCE_SQL_CHARACTER, 0xffffff);

	SSM(sci, SCI_STYLESETFORE, SCE_SQL_SQLPLUS, 0x404000);
	SSM(sci, SCI_STYLESETBACK, SCE_SQL_SQLPLUS, 0xffffff);

	SSM(sci, SCI_STYLESETFORE, SCE_SQL_SQLPLUS_PROMPT, 0x804040);
	SSM(sci, SCI_STYLESETBACK, SCE_SQL_SQLPLUS_PROMPT, 0xffffff);

	SSM(sci, SCI_STYLESETFORE, SCE_SQL_OPERATOR, 0x101030);
	SSM(sci, SCI_STYLESETBACK, SCE_SQL_OPERATOR, 0xffffff);

	SSM(sci, SCI_STYLESETFORE, SCE_SQL_IDENTIFIER, 0x100000);
	SSM(sci, SCI_STYLESETBACK, SCE_SQL_IDENTIFIER, 0xffffff);
	
	SSM(sci, SCI_STYLESETFORE, SCE_SQL_SQLPLUS_COMMENT, 0x808080);
	SSM(sci, SCI_STYLESETBACK, SCE_SQL_SQLPLUS_COMMENT, 0xffffff);

	SSM(sci, SCI_STYLESETFORE, SCE_SQL_COMMENTLINEDOC, 0x005020);
	SSM(sci, SCI_STYLESETBACK, SCE_SQL_COMMENTLINEDOC, 0xffffff);
	
	SSM(sci, SCI_STYLESETFORE, SCE_SQL_WORD2, 0x990000);
	SSM(sci, SCI_STYLESETBACK, SCE_SQL_WORD2, 0xffffff);

	SSM(sci, SCI_STYLESETFORE, SCE_SQL_COMMENTDOCKEYWORD, 0x400010);
	SSM(sci, SCI_STYLESETBACK, SCE_SQL_COMMENTDOCKEYWORD, 0xffffff);

	SSM(sci, SCI_STYLESETFORE, SCE_SQL_COMMENTDOCKEYWORDERROR, 0x001020);
	SSM(sci, SCI_STYLESETBACK, SCE_SQL_COMMENTDOCKEYWORDERROR, 0xffffff);

	SSM(sci, SCI_STYLESETFORE, SCE_SQL_QUOTEDIDENTIFIER, 0x990000);
	SSM(sci, SCI_STYLESETBACK, SCE_SQL_QUOTEDIDENTIFIER, 0x99eeee);
	
	SSM(sci, SCI_STYLESETFORE, SCE_SQL_USER1, 0x109040);
	SSM(sci, SCI_STYLESETBACK, SCE_SQL_USER1, 0xffffff);
	
	SSM(sci, SCI_STYLESETFORE, SCE_SQL_USER2, 0x802080);
	SSM(sci, SCI_STYLESETBACK, SCE_SQL_USER2, 0xffffff);

	SSM(sci, SCI_STYLESETFORE, SCE_SQL_USER3, 0x990000);
	SSM(sci, SCI_STYLESETBACK, SCE_SQL_USER3, 0xffffff);

	SSM(sci, SCI_STYLESETFORE, SCE_SQL_USER4, 0x905010);
	SSM(sci, SCI_STYLESETBACK, SCE_SQL_USER4, 0xffffff);
}
/* ********************************** */

void
create_and_register_sql_file_type (CssedWindow* window)
{
	CssedFileType* sql;
	GtkWidget* sql_menu;

	sql_menu = gtk_menu_item_new_with_label("SQL");

	sql = cssed_file_type_new();
	sql->char_added  = NULL;

	sql->modify_attemp_read_only = NULL;
	sql->double_click = NULL;
	sql->user_list_selection = NULL;
	sql->dwell_start = NULL;
	sql->dwell_end = NULL;
	sql->hot_spot_click = NULL;
	sql->hot_spot_doubleclick = NULL;
	sql->call_tip_click = NULL;

	sql->save_point_reached = NULL;
	sql->save_point_left = NULL;
	sql->uri_dropped = NULL;
	sql->margin_click = NULL;
	sql->modified = NULL;
	sql->key_pressed = NULL;

	// configuraton and styling
	sql->apply_style = filetype_sql_set_style;

	// THOSE ARE NOT USED TEMPORALY
	sql->get_config_page_widget = NULL;
	sql->save_configuration_from_widget = NULL;	
	sql->pop_menu = NULL;
	sql->clean_popmenu = NULL;
	sql->menu_item = sql_menu;
	sql->clean_menu = NULL;
	sql->load_ui = NULL;

	// file related issues, those are mostly to avoid to open or save files
	sql->open_file = NULL;
	sql->save_doc = NULL;

	sql->id = CSSED_FILETYPE_SQL;
	sql->style_min = SCE_C_DEFAULT;//(document_set_font_size)  (document_set_font_by_name) (Read/Write const only)
	sql->style_max = SCE_C_GLOBALCLASS; //(document_set_font_size) (document_set_font_by_name)(Read/Write const only)
	sql->can_fold = FALSE;
	sql->label_language = g_strdup("SQL"); // a name for the menu as CSS or XML
	sql->lable_file = g_strdup("Ansi SQL");

	cssed_file_type_add_pattern_spec (sql, "*.sql");
	cssed_file_type_menu_set_default_callback (window, sql_menu, sql);
	cssed_window_add_filetype (window, sql, FALSE);
}
