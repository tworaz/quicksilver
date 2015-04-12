// Copyright (c) 2015 Piotr Tworek. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_SHELL_BROWSER_SHELL_NATIVE_VIEW_QT_H_
#define CONTENT_SHELL_BROWSER_SHELL_NATIVE_VIEW_QT_H_

#include <QQuickItem>

namespace content {

class WebContents;

class  ShellNativeViewQt : public QQuickItem {
 public:
  ShellNativeViewQt();

 protected:
  void geometryChanged(const QRectF & newGeometry,
                       const QRectF & oldGeometry) override;
};

} // content

#endif // !CONTENT_SHELL_BROWSER_SHELL_NATIVE_VIEW_QT_H_
