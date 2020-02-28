//! \file

#ifndef Y_INFO_UUID64_INCLUDED
#define Y_INFO_UUID64_INCLUDED 1

#include "y/type/args.hpp"
#include "y/memory/buffer.hpp"
#include "y/code/round.hpp"
#include <cstring>
#include <iostream>

namespace upsylon {

    namespace core {

        //! operations for uuid64
        struct uuid64
        {
            //! internal call to Base64 ops
            static void encode(char         *target,
                               const size_t  targetLength,
                               const void   *source,
                               const size_t  sourceLength) throw();
        };
    }

    //! convert an integral type to a human-readable value
    template <typename T>
    class uuid64 : public memory::ro_buffer
    {
    public:
        Y_DECL_ARGS(T,type); //!< aliases
        static const size_t type_size  = sizeof(type);                           //!< sizeof
        static const size_t type_num3  = type_size/3;                            //!< for base64
        static const size_t type_rem3  = type_size%3;                            //!< for base64
        static const size_t type_xtra  = (type_rem3>0) ? (type_rem3+1) : 0;      //!< for base64
        static const size_t uuid_bytes = 4 * type_num3 + type_xtra;              //!< base64 bytes
        static const size_t uuid_chars = uuid_bytes+1;                           //!< internal chars
        static const size_t uuid_size  = Y_ALIGN_FOR_ITEM(ptrdiff_t,uuid_chars); //!< memory to hold chars

        //! setup
        inline uuid64( param_type args ) throw() : memory::ro_buffer(), uuid()
        {
            memset(uuid,0xff,sizeof(uuid));
            core::uuid64::encode(uuid,uuid_bytes,&args,type_size);
        }

        //! cleanup
        inline virtual ~uuid64() throw() { memset(uuid,0,sizeof(uuid)); }

        //! copy
        inline uuid64( const uuid64 &other ) throw() : memory::ro_buffer(), uuid()
        {
            memcpy( uuid, other.uuid, sizeof(uuid) );
        }

        //! buffer interface : read only access
        inline virtual const void *ro()     const throw() { return uuid; }

        //! buffer interface : number of chars
        inline virtual size_t      length() const throw() { return uuid_chars; }

        //! get content
        inline const char * operator*() const throw() { return uuid; }

        inline friend std::ostream & operator <<( std::ostream &os, const uuid64 &U )
        {
            for(size_t i=0;i<uuid_chars;++i) os << U.uuid[i];
            return os;
        }

    private:
        char uuid[ uuid_size ];
        Y_DISABLE_ASSIGN(uuid64);
    };

}

#endif

