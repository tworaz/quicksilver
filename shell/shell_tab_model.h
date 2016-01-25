// Copyright (c) 2015 Piotr Tworek. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef QUICKSILVER_SHELL_SHELL_TAB_MODEL_H_
#define QUICKSILVER_SHELL_SHELL_TAB_MODEL_H_

#include <QtCore/QAbstractListModel>

namespace QuickSilver {
class QSWebView;
}

class ShellTabModel : public QAbstractListModel {
  Q_OBJECT;
 public:
  explicit ShellTabModel(QObject* parent = 0);
  ~ShellTabModel() override;

  void Append(QuickSilver::QSWebView*);
  void Remove(QuickSilver::QSWebView*);
  bool Contains(QuickSilver::QSWebView* wv) const;
  void CloseAllTabs();

  // QAbstractListModel:
  QVariant data(const QModelIndex &index, int role) const override;
  int rowCount(const QModelIndex&) const override;
  QHash<int, QByteArray> roleNames() const override;

 private:
  QHash<int, QByteArray> role_names_;
  QList<QuickSilver::QSWebView*> data_;

  Q_DISABLE_COPY(ShellTabModel)
};

#endif // !QUICKSILVER_SHELL_SHELL_TAB_MODEL_H_

