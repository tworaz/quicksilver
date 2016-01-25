// Copyright 2015 Piotr Tworek. All rights reserved.
// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "quicksilver/browser/qs_browser_context.h"

#include <QtGui/QGuiApplication>
#undef signals

#include "base/command_line.h"
#include "base/files/file_util.h"
#include "base/nix/xdg_util.h"
#include "base/prefs/json_pref_store.h"
#include "base/prefs/pref_registry_simple.h"
#include "base/prefs/pref_service.h"
#include "components/content_settings/core/browser/host_content_settings_map.h"
#include "components/pref_registry/pref_registry_syncable.h"
#include "content/public/browser/browser_thread.h"
#include "content/public/browser/storage_partition.h"
#include "content/public/common/content_switches.h"
#include "quicksilver/browser/qs_permission_manager.h"
#include "quicksilver/browser/qs_pref_service_factory.h"
#include "quicksilver/browser/qs_url_request_context_getter.h"

using namespace content;

namespace QuickSilverImpl {

QSBrowserContext::QSResourceContext::QSResourceContext()
    : getter_(nullptr) {
}

QSBrowserContext::QSResourceContext::~QSResourceContext() {
}

net::HostResolver*
QSBrowserContext::QSResourceContext::GetHostResolver() {
  CHECK(getter_);
  return getter_->host_resolver();
}

net::URLRequestContext*
QSBrowserContext::QSResourceContext::GetRequestContext() {
  CHECK(getter_);
  return getter_->GetURLRequestContext();
}

QSBrowserContext::QSBrowserContext(bool off_the_record, net::NetLog* net_log)
    : resource_context_(new QSResourceContext),
      host_content_settings_map_(nullptr),
      pref_registry_(new user_prefs::PrefRegistrySyncable),
      off_the_record_(off_the_record),
      net_log_(net_log),
      ignore_certificate_errors_(false) {
  base::CommandLine* cmd_line = base::CommandLine::ForCurrentProcess();
  if (cmd_line->HasSwitch(switches::kIgnoreCertificateErrors))
    ignore_certificate_errors_ = true;

  scoped_ptr<base::Environment> env(base::Environment::Create());
  base::FilePath config_dir(
      base::nix::GetXDGDirectory(env.get(),
                                 base::nix::kXdgConfigHomeEnvVar,
                                 base::nix::kDotConfigDir));
  path_ = config_dir.Append(qApp->applicationDisplayName().toStdString());

  if (!base::PathExists(path_))
    base::CreateDirectory(path_);

  scoped_refptr<base::SequencedTaskRunner> local_state_task_runner =
      JsonPrefStore::GetTaskRunnerForFile(
          base::FilePath("LocalStorePool"),
          BrowserThread::GetBlockingPool());

  base::FilePath prefs_path = path_.Append(base::FilePath("Local Prefs"));
  HostContentSettingsMap::RegisterProfilePrefs(pref_registry_.get());
  if (off_the_record) {
    prefs_ = CreateOffTheRecordContextPrefs(prefs_path,
                                            local_state_task_runner.get(),
                                            pref_registry_,
                                            true);
  } else {
    prefs_ = CreateContextPrefs(prefs_path,
                                local_state_task_runner.get(),
                                pref_registry_,
                                true);
  }
}

QSBrowserContext::~QSBrowserContext() {
  if (resource_context_) {
    BrowserThread::DeleteSoon(BrowserThread::IO,
        FROM_HERE, resource_context_.release());
  }
  if (host_content_settings_map_.get()) {
    host_content_settings_map_->ShutdownOnUIThread();
  }
}

HostContentSettingsMap& QSBrowserContext::GetHostContentSettingsMap() {
  DCHECK_CURRENTLY_ON(BrowserThread::UI);
  if (!host_content_settings_map_.get()) {
    host_content_settings_map_ =
        new HostContentSettingsMap(prefs_.get(), off_the_record_);
  }
  return *host_content_settings_map_.get();
}

scoped_ptr<ZoomLevelDelegate>
QSBrowserContext::CreateZoomLevelDelegate(
    const base::FilePath& partition_path) {
  return scoped_ptr<ZoomLevelDelegate>();
}

base::FilePath QSBrowserContext::GetPath() const {
  return path_;
}

bool QSBrowserContext::IsOffTheRecord() const {
  return off_the_record_;
}

net::URLRequestContextGetter* QSBrowserContext::GetRequestContext() {
  return GetDefaultStoragePartition(this)->GetURLRequestContext();
}

net::URLRequestContextGetter*
QSBrowserContext::GetRequestContextForRenderProcess(
    int renderer_child_id) {
  return GetRequestContext();
}

net::URLRequestContextGetter* QSBrowserContext::GetMediaRequestContext() {
  return GetRequestContext();
}

net::URLRequestContextGetter*
QSBrowserContext::GetMediaRequestContextForRenderProcess(
    int renderer_child_id) {
  return GetRequestContext();
}

net::URLRequestContextGetter*
QSBrowserContext::GetMediaRequestContextForStoragePartition(
    const base::FilePath& partition_path, bool in_memory) {
  return GetRequestContext();
}

ResourceContext* QSBrowserContext::GetResourceContext() {
  return resource_context_.get();
}

DownloadManagerDelegate* QSBrowserContext::GetDownloadManagerDelegate() {
  NOTIMPLEMENTED();
  return nullptr;
}

BrowserPluginGuestManager* QSBrowserContext::GetGuestManager() {
  NOTIMPLEMENTED();
  return nullptr;
}

storage::SpecialStoragePolicy* QSBrowserContext::GetSpecialStoragePolicy() {
  return nullptr;
}

PushMessagingService* QSBrowserContext::GetPushMessagingService() {
  return nullptr;
}

SSLHostStateDelegate* QSBrowserContext::GetSSLHostStateDelegate() {
  return nullptr;
}

PermissionManager* QSBrowserContext::GetPermissionManager() {
  if (!permission_manager_.get()) {
    permission_manager_.reset(new QSPermissionManager(
        GetHostContentSettingsMap()));
  }
  return permission_manager_.get();
}

BackgroundSyncController* QSBrowserContext::GetBackgroundSyncController() {
  return nullptr;
}

net::URLRequestContextGetter* QSBrowserContext::CreateRequestContext(
    ProtocolHandlerMap* protocol_handlers,
    URLRequestInterceptorScopedVector request_interceptors) {
  DCHECK(!url_request_getter_.get());
  url_request_getter_ = CreateURLRequestContextGetter(
      protocol_handlers, request_interceptors.Pass());
  resource_context_->set_url_request_context_getter(url_request_getter_.get());
  return url_request_getter_.get();
}

net::URLRequestContextGetter*
QSBrowserContext::CreateRequestContextForStoragePartition(
    const base::FilePath& partition_path,
    bool in_memory,
    ProtocolHandlerMap* protocol_handlers,
    URLRequestInterceptorScopedVector request_interceptors) {
  return nullptr;
}

QSURLRequestContextGetter* QSBrowserContext::CreateURLRequestContextGetter(
    ProtocolHandlerMap* protocol_handlers,
    URLRequestInterceptorScopedVector request_interceptors) {
  return new QSURLRequestContextGetter(
      ignore_certificate_errors_,
      GetPath(),
      BrowserThread::UnsafeGetMessageLoopForThread(BrowserThread::IO),
      BrowserThread::UnsafeGetMessageLoopForThread(BrowserThread::FILE),
      protocol_handlers,
      request_interceptors.Pass(),
      net_log_);
}

} // namespace QuickSilverImpl

