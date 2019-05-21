PROJECT(test-z++)

SET(HERE ${CMAKE_CURRENT_SOURCE_DIR})

SET(R_STDCXX "")

IF(Y_FREEBSD)
	IF(Y_GNU)
	SET(R_STDCXX "/usr/local/lib/${Y_CC}/libstdc++.a")
	ENDIF()
ENDIF()

IF(Y_LINUX)
	IF(Y_INTEL)
	SET(ILP "")
		IF(Y32)
		SET(ILP "ia32")
		ENDIF()
		IF(Y64)
		SET(ILP "intel64") 
		ENDIF()
	SET(R_STDCXX "/opt/intel/lib/${ILP}/libintlc.so")
	ENDIF()
ENDIF()

IF(Y_DARWIN)
	IF(Y_GNU)
	SET(R_STDCXX "/opt/local/lib/libgcc/libstdc++.6.dylib")
	ENDIF()
ENDIF()

MESSAGE( STATUS "R_STDCXX=<${R_STDCXX}>" )
ADD_CUSTOM_TARGET(rmodule
COMMAND ${R} CMD SHLIB --preclean --clean ${HERE}/rmodule.cpp $<TARGET_FILE:y> ${R_STDCXX}
WORKING_DIRECTORY ${HERE}
SOURCES rmodule.cpp ../y/r++/r.hpp
DEPENDS ${HERE}/Makevars y
COMMENT "Building R module"
)

ADD_CUSTOM_COMMAND( TARGET rmodule POST_BUILD
COMMAND ${R} --no-save --slave < ${HERE}/rmodule.R
WORKING_DIRECTORY ${HERE}
COMMENT "Testing R module")
