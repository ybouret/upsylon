

#include "y/information/translator/shannon-fano/encoder.hpp"

namespace upsylon {

    namespace Information {

        namespace ShannonFano {

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
