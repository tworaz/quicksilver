// Copyright (c) 2015 Piotr Tworek. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef _QUICKSILVER_BROWSER_GLOBAL_DATA_H_
#define _QUICKSILVER_BROWSER_GLOBAL_DATA_H_

#include "base/macros.h"
#include "base/memory/scoped_ptr.h"
#include "quicksilver/common/content_client_qs.h"

class QOpenGLContext;

namespace base {
class RunLoop;
}

namespace content {
class BrowserMainRunner;
class ContentMainRunner;
}

namespace QuickSilverImpl {

class GlobalData {
 public:
  ~GlobalData();

  static GlobalData& GetInstance();

  bool Initialize(int argc, char* argv[]);

 private:
  GlobalData();

  ContentClientQS content_client_;
  scoped_ptr<content::ContentMainRunner> content_main_runner_;
  scoped_ptr<content::BrowserMainRunner> browser_main_runner_;
  scoped_ptr<base::RunLoop> run_loop_;

  DISALLOW_COPY_AND_ASSIGN(GlobalData);
};

} // namespace QuickSilverImpl

#endif // !_QUICKSILVER_BROWSER_GLOBAL_DATA_H_
