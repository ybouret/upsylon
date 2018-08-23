//! \file
#ifndef Y_FCN_FUNCTIONS_INCLUDED
#define Y_FCN_FUNCTIONS_INCLUDED 1

#include "y/math/types.hpp"

namespace upsylon {

	namespace math {

       
		float  qerfc( float  X ) throw(); //!< erfc
		double qerfc( double X ) throw(); //!< erfc

		float  qerf( float  X ) throw(); //!< erf
		double qerf( double X ) throw(); //!< erf

		float  iqerf( float  p ); //!< X|qerf(X)=p, -1<p<1
		double iqerf( double p ); //!< X|qerf(X)=p, -1<p<1
       
        float  iqerfc( float  p ); //!< X|qerfc(X)=p, 0<p<2
        double iqerfc( double p ); //!< X|qerfc(X)=p, 0<p<2
        
		float  gamma_log( float  X ) throw(); //!< ln(Gamma(x))
		double gamma_log( double X ) throw(); //!< ln(Gamma(x))

		float  gamma_i( float  a, float  x ); //!< incomplete gamma
		double gamma_i( double a, double x ); //!< incomplete gamma

		float  gamma_p( float  a, float  x ); //!< P(a,x)
		double gamma_p( double a, double x ); //!< P(a,x)

		float  gamma_q( float  a, float  x ); //!< Q(a,x)
		double gamma_q( double a, double x ); //!< Q(a,x)

		float  beta_i( float  a, float  b, float  x );//!< incomlete beta function
		double beta_i( double a, double b, double x );//!< incomlete beta function
        
        
	}

}


#endif
