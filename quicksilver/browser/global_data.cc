// Copyright (c) 2015 Piotr Tworek. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "quicksilver/browser/global_data.h"

#include <QOpenGLContext>
#include <QGuiApplication>
#include <QScreen>
#undef signals

#include "base/command_line.h"
#include "base/logging.h"
#include "base/run_loop.h"
#include "content/browser/renderer_host/render_process_host_impl.h"
#include "content/browser/utility_process_host_impl.h"
#include "content/browser/gpu/gpu_process_host.h"
#include "content/gpu/in_process_gpu_thread.h"
#include "content/renderer/in_process_renderer_thread.h"
#include "content/public/app/content_main.h"
#include "content/public/app/content_main_runner.h"
#include "content/public/browser/browser_main_runner.h"
#include "content/public/common/main_function_params.h"
#include "content/utility/in_process_utility_thread.h"
#include "quicksilver/common/content_main_delegate_qs.h"
#include "quicksilver/browser/message_pump_qt.h"

Q_GUI_EXPORT void qt_gl_set_global_share_context(QOpenGLContext*);

namespace QuickSilverImpl {

namespace {

GlobalData* g_global_data_instance;
QOpenGLContext* g_share_context;

static scoped_ptr<base::MessagePump> MessagePumpFactory() {
    return scoped_ptr<base::MessagePump>(new MessagePumpQt);
}

static void DeleteShareContext() {
  delete g_share_context;
	g_share_context = NULL;
}

static void DeleteGlobalDataInstance() {
  qt_gl_set_global_share_context(NULL);
  delete g_global_data_instance;
	g_global_data_instance = NULL;
}

} // namespace

// static
GlobalData& GlobalData::GetInstance() {
  if (!g_global_data_instance)
    g_global_data_instance = new GlobalData();
  return *g_global_data_instance;
}

GlobalData::GlobalData()
    : content_main_runner_(content::ContentMainRunner::Create()),
      browser_main_runner_(content::BrowserMainRunner::Create()) {
}

GlobalData::~GlobalData() {
  base::MessagePump::Delegate* delegate = run_loop_->loop_;
  while (delegate->DoWork()) {}
  run_loop_->AfterRun();
}

bool GlobalData::Initialize(int argc, char* argv[]) {
  static bool initialized = false;
  if (initialized)
    return true;

  base::MessageLoop::InitMessagePumpForUIFactory(MessagePumpFactory);

  base::CommandLine::Init(argc, (const char**)argv);

  SetContentClient(&content_client_);

  DVLOG(1) << "Device scale factor: " <<
      qApp->primaryScreen()->devicePixelRatio();
  std::vector<ui::ScaleFactor> supported_scale_factors;
  // XXX: Don't hardcode the list of supported scale factors,
  supported_scale_factors.push_back(ui::SCALE_FACTOR_100P);
  supported_scale_factors.push_back(ui::SCALE_FACTOR_150P);
  supported_scale_factors.push_back(ui::SCALE_FACTOR_200P);
  ui::SetSupportedScaleFactors(supported_scale_factors);

  content::UtilityProcessHostImpl::RegisterUtilityMainThreadFactory(
      content::CreateInProcessUtilityThread);
  content::RenderProcessHostImpl::RegisterRendererMainThreadFactory(
      content::CreateInProcessRendererThread);
  content::GpuProcessHost::RegisterGpuMainThreadFactory(
      content::CreateInProcessGpuThread);

	g_share_context = new QOpenGLContext;
  g_share_context->create();
	qAddPostRoutine(DeleteShareContext);
  qt_gl_set_global_share_context(g_share_context);

  content::ContentMainParams content_params(new ContentMainDelegateQS);
  content_params.argc = argc;
  content_params.argv = (const char**)argv;

  if (!content_main_runner_->Initialize(content_params)) {
    LOG(ERROR) << "Failed to initialize content main runner!";
    return false;
  }

  content::MainFunctionParams browser_params(
      *base::CommandLine::ForCurrentProcess());
  if (!browser_main_runner_->Initialize(browser_params)) {
    LOG(ERROR) << "Failed to initialize browser main runner!";
    return false;
  }

  run_loop_.reset(new base::RunLoop);
  run_loop_->BeforeRun();

  qAddPostRoutine(DeleteGlobalDataInstance);

  initialized = true;
  return true;
}

} // namespace QuickSilverImpl
