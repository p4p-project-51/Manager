cmake_minimum_required(VERSION 3.16.0)

# Get the latest git commit timestamp and set SOURCE_DATE_EPOCH
execute_process(
    COMMAND git log -1 --pretty=%ct
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    OUTPUT_VARIABLE SOURCE_DATE_EPOCH
    OUTPUT_STRIP_TRAILING_WHITESPACE
)
set(ENV{SOURCE_DATE_EPOCH} "${SOURCE_DATE_EPOCH}")

STRING(TIMESTAMP BUILD_DATE "%Y-%m-%d" UTC)

include($ENV{IDF_PATH}/tools/cmake/project.cmake)
project(Manager)
