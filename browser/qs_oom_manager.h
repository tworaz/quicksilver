// Copyright (c) 2015 Piotr Tworek. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef QUICKSILVER_BROWSER_QS_OOM_MANAGER_H_
#define QUICKSILVER_BROWSER_QS_OOM_MANAGER_H_

#include <vector>

#include "base/memory/memory_pressure_listener.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/singleton.h"
#include "base/timer/timer.h"

namespace QuickSilverImpl {

class QSWebViewImpl;

class QSOomManager {
 public:
  static QSOomManager* GetInstance();

  void AddWebView(QSWebViewImpl*);
  void RemoveWebView(QSWebViewImpl*);
  void WebViewActivated(QSWebViewImpl*);

 private:
  friend struct base::DefaultSingletonTraits<QSOomManager>;

  QSOomManager();
  ~QSOomManager();

  void OnMemoryPressure(base::MemoryPressureListener::MemoryPressureLevel);
  void OnModerateMemoryPressure();
  void OnCriticalMemoryPressure();
  void DiscardNextViewRenderer();

  scoped_ptr<base::MemoryPressureListener> memory_pressure_listener_;
  std::vector<QSWebViewImpl*> managed_views_;
  base::RepeatingTimer timer_;

  DISALLOW_COPY_AND_ASSIGN(QSOomManager);
};

} // namespace QuickSilverImpl

#endif // QUICKSILVER_BROWSER_QS_OOM_MANAGER_H_

