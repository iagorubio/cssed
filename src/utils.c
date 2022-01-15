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

#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <gtk/gtk.h>
#include <glib.h>

#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdarg.h>
#include <fcntl.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>

#ifdef WIN32
# include <direct.h>
# include  <io.h>
#else
# include <unistd.h>
# include <libgen.h>
#endif

#include "cssedwindow.h"
#include "interface.h"
#include "support.h"
#include "document.h"
#include "plugin.h"
#include "debug.h"
#include "utils.h"

void
cssed_error_message (gchar * title, gchar * message, ... )
{
	va_list args;
	GtkWidget* error_box;
	GtkWidget* label;
	GtkWidget* label_detail;
	gchar* error_message;

	error_box = create_error_dialog ();
	gtk_window_set_title ((GtkWindow *) error_box, "");

	label = lookup_widget (error_box, "label_msg");
	label_detail = lookup_widget (error_box, "label_detail");

	error_message = g_strdup_printf("<span weight='bold' size='larger'>%s</span>", title);
	gtk_label_set_text (GTK_LABEL(label), error_message);
	gtk_label_set_use_markup(GTK_LABEL(label), TRUE);
	g_free(error_message);

    va_start(args, message);
    error_message = g_strdup_vprintf(message, args);
    va_end(args);

	gtk_label_set_text (GTK_LABEL(label_detail), error_message);
	gtk_widget_show_all(error_box);
	g_free(error_message);
}

/* modal dialogs call with gtk_dialog_run and get the response */
GtkWidget*
create_ok_cancel_dialog (gchar* title, gchar* format, ...)
{
	GtkWidget *ok_cancel_dialog;
	GtkWidget *dialog_vbox;
	GtkWidget *hbox;
	GtkWidget *image;
	GtkWidget *vbox;
	GtkWidget *label_header;
	GtkWidget *label_message;
	GtkWidget *dialog_action_area;
	GtkWidget *cancelbutton;
	GtkWidget *okbutton;
	gchar* message;
	gchar* markup_title;
 	va_list args;

	markup_title = g_strconcat("<span size='larger'>",title,"</span>",NULL);
	
    va_start(args, format);
    message = g_strdup_vprintf(format, args);
    va_end(args);

	ok_cancel_dialog = gtk_dialog_new ();
	gtk_window_set_position (GTK_WINDOW (ok_cancel_dialog), GTK_WIN_POS_CENTER);
	
	dialog_vbox = GTK_DIALOG (ok_cancel_dialog)->vbox;
	gtk_widget_show (dialog_vbox);
	
	hbox = gtk_hbox_new (FALSE, 0);
	gtk_widget_show (hbox);
	gtk_box_pack_start (GTK_BOX (dialog_vbox), hbox, TRUE, TRUE, 0);
	
	image = gtk_image_new_from_stock ("gtk-dialog-question", GTK_ICON_SIZE_DIALOG);
	gtk_widget_show (image);
	gtk_box_pack_start (GTK_BOX (hbox), image, FALSE, FALSE, 20);
	
	vbox = gtk_vbox_new (FALSE, 0);
	gtk_widget_show (vbox);
	gtk_box_pack_start (GTK_BOX (hbox), vbox, TRUE, TRUE, 0);
	
	
	label_header = gtk_label_new (markup_title);
	gtk_widget_show (label_header);
	gtk_box_pack_start (GTK_BOX (vbox), label_header, FALSE, TRUE, 20);
	gtk_label_set_use_markup (GTK_LABEL (label_header), TRUE);
	gtk_label_set_justify (GTK_LABEL (label_header), GTK_JUSTIFY_LEFT);
	
	label_message = gtk_label_new (message);
	gtk_widget_show (label_message);
	gtk_box_pack_start (GTK_BOX (vbox), label_message, TRUE, TRUE, 0);
	gtk_label_set_use_markup (GTK_LABEL (label_message), TRUE);
	gtk_label_set_justify (GTK_LABEL (label_message), GTK_JUSTIFY_LEFT);
	gtk_label_set_line_wrap (GTK_LABEL (label_message), TRUE);
	
	dialog_action_area = GTK_DIALOG (ok_cancel_dialog)->action_area;
	gtk_widget_show (dialog_action_area);
	gtk_button_box_set_layout (GTK_BUTTON_BOX (dialog_action_area), GTK_BUTTONBOX_END);
	
	okbutton = gtk_button_new_from_stock ("gtk-ok");
	gtk_widget_show (okbutton);
	gtk_dialog_add_action_widget (GTK_DIALOG (ok_cancel_dialog), okbutton, GTK_RESPONSE_OK);
	GTK_WIDGET_SET_FLAGS (okbutton, GTK_CAN_DEFAULT);
	
	cancelbutton = gtk_button_new_from_stock ("gtk-cancel");
	gtk_widget_show (cancelbutton);
	gtk_dialog_add_action_widget (GTK_DIALOG (ok_cancel_dialog), cancelbutton, GTK_RESPONSE_CANCEL);
	GTK_WIDGET_SET_FLAGS (cancelbutton, GTK_CAN_DEFAULT);
	
	gtk_widget_grab_default (okbutton);
	g_free(markup_title);
	g_free(message);

	return ok_cancel_dialog;
}

GtkWidget*
create_yes_no_cancel_dialog (gchar* title, gchar* format, ...)
{
	GtkWidget *yes_no_cancel_dialog;
	GtkWidget *dialog_vbox;
	GtkWidget *hbox;
	GtkWidget *image;
	GtkWidget *vbox;
	GtkWidget *label_header;
	GtkWidget *label_message;
	GtkWidget *dialog_action_area;
	GtkWidget *cancelbutton;
	GtkWidget *nobutton;
	GtkWidget* yesbutton;
	gchar* markup_title;
	gchar* message;
 	va_list args;

	markup_title = g_strconcat("<span size='larger'>",title,"</span>",NULL);
	
    va_start(args, format);
    message = g_strdup_vprintf(format, args);
    va_end(args);
	
	yes_no_cancel_dialog = gtk_dialog_new ();
	gtk_window_set_position (GTK_WINDOW (yes_no_cancel_dialog), GTK_WIN_POS_CENTER);
	
	dialog_vbox = GTK_DIALOG (yes_no_cancel_dialog)->vbox;
	gtk_widget_show (dialog_vbox);
	
	hbox = gtk_hbox_new (FALSE, 0);
	gtk_widget_show (hbox);
	gtk_box_pack_start (GTK_BOX (dialog_vbox), hbox, TRUE, TRUE, 0);
	
	image = gtk_image_new_from_stock ("gtk-dialog-question", GTK_ICON_SIZE_DIALOG);
	gtk_widget_show (image);
	gtk_box_pack_start (GTK_BOX (hbox), image, FALSE, FALSE, 20);
	
	vbox = gtk_vbox_new (FALSE, 0);
	gtk_widget_show (vbox);
	gtk_box_pack_start (GTK_BOX (hbox), vbox, TRUE, TRUE, 0);
	
	
	label_header = gtk_label_new (markup_title);
	gtk_widget_show (label_header);
	gtk_box_pack_start (GTK_BOX (vbox), label_header, FALSE, TRUE, 20);
	gtk_label_set_use_markup (GTK_LABEL (label_header), TRUE);
	gtk_label_set_justify (GTK_LABEL (label_header), GTK_JUSTIFY_LEFT);
	
	label_message = gtk_label_new (message);
	gtk_widget_show (label_message);
	gtk_box_pack_start (GTK_BOX (vbox), label_message, TRUE, TRUE, 0);
	gtk_label_set_use_markup (GTK_LABEL (label_message), TRUE);
	gtk_label_set_justify (GTK_LABEL (label_message), GTK_JUSTIFY_LEFT);
	gtk_label_set_line_wrap (GTK_LABEL (label_message), TRUE);
	
	dialog_action_area = GTK_DIALOG (yes_no_cancel_dialog)->action_area;
	gtk_widget_show (dialog_action_area);
	gtk_button_box_set_layout (GTK_BUTTON_BOX (dialog_action_area), GTK_BUTTONBOX_END);
	
	yesbutton = gtk_button_new_from_stock ("gtk-yes");
	gtk_widget_show (yesbutton);
	gtk_dialog_add_action_widget (GTK_DIALOG (yes_no_cancel_dialog), yesbutton, GTK_RESPONSE_YES);
	GTK_WIDGET_SET_FLAGS (yesbutton, GTK_CAN_DEFAULT);
	
	nobutton = gtk_button_new_from_stock ("gtk-no");
	gtk_widget_show (nobutton);
	gtk_dialog_add_action_widget (GTK_DIALOG (yes_no_cancel_dialog), nobutton, GTK_RESPONSE_NO);
	GTK_WIDGET_SET_FLAGS (nobutton, GTK_CAN_DEFAULT);
	
	cancelbutton = gtk_button_new_from_stock ("gtk-cancel");
	gtk_widget_show (cancelbutton);
	gtk_dialog_add_action_widget (GTK_DIALOG (yes_no_cancel_dialog), cancelbutton, GTK_RESPONSE_CANCEL);
	GTK_WIDGET_SET_FLAGS (cancelbutton, GTK_CAN_DEFAULT);
	
	gtk_widget_grab_default (yesbutton);

	g_free(markup_title);
	g_free(message);

	return yes_no_cancel_dialog;
}

GtkWidget*
create_yes_no_save_dialog (gchar* title, gchar* format, ...)
{
	GtkWidget *yes_no_save_dialog;
	GtkWidget *dialog_vbox;
	GtkWidget *hbox;
	GtkWidget *image;
	GtkWidget *vbox;
	GtkWidget *label_header;
	GtkWidget *label_message;
	GtkWidget *dialog_action_area;
	GtkWidget *savebutton;
	GtkWidget *nobutton;
	GtkWidget* yesbutton;
	gchar* markup_title;
	gchar* message;
 	va_list args;

	markup_title = g_strconcat("<span size='larger'>",title,"</span>",NULL);
	
    va_start(args, format);
    message = g_strdup_vprintf(format, args);
    va_end(args);
	
	yes_no_save_dialog = gtk_dialog_new ();
	gtk_window_set_position (GTK_WINDOW (yes_no_save_dialog), GTK_WIN_POS_CENTER);
	
	dialog_vbox = GTK_DIALOG (yes_no_save_dialog)->vbox;
	gtk_widget_show (dialog_vbox);
	
	hbox = gtk_hbox_new (FALSE, 0);
	gtk_widget_show (hbox);
	gtk_box_pack_start (GTK_BOX (dialog_vbox), hbox, TRUE, TRUE, 0);
	
	image = gtk_image_new_from_stock ("gtk-dialog-question", GTK_ICON_SIZE_DIALOG);
	gtk_widget_show (image);
	gtk_box_pack_start (GTK_BOX (hbox), image, FALSE, FALSE, 20);
	
	vbox = gtk_vbox_new (FALSE, 0);
	gtk_widget_show (vbox);
	gtk_box_pack_start (GTK_BOX (hbox), vbox, TRUE, TRUE, 0);	
	
	label_header = gtk_label_new (markup_title);
	gtk_widget_show (label_header);
	gtk_box_pack_start (GTK_BOX (vbox), label_header, FALSE, TRUE, 20);
	gtk_label_set_use_markup (GTK_LABEL (label_header), TRUE);
	gtk_label_set_justify (GTK_LABEL (label_header), GTK_JUSTIFY_LEFT);
	
	label_message = gtk_label_new (message);
	gtk_widget_show (label_message);
	gtk_box_pack_start (GTK_BOX (vbox), label_message, TRUE, TRUE, 0);
	gtk_label_set_use_markup (GTK_LABEL (label_message), TRUE);
	gtk_label_set_justify (GTK_LABEL (label_message), GTK_JUSTIFY_LEFT);
	gtk_label_set_line_wrap (GTK_LABEL (label_message), TRUE);
	
	dialog_action_area = GTK_DIALOG (yes_no_save_dialog)->action_area;
	gtk_widget_show (dialog_action_area);
	gtk_button_box_set_layout (GTK_BUTTON_BOX (dialog_action_area), GTK_BUTTONBOX_END);
	
	yesbutton = gtk_button_new_from_stock ("gtk-yes");
	gtk_widget_show (yesbutton);
	gtk_dialog_add_action_widget (GTK_DIALOG (yes_no_save_dialog), yesbutton, GTK_RESPONSE_YES);
	GTK_WIDGET_SET_FLAGS (yesbutton, GTK_CAN_DEFAULT);
	
	nobutton = gtk_button_new_from_stock ("gtk-no");
	gtk_widget_show (nobutton);
	gtk_dialog_add_action_widget (GTK_DIALOG (yes_no_save_dialog), nobutton, GTK_RESPONSE_NO);
	GTK_WIDGET_SET_FLAGS (nobutton, GTK_CAN_DEFAULT);
	
	savebutton = gtk_button_new_from_stock ("gtk-save");
	gtk_widget_show (savebutton);
	gtk_dialog_add_action_widget (GTK_DIALOG (yes_no_save_dialog), savebutton, GTK_RESPONSE_OK);
	GTK_WIDGET_SET_FLAGS (savebutton, GTK_CAN_DEFAULT);
	
	gtk_widget_grab_default (savebutton);

	g_free(markup_title);
	g_free(message);

	return yes_no_save_dialog;
}

GtkWidget*
create_unsaved_files_dialog (void)
{
	GtkWidget *unsaved_files_dialog;
	GtkWidget *dialog_vbox;
	GtkWidget *hbox;
	GtkWidget *image;
	GtkWidget *vbox;
	GtkWidget *label_msg;
	GtkWidget *label_detail;
	GtkWidget *dialog_action_area;
	GtkWidget *button_save_all;
	GtkWidget *button_choose_files;
	GtkWidget *button_dont_save;
	GtkWidget *button_cancel;
	
	unsaved_files_dialog = gtk_dialog_new ();
	
	dialog_vbox = GTK_DIALOG (unsaved_files_dialog)->vbox;
	gtk_widget_show (dialog_vbox);
	
	hbox = gtk_hbox_new (FALSE, 0);
	gtk_widget_show (hbox);
	gtk_box_pack_start (GTK_BOX (dialog_vbox), hbox, TRUE, TRUE, 0);
	
	image = gtk_image_new_from_stock ("gtk-dialog-warning", GTK_ICON_SIZE_DIALOG);
	gtk_widget_show (image);
	gtk_box_pack_start (GTK_BOX (hbox), image, FALSE, FALSE, 20);
	
	vbox = gtk_vbox_new (FALSE, 0);
	gtk_widget_show (vbox);
	gtk_box_pack_start (GTK_BOX (hbox), vbox, TRUE, TRUE, 0);
	
	label_msg = gtk_label_new (_("<span size='larger'><b>There are documents not saved.  Changes will be lost.</b></span>"));
	gtk_widget_show (label_msg);
	gtk_box_pack_start (GTK_BOX (vbox), label_msg, FALSE, FALSE, 15);
	gtk_label_set_use_markup (GTK_LABEL (label_msg), TRUE);
	gtk_label_set_justify (GTK_LABEL (label_msg), GTK_JUSTIFY_LEFT);
	
	label_detail = gtk_label_new (_("<span>Please choose an action to be taken:\n\n\t<b>\"Save all\"</b> - will save all unsaved files.\n\t<b>\"Choose files\"</b> - will ask you to save each file.\n\t<b>\"Don't save\"</b> - will close cssed without saving any file.\n\t<b>\"Cancel\"</b> - will close this dialog but won't close cssed.</span>\n"));
	gtk_widget_show (label_detail);
	gtk_box_pack_start (GTK_BOX (vbox), label_detail, TRUE, TRUE, 0);
	gtk_label_set_use_markup (GTK_LABEL (label_detail), TRUE);
	gtk_label_set_justify (GTK_LABEL (label_detail), GTK_JUSTIFY_LEFT);
	gtk_label_set_line_wrap (GTK_LABEL (label_detail), TRUE);
	
	dialog_action_area = GTK_DIALOG (unsaved_files_dialog)->action_area;
	gtk_widget_show (dialog_action_area);
	gtk_button_box_set_layout (GTK_BUTTON_BOX (dialog_action_area), GTK_BUTTONBOX_END);
	
	button_save_all = gtk_button_new_with_mnemonic (_("_Save all"));
	gtk_widget_show (button_save_all);
	gtk_dialog_add_action_widget (GTK_DIALOG (unsaved_files_dialog), button_save_all, GTK_RESPONSE_OK);
	GTK_WIDGET_SET_FLAGS (button_save_all, GTK_CAN_DEFAULT);
	
	button_choose_files = gtk_button_new_with_mnemonic (_("_Choose files"));
	gtk_widget_show (button_choose_files);
	gtk_dialog_add_action_widget (GTK_DIALOG (unsaved_files_dialog), button_choose_files, GTK_RESPONSE_ACCEPT);
	GTK_WIDGET_SET_FLAGS (button_choose_files, GTK_CAN_DEFAULT);
	
	button_dont_save = gtk_button_new_with_mnemonic (_("_Don't save"));
	gtk_widget_show (button_dont_save);
	gtk_dialog_add_action_widget (GTK_DIALOG (unsaved_files_dialog), button_dont_save, GTK_RESPONSE_NO);
	GTK_WIDGET_SET_FLAGS (button_choose_files, GTK_CAN_DEFAULT);
	
	
	button_cancel = gtk_button_new_from_stock ("gtk-cancel");
	gtk_widget_show (button_cancel);
	gtk_dialog_add_action_widget (GTK_DIALOG (unsaved_files_dialog), button_cancel, GTK_RESPONSE_CANCEL);
	GTK_WIDGET_SET_FLAGS (button_cancel, GTK_CAN_DEFAULT);

	gtk_widget_grab_default (button_save_all);
	return unsaved_files_dialog;
}

/* Converts a GdkColor to a triplet #RRGGBB string
   the returned string must be freed by caller  */
gchar*
gdk_color_to_rgb_triplet_string(GdkColor* color)
{
	gchar* rgb;
    rgb = g_strdup_printf("#%02x%02x%02x",
                         color->red >> 8,
                         color->green >> 8,
                         color->blue >> 8);
	return rgb;
}

/* converts a GdkColor to scintilla rgb integer color */
gint
gdk_color_to_scintilla_integer_color( GdkColor* color )
{

	guint16 red;
	guint16 green;
	guint16 blue;
	gint scicolor;

	red =   (color->red/65535)*255;
	green = (color->green/65535)*255;
	blue =  (color->blue/65535)*255;

	scicolor = red | (green << 8) | (blue << 16);
	DBGMSG (__FILE__,__LINE__,"\tscicolor %d %x", scicolor, scicolor);

	return scicolor;
}

/*  */
GtkWidget*
create_custom_button_from_stock(gchar* label_text,gchar *stock_id)
{
  GtkWidget* button;
  GtkWidget* image;
  GtkWidget* box;
  GtkWidget* label;

  button = gtk_button_new();
  box = gtk_hbox_new(FALSE,0);

  image = gtk_image_new_from_stock(stock_id,GTK_ICON_SIZE_BUTTON);
  label = gtk_label_new(label_text);

  gtk_box_pack_start((GtkBox*)box, image, TRUE, TRUE, 0);
  gtk_box_pack_end  ((GtkBox*)box, label, TRUE, TRUE, 0);
  gtk_widget_show_all(box);
  gtk_container_add(GTK_CONTAINER(button),box);

  return button;
}
/* returned buffer must be freed by caller, input buffer must be allocated
	in memory and it will be disposed */
gchar*
allocated_buffer_add_string(gchar* buffer, const gchar* string)
{
	gchar* newstring;
	newstring = g_strconcat(buffer, string, NULL);
	g_free(buffer);
	return newstring;
}

/* FIXME must be moved to a window function */
void
init_default_config( CssedWindow *window )
{
	CssedConfig* cfg;

	cfg = cssed_window_get_config( window );

	cfg->window_x = 100;
	cfg->window_y = 100;
	cfg->window_width = 600;
	cfg->window_height = 400;
	cfg->initiall_state_maximized = FALSE;
	cfg->footer_pos = 200;
	cfg->sidebar_pos = 400;
	cfg->linenumbers = FALSE;
	cfg->autocompletion = FALSE;
	cfg->lineendings = FALSE;
	cfg->whitespaces = FALSE;
	cfg->folding = FALSE;
	cfg->lineswraped = FALSE;
	cfg->font = NULL;
	cfg->font_size = 10; //start with a visible font size by default
	cfg->opened_docs = NULL;
	cfg->DEFAULT_fore_color = 0x000077;
	cfg->DEFAULT_back_color = 0xffffff;
	cfg->DEFAULT_bold = FALSE;
	cfg->TAG_fore_color = 0xA46621;
	cfg->TAG_back_color = 0xffffff;
	cfg->TAG_bold = TRUE;
	cfg->PSEUDOCLASS_fore_color = 0x990066;
	cfg->PSEUDOCLASS_back_color = 0xffffff;
	cfg->PSEUDOCLASS_bold = FALSE;
	cfg->UNKNOWN_PSEUDOCLASS_fore_color = 0x0000ff;
	cfg->UNKNOWN_PSEUDOCLASS_back_color = 0xffffff;
	cfg->UNKNOWN_PSEUDOCLASS_bold = FALSE;
	cfg->OPERATOR_fore_color = 0x000099;
	cfg->OPERATOR_back_color = 0xffffff;
	cfg->OPERATOR_bold = FALSE;
	cfg->IDENTIFIER_fore_color = 0x990000;
	cfg->IDENTIFIER_back_color = 0xffffff;
	cfg->IDENTIFIER_bold = TRUE;
	cfg->UNKNOWN_IDENTIFIER_fore_color = 0x0000ff;
	cfg->UNKNOWN_IDENTIFIER_back_color = 0xffffff;
	cfg->UNKNOWN_IDENTIFIER_bold = FALSE;
	cfg->VALUE_fore_color = 0x222222;
	cfg->VALUE_back_color = 0xffffff;
	cfg->VALUE_bold = FALSE;
	cfg->COMMENT_fore_color = 0x666666;
	cfg->COMMENT_back_color = 0xffffff;
	cfg->COMMENT_bold = FALSE;
	cfg->IMPORTANT_fore_color = 0x000099;
	cfg->IMPORTANT_back_color = 0xffffff;
	cfg->IMPORTANT_bold = TRUE;
	cfg->DIRECTIVE_fore_color = 0x660066;
	cfg->DIRECTIVE_back_color = 0xffffff;
	cfg->DIRECTIVE_bold = FALSE;
	cfg->DOUBLESTRING_fore_color = 0x660033;
	cfg->DOUBLESTRING_back_color = 0xffffff;
	cfg->DOUBLESTRING_bold = FALSE;
	cfg->SINGLESTRING_fore_color = 0x660033;
	cfg->SINGLESTRING_back_color = 0xffffff;
	cfg->SINGLESTRING_bold = FALSE;
	cfg->CLASS_fore_color = 0x004400;
	cfg->CLASS_back_color = 0xffffff;
	cfg->CLASS_bold = TRUE;
	cfg->ID_fore_color = 0x004400;
	cfg->ID_back_color = 0xffffff;
	cfg->ID_bold = TRUE;
	cfg->VALID_VALUE_fore_color = 0x003399;
	cfg->VALID_VALUE_back_color = 0xffffff;
	cfg->VALID_VALUE_bold = FALSE;
	cfg->FUNCTION_fore_color = 0x993300;
	cfg->FUNCTION_back_color = 0xffffff;
	cfg->FUNCTION_bold = FALSE;
	cfg->NUMBER_fore_color = 0x330099;
	cfg->NUMBER_back_color = 0xffffff;
	cfg->NUMBER_bold = FALSE;
	cfg->UNIT_fore_color = 0x990000;
	cfg->UNIT_back_color = 0xffffff;
	cfg->UNIT_bold = FALSE;
	cfg->COLOR_fore_color = 0x006600;
	cfg->COLOR_back_color = 0xffffff;
	cfg->COLOR_bold = FALSE;
	cfg->HEXACOLOR_fore_color = 0x006600;
	cfg->HEXACOLOR_back_color = 0xffffff;
	cfg->HEXACOLOR_bold = FALSE;
	cfg->ATTR_MATCH_fore_color = 0x004444;
	cfg->ATTR_MATCH_back_color = 0xffffff;
	cfg->ATTR_MATCH_bold = FALSE;
	cfg->LANGUAGE_fore_color = 0x990000;
	cfg->LANGUAGE_back_color = 0xffffff;
	cfg->LANGUAGE_bold = FALSE;
#ifdef DARWIN
	cfg->browser_command = g_strdup("open");
#else
	cfg->browser_command = NULL;
#endif
}

/* FIXME must be moved to a window function */
void
save_config_to_file(CssedWindow* window)
{
	CssedConfig* cfg;
	CssedPlugin* plugin;
	gint nplugins;
	GList* plugins;
	//GString *configstr;

	gchar* pluginstr;
	gchar* csseddir;
	gchar* cssedcfg_file;
	gchar* outstr;
	gchar* stylestr;
	gchar* styleout;

	gint wx, wy, ww, wh; // window: x y width height.
	GdkWindow* gdw;
	GtkWidget* window_widget;
	GtkWidget* panned_footer;
	GtkWidget* panned_tree;
	GtkWidget* panned_sidebar;
	gint footer_pos;
	gint tree_pos;
	gint sidebar_pos;
	int fd;
#ifdef WIN32
	int written;
	gchar* tmp;
	gchar* line;
#else
	ssize_t written;
#endif

	cfg = cssed_window_get_config( window );
	window_widget = cssed_window_get_window_widget( window );
	csseddir = g_strconcat(g_get_home_dir(), G_DIR_SEPARATOR_S, ".cssed", NULL);

	if( !g_file_test(csseddir,G_FILE_TEST_EXISTS | G_FILE_TEST_IS_DIR) ){
#ifdef WIN32
		if( mkdir(csseddir) == -1 ){
#else
		if( mkdir(csseddir, S_IRWXU) == -1 ){
#endif
			g_free(csseddir);
			return;
		}
	}

	cssedcfg_file = g_strconcat(csseddir, G_DIR_SEPARATOR_S, "cssed-cfg.xml",NULL);
	if( g_file_test(cssedcfg_file, G_FILE_TEST_IS_REGULAR| G_FILE_TEST_EXISTS) )
		unlink(cssedcfg_file); // we're deleting the file, if anything goes wrong ... Lost configuration ! FIXME 

	gdw = gtk_widget_get_parent_window(cssed_window_get_document_notebook(window));

	panned_footer = lookup_widget( window_widget, "vpaned_main" ); //
	panned_sidebar = lookup_widget( window_widget, "hpaned_main" );
	panned_tree = lookup_widget( window_widget, "vpaned_css" );
	
	if(gdk_window_get_state (gdw) == GDK_WINDOW_STATE_MAXIMIZED){
		ww = cfg->window_width;
		wh = cfg->window_height;
		wx = cfg->window_x;
		wy = cfg->window_y;
		cfg->initiall_state_maximized = TRUE;
	}else{ // get x & y pos to write to config
		ww = window_widget->allocation.width;
		wh = window_widget->allocation.height;
		gdk_window_get_root_origin(gdw, &wx, &wy);
		cfg->initiall_state_maximized = FALSE;
	}
	// get panned positions
	footer_pos = gtk_paned_get_position(GTK_PANED(panned_footer));
	sidebar_pos =  gtk_paned_get_position(GTK_PANED(panned_sidebar));
	tree_pos =  gtk_paned_get_position(GTK_PANED(panned_tree));

#ifdef WIN32
	fd = _open(cssedcfg_file, _O_WRONLY|_O_CREAT, _S_IREAD|_S_IWRITE);
#else
	fd = open(cssedcfg_file, O_WRONLY|O_CREAT, S_IRUSR|S_IWUSR);
#endif

	if( fd == -1 ){
		g_free(cssedcfg_file);
		g_free(csseddir);
		return;
	}

	if( cfg->browser_command ){
		outstr = g_strdup_printf("<cssed-conf>\n\t<browser command='%s' />\n\t", cfg->browser_command);
	}else{
		outstr = g_strdup("<cssed-conf>\n\t");
	}

	written = write(fd, outstr, strlen(outstr) * sizeof(gchar));
	g_free(outstr);
	outstr = NULL;

	if( written == -1 ){
		DBGMSG (__FILE__,__LINE__,"Can't write to file descriptor %d", fd);
		cssed_error_message(_("Cannot write to configuration file"),
							_("I'm unable to write to the configuration file,\nsettings will not be saved.")
							);
		return;
	}

	if( cfg->font == NULL ){
		outstr = g_strdup_printf(
		//g_string_append_printf (configstr,
"<document>\n\t\t \
<linenumbers show=\"%s\" />\n\t\t \
<autocompletion enabled=\"%s\" />\n\t\t \
<lineendings show=\"%s\" /> \n\t\t \
<whitespaces show=\"%s\" /> \n\t\t \
<folding enabled=\"%s\" /> \n\t\t \
<lineswraped show=\"%s\" />\n\t\t \
<usetabs value=\"%s\" /> \n\t\t \
<tabswidth value=\"%i\" /> \n\t \
</document>\n\t  \
<window x=\"%d\" y=\"%d\" width=\"%d\" height=\"%d\" maximized=\"%s\">\n\t\t  \
<footer position=\"%d\" />\n\t\t\
<sidebar position=\"%d\" />\n\t\t\
<csstreeview position=\"%d\" />\n\t\
</window>\n",
		cfg->linenumbers?"true":"false",
		cfg->autocompletion?"true":"false",
		cfg->lineendings?"true":"false",
		cfg->whitespaces?"true":"false",
		cfg->folding?"true":"false",
		cfg->lineswraped?"true":"false",
		cfg->usetabs?"true":"false",
		cfg->tabswidth,
		wx>0?wx:0,
		wy>0?wy:0,
		ww,
		wh,
		cfg->initiall_state_maximized?"true":"false",
		footer_pos,
		sidebar_pos,
		tree_pos				
		);
	}else{
		outstr = g_strdup_printf(
		//g_string_append_printf (configstr,
"<document>\n\t\t \
<linenumbers show=\"%s\" />\n\t\t \
<autocompletion enabled=\"%s\" />\n\t\t \
<lineendings show=\"%s\" /> \n\t\t \
<whitespaces show=\"%s\" /> \n\t\t \
<folding enabled=\"%s\" /> \n\t\t \
<lineswraped show=\"%s\" />\n\t \t \
<usetabs value=\"%s\" /> \n\t\t \
<tabswidth value=\"%i\" /> \n\t \
<font name=\"%s\" size=\"%d\" />\n\t \
</document>\n\t  \
<window x=\"%d\" y=\"%d\" width=\"%d\" height=\"%d\" maximized=\"%s\">\n\t\t  \
<footer position=\"%d\" />\n\t\t\
<sidebar position=\"%d\" />\n\t\t\
<csstreeview position=\"%d\" />\n\t\
</window>\n",
		cfg->linenumbers?"true":"false",
		cfg->autocompletion?"true":"false",
		cfg->lineendings?"true":"false",
		cfg->whitespaces?"true":"false",
		cfg->folding?"true":"false",
		cfg->lineswraped?"true":"false",
		cfg->usetabs?"true":"false",
		cfg->tabswidth,
		cfg->font,
		cfg->font_size,
		wx>0?wx:0,
		wy>0?wy:0,
		ww,
		wh,
		cfg->initiall_state_maximized?"true":"false",
		footer_pos,
		sidebar_pos,
		tree_pos
		);
	}

	written = write(fd, outstr, strlen(outstr) * sizeof(gchar));
	g_free(outstr);
	outstr = NULL;

	if( written == -1 ){
		DBGMSG (__FILE__,__LINE__,"Can't write to file descriptor %d", fd);
		cssed_error_message(_("Cannot write to configuration file"),
							_("I'm unable to write to the configuration file,\nsettings will not be saved.")
							);
		return;
	}
	/* *********************** loaded plugins ************************ */

	nplugins = cssed_window_get_plugin_list_length( window );
	if( nplugins > 0 ){
		pluginstr = g_strdup("\t<plugins>\n\t\t");
		plugins = cssed_window_get_plugin_list_first( window );
		while( plugins ){
			plugin = plugins->data;
			pluginstr = allocated_buffer_add_string(pluginstr, "<plugin file=\"" );
			pluginstr = allocated_buffer_add_string(pluginstr, cssed_plugin_get_filename(plugin));
			pluginstr = allocated_buffer_add_string(pluginstr, "\" />\n\t" );
			
			plugins = g_list_next( plugins );
			if( plugins ) pluginstr = allocated_buffer_add_string( pluginstr, "\t" );
		}
		pluginstr = allocated_buffer_add_string( pluginstr, "</plugins>\n\t" );
		written = write( fd, pluginstr, strlen(pluginstr) * sizeof(gchar));
		g_free( pluginstr );
		pluginstr = NULL;
		if( written == -1 ){
			DBGMSG (__FILE__,__LINE__,"Can't write to file descriptor %d", fd);
			cssed_error_message(_("Cannot write to configuration file"),
								_("I'm unable to write to the configuration file,\nsettings will not be saved."));
			return;
		}
	}

#ifdef WIN32
	/* I have to strip the g_strdup call in two pieces as it seems
	that glib (Win32) have problems with long va_args */
	stylestr = g_strdup_printf(
"<style name=\"DEFAULT\" fore=\"%#06x\" back=\"%#06x\" bold=\"%s\"	/>\n\t\t \
<style name=\"TAG\" fore=\"%#06x\" back=\"%#06x\" bold=\"%s\" />\n\t\t \
<style name=\"PSEUDOCLASS\" fore=\"%#06x\" back=\"%#06x\" bold=\"%s\" />\n\t\t \
<style name=\"UNKNOWN_PSEUDOCLASS\" fore=\"%#06x\" back=\"%#06x\" bold=\"%s\" />\n\t\t \
<style name=\"OPERATOR\" fore=\"%#06x\" back=\"%#06x\" bold=\"%s\" />\n\t\t \
<style name=\"IDENTIFIER\" fore=\"%#06x\" back=\"%#06x\" bold=\"%s\" />\n\t\t \
<style name=\"UNKNOWN_IDENTIFIER\" fore=\"%#06x\" back=\"%#06x\" bold=\"%s\" />\n\t\t \
<style name=\"VALUE\" fore=\"%#06x\" back=\"%#06x\" bold=\"%s\" />\n\t\t \
<style name=\"COMMENT\" fore=\"%#06x\" back=\"%#06x\" bold=\"%s\" />\n\t\t \
<style name=\"IMPORTANT\" fore=\"%#06x\" back=\"%#06x\" bold=\"%s\" />\n\t\t \
<style name=\"DIRECTIVE\" fore=\"%#06x\" back=\"%#06x\" bold=\"%s\" />\n\t\t \
<style name=\"DOUBLESTRING\" fore=\"%#06x\" back=\"%#06x\" bold=\"%s\" />\n\t\t ",
	cfg->DEFAULT_fore_color, cfg->DEFAULT_back_color, cfg->DEFAULT_bold?"true":"false",
		cfg->TAG_fore_color, cfg->TAG_back_color, cfg->TAG_bold?"true":"false",
		cfg->PSEUDOCLASS_fore_color, cfg->PSEUDOCLASS_back_color, cfg->PSEUDOCLASS_bold?"true":"false",
		cfg->UNKNOWN_PSEUDOCLASS_fore_color, cfg->UNKNOWN_PSEUDOCLASS_back_color, cfg->UNKNOWN_PSEUDOCLASS_bold?"true":"false",
		cfg->OPERATOR_fore_color, cfg->OPERATOR_back_color, cfg->OPERATOR_bold?"true":"false",
		cfg->IDENTIFIER_fore_color, cfg->IDENTIFIER_back_color, cfg->IDENTIFIER_bold?"true":"false",
		cfg->UNKNOWN_IDENTIFIER_fore_color, cfg->UNKNOWN_IDENTIFIER_back_color, cfg->UNKNOWN_IDENTIFIER_bold?"true":"false",
		cfg->VALUE_fore_color, cfg->VALUE_back_color, cfg->VALUE_bold?"true":"false",
		cfg->COMMENT_fore_color, cfg->COMMENT_back_color, cfg->COMMENT_bold?"true":"false",
		cfg->IMPORTANT_fore_color, cfg->IMPORTANT_back_color, cfg->IMPORTANT_bold?"true":"false",
		cfg->DIRECTIVE_fore_color, cfg->DIRECTIVE_back_color, cfg->DIRECTIVE_bold?"true":"false",
		cfg->DOUBLESTRING_fore_color, cfg->DOUBLESTRING_back_color, cfg->DOUBLESTRING_bold?"true":"false"
		);

	line = g_strdup_printf(
		"<style name=\"SINGLESTRING\" fore=\"%#06x\" back=\"%#06x\" bold=\"%s\" />\n\t\t \
<style name=\"CLASS\" fore=\"%#06x\" back=\"%#06x\" bold=\"%s\" />\n\t\t \
<style name=\"ID\" fore=\"%#06x\" back=\"%#06x\" bold=\"%s\" />\n\t\t \
<style name=\"VALID_VALUE\" fore=\"%#06x\" back=\"%#06x\" bold=\"%s\" />\n\t\t \
<style name=\"FUNCTION\" fore=\"%#06x\" back=\"%#06x\" bold=\"%s\" />\n\t\t \
<style name=\"NUMBER\" fore=\"%#06x\" back=\"%#06x\" bold=\"%s\" />\n\t\t \
<style name=\"UNIT\" fore=\"%#06x\" back=\"%#06x\" bold=\"%s\" />\n\t\t \
<style name=\"COLOR\" fore=\"%#06x\" back=\"%#06x\" bold=\"%s\" />\n\t\t \
<style name=\"HEXACOLOR\" fore=\"%#06x\" back=\"%#06x\" bold=\"%s\" />\n\t\t \
<style name=\"ATTR_MATCH\" fore=\"%#06x\" back=\"%#06x\" bold=\"%s\" />\n\t\t \
<style name=\"LANGUAGE\" fore=\"%#06x\" back=\"%#06x\" bold=\"%s\" />\n\t",
		cfg->SINGLESTRING_fore_color, cfg->SINGLESTRING_back_color, cfg->SINGLESTRING_bold?"true":"false",
		cfg->CLASS_fore_color, cfg->CLASS_back_color, cfg->CLASS_bold?"true":"false",
		cfg->ID_fore_color, cfg->ID_back_color, cfg->ID_bold?"true":"false",
		cfg->VALID_VALUE_fore_color, cfg->VALID_VALUE_back_color, cfg->VALID_VALUE_bold?"true":"false",
		cfg->FUNCTION_fore_color, cfg->FUNCTION_back_color, cfg->FUNCTION_bold?"true":"false",
		cfg->NUMBER_fore_color, cfg->NUMBER_back_color, cfg->NUMBER_bold?"true":"false",
		cfg->UNIT_fore_color, cfg->UNIT_back_color, cfg->UNIT_bold?"true":"false",
		cfg->COLOR_fore_color, cfg->COLOR_back_color, cfg->COLOR_bold?"true":"false",
		cfg->HEXACOLOR_fore_color, cfg->HEXACOLOR_back_color, cfg->HEXACOLOR_bold?"true":"false",
		cfg->ATTR_MATCH_fore_color, cfg->ATTR_MATCH_back_color, cfg->ATTR_MATCH_bold?"true":"false",
		cfg->LANGUAGE_fore_color, cfg->LANGUAGE_back_color, cfg->LANGUAGE_bold?"true":"false"
	);
	tmp = stylestr;
	stylestr = g_strconcat(tmp, line, NULL);
	g_free(tmp);
	g_free(line);
#else
	stylestr = g_strdup_printf(
"<style name=\"DEFAULT\" fore=\"%#06x\" back=\"%#06x\" bold=\"%s\"	/>\n\t\t \
<style name=\"TAG\" fore=\"%#06x\" back=\"%#06x\" bold=\"%s\" />\n\t\t \
<style name=\"PSEUDOCLASS\" fore=\"%#06x\" back=\"%#06x\" bold=\"%s\" />\n\t\t \
<style name=\"UNKNOWN_PSEUDOCLASS\" fore=\"%#06x\" back=\"%#06x\" bold=\"%s\" />\n\t\t \
<style name=\"OPERATOR\" fore=\"%#06x\" back=\"%#06x\" bold=\"%s\" />\n\t\t \
<style name=\"IDENTIFIER\" fore=\"%#06x\" back=\"%#06x\" bold=\"%s\" />\n\t\t \
<style name=\"UNKNOWN_IDENTIFIER\" fore=\"%#06x\" back=\"%#06x\" bold=\"%s\" />\n\t\t \
<style name=\"VALUE\" fore=\"%#06x\" back=\"%#06x\" bold=\"%s\" />\n\t\t \
<style name=\"COMMENT\" fore=\"%#06x\" back=\"%#06x\" bold=\"%s\" />\n\t\t \
<style name=\"IMPORTANT\" fore=\"%#06x\" back=\"%#06x\" bold=\"%s\" />\n\t\t \
<style name=\"DIRECTIVE\" fore=\"%#06x\" back=\"%#06x\" bold=\"%s\" />\n\t\t \
<style name=\"DOUBLESTRING\" fore=\"%#06x\" back=\"%#06x\" bold=\"%s\" />\n\t\t \
<style name=\"SINGLESTRING\" fore=\"%#06x\" back=\"%#06x\" bold=\"%s\" />\n\t\t \
<style name=\"CLASS\" fore=\"%#06x\" back=\"%#06x\" bold=\"%s\" />\n\t\t \
<style name=\"ID\" fore=\"%#06x\" back=\"%#06x\" bold=\"%s\" />\n\t\t \
<style name=\"VALID_VALUE\" fore=\"%#06x\" back=\"%#06x\" bold=\"%s\" />\n\t\t \
<style name=\"FUNCTION\" fore=\"%#06x\" back=\"%#06x\" bold=\"%s\" />\n\t\t \
<style name=\"NUMBER\" fore=\"%#06x\" back=\"%#06x\" bold=\"%s\" />\n\t\t \
<style name=\"UNIT\" fore=\"%#06x\" back=\"%#06x\" bold=\"%s\" />\n\t\t \
<style name=\"COLOR\" fore=\"%#06x\" back=\"%#06x\" bold=\"%s\" />\n\t\t \
<style name=\"HEXACOLOR\" fore=\"%#06x\" back=\"%#06x\" bold=\"%s\" />\n\t\t \
<style name=\"ATTR_MATCH\" fore=\"%#06x\" back=\"%#06x\" bold=\"%s\" />\n\t\t \
<style name=\"LANGUAGE\" fore=\"%#06x\" back=\"%#06x\" bold=\"%s\" />\n\t",
		cfg->DEFAULT_fore_color, cfg->DEFAULT_back_color, cfg->DEFAULT_bold?"true":"false",
		cfg->TAG_fore_color, cfg->TAG_back_color, cfg->TAG_bold?"true":"false",
		cfg->PSEUDOCLASS_fore_color, cfg->PSEUDOCLASS_back_color, cfg->PSEUDOCLASS_bold?"true":"false",
		cfg->UNKNOWN_PSEUDOCLASS_fore_color, cfg->UNKNOWN_PSEUDOCLASS_back_color, cfg->UNKNOWN_PSEUDOCLASS_bold?"true":"false",
		cfg->OPERATOR_fore_color, cfg->OPERATOR_back_color, cfg->OPERATOR_bold?"true":"false",
		cfg->IDENTIFIER_fore_color, cfg->IDENTIFIER_back_color, cfg->IDENTIFIER_bold?"true":"false",
		cfg->UNKNOWN_IDENTIFIER_fore_color, cfg->UNKNOWN_IDENTIFIER_back_color, cfg->UNKNOWN_IDENTIFIER_bold?"true":"false",
		cfg->VALUE_fore_color, cfg->VALUE_back_color, cfg->VALUE_bold?"true":"false",
		cfg->COMMENT_fore_color, cfg->COMMENT_back_color, cfg->COMMENT_bold?"true":"false",
		cfg->IMPORTANT_fore_color, cfg->IMPORTANT_back_color, cfg->IMPORTANT_bold?"true":"false",
		cfg->DIRECTIVE_fore_color, cfg->DIRECTIVE_back_color, cfg->DIRECTIVE_bold?"true":"false",
		cfg->DOUBLESTRING_fore_color, cfg->DOUBLESTRING_back_color, cfg->DOUBLESTRING_bold?"true":"false",
		cfg->SINGLESTRING_fore_color, cfg->SINGLESTRING_back_color, cfg->SINGLESTRING_bold?"true":"false",
		cfg->CLASS_fore_color, cfg->CLASS_back_color, cfg->CLASS_bold?"true":"false",
		cfg->ID_fore_color, cfg->ID_back_color, cfg->ID_bold?"true":"false",
		cfg->VALID_VALUE_fore_color, cfg->VALID_VALUE_back_color, cfg->VALID_VALUE_bold?"true":"false",
		cfg->FUNCTION_fore_color, cfg->FUNCTION_back_color, cfg->FUNCTION_bold?"true":"false",
		cfg->NUMBER_fore_color, cfg->NUMBER_back_color, cfg->NUMBER_bold?"true":"false",
		cfg->UNIT_fore_color, cfg->UNIT_back_color, cfg->UNIT_bold?"true":"false",
		cfg->COLOR_fore_color, cfg->COLOR_back_color, cfg->COLOR_bold?"true":"false",
		cfg->HEXACOLOR_fore_color, cfg->HEXACOLOR_back_color, cfg->HEXACOLOR_bold?"true":"false",
		cfg->ATTR_MATCH_fore_color, cfg->ATTR_MATCH_back_color, cfg->ATTR_MATCH_bold?"true":"false",
		cfg->LANGUAGE_fore_color, cfg->LANGUAGE_back_color, cfg->LANGUAGE_bold?"true":"false"
	);
#endif
	styleout = g_strconcat("<highlighting> \n\t\t",stylestr,"</highlighting>\n</cssed-conf>",NULL);
	g_free(stylestr);

	written = write(fd, styleout, strlen(styleout) * sizeof(gchar));
	DBGMSG (__FILE__,__LINE__,"Config written %d chars",written);

	g_free( styleout );
	g_free( cssedcfg_file );
	g_free( csseddir );

#ifdef WIN32
	_close(fd);
#else
	close( fd );
#endif
}


void
cssed_config_copy_style_and_defaults( CssedConfig* source, CssedConfig* target )
{
    memcpy (target, source, sizeof(CssedConfig)); 
/*
	target->linenumbers  = source->linenumbers ;
	target->autocompletion  = source->autocompletion ;
	target->lineendings  = source->lineendings ;
	target->whitespaces  = source->whitespaces ;
	target->folding = source->folding ;
	target->usetabs = source->usetabs ;
	target->tabswidth = source->tabswidth ;
	target->lineswraped = source->lineswraped ;
	target->font = source->font ;
	target->font_size = source->font_size ;
	target->opened_docs = source->opened_docs ;
	target->browser_command  = source->browser_command;
	target->DEFAULT_fore_color = source->DEFAULT_fore_color ;
	target->DEFAULT_back_color = source->DEFAULT_back_color ;
	target->DEFAULT_bold = source->DEFAULT_bold ;
	target->TAG_fore_color = source->TAG_fore_color ;
	target->TAG_back_color = source->TAG_back_color ;
	target->TAG_bold = source->TAG_bold ;
	target->PSEUDOCLASS_fore_color = source->PSEUDOCLASS_fore_color ;
	target->PSEUDOCLASS_back_color = source->PSEUDOCLASS_back_color ;
	target->PSEUDOCLASS_bold = source->PSEUDOCLASS_bold ;
	target->UNKNOWN_PSEUDOCLASS_fore_color = source->UNKNOWN_PSEUDOCLASS_fore_color ;
	target->UNKNOWN_PSEUDOCLASS_back_color = source->UNKNOWN_PSEUDOCLASS_back_color ;
	target->UNKNOWN_PSEUDOCLASS_bold = source->UNKNOWN_PSEUDOCLASS_bold ;
	target->OPERATOR_fore_color = source->OPERATOR_fore_color ;
	target->OPERATOR_back_color = source->OPERATOR_back_color ;
	target->OPERATOR_bold = source->OPERATOR_bold ;
	target->IDENTIFIER_fore_color = source->IDENTIFIER_fore_color ;
	target->IDENTIFIER_back_color = source->IDENTIFIER_back_color ;
	target->IDENTIFIER_bold = source->IDENTIFIER_bold ;
	target->UNKNOWN_IDENTIFIER_fore_color = source->UNKNOWN_IDENTIFIER_fore_color ;
	target->UNKNOWN_IDENTIFIER_back_color = source->UNKNOWN_IDENTIFIER_back_color ;
	target->UNKNOWN_IDENTIFIER_bold = source->UNKNOWN_IDENTIFIER_bold ;
	target->VALUE_fore_color = source->VALUE_fore_color ;
	target->VALUE_back_color = source->VALUE_back_color ;
	target->VALUE_bold = source->VALUE_bold ;
	target->COMMENT_fore_color = source->COMMENT_fore_color ;
	target->COMMENT_back_color = source->COMMENT_back_color ;
	target->COMMENT_bold = source->COMMENT_bold ;
	target->IMPORTANT_fore_color = source->IMPORTANT_fore_color ;
	target->IMPORTANT_back_color = source->IMPORTANT_back_color ;
	target->IMPORTANT_bold = source->IMPORTANT_bold ;
	target->DIRECTIVE_fore_color = source->DIRECTIVE_fore_color ;
	target->DIRECTIVE_back_color = source->DIRECTIVE_back_color ;
	target->DIRECTIVE_bold = source->DIRECTIVE_bold ;
	target->DOUBLESTRING_fore_color = source->DOUBLESTRING_fore_color ;
	target->DOUBLESTRING_back_color = source->DOUBLESTRING_back_color ;
	target->DOUBLESTRING_bold = source->DOUBLESTRING_bold ;
	target->SINGLESTRING_fore_color = source->SINGLESTRING_fore_color ;
	target->SINGLESTRING_back_color = source->SINGLESTRING_back_color ;
	target->SINGLESTRING_bold = source->SINGLESTRING_bold ;
	target->CLASS_fore_color = source->CLASS_fore_color ;
	target->CLASS_back_color = source->CLASS_back_color ;
	target->CLASS_bold = source->CLASS_bold ;
	target->ID_fore_color = source->ID_fore_color ;
	target->ID_back_color = source->ID_back_color ;
	target->ID_bold = source->ID_bold ;
	target->VALID_VALUE_fore_color = source->VALID_VALUE_fore_color ;
	target->VALID_VALUE_back_color = source->VALID_VALUE_back_color ;
	target->VALID_VALUE_bold = source->VALID_VALUE_bold ;
	target->FUNCTION_fore_color = source->FUNCTION_fore_color ;
	target->FUNCTION_back_color = source->FUNCTION_back_color ;
	target->FUNCTION_bold = source->FUNCTION_bold ;
	target->NUMBER_fore_color = source->NUMBER_fore_color ;
	target->NUMBER_back_color = source->NUMBER_back_color ;
	target->NUMBER_bold = source->NUMBER_bold ;
	target->UNIT_fore_color = source->UNIT_fore_color ;
	target->UNIT_back_color = source->UNIT_back_color ;
	target->UNIT_bold = source->UNIT_bold ;
	target->COLOR_fore_color = source->COLOR_fore_color ;
	target->COLOR_back_color = source->COLOR_back_color ;
	target->COLOR_bold = source->COLOR_bold ;
	target->HEXACOLOR_fore_color = source->HEXACOLOR_fore_color ;
	target->HEXACOLOR_back_color = source->HEXACOLOR_back_color ;
	target->HEXACOLOR_bold = source->HEXACOLOR_bold ;
	target->ATTR_MATCH_fore_color = source->ATTR_MATCH_fore_color ;
	target->ATTR_MATCH_back_color = source->ATTR_MATCH_back_color ;
	target->ATTR_MATCH_bold = source->ATTR_MATCH_bold ;
	target->LANGUAGE_fore_color = source->LANGUAGE_fore_color ;
	target->LANGUAGE_back_color = source->LANGUAGE_back_color ;
	target->LANGUAGE_bold = source->LANGUAGE_bold ;

*/
}

/* FIXME this shoud be window function */
void
load_all_plugins( CssedWindow* cssedwin, GList* plugins_to_load )
{
	GList* plugins;
	CssedPlugin* plugin;

	g_return_if_fail( plugins_to_load != NULL );
	plugins = g_list_first(plugins_to_load);

	while( plugins ){
		plugin = cssed_init_plugin(cssedwin, plugins->data);
		if( plugin ){
			cssed_load_plugin(plugin);
		}
		plugin = NULL;
		plugins = g_list_next(plugins);
	}
	g_list_free(plugins);
}

/* used by the xml parsers FIXME */
gboolean
char_ptr_is_decimal_num(const gchar* ptr)
{
	gint len, i;
	len = strlen(ptr);

	if( len <= 0 )
		return FALSE;

	for(i=0;i<len;i++){
		if( !isdigit( ptr[i] ) )
				return FALSE;
	}
	return TRUE;
}

/* 	This should go to the - future - css support code FIXME 
*/
gint
tree_iter_doc_scanner_compare_string   (GtkTreeModel *model,
										GtkTreeIter *a,
										GtkTreeIter *b,
										gpointer user_data)
{
	gchar* first_string;
	gchar* second_string;
	gchar* cleaned_first;
	gchar* cleaned_second;
	gint len_first;
	gint len_second;
	gint first;
	gint second;
	gint col = GPOINTER_TO_INT(user_data);

	gtk_tree_model_get(model, a, col, &first_string, -1);
    gtk_tree_model_get(model, b, col, &second_string, -1);

	if((first_string==NULL) && (second_string==NULL))	return 0;
	if((first_string==NULL) && (second_string!=NULL))	return 1;
	if((first_string!=NULL) && (second_string==NULL))	return -1;

	//clean first
	pango_parse_markup (first_string, strlen(first_string), 0, NULL, &cleaned_first, NULL, NULL);
	pango_parse_markup (second_string, strlen(second_string), 0, NULL, &cleaned_second, NULL, NULL);

	switch( col ){
		case 0: // clean the number in the form <span>[111]</span> and convert to int
			len_first = strlen(cleaned_first);
			len_second = strlen(cleaned_second);
			return g_ascii_strncasecmp(cleaned_first, cleaned_second, len_first>len_second?len_first:len_second);
		break;

		case 1: // clean the number in the form <span>[111]</span> and convert to int
			first = atoi(cleaned_first);
			second = atoi(cleaned_second);
			if( first == second ) return 0;
			else return first>second?1:-1;
		break;
		case 2:
		case 3:
			len_first = strlen(cleaned_first);
			len_second = strlen(cleaned_second);
			return g_ascii_strncasecmp(cleaned_first, cleaned_second, len_first>len_second?len_first:len_second);
		break;
	}
	g_free( cleaned_first );
	g_free( cleaned_second );
	g_free( first_string );
	g_free( second_string );

	return 0;
}

/* THOSE SHOULD GO TO CALLBACKS */
/* load save text buffer */
void
text_buffer_save_contents( GtkTextBuffer* buffer, gchar* filename )
{
	GtkTextIter iter_start;
	GtkTextIter iter_end;
	gchar* text;
	FILE* f;

	gtk_text_buffer_get_start_iter(buffer,&iter_start);
	gtk_text_buffer_get_end_iter(buffer,&iter_end);

	text = gtk_text_buffer_get_text(buffer, &iter_start, &iter_end, FALSE);

	f = fopen (filename, "w");
	if (f == NULL)
	{
		g_free (text);
	}
	else
	{
		fwrite (text, sizeof (gchar), strlen(text), f);
		g_free (text);
		fclose (f);
	}
}

void
text_buffer_load_contents( GtkTextBuffer* buffer, gchar* filename )
{
	gchar* text;

	if ( g_file_get_contents( filename, &text, NULL, NULL ) ){
		gtk_text_buffer_set_text(buffer, text,  -1);
		g_free(text);
	}
}
/* load save scratch pad */
void
scratch_pad_load_contents( GtkTextBuffer* scratch_pad_buffer )
{
	gchar* file;

#ifdef WIN32
	file = g_strconcat( g_get_home_dir(), "\\cssed\\scratch", NULL );
#else
	file = g_strconcat( g_get_home_dir(), "/.cssed/scratch", NULL );
#endif

	text_buffer_load_contents(scratch_pad_buffer, file);
	g_free(file);
}
void
scratch_pad_save_contents( GtkTextBuffer* scratch_pad_buffer )
{
	gchar* file;

#ifdef WIN32
	file = g_strconcat( g_get_home_dir(), "\\cssed\\scratch", NULL );
#else
	file = g_strconcat( g_get_home_dir(), "/.cssed/scratch", NULL );
#endif
	text_buffer_save_contents(scratch_pad_buffer, file);
	g_free(file);
}

CssedEolMode
buffer_get_first_eol( gchar* buffer_start, gchar* buffer_end )
{
	gchar* iterator;
	
#ifdef WIN32
	g_return_val_if_fail( buffer_start != NULL, EOL_MODE_CRLF ); // default Win32
#else
	g_return_val_if_fail( buffer_start != NULL, EOL_MODE_LF ); // default Unix/Linux/OsX
#endif		
	
	iterator = buffer_start;
	
	if( buffer_end == NULL ){
		while( *iterator != '\0' ){
			if( *iterator == '\r' ){
				if( *(iterator + 1) == '\n' )
					return EOL_MODE_CRLF;
				else
					return EOL_MODE_CR;
			}else if( *iterator == '\n' ){
				return EOL_MODE_LF;
			}	
			++iterator;			
		}
	} else {
		if( !(buffer_end > buffer_start)){ // empty buffer
#ifdef WIN32
			return EOL_MODE_CRLF; // default
#else
			return EOL_MODE_LF; // default
#endif		
		} 		
		while( *iterator != '\0' && iterator != buffer_end ){
			if( *iterator == '\r' ){
				if( *(iterator + 1) == '\n' )
					return EOL_MODE_CRLF;
				else
					return EOL_MODE_CR;
			}else if( *iterator == '\n' ){
				return EOL_MODE_LF;
			}	
			++iterator;	
		}		
	}
	
#ifdef WIN32
	return EOL_MODE_CRLF; // default
#else
	return EOL_MODE_LF; // default
#endif	
	
}

// This is to be used with both Gtk2.2 and Gtk2.4
// FIXME: there're around a big bunch of different 
// file open - save funtions, that should be deleted
// and changed for a generic one.
// This is the seed for this generic function but right now
// it's only used in newly written code.

gchar* 
cssed_prompt_for_file (CssedWindow* window, gchar *title, gchar* path, CssedFileSelectorType type)
{
	GtkWidget *opensel = NULL; // keep compiler happy
	GtkWindow *window_widget;
	gchar *last_dir;
	gchar *filename, *basename;
#ifndef GTK_ATLEAST_2_4
	G_CONST_RETURN  gchar* sel_filename = NULL;
	gchar *fullname;
#endif	

	if( window != NULL )
		window_widget = GTK_WINDOW(cssed_window_get_window_widget(window));
	else
		window_widget = NULL;
		
#ifndef GTK_ATLEAST_2_4
	opensel = gtk_file_selection_new (title);
	switch( type ){
		case CSSED_FILESELECTOR_SAVE:
		case CSSED_FILESELECTOR_SELECT_FOLDER:
			if( window ){		
				if( path == NULL ){
					last_dir = cssed_window_get_last_save_dir( window );
					if( last_dir != NULL ){
						fullname = g_strconcat( last_dir, G_DIR_SEPARATOR_S, NULL);
						gtk_file_selection_set_filename( GTK_FILE_SELECTION (opensel), fullname );
						g_free(last_dir);
						g_free(fullname);
					}
				}else{
					gtk_file_selection_set_filename( GTK_FILE_SELECTION (opensel), path );
				}
			}
			break;
		case CSSED_FILESELECTOR_OPEN:
		case CSSED_FILESELECTOR_CREATE_FOLDER:
			if( window ){			
				if( path == NULL ){	
					last_dir = cssed_window_get_last_open_dir( window );
					if( last_dir != NULL ){
						fullname = g_strconcat( last_dir, G_DIR_SEPARATOR_S, NULL);
						gtk_file_selection_set_filename( GTK_FILE_SELECTION (opensel), fullname );
						g_free(last_dir);
						g_free(fullname);
					}
				}else{
					gtk_file_selection_set_filename( GTK_FILE_SELECTION (opensel), path );
				}
			}
			break;
	}
#else // defined GTK_ATLEAST_2_4
	switch( type ){
		case CSSED_FILESELECTOR_OPEN:
			opensel = gtk_file_chooser_dialog_new(title, window_widget, type,
												GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
												GTK_STOCK_OPEN, GTK_RESPONSE_OK, NULL);
			if( window ){		
				if( path == NULL ){
					last_dir = cssed_window_get_last_open_dir( window );
					if( last_dir != NULL ){
						gtk_file_chooser_set_current_folder( GTK_FILE_CHOOSER (opensel), last_dir );
						g_free(last_dir);
					}
				}else{
					if( g_file_test(path, G_FILE_TEST_IS_DIR) ) {
						gtk_file_chooser_set_current_folder(GTK_FILE_CHOOSER (opensel), path);
					}else{	
						//basename = 	g_path_get_basename (path);
						gtk_file_chooser_set_current_folder(GTK_FILE_CHOOSER (opensel), path);
						//gtk_file_chooser_set_current_name(GTK_FILE_CHOOSER (opensel), basename);
						//g_free( basename );
					}
				}
			}
			break;
		case CSSED_FILESELECTOR_SAVE:
			opensel = gtk_file_chooser_dialog_new(title, window_widget, type,
												GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
												GTK_STOCK_SAVE, GTK_RESPONSE_OK, NULL);
			if( window ){		
				if( path == NULL ){		
					last_dir = cssed_window_get_last_save_dir( window );
					if( last_dir != NULL ){
						gtk_file_chooser_set_current_folder( GTK_FILE_CHOOSER (opensel), last_dir );
						g_free(last_dir);
					}
				}else{
					if( g_file_test(path, G_FILE_TEST_IS_DIR) ) {
						gtk_file_chooser_set_current_folder(GTK_FILE_CHOOSER (opensel), path);
					}else{	
						gchar *dirname = g_path_get_dirname(path);
						if( dirname ){
							gtk_file_chooser_set_current_folder(GTK_FILE_CHOOSER (opensel), dirname);
							g_free(dirname);
						}
						basename = 	g_path_get_basename (path);
						gtk_file_chooser_set_current_name(GTK_FILE_CHOOSER (opensel), basename);
						g_free( basename );
					}
				}
			}
			break; 
		case CSSED_FILESELECTOR_SELECT_FOLDER:
			opensel = gtk_file_chooser_dialog_new(title, window_widget, type,
												GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
												GTK_STOCK_OPEN, GTK_RESPONSE_OK, NULL);
			if( window ){			
				if( path == NULL ){			
					last_dir = cssed_window_get_last_open_dir( window );
					if( last_dir != NULL ){
						gtk_file_chooser_set_current_folder( GTK_FILE_CHOOSER (opensel), last_dir );
						g_free(last_dir);
					}
				}else{
					 gtk_file_chooser_set_current_folder(GTK_FILE_CHOOSER (opensel), path);
				}
			}
			break;
		case CSSED_FILESELECTOR_CREATE_FOLDER:
			opensel = gtk_file_chooser_dialog_new(title, window_widget, type,
												GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
												GTK_STOCK_NEW, GTK_RESPONSE_OK, NULL);
			if( window ){				
				if( path == NULL ){		
					last_dir = cssed_window_get_last_save_dir( window );
					if( last_dir != NULL ){
						gtk_file_chooser_set_current_folder( GTK_FILE_CHOOSER (opensel), last_dir );
						g_free(last_dir);
					}
				}else{
					gtk_file_chooser_set_current_folder(GTK_FILE_CHOOSER (opensel), path);
				}
			}
			break;
	}
#endif
	if( gtk_dialog_run(GTK_DIALOG(opensel)) == GTK_RESPONSE_OK ){
#ifndef GTK_ATLEAST_2_4
		sel_filename = gtk_file_selection_get_filename( GTK_FILE_SELECTION(opensel) );
		filename = g_strdup(sel_filename);
#else 
		filename = gtk_file_chooser_get_filename( GTK_FILE_CHOOSER(opensel) );
#endif
		gtk_widget_destroy(opensel);
		return filename;
	}else{
		gtk_widget_destroy(opensel);
		return NULL;
	}
}

// shows an error message from GError, see document.h for error codes
void
cssed_show_message_from_error (GError *error, gchar *title, gboolean free_error)
{
	g_return_if_fail( error != NULL );
	
	switch(error->code){
		case CSSED_FILE_ERROR_LAST:
		case CSSED_ERROR_FILE_IS_OPENED:
			break;// those should be ignored
		default:
			cssed_error_message (title, error->message);
			break;
	}
	if( free_error ) g_error_free (error);
}

// for variable substitution, don't use it with large text as it uses a copy - so it doubles the
// memory needed for the string substitution.
gchar* cssed_string_replace_all(gchar *string, gchar *replaced_str, gchar *replacement_str)
{
	gchar* string_copy; // we'll manipulate it so we need a copy
	gchar *iterator, *match, *endstr; // catch position pointers
	GString *newstring;

	g_return_val_if_fail(string != NULL, NULL);
	g_return_val_if_fail(replaced_str != NULL, NULL);
	g_return_val_if_fail(replacement_str != NULL, NULL);

	newstring = g_string_new("");
	string_copy = g_strdup(string);
	endstr = string_copy + strlen(string_copy);
	iterator = string_copy;

	while( (match = strstr (iterator, replaced_str)) != NULL ){
		*match = '\0';
		newstring = g_string_append(newstring, iterator);
		newstring = g_string_append(newstring, replacement_str);
		iterator = match + strlen(replaced_str);
	}
	
	// if no match it'll copy the whole string, if there's something
	// before last match will append the leading text.
	if( iterator != endstr )
		newstring = g_string_append(newstring, iterator);

	g_free(string_copy);
	string_copy = g_string_free(newstring, FALSE);
	
	return string_copy;
} 

gint
cssed_string_unescape_uri( const gchar *source, gchar **retval )

 {
	gint pos, value, len, i=0;
	gchar dummy[] = " ";
	const gchar hex_digits[] = "0123456789ABCDEF";
	gchar *dest;

	g_return_val_if_fail(source, -1);
	len = strlen(source);
	*retval =  g_malloc0(len*sizeof(gchar));
	dest = *retval;

	while( i < len )
    {
		if( '\0' == *source ){
			dest[i] = '\0';
			return( i );
		}
		if( '%' != *source ){
			dest[i] = *source;
			source++;
		}else{
			*dummy = toupper( *(++source) );
			pos = (int)strcspn( hex_digits, dummy );

			if( pos < 16 )
			{
				value = pos;
				*dummy = toupper( *(++source) );
				pos = (int)strcspn( hex_digits, dummy );
				if( pos < 16 )
				{
					value = (value * 16) + pos;
					source++;
				}
				dest[i] = (char)value;
			} else {
				dest[i] = '%';
			}
		}
		i++; 
	}

	dest[len-1] = '\0';
	return( -1 );
}

