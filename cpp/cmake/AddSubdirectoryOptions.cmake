# splits a package option
function(parse_option OPTION)
    string(REGEX MATCH "^[^ ]+" OPTION_KEY ${OPTION})
    string(LENGTH ${OPTION} OPTION_LENGTH)
    string(LENGTH ${OPTION_KEY} OPTION_KEY_LENGTH)
    if (OPTION_KEY_LENGTH STREQUAL OPTION_LENGTH)
        # no value for key provided, assume user wants to set option to "ON"
        set(OPTION_VALUE "ON")
    else()
        math(EXPR OPTION_KEY_LENGTH "${OPTION_KEY_LENGTH}+1")
        string(SUBSTRING ${OPTION} "${OPTION_KEY_LENGTH}" "-1" OPTION_VALUE)
    endif()
    set(OPTION_KEY "${OPTION_KEY}" PARENT_SCOPE)
    set(OPTION_VALUE "${OPTION_VALUE}" PARENT_SCOPE)
endfunction()


function(add_subdirectory_options)
    set(oneValueArgs PATH)
    set(multiValueArgs OPTIONS)
    cmake_parse_arguments(SUBBY_ARGS "" "${oneValueArgs}" "${multiValueArgs}" "${ARGN}")

    foreach(OPTION ${SUBBY_ARGS_OPTIONS})
        parse_option(${OPTION}) # OPTION_KEY OPTION_value # CPM_ARGS_NAME
        set(PRESERVE_${OPTION_KEY} ${${OPTION_KEY}})

        set(${OPTION_KEY} ${OPTION_VALUE})
        message("Setting ${OPTION_KEY} = ${OPTION_VALUE}  (${PRESERVE_${OPTION_KEY}})")
    endforeach()

    # Adds the subdirectory
    add_subdirectory(${SUBBY_ARGS_PATH})

    # Revert
    foreach(OPTION ${SUBBY_ARGS_OPTIONS})
        parse_option(${OPTION}) # OPTION_KEY OPTION_value # CPM_ARGS_NAME
        set(${OPTION_KEY} ${PRESERVE_${OPTION_KEY}})
        message("Reverting ${OPTION_KEY} = ${PRESERVE_${OPTION_KEY}}  (${OPTION_VALUE})")
        unset(PRESERVE_${OPTION_KEY})
    endforeach()

endfunction()
