# Following tutorials shown here:
# https://www.youtube.com/watch?v=lr93-_cC8v4&list=PL8327DO66nu9qYVKLDmdLW_84-yE4auCR&index=2&ab_channel=BrendanGalea

cmake_minimum_required(VERSION 3.26.1)
project(vulkan-tutorial)
include(cmake/add_shader.cmake)

find_package(Vulkan REQUIRED)

include(FetchContent)

FetchContent_Declare(glfw GIT_REPOSITORY https://github.com/glfw/glfw.git)
FetchContent_Populate(glfw)
add_subdirectory(${glfw_SOURCE_DIR} ${glfw_BINARY_DIR})

FetchContent_Declare(glm GIT_REPOSITORY  https://github.com/g-truc/glm.git)
FetchContent_Populate(glm)
add_subdirectory(${glm_SOURCE_DIR} ${glm_BINARY_DIR})

add_subdirectory(src)