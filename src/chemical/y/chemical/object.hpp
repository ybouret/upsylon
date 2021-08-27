
//! \file

#ifndef Y_CHEMICAL_OBJECT_INCLUDED
#define Y_CHEMICAL_OBJECT_INCLUDED 1

#include "y/chemical/types.hpp"

namespace upsylon
{
    namespace Chemical
    {
        //______________________________________________________________________
        //
        //
        //! base type for dynamic classes
        //
        //______________________________________________________________________
        class Object : public object, public counted
        {
        public:
            virtual ~Object() throw(); //!< cleanup
            
        protected:
            explicit Object() throw(); //!< setup
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(Object);
        };
        
    }
    
}

#endif
