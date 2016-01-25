// Copyright (c) 2015 Piotr Tworek. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "quicksilver/public/browser/qs_context_menu_params.h"

#include "content/public/common/context_menu_params.h"
#include "ozone_qt/util/type_conversion.h"

using namespace content;

namespace QuickSilver {

QSContextMenuParams::QSContextMenuParams(const ContextMenuParams& params)
    : position_(QPoint(params.x, params.y))
    , link_url_(ToQt(params.link_url))
    , link_text_(ToQt(params.link_text)) {
}

QSContextMenuParams::QSContextMenuParams(const QSContextMenuParams& params)
    : position_(params.position_)
    , link_url_(params.link_url_)
    , link_text_(params.link_text_) {
}

QSContextMenuParams::~QSContextMenuParams() {
}

} // namespace QuickSilver
