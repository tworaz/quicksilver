// Copyright 2015 Piotr Tworek. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

import QtQuick 2.0
import QtQuick.Controls 1.4

Rectangle {
  id: root
  color: kTransparentBlack
  visible: false

  function toggle() {
     root.visible = !root.visible
  }

  ListView {
    anchors.margins: kPaddingSmall
    boundsBehavior: Flickable.StopAtBounds
    spacing: kPaddingSmall

    anchors.fill: parent
    model: TabModel

    header: Column {
      width: parent.width
      Rectangle {
        width: parent.width
        height: 60
        radius: kPaddingMedium
        color: "white"

        ToolButton {
          anchors.left: parent.left
          anchors.verticalCenter: parent.verticalCenter
          anchors.leftMargin: (parent.height - height) / 2
          iconSource: "qrc:/icons/back_black.png"
          onClicked: toggle()
        }

        Text {
          color: "black"
          text: "Tabs"
          anchors.verticalCenter: parent.verticalCenter
          horizontalAlignment: Text.AlignHCenter
          verticalAlignment: Text.AlignVCenter
          anchors.fill: parent
          font {
            bold: true
            pixelSize: 36
          }
        }

        ToolButton {
          anchors.right: parent.right
          anchors.verticalCenter: parent.verticalCenter
          anchors.rightMargin: (parent.height - height) / 2
          iconSource: "qrc:/icons/add_black.png"
          onClicked: {
            Browser.CreateTab(kHomePage)
            toggle()
          }
        }
      }
      Item {
        width: parent.width
        height: kPaddingSmall
      }
    }

    delegate: Rectangle {
      color: currentTab.visible ? "white" : "gray"
      width: parent.width
      radius: kPaddingMedium
      height: 40

      Component.onCompleted: {
        if (currentTab.visible) {
          gActiveTab = currentTab
        }
      }

      Connections {
        target: currentTab
        onVisibleChanged: {
          if (currentTab.visible) {
            gActiveTab = currentTab
          }
        }
      }

      MouseArea {
        anchors.fill: parent
        onClicked: {
          if (!currentTab.visible) {
            currentTab.show()
          }
          toggle()
        }
      }

      Text {
        height: parent.height
        anchors.left: parent.left
        anchors.right: closeButton.left
        anchors.leftMargin: kPaddingMedium
        elide: Text.ElideRight
        text: currentTab.title
        verticalAlignment: Text.AlignVCenter
        font.pixelSize: 16
      }

      ToolButton {
        id: closeButton
        anchors.right: parent.right
        anchors.verticalCenter: parent.verticalCenter
        anchors.rightMargin: (parent.height - height) / 2
        iconSource: "qrc:/icons/close_black.png"
        onClicked: {
          if (gActiveTab === currentTab) {
            gActiveTab = null
          }
          Browser.CloseTab(currentTab)
        }
      }
    } // Rectangle
  } // ListView
}
