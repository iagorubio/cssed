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
c_filetype_char_added(CssedDoc* doc, gchar  lastchar)
{
	if( lastchar == '\n')
		document_indent_as_last_line(doc);
}

void
filetype_c_set_style(CssedDoc* doc, GtkWidget *editor)
{
	ScintillaObject * sci;
	CssedWindow* window;
	CssedConfig* cfg;
	GtkWidget *scintilla;

	const gchar primaryKeyWords[] = "auto break case char class const continue delete do double else extern \
float for if inline int long new register return short signed static struct switch typedef unsigned void while";

	const gchar secondaryKeyWords[] = "gconstpointer gchar guchar gint guint gint8 gint16 \
gint32 gint64 gshort gushort glong gulong gfloat gdouble gboolean gpointer NULL FALSE TRUE \
GObject GtkAccelGroup GtkObject GtkWidget GtkMisc GtkLabel GtkAccelLabel GtkTipsQuery GtkArrow \
GtkImage GtkPixmap GtkContainer GtkBin GtkAlignment GtkFrame GtkAspectFrame GtkButton \
GtkToggleButton GtkCheckButton GtkRadioButton GtkOptionMenu GtkItem GtkMenuItem GtkCheckMenuItem \
GtkRadioMenuItem GtkImageMenuItem GtkSeparatorMenuItem GtkTearoffMenuItem GtkListItem GtkTreeItem \
GtkWindow GtkDialog GtkColorSelectionDialog GtkFileSelection GtkFontSelectionDialog \
GtkInputDialog GtkMessageDialog GtkPlug GtkEventBox GtkHandleBox GtkScrolledWindow GtkViewport \
GtkBox GtkButtonBox GtkHButtonBox GtkVButtonBox GtkVBox GtkColorSelection GtkFontSelection \
GtkGammaCurve GtkHBox GtkCombo GtkStatusbar GtkCList GtkCTree GtkFixed GtkPaned GtkHPaned \
GtkVPaned GtkLayout GtkList GtkMenuShell GtkMenuBar GtkMenu GtkNotebook GtkSocket GtkTable \
GtkTextView GtkToolbar GtkTree GtkTreeView GtkCalendar GtkDrawingArea GtkCurve GtkEntry \
GtkSpinButton GtkRuler GtkHRuler GtkVRuler GtkRange GtkScale GtkHScale GtkVScale GtkScrollbar \
GtkHScrollbar GtkVScrollbar GtkSeparator" "GtkHSeparator GtkVSeparator GtkInvisibleGtkOldEditable \
GtkText GtkPreview GtkProgress GtkProgressBar GtkAdjustment GtkCellRenderer \
GtkCellRendererPixbuf GtkCellRendererText GtkCellRendererToggle GtkItemFactory GtkTooltips \
GtkTreeViewColumn AtkObject GtkAccessible GtkIconFactory GtkIMContext GtkIMContextSimple \
GtkIMMulticontext GtkListStore GtkRcStyle GtkSettings GtkSizeGroup GtkStyle GtkTextBuffer \
GtkTextChildAnchor GtkTextMark GtkTextTag GtkTextTagTable GtkTreeModelSort GtkTreeSelection \
GtkTreeStore GtkWindowGroup GdkDragContext GdkScreen GdkPixbuf GdkDrawable GdkPixmap GdkImage \
GdkPixbufAnimation GdkDevice GInterface AtkImplementorIface GtkEditable GtkCellEditable \
GtkTreeModel GtkTreeDragSource GtkTreeDragDest GtkTreeSortable GdkPixbuf GdkPixbufAnimation \
GtkTreeModel GtkTreeSelection GtkTreeViewColumn \
GtkTreeView GtkTreeView GtkTreeSortable GtkTreeModelSort GtkCellRenderer GtkCellEditable \
GtkCellRendererPixbuf GtkCellRendererText GtkCellRendererToggle GtkListStore GtkTreeStore \
GtkTreeViewColumnSizing GtkTreeCellDataFunc GtkTreePath" ;

	const gchar docCommentKeyWords[] = "";

	const gchar typedefsKeyWords[] = "GtkAccelGroup GtkObject GTK_OBJECT_TYPE GTK_OBJECT_TYPE_NAME \
GtkObjectFlags GTK_OBJECT_FLAGS GTK_OBJECT_FLOATING GtkArgFlags GtkWidgetClass GtkWidgetFlags \
GTK_WIDGET_TYPE GTK_WIDGET_STATE GTK_WIDGET_SAVED_STATE GTK_WIDGET_FLAGS GTK_WIDGET_TOPLEVEL \
GTK_WIDGET_NO_WINDOW GTK_WIDGET_REALIZED GTK_WIDGET_MAPPED GTK_WIDGET_VISIBLE GTK_WIDGET_DRAWABLE \
GTK_WIDGET_SENSITIVE GTK_WIDGET_PARENT_SENSITIVE GTK_WIDGET_IS_SENSITIVE GTK_WIDGET_CAN_FOCUS \
GTK_WIDGET_HAS_FOCUS GTK_WIDGET_CAN_DEFAULT GTK_WIDGET_RECEIVES_DEFAULT GTK_WIDGET_HAS_DEFAULT \
GTK_WIDGET_HAS_GRAB GTK_WIDGET_RC_STYLE GTK_WIDGET_COMPOSITE_CHILD GTK_WIDGET_APP_PAINTABLE \
GTK_WIDGET_DOUBLE_BUFFERED GTK_WIDGET_SET_FLAGS GTK_WIDGET_UNSET_FLAGS GtkCallback \
GtkRequisition GtkAllocation GtkSelectionData GtkWidgetAuxInfo GtkWidgetShapeInfo \
GtkWidgetHelpType G_CONST_RETURN GtkImageType GTK_IS_RESIZE_CONTAINER \
GTK_CONTAINER_WARN_INVALID_CHILD_PROPERTY_ID GtkDialogFlags GtkResponseType GtkBoxChild \
GTK_BUTTONBOX_DEFAULT GTK_TREE_VIEW_COLUMN_GROW_ONLY GTK_TREE_VIEW_COLUMN_AUTOSIZE \
GTK_TREE_VIEW_COLUMN_FIXED GtkTreeViewDropPosition GtkTreeViewPrivate GtkTreeViewColumnDropFunc \
GtkTreeViewMappingFunc GtkTreeViewSearchEqualFunc GTK_STOCK_ADD GTK_STOCK_APPLY GTK_STOCK_BOLD \
GTK_STOCK_CANCEL GTK_STOCK_CDROM GTK_STOCK_CLEAR GTK_STOCK_CLOSE GTK_STOCK_COLOR_PICKER \
GTK_STOCK_CONVERT GTK_STOCK_COPY GTK_STOCK_CUT GTK_STOCK_DELETE GTK_STOCK_DIALOG_AUTHENTICATION \
GTK_STOCK_DIALOG_ERROR GTK_STOCK_DIALOG_INFO GTK_STOCK_DIALOG_QUESTION GTK_STOCK_DIALOG_WARNING \
GTK_STOCK_DND GTK_STOCK_DND_MULTIPLE GTK_STOCK_EXECUTE GTK_STOCK_FIND GTK_STOCK_FIND_AND_REPLACE \
GTK_STOCK_FLOPPY GTK_STOCK_GOTO_BOTTOM GTK_STOCK_GOTO_FIRST GTK_STOCK_GOTO_LAST \
GTK_STOCK_GOTO_TOP GTK_STOCK_GO_BACK GTK_STOCK_GO_DOWN GTK_STOCK_GO_FORWARD GTK_STOCK_GO_UP \
GTK_STOCK_HARDDISK GTK_STOCK_HELP GTK_STOCK_HOME GTK_STOCK_INDENT GTK_STOCK_INDEX \
GTK_STOCK_ITALIC GTK_STOCK_JUMP_TO GTK_STOCK_JUSTIFY_CENTER GTK_STOCK_JUSTIFY_FILL \
GTK_STOCK_JUSTIFY_LEFT GTK_STOCK_JUSTIFY_RIGHT" "GTK_STOCK_MISSING_IMAGE GTK_STOCK_NETWORK \
GTK_STOCK_NEW GTK_STOCK_NO GTK_STOCK_OK GTK_STOCK_OPEN GTK_STOCK_PASTE GTK_STOCK_PREFERENCES \
GTK_STOCK_PRINT GTK_STOCK_PRINT_PREVIEW GTK_STOCK_PROPERTIES GTK_STOCK_QUIT GTK_STOCK_REDO \
GTK_STOCK_REFRESH GTK_STOCK_REMOVE GTK_STOCK_REVERT_TO_SAVED GTK_STOCK_SAVE GTK_STOCK_SAVE_AS \
GTK_STOCK_SELECT_COLOR GTK_STOCK_SELECT_FONT GTK_STOCK_SORT_ASCENDING GTK_STOCK_SORT_DESCENDING \
GTK_STOCK_SPELL_CHECK GTK_STOCK_STOP GTK_STOCK_STRIKETHROUGH GTK_STOCK_UNDELETE \
GTK_STOCK_UNDERLINE GTK_STOCK_UNDO GTK_STOCK_UNINDENT GTK_STOCK_YES GTK_STOCK_ZOOM_100 \
GTK_STOCK_ZOOM_FIT GTK_STOCK_ZOOM_IN GTK_STOCK_ZOOM_OUT";

	window = document_get_window(doc);
	cfg = cssed_window_get_config(window);
	
	if( editor ) scintilla = editor;
	else scintilla = document_get_scintilla_widget(doc);
	
	sci = SCINTILLA(scintilla);

	SSM (sci, SCI_SETPROPERTY, (sptr_t) "fold.preprocessor", (sptr_t) "1");
	SSM (sci, SCI_SETPROPERTY, (sptr_t) "fold.at.else", (sptr_t) "1");

	SSM (sci, SCI_STYLECLEARALL, 0, 0);
	SSM (sci, SCI_SETLEXER, SCLEX_CPP, 0);

	SSM(sci, SCI_SETKEYWORDS, 0, (sptr_t) primaryKeyWords);
	SSM(sci, SCI_SETKEYWORDS, 1, (sptr_t) secondaryKeyWords);
	SSM(sci, SCI_SETKEYWORDS, 2, (sptr_t) docCommentKeyWords);
	SSM(sci, SCI_SETKEYWORDS, 4, (sptr_t) typedefsKeyWords);

	SSM(sci, SCI_STYLESETFORE, SCE_C_DEFAULT, 0x000000);
	SSM(sci, SCI_STYLESETBACK, SCE_C_DEFAULT, 0xffffff);

	SSM(sci, SCI_STYLESETFORE, SCE_C_COMMENT, 0x808080);
	SSM(sci, SCI_STYLESETBACK, SCE_C_COMMENT, 0xffffff);

	SSM(sci, SCI_STYLESETFORE, SCE_C_COMMENTLINE, 0x808080);
	SSM(sci, SCI_STYLESETBACK, SCE_C_COMMENTLINE, 0xffffff);

	SSM(sci, SCI_STYLESETFORE, SCE_C_COMMENTDOC, 0x808080);
	SSM(sci, SCI_STYLESETBACK, SCE_C_COMMENTDOC, 0xffffff);

	SSM(sci, SCI_STYLESETFORE, SCE_C_NUMBER, 0x408000);
	SSM(sci, SCI_STYLESETBACK, SCE_C_NUMBER, 0xffffff);

	SSM(sci, SCI_STYLESETFORE, SCE_C_WORD, 0x802080);
	SSM(sci, SCI_STYLESETBACK, SCE_C_WORD, 0xffffff);

	SSM(sci, SCI_STYLESETFORE, SCE_C_WORD2, 0x800020);
	SSM(sci, SCI_STYLESETBACK, SCE_C_WORD2, 0xffffff);

	SSM(sci, SCI_STYLESETFORE, SCE_C_STRING, 0x404000);
	SSM(sci, SCI_STYLESETBACK, SCE_C_STRING, 0xffffff);

	SSM(sci, SCI_STYLESETFORE, SCE_C_CHARACTER, 0x004040);
	SSM(sci, SCI_STYLESETBACK, SCE_C_CHARACTER, 0xffffff);

	SSM(sci, SCI_STYLESETFORE, SCE_C_UUID, 0x804040);
	SSM(sci, SCI_STYLESETBACK, SCE_C_UUID, 0xffffff);

	SSM(sci, SCI_STYLESETFORE, SCE_C_PREPROCESSOR, 0x804060);
	SSM(sci, SCI_STYLESETBACK, SCE_C_PREPROCESSOR, 0xffffff);

	SSM(sci, SCI_STYLESETFORE, SCE_C_OPERATOR, 0x101030);
	SSM(sci, SCI_STYLESETBACK, SCE_C_OPERATOR, 0xffffff);

	SSM(sci, SCI_STYLESETFORE, SCE_C_IDENTIFIER, 0x100000);
	SSM(sci, SCI_STYLESETBACK, SCE_C_IDENTIFIER, 0xffffff);

	SSM(sci, SCI_STYLESETFORE, SCE_C_STRINGEOL, 0x104010);
	SSM(sci, SCI_STYLESETBACK, SCE_C_STRINGEOL, 0xffffff);

	SSM(sci, SCI_STYLESETFORE, SCE_C_VERBATIM, 0x906040);
	SSM(sci, SCI_STYLESETBACK, SCE_C_VERBATIM, 0xffffff);

	SSM(sci, SCI_STYLESETFORE, SCE_C_REGEX, 0x905010);
	SSM(sci, SCI_STYLESETBACK, SCE_C_REGEX, 0xffffff);

	SSM(sci, SCI_STYLESETFORE, SCE_C_COMMENTLINEDOC, 0x005020);
	SSM(sci, SCI_STYLESETBACK, SCE_C_COMMENTLINEDOC, 0xffffff);

	SSM(sci, SCI_STYLESETFORE, SCE_C_COMMENTDOCKEYWORD, 0x400010);
	SSM(sci, SCI_STYLESETBACK, SCE_C_COMMENTDOCKEYWORD, 0xffffff);

	SSM(sci, SCI_STYLESETFORE, SCE_C_COMMENTDOCKEYWORDERROR, 0x001020);
	SSM(sci, SCI_STYLESETBACK, SCE_C_COMMENTDOCKEYWORDERROR, 0xffffff);

	SSM(sci, SCI_STYLESETFORE, SCE_C_GLOBALCLASS, 0x109040);
	SSM(sci, SCI_STYLESETBACK, SCE_C_GLOBALCLASS, 0xffffff);
}
/* ********************************** */

void
create_and_register_c_file_type (CssedWindow* window)
{
	CssedFileType* cfiletype;
	GtkWidget* cfiletype_menu;

	cfiletype_menu = gtk_menu_item_new_with_label("C/CPP");

	cfiletype = cssed_file_type_new();
	cfiletype->char_added  = c_filetype_char_added;

	cfiletype->modify_attemp_read_only = NULL;
	cfiletype->double_click = NULL;
	cfiletype->user_list_selection = NULL;
	cfiletype->dwell_start = NULL;
	cfiletype->dwell_end = NULL;
	cfiletype->hot_spot_click = NULL;
	cfiletype->hot_spot_doubleclick = NULL;
	cfiletype->call_tip_click = NULL;

	cfiletype->save_point_reached = NULL;
	cfiletype->save_point_left = NULL;
	cfiletype->uri_dropped = NULL;
	cfiletype->margin_click = NULL;
	cfiletype->modified = NULL;
	cfiletype->key_pressed = NULL;

	// configuraton and styling
	cfiletype->apply_style = filetype_c_set_style;

	// THOSE ARE NOT USED TEMPORALY
	cfiletype->get_config_page_widget = NULL;
	cfiletype->save_configuration_from_widget = NULL;	
	cfiletype->pop_menu = NULL;
	cfiletype->clean_popmenu = NULL;
	cfiletype->menu_item = cfiletype_menu;
	cfiletype->clean_menu = NULL;
	cfiletype->load_ui = NULL;

	// file related issues, those are mostly to avoid to open or save files
	cfiletype->open_file = NULL;
	cfiletype->save_doc = NULL;

	cfiletype->id = CSSED_FILETYPE_C;
	cfiletype->style_min = SCE_C_DEFAULT;//(document_set_font_size)  (document_set_font_by_name) (Read/Write const only)
	cfiletype->style_max = SCE_C_GLOBALCLASS; //(document_set_font_size) (document_set_font_by_name)(Read/Write const only)
	cfiletype->can_fold = TRUE;
	cfiletype->label_language = g_strdup("C"); // a name for the menu as CSS or XML
	cfiletype->lable_file = g_strdup("GTK C");

	cssed_file_type_add_pattern_spec(cfiletype, "*.c");
	cssed_file_type_add_pattern_spec(cfiletype, "*.h");
	cssed_file_type_add_pattern_spec(cfiletype, "*.cpp");
	cssed_file_type_add_pattern_spec(cfiletype, "*.cxx");
	cssed_file_type_add_pattern_spec(cfiletype, "*.hxx");
	cssed_file_type_menu_set_default_callback (window, cfiletype_menu, cfiletype);
	cssed_window_add_filetype (window, cfiletype, FALSE);
}
