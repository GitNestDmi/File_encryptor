import QtQuick 2.12

Rectangle {
    id: groupRec
    property bool isOn
    property bool cellEnablade : true
    property string indiOnTitle: indiTitle
    property string indiTitle
    property string indiColor:  "#adc25d"  //"#27CD59"
    property string indiOffColor: "transparent"
    property int sizeOfFont: groupRec.width/3 * 0.38
    property int alignVText: Text.AlignVCenter
    property int textStyle:      Text.AutoText

    Behavior on color {
        ColorAnimation {
            duration: 200
        }
    }
    //    width: parent.width
    //    height: parent.height
    color: isOn ? indiColor : indiOffColor
    border.width: 1
    radius: 10

    Text {
        visible: parent.visible
        color: cellEnablade ? "black" : "gray"
        id: cellTitle
        textFormat: textStyle
        anchors{
            fill: groupRec
            leftMargin: 2
            rightMargin: 2
        }
        text:  qsTr(isOn ? indiOnTitle : indiTitle)
        font.family: "Courier"
        fontSizeMode: Text.Fit
        padding: 5
        elide: Text.ElideMiddle
        font.pixelSize: sizeOfFont
        //        font.pixelSize: 20
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: alignVText
        renderType: Text.NativeRendering
    }
}
