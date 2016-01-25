// Copyright 2015 Piotr Tworek. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef QUICKSILVER_BROWSER_QS_WEB_WINDOW_H_
#define QUICKSILVER_BROWSER_QS_WEB_WINDOW_H_

#include "base/memory/scoped_ptr.h"
#include "base/strings/string16.h"
#include "ui/aura/client/window_tree_client.h"
#include "ui/aura/window_tree_host_observer.h"
#include "ui/gfx/display.h"
#include "ui/gfx/geometry/insets.h"
#include "ui/gfx/geometry/size.h"
#include "ui/gfx/transform.h"

class QWindow;

namespace aura {
class WindowTreeHost;
namespace client {
class FocusClient;
class DefaultCaptureClient;
class WindowTreeClient;
class ScreenPositionClientQt;
} // namespace client
} // namespace aura

namespace ui {
class InputMethodQt;
}

namespace QuickSilver {
class QSWebWindow;
}

namespace QuickSilverImpl {

class QSWebViewImpl;

class QSWebWindowImpl
    : public aura::client::WindowTreeClient
    , public aura::WindowTreeHostObserver {
 public:
  explicit QSWebWindowImpl(QuickSilver::QSWebWindow&, gfx::Size size);
  ~QSWebWindowImpl() override;

  void AddView(QSWebViewImpl*);
  void Show();
  void Hide();
  void Rotate(gfx::Display::Rotation);
  const gfx::Transform& GetInputEventTransform() const;
  void Focus();
  void Blur();

  void SetControlsHeight(int height);
  void SetInsets(gfx::Insets&);
  gfx::Insets GetInsets() const;
  gfx::Rect GetBounds() const;
  QWindow* GetQWindow() const;
  ui::InputMethodQt* GetInputMethod() const;

 private:
  // aura::WindowTreeHostObserver:
  void OnHostCloseRequested(const aura::WindowTreeHost*) override;

  // aura::client::WindowTreeClient:
  aura::Window* GetDefaultParent(aura::Window* context,
                                 aura::Window* window,
                                 const gfx::Rect& bounds) override;

  scoped_ptr<aura::WindowTreeHost> host_;
  scoped_ptr<aura::client::FocusClient> focus_client_;
  scoped_ptr<aura::client::DefaultCaptureClient> capture_client_;
  scoped_ptr<aura::client::ScreenPositionClientQt> screen_position_client_;
  QuickSilver::QSWebWindow& web_window_;
  gfx::Transform input_event_transform_;
  gfx::Insets insets_;

  DISALLOW_COPY_AND_ASSIGN(QSWebWindowImpl);
};

} // namespace QuickSilverImpl

#endif // QUICKSILVER_BROWSER_QS_WEB_WINDOW_H_

