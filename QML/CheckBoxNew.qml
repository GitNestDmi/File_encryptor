import QtQuick 2.12
import QtQuick.Controls 2.5

Item{
    property alias titleBox: title.text
    property bool itemChecked: false

    Rectangle{
        id: titleRect
        anchors.fill: parent
    }

    CheckBox {
        id: control
        checked: itemChecked
        anchors.fill: parent
        font.pixelSize: 16
        spacing: 10
        onHoveredChanged: {
            if(hovered)
                titleRect.color = "lightgray"
            else
                titleRect.color = "transparent"
        }
        onCheckedChanged: {
            itemChecked = control.checked
        }
        onToggled: {
            //control.checked = !control.checked
        }

        indicator: Rectangle {
            id: indicatorRec
            width: control.height
            height: control.height

            x: control.leftPadding
            y: parent.height / 2 - height / 2
            radius: 3
            border.color: "black"
            //     border.color: control.down ? "grey" : "black"

            Rectangle {
                height: indicatorRec.height/2
                width: height
                x: (indicatorRec.height- height)/2
                y:  (indicatorRec.width- width)/2
                radius: 2
                //color: control.down ? "grey" : "black"
                color: "black"
                visible: control.checked
            }
        }
        contentItem:  Text {
            id: title
            anchors.fill: parent
            leftPadding: control.indicator.width + control.spacing
            opacity: enabled ? 1.0 : 0.3
            //font.bold: true
            //        color: control.down ? "grey" : "black"
            color: "black"
            font.family: "Courier"
            font.pixelSize:  control.font.pixelSize
            verticalAlignment: Text.AlignVCenter
            fontSizeMode: Text.Fit
            elide: Text.ElideMiddle
            renderType: Text.NativeRendering
        }
    }
}


