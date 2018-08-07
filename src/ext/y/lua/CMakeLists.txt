PROJECT(lua C)

# from readme.html
SET(lua_srcs lapi.c lcode.c lctype.c ldebug.c ldo.c ldump.c lfunc.c lgc.c llex.c lmem.c lobject.c lopcodes.c lparser.c lstate.c lstring.c ltable.c ltm.c lundump.c lvm.c lzio.c lauxlib.c lbaselib.c lbitlib.c lcorolib.c ldblib.c liolib.c lmathlib.c loslib.c lstrlib.c ltablib.c lutf8lib.c loadlib.c linit.c)
SET(lua_incl lua.h luaconf.h lualib.h lauxlib.h lua.hpp)



ADD_LIBRARY(y-lua STATIC ${lua_srcs})

INSTALL( FILES ${lua_incl} DESTINATION include/y/lua )
INSTALL( TARGETS y-lua ARCHIVE       DESTINATION lib )

ADD_EXECUTABLE(y_lua lua.c)
TARGET_LINK_LIBRARIES(y_lua y-lua)

ADD_EXECUTABLE(y_luac luac.c)
TARGET_LINK_LIBRARIES(y_luac y-lua)

IF(NOT MSVC)
TARGET_LINK_LIBRARIES(y_lua  m)
TARGET_LINK_LIBRARIES(y_luac m)
ENDIF()

SET(LUA_TESTS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/tests)
ENABLE_TESTING()
ADD_TEST(Hello   y_lua ${LUA_TESTS_DIR}/hello.lua)
ADD_TEST(Bisect  y_lua ${LUA_TESTS_DIR}/bisect.lua)
ADD_TEST(Sort    y_lua ${LUA_TESTS_DIR}/sort.lua)
ADD_TEST(Sieve   y_lua ${LUA_TESTS_DIR}/sieve.lua)
ADD_TEST(Life    y_lua ${LUA_TESTS_DIR}/life.lua)
 
