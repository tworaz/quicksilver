// Copyright (c) 2015 Piotr Tworek. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "quicksilver/shell/shell_tab_model.h"

#include <QtCore/QDebug>

#include "quicksilver/public/browser/qs_web_view.h"

using namespace QuickSilver;

ShellTabModel::ShellTabModel(QObject* parent)
    : QAbstractListModel(parent) {
  role_names_[Qt::UserRole] = "currentTab";
}

ShellTabModel::~ShellTabModel() {
}

void ShellTabModel::Append(QSWebView* obj) {
  beginInsertRows(QModelIndex(), data_.count(), data_.count());
  data_.append(obj);
  endInsertRows();
}

void
ShellTabModel::Remove(QSWebView* wv) {
  int idx = data_.indexOf(wv);
  Q_ASSERT(idx >= 0 && idx < data_.size());
  beginRemoveRows(QModelIndex(), idx, idx);
  data_.removeAt(idx);
  endRemoveRows();
}

bool ShellTabModel::Contains(QSWebView* wv) const {
	return data_.contains(wv);
}

void ShellTabModel::CloseAllTabs() {
  for (QSWebView* wv : data_) {
    delete wv;
  }
  data_.clear();
}

QVariant ShellTabModel::data(const QModelIndex &index, int role) const {
  if (index.row() < 0 || index.row() >= data_.size())
      return QVariant();

  QByteArray roleName = role_names_.value(role);
  if (roleName.isEmpty()) {
      qWarning() << "No item for role " << role << " found!";
      return QVariant();
  }

  return QVariant::fromValue(data_.at(index.row()));
}

int ShellTabModel::rowCount(const QModelIndex&) const {
  return data_.size();
}

QHash<int, QByteArray> ShellTabModel::roleNames() const {
  return role_names_;
}
