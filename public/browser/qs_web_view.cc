// Copyright (c) 2015 Piotr Tworek. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "quicksilver/public/browser/qs_web_view.h"

#include <QtCore/QUrl>

#include "base/logging.h"
#include "ozone_qt/util/type_conversion.h"
#include "quicksilver/browser/qs_web_view_impl.h"
#include "quicksilver/public/browser/qs_web_context.h"
#include "quicksilver/public/browser/qs_web_window.h"

using namespace QuickSilverImpl;

namespace QuickSilver {

QSWebView::QSWebView(QSWebContext* context, QObject* parent)
    : QObject(parent)
    , impl_(new QSWebViewImpl(context, this)) {
}

QSWebView::QSWebView(QSWebViewImpl* impl, QObject* parent)
    : QObject(parent)
    , impl_(impl) {
}

QSWebView::~QSWebView() {
}

void QSWebView::SetDelegate(QSWebViewDelegate* delegate) {
  impl_->SetDelegate(delegate);
}

void QSWebView::loadURL(QString url) {
  QUrl qUrl(url);
  if (qUrl.scheme().isEmpty()) {
    qUrl.setScheme("http");
  }
  impl_->LoadURL(toGurl(qUrl.toString()));
}

void QSWebView::reload(bool force) {
  impl_->Reload(force);
}

void QSWebView::stop() {
  impl_->Stop();
}

void QSWebView::forward() {
  impl_->GoBackOrForward(1);
}

void QSWebView::back() {
  impl_->GoBackOrForward(-1);
}

void QSWebView::forceActiveFocus() {
  impl_->Focus();
}

void QSWebView::show() {
  impl_->Show();
}

bool QSWebView::fullscreen() const {
  return impl_->IsFullscreen();
}

QString QSWebView::currentURL() const {
  return QString(impl_->GetVisibleURL().spec().c_str());
}

QString QSWebView::title() const {
  return ToQt(impl_->GetTitle());
}

bool QSWebView::canGoForward() const {
  return impl_->CanGoForward();
}

bool QSWebView::canGoBack() const {
  return impl_->CanGoBack();
}

bool QSWebView::loading() const {
  return impl_->IsLoading();
}

double QSWebView::loadProgress() const {
  return impl_->LoadProgress();
}

bool QSWebView::visible() const {
  return impl_->IsVisible();
}

double QSWebView::overscrollY() const {
  return impl_->OverscrollY();
}

QVector2D QSWebView::scrollOffset() const {
  return ToQt(impl_->ScrollOffset());
}

QSizeF QSWebView::contentSize() const {
  return ToQt(impl_->WebContentSize());
}

QuickSilverImpl::QSWebViewImpl* QSWebView::impl() const {
  return impl_.data();
}

} // namespace QuickSilver

