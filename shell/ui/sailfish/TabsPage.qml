// Copyright 2015 Piotr Tworek. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

import QtQuick 2.0
import Sailfish.Silica 1.0

Page {
  allowedOrientations: Orientation.All

  Rectangle {
    id: bg
    color: "black"
    anchors.fill: parent
  }

  SilicaListView {
    z: bg.z + 1
    anchors.fill: parent
    spacing : Theme.paddingMedium

    PullDownMenu {
      MenuItem {
        text: "New tab"
        onClicked: {
          Browser.CreateTab(kHomePage)
          pageStack.pop()
        }
      }
    }

    header: PageHeader {
      title: "Tabs"
    }

    model: TabModel

    delegate: Rectangle {
      width: parent.width
      color: "green"
      radius: 20
      height: content.height
      opacity: currentTab.visible ? 1.0 : 0.5

      Component.onCompleted: {
        if (currentTab.visible) {
          gActiveTab = currentTab
        }
      }

      Connections {
        target: currentTab
        onVisibleChanged: {
          if (currentTab.visible) {
            console.log("Visible tab changed to: " + currentTab.title)
            gActiveTab = currentTab
          }
        }
      }

      MouseArea {
        anchors.fill: parent
        onClicked: {
          if (!currentTab.visible)
            currentTab.show()
          pageStack.pop()
        }
      }

      Row {
        id: content
        width: parent.width
        height: title.height
        spacing: Theme.paddingMedium
        Text {
          id: title
          width: parent.width - closeButton.width - Theme.paddingMedium
          height: font.pixelSize + 2 * Theme.paddingLarge
          text: currentTab.title.length ? currentTab.title : "about:blank"
          font.pixelSize: Theme.fontSizeLarge
          elide: Text.ElideRight
          horizontalAlignment: Text.AlignHCenter
          verticalAlignment: Text.AlignVCenter
        }
        Item {
          id: closeButton
          height: parent.height
          width: parent.height
          IconButton {
            anchors.centerIn: parent
            icon.source: "image://theme/icon-m-tab-close"
            onClicked: {
              if (gActiveTab === currentTab) {
                gActiveTab = null
              }
              Browser.CloseTab(currentTab)
            }
          }
        } // Item
      } // Row

    } // Rectangle

  } // SilicaListView
}
