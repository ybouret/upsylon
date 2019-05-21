PROJECT(test-z++)

SET(HERE ${CMAKE_CURRENT_SOURCE_DIR})

SET(R_STDCXX "")
IF(Y_FREEBSD)
	IF(Y_GNU)
	SET(R_STDCXX "/usr/local/lib/${Y_CC}/libstdc++.a")
	ENDIF()
ENDIF()

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
