#
# Copyright (C) 2018-2019 QuasarApp.
# Distributed under the lgplv3 software license, see the accompanying
# Everyone is permitted to copy and distribute verbatim copies
# of this license document, but changing it is not allowed.
#

IF(NOT CCACHE_INCLUDE)
    SET(CCACHE_INCLUDE True)
    IF(CMAKE_HOST_SYSTEM_NAME EQUAL Linux)

        execute_process(COMMAND "which" "ccache"
                        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
                        RESULT_VARIABLE which_result
                        OUTPUT_VARIABLE BIN)
        
        IF (${BIN} STREQUAL "")
            message("ccache disabled for ${PROJECT_NAME}")
        ELSE()
            SET(CMAKE_CXX_COMPILER "${BIN} ${CMAKE_CXX_COMPILER}")
            message("ccache enabled for ${CMAKE_CXX_COMPILER}")

        ENDIF (${BIN} STREQUAL "")

    ENDIF(CMAKE_HOST_SYSTEM_NAME EQUAL Linux)

ENDIF(NOT CCACHE_INCLUDE)

