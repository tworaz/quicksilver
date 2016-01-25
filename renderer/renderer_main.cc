// Copyright (c) 2015 Piotr Tworek. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <qs_process.h> // XXX: <QSProcess>

int main(int argc, const char* argv[]) {
  QuickSilver::QSProcess proc(argc, argv);
  return proc.exec();
}

