
FILE(GLOB test_srcs test-*.cpp)
ADD_EXECUTABLE(test-seem main.cpp ${test_srcs})
TARGET_LINK_Y(test-seem seem)

IF(FALSE)
SET(HERE ${CMAKE_CURRENT_SOURCE_DIR})
ENABLE_TESTING()

ADD_CUSTOM_TARGET(utest-seem
COMMAND ${CMAKE_CTEST_COMMAND} -V
DEPENDS test-seem)
ENDIF()

