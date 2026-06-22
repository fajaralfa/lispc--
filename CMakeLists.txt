cmake_minimum_required(VERSION 3.10)

project(LispInterpreter)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(SOURCES
    src/main.cpp
    src/tokenizer.cpp
    src/parser.cpp
    src/vm.cpp
)

add_executable(lisp ${SOURCES})

target_include_directories(lisp PRIVATE ${CMAKE_SOURCE_DIR}/src)