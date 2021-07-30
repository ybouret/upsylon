
//! \file

#ifndef Y_ALCHEMY_EXTENT_INCLUDED
#define Y_ALCHEMY_EXTENT_INCLUDED 1

#include "y/os/platform.hpp"
#include <iostream>

namespace upsylon
{
    namespace alchemy
    {
        //______________________________________________________________________
        //
        //
        //! extent of a side of a reaction
        //
        //______________________________________________________________________
        class extent
        {
        public:
            const size_t index; //!< of the species, 0 if no limit
            const double value; //!< the value
            
            extent(const size_t i, const double v) throw(); //!< init
            extent(const extent &) throw();                 //!< copy
            ~extent() throw();                              //!< quit
            
            //! display
            friend std::ostream & operator<<(std::ostream &, const extent &);
            
        private:
            Y_DISABLE_ASSIGN(extent);
        };
        
        //______________________________________________________________________
        //
        //
        //! couple of extents
        //
        //______________________________________________________________________
        class extents
        {
        public:
            const extent forward; //!< limit by reactants
            const extent reverse; //!< limit by products
            
            extents(const extent fwd, const extent rev) throw(); //!< init
            extents(const extents &) throw();                    //!< copy
            ~extents() throw();                                  //!< quit
            
            //! display
            friend std::ostream & operator<<(std::ostream &, const extents &);

            
        private:
            Y_DISABLE_ASSIGN(extents);
        };
        
    }
    
}


#endif
