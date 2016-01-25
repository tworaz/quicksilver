// Copyright (c) 2015 Piotr Tworek. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef _QUICKSILVER_COMMON_QS_MAIN_DELEGATE_H_
#define _QUICKSILVER_COMMON_QS_MAIN_DELEGATE_H_

#include "base/macros.h"
#include "base/memory/scoped_ptr.h"
#include "content/public/app/content_main_delegate.h"
#include "quicksilver/common/qs_content_client.h"

namespace QuickSilverImpl {

class QSContentBrowserClient;
class QSContentRendererClient;

class QSMainDelegate : public content::ContentMainDelegate {
 public:
  QSMainDelegate();
  ~QSMainDelegate() override;

  bool BasicStartupComplete(int* exit_code) override;
  void PreSandboxStartup() override;

  //content::ContentBrowserClient* CreateContentBrowserClient() override;
  content::ContentRendererClient* CreateContentRendererClient() override;

 private:
  QuickSilverImpl::QSContentClient content_client_;
  //scoped_ptr<QSContentBrowserClient> browser_client_;
  scoped_ptr<QSContentRendererClient> renderer_client_;

  DISALLOW_COPY_AND_ASSIGN(QSMainDelegate);
};

} // nemespace QuickSilverImpl

#endif // !_QUICKSILVER_COMMON_QS_MAIN_DELEGATE_H_
