//! \file
#ifndef Y_MATH_DATA_SET_INCLUDED
#define Y_MATH_DATA_SET_INCLUDED 1

#include "y/associative/map.hpp"

namespace upsylon
{
    namespace math
    {
        template <typename T>
        class data_set
        {
        public:
            inline explicit data_set() {}
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(data_set);
        };
    }
}

#endif

