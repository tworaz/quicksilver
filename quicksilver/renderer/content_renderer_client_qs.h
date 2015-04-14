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
  void RenderThreadStarted() override;
  void RenderFrameCreated(content::RenderFrame* render_frame) override;
  void RenderViewCreated(content::RenderView* render_view) override;
  bool IsPluginAllowedToUseCompositorAPI(const GURL& url) override;
  bool IsPluginAllowedToUseDevChannelAPIs() override;
  bool ShouldSuppressErrorPage(content::RenderFrame* render_frame,
                               const GURL& url) override;
  void GetNavigationErrorStrings(content::RenderView* render_view,
                                 blink::WebFrame* frame,
                                 const blink::WebURLRequest& failed_request,
                                 const blink::WebURLError& error,
                                 std::string* error_html,
                                 base::string16* error_description) override;

 private:
  static void ApplyCustomSettings(blink::WebView*);

  DISALLOW_COPY_AND_ASSIGN(ContentRendererClientQS);
};

}  // namespace QuickSilverImpl

#endif  // !QUICKSILVER_RENDERER_CONTENT_RENDERER_CLIENT_QS_H_
