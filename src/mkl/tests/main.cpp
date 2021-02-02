#include "y/utest/driver.hpp"

Y_UTEST_INIT(64)
{
    Y_UTEST(types);
    Y_UTEST(LU);
    Y_UTEST(data_set);
    Y_UTEST(drvs);

    Y_UTEST(zeroes);
    Y_UTEST(zircon);

    Y_UTEST(adjoint);
    Y_UTEST(diag_symm);
    Y_UTEST(svd);
    Y_UTEST(cholesky);
    Y_UTEST(diag);
    Y_UTEST(tridiag);
    Y_UTEST(cyclic);
    Y_UTEST(sparse);
    Y_UTEST(gram_schmidt);
    Y_UTEST(apk);

    Y_UTEST(ode);
    Y_UTEST(rk4);
    Y_UTEST(michaelis);
    Y_UTEST(ode_expl);
    Y_UTEST(ode_impl);

    Y_UTEST(functions);
    Y_UTEST(intg);

    Y_UTEST(bracket);
    Y_UTEST(min);
    Y_UTEST(opt);

    Y_UTEST(linear);
    Y_UTEST(spline);
    Y_UTEST(extend);
    Y_UTEST(smooth);
    Y_UTEST(bspl);

    Y_UTEST(average);
    Y_UTEST(median);
    Y_UTEST(correlation);


    Y_UTEST(euclidean);
    Y_UTEST(geometric);
    
    Y_UTEST(k_means);
    Y_UTEST(jacobian);
    Y_UTEST(znewt);

    Y_UTEST(tao);
    Y_UTEST(simplify);

    Y_UTEST(fitting_vars);
    Y_UTEST(fitting_lam);
    Y_UTEST(fitting_sample);
    Y_UTEST(fitting_diff);
    Y_UTEST(fitting_ode);
    Y_UTEST(fitting_poly);
    Y_UTEST(fitting_gls);
    Y_UTEST(fitting_circles);
    Y_UTEST(fitting_conics);
    Y_UTEST(fitting_gaussians);
    Y_UTEST(fitting_load);
}
Y_UTEST_EXEC()

