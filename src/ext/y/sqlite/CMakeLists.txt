PROJECT(sqlite3 C)

#ADD_DEFINITIONS()

ADD_LIBRARY(y-sqlite3 STATIC sqlite3.c)

INSTALL( FILES sqlite3.h sqlite3.h DESTINATION include/y/sqlite )
INSTALL( TARGETS y-sqlite3 ARCHIVE DESTINATION lib )

ADD_EXECUTABLE(y_sqlite shell.c)
TARGET_LINK_LIBRARIES(y_sqlite y-sqlite3)

