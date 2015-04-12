// Copyright 2015 Piotr Tworek. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

import QtQuick 2.4
import ChromiumQt 1.0
import QtQuick.Window 2.2
import QtQuick.Layouts 1.1
import QtQuick.Controls 1.3

Window {
    visible: true

    ColumnLayout {
        anchors.fill: parent
        spacing: 0

        RowLayout {
            width: parent.width
            anchors.top: parent.top
            spacing: 0
            Button {
                objectName: "newWindowButton"
                text: "New Window"
            }
            Button {
                objectName: "backButton"
                text: "Back"
            }
            Button {
                objectName: "forwardButton"
                text: "Forward"
            }
            Button {
                objectName: "refreshButton"
                text: "Refresh"
            }
            Button {
                objectName: "stopButton"
                text: "Stop"
            }
            TextField {
                objectName: "addressBar"
                Layout.fillWidth: true
            }
        }

				WebContentsView {
					  objectName: "webView"
            Layout.fillWidth: true
            Layout.fillHeight: true
				}
    }

    ProgressBar {
        objectName: "progressBar"
        visible: false
        indeterminate: true
        anchors.right: parent.right
        anchors.bottom: parent.bottom
    }
}

