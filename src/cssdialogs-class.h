/*  cssed (c) Iago Rubio, 2003-2005 - A tiny CSS editor.
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

/** @file cssdialogs-class.h
 *  @brief Header file for cssed dialog classes used by auto completion. 
 */
 
/** @typedef CssDialogClass.  
 *  @brief A utility data type for ::_cssdialog_class structures.
 */
/** @struct _cssdialog_class.
 *  @brief This structure implements a dialog to be used by auto completion.
 */
typedef struct _CssDialogClass {
	/** The keyword. This will be freed with g_free so you must allocate it ( you can use g_strdup for this ) */
	gchar* keyword;	
	/** The dialog creation function. It must return a dialog widget to be shown on screen */
	GtkWidget* (*create_function) ( CssedWindow*, gchar *, gboolean, gboolean);
	/** The dialog get_string function. All those dialogs only do one thing, build a string to be added to the document.
		This function must return this string when the dialog is closed using the OK button. The string will be freed with 
		g_free, so it must be allocated	*/
	gchar* (*get_string_function) ( CssedWindow*, gpointer );
} CssDialogClass;

/** Returns a pointer to a new CssDialogClass. 
 *	It will reserve the memory for you, and this memory will be disposed by cssed so you don't need to release it.
 *  @return A pointer to reserved enough memory for a CssDialogClass structure.
 */
CssDialogClass*
css_dialog_class_new();
