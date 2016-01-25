// Copyright (c) 2015 Piotr Tworek. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
//
#include "quicksilver/public/common/qs_process.h"

#include "base/command_line.h"
#include "base/message_loop/message_loop.h"
#include "base/thread_task_runner_handle.h"
#include "content/public/app/content_main.h"
#include "content/public/browser/browser_thread.h"
#include "quicksilver/common/qs_main_delegate.h"

using namespace QuickSilverImpl;
using namespace content;

namespace QuickSilver {

namespace {

static QSProcess* g_process_instance;

} // namespace

QSProcess::QSProcess(int argc, const char** argv)
    : argc_(argc)
    , argv_(argv) {
  g_process_instance = this;
}

QSProcess::~QSProcess() {
  DCHECK(g_process_instance == this);
  g_process_instance = nullptr;
}

//static
QSProcess* QSProcess::instance() {
  return g_process_instance;
}

int QSProcess::exec() {
  QuickSilverImpl::QSMainDelegate main_delegate;

  content::ContentMainParams params(&main_delegate);
  params.argc = argc_;
  params.argv = argv_;

  return content::ContentMain(params);
}

void QSProcess::exit() {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::UI));
  base::MessageLoop::current()->QuitWhenIdle();
  base::MessageLoop::current()->RunUntilIdle();
}

void QSProcess::appendSwitch(QString switch_string) const {
  base::CommandLine::ForCurrentProcess()->AppendSwitch(
      switch_string.toStdString());
}

void QSProcess::appendSwitch(QString switch_string, QString switch_value) const {
  base::CommandLine::ForCurrentProcess()->AppendSwitchASCII(
      switch_string.toStdString(), switch_value.toStdString());
}

bool QSProcess::hasSwitch(const char switch_constant[]) const {
  return base::CommandLine::ForCurrentProcess()->HasSwitch(switch_constant);
}

} // namespace QuickSilver

