// Copyright (c) 2015 Piotr Tworek. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "quicksilver/public/browser/qs_web_context.h"

#include "base/logging.h"
#include "quicksilver/browser/qs_browser_context.h"

using namespace QuickSilverImpl;

namespace QuickSilver {

QSWebContext::QSWebContext(bool priv)
    : impl_(new QSBrowserContext(priv, nullptr)) { // XXX: support net logging
}

QSWebContext::~QSWebContext() {
}

QuickSilverImpl::QSBrowserContext* QSWebContext::impl() const {
  return impl_.data();
}

} // namespace QuickSilver
