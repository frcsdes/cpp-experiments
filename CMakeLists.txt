cmake_minimum_required(VERSION 3.14)
project(cpp-experiments)

set(CMAKE_CXX_STANDARD 17)

set(SNIPPETS
    collection
    composition
    nice
    pointer
    result
    store
)

foreach(S ${SNIPPETS})
    add_executable(${S} ${S}.cpp)
endforeach()
