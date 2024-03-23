

/*
This is a UI file (.ui.qml) that is intended to be edited in Qt Design Studio only.
It is supposed to be strictly declarative and only uses a subset of QML. If you edit
this file manually, you might introduce QML code that is not supported by Qt Design Studio.
Check out https://doc.qt.io/qtcreator/creator-quick-ui-forms.html for details on .ui.qml files.
*/
import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Basic
import QtQuick.Controls.Fusion
import QtQuick.Controls.Material
import QtQuick.Controls.Universal

import SqueezeCentral2 1.0

Rectangle {
    id: largePlayerControls

    property int smallHeight: Math.round(height * 0.1)
    property int largeHeight: Math.round(height * 0.3)

    property alias currentPlayTimeText: currentTimeText.text
    property alias durationTextVal: durationText.text
    property alias artistTextVal: artistLabel.text
    property alias albumTextVal: albumLabel.text
    property alias sontTitleText: titleLabel.text
    property alias volumeVal: volumeSlider.value

    Rectangle {
        id: progressRect
        height: smallHeight
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.top

        ProgressBar {
            id: progressBar
            height: 10
            opacity: 0.9
            value: 0.5
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.top: parent.top
            anchors.leftMargin: 0
            anchors.topMargin: 0
        }

        Text {
            id: currentTimeText
            text: qsTr("00:00")
            anchors.left: parent.left
            anchors.top: progressBar.bottom
            width: parent.width / 2
            anchors.leftMargin: 0
            font.pixelSize: 12
        }

        Text {
            id: durationText
            text: qsTr("05:00")
            anchors.left: currentTimeText.right
            anchors.right: parent.right
            anchors.top: progressBar.bottom
            font.pixelSize: 12
            horizontalAlignment: Text.AlignRight
        }
    }

    Rectangle {
        id: infoRect
        height: largeHeight
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: progressRect.bottom

        Text {
            id: artistLabel
            text: qsTr("Text")
            anchors.left: parent.left
            anchors.top: parent.top
            width: parent.width
            font: Constants.largeFont
            // font.pixelSize: 12
        }

        Text {
            id: albumLabel
            anchors.left: parent.left
            anchors.top: artistLabel.bottom
            width: parent.width
            text: qsTr("Text")
            font: Constants.largeFont
            // font.pixelSize: 12
        }

        Text {
            id: titleLabel
            width: parent.width
            text: qsTr("Text")
            anchors.left: parent.left
            anchors.top: albumLabel.bottom
            font: Constants.largeFont
            // font.pixelSize: 12
        }
    }

    Rectangle {
        id: controls
        anchors.top: infoRect.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        height: largeHeight

        ImageButton {
            id: backButton
            width: Math.round(parent.width / 3)
            height: 100
            anchors.left: parent.left
            imageHeight: 100
            imageWidth: 100
            imageSource: "qrc:/content/Icons/new_reverse.png"
        }

        ImageButton {
            id: playButton
            width: Math.round(parent.width / 3)
            height: 100
            anchors.horizontalCenter: parent.horizontalCenter
            imageWidth: 100
            imageHeight: 100
            imageSource: "qrc:/content/Icons/pause.png"
        }

        ImageButton {
            id: forwardButton
            width: Math.round(parent.width / 3)
            height: 100
            anchors.right: parent.right
            imageWidth: 100
            imageHeight: 100
            imageSource: "qrc:/content/Icons/new_forward.png"
        }
    }

    ThreeStateButton {
        id: repeatButton
        state0Source: "qrc:/content/Icons/noloop.png"
        state1Source: "qrc:/content/Icons/loop_song.png"
        state2Source: "qrc:/content/Icons/loop_playlist.png"
        width: 100
        height: 100
        anchors.left: parent.left
        anchors.top: controls.bottom
    }

    Slider {
        id: volumeSlider
        value: 0.5
        anchors.left: repeatButton.right
        anchors.right: shuffleButton.left
        // anchors.left: parent.left
        // anchors.right: parent.right
        anchors.top: controls.bottom
    }

    ThreeStateButton {
        id: shuffleButton
        state0Source: "qrc:/content/Icons/noshuffle_new.png"
        state1Source: "qrc:/content/Icons/shuffle_new.png"
        state2Source: "qrc:/content/Icons/shuffle_by_album.png"
        width: 100
        height: 100
        anchors.right: parent.right
        anchors.top: controls.bottom
    }
}
