

#include "y/lang/stream/processor.hpp"
#include "y/lang/pattern/compiler.hpp"
#include "y/exception.hpp"

namespace upsylon {

    namespace  Lang {

        namespace Stream {

            Processor:: Processor() : codes()
            {
            }

            Processor:: ~Processor() throw()
            {
            }

            void Processor::on(const string &rx, const Instruction &I)
            {
                const Motif motif = RegExp(rx);
                if(motif->weak()) throw exception("weak pattern \"%s\" in Stream::Processor", *rx );
                const Code  code  = new Code_(motif,I);
                codes << code;
            }


        }

    }
}

