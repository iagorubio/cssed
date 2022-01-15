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

#ifndef PLAT_GTK
#   define PLAT_GTK 1
#endif

// this is the line numbers width template
#ifndef MARGIN_LINES_WIDTH_STRING
# define MARGIN_LINES_WIDTH_STRING "_99999"
#endif
// markers margin width in pixels
#ifndef MARGIN_MARKERS_WIDTH_PIXELS
# define MARGIN_MARKERS_WIDTH_PIXELS 14
#endif
// folding margin width in pixels
#ifndef MARGIN_FOLDING_WIDTH_PIXELS
# define MARGIN_FOLDING_WIDTH_PIXELS 12
#endif

// drag and drop on sheets' buttons
#ifndef WITH_DRAGABLE_SHEETS
# define WITH_DRAGABLE_SHEETS
#endif

#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>

#ifndef WIN32
# include <sys/mman.h>
# include <libgen.h>
# include <unistd.h>
#endif

#include <Scintilla.h>
#include <SciLexer.h>
#include <ScintillaWidget.h>

#include "cssedwindow.h"
#include "document.h"
#include "sciwrappers.h"
#include "file-type-manager.h"
#include "callbacks.h"
#include "cssdialogs-interface.h"
#include "interface.h"
#include "support.h"
#include "debug.h"
#include "cssdialogs-class.h"
#include "utils.h"

/*






*/

/* scintilla object related functions */
ScintillaObject*
sci_get_from_notebook_page(GtkNotebook* notebook, gint page_number)
{
	ScintillaObject* sci;
	GtkWidget* editor;

	editor = gtk_notebook_get_nth_page (notebook, page_number);
	sci = SCINTILLA(editor);
	return sci;
}

/* line numbers visibility */
void
sci_set_line_numbers(ScintillaObject * sci, gboolean set )
{
	if( set ){
			SSM (sci, SCI_SETMARGINWIDTHN, 0,
				SSM(sci, SCI_TEXTWIDTH, STYLE_LINENUMBER,
				(sptr_t) MARGIN_LINES_WIDTH_STRING));
	}else{
		SSM (sci, SCI_SETMARGINWIDTHN, 0, 0 );
	}
}
gboolean
sci_get_line_numbers(ScintillaObject * sci)
{
	gint margin_width;

	margin_width = SSM(sci, SCI_GETMARGINWIDTHN, 0, 0);
	if( margin_width > 0 ) return TRUE;
	else return FALSE;
}
/* folding margin visibility */
void
sci_set_folding_margin_visible (ScintillaObject * sci, gboolean set )
{
	if( set ){
		SSM (sci, SCI_SETMARGINWIDTHN, 2, MARGIN_FOLDING_WIDTH_PIXELS);
	}else{
		SSM (sci, SCI_SETMARGINWIDTHN, 2, 0);
	}
}
gboolean
sci_get_folding_margin_visible(ScintillaObject * sci)
{
	gint margin_width;

	margin_width = SSM(sci, SCI_GETMARGINWIDTHN, 2, 0);
	if( margin_width > 0 ) return TRUE;
	else return FALSE;
}
/* tabs */
void
sci_set_use_tabs (ScintillaObject * sci, gboolean set )
{
	SSM (sci, SCI_SETUSETABS, set, 0);
}

gboolean
sci_get_use_tabs (ScintillaObject * sci)
{
 	return SSM (sci, SCI_GETUSETABS, 0, 0);   
}

void
sci_set_tabs_width (ScintillaObject * sci, gint set )
{
  	SSM (sci, SCI_SETTABWIDTH, set, 0);  
}

gint
sci_get_tabs_width (ScintillaObject * sci)
{
  	return SSM (sci, SCI_GETTABWIDTH, 0, 0); 
}

/* end of lines */
void
sci_set_visible_eols(ScintillaObject* sci, gboolean set )
{
	SSM(sci,SCI_SETVIEWEOL,set,0);
}

gboolean
sci_get_visible_eols(ScintillaObject* sci)
{
	return SSM(sci, SCI_GETVIEWEOL,0,0);
}
void
sci_set_visible_white_spaces(ScintillaObject* sci, gboolean set )
{
	if(set){
		SSM(sci,SCI_SETVIEWWS,SCWS_VISIBLEALWAYS,0);
	}else{
		SSM(sci,SCI_SETVIEWWS,SCWS_INVISIBLE,0);
	}
}
gboolean
sci_get_visible_white_spaces(ScintillaObject* sci)
{
	return SSM(sci, SCI_GETVIEWWS,0,0);
}
void
sci_set_lines_wrapped(ScintillaObject* sci, gboolean set )
{
	if( set ){
		SSM(sci,SCI_SETWRAPMODE,SC_WRAP_WORD,0);
	}else{
		SSM(sci,SCI_SETWRAPMODE,SC_WRAP_NONE,0);
	}
}
gboolean
sci_get_lines_wrapped(ScintillaObject* sci)
{
	return SSM(sci, SCI_GETWRAPMODE,0,0);
}

gint
sci_get_eol_mode( ScintillaObject* sci)
{
	return SSM(sci, SCI_GETEOLMODE, 0, 0);
}

void
sci_set_eol_mode( ScintillaObject* sci, gint eolmode)
{
	SSM(sci, SCI_SETEOLMODE, eolmode, 0);
}

void
sci_convert_eols( ScintillaObject* sci, gint eolmode)
{
	SSM(sci, SCI_CONVERTEOLS, eolmode,0);
}


void
sci_add_text(ScintillaObject* sci, gchar* text)
{
	if( text != NULL ){// if null text is passed to scintilla will segfault
		SSM(sci, SCI_ADDTEXT, strlen(text) , (sptr_t) text);
	}
}

gboolean
sci_can_undo( ScintillaObject* sci )
{
	return SSM(sci, SCI_CANUNDO, 0, 0);
}

gboolean
sci_can_redo( ScintillaObject* sci )
{
	return SSM(sci, SCI_CANREDO, 0, 0);
}

void
sci_undo( ScintillaObject* sci )
{
	if( sci_can_undo(sci) ){
		SSM(sci, SCI_UNDO, 0, 0);
	}else{ // change it to a document function
		cssed_error_message(_("Undo limit reached"),
							_("There is no action in the undo list."));
	}
}

void
sci_redo( ScintillaObject* sci )
{
	if( sci_can_redo( sci ) ){
		SSM(sci, SCI_REDO,0,0);
	}else{// change it to a document function
		cssed_error_message(_("Redo limit reached"),
							_("There is no action in the redo list."));
	}
}

void
sci_start_undo_action( ScintillaObject* sci )
{
	SSM(sci,SCI_BEGINUNDOACTION,0,0 );
}

void
sci_end_undo_action( ScintillaObject* sci )
{
	SSM(sci, SCI_ENDUNDOACTION,0,0);
}
void
sci_set_undo_collection( ScintillaObject* sci, gboolean set )
{
	SSM(sci, SCI_SETUNDOCOLLECTION,set,0);
}

gboolean
sci_get_undo_collection( ScintillaObject* sci )
{
	return SSM(sci, SCI_GETUNDOCOLLECTION,0,0);
}

void
sci_empty_undo_buffer( ScintillaObject* sci )
{
	SSM(sci, SCI_EMPTYUNDOBUFFER,0,0);
}

void
sci_zoom_in( ScintillaObject* sci )
{
	SSM(sci, SCI_ZOOMIN,0,0);
}

void
sci_zoom_out( ScintillaObject* sci )
{
	SSM(sci, SCI_ZOOMOUT,0,0);
}

void
sci_zoom_off( ScintillaObject* sci )
{
	SSM(sci, SCI_SETZOOM,0,0);
}
void
sci_set_marker_at_line( ScintillaObject* sci, gint line_number, gboolean set, gint marker )
{
	DBGMSG (__FILE__,__LINE__,"Set marker to %d line %d \n",set,line_number);

	if( set ){
		SSM(sci, SCI_MARKERADD,line_number, marker);
	}else{
		SSM(sci, SCI_MARKERDELETE,line_number, marker);
	}
}
gboolean
sci_is_marker_set_at_line(ScintillaObject* sci, gint line, gint marker)
{
	gint state;

	state = SSM(sci, SCI_MARKERGET, line, marker );
	return((state & (1 << marker)));
}

gboolean
sci_marker_next(ScintillaObject* sci, gint line, gint marker_mask)
{
	gint marker_line;

	marker_line = SSM(sci, SCI_MARKERNEXT, line, marker_mask);
	DBGMSG (__FILE__,__LINE__, "Marker next: line %d next marker %d",line,marker_line);

	if( marker_line != -1 ){
		SSM(sci,SCI_GOTOLINE,marker_line,0);
		return TRUE;
	}else{
		return FALSE;
	}
}

gboolean
sci_marker_prev(ScintillaObject* sci, gint line, gint marker_mask)
{
	gint marker_line;

	marker_line = SSM(sci, SCI_MARKERPREVIOUS, line, marker_mask);
	DBGMSG (__FILE__,__LINE__, "Marker previous: line %d marker %d",line,marker_line);

	if( marker_line != -1 ){
		SSM(sci,SCI_GOTOLINE,marker_line,0);
		return TRUE;
	}else{
		return FALSE;
	}
}

gint
sci_get_line_from_position(ScintillaObject* sci, gint position )
{
	return SSM(sci, SCI_LINEFROMPOSITION, position, 0);
}

gint
sci_get_position_from_line(ScintillaObject* sci, gint line )
{
	return SSM(sci, SCI_POSITIONFROMLINE, line, 0);
}

gint
sci_get_current_position(ScintillaObject* sci )
{
	return SSM(sci, SCI_GETCURRENTPOS, 0, 0);
}

void
sci_set_current_position(ScintillaObject* sci, gint position )
{
	SSM(sci, SCI_GOTOPOS, position, 0);
}

void
sci_set_current_line(ScintillaObject* sci, gint line )
{
	SSM(sci, SCI_GOTOLINE, line, 0);
}

gint
sci_get_line_count( ScintillaObject* sci )
{
	return SSM(sci, SCI_GETLINECOUNT, 0, 0);
}

void
sci_set_selection_start(ScintillaObject* sci, gint position)
{
	SSM(sci, SCI_SETSELECTIONSTART, position, 0);
}

void
sci_set_selection_end(ScintillaObject* sci, gint position)
{
	SSM(sci, SCI_SETSELECTIONEND, position, 0);
}

gint
sci_get_line_end_from_position(ScintillaObject* sci, gint position)
{
	return SSM(sci, SCI_GETLINEENDPOSITION, position, 0);
}

void
sci_cut(ScintillaObject* sci)
{
	SSM(sci, SCI_CUT, 0, 0);
}

void
sci_copy(ScintillaObject* sci)
{
	SSM(sci, SCI_COPY, 0, 0);
}
void
sci_paste(ScintillaObject* sci)
{
	SSM(sci, SCI_PASTE, 0, 0);
}
gint
sci_get_selection_start(ScintillaObject* sci)
{
	return SSM(sci, SCI_GETSELECTIONSTART,0,0);
}

gint
sci_get_selection_end(ScintillaObject* sci)
{
	return SSM(sci, SCI_GETSELECTIONEND,0,0);
}

void
sci_replace_sel(ScintillaObject* sci, gchar* text)
{
	SSM(sci, SCI_REPLACESEL,0, (sptr_t) text);
}

void
sci_replace_sel_re(ScintillaObject* sci, gchar* text)
{
	gint len, selstart;

	SSM (sci, SCI_TARGETFROMSELECTION, 0, 0);
	selstart = SSM(sci, SCI_GETSELECTIONSTART,0,0);
	len = SSM (sci, SCI_REPLACETARGETRE, -1, (sptr_t) text);
	SSM(sci, SCI_SETSELECTIONEND,  selstart+len, 0);	
}

gint
sci_get_length(ScintillaObject* sci)
{
	return SSM(sci,SCI_GETLENGTH,0,0);
}
gint
sci_line_length(ScintillaObject* sci,gint line)
{
	return SSM(sci,SCI_LINELENGTH, line,0);
}
void // will not be null terminated
sci_get_line(ScintillaObject* sci, gint line, gchar* text)
{
	SSM(sci,SCI_GETLINE,line, (sptr_t) text);
}
void // the last char will be null terminated
sci_get_text(ScintillaObject* sci, gint len, gchar* text)
{
	SSM(sci, SCI_GETTEXT, len,(sptr_t) text );
}

gchar
sci_get_char_at(ScintillaObject* sci, gint pos)
{
	return (gchar) SSM(sci, SCI_GETCHARAT, pos, 0);
}

void
sci_get_selected_text(ScintillaObject* sci, gchar* text)
{
	SSM(sci, SCI_GETSELTEXT, 0, (sptr_t) text);
}

void
sci_get_xy_from_position(ScintillaObject* sci,gint pos, gint* x, gint* y)
{
	*x = SSM(sci, SCI_POINTXFROMPOSITION,0, (int) pos);
 	*y = SSM(sci, SCI_POINTYFROMPOSITION,0, (int) pos);
}
/* folding */
gboolean
sci_get_line_is_visible(ScintillaObject* sci, gint line)
{
	return SSM(sci,SCI_GETLINEVISIBLE, line,0);
}
void
sci_ensure_line_is_visible( ScintillaObject* sci, gint line)
{
	 SSM(sci,SCI_ENSUREVISIBLE,line,0);
}

gint
sci_get_fold_level(ScintillaObject* sci, gint line)
{
	return SSM(sci,SCI_GETFOLDLEVEL, line,0);
}

void
sci_toggle_fold(ScintillaObject* sci, gint line)
{
	SSM(sci, SCI_TOGGLEFOLD, line, 1);
}

gboolean
sci_get_fold_expanded(ScintillaObject* sci, gint line)
{
	return SSM(sci, SCI_GETFOLDEXPANDED, line, 0);
}

void
sci_colourise( ScintillaObject* sci, gint start, gint end)
{
	SSM(sci, SCI_COLOURISE, start, end);
}

// call tips
void
sci_calltipshow ( ScintillaObject* sci, gint start, gchar* definition)
{
	SSM(sci, SCI_CALLTIPSHOW, start, (sptr_t) definition);
}

void
sci_calltipcancel ( ScintillaObject* sci )
{
	SSM(sci, SCI_CALLTIPCANCEL, 0, 0);
}

gboolean
sci_calltipactive ( ScintillaObject* sci )
{
	return SSM(sci, SCI_CALLTIPACTIVE, 0, 0);
}

gint
sci_calltiposstart ( ScintillaObject* sci )
{
	return SSM(sci, SCI_CALLTIPPOSSTART, 0, 0);
}


void
sci_grab_focus(ScintillaObject* sci)
{
	SSM(sci, SCI_GRABFOCUS, 0, 0);
}

