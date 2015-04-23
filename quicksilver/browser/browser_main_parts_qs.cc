// Copyright 2015 Piotr Tworek. All rights reserved.
// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "quicksilver/browser/browser_main_parts_qs.h"

#include "base/logging.h"
#include "components/devtools_http_handler/devtools_http_handler.h"
#include "content/shell/browser/shell_net_log.h"

using namespace content;

namespace QuickSilverImpl {

BrowserMainPartsQS::BrowserMainPartsQS(
    const MainFunctionParams& parameters)
    : devtools_http_handler_(nullptr) {
}

BrowserMainPartsQS::~BrowserMainPartsQS() {
  DCHECK(!devtools_http_handler_);
}

void BrowserMainPartsQS::PreMainMessageLoopRun() {
  net_log_.reset(new ShellNetLog("quicksilver"));
  browser_context_.reset(new BrowserContextQS(false, net_log_.get()));
  off_the_record_browser_context_.reset(
      new BrowserContextQS(true, net_log_.get()));

  //devtools_http_handler_.reset(
  //    ShellDevToolsManagerDelegate::CreateHttpHandler(browser_context_.get()));
}

void BrowserMainPartsQS::PostMainMessageLoopRun() {
  devtools_http_handler_.reset();
  browser_context_.reset();
  off_the_record_browser_context_.reset();
}

} // namespace QuickSilverImpl
