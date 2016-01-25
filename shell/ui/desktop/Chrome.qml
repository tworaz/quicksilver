// Copyright 2015 Piotr Tworek. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

import QtQuick 2.0
import QtQuick.Window 2.0
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.1
import QuickSilver 1.0
import "../common"

Window {
  id: window
  width: WebWindow.qwindow.width
  height: WebWindow.qwindow.height
  x: WebWindow.qwindow.x
  y: WebWindow.qwindow.y
  flags: Qt.FramelessWindowHint | Qt.Window | Qt.Tool
  color: "transparent"

  readonly property string kHomePage: "https://www.duckduckgo.com"
  readonly property string kSearchEngineURL: "https://duckduckgo.com/?q=%1"

  // Colors
  readonly property color kTransparentBlack: "#DD000000"
  // Padding
  readonly property int kPaddingSmall: 6
  readonly property int kPaddingMedium: 12
  readonly property int kPaddingLarge: 24
  // Fonts
  readonly property int kFontSizeTiny: 12
  readonly property int kFontSizeSmall: 16
  readonly property int kFontSizeMedium: 20
  readonly property int kFontSizeLarge: 24
  readonly property int kFontSizeHuge: 32
  // Other
  readonly property double kMinScrollDelta: navBar.height / 2
  readonly property double kOverscrollYThreshold: window.height / 4

  property QtObject gActiveTab: null

  UIState {
    id: uiState
    navigationBarHeight: navBar.height

    onOverscrollProgressChanged: {
      if (overscrollProgress < 1.0) {
        return
      }
      if (gActiveTab && overscrollEdge == "top") {
        gActiveTab.reload()
      }
    }
  }

  Binding {
    target: WebWindow.qwindow
    property: "title"
    value: gActiveTab ? gActiveTab.title : "QuickSilver Shell"
  }

  Connections {
    target: Browser
    onShowContextMenu: contextMenu.show(menuParams)
    onShowJavaScriptDialog: jsDialog.show(dialogDescription)
    onCancelAllJavaScriptDialogs: jsDialog.cancel()
    onRequestPermission: permissionDialog.show(requestDescription)
    onCancelPermissionRequest: permissionRequest.cancel(requestId)
  }

  Component.onCompleted: {
    Browser.SetChromeWindow(window)
    Browser.searchEngineURL = kSearchEngineURL
  }

  TopIndicator {
    anchors.top: parent.top
    opacity: uiState.overscrollProgress
    visible: uiState.overscrollEdge == "top"
    text: "Reload"
  }

  TabsView {
    id: tabs
    anchors.fill: parent
    z: navBar.z + 1
  }

  ContextMenu {
    id: contextMenu
    anchors.fill: parent
    z: navBar.z + 1
  }

  JavaScriptDialog {
    id: jsDialog
    anchors.fill: parent
    z: navBar.z + 1
  }

  PermissionDialog {
    id: permissionDialog
    anchors.fill: parent
    z: navBar.z + 1
  }

  Rectangle {
    anchors.top: parent.top
    height: kPaddingSmall
    color: "red"
    width: gActiveTab ? gActiveTab.loadProgress * parent.width : 0
    visible: uiState.showLoadingIndicator && !tabs.visible
  }

  QSWebArea {
    id: inputRegion
    anchors.fill: parent
    webWindow: WebWindow
    controlsHeight: uiState.controlsNearPageBottom && uiState.showNavigationBar ?
        Math.min(navBar.height, window.height - navBar.y) : 0
  }

  // Bottom navigation bar
  Rectangle {
    id: navBar
    y: uiState.showNavigationBar ? parent.height - height : parent.height
    width: parent.width
    height: backButton.height + kPaddingLarge
    color: kTransparentBlack

    Behavior on y { PropertyAnimation {} }

    RowLayout {
      anchors.fill: parent
      spacing: kPaddingMedium
      Item { width: kPaddingSmall / 2 }
      ToolButton {
        id: backButton
        visible: gActiveTab ? gActiveTab.canGoBack : false
        width: visible ? implicitWidth : 0
        iconSource: "qrc:/icons/back.png"
        onClicked: gActiveTab.back()
      }
      ToolButton {
        visible: gActiveTab ? gActiveTab.canGoForward : false
        width: visible ? implicitWidth : 0
        iconSource: "qrc:/icons/forward.png"
        onClicked: gActiveTab.forward()
      }
      ToolButton {
        enabled: gActiveTab != null
        iconSource: uiState.showLoadingIndicator ? "qrc:/icons/stop.png" : "qrc:/icons/reload.png"
        onClicked: uiState.showLoadingIndicator ? gActiveTab.stop() : gActiveTab.reload()
      }
      TextField {
        Layout.fillWidth: true
        text: gActiveTab ? gActiveTab.currentURL : ""
        horizontalAlignment: TextInput.AlightRight
        placeholderText: "http://"
        onTextChanged: {
          if (!activeFocus) {
            cursorPosition = 0
          }
        }
        onAccepted: {
          var url = Browser.ValidateURL(text);
          if (gActiveTab) {
            gActiveTab.loadURL(url)
          } else {
            Browser.CreateTab(url)
          }
          inputRegion.forceActiveFocus()
        }
      }
      ToolButton {
        iconSource: "qrc:/icons/tabs.png"
        onClicked: tabs.toggle()
      }
      Item { width: kPaddingSmall / 2 }
    } // Row Layout
  }
}
