cmake_minimum_required(VERSION 3.10)
project(pr2 C)

set(CMAKE_C_STANDARD 99)

add_executable(pr2
    src/main.c
    src/users.c
    src/processes.c
    src/logging.c
    src/errors.c
)
target_include_directories(pr2 PRIVATE include)
