// Copyright (c) 2015 Piotr Tworek. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef _QUICKSILVER_PUBLIC_BROWSER_QS_WEB_VIEW_H_
#define _QUICKSILVER_PUBLIC_BROWSER_QS_WEB_VIEW_H_

#include <QtCore/QObject>
#include <QtCore/QSize>
#include <QtGui/QVector2D>

#include "qs_context_menu_params.h"

namespace QuickSilverImpl {
class QSWebViewImpl;
}

namespace QuickSilver {

class QSWebContext;
class QSWebViewDelegate;

class QSWebView : public QObject {
  Q_OBJECT;

  Q_PROPERTY(QString currentURL READ currentURL NOTIFY currentURLChanged FINAL)
  Q_PROPERTY(QString title READ title NOTIFY titleChanged FINAL)
  Q_PROPERTY(bool canGoForward READ canGoForward NOTIFY canGoForwardChanged FINAL)
  Q_PROPERTY(bool canGoBack READ canGoBack NOTIFY canGoBackChanged FINAL)
  Q_PROPERTY(bool loading READ loading NOTIFY loadingChanged FINAL)
  Q_PROPERTY(double loadProgress READ loadProgress NOTIFY loadProgressChanged FINAL)
  Q_PROPERTY(bool visible READ visible NOTIFY visibleChanged FINAL)
  Q_PROPERTY(bool fullscreen READ fullscreen NOTIFY fullscreenChanged FINAL);
  Q_PROPERTY(double overscrollY READ overscrollY NOTIFY overscrollYChanged FINAL);
  Q_PROPERTY(QVector2D scrollOffset READ scrollOffset NOTIFY scrollOffsetChanged FINAL);
  Q_PROPERTY(QSizeF contentSize READ contentSize NOTIFY contentSizeChanged FINAL);

 public:
  explicit QSWebView(QSWebContext* context, QObject* parent = nullptr);
  ~QSWebView() override;

  void SetDelegate(QSWebViewDelegate* delegate);

  Q_INVOKABLE void loadURL(QString url);
  Q_INVOKABLE void reload(bool force = false);
  Q_INVOKABLE void stop();
  Q_INVOKABLE void forward();
  Q_INVOKABLE void back();
  Q_INVOKABLE void forceActiveFocus();
  Q_INVOKABLE void show();
  Q_INVOKABLE bool fullscreen() const;

  QString currentURL() const;
  QString title() const;
  bool canGoForward() const;
  bool canGoBack() const;
  bool loading() const;
  double loadProgress() const;
  bool visible() const;
  double overscrollY() const;
  QVector2D scrollOffset() const;
  QSizeF contentSize() const;

  QuickSilverImpl::QSWebViewImpl* impl() const;

 signals:
  void currentURLChanged();
  void titleChanged();
  void canGoForwardChanged(bool);
  void canGoBackChanged(bool);
  void loadingChanged(bool);
  void loadProgressChanged(double);
  void visibleChanged(bool);
  void fullscreenChanged(bool);
  void overscrollYChanged(double);
  void overscrollYComplete();
  void scrollOffsetChanged();
  void contentSizeChanged();

 private:
  friend class QuickSilverImpl::QSWebViewImpl;

  QSWebView(QuickSilverImpl::QSWebViewImpl*, QObject* parent = nullptr);

  QScopedPointer<QuickSilverImpl::QSWebViewImpl> impl_;

  Q_DISABLE_COPY(QSWebView);
};

} // namespace QuickSilver

#endif // !_QUICKSILVER_PUBLIC_BROWSER_QS_WEB_VIEW_H_

