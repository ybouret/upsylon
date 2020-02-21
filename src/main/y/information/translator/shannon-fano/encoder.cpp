

#include "y/information/translator/shannon-fano/encoder.hpp"

namespace upsylon {

    namespace information {

        namespace ShannonFano {

            const char Encoder::CLID[] = "ShannonFanoEncoder";

            const char * Encoder:: name() const throw() { return CLID; }

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
