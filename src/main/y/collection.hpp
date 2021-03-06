//! \file

#ifndef Y_COLLECTION_INCLUDED
#define Y_COLLECTION_INCLUDED 1

#include "y/os/platform.hpp"

namespace upsylon {

    //__________________________________________________________________________
    //
    //
    //! interface for collection: got a size!
    //
    //__________________________________________________________________________
    class collection
    {
    public:
        virtual       ~collection() throw();     //!< cleanup
        virtual size_t size() const throw() = 0; //!< number of alive objects
        unit_t         ssize() const;            //!< signed size, with overflow control
        
    protected:                                   
        explicit collection() throw();           //!< setup
        
    private:
        Y_DISABLE_COPY_AND_ASSIGN(collection);
    };
    
}

#endif
