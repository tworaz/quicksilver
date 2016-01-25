// Copyright 2015 Piotr Tworek. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

import QtQuick 2.0
import QuickSilver 1.0

BackgroundItem {
  id: root
  onClicked: close()

  QtObject {
    id: priv
    property string linkUrl: ""
  }

  function show(params) {
    priv.linkUrl = params.linkUrl
    if (priv.linkUrl.length > 0) {
      visible = true
    }
  }

  function close() {
    visible = false
  }

  Rectangle {
    anchors.centerIn: parent
    radius: kPaddingMedium
    color: "white"
    readonly property real preferredWidth: 0.8 * parent.width
    readonly property real maxWidth: 800
    width: preferredWidth > maxWidth ? maxWidth : preferredWidth
    height: column.childrenRect.height + 2 * column.anchors.margins

    MouseArea {
      anchors.fill: parent
    }

    Column {
      id: column
      anchors.fill: parent
      anchors.margins: kPaddingMedium
      spacing: kPaddingSmall

      Text {
        width: parent.width
        font.pixelSize: kFontSizeTiny
        wrapMode: Text.WordWrap
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        text: priv.linkUrl
      }

      Rectangle {
        color: "black"
        height: 1
        width: parent.width
      }

      DialogButton {
        width: parent.width
        visible: priv.linkUrl.length > 0
        text: "Open Link in New Tab"
        onTriggered: {
          Browser.CreateTab(priv.linkUrl);
          close()
        }
      }

      DialogButton {
        width: parent.width
        visible: priv.linkUrl.length > 0
        text: "Open Link in Background Tab"
        onTriggered: {
          Browser.CreateTab(priv.linkUrl, false);
          close()
        }
      }

    } // Column
  } // Rectangle
}
