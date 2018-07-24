//! \file
#ifndef Y_MATH_DATA_SET_INCLUDED
#define Y_MATH_DATA_SET_INCLUDED 1

#include "y/associative/map.hpp"
#include "y/ios/istream.hpp"
#include "y/container/sequence.hpp"

namespace upsylon
{
    namespace math
    {
        //! temporaty object to load data files
        template <typename T>
        class data_set
        {
        public:
            Y_DECL_ARGS(T,type);
            typedef sequence<type> sequence_type;

            inline explicit data_set() {}
            inline virtual ~data_set() throw() {}
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(data_set);
        };
    }
}

#endif

