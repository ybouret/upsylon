
#include "y/ios/ovstream.hpp"
#include "y/exception.hpp"

namespace upsylon
{
    namespace ios
    {
        ovstream:: ovstream() throw() : ovstream_(), ios::ostream()
        {
        }

        ovstream:: ~ovstream() throw() {}

        ovstream:: ovstream(const size_t n, const bool filled) :
        ovstream_(n,as_capacity), ios::ostream()
        {
            if(filled)
            {
                assert(capacity()>=n);
                while(size()<n) push_back_(0);
            }
        }

        ovstream & ovstream:: set_fast(const size_t n)
        {
            adjust(n,0);
            return *this;
        }

        ovstream & ovstream:: set_zero(const size_t n)
        {
            make(n,0);
            return *this;
        }

        void ovstream:: write( char C )
        {
            push_back(C);
        }

        void ovstream:: flush()
        {

        }

    }
}

