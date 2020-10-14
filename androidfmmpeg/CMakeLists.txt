cmake_minimum_required(VERSION 3.10.2)


add_library(
        muzi-lib
        SHARED
        src/main/cpp/muzi-lib.cpp
        src/main/cpp/JavaListener.cpp
)

find_library(
        log-lib
        log)


target_link_libraries(
        muzi-lib
        OpenSLES
        ${log-lib}
)