

/*
This is a UI file (.ui.qml) that is intended to be edited in Qt Design Studio only.
It is supposed to be strictly declarative and only uses a subset of QML. If you edit
this file manually, you might introduce QML code that is not supported by Qt Design Studio.
Check out https://doc.qt.io/qtcreator/creator-quick-ui-forms.html for details on .ui.qml files.
*/
import QtQuick 6.5
import QtQuick.Controls 6.5
import QtQuick.Controls.Basic
import QtQuick.Controls.Fusion
import QtQuick.Controls.Material
import QtQuick.Controls.Universal
import SqueezeCentral2 1.0
import com.georgegalt.SqueezeServer

Rectangle {
    id: smallPlayerCard
    property alias coverImage: albumCoverImage.source
    property alias songInfoText: songInfo.scrollText
    property alias playerNameText: playerNameLbl.text
    property alias songProgress: songProgressBar.value
    property int buttonHeightMargins: 5
    property int buttonSideMargins: 10

    color: Material.background

    Connections {
        target: mainWindow
        function onTickTock() {
            songProgress = currentProgress / duration
            // albumCoverImage.source = (albumArtID == "") ? "http://192.168.1.50:9000/html/images/artists.png" : "http://192.168.1.50:9000/music/" + albumArtID + "/cover.jpg"
            // songInfo.scrollText = songTitle
        }
    }

    Connections {
        target: playerObj
        onPlaylistCurIndexChanged: {
            console.log("PLAYER INDEX CHANGED TO", currentIndex, playerObj)
            coverImage = (albumArtID == "") ? "http://192.168.1.50:9000/html/images/artists.png" : "http://192.168.1.50:9000/music/" + albumArtID + "/cover.jpg"
            songInfoText = songTitle
        }
    }

    // Connections {
    //     target: squeezeServer
    //     onSongChanged: {
    //         albumCoverImage.source = (albumArtID == "") ? "http://192.168.1.50:9000/html/images/artists.png" : "http://192.168.1.50:9000/music/" + albumArtID + "/cover.jpg"
    //         songInfo.scrollText = songTitle
    //         console.log("SONG CHANGED", songTitle, albumArtID)
    //     }
    // }


    Pane {
        anchors.fill: parent
        anchors.margins: 15
        Material.elevation: 6

        Label {
            id: playerNameLbl
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.right: parent.right
            // height: parent / 10
            text: playerName
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            font.pointSize: 20
        }

        Image {
            id: albumCoverImage
            width: parent.width / 2
            height: width
            anchors.top: playerNameLbl.bottom
            anchors.topMargin: 5
            source: (albumArtID == "") ? "http://192.168.1.50:9000/html/images/artists.png" : "http://192.168.1.50:9000/music/" + albumArtID + "/cover.jpg"
            anchors.horizontalCenter: parent.horizontalCenter
            fillMode: Image.PreserveAspectFit
            Material.elevation: 6
            MouseArea {
                id: mouseArea
                anchors.fill: parent
                Connections {
                    target: mouseArea
                    function onClicked(mouse) {
                        mainWindow.launchLargePlayer(macAddress)
                    }
                }
            }
        }

        Pane {
            id: frame
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.top: albumCoverImage.bottom
            anchors.bottom: parent.bottom
            anchors.margins: buttonSideMargins
            Material.elevation: 6

            ProgressBar {
                id: songProgressBar
                // value: currentProgress
                smooth: true
                anchors.verticalCenter: parent.verticalCenter
                anchors.horizontalCenter: parent.horizontalCenter
                width: parent.width * 4 / 5
            }

            ScrollingTextWindow {
                id: songInfo
                color: "#ffffff"
                anchors.top: parent.top
                anchors.bottom: songProgressBar.top
                anchors.horizontalCenter: parent.horizontalCenter
                scrollFontPointSize: 24

                width: parent.width * 4 / 5

                scrollText: songTitle
            }

            Rectangle {
                id: playButton
                anchors.top: songProgressBar.bottom
                anchors.bottom: parent.bottom
                anchors.topMargin: buttonHeightMargins
                anchors.bottomMargin: buttonHeightMargins
                anchors.horizontalCenter: parent.horizontalCenter
                width: height
                color: Constants.buttonColor

                Image {
                    id: playImage
                    source: pauseStatus == "pause" ? "qrc:/content/Icons/play.png" : "qrc:/content/Icons/pause.png"
                    // source: "Icons/play.png"
                    fillMode: Image.PreserveAspectFit
                    anchors.fill: parent
                    opacity: 1.0
                }
                MouseArea {
                    id: playButtonMouse
                    anchors.fill: parent
                    onPressed: playImage.opacity = 0.5
                    Connections {
                        target: playButtonMouse
                        function onReleased(mouse) {
                            console.log("Pause Status:", pauseStatus)
                            playImage.opacity = 1.0
                            if( pauseStatus == "pause" ) {
                                mainWindow.pauseButton( macAddress, 1 )
                                playImage.source = "qrc:/content/Icons/pause.png"
                            }
                            else {
                                mainWindow.pauseButton( macAddress, 0 )
                                playImage.source = "qrc:/content/Icons/play.png"
                            }
                        }
                    }
                }
            }

            Rectangle {
                id: backButton
                anchors.top: songProgressBar.bottom
                anchors.bottom: parent.bottom
                anchors.leftMargin: buttonSideMargins
                anchors.topMargin: buttonHeightMargins
                anchors.bottomMargin: buttonHeightMargins
                anchors.left: parent.left
                width: height
                color: Constants.buttonColor

                Image {
                    id: new_reverse
                    source: "qrc:/content/Icons/new_reverse.png"
                    fillMode: Image.PreserveAspectFit
                    anchors.fill: parent
                    opacity: 1.0
                }
                MouseArea {
                    id: backButtonMouse
                    anchors.fill: parent
                    onPressed: new_reverse.opacity = 0.5
                    Connections {
                        target: backButtonMouse
                        function onReleased(mouse) {
                            mainWindow.rewindButton(macAddress)
                            new_reverse.opacity = 1.0
                        }
                    }
                }
            }

            Rectangle {
                id: forwardButton
                anchors.right: parent.right
                anchors.top: songProgressBar.bottom
                anchors.bottom: parent.bottom
                anchors.rightMargin: buttonSideMargins
                anchors.topMargin: buttonHeightMargins
                anchors.bottomMargin: buttonHeightMargins
                width: height
                color: Constants.buttonColor

                Image {
                    id: new_forward
                    source: "qrc:/content/Icons/new_forward.png"
                    fillMode: Image.PreserveAspectFit
                    anchors.fill: parent
                    opacity: 1.0
                }
                MouseArea {
                    id: forwardButtonMouse
                    anchors.fill: parent
                    onPressed: new_forward.opacity = 0.5
                    Connections {
                        target: forwardButtonMouse
                        function onReleased(mouse) {
                            mainWindow.forwardButton(macAddress)
                            new_forward.opacity = 1.0
                        }
                    }
                }
            }
        }
    }
}
