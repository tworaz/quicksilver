// Copyright 2015 Piotr Tworek. All rights reserved.
// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef QUICKSILVER_BROWSER_BROWSER_MAIN_PARTS_QS_H_
#define QUICKSILVER_BROWSER_BROWSER_MAIN_PARTS_QS_H_

#include "base/memory/scoped_ptr.h"
#include "content/public/browser/browser_main_parts.h"
#include "content/public/common/main_function_params.h"
#include "net/log/net_log.h"
#include "quicksilver/browser/browser_context_qs.h"

namespace devtools_http_handler {
class DevToolsHttpHandler;
}

namespace net {
class NetLog;
}

namespace QuickSilverImpl {

class BrowserMainPartsQS : public content::BrowserMainParts {
  public:
   explicit BrowserMainPartsQS(const content::MainFunctionParams& parameters);
   ~BrowserMainPartsQS() override;

  // BrowserMainParts overrides.
  void PreMainMessageLoopRun() override;
  void PostMainMessageLoopRun() override;

  BrowserContextQS* browser_context() { return browser_context_.get(); }
  BrowserContextQS* off_the_record_browser_context() {
    return off_the_record_browser_context_.get();
  }

  net::NetLog* net_log() { return net_log_.get(); }

 private:
  scoped_ptr<net::NetLog> net_log_;
  scoped_ptr<BrowserContextQS> browser_context_;
  scoped_ptr<BrowserContextQS> off_the_record_browser_context_;
  scoped_ptr<devtools_http_handler::DevToolsHttpHandler> devtools_http_handler_;

  DISALLOW_COPY_AND_ASSIGN(BrowserMainPartsQS);
};

} // namespace QuickSilverImpl

#endif // !QUICKSILVER_BROWSER_BROWSER_MAIN_PARTS_QS_H_
