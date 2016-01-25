// Copyright 2015 Piotr Tworek. All rights reserved.
// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "quicksilver/browser/qs_content_browser_client.h"

#include "base/strings/string_util.h"
#include "content/public/common/content_switches.h"
#include "content/public/common/web_preferences.h"
#include "quicksilver/browser/qs_browser_context.h"
#include "quicksilver/browser/qs_browser_main_parts.h"
#include "quicksilver/browser/qs_web_contents_view_delegate.h"
#include "quicksilver/common/qs_profile.h"
#include "quicksilver/common/qs_switches.h"

using namespace content;

namespace QuickSilverImpl {

namespace {

QSContentBrowserClient* g_browser_client;

} // namespace

QSContentBrowserClient* QSContentBrowserClient::Get() {
  return g_browser_client;
}

QSContentBrowserClient::QSContentBrowserClient() {
  DCHECK(!g_browser_client);
  g_browser_client = this;
}

QSContentBrowserClient::~QSContentBrowserClient() {
  g_browser_client = NULL;
}

BrowserMainParts* QSContentBrowserClient::CreateBrowserMainParts(
    const MainFunctionParams& parameters) {
  browser_main_parts_ = new QSBrowserMainParts(parameters);
  return browser_main_parts_;
}

net::URLRequestContextGetter* QSContentBrowserClient::CreateRequestContext(
    BrowserContext* content_browser_context,
    ProtocolHandlerMap* protocol_handlers,
    URLRequestInterceptorScopedVector request_interceptors) {

  QSBrowserContext* ctx = static_cast<QSBrowserContext*>(
      content_browser_context);
  return ctx->CreateRequestContext(protocol_handlers,
                                   request_interceptors.Pass());
}

net::URLRequestContextGetter*
QSContentBrowserClient::CreateRequestContextForStoragePartition(
    BrowserContext* content_browser_context,
    const base::FilePath& partition_path,
    bool in_memory,
    ProtocolHandlerMap* protocol_handlers,
    URLRequestInterceptorScopedVector request_interceptors) {

  QSBrowserContext* ctx = static_cast<QSBrowserContext*>(
      content_browser_context);
  return ctx->CreateRequestContextForStoragePartition(
      partition_path,
      in_memory,
      protocol_handlers,
      request_interceptors.Pass());
}

bool QSContentBrowserClient::IsHandledURL(const GURL& url) {
  if (!url.is_valid())
    return false;
  // Keep in sync with ProtocolHandlers added by
  // QSURLRequestContextGetter::GetURLRequestContext().
  static const char* const kProtocolList[] = {
      url::kBlobScheme,
      url::kFileSystemScheme,
      //kChromeDevToolsScheme, // XXX: support devtools
      url::kDataScheme,
      url::kFileScheme,
  };
  for (size_t i = 0; i < arraysize(kProtocolList); ++i) {
    if (url.scheme() == kProtocolList[i])
      return true;
  }
  return false;
}

void QSContentBrowserClient::AppendExtraCommandLineSwitches(
    base::CommandLine* command_line, int child_process_id) {
  base::CommandLine* current_cmdline = base::CommandLine::ForCurrentProcess();
  if (current_cmdline->HasSwitch(switches::kQuickSilverProfile)) {
    std::string value = current_cmdline->
        GetSwitchValueASCII(switches::kQuickSilverProfile);
    command_line->AppendSwitchASCII(switches::kQuickSilverProfile, value);
  }
}

WebContentsViewDelegate* QSContentBrowserClient::GetWebContentsViewDelegate(
    WebContents* web_contents) {
  return new QSWebContentsViewDelegate(web_contents);
}

void QSContentBrowserClient::OverrideWebkitPrefs(RenderViewHost*,
                                                 WebPreferences* prefs) {
  if (CurrentProfile() == ProfileType::MOBILE ||
      CurrentProfile() == ProfileType::TABLET) {
    // Default values for mobile configurations should match android settings
    // from: content/public/common/web_preferences.cc.
    prefs->viewport_enabled = true;
    prefs->viewport_meta_enabled = true;
    prefs->default_minimum_page_scale_factor = 0.25f,
    prefs->default_maximum_page_scale_factor = 5.f;
  }
}

} // namespace QuickSilverImpl
