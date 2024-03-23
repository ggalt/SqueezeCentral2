

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
import SqueezeCentral2

ScrollView {
    id: smallPlayerScrollView

    property alias gridModel: myGrid.model
    GridView {
        id: myGrid
        width: parent.width
        model: 5
        cellWidth: parent.width / 2
        cellHeight: parent.width / 2
        delegate: SmallPlayerCard {
            id: smallPlayerCard
            height: myGrid.cellHeight - 10
            width: myGrid.cellWidth - 10
        }

    }
}
