
#include "y/information/translator/huffman/encoder.hpp"

namespace upsylon {

    namespace Information {

        namespace Huffman {

            EncodingTree:: EncodingTree() : EncodingWith<Tree>()
            {

            }

            EncodingTree:: ~EncodingTree() throw()
            {}

            const char EncodingTree:: CLID[] = "Huffman-Encoder";

            const char *EncodingTree:: label() const throw()
            {
                return CLID;
            }

            const char * Encoder::family() const throw() { return FMID;      }
            const char * Encoder::name()   const throw() { return EncoderID; }

            Encoder:: Encoder() : EncodingTree(), TranslatorQueue(), Q()
            {
            }

            Encoder::~Encoder() throw()
            {
            }

            void Encoder:: write(char C)
            {
                writeBits(Q,C);
                Q.compile(*this);
            }



            void Encoder:: flush()
            {
                flushBits(Q);
                Q.compile(*this);
                assert(0==Q.size());
            }

            void Encoder:: reset() throw()
            {
                startBits();
                free();
                Q.free();
            }

        }

    }
}
