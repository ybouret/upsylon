
#include "y/information/translator/huffman/encoder.hpp"

namespace upsylon {

    namespace information {

        namespace Huffman {


            const char * Encoder::family() const throw() { return FMID;    }
            const char * Encoder::name()   const throw() { return ENCODER; }

            Encoder:: Encoder() : Tree(), TranslatorQueue(8), Q(32)
            {
            }

            Encoder::~Encoder() throw()
            {
            }

            void Encoder:: write(char C)
            {
                inputByte(C,&Q);
                Q.compile(*this);
            }

            void Encoder:: flush()
            {
                eos->emit(Q);
                Q.zfinish(*this);
            }

            void Encoder:: reset() throw()
            {
                setupTree();
                free();
                Q.free();
            }

        }

    }
}
