cmake_minimum_required(VERSION 2.8.5)
project(dhc)

#SET(CMAKE_VERBOSE_MAKEFILE ON)
#set(VERBOSE, ON)
#SET(DEBUG, ON)

#set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/CMakeScripts")

# set some output directories
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# ensure that the cxx files get compiled with c++11 support enabled
set(CMAKE_CXX_FLAGS "--std=c++11 ${CMAKE_CXX_FLAGS}")

# add the primary executable
add_executable(dhc
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/lexer/lexer.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/graft/scanner.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/graft/pattern.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/graft/match.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/graft/pattern/character.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/graft/pattern/choice.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/graft/pattern/compound.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/graft/pattern/exclude.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/graft/pattern/repetition.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/graft/pattern/string.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/graft/match/character.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/graft/match/sequence.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/graft/match/string.cpp
)

# add a test executable
add_executable(hl
    ${CMAKE_CURRENT_SOURCE_DIR}/highlight.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/lexer/lexer.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/graft/scanner.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/graft/pattern.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/graft/match.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/graft/pattern/character.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/graft/pattern/choice.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/graft/pattern/compound.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/graft/pattern/exclude.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/graft/pattern/repetition.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/graft/pattern/string.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/graft/match/character.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/graft/match/sequence.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/graft/match/string.cpp
)
