import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.12
import QtQuick.Dialogs 1.2
import Qt.labs.settings 1.0
import NameFM 1.0

ApplicationWindow {
    visible: true
    minimumWidth: 650
    minimumHeight: 480
    maximumHeight: 520
    maximumWidth: 800

    ListModel {
        id: checkList
        ListElement {name : "Удалять входные файлы" }
        ListElement {name : "Включить счетчик к имени файла" }
        ListElement {name : "Работа по таймеру" }
    }
    ListModel {
        id: inputList
        ListElement {name : "Маска"}
        ListElement {name : "Ключ" }
        ListElement {name : "Периодичность, сек." }
    }

    header: Rectangle {
        //enabled:
        id: head
        height: 100
        NewLabel {
            id: labelModule
            text: "Программа модификации файлов"
            anchors{
                left: head.left
                top: head.top
                right: head.right
                margins: 5
            }
        }
        NewButton {
            enabled:  ( fMode.parameters.status && isOn) || ( !fMode.parameters.status)
            id: startBut
            height: 40
            width: parent.width/2 - 40
            anchors{
                left: parent.left
                top: labelModule.bottom
                bottom: parent.bottom
                margins: 15
            }
            fontSize: 20
            controlButtonTitle:  !isOn ? "Старт" : "Стоп"
            onControlHandle: {

                if( isOn ) {
                    fMode.stopModReplay();
                    isOn = ! isOn
                }
                else{
                    var replay = rectSettings.repeaterItem.itemAt(2).itemChecked
                    if(replay)
                        isOn = !isOn
                    fMode.startModification(replay);
                }
            }
        }
        NewInfCell {
            id: statusCell
            height: 40
            width: parent.width/2 - 40
            indiTitle: fMode.parameters.status ? "Идет обработка файлов   " :
                                                 "Ожидание начала работы..."
            isOn: fMode.parameters.status
            anchors{
                right: parent.right
                top: labelModule.bottom
                bottom: parent.bottom
                margins: 15
            }
        }
    }
    Rectangle{
        enabled: !fMode.parameters.status
        anchors{
            top: head.bottom
            bottom: parent.bottom
            left: parent.left
            right: parent.right
            margins: 10
        }
        RectTitle{
            id: rectPath
            titleText: "Параметры"
            height: parent.height * 0.55
            anchors{
                top: parent.top
                left: parent.left
                right: parent.right
                topMargin: 5
                margins: 10
            }
            ColumnLayout{
                anchors{
                    fill: parent
                    topMargin: 10
                    margins: 5
                }
                spacing: 15
                Repeater{
                    id:repeater1
                    model: 2
                    delegate:  Rectangle{
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        InputField{
                            id: input
                            anchors.fill: parent
                            anchors.rightMargin: parent.width * 0.15
                            widthLabel: parent.width * 0.31
                            titleText: !index ? "Путь для чтения": "Путь для сохраниения"
                            textValue: !index? fMode.parameters.searchPath : fMode.parameters.savePath
                            onValueChanged: {
                                if(!index)
                                    fMode.setParameters(NameFM.SEARCH_PATH, textValue);
                                else
                                    fMode.setParameters(NameFM.SAVE_PATH, textValue);
                            }
                        }
                        NewButton {
                            anchors{
                                right: parent.right
                                left: input.right
                                top: parent.top
                                bottom: parent.bottom
                            }
                            controlButtonTitle: "Обзор"
                            onControlHandle: {
                                if(!index)
                                    openfolderDialog.open();
                                else
                                    savefolderDialog.open();
                            }
                        }
                    }
                }
                Repeater{
                    id:repeater2
                    model: inputList
                    delegate:  InputField{
                        enabled: !(index === repeater2.count - 1)
                        widthLabel: parent.width * 0.31
                        titleText: name
                        textValue: {
                            if(!index)
                                fMode.parameters.mask;
                            else if (index === 1)
                                fMode.parameters.key;
                            else
                                fMode.parameters.timeMode;
                        }
                        onValueChanged: {
                            console.log(NameFM.KEY)
                            console.log(NameFM.KEY)
                            console.log(NameFM.TIME_MODE)
                            if(!index)
                                fMode.setParameters(NameFM.MASK, textValue);
                            else if (index === 1){
                                fMode.setParameters(NameFM.KEY, textValue.toString().replace("0x",""));
                            }
                            else
                                fMode.setParameters(NameFM.TIME_MODE, textValue);
                        }
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                    }
                }
            }
        }
        RectTitle{
            id:rectSettings
            anchors{
                top: rectPath.bottom
                bottom: parent.bottom
                left: parent.left
                right: parent.right
                margins: 10
                topMargin: 25
            }
            columns: 1
            gridRowSpacing: 15
            indicatorModel: checkList
            titleText: "Настройки"
            repeaterDelegate: CheckBoxNew {
                onItemCheckedChanged: {
                    if(index === 2)
                        repeater2.itemAt(2).enabled = itemChecked;
                    fMode.setCheckState(index, itemChecked);
                }
                titleBox: name
                Layout.fillWidth: true
                Layout.fillHeight: true
            }
        }
    }
    FileDialog{
        id: openfolderDialog
        selectFolder: true
        onAccepted: {
            fMode.setParameters(NameFM.SEARCH_PATH, fileUrl.toString().replace("file:///","") + "/");
        }
    }
    FileDialog{
        id: savefolderDialog
        selectFolder: true
        onAccepted: {
            fMode.setParameters(NameFM.SAVE_PATH, fileUrl.toString().replace("file:///","") + "/");
        }
    }

}




//    Timer{
//        id: timer
//        interval: 1200
//        repeat: false
//        running: false
//        onTriggered: {
//        }
//    }
