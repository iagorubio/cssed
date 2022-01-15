%define vendor  fedora

Summary:         CSS editor for web developers
Name:            cssed
Version:         0.4.1
Release:         1%{?dist}
Group:           Development/Tools
License:         GPL
Source:          http://dl.sourceforge.net/cssed/cssed-%{version}.tar.gz
Url:             http://cssed.sourceforge.net/
BuildRoot:       %{_tmppath}/cssed-%{version}-%{release}-root-%(%{__id_u} -n)
Requires:        gtk2 >= 2.4, glib2 >= 2.4
Buildrequires:   gtk2-devel >= 2.4, glib2-devel >= 2.4, libxml2-devel >= 2.0, desktop-file-utils, gettext

%description 
cssed is a tiny GTK+ CSS editor and validator
for web developers.

%package devel
Summary:         A set of files to develop cssed plugins
Group:           Development/Tools
Requires:        %{name} = %{version}

%description devel
cssed is a tiny GTK+ CSS editor and validator
for web developers. This package contains the
files needed to develop cssed plugins.

%prep
%setup -q

%build
%configure --with-ipc-queue --with-plugin-headers
make %{?_smp_mflags}

%install
rm -rf %{buildroot}
DESTDIR=%{buildroot} make install

cat << EOF > %{vendor}-cssed.desktop
[Desktop Entry]
Name=Cssed
Comment=CSS editor
Comment[es]=Editor de CSS
Exec=cssed
Icon=%{_prefix}/share/pixmaps/cssed_icon.png
Terminal=0
Type=Application
StartupWMClass=Cssed
MimeType=text/html;text/css;text/x-javascript;text/x-python;text/x-perl;application/x-php;text/x-java;text/javascript;text/x-php;application/x-cgi;application/x-javascript;application/x-perl;application/x-python;application/xhtml+xml;text/mathml;text/x-csrc;text/x-dtd;text/x-sql;text/xml;
EOF

desktop-file-install --delete-original  --vendor %{vendor} \
  --dir %{buildroot}%{_datadir}/applications               \
  --add-category Application                               \
  --add-category Development                               \
  --add-category X-Fedora                                  \
  %{vendor}-cssed.desktop

%find_lang %{name}

%clean
rm -rf %{buildroot}

%files -f %{name}.lang
%defattr(-,root,root)
%{_bindir}/cssed
%{_mandir}/man1/cssed.1.gz
%dir %{_datadir}/cssed
%dir %{_datadir}/cssed/data
%{_datadir}/cssed/data/cssed-def.xml
%{_datadir}/cssed/data/cssed-cfg.xml
%{_datadir}/cssed/data/cssed-def.dtd
%{_datadir}/applications/%{vendor}-cssed.desktop
%{_prefix}/share/pixmaps/cssed_icon.png
%doc AUTHORS COPYING ChangeLog README INSTALL NEWS

%files devel
%defattr(-,root,root)
%dir %{_includedir}/cssed
%{_libdir}/pkgconfig/cssed.pc
%{_includedir}/cssed/SciLexer.h
%{_includedir}/cssed/Scintilla.h
%{_includedir}/cssed/plugin.h


%changelog
* Fri May 05 2006  Iago Rubio <iago.rubio@hispalinux.es> 0.4.1-1
- Deleted images - now they are internally compiled - and old API headers.
* Sun Dec 04 2005  Iago Rubio <iago.rubio@hispalinux.es> 0.4.1-0
- Minor changes to use sanest include directory.
* Sat Dec 03 2005 Iago Rubio <iago.rubio@hispalinux.es> 0.4.1-0
- Minor changes to use sanest include directory.
* Sun Sep 18 2005 Iago Rubio <iago.rubio@hispalinux.es> 0.4.0-0
- Updated version tags.
* Fri Mar 09 2005 Iago Rubio <iago.rubio@hispalinux.es> 0.3.1-3
- Minimal corrections to follow the fedora extras guidelines.
* Fri Mar 09 2005 Iago Rubio <iago.rubio@hispalinux.es> 0.3.1-fc3
- Splited out development headers to a separate package.
* Fri Sep 23 2004 Iago Rubio <iago.rubio@hispalinux.es> 0.3.0-fc2
- Some changes to use GtkFileChooser on gtk+2 >= 2.4 systems.
* Mon Sep 13 2004 Iago Rubio <iago.rubio@hispalinux.es> 0.3.0
- Finished plugable interface. Lots of usability improvements.
  Added *.pc file to compile plugins with pkg-config.
* Wed Apr 07 2004 Iago Rubio <iago.rubio@hispalinux.es> 0.2.2
- Added plugable interface and some improvements.
* Wed Apr 07 2004 Iago Rubio <iago.rubio@hispalinux.es> 0.2.1
- Fixed bug that prevent open files that cannot be written.
* Tue Apr 06 2004 Iago Rubio <iago.rubio@hispalinux.es> 0.2.0
- Added doc scanner, fixed undo redo, first beta.
* Fri Feb 27 2004 Iago Rubio <iago.rubio@hispalinux.es> 0.1-5
- Fixed vte build problem.
* Thu Feb 12 2004 Iago Rubio <iago.rubio@hispalinux.es> 0.1.0-4
- Bug fixes and better highlighting
* Thu Jan 14 2004 Iago Rubio <iago.rubio@hispalinux.es> 0.1.0-3
- Updated RPM with validation support 
* Mon Dec 8 2003 Iago Rubio <iago.rubio@hispalinux.es> 0.1.0-2
- Initiall RPM.
