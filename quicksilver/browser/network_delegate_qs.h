// Copyright 2015 Piotr Tworek. All rights reserved.
// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef QUICKSILEVR_BROWSER_NETWORK_DELEGATE_QS_H_
#define QUICKSILEVR_BROWSER_NETWORK_DELEGATE_QS_H_

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "net/base/network_delegate_impl.h"

namespace QuickSilverImpl {

class NetworkDelegateQS : public net::NetworkDelegateImpl {
 public:
  NetworkDelegateQS();
  ~NetworkDelegateQS() override;

  static void SetAcceptAllCookies(bool accept);

 private:
  // net::NetworkDelegate implementation.
  int OnBeforeURLRequest(net::URLRequest* request,
                         const net::CompletionCallback& callback,
                         GURL* new_url) override;
  int OnBeforeSendHeaders(net::URLRequest* request,
                          const net::CompletionCallback& callback,
                          net::HttpRequestHeaders* headers) override;
  void OnSendHeaders(net::URLRequest* request,
                     const net::HttpRequestHeaders& headers) override;
  int OnHeadersReceived(
      net::URLRequest* request,
      const net::CompletionCallback& callback,
      const net::HttpResponseHeaders* original_response_headers,
      scoped_refptr<net::HttpResponseHeaders>* override_response_headers,
      GURL* allowed_unsafe_redirect_url) override;
  void OnBeforeRedirect(net::URLRequest* request,
                        const GURL& new_location) override;
  void OnResponseStarted(net::URLRequest* request) override;
  void OnRawBytesRead(const net::URLRequest& request, int bytes_read) override;
  void OnCompleted(net::URLRequest* request, bool started) override;
  void OnURLRequestDestroyed(net::URLRequest* request) override;
  void OnPACScriptError(int line_number, const base::string16& error) override;
  AuthRequiredResponse OnAuthRequired(
      net::URLRequest* request,
      const net::AuthChallengeInfo& auth_info,
      const AuthCallback& callback,
      net::AuthCredentials* credentials) override;
  bool OnCanGetCookies(const net::URLRequest& request,
                       const net::CookieList& cookie_list) override;
  bool OnCanSetCookie(const net::URLRequest& request,
                      const std::string& cookie_line,
                      net::CookieOptions* options) override;
  bool OnCanAccessFile(const net::URLRequest& request,
                       const base::FilePath& path) const override;
  bool OnCanThrottleRequest(const net::URLRequest& request) const override;
  bool OnFirstPartyOnlyCookieExperimentEnabled() const override;

  DISALLOW_COPY_AND_ASSIGN(NetworkDelegateQS);
};

}  // namespace QuickSilverImpl

#endif  // QUICKSILVER_BROWSER_NETWORK_DELEGATE_QS_H_
