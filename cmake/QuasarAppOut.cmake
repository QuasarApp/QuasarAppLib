#
# Copyright (C) 2018-2019 QuasarApp.
# Distributed under the lgplv3 software license, see the accompanying
# Everyone is permitted to copy and distribute verbatim copies
# of this license document, but changing it is not allowed.
#

IF(NOT CMAKE_BUILD_TYPE)
  SET(CMAKE_BUILD_TYPE Release)
ENDIF(NOT CMAKE_BUILD_TYPE)

SET(QUASARAPP_LIB_NAME QuasarApp)

IF(CMAKE_BUILD_TYPE EQUAL Release)
    SET(QUASARAPP_LIB_OUTPUT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/build/release)
ELSE()
    SET(QUASARAPP_LIB_OUTPUT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/build/debug)
ENDIF(CMAKE_BUILD_TYPE EQUAL Release)

message(QUASARAPP_LIB_OUTPUT_DIR = ${QUASARAPP_LIB_OUTPUT_DIR})

include(cmake/ccache.cmake)
