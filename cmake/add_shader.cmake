# https://github.com/ARM-software/vulkan-sdk/blob/master/Sample.cmake
function(add_shader TARGET SHADER)
    find_program(GLSLC glslc)
    set(current-shader-path ${CMAKE_SOURCE_DIR}/src/shaders/${SHADER})
    
    set(current-output-path ${CMAKE_BINARY_DIR}/src/shaders/${SHADER}.spv)
    get_filename_component(current-output-dir ${current-output-path} DIRECTORY)
    file(MAKE_DIRECTORY ${current-output-dir})

    add_custom_command(
        OUTPUT ${current-output-path}
        COMMAND ${GLSLC}
        ARGS ${current-shader-path} -o ${current-output-path}
        DEPENDS ${current-shader-path}
        IMPLICIT_DEPENDS CXX ${current-shader-path}
        VERBATIM)

    set_source_files_properties(${current-output-path} PROPERTIES GENERATED TRUE)
    target_sources(${TARGET} PRIVATE ${current-output-path})
endfunction(add_shader)
