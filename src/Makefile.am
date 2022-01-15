## cssed Makefile.am proccess with automake to produce Makefile.in
## Iago Rubio (c) 2003

if DEBUG
AM_CFLAGS = -Wall -Werror -ggdb
endif

INCLUDES = \
	-DPACKAGE_DATA_DIR=\""$(datadir)"\" \
	-DPACKAGE_LIB_DIR=\""${pkglibdir}"\" \
	-DPACKAGE_LOCALE_DIR=\""$(prefix)/$(DATADIRNAME)/locale"\" \
	-I$(top_srcdir)/scintilla/include \
	-I$(top_srcdir)/libcroco \
	@PACKAGE_CFLAGS@ 
	
bin_PROGRAMS = cssed

cssed_SOURCES = \
	main.c \
	support.c support.h \
	interface.c interface.h \
	document.c document.h	\
	sciwrappers.c sciwrappers.h	\
	xmlparser.c xmlparser.h \
	callbacks.c callbacks.h \
	selectorparser.c selectorparser.h \
	cssdialogs-interface.c cssdialogs-interface.h \
	cssdialogs-callbacks.c cssdialogs-callbacks.h \
	cssdialogs-functions.c cssdialogs-functions.h \
	utils.c utils.h \
	cssdialogs-strings.h \
	cssdialogs-menu.h cssdialogs-menu.c \
	cssparser.c cssparser.h \
	configparser.c configparser.h \
	configdlg-interface.c configdlg-interface.h \
	configdlg-callbacks.c configdlg-callbacks.h \
	selector-wizard.c selector-wizard.h \
	color-wizard.c color-wizard.h \
	floating-list.c floating-list.h \
	doc-scanner.h doc-scanner.c \
	box-wizard.c box-wizard.h \
	cssdialogs-class.c cssdialogs-class.h \
	cssdialogs-hash.c cssdialogs-hash.h \
	plugins-dialog.c plugins-dialog.h \
	plugin.c plugin.h \
	cssedwindow.h cssedwindow.c \
	document-list.c document-list.h \
	gotoline-dialog.c gotoline-dialog.h \
	file-type-manager.c	file-type-manager.h \
	filetype-css2.c filetype-css2.h \
	filetype-text.c filetype-text.h \
	filetype-c.c filetype-c.h \
	filetype-hypertext.c filetype-hypertext.h \
	filetype-phpscript.c filetype-phpscript.h \
	filetype-sql.c filetype-sql.h \
	filetype-perl.c filetype-perl.h \
	filetype-python.c filetype-python.h \
	filetype-xml.c filetype-xml.h \
	filetype-sh.c filetype-sh.h \
	filetype-diff.c filetype-diff.h \
	filetype-make.c filetype-make.h \
	filetype-js.c filetype-js.h \
	filetype-java.c filetype-java.h \
	filetype-conf.c filetype-conf.h \
	filetype-asm.c filetype-asm.h \
	debug.c debug.h \
	xpm-icons.h \
	filetypes.h

cssed_LDADD = @PACKAGE_LIBS@  ../scintilla/gtk/libscintilla.a ../libcroco/parser/libcroco.a
cssed_LINK = g++ -DGTK -o cssed
