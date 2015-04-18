// Copyright 2015 Piotr Tworek. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

import QtQuick 2.4
import QuickSilver 1.0
import QtQuick.Window 2.2
import QtQuick.Layouts 1.1
import QtQuick.Controls 1.3
import QtQuick.Controls.Styles 1.3

ApplicationWindow {
  visible: true
  width: 1280
  height: 720
  title: activeWebView.title

  readonly property string kHomePage: "https://www.google.com"
  property Item activeWebView: tabs.currentIndex < tabs.count ?
      tabs.getTab(tabs.currentIndex).item : null

  toolBar: ToolBar {
    RowLayout {
      anchors.fill: parent
      ToolButton {
        iconSource: "qrc:/icons/window-new.png"
        onClicked: tabs.createTab(kHomePage)
      }
      ToolButton {
        enabled: activeWebView.canGoBack
        iconSource: "qrc:/icons/back.png"
        onClicked: activeWebView.back()
      }
      ToolButton {
        enabled: activeWebView.canGoForward
        iconSource: "qrc:/icons/forward.png"
        onClicked: activeWebView.forward()
      }
      ToolButton {
        iconSource: activeWebView.loading ?
            "qrc:/icons/stop.png" :
            "qrc:/icons/reload.png";
        onClicked: activeWebView.loading ?
            activeWebView.stop() :
            activeWebView.reload(false)
      }
      TextField {
        Layout.fillWidth: true
        text: activeWebView.currentURL
        onAccepted: activeWebView.loadURL(text)
      }
    }
  }

  TabView {
    id: tabs
    anchors.fill: parent
    frameVisible: false
    tabsVisible: count > 1

    Component.onCompleted: {
      if (InitialURL) {
        createTab(InitialURL)
      } else {
        createTab(kHomePage)
      }
    }

    function createTab(url) {
      var tab = addTab("", tabWebViewComponent)
      tab.active = true
      if (url)
        tab.item.loadURL(url)
      tabs.currentIndex = tabs.count - 1
      return tab
    }

    Component {
      id: tabWebViewComponent

      QuickSilverWebView {
        Component.onCompleted: {
          parent.title = Qt.binding(function() { return title })
        }

        onNewWindowPermissionRequest: {
          console.log("Allowing new window request for: " + request.url)
          request.accept()
        }

        onCreateNewWindow: tabs.createTab()
      }
    }

    style: TabViewStyle {
      frameOverlap: 1
      tab: Rectangle {
        color: styleData.selected ? "steelblue" : "lightsteelblue"
        border.color:  "steelblue"
        implicitWidth: Math.max(text.width + button.width + 16, 80)
        implicitHeight: 30
        radius: 4
        Row {
          anchors.centerIn: parent
          spacing: 8
          Text {
            id: text
            anchors.verticalCenter: parent.verticalCenter
            text: styleData.title
            verticalAlignment: Text.AlignVCenter
            color: styleData.selected ? "white" : "black"
          }
          ToolButton {
            id: button
            iconSource: "qrc:/icons/window-close.png"
            onClicked: tabs.removeTab(styleData.index)
          }
        }
      }
      frame: Rectangle { color: "steelblue" }
    }
  } // TabView

  statusBar: StatusBar {
    ProgressBar {
      anchors.fill: parent
      visible: activeWebView.loading
      minimumValue: 0
      maximumValue: 1
      value: activeWebView.loadProgress
    }
  }
}

