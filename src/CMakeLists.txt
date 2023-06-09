cmake_minimum_required(VERSION 3.26.1)

add_executable(vulkan-tutorial)
set_property(TARGET vulkan-tutorial PROPERTY CXX_STANDARD 20)
set_property(TARGET vulkan-tutorial PROPERTY CXX_STANDARD_REQUIRED True)
set_property(TARGET vulkan-tutorial PROPERTY INTERPROCEDURAL_OPTIMIZATION True) # Link Time Optimizations

if (NOT MSVC)
    target_compile_options(vulkan-tutorial PRIVATE $<$<CONFIG:Debug>: -Weverything>)
    target_compile_options(vulkan-tutorial PRIVATE $<$<CONFIG:Debug>: -Wno-documentation-unknown-command>)
    target_compile_options(vulkan-tutorial PRIVATE $<$<CONFIG:Debug>: -Wno-documentation>)
    target_compile_options(vulkan-tutorial PRIVATE $<$<CONFIG:Debug>: -Wno-c++98-compat>)
    target_compile_options(vulkan-tutorial PRIVATE $<$<CONFIG:Debug>: -Wno-nonportable-system-include-path>)
    target_compile_options(vulkan-tutorial PRIVATE $<$<CONFIG:Debug>: -Wno-c++98-compat-pedantic>)
    target_compile_options(vulkan-tutorial PRIVATE $<$<CONFIG:Debug>: -Wno-newline-eof>)
else(NOT MSVC)
    target_compile_options(vulkan-tutorial PRIVATE $<$<CONFIG:Debug>: /Wall>)
endif(NOT MSVC)

# Add project *.hpp and *.cpp files
file(GLOB_RECURSE VULKAN_TUTORIAL_SOURCE_FILES ${CMAKE_SOURCE_DIR}/src/*.hpp ${CMAKE_SOURCE_DIR}/src/*.cpp)
target_sources(vulkan-tutorial PRIVATE ${VULKAN_TUTORIAL_SOURCE_FILES})
message(STATUS "Included Files:")
foreach (INCLUDED_FILE ${VULKAN_TUTORIAL_SOURCE_FILES})
    message(STATUS "        " ${INCLUDED_FILE})
endforeach()
message(STATUS "")


# Add project *.vert and *.frag files
add_shader(vulkan-tutorial
    ${CMAKE_SOURCE_DIR}/src/shaders/simple_shader.vert
    ${CMAKE_BINARY_DIR}/src/shaders/simple_shader.vert.spv)
add_shader(vulkan-tutorial
    ${CMAKE_SOURCE_DIR}/src/shaders/simple_shader.frag
    ${CMAKE_BINARY_DIR}/src/shaders/simple_shader.frag.spv)


target_link_libraries(vulkan-tutorial PRIVATE Vulkan::Vulkan)
target_link_libraries(vulkan-tutorial PRIVATE glfw)
target_link_libraries(vulkan-tutorial PRIVATE glm::glm)

target_compile_definitions(vulkan-tutorial PRIVATE GLFW_INCLUDE_VULKAN)
target_compile_definitions(vulkan-tutorial PRIVATE GLM_FORCE_RADIANS)
target_compile_definitions(vulkan-tutorial PRIVATE GLM_FORCE_DEPTH_ZERO_TO_ONE)