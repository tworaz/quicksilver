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

#ifndef CONTENT_BROWSER_RENDERER_HOST_INPUT_MOTION_EVENT_QT_H_
#define CONTENT_BROWSER_RENDERER_HOST_INPUT_MOTION_EVENT_QT_H_

#include <QList>
#include <QTouchEvent>

#include "base/time/time.h"
#include "ui/events/gesture_detection/motion_event.h"


namespace content {

class MotionEventQt : public ui::MotionEvent {
 public:
  MotionEventQt(const QList<QTouchEvent::TouchPoint>&,
                base::TimeTicks& eventTime,
                Action action,
                const Qt::KeyboardModifiers,
                float device_scale_factor,
                int index = -1);
  ~MotionEventQt() override;

  int GetId() const override;
  Action GetAction() const override;
  int GetActionIndex() const override;
  size_t GetPointerCount() const override;
  int GetPointerId(size_t pointer_index) const override;
  float GetX(size_t pointer_index) const override;
  float GetY(size_t pointer_index) const override;
  float GetRawX(size_t pointer_index) const override;
  float GetRawY(size_t pointer_index) const override;
  float GetTouchMajor(size_t pointer_index) const override;
  float GetTouchMinor(size_t pointer_index) const override;
  float GetOrientation(size_t pointer_index) const override;
  int GetFlags() const override;
  float GetPressure(size_t pointer_index) const override;
  base::TimeTicks GetEventTime() const override;
  ToolType GetToolType(size_t pointer_index) const override;
  int GetButtonState() const override;

 private:
  float ToDips(float pixels) const;

  QList<QTouchEvent::TouchPoint> touch_points_;
  base::TimeTicks event_time_;
  Action action_;
  int flags_;
  float device_scale_factor_;
  int index_;

  DISALLOW_COPY_AND_ASSIGN(MotionEventQt);
};

} // namespace content

#endif // CONTENT_BROWSER_RENDERER_HOST_INPUT_MOTION_EVENT_QT_H_
