// Copyright 2015 Piotr Tworek. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "content/shell/browser/shell_web_contents_view_delegate.h"

#include "content/public/browser/web_contents.h"
#include "content/shell/browser/shell_web_contents_view_delegate_creator.h"

namespace content {

WebContentsViewDelegate* CreateShellWebContentsViewDelegate(
    WebContents* web_contents) {
  return NULL;
}

}  // namespace content
