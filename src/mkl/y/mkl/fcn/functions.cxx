#include "y/mkl/fcn/functions.hpp"
#include "y/mkl/ztype.hpp"
#include "y/type/utils.hpp"
#include "y/exceptions.hpp"

#include "y/mkl/solve/zroot.hpp"

#include <cerrno>

namespace upsylon {
	
	namespace mkl {

        //typedef zroot<real_t>::bisection solver;
        typedef zroot<real_t>::ridder solver;

		real_t qerfc( real_t x) throw()
        {
			const real_t z  = REAL(fabs)(x);
			const real_t t  = REAL(1.0)/(REAL(1.0)+REAL(0.5)*z);
			const real_t ans= t*REAL(exp)(-z*z-REAL(1.26551223)+t*(REAL(1.00002368)+t*(REAL(0.37409196)+t*(REAL(0.09678418)+
                                                                                                     t*(-REAL(0.18628806)+t*(REAL(0.27886807)+t*(-REAL(1.13520398)+t*(REAL(1.48851587)+
                                                                                                                                                                      t*(-REAL(0.82215223)+t*REAL(0.17087277))))))))));
			return x >= REAL(0.0) ? ans : REAL(2.0)-ans;
		}
		
		real_t qerf( real_t x) throw()
        {
			return REAL(1.0)-qerfc( x );
		}

        namespace
        {

            struct zqerf_t
            {
                real_t p;
                inline real_t operator()(const real_t x) const throw()
                {
                    return qerf(x)-p;
                }
            };

        }


        real_t iqerf( real_t p )
        {
            assert(p> -1);
            assert(p < 1);
            triplet<real_t> x     = { -1, 0, 1 };
            triplet<real_t> f     = { 0,  0, 0 };
            const zqerf_t   zqerf = { p };
            while( (f.a=zqerf(x.a)) >= 0)
            {
                x.a += x.a;
            }
            while( (f.c=zqerf(x.c)) <= 0)
            {
                x.c+=x.c;
            }
            solver solve;
            return solve(zqerf,x,f);
        }

        namespace
        {

            struct zqerfc_t
            {
                real_t p;
                inline real_t operator()(const real_t x) const throw()
                {
                    return qerfc(x)-p;
                }
            };

        }

        real_t iqerfc( real_t p )
        {
            assert(p>0);
            assert(p<2);
            triplet<real_t> x      = { -1, 0, 1 };
            triplet<real_t> f      = { 0,  0, 0 };
            const zqerfc_t  zqerfc = { p };
            while( (f.a=zqerfc(x.a)) <= 0)
            {
                x.a += x.a;
            }
            while( (f.c=zqerfc(x.c)) >= 0)
            {
                x.c+=x.c;
            }
            solver solve;
            return solve(zqerfc,x,f);
        }




		real_t gamma_log( real_t xx ) throw() {
			static const double cof[6]=
			{
				+76.18009172947146,
				-86.50532032941677,
				+24.01409824083091,
				-01.231739572450155,
				-00.1208650973866179e-2,
				-00.5395239384953e-5
			};
			
			double x    = xx;
			double y    = xx;
			double tmp  = x+5.5;
			double ser  = 1.000000000190015;
			tmp -= (x+0.5)*log(tmp);
			ser += cof[0]/++y;
			ser += cof[1]/++y;
			ser += cof[2]/++y;
			ser += cof[3]/++y;
			ser += cof[4]/++y;
			ser += cof[5]/++y;
            return static_cast<real_t>( log(2.5066282746310005*ser/x)-tmp);
		}
		
		
		
        
		real_t gamma_i( real_t a, real_t x )
		{
			static const real_t EPS = numeric<real_t>::epsilon;
			//std::cerr << "gamma_i(" << a << "," << x << ")" << std::endl;
			
			if (x < REAL(0.0) || a <= REAL(0.0) )
				throw libc::exception(EDOM,"invalid (%g,%g) for gamma_i", a, x);
			
			real_t num = 1;
			real_t den = a;
			real_t ser = a;
			
			const real_t fac =  std::pow(x,a)*std::exp(-x);
			//std::cerr << "fac=" << fac << std::endl;
			//exit(1);
			real_t sum = num/den;
			real_t rho = sum;
			
			while( rho >= EPS ) {
				sum += ( rho = ( ( num *= x) / ( den *= ++ser ) ) );
			}
			
			real_t old_res = fac * sum;
			for(;;) {
				sum += ( ( num *= x) / ( den *= ++ser ) ) ;
				const real_t new_res = fac * sum;
				//std::cerr << "new_res=" << new_res << ", old_res=" << old_res << std::endl;
				if( (new_res - old_res) <= EPS * new_res )
					return new_res;
				old_res = new_res;
			}
		}
		
		real_t gamma_p( real_t a, real_t x ) {
			return clamp<real_t>(0,gamma_i(a,x) * REAL(exp)( -gamma_log(a) ),1);
		}
		
		
		real_t gamma_q( real_t a, real_t x ) {
			return clamp<real_t>(0,1-gamma_p(a,x),1);
		}
		
		static
		real_t betacf( const real_t a, const real_t b, real_t x )
		{
			static const real_t FPMIN = REAL(pow)( real_t(10.0), REAL(ceil)( REAL(log10)( real_t(REAL_MIN) ) ) );
			static const real_t EPS   = REAL_EPSILON;
			static const size_t MAXIT = 16384;
			
			//std::cout << "FPMIN=" << FPMIN << std::endl;
			//std::cout << "EPS  =" << EPS   << std::endl;
			
			const real_t qab = a+b;
			const real_t qap = a+REAL(1.0);
			const real_t qam = a-REAL(1.0);
			real_t c = REAL(1.0);
			real_t d = REAL(1.0) - qab *x/qap;
			if( REAL(fabs)(d) < FPMIN) d = FPMIN;
			d = REAL(1.0) / d;
			real_t h = d;
			for(size_t m=1;;)
			{
				const size_t m2 = m << 1;
				real_t aa = m * (b-m) * x / ((qam+m2)*(a+m2));
				d =  REAL(1.0)+aa*d;
				c =  REAL(1.0)+aa/c;
				if( REAL(fabs)(d) < FPMIN ) d = FPMIN;
				if( REAL(fabs)(c) < FPMIN ) c = FPMIN;
				d = REAL(1.0)/d;
				
				h *= (d*c);
				aa = -(a+m)*(qab+m)*x/((a+m2)*(qap+m2));
				
				d =  REAL(1.0)+aa*d;
				c =  REAL(1.0)+aa/c;
				if( REAL(fabs)(d) < FPMIN ) d = FPMIN;
				if( REAL(fabs)(c) < FPMIN ) c = FPMIN;
				d = REAL(1.0)/d;
				const real_t del = d*c;
				h *= del;
				if( REAL(fabs)(del-REAL(1.0)) <= EPS )
					break;
				if( ++m > MAXIT )
					throw libc::exception( EDOM, "beta_i(%g,%g,%g) #%u doesn't converge",a,b,x,unsigned(MAXIT));
			}
			
			return h;
		}
		
		real_t beta_i( real_t a, real_t b, real_t x )
		{
			if( a <= 0)
				throw libc::exception( EDOM, "[beta_i]: a=%g", a );
			
			if( b <= 0)
				throw libc::exception( EDOM, "[beta_i]: b=%g", b );
			
			if( x < 0 || x > 1 )
				throw libc::exception( EDOM, "[beta_i]: x=%g", x );
			
			if( x <= 0 )
				return 0;
			else
				if( x >= 1 )
					return 1;
				else
				{
					const real_t bt = REAL(exp)( gamma_log(a+b) - gamma_log(a) - gamma_log(b) + a * REAL(log)(x) + b * REAL(log)( real_t(REAL(1.0)-x) ) );
					if( x < ( a+ REAL(1.0))/(a+b+REAL(2.0)) )
						return bt * betacf(a,b,x)/a;
					else
						return REAL(1.0) - bt * betacf(b,a,REAL(1.0)-x)/b;
				}
			
		}

	} //math
	
} //yocto
