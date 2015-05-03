// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "quicksilver/renderer/content_renderer_client_qs.h"

#include "base/command_line.h"
#include "chrome/common/localized_error.h"
#include "chrome/renderer/net/net_error_helper.h"
#include "components/web_cache/renderer/web_cache_render_process_observer.h"
#include "content/public/common/content_switches.h"
#include "content/public/renderer/render_thread.h"
#include "content/renderer/render_view_impl.h"
#include "ppapi/shared_impl/ppapi_switches.h"
#include "third_party/WebKit/public/web/WebLocalFrame.h"
#include "third_party/WebKit/public/web/WebSettings.h"
#include "third_party/WebKit/public/web/WebView.h"

using namespace content;

namespace QuickSilverImpl {

ContentRendererClientQS::ContentRendererClientQS() {
}

ContentRendererClientQS::~ContentRendererClientQS() {
}

void ContentRendererClientQS::RenderThreadStarted() {
  blink::WebView::setUseExternalPopupMenus(true);
}

void ContentRendererClientQS::RenderFrameCreated(RenderFrame* render_frame) {
  if (render_frame->GetRenderView()->GetMainRenderFrame() == render_frame) {
    // Only attach NetErrorHelper to the main frame, since only the main frame
    // should get error pages.
    new NetErrorHelper(render_frame);
  }
}

void ContentRendererClientQS::RenderViewCreated(RenderView* render_view) {
  RenderViewImpl* rvi = static_cast<RenderViewImpl*>(render_view);
  ApplyCustomSettings(rvi->webview());
}

bool ContentRendererClientQS::IsPluginAllowedToUseCompositorAPI(
    const GURL& url) {
#if defined(ENABLE_PLUGINS)
  return base::CommandLine::ForCurrentProcess()->HasSwitch(
      switches::kEnablePepperTesting);
#else
  return false;
#endif
}

bool ContentRendererClientQS::IsPluginAllowedToUseDevChannelAPIs() {
#if defined(ENABLE_PLUGINS)
  return base::CommandLine::ForCurrentProcess()->HasSwitch(
      switches::kEnablePepperTesting);
#else
  return false;
#endif
}

bool ContentRendererClientQS::ShouldSuppressErrorPage(
    RenderFrame* render_frame, const GURL& url) {
  RenderView* render_view = render_frame->GetRenderView();
  RenderFrame* main_render_frame = render_view->GetMainRenderFrame();
  blink::WebFrame* web_frame = render_frame->GetWebFrame();
  NetErrorHelper* net_error_helper = NetErrorHelper::Get(main_render_frame);
  return net_error_helper->ShouldSuppressErrorPage(web_frame, url);
}

void ContentRendererClientQS::GetNavigationErrorStrings(
    RenderView* render_view,
    blink::WebFrame* frame,
    const blink::WebURLRequest& failed_request,
    const blink::WebURLError& error,
    std::string* error_html,
    base::string16* error_description) {

  bool is_post = EqualsASCII(failed_request.httpMethod(), "POST");

  if (error_html) {
    content::RenderFrame* main_render_frame =
        render_view->GetMainRenderFrame();
    NetErrorHelper* helper = NetErrorHelper::Get(main_render_frame);
    helper->GetErrorHTML(frame, error, is_post, error_html);
  }

  if (error_description) {
    *error_description = LocalizedError::GetErrorDetails(error, is_post);
  }
}

//static
void ContentRendererClientQS::ApplyCustomSettings(blink::WebView* web_view) {
  blink::WebSettings* settings = web_view->settings();

  // Default values for mobile configurations should match android settings
  // from: content/public/common/web_preferences.cc.
  base::CommandLine* command_line = base::CommandLine::ForCurrentProcess();
  if (command_line->HasSwitch(switches::kEnableViewportMeta)) {
    web_view->setIgnoreViewportTagScaleLimits(false);
    settings->setWideViewportQuirkEnabled(false);
    settings->setUseWideViewport(true);
    settings->setViewportMetaLayoutSizeQuirk(false);
    settings->setViewportMetaMergeContentQuirk(false);
    settings->setViewportMetaNonUserScalableQuirk(false);
    settings->setViewportMetaZeroValuesQuirk(false);
    settings->setShrinksViewportContentToFit(true);
    settings->setUseMobileViewportStyle(true);
  }

#if defined(OS_SAILFISH)
  settings->setAllowCustomScrollbarInMainFrame(false);
  settings->setTextAutosizingEnabled(true);
  settings->setAccessibilityFontScaleFactor(1.0f);
  settings->setDeviceScaleAdjustment(1.0f);
  settings->setFullscreenSupported(true);
  settings->setAutoZoomFocusedNodeToLegibleScale(true);
  settings->setDoubleTapToZoomEnabled(true);
  settings->setMediaControlsOverlayPlayButtonEnabled(true);
  settings->setMediaPlaybackRequiresUserGesture(true);
  settings->setSupportDeprecatedTargetDensityDPI(false);
  settings->setUseLegacyBackgroundSizeShorthandBehavior(false);
  settings->setForceZeroLayoutHeight(false);
  settings->setClobberUserAgentInitialScaleQuirk(false);
  settings->setIgnoreMainFrameOverflowHiddenQuirk(false);
  settings->setReportScreenSizeInPhysicalPixelsQuirk(false);
  settings->setShrinksViewportContentToFit(true);
  settings->setUseMobileViewportStyle(true);
#endif // OS_SAILFISH
}

}  // namespace QuickSilverImpl
