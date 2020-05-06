//! \file

#ifndef Y_SPADE_FIELD3D_INCLUDED
#define Y_SPADE_FIELD3D_INCLUDED 1

#include "y/spade/field2d.hpp"

namespace upsylon {
    
    namespace Spade {
        
        template <typename T>
        class Field3D : public Field<T>, public Layout3D
        {
        public:
            inline virtual ~Field3D() throw() {}
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(Field3D);
        };
        
    }
    
}
#endif

