
#include "y/information/translator/huffman/encoder.hpp"

namespace upsylon {

    namespace information {

        namespace Huffman {


            const char * Encoder::family() const throw() { return FMID;      }
            const char * Encoder::name()   const throw() { return EncoderID; }

            Encoder:: Encoder() : Tree(), TranslatorQueue(), Q()
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

            void Encoder:: writeBits(qbits &io, const char C)
            {
                inputByte(C,&io);
            }

            void Encoder:: flushBits(qbits &io)
            {
                eos->emit(io);
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
