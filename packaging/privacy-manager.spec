Name:    privacy-manager-server
Summary: Privacy Management
Version: 0.0.5
Release: 0
Group:   System/Libraries
License: SAMSUNG
Source0: %{name}-%{version}.tar.gz
Source1: privacy-manager-server.service
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
BuildRequires: gettext-tools

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

%package -n capi-security-privacy-manager
Summary:    Privacy Manager API
Group:      TO_BE/FILLED_IN
License:    TO BE FILLED IN
BuildRequires:  cmake
BuildRequires:  pkgconfig(dlog)
BuildRequires:  pkgconfig(capi-base-common)
BuildRequires:  pkgconfig(glib-2.0)
Requires(post): /sbin/ldconfig  
Requires(postun): /sbin/ldconfig
Requires: privacy-manager-client = %{version}-%{release}

%description -n capi-security-privacy-manager
The Privacy Manager API provides functions to get/set information about privacy information of installed packages.

%package  -n capi-security-privacy-manager-devel
Summary:  Privacy Manager API (Development)
Group:    TO_BE/FILLED_IN
Requires: privacy-manager-client = %{version}-%{release}

%description -n capi-security-privacy-manager-devel
The Privacy Manager API provides functions to get/set information about privacy information of installed packages.(DEV)

%prep
%setup -q

%build
#%{!?build_type:%define build_type "Release"}

echo cmake . -DPREFIX=%{_prefix} \
        -DEXEC_PREFIX=%{_exec_prefix} \
        -DLIBDIR=%{_libdir} \
        -DINCLUDEDIR=%{_includedir} \
        -DCMAKE_BUILD_TYPE=%{build_type} \
        -DVERSION=%{version} \
        -DFILTER_LISTED_PKG=ON \
		-DPRIVACY_POPUP=OFF
cmake . -DPREFIX=%{_prefix} \
        -DEXEC_PREFIX=%{_exec_prefix} \
        -DLIBDIR=%{_libdir} \
        -DINCLUDEDIR=%{_includedir} \
        -DCMAKE_BUILD_TYPE=%{build_type} \
        -DVERSION=%{version} \
        -DFILTER_LISTED_PKG=ON \
		-DPRIVACY_POPUP=OFF
make %{?jobs:-j%jobs}

%install
rm -rf %{buildroot}
mkdir -p %{buildroot}/usr/share/license
cp LICENSE.APLv2 %{buildroot}/usr/share/license/privacy-manager-server
mkdir -p %{buildroot}/usr/share/license
cp LICENSE.APLv2 %{buildroot}/usr/share/license/privacy-manager-client
mkdir -p %{buildroot}/usr/bin
cp res/usr/bin/* %{buildroot}/usr/bin/
mkdir -p %{buildroot}/opt/dbspace
cp res/opt/dbspace/.privacylist.db /%{buildroot}/opt/dbspace/
mkdir -p %{buildroot}/usr/share/privacy-manager/
cp res/usr/share/privacy-manager/privacy-filter-list.ini %{buildroot}/usr/share/privacy-manager/
#mkdir -p %{buildroot}/etc/rc.d/init.d
#cp res/etc/rc.d/init.d/* %{buildroot}/etc/rc.d/init.d/

%make_install
#mkdir -p %{buildroot}/etc/rc.d/rc3.d
#mkdir -p %{buildroot}/etc/rc.d/rc5.d
#ln -sf /etc/rc.d/init.d/privacy-manager-server.sh %{buildroot}/etc/rc.d/rc3.d/S10privacy-manager-server.sh
#ln -sf /etc/rc.d/init.d/privacy-manager-server.sh %{buildroot}/etc/rc.d/rc5.d/S10privacy-manager-server.sh

mkdir -p %{buildroot}%{_libdir}/systemd/system/multi-user.target.wants
#install -m 0644 %{SOURCE1} %{buildroot}%{_libdir}/systemd/system/privacy-manager-server.service
#ln -sf /usr/lib/systemd/system/privacy-manager-server.service %{buildroot}%{_libdir}/systemd/system/multi-user.target.wants/privacy-manager-server.service

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

%postun
/sbin/ldconfig

%files -n privacy-manager-server
%defattr(-,root,root,-)
%manifest packaging/privacy-manager-server.manifest
%{_libdir}/libprivacy-manager-server.so*
/usr/share/license/privacy-manager-server
/opt/dbspace/.privacylist.db
/usr/bin/*

%files -n privacy-manager-server-devel
%{_includedir}/privacy_manager/privacy_manager_daemon.h
%{_libdir}/pkgconfig/privacy-manager-server.pc

%files -n privacy-manager-client
%defattr(-,root,root,-)
%manifest packaging/privacy-manager-client.manifest
%{_libdir}/libprivacy-manager-client.so*
/usr/share/license/privacy-manager-client
/usr/share/privacy-manager/privacy-filter-list.ini
/usr/etc/package-manager/parserlib/libprivileges.so

%files -n privacy-manager-client-devel
%defattr(-,root,root,-)
%{_libdir}/pkgconfig/privacy-manager-client.pc

%{_includedir}/privacy_manager/PrivacyManagerClient.h
%{_includedir}/privacy_manager/PrivacyChecker.h
%{_includedir}/privacy_manager/privacy_info_client.h
%{_includedir}/privacy_manager/privacy_manager_client.h
%{_includedir}/privacy_manager/privacy_checker_client.h
%{_includedir}/privacy_manager/privacy_manager_client_types.h


%files -n capi-security-privacy-manager
%{_libdir}/libcapi-security-privacy-manager.so.*
%manifest packaging/capi-security-privacy-manager.manifest

%files -n capi-security-privacy-manager-devel
%{_includedir}/privacymgr/*.h
%{_libdir}/libcapi-security-privacy-manager.so
%{_libdir}/pkgconfig/capi-security-privacy-manager.pc
