import QtQuick 6.5
import QtQuick.Controls 6.5
import QtQuick.Controls.Basic
import QtQuick.Controls.Fusion
import QtQuick.Controls.Material
import QtQuick.Controls.Universal
import SqueezeCentral2

import com.georgegalt.Slimp3CLI
import com.georgegalt.PlayerModel
import com.georgegalt.SqueezeServer

Window {
    id: mainWindow
    width: Constants.width
    height: Constants.height

    visible: true

    function launchLargePlayer( macAddress ) {
        // PlayerModel.findPlayerByMAC(macAddress)
        SqueezeServer.makePlayerCurrent(macAddress)
        largePlayerCard.playerMAC = macAddress
        // console.log("mac:", macAddress, "Name:", PlayerModel.getPlayerFromMac(macAddress))
        stackView.push(largePlayerCard)
    }

    function testFunction( mytext ) {
        console.log("TEST FUNCTION TEXT:", myText)
    }

    signal pauseButton( mac: string, status: int )
    signal forwardButton( mac: string )
    signal rewindButton( mac: string )

    signal tickTock()


    ToolBar {
        id: toolBar
        anchors.right: parent.right
        anchors.left: parent.left
        contentHeight: leftToolbarButton.implicitHeight

        ToolButton {
            id: leftToolbarButton
            text: qsTr("‹")
            anchors.left: parent.left
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            anchors.topMargin: 0
            width: height
            font.pointSize: 30
            enabled: (stackView.depth > 1) ? true : false
            onClicked: stackView.pop()
        }
        Label {
            id: toolBarText
            text: "SqueezeboxServer: Disconnected"
            elide: Label.ElideRight
            anchors.left: leftToolbarButton.right
            anchors.right: rightToolbarButton.left
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            anchors.topMargin: 0
            verticalAlignment: Qt.AlignVCenter
            font.pointSize: 30
        }
        ToolButton {
            id: rightToolbarButton
            text: qsTr("⋮")
            anchors.right: parent.right
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            anchors.topMargin: 0
            width: height
            font.pointSize: 30
            onClicked: drawer.open()
        }
    }

    Rectangle {
        anchors.top: toolBar.bottom
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        color: Material.background

        Connections {
            target: tock
            function onTimeout() {
                tickTock()
            }
        }

        StackView {
            id: stackView
            anchors.fill: parent
            // anchors.top: toolBar.bottom
            // anchors.right: parent.right
            // anchors.bottom: parent.bottom
            // anchors.left: parent.left
            initialItem: smallPlayerScrollView
            SmallPlayerScrollView {
                id: smallPlayerScrollView
                // anchors.fill: parent
                clip: true
                gridModel: PlayerModel
                // visible: false
                // onVisibleChanged:  {
                //     console.log("LargePlayercard visible", visible)
                //     visible ? parent = stackView : parent = null
                // }
            }

            Drawer {
                id: drawer
                width: mainWindow.width * 0.33
                height: mainWindow.height
                edge: Qt.RightEdge

                Column {
                    anchors.fill: parent

                    ItemDelegate {
                        text: qsTr("Page 1")
                        width: parent.width
                        onClicked: {
                            stackView.push("Screen01.ui.qml")
                            drawer.close()
                        }
                    }
                    ItemDelegate {
                        text: qsTr("Page 2")
                        width: parent.width
                        onClicked: {
                            stackView.push("Screen02.ui.qml")
                            drawer.close()
                        }
                    }
                }
            }
        }
    }

    ///////////////////////////////////////////////////////////////////////////////////////
    /////   Create parentless views for stackview

    property LargePlayerCard largePlayerCard: LargePlayerCard {
        // id: largePlayerCard
        parent: null
        // anchors.fill: visible ? parent : null
        visible: false
        onVisibleChanged:  {
            console.log("LargePlayercard visible", visible)
            visible ? parent = stackView : parent = null
        }
    }

    // property SmallPlayerScrollView smallPlayerScrollView:
    // }

}

