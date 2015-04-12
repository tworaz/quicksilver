// Copyright (c) 2015 Piotr Tworek. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_BROWSER_WEB_CONTENTS_WEB_CONTENTS_VIEW_QT_H_
#define CONTENT_BROWSER_WEB_CONTENTS_WEB_CONTENTS_VIEW_QT_H_

#include "base/memory/scoped_ptr.h"
#include "content/browser/renderer_host/render_view_host_delegate_view.h"
#include "content/browser/web_contents/web_contents_view.h"

class QQuickItem;

namespace content {
class WebContents;
class WebContentsImpl;
class WebContentsViewDelegate;

class WebContentsViewQt
    : public WebContentsView,
      public RenderViewHostDelegateView {
 public:
  typedef QQuickItem* (*NativeViewCreateDelegate)(const WebContents*);

  static void SetNativeViewFactoryDelegate(NativeViewCreateDelegate);

  WebContentsViewQt(WebContentsImpl* web_contsnts,
                    WebContentsViewDelegate* delegate);

 private:
  ~WebContentsViewQt() override;

  gfx::NativeView GetNativeView() const override;
  gfx::NativeView GetContentNativeView() const override;
  gfx::NativeWindow GetTopLevelNativeWindow() const override;
  void GetContainerBounds(gfx::Rect* out) const override;
  void SizeContents(const gfx::Size& size) override;
  void Focus() override;
  void SetInitialFocus() override;
  void StoreFocus() override;
  void RestoreFocus() override;
  DropData* GetDropData() const override;
  gfx::Rect GetViewBounds() const override;
  void CreateView(const gfx::Size& initial_size,
                  gfx::NativeView context) override;
  RenderWidgetHostViewBase* CreateViewForWidget(
      RenderWidgetHost* render_widget_host,
      bool is_guest_view_hack) override;
  RenderWidgetHostViewBase* CreateViewForPopupWidget(
      RenderWidgetHost* render_widget_host) override;
  void SetPageTitle(const base::string16& title) override;
  void RenderViewCreated(RenderViewHost* host) override;
  void RenderViewSwappedIn(RenderViewHost* host) override;
  void SetOverscrollControllerEnabled(bool enabled) override;

  // The WebContentsImpl whose contents we display.
  WebContentsImpl* web_contents_;
  scoped_ptr<WebContentsViewDelegate> delegate_;
  QQuickItem* native_view_;

  static NativeViewCreateDelegate nv_factory_delegate_;

  DISALLOW_COPY_AND_ASSIGN(WebContentsViewQt);
};

} // namespace content

#endif // CONTENT_BROWSER_WEB_CONTENTS_WEB_CONTENTS_VIEW_QT_H_

