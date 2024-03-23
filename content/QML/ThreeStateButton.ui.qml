import QtQuick 6.5
import QtQuick.Controls 6.5
import QtQuick.Controls.Basic
import QtQuick.Controls.Fusion
import QtQuick.Controls.Material
import QtQuick.Controls.Universal
import SqueezeCentral2

// Rectangle {
//     id: threeStateButton

//     property string state0Source: ""
//     property string state1Source: ""
//     property string state2Source: ""
//     property int stateValue: 0
//     state: "State0"

//     Image {
//         id: stateImage
//         anchors.fill: parent
//         source: state0Source
//         fillMode: Image.PreserveAspectFit
//         MouseArea {
//             id: mouseArea
//             anchors.fill: parent
//             onClicked: threeStateButton.stateValue = (threeStateButton.stateValue + 1) % 3
//             // onClicked: threeStateButton.opacity = 0.5
//             // Connections {
//             //     target: mouseArea
//             //     onClicked: {
//             //         threeStateButton.stateValue = (threeStateButton.stateValue + 1) % 3
//             //         console.log("state is:",threeStateButton.stateValue = (threeStateButton.stateValue + 1) % 3)
//             //     }
//             // }
//         }
//     }

//     states: [
//         State {
//             name: "State0"
//             when: stateValue === 0
//             changes: stateImage.source = state0Source
//         },
//         State {
//             name: "State1"
//             when: stateValue === 1
//             changes: stateImage.source = state1Source
//         },
//         State {
//             name: "State2"
//             when: stateValue === 2
//             changes: stateImage.source = state2Source
//         }
//     ]
// }
Rectangle {
    id: button1
    // state: "off"
    // color: "red"

    property string state0Source: ""
    property string state1Source: ""
    property string state2Source: ""
    property int stateValue: 0
    state: "one"

    Image {
        id: stateImage
        anchors.fill: parent
        source: state0Source
        fillMode: Image.PreserveAspectFit
    }

    MouseArea {
        id: button1area
        anchors.fill: parent
        onReleased: parent.state === "one" ? parent.state = "two" : (parent.state === "two") ? parent.state = "three" : parent.state = "one"
    }
    states: [
        State {
            name: "one"
            PropertyChanges { target: stateImage; source: state0Source }
        },
        State {
            name: "two"
            PropertyChanges { target: stateImage; source: state1Source }
        },
        State {
            name: "three"
            PropertyChanges { target: stateImage; source: state2Source }
        }
    ]
}
