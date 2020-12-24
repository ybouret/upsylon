find_package (Python COMPONENTS Interpreter Development)
if(Python_FOUND)
	message( STATUS "will use Python" )
	#message( STATUS "Python_LINK_OPTIONS: ${Python_LINK_OPTIONS}" )
	#message( STATUS "Python_LIBRARIES:    ${Python_LIBRARIES}"    )
	include_directories(${Python_INCLUDE_DIRS})
	Python_add_library(spam MODULE spam.cpp)
	set_target_properties(spam
    PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
)
else()
	message( STATUS "will NOT use Python")
endif()
