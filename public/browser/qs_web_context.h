// Copyright (c) 2015 Piotr Tworek. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef _QUICKSILVER_PUBLIC_BROWSER_QS_WEB_CONTEXT_H_
#define _QUICKSILVER_PUBLIC_BROWSER_QS_WEB_CONTEXT_H_

#include <QtCore/QObject>

namespace QuickSilverImpl {
class QSBrowserContext;
}

namespace QuickSilver {

class QSWebContext : public QObject {
  Q_OBJECT

 public:
  explicit QSWebContext(bool off_the_record);
  ~QSWebContext() override;

  QuickSilverImpl::QSBrowserContext* impl() const;

 private:
  QScopedPointer<QuickSilverImpl::QSBrowserContext> impl_;

  Q_DISABLE_COPY(QSWebContext);
};

} // namespace QuickSilver

#endif // !_QUICKSILVER_PUBLIC_BROWSER_QS_WEB_CONTEXT_H_
