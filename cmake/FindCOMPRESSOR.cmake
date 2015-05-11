include(FindPackageMessage)
include(FindPackageHandleStandardArgs)

set(COMPRESSOR_SEARCH_PREFIX "" CACHE PATH "additional path to search for 7-zip or zip executable")

find_program(COMPRESSOR_EXECUTABLE
    NAMES 7z zip 7za
    HINTS ${COMPRESSOR_SEARCH_PREFIX}
)

#
# usage: ZIP_FILES(output_file input [input2 input3 ...])
#
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

    get_filename_component(compressor_basename ${COMPRESSOR_EXECUTABLE} NAME_WE)

    # create zip archive using 7-zip
    if(compressor_basename STREQUAL 7z OR compressor_basename STREQUAL 7za)
        add_custom_command(
            DEPENDS ${input_files}
            OUTPUT ${output_file_use}
            COMMAND ${COMPRESSOR_EXECUTABLE} a -tzip ${output_file_use} ${ARGN}
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        )
    # create zip archive using zip
    elseif(compressor_basename STREQUAL zip)
        add_custom_command(
            DEPENDS ${input_files}
            OUTPUT ${output_file_use}
            COMMAND ${COMPRESSOR_EXECUTABLE} ${output_file_use} ${ARGN}
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        )
    else()
        message(FATAL_ERROR "invalid compressor program base name: ${compressor_basename}")
    endif()

    string(REPLACE "/" "___" zip_target ${output_file_use})
    add_custom_target(ZIP_TARGET_${zip_target} ALL
        DEPENDS ${output_file_use}
    )
endfunction()

FIND_PACKAGE_HANDLE_STANDARD_ARGS(COMPRESSOR
    REQUIRED_VARS COMPRESSOR_EXECUTABLE
)
