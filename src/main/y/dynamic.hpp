//! \file
#ifndef Y_DYNAMIC_INCLUDED
#define Y_DYNAMIC_INCLUDED 1

#include "y/object.hpp"

namespace upsylon
{
    //! dynamic class interface
    class dynamic
    {
    public:
        virtual ~dynamic() throw();

        virtual size_t size()     const throw() = 0;
        virtual size_t capacity() const throw() = 0;

    protected:
        explicit dynamic() throw();

    private:
        Y_DISABLE_COPY_AND_ASSIGN(dynamic);
    };
}

#endif


