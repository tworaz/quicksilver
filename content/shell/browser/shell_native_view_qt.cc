// Copyright (c) 2015 Piotr Tworek. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "content/shell/browser/shell_native_view_qt.h"

#include "base/logging.h"

namespace content {

ShellNativeViewQt::ShellNativeViewQt()
    : QQuickItem(0) {
}

void ShellNativeViewQt::geometryChanged(const QRectF& newGeometry,
                                        const QRectF& oldGeometry) {
  QQuickItem::geometryChanged(newGeometry, oldGeometry);
  QList<QQuickItem*> childList = childItems();
  QList<QQuickItem*>::iterator it = childList.begin();
  for (; it != childList.end(); it++)
    (*it)->setSize(newGeometry.size());
}

} // namespace content
