# Copyright (c) 2015 Piotr Tworek. All rights reserved.
# Use of this source code is governed by a BSD-style license that can be
# found in the LICENSE file.

%bcond_with debugcode

%global version_file "chrome/VERSION"
%global version_script "build/util/version.py"
%global version_pattern "@MAJOR@.@MINOR@.@BUILD@.@PATCH@"
%global chrome_version %(python %{version_script} -f %{version_file} -t "%{version_pattern}")
%global chrome_version_major %(python %{version_script} -f %{version_file} -t "@MAJOR@")

Name:    quicksilver
Summary: Chromium based web browser component for Qt
Version: %{chrome_version}
Release: 1
Group:   Applications/Internet
License: LGPL and MIT
Source0: %{name}-%{version}.tar.bz2

BuildRequires: pkgconfig(Qt5Core)
BuildRequires: pkgconfig(Qt5Gui)
BuildRequires: pkgconfig(Qt5OpenGL)
BuildRequires: pkgconfig(Qt5Qml)
BuildRequires: pkgconfig(Qt5Quick)
BuildRequires: pkgconfig(expat)
BuildRequires: pkgconfig(flac)
BuildRequires: pkgconfig(glib-2.0)
BuildRequires: pkgconfig(gmodule-2.0)
BuildRequires: pkgconfig(gobject-2.0)
BuildRequires: pkgconfig(gthread-2.0)
BuildRequires: pkgconfig(libevent)
BuildRequires: pkgconfig(libpng)
BuildRequires: pkgconfig(libxml-2.0)
BuildRequires: pkgconfig(libxslt)
BuildRequires: pkgconfig(nss)
BuildRequires: pkgconfig(pangocairo)
BuildRequires: pkgconfig(udev)
BuildRequires: bison
BuildRequires: gperf
#BuildRequires: libicu55
BuildRequires: libjpeg-turbo-devel

%description
Chromium based web browser component for Qt.

%package shell
Summary: QuickSilver demo application
%description shell
Demo application for QuikSilver.

%package devel
Summary: QuickSilver development files
Group: Development/Libraries
%description devel
Development files for QuickSilver.

%prep
%setup -q

%if %{with debugcode}
%define BUILD_TYPE Debug
%else
%define BUILD_TYPE Release
%endif

%define OUTDIR out.qt.sailfish.%{_arch}
%define BUILD_DIR %{OUTDIR}/%{BUILD_TYPE}
%define QS_DATA_DIR %{_libdir}/%{name}

%build
# Let gyp control CFLAGS/CXXFLAGS.
export CFLAGS=""
export CXXFLAGS=""
export FFLAGS=""

%if "%{BUILD_TYPE}" == "Release"
  EXTRA_ARGS="${EXTRA_ARGS} --release"
%endif

# Try to reduce the size of debug binary in order to avoid
# linker OOM errors. Sailfish SDK only ships 32bit toolchains.
%if "%{BUILD_TYPE}" == "Debug"
  # Make the linker conserve memory as much as possible
  export LDFLAGS="${LDFLAGS} -Wl,--no-keep-memory"
  # Compile the code with -g1 instead of -g0
  EXTRA_ARGS+=" -Dfastbuild=1"
%endif

%ifarch armv7hl
  EXTRA_ARGS="${EXTRA_ARGS} --armv7hl --profile mobile"
%endif
%ifarch %ix86
  export CFLAGS="-march=core2 -mtune=atom -mssse3 -mfpmath=sse"
  export CXXFLAGS="${CFLAGS}"
  EXTRA_ARGS="${EXTRA_ARGS} --profile tablet"
%endif

quicksilver/configure \
  --sailfish \
  --scratchbox \
  --system-tools \
  --proprietary-codecs \
  --outdir %{OUTDIR} \
  -Dquicksilver_data_dir=%{QS_DATA_DIR} \
  ${EXTRA_ARGS}

quicksilver/build/prebuilt/ninja -C %{BUILD_DIR} quicksilver_shell chrome_sandbox

%install
# Binaries.
install -p -D %{BUILD_DIR}/quicksilver_shell %{buildroot}%{_bindir}/quicksilver_shell
install -p -D %{BUILD_DIR}/quicksilver_worker %{buildroot}%{QS_DATA_DIR}/quicksilver_worker
install -p -D %{BUILD_DIR}/chrome_sandbox %{buildroot}%{QS_DATA_DIR}/chrome-sandbox
install -p -D %{BUILD_DIR}/lib/libquicksilver.so.%{chrome_version_major} \
    %{buildroot}%{_libdir}/libquicksilver.so.%{chrome_version_major}
ln -sf %{_libdir}/libquicksilver.so.%{chrome_version_major} \
    %{buildroot}%{_libdir}/libquicksilver.so

# Supporting libraries and resources.
install -m 0644 -p -D %{BUILD_DIR}/quicksilver.pak %{buildroot}%{QS_DATA_DIR}/quicksilver.pak
install -m 0644 -p -D %{BUILD_DIR}/snapshot_blob.bin %{buildroot}%{QS_DATA_DIR}/snapshot_blob.bin
install -m 0644 -p -D %{BUILD_DIR}/natives_blob.bin %{buildroot}%{QS_DATA_DIR}/natives_blob.bin

# Development files.
install -m 0755 -d %{buildroot}%{_includedir}
install -m 0755 -d %{buildroot}/%{_libdir}/pkgconfig
cp -r %{BUILD_DIR}/QuickSilver %{buildroot}%{_includedir}/
sed -e "s#__version__#%{chrome_version}#" \
    -e "s#__prefix__#%{_prefix}#" \
    -e "s#__libdir__#%{_libdir}#" \
    -e "s#__includedir__#%{_includedir}#" \
    quicksilver/rpm/quicksilver.pc.in \
    > %{buildroot}%{_libdir}/pkgconfig/quicksilver.pc

# QuickSilver shell desktop file and icon
install -m 0644 -p -D quicksilver/rpm/chromium.png \
    %{buildroot}/%{_datadir}/icons/hicolor/86x86/apps/chromium.png
install -m 0644 -p -D quicksilver/rpm/quicksilver_shell.desktop \
    %{buildroot}/%{_datadir}/applications/quicksilver_shell.desktop

%files
%{_libdir}/libquicksilver.so.%{chrome_version_major}
%{QS_DATA_DIR}/quicksilver_worker
%{QS_DATA_DIR}/quicksilver.pak
%{QS_DATA_DIR}/snapshot_blob.bin
%{QS_DATA_DIR}/natives_blob.bin
%attr(4711, root, root) %{QS_DATA_DIR}/chrome-sandbox

%files devel
%defattr(-,root,root,-)
%{_libdir}/libquicksilver.so
%{_libdir}/pkgconfig
%{_includedir}/*

%files shell
%{_bindir}/quicksilver_shell
%{_datadir}/applications/quicksilver_shell.desktop
%{_datadir}/icons/hicolor/86x86/apps/chromium.png
