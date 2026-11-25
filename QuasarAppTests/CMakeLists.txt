#
# Copyright (C) 2018-2026 QuasarApp.
# Distributed under the lgplv3 software license, see the accompanying
# Everyone is permitted to copy and distribute verbatim copies
# of this license document, but changing it is not allowed.
#

set(CURRENT_PROJECT ${PROJECT_NAME}Test)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 COMPONENTS Core Test REQUIRED)

# The test translation file (en.qm) is generated from the en.ts source of
# this directory by the lrelease tool of the current Qt installation.
get_filename_component(QT_BIN_DIR "${QT_QMAKE_EXECUTABLE}" DIRECTORY)
find_program(LRELEASE_EXECUTABLE NAMES lrelease lrelease.exe HINTS "${QT_BIN_DIR}")

if (NOT LRELEASE_EXECUTABLE)
    message(FATAL_ERROR "The lrelease tool was not found (QT_QMAKE_EXECUTABLE=${QT_QMAKE_EXECUTABLE}). "
                        "It is required for the generation of the test translation file.")
endif()

set(TEST_LANGUAGES_DIR "${CMAKE_CURRENT_BINARY_DIR}/res")
set(TEST_TRANSLATION_QM "${TEST_LANGUAGES_DIR}/en.qm")

add_custom_command(
    OUTPUT ${TEST_TRANSLATION_QM}
    COMMAND ${CMAKE_COMMAND} -E make_directory ${TEST_LANGUAGES_DIR}
    COMMAND ${LRELEASE_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/res/en.ts -qm ${TEST_TRANSLATION_QM}
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/res/en.ts
    COMMENT "Generating the test translation file en.qm"
    VERBATIM
)

file(GLOB SOURCE_CPP
    "*.cpp" "*.h" "*.qrc"
    "units/*.cpp" "units/*.h" "units/*.qrc"
)

set(PUBLIC_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}")
set(PUBLIC_INCLUDE_DIR ${PUBLIC_INCLUDE_DIR} "${CMAKE_SOURCE_DIR}/src")
set(PUBLIC_INCLUDE_DIR ${PUBLIC_INCLUDE_DIR} "${CMAKE_SOURCE_DIR}/src/Core")
set(PUBLIC_INCLUDE_DIR ${PUBLIC_INCLUDE_DIR} "${CMAKE_SOURCE_DIR}/src/Params")
set(PUBLIC_INCLUDE_DIR ${PUBLIC_INCLUDE_DIR} "${CMAKE_SOURCE_DIR}/src/Settings")
set(PUBLIC_INCLUDE_DIR ${PUBLIC_INCLUDE_DIR} "${CMAKE_SOURCE_DIR}/src/Locales")
set(PUBLIC_INCLUDE_DIR ${PUBLIC_INCLUDE_DIR} "${CMAKE_SOURCE_DIR}/src/HelpData")
set(PUBLIC_INCLUDE_DIR ${PUBLIC_INCLUDE_DIR} "${CMAKE_SOURCE_DIR}/src/OptionData")
set(PUBLIC_INCLUDE_DIR ${PUBLIC_INCLUDE_DIR} "${CMAKE_SOURCE_DIR}/src/Utils")
set(PUBLIC_INCLUDE_DIR ${PUBLIC_INCLUDE_DIR} "${CMAKE_CURRENT_SOURCE_DIR}/units")

qt_add_executable(${CURRENT_PROJECT} ${SOURCE_CPP} ${TEST_TRANSLATION_QM})

target_link_libraries(${CURRENT_PROJECT} PRIVATE Qt${QT_VERSION_MAJOR}::Test QuasarApp)

target_include_directories(${CURRENT_PROJECT} PUBLIC ${PUBLIC_INCLUDE_DIR})

target_compile_definitions(${CURRENT_PROJECT} PRIVATE
    QA_TEST_LANGUAGES_DIR="${TEST_LANGUAGES_DIR}"
)

initTests()
addTests("${CURRENT_PROJECT}" ${CURRENT_PROJECT})
