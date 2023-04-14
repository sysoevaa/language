cmake_minimum_required(VERSION 3.20)
project(language)

set(CMAKE_CXX_STANDARD 20)

include_directories(.)

add_executable(language
        Lexeme.cpp
        Lexeme.h
        main.cpp
        Syntax.h
        Syntax.cpp
        TID.h
        TID.cpp
        ExpCheck.cpp
        ExpCheck.h Overloader.cpp Overloader.h PolizGenerator.cpp PolizGenerator.h)
