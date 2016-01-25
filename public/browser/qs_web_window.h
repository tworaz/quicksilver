// Copyright 2015 Piotr Tworek. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef QUICKSILVER_PUBLIC_BROWSER_QS_WEB_WINDOW_H_
#define QUICKSILVER_PUBLIC_BROWSER_QS_WEB_WINDOW_H_

#include <QtCore/QObject>

class QWindow;

namespace QuickSilverImpl {
class QSWebWindowImpl;
}

namespace QuickSilver {

class QSWebView;

class QSWebWindow : public QObject {
 Q_OBJECT

  Q_PROPERTY(QWindow* qwindow READ getQWindow CONSTANT FINAL)
  Q_PROPERTY(QString title READ title WRITE setTitle NOTIFY titleChanged FINAL)
  Q_PROPERTY(Qt::ScreenOrientation contentOrientation READ contentOrientation
             WRITE setContentOrientation
             NOTIFY contentOrientationChanged FINAL);

 public:
  explicit QSWebWindow(QSize size, QObject* parent = nullptr);
  ~QSWebWindow() override;

  void addView(QSWebView*);
  void show();
  void hide();
  void setTitle(QString title);
  QString title() const;
  QWindow* getQWindow() const;
  Qt::ScreenOrientation contentOrientation() const;
  void setContentOrientation(Qt::ScreenOrientation);

  QuickSilverImpl::QSWebWindowImpl* impl() const;

 signals:
  void closeRequest();
  void titleChanged(QString);
  void contentOrientationChanged(Qt::ScreenOrientation);

 private:
  QScopedPointer<QuickSilverImpl::QSWebWindowImpl> impl_;

  Q_DISABLE_COPY(QSWebWindow);
};

} // namespace QuickSilver

#endif // QUICKSILVER_PUBLIC_BROWSER_QS_WEB_WINDOW_H_
