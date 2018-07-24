//! \file
#ifndef Y_MATH_ROUND_INCLUDED
#define Y_MATH_ROUND_INCLUDED 1

#include "y/math/types.hpp"

namespace upsylon
{
    namespace math
    {
        //! round to a lower integer*exponent10
        template <typename T>
        inline T log_round_floor( T a ) throw()
        {
            assert( a > 0 );
            const T a_log = __floor( __log10(a) );
            const T a_one = __floor( a * __pow( T(10.0),-a_log));
            return  a_one * __pow( T(10.0),a_log);
        }

        //! round to a upper integer*exponent10
        template <typename T>
        inline T log_round_ceil( T a ) throw()
        {
            assert( a > 0 );
            const T a_log = __floor( __log10(a) );
            const T a_one = __ceil( a * __pow( T(10.0),-a_log));
            return  a_one * __pow( T(10.0),a_log);
        }

        //! return every time step to save around each dt_save, updated
        template <typename T>
        inline size_t simulation_save_every_(const T dt, T &dt_save) throw()
        {
            assert(dt>0);
            if(dt_save<=dt)
            {
                dt_save = dt;
            }
            size_t every = max_of<size_t>(__anint(dt_save/dt),1);
            dt_save = every*dt;
            return every;
        }

        //! simplify dt/dt_save and return every time step to save
        template <typename T>
        inline size_t simulation_save_every( T &dt, T &dt_save ) throw()
        {
            assert(dt>0);
            dt = log_round_ceil(dt);
            return simulation_save_every_(dt,dt_save);
        }

        //! number of iterations to run
        template <typename T>
        inline size_t simulation_iter( const T t_run, const T dt, const size_t every ) throw()
        {
            assert(dt>0);
            size_t iter = size_t(__ceil(__fabs(t_run/dt)));
            if(iter<every) { iter = every; }
            while( 0 != (iter%every) ) ++iter;
            return iter;
        }

    }
}

#endif

