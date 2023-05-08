# https://github.com/ARM-software/vulkan-sdk/blob/master/Sample.cmake
function(add_shader TARGET SHADER SPV)
    get_filename_component(current-output-dir ${SPV} DIRECTORY)
    file(MAKE_DIRECTORY ${current-output-dir})

    find_program(GLSLC glslc)
    add_custom_command(
        OUTPUT ${SPV}
        COMMAND ${GLSLC}
        ARGS ${SHADER} -o ${SPV}
        DEPENDS ${SHADER}
        IMPLICIT_DEPENDS CXX ${SHADER}
        COMMENT "Rebuilding: ${SPV}"
        VERBATIM)

    set_source_files_properties(${SPV} PROPERTIES GENERATED TRUE)
    target_sources(${TARGET} PRIVATE ${SHADER} ${SPV})
endfunction(add_shader)
