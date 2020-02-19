
#include "y/information/translator/queue.hpp"

namespace upsylon {

    namespace information {

        TranslatorQueue:: ~TranslatorQueue() throw()
        {
        }

        TranslatorQueue:: TranslatorQueue() throw() : list<char>() {}

        bool TranslatorQueue:: query(char &C)
        {
            if( size() > 0 )
            {
                C = front();
                pop_front();
                return true;
            }
            else
            {
                return false;
            }
        }

        void TranslatorQueue:: store(char C)
        {
            push_front(C);
        }

        bool TranslatorQueue::is_active() throw()
        {
            return size() > 0;
        }
    }

}

