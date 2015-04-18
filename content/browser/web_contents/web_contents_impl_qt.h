// Copyright (c) 2015 Piotr Tworek. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_BROWSER_WEB_CONTENTS_WEB_CONTENTS_IMPL_QT_H_
#define CONTENT_BROWSER_WEB_CONTENTS_WEB_CONTENTS_IMPL_QT_H_

#include "content/browser/web_contents/web_contents_impl.h"

namespace content {

class WebContentsImplQt : public WebContentsImpl {
 public:
  void CreateNewWindow(
      int render_process_id,
      int route_id,
      int main_frame_route_id,
      const ViewHostMsg_CreateWindow_Params& params,
      SessionStorageNamespace* session_storage_namespace) override;

  void* GetPlatformData() const { return platform_data_; };
  void SetPlatformData(void* data) { platform_data_ = data; };

 private:
  friend class WebContentsImpl;
  friend class WebContents;

  // See WebContents::Create for a description of these parameters.
  WebContentsImplQt(BrowserContext* browser_context,
                    WebContentsImpl* opener,
                    void* platform_data);

  WebContents* HandleNewWebContentsCreate(
      int render_process_id,
      int route_id,
      int main_frame_route_id,
      const ViewHostMsg_CreateWindow_Params& params,
      SessionStorageNamespace* session_storage_namespace,
      void* platform_data);

  void* platform_data_;

  DISALLOW_COPY_AND_ASSIGN(WebContentsImplQt);
};

} // namespace content

#endif
