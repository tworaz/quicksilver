// Copyright 2015 Piotr Tworek. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/gl/gl_share_context_qt.h"

#include <qpa/qplatformnativeinterface.h>
#include <QGuiApplication>
#include <QString>

#include "ui/gl/gl_implementation.h"
#include "ui/gl/gpu_timing.h"

Q_GUI_EXPORT QOpenGLContext *qt_gl_global_share_context();

namespace gfx {

GLShareContextQt::GLShareContextQt(GLShareGroup* share_group)
    : GLContextReal(share_group),
      handle_(0) {
  QString platform = qApp->platformName().toLower();
  QByteArray resource;
  if (platform == QLatin1String("xcb")) {
    if (GetGLImplementation() == kGLImplementationEGLGLES2) {
      resource = QByteArrayLiteral("eglcontext");
    } else {
      resource = QByteArrayLiteral("glxcontext");
    }
  } else if (platform == QLatin1String("wayland") ||
             platform == QLatin1String("wayland-egl")) {
    resource = QByteArrayLiteral("eglcontext");
  } else {
    CHECK(false) << "Platform not supported: " << qPrintable(platform);
  }

  DVLOG(1) << "Platform: " << qPrintable(platform)
           << ", GL implementation: "
           << GetGLImplementationName(GetGLImplementation())
           << ", QPA native resource type: " << qPrintable(resource);

  QOpenGLContext *qtShareContext = qt_gl_global_share_context();
  CHECK(qtShareContext) << "Qt global share context not initialized!";

  QPlatformNativeInterface* pni = QGuiApplication::platformNativeInterface();
  handle_ = pni->nativeResourceForContext(resource, qtShareContext);
}

GLShareContextQt::~GLShareContextQt() {
}

void* GLShareContextQt::GetHandle() {
  return handle_;
}

bool GLShareContextQt::WasAllocatedUsingRobustnessExtension() {
  return false;
}

bool GLShareContextQt::Initialize(GLSurface*, GpuPreference) {
  NOTREACHED();
  return false;
}

void GLShareContextQt::Destroy() {
  NOTREACHED();
}

bool GLShareContextQt::MakeCurrent(GLSurface*) {
  NOTREACHED();
  return false;
}

void GLShareContextQt::ReleaseCurrent(GLSurface*) {
  NOTREACHED();
}

bool GLShareContextQt::IsCurrent(GLSurface*) {
  NOTREACHED();
  return false;
}

void GLShareContextQt::OnSetSwapInterval(int) {
  NOTREACHED();
}

} // namespace gfx
