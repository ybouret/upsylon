//! \file

#ifndef Y_COLLECTION_INCLUDED
#define Y_COLLECTION_INCLUDED 1

#include "y/os/platform.hpp"

namespace upsylon {

    class collection
    {
    public:
        virtual ~collection() throw();

        virtual size_t size() const throw() = 0;

    protected:
        explicit collection() throw();

    private:
        Y_DISABLE_COPY_AND_ASSIGN(collection);
    };

}

#endif
