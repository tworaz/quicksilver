// copyright (c) 2015 piotr tworek. all rights reserved.
// use of this source code is governed by a bsd-style license that can be
// found in the license file.

#ifndef QUICKSILVER_BROWSER_PUBLIC_NEW_WINDOW_REQUEST_H_
#define QUICKSILVER_BROWSER_PUBLIC_NEW_WINDOW_REQUEST_H_

#include "PermissionRequest.h"

#include <QString>

class QQuickItem;

namespace QuickSilver {

class NewWindowPermissionRequest : public PermissionRequest {
  Q_OBJECT

  Q_PROPERTY(QString url READ url CONSTANT FINAL)

 public:
  explicit NewWindowPermissionRequest(QString url = QString());
  NewWindowPermissionRequest(const NewWindowPermissionRequest&);
  ~NewWindowPermissionRequest() override;

 private:
  QString url() const;

  QString url_;
};

} // namespace QuickSilver

Q_DECLARE_METATYPE(QuickSilver::NewWindowPermissionRequest)

#endif // QUICKSILVER_BROWSER_PUBLIC_NEW_WINDOW_REQUEST_H_

