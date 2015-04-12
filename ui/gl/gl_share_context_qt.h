// Copyright 2015 Piotr Tworek. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/gl/gl_context.h"

namespace gfx {

class GLShareGroup;

class GLShareContextQt : public GLContextReal {
 public:
  GLShareContextQt(GLShareGroup*);

  void* GetHandle() override;
  bool WasAllocatedUsingRobustnessExtension() override;
  bool Initialize(GLSurface*, GpuPreference) override;
  void Destroy() override;
  bool MakeCurrent(GLSurface *) override;
  void ReleaseCurrent(GLSurface *) override;
  bool IsCurrent(GLSurface *) override;
  void OnSetSwapInterval(int) override;

 private:
  ~GLShareContextQt() override;

  void* handle_;
};

} // namespace gfx
