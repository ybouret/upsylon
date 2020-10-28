//! \file

#ifndef Y_TYPE_GATEWAY_INCLUDED
#define Y_TYPE_GATEWAY_INCLUDED 1

#include "y/type/args.hpp"

namespace upsylon
{
    template <typename T>
    class gateway
    {
    public:
        Y_DECL_ARGS(T,type);
        inline virtual ~gateway() throw() {}

        inline type       & operator*()       throw() { return (type&)bulk(); }
        inline const_type & operator*() const throw() { return        bulk(); }

    protected:
        inline explicit gateway() throw() {}

    private:
        Y_DISABLE_COPY_AND_ASSIGN(gateway);
        virtual const_type & bulk() const throw() = 0;
    };
}


#endif
