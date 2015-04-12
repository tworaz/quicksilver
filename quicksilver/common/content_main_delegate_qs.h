// Copyright (c) 2015 Piotr Tworek. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef QUICKSILVER_COMMON_CONTENT_MAIN_DELEGATE_QS_H_
#define QUICKSILVER_COMMON_CONTENT_MAIN_DELEGATE_QS_H_

#include "base/macros.h"
#include "base/memory/scoped_ptr.h"
#include "content/public/app/content_main_delegate.h"

namespace QuickSilverImpl {

class ContentBrowserClientQS;
class ContentRendererClientQS;

class ContentMainDelegateQS : public content::ContentMainDelegate {
 public:
  ContentMainDelegateQS();
  ~ContentMainDelegateQS() override;

  bool BasicStartupComplete(int* exit_code) override;
  void PreSandboxStartup() override;

  content::ContentBrowserClient* CreateContentBrowserClient() override;
  content::ContentRendererClient* CreateContentRendererClient() override;

 private:
  scoped_ptr<ContentBrowserClientQS> browser_client_;
  scoped_ptr<ContentRendererClientQS> renderer_client_;

  DISALLOW_COPY_AND_ASSIGN(ContentMainDelegateQS);
};

} // namespace QuickSilverImpl

#endif // QUICKSILVER_COMMON_CONTENT_MAIN_DELEGATE_QS_H_
