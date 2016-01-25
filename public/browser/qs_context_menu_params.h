// Copyright (c) 2015 Piotr Tworek. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef QUICKSILVER_PUBLIC_BROWSER_QS_CONTEXT_MENU_PARAMS_H_
#define QUICKSILVER_PUBLIC_BROWSER_QS_CONTEXT_MENU_PARAMS_H_

#include <QtCore/QObject>
#include <QtCore/QPoint>
#include <QtCore/QUrl>

namespace content {
class ContextMenuParams;
}

namespace QuickSilver {

class QSContextMenuParams : public QObject {
  Q_OBJECT
  Q_PROPERTY(QPoint position MEMBER position_ CONSTANT FINAL)
  Q_PROPERTY(QUrl linkUrl MEMBER link_url_ CONSTANT FINAL)
  Q_PROPERTY(QUrl linkText MEMBER link_text_ CONSTANT FINAL)

public:
  QSContextMenuParams(const content::ContextMenuParams&);
  QSContextMenuParams(const QSContextMenuParams&);
  ~QSContextMenuParams() override;

private:
  QPoint position_;
  QUrl link_url_;
  QString link_text_;
};

} // namespace QuickSilver

#endif // QUICKSILVER_PUBLIC_BROWSER_QS_CONTEXT_MENU_PARAMS_H_
