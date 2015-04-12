// Copyright (c) 2015 Piotr Tworek. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "content/browser/web_contents/web_contents_view_qt.h"

#include <QQuickItem>
#include <QQuickWindow>
#undef signals

#include "content/browser/renderer_host/render_widget_host_impl.h"
#include "content/browser/renderer_host/render_widget_host_view_qt.h"
#include "content/browser/web_contents/web_contents_impl.h"
#include "content/public/browser/interstitial_page.h"
#include "content/public/browser/web_contents_view_delegate.h"
#include "ui/gfx/geometry/dip_util.h"
#include "ui/gfx/screen.h"
#include "ui/qt/type_conversion.h"

namespace content {

WebContentsViewQt::NativeViewCreateDelegate
    WebContentsViewQt::nv_factory_delegate_;

// static
void WebContentsViewQt::SetNativeViewFactoryDelegate(
    NativeViewCreateDelegate d) {
  nv_factory_delegate_ = d;
}

WebContentsView* CreateWebContentsView(
    WebContentsImpl* web_contents,
    WebContentsViewDelegate* delegate,
    RenderViewHostDelegateView** render_view_host_delegate_view) {
  WebContentsViewQt* rv = new WebContentsViewQt(web_contents, delegate);
  *render_view_host_delegate_view = rv;
  return rv;
}

WebContentsViewQt::WebContentsViewQt(
    WebContentsImpl* web_contents,
    WebContentsViewDelegate* delegate)
    : web_contents_(web_contents),
      delegate_(delegate) {
}

WebContentsViewQt::~WebContentsViewQt() {
  native_view_ = 0;
}

gfx::NativeView WebContentsViewQt::GetNativeView() const {
  return native_view_;
}

gfx::NativeView WebContentsViewQt::GetContentNativeView() const {
  RenderWidgetHostViewQt* rwhv = static_cast<RenderWidgetHostViewQt*>(
      web_contents_->GetRenderWidgetHostView());
  return rwhv ? rwhv->GetNativeView() : NULL;
}


gfx::NativeWindow WebContentsViewQt::GetTopLevelNativeWindow() const {
  NOTIMPLEMENTED();
  return NULL;
}

void WebContentsViewQt::GetContainerBounds(gfx::Rect* out) const {
  *out = GetViewBounds();
}

void WebContentsViewQt::SizeContents(const gfx::Size& size) {
  NOTIMPLEMENTED();
}

void WebContentsViewQt::Focus() {
  if (web_contents_->GetInterstitialPage()) {
    web_contents_->GetInterstitialPage()->Focus();
    return;
  }

  if (delegate_.get() && delegate_->Focus())
    return;

  RenderWidgetHostView* rwhv =
      web_contents_->GetFullscreenRenderWidgetHostView();
  if (!rwhv)
    rwhv = web_contents_->GetRenderWidgetHostView();
  if (rwhv)
    rwhv->Focus();
}

void WebContentsViewQt::SetInitialFocus() {
  if (web_contents_->FocusLocationBarByDefault()) {
    web_contents_->SetFocusToLocationBar(false);
  } else {
    Focus();
  }
}

void WebContentsViewQt::StoreFocus() {
  if (delegate_)
    delegate_->StoreFocus();
}

void WebContentsViewQt::RestoreFocus() {
  if (delegate_)
    delegate_->RestoreFocus();
}

DropData* WebContentsViewQt::GetDropData() const {
  NOTIMPLEMENTED();
  return NULL;
}

gfx::Rect WebContentsViewQt::GetViewBounds() const {
  DCHECK(native_view_->window());
  QPointF scenePoint = native_view_->mapToScene(QPointF(0, 0));
  QPointF screenPos;
  if (native_view_->window())
    screenPos = native_view_->window()->mapToGlobal(scenePoint.toPoint());
  gfx::Rect r = gfx::Rect(screenPos.x(), screenPos.y(),
                          native_view_->width(), native_view_->height());
  const gfx::Display disp = gfx::Screen::GetNativeScreen()->GetPrimaryDisplay();
  return gfx::ConvertRectToDIP(disp.device_scale_factor(), r);
}

void WebContentsViewQt::CreateView(
    const gfx::Size& initial_size,
    gfx::NativeView context) {
  CHECK(nv_factory_delegate_) <<
      "Please make sure WebContentsViewItemQt delegate is set";
  native_view_ = nv_factory_delegate_(web_contents_);
}

RenderWidgetHostViewBase* WebContentsViewQt::CreateViewForWidget(
    RenderWidgetHost* render_widget_host,
    bool is_guest_view_hack) {
  RenderWidgetHostViewQt* view = new RenderWidgetHostViewQt(
      render_widget_host, is_guest_view_hack);
  view->InitAsChild(native_view_);
  view->GetNativeView()->setParentItem(native_view_);

  RenderWidgetHostImpl* host_impl =
      RenderWidgetHostImpl::From(render_widget_host);

  if (!host_impl->is_hidden())
    view->Show();

  return view;
}

RenderWidgetHostViewBase* WebContentsViewQt::CreateViewForPopupWidget(
    RenderWidgetHost* render_widget_host) {
  return new RenderWidgetHostViewQt(render_widget_host, false);
}

void WebContentsViewQt::SetPageTitle(const base::string16& title) {
}

void WebContentsViewQt::RenderViewCreated(RenderViewHost* host) {
}

void WebContentsViewQt::RenderViewSwappedIn(RenderViewHost* host) {
}

void WebContentsViewQt::SetOverscrollControllerEnabled(bool enabled) {
}

} // namespace content
