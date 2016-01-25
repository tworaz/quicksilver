// Copyright 2015 Piotr Tworek. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

import QtQuick 2.0
import Sailfish.Silica 1.0
import org.nemomobile.policy 1.0
import QuickSilver 1.0
import "../common"

ApplicationWindow {
  id: window
  //_mainWindow: WebWindow.qwindow
  _backgroundVisible: false
  _clippingItem.opacity: 1.0
  cover: null
  allowedOrientations: Orientation.All

  readonly property string kHomePage: "https://www.duckduckgo.com"
  readonly property string kSearchEngineURL: "https://duckduckgo.com/?q=%1"

  // Colors
  readonly property color kTransparentBlack: "#DD000000"
  // Padding
  readonly property int kPaddingSmall: Theme.paddingSmall
  readonly property int kPaddingMedium: Theme.paddingMedium
  readonly property int kPaddingLarge: Theme.paddingLarge
  // Fonts
  readonly property int kFontSizeTiny: Theme.fontSizeTiny
  readonly property int kFontSizeSmall: Theme.fontSizeSmall
  readonly property int kFontSizeMedium: Theme.fontSizeMedium
  readonly property int kFontSizeLarge: Theme.fontSizeLarge
  readonly property int kFontSizeHuge: Theme.fontSizeHuge
  // Other
  readonly property double kMinScrollDelta: 24
  readonly property double kOverscrollYThreshold: window.height / 4

  property QtObject gActiveTab: null

  Component.onCompleted: {
    Browser.searchEngineURL = kSearchEngineURL
  }

  onWindowChanged: Browser.SetChromeWindow(window)
  onOrientationChanged: WebWindow.contentOrientation = orientation

  TabsPage {
    id: tabsView
  }

  ContextMenu {
    id: contextMenu
  }

  JavaScriptDialog {
    id: jsDialog
  }

  PermissionDialog {
    id: permissionDialog
  }

  Connections {
    target: Browser
    onShowContextMenu: contextMenu.show(menuParams)
    onShowJavaScriptDialog: jsDialog.show(dialogDescription)
    onCancelAllJavaScriptDialogs: jsDialog.cancel()
    onRequestPermission: permissionDialog.show(requestDescription)
    onCancelPermissionRequest: permissionRequest.cancel(requestId)
  }

  Permissions {
    applicationClass: "player"
    autoRelease: true
    enabled: true

    Resource {
        type: Resource.AudioPlayback
    }

    Resource {
        type: Resource.VideoPlayback
    }
  }

  initialPage: Component {
    Page {
      id: page
      allowedOrientations: Orientation.All

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

      TopIndicator {
        anchors.top: parent.top
        opacity: uiState.overscrollProgress
        visible: uiState.overscrollEdge == "top"
        text: "Reload"
      }

      Rectangle {
        anchors.top: parent.top
        height: Theme.paddingSmall
        opacity: 0.8
        color: "red"
        width: gActiveTab ? gActiveTab.loadProgress * parent.width : 0
        visible: uiState.showLoadingIndicator
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
        height: back.height// + kPaddingLarge
        color: "black"

        Behavior on y { PropertyAnimation {} }

        Row {
          anchors.fill: parent
          IconButton {
            id: back
            enabled: gActiveTab ? gActiveTab.canGoBack : false
            icon.source: "image://theme/icon-m-back"
            width: page.isPortrait ? 0 : icon.sourceSize.width
            visible: width > 0
            onClicked: gActiveTab.back()
          }
          IconButton {
            id: forward
            enabled: gActiveTab ? gActiveTab.canGoForward : false
            icon.source: "image://theme/icon-m-forward"
            width: page.isPortrait ? 0 : icon.sourceSize.width
            visible: width > 0
            onClicked: gActiveTab.forward()
          }
          IconButton {
            id: refresh
            enabled: gActiveTab != null
            icon.source: uiState.showLoadingIndicator ?
                             "image://theme/icon-m-reset" :
                             "image://theme/icon-m-refresh"
            onClicked: uiState.showLoadingIndicator ?
                             gActiveTab.stop() :
                             gActiveTab.reload()
          }
          TextField {
            id: urlEntry
            width: parent.width - back.width - forward.width - refresh.width - tabs.width
            text: gActiveTab ? gActiveTab.currentURL : "about:blank"
            onClicked: {
              if (selectedText.length === 0) {
                selectAll()
              }
            }
            EnterKey.onClicked: {
              var url = Browser.ValidateURL(text);
              if (gActiveTab) {
                gActiveTab.loadURL(url)
              } else {
                Browser.CreateTab(url)
              }
              inputRegion.forceActiveFocus()
            }
          }
          IconButton {
            id: tabs
            icon.source: "image://theme/icon-m-menu"
            onClicked: pageStack.push(tabsView)
          }
        } // Row
      } // Rectangle
    } // Page
  } // Component
}
