// Copyright 2015 Piotr Tworek. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "qt_port/ozone/ozone_platform_qt.h"

#include "ui/display/types/native_display_delegate.h"
#include "ui/ozone/public/gpu_platform_support.h"
#include "ui/ozone/public/gpu_platform_support_host.h"
#include "ui/ozone/public/system_input_injector.h"
#include "ui/platform_window/platform_window.h"
#include "qt_port/ozone/surface_factory_qt.h"

namespace ui {

OzonePlatformQt::OzonePlatformQt()
    : OzonePlatform() {
  surface_factory_.reset(new SurfaceFactoryQt);
}

OzonePlatformQt::~OzonePlatformQt() {
}

SurfaceFactoryOzone* OzonePlatformQt::GetSurfaceFactoryOzone() {
  return surface_factory_.get();
}

CursorFactoryOzone* OzonePlatformQt::GetCursorFactoryOzone() {
  NOTIMPLEMENTED();
  return NULL;
}

InputController* OzonePlatformQt::GetInputController() {
  NOTIMPLEMENTED();
  return NULL;
}

GpuPlatformSupport* OzonePlatformQt::GetGpuPlatformSupport() {
  return gpu_platform_.get();
}

GpuPlatformSupportHost* OzonePlatformQt::GetGpuPlatformSupportHost() {
  return gpu_platform_host_.get();
}

scoped_ptr<SystemInputInjector> OzonePlatformQt::CreateSystemInputInjector() {
  NOTIMPLEMENTED();
  return NULL;
}

scoped_ptr<PlatformWindow> OzonePlatformQt::CreatePlatformWindow(
    PlatformWindowDelegate* delegate,
    const gfx::Rect& bounds) {
  NOTIMPLEMENTED();
  return NULL;
}

scoped_ptr<ui::NativeDisplayDelegate> OzonePlatformQt::CreateNativeDisplayDelegate() {
  NOTIMPLEMENTED();
  return NULL;
}

void OzonePlatformQt::InitializeUI() {
  gpu_platform_host_.reset(CreateStubGpuPlatformSupportHost());
}

void OzonePlatformQt::InitializeGPU() {
  gpu_platform_.reset(CreateStubGpuPlatformSupport());
}

OzonePlatform* CreateOzonePlatformQt() {
  return new OzonePlatformQt;
}

} // namespace ui
