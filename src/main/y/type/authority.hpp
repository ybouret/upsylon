//! \file

#ifndef Y_TYPE_AUTHORITY_INCLUDED
#define Y_TYPE_AUTHORITY_INCLUDED 1

#include "y/type/args.hpp"

namespace upsylon
{

    //__________________________________________________________________________
    //
    //
    //! simple reference wrapper to build classes with persistent references
    //
    //__________________________________________________________________________
    template <typename T>
    class authority
    {
    public:
        Y_DECL_ARGS(T,type); //!< aliases


        inline explicit authority(type &persistent) throw() : host(persistent) {}       //!< setup
        inline virtual ~authority()                 throw() {}                          //!< cleanup
        inline          authority(const authority &other) throw() : host(other.host) {} //!< copy

        inline type       & operator*()       throw()  { return  host; } //!< access
        inline const_type & operator*() const throw()  { return  host; } //!< access
        

    private:
        type &host;
        Y_DISABLE_ASSIGN(authority);
    };
}

#endif
