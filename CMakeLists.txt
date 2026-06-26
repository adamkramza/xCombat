cmake_minimum_required(VERSION 3.16)


project(xCombat VERSION 0.1 LANGUAGES CXX)


set(CMAKE_AUTOMOC ON)

set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD_REQUIRED ON)


find_package(Qt6 6.2 COMPONENTS Quick REQUIRED)


qt_add_executable(appxCombat

   main.cpp

)


qt_add_qml_module(appxCombat
    URI xCombat
    VERSION 1.0

    QML_FILES
        main.qml
        Pocisk.qml
        Przeciwnik.qml
        KoniecGryWidok.qml
        MenuWidok.qml
        SterowanieWidok.qml
        WynikiWidok.qml

    SOURCES
        Sterowanie.h Sterowanie.cpp
        Pocisk.h Pocisk.cpp
        Przeciwnik.h Przeciwnik.cpp
        Przeciwnikszybki.h Przeciwnikszybki.cpp
        Przeciwnikduzy.h Przeciwnikduzy.cpp
        Przeciwnikdolny.h Przeciwnikdolny.cpp
        HistoriaWynikow.h HistoriaWynikow.cpp

    RESOURCES
        JET.png
        images/images.qrc
)


# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.

# If you are developing for iOS or macOS you should consider setting an

# explicit, fixed bundle identifier manually though.

set_target_properties(appxCombat PROPERTIES

#    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.appxCombat

   MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}

   MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}

   MACOSX_BUNDLE TRUE

   WIN32_EXECUTABLE TRUE

)


target_link_libraries(appxCombat

   PRIVATE Qt6::Quick)


include(GNUInstallDirs)

install(TARGETS appxCombat

   BUNDLE DESTINATION .

   LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})