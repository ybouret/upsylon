//! \file
#ifndef Y_DYNAMIC_INCLUDED
#define Y_DYNAMIC_INCLUDED 1

#include "y/object.hpp"

namespace upsylon
{
    //! common functions for dynamic containers
    class dynamic
    {
    public:
        //! destructor
        virtual ~dynamic() throw();

        //! currently available objects
        virtual size_t size()     const throw() = 0;

        //! maximum available objects
        virtual size_t capacity() const throw() = 0;

    protected:
        //! constructor
        explicit dynamic() throw();

    private:
        Y_DISABLE_COPY_AND_ASSIGN(dynamic);
    };
}

#endif


