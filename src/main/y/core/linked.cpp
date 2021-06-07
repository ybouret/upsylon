#include "y/core/linked.hpp"
#include "y/type/aliasing.hpp"

namespace upsylon {

    namespace core {

        linked:: ~linked() throw()
        {
            force_no_size();
        }

        linked:: linked() throw() : size(0)
        {
        }

        void linked:: force_no_size() throw()
        {
            aliasing::_(size) = 0;
        }

        void linked:: increase_size() throw()
        {
            aliasing::incr(size);
        }

        void linked:: increase_size(const size_t n) throw()
        {
            assert(n>0);
            aliasing::_(size) += n;
        }

        void linked:: decrease_size() throw()
        {
            assert(size>0);
            aliasing::decr(size);
        }

        bool linked:: is_vacant() const throw() { return size<=0; }
        
        bool linked:: has_nodes() const throw() { return size>0;  }

    }

}
