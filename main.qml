import QtQuick
import QtQuick.Controls

Window {

    //DODAŁEM CZCIONKĘ - GOOGLE FONTS - TA BĘDZIE LEPIEJ PASOWAĆ
    FontLoader
    {
            id: czcionkaGry
            source: "qrc:/gracz/Orbitron-Bold.ttf"
    }


    //GŁÓWNE OKNO PROGRAMU - WEDŁUG MOJEJ ROZDZIELCZOŚCI LAPKA - DODAŁEM FULLSCREEN
    id: root
    width: 1920
    height: 1080
    minimumWidth: width
    maximumWidth: width
    minimumHeight: height
    maximumHeight: height
    visible: true
    title: qsTr("MENU") // NAZWA OKNA GŁÓWNEGO
    visibility: Window.FullScreen
    Keys.enabled: true
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

    MenuWidok
    {
        id: mojeMenu
        anchors.fill: parent
        visible: root.activeView === "MENU"
    }

    SterowanieWidok
    {
        id: mojeSterowanie
        anchors.fill: parent
        visible: root.activeView === "STEROWANIE"
    }

    KoniecGryWidok
    {
        id: mojeKoniecGry
        anchors.fill: parent
        visible: root.activeView === "GAMEOVER"
    }

    WynikiWidok
    {
        id: mojeWyniki
        anchors.fill: parent
        visible: root.activeView === "WYNIKI"
    }



    //_________________________________________________________________________________
        //WIDOK: GRA
        Rectangle {
            id: gameView
            anchors.fill: parent
            visible: root.activeView === "GAME"
            color: "white"
            focus: true

            onVisibleChanged: {
                if (visible) {
                    forceActiveFocus()
                }
            }



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
                    duration: 30000 // Czas trwania pełnego cyklu przesuwania tła
                    loops: Animation.Infinite
                    running: root.activeView === "GAME" // Tło przewija się tylko podczas gry
                }

                // PIERWSZA KOPIA TŁA
                Image {
                    id: bgPart1
                    width: parent.width
                    height: parent.height
                    x: scrollingBackground.offset // Pozycja zależy od animacji
                    source: "qrc:/gracz/Gra1.png"
                    fillMode: Image.Stretch
                }

                // DRUGA KOPIA TŁA (doklejona z prawej strony)
                Image {
                    id: bgPart2
                    width: parent.width
                    height: parent.height
                    x: scrollingBackground.offset + parent.width // Zawsze dokładnie 1920 pikseli w prawo od pierwszej
                    source: "qrc:/gracz/Gra1.png"
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
                        kontrola.ustawPauze(true)
                        event.accepted = true;
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

                   // Bazowa szerokość w zależności od typu
                    property real bazowaSzerokosc: modelData.typ === 1 ? 140 : // UFO
                                                  modelData.typ === 2 ? 120  : // HELIKOPTER
                                                  modelData.typ === 3 ? 120 : // SAMOLOT 2
                                                  modelData.typ === 0 ? 100 : //SAMOLOT 1
                                                                      100//

                    // JEŚLI WYBUCH (typ 99), MNOŻYMY ROZMIAR RAZY 2.0, W PRZECIWNYM RAZIE ZOSTAJE BAZOWY
                    //zwiększamy rozmiar grafiki wybuchu
                    width: modelData.typ === 99 ? kontenerPrzeciwnika.bazowaSzerokosc * 2.0 : kontenerPrzeciwnika.bazowaSzerokosc
                    height: width * 0.7
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
                            if (modelData.typ === 3) return "qrc:/gracz/dolny.png"
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
                        font.letterSpacing: 2
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


            //ZMIANA LEVELU GRY
            //Tekst wyskakujący na środku ekranu gry
            Text {
                id: levelText
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


        }





}


