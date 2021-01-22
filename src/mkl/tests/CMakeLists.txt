PROJECT(test-mkl)
INCLUDE_DIRECTORIES(../../main/tests ../../spade)
FILE(GLOB srcs test-*.cpp)

ADD_EXECUTABLE(test-mkl main.cpp ${srcs} ../../main/tests/support.hpp)
TARGET_LINK_Y(test-mkl mkl)

ENABLE_TESTING()
SOURCE_GROUP(mk FILES test-types.cpp test-adjoint.cpp test-cholesky.cpp
test-diag-symm.cpp test-diag.cpp test-lu.cpp test-svd.cpp 
test-tridiag.cpp test-cyclic.cpp test-sparse.cpp
test-gram-schmidt.cpp
test-precond.cpp
test-apk.cpp
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
ADD_TEST(mkl:jacobian  test-mkl jacobian)
ADD_TEST(mkl:precond   test-mkl precond)
ADD_TEST(mkl:apk       test-mkl apk)

SOURCE_GROUP(ode FILES test-michaelis.cpp test-ode.cpp test-rk4.cpp
test-ode-expl.cpp test-ode-impl.cpp)
ADD_TEST(ode:michaelis   test-mkl michaelis)
ADD_TEST(ode:types       test-mkl ode)
ADD_TEST(ode:rk4         test-mkl rk4)
ADD_TEST(ode:expl        test-mkl ode_expl)
ADD_TEST(ode:impl        test-mkl ode_impl)

SOURCE_GROUP(opt FILES test-bracket.cpp test-min.cpp test-opt.cpp)
ADD_TEST(opt:bracket     test-mkl bracket)
ADD_TEST(opt:minimize    test-mkl min)
ADD_TEST(opt:optimize    test-mkl opt)

SOURCE_GROUP(fcn FILES test-drvs.cpp test-intg.cpp test-functions.cpp test-zroot.cpp test-jacobian.cpp test-znewt.cpp test-zircon.cpp test-zeroes.cpp)
ADD_TEST(fcn:drvs     test-mkl drvs)
ADD_TEST(fcn:intg     test-mkl intg)
ADD_TEST(fcn:func     test-mkl functions)
ADD_TEST(fcn:zroot    test-mkl zroot)
ADD_TEST(fcn:jacobian test-mkl jacobian)
ADD_TEST(fcn:znewt    test-mkl znewt)
ADD_TEST(fcn:zircon   test-mkl zircon)
ADD_TEST(fcn:zeroes   test-mkl zeroes)


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
ADD_TEST(io:ds test-mkl data_set ${CMAKE_CURRENT_SOURCE_DIR}/sample.txt)

SOURCE_GROUP(euclidean FILES test-euclidean.cpp)
ADD_TEST(euclidean test-mkl euclidean)

SOURCE_GROUP(geometric FILES test-geometric.cpp)
ADD_TEST(geometric test-mkl geometric)

SOURCE_GROUP(tao FILES
test-tao.cpp
test-simplify.cpp)
ADD_TEST(tao test-mkl tao)
ADD_TEST(simplify test-mkl simplify)

SOURCE_GROUP(fitting FILES
test-fitting-vars.cpp 
test-fitting-lam.cpp
test-fitting-sample.cpp
test-fitting-diff.cpp
test-fitting-circles.cpp
test-fitting-ode.cpp
test-fitting-poly.cpp
test-fitting-gls.cpp
test-fitting-conics.cpp
test-fitting-load.cpp
test-fitting-gaussians.cpp
)
ADD_TEST(fitting:vars    test-mkl fitting_vars)
ADD_TEST(fitting:lam     test-mkl fitting_lam)
ADD_TEST(fitting:sample  test-mkl fitting_sample)
ADD_TEST(fitting:diff    test-mkl fitting_diff)
ADD_TEST(fitting:ode     test-mkl fitting_ode)
ADD_TEST(fitting:poly    test-mkl fitting_poly)
ADD_TEST(fitting:gls     test-mkl fitting_gls)
ADD_TEST(fitting:circles test-mkl fitting_circles)
ADD_TEST(fitting:conics  test-mkl fitting_conics)
ADD_TEST(fitting:load    test-mkl fitting_load ${CMAKE_CURRENT_SOURCE_DIR}/sample.txt)
ADD_TEST(fitting:gaussians test-mkl fitting_gaussians)

ADD_CUSTOM_TARGET(utest-mkl
COMMAND ${CMAKE_CTEST_COMMAND} -C $<CONFIG>
DEPENDS test-mkl)

INSTALL( TARGETS test-mkl RUNTIME DESTINATION var/y )

