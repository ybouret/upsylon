
#include "y/information/qbytes.hpp"

namespace upsylon {

    namespace information {

        qbytes:: ~qbytes() throw()
        {
        }

        qbytes:: qbytes() throw() :
        list<char>(),
        ios::istream()
        {
        }

        bool qbytes:: query(char &C)
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

        bool qbytes:: is_active() throw()
        {
            return size()>0;
        }

        void qbytes:: store(char C)
        {
            push_front(C);
        }
    }

}

