
FILE(GLOB test_srcs test-*.cpp)
ADD_EXECUTABLE(test-sqlite++ main.cpp ${test_srcs})
TARGET_LINK_Y( test-sqlite++ sqlite++ sqlite3)

ENABLE_TESTING()

ADD_TEST(sql test-sqlite++ sql ${CMAKE_CURRENT_SOURCE_DIR}/chinook.db)

