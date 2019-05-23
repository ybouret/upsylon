#include "y/r++/r.hpp"
#include "y/sort/sorted-sum.hpp"

using namespace upsylon;

extern "C" SEXP vecsum(SEXP Rvec)
{
    R::Vector<double> a(Rvec);
    const double sum = sorted_sum(a);
	Rprintf("sum=%g\n",sum);
	return R_NilValue;
}
