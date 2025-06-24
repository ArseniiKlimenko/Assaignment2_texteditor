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

add_custom_command(TARGET Assaignment2_texteditor POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_SOURCE_DIR}/caesar.dll"
        $<TARGET_FILE_DIR:Assaignment2_texteditor>)
