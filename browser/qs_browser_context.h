// Copyright 2015 Piotr Tworek. All rights reserved.
// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef QUICKSILVER_BROWSER_QS_BROWSER_CONTEXT_H_
#define QUICKSILVER_BROWSER_QS_BROWSER_CONTEXT_H_

#include "base/files/file_path.h"
#include "content/public/browser/browser_context.h"
#include "content/public/browser/content_browser_client.h"
#include "content/public/browser/resource_context.h"
#include "net/url_request/url_request_job_factory.h"

class HostContentSettingsMap;
class PrefService;

namespace user_prefs {
class PrefRegistrySyncable;
}

namespace net {
class NetLog;
}

namespace QuickSilverImpl {

class QSURLRequestContextGetter;

class QSBrowserContext : public content::BrowserContext {
 public:
  QSBrowserContext(bool off_the_record, net::NetLog* net_log);
  ~QSBrowserContext() override;

  HostContentSettingsMap& GetHostContentSettingsMap();

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
  content::BackgroundSyncController* GetBackgroundSyncController() override;

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
  class QSResourceContext : public content::ResourceContext {
   public:
    QSResourceContext();
    ~QSResourceContext() override;

    // ResourceContext implementation:
    net::HostResolver* GetHostResolver() override;
    net::URLRequestContext* GetRequestContext() override;

    void set_url_request_context_getter(QSURLRequestContextGetter* getter) {
      getter_ = getter;
    }

  private:
    QSURLRequestContextGetter* getter_;

    DISALLOW_COPY_AND_ASSIGN(QSResourceContext);
  };

 private:
  QSURLRequestContextGetter* CreateURLRequestContextGetter(
      content::ProtocolHandlerMap* protocol_handlers,
      content::URLRequestInterceptorScopedVector request_interceptors);

  scoped_ptr<QSResourceContext> resource_context_;
  scoped_ptr<content::PermissionManager> permission_manager_;
  scoped_refptr<QSURLRequestContextGetter> url_request_getter_;

  // Preferences
  scoped_refptr<HostContentSettingsMap> host_content_settings_map_;
  scoped_refptr<user_prefs::PrefRegistrySyncable> pref_registry_;
  scoped_ptr<PrefService> prefs_;

  base::FilePath path_;
  bool off_the_record_;
  net::NetLog* net_log_;
  bool ignore_certificate_errors_;

  DISALLOW_COPY_AND_ASSIGN(QSBrowserContext);
};

} // namespace QuickSilverImpl

#endif // !QUICKSILVER_BROWSER_QS_BROWSER_CONTEXT_H_

