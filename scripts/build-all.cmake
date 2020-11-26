FILE(GLOB compilers "./forge/*" )

FOREACH( compiler ${compilers} )
	MESSAGE( STATUS "processing ${compiler}" )
	SET(known_compiler FALSE)
	
	#single configuration compilers
	SET(gnu         FALSE)
	SET(intel       FALSE)
	set(clang       FALSE)
	
	#mutliple configurations compilers
	set(msc         FALSE)
	set(xcode       FALSE)
	
	#detecting compiler
	IF( ${compiler} MATCHES ".*gnu.*" )
	SET(known_compiler TRUE)
	SET(gnu            TRUE)
	ENDIF()
	
	IF( ${compiler} MATCHES ".*intel.*")
	SET(known_compiler TRUE)
	SET(intel          TRUE)
	ENDIF()
	
	IF( ${compiler} MATCHES ".*clang.*" )
	SET(known_compiler TRUE)
	SET(clang          TRUE)
	ENDIF()
	
	IF( ${compiler} MATCHES "VisualStudio.*")
	SET(known_compiler TRUE)
	SET(msc            TRUE)
	ENDIF()
	
	IF( ${compiler} MATCHES "Xcode")
	SET(known_compiler TRUE)
	SET(xcode          TRUE)
	ENDIF()
	
	IF( NOT known_compiler )
	MESSAGE( FATAL_ERROR "unknown compiler" )
	ENDIF()
	
	IF(xcode)
		MESSAGE( STATUS "building for Xcode..." )
		EXECUTE_PROCESS(
		COMMAND           ${CMAKE_COMMAND} --build . --config Release --target ALL_BUILD
		WORKING_DIRECTORY ${compiler}
		OUTPUT_QUIET
 		)
	ENDIF()
	
	IF(msc)
		MESSAGE( STATUS "building for Microsoft..." )
		EXECUTE_PROCESS(
		COMMAND           ${CMAKE_COMMAND} --build . --config Release --target ALL_BUILD -- -m
		WORKING_DIRECTORY ${compiler}
		OUTPUT_QUIET
 		)
	ENDIF()
	
	
	
ENDFOREACH()

