cmake_minimum_required(VERSION 3.15)

project(AIJobSearch
    VERSION 1.0.0
    DESCRIPTION "Resume and job skill matching application"
    LANGUAGES C
)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

add_library(job_matcher_lib
    src/job_matcher.c
)

target_include_directories(job_matcher_lib
    PUBLIC
        ${PROJECT_SOURCE_DIR}/include
)

add_executable(ai_job_search
    src/main.c
)

target_link_libraries(ai_job_search
    PRIVATE
        job_matcher_lib
)

enable_testing()

add_executable(test_job_matcher
    tests/test_job_matcher.c
)

target_link_libraries(test_job_matcher
    PRIVATE
        job_matcher_lib
)

add_test(
    NAME JobMatcherTests
    COMMAND test_job_matcher
)
