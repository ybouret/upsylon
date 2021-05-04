//! \file

#ifndef Y_NATIVE_KEY_INCLUDED
#define Y_NATIVE_KEY_INCLUDED 1

#include "y/memory/buffer.hpp"
#include "y/type/traits.hpp"
#include "y/os/endian.hpp"

namespace upsylon
{
    //__________________________________________________________________________
    //
    //
    //! native key wrapper, suitable for suffix indexation
    //
    //__________________________________________________________________________
    template <typename T>
    class native_key : public memory::ro_buffer
    {
    public:
        //______________________________________________________________________
        //
        // types and definition
        //______________________________________________________________________
        Y_DECL_ARGS(T,type); //!< aliases

        //______________________________________________________________________
        //
        // C++
        //______________________________________________________________________
        inline virtual ~native_key() throw() { data=0; }                               //!< cleanup
        inline native_key(param_type value)    throw() : data( swap_be_as(value) )  {} //!< setup
        inline native_key(const native_key &k) throw() : data(k.data) {}               //!< copy

        //! copy
        inline native_key & operator=(const native_key &other) throw()
        {
            data = other.data;
            return *this;
        }

        //! assign value
        inline native_key & operator=(param_type value) throw()
        {
            data = swap_be_as(value);
            return *this;
        }


        //______________________________________________________________________
        //
        // buffer interface
        //______________________________________________________________________
        inline virtual const void *ro(    ) const throw() { return &data;     }
        inline virtual size_t      length() const throw() { return sizeof(T); }

        //______________________________________________________________________
        //
        // methods
        //______________________________________________________________________
        
        //! return original value
        inline mutable_type operator*() const throw() { return swap_be_as(data); }

    private:
        mutable_type data;
    };

}

#endif

