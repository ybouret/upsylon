//! \file

#ifndef Y_ASSOCIATIVE_SUFFIX_ADDRESS_INCLUDED
#define Y_ASSOCIATIVE_SUFFIX_ADDRESS_INCLUDED

#include "y/memory/buffer.hpp"

namespace upsylon
{
    //__________________________________________________________________________
    //
    //
    //! read-only buffer
    //
    //__________________________________________________________________________
    class suffix_address : public memory::ro_buffer
    {
    public:
        //______________________________________________________________________
        //
        // helper
        //______________________________________________________________________
        static const void *be(const void *) throw(); //!< big endian address

        //______________________________________________________________________
        //
        // C++
        //______________________________________________________________________
        virtual ~suffix_address() throw();                   //!< cleanup
        suffix_address(const suffix_address &) throw();      //!< copy
        template <typename T> inline
        suffix_address(T &obj) throw() : data( be(&obj) ) {} //!< setup

        //______________________________________________________________________
        //
        // buffer interface
        //______________________________________________________________________
        virtual size_t      length() const throw();
        virtual const void *ro()     const throw();

        //______________________________________________________________________
        //
        // methods
        //______________________________________________________________________
        const void         *addr()   const throw(); //!< retrieve original address

        //! cast to user's defined type, unsafe
        template <typename T> inline
        const T & to() const throw()
        {
            return *static_cast<const T *>( addr() );
        }


    private:
        Y_DISABLE_ASSIGN(suffix_address);
        const void * const data;

    };
}

#endif
