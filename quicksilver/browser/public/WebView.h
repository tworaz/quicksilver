// Copyright (c) 2015 Piotr Tworek. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef QUICKSILVER_BROWSER_PUBLIC_WEB_VIEW_H_
#define QUICKSILVER_BROWSER_PUBLIC_WEB_VIEW_H_

#include <QQuickItem>

namespace QuickSilverImpl {
class WebViewImpl;
}

namespace QuickSilver {

class NewWindowPermissionRequest;

class WebView : public QQuickItem {
 Q_OBJECT

 Q_PROPERTY(QString currentURL READ currentURL NOTIFY currentURLChanged)
 Q_PROPERTY(QString title READ title NOTIFY titleChanged)
 Q_PROPERTY(bool canGoForward READ canGoForward NOTIFY canGoForwardChanged)
 Q_PROPERTY(bool canGoBack READ canGoBack NOTIFY canGoBackChanged)
 Q_PROPERTY(bool loading READ loading NOTIFY loadingChanged)
 Q_PROPERTY(double loadProgress READ loadProgress NOTIFY loadProgressChanged)

 public:
  explicit WebView(QQuickItem* parent = 0);
  ~WebView() override;

  Q_INVOKABLE void loadURL(QString);
  Q_INVOKABLE void reload(bool force);
  Q_INVOKABLE void stop();
  Q_INVOKABLE void forward();
  Q_INVOKABLE void back();

 signals:
  void currentURLChanged();
  void titleChanged();
  void canGoForwardChanged(bool);
  void canGoBackChanged(bool);
  void loadingChanged(bool);
  void loadProgressChanged(double);
  void newWindowPermissionRequest(
      QuickSilver::NewWindowPermissionRequest* request);
  void createNewWindow();

 protected:
  friend class QuickSilverImpl::WebViewImpl;

  QString currentURL() const;
  QString title() const;
  bool canGoForward() const;
  bool canGoBack() const;
  bool loading() const;
  double loadProgress() const;

 private:
  void geometryChanged(const QRectF & newGeometry,
                       const QRectF & oldGeometry) final;

  QuickSilverImpl::WebViewImpl* impl_;
};

} // namespace QuickSilver

#endif // !QUICKSILVER_BROWSER_PUBLIC_WEB_VIEW_H_
