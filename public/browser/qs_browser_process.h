// Copyright (c) 2015 Piotr Tworek. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef _QUICKSILVER_PUBLIC_BROWSER_QS_BROWSER_PROCESS_H_
#define _QUICKSILVER_PUBLIC_BROWSER_QS_BROWSER_PROCESS_H_

// XXX: Should be someting like <QSProcess>
#include "quicksilver/public/common/qs_process.h"

namespace QuickSilver {

class QSBrowserProcess : public QSProcess {
 public:
  QSBrowserProcess(int argc, const char** argv);
  ~QSBrowserProcess() override;

  virtual void preEarlyInitialization() {}
  virtual void postEarlyInitialization() {}
  virtual void preMainMessageLoopStart() {}
  virtual void postMainMessageLoopStart() {}
  virtual void toolkitInitialized() {}
  virtual int preCreateThreads();
  virtual void preMainMessageLoopRun() {}
  virtual void postMainMessageLoopRun() {}
  virtual void postDestroyThreads() {}

 private:
  Q_DISABLE_COPY(QSBrowserProcess);
};

} // namespace QuickSilver

#endif // !_QUICKSILVER_PUBLIC_BROWSER_QS_BROWSER_PROCESS_H_
