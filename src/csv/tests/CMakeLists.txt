
FILE(GLOB test_srcs test-*.cpp)
ADD_EXECUTABLE(test-csv main.cpp ${test_srcs})
TARGET_LINK_Y(test-csv csv)

SET(HERE ${CMAKE_CURRENT_SOURCE_DIR})
ENABLE_TESTING()
ADD_TEST(CSV test-csv value)

ADD_CUSTOM_TARGET(utest-csv
COMMAND ${CMAKE_CTEST_COMMAND} -C $<CONFIG>
DEPENDS test-csv)

INSTALL( TARGETS test-csv RUNTIME DESTINATION var/y )
