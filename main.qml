import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.3
import SubDirectoryModel 1.0
import QtQuick.Dialogs 1.1

import SortFilterProxyModel 1.0

Window {
    id: window
    width: 705
    height: 480
    visible: true

    minimumHeight: 480
    minimumWidth: 705
    maximumHeight: 480
    maximumWidth: 705
    MessageDialog {
        id: messageDialog
        title: "Ошибка"
        onAccepted: {

            //Qt.quit()
        }
    }

    function onShowError(newError) {
        messageDialog.text = newError
        messageDialog.open()
    }

    Component.onCompleted: {
        SubDirectory.showError.connect(onShowError)
    }

    Item {
        id: root
        anchors.fill: parent
        anchors.topMargin: 15
        anchors.leftMargin: 10
        anchors.rightMargin: 10

        Item {
            id: find
            width: 200
            height: 26

            Item {
                id: image_find
                width: 20
                anchors.left: parent.left
                anchors.top: parent.top
                anchors.bottom: parent.bottom
                anchors.leftMargin: 5

                Image {
                    anchors.fill: parent
                    horizontalAlignment: Image.AlignHCenter
                    source: "png/find.png"
                    anchors.leftMargin: 0
                    fillMode: Image.PreserveAspectFit
                }
            }

            TextInput {
                anchors.left: image_find.right
                anchors.right: parent.right
                anchors.top: parent.top
                anchors.bottom: parent.bottom
                anchors.leftMargin: 5
                anchors.rightMargin: 10
                selectByMouse: true
                wrapMode: TextInput.NoWrap
                clip: true
                id: searchBox
                verticalAlignment: TextInput.AlignVCenter

                Text {
                    color: "#79000000"
                    anchors.fill: parent
                    verticalAlignment: Text.AlignVCenter
                    text: parent.text == "" ? "Поиск" : ""
                }

                onTextEdited: {

                    proxyModel.filterString = text
                }
            }

            Rectangle {
                anchors.fill: find
                border.color: "black"
                border.width: 1
                radius: 9
                color: "#efefef"
                smooth: true
                z: -1
            }
        }

        Row {
            id: header
            anchors.top: find.bottom
            anchors.topMargin: 20

            Column {
                id: column1
                width: 30
                Label {
                    text: "№"
                    width: 30
                    horizontalAlignment: Text.AlignHCenter

                    font.bold: true
                }
            }
            Column {
                id: column2
                width: 120
                Label {
                    text: "Страна"
                    font.bold: true
                }
            }
            Column {
                id: column3
                width: 200
                Label {
                    text: "Наименование"
                    font.bold: true
                }
            }

            Column {
                id: column4
                width: 300
                Label {
                    text: "Телефон"
                    font.bold: true
                }
            }
            Column {
                id: column5
                width: 30
            }
        }

        ListView {
            property int height_row: 40

            id: myListData
            anchors.top: header.bottom
            anchors.topMargin: 5
            anchors.bottomMargin: 10
            anchors.bottom: add_btn.top
            boundsBehavior: Flickable.StopAtBounds
            anchors.left: root.left
            anchors.right: root.right

            SubDirectoryModel {
                id: sourceModel
                list: SubDirectory
            }

            model: StringFilterProxyModel {
                id: proxyModel
                source: sourceModel
            }

            Component.onCompleted: {
                proxyModel.showError.connect(onShowError)
            }

            ScrollBar.vertical: ScrollBar {
                active: true
                id: scroll
            }

            clip: true

            Canvas {
                anchors.fill: myListData
                onPaint: {
                    const context = getContext("2d")
                    context.moveTo(0, 0)
                    context.lineTo(root.width, 0)
                    context.stroke() // отрисовка линии
                }
            }

            delegate: RowLayout {

                Rectangle {
                    anchors.fill: parent

                    id: row
                    color: "transparent"
                    MouseArea {
                        anchors.fill: parent
                        hoverEnabled: true
                        onEntered: row.color = "#dedede"
                        onExited: row.color = "transparent"
                    }
                }

                Item {
                    width: column1.width - 5
                    height: myListData.height_row

                    TextInput {
                        text: index + 1
                        id: num
                        readOnly: true
                        anchors.fill: parent
                        selectByMouse: true
                        wrapMode: TextInput.NoWrap
                        clip: true
                        verticalAlignment: TextInput.AlignVCenter
                        width: parent.width
                        horizontalAlignment: Text.AlignHCenter
                    }
                }

                Item {
                    width: column2.width - 5
                    height: myListData.height_row

                    ComboBox {
                        id: countr
                        background: Rectangle {
                            anchors.fill: parent
                            color: "transparent"
                            MouseArea {
                                anchors.fill: parent
                                hoverEnabled: true
                                onEntered: row.color = "#dedede"
                                onExited: row.color = "transparent"
                                onClicked: {
                                    countr.popup.open()
                                    countr.temp_text = countr.currentText
                                }
                            }
                        }

                        Component.onCompleted: currentIndex = indexOfValue(
                                                   country)
                        anchors.fill: parent
                        clip: true
                        model: ["США", "Россия", "Китай", "Германия"]
                        property string temp_text: currentText
                        onActivated: {

                            SubDirectory.country = currentText
                            SubDirectory.name = nam.text
                            SubDirectory.phone = phon.text

                            if ((countr.currentText != "" && nam.text != ""
                                 && phon.text != "()---")
                                    && temp_text != currentText) {

                                if (!isInsert) {
                                    isInsert = true
                                    SubDirectory.insertToDB()
                                } else
                                    SubDirectory.updateDB(numb)

                                nam.temp_text = nam.text
                                phon.temp_text = phon.text
                            }

                            country = currentText
                        }
                    }
                }

                Item {
                    width: column3.width - 5
                    height: myListData.height_row
                    TextInput {
                        text: name
                        id: nam
                        anchors.fill: parent
                        selectByMouse: true
                        wrapMode: TextInput.NoWrap
                        clip: true
                        verticalAlignment: TextInput.AlignVCenter

                        property string temp_text: text

                        onFocusChanged: {
                            temp_text = text
                        }

                        onTextChanged: {
                            model.name = text
                        }

                        onEditingFinished: {

                            SubDirectory.country = countr.currentText
                            SubDirectory.name = text
                            SubDirectory.phone = phon.text

                            if ((countr.currentText != "" && nam.text != ""
                                 && phon.text != "()---")
                                    && temp_text != text) {

                                if (!isInsert) {
                                    isInsert = true
                                    SubDirectory.insertToDB()
                                } else
                                    SubDirectory.updateDB(numb)

                                temp_text = text
                            }
                        }
                    }
                }

                Item {
                    width: column4.width - 5
                    height: myListData.height_row
                    TextInput {
                        text: phone
                        id: phon
                        anchors.fill: parent
                        selectByMouse: true
                        wrapMode: TextInput.NoWrap
                        clip: true
                        verticalAlignment: TextInput.AlignVCenter
                        maximumLength: 16
                        inputMask: "9(999)-999-99-99"

                        property string temp_text: text

                        onFocusChanged: {
                            temp_text = text
                        }

                        onTextChanged: {
                            model.phone = text
                        }

                        onEditingFinished: {

                            SubDirectory.country = countr.currentText
                            SubDirectory.name = nam.text
                            SubDirectory.phone = text

                            if ((countr.currentText != "" && nam.text != ""
                                 && phon.text != "()---")
                                    && temp_text != text) {

                                if (!isInsert) {
                                    isInsert = true
                                    SubDirectory.insertToDB()
                                } else
                                    SubDirectory.updateDB(numb)

                                temp_text = text
                            }
                        }
                    }
                }

                Item {
                    width: column4.width - 5
                    height: myListData.height_row
                    anchors.rightMargin: 10
                    anchors.right: parent.right
                    Button {
                        id: del
                        flat: true
                        icon.source: "png/trash.png"
                        width: 30
                        height: 30
                        anchors.verticalCenter: parent.verticalCenter
                        property int temp_numb: index

                        background: Rectangle {
                            anchors.fill: del
                            radius: 9
                            smooth: true
                            z: -1
                            color: "transparent"
                            MouseArea {
                                anchors.fill: parent
                                hoverEnabled: true
                                onEntered: {
                                    parent.color = "#52ff0000"
                                    row.color = "#dedede"
                                }
                                onExited: parent.color = "transparent"
                                onPressed: parent.color = "#80ff0000"
                                onReleased: parent.color = "#52ff0000"

                                onClicked: {

                                    sourceModel.deleteItem(numb)
                                }
                            }
                        }
                    }
                }

                Canvas {
                    anchors.fill: parent
                    onPaint: {
                        const context = getContext("2d")
                        context.moveTo(0, myListData.height_row)
                        context.lineTo(myListData.width, myListData.height_row)
                        context.stroke() // отрисовка линии
                    }
                }
            }
        }

        Button {
            id: add_btn
            width: 30
            height: 30
            anchors.right: root.right
            anchors.bottom: root.bottom
            rightPadding: 5
            leftPadding: 5
            bottomPadding: 5
            topPadding: 5
            anchors.bottomMargin: 10
            icon.source: "png/add.png"

            background: Rectangle {
                color: "#4e008333"
                anchors.fill: parent
                radius: 10
                z: -1
                MouseArea {
                    anchors.fill: parent
                    hoverEnabled: true
                    onPressed: parent.color = "#86008333"
                    onReleased: parent.color = "#4e008333"

                    onClicked: {
                        sourceModel.addSubDirectory()
                        scroll.position = 1 - scroll.size
                    }
                }
            }
        }

        FileDialog {
            id: export_json
            selectFolder: true
            onAccepted: {
                proxyModel.exportJSON(folder)
            }
        }

        Button {
            id: json_btn
            width: 30
            height: 30
            anchors.left: root.left
            anchors.bottom: root.bottom
            bottomPadding: 5
            topPadding: 5
            rightPadding: 5
            leftPadding: 5
            anchors.bottomMargin: 10
            icon.source: "png/export.png"

            background: Rectangle {
                color: "#4e000983"
                anchors.fill: parent
                radius: 10
                z: -1
                MouseArea {
                    anchors.fill: parent
                    hoverEnabled: true
                    onPressed: parent.color = "#6c000ba3"
                    onReleased: parent.color = "#4e000983"

                    onClicked: {
                        export_json.open()
                    }
                }
            }
        }
    }
}
