//! \file

#ifndef Y_GFX_ENTITY_INCLUDED
#define Y_GFX_ENTITY_INCLUDED 1

#include "y/ptr/counted.hpp"

namespace upsylon
{

    namespace graphic
    {
        class entity : public object, public counted
        {
        public:
            explicit entity() throw();
            virtual ~entity() throw();
        private:
            Y_DISABLE_COPY_AND_ASSIGN(entity);
        };
    }

}

#endif

