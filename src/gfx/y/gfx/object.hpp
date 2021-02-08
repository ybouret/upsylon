
//! \file

#ifndef Y_GFX_OBJECT_INCLUDED
#define Y_GFX_OBJECT_INCLUDED 1

#include "y/ptr/counted.hpp"


namespace upsylon
{
    namespace GFX
    {

        //! base class for dynamic objects
        class Object : public object, public counted
        {
        public:
            virtual ~Object() throw();
            Object()          throw();
        private:
            Y_DISABLE_COPY_AND_ASSIGN(Object);
        };

    }

}

#endif
