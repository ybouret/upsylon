PROJECT(test-lua++)
FILE(GLOB test_srcs test-*.cpp)
ADD_EXECUTABLE(test-lua++ main.cpp ${test_srcs})
TARGET_LINK_Y( test-lua++ lua++)

INSTALL( TARGETS test-lua++ RUNTIME DESTINATION var/y )
