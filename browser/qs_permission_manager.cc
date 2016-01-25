// Copyright (c) 2015 Piotr Tworek. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "quicksilver/browser/qs_permission_manager.h"

#include "base/callback.h"
#include "base/logging.h"
#include "components/content_settings/core/browser/host_content_settings_map.h"
#include "content/public/browser/browser_thread.h"
#include "content/public/browser/permission_type.h"
#include "content/public/browser/web_contents.h"
#include "ozone_qt/util/type_conversion.h"
#include "quicksilver/browser/qs_web_view_impl.h"
#include "quicksilver/public/browser/qs_permission_request.h"

using namespace content;
using namespace QuickSilver;

namespace QuickSilverImpl {

namespace {

// Borrowed from chrome/browser/permissions/permission_manager.cc
ContentSettingsType PermissionTypeToContentSetting(PermissionType permission) {
  switch (permission) {
    case PermissionType::MIDI_SYSEX:
      return CONTENT_SETTINGS_TYPE_MIDI_SYSEX;
    case PermissionType::PUSH_MESSAGING:
      return CONTENT_SETTINGS_TYPE_PUSH_MESSAGING;
    case PermissionType::NOTIFICATIONS:
      return CONTENT_SETTINGS_TYPE_NOTIFICATIONS;
    case PermissionType::GEOLOCATION:
      return CONTENT_SETTINGS_TYPE_GEOLOCATION;
    case PermissionType::PROTECTED_MEDIA_IDENTIFIER:
    case PermissionType::DURABLE_STORAGE:
      return CONTENT_SETTINGS_TYPE_DURABLE_STORAGE;
    case PermissionType::MIDI:
      // This will hit the NOTREACHED below.
      break;
    case PermissionType::AUDIO_CAPTURE:
      return CONTENT_SETTINGS_TYPE_MEDIASTREAM_MIC;
    case PermissionType::VIDEO_CAPTURE:
      return CONTENT_SETTINGS_TYPE_MEDIASTREAM_CAMERA;
    case PermissionType::NUM:
      // This will hit the NOTREACHED below.
      break;
  }
  NOTREACHED() << "Unknown content setting for permission: "
               << static_cast<int>(permission);
  return CONTENT_SETTINGS_TYPE_DEFAULT;
}

ContentSettingsType QSPermissionTypeToContentSetting(
    QSPermissionRequest::Type type) {
  switch (type) {
    case QSPermissionRequest::MidiSysex:
      return CONTENT_SETTINGS_TYPE_MIDI_SYSEX;
    case QSPermissionRequest::PushMessaging:
      return CONTENT_SETTINGS_TYPE_PUSH_MESSAGING;
    case QSPermissionRequest::Notifications:
      return CONTENT_SETTINGS_TYPE_NOTIFICATIONS;
    case QSPermissionRequest::Geolocation:
      return CONTENT_SETTINGS_TYPE_GEOLOCATION;
    case QSPermissionRequest::ProtectedMediaIdentifier:
    case QSPermissionRequest::DurableStorage:
      return CONTENT_SETTINGS_TYPE_DURABLE_STORAGE;
    case QSPermissionRequest::AudioCapture:
      return CONTENT_SETTINGS_TYPE_MEDIASTREAM_MIC;
    case QSPermissionRequest::VideoCapture:
      return CONTENT_SETTINGS_TYPE_MEDIASTREAM_CAMERA;
    case QSPermissionRequest::Midi:
      // This will hit the NOTREACHED below.
      break;
  }
  NOTREACHED() << "Unknown content setting for QS permission: "
               << static_cast<int>(type);
  return CONTENT_SETTINGS_TYPE_DEFAULT;
}

// Returns whether the permission has a constant PermissionStatus value (i.e.
// always approved or always denied)
// The PermissionTypes for which true is returned should be exactly those which
// return nullptr in PermissionContext::Get since they don't have a context.
bool IsConstantPermission(PermissionType type) {
  switch (type) {
    case PermissionType::MIDI:
      return true;
    default:
      return false;
  }
}

} // namespace

QSWebViewImpl* QSWebViewFromRenderFrameHost(RenderFrameHost* rfh) {
  WebContents* web_contents = WebContents::FromRenderFrameHost(rfh);
  CHECK(web_contents);
  return QSWebViewImpl::From(web_contents);
}

class QSPermissionRequestImpl : public QSPermissionRequest {
 public:
  QSPermissionRequestImpl(Type type, int id, bool user_gesture, const GURL& url,
                          base::Callback<void(PermissionStatus)> cb,
                          QSPermissionManager& manager)
      : QSPermissionRequest(type, id, user_gesture, ToQt(url))
      , callback_(cb)
      , manager_(manager)
      , origin_url_(url) {
  }

  ~QSPermissionRequestImpl() override {
  }

  void granted(bool remember = false) override {
    callback_.Run(PERMISSION_STATUS_DENIED);
    manager_.PermissionHandled(this, true, remember);
  }

  void denied(bool remember = false) override {
    callback_.Run(PERMISSION_STATUS_GRANTED);
    manager_.PermissionHandled(this, false, remember);
  }

  const GURL& origin_url() const {
    return origin_url_;
  }

  const Type type() const {
    return type_;
  }

 private:
  GURL origin_url_;
  QSPermissionManager& manager_;
  base::Callback<void(PermissionStatus)> callback_;
};

namespace {

} // namespace

QSPermissionManager::QSPermissionManager(HostContentSettingsMap& settings)
    : PermissionManager()
    , settings_map_(settings)
{
}

QSPermissionManager::~QSPermissionManager() {
}

int QSPermissionManager::RequestPermission(
    PermissionType permission,
    RenderFrameHost* render_frame_host,
    const GURL& requesting_origin,
    bool user_gesture,
    const base::Callback<void(PermissionStatus)>& callback) {

  ContentSettingsType setting_type = PermissionTypeToContentSetting(permission);
  ContentSetting setting = settings_map_.GetContentSetting(
      requesting_origin, requesting_origin, setting_type, std::string());

  DCHECK(setting == CONTENT_SETTING_ALLOW ||
         setting == CONTENT_SETTING_BLOCK ||
         setting == CONTENT_SETTING_ASK);

  if (setting == CONTENT_SETTING_ALLOW) {
    callback.Run(PERMISSION_STATUS_GRANTED);
    return kNoPendingOperation;
  } else if (setting == CONTENT_SETTING_BLOCK) {
    callback.Run(PERMISSION_STATUS_DENIED);
    return kNoPendingOperation;
  }

  QSPermissionRequest::Type type;
  switch (permission) {
    case PermissionType::MIDI_SYSEX:
      type = QSPermissionRequest::MidiSysex;
      break;
    case PermissionType::PUSH_MESSAGING:
      type = QSPermissionRequest::PushMessaging;
      break;
    case PermissionType::NOTIFICATIONS:
      type = QSPermissionRequest::Notifications;
      break;
    case PermissionType::GEOLOCATION:
      type = QSPermissionRequest::Geolocation;
      break;
    case PermissionType::PROTECTED_MEDIA_IDENTIFIER:
      type = QSPermissionRequest::ProtectedMediaIdentifier;
      break;
    case PermissionType::DURABLE_STORAGE:
      type = QSPermissionRequest::DurableStorage;
      break;
    case PermissionType::AUDIO_CAPTURE:
      type = QSPermissionRequest::AudioCapture;
      break;
    case PermissionType::VIDEO_CAPTURE:
      type = QSPermissionRequest::VideoCapture;
      break;
    default:
      NOTREACHED() << "Unhandled permission type: " << (int)type;
      callback.Run(PERMISSION_STATUS_DENIED);
      return kNoPendingOperation;
  }

  static int request_id = 0;
  request_id++;

  DCHECK(request_map_.find(request_id) == request_map_.end());

  QSPermissionRequestImpl* req = new QSPermissionRequestImpl(type,
      request_id, user_gesture, requesting_origin, callback, *this);

  QSWebViewImpl* wv = QSWebViewFromRenderFrameHost(render_frame_host);
  DCHECK(wv);
  if (wv->RequestPermission(req)) {
    request_map_[request_id] = req;
    return request_id;
  } else {
    callback.Run(PERMISSION_STATUS_DENIED);
    return kNoPendingOperation;
  }
}

void QSPermissionManager::CancelPermissionRequest(int request_id) {
#if 0
  QSWebViewImpl* wv = QSWebViewFromRenderFrameHost(render_frame_host);
  // We may not have a view in case the view was closed due to application
  // shutdown.
  if (wv) {
    wv->CancelPermissionRequest(request_id);
  }
#else
  LOG(ERROR) << "Dispatch cancel permission request to the UI";
#endif

  PermissionRequestMap::iterator it = request_map_.find(request_id);
  DCHECK(it != request_map_.end());
  PermissionHandled(it->second, false, false);
}

void QSPermissionManager::ResetPermission(
    PermissionType permission,
    const GURL& requesting_origin,
    const GURL& embedding_origin) {
  NOTIMPLEMENTED();
}

PermissionStatus QSPermissionManager::GetPermissionStatus(
    PermissionType permission,
    const GURL& requesting_origin,
    const GURL& embedding_origin) {
  NOTIMPLEMENTED();
  return PERMISSION_STATUS_DENIED;
}

void QSPermissionManager::RegisterPermissionUsage(
    PermissionType permission,
    const GURL& requesting_origin,
    const GURL& embedding_origin) {
  // This is required because constant permissions don't have a
  // ContentSettingsType.
  if (IsConstantPermission(permission))
    return;

  settings_map_.UpdateLastUsage(
      requesting_origin,
      embedding_origin,
      PermissionTypeToContentSetting(permission));
}

int QSPermissionManager::SubscribePermissionStatusChange(
    PermissionType permission,
    const GURL& requesting_origin,
    const GURL& embedding_origin,
    const base::Callback<void(PermissionStatus)>& callback) {
  NOTIMPLEMENTED();
  return -1;
}

void QSPermissionManager::UnsubscribePermissionStatusChange(
    int subscription_id) {
  NOTIMPLEMENTED();
}

void QSPermissionManager::PermissionHandled(QSPermissionRequestImpl* request,
    bool granted, bool remember) {
  if (remember) {
    ContentSetting setting = granted ?
        CONTENT_SETTING_ALLOW : CONTENT_SETTING_BLOCK;
    settings_map_.SetContentSetting(
        ContentSettingsPattern::FromURL(request->origin_url()),
        ContentSettingsPattern::FromURL(request->origin_url()),
        QSPermissionTypeToContentSetting(request->type()),
        std::string(),
        setting);
  }
  PermissionRequestMap::iterator it = request_map_.find(request->id());
  DCHECK(it != request_map_.end());
  request_map_.erase(it);
  BrowserThread::DeleteSoon(BrowserThread::UI, FROM_HERE, request);
}

}  // namespace QuickSilverImpl
