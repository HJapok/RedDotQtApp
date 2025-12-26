cmake_minimum_required(VERSION 3.16)
project(RedDotQtApp LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# ---- Qt6 ----
find_package(Qt6 REQUIRED COMPONENTS Widgets)

# ---- OpenCV ----
find_package(OpenCV REQUIRED)

add_executable(RedDotQtApp
    main.cpp
    mainwindow.cpp
    mainwindow.h
    vision.cpp
    vision.h
    tcp.cpp
    tcp.h
)

target_link_libraries(RedDotQtApp
    Qt6::Widgets
    ${OpenCV_LIBS}
    ws2_32
)
