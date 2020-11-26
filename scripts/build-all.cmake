INCLUDE(ProcessorCount)
ProcessorCount(NP)
FILE(GLOB compilers "./forge/*" )
LIST(SORT compilers)

FOREACH( compiler ${compilers} )
	MESSAGE( STATUS "${compiler}" )
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
		
	#processing
	IF( gnu OR intel OR clang )
		FILE(GLOB generators "${compiler}/*" )
		FOREACH( generator ${generators} )
 			FILE(GLOB configs "${generator}/*")
			FOREACH( config ${configs} )
				MESSAGE(STATUS "[${config}]...")
				EXECUTE_PROCESS(
				COMMAND           ${CMAKE_COMMAND} --build . --target all -- -j ${NP}
				WORKING_DIRECTORY ${config}
				OUTPUT_QUIET
				)
			ENDFOREACH()
		ENDFOREACH()
	ENDIF()
	
	IF(xcode)
		MESSAGE( STATUS "[Xcode/Release]..." )
 		EXECUTE_PROCESS(
		COMMAND           ${CMAKE_COMMAND} --build . --config Release --target ALL_BUILD
		WORKING_DIRECTORY ${compiler}
		OUTPUT_QUIET
 		)
	ENDIF()
	
	IF(msc)
		MESSAGE( STATUS "[Microsoft/Release]..." )
 		EXECUTE_PROCESS(
		COMMAND           ${CMAKE_COMMAND} --build . --config Release --target ALL_BUILD -- -m
		WORKING_DIRECTORY ${compiler}
		OUTPUT_QUIET
 		)
	ENDIF()
	
ENDFOREACH()

