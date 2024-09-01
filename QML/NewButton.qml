import QtQuick 2.12
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4
import QtGraphicalEffects 1.0


Button {
    id: controlButton //  попробовать делать 3рект темнее
    height: 45
    property bool isOn: false
    onIsOnChanged: timerColor.start();
    property string controlButtonTitle
    property int fontSize: 14//controlButton.width/2 * 0.55
    property int buttonRadius: 10
    property bool visibleButton: true
    property bool enableButton: true

    property string borderColor: Qt.lighter("gray", 1.8)// "#a8a7ab"
    property string unpressedColor: Qt.lighter("gray", 1.65)
    property string pressedColor: Qt.lighter("gray", 1.4)//"#e1e1e1"

    signal controlHandle (bool controlState)
    visible: visibleButton
    enabled: enableButton
    opacity: enabled? 1 : 0.5

    Timer{
        id: timerColor
        interval: 100;
        repeat: false
        onTriggered: {
            borderColor = isOn ?  Qt.lighter("green", 1.9) : Qt.lighter("gray", 1.8)
            unpressedColor = isOn ?  Qt.lighter("green", 1.5) : Qt.lighter("gray", 1.65)
            pressedColor = isOn ?  Qt.lighter("green", 1.4) : Qt.lighter("gray", 1.4)
        }
    }
    style: ButtonStyle{
        background: Rectangle  {
            id: butRect1
            radius: buttonRadius
            color: "#3e3c3c"
            Rectangle{
                id: butRect2
                radius: buttonRadius
                color: borderColor
                anchors{
                    fill: parent
                    margins: 2
                    topMargin: 1
                    leftMargin: 1
                }
                Behavior on color {
                    ColorAnimation {
                        duration: 300
                    }
                }
                Rectangle {
                    id: butRect3
                    property bool ctrlPressed : control.pressed
                    radius: buttonRadius
                    color: unpressedColor
                    Behavior on color {
                        ColorAnimation {
                            duration: 300
                        }
                    }
                    anchors{
                        fill: parent
                        margins: 4
                        bottomMargin: 2
                        rightMargin: 2
                    }

                    onCtrlPressedChanged: {
                        if(ctrlPressed){
                            animOnClickB3LM.start()
                            animOnClickB3TM.start()
                            animOnClickB2LM.start()
                            animOnClickB2TM.start()
                            animOnClickB3Color.start()
                        }
                        else
                            timer.start();
                    }

                    Timer {
                        id:timer
                        interval: 500;
                        repeat: false
                        onTriggered: {
                            animOffClickB3LM.start()
                            animOffClickB3TM.start()
                            animOffClickB2LM.start()
                            animOffClickB2TM.start()
                            animOffClickB3Color.start()
                        }
                    }
                    //--------------------------
                    PropertyAnimation {
                        id: animOnClickB3Color
                        target: butRect3
                        property: "color"
                        from: butRect3.color
                        to:  pressedColor
                        duration: 120
                    }
                    PropertyAnimation {
                        id: animOnClickB3LM
                        target: butRect3
                        property: "anchors.leftMargin"
                        from: butRect3.anchors.leftMargin
                        to: 2
                        duration: 150
                    }
                    PropertyAnimation {
                        id: animOnClickB3TM
                        target: butRect3
                        property: "anchors.topMargin"
                        from: butRect3.anchors.topMargin
                        to: 2
                        duration: 150
                    }
                    PropertyAnimation {
                        id: animOnClickB2LM
                        target: butRect2
                        property: "anchors.leftMargin"
                        from: butRect2.anchors.leftMargin
                        to: 2
                        duration: 150
                    }
                    PropertyAnimation {
                        id: animOnClickB2TM
                        target: butRect2
                        property: "anchors.topMargin"
                        from: butRect2.anchors.topMargin
                        to: 2
                        duration: 150
                    }
                    //--------------------------
                    PropertyAnimation {
                        id: animOffClickB3Color
                        target: butRect3
                        property: "color"
                        from:  butRect3.color
                        to: unpressedColor
                        duration: 120
                    }
                    PropertyAnimation {
                        id: animOffClickB3LM
                        target: butRect3
                        property: "anchors.leftMargin"
                        from: butRect3.anchors.leftMargin
                        to: 4
                        duration: 150
                    }
                    PropertyAnimation {
                        id: animOffClickB3TM
                        target: butRect3
                        property: "anchors.topMargin"
                        from: butRect3.anchors.topMargin
                        to: 4
                        duration: 150
                    }
                    PropertyAnimation {
                        id: animOffClickB2LM
                        target: butRect2
                        property: "anchors.leftMargin"
                        from: butRect2.anchors.leftMargin
                        to: 1
                        duration: 150
                    }
                    PropertyAnimation {
                        id: animOffClickB2TM
                        target: butRect2
                        property: "anchors.topMargin"
                        from: butRect2.anchors.topMargin
                        to: 1
                        duration: 150
                    }
                }
            }
        }
        label : Rectangle{
            color: "transparent"
            Text {
                id: buttonLabel
                anchors.fill: parent
                anchors.margins: 3
                text: controlButtonTitle
                font.pixelSize: fontSize
                fontSizeMode: Text.Fit
                font.bold: true
                font.family: "Courier"
                elide: Text.ElideMiddle
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
                renderType: Text.NativeRendering
            }
        }
    }
    onClicked: {
        controlButton.controlHandle(isOn)
    }
}

