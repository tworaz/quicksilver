// Copyright (c) 2015 Piotr Tworek. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "qt_port/ozone/surface_factory_qt.h"

#include <QGuiApplication>
#include <QOpenGLContext>
#include <qpa/qplatformnativeinterface.h>
#include <EGL/egl.h>
#include <EGL/eglext.h>

#include "base/at_exit.h"
#include "base/command_line.h"
#include "base/files/file_path.h"
#include "base/native_library.h"
#include "ui/gl/gl_implementation.h"

Q_GUI_EXPORT QOpenGLContext *qt_gl_global_share_context();

namespace ui {

namespace {
#if defined(OS_SAILFISH)
const char kEGLLibraryName[] = "libEGL.so.1";
const char kGLESv2LibraryName[] = "libGLESv2.so.2";
#else
const char kEGLLibraryName[] = "libEGL.so";
const char kGLESv2LibraryName[] = "libGLESv2.so";
#endif

void* NativeResourceForContext(const QByteArray& resource) {
  DCHECK(qt_gl_global_share_context());
  return qApp->platformNativeInterface()->nativeResourceForContext(
      resource, qt_gl_global_share_context());
}

void* NativeResourceForIntegration(const QByteArray& resource) {
  return qApp->platformNativeInterface()->
      nativeResourceForIntegration(resource);
}

void* NativeResourceForScreen(const QByteArray& resource) {
  return qApp->platformNativeInterface()->
      nativeResourceForScreen(resource, qApp->primaryScreen());
}

base::NativeLibrary LoadLibrary(const base::FilePath& filename) {
  base::NativeLibraryLoadError error;
  base::NativeLibrary library = base::LoadNativeLibrary(filename, &error);
  if (!library) {
    LOG(ERROR) << "Failed to load " << filename.MaybeAsASCII()
               << ": " << error.ToString();
    return NULL;
  }
  return library;
}

base::NativeLibrary LoadLibrary(const char* filename) {
  return LoadLibrary(base::FilePath(filename));
}

} // namespace

SurfaceFactoryQt::SurfaceFactoryQt()
    : SurfaceFactoryOzone() {
}

SurfaceFactoryQt::~SurfaceFactoryQt() {
}

bool SurfaceFactoryQt::LoadEGLGLES2Bindings(
    AddGLLibraryCallback add_gl_library,
    SetGLGetProcAddressProcCallback set_gl_get_proc_address) {

  base::NativeLibrary egl_library = LoadLibrary(kEGLLibraryName);
  if (!egl_library)
    return false;

  base::NativeLibrary gles_library = LoadLibrary(kGLESv2LibraryName);
  if (!gles_library)
    return false;

  gfx::GLGetProcAddressProc get_proc_address =
       reinterpret_cast<gfx::GLGetProcAddressProc>(
            base::GetFunctionPointerFromNativeLibrary(
                  egl_library, "eglGetProcAddress"));
  if (!get_proc_address) {
    LOG(ERROR) << "eglGetProcAddress not found.";
    base::UnloadNativeLibrary(egl_library);
    base::UnloadNativeLibrary(gles_library);
    return false;
  }

  gfx::SetGLGetProcAddressProc(get_proc_address);
  gfx::AddGLNativeLibrary(egl_library);
  gfx::AddGLNativeLibrary(gles_library);

  return true;
}

intptr_t SurfaceFactoryQt::GetNativeDisplay() {
  static intptr_t display = 0;
  if (display)
    return display;

  void* nativedisp = NativeResourceForIntegration(
      QByteArrayLiteral("nativedisplay"));
  if (nativedisp) {
    display =  reinterpret_cast<intptr_t>(nativedisp);
    return display;
  }

  nativedisp = NativeResourceForScreen(QByteArrayLiteral("display"));
  if (nativedisp) {
    display =  reinterpret_cast<intptr_t>(nativedisp);
    return display;
  }

  NOTREACHED() << "No default display found!";
  return 0;
}

const int32* SurfaceFactoryQt::GetEGLSurfaceProperties(
    const int32* desired_list) {
  EGLint renderable_type = EGL_OPENGL_ES2_BIT;
  if (base::CommandLine::ForCurrentProcess()->HasSwitch(
          switches::kEnableUnsafeES3APIs)) {
    renderable_type = EGL_OPENGL_ES3_BIT_KHR;
  }
  static const EGLint kConfigAttribs[] = {
      EGL_BUFFER_SIZE, 32,
      EGL_ALPHA_SIZE, 8,
      EGL_BLUE_SIZE, 8,
      EGL_GREEN_SIZE, 8,
      EGL_RED_SIZE, 8,
      // According to egl spec depth size defaulted to zero and smallest size
      // preffered. Force depth to 24 bits to have same depth buffer on different
      // platforms.
      EGL_DEPTH_SIZE, 24,
      EGL_RENDERABLE_TYPE, renderable_type,
      EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
      EGL_NONE
  };
  return kConfigAttribs;
}

} // namespace ui

