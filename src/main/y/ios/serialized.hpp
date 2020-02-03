//! \file
#ifndef Y_IOS_SERIALIZED_INCLUDED
#define Y_IOS_SERIALIZED_INCLUDED 1

#include "y/ios/serializable.hpp"
#include "y/hashing/function.hpp"

namespace upsylon    {

    namespace ios {

        struct serialized
        {
            template <typename T> static inline
            T  key( const serializable &input, hashing::function &H )
            {
                H.set();
                input.hash_with(H);
                return H.key<T>();
            }

            static   digest  md(const serializable &input, hashing::function &H );
            
        };

    }

}

#endif

