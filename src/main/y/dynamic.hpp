//! \file
#ifndef Y_DYNAMIC_INCLUDED
#define Y_DYNAMIC_INCLUDED 1

#include "y/collection.hpp"

namespace upsylon
{
    //__________________________________________________________________________
    //
    //
    //! common functions for dynamic collection and capacity()
    //
    //__________________________________________________________________________
    class dynamic : public virtual collection
    {
    public:
        //______________________________________________________________________
        //
        // virtual interface
        //______________________________________________________________________
        virtual       ~dynamic()        throw();     //!< cleanup
        virtual size_t capacity() const throw() = 0; //!< maximum available objects

        //______________________________________________________________________
        //
        // non-virtual interface
        //______________________________________________________________________

        //! available objects
        inline  size_t available() const throw() { assert(size()<=capacity()); return capacity() - size(); }
        
        //! helper
        inline bool is_filled() const throw()  { return size()>=capacity(); }

        //! helper
        inline bool has_space() const throw()  { return size()<capacity();  }

    protected:
        explicit dynamic() throw();//!< constructor

    private:
        Y_DISABLE_COPY_AND_ASSIGN(dynamic);
    };


}


#endif


