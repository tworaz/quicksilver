// Copyright 2015 Piotr Tworek. All rights reserved.
// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef QUICKSILVER_BROWSER_BROWSER_CONTEXT_QS_H_
#define QUICKSILVER_BROWSER_BROWSER_CONTEXT_QS_H_

#include "base/files/file_path.h"
#include "content/public/browser/browser_context.h"
#include "content/public/browser/content_browser_client.h"
#include "content/public/browser/resource_context.h"
#include "net/url_request/url_request_job_factory.h"

namespace net {
class NetLog;
}

namespace QuickSilverImpl {

class URLRequestContextGetterQS;

class BrowserContextQS : public content::BrowserContext {
 public:
  BrowserContextQS(bool off_the_record, net::NetLog* net_log);
  ~BrowserContextQS() override;

  // BrowserContext implementation
  scoped_ptr<content::ZoomLevelDelegate> CreateZoomLevelDelegate(
      const base::FilePath& partition_path) override;
  base::FilePath GetPath() const override;
  bool IsOffTheRecord() const override;
  net::URLRequestContextGetter* GetRequestContext() override;
  net::URLRequestContextGetter* GetRequestContextForRenderProcess(
      int renderer_child_id) override;
  net::URLRequestContextGetter* GetMediaRequestContext() override;
  net::URLRequestContextGetter* GetMediaRequestContextForRenderProcess(
      int renderer_child_id) override;
  net::URLRequestContextGetter* GetMediaRequestContextForStoragePartition(
      const base::FilePath& partition_path, bool in_memory) override;
  content::ResourceContext* GetResourceContext() override;
  content::DownloadManagerDelegate* GetDownloadManagerDelegate() override;
  content::BrowserPluginGuestManager* GetGuestManager() override;
  storage::SpecialStoragePolicy* GetSpecialStoragePolicy() override;
  content::PushMessagingService* GetPushMessagingService() override;
  content::SSLHostStateDelegate* GetSSLHostStateDelegate() override;
  content::PermissionManager* GetPermissionManager() override;

  net::URLRequestContextGetter* CreateRequestContext(
      content::ProtocolHandlerMap* protocol_handlers,
      content::URLRequestInterceptorScopedVector request_interceptors);
  net::URLRequestContextGetter* CreateRequestContextForStoragePartition(
      const base::FilePath& partition_path,
      bool in_memory,
      content::ProtocolHandlerMap* protocol_handlers,
      content::URLRequestInterceptorScopedVector request_interceptors);

 protected:
  // Contains URLRequestContextGetter required for resource loading.
  class ResourceContextQS : public content::ResourceContext {
   public:
    ResourceContextQS();
    ~ResourceContextQS() override;

    // ResourceContext implementation:
    net::HostResolver* GetHostResolver() override;
    net::URLRequestContext* GetRequestContext() override;

    void set_url_request_context_getter(URLRequestContextGetterQS* getter) {
      getter_ = getter;
    }

  private:
    URLRequestContextGetterQS* getter_;

    DISALLOW_COPY_AND_ASSIGN(ResourceContextQS);
  };

 private:
  URLRequestContextGetterQS* CreateURLRequestContextGetter(
      content::ProtocolHandlerMap* protocol_handlers,
      content::URLRequestInterceptorScopedVector request_interceptors);

  scoped_ptr<ResourceContextQS> resource_context_;
  scoped_ptr<content::PermissionManager> permission_manager_;
  scoped_refptr<URLRequestContextGetterQS> url_request_getter_;

  base::FilePath path_;
  bool off_the_record_;
  net::NetLog* net_log_;
  bool ignore_certificate_errors_;

  DISALLOW_COPY_AND_ASSIGN(BrowserContextQS);
};

} // namespace QuickSilverImpl

#endif // !QUICKSILVER_BROWSER_BROWSER_CONTEXT_QS_H_
