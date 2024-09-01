import QtQuick 2.12
import QtQuick.Layouts 1.1

Rectangle {
    id: rect
    property string titleText : "-"
    property alias indicatorModel: repeater.model
    property alias gridRowSpacing: gridLayout.rowSpacing
    property alias gridColumnSpacing: gridLayout.columnSpacing
    property alias columns: gridLayout.columns
    property alias rows: gridLayout.rows
    property var repeaterItem: repeater
    property alias repeaterDelegate: repeater.delegate
    property int gridFlow:  GridLayout.LeftToRight

    border.width: 1
    color: "white"//"#fffaf0"
    radius: 10
    Rectangle{
        id: rectText
        gradient:Gradient {
            GradientStop {
                position: 0 ; color: "white"
            }
            GradientStop {
                position: 0.2 ; color: "white"
            }
            GradientStop {
                position: 1 ; color: "white"//"#f4fdde"
            }
        }
        x: 15
        y: - height/2
        height: 20
        width: title.text.length *12
        Text {
            anchors.fill: parent
            id: title
            text:  titleText
            font.family: "Courier"
            font.bold: true
            // fontSizeMode: Text.Fit
            font.pixelSize: 16
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
        }
    }
    Rectangle {
        id: column
        anchors {
            fill: parent
            top: rectText.bottom
            margins: 10
            topMargin: 16
        }
        GridLayout {
            id: gridLayout
            flow: gridFlow
            anchors.fill:parent
            Repeater {
                id: repeater
                //            Layout.fillHeight: true
                //           Layout.fillWidth: true
            }
        }
    }
}
