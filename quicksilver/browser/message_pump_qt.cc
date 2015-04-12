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

#include "quicksilver/browser/message_pump_qt.h"

#include <QCoreApplication>
#include <QEventLoop>
#include <QEvent>

#include "base/message_loop/message_loop.h"

namespace QuickSilverImpl {

namespace {

// Return a timeout suitable for the glib loop, -1 to block forever,
// 0 to return right away, or a timeout in milliseconds from now.
int GetTimeIntervalMilliseconds(const base::TimeTicks& from) {
  if (from.is_null())
    return -1;

  // Be careful here.  TimeDelta has a precision of microseconds, but we want a
  // value in milliseconds.  If there are 5.5ms left, should the delay be 5 or
  // 6?  It should be 6 to avoid executing delayed work too early.
  int delay = static_cast<int>(
      ceil((from - base::TimeTicks::Now()).InMillisecondsF()));

  // If this value is negative, then we need to run delayed work soon.
  return delay < 0 ? 0 : delay;
}

} // namespace

MessagePumpQt::MessagePumpQt()
    // Usually this gets passed through Run, but since we have
    // our own event loop, attach it explicitly ourselves.
    : delegate_(base::MessageLoopForUI::current())
    , explicit_loop_(0)
    , timer_id_(0) {
}

void MessagePumpQt::Run(Delegate* delegate) {
  Q_ASSERT(delegate == delegate_);
  // This is used only when MessagePumpQt is used outside of the GUI thread.
  QEventLoop loop;
  explicit_loop_ = &loop;
  loop.exec();
  explicit_loop_ = 0;
}

void MessagePumpQt::Quit() {
  DCHECK(explicit_loop_);
  explicit_loop_->quit();
}

void MessagePumpQt::ScheduleWork() {
  QCoreApplication::postEvent(this, new QEvent(QEvent::User));
}

void MessagePumpQt::ScheduleDelayedWork(
  const base::TimeTicks& delayed_work_time) {
  if (delayed_work_time.is_null()) {
    killTimer(timer_id_);
    timer_id_ = 0;
    timer_scheduled_time_ = base::TimeTicks();
  } else if (!timer_id_ || delayed_work_time < timer_scheduled_time_) {
    killTimer(timer_id_);
    timer_id_ = startTimer(GetTimeIntervalMilliseconds(delayed_work_time));
    timer_scheduled_time_ = delayed_work_time;
  }
}

void MessagePumpQt::customEvent(QEvent* ev) {
  if (handleScheduledWork())
    QCoreApplication::postEvent(this, new QEvent(QEvent::User));
}

void MessagePumpQt::timerEvent(QTimerEvent *ev) {
  DCHECK(timer_id_ == ev->timerId());
  killTimer(timer_id_);
  timer_id_ = 0;
  timer_scheduled_time_ = base::TimeTicks();

  base::TimeTicks next_delayed_work_time;
  delegate_->DoDelayedWork(&next_delayed_work_time);
  ScheduleDelayedWork(next_delayed_work_time);
}

bool MessagePumpQt::handleScheduledWork() {
  bool more_work_is_plausible = delegate_->DoWork();

  base::TimeTicks delayed_work_time;
  more_work_is_plausible |= delegate_->DoDelayedWork(&delayed_work_time);

  if (more_work_is_plausible)
    return true;

  more_work_is_plausible |= delegate_->DoIdleWork();
  if (!more_work_is_plausible)
    ScheduleDelayedWork(delayed_work_time);

  return more_work_is_plausible;
}

} // namespace quickSilverImpl
