//! \file
#ifndef Y_MATH_TIMINGS_INCLUDED
#define Y_MATH_TIMINGS_INCLUDED 1

#include "y/math/types.hpp"

namespace upsylon
{
    namespace math
    {

        //! computing different timings value for simulations
        struct timings
        {
            //! integer*10^exponent <= a
            template <typename T> static inline
            T round_floor( const T a )
            {
                assert(a>0);
                const T a_log = floor_of( log10_of(a) );
                const T a_one = floor_of( a * pow_of( T(10.0),-a_log));
                return  a_one * pow_of( T(10.0),a_log);
            }

            //! integer*exponent10 >= a
            template <typename T> static inline
            T round_ceil( const T a ) throw()
            {
                assert( a > 0 );
                const T a_log = floor_of( log10_of(a) );
                const T a_one = ceil_of( a * pow_of( T(10.0),-a_log));
                return  a_one * pow_of( T(10.0),a_log);
            }

            //! adjust dt, dt_save and return every time step to save
            template <typename T> static inline
            size_t save_every( T &dt, T &dt_save ) throw()
            {
                assert(dt>0);
                dt = round_floor(dt);
                if(dt_save<=dt) { dt_save=dt; }
                const size_t count = max_of<size_t>( size_t( anint_of(dt_save/dt)), 1 );
                dt_save = count*dt;
                return count;
            }

            //! number of iterations, multiple of save every
            template <typename T> static inline
            size_t iterations( T &t_run, const T dt, const size_t every)
            {
                assert(dt>0);
                assert(every>0);
                size_t iter = size_t( ceil_of(fabs_of(t_run/dt) ) );
                if(iter<every) { iter=every; }
                while( 0 != (iter%every) ) ++iter;
                t_run = iter*dt;
                return iter;
            }

            //! one call setup
            template <typename T> static inline
            size_t setup(T      &t_run,
                         T      &dt,
                         T      &dt_save,
                         size_t &every)
            {
                every = save_every(dt,dt_save);
                return iterations(t_run,dt,every);
            }

        };

    };
}

#endif

