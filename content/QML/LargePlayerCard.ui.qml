import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import QtQuick.Controls.Basic
import QtQuick.Controls.Fusion
import QtQuick.Controls.Material
import QtQuick.Controls.Universal

import SqueezeCentral2 1.0
import com.georgegalt.SqueezeServer

Pane {
    id: pane
    width: Constants.width
    height: Constants.height

    property string playerMAC: ""
    property string artistName: ""
    property string albumName: ""
    property string songTitle: ""
    property string albumArtID: ""
    property int duration: 0
    property int remainingTime: 0

    Connections {
        target: pane
        onVisibleChanged: {
            // console.log("LARGEPLAYERCARD IS VISIBLE?", pane.visible)
            if( pane.visible ) {
                largePlayerControls.artistTextVal = SqueezeServer.curPlayer.currentArtist
                largePlayerControls.albumTextVal = SqueezeServer.curPlayer.currentAlbum
                largePlayerControls.sontTitleText = SqueezeServer.curPlayer.currentTitle
                albumArtID = SqueezeServer.curPlayer.coverArtSource
                // artistName = SqueezeServer.curPlayer.currentArtist
                // albumName = SqueezeServer.curPlayer.currentAlbum
                // songTitle = SqueezeServer.curPlayer.currentTitle
                // albumArtID = SqueezeServer.curPlayer.coverArtSource
            }
        }
    }

    // onVisibleChanged: {
    //     if( pane.visible ) {
    //         artistName = SqueezeServer.curPlayer.currentArtist
    //         albumName = SqueezeServer.curPlayer.currentAlbum
    //         songTitle = SqueezeServer.curPlayer.currentTitle
    //         albumArtID = SqueezeServer.curPlayer.coverArtSource
    //     }
    // }
    Flipable {
        id: coverFlip
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.top
        height: Math.round(parent.height * 2 / 3)

        property bool flipped: false

        front: Image {
            source: (albumArtID == "") ? "http://192.168.1.50:9000/html/images/artists.png" : "http://192.168.1.50:9000/music/" + albumArtID + "/cover.jpg"
            anchors.centerIn: parent
            width: (parent.width > parent.height) ? parent.height * 0.8 : parent.width * 0.8
            height: width
        }
        back: Image {
            source: "../Icons/play.png"
            anchors.centerIn: parent
        }

        transform: Rotation {
            id: rotation
            origin.x: coverFlip.width / 2
            origin.y: coverFlip.height / 2
            axis.x: 0
            axis.y: 1
            axis.z: 0 // set axis.y to 1 to rotate around y-axis
            angle: 0 // the default angle
        }

        MouseArea {
            anchors.fill: parent
            onClicked: coverFlip.flipped = !coverFlip.flipped
        }
    }
    LargePlayerControls {
        id: largePlayerControls
        anchors.top: coverFlip.bottom
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right
    }
    transitions: Transition {
        NumberAnimation {
            target: rotation
            property: "angle"
            duration: 1000
        }
    }
    states: [
        State {
            name: "back"
            when: coverFlip.flipped
            PropertyChanges {
                target: rotation
                angle: 180
            }
        }
    ]
}
