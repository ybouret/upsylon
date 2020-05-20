PROJECT(test-r++)

SET(HERE ${CMAKE_CURRENT_SOURCE_DIR})

ADD_CUSTOM_TARGET(rmodule ALL
COMMAND ${R} CMD SHLIB --preclean --clean ${HERE}/rmodule.cpp $<TARGET_FILE:y> ${R_STDCXX}
WORKING_DIRECTORY ${HERE}
SOURCES rmodule.cpp ../y/r++/r.hpp ${HERE}/rmodule.R ${HERE}/Makevars
DEPENDS y
COMMENT "Building R module"
)

IF(TRUE)
ADD_CUSTOM_COMMAND( TARGET rmodule POST_BUILD
COMMAND ${R} --no-save --slave < ${HERE}/rmodule.R
WORKING_DIRECTORY ${HERE}
COMMENT "Testing R module")
ENDIF()

IF(FALSE)
ADD_CUSTOM_TARGET(utest-r++
COMMAND ${R} -s --no-save --slave < ${HERE}/rmodule.R
WORKING_DIRECTORY ${HERE}
DEPENDS rmodule
COMMENT "Testing R module")
ENDIF()
