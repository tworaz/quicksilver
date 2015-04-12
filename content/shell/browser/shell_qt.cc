// Copyright 2015 Piotr Tworek. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "content/shell/browser/shell.h"

#include <QMap>
#include <QQmlApplicationEngine>

#include "base/logging.h"
#include "content/browser/web_contents/web_contents_view_qt.h"
#include "content/public/browser/web_contents.h"
#include "content/shell/browser/shell_native_view_qt.h"
#include "content/shell/browser/shell_window_qt.h"
#include "ui/qt/type_conversion.h"

namespace content {

const int kDefaultTestWindowWidthDip = 800;
const int kDefaultTestWindowHeightDip = 600;

namespace {

QQmlApplicationEngine* g_engine;
QMap<const WebContents*, ShellWindowQt*> g_shell_window_map;

static QQuickItem* CreateWebContentsViewItemQt(
    const WebContents* web_contents) {
  ShellWindowQt* win = new ShellWindowQt(*g_engine);
  DCHECK(!g_shell_window_map.contains(web_contents));
  g_shell_window_map[web_contents] = win;
  return win->GetWebContentsViewItem();
}

} // namespace

// static
void Shell::PlatformInitialize(const gfx::Size& default_window_size) {
  g_engine = new QQmlApplicationEngine();

  WebContentsViewQt::SetNativeViewFactoryDelegate(&CreateWebContentsViewItemQt);

  qmlRegisterType<ShellNativeViewQt>("ChromiumQt", 1, 0, "WebContentsView");
}

void Shell::PlatformExit() {
  LOG(INFO) << "All windows closed, exiting";
  DCHECK(g_shell_window_map.isEmpty());
  delete g_engine;
}

void Shell::PlatformCleanUp() {
  LOG(INFO) << "Shell Qt window closed";
  DCHECK(g_shell_window_map.contains(web_contents()));
  g_shell_window_map.remove(web_contents());
  delete static_cast<ShellWindowQt*>(window_);
}

void Shell::PlatformEnableUIControl(UIControl control, bool is_enabled) {
  ShellWindowQt* win = static_cast<ShellWindowQt*>(window_);
  switch (control) {
  case BACK_BUTTON:
    win->EnableBackButton(is_enabled);
    break;
  case FORWARD_BUTTON:
    win->EnableForwardButton(is_enabled);
    break;
  case STOP_BUTTON:
    win->EnableStopButton(is_enabled);
    break;
  default:
    NOTREACHED();
  }
}

void Shell::PlatformSetAddressBarURL(const GURL& url) {
  static_cast<ShellWindowQt*>(window_)->SetAddressBarURL(toQt(url.spec()));
}

void Shell::PlatformSetIsLoading(bool loading) {
  static_cast<ShellWindowQt*>(window_)->SetIsLoading(loading);
}

void Shell::PlatformCreateWindow(int width, int height) {
}

void Shell::PlatformSetContents() {
  DCHECK(g_shell_window_map.contains(web_contents()));
  ShellWindowQt* win = g_shell_window_map[web_contents()];
  win->SetShell(this);
  win->Resize(kDefaultTestWindowWidthDip, kDefaultTestWindowHeightDip);
  window_ = win;
}

void Shell::PlatformResizeSubViews() {
}

void Shell::Close() {
  delete this;
}

void Shell::PlatformSetTitle(const base::string16& title) {
  static_cast<ShellWindowQt*>(window_)->SetTitle(title);
}

bool Shell::PlatformHandleContextMenu(
    const content::ContextMenuParams& params) {
  NOTIMPLEMENTED();
  return false;
}

} // namespace content

