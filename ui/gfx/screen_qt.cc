// Copyright 2015 Piotr Tworek. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/gfx/screen.h"

#include <QGuiApplication>
#include <QScreen>

#include "base/logging.h"
#include "ui/gfx/display.h"
#include "ui/gfx/geometry/size_conversions.h"
#include "ui/qt/type_conversion.h"

namespace gfx {

class ScreenQt : public Screen {
 public:
  ScreenQt() {}

  gfx::Point GetCursorScreenPoint() override { return gfx::Point(); }

  gfx::NativeWindow GetWindowUnderCursor() override {
    NOTIMPLEMENTED();
    return NULL;
  }

  gfx::NativeWindow GetWindowAtScreenPoint(const gfx::Point& point) override {
    NOTIMPLEMENTED();
    return NULL;
  }

  gfx::Display GetPrimaryDisplay() const override {
    QGuiApplication* app = static_cast<QGuiApplication*>(
        QGuiApplication::instance());
    const QScreen* screen = app->primaryScreen();
    gfx::Display display(0);

    const float device_scale_factor =
        gfx::Display::HasForceDeviceScaleFactor() ?
        gfx::Display::GetForcedDeviceScaleFactor() : screen->devicePixelRatio();

    display.SetRotationAsDegree(0); // TODO: Get this from screen
    gfx::Rect bounds_in_pixels = toGfx(screen->availableGeometry());
    display.SetScaleAndBounds(device_scale_factor, bounds_in_pixels);

    return display;
  }

  gfx::Display GetDisplayNearestWindow(gfx::NativeView view) const override {
    return GetPrimaryDisplay();
  }

  gfx::Display GetDisplayNearestPoint(const gfx::Point& point) const override {
    return GetPrimaryDisplay();
  }

  int GetNumDisplays() const override { return 1; }

  std::vector<gfx::Display> GetAllDisplays() const override {
    return std::vector<gfx::Display>(1, GetPrimaryDisplay());
  }

  gfx::Display GetDisplayMatching(const gfx::Rect& match_rect) const override {
    return GetPrimaryDisplay();
  }

  void AddObserver(DisplayObserver* observer) override {
    NOTIMPLEMENTED();
  }

  void RemoveObserver(DisplayObserver* observer) override {
    NOTIMPLEMENTED();
  }

 private:
  DISALLOW_COPY_AND_ASSIGN(ScreenQt);
};

Screen* CreateNativeScreen() {
  return new ScreenQt;
}

}  // namespace gfx
