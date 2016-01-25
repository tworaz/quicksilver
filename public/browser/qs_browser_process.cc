// Copyright (c) 2015 Piotr Tworek. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "quicksilver/public/browser/qs_browser_process.h"

#include "quicksilver/public/browser/qs_context_menu_params.h"
#include "quicksilver/public/browser/qs_javascript_dialog.h"
#include "quicksilver/public/browser/qs_permission_request.h"
#include "quicksilver/public/browser/qs_web_area.h"

namespace QuickSilver {

QSBrowserProcess::QSBrowserProcess(int argc, const char** argv)
    : QSProcess(argc, argv) {

  qmlRegisterType<QSWebArea>("QuickSilver", 1, 0, "QSWebArea");
  qmlRegisterInterface<QSContextMenuParams>("QSContextMenuParams");
  qmlRegisterUncreatableType<QSJavaScriptDialog>(
      "QuickSilver", 1, 0, "QSJavaScriptDialog", "");
  qmlRegisterUncreatableType<QSPermissionRequest>(
      "QuickSilver", 1, 0, "QSPermissionRequest", "");
}

QSBrowserProcess::~QSBrowserProcess() {
}

int QSBrowserProcess::preCreateThreads() {
  return 0;
}

} // namespace QuickSilver
