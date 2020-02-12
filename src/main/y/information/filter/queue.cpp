#include "y/information/filter/queue.hpp"

namespace upsylon {

    namespace information {
        
        q_filter:: ~q_filter() throw()
        {
        }

        q_filter:: q_filter() throw() :
        filter(),
        list<char>()
        {
        }

        bool q_filter:: query(char &C)
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

        bool q_filter:: is_active() throw()
        {
            return size()>0;
        }

        void q_filter:: store(char C)
        {
            push_front(C);
        }
    }

}

