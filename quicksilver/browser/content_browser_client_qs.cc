// Copyright 2015 Piotr Tworek. All rights reserved.
// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "quicksilver/browser/content_browser_client_qs.h"

#include "base/strings/string_util.h"
#include "content/public/common/content_switches.h"
#include "quicksilver/browser/browser_main_parts_qs.h"

using namespace content;

namespace QuickSilverImpl {

namespace {
ContentBrowserClientQS* g_browser_client;
} // namespace

ContentBrowserClientQS* ContentBrowserClientQS::Get() {
  return g_browser_client;
}

ContentBrowserClientQS::ContentBrowserClientQS() {
  DCHECK(!g_browser_client);
  g_browser_client = this;
}

ContentBrowserClientQS::~ContentBrowserClientQS() {
  g_browser_client = NULL;
}

BrowserMainParts* ContentBrowserClientQS::CreateBrowserMainParts(
    const MainFunctionParams& parameters) {
  browser_main_parts_ = new BrowserMainPartsQS(parameters);
  return browser_main_parts_;
}

net::URLRequestContextGetter* ContentBrowserClientQS::CreateRequestContext(
    BrowserContext* content_browser_context,
    ProtocolHandlerMap* protocol_handlers,
    URLRequestInterceptorScopedVector request_interceptors) {
  BrowserContextQS* browser_context_qs =
      BrowserContextQSForBrowserContext(content_browser_context);
  return browser_context_qs->CreateRequestContext(
      protocol_handlers, request_interceptors.Pass());
}

net::URLRequestContextGetter*
ContentBrowserClientQS::CreateRequestContextForStoragePartition(
    BrowserContext* content_browser_context,
    const base::FilePath& partition_path,
    bool in_memory,
    ProtocolHandlerMap* protocol_handlers,
    URLRequestInterceptorScopedVector request_interceptors) {
  BrowserContextQS* browser_context_qs =
      BrowserContextQSForBrowserContext(content_browser_context);
  return browser_context_qs->CreateRequestContextForStoragePartition(
      partition_path,
      in_memory,
      protocol_handlers,
      request_interceptors.Pass());
}

bool ContentBrowserClientQS::IsHandledURL(const GURL& url) {
  if (!url.is_valid())
    return false;
  DCHECK_EQ(url.scheme(), base::StringToLowerASCII(url.scheme()));
  // Keep in sync with ProtocolHandlers added by
  // URLRequestContextGetterQS::GetURLRequestContext().
  static const char* const kProtocolList[] = {
      url::kBlobScheme,
      url::kFileSystemScheme,
      //kChromeDevToolsScheme,
      url::kDataScheme,
      url::kFileScheme,
  };
  for (size_t i = 0; i < arraysize(kProtocolList); ++i) {
    if (url.scheme() == kProtocolList[i])
      return true;
  }
  return false;
}

void ContentBrowserClientQS::AppendExtraCommandLineSwitches(
    base::CommandLine* command_line,
    int child_process_id) {
}

BrowserContextQS* ContentBrowserClientQS::browser_context() {
  return browser_main_parts_->browser_context();
}

BrowserContextQS* ContentBrowserClientQS::off_the_record_browser_context() {
  return browser_main_parts_->off_the_record_browser_context();
}

BrowserContextQS* ContentBrowserClientQS::BrowserContextQSForBrowserContext(
    content::BrowserContext* content_browser_context) {
  if (content_browser_context == browser_context())
    return browser_context();
  DCHECK_EQ(content_browser_context, off_the_record_browser_context());
  return off_the_record_browser_context();
}

} // namespace QuickSilverImpl
