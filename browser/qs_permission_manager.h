// Copyright (c) 2015 Piotr Tworek. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef QUICKSILVERL_BROWSER_QS_PERMISSION_MANAGER_H_
#define QUICKSILVERL_BROWSER_QS_PERMISSION_MANAGER_H_

#include <map>

#include "base/callback_forward.h"
#include "base/macros.h"
#include "content/public/browser/permission_manager.h"

class HostContentSettingsMap;

namespace QuickSilverImpl {

class QSPermissionRequestImpl;

class QSPermissionManager : public content::PermissionManager {
 public:
  QSPermissionManager(HostContentSettingsMap& settings);
  ~QSPermissionManager() override;

  // PermissionManager implementation.
  int RequestPermission(
      content::PermissionType permission,
      content::RenderFrameHost* render_frame_host,
      const GURL& requesting_origin,
      bool user_gesture,
      const base::Callback<void(content::PermissionStatus)>& callback) override;
  void CancelPermissionRequest(int request_id) override;
  void ResetPermission(content::PermissionType permission,
                       const GURL& requesting_origin,
                       const GURL& embedding_origin) override;
  content::PermissionStatus GetPermissionStatus(
      content::PermissionType permission,
      const GURL& requesting_origin,
      const GURL& embedding_origin) override;
  void RegisterPermissionUsage(content::PermissionType permission,
                               const GURL& requesting_origin,
                               const GURL& embedding_origin) override;
  int SubscribePermissionStatusChange(
      content::PermissionType permission,
      const GURL& requesting_origin,
      const GURL& embedding_origin,
      const base::Callback<void(content::PermissionStatus)>& callback) override;
  void UnsubscribePermissionStatusChange(int subscription_id) override;

  void PermissionHandled(QSPermissionRequestImpl*, bool granted, bool remember);

 private:
  typedef std::map<int, QSPermissionRequestImpl*> PermissionRequestMap;

  HostContentSettingsMap& settings_map_;
  PermissionRequestMap request_map_;

  DISALLOW_COPY_AND_ASSIGN(QSPermissionManager);
};

}  // namespace content

#endif // QUICKSILVERL_BROWSER_QS_PERMISSION_MANAGER_H_
