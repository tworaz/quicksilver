// Copyright (c) 2015 Piotr Tworek. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef _QUICKSILVER_SHELL_SHELL_BROWSER_PROCESS_H_
#define _QUICKSILVER_SHELL_SHELL_BROWSER_PROCESS_H_

#include <QtCore/QScopedPointer>

#include "quicksilver/public/browser/qs_browser_process.h"

class ShellChrome;

class ShellBrowserProcess : public QuickSilver::QSBrowserProcess {
 public:
  ShellBrowserProcess(int argc, const char** argv);
  ~ShellBrowserProcess() override;

  void SetInitialURLs(QList<QString>&);

  // QuickSilver::QSBrowserProcess implementation.
  void preEarlyInitialization() override;
  void toolkitInitialized() override;
  void preMainMessageLoopRun() override;

 private:
  QScopedPointer<ShellChrome> chrome_;
  QList<QString> initial_urls_;

  Q_DISABLE_COPY(ShellBrowserProcess);
};

#endif // !_QUICKSILVER_SHELL_SHELL_BROWSER_PROCESS_H_
