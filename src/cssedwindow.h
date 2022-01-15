/*  cssed (c) Iago Rubio 2003, 2005 - A tiny CSS editor.
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Library General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

#ifdef WITH_TERMINAL
# include <vte/vte.h>
#endif

/** @file cssedwindow.h
 *  @brief Header file for the cssed main window object. 
 *  Those functions are the interface for cssed's ::_CssedWindow object.
 */

/** a macro to cast a pointer to ::CssedWindow */
#define CSSED_WINDOW(n) (CssedWindow*) n

/** colors used in the output window */
#define ERROR_COLOR_STRING "red"
#define WARNING_COLOR_STRING "navy"

/** registry name for Windows */
#ifdef WIN32
#  define CSSED_REGISTRY_NAME "cssed-0.4.0"
#endif

/** error domain to be used on GError */
#define CSSED_ERROR_DOMAIN  g_quark_from_string ("CSSED_ERROR")

typedef enum {
	// those match with GFileError
	CSSED_FILE_ERROR_EXIST,
	CSSED_FILE_ERROR_ISDIR,
	CSSED_FILE_ERROR_ACCES,
	CSSED_FILE_ERROR_NAMETOOLONG,
	CSSED_FILE_ERROR_NOENT,
	CSSED_FILE_ERROR_NOTDIR,
	CSSED_FILE_ERROR_NXIO,
	CSSED_FILE_ERROR_NODEV,
	CSSED_FILE_ERROR_ROFS,
	CSSED_FILE_ERROR_TXTBSY,
	CSSED_FILE_ERROR_FAULT,
	CSSED_FILE_ERROR_LOOP,
	CSSED_FILE_ERROR_NOSPC,
	CSSED_FILE_ERROR_NOMEM,
	CSSED_FILE_ERROR_MFILE,
	CSSED_FILE_ERROR_NFILE,
	CSSED_FILE_ERROR_BADF,
	CSSED_FILE_ERROR_INVAL,
	CSSED_FILE_ERROR_PIPE,
	CSSED_FILE_ERROR_AGAIN,
	CSSED_FILE_ERROR_INTR,
	CSSED_FILE_ERROR_IO,
	CSSED_FILE_ERROR_PERM,
	CSSED_FILE_ERROR_FAILED,
	// this is not an error code it's just
	// to know were the GFIleError matching ends
	CSSED_FILE_ERROR_LAST, 
	// specific cssed error codes
	CSSED_ERROR_FILE_IS_OPENED, // the file is already opened
	CSSED_ERROR_FILE_NOT_REGULAR, // the file is not a regular file, may be a device or whatever
	CSSED_ERROR_DOC_IS_NULL // a NULL document have been passed or returned
} CssedError;

/** @enum CssedEolMode cssed's end of line modes */
typedef enum {
	/** Windows end of line - Carriage return / Line feed */
	EOL_MODE_CRLF,
	/** Mac end of line - Carriage return */
	EOL_MODE_CR,
	/** Unix end of line - Line feed */
	EOL_MODE_LF
} CssedEolMode;

/** @enum CssedCodePage cssed's code pages */
typedef enum {
	/** Default code page */
	CSSED_CODEPAGE_DEFAULT,
	/** UTF8 code page */
	CSSED_CODEPAGE_UTF8,
	/** DBCS code page (Thai, Hangul) */
	CSSED_CODEPAGE_DBCS
} CssedCodePage;

typedef struct _CssedDoc CssedDoc;
typedef struct _CssedWindow CssedWindow;

// configuration will be private in a near future
typedef struct _CssedConfig {
	// window
	gint window_x;
	gint window_y;
	gint window_width;
	gint window_height;
	gint footer_pos;
	gint sidebar_pos;
	gint css_treeview_pos;
	gboolean initiall_state_maximized;
	// browser command
	gchar* browser_command;
	// document
	gboolean linenumbers;
	gboolean autocompletion;
	gboolean lineendings;
	gboolean whitespaces;
	gboolean folding;
	gboolean lineswraped;
	gchar* font;
	gint font_size;
	gboolean usetabs;
	gint tabswidth;
	// opened docs
	GList* opened_docs;
	// highlighting
	gint DEFAULT_fore_color;
	gint DEFAULT_back_color;
	gboolean DEFAULT_bold;
	gint TAG_fore_color;
	gint TAG_back_color;
	gboolean TAG_bold;
	gint PSEUDOCLASS_fore_color;
	gint PSEUDOCLASS_back_color;
	gboolean PSEUDOCLASS_bold;
	gint UNKNOWN_PSEUDOCLASS_fore_color;
	gint UNKNOWN_PSEUDOCLASS_back_color;	
	gboolean UNKNOWN_PSEUDOCLASS_bold;
	gint OPERATOR_fore_color;
	gint OPERATOR_back_color;
	gboolean OPERATOR_bold;
	gint IDENTIFIER_fore_color;	
	gint IDENTIFIER_back_color;	
	gboolean IDENTIFIER_bold;
	gint UNKNOWN_IDENTIFIER_fore_color;
	gint UNKNOWN_IDENTIFIER_back_color;
	gboolean UNKNOWN_IDENTIFIER_bold;
	gint VALUE_fore_color;
	gint VALUE_back_color;	
	gboolean VALUE_bold;
	gint COMMENT_fore_color;
	gint COMMENT_back_color;
	gboolean COMMENT_bold;
	gint IMPORTANT_fore_color;
	gint IMPORTANT_back_color;
	gboolean IMPORTANT_bold;
	gint DIRECTIVE_fore_color;
	gint DIRECTIVE_back_color;
	gboolean DIRECTIVE_bold;
	gint DOUBLESTRING_fore_color;
	gint DOUBLESTRING_back_color;	
	gboolean DOUBLESTRING_bold;
	gint SINGLESTRING_fore_color;
	gint SINGLESTRING_back_color;
	gboolean SINGLESTRING_bold;
	gint CLASS_fore_color;
	gint CLASS_back_color;	
	gboolean CLASS_bold;
	gint ID_fore_color;
	gint ID_back_color;	
	gboolean ID_bold;
	gint VALID_VALUE_fore_color;
	gint VALID_VALUE_back_color;
	gboolean VALID_VALUE_bold;
	gint FUNCTION_fore_color;
	gint FUNCTION_back_color;
	gboolean FUNCTION_bold;
	gint NUMBER_fore_color;
	gint NUMBER_back_color;
	gboolean NUMBER_bold;
	gint UNIT_fore_color;
	gint UNIT_back_color;
	gboolean UNIT_bold;
	gint COLOR_fore_color;
	gint COLOR_back_color;	
	gboolean COLOR_bold;
	gint HEXACOLOR_fore_color;
	gint HEXACOLOR_back_color;	
	gboolean HEXACOLOR_bold;
	gint ATTR_MATCH_fore_color;
	gint ATTR_MATCH_back_color;	
	gboolean ATTR_MATCH_bold;
	gint LANGUAGE_fore_color;
	gint LANGUAGE_back_color;	
	gboolean LANGUAGE_bold;
} CssedConfig;

typedef struct _CssedConfigParserData {
	CssedWindow* window;
	gint depth;
	gint state;	
	GList* plugins_list; 					// store plugins to be loaded from the config file
}  CssedConfigParserData;


/* ****************************************************************************************** *
	File typer related structures - testing here 
 * ****************************************************************************************** */

#define CSSED_FILETYPE_DEFAULT CSSED_FILETYPE_TEXT

typedef enum {
	CSSED_FILETYPE_UNKNOW,		// internall use. Never use it. 
	CSSED_FILETYPE_TEXT,		// The default file type, plain text
	CSSED_FILETYPE_BYNARY,		// not used ,but will be link insertion
	CSSED_FILETYPE_CSS,			// core language
	CSSED_FILETYPE_HTML,		// this is not just HTML but ASP and PHP
	CSSED_FILETYPE_PHP,			// This is standalon PHP not client side PHP (for sh-like scripts)
	CSSED_FILETYPE_XML,			// XML and SVG
	CSSED_FILETYPE_SH,			// shell and bash scripts
	CSSED_FILETYPE_C,			// not just C but CPP
	CSSED_FILETYPE_APACHE_CONF,	// apache conf files
	CSSED_FILETYPE_LISP,		// mostly for emacs.el files (not ready)
	CSSED_FILETYPE_PERL,		// perl scripts
	CSSED_FILETYPE_PYTHON,		// python
	CSSED_FILETYPE_SQL,			// SQL (Ansi) (not ready)
	CSSED_FILETYPE_DIFF,		// diff and patch files
	CSSED_FILETYPE_JAVA,		// java
	CSSED_FILETYPE_JS,			// javascript
	CSSED_FILETYPE_MAKE,		// Makefiles - I use make for almost everything :)
	// Those are file types supported by scintilla, so they have it's place here
	CSSED_FILETYPE_APDL,		
	CSSED_FILETYPE_AU3,
	CSSED_FILETYPE_AVE,
	CSSED_FILETYPE_ADA,
	CSSED_FILETYPE_ASM,
	CSSED_FILETYPE_ASN1,
	CSSED_FILETYPE_BAAN,
	CSSED_FILETYPE_BASIC,
	CSSED_FILETYPE_BULLANT,
	CSSED_FILETYPE_CLW,
	CSSED_FILETYPE_CAML,
	CSSED_FILETYPE_CRONTAB,
	CSSED_FILETYPE_ESCRIPT,
	CSSED_FILETYPE_EIFFEL,
	CSSED_FILETYPE_ERLANG,
	CSSED_FILETYPE_FORTH,
	CSSED_FILETYPE_FORTRAN,
	CSSED_FILETYPE_GUI4CLI,
	CSSED_FILETYPE_KIX,
	CSSED_FILETYPE_LOUT,
	CSSED_FILETYPE_LUA,
	CSSED_FILETYPE_MMIXAL,
	CSSED_FILETYPE_MPT,
	CSSED_FILETYPE_MSSQL,
	CSSED_FILETYPE_MATLAB,
	CSSED_FILETYPE_METAPOST,
	CSSED_FILETYPE_NSIS,
	CSSED_FILETYPE_PB,
	CSSED_FILETYPE_POV,
	CSSED_FILETYPE_PS,
	CSSED_FILETYPE_PASCAL,
	CSSED_FILETYPE_RUBY,
	CSSED_FILETYPE_SCRIPTOL,
	CSSED_FILETYPE_SPECMAN,
	CSSED_FILETYPE_TEX,
	CSSED_FILETYPE_VB,
	CSSED_FILETYPE_VHDL,
	CSSED_FILETYPE_VERILOG,
	CSSED_FILETYPE_YAML,
	// if you miss something here, just request  for inclusion
	CSSED_FILETYPE_USR,			// user defined file types
	CSSED_FILETYPE_USR1,
	CSSED_FILETYPE_USR2,
	CSSED_FILETYPE_USR3,
	CSSED_FILETYPE_USR4,
	CSSED_FILETYPE_USR5,
	CSSED_FILETYPE_USR6,
	CSSED_FILETYPE_USR7,
	CSSED_FILETYPE_USR8,
	CSSED_FILETYPE_USR9
} CssedFileTypeId;

#define	CSSED_NUMFILETYPES	(CSSED_FILETYPE_USR9+1)

typedef struct _CssedFileType {
	// notifications 
	void (*char_added)			 	(CssedDoc* doc, gchar  ch);
	void (*modify_attemp_read_only)	(CssedDoc* doc);
	void (*double_click)			(CssedDoc* doc);
	void (*user_list_selection)		(CssedDoc* doc, gint type, gchar* text);
	void (*dwell_start) 			(CssedDoc* doc, gint pos, gint x, gint y);
	void (*dwell_end) 				(CssedDoc* doc, gint pos, gint x, gint y);
	void (*hot_spot_click) 			(CssedDoc* doc, gint pos, gint modifiers);
	void (*hot_spot_doubleclick) 	(CssedDoc* doc, gint pos, gint modifiers);
	void (*call_tip_click)			(CssedDoc* doc, gint position) ;//position =1 if up arrow, 2 if down arrow, and 0 if elsewhere.
	// those are managed by cssed so things can break if you do something wrong here
	void (*save_point_reached) 	(CssedDoc* doc);// - managed by cssed (CssedDoc*)
	void (*save_point_left) 	(CssedDoc* doc); //- managed by cssed (CssedDoc*)
	void (*uri_dropped) 		(CssedDoc* doc, gchar* uri); //- managed by cssed (CssedDoc*)
	void (*margin_click) 		(CssedDoc* doc, gint pos, gint margin); //- managed by cssed (CssedDoc*)
	void (*modified) 			(CssedDoc* doc, gint modificationType); //- managed by cssed (CssedDoc*) 
	void (*key_pressed) 		(CssedDoc* doc, gchar key, gint modifiers);// - managed by cssed 
	// configuraton and styling , when editor != NULL, configure ir, otherwise get with document_get_scintilla_widget()
	void 		(*apply_style)						(CssedDoc* doc, GtkWidget *editor); 
	// THOSE ARE NOT IMPLEMENTED YET (FIXME)
	GtkWidget* 	(*get_config_page_widget) 			(gpointer user_data);
	void 		(*save_configuration_from_widget) 	(GtkWidget* widget, gpointer user_data);	
	void 		(*clean_popmenu) 					(CssedWindow* win);// attach should be not NULL to call this	
	void 		(*clean_menu)  						(CssedWindow* win);
	void 		(*load_ui)  						(CssedWindow* win);
	GtkWidget* 	menu_item; // (top level to the menu bar)
	GtkWidget* 	pop_menu;
	// file related issues, those are mostly to avoid to open or save files
	gboolean (*open_file) 	(gchar *filename) ;// If not NULL, cssed will call it and call document_open only if it returns TRUE 
	gboolean (*save_doc) 	(CssedDoc* doc); // If not NULL, cssed will call it and call document_save only if it returns TRUE 	
	// properties
	guint 		id; // one of the CssedFileTypeId enum values	
	gint 		style_min ;//(document_set_font_size)  (document_set_font_by_name) (Read/Write const only)
	gint 		style_max; //(document_set_font_size) (document_set_font_by_name)(Read/Write const only)
	gboolean 	can_fold ;//(Read/Write const only)
	gchar* 		label_language; // a name for the menu as CSS or XML
	gchar* 		lable_file; // a name for the file type as CSS-2 CSS-SVG or Dockbook
	GSList* 	patterns; // patterns as "*.css" "Make*" as so on
} CssedFileType;

typedef struct _CssedFileTypeManager {
	CssedFileType* filetype[CSSED_NUMFILETYPES];
	GHashTable *patterns_hash_table;
}CssedFileTypeManager ;


/* ****************************************************************************************** */
/* window functions*/
CssedWindow* cssed_window_new(void);
void cssed_window_quit (CssedWindow* window);
void cssed_window_destroy (CssedWindow* window);
gint cssed_window_get_width (CssedWindow* window);
gint cssed_window_get_height (CssedWindow* window);
GtkWidget* cssed_window_get_window_widget  (CssedWindow* window);
void cssed_window_set_window_widget  (CssedWindow* window, GtkWidget* widget);
GtkWidget* cssed_window_get_document_notebook  (CssedWindow* window);
void cssed_window_set_document_notebook  (CssedWindow* window, GtkWidget* widget);
GtkWidget* cssed_window_get_footer_notebook  (CssedWindow* window);
void cssed_window_set_footer_notebook  (CssedWindow* window, GtkWidget* widget);
GtkWidget* cssed_window_get_sidebar_notebook  (CssedWindow* window);
void cssed_window_set_sidebar_notebook  (CssedWindow* window, GtkWidget* widget);
GtkWidget* cssed_window_get_css_definition_treeview  (CssedWindow* window);
void cssed_window_set_css_definition_treeview  (CssedWindow* window, GtkWidget* widget);
GtkWidget* cssed_window_get_selector_scanner_treeview  (CssedWindow* window);
void cssed_window_set_selector_scanner_treeview  (CssedWindow* window, GtkWidget* widget);
GtkWidget* cssed_window_get_output_treeview  (CssedWindow* window);
void cssed_window_set_output_treeview  (CssedWindow* window, GtkWidget* widget);
GtkWidget* cssed_window_get_digest_treeview  (CssedWindow* window);
void cssed_window_set_digest_treeview  (CssedWindow* window, GtkWidget* widget);
GtkWidget* cssed_window_get_pop_menu  (CssedWindow* window);
void cssed_window_set_pop_menu  (CssedWindow* window, GtkWidget* widget);

gchar* cssed_window_get_scratch_buffer  (CssedWindow* window);
gchar* cssed_window_get_static_buffer  (CssedWindow* window);

void cssed_window_clear_scratch_buffer  (CssedWindow* window);
void cssed_window_clear_static_buffer  (CssedWindow* window);

void cssed_window_scratch_buffer_write  (CssedWindow* window, gchar* text);
void cssed_window_static_buffer_write  (CssedWindow* window, gchar* text);

void cssed_window_set_scratch_buffer  (CssedWindow* window, GtkTextBuffer* widget);
void cssed_window_set_static_buffer  (CssedWindow* window, GtkTextBuffer* widget);

// filetypes_menu
GtkWidget* cssed_window_get_filetypes_menu  (CssedWindow* window);
void cssed_window_set_filetypes_menu  (CssedWindow* window, GtkWidget* widget);
//
void cssed_window_dynamic_menus_set_full (CssedWindow* window,
										  GtkWidget *menu_item_line_numbers,
										  GtkWidget *menu_item_endings,
										  GtkWidget *menu_item_white_spaces,
										  GtkWidget *menu_item_lines_wrapped,
										  GtkWidget *menu_item_eol_mode_cr,
										  GtkWidget *menu_item_eol_mode_lf,
										  GtkWidget *menu_item_eol_mode_crlf,
										  GtkWidget *menu_item_autocompletion,
										  GtkWidget *menu_item_folding);
										  
void cssed_window_dynamic_menus_set_state(CssedWindow* window,
										  gboolean line_numbers_enabled,
										  gboolean line_endings_enabled,
										  gboolean white_spaces_enabled,
										  gboolean lines_wrapped_enabled,
										  gboolean autocompletion_enabled,
										  gboolean folding_enabled);
										  
void cssed_window_dynamic_menus_set_eols_state (CssedWindow* window, 
												CssedEolMode eols);										  
//
void cssed_window_dynamic_widgets_set(	CssedWindow* window,
										GtkWidget* save_button,			
										GtkWidget* save_main_menu,	
										GtkWidget* undo_button,
										GtkWidget* undo_main_menu,
										GtkWidget* undo_pop_menu,
										GtkWidget* redo_button,
										GtkWidget* redo_main_menu,
										GtkWidget* redo_pop_menu);
//
void 
cssed_window_panes_set(	CssedWindow* window, 
						GtkWidget* vpaned_main,
						GtkWidget* hpaned_up,
						GtkWidget* vpaned_sidebar);

void
cssed_window_ensure_output_visible (CssedWindow* window);

void
cssed_window_ensure_selector_scanner_visible (CssedWindow* window);

void cssed_window_set_last_open_dir (CssedWindow* window, gchar* dirname);
gchar* cssed_window_get_last_open_dir (CssedWindow* window);
void cssed_window_set_last_save_dir (CssedWindow* window, gchar* dirname);
gchar* cssed_window_get_last_save_dir (CssedWindow* window);

//void cssed_window_set_last_scan_file (CssedWindow* window, gchar* filename);
//gchar* cssed_window_get_last_scan_file (CssedWindow* window);
void cssed_window_output_write (CssedWindow* window, gchar* format, ...);
void cssed_window_output_clear (CssedWindow* window);

void cssed_window_digest_write (CssedWindow* window, gchar* first_col,  gchar* second_col);
void cssed_window_digest_clear (CssedWindow* window);

void cssed_window_selector_scanner_write (CssedWindow* window, gchar* first_col,  gchar* second_col);
void cssed_window_selector_scanner_clear (CssedWindow* window);
void cssed_window_selector_scanner_edited  (CssedWindow* window,
											gchar *path_string,
											gchar *new_text );

// config
CssedConfig* cssed_window_get_config (CssedWindow* window);
CssedConfig* cssed_window_config_new ();

// widgets
void cssed_window_enable_save  (CssedWindow* window);
void cssed_window_disable_save (CssedWindow* window);
void cssed_window_enable_undo  (CssedWindow* window);
void cssed_window_disable_undo (CssedWindow* window);
void cssed_window_enable_redo  (CssedWindow* window);
void cssed_window_disable_redo (CssedWindow* window);

// config
void cssed_window_show_preferences_dialog (CssedWindow* window);

// document list
void cssed_window_add_document_to_list (CssedWindow* window, CssedDoc* doc);
void cssed_window_delete_document_from_list (CssedWindow* window, CssedDoc* doc);
gint cssed_window_get_document_list_length (CssedWindow* window);
GList* cssed_window_get_document_list_first (CssedWindow* window);
gint cssed_window_get_document_list_index (CssedWindow* window, CssedDoc* doc);
gboolean cssed_window_set_document_list_index (CssedWindow* window, CssedDoc* doc, gint index);

// plugins list
void cssed_window_add_plugin_to_list (CssedWindow* window, gpointer plugin);
void cssed_window_delete_plugin_from_list (CssedWindow* window, gpointer plugin);
gint cssed_window_get_plugin_list_length (CssedWindow* window);
GList* cssed_window_get_plugin_list_first (CssedWindow* window);

// keyword list
gchar* cssed_window_get_keyword_list (CssedWindow* window);
void cssed_window_set_keyword_list (CssedWindow* window, gchar* keywords);
void cssed_window_free_keyword_list (CssedWindow* window);

void cssed_window_css_dialog_insert (CssedWindow* window, gchar* key, gpointer class);
void cssed_window_css_dialog_remove_by_keyword (CssedWindow* window, gchar* key);
gpointer cssed_window_css_dialog_lookup_by_keyword (CssedWindow* window, gchar* key);
gboolean cssed_window_css_dialog_keyword_in_use (CssedWindow* window, gchar* key);
gboolean cssed_window_is_file_opened (CssedWindow* window, gchar* file);
gint cssed_window_get_num_docs (CssedWindow* window);
void cssed_window_set_ndoc_active (CssedWindow* window, gint doc_number);

// file type manager
void cssed_window_set_filetype_manager (CssedWindow* window, CssedFileTypeManager* manager);
CssedFileTypeManager* cssed_window_get_filetype_manager (CssedWindow* window);
void cssed_window_add_filetype (CssedWindow* window, CssedFileType* filetype, gboolean free_previous);

// window size and paned gutter position values are now being messed by plugins
// loaded that vary the configuration stored positions. the former are set 
// before to load plugins, so plugin sizes changes the configurated positions
// This functions are just to re-set the stored values.
void cssed_window_apply_stored_size_and_position( CssedWindow* window );
void cssed_window_apply_stored_paned_positions( CssedWindow* window );


// to show help and such in a browser window
void cssed_window_show_in_default_browser (CssedWindow* window, gchar* uri);
