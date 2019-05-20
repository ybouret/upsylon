#include <R.h>
#include <Rinternals.h>

extern "C" SEXP vecsum(SEXP Rvec)
{
	const double *a = REAL(Rvec);
	const size_t  n = length(Rvec);
	double        sum = 0;
	for(size_t i=0;i<n;++i) sum += a[i];
	Rprintf("sum=%g\n",sum);
	return R_NilValue;
}
