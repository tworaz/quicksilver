// Copyright 2015 Piotr Tworek. All rights reserved.
// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "quicksilver/renderer/qs_content_renderer_client.h"

#include "base/command_line.h"
#include "chrome/common/localized_error.h"
#include "chrome/renderer/net/net_error_helper.h"
#include "components/web_cache/renderer/web_cache_render_process_observer.h"
#include "content/public/common/content_switches.h"
#include "content/public/renderer/render_thread.h"
#include "content/renderer/render_view_impl.h"
#include "ppapi/shared_impl/ppapi_switches.h"
#include "quicksilver/common/qs_profile.h"
#include "quicksilver/renderer/qs_render_view_observer.h"
#include "third_party/WebKit/public/web/WebLocalFrame.h"
#include "third_party/WebKit/public/web/WebSettings.h"
#include "third_party/WebKit/public/web/WebView.h"

using namespace content;

namespace QuickSilverImpl {

QSContentRendererClient::QSContentRendererClient() {
}

QSContentRendererClient::~QSContentRendererClient() {
}

void QSContentRendererClient::RenderThreadStarted() {
  blink::WebView::setUseExternalPopupMenus(true);
}

void QSContentRendererClient::RenderFrameCreated(RenderFrame* render_frame) {
  if (render_frame->GetRenderView()->GetMainRenderFrame() == render_frame) {
    // Only attach NetErrorHelper to the main frame, since only the main frame
    // should get error pages.
    new NetErrorHelper(render_frame);
  }
}

void QSContentRendererClient::RenderViewCreated(RenderView* render_view) {
  RenderViewImpl* rvi = static_cast<RenderViewImpl*>(render_view);
  ApplyCustomSettings(rvi->webview());

  new QSRenderViewObserver(render_view);
}

bool QSContentRendererClient::IsPluginAllowedToUseCompositorAPI(
    const GURL& url) {
#if defined(ENABLE_PLUGINS)
  return base::CommandLine::ForCurrentProcess()->HasSwitch(
      switches::kEnablePepperTesting);
#else
  return false;
#endif
}

bool QSContentRendererClient::IsPluginAllowedToUseDevChannelAPIs() {
#if defined(ENABLE_PLUGINS)
  return base::CommandLine::ForCurrentProcess()->HasSwitch(
      switches::kEnablePepperTesting);
#else
  return false;
#endif
}

bool QSContentRendererClient::ShouldSuppressErrorPage(
    RenderFrame* render_frame, const GURL& url) {
  RenderView* render_view = render_frame->GetRenderView();
  RenderFrame* main_render_frame = render_view->GetMainRenderFrame();
  blink::WebFrame* web_frame = render_frame->GetWebFrame();
  NetErrorHelper* net_error_helper = NetErrorHelper::Get(main_render_frame);
  return net_error_helper->ShouldSuppressErrorPage(web_frame, url);
}

void QSContentRendererClient::GetNavigationErrorStrings(
    RenderView* render_view,
    blink::WebFrame* frame,
    const blink::WebURLRequest& failed_request,
    const blink::WebURLError& error,
    std::string* error_html,
    base::string16* error_description) {

  bool is_post = base::EqualsASCII(
      base::StringPiece16(failed_request.httpMethod()), "POST");

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
void QSContentRendererClient::ApplyCustomSettings(blink::WebView* web_view) {
  blink::WebSettings* settings = web_view->settings();

  if (CurrentProfile() == ProfileType::MOBILE ||
      CurrentProfile() == ProfileType::TABLET) {
    // Default values for mobile configurations should match android settings
    // from: content/public/common/web_preferences.cc.
    //
    // NOTE: Only Android prefs which are ifdefed OS_ANDROID should be handled
    // here. Providing custom values for generic prefs available for all
    // platforms in content::WebPreferences should be done via
    // QSContentBrowserClient::OverrideWebkitPrefs.
    web_view->setIgnoreViewportTagScaleLimits(false);
    web_view->setMaximumLegibleScale(4.f);
    settings->setWideViewportQuirkEnabled(false);
    settings->setUseWideViewport(true);
    settings->setViewportMetaLayoutSizeQuirk(false);
    settings->setViewportMetaMergeContentQuirk(false);
    settings->setViewportMetaNonUserScalableQuirk(false);
    settings->setViewportMetaZeroValuesQuirk(false);
    settings->setClobberUserAgentInitialScaleQuirk(false);
    settings->setIgnoreMainFrameOverflowHiddenQuirk(false);
    settings->setShrinksViewportContentToFit(true);
    settings->setUseMobileViewportStyle(true);
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
    settings->setReportScreenSizeInPhysicalPixelsQuirk(false);
  }
}

}  // namespace QuickSilverImpl
