// Copyright 2015 Piotr Tworek. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "quicksilver/browser/qs_web_contents_view_delegate.h"

#include "quicksilver/browser/qs_web_view_impl.h"
#include "quicksilver/public/browser/qs_context_menu_params.h"

using namespace content;
using namespace QuickSilver;

namespace QuickSilverImpl {

QSWebContentsViewDelegate::QSWebContentsViewDelegate(WebContents* wc)
    : web_contents_(wc) {
}

QSWebContentsViewDelegate::~QSWebContentsViewDelegate() {
}

void QSWebContentsViewDelegate::ShowContextMenu(
    RenderFrameHost* render_frame_host,
    const ContextMenuParams& params) {
  const QSContextMenuParams qs_params(params);
  QSWebViewImpl::From(web_contents_)->ShowContextMenu(qs_params);
}

void QSWebContentsViewDelegate::ShowDisambiguationPopup(
    const gfx::Rect& target_rect,
    const SkBitmap& zoomed_bitmap,
    const gfx::NativeView content,
    const base::Callback<void(ui::GestureEvent*)>& gesture_cb,
    const base::Callback<void(ui::MouseEvent*)>& mouse_cb) {
  NOTIMPLEMENTED();
}

} // namespace QuickSilverImpl

