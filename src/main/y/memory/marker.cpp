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
    }
}

#include "y/code/round.hpp"

namespace upsylon {

    namespace memory {

        marker:: marker(const marker &origin, const size_t delta ) throw() :
        offset( align( origin.offset+origin.length) ),
        length( delta )
        {
        }

    }

}
