//! \file

#ifndef Y_SEQUENCE_SHARED_INCLUDED
#define Y_SEQUENCE_SHARED_INCLUDED 1

#include "y/ptr/counted.hpp"

namespace upsylon {

    template <typename SEQUENCE>
    class shared : public counted_object, public SEQUENCE
    {
    public:
        typedef typename SEQUENCE::type         type;
        typedef typename SEQUENCE::mutable_type mutable_type;
        typedef typename SEQUENCE::const_type   const_type;


        inline virtual ~shared() throw() {}
        inline          shared() : SEQUENCE() {}
        inline          shared(const size_t n, const as_capacity_t &_) : SEQUENCE(n,_) {}
        inline          shared(const shared &other) : SEQUENCE(other) {}

    private:
        Y_DISABLE_ASSIGN(shared);

    };

}

#endif
