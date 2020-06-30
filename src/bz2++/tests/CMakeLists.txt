PROJECT(test-bz2++)
FILE(GLOB test_srcs test-*.cpp)
ADD_EXECUTABLE(test-bz2++ main.cpp ${test_srcs})
TARGET_LINK_Y( test-bz2++ bz2++)

INSTALL( TARGETS test-bz2++ RUNTIME DESTINATION var )
