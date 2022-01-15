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

#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <stdio.h>

#include <gdk/gdkkeysyms.h>
#include <gtk/gtk.h>

#include "cssedwindow.h"
#include "configdlg-callbacks.h"
#include "configdlg-interface.h"
#include "support.h"
#include "utils.h"

#define FOREGROUND_STRING _("foreground")
#define BACKGROUND_STRING _("background")
#define BOLD_STRING _("bold")
#define BLANK_STRING "    "

#define GLADE_HOOKUP_OBJECT(component,widget,name) \
  g_object_set_data_full (G_OBJECT (component), name, \
    gtk_widget_ref (widget), (GDestroyNotify) gtk_widget_unref)

#define GLADE_HOOKUP_OBJECT_NO_REF(component,widget,name) \
  g_object_set_data (G_OBJECT (component), name, widget)

void
attach_colored_label_to_button( GtkWidget* button, gint color)
{
	GtkWidget* label;
	gchar* markup;
	gchar* colorstr;
	gchar red_one, red_two, green_one, green_two, blue_one, blue_two;
	
	colorstr = g_strdup_printf("%06x",color);
	if( strlen(colorstr) != 6 ){
		if( strlen( colorstr) == 1 ){ // black or gray??
			g_free( colorstr );
			colorstr = g_strdup_printf("%02x%02x%02x",color,color,color);
		}else if( strlen( colorstr) == 2 ){ // blue 
			g_free( colorstr );
			colorstr = g_strdup_printf("%02x0000",color);
		}else if( strlen( colorstr) == 4 ){ // green 
			g_free( colorstr );
			colorstr = g_strdup_printf("00%04x",color);
		}
	}else{
		red_one = colorstr[4];
		red_two  = colorstr[5];
		green_one = colorstr[2];
		green_two = colorstr[3];
		blue_one = colorstr[0];
		blue_two = colorstr[1];
		
		colorstr[0] = red_one;
		colorstr[1] = red_two;
		colorstr[2] = green_one;
		colorstr[3] = green_two;
		colorstr[4] = blue_one;
		colorstr[5] = blue_two;		
	}
	
	markup = g_strdup_printf("<span background='#%s'>    </span>", colorstr);
	label = gtk_label_new(markup);
	gtk_label_set_use_markup(GTK_LABEL(label), TRUE);
	gtk_widget_show(label);
	gtk_container_add(GTK_CONTAINER(button),label);	
	g_free( colorstr );
	g_free( markup );
}

// The tmpconfig will be filled with the old configuration and changes will
// be made directly to the window->config structure.
// The caller will restore old configuration if the user cancel the dialog	
GtkWidget *
create_preferences_dialog (CssedWindow * window, CssedConfig* tempconfig)
{
	GtkWidget *preferences_dialog;
	GtkWidget *dialog_vbox;
	GtkWidget *notebook;
	GtkWidget *scrolledwindow_highlighting;
	GtkWidget *viewport_highlighting;
	GtkWidget *table_highlighting;
	GtkWidget *label_tag_fg;
	GtkWidget *label_pseudoclass_fg;
	GtkWidget *label_unk_pseudoclass_fg;
  	GtkWidget *label_comment_fg;
	GtkWidget *label_operator_fg;
	GtkWidget *label_vproperty_fg;
	GtkWidget *label_unk_property_fg;
	GtkWidget *label_value_fg;
	GtkWidget *label_unk_value_fg;
	GtkWidget *label_default_fg;
	GtkWidget *label_important_fg;
	GtkWidget *label_directive_fg;
	GtkWidget *label_string_double_fg;
	GtkWidget *label_string_single_fg;
	GtkWidget *label_class_fg;
	GtkWidget *label_id_fg;
	GtkWidget *label_lang_fg;
	GtkWidget *label_attr_match_fg;
	GtkWidget *label_hexacolor_fg;
	GtkWidget *label_color_fg;
	GtkWidget *label_unit_fg;
	GtkWidget *label_number_fg;
	GtkWidget *label_function_fg;
	GtkWidget *button_vproperty_fg;
	GtkWidget *button_operator_fg;
	GtkWidget *button_unk_pseudoclass_fg;
	GtkWidget *button_pseudoclass_fg;
	GtkWidget *button_tag_fg;
	//GtkWidget *button_comment_fg;
	GtkWidget *button_default_bg;
	GtkWidget *button_unk_property_fg;
	GtkWidget *button_value_fg;
	GtkWidget *button_unk_value_fg;
	GtkWidget *button_important_fg;
	GtkWidget *button_directive_fg;
	GtkWidget *button_string_double_fg;
	GtkWidget *button_string_single_fg;
	GtkWidget *button_class_fg;
	GtkWidget *button_id_fg;
	GtkWidget *button_function_fg;
	GtkWidget *button_number_fg;
	GtkWidget *button_unit_fg;
	GtkWidget *button_color_fg;
	GtkWidget *button_hexacolor_fg;
	GtkWidget *button_attr_match_fg;
	GtkWidget *button_lang_fg;
	GtkWidget *label_default_bg;
	GtkWidget *label_tag_bg;
	GtkWidget *label_pseudoclass_bg;
	GtkWidget *label_unk_pseudoclass_bg;
	GtkWidget *label_operator_bg;
	//GtkWidget *label_comment_bg;
	GtkWidget *label_vproperty_bg;
	GtkWidget *label_unk_property_bg;
	GtkWidget *label_value_bg;
	GtkWidget *label_unk_value_bg;
	GtkWidget *label_important_bg;
	GtkWidget *label_directive_bg;
	GtkWidget *label_string_double_bg;
	GtkWidget *label_string_single_bg;
	GtkWidget *label_class_bg;
	GtkWidget *label_id_bg;
	GtkWidget *label_function_bg;
	GtkWidget *label_number_bg;
	GtkWidget *label_unit_bg;
	GtkWidget *label_color_bg;
	GtkWidget *label_hexacolor_bg;
	GtkWidget *label_attr_match_bg;
	GtkWidget *label_lang_bg;
	GtkWidget *label_default;
	GtkWidget *label_tag;
	GtkWidget *label_unk_pseudoclass;
	GtkWidget *label_operator;
	GtkWidget *label_vproperty;
	GtkWidget *label_unk_property;
	GtkWidget *label_value;
	GtkWidget *label_unk_value;
	GtkWidget *label_important;
	GtkWidget *label_directive;
	GtkWidget *checkbutton_default_bold;
	GtkWidget *checkbutton_tag_bold;
	//GtkWidget *checkbutton_comment_bold;
	GtkWidget *checkbutton_pseudoclass_bold;
	GtkWidget *checkbutton_unk_pseudoclass_bold;
	GtkWidget *checkbutton_operator_bold;
	GtkWidget *checkbutton_vproperty_bold;
	GtkWidget *checkbutton_unk_property_bold;
	GtkWidget *checkbutton_value_bold;
	GtkWidget *checkbutton_unk_value_bold;
	GtkWidget *checkbutton_important_bold;
	GtkWidget *checkbutton_directive_bold;
	GtkWidget *checkbutton_string_double_bold;
	GtkWidget *checkbutton_string_single_bold;
	GtkWidget *checkbutton_class_bold;
	GtkWidget *checkbutton_id_bold;
	GtkWidget *checkbutton_function_bold;
	GtkWidget *checkbutton_number_bold;
	GtkWidget *checkbutton_unit_bold;
	GtkWidget *checkbutton_color_bold;
	GtkWidget *checkbutton_hexacolor_bold;
	GtkWidget *checkbutton_attr_match_bold;
	GtkWidget *checkbutton_lang_bold;
	GtkWidget *button_tag_bg;
	GtkWidget *button_pseudoclass_bg;
	GtkWidget *button_unk_pseudoclass_bg;
	GtkWidget *button_operator_bg;
	GtkWidget *button_vproperty_bg;
	GtkWidget *button_unk_property_bg;
	GtkWidget *button_value_bg;
	GtkWidget *button_unk_value_bg;
	GtkWidget *button_important_bg;
	GtkWidget *button_directive_bg;
	GtkWidget *button_string_double_bg;
	GtkWidget *button_string_single_bg;
	GtkWidget *button_class_bg;
	GtkWidget *button_id_bg;
	GtkWidget *button_function_bg;
	GtkWidget *button_number_bg;
	GtkWidget *button_unit_bg;
	GtkWidget *button_color_bg;
	GtkWidget *button_hexacolor_bg;
	GtkWidget *button_attr_match_bg;
	GtkWidget *button_lang_bg;
	GtkWidget *label_string_double;
	GtkWidget *label_string_single;
	GtkWidget *label_class;
	GtkWidget *label_id;
	GtkWidget *label_function;
	GtkWidget *label_number;
	GtkWidget *label_unit;
	GtkWidget *label_color;
	GtkWidget *label_hexacolor;
	GtkWidget *label_attr_match;
	GtkWidget *label_lang;
	GtkWidget *label_pseudoclass;
	GtkWidget *button_default_fg;
	GtkWidget *label_tab_highlight;
	GtkWidget *button_comment_fg;
	GtkWidget *label_comment_bg;
	GtkWidget *button_comment_bg;
	GtkWidget *checkbutton_comment_bold;
	GtkWidget *label_comment;
	GtkWidget *table_defaults;
	GtkWidget *label_header_defaults;
	GtkWidget *checkbutton_line_numbers;
	GtkWidget *checkbutton_line_endings;
	GtkWidget *checkbutton_white_spaces;
	GtkWidget *checkbutton_line_wrap;
	GtkWidget *checkbutton_auto_completion;
	GtkWidget *checkbutton_folding;
	GtkWidget *checkbutton_usetabs;
	GtkWidget *spinbutton_tabswidth;
	GtkWidget *hbox_tabswidth;	
	GtkWidget *label_tabswidth;	
	GtkWidget *label_detail_defaults;
	GtkWidget *label_tab_doc_settings;
	GtkWidget* vbox_font;
	GtkWidget* hbox_current_and_apply;
	GtkWidget* label_current_info;
	GtkWidget* blank_label;
	GtkWidget* label_current;
	GtkWidget* apply_button;
	GtkWidget* label_font;
	GtkWidget* fontsel;
	GtkWidget* vbox_other_settings;
	GtkWidget* hbox_browser;	
	GtkWidget* hbox_label;	
	GtkWidget* entry_browser;
	GtkWidget* label_browser;
	GtkWidget* label_browser2;
	GtkWidget* button_browser_apply;
	GtkWidget* label_current_browser;
	GtkWidget* label_other;
	GtkWidget *dialog_action_area;
	GtkWidget *cancelbutton;
	GtkWidget *okbutton;
	GtkTooltips *tooltips;
	//GtkListStore *store;
	//////GtkTreeIter iter;
	CssedConfig* cfg;

	cfg = cssed_window_get_config( window );
	cssed_config_copy_style_and_defaults( cfg, tempconfig );
	
	tooltips = gtk_tooltips_new ();

	preferences_dialog = gtk_dialog_new ();
	gtk_widget_set_size_request (preferences_dialog, 600, 400);
	gtk_window_set_title (GTK_WINDOW (preferences_dialog), _("Preferences"));
	gtk_window_set_transient_for ( GTK_WINDOW(preferences_dialog),
				      GTK_WINDOW ( cssed_window_get_window_widget(window) ));

	dialog_vbox = GTK_DIALOG (preferences_dialog)->vbox;
	gtk_widget_show (dialog_vbox);

	notebook = gtk_notebook_new ();
	gtk_widget_show (notebook);
	GTK_WIDGET_UNSET_FLAGS (notebook, GTK_CAN_FOCUS);
	gtk_notebook_set_show_tabs (GTK_NOTEBOOK (notebook), TRUE);
	gtk_notebook_set_scrollable (GTK_NOTEBOOK (notebook), FALSE);
	gtk_box_pack_start (GTK_BOX (dialog_vbox), notebook, TRUE, TRUE, 0);
	
	/*  ************** document defaults *********************** */
	table_defaults = gtk_table_new (10, 1, FALSE);
	gtk_widget_show (table_defaults);
	gtk_container_add (GTK_CONTAINER (notebook), table_defaults);

	// This label is unused right now
	label_header_defaults =gtk_label_new ("");
	// label_header_defaults = gtk_label_new (_
	// ("<span size='small'><b><u>Document default settings</u></b></span>"));
	gtk_widget_show (label_header_defaults);
	gtk_table_attach (GTK_TABLE (table_defaults), label_header_defaults,
			  0, 1, 0, 1, (GtkAttachOptions) (GTK_FILL),
			  (GtkAttachOptions) (0), 5, 10);
	gtk_label_set_use_markup (GTK_LABEL (label_header_defaults), TRUE);
	gtk_label_set_justify (GTK_LABEL (label_header_defaults),
			       GTK_JUSTIFY_LEFT);
	gtk_misc_set_alignment (GTK_MISC (label_header_defaults), 0, 0.5);

	checkbutton_line_numbers =
		gtk_check_button_new_with_mnemonic (_("View line numbers"));
	gtk_widget_show (checkbutton_line_numbers);
	gtk_table_attach (GTK_TABLE (table_defaults),
			  checkbutton_line_numbers, 0, 1, 2, 3,
			  (GtkAttachOptions) (GTK_FILL),
			  (GtkAttachOptions) (0), 50, 2);
	gtk_tooltips_set_tip (tooltips, checkbutton_line_numbers,
			      _("check this if you want documents to be opened with line numbers visible."),
			      NULL);
	gtk_toggle_button_set_active( GTK_TOGGLE_BUTTON(checkbutton_line_numbers),
								  cfg->linenumbers );
								  
	checkbutton_line_endings =
		gtk_check_button_new_with_mnemonic (_("View line endings"));
	gtk_widget_show (checkbutton_line_endings);
	gtk_table_attach (GTK_TABLE (table_defaults),
			  checkbutton_line_endings, 0, 1, 3, 4,
			  (GtkAttachOptions) (GTK_FILL),
			  (GtkAttachOptions) (0), 50, 2);
	gtk_tooltips_set_tip (tooltips, checkbutton_line_endings,
			      _("check this if you want documents to be opened with line endings visible."),
			      NULL);
	gtk_toggle_button_set_active( GTK_TOGGLE_BUTTON(checkbutton_line_endings),
								  cfg->lineendings );

	checkbutton_white_spaces =
		gtk_check_button_new_with_mnemonic (_("View white spaces"));
	gtk_widget_show (checkbutton_white_spaces);
	gtk_table_attach (GTK_TABLE (table_defaults),
			  checkbutton_white_spaces, 0, 1, 4, 5,
			  (GtkAttachOptions) (GTK_FILL),
			  (GtkAttachOptions) (0), 50, 2);
	gtk_tooltips_set_tip (tooltips, checkbutton_white_spaces,
			      _("check this if you want documents to be opened with white spaces visible."),
			      NULL);
	gtk_toggle_button_set_active( GTK_TOGGLE_BUTTON(checkbutton_white_spaces),
								  cfg->whitespaces );

	checkbutton_line_wrap =
		gtk_check_button_new_with_mnemonic (_("Wrap long lines (\"view lines wrapped\" in the document menu)"));
	gtk_widget_show (checkbutton_line_wrap);
	gtk_table_attach (GTK_TABLE (table_defaults), checkbutton_line_wrap,
			  0, 1, 5, 6, (GtkAttachOptions) (GTK_FILL),
			  (GtkAttachOptions) (0), 50, 2);
	gtk_tooltips_set_tip (tooltips, checkbutton_line_wrap,
			      _("check this if you want the document text lines to wrap when they reach the editor right side"),
			      NULL);
	gtk_toggle_button_set_active( GTK_TOGGLE_BUTTON(checkbutton_line_wrap),
								  cfg->lineswraped );
								  
	checkbutton_auto_completion =
		gtk_check_button_new_with_mnemonic (_
						    ("Enable autocompletion"));
	gtk_widget_show (checkbutton_auto_completion);
	gtk_table_attach (GTK_TABLE (table_defaults),
			  checkbutton_auto_completion, 0, 1, 6, 7,
			  (GtkAttachOptions) (GTK_FILL),
			  (GtkAttachOptions) (0), 50, 2);
	gtk_tooltips_set_tip (tooltips, checkbutton_auto_completion,
				_("check this if you want documents to start with auto completion enabled"),
			      NULL);
	gtk_toggle_button_set_active( GTK_TOGGLE_BUTTON(checkbutton_auto_completion),
								  cfg->autocompletion );

	checkbutton_folding =
	gtk_check_button_new_with_mnemonic (_("Enable folding"));
	gtk_widget_show (checkbutton_folding);
	gtk_table_attach (GTK_TABLE (table_defaults), 
                    checkbutton_folding, 0, 1, 7, 8, 
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 50, 2);
	gtk_tooltips_set_tip (tooltips, checkbutton_folding,
			      _("check this if you want the documents to start with folding enabled by default"),
			      NULL);
	gtk_toggle_button_set_active( GTK_TOGGLE_BUTTON(checkbutton_folding),
								  cfg->folding );
	
	checkbutton_usetabs =
	gtk_check_button_new_with_mnemonic (_("Use tabs instead of white spaces"));
	gtk_widget_show (checkbutton_usetabs);
	gtk_table_attach (GTK_TABLE (table_defaults), 
                    checkbutton_usetabs, 0, 1, 8, 9, 
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 50, 2);
	gtk_tooltips_set_tip (tooltips, checkbutton_usetabs,
			      _("check this if you want to use the tab character instead of white spaces"),
			      NULL);
	gtk_toggle_button_set_active( GTK_TOGGLE_BUTTON(checkbutton_usetabs),
								  cfg->usetabs );
	
    hbox_tabswidth = gtk_hbox_new (FALSE, 0);
    gtk_widget_show (hbox_tabswidth);
    spinbutton_tabswidth = gtk_spin_button_new_with_range  (1, 15, 1);
    gtk_widget_show (spinbutton_tabswidth);
    label_tabswidth = gtk_label_new (_("Ammount of spaces on each tab"));
    gtk_widget_show (label_tabswidth);
    gtk_box_pack_start (GTK_BOX(hbox_tabswidth),  label_tabswidth, FALSE, FALSE, 5);
    gtk_box_pack_start (GTK_BOX(hbox_tabswidth),  spinbutton_tabswidth, FALSE, FALSE, 5);
	gtk_table_attach (GTK_TABLE (table_defaults), 
                    hbox_tabswidth, 0, 1, 9, 10, 
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 50, 2);	
    gtk_spin_button_set_value  (GTK_SPIN_BUTTON(spinbutton_tabswidth),
                                             (gdouble) cfg->tabswidth);         
                    

	label_detail_defaults =
		gtk_label_new (_
			       ("<span size='large'>Select here the document's default settings</span>"));
	gtk_widget_show (label_detail_defaults);
	gtk_table_attach (GTK_TABLE (table_defaults), label_detail_defaults,
			  0, 1, 1, 2,
			  (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
			  (GtkAttachOptions) (0), 5, 5);
	gtk_label_set_use_markup (GTK_LABEL (label_detail_defaults), TRUE);
	gtk_label_set_justify (GTK_LABEL (label_detail_defaults), GTK_JUSTIFY_LEFT);
	gtk_misc_set_alignment (GTK_MISC (label_detail_defaults), 0, 0.5);

	label_tab_doc_settings = gtk_label_new (_("<span size='medium'><b>Document settings</b></span>"));
	gtk_label_set_use_markup( GTK_LABEL(label_tab_doc_settings), TRUE);
	gtk_widget_show (label_tab_doc_settings);
	gtk_notebook_set_tab_label (GTK_NOTEBOOK (notebook),
				    gtk_notebook_get_nth_page (GTK_NOTEBOOK
							       (notebook), 0),
				    label_tab_doc_settings);
	gtk_label_set_justify (GTK_LABEL (label_tab_doc_settings),
			       GTK_JUSTIFY_LEFT);
	
	/* ********************** Highlighting ************************ */
			   
	scrolledwindow_highlighting = gtk_scrolled_window_new (NULL, NULL);
	gtk_widget_show (scrolledwindow_highlighting);
	gtk_container_add (GTK_CONTAINER (notebook),   scrolledwindow_highlighting);

	viewport_highlighting = gtk_viewport_new (NULL, NULL);
	gtk_widget_show (viewport_highlighting);
	gtk_container_add (GTK_CONTAINER (scrolledwindow_highlighting),
			   viewport_highlighting);
	gtk_container_set_border_width (GTK_CONTAINER (viewport_highlighting),
					5);
	gtk_viewport_set_shadow_type (GTK_VIEWPORT (viewport_highlighting),
				      GTK_SHADOW_NONE);

	table_highlighting = gtk_table_new (22, 6, FALSE);
	gtk_widget_show (table_highlighting);
	gtk_container_add (GTK_CONTAINER (viewport_highlighting),
			   table_highlighting);
	gtk_table_set_row_spacings (GTK_TABLE (table_highlighting), 6);
	gtk_table_set_col_spacings (GTK_TABLE (table_highlighting), 6);

	label_tag_fg = gtk_label_new (FOREGROUND_STRING);
	gtk_widget_show (label_tag_fg);
	gtk_table_attach (GTK_TABLE (table_highlighting), label_tag_fg, 1, 2,
			  1, 2, (GtkAttachOptions) (GTK_FILL),
			  (GtkAttachOptions) (0), 0, 0);
	gtk_label_set_justify (GTK_LABEL (label_tag_fg), GTK_JUSTIFY_LEFT);
	gtk_misc_set_alignment (GTK_MISC (label_tag_fg), 0, 0.5);

	label_pseudoclass_fg = gtk_label_new (FOREGROUND_STRING);
	gtk_widget_show (label_pseudoclass_fg);
	gtk_table_attach (GTK_TABLE (table_highlighting),
			  label_pseudoclass_fg, 1, 2, 2, 3,
			  (GtkAttachOptions) (GTK_FILL),
			  (GtkAttachOptions) (0), 0, 0);
	gtk_label_set_justify (GTK_LABEL (label_pseudoclass_fg),
			       GTK_JUSTIFY_LEFT);
	gtk_misc_set_alignment (GTK_MISC (label_pseudoclass_fg), 0, 0.5);

	label_unk_pseudoclass_fg = gtk_label_new (FOREGROUND_STRING);
	gtk_widget_show (label_unk_pseudoclass_fg);
	gtk_table_attach (GTK_TABLE (table_highlighting),
			  label_unk_pseudoclass_fg, 1, 2, 3, 4,
			  (GtkAttachOptions) (GTK_FILL),
			  (GtkAttachOptions) (0), 0, 0);
	gtk_label_set_justify (GTK_LABEL (label_unk_pseudoclass_fg),
			       GTK_JUSTIFY_LEFT);
	gtk_misc_set_alignment (GTK_MISC (label_unk_pseudoclass_fg), 0, 0.5);

	label_operator_fg = gtk_label_new (FOREGROUND_STRING);
	gtk_widget_show (label_operator_fg);
	gtk_table_attach (GTK_TABLE (table_highlighting), label_operator_fg,
			  1, 2, 4, 5, (GtkAttachOptions) (GTK_FILL),
			  (GtkAttachOptions) (0), 0, 0);
	gtk_label_set_justify (GTK_LABEL (label_operator_fg),
			       GTK_JUSTIFY_LEFT);
	gtk_misc_set_alignment (GTK_MISC (label_operator_fg), 0, 0.5);

	label_vproperty_fg = gtk_label_new (FOREGROUND_STRING);
	gtk_widget_show (label_vproperty_fg);
	gtk_table_attach (GTK_TABLE (table_highlighting), label_vproperty_fg,
			  1, 2, 5, 6, (GtkAttachOptions) (GTK_FILL),
			  (GtkAttachOptions) (0), 0, 0);
	gtk_label_set_justify (GTK_LABEL (label_vproperty_fg),
			       GTK_JUSTIFY_LEFT);
	gtk_misc_set_alignment (GTK_MISC (label_vproperty_fg), 0, 0.5);

	label_unk_property_fg = gtk_label_new (FOREGROUND_STRING);
	gtk_widget_show (label_unk_property_fg);
	gtk_table_attach (GTK_TABLE (table_highlighting),
			  label_unk_property_fg, 1, 2, 6, 7,
			  (GtkAttachOptions) (GTK_FILL),
			  (GtkAttachOptions) (0), 0, 0);
	gtk_label_set_justify (GTK_LABEL (label_unk_property_fg),
			       GTK_JUSTIFY_LEFT);
	gtk_misc_set_alignment (GTK_MISC (label_unk_property_fg), 0, 0.5);

	label_value_fg = gtk_label_new (FOREGROUND_STRING);
	gtk_widget_show (label_value_fg);
	gtk_table_attach (GTK_TABLE (table_highlighting), label_value_fg, 1,
			  2, 7, 8, (GtkAttachOptions) (GTK_FILL),
			  (GtkAttachOptions) (0), 0, 0);
	gtk_label_set_justify (GTK_LABEL (label_value_fg), GTK_JUSTIFY_LEFT);
	gtk_misc_set_alignment (GTK_MISC (label_value_fg), 0, 0.5);

	label_unk_value_fg = gtk_label_new (FOREGROUND_STRING);
	gtk_widget_show (label_unk_value_fg);
	gtk_table_attach (GTK_TABLE (table_highlighting), label_unk_value_fg,
			  1, 2, 8, 9, (GtkAttachOptions) (GTK_FILL),
			  (GtkAttachOptions) (0), 0, 0);
	gtk_label_set_justify (GTK_LABEL (label_unk_value_fg),
			       GTK_JUSTIFY_LEFT);
	gtk_misc_set_alignment (GTK_MISC (label_unk_value_fg), 0, 0.5);

	label_default_fg = gtk_label_new (FOREGROUND_STRING);
	gtk_widget_show (label_default_fg);
	gtk_table_attach (GTK_TABLE (table_highlighting), label_default_fg, 1,
			  2, 0, 1, (GtkAttachOptions) (GTK_FILL),
			  (GtkAttachOptions) (0), 0, 0);
	gtk_label_set_justify (GTK_LABEL (label_default_fg),
			       GTK_JUSTIFY_LEFT);
	gtk_misc_set_alignment (GTK_MISC (label_default_fg), 0, 0.5);

	label_important_fg = gtk_label_new (FOREGROUND_STRING);
	gtk_widget_show (label_important_fg);
	gtk_table_attach (GTK_TABLE (table_highlighting), label_important_fg,
			  1, 2, 9, 10, (GtkAttachOptions) (GTK_FILL),
			  (GtkAttachOptions) (0), 0, 0);
	gtk_label_set_justify (GTK_LABEL (label_important_fg),
			       GTK_JUSTIFY_LEFT);
	gtk_misc_set_alignment (GTK_MISC (label_important_fg), 0, 0.5);

	label_directive_fg = gtk_label_new (FOREGROUND_STRING);
	gtk_widget_show (label_directive_fg);
	gtk_table_attach (GTK_TABLE (table_highlighting), label_directive_fg,
			  1, 2, 10, 11, (GtkAttachOptions) (GTK_FILL),
			  (GtkAttachOptions) (0), 0, 0);
	gtk_label_set_justify (GTK_LABEL (label_directive_fg),
			       GTK_JUSTIFY_LEFT);
	gtk_misc_set_alignment (GTK_MISC (label_directive_fg), 0, 0.5);

	label_string_double_fg = gtk_label_new (FOREGROUND_STRING);
	gtk_widget_show (label_string_double_fg);
	gtk_table_attach (GTK_TABLE (table_highlighting),
			  label_string_double_fg, 1, 2, 11, 12,
			  (GtkAttachOptions) (GTK_FILL),
			  (GtkAttachOptions) (0), 0, 0);
	gtk_label_set_justify (GTK_LABEL (label_string_double_fg),
			       GTK_JUSTIFY_LEFT);
	gtk_misc_set_alignment (GTK_MISC (label_string_double_fg), 0, 0.5);

	label_string_single_fg = gtk_label_new (FOREGROUND_STRING);
	gtk_widget_show (label_string_single_fg);
	gtk_table_attach (GTK_TABLE (table_highlighting),
			  label_string_single_fg, 1, 2, 12, 13,
			  (GtkAttachOptions) (GTK_FILL),
			  (GtkAttachOptions) (0), 0, 0);
	gtk_label_set_justify (GTK_LABEL (label_string_single_fg),
			       GTK_JUSTIFY_LEFT);
	gtk_misc_set_alignment (GTK_MISC (label_string_single_fg), 0, 0.5);

	label_class_fg = gtk_label_new (FOREGROUND_STRING);
	gtk_widget_show (label_class_fg);
	gtk_table_attach (GTK_TABLE (table_highlighting), label_class_fg, 1,
			  2, 13, 14, (GtkAttachOptions) (GTK_FILL),
			  (GtkAttachOptions) (0), 0, 0);
	gtk_label_set_justify (GTK_LABEL (label_class_fg), GTK_JUSTIFY_LEFT);
	gtk_misc_set_alignment (GTK_MISC (label_class_fg), 0, 0.5);

	label_id_fg = gtk_label_new (FOREGROUND_STRING);
	gtk_widget_show (label_id_fg);
	gtk_table_attach (GTK_TABLE (table_highlighting), label_id_fg, 1, 2,
			  14, 15, (GtkAttachOptions) (GTK_FILL),
			  (GtkAttachOptions) (0), 0, 0);
	gtk_label_set_justify (GTK_LABEL (label_id_fg), GTK_JUSTIFY_LEFT);
	gtk_misc_set_alignment (GTK_MISC (label_id_fg), 0, 0.5);

	label_lang_fg = gtk_label_new (FOREGROUND_STRING);
	gtk_widget_show (label_lang_fg);
	gtk_table_attach (GTK_TABLE (table_highlighting), label_lang_fg, 1, 2,
			  21, 22, (GtkAttachOptions) (GTK_FILL),
			  (GtkAttachOptions) (0), 0, 0);
	gtk_label_set_justify (GTK_LABEL (label_lang_fg), GTK_JUSTIFY_LEFT);
	gtk_misc_set_alignment (GTK_MISC (label_lang_fg), 0, 0.5);

	label_attr_match_fg = gtk_label_new (FOREGROUND_STRING);
	gtk_widget_show (label_attr_match_fg);
	gtk_table_attach (GTK_TABLE (table_highlighting), label_attr_match_fg,
			  1, 2, 20, 21, (GtkAttachOptions) (GTK_FILL),
			  (GtkAttachOptions) (0), 0, 0);
	gtk_label_set_justify (GTK_LABEL (label_attr_match_fg),
			       GTK_JUSTIFY_LEFT);
	gtk_misc_set_alignment (GTK_MISC (label_attr_match_fg), 0, 0.5);

	label_hexacolor_fg = gtk_label_new (FOREGROUND_STRING);
	gtk_widget_show (label_hexacolor_fg);
	gtk_table_attach (GTK_TABLE (table_highlighting), label_hexacolor_fg,
			  1, 2, 19, 20, (GtkAttachOptions) (GTK_FILL),
			  (GtkAttachOptions) (0), 0, 0);
	gtk_label_set_justify (GTK_LABEL (label_hexacolor_fg),
			       GTK_JUSTIFY_LEFT);
	gtk_misc_set_alignment (GTK_MISC (label_hexacolor_fg), 0, 0.5);

	label_color_fg = gtk_label_new (FOREGROUND_STRING);
	gtk_widget_show (label_color_fg);
	gtk_table_attach (GTK_TABLE (table_highlighting), label_color_fg, 1,
			  2, 18, 19, (GtkAttachOptions) (GTK_FILL),
			  (GtkAttachOptions) (0), 0, 0);
	gtk_label_set_justify (GTK_LABEL (label_color_fg), GTK_JUSTIFY_LEFT);
	gtk_misc_set_alignment (GTK_MISC (label_color_fg), 0, 0.5);

	label_unit_fg = gtk_label_new (FOREGROUND_STRING);
	gtk_widget_show (label_unit_fg);
	gtk_table_attach (GTK_TABLE (table_highlighting), label_unit_fg, 1, 2,
			  17, 18, (GtkAttachOptions) (GTK_FILL),
			  (GtkAttachOptions) (0), 0, 0);
	gtk_label_set_justify (GTK_LABEL (label_unit_fg), GTK_JUSTIFY_LEFT);
	gtk_misc_set_alignment (GTK_MISC (label_unit_fg), 0, 0.5);

	label_number_fg = gtk_label_new (FOREGROUND_STRING);
	gtk_widget_show (label_number_fg);
	gtk_table_attach (GTK_TABLE (table_highlighting), label_number_fg, 1,
			  2, 16, 17, (GtkAttachOptions) (GTK_FILL),
			  (GtkAttachOptions) (0), 0, 0);
	gtk_label_set_justify (GTK_LABEL (label_number_fg), GTK_JUSTIFY_LEFT);
	gtk_misc_set_alignment (GTK_MISC (label_number_fg), 0, 0.5);

	label_function_fg = gtk_label_new (FOREGROUND_STRING);
	gtk_widget_show (label_function_fg);
	gtk_table_attach (GTK_TABLE (table_highlighting), label_function_fg,
			  1, 2, 15, 16, (GtkAttachOptions) (GTK_FILL),
			  (GtkAttachOptions) (0), 0, 0);
	gtk_label_set_justify (GTK_LABEL (label_function_fg),
			       GTK_JUSTIFY_LEFT);
	gtk_misc_set_alignment (GTK_MISC (label_function_fg), 0, 0.5);
	
	// ************************** BUTTONS FIRST COL ****************************
	button_vproperty_fg = gtk_button_new ();
	attach_colored_label_to_button( button_vproperty_fg, cfg->IDENTIFIER_fore_color);
	gtk_widget_show (button_vproperty_fg);
	gtk_table_attach (GTK_TABLE (table_highlighting), button_vproperty_fg,
			  2, 3, 5, 6, (GtkAttachOptions) (GTK_FILL),
			  (GtkAttachOptions) (0), 5, 0);
	gtk_container_set_border_width (GTK_CONTAINER (button_vproperty_fg),
					1);
	//gtk_tooltips_set_tip (tooltips, button_vproperty_fg, _("click here to set the foreground color for this style"), NULL);

	button_operator_fg = gtk_button_new();
	attach_colored_label_to_button( button_operator_fg, cfg->OPERATOR_fore_color);
	gtk_widget_show (button_operator_fg);
	gtk_table_attach (GTK_TABLE (table_highlighting), button_operator_fg,
			  2, 3, 4, 5, (GtkAttachOptions) (GTK_FILL),
			  (GtkAttachOptions) (0), 5, 0);
	gtk_container_set_border_width (GTK_CONTAINER (button_operator_fg),
					1);
	//gtk_tooltips_set_tip (tooltips, button_operator_fg, _("click here to set the foreground color for this style"), NULL);

	button_unk_pseudoclass_fg = 	gtk_button_new();
	attach_colored_label_to_button( button_unk_pseudoclass_fg, 
										cfg->UNKNOWN_PSEUDOCLASS_fore_color);
	gtk_widget_show (button_unk_pseudoclass_fg);
	gtk_table_attach (GTK_TABLE (table_highlighting),
			  button_unk_pseudoclass_fg, 2, 3, 3, 4,
			  (GtkAttachOptions) (GTK_FILL),
			  (GtkAttachOptions) (0), 5, 0);
	gtk_container_set_border_width (GTK_CONTAINER
					(button_unk_pseudoclass_fg), 1);
	//gtk_tooltips_set_tip (tooltips, button_unk_pseudoclass_fg, _("click here to set the foreground color for this style"), NULL);

	button_pseudoclass_fg = gtk_button_new();
	attach_colored_label_to_button( button_pseudoclass_fg, 
										cfg->PSEUDOCLASS_fore_color);
	gtk_widget_show (button_pseudoclass_fg);
	gtk_table_attach (GTK_TABLE (table_highlighting),
			  button_pseudoclass_fg, 2, 3, 2, 3,
			  (GtkAttachOptions) (GTK_FILL),
			  (GtkAttachOptions) (0), 5, 0);
	gtk_container_set_border_width (GTK_CONTAINER (button_pseudoclass_fg),
					1);
	//gtk_tooltips_set_tip (tooltips, button_pseudoclass_fg, _("click here to set the foreground color for this style"), NULL);

	button_tag_fg = gtk_button_new();
	attach_colored_label_to_button( button_tag_fg, 
										cfg->TAG_fore_color);	
	gtk_widget_show (button_tag_fg);
	gtk_table_attach (GTK_TABLE (table_highlighting), button_tag_fg, 2, 3,
			  1, 2, (GtkAttachOptions) (GTK_FILL),
			  (GtkAttachOptions) (0), 5, 0);
	gtk_container_set_border_width (GTK_CONTAINER (button_tag_fg), 1);
	//gtk_tooltips_set_tip (tooltips, button_tag_fg, _("click here to set the foreground color for this style"), NULL);

	button_default_bg = gtk_button_new();
	attach_colored_label_to_button( button_default_bg, 
										cfg->DEFAULT_back_color);	
	gtk_widget_show (button_default_bg);
	gtk_table_attach (GTK_TABLE (table_highlighting), button_default_bg,
			  4, 5, 0, 1, (GtkAttachOptions) (0),
			  (GtkAttachOptions) (0), 5, 0);
	gtk_container_set_border_width (GTK_CONTAINER (button_default_bg), 1);

	button_unk_property_fg= gtk_button_new();
	attach_colored_label_to_button( button_unk_property_fg, 
										cfg->UNKNOWN_IDENTIFIER_fore_color);
	gtk_widget_show (button_unk_property_fg);
	gtk_table_attach (GTK_TABLE (table_highlighting),
			  button_unk_property_fg, 2, 3, 6, 7,
			  (GtkAttachOptions) (GTK_FILL),
			  (GtkAttachOptions) (0), 5, 0);
	gtk_container_set_border_width (GTK_CONTAINER
					(button_unk_property_fg), 1);
	//gtk_tooltips_set_tip (tooltips, button_unk_property_fg, _("click here to set the foreground color for this style"), NULL);

	button_value_fg = gtk_button_new();
	attach_colored_label_to_button( button_value_fg, cfg->VALID_VALUE_fore_color);
	gtk_widget_show (button_value_fg);
	gtk_table_attach (GTK_TABLE (table_highlighting), button_value_fg, 2,
			  3, 7, 8, (GtkAttachOptions) (GTK_FILL),
			  (GtkAttachOptions) (0), 5, 0);
	gtk_container_set_border_width (GTK_CONTAINER (button_value_fg), 1);
	//gtk_tooltips_set_tip (tooltips, button_value_fg, _("click here to set the foreground color for this style"), NULL);

	button_unk_value_fg = gtk_button_new();
	attach_colored_label_to_button( button_unk_value_fg, cfg->VALUE_fore_color);
	gtk_widget_show (button_unk_value_fg);
	gtk_table_attach (GTK_TABLE (table_highlighting), button_unk_value_fg,
			  2, 3, 8, 9, (GtkAttachOptions) (GTK_FILL),
			  (GtkAttachOptions) (0), 5, 0);
	gtk_container_set_border_width (GTK_CONTAINER (button_unk_value_fg),
					1);
	//gtk_tooltips_set_tip (tooltips, button_unk_value_fg, _("click here to set the foreground color for this style"), NULL);

	button_important_fg = gtk_button_new();
	attach_colored_label_to_button( button_important_fg, cfg->IMPORTANT_fore_color);
	gtk_widget_show (button_important_fg);
	gtk_table_attach (GTK_TABLE (table_highlighting), button_important_fg,
			  2, 3, 9, 10, (GtkAttachOptions) (GTK_FILL),
			  (GtkAttachOptions) (0), 5, 0);
	gtk_container_set_border_width (GTK_CONTAINER (button_important_fg),
					1);
	//gtk_tooltips_set_tip (tooltips, button_important_fg, _("click here to set the foreground color for this style"), NULL);

	button_directive_fg = gtk_button_new();
	attach_colored_label_to_button( button_directive_fg, cfg->DIRECTIVE_fore_color);
	gtk_widget_show (button_directive_fg);
	gtk_table_attach (GTK_TABLE (table_highlighting), button_directive_fg,
			  2, 3, 10, 11, (GtkAttachOptions) (GTK_FILL),
			  (GtkAttachOptions) (0), 5, 0);
	gtk_container_set_border_width (GTK_CONTAINER (button_directive_fg),
					1);
	//gtk_tooltips_set_tip (tooltips, button_directive_fg, _("click here to set the foreground color for this style"), NULL);

	button_string_double_fg =  gtk_button_new();
	attach_colored_label_to_button( button_string_double_fg, cfg->DOUBLESTRING_fore_color);
	gtk_widget_show (button_string_double_fg);
	gtk_table_attach (GTK_TABLE (table_highlighting),
			  button_string_double_fg, 2, 3, 11, 12,
			  (GtkAttachOptions) (GTK_FILL),
			  (GtkAttachOptions) (0), 5, 0);
	gtk_container_set_border_width (GTK_CONTAINER
					(button_string_double_fg), 1);
	//gtk_tooltips_set_tip (tooltips, button_string_double_fg, _("click here to set the foreground color for this style"), NULL);

	button_string_single_fg=  gtk_button_new();
	attach_colored_label_to_button( button_string_single_fg, cfg->SINGLESTRING_fore_color);
	gtk_widget_show (button_string_single_fg);
	gtk_table_attach (GTK_TABLE (table_highlighting),
			  button_string_single_fg, 2, 3, 12, 13,
			  (GtkAttachOptions) (GTK_FILL),
			  (GtkAttachOptions) (0), 5, 0);
	gtk_container_set_border_width (GTK_CONTAINER
					(button_string_single_fg), 1);
	//gtk_tooltips_set_tip (tooltips, button_string_single_fg, _("click here to set the foreground color for this style"), NULL);

	button_class_fg =  gtk_button_new();
	attach_colored_label_to_button( button_class_fg, cfg->CLASS_fore_color);
	gtk_widget_show (button_class_fg);
	gtk_table_attach (GTK_TABLE (table_highlighting), button_class_fg, 2,
			  3, 13, 14, (GtkAttachOptions) (GTK_FILL),
			  (GtkAttachOptions) (0), 5, 0);
	gtk_container_set_border_width (GTK_CONTAINER (button_class_fg), 1);
	//gtk_tooltips_set_tip (tooltips, button_class_fg, _("click here to set the foreground color for this style"), NULL);

	button_id_fg =  gtk_button_new();
	attach_colored_label_to_button( button_id_fg, cfg->ID_fore_color);
	gtk_widget_show (button_id_fg);
	gtk_table_attach (GTK_TABLE (table_highlighting), button_id_fg, 2, 3,
			  14, 15, (GtkAttachOptions) (GTK_FILL),
			  (GtkAttachOptions) (0), 5, 0);
	gtk_container_set_border_width (GTK_CONTAINER (button_id_fg), 1);
	//gtk_tooltips_set_tip (tooltips, button_id_fg, _("click here to set the foreground color for this style"), NULL);

	button_function_fg =  gtk_button_new();
	attach_colored_label_to_button( button_function_fg, cfg->FUNCTION_fore_color);
	gtk_widget_show (button_function_fg);
	gtk_table_attach (GTK_TABLE (table_highlighting), button_function_fg,
			  2, 3, 15, 16, (GtkAttachOptions) (GTK_FILL),
			  (GtkAttachOptions) (0), 5, 0);
	gtk_container_set_border_width (GTK_CONTAINER (button_function_fg),
					1);
	//gtk_tooltips_set_tip (tooltips, button_function_fg, _("click here to set the foreground color for this style"), NULL);

	button_number_fg =  gtk_button_new();
	attach_colored_label_to_button( button_number_fg, cfg->NUMBER_fore_color);
	gtk_widget_show (button_number_fg);
	gtk_table_attach (GTK_TABLE (table_highlighting), button_number_fg, 2,
			  3, 16, 17, (GtkAttachOptions) (GTK_FILL),
			  (GtkAttachOptions) (0), 5, 0);
	gtk_container_set_border_width (GTK_CONTAINER (button_number_fg), 1);
	//gtk_tooltips_set_tip (tooltips, button_number_fg, _("click here to set the foreground color for this style"), NULL);

	button_unit_fg =  gtk_button_new();
	attach_colored_label_to_button( button_unit_fg, cfg->UNIT_fore_color);
	gtk_widget_show (button_unit_fg);
	gtk_table_attach (GTK_TABLE (table_highlighting), button_unit_fg, 2,
			  3, 17, 18, (GtkAttachOptions) (GTK_FILL),
			  (GtkAttachOptions) (0), 5, 0);
	gtk_container_set_border_width (GTK_CONTAINER (button_unit_fg), 1);
	//gtk_tooltips_set_tip (tooltips, button_unit_fg, _("click here to set the foreground color for this style"), NULL);

	button_color_fg =  gtk_button_new();
	attach_colored_label_to_button( button_color_fg, cfg->COLOR_fore_color);
	gtk_widget_show (button_color_fg);
	gtk_table_attach (GTK_TABLE (table_highlighting), button_color_fg, 2,
			  3, 18, 19, (GtkAttachOptions) (GTK_FILL),
			  (GtkAttachOptions) (0), 5, 0);
	gtk_container_set_border_width (GTK_CONTAINER (button_color_fg), 1);
	//gtk_tooltips_set_tip (tooltips, button_color_fg, _("click here to set the foreground color for this style"), NULL);

	button_hexacolor_fg =  gtk_button_new();
	attach_colored_label_to_button( button_hexacolor_fg, cfg->HEXACOLOR_fore_color);
	gtk_widget_show (button_hexacolor_fg);
	gtk_table_attach (GTK_TABLE (table_highlighting), button_hexacolor_fg,
			  2, 3, 19, 20, (GtkAttachOptions) (GTK_FILL),
			  (GtkAttachOptions) (0), 5, 0);
	gtk_container_set_border_width (GTK_CONTAINER (button_hexacolor_fg),
					1);
	//gtk_tooltips_set_tip (tooltips, button_hexacolor_fg, _("click here to set the foreground color for this style"), NULL);

	button_attr_match_fg =  gtk_button_new();
	attach_colored_label_to_button( button_attr_match_fg, cfg->ATTR_MATCH_fore_color);
	gtk_widget_show (button_attr_match_fg);
	gtk_table_attach (GTK_TABLE (table_highlighting),
			  button_attr_match_fg, 2, 3, 20, 21,
			  (GtkAttachOptions) (GTK_FILL),
			  (GtkAttachOptions) (0), 5, 0);
	gtk_container_set_border_width (GTK_CONTAINER (button_attr_match_fg),
					1);
	//gtk_tooltips_set_tip (tooltips, button_attr_match_fg, _("click here to set the foreground color for this style"), NULL);

	button_lang_fg =  gtk_button_new();
	attach_colored_label_to_button( button_lang_fg, cfg->LANGUAGE_fore_color);
	gtk_widget_show (button_lang_fg);
	gtk_table_attach (GTK_TABLE (table_highlighting), button_lang_fg, 2,
			  3, 21, 22, (GtkAttachOptions) (GTK_FILL),
			  (GtkAttachOptions) (0), 5, 0);
	gtk_container_set_border_width (GTK_CONTAINER (button_lang_fg), 1);
	//gtk_tooltips_set_tip (tooltips, button_lang_fg, _("click here to set the foreground color for this style"), NULL);

	button_default_fg =  gtk_button_new();
	attach_colored_label_to_button( button_default_fg, cfg->DEFAULT_fore_color);
	gtk_widget_show (button_default_fg);
	gtk_table_attach (GTK_TABLE (table_highlighting), button_default_fg,
			  2, 3, 0, 1, (GtkAttachOptions) (0),
			  (GtkAttachOptions) (0), 5, 1);
	//gtk_tooltips_set_tip (tooltips, button_default_fg,
			     // _
			     // ("click here to set the foreground color for this style"),
			     // NULL);
				  
	label_default_bg = gtk_label_new (BACKGROUND_STRING);
	gtk_widget_show (label_default_bg);
	gtk_table_attach (GTK_TABLE (table_highlighting), label_default_bg, 3,
			  4, 0, 1, (GtkAttachOptions) (GTK_FILL),
			  (GtkAttachOptions) (0), 0, 0);
	gtk_label_set_justify (GTK_LABEL (label_default_bg),
			       GTK_JUSTIFY_LEFT);
	gtk_misc_set_alignment (GTK_MISC (label_default_bg), 0, 0.5);

	label_tag_bg = gtk_label_new (BACKGROUND_STRING);
	gtk_widget_show (label_tag_bg);
	gtk_table_attach (GTK_TABLE (table_highlighting), label_tag_bg, 3, 4,
			  1, 2, (GtkAttachOptions) (GTK_FILL),
			  (GtkAttachOptions) (0), 0, 0);
	gtk_label_set_justify (GTK_LABEL (label_tag_bg), GTK_JUSTIFY_LEFT);
	gtk_misc_set_alignment (GTK_MISC (label_tag_bg), 0, 0.5);

	label_pseudoclass_bg = gtk_label_new (BACKGROUND_STRING);
	gtk_widget_show (label_pseudoclass_bg);
	gtk_table_attach (GTK_TABLE (table_highlighting),
			  label_pseudoclass_bg, 3, 4, 2, 3,
			  (GtkAttachOptions) (GTK_FILL),
			  (GtkAttachOptions) (0), 0, 0);
	gtk_label_set_justify (GTK_LABEL (label_pseudoclass_bg),
			       GTK_JUSTIFY_LEFT);
	gtk_misc_set_alignment (GTK_MISC (label_pseudoclass_bg), 0, 0.5);

	label_unk_pseudoclass_bg = gtk_label_new (BACKGROUND_STRING);
	gtk_widget_show (label_unk_pseudoclass_bg);
	gtk_table_attach (GTK_TABLE (table_highlighting),
			  label_unk_pseudoclass_bg, 3, 4, 3, 4,
			  (GtkAttachOptions) (GTK_FILL),
			  (GtkAttachOptions) (0), 0, 0);
	gtk_label_set_justify (GTK_LABEL (label_unk_pseudoclass_bg),
			       GTK_JUSTIFY_LEFT);
	gtk_misc_set_alignment (GTK_MISC (label_unk_pseudoclass_bg), 0, 0.5);

	label_operator_bg = gtk_label_new (BACKGROUND_STRING);
	gtk_widget_show (label_operator_bg);
	gtk_table_attach (GTK_TABLE (table_highlighting), label_operator_bg,
			  3, 4, 4, 5, (GtkAttachOptions) (GTK_FILL),
			  (GtkAttachOptions) (0), 0, 0);
	gtk_label_set_justify (GTK_LABEL (label_operator_bg),
			       GTK_JUSTIFY_LEFT);
	gtk_misc_set_alignment (GTK_MISC (label_operator_bg), 0, 0.5);

	label_vproperty_bg = gtk_label_new (BACKGROUND_STRING);
	gtk_widget_show (label_vproperty_bg);
	gtk_table_attach (GTK_TABLE (table_highlighting), label_vproperty_bg,
			  3, 4, 5, 6, (GtkAttachOptions) (GTK_FILL),
			  (GtkAttachOptions) (0), 0, 0);
	gtk_label_set_justify (GTK_LABEL (label_vproperty_bg),
			       GTK_JUSTIFY_LEFT);
	gtk_misc_set_alignment (GTK_MISC (label_vproperty_bg), 0, 0.5);

	label_unk_property_bg = gtk_label_new (BACKGROUND_STRING);
	gtk_widget_show (label_unk_property_bg);
	gtk_table_attach (GTK_TABLE (table_highlighting),
			  label_unk_property_bg, 3, 4, 6, 7,
			  (GtkAttachOptions) (GTK_FILL),
			  (GtkAttachOptions) (0), 0, 0);
	gtk_label_set_justify (GTK_LABEL (label_unk_property_bg),
			       GTK_JUSTIFY_LEFT);
	gtk_misc_set_alignment (GTK_MISC (label_unk_property_bg), 0, 0.5);

	label_value_bg = gtk_label_new (BACKGROUND_STRING);
	gtk_widget_show (label_value_bg);
	gtk_table_attach (GTK_TABLE (table_highlighting), label_value_bg, 3,
			  4, 7, 8, (GtkAttachOptions) (GTK_FILL),
			  (GtkAttachOptions) (0), 0, 0);
	gtk_label_set_justify (GTK_LABEL (label_value_bg), GTK_JUSTIFY_LEFT);
	gtk_misc_set_alignment (GTK_MISC (label_value_bg), 0, 0.5);

	label_unk_value_bg = gtk_label_new (BACKGROUND_STRING);
	gtk_widget_show (label_unk_value_bg);
	gtk_table_attach (GTK_TABLE (table_highlighting), label_unk_value_bg,
			  3, 4, 8, 9, (GtkAttachOptions) (GTK_FILL),
			  (GtkAttachOptions) (0), 0, 0);
	gtk_label_set_justify (GTK_LABEL (label_unk_value_bg),
			       GTK_JUSTIFY_LEFT);
	gtk_misc_set_alignment (GTK_MISC (label_unk_value_bg), 0, 0.5);

	label_important_bg = gtk_label_new (BACKGROUND_STRING);
	gtk_widget_show (label_important_bg);
	gtk_table_attach (GTK_TABLE (table_highlighting), label_important_bg,
			  3, 4, 9, 10, (GtkAttachOptions) (GTK_FILL),
			  (GtkAttachOptions) (0), 0, 0);
	gtk_label_set_justify (GTK_LABEL (label_important_bg),
			       GTK_JUSTIFY_LEFT);
	gtk_misc_set_alignment (GTK_MISC (label_important_bg), 0, 0.5);

	label_directive_bg = gtk_label_new (BACKGROUND_STRING);
	gtk_widget_show (label_directive_bg);
	gtk_table_attach (GTK_TABLE (table_highlighting), label_directive_bg,
			  3, 4, 10, 11, (GtkAttachOptions) (GTK_FILL),
			  (GtkAttachOptions) (0), 0, 0);
	gtk_label_set_justify (GTK_LABEL (label_directive_bg),
			       GTK_JUSTIFY_LEFT);
	gtk_misc_set_alignment (GTK_MISC (label_directive_bg), 0, 0.5);

	label_string_double_bg = gtk_label_new (BACKGROUND_STRING);
	gtk_widget_show (label_string_double_bg);
	gtk_table_attach (GTK_TABLE (table_highlighting),
			  label_string_double_bg, 3, 4, 11, 12,
			  (GtkAttachOptions) (GTK_FILL),
			  (GtkAttachOptions) (0), 0, 0);
	gtk_label_set_justify (GTK_LABEL (label_string_double_bg),
			       GTK_JUSTIFY_LEFT);
	gtk_misc_set_alignment (GTK_MISC (label_string_double_bg), 0, 0.5);

	label_string_single_bg = gtk_label_new (BACKGROUND_STRING);
	gtk_widget_show (label_string_single_bg);
	gtk_table_attach (GTK_TABLE (table_highlighting),
			  label_string_single_bg, 3, 4, 12, 13,
			  (GtkAttachOptions) (GTK_FILL),
			  (GtkAttachOptions) (0), 0, 0);
	gtk_label_set_justify (GTK_LABEL (label_string_single_bg),
			       GTK_JUSTIFY_LEFT);
	gtk_misc_set_alignment (GTK_MISC (label_string_single_bg), 0, 0.5);

	label_class_bg = gtk_label_new (BACKGROUND_STRING);
	gtk_widget_show (label_class_bg);
	gtk_table_attach (GTK_TABLE (table_highlighting), label_class_bg, 3,
			  4, 13, 14, (GtkAttachOptions) (GTK_FILL),
			  (GtkAttachOptions) (0), 0, 0);
	gtk_label_set_justify (GTK_LABEL (label_class_bg), GTK_JUSTIFY_LEFT);
	gtk_misc_set_alignment (GTK_MISC (label_class_bg), 0, 0.5);

	label_id_bg = gtk_label_new (BACKGROUND_STRING);
	gtk_widget_show (label_id_bg);
	gtk_table_attach (GTK_TABLE (table_highlighting), label_id_bg, 3, 4,
			  14, 15, (GtkAttachOptions) (GTK_FILL),
			  (GtkAttachOptions) (0), 0, 0);
	gtk_label_set_justify (GTK_LABEL (label_id_bg), GTK_JUSTIFY_LEFT);
	gtk_misc_set_alignment (GTK_MISC (label_id_bg), 0, 0.5);

	label_function_bg = gtk_label_new (BACKGROUND_STRING);
	gtk_widget_show (label_function_bg);
	gtk_table_attach (GTK_TABLE (table_highlighting), label_function_bg,
			  3, 4, 15, 16, (GtkAttachOptions) (GTK_FILL),
			  (GtkAttachOptions) (0), 0, 0);
	gtk_label_set_justify (GTK_LABEL (label_function_bg),
			       GTK_JUSTIFY_LEFT);
	gtk_misc_set_alignment (GTK_MISC (label_function_bg), 0, 0.5);

	label_number_bg = gtk_label_new (BACKGROUND_STRING);
	gtk_widget_show (label_number_bg);
	gtk_table_attach (GTK_TABLE (table_highlighting), label_number_bg, 3,
			  4, 16, 17, (GtkAttachOptions) (GTK_FILL),
			  (GtkAttachOptions) (0), 0, 0);
	gtk_label_set_justify (GTK_LABEL (label_number_bg), GTK_JUSTIFY_LEFT);
	gtk_misc_set_alignment (GTK_MISC (label_number_bg), 0, 0.5);

	label_unit_bg = gtk_label_new (BACKGROUND_STRING);
	gtk_widget_show (label_unit_bg);
	gtk_table_attach (GTK_TABLE (table_highlighting), label_unit_bg, 3, 4,
			  17, 18, (GtkAttachOptions) (GTK_FILL),
			  (GtkAttachOptions) (0), 0, 0);
	gtk_label_set_justify (GTK_LABEL (label_unit_bg), GTK_JUSTIFY_LEFT);
	gtk_misc_set_alignment (GTK_MISC (label_unit_bg), 0, 0.5);

	label_color_bg = gtk_label_new (BACKGROUND_STRING);
	gtk_widget_show (label_color_bg);
	gtk_table_attach (GTK_TABLE (table_highlighting), label_color_bg, 3,
			  4, 18, 19, (GtkAttachOptions) (GTK_FILL),
			  (GtkAttachOptions) (0), 0, 0);
	gtk_label_set_justify (GTK_LABEL (label_color_bg), GTK_JUSTIFY_LEFT);
	gtk_misc_set_alignment (GTK_MISC (label_color_bg), 0, 0.5);

	label_hexacolor_bg = gtk_label_new (BACKGROUND_STRING);
	gtk_widget_show (label_hexacolor_bg);
	gtk_table_attach (GTK_TABLE (table_highlighting), label_hexacolor_bg,
			  3, 4, 19, 20, (GtkAttachOptions) (GTK_FILL),
			  (GtkAttachOptions) (0), 0, 0);
	gtk_label_set_justify (GTK_LABEL (label_hexacolor_bg),
			       GTK_JUSTIFY_LEFT);
	gtk_misc_set_alignment (GTK_MISC (label_hexacolor_bg), 0, 0.5);

	label_attr_match_bg = gtk_label_new (BACKGROUND_STRING);
	gtk_widget_show (label_attr_match_bg);
	gtk_table_attach (GTK_TABLE (table_highlighting), label_attr_match_bg,
			  3, 4, 20, 21, (GtkAttachOptions) (GTK_FILL),
			  (GtkAttachOptions) (0), 0, 0);
	gtk_label_set_justify (GTK_LABEL (label_attr_match_bg),
			       GTK_JUSTIFY_LEFT);
	gtk_misc_set_alignment (GTK_MISC (label_attr_match_bg), 0, 0.5);

	label_lang_bg = gtk_label_new (BACKGROUND_STRING);
	gtk_widget_show (label_lang_bg);
	gtk_table_attach (GTK_TABLE (table_highlighting), label_lang_bg, 3, 4,
			  21, 22, (GtkAttachOptions) (GTK_FILL),
			  (GtkAttachOptions) (0), 0, 0);
	gtk_label_set_justify (GTK_LABEL (label_lang_bg), GTK_JUSTIFY_LEFT);
	gtk_misc_set_alignment (GTK_MISC (label_lang_bg), 0, 0.5);

	label_default =
		gtk_label_new (_
			       ("<span size='large'>Default</span> - default style"));
	gtk_widget_show (label_default);
	gtk_table_attach (GTK_TABLE (table_highlighting), label_default, 0, 1,
			  0, 1, (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
			  (GtkAttachOptions) (0), 5, 0);
	gtk_label_set_use_markup (GTK_LABEL (label_default), TRUE);
	gtk_label_set_justify (GTK_LABEL (label_default), GTK_JUSTIFY_LEFT);
	gtk_label_set_line_wrap (GTK_LABEL (label_default), TRUE);
	gtk_misc_set_alignment (GTK_MISC (label_default), 0, 0.5);

	label_tag =
		gtk_label_new (_
			       ("<span size='large'>Tag</span> - selector style (td,a,etc...)"));
	gtk_widget_show (label_tag);
	gtk_table_attach (GTK_TABLE (table_highlighting), label_tag, 0, 1, 1,
			  2, (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
			  (GtkAttachOptions) (0), 5, 0);
	gtk_label_set_use_markup (GTK_LABEL (label_tag), TRUE);
	gtk_label_set_justify (GTK_LABEL (label_tag), GTK_JUSTIFY_LEFT);
	gtk_label_set_line_wrap (GTK_LABEL (label_tag), TRUE);
	gtk_misc_set_alignment (GTK_MISC (label_tag), 0, 0.5);

	label_unk_pseudoclass =
		gtk_label_new (_
			       ("<span size='large'>Unknown Pseudoclass</span>"));
	gtk_widget_show (label_unk_pseudoclass);
	gtk_table_attach (GTK_TABLE (table_highlighting),
			  label_unk_pseudoclass, 0, 1, 3, 4,
			  (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
			  (GtkAttachOptions) (0), 5, 0);
	gtk_label_set_use_markup (GTK_LABEL (label_unk_pseudoclass), TRUE);
	gtk_label_set_justify (GTK_LABEL (label_unk_pseudoclass),
			       GTK_JUSTIFY_LEFT);
	gtk_misc_set_alignment (GTK_MISC (label_unk_pseudoclass), 0, 0.5);

	label_operator =
		gtk_label_new (_
			       ("<span size='large'>Operator</span> -braces, etc ..."));
	gtk_widget_show (label_operator);
	gtk_table_attach (GTK_TABLE (table_highlighting), label_operator, 0,
			  1, 4, 5, (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
			  (GtkAttachOptions) (0), 5, 0);
	gtk_label_set_use_markup (GTK_LABEL (label_operator), TRUE);
	gtk_label_set_justify (GTK_LABEL (label_operator), GTK_JUSTIFY_LEFT);
	gtk_misc_set_alignment (GTK_MISC (label_operator), 0, 0.5);

	label_vproperty =
		gtk_label_new (_("<span size='large'>Valid Property</span>"));
	gtk_widget_show (label_vproperty);
	gtk_table_attach (GTK_TABLE (table_highlighting), label_vproperty, 0,
			  1, 5, 6, (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
			  (GtkAttachOptions) (0), 5, 0);
	gtk_label_set_use_markup (GTK_LABEL (label_vproperty), TRUE);
	gtk_label_set_justify (GTK_LABEL (label_vproperty), GTK_JUSTIFY_LEFT);
	gtk_misc_set_alignment (GTK_MISC (label_vproperty), 0, 0.5);

	label_unk_property =
		gtk_label_new (_
			       ("<span size='large'>Invalid Property</span>"));
	gtk_widget_show (label_unk_property);
	gtk_table_attach (GTK_TABLE (table_highlighting), label_unk_property,
			  0, 1, 6, 7,
			  (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
			  (GtkAttachOptions) (0), 5, 0);
	gtk_label_set_use_markup (GTK_LABEL (label_unk_property), TRUE);
	gtk_label_set_justify (GTK_LABEL (label_unk_property),
			       GTK_JUSTIFY_LEFT);
	gtk_misc_set_alignment (GTK_MISC (label_unk_property), 0, 0.5);

	label_value = gtk_label_new (_("<span size='large'>Value</span>"));
	gtk_widget_show (label_value);
	gtk_table_attach (GTK_TABLE (table_highlighting), label_value, 0, 1,
			  7, 8, (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
			  (GtkAttachOptions) (0), 5, 0);
	gtk_label_set_use_markup (GTK_LABEL (label_value), TRUE);
	gtk_label_set_justify (GTK_LABEL (label_value), GTK_JUSTIFY_LEFT);
	gtk_misc_set_alignment (GTK_MISC (label_value), 0, 0.5);

	label_unk_value =
		gtk_label_new (_("<span size='large'>Unknown Value</span>"));
	gtk_widget_show (label_unk_value);
	gtk_table_attach (GTK_TABLE (table_highlighting), label_unk_value, 0,
			  1, 8, 9, (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
			  (GtkAttachOptions) (0), 5, 0);
	gtk_label_set_use_markup (GTK_LABEL (label_unk_value), TRUE);
	gtk_label_set_justify (GTK_LABEL (label_unk_value), GTK_JUSTIFY_LEFT);
	gtk_misc_set_alignment (GTK_MISC (label_unk_value), 0, 0.5);

	label_important =
		gtk_label_new (_("<span size='large'>Important</span>"));
	gtk_widget_show (label_important);
	gtk_table_attach (GTK_TABLE (table_highlighting), label_important, 0,
			  1, 9, 10,
			  (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
			  (GtkAttachOptions) (0), 5, 0);
	gtk_label_set_use_markup (GTK_LABEL (label_important), TRUE);
	gtk_label_set_justify (GTK_LABEL (label_important), GTK_JUSTIFY_LEFT);
	gtk_misc_set_alignment (GTK_MISC (label_important), 0, 0.5);

	label_directive =
		gtk_label_new (_("<span size='large'>Directive</span>"));
	gtk_widget_show (label_directive);
	gtk_table_attach (GTK_TABLE (table_highlighting), label_directive, 0,
			  1, 10, 11,
			  (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
			  (GtkAttachOptions) (0), 5, 0);
	gtk_label_set_use_markup (GTK_LABEL (label_directive), TRUE);
	gtk_label_set_justify (GTK_LABEL (label_directive), GTK_JUSTIFY_LEFT);
	gtk_misc_set_alignment (GTK_MISC (label_directive), 0, 0.5);

  label_comment_fg = gtk_label_new (FOREGROUND_STRING);
  gtk_widget_show (label_comment_fg);
  gtk_table_attach (GTK_TABLE (table_highlighting), label_comment_fg, 1, 2, 22, 23,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_label_set_justify (GTK_LABEL (label_comment_fg), GTK_JUSTIFY_LEFT);
  gtk_misc_set_alignment (GTK_MISC (label_comment_fg), 0, 0.5);

  button_comment_fg = gtk_button_new();
  attach_colored_label_to_button( button_comment_fg, cfg->COMMENT_fore_color);
  gtk_widget_show (button_comment_fg);
  gtk_table_attach (GTK_TABLE (table_highlighting), button_comment_fg, 2, 3, 22, 23,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 5, 0);
  gtk_container_set_border_width (GTK_CONTAINER (button_comment_fg), 1);
  gtk_tooltips_set_tip (tooltips, button_comment_fg, _("click here to set the foreground color for this style"), NULL);

  label_comment_bg = gtk_label_new (BACKGROUND_STRING);
  gtk_widget_show (label_comment_bg);
  gtk_table_attach (GTK_TABLE (table_highlighting), label_comment_bg, 3, 4, 22, 23,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_label_set_justify (GTK_LABEL (label_comment_bg), GTK_JUSTIFY_LEFT);
  gtk_misc_set_alignment (GTK_MISC (label_comment_bg), 0, 0.5);

  button_comment_bg = gtk_button_new();
  attach_colored_label_to_button( button_comment_bg, cfg->COMMENT_back_color);
  gtk_widget_show (button_comment_bg);
  gtk_table_attach (GTK_TABLE (table_highlighting), button_comment_bg, 4, 5, 22, 23,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 5, 0);
  gtk_container_set_border_width (GTK_CONTAINER (button_comment_bg), 1);

  checkbutton_comment_bold = gtk_check_button_new_with_mnemonic (BOLD_STRING);
  gtk_widget_show (checkbutton_comment_bold);
  gtk_table_attach (GTK_TABLE (table_highlighting), checkbutton_comment_bold, 5, 6, 22, 23,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 5, 0);
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(checkbutton_comment_bold),
								 cfg->COMMENT_bold);
  label_comment = gtk_label_new (_("<span size='large'>Comment</span>"));
  gtk_widget_show (label_comment);
  gtk_table_attach (GTK_TABLE (table_highlighting), label_comment, 0, 1, 22, 23,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 5, 0);
  gtk_label_set_use_markup (GTK_LABEL (label_comment), TRUE);
  gtk_label_set_justify (GTK_LABEL (label_comment), GTK_JUSTIFY_LEFT);
  gtk_misc_set_alignment (GTK_MISC (label_comment), 0, 0.5);
/*  **************************************************************** */
	/* ******************** CHECKBUTTONS **************************** */
	checkbutton_default_bold =
		gtk_check_button_new_with_mnemonic (BOLD_STRING);
	gtk_widget_show (checkbutton_default_bold);
	gtk_table_attach (GTK_TABLE (table_highlighting),
			  checkbutton_default_bold, 5, 6, 0, 1,
			  (GtkAttachOptions) (GTK_FILL),
			  (GtkAttachOptions) (0), 5, 0);
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(checkbutton_default_bold),
								 cfg->DEFAULT_bold);	

	checkbutton_tag_bold =
		gtk_check_button_new_with_mnemonic (BOLD_STRING);
	gtk_widget_show (checkbutton_tag_bold);
	gtk_table_attach (GTK_TABLE (table_highlighting),
			  checkbutton_tag_bold, 5, 6, 1, 2,
			  (GtkAttachOptions) (GTK_FILL),
			  (GtkAttachOptions) (0), 5, 0);
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(checkbutton_tag_bold),
								 cfg->TAG_bold);	

	checkbutton_pseudoclass_bold =
		gtk_check_button_new_with_mnemonic (BOLD_STRING);
	gtk_widget_show (checkbutton_pseudoclass_bold);
	gtk_table_attach (GTK_TABLE (table_highlighting),
			  checkbutton_pseudoclass_bold, 5, 6, 2, 3,
			  (GtkAttachOptions) (GTK_FILL),
			  (GtkAttachOptions) (0), 5, 0);
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(checkbutton_pseudoclass_bold),
								 cfg->PSEUDOCLASS_bold);	


	checkbutton_unk_pseudoclass_bold =
		gtk_check_button_new_with_mnemonic (BOLD_STRING);
	gtk_widget_show (checkbutton_unk_pseudoclass_bold);
	gtk_table_attach (GTK_TABLE (table_highlighting),
			  checkbutton_unk_pseudoclass_bold, 5, 6, 3, 4,
			  (GtkAttachOptions) (GTK_FILL),
			  (GtkAttachOptions) (0), 5, 0);
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(checkbutton_unk_pseudoclass_bold),
								 cfg->UNKNOWN_PSEUDOCLASS_bold);	

	checkbutton_operator_bold =
		gtk_check_button_new_with_mnemonic (BOLD_STRING);
	gtk_widget_show (checkbutton_operator_bold);
	gtk_table_attach (GTK_TABLE (table_highlighting),
			  checkbutton_operator_bold, 5, 6, 4, 5,
			  (GtkAttachOptions) (GTK_FILL),
			  (GtkAttachOptions) (0), 5, 0);
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(checkbutton_operator_bold),
								 cfg->OPERATOR_bold);	

	checkbutton_vproperty_bold =
		gtk_check_button_new_with_mnemonic (BOLD_STRING);
	gtk_widget_show (checkbutton_vproperty_bold);
	gtk_table_attach (GTK_TABLE (table_highlighting),
			  checkbutton_vproperty_bold, 5, 6, 5, 6,
			  (GtkAttachOptions) (GTK_FILL),
			  (GtkAttachOptions) (0), 5, 0);
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(checkbutton_vproperty_bold),
								 cfg->IDENTIFIER_bold);	

	checkbutton_unk_property_bold =
		gtk_check_button_new_with_mnemonic (BOLD_STRING);
	gtk_widget_show (checkbutton_unk_property_bold);
	gtk_table_attach (GTK_TABLE (table_highlighting),
			  checkbutton_unk_property_bold, 5, 6, 6, 7,
			  (GtkAttachOptions) (GTK_FILL),
			  (GtkAttachOptions) (0), 5, 0);
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(checkbutton_unk_property_bold),
								 cfg->UNKNOWN_IDENTIFIER_bold);	

	checkbutton_value_bold =
		gtk_check_button_new_with_mnemonic (BOLD_STRING);
	gtk_widget_show (checkbutton_value_bold);
	gtk_table_attach (GTK_TABLE (table_highlighting),
			  checkbutton_value_bold, 5, 6, 7, 8,
			  (GtkAttachOptions) (GTK_FILL),
			  (GtkAttachOptions) (0), 5, 0);
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(checkbutton_value_bold),
								 cfg->VALID_VALUE_bold);	

	checkbutton_unk_value_bold =
		gtk_check_button_new_with_mnemonic (BOLD_STRING);
	gtk_widget_show (checkbutton_unk_value_bold);
	gtk_table_attach (GTK_TABLE (table_highlighting),
			  checkbutton_unk_value_bold, 5, 6, 8, 9,
			  (GtkAttachOptions) (GTK_FILL),
			  (GtkAttachOptions) (0), 5, 0);
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(checkbutton_unk_value_bold),
								 cfg->VALUE_bold);	

	checkbutton_important_bold =
		gtk_check_button_new_with_mnemonic (BOLD_STRING);
	gtk_widget_show (checkbutton_important_bold);
	gtk_table_attach (GTK_TABLE (table_highlighting),
			  checkbutton_important_bold, 5, 6, 9, 10,
			  (GtkAttachOptions) (GTK_FILL),
			  (GtkAttachOptions) (0), 5, 0);
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(checkbutton_important_bold),
								 cfg->IMPORTANT_bold);	

	checkbutton_directive_bold =
		gtk_check_button_new_with_mnemonic (BOLD_STRING);
	gtk_widget_show (checkbutton_directive_bold);
	gtk_table_attach (GTK_TABLE (table_highlighting),
			  checkbutton_directive_bold, 5, 6, 10, 11,
			  (GtkAttachOptions) (GTK_FILL),
			  (GtkAttachOptions) (0), 5, 0);
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(checkbutton_directive_bold),
								 cfg->DIRECTIVE_bold);	

	checkbutton_string_double_bold =
		gtk_check_button_new_with_mnemonic (BOLD_STRING);
	gtk_widget_show (checkbutton_string_double_bold);
	gtk_table_attach (GTK_TABLE (table_highlighting),
			  checkbutton_string_double_bold, 5, 6, 11, 12,
			  (GtkAttachOptions) (GTK_FILL),
			  (GtkAttachOptions) (0), 5, 0);
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(checkbutton_string_double_bold),
								 cfg->DOUBLESTRING_bold);	

	checkbutton_string_single_bold =
		gtk_check_button_new_with_mnemonic (BOLD_STRING);
	gtk_widget_show (checkbutton_string_single_bold);
	gtk_table_attach (GTK_TABLE (table_highlighting),
			  checkbutton_string_single_bold, 5, 6, 12, 13,
			  (GtkAttachOptions) (GTK_FILL),
			  (GtkAttachOptions) (0), 5, 0);
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(checkbutton_string_single_bold),
								 cfg->SINGLESTRING_bold);	

	checkbutton_class_bold =
		gtk_check_button_new_with_mnemonic (BOLD_STRING);
	gtk_widget_show (checkbutton_class_bold);
	gtk_table_attach (GTK_TABLE (table_highlighting),
			  checkbutton_class_bold, 5, 6, 13, 14,
			  (GtkAttachOptions) (GTK_FILL),
			  (GtkAttachOptions) (0), 5, 0);
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(checkbutton_class_bold),
								 cfg->CLASS_bold);	

	checkbutton_id_bold =
		gtk_check_button_new_with_mnemonic (BOLD_STRING);
	gtk_widget_show (checkbutton_id_bold);
	gtk_table_attach (GTK_TABLE (table_highlighting), checkbutton_id_bold,
			  5, 6, 14, 15, (GtkAttachOptions) (GTK_FILL),
			  (GtkAttachOptions) (0), 5, 0);
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(checkbutton_id_bold),
								 cfg->ID_bold);	


	checkbutton_function_bold =
		gtk_check_button_new_with_mnemonic (BOLD_STRING);
	gtk_widget_show (checkbutton_function_bold);
	gtk_table_attach (GTK_TABLE (table_highlighting),
			  checkbutton_function_bold, 5, 6, 15, 16,
			  (GtkAttachOptions) (GTK_FILL),
			  (GtkAttachOptions) (0), 5, 0);
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(checkbutton_function_bold),
								 cfg->FUNCTION_bold);	

	checkbutton_number_bold =
		gtk_check_button_new_with_mnemonic (BOLD_STRING);
	gtk_widget_show (checkbutton_number_bold);
	gtk_table_attach (GTK_TABLE (table_highlighting),
			  checkbutton_number_bold, 5, 6, 16, 17,
			  (GtkAttachOptions) (GTK_FILL),
			  (GtkAttachOptions) (0), 5, 0);
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(checkbutton_number_bold),
								 cfg->NUMBER_bold);	

	checkbutton_unit_bold =
		gtk_check_button_new_with_mnemonic (BOLD_STRING);
	gtk_widget_show (checkbutton_unit_bold);
	gtk_table_attach (GTK_TABLE (table_highlighting),
			  checkbutton_unit_bold, 5, 6, 17, 18,
			  (GtkAttachOptions) (GTK_FILL),
			  (GtkAttachOptions) (0), 5, 0);
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(checkbutton_unit_bold),
								 cfg->UNIT_bold);	

	checkbutton_color_bold =
		gtk_check_button_new_with_mnemonic (BOLD_STRING);
	gtk_widget_show (checkbutton_color_bold);
	gtk_table_attach (GTK_TABLE (table_highlighting),
			  checkbutton_color_bold, 5, 6, 18, 19,
			  (GtkAttachOptions) (GTK_FILL),
			  (GtkAttachOptions) (0), 5, 0);
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(checkbutton_color_bold),
								 cfg->COLOR_bold);	

	checkbutton_hexacolor_bold =
		gtk_check_button_new_with_mnemonic (BOLD_STRING);
	gtk_widget_show (checkbutton_hexacolor_bold);
	gtk_table_attach (GTK_TABLE (table_highlighting),
			  checkbutton_hexacolor_bold, 5, 6, 19, 20,
			  (GtkAttachOptions) (GTK_FILL),
			  (GtkAttachOptions) (0), 5, 0);
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(checkbutton_hexacolor_bold),
								 cfg->HEXACOLOR_bold);	

	checkbutton_attr_match_bold =
		gtk_check_button_new_with_mnemonic (BOLD_STRING);
	gtk_widget_show (checkbutton_attr_match_bold);
	gtk_table_attach (GTK_TABLE (table_highlighting),
			  checkbutton_attr_match_bold, 5, 6, 20, 21,
			  (GtkAttachOptions) (GTK_FILL),
			  (GtkAttachOptions) (0), 5, 0);
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(checkbutton_attr_match_bold),
								 cfg->ATTR_MATCH_bold);	

	checkbutton_lang_bold =
		gtk_check_button_new_with_mnemonic (BOLD_STRING);
	gtk_widget_show (checkbutton_lang_bold);
	gtk_table_attach (GTK_TABLE (table_highlighting),
			  checkbutton_lang_bold, 5, 6, 21, 22,
			  (GtkAttachOptions) (GTK_FILL),
			  (GtkAttachOptions) (0), 5, 0);
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(checkbutton_lang_bold),
								 cfg->LANGUAGE_bold);	


	/* ********************* BACKGROUND BUTTONS ******************* */
	button_tag_bg = gtk_button_new();
	attach_colored_label_to_button( button_tag_bg, cfg->TAG_back_color);
	gtk_widget_show (button_tag_bg);
	gtk_table_attach (GTK_TABLE (table_highlighting), button_tag_bg, 4, 5,
			  1, 2, (GtkAttachOptions) (GTK_FILL),
			  (GtkAttachOptions) (0), 5, 0);
	gtk_container_set_border_width (GTK_CONTAINER (button_tag_bg), 1);

	button_pseudoclass_bg  = gtk_button_new();
	attach_colored_label_to_button( button_pseudoclass_bg, cfg->PSEUDOCLASS_back_color);
	gtk_widget_show (button_pseudoclass_bg);
	gtk_table_attach (GTK_TABLE (table_highlighting),
			  button_pseudoclass_bg, 4, 5, 2, 3,
			  (GtkAttachOptions) (GTK_FILL),
			  (GtkAttachOptions) (0), 5, 0);
	gtk_container_set_border_width (GTK_CONTAINER (button_pseudoclass_bg),
					1);

	button_unk_pseudoclass_bg = gtk_button_new();
	attach_colored_label_to_button( button_unk_pseudoclass_bg, cfg->UNKNOWN_PSEUDOCLASS_back_color);
	gtk_widget_show (button_unk_pseudoclass_bg);
	gtk_table_attach (GTK_TABLE (table_highlighting),
			  button_unk_pseudoclass_bg, 4, 5, 3, 4,
			  (GtkAttachOptions) (GTK_FILL),
			  (GtkAttachOptions) (0), 5, 0);
	gtk_container_set_border_width (GTK_CONTAINER
					(button_unk_pseudoclass_bg), 1);

	button_operator_bg = gtk_button_new();
	attach_colored_label_to_button( button_operator_bg, cfg->OPERATOR_back_color);
	gtk_widget_show (button_operator_bg);
	gtk_table_attach (GTK_TABLE (table_highlighting), button_operator_bg,
			  4, 5, 4, 5, (GtkAttachOptions) (GTK_FILL),
			  (GtkAttachOptions) (0), 5, 0);
	gtk_container_set_border_width (GTK_CONTAINER (button_operator_bg),
					1);

	button_vproperty_bg = gtk_button_new();
	attach_colored_label_to_button( button_vproperty_bg, cfg->IDENTIFIER_back_color);
	gtk_widget_show (button_vproperty_bg);
	gtk_table_attach (GTK_TABLE (table_highlighting), button_vproperty_bg,
			  4, 5, 5, 6, (GtkAttachOptions) (GTK_FILL),
			  (GtkAttachOptions) (0), 5, 0);
	gtk_container_set_border_width (GTK_CONTAINER (button_vproperty_bg),
					1);

	button_unk_property_bg = gtk_button_new();
	attach_colored_label_to_button( button_unk_property_bg, cfg->UNKNOWN_IDENTIFIER_back_color);
	gtk_widget_show (button_unk_property_bg);
	gtk_table_attach (GTK_TABLE (table_highlighting),
			  button_unk_property_bg, 4, 5, 6, 7,
			  (GtkAttachOptions) (GTK_FILL),
			  (GtkAttachOptions) (0), 5, 0);
	gtk_container_set_border_width (GTK_CONTAINER
					(button_unk_property_bg), 1);

	button_value_bg  = gtk_button_new();
	attach_colored_label_to_button( button_value_bg, cfg->VALID_VALUE_back_color);
	gtk_widget_show (button_value_bg);
	gtk_table_attach (GTK_TABLE (table_highlighting), button_value_bg, 4,
			  5, 7, 8, (GtkAttachOptions) (GTK_FILL),
			  (GtkAttachOptions) (0), 5, 0);
	gtk_container_set_border_width (GTK_CONTAINER (button_value_bg), 1);

	button_unk_value_bg  = gtk_button_new();
	attach_colored_label_to_button( button_unk_value_bg, cfg->VALUE_back_color);
	gtk_widget_show (button_unk_value_bg);
	gtk_table_attach (GTK_TABLE (table_highlighting), button_unk_value_bg,
			  4, 5, 8, 9, (GtkAttachOptions) (GTK_FILL),
			  (GtkAttachOptions) (0), 5, 0);
	gtk_container_set_border_width (GTK_CONTAINER (button_unk_value_bg),
					1);

	button_important_bg = gtk_button_new();
	attach_colored_label_to_button( button_important_bg, cfg->IMPORTANT_back_color);
	gtk_widget_show (button_important_bg);
	gtk_table_attach (GTK_TABLE (table_highlighting), button_important_bg,
			  4, 5, 9, 10, (GtkAttachOptions) (GTK_FILL),
			  (GtkAttachOptions) (0), 5, 0);
	gtk_container_set_border_width (GTK_CONTAINER (button_important_bg),
					1);

	button_directive_bg = gtk_button_new();
	attach_colored_label_to_button( button_directive_bg, cfg->DIRECTIVE_back_color);
	gtk_widget_show (button_directive_bg);
	gtk_table_attach (GTK_TABLE (table_highlighting), button_directive_bg,
			  4, 5, 10, 11, (GtkAttachOptions) (GTK_FILL),
			  (GtkAttachOptions) (0), 5, 0);
	gtk_container_set_border_width (GTK_CONTAINER (button_directive_bg),
					1);

	button_string_double_bg = gtk_button_new();
	attach_colored_label_to_button( button_string_double_bg, cfg->DOUBLESTRING_back_color);
	gtk_widget_show (button_string_double_bg);
	gtk_table_attach (GTK_TABLE (table_highlighting),
			  button_string_double_bg, 4, 5, 11, 12,
			  (GtkAttachOptions) (GTK_FILL),
			  (GtkAttachOptions) (0), 5, 0);
	gtk_container_set_border_width (GTK_CONTAINER
					(button_string_double_bg), 1);

	button_string_single_bg = gtk_button_new();
	attach_colored_label_to_button( button_string_single_bg, cfg->SINGLESTRING_back_color);
	gtk_widget_show (button_string_single_bg);
	gtk_table_attach (GTK_TABLE (table_highlighting),
			  button_string_single_bg, 4, 5, 12, 13,
			  (GtkAttachOptions) (GTK_FILL),
			  (GtkAttachOptions) (0), 5, 0);
	gtk_container_set_border_width (GTK_CONTAINER
					(button_string_single_bg), 1);

	button_class_bg = gtk_button_new();
	attach_colored_label_to_button( button_class_bg, cfg->CLASS_back_color);
	gtk_widget_show (button_class_bg);
	gtk_table_attach (GTK_TABLE (table_highlighting), button_class_bg, 4,
			  5, 13, 14, (GtkAttachOptions) (GTK_FILL),
			  (GtkAttachOptions) (0), 5, 0);
	gtk_container_set_border_width (GTK_CONTAINER (button_class_bg), 1);

	button_id_bg = gtk_button_new();
	attach_colored_label_to_button( button_id_bg, cfg->ID_back_color);
	gtk_widget_show (button_id_bg);
	gtk_table_attach (GTK_TABLE (table_highlighting), button_id_bg, 4, 5,
			  14, 15, (GtkAttachOptions) (GTK_FILL),
			  (GtkAttachOptions) (0), 5, 0);
	gtk_container_set_border_width (GTK_CONTAINER (button_id_bg), 1);

	button_function_bg = gtk_button_new();
	attach_colored_label_to_button( button_function_bg, cfg->FUNCTION_back_color);
	gtk_widget_show (button_function_bg);
	gtk_table_attach (GTK_TABLE (table_highlighting), button_function_bg,
			  4, 5, 15, 16, (GtkAttachOptions) (GTK_FILL),
			  (GtkAttachOptions) (0), 5, 0);
	gtk_container_set_border_width (GTK_CONTAINER (button_function_bg),
					1);

	button_number_bg = gtk_button_new();
	attach_colored_label_to_button( button_number_bg, cfg->NUMBER_back_color);
	gtk_widget_show (button_number_bg);
	gtk_table_attach (GTK_TABLE (table_highlighting), button_number_bg, 4,
			  5, 16, 17, (GtkAttachOptions) (GTK_FILL),
			  (GtkAttachOptions) (0), 5, 0);
	gtk_container_set_border_width (GTK_CONTAINER (button_number_bg), 1);

	button_unit_bg = gtk_button_new();
	attach_colored_label_to_button( button_unit_bg, cfg->UNIT_back_color);
	gtk_widget_show (button_unit_bg);
	gtk_table_attach (GTK_TABLE (table_highlighting), button_unit_bg, 4,
			  5, 17, 18, (GtkAttachOptions) (GTK_FILL),
			  (GtkAttachOptions) (0), 5, 0);
	gtk_container_set_border_width (GTK_CONTAINER (button_unit_bg), 1);

	button_color_bg = gtk_button_new();
	attach_colored_label_to_button( button_color_bg, cfg->COLOR_back_color);
	gtk_widget_show (button_color_bg);
	gtk_table_attach (GTK_TABLE (table_highlighting), button_color_bg, 4,
			  5, 18, 19, (GtkAttachOptions) (GTK_FILL),
			  (GtkAttachOptions) (0), 5, 0);
	gtk_container_set_border_width (GTK_CONTAINER (button_color_bg), 1);

	button_hexacolor_bg = gtk_button_new();
	attach_colored_label_to_button( button_hexacolor_bg, cfg->HEXACOLOR_back_color);
	gtk_widget_show (button_hexacolor_bg);
	gtk_table_attach (GTK_TABLE (table_highlighting), button_hexacolor_bg,
			  4, 5, 19, 20, (GtkAttachOptions) (GTK_FILL),
			  (GtkAttachOptions) (0), 5, 0);
	gtk_container_set_border_width (GTK_CONTAINER (button_hexacolor_bg),
					1);

	button_attr_match_bg = gtk_button_new();
	attach_colored_label_to_button( button_attr_match_bg, cfg->ATTR_MATCH_back_color);
	gtk_widget_show (button_attr_match_bg);
	gtk_table_attach (GTK_TABLE (table_highlighting),
			  button_attr_match_bg, 4, 5, 20, 21,
			  (GtkAttachOptions) (GTK_FILL),
			  (GtkAttachOptions) (0), 5, 0);
	gtk_container_set_border_width (GTK_CONTAINER (button_attr_match_bg),
					1);

	button_lang_bg = gtk_button_new();
	attach_colored_label_to_button( button_lang_bg, cfg->LANGUAGE_back_color);
	gtk_widget_show (button_lang_bg);
	gtk_table_attach (GTK_TABLE (table_highlighting), button_lang_bg, 4,
			  5, 21, 22, (GtkAttachOptions) (GTK_FILL),
			  (GtkAttachOptions) (0), 5, 0);
	gtk_container_set_border_width (GTK_CONTAINER (button_lang_bg), 1);

	label_string_double =
		gtk_label_new (_
			       ("<span size='large'>Double quoted string</span>"));
	gtk_widget_show (label_string_double);
	gtk_table_attach (GTK_TABLE (table_highlighting), label_string_double,
			  0, 1, 11, 12,
			  (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
			  (GtkAttachOptions) (0), 5, 0);
	gtk_label_set_use_markup (GTK_LABEL (label_string_double), TRUE);
	gtk_label_set_justify (GTK_LABEL (label_string_double),
			       GTK_JUSTIFY_LEFT);
	gtk_misc_set_alignment (GTK_MISC (label_string_double), 0, 0.5);

	label_string_single =
		gtk_label_new (_
			       ("<span size='large'>Single quoted string</span>"));
	gtk_widget_show (label_string_single);
	gtk_table_attach (GTK_TABLE (table_highlighting), label_string_single,
			  0, 1, 12, 13,
			  (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
			  (GtkAttachOptions) (0), 5, 0);
	gtk_label_set_use_markup (GTK_LABEL (label_string_single), TRUE);
	gtk_label_set_justify (GTK_LABEL (label_string_single),
			       GTK_JUSTIFY_LEFT);
	gtk_misc_set_alignment (GTK_MISC (label_string_single), 0, 0.5);

	label_class =
		gtk_label_new (_
			       ("<span size='large'>Class</span> - a CSS class <i>tag.class</i>"));
	gtk_widget_show (label_class);
	gtk_table_attach (GTK_TABLE (table_highlighting), label_class, 0, 1,
			  13, 14, (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
			  (GtkAttachOptions) (0), 5, 0);
	gtk_label_set_use_markup (GTK_LABEL (label_class), TRUE);
	gtk_label_set_justify (GTK_LABEL (label_class), GTK_JUSTIFY_LEFT);
	gtk_misc_set_alignment (GTK_MISC (label_class), 0, 0.5);

	label_id =
		gtk_label_new (_
			       ("<span size='large'>Id</span> - a CSS id <i>#id</i>"));
	gtk_widget_show (label_id);
	gtk_table_attach (GTK_TABLE (table_highlighting), label_id, 0, 1, 14,
			  15, (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
			  (GtkAttachOptions) (0), 5, 0);
	gtk_label_set_use_markup (GTK_LABEL (label_id), TRUE);
	gtk_label_set_justify (GTK_LABEL (label_id), GTK_JUSTIFY_LEFT);
	gtk_misc_set_alignment (GTK_MISC (label_id), 0, 0.5);

	label_function =
		gtk_label_new (_
			       ("<span size='large'>Function</span> - url() , attr() , etc ..."));
	gtk_widget_show (label_function);
	gtk_table_attach (GTK_TABLE (table_highlighting), label_function, 0,
			  1, 15, 16,
			  (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
			  (GtkAttachOptions) (0), 5, 0);
	gtk_label_set_use_markup (GTK_LABEL (label_function), TRUE);
	gtk_label_set_justify (GTK_LABEL (label_function), GTK_JUSTIFY_LEFT);
	gtk_misc_set_alignment (GTK_MISC (label_function), 0, 0.5);

	label_number = gtk_label_new (_("<span size='large'>Number</span>"));
	gtk_widget_show (label_number);
	gtk_table_attach (GTK_TABLE (table_highlighting), label_number, 0, 1,
			  16, 17, (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
			  (GtkAttachOptions) (0), 5, 0);
	gtk_label_set_use_markup (GTK_LABEL (label_number), TRUE);
	gtk_label_set_justify (GTK_LABEL (label_number), GTK_JUSTIFY_LEFT);
	gtk_misc_set_alignment (GTK_MISC (label_number), 0, 0.5);

	label_unit =
		gtk_label_new (_
			       ("<span size='large'>Valid Unit</span> - pt, px, em, etc ..."));
	gtk_widget_show (label_unit);
	gtk_table_attach (GTK_TABLE (table_highlighting), label_unit, 0, 1,
			  17, 18, (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
			  (GtkAttachOptions) (0), 5, 0);
	gtk_label_set_use_markup (GTK_LABEL (label_unit), TRUE);
	gtk_label_set_justify (GTK_LABEL (label_unit), GTK_JUSTIFY_LEFT);
	gtk_misc_set_alignment (GTK_MISC (label_unit), 0, 0.5);

	label_color = gtk_label_new (_("<span size='large'>Color</span>"));
	gtk_widget_show (label_color);
	gtk_table_attach (GTK_TABLE (table_highlighting), label_color, 0, 1,
			  18, 19, (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
			  (GtkAttachOptions) (0), 5, 0);
	gtk_label_set_use_markup (GTK_LABEL (label_color), TRUE);
	gtk_label_set_justify (GTK_LABEL (label_color), GTK_JUSTIFY_LEFT);
	gtk_misc_set_alignment (GTK_MISC (label_color), 0, 0.5);

	label_hexacolor =
		gtk_label_new (_
			       ("<span size='large'>Hexadecimal color</span>"));
	gtk_widget_show (label_hexacolor);
	gtk_table_attach (GTK_TABLE (table_highlighting), label_hexacolor, 0,
			  1, 19, 20,
			  (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
			  (GtkAttachOptions) (0), 5, 0);
	gtk_label_set_use_markup (GTK_LABEL (label_hexacolor), TRUE);
	gtk_label_set_justify (GTK_LABEL (label_hexacolor), GTK_JUSTIFY_LEFT);
	gtk_misc_set_alignment (GTK_MISC (label_hexacolor), 0, 0.5);

	label_attr_match =
		gtk_label_new (_
			       ("<span size='large'>Attribute Match</span> as img[alt=\"hi\"]"));
	gtk_widget_show (label_attr_match);
	gtk_table_attach (GTK_TABLE (table_highlighting), label_attr_match, 0,
			  1, 20, 21,
			  (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
			  (GtkAttachOptions) (0), 5, 0);
	gtk_label_set_use_markup (GTK_LABEL (label_attr_match), TRUE);
	gtk_label_set_justify (GTK_LABEL (label_attr_match),
			       GTK_JUSTIFY_LEFT);
	gtk_misc_set_alignment (GTK_MISC (label_attr_match), 0, 0.5);

	label_lang =
		gtk_label_new (_
			       ("<span size='large'>Language</span> - as td:lang(en)"));
	gtk_widget_show (label_lang);
	gtk_table_attach (GTK_TABLE (table_highlighting), label_lang, 0, 1,
			  21, 22, (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
			  (GtkAttachOptions) (0), 5, 0);
	gtk_label_set_use_markup (GTK_LABEL (label_lang), TRUE);
	gtk_label_set_justify (GTK_LABEL (label_lang), GTK_JUSTIFY_LEFT);
	gtk_misc_set_alignment (GTK_MISC (label_lang), 0, 0.5);

	label_pseudoclass =
		gtk_label_new (_
			       ("<span size='large'>Pseudoclass</span> - hover, link, ..."));
	gtk_widget_show (label_pseudoclass);
	gtk_table_attach (GTK_TABLE (table_highlighting), label_pseudoclass,
			  0, 1, 2, 3,
			  (GtkAttachOptions) (GTK_EXPAND | GTK_SHRINK |
					      GTK_FILL),
			  (GtkAttachOptions) (0), 5, 0);
	gtk_label_set_use_markup (GTK_LABEL (label_pseudoclass), TRUE);
	gtk_label_set_justify (GTK_LABEL (label_pseudoclass),
			       GTK_JUSTIFY_LEFT);
	gtk_misc_set_alignment (GTK_MISC (label_pseudoclass), 0, 0.5);

	label_tab_highlight = gtk_label_new (_("<span size='medium'><b>Highlighting</b></span>"));
	gtk_label_set_use_markup( GTK_LABEL(label_tab_highlight), TRUE);
	gtk_widget_show (label_tab_highlight);	
	gtk_notebook_set_tab_label (GTK_NOTEBOOK (notebook),
				    gtk_notebook_get_nth_page (GTK_NOTEBOOK
							       (notebook), 1),
				    label_tab_highlight);
	gtk_label_set_justify (GTK_LABEL (label_tab_highlight),
			       GTK_JUSTIFY_LEFT);
	/* *********************** fonts ****************** */
	vbox_font = gtk_vbox_new(FALSE,15);

	gtk_widget_show (vbox_font);
	gtk_container_add (GTK_CONTAINER (notebook), vbox_font);
	
	fontsel = gtk_font_selection_new();
	gtk_widget_show (fontsel);
	gtk_box_pack_start(GTK_BOX(vbox_font),fontsel,FALSE,FALSE,10);
	
	hbox_current_and_apply = gtk_hbox_new(FALSE,2);
	gtk_widget_show (hbox_current_and_apply);
	
	label_current_info = gtk_label_new("Current font:");
	gtk_widget_show(label_current_info);
	gtk_box_pack_start(GTK_BOX(hbox_current_and_apply),label_current_info,FALSE,FALSE,5);
		
	if( cfg->font != NULL){
		gchar* cfont;
		gchar* label_str;
		cfont = g_strdup(cfg->font);
		g_strdelimit(cfont,"!",' ');
		label_str = g_strdup_printf("%s, %d",cfont,cfg->font_size);
		label_current = gtk_label_new(label_str);
		gtk_font_selection_set_font_name(GTK_FONT_SELECTION(fontsel), label_str);
		g_free(label_str);
		g_free(cfont);
	}else{
		label_current = gtk_label_new("Default");
	}
	
	gtk_widget_show(label_current);
	gtk_label_set_justify(GTK_LABEL(label_current),GTK_JUSTIFY_LEFT); 
	gtk_box_pack_start(GTK_BOX(hbox_current_and_apply),label_current,FALSE,FALSE,5);
	
	blank_label = gtk_label_new("");
	gtk_widget_show(blank_label);
	gtk_label_set_justify(GTK_LABEL(blank_label),GTK_JUSTIFY_LEFT); 
	gtk_box_pack_start(GTK_BOX(hbox_current_and_apply),blank_label,TRUE,TRUE,5);
	
	apply_button = gtk_button_new_from_stock( GTK_STOCK_APPLY );
	gtk_widget_show(apply_button);
	gtk_box_pack_start(GTK_BOX(hbox_current_and_apply),apply_button,FALSE,FALSE,5);
	
	gtk_box_pack_start(GTK_BOX(vbox_font),hbox_current_and_apply,FALSE,FALSE,5);
	
	
	label_font = gtk_label_new (_("<span size='medium'><b>Font</b></span>"));
	gtk_label_set_use_markup( GTK_LABEL(label_font), TRUE);
	gtk_widget_show (label_font);	
	gtk_notebook_set_tab_label (GTK_NOTEBOOK (notebook),
				    gtk_notebook_get_nth_page (GTK_NOTEBOOK(notebook), 2),
				    label_font);
	gtk_label_set_justify (GTK_LABEL (label_font),
			       GTK_JUSTIFY_LEFT);	

	
	/* ********************************************************************* */
	label_other = gtk_label_new (_("<span size='medium'><b>Other</b></span>"));
	gtk_label_set_use_markup( GTK_LABEL(label_other), TRUE);
	gtk_widget_show (label_other);	

	vbox_other_settings = gtk_vbox_new(FALSE, 5);
	gtk_widget_show (vbox_other_settings);

	hbox_browser = gtk_hbox_new(FALSE, 5);
	gtk_widget_show (hbox_browser);	

	entry_browser = gtk_entry_new();
	gtk_widget_show (entry_browser);	

	button_browser_apply = gtk_button_new_with_label(_("Apply changes"));
	gtk_widget_show (button_browser_apply);	

	hbox_label = gtk_hbox_new(FALSE, 5);
	gtk_widget_show (hbox_label);	

	label_browser = gtk_label_new(_("Default browser command"));
	gtk_widget_show (label_browser);	

	label_browser2 = gtk_label_new(_("Current browser command: "));
	gtk_widget_show (label_browser2);	

	if( cfg->browser_command == NULL )
		label_current_browser = gtk_label_new(_("None"));
	else{
		label_current_browser = gtk_label_new(cfg->browser_command);
		gtk_entry_set_text(GTK_ENTRY(entry_browser), cfg->browser_command);
	}
	gtk_widget_show (label_current_browser);	

	blank_label = gtk_label_new("");
	gtk_widget_show(blank_label);

	gtk_box_pack_start(GTK_BOX(hbox_browser), label_browser, FALSE, FALSE, 5);
	gtk_box_pack_start(GTK_BOX(hbox_browser), entry_browser, FALSE, TRUE, 5);
	gtk_box_pack_start(GTK_BOX(hbox_browser), button_browser_apply, FALSE, FALSE, 5);
	gtk_box_pack_start(GTK_BOX(vbox_other_settings), hbox_browser, FALSE, FALSE, 5);

	gtk_box_pack_start(GTK_BOX(hbox_label), label_browser2, FALSE, FALSE, 5);
	gtk_box_pack_start(GTK_BOX(hbox_label), label_current_browser, FALSE, FALSE, 5);
	gtk_box_pack_start(GTK_BOX(hbox_label), blank_label,TRUE,TRUE,5);

	gtk_box_pack_start(GTK_BOX(vbox_other_settings), hbox_label, FALSE, FALSE, 5);

	gtk_notebook_append_page        (GTK_NOTEBOOK(notebook),
                                    vbox_other_settings,
                                    label_other);

	/* ******************** action area ******************* */
	dialog_action_area = GTK_DIALOG (preferences_dialog)->action_area;
	gtk_widget_show (dialog_action_area);
	gtk_button_box_set_layout (GTK_BUTTON_BOX (dialog_action_area),
				   GTK_BUTTONBOX_END);

	cancelbutton = gtk_button_new_from_stock ("gtk-cancel");
	gtk_widget_show (cancelbutton);
	gtk_dialog_add_action_widget (GTK_DIALOG (preferences_dialog),
				      cancelbutton, GTK_RESPONSE_CANCEL);
	GTK_WIDGET_SET_FLAGS (cancelbutton, GTK_CAN_DEFAULT);

	okbutton = gtk_button_new_from_stock ("gtk-ok");
	gtk_widget_show (okbutton);
	gtk_dialog_add_action_widget (GTK_DIALOG (preferences_dialog),
				      okbutton, GTK_RESPONSE_OK);
	GTK_WIDGET_SET_FLAGS (okbutton, GTK_CAN_DEFAULT);

	/* ********************************************************************* */	
	g_signal_connect ((gpointer) button_browser_apply, "clicked",
					G_CALLBACK (on_apply_browser_button_clicked),window);
	//
	g_signal_connect ((gpointer) button_comment_fg, "clicked",
					G_CALLBACK (on_button_comment_fg_clicked),window);
	g_signal_connect ((gpointer) button_comment_bg, "clicked",
					G_CALLBACK (on_button_comment_bg_clicked),window);
	g_signal_connect ((gpointer) checkbutton_comment_bold, "toggled",
					G_CALLBACK (on_checkbutton_comment_bold_toggled),window);
	g_signal_connect ((gpointer) button_vproperty_fg, "clicked",
			  G_CALLBACK (on_button_vproperty_fg_clicked), window);
	g_signal_connect ((gpointer) button_operator_fg, "clicked",
			  G_CALLBACK (on_button_operator_fg_clicked), window);
	g_signal_connect ((gpointer) button_unk_pseudoclass_fg, "clicked",
			  G_CALLBACK (on_button_unk_pseudoclass_fg_clicked),
			  window);
	g_signal_connect ((gpointer) button_pseudoclass_fg, "clicked",
			  G_CALLBACK (on_button_pseudoclass_fg_clicked),
			  window);
	g_signal_connect ((gpointer) button_tag_fg, "clicked",
			  G_CALLBACK (on_button_tag_fg_clicked), window);
	g_signal_connect ((gpointer) button_default_bg, "clicked",
			  G_CALLBACK (on_button_default_bg_clicked), window);
	g_signal_connect ((gpointer) button_unk_property_fg, "clicked",
			  G_CALLBACK (on_button_unk_property_fg_clicked),
			  window);
	g_signal_connect ((gpointer) button_value_fg, "clicked",
			  G_CALLBACK (on_button_value_fg_clicked), window);
	g_signal_connect ((gpointer) button_unk_value_fg, "clicked",
			  G_CALLBACK (on_button_unk_value_fg_clicked), window);
	g_signal_connect ((gpointer) button_important_fg, "clicked",
			  G_CALLBACK (on_button_important_fg_clicked), window);
	g_signal_connect ((gpointer) button_directive_fg, "clicked",
			  G_CALLBACK (on_button_directive_fg_clicked), window);
	g_signal_connect ((gpointer) button_string_double_fg, "clicked",
			  G_CALLBACK (on_button_string_double_fg_clicked),
			  window);
	g_signal_connect ((gpointer) button_string_single_fg, "clicked",
			  G_CALLBACK (on_button_string_single_fg_clicked),
			  window);
	g_signal_connect ((gpointer) button_class_fg, "clicked",
			  G_CALLBACK (on_button_class_fg_clicked), window);
	g_signal_connect ((gpointer) button_id_fg, "clicked",
			  G_CALLBACK (on_button_id_fg_clicked), window);
	g_signal_connect ((gpointer) button_function_fg, "clicked",
			  G_CALLBACK (on_button_function_fg_clicked), window);
	g_signal_connect ((gpointer) button_number_fg, "clicked",
			  G_CALLBACK (on_button_number_fg_clicked), window);
	g_signal_connect ((gpointer) button_unit_fg, "clicked",
			  G_CALLBACK (on_button_unit_fg_clicked), window);
	g_signal_connect ((gpointer) button_color_fg, "clicked",
			  G_CALLBACK (on_button_color_fg_clicked), window);
	g_signal_connect ((gpointer) button_hexacolor_fg, "clicked",
			  G_CALLBACK (on_button_hexacolor_fg_clicked), window);
	g_signal_connect ((gpointer) button_attr_match_fg, "clicked",
			  G_CALLBACK (on_button_attr_match_fg_clicked), window);
	g_signal_connect ((gpointer) button_lang_fg, "clicked",
			  G_CALLBACK (on_button_lang_fg_clicked), window);
	g_signal_connect ((gpointer) checkbutton_default_bold, "toggled",
			  G_CALLBACK (on_checkbutton_default_bold_toggled),
			  window);
	g_signal_connect ((gpointer) checkbutton_tag_bold, "toggled",
			  G_CALLBACK (on_checkbutton_tag_bold_toggled), window);
	g_signal_connect ((gpointer) checkbutton_pseudoclass_bold, "toggled",
			  G_CALLBACK
			  (on_checkbutton_pseudoclass_bold_toggled), window);
	g_signal_connect ((gpointer) checkbutton_unk_pseudoclass_bold,
			  "toggled",
			  G_CALLBACK
			  (on_checkbutton_unk_pseudoclass_bold_toggled),
			  window);
	g_signal_connect ((gpointer) checkbutton_operator_bold, "toggled",
			  G_CALLBACK (on_checkbutton_operator_bold_toggled),
			  window);
	g_signal_connect ((gpointer) checkbutton_vproperty_bold, "toggled",
			  G_CALLBACK (on_checkbutton_vproperty_bold_toggled),
			  window);
	g_signal_connect ((gpointer) checkbutton_unk_property_bold, "toggled",
			  G_CALLBACK
			  (on_checkbutton_unk_property_bold_toggled), window);
	g_signal_connect ((gpointer) checkbutton_value_bold, "toggled",
			  G_CALLBACK (on_checkbutton_value_bold_toggled),
			  window);
	g_signal_connect ((gpointer) checkbutton_unk_value_bold, "toggled",
			  G_CALLBACK (on_checkbutton_unk_value_bold_toggled),
			  window);
	g_signal_connect ((gpointer) checkbutton_important_bold, "toggled",
			  G_CALLBACK (on_checkbutton_important_bold_toggled),
			  window);
	g_signal_connect ((gpointer) checkbutton_directive_bold, "toggled",
			  G_CALLBACK (on_checkbutton_directive_bold_toggled),
			  window);
	g_signal_connect ((gpointer) checkbutton_string_double_bold,
			  "toggled",
			  G_CALLBACK
			  (on_checkbutton_string_double_bold_toggled), window);
	g_signal_connect ((gpointer) checkbutton_string_single_bold,
			  "toggled",
			  G_CALLBACK
			  (on_checkbutton_string_single_bold_toggled), window);
	g_signal_connect ((gpointer) checkbutton_class_bold, "toggled",
			  G_CALLBACK (on_checkbutton_class_bold_toggled),
			  window);
	g_signal_connect ((gpointer) checkbutton_id_bold, "toggled",
			  G_CALLBACK (on_checkbutton_id_bold_toggled), window);
	g_signal_connect ((gpointer) checkbutton_function_bold, "toggled",
			  G_CALLBACK (on_checkbutton_function_bold_toggled),
			  window);
	g_signal_connect ((gpointer) checkbutton_number_bold, "toggled",
			  G_CALLBACK (on_checkbutton_number_bold_toggled),
			  window);
	g_signal_connect ((gpointer) checkbutton_unit_bold, "toggled",
			  G_CALLBACK (on_checkbutton_unit_bold_toggled),
			  window);
	g_signal_connect ((gpointer) checkbutton_color_bold, "toggled",
			  G_CALLBACK (on_checkbutton_color_bold_toggled),
			  window);
	g_signal_connect ((gpointer) checkbutton_hexacolor_bold, "toggled",
			  G_CALLBACK (on_checkbutton_hexacolor_bold_toggled),
			  window);
	g_signal_connect ((gpointer) checkbutton_attr_match_bold, "toggled",
			  G_CALLBACK (on_checkbutton_attr_match_bold_toggled),
			  window);
	g_signal_connect ((gpointer) checkbutton_lang_bold, "toggled",
			  G_CALLBACK (on_checkbutton_lang_bold_toggled),
			  window);
	g_signal_connect ((gpointer) button_tag_bg, "clicked",
			  G_CALLBACK (on_button_tag_bg_clicked), window);
	g_signal_connect ((gpointer) button_pseudoclass_bg, "clicked",
			  G_CALLBACK (on_button_pseudoclass_bg_clicked),
			  window);
	g_signal_connect ((gpointer) button_unk_pseudoclass_bg, "clicked",
			  G_CALLBACK (on_button_unk_pseudoclass_bg_clicked),
			  window);
	g_signal_connect ((gpointer) button_operator_bg, "clicked",
			  G_CALLBACK (on_button_operator_bg_clicked), window);
	g_signal_connect ((gpointer) button_vproperty_bg, "clicked",
			  G_CALLBACK (on_button_vproperty_bg_clicked), window);
	g_signal_connect ((gpointer) button_unk_property_bg, "clicked",
			  G_CALLBACK (on_button_unk_property_bg_clicked),
			  window);
	g_signal_connect ((gpointer) button_value_bg, "clicked",
			  G_CALLBACK (on_button_value_bg_clicked), window);
	g_signal_connect ((gpointer) button_unk_value_bg, "clicked",
			  G_CALLBACK (on_button_unk_value_bg_clicked), window);
	g_signal_connect ((gpointer) button_important_bg, "clicked",
			  G_CALLBACK (on_button_important_bg_clicked), window);
	g_signal_connect ((gpointer) button_directive_bg, "clicked",
			  G_CALLBACK (on_button_directive_bg_clicked), window);
	g_signal_connect ((gpointer) button_string_double_bg, "clicked",
			  G_CALLBACK (on_button_string_double_bg_clicked),
			  window);
	g_signal_connect ((gpointer) button_string_single_bg, "clicked",
			  G_CALLBACK (on_button_string_single_bg_clicked),
			  window);
	g_signal_connect ((gpointer) button_class_bg, "clicked",
			  G_CALLBACK (on_button_class_bg_clicked), window);
	g_signal_connect ((gpointer) button_id_bg, "clicked",
			  G_CALLBACK (on_button_id_bg_clicked), window);
	g_signal_connect ((gpointer) button_function_bg, "clicked",
			  G_CALLBACK (on_button_function_bg_clicked), window);
	g_signal_connect ((gpointer) button_number_bg, "clicked",
			  G_CALLBACK (on_button_number_bg_clicked), window);
	g_signal_connect ((gpointer) button_unit_bg, "clicked",
			  G_CALLBACK (on_button_unit_bg_clicked), window);
	g_signal_connect ((gpointer) button_color_bg, "clicked",
			  G_CALLBACK (on_button_color_bg_clicked), window);
	g_signal_connect ((gpointer) button_hexacolor_bg, "clicked",
			  G_CALLBACK (on_button_hexacolor_bg_clicked), window);
	g_signal_connect ((gpointer) button_attr_match_bg, "clicked",
			  G_CALLBACK (on_button_attr_match_bg_clicked), window);
	g_signal_connect ((gpointer) button_lang_bg, "clicked",
			  G_CALLBACK (on_button_lang_bg_clicked), window);
	g_signal_connect ((gpointer) button_default_fg, "clicked",
			  G_CALLBACK (on_button_default_fg_clicked), window);
	// document settings
	g_signal_connect ((gpointer) checkbutton_line_numbers, "toggled",
			  G_CALLBACK (on_checkbutton_line_numbers_toggled),
			  window);  
	g_signal_connect ((gpointer) checkbutton_line_endings, "toggled",
			  G_CALLBACK (on_checkbutton_line_endings_toggled),
			  window); 
	g_signal_connect ((gpointer) checkbutton_white_spaces, "toggled",
			  G_CALLBACK (on_checkbutton_white_spaces_toggled),
			  window); 
	g_signal_connect ((gpointer) checkbutton_line_wrap, "toggled",
			  G_CALLBACK (on_checkbutton_line_wrap_toggled),
			  window); 
	g_signal_connect ((gpointer) checkbutton_auto_completion, "toggled",
			  G_CALLBACK (on_checkbutton_auto_completion_toggled),
			  window); 
	g_signal_connect ((gpointer) checkbutton_folding, "toggled",
			  G_CALLBACK (on_checkbutton_folding_toggled),
			  window);  
	g_signal_connect ((gpointer) checkbutton_usetabs, "toggled",
			  G_CALLBACK (on_checkbutton_usetabs_toggled),
			  window);   
	g_signal_connect ((gpointer) spinbutton_tabswidth, "value-changed",
			  G_CALLBACK (on_spinbutton_tabswidth_value_changed),
			  window);   
 
  
	// font
	g_signal_connect ((gpointer) apply_button, "clicked",
			G_CALLBACK (on_apply_font_button_clicked),
			window);
	// browsers

	/* Store pointers to some widgets, for use by lookup_widget(). */
	GLADE_HOOKUP_OBJECT_NO_REF (preferences_dialog, preferences_dialog, "preferences_dialog");
	GLADE_HOOKUP_OBJECT (preferences_dialog, notebook, "notebook");
	//
	GLADE_HOOKUP_OBJECT (preferences_dialog, entry_browser, "entry_browser");
	GLADE_HOOKUP_OBJECT (preferences_dialog, label_current_browser, "label_current_browser");		
	//
	GLADE_HOOKUP_OBJECT (preferences_dialog, fontsel, "fontsel");

	GLADE_HOOKUP_OBJECT (preferences_dialog, label_current, "label_current_font");
	GLADE_HOOKUP_OBJECT (preferences_dialog, button_vproperty_fg, "button_vproperty_fg");				 
	GLADE_HOOKUP_OBJECT (preferences_dialog, button_operator_fg, "button_operator_fg");
	GLADE_HOOKUP_OBJECT (preferences_dialog, button_unk_pseudoclass_fg, "button_unk_pseudoclass_fg");
	GLADE_HOOKUP_OBJECT (preferences_dialog, button_pseudoclass_fg, "button_pseudoclass_fg");
	GLADE_HOOKUP_OBJECT (preferences_dialog, button_tag_fg, "button_tag_fg");
	GLADE_HOOKUP_OBJECT (preferences_dialog, button_default_bg, "button_default_bg");
	GLADE_HOOKUP_OBJECT (preferences_dialog, button_unk_property_fg, "button_unk_property_fg");
	GLADE_HOOKUP_OBJECT (preferences_dialog, button_value_fg, "button_value_fg");
	GLADE_HOOKUP_OBJECT (preferences_dialog, button_unk_value_fg, "button_unk_value_fg");
	GLADE_HOOKUP_OBJECT (preferences_dialog, button_important_fg, "button_important_fg");
	GLADE_HOOKUP_OBJECT (preferences_dialog, button_directive_fg, "button_directive_fg");
	GLADE_HOOKUP_OBJECT (preferences_dialog, button_string_double_fg, "button_string_double_fg");
	GLADE_HOOKUP_OBJECT (preferences_dialog, button_string_single_fg, "button_string_single_fg");
	GLADE_HOOKUP_OBJECT (preferences_dialog, button_class_fg, "button_class_fg");
	GLADE_HOOKUP_OBJECT (preferences_dialog, button_id_fg, "button_id_fg");
	GLADE_HOOKUP_OBJECT (preferences_dialog, button_function_fg, "button_function_fg");
	GLADE_HOOKUP_OBJECT (preferences_dialog, button_number_fg, "button_number_fg");
	GLADE_HOOKUP_OBJECT (preferences_dialog, button_unit_fg, "button_unit_fg");
	GLADE_HOOKUP_OBJECT (preferences_dialog, button_color_fg, "button_color_fg");
	GLADE_HOOKUP_OBJECT (preferences_dialog, button_hexacolor_fg, "button_hexacolor_fg");
	GLADE_HOOKUP_OBJECT (preferences_dialog, button_attr_match_fg, "button_attr_match_fg");
	GLADE_HOOKUP_OBJECT (preferences_dialog, button_lang_fg, "button_lang_fg");
	GLADE_HOOKUP_OBJECT (preferences_dialog, checkbutton_default_bold, "checkbutton_default_bold");
	GLADE_HOOKUP_OBJECT (preferences_dialog, checkbutton_tag_bold, "checkbutton_tag_bold");
	GLADE_HOOKUP_OBJECT (preferences_dialog, checkbutton_pseudoclass_bold, "checkbutton_pseudoclass_bold");
	GLADE_HOOKUP_OBJECT (preferences_dialog, checkbutton_unk_pseudoclass_bold, "checkbutton_unk_pseudoclass_bold");
	GLADE_HOOKUP_OBJECT (preferences_dialog, checkbutton_operator_bold, "checkbutton_operator_bold");
	GLADE_HOOKUP_OBJECT (preferences_dialog, checkbutton_vproperty_bold, "checkbutton_vproperty_bold");
	GLADE_HOOKUP_OBJECT (preferences_dialog, checkbutton_unk_property_bold, "checkbutton_unk_property_bold");
	GLADE_HOOKUP_OBJECT (preferences_dialog, checkbutton_value_bold, "checkbutton_value_bold");
	GLADE_HOOKUP_OBJECT (preferences_dialog, checkbutton_unk_value_bold, "checkbutton_unk_value_bold");
	GLADE_HOOKUP_OBJECT (preferences_dialog, checkbutton_important_bold, "checkbutton_important_bold");
	GLADE_HOOKUP_OBJECT (preferences_dialog, checkbutton_directive_bold, "checkbutton_directive_bold");
	GLADE_HOOKUP_OBJECT (preferences_dialog, checkbutton_string_double_bold, "checkbutton_string_double_bold");
	GLADE_HOOKUP_OBJECT (preferences_dialog, checkbutton_string_single_bold, "checkbutton_string_single_bold");
	GLADE_HOOKUP_OBJECT (preferences_dialog, checkbutton_class_bold, "checkbutton_class_bold");
	GLADE_HOOKUP_OBJECT (preferences_dialog, checkbutton_id_bold, "checkbutton_id_bold");
	GLADE_HOOKUP_OBJECT (preferences_dialog, checkbutton_function_bold, "checkbutton_function_bold");
	GLADE_HOOKUP_OBJECT (preferences_dialog, checkbutton_number_bold, "checkbutton_number_bold");
	GLADE_HOOKUP_OBJECT (preferences_dialog, checkbutton_unit_bold, "checkbutton_unit_bold");
	GLADE_HOOKUP_OBJECT (preferences_dialog, checkbutton_color_bold, "checkbutton_color_bold");
	GLADE_HOOKUP_OBJECT (preferences_dialog, checkbutton_hexacolor_bold, "checkbutton_hexacolor_bold");
	GLADE_HOOKUP_OBJECT (preferences_dialog, checkbutton_attr_match_bold, "checkbutton_attr_match_bold");
	GLADE_HOOKUP_OBJECT (preferences_dialog, checkbutton_lang_bold, "checkbutton_lang_bold");
	GLADE_HOOKUP_OBJECT (preferences_dialog, button_tag_bg, "button_tag_bg");
	GLADE_HOOKUP_OBJECT (preferences_dialog, button_pseudoclass_bg, "button_pseudoclass_bg");
	GLADE_HOOKUP_OBJECT (preferences_dialog, button_unk_pseudoclass_bg, "button_unk_pseudoclass_bg");
	GLADE_HOOKUP_OBJECT (preferences_dialog, button_operator_bg, "button_operator_bg");
	GLADE_HOOKUP_OBJECT (preferences_dialog, button_vproperty_bg, "button_vproperty_bg");
	GLADE_HOOKUP_OBJECT (preferences_dialog, button_unk_property_bg, "button_unk_property_bg");
	GLADE_HOOKUP_OBJECT (preferences_dialog, button_value_bg, "button_value_bg");
	GLADE_HOOKUP_OBJECT (preferences_dialog, button_unk_value_bg, "button_unk_value_bg");
	GLADE_HOOKUP_OBJECT (preferences_dialog, button_important_bg, "button_important_bg");
	GLADE_HOOKUP_OBJECT (preferences_dialog, button_directive_bg, "button_directive_bg");
	GLADE_HOOKUP_OBJECT (preferences_dialog, button_string_double_bg, "button_string_double_bg");
	GLADE_HOOKUP_OBJECT (preferences_dialog, button_string_single_bg, "button_string_single_bg");
	GLADE_HOOKUP_OBJECT (preferences_dialog, button_class_bg, "button_class_bg");
	GLADE_HOOKUP_OBJECT (preferences_dialog, button_id_bg, "button_id_bg");
	GLADE_HOOKUP_OBJECT (preferences_dialog, button_function_bg, "button_function_bg");
	GLADE_HOOKUP_OBJECT (preferences_dialog, button_number_bg, "button_number_bg");
	GLADE_HOOKUP_OBJECT (preferences_dialog, button_unit_bg, "button_unit_bg");
	GLADE_HOOKUP_OBJECT (preferences_dialog, button_color_bg, "button_color_bg");
	GLADE_HOOKUP_OBJECT (preferences_dialog, button_hexacolor_bg, "button_hexacolor_bg");
	GLADE_HOOKUP_OBJECT (preferences_dialog, button_attr_match_bg, "button_attr_match_bg");
	GLADE_HOOKUP_OBJECT (preferences_dialog, button_lang_bg, "button_lang_bg");
	GLADE_HOOKUP_OBJECT (preferences_dialog, button_default_fg, "button_default_fg");
	GLADE_HOOKUP_OBJECT (preferences_dialog, checkbutton_line_numbers, "checkbutton_line_numbers");
	GLADE_HOOKUP_OBJECT (preferences_dialog, checkbutton_line_endings, "checkbutton_line_endings");
	GLADE_HOOKUP_OBJECT (preferences_dialog, checkbutton_white_spaces, "checkbutton_white_spaces");
	GLADE_HOOKUP_OBJECT (preferences_dialog, checkbutton_line_wrap, "checkbutton_line_wrap");
	GLADE_HOOKUP_OBJECT (preferences_dialog, checkbutton_auto_completion, "checkbutton_auto_completion");
	GLADE_HOOKUP_OBJECT (preferences_dialog, checkbutton_folding, "checkbutton_folding");
	GLADE_HOOKUP_OBJECT (preferences_dialog, cancelbutton, "cancelbutton");
	GLADE_HOOKUP_OBJECT (preferences_dialog, okbutton, "okbutton");
	GLADE_HOOKUP_OBJECT_NO_REF (preferences_dialog, dialog_action_area, "dialog_action_area");

	return preferences_dialog;
}
