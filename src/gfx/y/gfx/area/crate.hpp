//! \file

//! \file

#ifndef Y_GFX_CRATE_INCLUDED
#define Y_GFX_CRATE_INCLUDED 1

#include "y/gfx/area/sub.hpp"


namespace upsylon
{
    namespace GFX
    {
        //______________________________________________________________________
        //
        //
        //! a crate for sub-areas
        //
        //______________________________________________________________________

        class Crate
        {
        public:
            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            virtual unit_t items() const throw() = 0;              //!< get hold items
            static  unit_t Items(const SubArea::Handle &) throw(); //!< 0 or n

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            virtual ~Crate() throw();
        protected:
            explicit Crate() throw();

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Crate);
        };

    }
}

#endif
