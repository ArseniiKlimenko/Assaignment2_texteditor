cmake_minimum_required(VERSION 3.31)
project(Assaignment2_texteditor)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_WIN32_EXECUTABLE FALSE)

add_executable(Assaignment2_texteditor
        main.cpp
        TextEditor.h
        Text.h
        Line.h
        Caesarload.h
        dll3.h
)