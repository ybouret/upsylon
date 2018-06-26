FILE(GLOB srcs test-*.cpp)

ADD_EXECUTABLE(test-main main.cpp ${srcs})
