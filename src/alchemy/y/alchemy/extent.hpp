
//! \file

#ifndef Y_ALCHEMY_EXTENT_INCLUDED
#define Y_ALCHEMY_EXTENT_INCLUDED 1

#include "y/os/platform.hpp"
#include <iostream>

namespace upsylon
{
    namespace Alchemy
    {
        //______________________________________________________________________
        //
        //
        //! extent of a side of a reaction
        //
        //______________________________________________________________________
        class Extent
        {
        public:
            const size_t index; //!< of the species, 0 if no limit
            const double value; //!< the value
            
            Extent(const size_t i, const double v) throw(); //!< init
            Extent(const Extent &) throw();                 //!< copy
            ~Extent() throw();                              //!< quit
            
            //! display
            friend std::ostream & operator<<(std::ostream &, const Extent &);
            
        private:
            Y_DISABLE_ASSIGN(Extent);
        };
        
        //______________________________________________________________________
        //
        //
        //! couple of extents
        //
        //______________________________________________________________________
        class Extents
        {
        public:
            const Extent forward; //!< limit by reactants
            const Extent reverse; //!< limit by products
            
            Extents(const Extent fwd, const Extent rev) throw(); //!< init
            Extents(const Extents &) throw();                    //!< copy
            ~Extents() throw();                                  //!< quit
            
            //! display
            friend std::ostream & operator<<(std::ostream &, const Extents &);

            
        private:
            Y_DISABLE_ASSIGN(Extents);
        };
        
    }
    
}


#endif
