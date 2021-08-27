//! \file

#ifndef Y_CHEMICAL_INDEXED_INCLUDED
#define Y_CHEMICAL_INDEXED_INCLUDED 1

#include "y/chemical/object.hpp"

namespace upsylon
{
    namespace Chemical
    {
        //______________________________________________________________________
        //
        //
        //! base type for indexed class
        //
        //______________________________________________________________________
        class Indexed : public Object
        {
        public:
            virtual ~Indexed() throw();
            
            const size_t indx; //!< the [1...] index
            
        protected:
            explicit Indexed() throw();
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(Indexed);
        };
        
        
    }
    
}

#endif
