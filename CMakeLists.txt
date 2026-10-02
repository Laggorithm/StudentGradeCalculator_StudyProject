cmake_minimum_required(VERSION 3.20)
project(StudentGradeCalculator C)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

add_executable(StudentGradeCalculator src/main.c)

if(CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(StudentGradeCalculator PRIVATE -Wall -Wextra -Wpedantic)
endif()
