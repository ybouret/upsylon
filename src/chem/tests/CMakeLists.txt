INCLUDE_DIRECTORIES(../../main/tests)
FILE(GLOB srcs test-*.cpp)
FILE(GLOB data *.lua)

ADD_EXECUTABLE(test-chem main.cpp ${srcs} ${data})
TARGET_LINK_Y(test-chem chem)

SET( HERE ${CMAKE_CURRENT_SOURCE_DIR} )
ENABLE_TESTING()
ADD_TEST(Species     test-chem species)
ADD_TEST(Library     test-chem lib)
ADD_TEST(Equilibrium test-chem eq)
ADD_TEST(Equilibria  test-chem cs)
ADD_TEST(Balance     test-chem balance)
ADD_TEST(Normalize   test-chem normalize)
ADD_TEST(Boot        test-chem boot)
ADD_TEST(BootAB      test-chem bootAB)
ADD_TEST(Lua:Sample  test-chem lua ${HERE}/sample.lua)
ADD_TEST(Lua:Acetic  test-chem lua ${HERE}/acetic.lua)
ADD_TEST(Lua:Citric  test-chem lua ${HERE}/citric.lua)
ADD_TEST(Kinetics    test-chem kinetics)

ADD_CUSTOM_TARGET(utest-chem
COMMAND ${CMAKE_CTEST_COMMAND} -V
DEPENDS test-chem)
