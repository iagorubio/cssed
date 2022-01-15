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

#ifdef WIN32
# include <windows.h>
#else
# include <unistd.h>
#endif

#include <sys/types.h>
#include <string.h>

#include <gtk/gtk.h>

#include "cssedwindow.h"
#include "configparser.h"
#include "interface.h"
#include "document.h"
#include "support.h"
#include "utils.h"
#include "debug.h"

#ifdef WITH_HELP_MENUS
# include <stdlib.h>
#endif


#ifdef WITH_IPC_QUEUE
#ifdef DARWIN
# undef WITH_IPC_QUEUE
#else
# include <sys/types.h>
# include <sys/ipc.h>
# include <sys/msg.h>
# include <pthread.h>
# include <signal.h>
# include <limits.h>

# define CSSED_IPC_MSG_QUEUE 5423623

struct ipcmsg
{
	long type;
	char file[PATH_MAX+1];
};

struct ipcproc_data
{
	CssedWindow *window;
	gint ident;
	gboolean run;
};

gpointer listen_queue_proc (gpointer * ident);
#endif
#endif

#ifdef WIN32
char *package_prefix;
char *package_datadir;
char *package_libdir;
#ifdef NDEBUG
static void nulloutput(const gchar* s){};
static void null_log(const gchar* dom, GLogLevelFlags l, const gchar* m, gpointer d){};
#endif
#endif

gboolean
apply_stored_pos_on_idle (gpointer window)
{
	// apply paned positions stored on the configuration	
	cssed_window_apply_stored_paned_positions(CSSED_WINDOW(window)); //PANEDBUG
	return FALSE;
}
int
main (int argc, char *argv[])
{
	GtkWidget *window;				// the main window
	CssedWindow *cssedwin;		// the main window object
	gint i;							// to parse arguments
	GList* plugins_to_load = NULL;	// list wilth all plugins to be loaded when the window is created.
	gchar* full_path;
	gchar* curdir;
#ifdef WIN32
    gchar *temp;
#endif
#ifdef WITH_IPC_QUEUE
	gint identifier;
	key_t clave;
	struct ipcmsg buff_msg;
	struct ipcproc_data pdata;
	gboolean isserv;
	GThread *listener = NULL;

	if (!g_thread_supported ())
	{
		g_thread_init (NULL);
		gdk_threads_init ();
	}
#endif

	cssedwin = cssed_window_new();

#ifdef WIN32
    package_prefix = g_win32_get_package_installation_directory(CSSED_REGISTRY_NAME, NULL);
    package_datadir = g_strdup_printf("%s%s", package_prefix, "\\share");
	package_libdir = g_strdup_printf("%s%s", package_prefix, "\\lib");
#endif


#ifdef ENABLE_NLS
#ifdef WIN32
    temp = g_strdup_printf("%s%s", package_prefix, "/lib/locale");
    bindtextdomain(GETTEXT_PACKAGE, temp);
    g_free(temp);
#else
    bindtextdomain(GETTEXT_PACKAGE, PACKAGE_LOCALE_DIR);
#endif
    bind_textdomain_codeset(GETTEXT_PACKAGE, "UTF-8");
    textdomain(GETTEXT_PACKAGE);
#endif

#ifdef WITH_IPC_QUEUE
	isserv = FALSE;
	clave = CSSED_IPC_MSG_QUEUE + getuid ();		// one queue for each user
	DBGMSG (__FILE__,__LINE__,"Will start ipc test %d\n", getpid ());
	// try to open the queue
	if ((identifier = msgget (clave, 0)) < 0)
	{
		DBGMSG (__FILE__,__LINE__,"Queue doesn't exists\n");
		if ((identifier =
			 msgget (clave, IPC_CREAT | 0666 | IPC_EXCL)) < 0)
		{
			DBGMSG (__FILE__,__LINE__,"Can't create queue %d\n",	 getpid ());
		}
		else
		{
			DBGMSG (__FILE__,__LINE__,"Queue created, this instance is the server %d\n", getpid ());
			isserv = TRUE;
			pdata.window = cssedwin;
			pdata.ident = identifier;
			pdata.run = TRUE;
			listener =	g_thread_create ((GThreadFunc) listen_queue_proc, &pdata, TRUE, NULL);
		}
	}
	else
	{
		DBGMSG (__FILE__,__LINE__,"Queue exists\n");
		//*
		if( argc > 1 ){ // avoid to open or listen if no args
			// the queue exists
			// check if dead (timeout)
			struct msqid_ds msg_stat;
			gint timediff;
			msgctl ( identifier, IPC_STAT, &msg_stat );
			if ( msg_stat.msg_qnum > 0 )
			{
				timediff = time (NULL) - msg_stat.msg_ctime;
				if ( timediff > 5 )
				{
					DBGMSG (__FILE__,__LINE__,"more than 5 seconds no reads diff=%d, deleting queue\n", timediff);
					msgctl (identifier, IPC_RMID, NULL);
					// must check here
					if ((identifier = msgget(clave, IPC_CREAT|0666|IPC_EXCL)) < 0)
					{
						DBGMSG (__FILE__,__LINE__,"Can't create queue %d\n", getpid ());
					}
					else
					{
						DBGMSG (__FILE__,__LINE__,"Queue created, this instance is the server %d\n", getpid ());
						isserv = TRUE;
						pdata.window = cssedwin;
						pdata.ident = identifier;
						pdata.run = TRUE;
						listener = g_thread_create ((GThreadFunc) listen_queue_proc, &pdata, TRUE, NULL);
					}
				}
			}
			else
			{	// no messages send message
				buff_msg.type = getpid ();
				memset(buff_msg.file, 0, PATH_MAX+1);
				if( g_path_is_absolute (argv[1]) ){
					if( strlen(argv[1]) >= PATH_MAX ) memcpy(buff_msg.file , argv[1], PATH_MAX);
					else memcpy(buff_msg.file , argv[1], strlen(argv[1]));
				}else{
					curdir = g_get_current_dir ();
					full_path = g_build_filename( curdir, argv[1], NULL );
					if( strlen(full_path) >= PATH_MAX )	memcpy(buff_msg.file , full_path, PATH_MAX);
					else memcpy(buff_msg.file , full_path, strlen(full_path));
					g_free(	full_path );
					g_free( curdir );
				}
				DBGMSG (__FILE__,__LINE__,"Queue exists %d\n", getpid ());

				if ((msgsnd (identifier, &buff_msg, PATH_MAX, 0)) < 0)
				{
					DBGMSG (__FILE__,__LINE__,
						"Can't send message, will start but not a server %d\n",
						getpid ()
						);
				}
				else
				{
					for (i = 2; i < argc; i++)
					{
						ssize_t msgretval;
						memset(buff_msg.file, 0, PATH_MAX);
						if( g_path_is_absolute (argv[i]) ){
							if( strlen(argv[1]) >= PATH_MAX ) memcpy(buff_msg.file , argv[i], PATH_MAX);
							else memcpy(buff_msg.file , argv[i], strlen(argv[i]));
						}else{
							curdir = g_get_current_dir ();
							full_path = g_build_filename( curdir, argv[i], NULL );
							if( strlen(full_path) >= PATH_MAX )	memcpy(buff_msg.file , full_path, PATH_MAX);
							else memcpy(buff_msg.file , full_path, strlen(full_path));
							g_free(full_path);
							g_free(curdir);
						}
						msgretval = msgsnd (identifier, &buff_msg, PATH_MAX, 0);
						if( msgretval != -1 ){
							DBGMSG (__FILE__,__LINE__,"Msg sent %s\n", argv[i]);
						}else{
							DBGMSG (__FILE__,__LINE__,"Cannot send msg %s\n", argv[i]);
						}
					}
					DBGMSG (__FILE__,__LINE__,"Msgs sent bail out %d\n", getpid ());
					return 0;
				}
			}
		}// end args
	}
#endif

	gtk_set_locale ();
	gtk_init (&argc, &argv);
	gdk_rgb_init ();

#ifdef WIN32
#ifdef NDEBUG
	// no output console
	g_set_print_handler( nulloutput );
	g_set_printerr_handler( nulloutput );
	g_log_set_handler("Gdk",
				G_LOG_LEVEL_MASK | G_LOG_FLAG_FATAL | G_LOG_FLAG_RECURSION,
				null_log, NULL);
	g_log_set_handler("Gtk",
				G_LOG_LEVEL_MASK | G_LOG_FLAG_FATAL | G_LOG_FLAG_RECURSION,
				null_log, NULL);				
	  g_log_set_handler("GLib",
				G_LOG_LEVEL_MASK | G_LOG_FLAG_FATAL | G_LOG_FLAG_RECURSION,
				null_log, NULL);				
	  g_log_set_handler("GLib-GObject",
				G_LOG_LEVEL_MASK | G_LOG_FLAG_FATAL | G_LOG_FLAG_RECURSION,
				null_log, NULL);				
	  g_log_set_handler("GModule",
				G_LOG_LEVEL_MASK | G_LOG_FLAG_FATAL | G_LOG_FLAG_RECURSION,
				null_log, NULL);				
	  g_log_set_handler("GThread",
				G_LOG_LEVEL_MASK | G_LOG_FLAG_FATAL | G_LOG_FLAG_RECURSION,
				null_log, NULL);
 
#endif
#endif
/*
#ifdef WIN32
    temp = g_build_filename(package_datadir, PACKAGE, "pixmaps", NULL);
    add_pixmap_directory(temp);
    g_free(temp);
#else
    add_pixmap_directory(PACKAGE_DATA_DIR "/" PACKAGE "/pixmaps");
#endif
*/
	init_default_config (cssedwin);
	// this will return a GList with the filenames of plugins to be loaded
	plugins_to_load = parse_configuration_file (cssedwin);
	// create the cssed "window" - not a good metaphor, may be create app should sound better.
	window = create_window (cssedwin);
	// load all files in plugins list
	if( plugins_to_load != NULL ){
		// this also frees resources
		load_all_plugins(cssedwin, plugins_to_load);
	}
	
	// apply the size settings stored on the configuration
	cssed_window_apply_stored_size_and_position (cssedwin);
	
	// on IDLE set the pos PANEDBUG
	g_idle_add  (apply_stored_pos_on_idle, (gpointer) cssedwin);
	
	// show window widget
	gtk_widget_show (window);

	/* args processing everything passed in taken as a file name */
	for ( i = 1; i < argc; i++ )
	{
		if( g_path_is_absolute (argv[i]) ){
			document_open_with_error_check ( cssedwin, argv[i] );
		}else{
			curdir = g_get_current_dir ();
			full_path = g_build_filename( curdir, argv[i], NULL );
			document_open_with_error_check ( cssedwin, full_path );
			g_free(full_path);
			g_free(curdir);
		}
	}

	
#ifndef WITH_IPC_QUEUE
	gtk_main ();
#else
	gdk_threads_enter();
	gtk_main();
	gdk_threads_leave();

	  if( isserv ){
		DBGMSG (__FILE__,__LINE__,"It's a server let's kill the queue\n");
		if( (msgctl( identifier, IPC_RMID, NULL )) < 0){
		  DBGMSG (__FILE__,__LINE__,"Can't delete queue\n");
		}else{
		  DBGMSG (__FILE__,__LINE__,"Queue deleted\n");
		}
		pdata.run = FALSE;
		g_thread_join(listener); // wait for the ipc reader thread to terminate
	  }
#endif

#ifdef WIN32
    g_free(package_prefix);
    g_free(package_datadir);
    g_free(package_libdir);
#endif

	return 0;
}

#ifdef WIN32
int APIENTRY WinMain(struct HINSTANCE__ *hInstance,
					 struct HINSTANCE__ *hPrevInstance,
					 LPSTR cmd,
					 int show)
	{
		return main( __argc, __argv );
	}
#endif

#ifdef WITH_IPC_QUEUE
gpointer
listen_queue_proc (gpointer * data)
{
	struct ipcmsg buff_msg;
	CssedWindow* cssedwin;
	int identifier;
	int size;
	struct ipcproc_data *pdata;

	pdata = (struct ipcproc_data*) data;
	identifier = pdata->ident;
	cssedwin = pdata->window;

	DBGMSG (__FILE__,__LINE__,"Checking queue %d\n", getpid ());

	while (pdata->run)
	{
		size = msgrcv(identifier, &buff_msg, PATH_MAX, 0, 0);
		while (size > 0)
		{
			gdk_threads_enter ();
			document_open_with_error_check (cssedwin, buff_msg.file);
			gdk_threads_leave ();
			size = msgrcv (identifier, &buff_msg, PATH_MAX, 0, 0);
		}
		sleep (2);
	}
	return 0;
}
#endif

