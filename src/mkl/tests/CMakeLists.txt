PROJECT(test-mkl)
INCLUDE_DIRECTORIES(../../main/tests)
FILE(GLOB srcs test-*.cpp)

ADD_EXECUTABLE(test-mkl main.cpp ${srcs} ../../main/tests/support.hpp)
TARGET_LINK_Y(test-mkl mkl)

ENABLE_TESTING()
SOURCE_GROUP(mk FILES test-types.cpp test-adjoint.cpp test-cholesky.cpp
test-diag-symm.cpp test-diag.cpp test-lu.cpp test-svd.cpp 
test-tridiag.cpp test-cyclic.cpp test-sparse.cpp
test-gram-schmidt.cpp
)
ADD_TEST(mkl:types    test-mkl types)
ADD_TEST(mkl:adjoint  test-mkl adjoint)
ADD_TEST(mkl:cholesky test-mkl cholesky)
ADD_TEST(mkl:diagsymm test-mkl diag_symm)
ADD_TEST(mkl:diag     test-mkl diag)
ADD_TEST(mkl:lu       test-mkl LU)
ADD_TEST(mkl:svd      test-mkl svd)
ADD_TEST(mkl:tridiag  test-mkl tridiag)
ADD_TEST(mkl:cyclic   test-mkl cyclic)
ADD_TEST(mkl:sparse   test-mkl sparse)
ADD_TEST(mkl:gram-schmidt test-mkl gram_schmidt)



SOURCE_GROUP(ode FILES test-michaelis.cpp test-ode.cpp test-rk4.cpp
test-ode-expl.cpp test-ode-impl.cpp)
ADD_TEST(ode:michaelis   test-mkl michaelis)
ADD_TEST(ode:types       test-mkl ode)
ADD_TEST(ode:rk4         test-mkl rk4)
ADD_TEST(ode:expl        test-mkl ode_expl)
ADD_TEST(ode:impl        test-mkl ode_impl)

SOURCE_GROUP(opt FILES test-bracket.cpp test-min.cpp)
ADD_TEST(opt:bracket     test-mkl bracket)
ADD_TEST(opt:minimize    test-mkl min)

SOURCE_GROUP(fcn FILES test-drvs.cpp test-intg.cpp test-functions.cpp test-zfind.cpp test-zroot.cpp)
ADD_TEST(fcn:drvs    test-mkl drvs)
ADD_TEST(fcn:intg    test-mkl intg)
ADD_TEST(fcn:func    test-mkl functions)
ADD_TEST(fcn:zfind   test-mkl zfind)
ADD_TEST(fcn:zroot   test-mkl zroot)

SOURCE_GROUP(sig FILES test-bspl.cpp test-extend.cpp test-linear.cpp test-smooth.cpp test-spline.cpp)
ADD_TEST(sig:linear      test-mkl linear)
ADD_TEST(sig:spline      test-mkl spline)
ADD_TEST(sig:extend      test-mkl extend)
ADD_TEST(sig:smooth      test-mkl smooth)
ADD_TEST(sig:bsplines    test-mkl bspl)

SOURCE_GROUP(stat FILES test-stat-average.cpp test-stat-median.cpp test-stat-corr.cpp test-k-means.cpp)
ADD_TEST(stat:average test-mkl average)
ADD_TEST(stat:median  test-mkl median)
ADD_TEST(stat:corr    test-mkl correlation)
ADD_TEST(stat:k-means test-mkl k_means)

SOURCE_GROUP(io FILES test-data-set.cpp)
ADD_TEST(io:ds   test-mkl data_set ${CMAKE_CURRENT_SOURCE_DIR}/sample.txt)

SOURCE_GROUP(fit FILES test-fit.cpp test-fit-ode.cpp test-fit-poly.cpp test-fit-gauss.cpp)
ADD_TEST(fit       test-mkl fit)
ADD_TEST(fit2      test-mkl fit2 ${CMAKE_CURRENT_SOURCE_DIR}/sample.txt)
ADD_TEST(fit:ode   test-mkl fit_ode)
ADD_TEST(fit:poly  test-mkl fit_poly)
ADD_TEST(fit:gauss test-mkl fit_gauss)


SOURCE_GROUP(euclidean FILES test-euclidean.cpp)
ADD_TEST(euclidean test-mkl euclidean)

SOURCE_GROUP( quark FILES
test-quark1-ld.cpp
test-quark1-set.cpp
test-quark1-add.cpp
test-quark1-dot.cpp
test-quark1-neg.cpp
test-quark2.cpp
test-quark3.cpp
)
ADD_TEST(quark1:ld  test-mkl quark1_ld)
ADD_TEST(quark1:set test-mkl quark1_set)
ADD_TEST(quark1:add test-mkl quark1_add)
ADD_TEST(quark1:dot test-mkl quark1_dot)
ADD_TEST(quark1:neg test-mkl quark1_neg)
ADD_TEST(quark2    test-mkl quark2)
ADD_TEST(quark3    test-mkl quark3)

ADD_CUSTOM_TARGET(utest-mkl
COMMAND ${CMAKE_CTEST_COMMAND} -C $<CONFIG>
DEPENDS test-mkl)

INSTALL( TARGETS test-mkl RUNTIME DESTINATION var/y )

