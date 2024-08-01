#
# Copyright (C) 2018-2024 QuasarApp.
# Distributed under the lgplv3 software license, see the accompanying
# Everyone is permitted to copy and distribute verbatim copies
# of this license document, but changing it is not allowed.
#

if(DEFINED QUASARAPP_LIBRARY)
  return()
else()
  set(QUASARAPP_LIBRARY 1)
  add_definitions(-DQUASARAPP_LIBRARY)
endif()

project(QuasarApp)

if(TARGET ${PROJECT_NAME})
  message("The ${PROJECT_NAME} arledy included in main Project")
  return()
endif()

cmake_minimum_required(VERSION 3.1)


include(CMake/QuasarApp.cmake)
include(CMake/Version.cmake)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
option(BUILD_SHARED_LIBS "Enable or disable shared libraryes" OFF)

if (NOT QT_VERSION_MAJOR)
    find_package(QT NAMES Qt6 Qt5 COMPONENTS Core REQUIRED)
endif()

find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core REQUIRED)

option(QA_ALLOW_NOT_SUPPORTED_OPTIONS "Enable for allow any command line options" ON)
option(QA_DISABLE_LOG "Disabled all logs (force sets verbose to 0)" OFF)

if (QA_DISABLE_LOG)
    add_definitions(-DQA_DISABLE_LOG)
endif()

if (QA_ALLOW_NOT_SUPPORTED_OPTIONS)
    add_definitions(-DQA_ALLOW_NOT_SUPPORTED_OPTIONS)
endif()

file(GLOB SOURCE_CPP
    "*.cpp" "*.h"
)

add_library(${PROJECT_NAME} ${SOURCE_CPP})
target_link_libraries(${PROJECT_NAME} PUBLIC Qt${QT_VERSION_MAJOR}::Core)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

setVersion(1 5 4)

initAll()
make_directory("${CMAKE_CURRENT_SOURCE_DIR}/Distro")

addDoc(${PROJECT_NAME}Docs ${CMAKE_CURRENT_SOURCE_DIR}/doxygen.conf)
