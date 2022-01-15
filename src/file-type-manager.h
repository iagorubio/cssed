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

/* this is just eye candy to set the file type as data on
   the menu item, to use the function cssed_file_type_menu_set_default_callback()
   as menu item callback  */
#define cssed_file_type_menu_set_filetype(n, w) g_object_set_data(G_OBJECT(n), "filetype", w);

CssedFileType*
cssed_file_type_new();

void
cssed_file_type_free(CssedFileType* type);

void
cssed_file_type_add_pattern_spec(CssedFileType* type, gchar* spec);


CssedFileTypeManager*
cssed_file_type_manager_new();

void
cssed_file_type_manager_free( CssedFileTypeManager *manager );

GSList*
cssed_file_type_manager_get_ids_from_filename (CssedFileTypeManager *manager, gchar *filename);

CssedFileType*
cssed_file_type_manager_get_filetype_from_filename( CssedFileTypeManager *manager, gchar *filename );

CssedFileType*
cssed_file_type_manager_get_filetype_from_id( CssedFileTypeManager *manager, CssedFileTypeId id );

GSList*
cssed_file_type_manager_get_ids_from_pattern( CssedFileTypeManager *manager, gchar *pattern );

CssedFileTypeId
cssed_file_type_manager_get_default_id_from_filename( CssedFileTypeManager *manager, gchar *filename );

void
cssed_file_type_manager_add_pattern( CssedFileTypeManager *manager, gchar *pattern, CssedFileTypeId id );

gboolean
cssed_file_type_manager_delete_pattern( CssedFileTypeManager *manager, gchar *pattern );

gboolean
cssed_file_type_manager_pattern_exists( CssedFileTypeManager *manager, gchar *pattern );

void
cssed_file_type_manager_add_filetype ( CssedFileTypeManager *manager, CssedFileType* type );

// this is used as a callbak for the menu item that CssedFileType adds to the main menu
//void
//on_file_type_menu_activate (GtkMenuItem * menuitem, gpointer user_data);

void
cssed_file_type_menu_set_default_callback(CssedWindow *window, GtkWidget *filetype_menu, CssedFileType *filetype);

