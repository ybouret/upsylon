
#include "y/information/translator/huffman/encoder.hpp"

namespace upsylon {

    namespace information {

        namespace Huffman {

            const char Encoder::CLID[] = "HuffmanEncoder";

            const char * Encoder::name() const throw() { return CLID; }

            Encoder:: Encoder() : Tree(), TranslatorQueue(8), Q(32)
            {
            }

            Encoder::~Encoder() throw()
            {
            }

            void Encoder:: write(char C)
            {
                update(C,&Q);
                Q.compile(*this);
            }

            void Encoder:: flush()
            {
                eos->emit(Q);
                Q.zfinish(*this);
            }

            void Encoder:: reset() throw()
            {
                restart();
                free();
                Q.free();
            }

        }

    }
}
