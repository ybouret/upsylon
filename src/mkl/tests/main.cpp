#include "y/utest/driver.hpp"

Y_UTEST_INIT(64)
{
    Y_UTEST(types);
    Y_UTEST(LU);
    Y_UTEST(data_set);
    Y_UTEST(drvs);
    Y_UTEST(zfind);
    Y_UTEST(zsolver);
    Y_UTEST(lfind);
    Y_UTEST(adjoint);
    Y_UTEST(diag_symm);
    Y_UTEST(svd);
    Y_UTEST(cholesky);
    Y_UTEST(diag);
    Y_UTEST(tridiag);
    Y_UTEST(cyclic);
    Y_UTEST(sparse);
    Y_UTEST(gram_schmidt);

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

    Y_UTEST(average);
    Y_UTEST(median);
    Y_UTEST(correlation);


    Y_UTEST(euclidean);

    Y_UTEST(fit);
    Y_UTEST(fit2);
    Y_UTEST(fit_ode);
    Y_UTEST(fit_poly);
    Y_UTEST(fit_gauss);

    Y_UTEST(quark1_ld);
    Y_UTEST(quark1_set);
    Y_UTEST(quark1_add);
    Y_UTEST(quark1_dot);
    Y_UTEST(quark1_neg);
    Y_UTEST(quark2);    
    Y_UTEST(quark3);

    Y_UTEST(k_means);
}
Y_UTEST_EXEC()

