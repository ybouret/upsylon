if(NOT Y_SUNOS)
if(${CMAKE_VERSION} VERSION_GREATER_EQUAL "3.12" )

	find_package (Python COMPONENTS Interpreter Development)
	if(Python_FOUND)
		message( STATUS "Python was found..."    )
		message( STATUS "[${Python_EXECUTABLE}]" )
	endif()
	
	if( Python_FOUND AND ("${Python_VERSION}" VERSION_GREATER_EQUAL "3") )
		set(USE_PYTHON ON)
		message( STATUS "...and will use Python ${Python_VERSION}" )
	else()
		set(USE_PYTHON OFF)
		message( STATUS "...and will NOT use Python ${Python_VERSION}" )
	endif()
	
	if(USE_PYTHON)
		#adjust environment
		include_directories(${Python_INCLUDE_DIRS})
		
		#create extension
		Python_add_library(spam MODULE spam.cpp)
		
		#copy module next to python code
		ADD_CUSTOM_COMMAND( TARGET spam POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:spam> ${CMAKE_CURRENT_SOURCE_DIR}/$<TARGET_FILE_NAME:spam>
		COMMAND ${Python_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/call-spam.py
		COMMENT "Copy spam module...")
	endif()

endif()

endif()

