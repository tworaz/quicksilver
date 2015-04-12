# Copyright (c) 2015 Piotr Tworek. All rights reserved.
# Use of this source code is governed by a BSD-style license that can be
# found in the LICENSE file.

Name:    quicksilver
Summary: Chromium based WebView component for Qt Quick
Version: 44.0.2366.0
Release: 1
Group:   Applications/Internet
License: LGPL and MIT
Source0: %{name}-%{version}.tar.bz2

BuildRequires: pkgconfig(Qt5Core)
BuildRequires: pkgconfig(Qt5Qml)
BuildRequires: pkgconfig(Qt5Gui)
BuildRequires: pkgconfig(Qt5Quick)
BuildRequires: pkgconfig(Qt5OpenGL)
BuildRequires: pkgconfig(glib-2.0)
BuildRequires: pkgconfig(gmodule-2.0)
BuildRequires: pkgconfig(gobject-2.0)
BuildRequires: pkgconfig(gthread-2.0)
BuildRequires: pkgconfig(nss)
BuildRequires: pkgconfig(expat)
BuildRequires: pkgconfig(pangocairo)
BuildRequires: pkgconfig(libevent)
BuildRequires: pkgconfig(libpng)
BuildRequires: pkgconfig(libxslt)
BuildRequires: pkgconfig(libxml-2.0)
BuildRequires: pkgconfig(flac)
BuildRequires: pkgconfig(udev)
BuildRequires: pkgconfig(icu-uc)
BuildRequires: pkgconfig(icu-i18n)
BuildRequires: libjpeg-turbo-devel
BuildRequires: qt5-qttools
BuildRequires: bison
BuildRequires: gperf

%description
Chromium based WebView component for Qt Quick.

%package shell
Summary: QuickSilver demo application
%description shell
Demo application for QuikSilver.

%package devel
Summary: QuickSilver development files
Group: Development/Libraries
%description devel
Development files for QuickSilver WebView component.

%prep
%setup -q

%define BUILD_TYPE Release

%define OUTDIR out.qt.sailfish.%{_arch}
%define BUILD_DIR %{OUTDIR}/%{BUILD_TYPE}
%define QS_DATA_DIR %{_libdir}/%{name}

%build
# Let gyp control CFLAGS/CXXFLAGS
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
  export EXTRA_ARGS="${EXTRA_ARGS} -Dfastbuild=1"
%endif

%ifarch armv7hl
  EXTRA_ARGS="${EXTRA_ARGS} --armv7hl"
%endif

qt_port/configure \
  --sailfish \
  --scratchbox \
  --system-tools \
  --outdir %{OUTDIR} \
  -Dquicksilver_data_dir=%{QS_DATA_DIR} \
  ${EXTRA_ARGS}

qt_port/build/prebuilt/ninja -C %{BUILD_DIR} quicksilver_shell

%install
# Binaries.
install -p -D %{BUILD_DIR}/quicksilver_shell %{buildroot}%{_bindir}/quicksilver_shell
install -p -D %{BUILD_DIR}/quicksilver_renderer %{buildroot}%{QS_DATA_DIR}/quicksilver_renderer
install -p -D %{BUILD_DIR}/lib/libquicksilver.so %{buildroot}%{_libdir}/libquicksilver.so

# Supporting libraries and resources.
install -p -D %{BUILD_DIR}/libffmpegsumo.so %{buildroot}%{QS_DATA_DIR}/libffmpegsumo.so
install -m 0644 -p -D %{BUILD_DIR}/quicksilver.pak %{buildroot}%{QS_DATA_DIR}/quicksilver.pak
install -m 0644 -p -D %{BUILD_DIR}/snapshot_blob.bin %{buildroot}%{QS_DATA_DIR}/snapshot_blob.bin
install -m 0644 -p -D %{BUILD_DIR}/natives_blob.bin %{buildroot}%{QS_DATA_DIR}/natives_blob.bin

%files
%{_libdir}/libquicksilver.so
%{QS_DATA_DIR}/quicksilver_renderer
%{QS_DATA_DIR}/quicksilver.pak
%{QS_DATA_DIR}/libffmpegsumo.so
%{QS_DATA_DIR}/snapshot_blob.bin
%{QS_DATA_DIR}/natives_blob.bin

%files shell
%{_bindir}/quicksilver_shell

