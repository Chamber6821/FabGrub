if (NOT DEFINED PATH)
    message(FATAL_ERROR "Variable PATH undefined. Use flag -DPATH=<paths> to create folders and their parents")
endif ()

file(MAKE_DIRECTORY ${PATH})
