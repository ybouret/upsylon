
#include "y/information/translator/encoding-queue.hpp"

namespace upsylon {

    namespace information {

        TranslatorEncodingQueue:: ~TranslatorEncodingQueue() throw()
        {
        }


        TranslatorEncodingQueue:: TranslatorEncodingQueue() throw() :
        TranslatorQueue(), Q()
        {
        }

        void TranslatorEncodingQueue:: write(char C)
        {
            writeBits(Q,C);
            Q.compile(*this);
        }

        void TranslatorEncodingQueue:: flush()
        {
            flushBits(Q);
            Q.zfinish(*this);
        }

    }

}


