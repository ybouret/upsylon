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
        //______________________________________________________________________
        //
        // interface
        //______________________________________________________________________
        virtual     ~suffix_stem() throw();     //!< cleanup
        virtual void free()        throw() = 0; //!< reset, keep memory
        virtual void trim()        throw() = 0; //!< trim excess node
        virtual void reserve(size_t)       = 0; //!< pre-allocate node

        //______________________________________________________________________
        //
        // common data
        //______________________________________________________________________
        const size_t nodes; //!< number of nodes into tree, >=1


    protected:
        explicit suffix_stem() throw(); //!< setup
        
    private:
        Y_DISABLE_COPY_AND_ASSIGN(suffix_stem);
    };

}

#endif
