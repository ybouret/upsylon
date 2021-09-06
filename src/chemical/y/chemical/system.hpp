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

        private:
            Y_DISABLE_COPY_AND_ASSIGN(System);
        };
    }

}


#endif

