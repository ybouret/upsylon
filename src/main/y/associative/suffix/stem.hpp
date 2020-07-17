//! \file
#ifndef Y_FILE_ASSOCIATIVE_SUFFIX_STEM_INCLUDED
#define Y_FILE_ASSOCIATIVE_SUFFIX_STEM_INCLUDED 1

#include "y/os/platform.hpp"

namespace upsylon {

    //__________________________________________________________________________
    //
    //
    //! base class for suffix-(x)store
    //
    //__________________________________________________________________________
    class suffix_stem
    {
    public:
        virtual     ~suffix_stem() throw();     //!< cleanup
        virtual void free()        throw() = 0; //!< reset, keep memory

    protected:
        explicit suffix_stem() throw(); //!< setup
        
    private:
        Y_DISABLE_COPY_AND_ASSIGN(suffix_stem);
    };

}

#endif
