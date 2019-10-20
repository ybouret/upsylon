//! \file

#ifndef Y_SEQUENCE_ADDRESSABLE_INCLUDED
#define Y_SEQUENCE_ADDRESSABLE_INCLUDED 1

#include "y/sequence/accessible.hpp"

namespace upsylon {

    
    //! collection of addressable items
    template <typename T>
    class addressable : public accessible<T>
    {
    public:
        Y_DECL_ARGS(T,type);                                     //!< aliases
        inline virtual ~addressable() throw() {}                 //!< cleanup
        virtual type &       operator[](const size_t)       = 0; //!< access

    protected:                                                    
        inline explicit addressable() throw() {}                 //!< setup
    private:
        Y_DISABLE_COPY_AND_ASSIGN(addressable);
    };

   

}

#endif
