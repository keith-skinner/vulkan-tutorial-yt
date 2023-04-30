cmake_minimum_required(VERSION 3.26.1)

add_executable(vulkan-tutorial)
set_property(TARGET vulkan-tutorial PROPERTY CXX_STANDARD 20)
set_property(TARGET vulkan-tutorial PROPERTY CXX_STANDARD_REQUIRED True)
set_property(TARGET vulkan-tutorial PROPERTY INTERPROCEDURAL_OPTIMIZATION True) # Link Time Optimizations

if (NOT MSVC)
    target_compile_options(
        vulkan-tutorial
    PRIVATE
        $<$<CONFIG:Debug>: -Weverything>
        $<$<CONFIG:Debug>: -Wno-documentation-unknown-command>
        $<$<CONFIG:Debug>: -Wno-documentation>
        $<$<CONFIG:Debug>: -Wno-c++98-compat>
        $<$<CONFIG:Debug>: -Wno-nonportable-system-include-path>
        $<$<CONFIG:Debug>: -Wno-c++98-compat-pedantic>
        $<$<CONFIG:Debug>: -Wno-newline-eof>
    )
else()
    target_compile_options(
        vulkan-tutorial
    PRIVATE
        $<$<CONFIG:Debug>: /Wall>
    )
endif()

file(GLOB_RECURSE VULKAN_TUTORIAL_FILES *.hpp *.cpp)
target_sources( vulkan-tutorial PRIVATE ${VULKAN_TUTORIAL_FILES})
message(STATUS "Included Files:" )
foreach (INCLUDED_FILE ${VULKAN_TUTORIAL_FILES})
    message("        " ${INCLUDED_FILE})
endforeach()

target_link_libraries(vulkan-tutorial PRIVATE Vulkan::Vulkan)
target_link_libraries(vulkan-tutorial PRIVATE glfw)
target_link_libraries(vulkan-tutorial PRIVATE glm::glm)

target_compile_definitions(
    vulkan-tutorial
PRIVATE
    GLFW_INCLUDE_VULKAN
    GLM_FORCE_RADIANS
    GLM_FORCE_DEPTH_ZERO_TO_ONE
)