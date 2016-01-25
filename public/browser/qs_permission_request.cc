// Copyright (c) 2015 Piotr Tworek. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "quicksilver/public/browser/qs_permission_request.h"

namespace QuickSilver {

QSPermissionRequest::QSPermissionRequest(Type type, int id,
                                         bool user_gesture, QUrl url)
    : type_(type)
    , id_(id)
    , user_gesture_(user_gesture)
    , url_(url) {
}

QSPermissionRequest::~QSPermissionRequest() {
}

} // namespace QuickSilver
