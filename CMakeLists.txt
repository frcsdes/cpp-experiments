cmake_minimum_required(VERSION 3.14)
project(cpp-experiments)

set(CMAKE_CXX_STANDARD 20)

set(SNIPPETS
    base
    collection
    composition
    map
    nice
    pointer
    result
    store
    swizzling
    swizzling2
    timer
)

foreach(S ${SNIPPETS})
    add_executable(${S} ${S}.cpp)
    if(MSVC)
        target_compile_options(${S} PRIVATE /WX /W4)
    else(MSVC)
        target_compile_options(${S} PRIVATE -fconcepts -Werror -Wall -Wextra -Wpedantic)
    endif(MSVC)
endforeach()
