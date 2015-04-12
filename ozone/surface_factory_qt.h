// Copyright (c) 2015 Piotr Tworek. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef QT_PORT_OZONE_SURFACE_FACTORY_QT
#define QT_PORT_OZONE_SURFACE_FACTORY_QT

#include "ui/ozone/public/surface_factory_ozone.h"

namespace ui {

class SurfaceFactoryQt : public ui::SurfaceFactoryOzone {
 public:
  SurfaceFactoryQt();
  ~SurfaceFactoryQt() override;

  bool LoadEGLGLES2Bindings(
      AddGLLibraryCallback add_gl_library,
      SetGLGetProcAddressProcCallback set_gl_get_proc_address) override;
  intptr_t GetNativeDisplay() override;
  const int32* GetEGLSurfaceProperties(const int32* desired_list) override;
};

} // namespace ui

#endif // QT_PORT_OZONE_SURFACE_FACTORY_QT
