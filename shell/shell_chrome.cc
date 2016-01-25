// Copyright (c) 2015 Piotr Tworek. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "quicksilver/shell/shell_chrome.h"

#include <QtCore/QDebug>
#include <QtCore/QSize>
#include <QtGui/QGuiApplication>
#include <QtGui/QScreen>
#include <QtQml/QQmlApplicationEngine>
#include <QtQml/QQmlComponent>
#include <QtQml/QQmlContext>
#include <QtQuick/QQuickView>

#include "quicksilver/public/browser/qs_browser_process.h"
#include "quicksilver/public/browser/qs_web_context.h"
#include "quicksilver/public/browser/qs_web_view.h"
#include "quicksilver/public/browser/qs_web_window.h"

using namespace QuickSilver;

ShellChrome::ShellChrome(QObject* parent)
    : QObject(parent)
#if defined(OS_SAILFISH)
    , view_(new QQuickView)
#else
    , engine_(new QQmlApplicationEngine)
    , component_(new QQmlComponent(engine_.data()))
#endif
{
  QSize window_size;
#if defined(OS_SAILFISH)
  const QScreen* screen = qApp->primaryScreen();
  window_size = screen->availableSize();
#else
  window_size = QSize(540, 960);
#endif

  web_window_.reset(new QSWebWindow(window_size));
  connect(web_window_.data(), &QSWebWindow::closeRequest,
          this, &ShellChrome::OnWebWindowCloseRequest);
  connect(web_window_->getQWindow(), &QWindow::visibilityChanged,
          this, &ShellChrome::OnWebWindowVisibilityChanged);
  web_window_->show();

  // Normal browsing context.
  web_context_.reset(new QSWebContext(false));

  QQmlContext* context;
#if defined(OS_SAILFISH)
  context = view_->rootContext();
#else
  context = engine_->rootContext();
#endif
  context->setContextProperty("TabModel", &tab_model_);
  context->setContextProperty("Browser", this);
  context->setContextProperty("WebWindow", web_window_.data());

#if defined(OS_SAILFISH)
  view_->setSource(QUrl("qrc:/ui/sailfish/Chrome.qml"));
#else
  component_->loadUrl(QUrl("qrc:/ui/desktop/Chrome.qml"));
  if (!component_->isReady())
    qFatal("Failed to load QML component: %s",
           qPrintable(component_->errorString()));
  component_->create();
#endif
}

ShellChrome::~ShellChrome() {
}

void ShellChrome::CreateTab(QString url, bool show) {
  QSWebView* wv = new QSWebView(web_context_.data(), web_window_.data());
  wv->SetDelegate(this);
  web_window_->addView(wv);
  tab_model_.Append(wv);
  wv->loadURL(url);
  if (show) {
    wv->show();
  }
}

void ShellChrome::CloseTab(QSWebView* wv) {
  Q_ASSERT(tab_model_.Contains(wv));
  tab_model_.Remove(wv);
  wv->SetDelegate(nullptr);
  delete wv;
}

void ShellChrome::SetChromeWindow(QObject* obj) {
  QQuickWindow *view = qobject_cast<QQuickWindow*>(obj);
  Q_ASSERT(view);
  view->setTransientParent(web_window_->getQWindow());
  web_window_->setContentOrientation(view->contentOrientation());
#if defined(OS_SAILFISH)
  view->showFullScreen();
#else
  view->show();
#endif
}

QUrl ShellChrome::ValidateURL(QString url) {
  QUrl ret(url);

  if (search_engine_url_.isEmpty()) {
    return ret;
  }

  QUrl temp = ret;
  // QUrl can't determine TLD for URLs withoht schemes
  if (ret.scheme().isEmpty()) {
    temp = QUrl(QString("http://%1").arg(url));
  }

  if ((temp.topLevelDomain().isEmpty() && ret.scheme().isEmpty()) ||
      !ret.isValid()) {
    //QString searchUrl("https://duckduckgo.com/?q=%1");
    QString searchUrl(search_engine_url_);
    searchUrl = searchUrl.arg(url);
    ret = QUrl(searchUrl);
  }
  return ret;
}

void ShellChrome::OnWebWindowCloseRequest() {
  tab_model_.CloseAllTabs();
  web_context_.reset();
  web_window_.reset();
  QSBrowserProcess::instance()->exit();
}

void ShellChrome::OnWebWindowVisibilityChanged(QWindow::Visibility visibility) {
#if defined(OS_SAILFISH)
  // Witout this we sometimes end up with black output surface.
  switch (visibility) {
    case QWindow::Windowed:
    case QWindow::Maximized:
    case QWindow::FullScreen:
    case QWindow::AutomaticVisibility:
      view_->update();
      break;
  }
#endif
}

QString ShellChrome::searchEngineURL() const {
  return search_engine_url_;
}

void ShellChrome::setSearchEngineURL(QString url) {
  if (search_engine_url_ != url) {
    search_engine_url_ = url;
    emit searchEngineURLChanged();
  }
}

bool ShellChrome::CanCreateChild(const QSWebView*, QUrl& url) {
  qDebug() << "Allowing child view for:" << url;
  return true;
}

void ShellChrome::WebViewCreated(const QSWebView*, QSWebView* wv) {
  wv->SetDelegate(this);
  web_window_->addView(wv);
  tab_model_.Append(wv);
  wv->show();
}

void ShellChrome::RequestWebViewClose(QSWebView* wv) {
  CloseTab(wv);
}

void ShellChrome::ShowContextMenu(const QSWebView*,
                                  const QSContextMenuParams& params) {
  QSContextMenuParams newParams(params);
  emit showContextMenu(&newParams);
}

bool ShellChrome::ShowJavaScriptDialog(const QSWebView*,
                                       QSJavaScriptDialog* dialog) {
  emit showJavaScriptDialog(dialog);
  return true;
}

void ShellChrome::CancelAllJavaScriptDialogs(const QSWebView*) {
  emit cancelAllJavaScriptDialogs();
}

bool ShellChrome::RequestPermission(const QSWebView*, QSPermissionRequest* req) {
  emit requestPermission(req);
  return true;
}

void ShellChrome::CancelPermissionRequest(const QSWebView*, int request_id) {
  emit cancelPermissionRequest(request_id);
}
