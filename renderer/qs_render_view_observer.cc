// Copyright (c) 2015 Piotr Tworek. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "quicksilver/renderer/qs_render_view_observer.h"

#include "content/common/view_messages.h"
#include "content/renderer/render_view_impl.h"
#include "ipc/ipc_message.h"
#include "quicksilver/common/qs_messages.h"
#include "third_party/WebKit/public/web/WebView.h"

using namespace content;

namespace QuickSilverImpl {

namespace {
static int kSizeUpdateDelay = 100;
}

QSRenderViewObserver::QSRenderViewObserver(RenderView* render_view)
    : RenderViewObserver(render_view)
    , contents_size_(0, 0)
    , total_scale_factor_(0.f) {
}

QSRenderViewObserver::~QSRenderViewObserver() {
}

#if 0
bool QSRenderViewObserver::OnMessageReceived(const IPC::Message& message) {
  if (message.type() == ViewMsg_Resize::ID) {
    RenderViewImpl* rvi = static_cast<RenderViewImpl*>(render_view());
    if (rvi->HasIMETextFocus()) {
      base::ThreadTaskRunnerHandle::Get()->PostTask(FROM_HERE,
          base::Bind(&QSRenderViewObserver::ScrollFocusedEditableNodeIntoView,
              base::Unretained(this)));
    }
  }
  return false;
}
#endif

void QSRenderViewObserver::DidUpdateLayout() {
  if (size_update_timer_.IsRunning()) {
    return;
  }
  size_update_timer_.Start(FROM_HERE,
                           base::TimeDelta::FromMilliseconds(kSizeUpdateDelay),
                           this, &QSRenderViewObserver::UpdateContentSize);
}

void QSRenderViewObserver::DidCommitCompositorFrame() {
  if (size_update_timer_.IsRunning()) {
    return;
  }
  size_update_timer_.Start(FROM_HERE,
                           base::TimeDelta::FromMilliseconds(kSizeUpdateDelay),
                           this, &QSRenderViewObserver::UpdateContentSize);
}

#if 0
void QSRenderViewObserver::ScrollFocusedEditableNodeIntoView() {
  RenderViewImpl* rvi = static_cast<RenderViewImpl*>(render_view());
  if (rvi->HasIMETextFocus()) {
    // The rect parameter is not actually used by scrollFocusedNodeIntoRect
    // implementation.
    blink::WebRect unused;
    rvi->GetWebView()->scrollFocusedNodeIntoRect(unused);
  }
}
#endif

void QSRenderViewObserver::UpdateContentSize() {
  blink::WebView* web_view = render_view()->GetWebView();
  blink::WebFrame* main_frame = web_view->mainFrame();

  blink::WebSize cs = main_frame->contentsSize();
  gfx::SizeF new_size(cs.width, cs.height);
  new_size.Scale(web_view->pageScaleFactor());
  new_size.Scale(web_view->deviceScaleFactor());

  float tsf = web_view->pageScaleFactor() * web_view->deviceScaleFactor();

  if (new_size != contents_size_ || tsf != total_scale_factor_) {
    contents_size_ = new_size;
    total_scale_factor_ = tsf;
    Send(new QuickSilverMsg_ViewScrollParamsChanged(routing_id(),
                                                    new_size, tsf));
  }
}

} // namespace QuickSilverImpl
