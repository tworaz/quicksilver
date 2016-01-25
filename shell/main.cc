// Copyright (c) 2015 Piotr Tworek. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <QtCore/QList>

#include "shell_browser_process.h"

int main(int argc, const char** argv) {
  ShellBrowserProcess proc(argc, argv);

  QList<QString> urls;
  for (int i = 1; i < argc; ++i) {
    if (argv[i][0] != '-') {
      urls.append(argv[i]);
    }
  }
  proc.SetInitialURLs(urls);
  urls.clear();

  return proc.exec();
}
