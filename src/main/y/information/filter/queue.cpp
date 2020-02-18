#include "y/information/filter/queue.hpp"

namespace upsylon {

    namespace information {
        
        filterQ:: ~filterQ() throw()
        {
        }

        filterQ:: filterQ() throw() :
        filter(),
        list<char>()
        {
        }

        filterQ:: filterQ(const size_t n) :
        filter(),
        list<char>(n,as_capacity)
        {
        }

        bool filterQ:: query(char &C)
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

        bool filterQ:: is_active() throw()
        {
            return size()>0;
        }

        void filterQ:: store(char C)
        {
            push_front(C);
        }
    }

}

