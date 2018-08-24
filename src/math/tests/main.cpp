#include "y/utest/driver.hpp"

Y_UTEST_INIT(64)
{
    Y_UTEST(types);
    Y_UTEST(tao);
    Y_UTEST(tao2);
    Y_UTEST(tao3);
    Y_UTEST(LU);
    Y_UTEST(data_set);
    Y_UTEST(drvs);
    Y_UTEST(zfind);
    Y_UTEST(adjoint);
    Y_UTEST(diag_symm);
    Y_UTEST(svd);
    Y_UTEST(cholesky);
    Y_UTEST(diag);
    Y_UTEST(ode);
    Y_UTEST(rk4);
    Y_UTEST(functions);
    Y_UTEST(intg);
    Y_UTEST(michaelis);
    Y_UTEST(ode_expl);
    Y_UTEST(ode_impl);
    Y_UTEST(shapes2d);

}
Y_UTEST_EXEC()

