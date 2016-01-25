// Copyright 2015 Piotr Tworek. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

import QtQuick 2.0

Item {
  // Should be set by the user
  property real navigationBarHeight: 0

  property real overscrollProgress: 0.0
  property string overscrollEdge: "none"
  readonly property bool showNavigationBar: priv.shouldShowNavigationBar || controlsNearPageBottom
  readonly property bool controlsNearPageBottom: priv.pixelsFromBottom <= navigationBarHeight
  readonly property bool showLoadingIndicator: gActiveTab ? gActiveTab.loading : false
  readonly property real overscrollBarOpacity: 0.0

  QtObject {
    id: priv
    readonly property bool activeTabFullscreen: (gActiveTab && gActiveTab.fullscreen)
    readonly property real overscrollY: gActiveTab ? gActiveTab.overscrollY: 0.0
    readonly property vector2d scrollOffset: gActiveTab ? gActiveTab.scrollOffset : Qt.vector2d(0, 0)
    readonly property size contentSize: gActiveTab ? gActiveTab.contentSize : Qt.size(540, 960)
    readonly property real pixelsFromBottom: priv.contentSize.height - priv.scrollOffset.y - parent.height

    property bool shouldShowNavigationBar: true
    property vector2d previousScrollOffset: Qt.vector2d(0, 0)

    onActiveTabFullscreenChanged: {
      priv.shouldShowNavigationBar = !priv.activeTabFullscreen
    }

    onOverscrollYChanged: {
      if (priv.activeTabFullscreen) {
        return;
      }

      if (overscrollY != 0) {
        priv.shouldShowNavigationBar = true
      }

      if (overscrollY > 0) {
        overscrollEdge = "top"
      } else if (overscrollY < 0) {
        overscrollEdge = "bottom"
      } else {
        overscrollEdge = "none"
      }

      var p = Math.abs(priv.overscrollY / kOverscrollYThreshold)
      overscrollProgress = p <= 1.0 ? p : 1.0
    }

    onScrollOffsetChanged: {
      var deltaY = scrollOffset.y - priv.previousScrollOffset.y
      priv.previousScrollOffset = scrollOffset

      // Don't hide the navigation bar immediately when scrolling down.
      if (deltaY > 0 && deltaY < kMinScrollDelta) {
        return;
      }
      priv.shouldShowNavigationBar = (deltaY <= 0) && !priv.activeTabFullscreen
    }
  } // QtObject
}
