//! \file
#ifndef Y_INK_OPS_CRYPT_INCLUDED
#define Y_INK_OPS_CRYPT_INCLUDED

#include "y/ink/bitmap.hpp"
#include "y/crypto/bc/operating.hpp"
#include "y/crypto/sc/kstream.hpp"

namespace upsylon
{
    namespace Ink
    {
        //! crypt functions
        struct Crypt
        {
            //! run a stream cipher
            static void Run( Bitmap &tgt, const Bitmap &src, crypto::kstream   &ks ) throw();

            //! run an initialized operating cipher with block ciphers
            static void Run( Bitmap &tgt, const Bitmap &src, crypto::operating &op, crypto::ciphers &c );
        };

    }
}

#endif

