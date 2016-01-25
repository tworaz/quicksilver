// Copyright 2015 Piotr Tworek. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

import QtQuick 2.0
import QuickSilver 1.0

BackgroundItem {
  id: root

  QtObject {
    id: priv
    property QtObject requestData
  }

  function show(request) {
    priv.requestData = request
    switch (request.type) {
      case QSPermissionRequest.Geolocation:
        dialogLabel.text = request.url + " wants to know your location."
        break;
      default:
        console.warn("Unhandled permission request type: " + request.type);
        visible = false;
        request.denied();
        return;
    }

    forceActiveFocus()
    visible = true
  }

  function cancel(request_id) {
    console.assert(priv.requestData && priv.requestData.id === requestId);
    priv.requestData = null
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

    Column {
      id: column
      anchors.fill: parent
      anchors.margins: kPaddingMedium
      spacing: kPaddingMedium

      Text {
        id: dialogLabel
        width: parent.width
        font.pixelSize: kFontSizeLarge
        wrapMode: Text.WordWrap
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        text: "Dialog text"
      }

      Rectangle {
        color: "black"
        height: 1
        width: parent.width
      }

      Row {
        width: parent.width
        spacing: kPaddingMedium
        property int buttonCount: okButton.visible + cancelButton.visible
        property int buttonWidth: buttonCount > 1 ?
            (width - spacing) / buttonCount : width

        DialogButton {
          id: okButton
          width: parent.buttonWidth
          text: "Allow"
          onTriggered: {
            priv.requestData.granted(true)
            root.visible = false
            priv.requestData = null
          }
        }
        DialogButton {
          id: cancelButton
          width: parent.buttonWidth
          text: "Deny"
          onTriggered: {
            priv.requestData.denied(true)
            root.visible = false
            priv.requestData = null
          }
        }
      }

    } // Column
  } // Rectangle
}
