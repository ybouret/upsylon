
//! \file
#ifndef Y_INFORMATION_ALPHABET_INCLUDED
#define Y_INFORMATION_ALPHANET_INCLUDED 1

#include "y/information/qbits.hpp"

namespace upsylon {

    namespace information {

        class Alphabet
        {
        public:
            typedef size_t        CodeType;
            static const size_t   Chars = 256;
            static const CodeType NYT   = Chars; //!< Not Yet Transmitter
            static const CodeType EOS   = NYT+1; //!< End Of Stream
            static const size_t   Codes = EOS+1; //!< maximum possible codes
            
        };


    }
}

#endif

