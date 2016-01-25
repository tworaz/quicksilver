// Copyright (c) 2015 Piotr Tworek. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "quicksilver/public/browser/qs_web_area.h"

#include <QtGui/QGuiApplication>

#include "base/command_line.h"
#include "base/logging.h"
#include "ozone_qt/components/ime/input_method_qt.h"
#include "ozone_qt/platform/ozone_qt_switches.h"
#include "ozone_qt/platform/platform_event_source_qt.h"
#include "ozone_qt/util/type_conversion.h"
#include "quicksilver/browser/qs_web_window_impl.h"
#include "quicksilver/public/browser/qs_web_window.h"
#include "ui/gfx/geometry/insets.h"
#include "ui/gfx/geometry/rect_f.h"

namespace QuickSilver {

namespace {
const int kMinInsetsUpdateDelay = 100;
}

class QSWebArea::Priv {
 public:
  Priv()
      : simulate_touch_events(false)
      , insets(0, 0, 0, 0) { }

  // Should we simulate touch events when using mouse.
  bool simulate_touch_events;
  gfx::Insets insets;
};

QSWebArea::QSWebArea(QQuickItem* parent)
    : QQuickItem(parent)
    , priv_(new Priv)
    , web_window_(nullptr)
    , controls_height_(0) {

  QQuickItem::setAcceptedMouseButtons(Qt::AllButtons);
  QQuickItem::setAcceptHoverEvents(true);
  QQuickItem::setActiveFocusOnTab(true);
  QQuickItem::setFocus(true);

  priv_->simulate_touch_events = base::CommandLine::ForCurrentProcess()->
      HasSwitch(switches::kSimulateTouchScreenWithMouse);
}

QSWebArea::~QSWebArea() {
  delete priv_;
  priv_ = nullptr;
  web_window_ = nullptr;
}

int QSWebArea::controlsHeight() const {
  return controls_height_;
}

void QSWebArea::setControlsHeight(int height) {
  if (controls_height_ != height) {
    controls_height_ = height;
    emit controlsHeightChanged();
    if (web_window_) {
      web_window_->impl()->SetControlsHeight(height);
    }
  }
}

QObject* QSWebArea::webWindow() const {
  return web_window_;
}

void QSWebArea::setWebWindow(QObject* winObj) {
  QSWebWindow* ww = qobject_cast<QSWebWindow*>(winObj);
  if (!ww) {
    LOG(WARNING) << "WebWindow needs to be of QSWebWindow type!";
    return;
  }
  if (web_window_ != ww) {
    web_window_ = ww;
    if (controls_height_) {
      web_window_->impl()->SetControlsHeight(controls_height_);
    }
    emit webWindowChanged();
  }
}

void QSWebArea::keyPressEvent(QKeyEvent* event) {
  ForwardEvent(*event);
}

void QSWebArea::keyReleaseEvent(QKeyEvent* event) {
  ForwardEvent(*event);
}

void QSWebArea::touchEvent(QTouchEvent* event) {
  forceActiveFocus();
  ForwardEvent(*event);
  event->setAccepted(true);
}

void QSWebArea::mouseMoveEvent(QMouseEvent* event) {
  if (priv_->simulate_touch_events) {
    event->ignore();
    return;
  }
  ForwardEvent(*event);
}

void QSWebArea::mousePressEvent(QMouseEvent* event) {
  forceActiveFocus();
  if (priv_->simulate_touch_events) {
    event->ignore();
    return;
  }
  ForwardEvent(*event);
}

void QSWebArea::mouseReleaseEvent(QMouseEvent* event) {
  if (priv_->simulate_touch_events) {
    event->ignore();
    return;
  }
  ForwardEvent(*event);
}

void QSWebArea::wheelEvent(QWheelEvent* event) {
  if (priv_->simulate_touch_events) {
    event->ignore();
    return;
  }
  ForwardEvent(*event);
}

void QSWebArea::hoverMoveEvent(QHoverEvent* event) {
  if (priv_->simulate_touch_events) {
    event->ignore();
    return;
  }
  ForwardEvent(*event);
}

void QSWebArea::focusInEvent(QFocusEvent* event) {
  web_window_->impl()->Focus();
}

void QSWebArea::focusOutEvent(QFocusEvent* event) {
  web_window_->impl()->Blur();
}

void QSWebArea::geometryChanged(const QRectF& new_geometry,
                                const QRectF& old_geometry) {
  gfx::RectF rect = ToGfx(new_geometry);
  gfx::RectF window_bounds = gfx::RectF(web_window_->impl()->GetBounds());
  const gfx::Transform& transform = web_window_->impl()->
      GetInputEventTransform();
  transform.TransformRect(&window_bounds);
  window_bounds.set_origin(gfx::Point(0, 0));
  priv_->insets = ToNearestRect(window_bounds).InsetsFrom(ToNearestRect(rect));
  UpdateWindowInsets();
}

void QSWebArea::inputMethodEvent(QInputMethodEvent* event) {
  web_window_->impl()->GetInputMethod()->ProcessEvent(*event);
}

QVariant QSWebArea::inputMethodQuery(Qt::InputMethodQuery query) const {
  return web_window_->impl()->GetInputMethod()->Query(query);
}

void QSWebArea::UpdateWindowInsets() {
  if (!web_window_) {
    LOG(WARNING) << "Can't update window insets without a window!";
    return;
  }
  web_window_->impl()->SetInsets(priv_->insets);
}

template<class T>
void QSWebArea::ForwardEvent(const T& event) {
  if (web_window_) {
    const gfx::Transform& transform = web_window_->impl()->
        GetInputEventTransform();
    static_cast<ui::PlatformEventSourceQt*>(
        ui::PlatformEventSource::GetInstance())->ProcessEvent(event, transform);
  } else {
    LOG(WARNING) << "No QSWebWindow to which events should be forwarded";
  }
}

void QSWebArea::ForwardEvent(const QKeyEvent& event) {
  static_cast<ui::PlatformEventSourceQt*>(
      ui::PlatformEventSource::GetInstance())->ProcessEvent(event);
}

} // namespace QuickSilver
