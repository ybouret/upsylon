INCLUDE_DIRECTORIES(../../main/tests)
FILE(GLOB srcs test-*.cpp)

ADD_EXECUTABLE(test-m main.cpp ${srcs} ../../main/tests/support.hpp)
TARGET_LINK_Y(test-m m)

ENABLE_TESTING()
ADD_TEST(Types      test-m types)
ADD_TEST(TAO        test-m tao)
#ADD_TEST(TAO2       test-m tao2)
#ADD_TEST(TAO3       test-m tao3)
ADD_TEST(LU         test-m LU)
ADD_TEST(DataSet    test-m data_set ${CMAKE_CURRENT_SOURCE_DIR}/sample.txt)
ADD_TEST(Derivative test-m drvs)
ADD_TEST(ZFind      test-m zfind)
ADD_TEST(Adjoint    test-m adjoint)
ADD_TEST(DiagSymm   test-m diag_symm)
ADD_TEST(SVD        test-m svd)
ADD_TEST(Diag       test-m diag)
ADD_TEST(ODE        test-m ode)
ADD_TEST(RK4        test-m rk4)
ADD_TEST(Functions  test-m functions)
ADD_TEST(Intg       test-m intg)
ADD_TEST(Michaelis  test-m michaelis)
ADD_TEST(ODE_expl   test-m ode_expl)
ADD_TEST(ODE_impl   test-m ode_impl)
ADD_TEST(Shapes2d   test-m shapes2d)
ADD_TEST(Bracket    test-m bracket)


ADD_CUSTOM_TARGET(utest-m
COMMAND ${CMAKE_CTEST_COMMAND} -V
DEPENDS test-m)
