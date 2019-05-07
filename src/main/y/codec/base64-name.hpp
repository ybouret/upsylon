//! \file
#ifndef Y_CODEC_BASE64_NAME_INCLUDED
#define Y_CODEC_BASE64_NAME_INCLUDED

#include "y/type/ints.hpp"
#include "y/codec/base64-defs.hpp"
#include "y/code/round.hpp"
#include <iostream>
#include <cstring>

namespace upsylon
{

    namespace core
    {
        //! formatting name
        struct base64_name_ops
        {
            //! convert qword into precomputed field of chars
            static void format(char        *field,
                               const size_t bytes,
                               uint64_t     qword) throw();
        };
    }

    //! get a short string for a value
    template <typename T>
    class base64_name
    {
    public:
        static  const size_t req_bytes = 1+Y_BASE64_BYTES_FOR(sizeof(T)); //!< bytes to hold converted type
        static  const size_t rnd_bytes = Y_ROUND4(req_bytes);             //!< memory alignment

        //! setup
        inline base64_name( const T &x ) throw() : content()
        {
            const union {
                T                                      source;
                typename unsigned_int<sizeof(T)>::type target;
            } converted = { x };
            core::base64_name_ops::format( (char *)content, rnd_bytes, converted.target );
        }

        //! hard copy
        inline base64_name(const base64_name &other) throw() : content()
        {
            memcpy((char*)content,other.content,rnd_bytes);
        }

        //! cleanup
        inline ~base64_name() throw() { memset((char*)content,0,rnd_bytes); }

        //! content operator
        const char * operator*() const throw() { return content; }

        //! display
        inline friend std::ostream & operator<<( std::ostream &os, const base64_name &n )
        {
            return (os<< n.content);
        }

    private:
        Y_DISABLE_ASSIGN(base64_name);
        const char content[rnd_bytes];
    };

}


#endif

