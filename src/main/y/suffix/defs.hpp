
//! \file

#ifndef Y_SUFFIX_DEFS_INCLUDED
#define Y_SUFFIX_DEFS_INCLUDED 1

#include "y/os/endian.hpp"

namespace upsylon
{

    struct suffix
    {
    public:

        static  const size_t flag_size = sizeof(void*);
        typedef unsigned_int<flag_size>::type flag_t;
        static  void *                        in_use() throw();

        //! byte to printable text
        static const char *byte_to_text(const uint8_t &) throw();

        //! integral type to printable text: no thread-safe!
        template <typename T> static inline const char *code_to_text(const T &x) throw()
        {
            const union {
                T       _;
                uint8_t b[ sizeof(T) ];
            } alias = { swap_be_as(x) };
            clr();
            cat(alias.b,sizeof(T));
            return text;
        }

    private:
        static char text[];
        static void clr() throw();
        static void cat(const char *) throw();
        static void cat(const uint8_t b[], const size_t n) throw();

    };


}

#endif

