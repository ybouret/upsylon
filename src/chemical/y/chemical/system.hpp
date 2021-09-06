//! \file
#ifndef Y_CHEMICAL_SYSTEM_INCLUDED
#define Y_CHEMICAL_SYSTEM_INCLUDED 1

#include "y/chemical/equilibria.hpp"


namespace upsylon
{
    namespace Chemical
    {

        class System : public Object
        {
        public:

            explicit System(const Library &, const Equilibria &, const unsigned);
            virtual ~System() throw();

            const Library    &lib; //!< (frozen) library
            const Equilibria &eqs; //!< (frozen) equilibria
            const size_t      N;   //!< eqs size
            const size_t      M;   //!< lib size
            const Matrix      Nu;  //!< [NxM] topology
            const Matrix      NuT; //!< [MxN] Nu'

        private:
            Y_DISABLE_COPY_AND_ASSIGN(System);

            const Freezable::Latch libLatch;
            const Freezable::Latch eqsLatch;

        };
    }

}


#endif

