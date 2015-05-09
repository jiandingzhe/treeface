include(FindPackageMessage)
include(FindPackageHandleStandardArgs)

set(ZIP_SEARCH_PREFIX "" CACHE PATH "additional path to search for zip executable")

find_program(ZIP_EXECUTABLE zip
    HINTS ${ZIP_SEARCH_PREFIX}
)


function(ZIP_FILES OUTPUT_FILE)
    foreach(curr_file ${ARGN})
        if(IS_ABSOLUTE ${curr_file})
            list(APPEND input_files ${curr_file})
        else()
            list(APPEND input_files ${CMAKE_CURRENT_SOURCE_DIR}/${curr_file})
        endif()
    endforeach(curr_file)

    if(IS_ABSOLUTE ${OUTPUT_FILE})
        set(output_file_use ${OUTPUT_FILE})
    else()
        set(output_file_use ${CMAKE_CURRENT_BINARY_DIR}/${OUTPUT_FILE})
    endif()

    add_custom_target(${OUTPUT_FILE} ALL
        DEPENDS ${input_files}
        COMMAND ${ZIP_EXECUTABLE} ${output_file_use} ${ARGN}
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    )
endfunction()

FIND_PACKAGE_HANDLE_STANDARD_ARGS(ZIP
    REQUIRED_VARS ZIP_EXECUTABLE
)
