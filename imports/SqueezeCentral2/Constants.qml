pragma Singleton
import QtQuick 6.5
import QtQuick.Studio.Application
import QtQuick.Controls.Basic
import QtQuick.Controls.Fusion
import QtQuick.Controls.Material
import QtQuick.Controls.Universal

QtObject {
    readonly property int width: 1080
    readonly property int height: 1920

    property string relativeFontDirectory: "fonts"

    /* Edit this comment to add your custom font */
    readonly property font font: Qt.font({
                                             family: Qt.application.font.family,
                                             pixelSize: Qt.application.font.pixelSize * 2
                                         })
    readonly property font largeFont: Qt.font({
                                                  family: Qt.application.font.family,
                                                  pixelSize: Qt.application.font.pixelSize * 4
                                              })

    readonly property color backgroundColor: Material.color(Material.background)
    readonly property color accent: Material.color(Material.accent)
    readonly property color primary: Material.color(Material.Indigo)
    readonly property color controlDefault: Material.color(Material.Teal)
    readonly property color buttonColor: Material.color(Material.Grey)
    readonly property color buttonDepressed: Material.color(Material.BlueGrey)
    readonly property color cardBackground: Material.color(Material.Cyan)
    /*
    readonly property color
    readonly property color
    readonly property color
    readonly property color
    readonly property color
    readonly property color
    readonly property color
    readonly property color
    readonly property color
    readonly property color
    readonly property color
    readonly property color
    readonly property color
    readonly property color
    readonly property color
    readonly property color
    readonly property color
    readonly property color
    readonly property color
    readonly property color
    readonly property color
    readonly property color
    readonly property color
    readonly property color
    readonly property color

  */

    property StudioApplication application: StudioApplication {
        fontPath: Qt.resolvedUrl("../../content/" + relativeFontDirectory)
    }
}
