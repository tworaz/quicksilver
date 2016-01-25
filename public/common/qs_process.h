// Copyright (c) 2015 Piotr Tworek. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef _QUICKSILVER_PUBLIC_COMMON_QS_PROCESS_H_
#define _QUICKSILVER_PUBLIC_COMMON_QS_PROCESS_H_

#include <QtCore/QObject>
#include <QtCore/QString>

namespace QuickSilver {

class QSProcess : public QObject {
 public:
  QSProcess(int argc, const char** argv);
  ~QSProcess() override;

  static QSProcess* instance();
  int exec();
  void exit();

  void appendSwitch(QString switch_string) const;
  void appendSwitch(QString switch_string, QString switch_value) const;
  bool hasSwitch(const char switch_constant[]) const;

 private:
  int argc_;
  const char** argv_;
};

} // namespace QuickSilver

#endif // !_QUICKSILVER_PUBLIC_COMMON_QS_PROCESS_H_
