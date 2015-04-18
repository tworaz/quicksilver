// copyright (c) 2015 piotr tworek. all rights reserved.
// use of this source code is governed by a bsd-style license that can be
// found in the license file.

#include "quicksilver/browser/public/PermissionRequest.h"

namespace QuickSilver {

PermissionRequest::PermissionRequest(bool accepted)
    : QObject(0),
      accepted_(accepted) {
}

PermissionRequest::PermissionRequest(const PermissionRequest& other) {
  accepted_ = other.accepted_;
}

PermissionRequest::~PermissionRequest() {
}

void PermissionRequest::accept() {
  accepted_ = true;
}

void PermissionRequest::deny() {
  accepted_ = false;
}

} // namespace QuickSilver
