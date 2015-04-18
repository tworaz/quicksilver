// copyright (c) 2015 piotr tworek. all rights reserved.
// use of this source code is governed by a bsd-style license that can be
// found in the license file.

#include "quicksilver/browser/public/NewWindowPermissionRequest.h"

namespace QuickSilver {

NewWindowPermissionRequest::NewWindowPermissionRequest(QString url)
    : PermissionRequest(false),
      url_(url) {
}

NewWindowPermissionRequest::NewWindowPermissionRequest(
    const NewWindowPermissionRequest& other)
    : PermissionRequest(other) {
  url_ = other.url_;
}

NewWindowPermissionRequest::~NewWindowPermissionRequest() {
}

QString NewWindowPermissionRequest::url() const {
  return url_;
}

}
