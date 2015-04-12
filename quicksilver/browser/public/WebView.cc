// Copyright (c) 2015 Piotr Tworek. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "quicksilver/browser/public/WebView.h"

#include "ui/qt/type_conversion.h"
#include "url/gurl.h"
#include "quicksilver/browser/web_view_impl.h"

namespace QuickSilver {

WebView::WebView(QQuickItem* parent)
    : QQuickItem(parent),
      impl_(new QuickSilverImpl::WebViewImpl(this)) {
}

WebView::~WebView() {
  delete impl_;
}

void WebView::loadURL(QString u) {
  QUrl url(u);
  if (url.scheme().isEmpty())
    url.setScheme("http");
  impl_->LoadURL(toGurl(url));
}

void WebView::reload(bool force) {
  impl_->Reload(force);
}

void WebView::stop() {
  impl_->Stop();
}

void WebView::forward() {
  impl_->GoBackOrForward(1);
}

void WebView::back() {
  impl_->GoBackOrForward(-1);
}

QString WebView::currentURL() const {
  return QString(impl_->GetVisibleURL().spec().c_str());
}

QString WebView::title() const {
  return toQt(impl_->GetTitle());
}

bool WebView::canGoForward() const {
  return impl_->CanGoForward();
}

bool WebView::canGoBack() const {
  return impl_->CanGoBack();
}

bool WebView::loading() const {
  return impl_->IsLoading();
}

double WebView::loadProgress() const {
  return impl_->LoadProgress();
}

void WebView::geometryChanged(const QRectF& newGeometry,
                              const QRectF& oldGeometry) {
  QQuickItem::geometryChanged(newGeometry, oldGeometry);
  QList<QQuickItem*> childList = childItems();
  QList<QQuickItem*>::iterator it = childList.begin();
  for (; it != childList.end(); it++)
    (*it)->setSize(newGeometry.size());
}

} // namespace QuickSilver
