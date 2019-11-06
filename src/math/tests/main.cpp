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
    Y_UTEST(tridiag);
    Y_UTEST(cyclic);
    Y_UTEST(sparse);
    Y_UTEST(atom1);
    Y_UTEST(atom2);
    Y_UTEST(atom3);

    Y_UTEST(ode);
    Y_UTEST(rk4);
    Y_UTEST(michaelis);
    Y_UTEST(ode_expl);
    Y_UTEST(ode_impl);

    Y_UTEST(functions);
    Y_UTEST(intg);

    Y_UTEST(bracket);
    Y_UTEST(min);
    

    Y_UTEST(linear);
    Y_UTEST(spline);
    Y_UTEST(extend);
    Y_UTEST(smooth);
    Y_UTEST(bspl);
    Y_UTEST(stat);


    Y_UTEST(euclidean);

    Y_UTEST(adjust);
    Y_UTEST(adjust2);
    Y_UTEST(adjust_ode);
    Y_UTEST(adjust_poly);
    Y_UTEST(adjust_gauss);

}
Y_UTEST_EXEC()

