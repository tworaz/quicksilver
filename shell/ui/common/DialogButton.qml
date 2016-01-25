// Copyright 2015 Piotr Tworek. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

import QtQuick 2.0

Rectangle {
  id: root
  color: buttonMouseArea.pressed ? "gray" : "black"
  radius: kPaddingMedium
  height: label.font.pixelSize + 2 * kPaddingMedium

  property alias text: label.text
  signal triggered()

  Text {
    id: label
    anchors.fill: parent
    horizontalAlignment: Text.AlignHCenter
    verticalAlignment: Text.AlignVCenter
    font.pixelSize: kFontSizeMedium
    font.bold: true
    color: "white"
  }
  MouseArea {
    id: buttonMouseArea
    anchors.fill: parent
    onClicked: root.triggered()
  }
}
