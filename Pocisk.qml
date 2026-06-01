import QtQuick

Rectangle {
    id: pocisk
    width: 60
    height: 20
    color: "transparent"


    x: modelData.x
    y: modelData.y


    Image {
            id: grafikaPocisku
            source: "qrc:/gracz/Rakieta.png" // Ścieżka do Twojego pliku PNG
            anchors.fill: parent
            fillMode: Image.PreserveAspectFit
        }
}



