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

#include "content/browser/renderer_host/input/motion_event_qt.h"

#include "ui/events/event.h"

namespace content {

namespace {

static inline int flagsFromModifiers(Qt::KeyboardModifiers modifiers) {
  int modifierFlags = ui::EF_NONE;
  if ((modifiers & Qt::ControlModifier) != 0)
    modifierFlags |= ui::EF_CONTROL_DOWN;
  if ((modifiers & Qt::MetaModifier) != 0)
    modifierFlags |= ui::EF_COMMAND_DOWN;
  if ((modifiers & Qt::ShiftModifier) != 0)
    modifierFlags |= ui::EF_SHIFT_DOWN;
  if ((modifiers & Qt::AltModifier) != 0)
    modifierFlags |= ui::EF_ALT_DOWN;
  return modifierFlags;
}

} // namespace

MotionEventQt::MotionEventQt(
    const QList<QTouchEvent::TouchPoint>& touchPoints,
    base::TimeTicks& eventTime,
    ui::MotionEvent::Action action,
    const Qt::KeyboardModifiers modifiers,
    float device_scale_factor,
    int index)
        : touch_points_(touchPoints),
          event_time_(eventTime),
          action_(action),
          flags_(flagsFromModifiers(modifiers)),
          device_scale_factor_(device_scale_factor),
          index_(index) {
  // ACTION_DOWN and ACTION_UP must be accesssed through pointer_index 0
  DCHECK((action != ACTION_DOWN && action != ACTION_UP) || index == 0);
}

MotionEventQt::~MotionEventQt() {
}

int MotionEventQt::GetId() const {
  return 0;
}

ui::MotionEvent::Action MotionEventQt::GetAction() const {
  return action_;
}

int MotionEventQt::GetActionIndex() const {
  return index_;
}

size_t MotionEventQt::GetPointerCount() const {
  return touch_points_.size();
}

int MotionEventQt::GetPointerId(size_t pointer_index) const {
  return touch_points_.at(pointer_index).id();
}

float MotionEventQt::GetX(size_t pointer_index) const {
  return ToDips(touch_points_.at(pointer_index).pos().x());
}

float MotionEventQt::GetY(size_t pointer_index) const {
  return ToDips(touch_points_.at(pointer_index).pos().y());
}

float MotionEventQt::GetRawX(size_t pointer_index) const {
  return touch_points_.at(pointer_index).screenPos().x();
}

float MotionEventQt::GetRawY(size_t pointer_index) const {
  return touch_points_.at(pointer_index).screenPos().y();
}

float MotionEventQt::GetTouchMajor(size_t pointer_index) const {
  QRectF touchRect = touch_points_.at(pointer_index).rect();
  return ToDips(std::max(touchRect.height(), touchRect.width()));
}

float MotionEventQt::GetTouchMinor(size_t pointer_index) const {
    QRectF touchRect = touch_points_.at(pointer_index).rect();
    return ToDips(std::min(touchRect.height(), touchRect.width()));
}

float MotionEventQt::GetOrientation(size_t pointer_index) const {
    return 0;
}

int MotionEventQt::GetFlags() const {
  return flags_;
}

float MotionEventQt::GetPressure(size_t pointer_index) const {
  return touch_points_.at(pointer_index).pressure();
}

base::TimeTicks MotionEventQt::GetEventTime() const {
  return event_time_;
}

ui::MotionEvent::ToolType MotionEventQt::GetToolType(
    size_t pointer_index) const {
  return ui::MotionEvent::TOOL_TYPE_UNKNOWN;
}

int MotionEventQt::GetButtonState() const {
  return 0;
}

float MotionEventQt::ToDips(float pixels) const {
  return pixels / device_scale_factor_;
}

} // namespace content
