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
 
typedef struct _cssed_treeview_properties_data {
	GtkTreeIter* first_col;
	GtkTreeIter* second_col;
	GtkTreeIter* thrid_col;
	GtkTreeStore* store;
	CssedWindow* window;
	gchar *filename;
} Tcssed_tp_data;
	
void
parse_css_definition_and_fill_tree_view(Tcssed_tp_data* data);
