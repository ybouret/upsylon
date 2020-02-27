//! \file

#ifndef Y_INFORMATION_BASE64_INCLUDED
#define Y_INFORMATION_BASE64_INCLUDED 1

#include "y/string.hpp"

namespace upsylon {

    namespace Information {

#define Y_BASE64_NUM3(LENGTH)   ( (LENGTH) / 3 )
#define Y_BASE64_REM3(LENGTH)   ( (LENGTH) % 3 )
#define Y_BASE64_XTRA_RAW(REM)  ( (REM) > 0 ? (REM)+1 : 0 )
#define Y_BASE64_XTRA_PAD(REM)  ( (REM) > 0 ? 4       : 0 )

#define Y_BASE64_RAW_BYTES_FOR(LENGTH) ( (4 * Y_BASE64_NUM3(LENGTH)) + Y_BASE64_XTRA_RAW( Y_BASE64_REM3(LENGTH) ) )
#define Y_BASE64_PAD_BYTES_FOR(LENGTH) ( (4 * Y_BASE64_NUM3(LENGTH)) + Y_BASE64_XTRA_PAD( Y_BASE64_REM3(LENGTH) ) )

        struct Base64
        {
            enum Mode
            {
                Raw,
                Pad
            };
            static const char    padding = '=';
            static const char    std_table[64];
            static const char    url_table[64];
            static const uint8_t inv_table[256];

            static size_t BytesFor(const size_t inputLength, const Mode m) throw();

            static void Encode3to4(void *target, const void *source, const char *table) throw();
            static void Decode4to3(void *target, const void *source) throw();

            static void Encode2to3(void *target, const void *source, const char *table) throw();
            static void Decode3to2(void *target, const void *source)                    throw();
            
            static void Encode1to2(void *target, const void *source, const char *table) throw();
            static void Decode2to1(void *target, const void *source)                    throw();
            
            //! encode with target.size >= BytesFor(length,mode)
            static void Encode(void        *target,
                               const void  *source,
                               const size_t length,
                               const char  *table,
                               const Mode   mode) throw();
        };

    }

}

#endif
