// Copyright 2015 Piotr Tworek. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "quicksilver/browser/qs_web_window_impl.h"

#include "base/logging.h"
#include "content/public/browser/web_contents.h"
#include "ozone_qt/components/aura_client/screen_position_client_qt.h"
#include "ozone_qt/components/ime/input_method_qt.h"
#include "ozone_qt/components/screen/screen_qt.h"
#include "ozone_qt/util/type_conversion.h"
#include "quicksilver/browser/qs_stack_layout.h"
#include "quicksilver/browser/qs_web_view_impl.h"
#include "quicksilver/public/browser/qs_web_window.h"
#include "ui/aura/client/default_capture_client.h"
#include "ui/aura/env.h"
#include "ui/aura/test/test_focus_client.h" // XXX: Don't use aura test stuff
#include "ui/aura/window.h"
#include "ui/aura/window_tree_host.h"
#include "ui/gfx/screen.h"
#include "ui/wm/core/default_activation_client.h"

using namespace QuickSilver;

namespace QuickSilverImpl {

QSWebWindowImpl::QSWebWindowImpl(QSWebWindow& ww, gfx::Size size)
    : web_window_(ww) {
  CHECK(aura::Env::GetInstance());
  host_.reset(aura::WindowTreeHost::Create(gfx::Rect(size)));
  host_->InitHost();
  host_->window()->SetLayoutManager(new QSStackLayout(host_->window()));

  focus_client_.reset(new aura::test::TestFocusClient());
  aura::client::SetFocusClient(host_->window(), focus_client_.get());

  screen_position_client_.reset(
      new aura::client::ScreenPositionClientQt(*GetQWindow()));
  aura::client::SetScreenPositionClient(host_->window(),
                                        screen_position_client_.get());

  new wm::DefaultActivationClient(host_->window());
  capture_client_.reset(
      new aura::client::DefaultCaptureClient(host_->window()));

  aura::client::SetWindowTreeClient(host_->window(), this);
  host_->AddObserver(this);
}

QSWebWindowImpl::~QSWebWindowImpl() {
  host_->RemoveObserver(this);
  aura::client::SetWindowTreeClient(host_->window(), nullptr);
  aura::client::SetScreenPositionClient(host_->window(), nullptr);
  aura::client::SetFocusClient(host_->window(), nullptr);
}

void QSWebWindowImpl::AddView(QSWebViewImpl* wv) {
  aura::Window* parent = host_->window();
  aura::Window* content = wv->web_contents()->GetNativeView();
  if (!parent->Contains(content)) {
    parent->AddChild(content);
  }
  wv->SetWebWindow(this);
}

void QSWebWindowImpl::Show() {
  host_->Show();
}

void QSWebWindowImpl::Hide() {
  host_->Hide();
}

void QSWebWindowImpl::Rotate(gfx::Display::Rotation rotation) {
  float scale = gfx::Screen::GetNativeScreen()->
      GetPrimaryDisplay().device_scale_factor();
  gfx::Transform transform;
  switch (rotation) {
    case gfx::Display::ROTATE_0:
      break;
    case gfx::Display::ROTATE_90:
      transform.Translate(host_->GetBounds().width() / scale, 0);
      transform.Rotate(90);
      break;
    case gfx::Display::ROTATE_270:
      transform.Translate(0, host_->GetBounds().height() / scale);
      transform.Rotate(270);
      break;
    case gfx::Display::ROTATE_180:
      transform.Translate(host_->GetBounds().width() / scale,
                       host_->GetBounds().height() / scale);
      transform.Rotate(180);
      break;
  }
  host_->SetRootTransform(transform);
  static_cast<gfx::ScreenQt*>(gfx::Screen::GetNativeScreen())->
      SetRotation(rotation);

  input_event_transform_ = host_->GetRootTransform();
  input_event_transform_.Scale(1 / scale, 1 / scale);
}

const gfx::Transform& QSWebWindowImpl::GetInputEventTransform() const {
  return input_event_transform_;
}

void QSWebWindowImpl::Focus() {
  host_->GetInputMethod()->OnFocus();
  focus_client_->FocusWindow(host_->window());
  aura::Window* win = static_cast<QSStackLayout*>(
      host_->window()->layout_manager())->top_window();

  if (!win)
    return;

  QSWebViewImpl* wv = QSWebViewImpl::From(win);
  if (wv) {
    wv->Focus();
  }
}

void QSWebWindowImpl::Blur() {
  host_->GetInputMethod()->OnBlur();
  focus_client_->FocusWindow(nullptr);
}

void QSWebWindowImpl::SetControlsHeight(int height) {
  QSStackLayout* layout = static_cast<QSStackLayout*>(
      host_->window()->layout_manager());
  gfx::Insets insets(0, 0, height, 0);
  float dsf = gfx::Screen::GetNativeScreen()->
      GetPrimaryDisplay().device_scale_factor();
  gfx::Insets scaled_insets = insets.Scale(1 / dsf, 1 / dsf);
  layout->SetInsets(scaled_insets);
}

void QSWebWindowImpl::SetInsets(gfx::Insets& insets) {
  float dsf = gfx::Screen::GetNativeScreen()->
      GetPrimaryDisplay().device_scale_factor();
  insets_ = insets.Scale(1 / dsf, 1 / dsf);

  QSStackLayout* layout = static_cast<QSStackLayout*>(
      host_->window()->layout_manager());

  aura::Window* win = layout->top_window();

  if (!win)
    return;

  QSWebViewImpl* wv = QSWebViewImpl::From(win);
  if (wv) {
    wv->WindowInsetsChanged();
  }
}

gfx::Insets QSWebWindowImpl::GetInsets() const {
  return insets_;
}

gfx::Rect QSWebWindowImpl::GetBounds() const {
  return host_->GetBounds();
}

QWindow* QSWebWindowImpl::GetQWindow() const {
  return reinterpret_cast<QWindow*>(host_->GetAcceleratedWidget());
}

ui::InputMethodQt* QSWebWindowImpl::GetInputMethod() const {
  return static_cast<ui::InputMethodQt*>(host_->GetInputMethod());
}

void QSWebWindowImpl::OnHostCloseRequested(const aura::WindowTreeHost* host) {
  web_window_.closeRequest();
}

aura::Window* QSWebWindowImpl::GetDefaultParent(aura::Window*,
                                                aura::Window*,
                                                const gfx::Rect&) {
  return host_->window();
}

} // namespace QuickSilverImpl

