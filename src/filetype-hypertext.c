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
hypertext_filetype_char_added(CssedDoc* doc, gchar  lastchar)
{
	if( lastchar == '\n')
		document_indent_as_last_line(doc);
}

void
filetype_hypertext_set_style( CssedDoc* doc, GtkWidget* editor)
{
	ScintillaObject * sci;
	GtkWidget *scintilla;
	CssedWindow* window;
	CssedConfig* cfg;
	gint bstyle;
	gint bastyle;
	gint jstyle;
	gint jastyle;
	gint hpstyle;

	const gchar htmlKeyWords[] =
		"a abbr acronym address applet area b base basefont bdo big blockquote body br button caption center cite code col colgroup dd del dfn dir div dl dt em embed fieldset font form frame frameset h1 h2 h3 h4 h5 h6 head hr html i iframe img input ins isindex kbd label legend li link map menu meta noframes noscript object ol optgroup option p param pre q quality s samp script select small span strike strong style sub sup table tbody td textarea tfoot th thead title tr tt u ul var xmlns leftmargin topmargin abbr accept-charset accept accesskey action align alink alt archive axis background bgcolor border cellpadding cellspacing char charoff charset checked cite class classid clear codebase codetype color cols colspan compact content coords data datafld dataformatas datapagesize datasrc datetime declare defer dir disabled enctype face for frame frameborder selected headers height href hreflang hspace http-equiv id ismap label lang language link longdesc marginwidth marginheight maxlength media framespacing method multiple name nohref noresize noshade nowrap object onblur onchange onclick ondblclick onfocus onkeydown onkeypress onkeyup onload onmousedown onmousemove onmouseover onmouseout onmouseup onreset onselect onsubmit onunload profile prompt pluginspage readonly rel rev rows rowspan rules scheme scope scrolling shape size span src standby start style summary tabindex target text title type usemap valign value valuetype version vlink vspace width text password checkbox radio submit reset file hidden image public doctype xml";

	const gchar jsKeyWords[] =
		"break this for while null else var false void new delete typeof if in continue true function with return case super extends do const try debugger catch switch finally enum export default class throw import length concat join pop push reverse shift slice splice sort unshift Date Infinity NaN undefined escape eval isFinite isNaN Number parseFloat parseInt string unescape Math abs acos asin atan atan2 ceil cos exp floor log max min pow random round sin sqrt tan MAX_VALUE MIN_VALUE NEGATIVE_INFINITY POSITIVE_INFINITY toString valueOf String length anchor big bold charAt charCodeAt concat fixed fontcolor fontsize fromCharCode indexOf italics lastIndexOf link slice small split strike sub substr substring sup toLowerCase toUpperCase";

	const gchar vbsKeyWords[] =
		"and as byref byval case call const continue dim do each else elseif end error exit false for function global goto if in loop me new next not nothing on optional or private public redim rem resume select set sub then to true type while with boolean byte currency date double integer long object single string type variant";

	const gchar pyKeyWords[] = 	"and assert break class continue complex def del elif else except exec finally for from global if import in is int lambda not or pass print raise return tuple try unicode while yield long float str list";


	const gchar phpKeyWords[] = "and or xor FILE exception php_user_filter LINE array as break case cfunction class const continue declare default die do echo else elseif empty enddeclare endfor endforeach endif endswitch endwhile eval exit extends for foreach function global if include include_once isset list new old_function print require require_once return static switch unset use var while FUNCTION CLASS	METHOD";

	const gchar sgmlKeyWords[] = "DOCTYPE";

	window = document_get_window(doc);
	cfg = cssed_window_get_config(window);
	
	if( editor ) scintilla = editor;
	else scintilla = document_get_scintilla_widget(doc);
	
	sci = SCINTILLA(scintilla);

	SSM (sci, SCI_SETPROPERTY, (sptr_t) "fold.html", (sptr_t) "1");
	SSM (sci, SCI_SETPROPERTY, (sptr_t) "fold.html.preprocessor", (sptr_t) "1");

	SSM (sci, SCI_STYLECLEARALL, 0, 0);
	SSM (sci, SCI_SETLEXER, SCLEX_HTML, 0);
	SSM (sci,SCI_SETSTYLEBITS, 7, 0);

	SSM(sci, SCI_SETKEYWORDS, 0, (sptr_t) htmlKeyWords);
	SSM(sci, SCI_SETKEYWORDS, 1, (sptr_t) jsKeyWords);
	SSM(sci, SCI_SETKEYWORDS, 2, (sptr_t) vbsKeyWords);
	SSM(sci, SCI_SETKEYWORDS, 3, (sptr_t) pyKeyWords);
	SSM(sci, SCI_SETKEYWORDS, 4, (sptr_t) phpKeyWords);
	SSM(sci, SCI_SETKEYWORDS, 5, (sptr_t) sgmlKeyWords);

	// Unknown tags and attributes are highlighed in red.
	// If a tag is actually OK, it should be added in lower case to the htmlKeyWords string.

	SSM(sci, SCI_STYLESETFORE, SCE_H_DEFAULT, 0x000000);
	SSM(sci, SCI_STYLESETBACK, SCE_H_DEFAULT, 0xffffff);

	SSM(sci, SCI_STYLESETFORE, SCE_H_TAG, 0x990000);
	SSM(sci, SCI_STYLESETBACK, SCE_H_TAG, 0xffffff);

	SSM(sci, SCI_STYLESETFORE, SCE_H_TAGUNKNOWN, 0x0000ff);
	SSM(sci, SCI_STYLESETBACK, SCE_H_TAGUNKNOWN, 0xffffff);

	SSM(sci, SCI_STYLESETFORE, SCE_H_ATTRIBUTE, 0x990066);
	SSM(sci, SCI_STYLESETBACK, SCE_H_ATTRIBUTE, 0xffffff);

	SSM(sci, SCI_STYLESETFORE, SCE_H_ATTRIBUTEUNKNOWN, 0x00ff);
	SSM(sci, SCI_STYLESETBACK, SCE_H_ATTRIBUTEUNKNOWN, 0xffffff);

	SSM(sci, SCI_STYLESETFORE, SCE_H_NUMBER, 0x800080);
	SSM(sci, SCI_STYLESETBACK, SCE_H_NUMBER, 0xffffff);

	SSM(sci, SCI_STYLESETFORE, SCE_H_DOUBLESTRING, 0x008000);
	SSM(sci, SCI_STYLESETBACK, SCE_H_DOUBLESTRING, 0xffffff);

	SSM(sci, SCI_STYLESETFORE, SCE_H_SINGLESTRING, 0x008000);
	SSM(sci, SCI_STYLESETBACK, SCE_H_SINGLESTRING, 0xffffff);

	SSM(sci, SCI_STYLESETFORE, SCE_H_OTHER, 0x800080);
	SSM(sci, SCI_STYLESETBACK, SCE_H_OTHER, 0xffffff);

	SSM(sci, SCI_STYLESETFORE, SCE_H_COMMENT, 0x808080);
	SSM(sci, SCI_STYLESETBACK, SCE_H_COMMENT, 0xffffff);

	SSM(sci, SCI_STYLESETFORE, SCE_H_ENTITY, 0x800080);
	SSM(sci, SCI_STYLESETBACK, SCE_H_ENTITY, 0xffffff);

	SSM(sci, SCI_STYLESETFORE, SCE_H_TAGEND, 0x800000);
	SSM(sci, SCI_STYLESETBACK, SCE_H_TAGEND, 0xffffff);

	SSM(sci, SCI_STYLESETFORE, SCE_H_XMLSTART, 0x990000);// <?
	SSM(sci, SCI_STYLESETBACK, SCE_H_XMLSTART, 0xf0f0f0);
	SSM(sci, SCI_STYLESETEOLFILLED, SCE_H_XMLSTART, 1);

	SSM(sci, SCI_STYLESETFORE, SCE_H_XMLEND, 0x990000);// ?>
	SSM(sci, SCI_STYLESETBACK, SCE_H_XMLEND, 0xf0f0f0);

	SSM(sci, SCI_STYLESETFORE, SCE_H_SCRIPT, 0x800000);// <script
	SSM(sci, SCI_STYLESETBACK, SCE_H_SCRIPT, 0xf0f0f0);

	SSM(sci, SCI_STYLESETFORE, SCE_H_ASP, 0x4f4f00);	// <% ... %>
	SSM(sci, SCI_STYLESETBACK, SCE_H_ASP, 0xf0f0f0);
	SSM(sci, SCI_STYLESETEOLFILLED, SCE_H_ASP, 1);

	SSM(sci, SCI_STYLESETFORE, SCE_H_ASPAT, 0x4f4f00);	// <%@ ... %>
	SSM(sci, SCI_STYLESETBACK, SCE_H_ASPAT, 0xf0f0f0);
	SSM(sci, SCI_STYLESETEOLFILLED, SCE_H_ASPAT, 1);

	SSM(sci, SCI_STYLESETFORE, SCE_H_CDATA, 0x990066);
	SSM(sci, SCI_STYLESETBACK, SCE_H_CDATA, 0xffffff);

	SSM(sci, SCI_STYLESETFORE, SCE_H_QUESTION, 0x990066);
	SSM(sci, SCI_STYLESETBACK, SCE_H_QUESTION, 0xffffff);

	SSM(sci, SCI_STYLESETFORE, SCE_H_VALUE, 0x990066);
	SSM(sci, SCI_STYLESETBACK, SCE_H_VALUE, 0xffffff);

	SSM(sci, SCI_STYLESETFORE, SCE_H_XCCOMMENT, 0x990066);
	SSM(sci, SCI_STYLESETBACK, SCE_H_XCCOMMENT, 0xffffff);

	SSM(sci, SCI_STYLESETFORE, SCE_H_SGML_DEFAULT, 0x990066);
	SSM(sci, SCI_STYLESETBACK, SCE_H_SGML_DEFAULT, 0xffffff);

	SSM(sci, SCI_STYLESETFORE, SCE_H_SGML_COMMAND, 0x990000);
	SSM(sci, SCI_STYLESETBACK, SCE_H_SGML_COMMAND, 0xffffff);

	SSM(sci, SCI_STYLESETFORE, SCE_H_SGML_1ST_PARAM, 0x808000);
	SSM(sci, SCI_STYLESETBACK, SCE_H_SGML_1ST_PARAM, 0xffffff);

	SSM(sci, SCI_STYLESETFORE, SCE_H_SGML_DOUBLESTRING, 0x008000);
	SSM(sci, SCI_STYLESETBACK, SCE_H_SGML_DOUBLESTRING, 0xffffff);

	SSM(sci, SCI_STYLESETFORE, SCE_H_SGML_SIMPLESTRING, 0x008000);
	SSM(sci, SCI_STYLESETBACK, SCE_H_SGML_SIMPLESTRING, 0xffffff);

	SSM(sci, SCI_STYLESETFORE, SCE_H_SGML_ERROR, 0x000080);
	SSM(sci, SCI_STYLESETBACK, SCE_H_SGML_ERROR, 0xffffff);

	SSM(sci, SCI_STYLESETFORE, SCE_H_SGML_SPECIAL, 0x008080);
	SSM(sci, SCI_STYLESETBACK, SCE_H_SGML_SPECIAL, 0xffffff);

	SSM(sci, SCI_STYLESETFORE, SCE_H_SGML_ENTITY, 0x808060);
	SSM(sci, SCI_STYLESETBACK, SCE_H_SGML_ENTITY, 0xffffff);

	SSM(sci, SCI_STYLESETFORE, SCE_H_SGML_COMMENT, 0x202020);
	SSM(sci, SCI_STYLESETBACK, SCE_H_SGML_COMMENT, 0xffffff);

	SSM(sci, SCI_STYLESETFORE, SCE_H_SGML_1ST_PARAM_COMMENT, 0x202020);
	SSM(sci, SCI_STYLESETBACK, SCE_H_SGML_1ST_PARAM_COMMENT, 0xffffff);

	SSM(sci, SCI_STYLESETFORE, SCE_H_SGML_BLOCK_DEFAULT, 0x808020);
	SSM(sci, SCI_STYLESETBACK, SCE_H_SGML_BLOCK_DEFAULT, 0xffffff);

	SSM(sci, SCI_STYLESETFORE, SCE_HB_DEFAULT, 0x000000);
	SSM(sci, SCI_STYLESETBACK, SCE_HB_DEFAULT, 0xffffff);

	SSM(sci, SCI_STYLESETFORE, SCE_HB_COMMENTLINE, 0x808080);
	SSM(sci, SCI_STYLESETBACK, SCE_HB_COMMENTLINE, 0xffffff);

	SSM(sci, SCI_STYLESETFORE, SCE_HB_NUMBER, 0x008080);
	SSM(sci, SCI_STYLESETBACK, SCE_HB_NUMBER, 0xffffff);

	SSM(sci, SCI_STYLESETFORE, SCE_HB_WORD, 0x008080);
	SSM(sci, SCI_STYLESETBACK, SCE_HB_WORD, 0xffffff);
	SSM(sci, SCI_STYLESETBOLD, SCE_HB_WORD, 1);

	SSM(sci, SCI_STYLESETFORE, SCE_HB_STRING, 0x008000);
	SSM(sci, SCI_STYLESETBACK, SCE_HB_STRING, 0x008000);

	SSM(sci, SCI_STYLESETFORE, SCE_HB_IDENTIFIER, 0x103000);
	SSM(sci, SCI_STYLESETBACK, SCE_HB_IDENTIFIER, 0xffffff);
//~ #define SCE_HB_START 70

	// Show the whole section of VBScript
	for (bstyle=SCE_HB_DEFAULT; bstyle<=SCE_HB_STRINGEOL; bstyle++) {
		SSM(sci, SCI_STYLESETBACK, bstyle, 0xf5f5f5);
		// This call extends the backround colour of the last style on the line to the edge of the window
		SSM(sci, SCI_STYLESETEOLFILLED, bstyle, 1);
	}
	SSM(sci,SCI_STYLESETBACK, SCE_HB_STRINGEOL, 0x7F7FFF);

	SSM(sci, SCI_STYLESETFORE, SCE_HBA_DEFAULT, 0x000000);
	SSM(sci, SCI_STYLESETBACK, SCE_HBA_DEFAULT, 0xffffff);

	SSM(sci, SCI_STYLESETFORE, SCE_HBA_COMMENTLINE, 0x808080);
	SSM(sci, SCI_STYLESETBACK, SCE_HBA_COMMENTLINE, 0xffffff);

	SSM(sci, SCI_STYLESETFORE, SCE_HBA_NUMBER, 0x008080);
	SSM(sci, SCI_STYLESETBACK, SCE_HBA_NUMBER, 0xffffff);

	SSM(sci, SCI_STYLESETFORE, SCE_HBA_WORD, 0x008080);
	SSM(sci, SCI_STYLESETBACK, SCE_HBA_WORD, 0xffffff);
	SSM(sci, SCI_STYLESETBOLD, SCE_HBA_WORD, 1);

	SSM(sci, SCI_STYLESETFORE, SCE_HBA_STRING, 0x008000);
	SSM(sci, SCI_STYLESETBACK, SCE_HBA_STRING, 0xffffff);

	SSM(sci, SCI_STYLESETFORE, SCE_HBA_IDENTIFIER, 0x103000);
	SSM(sci, SCI_STYLESETBACK, SCE_HBA_IDENTIFIER, 0xffffff);

	// Show the whole section of ASP VBScript
	for (bastyle=SCE_HBA_DEFAULT; bastyle<=SCE_HBA_STRINGEOL; bastyle++) {
		SSM(sci, SCI_STYLESETBACK, bastyle, 0xf5f5f5);
		// This call extends the backround colour of the last style on the line to the edge of the window
		SSM(sci, SCI_STYLESETEOLFILLED, bastyle, 1);
	}
	//SSM(sci, SCI_STYLESETBACK, SCE_HBA_STRINGEOL, 0xCFCF7F);

//~ #define SCE_HBA_START 80

	// If there is no need to support embedded Javascript, the following code can be dropped.
	// Javascript will still be correctly processed but will be displayed in the default style.
	SSM(sci, SCI_STYLESETFORE, SCE_HJ_START, 0x808000);
	SSM(sci, SCI_STYLESETBACK, SCE_HJ_START, 0xffffff);

	SSM(sci, SCI_STYLESETFORE, SCE_HJ_DEFAULT, 0x000000);
	SSM(sci, SCI_STYLESETBACK, SCE_HJ_DEFAULT, 0xffffff);

	SSM(sci, SCI_STYLESETFORE, SCE_HJ_COMMENT, 0x222222);
	SSM(sci, SCI_STYLESETBACK, SCE_HJ_COMMENT, 0xffffff);

	SSM(sci, SCI_STYLESETFORE, SCE_HJ_COMMENTLINE, 0x808080);
	SSM(sci, SCI_STYLESETBACK, SCE_HJ_COMMENTLINE, 0xffffff);

	SSM(sci, SCI_STYLESETFORE, SCE_HJ_COMMENTDOC, 0x808080);
	SSM(sci, SCI_STYLESETBACK, SCE_HJ_COMMENTDOC, 0xffffff);

	SSM(sci, SCI_STYLESETFORE, SCE_HJ_NUMBER, 0x606000);
	SSM(sci, SCI_STYLESETBACK, SCE_HJ_NUMBER, 0xffffff);

	SSM(sci, SCI_STYLESETFORE, SCE_HJ_WORD, 0x000000);
	SSM(sci, SCI_STYLESETBACK, SCE_HJ_WORD, 0xffffff);

	SSM(sci, SCI_STYLESETFORE, SCE_HJ_KEYWORD, 0x001050);
	SSM(sci, SCI_STYLESETBACK, SCE_HJ_KEYWORD, 0xffffff);

	SSM(sci, SCI_STYLESETFORE, SCE_HJ_DOUBLESTRING, 0x080080);
	SSM(sci, SCI_STYLESETBACK, SCE_HJ_DOUBLESTRING, 0xffffff);

	SSM(sci, SCI_STYLESETFORE, SCE_HJ_DOUBLESTRING, 0x800080);
	SSM(sci, SCI_STYLESETBACK, SCE_HJ_DOUBLESTRING, 0xffffff);

	SSM(sci, SCI_STYLESETFORE, SCE_HJ_SYMBOLS, 0x501000);
	SSM(sci, SCI_STYLESETBACK, SCE_HJ_SYMBOLS, 0xffffff);

//~ #define SCE_HJ_REGEX 52
	SSM(sci, SCI_STYLESETFORE, SCE_HJA_START, 0x808000);
	SSM(sci, SCI_STYLESETBACK, SCE_HJA_START, 0xffffff);

	SSM(sci, SCI_STYLESETFORE, SCE_HJA_DEFAULT, 0x000000);
	SSM(sci, SCI_STYLESETBACK, SCE_HJA_DEFAULT, 0xffffff);

	SSM(sci, SCI_STYLESETFORE, SCE_HJA_COMMENT, 0x606060);
	SSM(sci, SCI_STYLESETBACK, SCE_HJA_COMMENT, 0xffffff);

	SSM(sci, SCI_STYLESETFORE, SCE_HJA_COMMENTLINE, 0x808080);
	SSM(sci, SCI_STYLESETBACK, SCE_HJA_COMMENTLINE, 0xffffff);

	SSM(sci, SCI_STYLESETFORE, SCE_HJA_COMMENTDOC, 0x808080);
	SSM(sci, SCI_STYLESETBACK, SCE_HJA_COMMENTDOC, 0xffffff);

	SSM(sci, SCI_STYLESETFORE, SCE_HJA_NUMBER, 0x408000);
	SSM(sci, SCI_STYLESETBACK, SCE_HJA_NUMBER, 0xffffff);

	SSM(sci, SCI_STYLESETFORE, SCE_HJA_WORD, 0x000000);
	SSM(sci, SCI_STYLESETBACK, SCE_HJA_WORD, 0xffffff);

	SSM(sci, SCI_STYLESETFORE, SCE_HJA_DOUBLESTRING, 0x800080);
	SSM(sci, SCI_STYLESETBACK, SCE_HJA_DOUBLESTRING, 0xffffff);

	SSM(sci, SCI_STYLESETFORE, SCE_HJA_SINGLESTRING, 0x800080);
	SSM(sci, SCI_STYLESETBACK, SCE_HJA_SINGLESTRING, 0xffffff);

	SSM(sci, SCI_STYLESETFORE, SCE_HJA_SYMBOLS, 0x005000);
	SSM(sci, SCI_STYLESETBACK, SCE_HJA_SYMBOLS, 0xffffff);

	SSM(sci, SCI_STYLESETFORE, SCE_HJA_KEYWORD, 0x005000);
	SSM(sci, SCI_STYLESETBACK, SCE_HJA_KEYWORD, 0xffffff);

	// Show the whole section of Javascript with silver background
	for (jstyle=SCE_HJ_DEFAULT; jstyle<=SCE_HJ_SYMBOLS; jstyle++) {
		SSM(sci, SCI_STYLESETBACK, jstyle, 0xf5f5f5);
		SSM(sci, SCI_STYLESETEOLFILLED, jstyle, 1);
	}
	SSM(sci, SCI_STYLESETBACK, SCE_HJ_STRINGEOL, 0xDFDF7F);
	SSM(sci, SCI_STYLESETEOLFILLED, SCE_HJ_STRINGEOL, 1);

	// Show the whole section of Javascript with silver background
	for (jastyle=SCE_HJA_DEFAULT; jastyle<=SCE_HJA_SYMBOLS; jastyle++) {
		SSM(sci, SCI_STYLESETBACK, jastyle, 0xf5f5f5);
		SSM(sci, SCI_STYLESETEOLFILLED, jastyle, 1);
	}
	SSM(sci, SCI_STYLESETBACK, SCE_HJA_STRINGEOL, 0x00AF5F);
	SSM(sci, SCI_STYLESETEOLFILLED, SCE_HJA_STRINGEOL, 1);
//~ #define SCE_HJA_REGEX 67
	SSM(sci, SCI_STYLESETFORE, SCE_HP_START, 0x808000);
	SSM(sci, SCI_STYLESETBACK, SCE_HP_START, 0xf0f0f0);
	SSM(sci, SCI_STYLESETEOLFILLED, SCE_HP_START, 1);

	SSM(sci, SCI_STYLESETFORE, SCE_HP_DEFAULT, 0x000000);
	SSM(sci, SCI_STYLESETBACK, SCE_HP_DEFAULT, 0xffffff);

	SSM(sci, SCI_STYLESETFORE, SCE_HP_COMMENTLINE, 0x808080);
	SSM(sci, SCI_STYLESETBACK, SCE_HP_COMMENTLINE, 0xffffff);

	SSM(sci, SCI_STYLESETFORE, SCE_HP_NUMBER, 0x008080);
	SSM(sci, SCI_STYLESETBACK, SCE_HP_NUMBER, 0xffffff);

	SSM(sci, SCI_STYLESETFORE, SCE_HP_WORD, 0x990000);
	SSM(sci, SCI_STYLESETBACK, SCE_HP_WORD, 0xffffff);

	SSM(sci, SCI_STYLESETFORE, SCE_HP_STRING, 0x008000);
	SSM(sci, SCI_STYLESETBACK, SCE_HP_STRING, 0xffffff);

	SSM(sci, SCI_STYLESETFORE, SCE_HP_CHARACTER, 0x006060);
	SSM(sci, SCI_STYLESETBACK, SCE_HP_CHARACTER, 0xffffff);

	SSM(sci, SCI_STYLESETFORE, SCE_HP_TRIPLE, 0x002060);
	SSM(sci, SCI_STYLESETBACK, SCE_HP_TRIPLE, 0xffffff);

	SSM(sci, SCI_STYLESETFORE, SCE_HP_TRIPLEDOUBLE, 0x002060);
	SSM(sci, SCI_STYLESETBACK, SCE_HP_TRIPLEDOUBLE, 0xffffff);

	SSM(sci, SCI_STYLESETFORE, SCE_HP_CLASSNAME, 0x202010);
	SSM(sci, SCI_STYLESETBACK, SCE_HP_CLASSNAME, 0xffffff);

	SSM(sci, SCI_STYLESETFORE, SCE_HP_CLASSNAME, 0x102020);
	SSM(sci, SCI_STYLESETBACK, SCE_HP_CLASSNAME, 0xffffff);

	SSM(sci, SCI_STYLESETFORE, SCE_HP_OPERATOR, 0x602020);
	SSM(sci, SCI_STYLESETBACK, SCE_HP_OPERATOR, 0xffffff);

	SSM(sci, SCI_STYLESETFORE, SCE_HP_IDENTIFIER, 0x001060);
	SSM(sci, SCI_STYLESETBACK, SCE_HP_IDENTIFIER, 0xffffff);

	SSM(sci, SCI_STYLESETFORE, SCE_HPA_START, 0x808000);
	SSM(sci, SCI_STYLESETBACK, SCE_HPA_START, 0xffffff);

	SSM(sci, SCI_STYLESETFORE, SCE_HPA_DEFAULT, 0x000000);
	SSM(sci, SCI_STYLESETBACK, SCE_HPA_DEFAULT, 0xffffff);

	SSM(sci, SCI_STYLESETFORE, SCE_HPA_COMMENTLINE, 0x808080);
	SSM(sci, SCI_STYLESETBACK, SCE_HPA_COMMENTLINE, 0xffffff);

	SSM(sci, SCI_STYLESETFORE, SCE_HPA_NUMBER, 0x408000);
	SSM(sci, SCI_STYLESETBACK, SCE_HPA_NUMBER, 0xffffff);

	SSM(sci, SCI_STYLESETFORE, SCE_HPA_STRING, 0x008080);
	SSM(sci, SCI_STYLESETBACK, SCE_HPA_STRING, 0xffffff);

	SSM(sci, SCI_STYLESETFORE, SCE_HPA_CHARACTER, 0x505080);
	SSM(sci, SCI_STYLESETBACK, SCE_HPA_CHARACTER, 0xffffff);

	SSM(sci, SCI_STYLESETFORE, SCE_HPA_WORD, 0x990000);
	SSM(sci, SCI_STYLESETBACK, SCE_HPA_WORD, 0xffffff);

	SSM(sci, SCI_STYLESETFORE, SCE_HPA_TRIPLE, 0x002060);
	SSM(sci, SCI_STYLESETBACK, SCE_HPA_TRIPLE, 0xffffff);

	SSM(sci, SCI_STYLESETFORE, SCE_HPA_TRIPLEDOUBLE, 0x002060);
	SSM(sci, SCI_STYLESETBACK, SCE_HPA_TRIPLEDOUBLE, 0xffffff);

	SSM(sci, SCI_STYLESETFORE, SCE_HPA_CLASSNAME, 0x202010);
	SSM(sci, SCI_STYLESETBACK, SCE_HPA_CLASSNAME, 0xffffff);

	SSM(sci, SCI_STYLESETFORE, SCE_HPA_DEFNAME, 0x102020);
	SSM(sci, SCI_STYLESETBACK, SCE_HPA_DEFNAME, 0xffffff);

	SSM(sci, SCI_STYLESETFORE, SCE_HPA_OPERATOR, 0x601010);
	SSM(sci, SCI_STYLESETBACK, SCE_HPA_OPERATOR, 0xffffff);

	SSM(sci, SCI_STYLESETFORE, SCE_HPA_IDENTIFIER, 0x105010);
	SSM(sci, SCI_STYLESETBACK, SCE_HPA_IDENTIFIER, 0xffffff);

	SSM(sci, SCI_STYLESETFORE, SCE_HPHP_DEFAULT, 0x000000);
	SSM(sci, SCI_STYLESETBACK, SCE_HPHP_DEFAULT, 0xffffff);

	SSM(sci, SCI_STYLESETFORE, SCE_HP_COMMENTLINE, 0x808080);
	SSM(sci, SCI_STYLESETBACK, SCE_HP_COMMENTLINE, 0xffffff);

	SSM(sci, SCI_STYLESETFORE, SCE_HP_NUMBER, 0x408000);
	SSM(sci, SCI_STYLESETBACK, SCE_HP_NUMBER, 0xffffff);

	SSM(sci, SCI_STYLESETFORE, SCE_HPHP_SIMPLESTRING, 0x008000);
	SSM(sci, SCI_STYLESETBACK, SCE_HPHP_SIMPLESTRING, 0xffffff);

	SSM(sci, SCI_STYLESETFORE, SCE_HPHP_HSTRING, 0x008000);
	SSM(sci, SCI_STYLESETBACK, SCE_HPHP_HSTRING, 0xffffff);

	SSM(sci, SCI_STYLESETFORE, SCE_HPHP_NUMBER, 0x006060);
	SSM(sci, SCI_STYLESETBACK, SCE_HPHP_NUMBER, 0xffffff);

	SSM(sci, SCI_STYLESETFORE, SCE_HPHP_WORD, 0x990000);
	SSM(sci, SCI_STYLESETBACK, SCE_HPHP_WORD, 0xffffff);

	SSM(sci, SCI_STYLESETFORE, SCE_HPHP_VARIABLE, 0x800080);
	SSM(sci, SCI_STYLESETBACK, SCE_HPHP_VARIABLE, 0xffffff);

	SSM(sci, SCI_STYLESETFORE, SCE_HPHP_COMMENT, 0x808080);
	SSM(sci, SCI_STYLESETBACK, SCE_HPHP_COMMENT, 0xffffff);

	SSM(sci, SCI_STYLESETFORE, SCE_HPHP_COMMENTLINE, 0x808080);
	SSM(sci, SCI_STYLESETBACK, SCE_HPHP_COMMENTLINE, 0xffffff);

	SSM(sci, SCI_STYLESETFORE, SCE_HPHP_OPERATOR, 0x602010);
	SSM(sci, SCI_STYLESETBACK, SCE_HPHP_OPERATOR, 0xffffff);

	SSM(sci, SCI_STYLESETFORE, SCE_HPHP_HSTRING_VARIABLE, 0x601010);
	SSM(sci, SCI_STYLESETBACK, SCE_HPHP_HSTRING_VARIABLE, 0xffffff);

	SSM(sci, SCI_STYLESETFORE, SCE_HPHP_COMPLEX_VARIABLE, 0x105010);
	SSM(sci, SCI_STYLESETBACK, SCE_HPHP_COMPLEX_VARIABLE, 0xffffff);

	// Show the whole section of php with silver bg
	for (hpstyle=SCE_HP_START; hpstyle<=SCE_HPHP_OPERATOR; hpstyle++) {
		SSM(sci, SCI_STYLESETBACK, hpstyle, 0xf5f5f5);
		SSM(sci, SCI_STYLESETEOLFILLED, hpstyle, 1);
	}
}

void
create_and_register_hypertext_file_type (CssedWindow* window)
{
	CssedFileType* hypertext;
	GtkWidget* hypertext_menu;

	hypertext_menu = gtk_menu_item_new_with_label("HTML/PHP");

	hypertext = cssed_file_type_new();
	hypertext->char_added  = hypertext_filetype_char_added;

	hypertext->modify_attemp_read_only = NULL;
	hypertext->double_click = NULL;
	hypertext->user_list_selection = NULL;
	hypertext->dwell_start = NULL;
	hypertext->dwell_end = NULL;
	hypertext->hot_spot_click = NULL;
	hypertext->hot_spot_doubleclick = NULL;
	hypertext->call_tip_click = NULL;

	hypertext->save_point_reached = NULL;
	hypertext->save_point_left = NULL;
	hypertext->uri_dropped = NULL;
	hypertext->margin_click = NULL;
	hypertext->modified = NULL;
	hypertext->key_pressed = NULL;

	// configuraton and styling
	hypertext->apply_style = filetype_hypertext_set_style;

	// THOSE ARE NOT USED TEMPORALLY
	hypertext->get_config_page_widget = NULL;
	hypertext->save_configuration_from_widget = NULL;	
	hypertext->pop_menu = NULL;
	hypertext->clean_popmenu = NULL;
	hypertext->menu_item = hypertext_menu;
	hypertext->clean_menu = NULL;
	hypertext->load_ui = NULL;

	// file related issues, those are mostly to avoid to open or save files
	hypertext->open_file = NULL;
	hypertext->save_doc = NULL;

	hypertext->id = CSSED_FILETYPE_HTML;
	hypertext->style_min = SCE_H_DEFAULT;//(document_set_font_size)  (document_set_font_by_name) (Read/Write const only)
	hypertext->style_max = SCE_HPHP_OPERATOR; //(document_set_font_size) (document_set_font_by_name)(Read/Write const only)
	hypertext->can_fold = TRUE;
	hypertext->label_language = g_strdup("HTML/PHP"); // a name for the menu as CSS or XML
	hypertext->lable_file = g_strdup("Hypertext (HTML/PHP)");

	cssed_file_type_add_pattern_spec (hypertext, "*.html");
	cssed_file_type_add_pattern_spec (hypertext, "*.htm");
	cssed_file_type_add_pattern_spec (hypertext, "*.php");
	cssed_file_type_add_pattern_spec (hypertext, "*.php3");
	cssed_file_type_add_pattern_spec (hypertext, "*.jsp");
	cssed_file_type_menu_set_default_callback (window, hypertext_menu, hypertext);
	cssed_window_add_filetype (window, hypertext, FALSE);
}
