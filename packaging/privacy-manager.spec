Name:    privacy-manager-server
Summary: Privacy Management
Version: 0.0.6
Release: 1
Group:   System/Libraries
License: Apache-2.0
Source0: %{name}-%{version}.tar.gz
Source1: privacy-manager-server.service
Source2: privacy-manager-server.socket
BuildRequires: cmake
BuildRequires: pkgconfig(dlog)
BuildRequires: pkgconfig(glib-2.0)
BuildRequires: pkgconfig(sqlite3)
BuildRequires: pkgconfig(capi-base-common)
BuildRequires: pkgconfig(dbus-1)
BuildRequires: pkgconfig(dbus-glib-1)
BuildRequires: pkgconfig(db-util)
BuildRequires: pkgconfig(pkgmgr-info)
BuildRequires: pkgconfig(capi-system-info)
BuildRequires: pkgconfig(libprivilege-control)
BuildRequires: pkgconfig(security-server)
BuildRequires: pkgconfig(libsmack)
BuildRequires: pkgconfig(vconf)
BuildRequires: pkgconfig(libsystemd-daemon)
BuildRequires: gettext-tools
BuildRequires: pkgconfig(aul)

Requires(post):   /sbin/ldconfig
Requires(postun): /sbin/ldconfig

%description
Privacy Management

%package -n privacy-manager-server-devel
summary: privacy-manager server
Group: Development/Libraries
Requires: privacy-manager-server = %{version}-%{release}

%description -n privacy-manager-server-devel
privacy-manager server devel

%package -n privacy-manager-client
summary: privacy-manager client
Group: Development/Libraries
Requires: privacy-manager-server = %{version}-%{release}

%description -n privacy-manager-client
privacy-manager client

%package -n privacy-manager-client-devel
Summary:    privacy-manager client devel
Group:      Development/Libraries
BuildRequires:  pkgconfig(libxml-2.0)
Requires:   privacy-manager-client = %{version}-%{release}

%description -n privacy-manager-client-devel
Privacy Management(development files)

%prep
%setup -q

%build
#%{!?build_type:%define build_type "Release"}

export CFLAGS="$CFLAGS -DTIZEN_DEBUG_ENABLE"
export CXXFLAGS="$CXXFLAGS -DTIZEN_DEBUG_ENABLE"
export FFLAGS="$FFLAGS -DTIZEN_DEBUG_ENABLE"

export CFLAGS="$CFLAGS -DTIZEN_ENGINEER_MODE"
export CXXFLAGS="$CXXFLAGS -DTIZEN_ENGINEER_MODE"
export FFLAGS="$FFLAGS -DTIZEN_ENGINEER_MODE"

echo cmake . -DPREFIX=%{_prefix} \
        -DEXEC_PREFIX=%{_exec_prefix} \
        -DLIBDIR=%{_libdir} \
        -DINCLUDEDIR=%{_includedir} \
        -DCMAKE_BUILD_TYPE=%{build_type} \
        -DVERSION=%{version} \
        -DFILTER_LISTED_PKG=ON
cmake . -DPREFIX=%{_prefix} \
        -DEXEC_PREFIX=%{_exec_prefix} \
        -DLIBDIR=%{_libdir} \
        -DINCLUDEDIR=%{_includedir} \
        -DCMAKE_BUILD_TYPE=%{build_type} \
        -DVERSION=%{version} \
        -DFILTER_LISTED_PKG=ON
make %{?jobs:-j%jobs}

%install
rm -rf %{buildroot}
mkdir -p %{buildroot}/usr/share/license
cp LICENSE.Apache-2.0 %{buildroot}/usr/share/license/privacy-manager-server
mkdir -p %{buildroot}/usr/share/license
cp LICENSE.Apache-2.0 %{buildroot}/usr/share/license/privacy-manager-client
mkdir -p %{buildroot}/usr/bin
cp res/usr/bin/* %{buildroot}/usr/bin/
mkdir -p %{buildroot}/opt/dbspace
cp res/opt/dbspace/.privacylist.db /%{buildroot}/opt/dbspace/
mkdir -p %{buildroot}/usr/share/privacy-manager/
cp res/usr/share/privacy-manager/privacy-filter-list.ini %{buildroot}/usr/share/privacy-manager/
cp res/usr/share/privacy-manager/privacy-location-filter-list.ini %{buildroot}/usr/share/privacy-manager/
mkdir -p %{buildroot}/etc/vasum/vsmzone.resource/
cat << EOF > %{buildroot}/etc/vasum/vsmzone.resource/privacy-manager.res
LINK=["/opt/dbspace/.privacy.db,/opt/dbspace/.privacy.db"]
LINK=["/opt/dbspace/.privacylist.db,/opt/dbspace/.privacylist.db"]
EOF

%make_install
mkdir -p %{buildroot}%{_libdir}/systemd/system/multi-user.target.wants
mkdir -p %{buildroot}%{_libdir}/systemd/system/sockets.target.wants

install -m 0644 %{SOURCE1} %{buildroot}%{_libdir}/systemd/system/
install -m 0644 %{SOURCE2} %{buildroot}%{_libdir}/systemd/system/

ln -sf ../%{SOURCE1} %{buildroot}%{_libdir}/systemd/system/multi-user.target.wants/
ln -sf ../%{SOURCE2} %{buildroot}%{_libdir}/systemd/system/sockets.target.wants/

%clean
rm -rf %{buildroot}

%post -n privacy-manager-server
/sbin/ldconfig

echo "Check privacy DB"
if [ ! -f /opt/dbspace/.privacy.db ]
then
	echo "Create privacy DB"
	/usr/bin/privacy_manager_create_clean_db.sh
fi
rm /usr/bin/privacy_manager_create_clean_db.sh
rm /usr/bin/privacy_db.sql

/usr/sbin/setcap cap_chown,cap_dac_override,cap_lease+eip /usr/bin/privacy-manager-server

%postun
/sbin/ldconfig

%files -n privacy-manager-server
%defattr(-,system,system,-)
%manifest packaging/privacy-manager-server.manifest
%{_bindir}/*
%{_libdir}/systemd/*
/usr/share/license/privacy-manager-server
/opt/dbspace/.privacylist.db
/etc/vasum/vsmzone.resource/privacy-manager.res
%{_libdir}/systemd/system/*

%files -n privacy-manager-server-devel
%{_libdir}/pkgconfig/privacy-manager-server.pc

%files -n privacy-manager-client
%defattr(-,system,system,-)
%manifest packaging/privacy-manager-client.manifest
%{_libdir}/libprivacy-manager-client.so*
/usr/share/license/privacy-manager-client
/usr/share/privacy-manager/privacy-filter-list.ini
/usr/share/privacy-manager/privacy-location-filter-list.ini
/usr/etc/package-manager/parserlib/libprivileges.so

%files -n privacy-manager-client-devel
%defattr(-,system,system,-)
%{_libdir}/pkgconfig/privacy-manager-client.pc

%{_includedir}/privacy_manager/PrivacyManagerClient.h
%{_includedir}/privacy_manager/PrivacyChecker.h
%{_includedir}/privacy_manager/privacy_info_client.h
%{_includedir}/privacy_manager/privacy_manager_client.h
%{_includedir}/privacy_manager/privacy_checker_client.h
%{_includedir}/privacy_manager/privacy_manager_client_types.h

