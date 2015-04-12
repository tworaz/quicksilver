// Copyright (c) 2015 Piotr Tworek. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_QT_ENV_H_
#define UI_QT_ENV_H_

#include "base/memory/scoped_ptr.h"

namespace ui {
class PlatformEventSource;
}

namespace qt {

class Env {
 public:
  static void CreateInstance(bool create_event_source);
  static Env* GetInstance();

 private:
  Env();
  ~Env();

  void Init(bool create_event_source);

  scoped_ptr<ui::PlatformEventSource> event_source_;

  DISALLOW_COPY_AND_ASSIGN(Env);
};

} // qt

#endif // UI_QT_ENV_H_
