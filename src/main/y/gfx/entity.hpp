//! \file

#ifndef Y_GFX_ENTITY_INCLUDED
#define Y_GFX_ENTITY_INCLUDED 1

#include "y/ptr/counted.hpp"

namespace upsylon
{

    namespace graphic
    {

        //______________________________________________________________________
        //
        //! base class for dynamic objects of graphic
        //______________________________________________________________________
        class entity : public object, public counted
        {
        public:
            explicit entity() throw(); //!< setup
            virtual ~entity() throw(); //!< cleanup
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(entity);
        };
    }

}

#endif

