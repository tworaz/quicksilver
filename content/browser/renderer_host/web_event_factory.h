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

#ifndef CONTENT_BROWSER_RENDERER_HOST_WEB_EVENT_FACTORY_H_
#define CONTENT_BROWSER_RENDERER_HOST_WEB_EVENT_FACTORY_H_

#include "content/public/browser/native_web_keyboard_event.h"
#include "third_party/WebKit/public/web/WebInputEvent.h"

class QHoverEvent;
class QKeyEvent;
class QMouseEvent;
class QWheelEvent;

namespace content {

class WebEventFactory {
 public:
  static blink::WebMouseEvent toWebMouseEvent(QMouseEvent*,
      float device_scale_factor);
  static blink::WebMouseEvent toWebMouseEvent(QHoverEvent*,
      float device_scale_factor);
  static blink::WebMouseWheelEvent toWebWheelEvent(QWheelEvent*,
      float device_scale_factor);
  static content::NativeWebKeyboardEvent toWebKeyboardEvent(QKeyEvent*);
};

} // namespace content

#endif // CONTENT_BROWSER_RENDERER_HOST_WEB_EVENT_FACTORY_H_
