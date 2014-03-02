cmake_minimum_required(VERSION 2.8.5)
project(dhc)

#SET(CMAKE_VERBOSE_MAKEFILE ON)
#set(VERBOSE, ON)
#SET(DEBUG, ON)

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/CMakeScripts")

find_package(ICU 4.4 REQUIRED)

# set some output directories
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# ensure that the cxx files get compiled with c++11 support enabled
set(CMAKE_CXX_FLAGS "--std=c++11 ${CMAKE_CXX_FLAGS}")

include_directories(
    ${ICU_INCLUDE_DIRS}
)

# build graft as a standalone library
set(GRAFT_LIB_SRC ${CMAKE_CURRENT_SOURCE_DIR}/graft)
add_library (graft
    ${GRAFT_LIB_SRC}/ustream.cpp
    ${GRAFT_LIB_SRC}/scanner.cpp
    ${GRAFT_LIB_SRC}/pattern.cpp
    ${GRAFT_LIB_SRC}/match.cpp
    ${GRAFT_LIB_SRC}/pattern/character.cpp
    ${GRAFT_LIB_SRC}/pattern/choice.cpp
    ${GRAFT_LIB_SRC}/pattern/compound.cpp
    ${GRAFT_LIB_SRC}/pattern/exclude.cpp
    ${GRAFT_LIB_SRC}/pattern/repetition.cpp
    ${GRAFT_LIB_SRC}/pattern/string.cpp
    ${GRAFT_LIB_SRC}/match/character.cpp
    ${GRAFT_LIB_SRC}/match/sequence.cpp
    ${GRAFT_LIB_SRC}/match/string.cpp
)

# add the primary executable
add_executable(dhc
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/lexer/lexer.cpp
)

target_link_libraries(dhc
    graft
    ${ICU_LIBRARIES}
)

# add a test executable
add_executable(hl
    ${CMAKE_CURRENT_SOURCE_DIR}/highlight.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/lexer/lexer.cpp
)

target_link_libraries(hl
    graft
    ${ICU_LIBRARIES}
)
