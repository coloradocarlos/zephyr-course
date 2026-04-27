cmake_minimum_required(VERSION 3.20.0)

set(DTC_OVERLAY_FILE
    "${CMAKE_CURRENT_SOURCE_DIR}/boards/osd32mp1_brk.overlay"
)

# This will allow west to find the board named my_little_board
list(APPEND BOARD_ROOT ${CMAKE_CURRENT_SOURCE_DIR})

find_package(Zephyr REQUIRED)
project(my_zephyr_app)

target_sources(app PRIVATE
	src/main.cpp
)
