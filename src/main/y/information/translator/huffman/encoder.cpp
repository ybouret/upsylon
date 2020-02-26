
#include "y/information/translator/huffman/encoder.hpp"

namespace upsylon {

    namespace information {

        namespace Huffman {


            const char * Encoder::family() const throw() { return FMID;      }
            const char * Encoder::name()   const throw() { return EncoderID; }

            Encoder:: Encoder() : Tree(), TranslatorEncodingQueue()
            {
            }

            Encoder::~Encoder() throw()
            {
            }

#if 0
            void Encoder:: write(char C)
            {
                inputByte(C,&Q);
                Q.compile(*this);
            }
#endif

            void Encoder:: writeBits(qbits &io, const char C)
            {
                inputByte(C,&io);
            }

            void Encoder:: flushBits(qbits &io)
            {
                eos->emit(io);
            }

#if 0
            void Encoder:: flush()
            {
                eos->emit(Q);
                Q.zfinish(*this);
            }
#endif
            void Encoder:: reset() throw()
            {
                setupTree();
                free();
                Q.free();
            }

        }

    }
}
