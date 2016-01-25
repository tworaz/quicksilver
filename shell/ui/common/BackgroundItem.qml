// Copyright 2015 Piotr Tworek. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

import QtQuick 2.0
import QuickSilver 1.0

Rectangle {
  id: root
  color: kTransparentBlack
  anchors.centerIn: parent
  visible: false

  signal clicked()

  Connections {
    target: WebWindow
    onContentOrientationChanged: {
      switch (WebWindow.contentOrientation) {
        case Qt.PortraitOrientation:
          root.rotation = 0;
          break;
        case Qt.LandscapeOrientation:
          root.rotation = 90
          break;
        case Qt.InvertedPortraitOrientation:
          root.rotation = 180
          break;
        case Qt.InvertedLandscapeOrientation:
          root.rotation = 270
          break;
      }

      var isLandscape = (rotation === 90 || rotation === 270)
      if (isLandscape) {
        root.width = parent.height
        root.height = parent.width
      } else {
        root.width = parent.width
        root.height = parent.height
      }
    }
  }

  MouseArea {
    anchors.fill: parent
    onClicked: root.clicked()
  }
}
