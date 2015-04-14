/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
** This file is part of the QtWebEngine module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see http://www.qt.io/terms-conditions. For further
** information use the contact form at http://www.qt.io/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 3 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPLv3 included in the
** packaging of this file. Please review the following information to
** ensure the GNU Lesser General Public License version 3 requirements
** will be met: https://www.gnu.org/licenses/lgpl.html.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 2.0 or later as published by the Free
** Software Foundation and appearing in the file LICENSE.GPL included in
** the packaging of this file. Please review the following information to
** ensure the GNU General Public License version 2.0 requirements will be
** met: http://www.gnu.org/licenses/gpl-2.0.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef UI_BASE_CLIPBOARD_CLIPBOARD_QT_H_
#define UI_BASE_CLIPBOARD_CLIPBOARD_QT_H_

#include "ui/base/clipboard/clipboard.h"

#include <QClipboard>
#include <QMap>
#include <QObject>

namespace ui {

class ClipboardChangeObserver : public QObject {
  Q_OBJECT
 public:
  ClipboardChangeObserver();
  ~ClipboardChangeObserver() override;
  quint64 getSequenceNumber(QClipboard::Mode mode) {
    return sequenceNumber.value(mode);
  }

 private Q_SLOTS:
  void trackChange(QClipboard::Mode mode);

 private:
  QMap<QClipboard::Mode, quint64> sequenceNumber;
};

class ClipboardQt : public Clipboard {
 public:
  ClipboardQt();
  ~ClipboardQt() override;

  uint64 GetSequenceNumber(ClipboardType type) const override;
  bool IsFormatAvailable(const FormatType& format,
                         ClipboardType type) const override;
  void Clear(ClipboardType type) override;
  void ReadAvailableTypes(ClipboardType type, std::vector<base::string16>*
                          types, bool* contains_filenames) const override;
  void ReadText(ClipboardType type, base::string16* result) const override;
  void ReadAsciiText(ClipboardType type, std::string* result) const override;
  void ReadHTML(ClipboardType type,
                      base::string16* markup,
                      std::string* src_url,
                      uint32* fragment_start,
                      uint32* fragment_end) const override;
  void ReadRTF(ClipboardType type, std::string* result) const override;
  SkBitmap ReadImage(ClipboardType type) const override;
  void ReadCustomData(ClipboardType clipboard_type,
                      const base::string16& type,
                      base::string16* result) const override;
  void ReadBookmark(base::string16* title, std::string* url) const override;
  void ReadData(const FormatType& format, std::string* result) const override;

 protected:
  void WriteObjects(ClipboardType type, const ObjectMap& objects) override;
  void WriteText(const char* text_data, size_t text_len) override;
  void WriteHTML(const char* markup_data, size_t markup_len,
                 const char* url_data, size_t url_len) override;
  void WriteRTF(const char* rtf_data, size_t data_len) override;
  void WriteBookmark(const char* title_data, size_t title_len,
                     const char* url_data, size_t url_len) override;
  void WriteWebSmartPaste() override;
  void WriteBitmap(const SkBitmap& bitmap) override;
  void WriteData(const FormatType& format, const char* data_data,
                 size_t data_len) override;

  DISALLOW_COPY_AND_ASSIGN(ClipboardQt);
};

} // namespace ui

#endif // UI_BASE_CLIPBOARD_CLIPBOARD_QT_H_
