
#include "y/information/translator/queue.hpp"

namespace upsylon {

    namespace Information {

        TranslatorQueue:: ~TranslatorQueue() throw()
        {
        }

        TranslatorQueue:: TranslatorQueue() throw() : list<char>() {}

        TranslatorQueue:: TranslatorQueue(const size_t n) throw() : list<char>(n,as_capacity) {}

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

