cmake_minimum_required(VERSION 3.20)
project(StudentGradeCalculator C)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

add_library(grade_core src/app.c src/input.c)
target_include_directories(grade_core PUBLIC src)

add_executable(StudentGradeCalculator src/main.c)
target_link_libraries(StudentGradeCalculator PRIVATE grade_core)

if(CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(StudentGradeCalculator PRIVATE -Wall -Wextra -Wpedantic)
    target_compile_options(grade_core PRIVATE -Wall -Wextra -Wpedantic)
endif()

# Temporary stage-one tester. Remove at the beginning of stage two.
include(CTest)
if(BUILD_TESTING)
    add_executable(Stage1Tester tests/stage1_tester.c)
    target_link_libraries(Stage1Tester PRIVATE grade_core)
    if(CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
        target_compile_options(Stage1Tester PRIVATE -Wall -Wextra -Wpedantic)
    endif()
    add_test(NAME Stage1Input COMMAND Stage1Tester)
endif()
