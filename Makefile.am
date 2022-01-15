## Process this file with automake to produce Makefile.in

SUBDIRS = libcroco scintilla pixmaps src po

man_MANS = cssed.1

EXTRA_DIST = \
	autogen.sh \
	bootstrap \
	libcroco \
	scintilla \
	data \
	cssed.pc.in

if INSTALLHEADERS
pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = cssed.pc
endif

install-data-local:
	@$(NORMAL_INSTALL)
	if test -d $(srcdir)/data; then \
		$(mkinstalldirs) $(DESTDIR)$(pkgdatadir)/data; \
		for data in $(srcdir)/data/*; do \
			if test -f $$data; then \
			  $(INSTALL_DATA) $$data $(DESTDIR)$(pkgdatadir)/data; \
			fi \
		done \
	fi
	if test -e $(srcdir)/pixmaps/cssed_icon.png; then \
		$(mkinstalldirs) $(DESTDIR)$(prefix)/share/pixmaps/; \
		$(INSTALL_DATA) $(srcdir)/pixmaps/cssed_icon.png $(DESTDIR)$(prefix)/share/pixmaps/cssed_icon.png; \
	fi

if INSTALLHEADERS
	if test -d $(srcdir)/include; then \
	  $(mkinstalldirs) $(DESTDIR)$(pkgincludedir); \
	  for header in $(srcdir)/include/*.h; do \
	    if test -f $$header; then \
	      $(INSTALL_DATA) $$header $(DESTDIR)$(pkgincludedir); \
	    fi \
	  done \
	fi 
endif

if FREEDESKTOP_SUPPORT
	if test -e $(srcdir)/freedesktop/cssed.applications; then \
		$(mkinstalldirs) $(DESTDIR)$(datadir)/application-registry; \
		$(INSTALL_DATA) $(srcdir)/freedesktop/cssed.applications $(DESTDIR)$(datadir)/application-registry; \
	fi
	if test -e $(srcdir)/freedesktop/cssed.desktop; then \
		$(mkinstalldirs) $(DESTDIR)$(datadir)/applications; \
		if UPDATE_DATABASES; then \
			$(DESKTOP_FILE_INSTALL) --rebuild-mime-info-cache --dir $(DESTDIR)$(datadir)/applications $(srcdir)/freedesktop/cssed.desktop; \
		else \
			$(INSTALL_DATA) $(srcdir)/freedesktop/cssed.desktop $(DESTDIR)$(datadir)/applications; \
		fi \
	fi
	if test -e $(srcdir)/freedesktop/cssed.mime; then \
		$(mkinstalldirs) $(DESTDIR)$(datadir)/mime-info; \
		$(INSTALL_DATA) $(srcdir)/freedesktop/cssed.mime $(DESTDIR)$(datadir)/mime-info; \
	fi	
endif
uninstall-local:
	if test -d $(DESTDIR)$(pkgdatadir); then \
		rm -rf $(DESTDIR)$(pkgdatadir); \
	fi
	if test -e $(DESTDIR)$(prefix)/share/pixmaps/cssed_icon.png; then \
		rm -rf $(DESTDIR)$(prefix)/share/pixmaps/cssed_icon.png; \
	fi
if INSTALLHEADERS
	if test -d $(DESTDIR)$(pkgincludedir); then \
		rm -rf $(DESTDIR)$(pkgincludedir); \
	fi
endif
if FREEDESKTOP_SUPPORT
	if test -e $(datadir)/applications/cssed.desktop; then \
		rm -rf $(datadir)/applications/cssed.desktop; \
	fi
	if test -e $(datadir)/application-registry/cssed.applications; then \
		rm -rf $(datadir)/application-registry/cssed.applications; \
	fi
	if test -e $(datadir)/mime-info/cssed.mime; then \
		rm -rf $(datadir)/mime-info/cssed.applications; \
	fi
endif
dist-hook:
	if test -d pixmaps; then \
	  mkdir $(distdir)/pixmaps; \
	  for pixmap in pixmaps/*; do \
	    if test -f $$pixmap; then \
	      cp -p $$pixmap $(distdir)/pixmaps; \
	    fi \
	  done \
	fi
	if test -d include; then \
	  mkdir $(distdir)/include; \
	  for header in include/*.h; do \
	    if test -f $$header; then \
	      cp -p $$header $(distdir)/include; \
	    fi \
	  done \
	fi
	for manpage in $(man_MANS); do \
		if test -e $$manpage; then \
			cp -p $$manpage $(distdir); \
		fi \
	done
	if test -d freedesktop; then \
	  mkdir $(distdir)/freedesktop; \
	  for file in freedesktop/*.in freedesktop/*.mime; do \
	    if test -f $$file; then \
	      cp -p $$file $(distdir)/freedesktop; \
	    fi \
	  done \
	fi	
