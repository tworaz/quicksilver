// Copyright (c) 2015 Piotr Tworek. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef QUICKSILVER_PUBLIC_BROWSER_QS_WEB_AREA_H_
#define QUICKSILVER_PUBLIC_BROWSER_QS_WEB_AREA_H_

#include <QtQuick/QQuickItem>

namespace QuickSilver {

class QSWebWindow;

class QSWebArea : public QQuickItem {
  Q_OBJECT
  Q_PROPERTY(QObject* webWindow READ webWindow
             WRITE setWebWindow NOTIFY webWindowChanged FINAL)
  Q_PROPERTY(int controlsHeight READ controlsHeight
	     WRITE setControlsHeight NOTIFY controlsHeightChanged FINAL)

 public:
  explicit QSWebArea(QQuickItem* parent = nullptr);
  ~QSWebArea() override;

  int controlsHeight() const;
  void setControlsHeight(int height);

 signals:
  void webWindowChanged();
  void controlsHeightChanged();

 private:
  QObject* webWindow() const;
  void setWebWindow(QObject*);

  // QQuickItem:
  void keyPressEvent(QKeyEvent*) override;
  void keyReleaseEvent(QKeyEvent*) override;
  void touchEvent(QTouchEvent*) override;
  void mouseMoveEvent(QMouseEvent*) override;
  void mousePressEvent(QMouseEvent*) override;
  void mouseReleaseEvent(QMouseEvent*) override;
  void wheelEvent(QWheelEvent*) override;
  void hoverMoveEvent(QHoverEvent* event) override;
  void focusInEvent(QFocusEvent*) override;
  void focusOutEvent(QFocusEvent*) override;
  void geometryChanged(const QRectF&, const QRectF&) override;
  void inputMethodEvent(QInputMethodEvent*) override;
  QVariant inputMethodQuery(Qt::InputMethodQuery) const override;

  void UpdateWindowInsets();

  template<class T>
  void ForwardEvent(const T& event);
  void ForwardEvent(const QKeyEvent& event);

  QSWebWindow* web_window_; // Not owned
  int controls_height_;

  class Priv;
  class Priv* priv_;

  Q_DISABLE_COPY(QSWebArea);
};

} // namespace QuickSilver

#endif // !QUICKSILVER_PUBLIC_BROWSER_QS_WEB_AREA_H_

