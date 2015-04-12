// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef QUICKSILVER_RENDERER_CONTENT_RENDERER_CLIENT_QS_H_
#define QUICKSILVER_RENDERER_CONTENT_RENDERER_CLIENT_QS_H_

#include "content/public/renderer/content_renderer_client.h"

namespace blink {
class WebView;
}

namespace QuickSilverImpl {

class ContentRendererClientQS : public content::ContentRendererClient {
 public:
  ContentRendererClientQS();
  ~ContentRendererClientQS() override;

  // ContentRendererClient implementation.
  void RenderViewCreated(content::RenderView* render_view) override;
  bool IsPluginAllowedToUseCompositorAPI(const GURL& url) override;
  bool IsPluginAllowedToUseDevChannelAPIs() override;

 private:
  static void ApplyCustomSettings(blink::WebView*);

  DISALLOW_COPY_AND_ASSIGN(ContentRendererClientQS);
};

}  // namespace QuickSilverImpl

#endif  // !QUICKSILVER_RENDERER_CONTENT_RENDERER_CLIENT_QS_H_
