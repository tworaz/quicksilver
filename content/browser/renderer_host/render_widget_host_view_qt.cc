// Copyright (c) 2015 Piotr Tworek. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
** This file is part of the QtWebEngine module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see http://www.qt.io/terms-conditions. For further
** information use the contact form at http://www.qt.io/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 3 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPLv3 included in the
** packaging of this file. Please review the following information to
** ensure the GNU Lesser General Public License version 3 requirements
** will be met: https://www.gnu.org/licenses/lgpl.html.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 2.0 or later as published by the Free
** Software Foundation and appearing in the file LICENSE.GPL included in
** the packaging of this file. Please review the following information to
** ensure the GNU General Public License version 2.0 requirements will be
** met: http://www.gnu.org/licenses/gpl-2.0.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "content/browser/renderer_host/render_widget_host_view_qt.h"

#include <QGuiApplication>
#include <QQmlEngine>
#include <QQuickItem>
#include <QQuickWindow>
#include <QScreen>
#include <QStyleHints>
#include <QTextCharFormat>
#undef signals

#include "base/command_line.h"
#include "cc/output/compositor_frame_ack.h"
#include "content/browser/renderer_host/input/motion_event_qt.h"
#include "content/browser/renderer_host/render_view_host_impl.h"
#include "content/browser/renderer_host/render_widget_host_impl.h"
#include "content/browser/renderer_host/web_event_factory.h"
#include "content/common/view_messages.h"
#include "content/public/browser/browser_thread.h"
#include "content/public/browser/render_process_host.h"
#include "content/public/common/content_switches_qt.h"
#include "third_party/WebKit/public/platform/WebScreenInfo.h"
#include "ui/events/blink/blink_event_util.h"
#include "ui/gfx/geometry/dip_util.h"
#include "ui/qt/type_conversion.h"

using blink::WebScreenInfo;

namespace content {

namespace {

static inline int FirstAvailableId(const QMap<int, int> &map) {
  ui::BitSet32 usedIds;
  QMap<int, int>::const_iterator end = map.end();
  for (QMap<int, int>::const_iterator it = map.begin(); it != end; ++it)
    usedIds.mark_bit(it.value());
  return usedIds.first_unmarked_bit();
}

static inline bool CompareTouchPoints(const QTouchEvent::TouchPoint& lhs,
                                      const QTouchEvent::TouchPoint& rhs) {
  // TouchPointPressed < TouchPointMoved < TouchPointReleased
  return lhs.state() < rhs.state();
}

static inline ui::GestureProvider::Config QtGestureProviderConfig() {
  ui::GestureProvider::Config config = ui::GetGestureProviderConfig(
      ui::GestureProviderConfigType::CURRENT_PLATFORM);
  // Causes an assert in CreateWebGestureEventFromGestureEventData and
  // we don't need them in Qt.
  config.gesture_begin_end_types_enabled = false;
  config.gesture_detector_config.swipe_enabled = false;
  config.gesture_detector_config.two_finger_tap_enabled = false;
  return config;
}

ui::LatencyInfo CreateLatencyInfo(const blink::WebInputEvent& event) {
  ui::LatencyInfo latency_info;
  // The latency number should only be added if the timestamp is valid.
  if (event.timeStampSeconds) {
    const int64 time_micros = static_cast<int64>(
        event.timeStampSeconds * base::Time::kMicrosecondsPerSecond);
    latency_info.AddLatencyNumberWithTimestamp(
        ui::INPUT_EVENT_LATENCY_ORIGINAL_COMPONENT,
        0,
        0,
        base::TimeTicks() + base::TimeDelta::FromMicroseconds(time_micros),
        1);
  }
  return latency_info;
}

} // namespace

class RenderWidgetHostViewQtDelegate : public QQuickItem {
 public:
  RenderWidgetHostViewQtDelegate(RenderWidgetHostViewQt& rwhv)
      : QQuickItem(0),
        rwhv_(rwhv),
        ignore_mouse_events_(false) {
    setFlags(QQuickItem::ItemHasContents);
    setAcceptedMouseButtons(Qt::AllButtons);
    setAcceptHoverEvents(true);
    setActiveFocusOnTab(true);
    setFocus(true);
    ignore_mouse_events_ = base::CommandLine::ForCurrentProcess()->
        HasSwitch(switches::kSimulateTouchScreenWithMouse);
  }

 protected:
  void geometryChanged(const QRectF & newGeometry,
                       const QRectF & oldGeometry) final {
    QQuickItem::geometryChanged(newGeometry, oldGeometry);
    rwhv_.HandleResize();
  }

  void focusInEvent(QFocusEvent* event) final {
    rwhv_.HandleFocusEvent(event);
  }

  void focusOutEvent(QFocusEvent* event) final {
    rwhv_.HandleFocusEvent(event);
  }

  void keyPressEvent(QKeyEvent* event) final {
    forceActiveFocus();
    rwhv_.HandleKeyEvent(event);
  }

  void keyReleaseEvent(QKeyEvent* event) final {
    rwhv_.HandleKeyEvent(event);
  }

  void mouseMoveEvent(QMouseEvent* event) final {
    if (!ignore_mouse_events_) {
      rwhv_.HandleMouseEvent(event);
    } else {
      event->ignore();
    }
  }

  void mousePressEvent(QMouseEvent* event) final {
    if (!ignore_mouse_events_) {
      forceActiveFocus();
      rwhv_.HandleMouseEvent(event);
    } else {
      event->ignore();
    }
  }

  void mouseReleaseEvent(QMouseEvent* event) final {
    if (!ignore_mouse_events_) {
      rwhv_.HandleMouseEvent(event);
    } else {
      event->ignore();
    }
  }

  void hoverMoveEvent(QHoverEvent* event) final {
    if (!ignore_mouse_events_) {
      rwhv_.HandleHoverEvent(event);
    } else {
      event->ignore();
    }
  }

  void wheelEvent(QWheelEvent* event) final {
    if (!ignore_mouse_events_) {
      rwhv_.HandleWheelEvent(event);
    } else {
      event->ignore();
    }
  }

  void touchEvent(QTouchEvent * event) final {
    rwhv_.HandleTouchEvent(event);
  }

  void inputMethodEvent(QInputMethodEvent* event) final {
    rwhv_.HandleInputMethodEvent(event);
  }

  QSGNode* updatePaintNode(QSGNode* oldNode, UpdatePaintNodeData*) final {
    return rwhv_.UpdatePaintNode(oldNode);
  }

  QVariant inputMethodQuery(Qt::InputMethodQuery query) const final {
    return rwhv_.InputMethodQuery(query);
  }

 private:
  RenderWidgetHostViewQt& rwhv_;
  bool ignore_mouse_events_;
};

RenderWidgetHostViewQt::RenderWidgetHostViewQt(RenderWidgetHost* host,
                                               bool is_guest_view_hack)
    : host_(RenderWidgetHostImpl::From(host)),
      gesture_provider_(QtGestureProviderConfig(), this),
      send_motion_action_down_(false),
      input_method_(*qApp->inputMethod()),
      text_input_type_(ui::TEXT_INPUT_TYPE_NONE),
      compositor_data_(new ChromiumCompositorData),
      is_guest_view_hack_(is_guest_view_hack),
      needs_delegated_frame_ack_(false),
      pending_output_surface_id_(0) {
  if (!is_guest_view_hack_)
    host_->SetView(this);
}

RenderWidgetHostViewQt::~RenderWidgetHostViewQt() {
  native_view_->setParent(NULL);
  native_view_->setParentItem(NULL);
  host_ = NULL;
}

void RenderWidgetHostViewQt::InitAsChild(gfx::NativeView parent_view) {
  native_view_.reset(new RenderWidgetHostViewQtDelegate(*this));
  native_view_->setSize(QSizeF(parent_view->width(), parent_view->height()));
  // QML code shouldn't manage native view for RenderWidgetHostView
  QQmlEngine::setObjectOwnership(native_view_.get(), QQmlEngine::CppOwnership);
}

RenderWidgetHost* RenderWidgetHostViewQt::GetRenderWidgetHost() const {
  return host_;
}

void RenderWidgetHostViewQt::SetSize(const gfx::Size& size) {
  if (size.IsEmpty())
    return;
  NOTIMPLEMENTED() << ", size: " << size.ToString();
}

void RenderWidgetHostViewQt::SetBounds(const gfx::Rect& rect) {
  NOTIMPLEMENTED();
}

gfx::Vector2dF RenderWidgetHostViewQt::GetLastScrollOffset() const {
  return last_scroll_offset_;
}

gfx::NativeView RenderWidgetHostViewQt::GetNativeView() const {
  return static_cast<gfx::NativeView>(native_view_.get());
}

gfx::NativeViewId RenderWidgetHostViewQt::GetNativeViewId() const {
  return static_cast<gfx::NativeViewId>(NULL);
}

gfx::NativeViewAccessible RenderWidgetHostViewQt::GetNativeViewAccessible() {
  NOTIMPLEMENTED();
  return static_cast<gfx::NativeViewAccessible>(NULL);
}

bool RenderWidgetHostViewQt::HasFocus() const {
  return native_view_->hasFocus();
}

bool RenderWidgetHostViewQt::IsSurfaceAvailableForCopy() const {
  NOTIMPLEMENTED();
  return false;
}

void RenderWidgetHostViewQt::Show() {
  native_view_->setVisible(true);
}

void RenderWidgetHostViewQt::Hide() {
  native_view_->setVisible(false);
}

bool RenderWidgetHostViewQt::IsShowing() {
  NOTIMPLEMENTED();
  return true;
}

gfx::Rect RenderWidgetHostViewQt::GetViewBounds() const {
  DCHECK(native_view_->window());
  QPointF scenePoint = native_view_->mapToScene(QPointF(0, 0));
  QPointF screenPos;
  if (native_view_->window())
    screenPos = native_view_->window()->mapToGlobal(scenePoint.toPoint());
  gfx::Rect r = gfx::Rect(screenPos.x(), screenPos.y(),
                          native_view_->width(), native_view_->height());
  return gfx::ConvertRectToDIP(current_device_scale_factor_, r);
}

void RenderWidgetHostViewQt::SetBackgroundColor(SkColor color) {
  NOTIMPLEMENTED();
}

void RenderWidgetHostViewQt::SetInsets(const gfx::Insets& insets) {
  NOTIMPLEMENTED();
}

bool RenderWidgetHostViewQt::LockMouse() {
  locked_mouse_position_ = QCursor::pos();
  native_view_->grabMouse();
  qApp->setOverrideCursor(Qt::BlankCursor);
  return true;
}

void RenderWidgetHostViewQt::UnlockMouse() {
  native_view_->ungrabMouse();
  qApp->restoreOverrideCursor();
  host_->LostMouseLock();
}

void RenderWidgetHostViewQt::InitAsPopup(
    RenderWidgetHostView* parent_host_view,
    const gfx::Rect& pos) {
  NOTIMPLEMENTED();
}

void RenderWidgetHostViewQt::InitAsFullscreen(
    RenderWidgetHostView* reference_host_view) {
  NOTIMPLEMENTED();
}

void RenderWidgetHostViewQt::MovePluginWindows(
    const std::vector<WebPluginGeometry>& moves) {
}

void RenderWidgetHostViewQt::Focus() {
  host_->SetInputMethodActive(true);
  native_view_->setFocus(true);
  native_view_->forceActiveFocus();
  host_->Focus();
}

void RenderWidgetHostViewQt::Blur() {
  host_->SetInputMethodActive(false);
  host_->Blur();
}

void RenderWidgetHostViewQt::UpdateCursor(const WebCursor& cursor) {
  native_view_->window()->setCursor(QCursor(
      static_cast<Qt::CursorShape>(cursor.GetNativeCursor())));
}

void RenderWidgetHostViewQt::SetIsLoading(bool is_loading) {
}

void RenderWidgetHostViewQt::TextInputTypeChanged(
    ui::TextInputType type, ui::TextInputMode input_mode,
    bool can_compose_inline, int flags) {
  text_input_type_ = type;

  bool visible = (type != ui::TEXT_INPUT_TYPE_NONE);
  native_view_->setFlag(QQuickItem::ItemAcceptsInputMethod, visible);
  input_method_.update(Qt::ImQueryInput | Qt::ImEnabled | Qt::ImHints);
  input_method_.setVisible(visible);
}

void RenderWidgetHostViewQt::ImeCancelComposition() {
  input_method_.reset();
}

void RenderWidgetHostViewQt::ImeCompositionRangeChanged(
    const gfx::Range& range,
    const std::vector<gfx::Rect>& character_bounds) {
  NOTIMPLEMENTED();
}

void RenderWidgetHostViewQt::RenderProcessGone(
    base::TerminationStatus status, int error_code) {
  Destroy();
}

void RenderWidgetHostViewQt::Destroy() {
  delete this;
}

void RenderWidgetHostViewQt::SetTooltipText(
    const base::string16& tooltip_text) {
  //NOTIMPLEMENTED();
}

void RenderWidgetHostViewQt::SelectionChanged(
    const base::string16& text,
    size_t offset,
    const gfx::Range& range) {
  RenderWidgetHostViewBase::SelectionChanged(text, offset, range);
  NOTIMPLEMENTED();
}

void RenderWidgetHostViewQt::SelectionBoundsChanged(
    const ViewHostMsg_SelectionBounds_Params& params) {
  if (selection_range_.IsValid()) {
    if (params.is_anchor_first) {
      selection_anchor_position_ =
          selection_range_.GetMin() - selection_text_offset_;
      selection_cursor_position_ =
          selection_range_.GetMax() - selection_text_offset_;
    } else {
      selection_anchor_position_ =
          selection_range_.GetMax() - selection_text_offset_;
      selection_cursor_position_ =
          selection_range_.GetMin() - selection_text_offset_;
    }
  }
  caret_rect_ = gfx::UnionRects(params.anchor_rect, params.focus_rect);
}

void RenderWidgetHostViewQt::CopyFromCompositingSurface(
    const gfx::Rect& src_subrect,
    const gfx::Size& dst_size,
    ReadbackRequestCallback& callback,
    const SkColorType color_type) {
  NOTIMPLEMENTED();
}

void RenderWidgetHostViewQt::CopyFromCompositingSurfaceToVideoFrame(
    const gfx::Rect& src_subrect,
    const scoped_refptr<media::VideoFrame>& target,
    const base::Callback<void(bool)>& callback) {
  NOTIMPLEMENTED();
}

bool RenderWidgetHostViewQt::CanCopyToVideoFrame() const {
  NOTIMPLEMENTED();
  return false;
}

bool RenderWidgetHostViewQt::HasAcceleratedSurface(
    const gfx::Size& desired_size) {
  NOTIMPLEMENTED();
  return false;
}

void RenderWidgetHostViewQt::GetScreenInfo(blink::WebScreenInfo* result) {
  DCHECK(native_view_->window());

  QScreen* screen;
  if (native_view_->window()) {
    screen = native_view_->window()->screen();
  } else {
    screen = qApp->primaryScreen();
  }

  result->deviceScaleFactor = current_device_scale_factor_;
  result->depthPerComponent = 8;
  result->depth = screen->depth();
  result->isMonochrome = (result->depth == 1);

  QRect g = screen->geometry();
  result->rect = blink::WebRect(g.x(), g.y(), g.width(), g.height());
  g = screen->availableGeometry();
  result->availableRect = blink::WebRect(g.x(), g.y(), g.width(), g.height());
}

gfx::Rect RenderWidgetHostViewQt::GetBoundsInRootWindow() {
  DCHECK(native_view_->window());
  return gfx::ConvertRectToDIP(current_device_scale_factor_,
      toGfx(native_view_->window()->frameGeometry()));
}

gfx::GLSurfaceHandle RenderWidgetHostViewQt::GetCompositingSurface() {
  return gfx::GLSurfaceHandle(gfx::kNullPluginWindow, gfx::NULL_TRANSPORT);
}

void RenderWidgetHostViewQt::OnSwapCompositorFrame(uint32 output_surface_id,
    scoped_ptr<cc::CompositorFrame> frame) {
  last_scroll_offset_ = frame->metadata.root_scroll_offset;
  DCHECK(!needs_delegated_frame_ack_);
  needs_delegated_frame_ack_ = true;
  pending_output_surface_id_ = output_surface_id;
  DCHECK(frame->delegated_frame_data);
  DCHECK(!compositor_data_->frameData ||
         compositor_data_->frameData->resource_list.empty());
  compositor_data_->frameData = frame->delegated_frame_data.Pass();

  native_view_->update();
}

void RenderWidgetHostViewQt::ProcessAckedTouchEvent(
    const TouchEventWithLatencyInfo& touch, InputEventAckState ack_result) {
  const bool event_consumed = ack_result == INPUT_EVENT_ACK_STATE_CONSUMED;
  gesture_provider_.OnAsyncTouchEventAck(event_consumed);
}

void RenderWidgetHostViewQt::OnGestureEvent(
    const ui::GestureEventData& gesture) {
  blink::WebGestureEvent web_gesture =
      ui::CreateWebGestureEventFromGestureEventData(gesture);
  host_->ForwardGestureEventWithLatencyInfo(web_gesture,
                                            CreateLatencyInfo(web_gesture));
}

QSGNode* RenderWidgetHostViewQt::UpdatePaintNode(QSGNode* oldNode) {
  DelegatedFrameNode* frameNode = static_cast<DelegatedFrameNode *>(oldNode);
  if (!frameNode)
    frameNode = new DelegatedFrameNode;

  frameNode->commit(compositor_data_.data(), &resources_to_release_,
                    native_view_.get());

  // This is possibly called from the Qt render thread, post the ack back to
  // the UI to tell the child compositors to release resources and trigger a
  // new frame.
  if (needs_delegated_frame_ack_) {
    needs_delegated_frame_ack_ = false;
    BrowserThread::PostTask(BrowserThread::UI, FROM_HERE,
        base::Bind(&RenderWidgetHostViewQt::SendDelegatedFrameAck,
                   AsWeakPtr()));
  }

  return frameNode;
}

void RenderWidgetHostViewQt::SendDelegatedFrameAck() {
  cc::CompositorFrameAck ack;
  resources_to_release_.swap(ack.resources);
  RenderWidgetHostImpl::SendSwapCompositorFrameAck(
      host_->GetRoutingID(), pending_output_surface_id_,
      host_->GetProcess()->GetID(), ack);
}

QVariant RenderWidgetHostViewQt::InputMethodQuery(Qt::InputMethodQuery query) {
  switch (query) {
  case Qt::ImEnabled:
    return QVariant(text_input_type_ != ui::TEXT_INPUT_TYPE_NONE);
  case Qt::ImCursorRectangle:
    return toQt(caret_rect_);
  case Qt::ImCursorPosition:
    return static_cast<uint>(selection_cursor_position_);
  case Qt::ImAnchorPosition:
    return static_cast<uint>(selection_anchor_position_);
  case Qt::ImSurroundingText:
    return toQt(selection_text_);
  case Qt::ImCurrentSelection:
    return toQt(GetSelectedText());
  case Qt::ImHints:
    return int(toQt(text_input_type_));
  default:
    return QVariant();
  }
}

void RenderWidgetHostViewQt::HandleResize() {
  host_->InvalidateScreenInfo();
  host_->WasResized();
  UpdateScreenInfo(native_view_.get());
}

void RenderWidgetHostViewQt::HandleFocusEvent(QFocusEvent* event) {
  if (event->gotFocus()) {
    host_->GotFocus();
    host_->SetActive(true);
    Q_ASSERT(host_->IsRenderView());
    if (event->reason() == Qt::TabFocusReason) {
      static_cast<content::RenderViewHostImpl*>(host_)->
          SetInitialFocus(false);
    } else if (event->reason() == Qt::BacktabFocusReason) {
      static_cast<content::RenderViewHostImpl*>(host_)->
          SetInitialFocus(true);
    }
    event->accept();
  } else if (event->lostFocus()) {
    host_->SetActive(false);
    host_->Blur();
    event->accept();
  }
}

void RenderWidgetHostViewQt::HandleKeyEvent(QKeyEvent* event) {
  if (IsMouseLocked() &&
      event->key() == Qt::Key_Escape &&
      event->type() == QEvent::KeyRelease) {
      UnlockMouse();
  }

  NativeWebKeyboardEvent webEvent = WebEventFactory::toWebKeyboardEvent(event);
  if (webEvent.type == blink::WebInputEvent::RawKeyDown &&
    !event->text().isEmpty()) {
    // Blink won't consume the RawKeyDown, but rather the Char event in
    // this case. Make sure to skip the former on the way back. The same
    // os_event will be set on both of them.
    webEvent.skip_in_browser = true;
    host_->ForwardKeyboardEvent(webEvent);

    webEvent.skip_in_browser = false;
    webEvent.type = blink::WebInputEvent::Char;
    host_->ForwardKeyboardEvent(webEvent);
  } else {
    host_->ForwardKeyboardEvent(webEvent);
  }
}

void RenderWidgetHostViewQt::HandleMouseEvent(QMouseEvent* event) {
  blink::WebMouseEvent webEvent =
      WebEventFactory::toWebMouseEvent(event, current_device_scale_factor_);
  if (event->type() == QMouseEvent::MouseButtonPress) {
    Focus();

    const QStyleHints* hints = qGuiApp->styleHints();
    if (event->button() != click_helper_.last_press_button ||
        (event->timestamp() - click_helper_.last_press_timestamp >
            static_cast<ulong>(hints->mouseDoubleClickInterval())) ||
        (event->pos() - click_helper_.last_press_position).manhattanLength() >
            hints->startDragDistance()) {
        click_helper_.click_counter = 0;
    }

    click_helper_.last_press_timestamp = event->timestamp();
    webEvent.clickCount = ++click_helper_.click_counter;
    click_helper_.last_press_button = event->button();
    click_helper_.last_press_position = QPointF(event->pos()).toPoint();
  }

  if (IsMouseLocked()) {
    webEvent.movementX = -(locked_mouse_position_.x() - event->globalX());
    webEvent.movementY = -(locked_mouse_position_.y() - event->globalY());
    QCursor::setPos(locked_mouse_position_);
  }

  host_->ForwardMouseEvent(webEvent);
}

void RenderWidgetHostViewQt::HandleWheelEvent(QWheelEvent* evt) {
  host_->ForwardWheelEvent(WebEventFactory::toWebWheelEvent(evt,
      current_device_scale_factor_));
}

void RenderWidgetHostViewQt::HandleHoverEvent(QHoverEvent* evt) {
  host_->ForwardMouseEvent(WebEventFactory::toWebMouseEvent(evt,
      current_device_scale_factor_));
}

void RenderWidgetHostViewQt::HandleTouchEvent(QTouchEvent* evt) {
  // Chromium expects the touch event timestamps to be comparable to
  // base::TimeTicks::Now(). Most importantly we also have to preserve the
  // relative time distance between events. Calculate a delta between event
  // timestamps and Now() on the first received event, and apply this delta to
  // all successive events. This delta is most likely smaller than it
  // should by calculating it here but this will hopefully cause less than one
  // frame of delay.
  base::TimeTicks eventTimestamp = base::TimeTicks() +
      base::TimeDelta::FromMilliseconds(evt->timestamp());
  if (events_to_now_delta_ == base::TimeDelta())
    events_to_now_delta_ = base::TimeTicks::Now() - eventTimestamp;
  eventTimestamp += events_to_now_delta_;

  QList<QTouchEvent::TouchPoint> touchPoints =
      MapTouchPointIds(evt->touchPoints());

  if (evt->type() == QEvent::TouchCancel) {
    MotionEventQt cancelEvent(touchPoints, eventTimestamp,
        ui::MotionEvent::ACTION_CANCEL, evt->modifiers(),
        current_device_scale_factor_);
    ProcessMotionEvent(cancelEvent);
    return;
  }

  if (evt->type() == QEvent::TouchBegin)
    send_motion_action_down_ = true;

  // Make sure that ACTION_POINTER_DOWN is delivered before ACTION_MOVE,
  // and ACTION_MOVE before ACTION_POINTER_UP.
  std::sort(touchPoints.begin(), touchPoints.end(), CompareTouchPoints);

  for (int i = 0; i < touchPoints.size(); ++i) {
    ui::MotionEvent::Action action;
    switch (touchPoints[i].state()) {
    case Qt::TouchPointPressed:
      if (send_motion_action_down_) {
        action = ui::MotionEvent::ACTION_DOWN;
        send_motion_action_down_ = false;
      } else {
        action = ui::MotionEvent::ACTION_POINTER_DOWN;
      }
      break;
    case Qt::TouchPointMoved:
      action = ui::MotionEvent::ACTION_MOVE;
      break;
    case Qt::TouchPointReleased:
      action = touchPoints.size() > 1 ?
          ui::MotionEvent::ACTION_POINTER_UP :
          ui::MotionEvent::ACTION_UP;
      break;
    default:
      // Ignore Qt::TouchPointStationary
      continue;
    }

    MotionEventQt motionEvent(touchPoints, eventTimestamp, action,
        evt->modifiers(), current_device_scale_factor_, i);
    ProcessMotionEvent(motionEvent);
  }
}

QList<QTouchEvent::TouchPoint> RenderWidgetHostViewQt::MapTouchPointIds(
    const QList<QTouchEvent::TouchPoint>& inputPoints) {
  QList<QTouchEvent::TouchPoint> outputPoints = inputPoints;
  for (int i = 0; i < outputPoints.size(); ++i) {
    QTouchEvent::TouchPoint& point = outputPoints[i];

    int qtId = point.id();
    QMap<int, int>::const_iterator it = touch_id_mapping_.find(qtId);
    if (it == touch_id_mapping_.end())
      it = touch_id_mapping_.insert(qtId, FirstAvailableId(touch_id_mapping_));
    point.setId(it.value());

    if (point.state() == Qt::TouchPointReleased)
      touch_id_mapping_.remove(qtId);
  }
  return outputPoints;
}

void RenderWidgetHostViewQt::ProcessMotionEvent(const ui::MotionEvent& event) {

  ui::FilteredGestureProvider::TouchHandlingResult result =
      gesture_provider_.OnTouchEvent(event);
  if (!result.succeeded)
    return;

  blink::WebTouchEvent web_event =
      ui::CreateWebTouchEventFromMotionEvent(event, result.did_generate_scroll);
  host_->ForwardTouchEventWithLatencyInfo(web_event,
                                          CreateLatencyInfo(web_event));
}

void RenderWidgetHostViewQt::HandleInputMethodEvent(QInputMethodEvent* evt) {
  QString commitString = evt->commitString();
  QString preeditString = evt->preeditString();
  int replacementStart = evt->replacementStart();
  int replacementLength = evt->replacementLength();

  int cursorPositionInPreeditString = -1;
  gfx::Range selectionRange = gfx::Range::InvalidRange();

  const QList<QInputMethodEvent::Attribute> &attributes = evt->attributes();
  std::vector<blink::WebCompositionUnderline> underlines;

  QList<QInputMethodEvent::Attribute>::const_iterator it = attributes.begin();
  for (; it != attributes.end(); it++) {
    const QInputMethodEvent::Attribute& attribute = *it;

    switch (attribute.type) {
    case QInputMethodEvent::TextFormat: {
      if (preeditString.isEmpty())
          break;

      QTextCharFormat textCharFormat =
          attribute.value.value<QTextFormat>().toCharFormat();
      QColor qcolor = textCharFormat.underlineColor();
      QColor qBackgroundColor = textCharFormat.background().color();
      blink::WebColor color = SkColorSetARGB(qcolor.alpha(), qcolor.red(),
                                             qcolor.green(), qcolor.blue());
      blink::WebColor backgroundColor = SkColorSetARGB(
          qBackgroundColor.alpha(), qBackgroundColor.red(),
          qBackgroundColor.green(), qBackgroundColor.blue());
      int start = qMin(attribute.start, (attribute.start + attribute.length));
      int end = qMax(attribute.start, (attribute.start + attribute.length));
      underlines.push_back(blink::WebCompositionUnderline(
          start, end, color, false, backgroundColor));
      break;
    }
    case QInputMethodEvent::Cursor:
      if (attribute.length)
        cursorPositionInPreeditString = attribute.start;
      break;
    case QInputMethodEvent::Selection:
      selectionRange.set_start(qMin(attribute.start,
                               (attribute.start + attribute.length)));
      selectionRange.set_end(qMax(attribute.start,
                             (attribute.start + attribute.length)));
      break;
    default:
        break;
    }
  }

  if (preeditString.isEmpty()) {
    gfx::Range replacementRange = (replacementLength > 0) ?
        gfx::Range(replacementStart, replacementStart + replacementLength) :
        gfx::Range::InvalidRange();
    host_->ImeConfirmComposition(toString16(commitString),
        replacementRange, false);
  } else {
    if (!selectionRange.IsValid()) {
      // We did not receive a valid selection range, hence the range is
      // going to mark the cursor position.
      int newCursorPosition = (cursorPositionInPreeditString < 0) ?
          preeditString.length() : cursorPositionInPreeditString;
      selectionRange.set_start(newCursorPosition);
      selectionRange.set_end(newCursorPosition);
    }
    host_->ImeSetComposition(toString16(preeditString), underlines,
        selectionRange.start(), selectionRange.end());
  }
}

// static
void RenderWidgetHostViewBase::GetDefaultScreenInfo(WebScreenInfo* results) {
  NOTIMPLEMENTED();
}

} // namespace content
