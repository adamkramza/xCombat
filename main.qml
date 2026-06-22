import QtQuick
import QtQuick.Controls

Window {

    FontLoader
    {
            id: czcionkaGry
            source: "qrc:/gracz/Orbitron-Bold.ttf" // Upewnij się, że ścieżka qrc jest poprawna!
    }


    //GŁÓWNE OKNO PROGRAMU - WEDŁUG MOJEJ ROZDZIELCZOŚCI
    id: root
    width: 1920
    height: 1080
    minimumWidth: width
    maximumWidth: width
    minimumHeight: height
    maximumHeight: height
    visible: true
    title: qsTr("MENU") // NAZWA OKNA GŁÓWNEGO

    //LOGIKA SYSTEMOWA
    property string activeView: "MENU"
    property bool gameStarted: false

    //Kontrola życia - zmienia obraz - przełącza ekran na gameover jeżeli <=0
    property int aktualneZycie: kontrola.zycie
    onAktualneZycieChanged: {
        if (aktualneZycie <= 0 && root.activeView === "GAME") {
            root.activeView = "GAMEOVER"
        }
    }
//_________________________________________________________________________________
    //WIDOK 1: MENU GŁÓWNE / PAUZA
    //Ekran początkowy - po uruchomieniu gry
    Rectangle {
        id: menuGlowne
        anchors.fill: parent
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
                text: root.gameStarted ? "PAUZA" : "MENU"
                font.pixelSize: 100
                font.bold: true
                font.family: czcionkaGry.name
                font.letterSpacing: 2
                color: root.gameStarted ? "#ff4444" : "#deff9a"
                anchors.horizontalCenter: parent.horizontalCenter
            }

            // PRZYCISK START/WZNÓW
            Button {
                text: root.gameStarted ? "WZNÓW" : "START"
                width: 300
                height: 50
                font.pixelSize: 30
                font.bold: true
                font.family: czcionkaGry.name
                font.letterSpacing: 2
                anchors.horizontalCenter: parent.horizontalCenter
                onClicked: {
                    root.activeView = "GAME"
                    root.gameStarted = true
                    gracz1.forceActiveFocus()
                }
            }

            // PRZYCISK STEROWANIE
            Button {
                text: "STEROWANIE"
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

//_________________________________________________________________________________
    //WIDOK 2: GRA
    Rectangle {
        id: gameView
        anchors.fill: parent
        visible: root.activeView === "GAME"
        color: "white"

        //PRZESUWAJACE SIĘ TŁO
        Item {
            id: scrollingBackground
            anchors.fill: parent
            z: 0

            // Właściwość kontrolująca przesunięcie (od 0 do -1920)
            property real offset: 0

            // Animacja, która bez przerwy zmienia wartość offsetu
            NumberAnimation on offset {
                from: 0
                to: -root.width // Przesuwamy o pełną szerokość ekranu (1920)
                duration: 50000 // Czas trwania pełnego cyklu przesuwania tła
                loops: Animation.Infinite
                running: root.activeView === "GAME" // Tło przewija się tylko podczas gry
            }

            // PIERWSZA KOPIA TŁA
            Image {
                id: bgPart1
                width: parent.width
                height: parent.height
                x: scrollingBackground.offset // Pozycja zależy od animacji
                source: "qrc:/gracz/1.Pustynia.png"
                fillMode: Image.Stretch
            }

            // DRUGA KOPIA TŁA (doklejona z prawej strony)
            Image {
                id: bgPart2
                width: parent.width
                height: parent.height
                x: scrollingBackground.offset + parent.width // Zawsze dokładnie 1920 pikseli w prawo od pierwszej
                source: "qrc:/gracz/1.Pustynia.png"
                fillMode: Image.Stretch
            }
        }

        // 1. GRACZ
        //Konstrolowany samolot
        Rectangle  {
            id: gracz1
            width: 200;
            height: 100;
            color: "transparent"
            x: kontrola.x - width/2
            y: kontrola.y - height/2
            focus: true

            Image {
                id: player
                width:200
                height:100
                source: "qrc:/gracz/JET.png" // tu mamy grafikę samolotu gracza
                anchors.fill: parent
                fillMode: Image.PreserveAspectFit
            }

            property var keys: ({})


            Keys.onPressed: (event) => {
                if (event.key === Qt.Key_Escape) {
                    root.activeView = "MENU"
                    return;
                }
                if (event.key === Qt.Key_Space && !event.isAutoRepeat) {
                    kontrola.strzalRakieta();
                }
                keys[event.key] = true;
            }


            //STEROWANIE GRACZEM
            Keys.onReleased: (event) => { delete keys[event.key] }

            Timer {
                interval: 16
                running: root.activeView === "GAME"
                repeat: true
                onTriggered: {
                    if (gracz1.keys[Qt.Key_Left])  kontrola.wLewo();
                    if (gracz1.keys[Qt.Key_Right]) kontrola.wPrawo();
                    if (gracz1.keys[Qt.Key_Up])    kontrola.wGore();
                    if (gracz1.keys[Qt.Key_Down])  kontrola.wDol();
                }
            }

        }

        // POCISKI
        Repeater {
            model: kontrola.pociski

            delegate: Rectangle
            {
                width:  60
                height: 20
                color: "transparent"

                // Pozycja z C++
                x: modelData.x
                y: modelData.y

                Image {
                    id: grafikaBroni
                    anchors.fill: parent
                    fillMode: Image.PreserveAspectFit

                    source: "qrc:/gracz/Rakieta.png"
                }
            }
        }


        // PRZECIWNICY
        Repeater {
            model: kontrola.przeciwnicy
            delegate: Rectangle {
                id: kontenerPrzeciwnika

                //Bazowa szerokość w zależności od typu
                property real bazowaSzerokosc: modelData.typ === 1 ? 140 : // Duży
                                              modelData.typ === 2 ? 80  : // Szybki
                                              modelData.typ === 3 ? 120 : // Dolny
                                                                    100   // Zwykły (typ 0)

                // JEŚLI WYBUCH (typ 99), MNOŻYMY ROZMIAR RAZY 2.0, W PRZECIWNYM RAZIE ZOSTAJE BAZOWY
                //zwiększamy rozmiar grafiki wybuchu
                width: modelData.typ === 99 ? kontenerPrzeciwnika.bazowaSzerokosc * 2.0 : kontenerPrzeciwnika.bazowaSzerokosc
                height: width * 0.8
                color: "transparent"

                x: modelData.x
                y: modelData.y

                // Wyśrodkowanie powiększonego wybuchu
                transform: Translate {
                    x: modelData.typ === 99 ? -(kontenerPrzeciwnika.bazowaSzerokosc * 2.0 - kontenerPrzeciwnika.bazowaSzerokosc) / 2 : 0
                    y: modelData.typ === 99 ? -(kontenerPrzeciwnika.bazowaSzerokosc * 2.0 - kontenerPrzeciwnika.bazowaSzerokosc) * 0.8 / 2 : 0
                }

                //Grafika przeciwników i wybuchu
                Image {
                    id: grafikaPrzeciwnika
                    anchors.fill: parent
                    fillMode: Image.Stretch

                    source:
                    {
                        if (modelData.typ === 99) return "qrc:/gracz/wybuch.png"
                        if (modelData.typ === 1) return "qrc:/gracz/UFO1.png"
                        if (modelData.typ === 2) return "qrc:/gracz/Przeciwnikhelikopter1.png"
                        if (modelData.typ === 3) return "qrc:/gracz/UFO2.png"
                        return "qrc:/gracz/Przeciwniksamolot1.png"
                    }
                }
            }
        }



        // PANEL PUNKTACJI - prawy górny róg
        //prawy górny róg

        Rectangle {
            id: panel_punktacji
            anchors {
                top: parent.top
                right: parent.right
                margins: 30
            }

            width: 280
            height: 50
            color: "#cc1a1a1a"
            border.color: "#66ffffff"
            border.width: 2
            radius: 4

            Row {
                anchors.centerIn: parent
                spacing: 10

                Text {
                    text: "PUNKTY:"
                    color: "#aaaaaa"
                    font.pixelSize: 30
                    font.bold: true
                    font.family: czcionkaGry.name
                    font.letterSpacing: 2
                }

                Text {
                    id: punkty
                    text: Math.floor(kontrola.punkty)
                    color: "white"
                    font.pixelSize: 30
                    font.bold: true
                    font.family: czcionkaGry.name
                }
            }
        }


        // PASEK PANCERZA - środek góra
        Rectangle {
            id: pasek_pancerz
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: parent.top
            anchors.topMargin: 20

            width: 500
            height: 50

            color: "#2b2b2b"
            border.color: "#555555"
            border.width: 2
            radius: 6
            z: 100

            Rectangle {
                width: parent.width * kontrola.zycie / 100
                height: parent.height
                radius: 5

                color: kontrola.zycie <= 25 ? "#ff4444" :
                       kontrola.zycie <= 50 ? "#ffbb00" : "#00ff00"

                Behavior on color {
                    ColorAnimation { duration: 300 }
                }

                Behavior on width {
                    NumberAnimation { duration: 200 }
                }
            }

            Row {
                anchors.centerIn: parent
                spacing: 15

                Text {
                    text: kontrola.zycie <= 25 ? "ALARM: KADŁUB USZKODZONY" : "PANCERZ"
                    color: kontrola.zycie <= 25 ? "white" : "black"
                    font.pixelSize: 20
                    font.bold: true
                    font.family: czcionkaGry.name
                    font.letterSpacing: 2
                }

                Text {
                    text: kontrola.zycie<= 25? "!" : kontrola.zycie + "%"
                    color: kontrola.zycie <=25 ? "white": "black"
                    font.pixelSize: 20
                    font.family: czcionkaGry.name
                    font.bold: true
                }
            }
        }

        //PANEL POZIOMU
        Rectangle {
            id: poziom
            anchors
            {
                top: parent.top
                left: parent.left
                margins: 30
            }

            width: 280
            height: 50
            color: "#cc1a1a1a"
            border.color: "#66ffffff"
            border.width: 2
            radius: 4

            Row {
                anchors.centerIn: parent
                spacing: 10

                Text {
                    text: "POZIOM"
                    color: "#aaaaaa"
                    font.pixelSize: 30
                    font.family: czcionkaGry.name
                    font.bold: true
                    font.letterSpacing: 2
                }

                Text {
                    id: poziom1
                    text: Math.floor(kontrola.level)
                    color: "white"
                    font.pixelSize: 30
                    font.bold: true
                }
            }
        }


    }


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
            source: "qrc:/gracz/GameOverxCombat.png"
            fillMode: Image.PreserveAspectCrop
            asynchronous: true
        }

        Column {
            anchors.centerIn: parent
            spacing: 40

            Text {
                text: "STEROWANIE"
                font.pixelSize: 80
                font.family: czcionkaGry.name
                font.bold: true
                color: "#deff9a"
                anchors.horizontalCenter: parent.horizontalCenter
            }

            Button {
                text: "POWRÓT"
                width: 300
                height: 50
                font.pixelSize: 30
                font.family: czcionkaGry.name
                font.bold: true
                anchors.horizontalCenter: parent.horizontalCenter
                onClicked: {
                    root.activeView = "MENU"
                }
            }
        }
    }

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
    }

    //ZMIANA LEVELU GRY
    // Tekst wyskakujący na środku ekranu gry
    Text {
        id: level_Tekst
        anchors.centerIn: parent
        text: "POZIOM: " + kontrola.level
        font.pixelSize: 80
        font.family: czcionkaGry.name
        font.bold: true
        color: "#ffbb00"
        visible: false
        z: 500


        style: Text.Outline
        styleColor: "black"
    }

    // Timer wygaszający napis po 2 sekundach
    Timer {
        id: levelTimer
        interval: 2000
        repeat: false
        onTriggered: levelText.visible = false
    }


    // Łączy QML z sygnałem levelZmiana()
    Connections
    {
        target: kontrola

        function onLevelZmiana() {
            // Napis ma wyskakiwać tylko jeśli faktycznie gramy
            if (root.activeView === "GAME") {
                levelText.visible = true;
                levelTimer.restart(); // Uruchomienie odliczania czasu
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

        Column {
            anchors.centerIn: parent
            spacing: 30

            Text {
                text: "NAJLEPSZE WYNIKI"
                font.pixelSize: 80
                font.family: czcionkaGry.name
                font.bold: true
                color: "#deff9a"
                anchors.horizontalCenter: parent.horizontalCenter
            }

            // Tabela pobierająca dane wyników
            ListView {
                width: 450
                height: 300
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


