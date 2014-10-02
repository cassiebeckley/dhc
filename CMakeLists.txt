cmake_minimum_required(VERSION 2.8.5)
project(dhc)

#SET(CMAKE_VERBOSE_MAKEFILE ON)
#set(VERBOSE, ON)
#SET(DEBUG, ON)

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/CMakeScripts")

find_package(ICU 4.4 REQUIRED)
find_package(Doxygen)

if (DOXYGEN_FOUND)
    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in
    ${CMAKE_BINARY_DIR}/Doxyfile @ONLY)
    add_custom_target(doc
        ${DOXYGEN_EXECUTABLE} ${CMAKE_BINARY_DIR}/Doxyfile
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
        COMMENT "Generating API documentation with Doxygen" VERBATIM
    )
endif(DOXYGEN_FOUND)

# set some output directories
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# ensure that the cxx files get compiled with c++11 support enabled
# and also that the compiler complains a lot
set(CMAKE_CXX_FLAGS "-Wall -Wextra -Werror --std=c++11 ${CMAKE_CXX_FLAGS} -pedantic")

include_directories(
    ${ICU_INCLUDE_DIRS}
    ${CMAKE_CURRENT_SOURCE_DIR}/lib
)

# build graft as a standalone library
set(GRAFT_LIB_SRC ${CMAKE_CURRENT_SOURCE_DIR}/lib/graft)
add_library (graft
    ${GRAFT_LIB_SRC}/scanner.cpp
    ${GRAFT_LIB_SRC}/scanner/character.cpp
    ${GRAFT_LIB_SRC}/pattern.cpp
    ${GRAFT_LIB_SRC}/match.cpp
    ${GRAFT_LIB_SRC}/pattern/character.cpp
    ${GRAFT_LIB_SRC}/pattern/property.cpp
    ${GRAFT_LIB_SRC}/pattern/choice.cpp
    ${GRAFT_LIB_SRC}/pattern/compound.cpp
    ${GRAFT_LIB_SRC}/pattern/exclude.cpp
    ${GRAFT_LIB_SRC}/pattern/repetition.cpp
    ${GRAFT_LIB_SRC}/pattern/string.cpp
    ${GRAFT_LIB_SRC}/pattern/type.cpp
    ${GRAFT_LIB_SRC}/match/character.cpp
    ${GRAFT_LIB_SRC}/match/sequence.cpp
    ${GRAFT_LIB_SRC}/match/string.cpp
)

# add the primary executable
add_executable(dhc
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/lexer/lexer.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/lexer/layout.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/lexer/match/digit.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/lexer/match/integer.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/lexer/match/qualified.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/lexer/match/lit_char.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/lexer/match/lit_string.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/lexer/match/lit_float.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/parser/parser.cpp

    ${CMAKE_CURRENT_SOURCE_DIR}/kernel/expression.cpp

    ${CMAKE_CURRENT_SOURCE_DIR}/kernel/expression/application.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/kernel/expression/case.cpp

    ${CMAKE_CURRENT_SOURCE_DIR}/kernel/expression/value.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/kernel/expression/value/function.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/kernel/expression/value/uservalue.cpp

    ${CMAKE_CURRENT_SOURCE_DIR}/kernel/type.cpp

    ${CMAKE_CURRENT_SOURCE_DIR}/ast/module.cpp
)

message("ICU: " ${ICU_LIBRARIES})

target_link_libraries(dhc
    graft
    ${ICU_LIBRARIES}
    # TODO: remove, not cross-platform
    ${ICU_ROOT}/lib/icuin.dll.a
    ${ICU_ROOT}/lib/icuio.dll.a
)
