// Copyright (c) 2015 Piotr Tworek. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef _QUICKSILVER_SHELL_SHELL_CHROME_H_
#define _QUICKSILVER_SHELL_SHELL_CHROME_H_

#include <QtCore/QObject>
#include <QtGui/QWindow>

#include "quicksilver/public/browser/qs_context_menu_params.h"
#include "quicksilver/public/browser/qs_web_view_delegate.h"
#include "quicksilver/shell/shell_tab_model.h"

class QQmlApplicationEngine;
class QQmlComponent;
class QQuickView;

namespace QuickSilver {
class QSContextMenuParams;
class QSJavaScriptDialog;
class QSWebContext;
class QSWebView;
class QSWebWindow;
};

class ShellChrome
    : public QObject
    , public QuickSilver::QSWebViewDelegate {
  Q_OBJECT
  using QSWebView = QuickSilver::QSWebView;

  Q_PROPERTY(QString searchEngineURL READ searchEngineURL
             WRITE setSearchEngineURL
             NOTIFY searchEngineURLChanged FINAL)

 public:
  explicit ShellChrome(QObject* parent = nullptr);
  ~ShellChrome() override;

  Q_INVOKABLE void CreateTab(QString url, bool show = true);
  Q_INVOKABLE void CloseTab(QuickSilver::QSWebView*);
  Q_INVOKABLE void SetChromeWindow(QObject*);
  Q_INVOKABLE QUrl ValidateURL(QString);

 signals:
  void showContextMenu(QuickSilver::QSContextMenuParams* menuParams);
  void showJavaScriptDialog(QuickSilver::QSJavaScriptDialog* dialogDescription);
  void cancelAllJavaScriptDialogs();
  void requestPermission(QuickSilver::QSPermissionRequest* requestDescription);
  void cancelPermissionRequest(int requestId);
  void searchEngineURLChanged();

 private slots:
  void OnWebWindowCloseRequest();
  void OnWebWindowVisibilityChanged(QWindow::Visibility);

 private:
  QString searchEngineURL() const;
  void setSearchEngineURL(QString);

  // QuickSilver::QSWebViewDelegate:
  bool CanCreateChild(const QSWebView*, QUrl& url) override;
  void WebViewCreated(const QSWebView*, QSWebView*) override;
  void RequestWebViewClose(QSWebView*) override;
  void ShowContextMenu(const QSWebView*,
                       const QuickSilver::QSContextMenuParams&) override;
  bool ShowJavaScriptDialog(const QSWebView*,
                            QuickSilver::QSJavaScriptDialog*) override;
  void CancelAllJavaScriptDialogs(const QSWebView*) override;
  bool RequestPermission(const QSWebView*,
                         QuickSilver::QSPermissionRequest*) override;
  void CancelPermissionRequest(const QSWebView*, int request_id) override;

  QScopedPointer<QuickSilver::QSWebWindow> web_window_;
  QScopedPointer<QuickSilver::QSWebContext> web_context_;

  ShellTabModel tab_model_;

#if defined(OS_SAILFISH)
  QScopedPointer<QQuickView> view_;
#else
  QScopedPointer<QQmlApplicationEngine> engine_;
  QScopedPointer<QQmlComponent> component_;
#endif

  QString search_engine_url_;

  Q_DISABLE_COPY(ShellChrome);
};

#endif //! _QUICKSILVER_SHELL_SHELL_CHROME_H_
