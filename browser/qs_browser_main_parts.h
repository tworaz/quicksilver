// Copyright 2015 Piotr Tworek. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef QUICKSILVER_BROWSER_QS_BROWSER_MAIN_PARTS_H_
#define QUICKSILVER_BROWSER_QS_BROWSER_MAIN_PARTS_H_

#include "base/memory/scoped_ptr.h"
#include "content/public/browser/browser_main_parts.h"
#include "content/public/common/main_function_params.h"

namespace base {
class MemoryPressureMonitor;
}

namespace devtools_http_handler {
class DevToolsHttpHandler;
}

namespace QuickSilverImpl {

class QSBrowserMainParts : public content::BrowserMainParts {
  public:
   explicit QSBrowserMainParts(const content::MainFunctionParams& parameters);
   ~QSBrowserMainParts() override;

  // BrowserMainParts overrides.
  void PreEarlyInitialization() override;
  void PostEarlyInitialization() override;
  void PreMainMessageLoopStart() override;
  void PostMainMessageLoopStart() override;
  void ToolkitInitialized() override;
  int PreCreateThreads() override;
  void PreMainMessageLoopRun() override;
  void PostMainMessageLoopRun() override;
  void PostDestroyThreads() override;

 private:
  scoped_ptr<devtools_http_handler::DevToolsHttpHandler> devtools_http_handler_;
  scoped_ptr<base::MemoryPressureMonitor> memory_pressure_monitor_;

  DISALLOW_COPY_AND_ASSIGN(QSBrowserMainParts);
};

} // namespace QuickSilverImpl

#endif // !QUICKSILVER_BROWSER_QS_BROWSER_MAIN_PARTS_H_
