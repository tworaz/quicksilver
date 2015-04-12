// Copyright 2015 Piotr Tworek. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

import QtQuick 2.0
import Sailfish.Silica 1.0
import QuickSilver 1.0

ApplicationWindow {
  readonly property string kHomePage: "https://www.google.com"

  initialPage: Component {
    Page {
      SilicaFlickable {
        anchors.fill: parent

        QuickSilverWebView {
          id: webview
          anchors.top: parent.top
          anchors.bottom: addressBar.top
          width: parent.width

          Component.onCompleted: {
            if (InitialURL) {
              loadURL(InitialURL)
            } else {
              loadURL(kHomePage)
            }
          }

          onTitleChanged: {
            console.log("Title changed: " + title)
          }

          onLoadingChanged: {
            if (loading) {
              console.log("Loading started")
            } else {
              console.log("Loading finished")
            }
          }

          onLoadProgressChanged: {
            console.log("Loading progress: " + loadProgress)
          }
        } // QSWebView

        Row {
          id: addressBar
          anchors.bottom: parent.bottom
          width: parent.width

          IconButton {
            id: back
            icon.source: "image://theme/icon-m-back"
            enabled: webview.canGoBack
            onClicked: webview.back()
          }
          IconButton {
            id: forward
            icon.source: "image://theme/icon-m-forward"
            enabled: webview.canGoForward
            onClicked: webview.forward()
          }
          TextField {
            width: parent.width - back.width - forward.width
            text: webview.currentURL
            EnterKey.onClicked: {
              webview.loadURL(text)
              webview.forceActiveFocus()
            }
          }
        } // Row
      }
    }
  }

}
