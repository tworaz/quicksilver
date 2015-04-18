// Copyright (c) 2015 Piotr Tworek. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <QtQml>

#include "quicksilver/browser/global_data.h"
#include "quicksilver/browser/public/WebView.h"
#include "quicksilver/browser/public/NewWindowPermissionRequest.h"

using namespace QuickSilverImpl;

namespace QuickSilver {

bool Initialize(int argc, char* argv[]) {
  qmlRegisterType<WebView>("QuickSilver", 1, 0, "QuickSilverWebView");
  qmlRegisterUncreatableType<NewWindowPermissionRequest>(
      "QuickSilver", 1, 0, "NewWindowPermissionRequest", "");
  return GlobalData::GetInstance().Initialize(argc, argv);
}

} // namespace QuickSilver
