cmake_minimum_required(VERSION 3.14)

project(task1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Network Sql)

add_executable(task1
  main.cpp
)
target_link_libraries(task1 Qt${QT_VERSION_MAJOR}::Core
                            Qt${QT_VERSION_MAJOR}::Network
                            Qt${QT_VERSION_MAJOR}::Sql)

install(TARGETS task1 LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})