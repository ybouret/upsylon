//! \file

#ifndef Y_INFORMATION_BASE64_INCLUDED
#define Y_INFORMATION_BASE64_INCLUDED 1

#include "y/string.hpp"

namespace upsylon {

    namespace Information {
        //! number of 3 chars block
#define Y_BASE64_NUM3(LENGTH)   ( (LENGTH) / 3 )

        //!  remaining chars
#define Y_BASE64_REM3(LENGTH)   ( (LENGTH) % 3 )

        //! extra chars for Raw mode
#define Y_BASE64_XTRA_RAW(REM)  ( (REM) > 0 ? (REM)+1 : 0 )

        //! extra chars for Pad mode
#define Y_BASE64_XTRA_PAD(REM)  ( (REM) > 0 ? 4       : 0 )

        //! bytes to encode LENGTH, Raw mode
#define Y_BASE64_RAW_BYTES_FOR(LENGTH) ( (4 * Y_BASE64_NUM3(LENGTH)) + Y_BASE64_XTRA_RAW( Y_BASE64_REM3(LENGTH) ) )

        //! bytes to encode LENGTH, Pad mode
#define Y_BASE64_PAD_BYTES_FOR(LENGTH) ( (4 * Y_BASE64_NUM3(LENGTH)) + Y_BASE64_XTRA_PAD( Y_BASE64_REM3(LENGTH) ) )

        //! Base64 I/O
        struct Base64 {

            //! fill mode
            enum Mode
            {
                Raw, //!< no padding
                Pad  //!< with padding
            };
            static const char    padding = '=';  //!< padding character
            static const char    std_table[64];  //!< standard encoding table
            static const char    url_table[64];  //!< url encoding table
            static const uint8_t inv_table[256]; //!< inverse table
            
            static size_t BytesFor(const size_t inputLength, const Mode m) throw();                //!< number of bytes to encode inputLength
            static void   Encode3to4(void *target, const void *source, const char *table) throw(); //!< 'ABC'  -> 'WXYZ'
            static void   Decode4to3(void *target, const void *source) throw();                    //!< 'WXYZ' -> 'ABC'
            static void   Encode2to3(void *target, const void *source, const char *table) throw(); //!< 'AB'   -> 'WXy' ['=']
            static void   Decode3to2(void *target, const void *source)                    throw(); //!< 'WXy'  -> 'AB'
            static void   Encode1to2(void *target, const void *source, const char *table) throw(); //!< 'A'    -> 'Wx' ['==']
            static void   Decode2to1(void *target, const void *source)                    throw(); //!< 'Wx'   -> 'A'

            //! encode block with target size >= BytesFor(length,mode)
            static void Encode(void        *target,
                               const void  *source,
                               const size_t length,
                               const char  *table,
                               const Mode   mode) throw();
        };

    }

}

#endif
