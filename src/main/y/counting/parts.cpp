#include "y/counting/parts.hpp"
#include "y/exceptions.hpp"
#include "y/type/ints-display.hpp"
#include "y/type/aliasing.hpp"

namespace upsylon {

    integer_partitions:: integer_partitions(const size_t n) :
    counting( integer_partition::outcomes(n),0),
    part(n)
    {
        aliasing::_(index) = 1;
    }

    integer_partitions:: integer_partitions(const integer_partitions &other) :
    counting( other ),
    part( other.part )
    {
        assert(count==other.count);
        assert(index==other.index);
        
    }

    integer_partitions:: ~integer_partitions() throw()
    {
    }

    void integer_partitions:: onBoot() throw()
    {
        assert(1==index);
        part.initialize();
    }

    void integer_partitions:: onNext()
    {
        assert(index<=count);
        if(!part.build_next()) throw libc::exception(EINVAL,"corrupted integer_partitions");
    }

    std::ostream & integer_partitions:: show(std::ostream &os) const
    {
        const size_t *curr = &part[1];
        return display_int::to(os<< '{',curr,part.size(),",") << '}';
    }

    const accessible<size_t> & integer_partitions:: operator*() const throw()
    {
        return part;
    }


}

