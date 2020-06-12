
INCLUDE_DIRECTORIES(../../math ../../main/tests)

FILE(GLOB test_srcs test-*.cpp)
ADD_EXECUTABLE(test-spade main.cpp ${test_srcs})
TARGET_LINK_Y(test-spade spade)

SET(HERE ${CMAKE_CURRENT_SOURCE_DIR})
ENABLE_TESTING()
ADD_TEST(types        test-spade types)
ADD_TEST(layout       test-spade layout)
ADD_TEST(field        test-spade field)
ADD_TEST(dispatch     test-spade dispatch)
ADD_TEST(fragment     test-spade fragment)
ADD_TEST(ghosts       test-spade ghosts)
ADD_TEST(tessellation test-spade tessellation)
ADD_TEST(transfer     test-spade transfer)
ADD_TEST(workspace    test-spade workspace)
ADD_TEST(mesh         test-spade mesh)
ADD_TEST(vtk          test-spade vtk)
ADD_TEST(dct          test-spade dct)

ADD_CUSTOM_TARGET(utest-spade
COMMAND ${CMAKE_CTEST_COMMAND} -C $<CONFIG>
DEPENDS test-spade)

