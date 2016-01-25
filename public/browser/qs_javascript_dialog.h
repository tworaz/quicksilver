// Copyright (c) 2015 Piotr Tworek. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef QUICKSILVER_PUBLIC_BROWSER_QS_JAVASCRIPT_DIALOG_H_
#define QUICKSILVER_PUBLIC_BROWSER_QS_JAVASCRIPT_DIALOG_H_

#include <QtCore/QObject>
#include <QtCore/QUrl>

namespace QuickSilver {

class QSJavaScriptDialog : public QObject {
  Q_OBJECT
  Q_ENUMS(Type)
  Q_PROPERTY(Type type MEMBER type_ CONSTANT FINAL)
  Q_PROPERTY(QUrl originUrl MEMBER origin_url_ CONSTANT FINAL)
  Q_PROPERTY(QString messageText MEMBER message_text_ CONSTANT FINAL)
  Q_PROPERTY(QString promptText MEMBER prompt_text_ CONSTANT FINAL)

 public:
  enum Type {
    Alert,
    Confirm,
    Prompt,
  };

  QSJavaScriptDialog(Type type, QUrl url, QString msg, QString prompt);
  ~QSJavaScriptDialog() override;

  Q_INVOKABLE virtual void accept(QString userInput = QString()) = 0;
  Q_INVOKABLE virtual void reject() = 0;

 private:
  Type type_;
  QUrl origin_url_;
  QString message_text_;
  QString prompt_text_;
};

} // namespace QuickSilver

#endif // QUICKSILVER_PUBLIC_BROWSER_QS_JAVASCRIPT_DIALOG_H_
