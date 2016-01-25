// Copyright 2015 Piotr Tworek. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef QUICKSILVER_BROWSER_QS_WEB_CONTENTS_VIEW_DELEGATE_H_
#define QUICKSILVER_BROWSER_QS_WEB_CONTENTS_VIEW_DELEGATE_H_

#include "content/public/browser/web_contents_view_delegate.h"

namespace content {
class WebContents;
class ContextMenuParams;
class RenderFrameHost;
}

namespace QuickSilverImpl {

class QSWebContentsViewDelegate
  : public content::WebContentsViewDelegate {
 public:
  QSWebContentsViewDelegate(content::WebContents*);
  ~QSWebContentsViewDelegate() override;

  // QSWebContentsViewDelegate:
  void ShowContextMenu(content::RenderFrameHost*,
                       const content::ContextMenuParams& params) override;
  void ShowDisambiguationPopup(
      const gfx::Rect& target_rect,
      const SkBitmap& zoomed_bitmap,
      const gfx::NativeView content,
      const base::Callback<void(ui::GestureEvent*)>& gesture_cb,
      const base::Callback<void(ui::MouseEvent*)>& mouse_cb) override;

 private:
  content::WebContents* web_contents_; // Not owned

  DISALLOW_COPY_AND_ASSIGN(QSWebContentsViewDelegate);
};

} // namespace QuickSilverImpl

#endif // !QUICKSILVER_BROWSER_QS_WEB_CONTENTS_VIEW_DELEGATE_H_

