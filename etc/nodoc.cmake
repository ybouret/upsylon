SET(CMAKE_ALLOW_LOOSE_LOOP_CONSTRUCTS ON)

MESSAGE("")
MESSAGE(STATUS "removing doc")

IF(IS_DIRECTORY doc)
	EXECUTE_PROCESS(COMMAND ${CMAKE_COMMAND} -E remove_directory doc)
ENDIF()

MESSAGE("")