// Copyright (c) 2015 Piotr Tworek. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef QUICKSILVERL_BROWSER_PERMISSION_MANAGER_QS_H_
#define QUICKSILVERL_BROWSER_PERMISSION_MANAGER_QS_H_

#include "base/callback_forward.h"
#include "base/macros.h"
#include "content/public/browser/permission_manager.h"

namespace QuickSilverImpl {

class PermissionManagerQS : public content::PermissionManager {
 public:
  PermissionManagerQS();
  ~PermissionManagerQS() override;

  // PermissionManager implementation.
  void RequestPermission(
      content::PermissionType permission,
      content::WebContents* web_contents,
      int request_id,
      const GURL& requesting_origin,
      bool user_gesture,
      const base::Callback<void(content::PermissionStatus)>& callback) override;
  void CancelPermissionRequest(content::PermissionType permission,
                               content::WebContents* web_contents,
                               int request_id,
                               const GURL& requesting_origin) override;
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

 private:
  DISALLOW_COPY_AND_ASSIGN(PermissionManagerQS);
};

}  // namespace content

#endif // QUICKSILVERL_BROWSER_PERMISSION_MANAGER_QS_H_
