//! \file

#ifndef Y_SUFFIX_KEY_TO_PATH_INCLUDED
#define Y_SUFFIX_KEY_TO_PATH_INCLUDED 1

#include "y/memory/buffer.hpp"
#include "y/type/args.hpp"
#include "y/type/class-conversion.hpp"

namespace upsylon
{
    //______________________________________________________________________
    //
    //
    //! helper to convert integral types or ro_buffers to paths
    //
    //______________________________________________________________________
    template <typename KEY>
    struct key_to_path
    {
    public:
        Y_DECL_ARGS(KEY,key_type);             //!< aliases

        //! detect kind of key
        static const bool ro_buffer_key = Y_IS_SUPERSUBCLASS(memory::ro_buffer,mutable_key_type);

        //! key -> const char *
        static inline const char *addr_of(const_key_type &key) throw()
        {
            static const int2type<ro_buffer_key> kind = {};
            return __addr_of(key,kind);
        }

        //! key -> number of  bytes
        static inline size_t     size_of(const_key_type &key) throw()
        {
            static const int2type<ro_buffer_key> kind = {};
            return __size_of(key,kind);
        }

    private:
        static inline const char * __addr_of( const_key_type &key, const int2type<false> & )  throw()
        {
            return (const char *)&key;
        }

        static inline size_t  __size_of( const_key_type &, const int2type<false> &)  throw()
        {
            return sizeof(KEY);
        }

        static inline const char * __addr_of( const_key_type &key, const int2type<true> & )  throw()
        {
            return static_cast<const char*>(key.ro());
        }

        static inline size_t  __size_of( const_key_type &key, const int2type<true> & )  throw()
        {
            return key.length();
        }
    };
}


#endif
