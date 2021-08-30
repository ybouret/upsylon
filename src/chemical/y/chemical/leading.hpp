
//! \file

#ifndef Y_CHEMICAL_LEADING_INCLUDED
#define Y_CHEMICAL_LEADING_INCLUDED 1

#include "y/chemical/equilibrium.hpp"


namespace upsylon
{
    namespace Chemical
    {
        //______________________________________________________________________
        //
        //
        //! leading information
        //
        //______________________________________________________________________
        class Leading : public Object
        {
        public:
            typedef arc_ptr<Leading>               Pointer;  //!< alias
            typedef vector<const Actor,Allocator>  Limiting; //!< array alias

            explicit Leading(const Equilibrium &);
            virtual ~Leading() throw();

            const Limiting reac;
            const Limiting prod;
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(Leading);
        };

    }

}

#endif

