import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import SqueezeCentral2 1.0
import QtQuick.Controls.Basic
import QtQuick.Controls.Fusion
import QtQuick.Controls.Material
import QtQuick.Controls.Universal

Rectangle {
    id: buttonID

    property alias imageWidth: buttonImage.width
    property alias imageHeight: buttonImage.height
    property alias imageSource: buttonImage.source
    property alias imageOpacity: buttonImage.opacity

    state: "released"
    // color: Constants.backgroundColor
    color: Material.primary

    Image {
        id: buttonImage
        anchors.verticalCenter: parent.verticalCenter
        anchors.horizontalCenter: parent.horizontalCenter
        fillMode: Image.PreserveAspectFit
        opacity: 1.0
    }

    MouseArea {
        id: buttonMouse
        anchors.fill: parent
        onPressed: buttonID.state = "pressed"
        onReleased: buttonID.state = "released"
    }

    states: [
        State {
            name: "pressed"
            PropertyChanges {
                target: buttonImage
                opacity: 0.5
            }
            PropertyChanges {
                target: buttonID
                color: Constants.buttonDepressed
            }
        },
        State {
            name: "released"
            PropertyChanges {
                target: buttonImage
                opacity: 1.0
            }
            PropertyChanges {
                target: buttonID
                color: Constants.buttonColor
            }
        }
    ]
}
