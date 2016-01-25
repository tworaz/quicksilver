// Copyright (c) 2015 Piotr Tworek. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "quicksilver/shell/shell_browser_process.h"

#include <QtGui/QGuiApplication>
#include <QtGui/QScreen>
#include <QtGui/QTouchDevice>

#include "quicksilver/shell/shell_chrome.h"

namespace {

const QString kHomePage = "https://www.duckduckgo.com";
const char kForceGpuMemAvailableMB[] = "force-gpu-mem-available-mb";
const char kForceDeviceScaleFactor[] = "force-device-scale-factor";
const char kSimulateTouchScreenWithMouse[] = "simulate-touch-screen-with-mouse";

} // namespace

ShellBrowserProcess::ShellBrowserProcess(int argc, const char** argv)
    : QuickSilver::QSBrowserProcess(argc, argv) {
}

ShellBrowserProcess::~ShellBrowserProcess() {
}

void ShellBrowserProcess::SetInitialURLs(QList<QString>& urls) {
  initial_urls_.append(urls);
}

void ShellBrowserProcess::preEarlyInitialization() {
  // This needs to be decided before ozone layer is initialized, which
  // means before toolkitInitialized() is called.
  bool hasTouchScreen = false;
  for (const QTouchDevice* device : QTouchDevice::devices()) {
    if (device->type() == QTouchDevice::TouchScreen) {
      hasTouchScreen = true;
    }
  }

  if (!hasTouchScreen) {
    QSBrowserProcess::appendSwitch(kSimulateTouchScreenWithMouse);
  }

#if defined(OS_SAILFISH)
  // XXX: Maybe there is some programatic way to detect amout of GPU memory.
  if (!QSBrowserProcess::hasSwitch(kForceGpuMemAvailableMB)) {
    QSBrowserProcess::appendSwitch(kForceGpuMemAvailableMB, "128");
  }
#endif
}

void ShellBrowserProcess::toolkitInitialized() {
#if defined(OS_SAILFISH)
  if (!QSBrowserProcess::hasSwitch(kForceDeviceScaleFactor)) {
    // XXX: Better device scale factor detection would be nice.
    const QScreen* screen = qApp->primaryScreen();
    int dim = qMax(screen->size().width(), screen->size().height());
    if (dim >= 1000) {
      QSBrowserProcess::appendSwitch(kForceDeviceScaleFactor, "2");
    } else {
      QSBrowserProcess::appendSwitch(kForceDeviceScaleFactor, "1.5");
    }
  }
#else
  if (!QSBrowserProcess::hasSwitch(kForceDeviceScaleFactor)) {
    QSBrowserProcess::appendSwitch(kForceDeviceScaleFactor, "1.5");
  }
#endif
  qApp->setApplicationDisplayName("quicksilver_shell");
}

void ShellBrowserProcess::preMainMessageLoopRun() {
  chrome_.reset(new ShellChrome);

  if (initial_urls_.isEmpty()) {
    chrome_->CreateTab(kHomePage);
  } else {
    QList<QString>::iterator it = initial_urls_.begin();
    for (; it != initial_urls_.end(); ++it) {
      chrome_->CreateTab(*it);
    }
    initial_urls_.clear();
  }
}
