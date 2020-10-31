
//! file
#ifndef Y_PREFIX_STRINGS_INCLUDED
#define Y_PREFIX_STRINGS_INCLUDED 1

#include "y/associative/prefix/depot.hpp"

namespace upsylon {
    
    //__________________________________________________________________________
    //
    //
    //! depot to store keys, without any data
    //
    //__________________________________________________________________________
    
    class prefix_strings : public prefix_depot<char>
    {
    public:
        explicit prefix_strings();
        virtual ~prefix_strings() throw();
        prefix_strings(const prefix_strings &);
        prefix_strings & operator=(const prefix_strongs &);
    };
    
}

#endif
