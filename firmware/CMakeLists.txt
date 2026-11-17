cmake_minimum_required(VERSION 3.16)
set(PROJECT_VER "0.9")
include($ENV{IDF_PATH}/tools/cmake/project.cmake)
project(radar_flasher)

# Every image is checked for radar flash-loader content.
idf_build_get_property(build_dir BUILD_DIR)
idf_build_get_property(python PYTHON)
add_custom_target(check_no_loader ALL
        COMMAND ${python} ${CMAKE_CURRENT_LIST_DIR}/scripts/check_no_loader.py
                ${build_dir}/${CMAKE_PROJECT_NAME}.bin
        DEPENDS gen_project_binary
        COMMENT "Checking the image for radar flash-loader content")

# Open build gate: no dev-kit radar-flash module symbols, strings or partitions in the open image.
add_custom_target(check_open_image ALL
        COMMAND ${python} ${CMAKE_CURRENT_LIST_DIR}/scripts/check_open_image.py
                ${build_dir}/${CMAKE_PROJECT_NAME}.elf ${build_dir}/${CMAKE_PROJECT_NAME}.bin
        DEPENDS gen_project_binary
        COMMENT "Open build gate: no dev-kit module content")
