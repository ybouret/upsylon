INCLUDE_DIRECTORIES(../../main/tests)
FILE(GLOB srcs test-*.cpp)

ADD_EXECUTABLE(test-chem main.cpp ${srcs})
TARGET_LINK_Y(test-chem chem)

ENABLE_TESTING()
ADD_TEST(Species test-chem species)
ADD_TEST(Library test-chem lib)

ADD_CUSTOM_TARGET(utest-chem
COMMAND ${CMAKE_CTEST_COMMAND} -V
DEPENDS test-chem)
