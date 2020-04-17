
FILE(GLOB test_srcs test-*.cpp)
ADD_EXECUTABLE(test-json main.cpp ${test_srcs})
TARGET_LINK_Y(test-json json)

SET(HERE ${CMAKE_CURRENT_SOURCE_DIR})
ENABLE_TESTING()
ADD_TEST(db      test-json value ${HERE}/db.js)
ADD_TEST(sample1 test-json value ${HERE}/sample1.js)
ADD_TEST(sample2 test-json value ${HERE}/sample2.js)
ADD_TEST(sample3 test-json value ${HERE}/sample3.js)
ADD_TEST(sample4 test-json value ${HERE}/sample4.js)

ADD_CUSTOM_TARGET(utest-json
COMMAND ${CMAKE_CTEST_COMMAND} -C $<CONFIG>
DEPENDS test-json)

