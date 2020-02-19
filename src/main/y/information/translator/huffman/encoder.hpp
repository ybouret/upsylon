
//! \file

#ifndef Y_TRANSLATOR_HUFFMAN_ENCODER_INCLUDED
#define Y_TRANSLATOR_HUFFMAN_ENCODER_INCLUDED 1

#include "y/information/translator/huffman.hpp"
#include "y/information/translator/queue.hpp"

namespace upsylon {

    namespace information {

        namespace Huffman {

            class Encoder : public Tree, public TranslatorQueue
            {
            public:
                explicit Encoder();
                virtual ~Encoder() throw();
                
            private:
                Y_DISABLE_COPY_AND_ASSIGN(Encoder);
            };

        }

    }
}

#endif

