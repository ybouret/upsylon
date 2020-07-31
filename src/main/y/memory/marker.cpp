#include "y/memory/marker.hpp"

namespace upsylon {

    namespace memory {

        marker:: marker() throw() : offset(0), length(0) {}

        marker:: ~marker() throw() { offset=length=0; }

        marker:: marker(const marker &_) throw() : offset(_.offset), length(_.length) {}

        marker & marker:: operator=(const marker &_) throw()
        {
            offset = _.offset;
            length = _.length;
            return *this;
        }

        marker:: marker(const size_t _off, const size_t _len) throw() : offset(_off), length(_len) {}

        marker:: marker(const size_t _len) throw() : offset(0), length(_len) {}

        void * marker:: shift(void *addr) const throw()
        {
            return static_cast<char*>(addr)+offset;
        }


        const void * marker:: shift(const void *addr) const throw()
        {
            return static_cast<const char*>(addr)+offset;
        }

    }
}

#include "y/code/round.hpp"

namespace upsylon {

    namespace memory {

        size_t marker:: next_offset() const throw()
        {
            return align(offset+length);
        }

        marker:: marker(const marker &origin, const size_t delta ) throw() :
        offset( origin.next_offset() ),
        length( delta )
        {
        }

    }

}

#include <iostream>

namespace upsylon {

    namespace memory {

        std::ostream & operator<<(std::ostream &os, const marker &m)
        {
            os << "[@" << m.offset << "+" << m.length << "]";
            return os;
        }
    }
}

