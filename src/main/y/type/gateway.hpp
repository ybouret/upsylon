//! \file

#ifndef Y_TYPE_GATEWAY_INCLUDED
#define Y_TYPE_GATEWAY_INCLUDED 1

#include "y/type/args.hpp"

namespace upsylon
{
    //__________________________________________________________________________
    //
    //
    //! base class to get read only and read/write instances
    //
    //__________________________________________________________________________
    template <typename T>
    class gateway
    {
    public:
        Y_DECL_ARGS(T,type); //!< aliases

        inline virtual     ~gateway()         throw() {}                         //!< cleanup
        inline type       & operator*()       throw() { return (type&)bulk(); }  //!< r/w
        inline const_type & operator*() const throw() { return        bulk(); }  //!< r/o

    protected:
        inline explicit     gateway() throw() {}  //!< setup

    private:
        Y_DISABLE_COPY_AND_ASSIGN(gateway);
        virtual const_type & bulk() const throw() = 0;
    };
}


#endif
