// Copyright 2015 Piotr Tworek. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

import QtQuick 2.0

Rectangle {
  width: parent.width
  height: kFontSizeLarge + 2 * kPaddingMedium
  color: kTransparentBlack

  property alias text: label.text

  Text {
    id: label
    anchors.fill: parent
    horizontalAlignment: Text.AlignHCenter
    verticalAlignment: Text.AlignVCenter
    font.pixelSize: kFontSizeLarge
    color: "white"
  }
}
