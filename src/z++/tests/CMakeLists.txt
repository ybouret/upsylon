PROJECT(test-z++)
FILE(GLOB test_srcs test-*.cpp)
ADD_EXECUTABLE(test-z++ main.cpp ${test_srcs})
TARGET_LINK_Y( test-z++ z++)

INSTALL( TARGETS test-z++ RUNTIME DESTINATION var/y )
