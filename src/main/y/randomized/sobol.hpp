// \file
#ifndef Y_RANDOMIZED_SOBOL_INCLUDED
#define Y_RANDOMIZED_SOBOL_INCLUDED

#include "y/type/utils.hpp"
#include "y/sequence/array.hpp"

namespace upsylon
{
    namespace randomized
    {

        //! a Sobol's sequence
        class sobol
        {
        public:
            static const size_t    MAXBIT = 30; //!< max bits
            static const size_t    MAXDIM = 6;  //!< max dims


            //! setup
            explicit sobol() throw();

            //! initialize
            void reset() throw();

            //! compute Sobol vector x[0..n-1]
            void compute( const size_t n, double *x );
            

            //! compute Sobol vector x[0..n-1]
            void compute( array<double> &x );

            //! skip1 for a dimension of n
            void skip1(const size_t n);

        private:
            unsigned long  in;
            const double   fac;
            unsigned long  ix[MAXDIM+1];
            unsigned long *iu[MAXBIT+1];
            unsigned long  iv[MAXDIM*MAXBIT+1];

            unsigned long cycle();

            static const unsigned long mdeg[MAXDIM+1];
            static const unsigned long ip[MAXDIM+1];
            static const unsigned long iv0[MAXDIM*MAXBIT+1];

        };

    }
}


#endif

