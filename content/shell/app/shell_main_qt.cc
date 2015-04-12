// Copyright (c) 2015 Piotr Tworek. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <QGuiApplication>
#include <QOpenGLContext>

#include "content/public/app/content_main.h"
#include "content/shell/app/shell_main_delegate.h"

Q_GUI_EXPORT void qt_gl_set_global_share_context(QOpenGLContext*);

int main(int argc, const char** argv) {
  QGuiApplication app(argc, (char**)(argv));
  Q_INIT_RESOURCE(shell);

  QOpenGLContext context;
  context.create();
  qt_gl_set_global_share_context(&context);

  content::ShellMainDelegate delegate;
  content::ContentMainParams params(&delegate);
  params.argc = argc;
  params.argv = argv;
  return content::ContentMain(params);
}
