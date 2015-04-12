// Copyright (c) 2015 Piotr Tworek. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_BROWSER_RENDERER_HOST_RENDER_WIDGET_HOST_VIEW_QT_H_
#define CONTENT_BROWSER_RENDERER_HOST_RENDER_WIDGET_HOST_VIEW_QT_H_

#include <QVariant>

#include "content/browser/renderer_host/render_widget_host_view_base.h"
#include "content/browser/renderer_host/delegated_frame_node.h"
#include "content/common/content_export.h"
#include "cc/resources/returned_resource.h"

class QQuickItem;
class QInputMethod;
class QFocusEvent;
class QHoverEvent;
class QMouseEvent;
class QWheelEvent;
class QInputMethodEvent;
class QKeyEvent;

namespace content {

class RenderWidgetHostImpl;

class CONTENT_EXPORT RenderWidgetHostViewQt
    : public RenderWidgetHostViewBase
    , public base::SupportsWeakPtr<RenderWidgetHostViewQt> {
 public:
  // When |is_guest_view_hack| is true, this view isn't really the view for
  // the |widget|, a RenderWidgetHostViewGuest is.
  //
  // TODO(lazyboy): Remove |is_guest_view_hack| once BrowserPlugin has migrated
  // to use RWHVChildFrame (http://crbug.com/330264).
  RenderWidgetHostViewQt(RenderWidgetHost* host, bool is_guest_view_hack);

  // RenderWidgetHostView implementation.
  void InitAsChild(gfx::NativeView parent_view) override;
  RenderWidgetHost* GetRenderWidgetHost() const override;
  void SetSize(const gfx::Size& size) override;
  void SetBounds(const gfx::Rect& rect) override;
  gfx::Vector2dF GetLastScrollOffset() const override;
  gfx::NativeView GetNativeView() const override;
  gfx::NativeViewId GetNativeViewId() const override;
  gfx::NativeViewAccessible GetNativeViewAccessible() override;
  bool HasFocus() const override;
  bool IsSurfaceAvailableForCopy() const override;
  void Show() override;
  void Hide() override;
  bool IsShowing() override;
  gfx::Rect GetViewBounds() const override;
  void SetBackgroundColor(SkColor color) override;
  void SetInsets(const gfx::Insets& insets) override;

  bool LockMouse() override;
  void UnlockMouse() override;

  // Overridden from RenderWidgetHostViewBase
  void InitAsPopup(RenderWidgetHostView* parent_host_view,
                   const gfx::Rect& pos) override;
  void InitAsFullscreen(RenderWidgetHostView* reference_host_view) override;
  void MovePluginWindows(const std::vector<WebPluginGeometry>& moves) override;
  void Focus() override;
  void Blur() override;
  void UpdateCursor(const WebCursor& cursor) override;
  void SetIsLoading(bool is_loading) override;
  void TextInputTypeChanged(ui::TextInputType type,
                            ui::TextInputMode input_mode,
                            bool can_compose_inline,
                            int flags) override;
  void ImeCancelComposition() override;
  void ImeCompositionRangeChanged(
      const gfx::Range& range,
      const std::vector<gfx::Rect>& character_bounds) override;
  void RenderProcessGone(base::TerminationStatus status,
                         int error_code) override;
  void Destroy() override;
  void SetTooltipText(const base::string16& tooltip_text) override;
  void SelectionChanged(const base::string16& text,
                        size_t offset,
                        const gfx::Range& range) override;
  void SelectionBoundsChanged(
      const ViewHostMsg_SelectionBounds_Params& params) override;
  void CopyFromCompositingSurface(const gfx::Rect& src_subrect,
                                  const gfx::Size& dst_size,
                                  ReadbackRequestCallback& callback,
                                  const SkColorType color_type) override;
  void CopyFromCompositingSurfaceToVideoFrame(
      const gfx::Rect& src_subrect,
      const scoped_refptr<media::VideoFrame>& target,
      const base::Callback<void(bool)>& callback) override;
  bool CanCopyToVideoFrame() const override;
  bool HasAcceleratedSurface(const gfx::Size& desired_size) override;
  void GetScreenInfo(blink::WebScreenInfo* results) override;
  gfx::Rect GetBoundsInRootWindow() override;
  gfx::GLSurfaceHandle GetCompositingSurface() override;
  void OnSwapCompositorFrame(uint32 output_surface_id,
      scoped_ptr<cc::CompositorFrame> frame) override;

 protected:
  friend class RenderWidgetHostViewQtDelegate;

  QSGNode* UpdatePaintNode(QSGNode*);
  void SendDelegatedFrameAck();

  QVariant InputMethodQuery(Qt::InputMethodQuery);

  void HandleResize();
  void HandleFocusEvent(QFocusEvent*);
  void HandleKeyEvent(QKeyEvent*);
  void HandleMouseEvent(QMouseEvent*);
  void HandleWheelEvent(QWheelEvent*);
  void HandleHoverEvent(QHoverEvent*);
  void HandleInputMethodEvent(QInputMethodEvent*);

 private:
  ~RenderWidgetHostViewQt() override;

  struct MultipleMouseClickHelper {
    QPoint last_press_position;
    Qt::MouseButton last_press_button;
    int click_counter;
    ulong last_press_timestamp;

    MultipleMouseClickHelper()
        : last_press_position(QPoint())
        , last_press_button(Qt::NoButton)
        , click_counter(0)
        , last_press_timestamp(0) {}
  };

  // The model object.
  RenderWidgetHostImpl* host_;

  QInputMethod& input_method_;
  ui::TextInputType text_input_type_;
  gfx::Rect caret_rect_;
  size_t selection_anchor_position_;
  size_t selection_cursor_position_;

  MultipleMouseClickHelper click_helper_;

  // True when this view acts as a platform view hack for a
  // RenderWidgetHostViewGuest.
  bool is_guest_view_hack_;

  scoped_ptr<QQuickItem> native_view_;

  QExplicitlySharedDataPointer<ChromiumCompositorData> compositor_data_;
  cc::ReturnedResourceArray resources_to_release_;
  bool needs_delegated_frame_ack_;
  uint32 pending_output_surface_id_;

  gfx::Vector2dF last_scroll_offset_;

  QPoint locked_mouse_position_;

  DISALLOW_COPY_AND_ASSIGN(RenderWidgetHostViewQt);
};

} // namespace content

#endif // CONTENT_BROWSER_RENDERER_HOST_RENDER_WIDGET_HOST_VIEW_QT_H_

