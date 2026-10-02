cmake_minimum_required(VERSION 3.20)
project(StudentGradeCalculator C)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

add_library(grade_core src/app.c src/input.c src/student.c src/grading.c)
target_include_directories(grade_core PUBLIC src)

add_executable(StudentGradeCalculator src/main.c)
target_link_libraries(StudentGradeCalculator PRIVATE grade_core)

if(CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(StudentGradeCalculator PRIVATE -Wall -Wextra -Wpedantic)
    target_compile_options(grade_core PRIVATE -Wall -Wextra -Wpedantic)
endif()
