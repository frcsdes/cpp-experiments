cmake_minimum_required(VERSION 3.14)
project(cpp-experiments)

set(CMAKE_CXX_STANDARD 20)

set(SNIPPETS
    base
    collection
    composition
    nice
    pointer
    result
    store
    timer
    swizzling
    swizzling2
)

foreach(S ${SNIPPETS})
    add_executable(${S} ${S}.cpp)
endforeach()
