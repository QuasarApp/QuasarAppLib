#
# Copyright (C) 2018-2019 QuasarApp.
# Distributed under the lgplv3 software license, see the accompanying
# Everyone is permitted to copy and distribute verbatim copies
# of this license document, but changing it is not allowed.
#

cmake_minimum_required(VERSION 3.1)

IF(NOT QUASARAPP_LIB)
    SET(QUASARAPP_LIB True)
    include(QuasarAppOut.cmake)

    target_link_libraries(${PROJECT_NAME}  ${QUASARAPP_LIB_NAME})
    target_include_directories(${PROJECT_NAME} ${CMAKE_CURRENT_SOURCE_DIR})

ENDIF(NOT QUASARAPP_LIB)

