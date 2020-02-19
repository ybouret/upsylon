
#include "y/information/translator/huffman/encoder.hpp"

namespace upsylon {

    namespace information {

        namespace Huffman {

            Encoder:: Encoder() : Tree(), TranslatorQueue(8), io(32)
            {
            }

            Encoder::~Encoder() throw()
            {
            }

            void Encoder:: write(char C)
            {
                update(C,&io);
                io.compile(*this);
            }

            void Encoder:: flush()
            {
                eos->emit(io);
                io.zfinish(*this);
            }

            void Encoder:: reset() throw()
            {
                restart();
                free();
                io.free();
            }

        }

    }
}
