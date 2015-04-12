// Copyright 2015 Piotr Tworek. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_SHELL_BROWSER_SHELL_QT_H_
#define CONTENT_SHELL_BROWSER_SHELL_QT_H_

#include <QObject>
#include <QQmlApplicationEngine>
#include <QQmlComponent>
#include <QString>

#include "base/macros.h"
#include "base/memory/scoped_ptr.h"
#include "base/strings/string16.h"

class QQuickWindow;
class QQuickCloseEvent;
class QQuickItem;

namespace content {
class Shell;
class WebContentsViewItemQt;

class ShellWindowQt : public QObject {
  Q_OBJECT
 public:
  ShellWindowQt(QQmlApplicationEngine& engine);
  ~ShellWindowQt() override;

  void SetShell(Shell*);
  void Resize(int width, int height);
  void SetTitle(const base::string16&);
  void SetAddressBarURL(QString);
  void SetIsLoading(bool);
  void EnableBackButton(bool enable);
  void EnableForwardButton(bool enable);
  void EnableStopButton(bool enable);
  QQuickItem* GetWebContentsViewItem() const;

 private slots:
  void OnWindowClosing(QQuickCloseEvent*);
  void OnNewWindowButtonClicked();
  void OnBackButtonClicked();
  void OnForwardButtonClicked();
  void OnRefreshButtonClicked();
  void OnStopButtonClicked();
  void OnAddressEditingFinished();

 private:
  Shell* shell_;
  QQmlApplicationEngine& engine_;
  scoped_ptr<QQmlComponent> component_;
  scoped_ptr<QQuickWindow> window_;

  QQuickItem* new_window_button_;
  QQuickItem* back_button_;
  QQuickItem* forward_button_;
  QQuickItem* refresh_button_;
  QQuickItem* stop_button_;
  QQuickItem* address_bar_;
  QQuickItem* progress_bar_;
  QQuickItem* web_contents_view_item_;

  QString current_url_;

  DISALLOW_COPY_AND_ASSIGN(ShellWindowQt);
};

} // namespace content

#endif // CONTENT_SHELL_BROWSER_SHELL_QT_H_
