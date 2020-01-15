PROJECT(test-m)
INCLUDE_DIRECTORIES(../../main/tests)
FILE(GLOB srcs test-*.cpp)

ADD_EXECUTABLE(test-m main.cpp ${srcs} ../../main/tests/support.hpp)
TARGET_LINK_Y(test-m m)

ENABLE_TESTING()
SOURCE_GROUP(mk FILES test-types.cpp test-adjoint.cpp test-cholesky.cpp
test-diag-symm.cpp test-diag.cpp test-lu.cpp test-svd.cpp test-tao.cpp
test-tao2.cpp test-tao3.cpp test-tridiag.cpp test-cyclic.cpp test-sparse.cpp
)
ADD_TEST(mk:types    test-m types)
ADD_TEST(mk:adjoint  test-m adjoint)
ADD_TEST(mk:cholesky test-m cholesky)
ADD_TEST(mk:diagsymm test-m diag_symm)
ADD_TEST(mk:diag     test-m diag)
ADD_TEST(mk:lu       test-m LU)
ADD_TEST(mk:svd      test-m svd)
ADD_TEST(mk:tridiag  test-m tridiag)
ADD_TEST(mk:cyclic   test-m cyclic)
#ADD_TEST(mk:tao      test-m tao)
#ADD_TEST(mk:tao2     test-m tao2)
#ADD_TEST(mk:tao3     test-m tao3)
ADD_TEST(mk:sparse   test-m sparse)

SOURCE_GROUP(atom FILES test-atom1.cpp test-atom2.cpp test-atom3.cpp test-atom0.cpp test-atom1b.cpp)
ADD_TEST(atom:level0 test-m atom0)
ADD_TEST(atom:level1 test-m atom1)
ADD_TEST(atom:level1b test-m atom1b)
ADD_TEST(atom:level2 test-m atom2)
ADD_TEST(atom:level3 test-m atom3)



SOURCE_GROUP(ode FILES test-michaelis.cpp test-ode.cpp test-rk4.cpp
test-ode-expl.cpp test-ode-impl.cpp)
ADD_TEST(ode:michaelis   test-m michaelis)
ADD_TEST(ode:types       test-m ode)
ADD_TEST(ode:rk4         test-m rk4)
ADD_TEST(ode:expl        test-m ode_expl)
ADD_TEST(ode:impl        test-m ode_impl)

SOURCE_GROUP(opt FILES test-bracket.cpp test-min.cpp)
ADD_TEST(opt:bracket     test-m bracket)
ADD_TEST(opt:minimize    test-m min)

SOURCE_GROUP(fcn FILES test-drvs.cpp test-intg.cpp test-functions.cpp test-zfind.cpp)
ADD_TEST(fcn:drvs    test-m drvs)
ADD_TEST(fcn:intg    test-m intg)
ADD_TEST(fcn:func    test-m functions)
ADD_TEST(fcn:zfind   test-m zfind)

SOURCE_GROUP(sig FILES test-bspl.cpp test-extend.cpp test-linear.cpp test-smooth.cpp test-spline.cpp)
ADD_TEST(sig:linear      test-m linear)
ADD_TEST(sig:spline      test-m spline)
ADD_TEST(sig:extend      test-m extend)
ADD_TEST(sig:smooth      test-m smooth)
ADD_TEST(sig:bsplines    test-m bspl)

SOURCE_GROUP(stat FILES test-stat-average.cpp test-stat-median.cpp test-stat-corr.cpp)
ADD_TEST(stat:average test-m average)
ADD_TEST(stat:median  test-m median)
ADD_TEST(stat:corr    test-m correlation)

SOURCE_GROUP(io FILES test-data-set.cpp)
ADD_TEST(io:ds   test-m data_set ${CMAKE_CURRENT_SOURCE_DIR}/sample.txt)

SOURCE_GROUP(fit FILES test-fit.cpp test-fit-ode.cpp test-fit-poly.cpp test-fit-gauss.cpp)
ADD_TEST(fit       test-m fit)
ADD_TEST(fit2      test-m fit2 ${CMAKE_CURRENT_SOURCE_DIR}/sample.txt)
ADD_TEST(fit:ode   test-m fit_ode)
ADD_TEST(fit:poly  test-m fit_poly)
ADD_TEST(fit:gauss test-m fit_gauss)


SOURCE_GROUP(euclidean FILES test-euclidean.cpp)
ADD_TEST(euclidean test-m euclidean)

SOURCE_GROUP( quark FILES
test-quark1-ld.cpp
test-quark1-set.cpp
test-quark1-add.cpp
test-quark1-dot.cpp
)
ADD_TEST(quark:ld  test-m quark1_ld)
ADD_TEST(quark:set test-m quark1_set)
ADD_TEST(quark:add test-m quark1_add)
ADD_TEST(quark:dot test-m quark1_dot)

ADD_CUSTOM_TARGET(utest-m
COMMAND ${CMAKE_CTEST_COMMAND} --progress -C $<CONFIG>
DEPENDS test-m)
