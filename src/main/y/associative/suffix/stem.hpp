//! \file
#ifndef Y_FILE_ASSOCIATIVE_SUFFIX_STEM_INCLUDED
#define Y_FILE_ASSOCIATIVE_SUFFIX_STEM_INCLUDED 1

#include "y/os/platform.hpp"

namespace upsylon {

    class suffix_stem
    {
    public:
        virtual ~suffix_stem() throw();

    protected:
        explicit suffix_stem() throw();
        
    private:
        Y_DISABLE_COPY_AND_ASSIGN(suffix_stem);
    };

}

#endif
