// Copyright (c) 2015 Piotr Tworek. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/gfx/x/x11_types.h"

#include <QGuiApplication>
#include <qpa/qplatformnativeinterface.h>
#include <X11/Xlib.h>

namespace gfx {

XDisplay* GetXDisplay() {
  static void* display = NULL;
  if (!display)
    display = qApp->platformNativeInterface()->
        nativeResourceForScreen(QByteArrayLiteral("display"),
                                qApp->primaryScreen());
  if (!display)
      display = XOpenDisplay(NULL);
  return static_cast<XDisplay*>(display);
}

} // namespace gfx
