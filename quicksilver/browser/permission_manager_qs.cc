// Copyright (c) 2015 Piotr Tworek. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "quicksilver/browser/permission_manager_qs.h"

#include "base/callback.h"
#include "base/logging.h"
#include "content/public/browser/permission_type.h"

using namespace content;

namespace QuickSilverImpl {

PermissionManagerQS::PermissionManagerQS()
    : PermissionManager() {
}

PermissionManagerQS::~PermissionManagerQS() {
}

void PermissionManagerQS::RequestPermission(
    PermissionType permission,
    WebContents* web_contents,
    int request_id,
    const GURL& requesting_origin,
    bool user_gesture,
    const base::Callback<void(PermissionStatus)>& callback) {
  NOTIMPLEMENTED();
  callback.Run(PERMISSION_STATUS_DENIED);
}

void PermissionManagerQS::CancelPermissionRequest(
    PermissionType permission,
    WebContents* web_contents,
    int request_id,
    const GURL& requesting_origin) {
  NOTIMPLEMENTED();
}

void PermissionManagerQS::ResetPermission(
    PermissionType permission,
    const GURL& requesting_origin,
    const GURL& embedding_origin) {
  NOTIMPLEMENTED();
}

PermissionStatus PermissionManagerQS::GetPermissionStatus(
    PermissionType permission,
    const GURL& requesting_origin,
    const GURL& embedding_origin) {
  NOTIMPLEMENTED();
  return PERMISSION_STATUS_DENIED;
}

void PermissionManagerQS::RegisterPermissionUsage(
    PermissionType permission,
    const GURL& requesting_origin,
    const GURL& embedding_origin) {
  NOTIMPLEMENTED();
}

int PermissionManagerQS::SubscribePermissionStatusChange(
    PermissionType permission,
    const GURL& requesting_origin,
    const GURL& embedding_origin,
    const base::Callback<void(PermissionStatus)>& callback) {
  NOTIMPLEMENTED();
  return -1;
}

void PermissionManagerQS::UnsubscribePermissionStatusChange(
    int subscription_id) {
  NOTIMPLEMENTED();
}

}  // namespace QuickSilverImpl
