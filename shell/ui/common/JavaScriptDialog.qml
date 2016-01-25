// Copyright 2015 Piotr Tworek. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

import QtQuick 2.0
import QuickSilver 1.0

BackgroundItem {
  id: root

  QtObject {
    id: priv
    property QtObject dialogData
  }

  function show(dialog) {
    priv.dialogData = dialog
    switch (dialog.type) {
      case QSJavaScriptDialog.Alert:
        dialogLabel.text = dialog.messageText
        dialogPrompt.text = ""
        dialogPrompt.visible = false
        okButton.visible = true
        cancelButton.visible = false
        root.visible = true
        okButton.forceActiveFocus()
        break;
      case QSJavaScriptDialog.Confirm:
        dialogLabel.text = dialog.messageText
        dialogPrompt.text = ""
        dialogPrompt.visible = false
        okButton.visible = true
        cancelButton.visible = true
        root.visible = true
        okButton.forceActiveFocus()
        break;
      case QSJavaScriptDialog.Prompt:
        dialogLabel.text = dialog.messageText
        dialogPrompt.text = dialog.promptText
        dialogPrompt.visible = true
        okButton.visible = true
        cancelButton.visible = true
        root.visible = true
        dialogPrompt.cursorVisible = true
        dialogPrompt.forceActiveFocus()
        break;
      default:
        console.warn("Unhandled JS dialog type: " + dialog.type)
        priv.dialogData = null
        dialog.reject()
        break;
    }
  }

  function cancel() {
    visible = false
    priv.dialogData = null
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
      }

      TextInput {
        id: dialogPrompt
        width: parent.width
        height: visible ? implicitHeight : 0
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        font.pixelSize: kFontSizeMedium
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
          text: "OK"
          onTriggered: {
            priv.dialogData.accept(dialogPrompt.text)
            root.visible = false
            priv.dialogData = null
          }
        }
        DialogButton {
          id: cancelButton
          width: parent.buttonWidth
          text: "Cancel"
          onTriggered: {
            priv.dialogData.reject()
            root.visible = false
            priv.dialogData = null
          }
        }
      }

    } // Column
  } // Rectangle
}

