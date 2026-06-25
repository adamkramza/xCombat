

import QtQuick
import QtQuick.Controls

//_________________________________________________________________________________
    //WIDOK 1: MENU GŁÓWNE / PAUZA
    //Ekran początkowy - po uruchomieniu gry
    Rectangle {
        property var rootWindow
        property string fontName
        id: menuGlowne
        // anchors.fill: parent
        color: "#1a1a1a"
        visible: root.activeView === "MENU"
        z: 100

        //TŁO
        Image {
            id: menuTlo
            anchors.fill: parent

            //Zmiana grafiki w zależności od tego, czy gra jest zapauzowana, czy to menu start
            source: root.gameStarted ? "qrc:/gracz/Pauza.png" : "qrc:/gracz/Menu.png"

            //Chroni przed zniekształceniami i przyspiesza ładowanie tekstur
            fillMode: Image.PreserveAspectCrop
            asynchronous: true
        }


        Column {
            anchors.centerIn: parent
            spacing: 20

            Text {
                text: root.gameStarted ? "PAUZA" : "DOWÓDZTWO"
                font.pixelSize: 60
                font.bold: true
                font.family: czcionkaGry.name
                font.letterSpacing: 2
                style: Text.Outline
                styleColor: "#001018"
                color: root.gameStarted ? "#ff4444" : "#aaaaaa"
                anchors.horizontalCenter: parent.horizontalCenter

            }

            // PRZYCISK START/WZNÓW
            Button {
                text: root.gameStarted ? "ODLOT" : "START MISJI"
                width: 300
                height: 50

                font.pixelSize: 30
                font.bold: true
                font.family: czcionkaGry.name
                font.letterSpacing: 2
                anchors.horizontalCenter: parent.horizontalCenter

                onClicked: {
                    if (!root.gameStarted)
                     {
                         kontrola.resetujGre();
                         kontrola.startGame()
                     }
                     else
                     {
                         kontrola.ustawPauze(false)
                     }

                     root.activeView = "GAME"
                     root.gameStarted = true

                     gracz1.forceActiveFocus()
                }
            }

            // PRZYCISK STEROWANIE
            Button {
                text: "ODPRAWA"
                width: 300
                height: 50
                font.pixelSize: 30
                font.bold: true
                font.family: czcionkaGry.name
                font.letterSpacing: 2
                anchors.horizontalCenter: parent.horizontalCenter
                onClicked: {
                    root.activeView = "STEROWANIE" // Zmieniamy widok na ekran sterowania - nowy ekran/tło
                }
            }

            // PRZYCISK POWRÓT DO MENU (Widoczny tylko podczas PAUZY)
            //Powrót do Menu resetuje grę od początku
            Button {
                text: "MENU GŁÓWNE"
                width: 300
                height: 50
                font.pixelSize: 30
                font.bold: true
                font.family: czcionkaGry.name
                font.letterSpacing: 2
                anchors.horizontalCenter: parent.horizontalCenter

                // Przycisk pojawi się tylko wtedy, kiedy gra została już wystartowana
                visible: root.gameStarted

                onClicked: {
                    kontrola.resetujGre()    // funkcja resetująca grę
                      kontrola.ustawPauze(true)
                    root.gameStarted = false // Przełączenie flagi menu w QML
                    root.activeView = "MENU"  // Powrót do menu głównego
                }
            }
            //PRZYCISK Z WYNIKAMI
            Button {
                text: "WYNIKI"
                width: 300
                height: 50
                font.pixelSize: 30
                font.bold: true
                font.letterSpacing: 2
                font.family: czcionkaGry.name
                anchors.horizontalCenter: parent.horizontalCenter
                onClicked: {
                    root.activeView = "WYNIKI"
                }
            }

            // PRZYCISK WYJŚCIE
            //Przycisk służący do zamknięcia aplikacji
            Button {
                text: "WYJŚCIE"
                width: 300
                height: 50
                font.pixelSize: 30
                font.bold: true
                font.family: czcionkaGry.name
                font.letterSpacing: 2
                anchors.horizontalCenter: parent.horizontalCenter
                onClicked: Qt.quit()
            }
        }
    }