// Copyright 2015 Piotr Tworek. All rights reserved.
// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "quicksilver/browser/qs_network_delegate.h"

#include "base/command_line.h"
#include "content/public/common/content_switches.h"
#include "net/base/net_errors.h"
#include "net/base/static_cookie_policy.h"
#include "net/url_request/url_request.h"

namespace QuickSilverImpl {

namespace {
bool g_accept_all_cookies = true;
}

QSNetworkDelegate::QSNetworkDelegate() {
}

QSNetworkDelegate::~QSNetworkDelegate() {
}

void QSNetworkDelegate::SetAcceptAllCookies(bool accept) {
  g_accept_all_cookies = accept;
}

int QSNetworkDelegate::OnBeforeURLRequest(
    net::URLRequest* request,
    const net::CompletionCallback& callback,
    GURL* new_url) {
  return net::OK;
}

int QSNetworkDelegate::OnBeforeSendHeaders(
    net::URLRequest* request,
    const net::CompletionCallback& callback,
    net::HttpRequestHeaders* headers) {
  return net::OK;
}

void QSNetworkDelegate::OnSendHeaders(
    net::URLRequest* request,
    const net::HttpRequestHeaders& headers) {
}

int QSNetworkDelegate::OnHeadersReceived(
    net::URLRequest* request,
    const net::CompletionCallback& callback,
    const net::HttpResponseHeaders* original_response_headers,
    scoped_refptr<net::HttpResponseHeaders>* override_response_headers,
    GURL* allowed_unsafe_redirect_url) {
  return net::OK;
}

void QSNetworkDelegate::OnBeforeRedirect(net::URLRequest* request,
                                         const GURL& new_location) {
}

void QSNetworkDelegate::OnResponseStarted(net::URLRequest* request) {
}

void QSNetworkDelegate::OnCompleted(net::URLRequest* request, bool started) {
}

void QSNetworkDelegate::OnURLRequestDestroyed(net::URLRequest* request) {
}

void QSNetworkDelegate::OnPACScriptError(int line_number,
                                         const base::string16& error) {
}

QSNetworkDelegate::AuthRequiredResponse QSNetworkDelegate::OnAuthRequired(
    net::URLRequest* request,
    const net::AuthChallengeInfo& auth_info,
    const AuthCallback& callback,
    net::AuthCredentials* credentials) {
  return AUTH_REQUIRED_RESPONSE_NO_ACTION;
}

bool QSNetworkDelegate::OnCanGetCookies(const net::URLRequest& request,
                                        const net::CookieList& cookie_list) {
  net::StaticCookiePolicy::Type policy_type = g_accept_all_cookies ?
      net::StaticCookiePolicy::ALLOW_ALL_COOKIES :
      net::StaticCookiePolicy::BLOCK_ALL_THIRD_PARTY_COOKIES;
  net::StaticCookiePolicy policy(policy_type);
  int rv = policy.CanGetCookies(
      request.url(), request.first_party_for_cookies());
  return rv == net::OK;
}

bool QSNetworkDelegate::OnCanSetCookie(const net::URLRequest& request,
                                       const std::string& cookie_line,
                                       net::CookieOptions* options) {
  net::StaticCookiePolicy::Type policy_type = g_accept_all_cookies ?
      net::StaticCookiePolicy::ALLOW_ALL_COOKIES :
      net::StaticCookiePolicy::BLOCK_ALL_THIRD_PARTY_COOKIES;
  net::StaticCookiePolicy policy(policy_type);
  int rv = policy.CanSetCookie(
      request.url(), request.first_party_for_cookies());
  return rv == net::OK;
}

bool QSNetworkDelegate::OnCanAccessFile(const net::URLRequest& request,
                                        const base::FilePath& path) const {
  return true;
}

bool QSNetworkDelegate::OnFirstPartyOnlyCookieExperimentEnabled() const {
  return base::CommandLine::ForCurrentProcess()->HasSwitch(
      switches::kEnableExperimentalWebPlatformFeatures);
}

}  // namespace QuickSilverImpl
