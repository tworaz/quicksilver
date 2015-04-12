// Copyright 2015 Piotr Tworek. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef QT_PORT_OZONE_OZONE_PLATFORM_QT_H_
#define QT_PORT_OZONE_OZONE_PLATFORM_QT_H_

#include "ui/ozone/public/ozone_platform.h"

#include "base/memory/scoped_ptr.h"

namespace ui {

//class GpuPlatformSupportQt;
//class SurfaceFactoryQt;

class OzonePlatformQt : public OzonePlatform {
 public:
  OzonePlatformQt();
  ~OzonePlatformQt() override;

  SurfaceFactoryOzone* GetSurfaceFactoryOzone() final;
  CursorFactoryOzone* GetCursorFactoryOzone() final;
  InputController* GetInputController() final;
  GpuPlatformSupport* GetGpuPlatformSupport() final;
  GpuPlatformSupportHost* GetGpuPlatformSupportHost() final;
  scoped_ptr<SystemInputInjector> CreateSystemInputInjector() final;
  scoped_ptr<PlatformWindow> CreatePlatformWindow(
      PlatformWindowDelegate* delegate,
      const gfx::Rect& bounds) final;
  scoped_ptr<ui::NativeDisplayDelegate>
      CreateNativeDisplayDelegate() final;

 private:
  void InitializeUI() final;
  void InitializeGPU() final;

  scoped_ptr<SurfaceFactoryOzone> surface_factory_;
  scoped_ptr<GpuPlatformSupport> gpu_platform_;
  scoped_ptr<GpuPlatformSupportHost> gpu_platform_host_;

  DISALLOW_COPY_AND_ASSIGN(OzonePlatformQt);
};

}  // namespace ui

#endif // QT_PORT_OZONE_OZONE_PLATFORM_QT_H_
