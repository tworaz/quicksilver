// copyright (c) 2015 piotr tworek. all rights reserved.
// use of this source code is governed by a bsd-style license that can be
// found in the license file.

#ifndef QUICKSILVER_BROWSER_PUBLIC_PERMISSION_REQUEST_H_
#define QUICKSILVER_BROWSER_PUBLIC_PERMISSION_REQUEST_H_

#include <QObject>

namespace QuickSilver {

class PermissionRequest : public QObject {
  Q_OBJECT

 public:
  explicit PermissionRequest(bool accepted = false);
  PermissionRequest(const PermissionRequest&);
  ~PermissionRequest() override;

  Q_INVOKABLE void accept();
  Q_INVOKABLE void deny();

  bool isAccepted() const { return accepted_; }

 protected:
  bool accepted_;
};

} // namespace QuickSilver

#endif // QUICKSILVER_BROWSER_PUBLIC_PERMISSION_REQUEST_H_
