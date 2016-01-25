// Copyright (c) 2015 Piotr Tworek. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef QUICKSILVER_PUBLIC_BROWSER_QS_PERMISSION_REQUEST_H_
#define QUICKSILVER_PUBLIC_BROWSER_QS_PERMISSION_REQUEST_H_

#include <QtCore/QObject>
#include <QtCore/QUrl>

namespace QuickSilver {

class QSPermissionRequest : public QObject {
  Q_OBJECT
  Q_ENUMS(Type)
  Q_PROPERTY(Type type MEMBER type_ CONSTANT FINAL)
  Q_PROPERTY(int id MEMBER id_ CONSTANT FINAL)
  Q_PROPERTY(bool userGesture MEMBER user_gesture_ CONSTANT FINAL)
  Q_PROPERTY(QUrl url MEMBER url_ CONSTANT FINAL);

 public:
  enum Type {
    MidiSysex,
    PushMessaging,
    Notifications,
    Geolocation,
    ProtectedMediaIdentifier,
    Midi,
    DurableStorage,
    AudioCapture,
    VideoCapture
  };

  QSPermissionRequest(Type, int id, bool user_gesture, QUrl url);
  ~QSPermissionRequest() override;

  int id() const { return id_; }

  Q_INVOKABLE virtual void granted(bool remember = false) = 0;
  Q_INVOKABLE virtual void denied(bool remember = false) = 0;

 protected:
  Type type_;
  int id_;
  bool user_gesture_;
  QUrl url_;
};

} // namespace QuickSilver

#endif // QUICKSILVER_PUBLIC_BROWSER_QS_PERMISSION_REQUEST_H_
