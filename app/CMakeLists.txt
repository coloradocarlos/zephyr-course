cmake_minimum_required(VERSION 3.20.0)

set(DTC_OVERLAY_FILE
    "${CMAKE_CURRENT_SOURCE_DIR}/boards/osd32mp1_brk.overlay"
    "${CMAKE_CURRENT_SOURCE_DIR}/app.overlay"
)

# This will allow west to find the board named my_little_board
list(APPEND BOARD_ROOT ${CMAKE_CURRENT_SOURCE_DIR})
# Repo-root bindings: dts/bindings/doorstep,somedriver.yaml (fixes DT_HAS_* Kconfig symbols)
list(APPEND DTS_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/..)

find_package(Zephyr REQUIRED)
project(my_zephyr_app)

zephyr_include_directories(include)

target_sources(app PRIVATE
#	src/main.cpp
#   src/hello_world.c
    src/driver_demo.cpp
)

add_subdirectory(drivers/doorstep_somedriver)
