import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3

Rectangle {
    id: inputItem
    property bool editable: false
    property string titleText: "А"
    property alias textValue: textInput.text
    property int widthLabel: label.text.length * 13
    property int fontSize: 16 //inputItem.height*inputItem.width * 0.002

    property bool isSelect: false
    signal valueChanged()



    MouseArea {
        anchors.fill: parent
        onClicked: {
            isSelect = false
            inputItem.forceActiveFocus(false)
        }
    }

    Label  {
        id: label
        anchors.left:  parent.left
        anchors.leftMargin: 5
        height: parent.height
        width: widthLabel
        text: titleText
        font.bold: editable?((isSelect)?1:0):0
        font.pixelSize: fontSize
        elide: Text.ElideMiddle
        fontSizeMode: Text.Fit
        horizontalAlignment: Text.AlignRight
        verticalAlignment: Text.AlignVCenter
        renderType: Text.NativeRendering
    }
    Rectangle {
        id: textInputRect
        anchors{
            bottom: parent.bottom
            top: parent.top
            left:  label.right
            leftMargin: 10
            rightMargin: 10
        }

//        height: parent.height
        width: parent.width - label.width - anchors.leftMargin*2
        border.width: 1
        TextInput {
            id: textInput
            anchors.fill: parent
            font.family: "Courier"
            clip: true
           // wrapMode: Text .WrapAnywhere
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
            renderType: Text.NativeRendering
            font.pixelSize: fontSize
            onActiveFocusChanged: {
             isSelect = activeFocus
            }
            onEditingFinished: valueChanged()
            Keys.onPressed: {
                if( (event.key === Qt.Key_Enter) || (event.key === Qt.Key_Return))  {
                    textInputRect.forceActiveFocus(false)
                }
            }
        }
    }
}
