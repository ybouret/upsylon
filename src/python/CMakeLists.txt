find_package (Python COMPONENTS Interpreter Development)
if(Python_FOUND)
	message( STATUS "will use Python" )
	#message( STATUS "Python_LINK_OPTIONS: ${Python_LINK_OPTIONS}" )
	#message( STATUS "Python_LIBRARIES:    ${Python_LIBRARIES}"    )
	include_directories(${Python_INCLUDE_DIRS})
	Python_add_library(spam MODULE spam.cpp)
	ADD_CUSTOM_COMMAND( TARGET spam POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:spam> ${CMAKE_CURRENT_SOURCE_DIR}/$<TARGET_FILE_NAME:spam>
	COMMENT "Copy spam module...")
else()
	message( STATUS "will NOT use Python")
endif()
