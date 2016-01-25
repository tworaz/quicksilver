// Copyright (c) 2015 Piotr Tworek. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "quicksilver/public/browser/qs_javascript_dialog.h"

namespace QuickSilver {

QSJavaScriptDialog::QSJavaScriptDialog(Type type, QUrl url,
                                       QString msg, QString prompt)
    : QObject(nullptr)
    , type_(type)
    , origin_url_(url)
    , message_text_(msg)
    , prompt_text_(prompt) {
}

QSJavaScriptDialog::~QSJavaScriptDialog() {
}

} // namespace QuickSilver
