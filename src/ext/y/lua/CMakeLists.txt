PROJECT(lua C)

# from readme.html
SET(lua_srcs 
lapi.c lcode.c lctype.c ldebug.c ldo.c ldump.c lfunc.c lgc.c llex.c lmem.c lobject.c lopcodes.c lparser.c lstate.c lstring.c ltable.c ltm.c lundump.c lvm.c lzio.c lauxlib.c lbaselib.c lcorolib.c ldblib.c liolib.c lmathlib.c loadlib.c loslib.c lstrlib.c ltablib.c lutf8lib.c linit.c
)
SET(lua_incl
lua.h luaconf.h lualib.h lauxlib.h lua.hpp)

IF(Y_SUNOS AND Y_GNU)
ADD_DEFINITIONS(-std=c99)
ENDIF()


ADD_LIBRARY(y-lua STATIC ${lua_srcs})

INSTALL( FILES ${lua_incl} DESTINATION include/y/lua )
INSTALL( TARGETS y-lua ARCHIVE       DESTINATION lib )

ADD_EXECUTABLE(y_lua lua.c)
TARGET_LINK_LIBRARIES(y_lua y-lua)
INSTALL(TARGETS y_lua RUNTIME DESTINATION bin)


IF(NOT MSVC)
	TARGET_LINK_LIBRARIES(y_lua  m)
ENDIF()

SET(LUA_TESTS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/tests)

ADD_CUSTOM_COMMAND(TARGET y_lua
POST_BUILD
COMMAND y_lua ${LUA_TESTS_DIR}/hello.lua
COMMAND y_lua ${LUA_TESTS_DIR}/bisect.lua
COMMAND y_lua ${LUA_TESTS_DIR}/sort.lua
COMMAND y_lua ${LUA_TESTS_DIR}/sieve.lua
COMMENT "Testing Lua"
)


