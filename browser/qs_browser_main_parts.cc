// Copyright 2015 Piotr Tworek. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "quicksilver/browser/qs_browser_main_parts.h"

#include "components/devtools_http_handler/devtools_http_handler.h"
#include "ozone_qt/components/ime/input_method_qt.h"
#include "ozone_qt/components/mce/memory_pressure_monitor_mce.h"
#include "ozone_qt/components/screen/screen_qt.h"
#include "quicksilver/public/browser/qs_browser_process.h"
#include "ui/aura/env.h"
#include "ui/base/ime/input_method_delegate.h"
#include "ui/base/ime/input_method_factory.h"
#include "ui/gfx/geometry/size.h"

using namespace content;

namespace QuickSilverImpl {

namespace {

QuickSilver::QSBrowserProcess* GetBrowserProcess() {
  return static_cast<QuickSilver::QSBrowserProcess*>(
      QuickSilver::QSBrowserProcess::instance());
}

scoped_ptr<ui::InputMethod> InputMethodFactory(
    ui::internal::InputMethodDelegate* delegate) {
  return make_scoped_ptr(new ui::InputMethodQt(delegate));
}

} // namespace

QSBrowserMainParts::QSBrowserMainParts(const MainFunctionParams& parameters)
    : devtools_http_handler_(nullptr) {
}

QSBrowserMainParts::~QSBrowserMainParts() {
  DCHECK(!devtools_http_handler_);
  aura::Env::DeleteInstance();
}

void QSBrowserMainParts::PreEarlyInitialization() {
  SetUpInputMethodFactory(&InputMethodFactory);
  GetBrowserProcess()->preEarlyInitialization();
}

void QSBrowserMainParts::PostEarlyInitialization() {
  GetBrowserProcess()->postEarlyInitialization();
}

void QSBrowserMainParts::PreMainMessageLoopStart() {
  gfx::Screen::SetScreenInstance(gfx::SCREEN_TYPE_NATIVE, new gfx::ScreenQt);
  DCHECK(aura::Env::GetInstance());

  GetBrowserProcess()->preMainMessageLoopStart();
}

void QSBrowserMainParts::PostMainMessageLoopStart() {
  GetBrowserProcess()->postMainMessageLoopStart();
}

void QSBrowserMainParts::ToolkitInitialized() {
  GetBrowserProcess()->toolkitInitialized();
}

int QSBrowserMainParts::PreCreateThreads() {
  return GetBrowserProcess()->preCreateThreads();
}

void QSBrowserMainParts::PreMainMessageLoopRun() {
#if defined(OS_SAILFISH)
  memory_pressure_monitor_.reset(new base::MemoryPressureMonitorMce);
#endif
  GetBrowserProcess()->preMainMessageLoopRun();
}

void QSBrowserMainParts::PostMainMessageLoopRun() {
  devtools_http_handler_.reset();
  memory_pressure_monitor_.reset();
  GetBrowserProcess()->postMainMessageLoopRun();
}

void QSBrowserMainParts::PostDestroyThreads() {
  GetBrowserProcess()->postDestroyThreads();
}

} // namespace QuickSilverImpl
