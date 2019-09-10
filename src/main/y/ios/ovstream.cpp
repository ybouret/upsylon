
#include "y/ios/ovstream.hpp"
#include "y/exception.hpp"
#include <cstring>

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

        void ovstream:: copy(const array<uint8_t> &other)
        {
            array<uint8_t> &self = *this;
            if( &self != &other )
            {
                const size_t n = other.size();
                set_fast(n);
                memcpy( *self, *other, n );
            }
        }

    }
}

