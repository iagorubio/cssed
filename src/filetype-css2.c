#include <gtk/gtk.h>

#ifndef PLAT_GTK
#   define PLAT_GTK 1
#endif

#include "cssedwindow.h"
#include "document.h"
#include "file-type-manager.h"
#include "callbacks.h" // on_menu_* must be moved to css2 filetype UI functions FIXME
#include "support.h"
#include "debug.h"
#include "utils.h"

#include <Scintilla.h>
#include <SciLexer.h>
#include <ScintillaWidget.h>

#define AUTOCOMP_DELAY 500 // miliseconds

static gboolean  charadded_timeout_active; // This is being tested but I'll get rid of this global ASAP.

gboolean 
css2_autocomp_execute_value (gpointer data)
{
	CssedDoc *doc;
	gint linestart;
	gchar* buffer;
	gint line;
	gint linelength;
	gint position;
		
	doc = (CssedDoc*) data;
	
	if( charadded_timeout_active ){
		// get if in selector
		if(document_current_pos_is_into_selector(doc)){
			DBGMSG (__FILE__,__LINE__,"CSS auto completion: Into a selector");
			position = document_get_current_position(doc);
			line = document_get_line_from_position(doc,position);
			linestart  = document_get_position_from_line(doc,line);
			// we can not assume the user is at the end of a line
			// so check if we are. reserve memory for the whole line
			linelength = document_get_line_length(doc,line);
			DBGMSG (__FILE__,__LINE__,"Line %d start %d pos %d length %d",line,linestart,position,linelength);

			if( position > linestart){
				buffer = g_malloc0(sizeof(gchar)*( linelength )+1);
				document_get_text_line(doc,line,buffer);
				DBGMSG (__FILE__,__LINE__,"Line contains: \"%s\"", buffer);
				buffer[position-linestart-1] = '\0';
				g_strstrip( buffer );
				DBGMSG (__FILE__,__LINE__,"Cleaned: \"%s\"", buffer);
				document_pop_submenu_from_keyword_in_treeview(doc,buffer);
				g_free(buffer);
			}
		}
		charadded_timeout_active = FALSE;	
	}
	return FALSE;
}

gboolean 
css2_autocomp_execute_property  (gpointer data)
{
	CssedWindow *window;
	CssedDoc *doc;
	
	if( charadded_timeout_active ){	
		doc = (CssedDoc*) data;	
		window = document_get_window(doc);	
		
		//document_add_text(doc,"\n");
		//document_indent_as_last_line(doc);
		document_show_user_list (doc, 0, cssed_window_get_keyword_list (window));
		charadded_timeout_active = FALSE;
	}
	return FALSE;		
}

gboolean 
css2_autocomp_execute_selector_start  (gpointer data)
{
	CssedWindow *window;
	CssedDoc *doc;

	if( charadded_timeout_active ){		
		doc = (CssedDoc*) data;	
		window = document_get_window(doc);	
		
		document_add_text(doc,"\n\t\n}\n");
		document_set_current_position(doc, (document_get_current_position(doc)-3) );
		document_show_user_list (doc, 0, cssed_window_get_keyword_list (window));
		charadded_timeout_active = FALSE;
	}
	return FALSE;		
}

void
css2_filetype_char_added(CssedDoc* doc, gchar  lastchar)
{
	CssedWindow *window;
	int pos;
	
	window = document_get_window(doc);

	if( !document_get_autocompletion_enabled(doc) ) return;
	
	pos = document_get_current_position(doc);
	
	if( lastchar == ':' /* || lastchar == ';' */ || lastchar == '{' ){
		if( lastchar == ':' && !charadded_timeout_active ){ // if it's active let it go
			charadded_timeout_active = TRUE;
			g_timeout_add (AUTOCOMP_DELAY, css2_autocomp_execute_value, doc);	
			return;		
		//}else if(  lastchar == ';' && !charadded_timeout_active ){ // if it's active let it go
		//	charadded_timeout_active = TRUE;
		//	g_timeout_add (AUTOCOMP_DELAY, css2_autocomp_execute_property, doc);	
		//	return;		
		}else if(  lastchar == '{' && !charadded_timeout_active ){ // if it's active let it go
			charadded_timeout_active = TRUE;
			g_timeout_add (AUTOCOMP_DELAY, css2_autocomp_execute_selector_start, doc);	
			return;		
		}
	}else if( lastchar == '\n'){ // indent and test if last line endedd in ;
		if( !charadded_timeout_active ){
			if(  pos > 3 && document_get_char_at(doc, pos-2) == ';' ){
				charadded_timeout_active = TRUE;
				g_timeout_add (AUTOCOMP_DELAY, css2_autocomp_execute_property, doc);				
				document_indent_as_last_line(doc);
				return;
			}else{
				document_indent_as_last_line(doc);
			}
		}else{
			document_indent_as_last_line(doc);
		}
	}else{
		charadded_timeout_active = FALSE;
	}
	
	if( lastchar == '*' ){ // close comments
		if(  pos >  2 && document_get_char_at(doc, pos-2) == '/' ){
			document_add_text(doc,"  */");
			document_set_current_position(doc, (document_get_current_position(doc)-3) );
		}
	}
}

void
css2_filetype_set_keyworkds (ScintillaObject * sci, gchar* keywords)
{
	enum {
		SC_KEYS_PROPERTIES,
		SC_KEYS_PSEUDO,
		SC_KEYS_VALUES,
		SC_KEYS_FUNCS,
		SC_KEYS_UNITS,
		SC_KEYS_COLORS
	};

	const gchar* values = "above absolute always armenian auto avoid baseline behind below bidi-override \
blink block bold bolder both bottom capitalize caption center center-left center-right child circle cjk-ideographic close-quote \
code collapse collapse compact condensed continuous crop cross crosshair dashed decimal decimal-leading-zero default digits \
disc dotted double embed e-resize expanded extra-condensed extra-expanded far-left far-right fast female fixed \
georgian groove hebrew help hide high hiragana hidden higher hiragana-iroha icon inherit inline inset inside invert inline-table faster \
invert italic justify katakana katakana-iroha large larger landscape left left-side leftwards level lighter line-through list-item loud \
low lower lowercase lower-greek lower-alpha lower-latin lower-roman ltr male marker medium menu message-box middle mix move ne-resize \
no-close-quote none no-open-quote no-repeat normal nowrap n-resize nw-resize oblique once open-quote outset outside overline \
pointer portrait pre relative repeat repeat-x repeat-y ridge right rightwards right-side rtl run-in scroll semi-condensed semi-expanded \
separate se-resize show silent slow slower small smaller small-caps small-caption side soft solid spell-out square s-resize static status-bar sub super sw-resize \
table table-caption table-cell table-column table-column-group table-footer-group table-header-group table-row table-row-group text thick thin \
top text-bottom text-top transparent ultra-condensed ultra-expanded underline upper-alpha uppercase upper-latin upper-roman visible wait wider w-resize x-fast \
x-high x-loud x-low x-slow x-soft  xx-small x-large xx-small x-small xx-large serif sans-serif cursive fantasy monospace";

	const gchar* namedColors = "aqua black blue fuchsia green lime maroon  navy olive purple red silver teal white yellow activeborder \
activecaption inactivecolor activecolor appworkspace background buttonface buttonhighlight buttonshadow buttontext captiontext graytext gray highlighttext highlight \
inactiveborder inactivecaptiontext inactivecaption infobackground infotext menu menutext scrollbar threeddarkshadow threedface threedhighlight \
threedlightshadow threedshadow windowframe windowtext window";

	SSM (sci, SCI_SETKEYWORDS, SC_KEYS_PROPERTIES, (sptr_t) keywords);
	SSM (sci, SCI_SETKEYWORDS, SC_KEYS_PSEUDO, (sptr_t) "active aural after before first first-letter first-line first-child focus hover link print visited visual left");
	SSM (sci, SCI_SETKEYWORDS, SC_KEYS_VALUES, (sptr_t) values);
	SSM (sci, SCI_SETKEYWORDS, SC_KEYS_FUNCS, (sptr_t) "counter attr url rgb rect");
	SSM (sci, SCI_SETKEYWORDS, SC_KEYS_UNITS, (sptr_t) "em ex px in cm mm pt pc mhz khz deg rad gra ms s"); // \%  is recogniced by the lexer
	SSM (sci, SCI_SETKEYWORDS, SC_KEYS_COLORS, (sptr_t) namedColors);
}

void
css2_filetype_set_style (CssedDoc* doc, GtkWidget* editor)
{
	ScintillaObject * sci;
	GtkWidget *scintilla;
	CssedWindow* window;
	CssedConfig* cfg;

	window = document_get_window(doc);
	cfg = cssed_window_get_config(window);
	
	if( editor ) scintilla = editor;
	else scintilla = document_get_scintilla_widget(doc);
	
	sci = SCINTILLA(scintilla);

	SSM (sci, SCI_STYLECLEARALL, 0, 0);
	SSM (sci, SCI_SETLEXER, SCLEX_CSS, 0);

	SSM (sci, SCI_SETWORDCHARS, 0, (sptr_t) "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789-#_");
	
	SSM (sci, SCI_STYLESETFORE, SCE_CSS_DEFAULT, cfg->DEFAULT_fore_color);
	SSM (sci, SCI_STYLESETBACK, SCE_CSS_DEFAULT, cfg->DEFAULT_back_color);
	SSM (sci, SCI_STYLESETBOLD, SCE_CSS_DEFAULT, cfg->DEFAULT_bold);
	SSM (sci, SCI_STYLESETFORE, SCE_CSS_TAG, cfg->TAG_fore_color);
	SSM (sci, SCI_STYLESETBACK, SCE_CSS_TAG, cfg->TAG_back_color);
	SSM (sci, SCI_STYLESETBOLD, SCE_CSS_TAG, cfg->TAG_bold);
	SSM (sci, SCI_STYLESETFORE, SCE_CSS_PSEUDOCLASS, cfg->PSEUDOCLASS_fore_color);
	SSM (sci, SCI_STYLESETBACK, SCE_CSS_PSEUDOCLASS, cfg->PSEUDOCLASS_back_color);
	SSM (sci, SCI_STYLESETBOLD, SCE_CSS_PSEUDOCLASS, cfg->PSEUDOCLASS_bold);
	SSM (sci, SCI_STYLESETFORE, SCE_CSS_UNKNOWN_PSEUDOCLASS, cfg->UNKNOWN_PSEUDOCLASS_fore_color);
	SSM (sci, SCI_STYLESETBACK, SCE_CSS_UNKNOWN_PSEUDOCLASS, cfg->UNKNOWN_PSEUDOCLASS_back_color);
	SSM (sci, SCI_STYLESETBOLD, SCE_CSS_UNKNOWN_PSEUDOCLASS, cfg->UNKNOWN_PSEUDOCLASS_bold);
	SSM (sci, SCI_STYLESETFORE, SCE_CSS_OPERATOR, cfg->OPERATOR_fore_color);
	SSM (sci, SCI_STYLESETBACK, SCE_CSS_OPERATOR, cfg->OPERATOR_back_color);
	SSM (sci, SCI_STYLESETBOLD, SCE_CSS_OPERATOR, cfg->OPERATOR_bold);
	SSM (sci, SCI_STYLESETFORE, SCE_CSS_IDENTIFIER, cfg->IDENTIFIER_fore_color);
	SSM (sci, SCI_STYLESETBACK, SCE_CSS_IDENTIFIER, cfg->IDENTIFIER_back_color);
	SSM (sci, SCI_STYLESETBOLD, SCE_CSS_IDENTIFIER, cfg->IDENTIFIER_bold);
	SSM (sci, SCI_STYLESETFORE, SCE_CSS_UNKNOWN_IDENTIFIER, cfg->UNKNOWN_IDENTIFIER_fore_color);
	SSM (sci, SCI_STYLESETBACK, SCE_CSS_UNKNOWN_IDENTIFIER, cfg->UNKNOWN_IDENTIFIER_back_color);
	SSM (sci, SCI_STYLESETBOLD, SCE_CSS_UNKNOWN_IDENTIFIER, cfg->UNKNOWN_IDENTIFIER_bold);
	SSM (sci, SCI_STYLESETFORE, SCE_CSS_VALUE, cfg->VALUE_fore_color);
	SSM (sci, SCI_STYLESETBACK, SCE_CSS_VALUE, cfg->VALUE_back_color);
	SSM (sci, SCI_STYLESETBOLD, SCE_CSS_VALUE, cfg->VALUE_bold);
	SSM (sci, SCI_STYLESETFORE, SCE_CSS_COMMENT, cfg->COMMENT_fore_color);
	SSM (sci, SCI_STYLESETBACK, SCE_CSS_COMMENT, cfg->COMMENT_back_color);
	SSM (sci, SCI_STYLESETBOLD, SCE_CSS_COMMENT, cfg->COMMENT_bold);
	SSM (sci, SCI_STYLESETFORE, SCE_CSS_IMPORTANT, cfg->IMPORTANT_fore_color);
	SSM (sci, SCI_STYLESETBACK, SCE_CSS_IMPORTANT, cfg->IMPORTANT_back_color);
	SSM (sci, SCI_STYLESETBOLD, SCE_CSS_IMPORTANT, cfg->IMPORTANT_bold);
	SSM (sci, SCI_STYLESETFORE, SCE_CSS_DIRECTIVE, cfg->DIRECTIVE_fore_color);
	SSM (sci, SCI_STYLESETBACK, SCE_CSS_DIRECTIVE, cfg->DIRECTIVE_back_color);
	SSM (sci, SCI_STYLESETBOLD, SCE_CSS_DIRECTIVE, cfg->DIRECTIVE_bold);
	SSM (sci, SCI_STYLESETFORE, SCE_CSS_DOUBLESTRING, cfg->DOUBLESTRING_fore_color);
	SSM (sci, SCI_STYLESETBACK, SCE_CSS_DOUBLESTRING, cfg->DOUBLESTRING_back_color);
	SSM (sci, SCI_STYLESETBOLD, SCE_CSS_DOUBLESTRING, cfg->DOUBLESTRING_bold);
	SSM (sci, SCI_STYLESETFORE, SCE_CSS_SINGLESTRING, cfg->SINGLESTRING_fore_color);
	SSM (sci, SCI_STYLESETBACK, SCE_CSS_SINGLESTRING, cfg->SINGLESTRING_back_color);
	SSM (sci, SCI_STYLESETBOLD, SCE_CSS_SINGLESTRING, cfg->SINGLESTRING_bold);
	SSM (sci, SCI_STYLESETFORE, SCE_CSS_CLASS, cfg->CLASS_fore_color);
	SSM (sci, SCI_STYLESETBACK, SCE_CSS_CLASS, cfg->CLASS_back_color);
	SSM (sci, SCI_STYLESETBOLD, SCE_CSS_CLASS, cfg->CLASS_bold);
	SSM (sci, SCI_STYLESETFORE, SCE_CSS_ID, cfg->ID_fore_color);
	SSM (sci, SCI_STYLESETBACK, SCE_CSS_ID, cfg->ID_back_color);
	SSM (sci, SCI_STYLESETBOLD, SCE_CSS_ID, cfg->ID_bold);
	SSM (sci, SCI_STYLESETFORE, SCE_CSS_VALID_VALUE, cfg->VALID_VALUE_fore_color);
	SSM (sci, SCI_STYLESETBACK, SCE_CSS_VALID_VALUE, cfg->VALID_VALUE_back_color);
	SSM (sci, SCI_STYLESETBOLD, SCE_CSS_VALID_VALUE, cfg->VALID_VALUE_bold);
	SSM (sci, SCI_STYLESETFORE, SCE_CSS_FUNCTION, cfg->FUNCTION_fore_color);
	SSM (sci, SCI_STYLESETBACK, SCE_CSS_FUNCTION, cfg->FUNCTION_back_color);
	SSM (sci, SCI_STYLESETBOLD, SCE_CSS_FUNCTION, cfg->FUNCTION_bold);
	SSM (sci, SCI_STYLESETFORE, SCE_CSS_NUMBER, cfg->NUMBER_fore_color);
	SSM (sci, SCI_STYLESETBACK, SCE_CSS_NUMBER, cfg->NUMBER_back_color);
	SSM (sci, SCI_STYLESETBOLD, SCE_CSS_NUMBER, cfg->NUMBER_bold);
	SSM (sci, SCI_STYLESETFORE, SCE_CSS_UNIT, cfg->UNIT_fore_color);
	SSM (sci, SCI_STYLESETBACK, SCE_CSS_UNIT, cfg->UNIT_back_color);
	SSM (sci, SCI_STYLESETBOLD, SCE_CSS_UNIT, cfg->UNIT_bold);
	SSM (sci, SCI_STYLESETFORE, SCE_CSS_COLOR, cfg->COLOR_fore_color);
	SSM (sci, SCI_STYLESETBACK, SCE_CSS_COLOR, cfg->COLOR_back_color);
	SSM (sci, SCI_STYLESETBOLD, SCE_CSS_COLOR, cfg->COLOR_bold);
	SSM (sci, SCI_STYLESETFORE, SCE_CSS_HEXACOLOR, cfg->HEXACOLOR_fore_color);
	SSM (sci, SCI_STYLESETBACK, SCE_CSS_HEXACOLOR, cfg->HEXACOLOR_back_color);
	SSM (sci, SCI_STYLESETBOLD, SCE_CSS_HEXACOLOR, cfg->HEXACOLOR_bold);
	SSM (sci, SCI_STYLESETFORE, SCE_CSS_ATTR_MATCH, cfg->ATTR_MATCH_fore_color);
	SSM (sci, SCI_STYLESETBACK, SCE_CSS_ATTR_MATCH, cfg->ATTR_MATCH_back_color);
	SSM (sci, SCI_STYLESETBOLD, SCE_CSS_ATTR_MATCH, cfg->ATTR_MATCH_bold);
	SSM (sci, SCI_STYLESETFORE, SCE_CSS_LANGUAGE, cfg->LANGUAGE_fore_color);
	SSM (sci, SCI_STYLESETBACK, SCE_CSS_LANGUAGE, cfg->LANGUAGE_back_color);
	SSM (sci, SCI_STYLESETBOLD, SCE_CSS_LANGUAGE, cfg->LANGUAGE_bold);

	css2_filetype_set_keyworkds (sci, (gchar*) cssed_window_get_keyword_list(window));
}

void 
css2_filetype_key_pressed 	(CssedDoc* doc, gchar key, gint modifiers)
{
	CssedWindow *window;
	gint kill_line;
	gint start_pos;
	gint line_len;
	gint end_pos;
	gint curpos;

	window = document_get_window(doc);
	
	if( (key=='a') && (modifiers==4) ){ // Alt + A fire autocompletion
		document_show_user_list (doc,0, cssed_window_get_keyword_list( window ));
	}else if( (key=='l') && (modifiers==4) ){ // Alt + l selects current line
		curpos = document_get_current_position(doc);
		kill_line = document_get_line_from_position(doc, curpos);
		start_pos = document_get_position_from_line(doc, kill_line);
		line_len = document_get_line_length(doc, kill_line);
		end_pos = start_pos + line_len;
		document_set_selection_range(doc, start_pos, end_pos);
	}else if((key==';') && ((modifiers==5)||(modifiers==4))){ // Alt + ";" adds ; (no auto complete)
		document_add_text(doc,";");
	}else if((key==':') && ((modifiers==5)||(modifiers==4))){ // Alt + ":" adds ; (no auto complete)
		document_add_text(doc,":");
	}else if((key=='{') && ((modifiers==5)||(modifiers==4))){ // Alt + "{" adds { (no auto complete)
		document_add_text(doc,"{");
	}else if( (key=='s') && (modifiers==4) ){ // Alt + s scan selector
		on_menu_scan_selector_activate (NULL, window);
	}
}

void
create_and_register_css2_file_type (CssedWindow* window)
{
	CssedFileType* css2;
	GtkWidget* css_menu;

	css_menu = gtk_menu_item_new_with_label("CSS");

	css2 = cssed_file_type_new();
	css2->char_added  = css2_filetype_char_added;

	css2->modify_attemp_read_only = NULL;
	css2->double_click = NULL;
	css2->user_list_selection = NULL;
	css2->dwell_start = NULL;
	css2->dwell_end = NULL;
	css2->hot_spot_click = NULL;
	css2->hot_spot_doubleclick = NULL;
	css2->call_tip_click = NULL;

	css2->save_point_reached = NULL;
	css2->save_point_left = NULL;
	css2->uri_dropped = NULL;
	css2->margin_click = NULL;
	css2->modified = NULL;

	css2->key_pressed = css2_filetype_key_pressed;
	// configuraton and styling
	css2->apply_style = css2_filetype_set_style;

	// THOSE ARE NOT IMPLEMENTED YET !!!! (FIXME)
	css2->get_config_page_widget = NULL;
	css2->save_configuration_from_widget = NULL;	
	css2->pop_menu = NULL;
	css2->clean_popmenu = NULL;
	css2->menu_item = css_menu;
	css2->clean_menu = NULL;
	css2->load_ui = NULL;

	// file related issues, those are mostly to avoid to open or save files
	css2->open_file = NULL;
	css2->save_doc = NULL;

	css2->id = CSSED_FILETYPE_CSS;
	css2->style_min = SCE_CSS_DEFAULT;
	css2->style_max = SCE_CSS_LANGUAGE;
	css2->can_fold = TRUE;
	css2->label_language = g_strdup("CSS"); // a name for the menu as CSS or XML
	css2->lable_file = g_strdup("CSS-2");

	cssed_file_type_add_pattern_spec (css2, "*.css");
	cssed_file_type_menu_set_default_callback (window, css_menu, css2);
	cssed_window_add_filetype (window, css2, FALSE);
}
