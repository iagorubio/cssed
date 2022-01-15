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
filetype_conf_set_style(CssedDoc* doc, GtkWidget* editor)
{
	ScintillaObject * sci;
	GtkWidget *scintilla;
		
	const gchar directiveKeyWords[] = "acceptfilter acceptmutex accessconfig accessfilename \
	action addalt addaltbyencoding addaltbytype addcharset adddefaultcharset adddescription \
	addencoding addhandler addicon addiconbyencoding addiconbytype addlanguage addmodule \
	addmoduleinfo addtype agentlog alias aliasmatch allow allowconnect allowoverride \
	anonymous anonymous_authoritative anonymous_logemail anonymous_mustgiveemail \
	anonymous_nouserid anonymous_verifyemail authauthoritative authdbauthoritative \
	authdbgroupfile authdbmauthoritative authdbmgroupfile authdbmgroupfile authdbuserfile \
	authdbmuserfile authdigestfile authgroupfile authname authtype authuserfile bindaddress \
	browsermatch browsermatchnocase bs2000account cachedefaultexpire cachedirlength cachedirlevels \
	cacheforcecompletion cachegcinterval cachelastmodifiedfactor cachemaxexpire cachenegotiateddocs \
	cacheroot cachesize cgicommandargs checkspelling clearmodulelist contentdigest cookiedomain \
	cookieexpires cookieformat cookielog cookielog cookieprefix cookiestyle cookietracking \
	coredumpdirectory customlog defaulticon defaultlanguage defaulttype deny directory \
	directorymatch directoryindex documentroot ebcdicconvert ebcdicconvertbytype ebcdickludge \
	enableexceptionhook errordocument errorheader errorlog example expiresactive expiresbytype \
	expiresdefault extendedstatus fancyindexing fileetag files filesmatch forcetype forensiclog \
	group header headername hostnamelookups identitycheck ifdefine ifmodule imapbase imapdefault \
	imapmenu include indexignore indexoptions indexorderdefault isapireadaheadbuffer \
	isapilognotsupported isapiappendlogtoerrors isapiappendlogtoquery keepalive keepalivetimeout \
	languagepriority limit limitexcept limitinternalrecursion limitrequestbody limitrequestfields \
	limitrequestfieldsize limitrequestline listen listenbacklog loadfile loadmodule location \
	locationmatch lockfile logformat loglevel " "maxclients maxkeepaliverequests maxrequestsperchild \
	maxspareservers metadir metafiles metasuffix mimemagicfile minspareservers mmapfile \
	namevirtualhost nocache options order passenv pidfile port protocolreqcheck \
	proxyblock proxydomain proxypass proxypassreverse proxyreceivebuffersize proxyremote \
	proxyrequests proxyvia readmename redirect redirectmatch redirectpermanent redirecttemp \
	refererignore refererlog removeencoding removehandler removetype require resourceconfig \
	rewritebase rewritecond rewriteengine rewritelock rewritelog rewriteloglevel rewritemap \
	rewriteoptions rewriterule rlimitcpu rlimitmem rlimitnproc satisfy scoreboardfile script \
	scriptalias scriptaliasmatch scriptinterpretersource scriptlog scriptlogbuffer \
	scriptloglength sendbuffersize serveradmin serveralias servername serverpath serverroot \
	serversignature servertokens servertype setenv setenvif setenvifnocase sethandler \
	shmemuidisuser startservers threadsperchild timeout transferlog typesconfig unsetenv \
	usecanonicalname user userdir virtualhost virtualdocumentroot virtualdocumentrootip \
	virtualscriptalias virtualscriptaliasip xbithack numservers startthreads minsparethreads \
	maxsparethreads maxthreadsperchild addoutputfilter from";
	
	const gchar paramaterKeyWords[] = "on off uslock pthread sysvsem fcntl flock os2sem tpfcore none default all none basic digest double registry script on off email minimal productonly os full standalone dns indexes multiviews includes followsymlinks execcgi";
	
	if( editor ) scintilla = editor;
	else scintilla = document_get_scintilla_widget(doc);
	
	sci = SCINTILLA(scintilla);
	
	SSM (sci, SCI_STYLECLEARALL, 0, 0);
	SSM (sci, SCI_SETLEXER, SCLEX_CONF, 0);
	
	SSM(sci, SCI_SETKEYWORDS, 0, (sptr_t) directiveKeyWords);
	SSM(sci, SCI_SETKEYWORDS, 1, (sptr_t) paramaterKeyWords);
	
	SSM(sci, SCI_STYLESETFORE, SCE_CONF_DEFAULT, 0x000000);
	SSM(sci, SCI_STYLESETBACK, SCE_CONF_DEFAULT, 0xffffff);
	
	SSM(sci, SCI_STYLESETFORE, SCE_CONF_COMMENT, 0x808080);
	SSM(sci, SCI_STYLESETBACK, SCE_CONF_COMMENT, 0xffffff);
	
	SSM(sci, SCI_STYLESETFORE, SCE_CONF_NUMBER, 0x800080);
	SSM(sci, SCI_STYLESETBACK, SCE_CONF_NUMBER, 0xffffff);
	
	SSM(sci, SCI_STYLESETFORE, SCE_CONF_IDENTIFIER, 0x808000);
	SSM(sci, SCI_STYLESETBACK, SCE_CONF_IDENTIFIER, 0xffffff);
	
	SSM(sci, SCI_STYLESETFORE, SCE_CONF_EXTENSION, 0x801060);
	SSM(sci, SCI_STYLESETBACK, SCE_CONF_EXTENSION, 0xffffff);
	
	SSM(sci, SCI_STYLESETFORE, SCE_CONF_PARAMETER, 0x606000);
	SSM(sci, SCI_STYLESETBACK, SCE_CONF_PARAMETER, 0xffffff);
	
	SSM(sci, SCI_STYLESETFORE, SCE_CONF_STRING, 0x008000);
	SSM(sci, SCI_STYLESETBACK, SCE_CONF_STRING, 0xffffff);
	
	SSM(sci, SCI_STYLESETFORE, SCE_CONF_OPERATOR, 0x801000);
	SSM(sci, SCI_STYLESETBACK, SCE_CONF_OPERATOR, 0xffffff);
	
	SSM(sci, SCI_STYLESETFORE, SCE_CONF_IP, 0x008040);
	SSM(sci, SCI_STYLESETBACK, SCE_CONF_IP, 0xffffff);
	
	SSM(sci, SCI_STYLESETFORE, SCE_CONF_DIRECTIVE, 0x800000);
	SSM(sci, SCI_STYLESETBACK, SCE_CONF_DIRECTIVE, 0xffffff);

}
/* ********************************** */

void
create_and_register_conf_file_type (CssedWindow* window)
{
	CssedFileType* conf;
	GtkWidget* conf_menu;

	conf_menu = gtk_menu_item_new_with_label("Apache Conf");

	conf = cssed_file_type_new();
	conf->char_added  = NULL;

	conf->modify_attemp_read_only = NULL;
	conf->double_click = NULL;
	conf->user_list_selection = NULL;
	conf->dwell_start = NULL;
	conf->dwell_end = NULL;
	conf->hot_spot_click = NULL;
	conf->hot_spot_doubleclick = NULL;
	conf->call_tip_click = NULL;

	conf->save_point_reached = NULL;
	conf->save_point_left = NULL;
	conf->uri_dropped = NULL;
	conf->margin_click = NULL;
	conf->modified = NULL;
	conf->key_pressed = NULL;

	conf->apply_style = filetype_conf_set_style;

	conf->get_config_page_widget = NULL;
	conf->save_configuration_from_widget = NULL;	
	conf->pop_menu = NULL;
	conf->clean_popmenu = NULL;
	conf->menu_item = conf_menu;
	conf->clean_menu = NULL;
	conf->load_ui = NULL;

	// file related issues, those are mostly to avoid to open or save files
	conf->open_file = NULL;
	conf->save_doc = NULL;

	conf->id = CSSED_FILETYPE_APACHE_CONF;
	conf->style_min = SCE_CONF_DEFAULT;
	conf->style_max = SCE_CONF_DIRECTIVE;
	conf->can_fold = FALSE;
	conf->label_language = g_strdup("Apache Conf");
	conf->lable_file = g_strdup("Apache Configurations");

	cssed_file_type_menu_set_default_callback (window, conf_menu, conf);
	cssed_window_add_filetype (window, conf, FALSE);
}
