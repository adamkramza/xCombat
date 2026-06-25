

import QtQuick
import QtQuick.Controls

//_________________________________________________________________________________
    //WIDOK 3: STEROWANIE
    //Informacje na temat mechaniki gry i sterowania
    Rectangle {
        id: widok_sterowanie
        anchors.fill: parent
        visible: root.activeView === "STEROWANIE"
        color: "#1a1a1a"

        Image {
            id: widok_sterowanie_tlo
            anchors.fill: parent
            source: "qrc:/gracz/Sterowanie.png"
            fillMode: Image.PreserveAspectCrop
            asynchronous: true
        }

        Column {
            anchors.centerIn: parent
            spacing: 40



            Button {
                text: "POWRÓT"
                width: 300
                height: 50
                font.pixelSize: 30
                font.family: czcionkaGry.name
                font.bold: true
                anchors.horizontalCenter: parent.horizontalCenter
                onClicked: {
                     kontrola.ustawPauze(true)
                    root.activeView = "MENU"
                }
            }
        }
    }
