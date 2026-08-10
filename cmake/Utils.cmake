function(copy_shaders)
    cmake_parse_arguments(ARG "" "DESTINATION" "SOURCES" ${ARGN})

    if(NOT ARG_SOURCES)
        message(FATAL_ERROR "copy_shaders: no SOURCES given")
    endif()
    if(NOT ARG_DESTINATION)
        set(ARG_DESTINATION ${CMAKE_BINARY_DIR}/shaders)
    endif()

    set(shader_outputs)

    foreach(shader_source ${ARG_SOURCES})
        get_filename_component(shader_name ${shader_source} NAME)
        get_filename_component(shader_source_abs ${shader_source} ABSOLUTE BASE_DIR ${CMAKE_CURRENT_SOURCE_DIR})
        set(shader_output ${ARG_DESTINATION}/${shader_name})

        add_custom_command(
            OUTPUT ${shader_output}
            COMMAND ${CMAKE_COMMAND} -E make_directory ${ARG_DESTINATION}
            COMMAND ${CMAKE_COMMAND} -E copy ${shader_source_abs} ${shader_output}
            DEPENDS ${shader_source_abs}
            COMMENT "Copying ${shader_name} to ${ARG_DESTINATION}"
            VERBATIM
        )
        list(APPEND shader_outputs ${shader_output})
    endforeach()

    add_custom_target(shaders ALL DEPENDS ${shader_outputs})
endfunction()
