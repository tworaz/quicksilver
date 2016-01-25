// Copyright 2015 Piotr Tworek. All rights reserved.
// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef QUICKSILVER_BROWSER_QS_CONTENT_BROWSER_CLIENT_H_
#define QUICKSILVER_BROWSER_QS_CONTENT_BROWSER_CLIENT_H_

#include "content/public/browser/content_browser_client.h"

namespace QuickSilverImpl {

class QSBrowserMainParts;

class QSContentBrowserClient : public content::ContentBrowserClient {
 public:
  // Gets the current instance.
  static QSContentBrowserClient* Get();

  QSContentBrowserClient();
  ~QSContentBrowserClient() override;

  // ContentBrowserClient:
  content::BrowserMainParts* CreateBrowserMainParts(
      const content::MainFunctionParams& parameters) override;
  net::URLRequestContextGetter* CreateRequestContext(
      content::BrowserContext* browser_context,
      content::ProtocolHandlerMap* protocol_handlers,
      content::URLRequestInterceptorScopedVector request_interceptors) override;
  net::URLRequestContextGetter* CreateRequestContextForStoragePartition(
      content::BrowserContext* browser_context,
      const base::FilePath& partition_path,
      bool in_memory,
      content::ProtocolHandlerMap* protocol_handlers,
      content::URLRequestInterceptorScopedVector request_interceptors) override;
  bool IsHandledURL(const GURL& url) override;
  void AppendExtraCommandLineSwitches(base::CommandLine* command_line,
                                      int child_process_id) override;
  content::WebContentsViewDelegate* GetWebContentsViewDelegate(
      content::WebContents*) override;
  void OverrideWebkitPrefs(content::RenderViewHost*,
                           content::WebPreferences*) override;

 private:
  QSBrowserMainParts* browser_main_parts_;

  DISALLOW_COPY_AND_ASSIGN(QSContentBrowserClient);
};

} // namespace QuickSilverImpl

#endif // !QUICKSILVER_BROWSER_QS_CONTENT_BROWSER_CLIENT_H_
