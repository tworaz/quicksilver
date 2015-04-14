// Copyright 2015 Piotr Tworek. All rights reserved.
// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "quicksilver/browser/browser_context_qs.h"

#include <QGuiApplication>
#undef signals

#include "base/command_line.h"
#include "base/files/file_util.h"
#include "base/nix/xdg_util.h"
#include "content/public/browser/browser_thread.h"
#include "content/public/browser/storage_partition.h"
#include "content/public/common/content_switches.h"
#include "quicksilver/browser/permission_manager_qs.h"
#include "quicksilver/browser/url_request_context_getter_qs.h"

using namespace content;

namespace QuickSilverImpl {

BrowserContextQS::ResourceContextQS::ResourceContextQS()
    : getter_(NULL) {
}

BrowserContextQS::ResourceContextQS::~ResourceContextQS() {
}

net::HostResolver*
BrowserContextQS::ResourceContextQS::GetHostResolver() {
  CHECK(getter_);
  return getter_->host_resolver();
}

net::URLRequestContext*
BrowserContextQS::ResourceContextQS::GetRequestContext() {
  CHECK(getter_);
  return getter_->GetURLRequestContext();
}

BrowserContextQS::BrowserContextQS(bool off_the_record, net::NetLog* net_log)
    : resource_context_(new ResourceContextQS),
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
}

BrowserContextQS::~BrowserContextQS() {
  if (resource_context_) {
    BrowserThread::DeleteSoon(BrowserThread::IO,
        FROM_HERE, resource_context_.release());
  }
}

scoped_ptr<ZoomLevelDelegate>
BrowserContextQS::CreateZoomLevelDelegate(
    const base::FilePath& partition_path) {
  return scoped_ptr<ZoomLevelDelegate>();
}

base::FilePath BrowserContextQS::GetPath() const {
  return path_;
}

bool BrowserContextQS::IsOffTheRecord() const {
  return off_the_record_;
}

net::URLRequestContextGetter* BrowserContextQS::GetRequestContext() {
  return GetDefaultStoragePartition(this)->GetURLRequestContext();
}

net::URLRequestContextGetter*
BrowserContextQS::GetRequestContextForRenderProcess(
    int renderer_child_id) {
  return GetRequestContext();
}

net::URLRequestContextGetter* BrowserContextQS::GetMediaRequestContext() {
  return GetRequestContext();
}

net::URLRequestContextGetter*
BrowserContextQS::GetMediaRequestContextForRenderProcess(
    int renderer_child_id) {
  return GetRequestContext();
}

net::URLRequestContextGetter*
BrowserContextQS::GetMediaRequestContextForStoragePartition(
    const base::FilePath& partition_path, bool in_memory) {
  return GetRequestContext();
}

ResourceContext* BrowserContextQS::GetResourceContext() {
  return resource_context_.get();
}

DownloadManagerDelegate* BrowserContextQS::GetDownloadManagerDelegate() {
  NOTIMPLEMENTED();
  return NULL;
}

BrowserPluginGuestManager* BrowserContextQS::GetGuestManager() {
  NOTIMPLEMENTED();
  return NULL;
}

storage::SpecialStoragePolicy* BrowserContextQS::GetSpecialStoragePolicy() {
  return NULL;
}

PushMessagingService* BrowserContextQS::GetPushMessagingService() {
  return NULL;
}

SSLHostStateDelegate* BrowserContextQS::GetSSLHostStateDelegate() {
  return NULL;
}

PermissionManager* BrowserContextQS::GetPermissionManager() {
  if (!permission_manager_.get())
    permission_manager_.reset(new PermissionManagerQS());
  return permission_manager_.get();
}

net::URLRequestContextGetter* BrowserContextQS::CreateRequestContext(
    ProtocolHandlerMap* protocol_handlers,
    URLRequestInterceptorScopedVector request_interceptors) {
  DCHECK(!url_request_getter_.get());
  url_request_getter_ = CreateURLRequestContextGetter(
      protocol_handlers, request_interceptors.Pass());
  resource_context_->set_url_request_context_getter(url_request_getter_.get());
  return url_request_getter_.get();
}

net::URLRequestContextGetter*
BrowserContextQS::CreateRequestContextForStoragePartition(
    const base::FilePath& partition_path,
    bool in_memory,
    ProtocolHandlerMap* protocol_handlers,
    URLRequestInterceptorScopedVector request_interceptors) {
  return NULL;
}

URLRequestContextGetterQS* BrowserContextQS::CreateURLRequestContextGetter(
    ProtocolHandlerMap* protocol_handlers,
    URLRequestInterceptorScopedVector request_interceptors) {
  return new URLRequestContextGetterQS(
      ignore_certificate_errors_,
      GetPath(),
      BrowserThread::UnsafeGetMessageLoopForThread(BrowserThread::IO),
      BrowserThread::UnsafeGetMessageLoopForThread(BrowserThread::FILE),
      protocol_handlers,
      request_interceptors.Pass(),
      net_log_);
}

} // namespace QuickSilverImpl
