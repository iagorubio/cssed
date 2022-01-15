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
# include <config.h>
#endif

#include <string.h>
#include <gtk/gtk.h>

#include "cssedwindow.h"
#include "document.h"
#include "support.h"
#include "utils.h"
#include "debug.h"
#include "file-type-manager.h"

#define g_slist_first(n) g_slist_nth (n,0)

// FILE TYPE IMPLEMENTATION
CssedFileType*
cssed_file_type_new()
{
	CssedFileType* type;
	type = g_malloc0(sizeof(CssedFileType));
	return type;
}

void
cssed_file_type_free_item (gpointer item, gpointer user_data)
{
	g_free(item);
}

void
cssed_file_type_free(CssedFileType* type)
{
	if( !type ) return;
	g_free(type->label_language);
	g_free(type->lable_file);
	g_slist_foreach(type->patterns, cssed_file_type_free_item, NULL);
	g_slist_free(type->patterns);
	g_free(type);
}

void
cssed_file_type_add_pattern_spec(CssedFileType* type, gchar* spec)
{
	type->patterns = g_slist_append (type->patterns, (gpointer) g_strdup(spec));
}

//FILE TYPE MANAGER IMPLEMENTATION
gboolean
patterns_hash_table_key_equal_func(gconstpointer a,
                                   gconstpointer b)
{
	if( !a && !b ) return TRUE;
	if( !a ) return FALSE;
	if( !b ) return FALSE;
	if( strcmp((gchar*) a, (gchar*) b) == 0 ) return TRUE;
	return FALSE;
}

void
patterns_hash_table_free_entry	   (gpointer key,
                                    gpointer value,
                                    gpointer user_data)
{
	GSList *list;
	list = g_slist_first((GSList*) value);
	while(list)
	{
		g_free(list->data);
		list = g_slist_next(list);
	}
	g_slist_free (list);
	g_free(key);
}

CssedFileTypeManager*
cssed_file_type_manager_new()
{
	CssedFileTypeManager* manager;

	manager = g_malloc0(sizeof(CssedFileTypeManager));
	memset (manager->filetype, 0, sizeof(gpointer) * CSSED_NUMFILETYPES);
	manager->patterns_hash_table = g_hash_table_new (g_str_hash, g_str_equal);

	return manager;
}

void
cssed_file_type_manager_free( CssedFileTypeManager *manager )
{
	gint i;

	if(!manager) return;

	g_hash_table_foreach(manager->patterns_hash_table, patterns_hash_table_free_entry, NULL);
	g_hash_table_destroy(manager->patterns_hash_table);
	// now free all file types
	for(i=0;i<CSSED_NUMFILETYPES;i++){
		if( manager->filetype[i] != NULL )
			cssed_file_type_free(manager->filetype[i]);
	}
	g_free(manager);
}

gboolean
patterns_hash_table_find_func   (gpointer key,
                                 gpointer value,
                                 gpointer user_data)
{
	gchar *filename;
	gchar *toupper_pattern;
	gchar *pattern;
	gboolean match;

	filename = (gchar*) user_data;
	pattern = (gchar*) key;
	
	if( !filename ) return FALSE;
	match = g_pattern_match_simple( pattern, filename);
	DBGMSG (__FILE__,__LINE__,"Pattern matching:\n%s with %s (%s)", filename, pattern, match?"MATCH !!":"NO");
	if( match ) return TRUE;
	
	// check for uppercase strings, mixed case is not managed
	if( g_ascii_isupper(filename[strlen(filename)-1]) ){
		toupper_pattern  = g_ascii_strup(pattern, -1);
		match = g_pattern_match_simple(toupper_pattern, filename);
		DBGMSG (__FILE__,__LINE__,"Pattern matching:\n%s with %s (%s)", filename, (gchar*) toupper_pattern, match?"MATCH !!":"NO");
		g_free(toupper_pattern);
		if( match ) return TRUE;
	}
	
	return FALSE;
}

GSList*
cssed_file_type_manager_get_ids_from_filename (CssedFileTypeManager *manager, gchar *filename)
{
	GSList *ids; // GList containing all file type ids that matches the file
	GHashTable* hash;

	hash = manager->patterns_hash_table;
	ids = (GSList*) g_hash_table_find (hash, patterns_hash_table_find_func, filename);
	return ids;
}


GSList*
cssed_file_type_manager_get_ids_from_pattern( CssedFileTypeManager *manager, gchar *pattern )
{
	GSList *ids; // GList containing all file type ids that mathes the file
	GHashTable* hash;

	hash = manager->patterns_hash_table;
	ids = (GSList*) g_hash_table_lookup (hash, pattern);
	return ids;
}

CssedFileTypeId
cssed_file_type_manager_get_default_id_from_filename( CssedFileTypeManager *manager, gchar *filename )
{
	gpointer ids; // GSList containing all file type ids that mathes the file
	GSList* idlist;

	if( !filename ) return CSSED_FILETYPE_UNKNOW; 
	if( !manager )  return CSSED_FILETYPE_UNKNOW; 

	// lookup on patterns hash table
	ids = cssed_file_type_manager_get_ids_from_filename (manager, filename);
	if( !ids ){
		return CSSED_FILETYPE_UNKNOW;
	}else{
		idlist = g_slist_first( ids ); // the order matters, first is default
		if( !idlist ) return CSSED_FILETYPE_UNKNOW;
		else return *((CssedFileTypeId*) idlist->data);
	}
}

CssedFileType*
cssed_file_type_manager_get_filetype_from_filename( CssedFileTypeManager *manager, gchar *filename )
{
	gint defaultid;

	defaultid = cssed_file_type_manager_get_default_id_from_filename( manager, filename );
	if( defaultid == CSSED_FILETYPE_UNKNOW ) defaultid = CSSED_FILETYPE_TEXT;
	return manager->filetype[defaultid];
}

CssedFileType*
cssed_file_type_manager_get_filetype_from_id( CssedFileTypeManager *manager, CssedFileTypeId id )
{
	if( id <= CSSED_FILETYPE_UNKNOW || id >= CSSED_NUMFILETYPES ) return NULL;
	return manager->filetype[id];
}

void
cssed_file_type_manager_add_pattern( CssedFileTypeManager *manager, gchar *pattern, CssedFileTypeId id )
{
	GHashTable *hash;
	GSList *val = NULL;
	gchar *key;
	gint *idptr;

	if( !manager ) return; // no NULLs please
	if( !pattern ) return; // no NULLs please
	hash = manager->patterns_hash_table;	
	if( !g_hash_table_lookup_extended (hash, pattern, (gpointer) &key, (gpointer) &val) )
			key = g_strdup(pattern);
	idptr = g_new0(gint,1);
	*idptr = id;
	val = g_slist_append(val, idptr);
	g_hash_table_insert(hash, key, val);
}

gboolean
cssed_file_type_manager_delete_pattern( CssedFileTypeManager *manager, gchar *pattern )
{
	GSList *val = NULL;
	gchar *key;
	GHashTable *hash;

	hash = manager->patterns_hash_table;

	if( g_hash_table_lookup_extended (hash, pattern, (gpointer) &key, (gpointer) &val) ){
		g_hash_table_remove (hash, (gpointer) &key);
		patterns_hash_table_free_entry ((gpointer) key,	(gpointer) val,	NULL);
		return TRUE;
	}else{
		return FALSE;
	}
}

gboolean
cssed_file_type_manager_pattern_exists( CssedFileTypeManager *manager, gchar *pattern )
{
	if( g_hash_table_lookup(manager->patterns_hash_table, pattern) != NULL ) return TRUE;
	else return FALSE;
}

/* *********************************************************************
  This is to use by callback as a file type menu item.
  It should be set as menu item's data the filetype.
  Use the function below to set the data. 
* ******************************************************************** */
void
on_file_type_menu_activate (GtkMenuItem * menuitem, gpointer user_data)
{
	CssedFileType* filetype;
	CssedWindow* window;
	CssedDoc* doc;

	filetype = g_object_get_data(G_OBJECT(menuitem), "filetype");
	if ( filetype ){
		window = CSSED_WINDOW(user_data);
		doc = document_get_current(window);
		document_set_filetype( doc, filetype );
	}
}

void
cssed_file_type_menu_set_default_callback (CssedWindow *window, GtkWidget *filetype_menu, CssedFileType *filetype)
{
	g_object_set_data(G_OBJECT(filetype_menu), "filetype", filetype);
	g_signal_connect ((gpointer) filetype_menu, "activate",
			  G_CALLBACK (on_file_type_menu_activate), window);
}
