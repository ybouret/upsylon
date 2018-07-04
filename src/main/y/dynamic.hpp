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

        inline  size_t available() const throw() { assert(size()<=capacity()); return capacity() - size(); }
        
        //! helper
        inline bool is_filled() const throw()  { return size()>=capacity(); }

        //! helper
        inline bool has_space() const throw()  { return size()<capacity();  }

        //! helper
    protected:
        //! constructor
        explicit dynamic() throw();

    private:
        Y_DISABLE_COPY_AND_ASSIGN(dynamic);
    };
}

#endif


