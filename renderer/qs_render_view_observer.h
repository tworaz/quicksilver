// Copyright (c) 2015 Piotr Tworek. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef QUICKSILVER_RENDERER_QS_RENDER_VIEW_OBSERVER_H_
#define QUICKSILVER_RENDERER_QS_RENDER_VIEW_OBSERVER_H_

#include "base/timer/timer.h"
#include "content/public/renderer/render_view_observer.h"
#include "ui/gfx/geometry/size_f.h"

namespace QuickSilverImpl {

class QSRenderViewObserver : public content::RenderViewObserver {
 public:
  explicit QSRenderViewObserver(content::RenderView* render_view);
  ~QSRenderViewObserver() override;

 private:
  // RenderViewObserver:
#if 0
  bool OnMessageReceived(const IPC::Message& message) override;
#endif
  void DidUpdateLayout() override;
  void DidCommitCompositorFrame() override;

#if 0
  void ScrollFocusedEditableNodeIntoView();
#endif
  void UpdateContentSize();

  gfx::SizeF contents_size_;
  float total_scale_factor_;
  base::OneShotTimer size_update_timer_;

  DISALLOW_COPY_AND_ASSIGN(QSRenderViewObserver);
};

}  // namespace QuickSilverImpl

#endif // QUICKSILVER_RENDERER_QS_RENDER_VIEW_OBSERVER_H_
