// Copyright 2015 Piotr Tworek. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "quicksilver/public/browser/qs_web_window.h"

#include <QtGui/QWindow>

#include "ozone_qt/util/type_conversion.h"
#include "quicksilver/browser/qs_web_window_impl.h"
#include "quicksilver/public/browser/qs_web_view.h"
#include "ui/gfx/display.h"

using namespace QuickSilverImpl;

namespace QuickSilver {

QSWebWindow::QSWebWindow(QSize size, QObject* parent)
    : QObject(parent)
    , impl_(new QSWebWindowImpl(*this, ToGfx(size))) {
  Q_ASSERT(getQWindow());
}

QSWebWindow::~QSWebWindow() {
}

void QSWebWindow::addView(QSWebView* web_view) {
  impl_->AddView(web_view->impl());
}

void QSWebWindow::show() {
  impl_->Show();
}

void QSWebWindow::hide() {
  impl_->Hide();
}

void QSWebWindow::setTitle(QString title) {
  if (title != getQWindow()->title()) {
    getQWindow()->setTitle(title);
    emit titleChanged(title);
  }
}

QString QSWebWindow::title() const {
  return getQWindow()->title();
}

QWindow* QSWebWindow::getQWindow() const {
  return impl_->GetQWindow();
}

Qt::ScreenOrientation QSWebWindow::contentOrientation() const {
  return getQWindow()->contentOrientation();
}

void QSWebWindow::setContentOrientation(Qt::ScreenOrientation orientation) {
  if (orientation != getQWindow()->contentOrientation()) {
    switch (orientation) {
      case Qt::PrimaryOrientation:
      case Qt::PortraitOrientation:
        impl_->Rotate(gfx::Display::ROTATE_0);
        break;
      case Qt::LandscapeOrientation:
        impl_->Rotate(gfx::Display::ROTATE_90);
        break;
      case Qt::InvertedPortraitOrientation:
        impl_->Rotate(gfx::Display::ROTATE_180);
        break;
      case Qt::InvertedLandscapeOrientation:
        impl_->Rotate(gfx::Display::ROTATE_270);
        break;
      default:
        NOTREACHED();
        break;
    }
    getQWindow()->reportContentOrientationChange(orientation);
    emit contentOrientationChanged(orientation);
  }
}

QSWebWindowImpl* QSWebWindow::impl() const {
  return impl_.data();
}

} // namespace QuickSilver
