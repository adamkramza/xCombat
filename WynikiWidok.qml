import QtQuick
import QtQuick.Controls


// WIDOK 5: WYNIKI
Rectangle
{
    id: wynikiView
    anchors.fill: parent
    visible: root.activeView === "WYNIKI"
    color: "#1a1a1a"
    z: 999

    Image {
        id: widok_wyniki_tlo
        anchors.fill: parent
        source: "qrc:/gracz/Wyniki.png"
        fillMode: Image.PreserveAspectCrop
        asynchronous: true
    }
    Column {
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: parent.top
        anchors.topMargin: 80
        spacing: 20

        Text {
            text: "WŁADCY PRZESTWORZY"
            font.pixelSize: 50
            font.family: czcionkaGry.name
            font.bold: true
            color: "#aaaaaa"
            anchors.horizontalCenter: parent.horizontalCenter
        }

        // Tabela pobierająca dane wyników
        ListView {
            width: 450
            height: 500
            clip: true
            spacing: 10

            // Za każdym razem jak wchodzimy w ten ekran, odświeżamy model z backendu
            model: root.activeView === "WYNIKI" ? historia.pobierzWyniki() : []

            delegate: Text {
                text: (index + 1) + ". " + modelData
                color: "white"
                font.pixelSize: 30
                font.family: czcionkaGry.name
                font.bold: true
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.left: parent.left
                anchors.leftMargin: 40
            }
        }

        Button {
            text: "POWRÓT"
            width: 300
            height: 50
            font.pixelSize: 30
            font.family: czcionkaGry.name
            font.bold: true
            anchors.horizontalCenter: parent.horizontalCenter
            onClicked: root.activeView = "MENU"
        }
    }
}