import QtQuick
import QtQuick.Controls


//_________________________________________________________________________________
    // WIDOK 4: GAME OVER
    //Ekran po zniszczeniu - ilość punktów, przycisk powrotu do menu głównego
    Rectangle {
        id: widok_koniecgry
        anchors.fill: parent
        visible: root.activeView === "GAMEOVER"
        color: "#1a1a1a"
        z: 999

        Image {
            id: widok_koniecgry_tlo
            anchors.fill: parent
            source: "qrc:/gracz/GameOverxCombat.png"
            fillMode: Image.PreserveAspectCrop
            asynchronous: true
        }

        Column {
            anchors.centerIn: parent
            spacing: 30

            Text {
                text: "KONIEC GRY"
                font.pixelSize: 100
                font.family: czcionkaGry.name
                font.bold: true
                color: "#deff9a"
                anchors.horizontalCenter: parent.horizontalCenter
            }
            //zapisywanie wynikow
            TextField {
                id: poleImie
                placeholderText: "Wpisz swoje imię"
                font.pixelSize: 24
                width: 400
                height: 50
                font.family: czcionkaGry.name
                anchors.horizontalCenter: parent.horizontalCenter
                horizontalAlignment: TextInput.AlignHCenter
                color: "black"
                background: Rectangle
                                {
                                    color: "white"
                                    radius: 6
                                    border.color: "#00f0ff"
                                    border.width: 2
                                }
            }


            Button {
                            text: "ZAPISZ WYNIK"
                            width: 300
                            height: 50
                            font.pixelSize: 30
                            font.family: czcionkaGry.name
                            font.bold: true
                            anchors.horizontalCenter: parent.horizontalCenter

                            onClicked: {
                                if (poleImie.text !== "") {
                                    historia.zapiszWynik(poleImie.text, Math.floor(kontrola.punkty));
                                    poleImie.text = "";
                                    kontrola.resetujGre()
                                      kontrola.ustawPauze(true)
                                    root.gameStarted = false
                                    root.activeView = "WYNIKI";
                                }
                            }
                        }


            Button {
                text: "MENU GŁÓWNE"
                width: 300
                height: 50
                font.pixelSize: 30
                font.family: czcionkaGry.name
                font.bold: true
                anchors.horizontalCenter: parent.horizontalCenter

                onClicked: {
                    kontrola.resetujGre()
                    root.gameStarted = false
                    root.activeView = "MENU"
                }
            }

            Row {
                anchors.horizontalCenter: parent.horizontalCenter
                spacing: 15

                Text {
                    text: "ZDOBYTE PUNKTY:"
                    font.pixelSize: 30
                    font.family: czcionkaGry.name
                    font.bold: true
                    color: "#aaaaaa"
                    font.letterSpacing: 2
                }

                Text {
                    text: Math.floor(kontrola.punkty)
                    font.pixelSize: 36
                    font.bold: true
                    font.family: czcionkaGry.name
                    color: "#deff9a"
                }
            }

        }

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
    }
