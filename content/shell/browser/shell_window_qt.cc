// Copyright 2015 Piotr Tworek. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "content/shell/browser/shell_window_qt.h"

#include <QQuickWindow>
#include <QQuickItem>
#include <private/qquickwindow_p.h>

#undef signals

#include "base/logging.h"
#include "content/public/browser/browser_thread.h"
#include "content/public/browser/web_contents.h"
#include "content/shell/browser/shell.h"
#include "content/shell/browser/shell_browser_context.h"
#include "content/shell/browser/shell_content_browser_client.h"
#include "ui/qt/type_conversion.h"

namespace content {

namespace {
const char kHomePage[] = "https://www.google.com";
}

ShellWindowQt::ShellWindowQt(QQmlApplicationEngine& engine)
    : shell_(NULL),
      engine_(engine) {
  component_.reset(new QQmlComponent(&engine_));
  component_->loadUrl(QUrl("qrc:/shell.qml"));
  CHECK(component_->isReady()) << qPrintable(component_->errorString());
  window_.reset(qobject_cast<QQuickWindow*>(component_->create()));

  new_window_button_ = window_->findChild<QQuickItem*>("newWindowButton");
  CHECK(new_window_button_);
  back_button_ = window_->findChild<QQuickItem*>("backButton");
  CHECK(back_button_);
  forward_button_ = window_->findChild<QQuickItem*>("forwardButton");
  CHECK(forward_button_);
  refresh_button_ = window_->findChild<QQuickItem*>("refreshButton");
  CHECK(refresh_button_);
  stop_button_ = window_->findChild<QQuickItem*>("stopButton");
  CHECK(stop_button_);
  address_bar_ = window_->findChild<QQuickItem*>("addressBar");
  CHECK(address_bar_);
  progress_bar_ = window_->findChild<QQuickItem*>("progressBar");
  CHECK(progress_bar_);
  web_contents_view_item_ = window_->
      findChild<QQuickItem*>("webView");
  CHECK(web_contents_view_item_);

  connect(window_.get(), SIGNAL(closing(QQuickCloseEvent*)),
          this, SLOT(OnWindowClosing(QQuickCloseEvent*)));
  connect(new_window_button_, SIGNAL(clicked()),
          this, SLOT(OnNewWindowButtonClicked()));
  connect(back_button_, SIGNAL(clicked()),
          this, SLOT(OnBackButtonClicked()));
  connect(forward_button_, SIGNAL(clicked()),
          this, SLOT(OnForwardButtonClicked()));
  connect(refresh_button_, SIGNAL(clicked()),
          this, SLOT(OnRefreshButtonClicked()));
  connect(stop_button_, SIGNAL(clicked()),
          this, SLOT(OnStopButtonClicked()));
  connect(address_bar_, SIGNAL(editingFinished()),
          this, SLOT(OnAddressEditingFinished()));
}

ShellWindowQt::~ShellWindowQt() {
}

void ShellWindowQt::SetShell(Shell* shell) {
  shell_ = shell;
  window_->show();
}

void ShellWindowQt::Resize(int width, int height) {
  window_->setWidth(width);
  window_->setHeight(height);
}

void ShellWindowQt::SetTitle(const base::string16& title) {
  window_->setTitle(toQt(title));
}

void ShellWindowQt::SetAddressBarURL(QString url) {
  current_url_ = url;
  address_bar_->setProperty("text",  url);
}

void ShellWindowQt::SetIsLoading(bool loading) {
  progress_bar_->setVisible(loading);
}

void ShellWindowQt::EnableBackButton(bool enable) {
  back_button_->setEnabled(enable);
}

void ShellWindowQt::EnableForwardButton(bool enable) {
  forward_button_->setEnabled(enable);
}

void ShellWindowQt::EnableStopButton(bool enable) {
  stop_button_->setEnabled(enable);
}

QQuickItem* ShellWindowQt::GetWebContentsViewItem() const {
  return web_contents_view_item_;
}

void ShellWindowQt::OnWindowClosing(QQuickCloseEvent* evt) {
  evt->setAccepted(false);
  shell_->Close();
}

void ShellWindowQt::OnBackButtonClicked() {
  shell_->GoBackOrForward(-1);
}

void ShellWindowQt::OnNewWindowButtonClicked() {
  ShellBrowserContext* browser_context =
      ShellContentBrowserClient::Get()->browser_context();
  Shell::CreateNewWindow(browser_context,
                         GURL(kHomePage),
                         NULL,
                         gfx::Size());
}

void ShellWindowQt::OnForwardButtonClicked() {
  shell_->GoBackOrForward(1);
}

void ShellWindowQt::OnRefreshButtonClicked() {
  shell_->Reload();
}

void ShellWindowQt::OnStopButtonClicked() {
  shell_->Stop();
}

void ShellWindowQt::OnAddressEditingFinished() {
  QVariant prop = address_bar_->property("text");
  if (prop.toString() != current_url_) {
    QUrl url = prop.toUrl();
    if (url.scheme().isEmpty())
      url.setScheme("http");
    shell_->LoadURL(toGurl(url));
  }
}

} // namespace content
