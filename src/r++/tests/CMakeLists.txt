PROJECT(test-r++)

SET(HERE ${CMAKE_CURRENT_SOURCE_DIR})

ADD_CUSTOM_TARGET(rmodule ALL
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
