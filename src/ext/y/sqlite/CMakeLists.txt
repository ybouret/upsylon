PROJECT(sqlite3 C)

IF(Y_SOLARIS)
ADD_DEFINITIONS(-std=c99)
ENDIF()

ADD_LIBRARY(y-sqlite3 STATIC sqlite3.c)

INSTALL( FILES sqlite3.h sqlite3.h DESTINATION include/y/sqlite )
INSTALL( TARGETS y-sqlite3 ARCHIVE DESTINATION lib )

ADD_EXECUTABLE(y_sqlite shell.c)
TARGET_LINK_LIBRARIES(y_sqlite y-sqlite3)
IF(Y_LINUX)
	TARGET_LINK_LIBRARIES(y_sqlite dl pthread)
ENDIF()

IF(Y_FREEBSD)
	TARGET_LINK_LIBRARIES(y_sqlite pthread)
ENDIF()
