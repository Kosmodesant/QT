cmake_minimum_required(VERSION 3.5)

project(08_PreDataBase VERSION 0.1 LANGUAGES CXX)


set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS   Widgets
                                                    Concurrent
                                                    Sql)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS  Widgets
                                                        Concurrent
                                                        Sql)

set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
        database.h
        database.cpp
        dbdata.h
        dbdata.cpp
        dbdata.ui
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(${PROJECT_NAME}
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )

# Define target properties for Android with Qt 6 as:
#    set_property(TARGET testQt APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(${PROJECT_NAME} SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(${PROJECT_NAME}
            ${PROJECT_SOURCES}
        )
    endif()
endif()

if(CMAKE_SYSTEM_NAME STREQUAL "Windows")    # Для Windows
    target_link_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/PG_Libs)
    target_link_libraries(${PROJECT_NAME} PRIVATE   Qt${QT_VERSION_MAJOR}::Widgets
                                                    Qt${QT_VERSION_MAJOR}::Concurrent
                                                    Qt${QT_VERSION_MAJOR}::Sql
                                                    libcrypto-3-x64
                                                    libiconv-2
                                                    libintl-9
                                                    libpq
                                                    libssl-3-x64)
elseif(CMAKE_SYSTEM_NAME STREQUAL "Linux")  # Для Linux
    target_link_libraries(${PROJECT_NAME} PRIVATE   Qt${QT_VERSION_MAJOR}::Widgets
                                                    Qt${QT_VERSION_MAJOR}::Concurrent
                                                    Qt${QT_VERSION_MAJOR}::Sql)
else()
    message(FATAL_ERROR "Unknow OS")
endif()


set_target_properties(${PROJECT_NAME} PROPERTIES
    OUTPUT_NAME "database"
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(${PROJECT_NAME})
endif()
